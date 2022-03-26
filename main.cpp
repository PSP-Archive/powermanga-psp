//******************************************************************************
// Copyright (C) 1998-2005 TLK Games all rights reserved.
//------------------------------------------------------------------------------
// file		: "main.cpp"
// created	: 1999-08-17
// updates	: 2005-01-26
// id		: $Id: main.cpp,v 1.6 2005/01/31 09:44:33 patriceduhamel Exp $
//------------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA 02111-1307, USA.
//******************************************************************************

// for ShellExecute, to launch html help
#ifdef _WIN32
#include <windows.h>
#include "shellapi.h"
#endif

#include <pspsdk.h>
#include <pspkernel.h>

#include "powermanga.hpp"
#include <errno.h>
#include "configfile.hpp"

/* Define the module info section */
PSP_MODULE_INFO("powermanga_psp", 0, 1, 1);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);


//..............................................................................
extern const char	*nomfichierscore[] = { SCOREFILE "-easy", SCOREFILE, SCOREFILE "-hard" };
extern const char	nomprefix[] = PREFIX;
extern Uint32		iCompteurBoucle;
extern Sint32		vmode;// 0=320*200 ; 1=640*400
extern Sint32		vmode2;// 0=320*200 ; 1=640*400
//..............................................................................
Sint32			quit_game = 0;// 1=leave the game
#ifdef SDL_TLK
Uint32			iFrame = 14;// game speed : 70 frames/sec (1000 <=> 1 seconde ; 1000 / 70 =~ 14)
Uint32			iFrame2 = 20;// music speed : 50 frames/sec
Uint32			iFrame3 = 35;// game speed : 28 frames/sec
#else
Uint32			iFrame = 14286;// game speed : 70 frames/sec (1000000 <=> 1 seconde ; 1000000 / 70 =~ 14286)
Uint32			iFrame2 = 20000;// music speed : 50 frames/sec
Uint32			iFrame3 = 35715;// game speed : 28 frames/sec
#endif
extern Uint32		iJoueIntro;// (1=animation du debut/start anim ; 2=animation de fin/end anim )
_etbfonte1*		pEtbfonte1;// object handle 8x8 chars (about CTRL + A)
configfile*		power_conf;
//..............................................................................
extern Sint32		Load_Scroll_Text();
extern void		Free_Scroll_Text();
extern Sint32		LoadGameText();
void			scan_arguments(Sint32 _iNombreArgument, char **_pArguments);
void			_destroy_congra();
Sint32			powermanga();
void			zeMainLoop();

//..............................................................................

#ifdef _GP2X
/**
 * Returns to the standard GP2X menu.
 */
void returnToMenu(void)
{
    // This is how to quit back to the menu - calling exit() will just cause the
    // GP2X to "hang". execl() will replace the current process image with that
    // of the menu program, and then execute it
    //chdir("/usr/gp2x");
    //execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);

	sceKernelExitGame();
}
#endif

//------------------------------------------------------------------------------
// main : a very simple main function
// input => _iNombreArgument : taille de la chaine d'argument / size of string
//=> _pArguments: pointeur sur le pointeur contenat la dite chaine
//pointer to the pointer to the arguments
//------------------------------------------------------------------------------
int main(Sint32 _iNombreArgument, char **_pArguments)
{
#ifdef _GP2X
    // Use atexit() to call the return-to-menu function, in case of crashes, etc.
    atexit(returnToMenu);
#endif

	// allocate memory table
	if(!mem_initialise(22000))
		exit(0);

	// load config file
	power_conf = new configfile();
	power_conf->loadconfig();
	if(power_conf->scanZeArgs(_iNombreArgument, _pArguments))
	{
#ifdef _GP2X
        // We require a 320x200 output size to fit on the GP2X's display
        power_conf->resolution = 320;
        power_conf->fullscreen = 1;
#endif
        if(power_conf->resolution == 320)
		{
			vmode = 0;
			vmode2 = 0;
		}
		else
		{	vmode = 1;
#ifdef WIN32
			vmode2 = 1;
#else
			vmode2 = 0;
#endif
		}
		powermanga();
	}

	//free video ressources (xorg-x11 or SDL)
	xw_kill();
	_score::destroy();
	if(pEtbfonte1)
	{	delete pEtbfonte1;
		pEtbfonte1 = NULL;
	}
	_destroy_congra();
	_score::freeTextAbout();
	_score::freeTextOrder();
	Free_Scroll_Text();
	power_conf->saveconfig();
	delete power_conf;

	//free all memory
	mem_libereTous();


	// launch html donation page before leaving
#ifdef _WIN32
	ShellExecute(0, "open", ".\\html\\ar01s06s02.html", 0, 0, SW_SHOWNORMAL);
#endif

	exit(0);
}


//------------------------------------------------------------------------------
// initialize and run!
//------------------------------------------------------------------------------
Sint32 powermanga()
{
	if(power_conf->verbose)
		power_conf->configinfo();

#ifdef _VERBEUX_
	if(power_conf->verbose)
	{	fprintf(stdout, "*********************************************"
			"************************************\n"
			"> main.cpp/powermanga() : " POWERMANGA_VERSION "\n");
		type_routine_gfx();
	}
#endif
	if(!LoadGameText()) return 0;
	if(!_score::loadTextAbout()) return 0;
	if(!Load_Scroll_Text()) return 0;
#ifdef USE_SDLMIXER
	if(!seal_init()) return 0;
#endif
	pEtbfonte1 = new _etbfonte1();
	if(!pEtbfonte1) return 0;
	if(!xw_init()) return 0;
	if(!inits_game()) return 0;
	fps_init();
	zeMainLoop();
	fps_affiche();
#ifdef _VERBEUX_
	if(power_conf->verbose > 0)
		fprintf(stdout, "> main.cpp/powermanga(): The End\n");
#endif
	return 0;
}

//------------------------------------------------------------------------------
// main loop of the game
//------------------------------------------------------------------------------
void zeMainLoop()
{
	Sint32 _iTempsPause = 0;
	Sint32 _iTempsDifference = 0;
	do
	{	iCompteurBoucle++; //counter of loops
		if(!power_conf->nosync)
		{	_iTempsDifference = synchro_CalculDifference();
			if(iJoueIntro)
				_iTempsPause = synchro_processusPause(iFrame3 -
						_iTempsDifference + _iTempsPause);
			else
				_iTempsPause = synchro_processusPause(iFrame -
						_iTempsDifference + _iTempsPause);
		}
		if(!UpdateFrame())
			quit_game = 1;	//"powermanga.cpp" : update all
#ifdef USE_SDLMIXER
        seal_exec();        //play music and sounds
#endif
		xw_exec();		//display window or screen
	} while (!quit_game);
}

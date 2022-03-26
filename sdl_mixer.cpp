//******************************************************************************
// copyright (c) 2003-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "seal.cpp"
// created	: 2003-06-27
// updates	: 2005-01-09
// id		: $Id: sdl_mixer.cpp,v 1.1.1.1 2005/01/25 05:55:02 gurumeditation Exp $
//-----------------------------------------------------------------------------
// description	: manage sound's effects and music's modules with sdl_mixer
//-----------------------------------------------------------------------------
// functions :  seal_init()
//              seal_initialise(int)
//              seal_chargeModule(int)
//              seal_exec()
//              seal_kill()
//              seal_restaure()
//              seal_joue(Uint32)
//-----------------------------------------------------------------------------
// liste des fonctions utilisant seal_joue(Uint32) :
// - New_Element_Tir()  "powermanga sub2.cpp"
// - Add_Ve_Special()   "powermanga sub1.cpp"
// - Add_Explosion()    "powermanga sub2.cpp"
// - bonus_execution()  "bonus.cpp"
// - testBoutonOption() "options.cpp"
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTAgBILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA  02111-1307, USA.
//******************************************************************************
//..............................................................................
#include "powermanga.hpp"
#ifdef USE_SDLMIXER
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mixer.h>
//..............................................................................
extern short            touch[];                               //tableau des flags des touches du clavier.
extern int              quit_game;                             //true=demande a quitter le jeu
extern bool             aff_game_over;                         //true=affichage du game over en cours
extern bool             player_pause;                          //true=pause en cours
extern int              etat_menu;                             //MENU_OFF(false)=menu inactif ; MENU_ON(true)=menu actif
extern Uint32     seal_thread_run;                       //1=thread play music running

//constantes ...................................................................
const Uint32      iNOMBRESONS = 28;
const Uint32      iBRUITAGESVOIX = 4;                    //nombre de voix pour les bruitages (avec module)
const Uint32      iBRUITAGESVOIX2 = 8;                   //nombre de voix pour les bruitages (sans module)

//..............................................................................
Uint32            iBruitagesVoix;                        //nombre de voix pour les bruitages
Mix_Music              *lpModule;
Uint32            iFlagBruitages[iNOMBRESONS];
Uint32            iFlagMusique;                          //1=alors musique active
int                     iNumeroModule;                         //numero du module en cours (0 a n) -1=pas de module charge
int                     iNumeroModule2;                        //numero du module en cours (0 a n) -1=pas de module charge
Uint32            iChangeSonMusique;                     //1=on active/desactive musique
Uint32            iVolumeModule;
Uint32            iVolumeModule2;
Uint32            iTailleSons;                           //taille des sons
Uint32            nDeviceId = 0;

//noms des fichiers sons........................................................
char *pFichiersSons[iNOMBRESONS] =
{ "sounds/vchange1.wav",                                       //0
  "sounds/vchange2.wav",                                       //1
  "sounds/select.wav",                                         //2
  "sounds/noselect.wav",                                       //3
  "sounds/bonus4.wav",                                         //4
  "sounds/bonus3.wav",                                         //5
  "sounds/bonus1.wav",                                         //6
  "sounds/bonus2.wav",                                         //7
  "sounds/nmispec.wav",                                        //8
  "sounds/ondechoc.wav",                                       //9
  "sounds/tir2.wav",                                           //10
  "sounds/tir1.wav",                                           //11
  "sounds/xploc1.wav",                                         //12
  "sounds/xploc2.wav",                                         //13
  "sounds/xploc3.wav",                                         //14
  "sounds/xploc4.wav",                                         //15
  "sounds/xplob1.wav",                                         //16
  "sounds/xplob2.wav",                                         //17
  "sounds/xplob3.wav",                                         //18
  "sounds/xplob4.wav",                                         //19
  "sounds/xploa1.wav",                                         //20
  "sounds/xploa2.wav",                                         //21
  "sounds/xploa3.wav",                                         //22
  "sounds/xploa4.wav",                                         //23
  "sounds/tir1nmi.wav",                                        //24
  "sounds/tir2nmi.wav",                                        //25
  "sounds/tir3nmi.wav",                                        //26
  "sounds/tir4nmi.wav"                                         //27
};

//noms des modules utilises ....................................................
char *pFichiersModules[] =
{ "sounds/intro.ogg",                                          //0
  "sounds/game.ogg",                                           //1
  "sounds/end.ogg"                                             //2
};

//nom des erreurs possibles ....................................................
char *SealErrorText[14] =
{ "AUDIO_ERROR_NONE",                                          //0x0000
  "AUDIO_ERROR_INVALHANDLE",                                   //0x0001
  "AUDIO_ERROR_INVALPARAM",                                    //0x0002
  "AUDIO_ERROR_NOTSUPPORTED",                                  //0x0003
  "AUDIO_ERROR_BADDEVICEID",                                   //0x0004
  "AUDIO_ERROR_NODEVICE",                                      //0x0005
  "AUDIO_ERROR_DEVICEBUSY",                                    //0x0006
  "AUDIO_ERROR_BADFORMAT",                                     //0x0007
  "AUDIO_ERROR_NOMEMORY",                                      //0x0008
  "AUDIO_ERROR_NODRAMMEMORY",                                  //0x0009
  "AUDIO_ERROR_FILENOTFOUND",                                  //0x000A
  "AUDIO_ERROR_BADFILEFORMAT",                                 //0x000B
  "AUDIO_LAST_ERROR",                                          //0x000C
  "SEAL error was not defined"                                 //0x000D
};


//..............................................................................
Mix_Chunk              *pSons[iNOMBRESONS];                    //structures de tous les sons
Uint32		fSons[iNOMBRESONS];                    //flag pour chaque son 1=charge
extern configfile*	power_conf;

//prototypes des fonctions loacales ............................................
int                     seal_initialise(int);
void                    seal_restaure();
int                     seal_chargeModule(int);
void                    seal_error(char *_pErreur1, char *_pErreur2,
                                   int _iErreur);
char                   *locate_data_file(const char *const name);

//------------------------------------------------------------------------------
// seal : premiere initialisation / first initializations
//------------------------------------------------------------------------------
int seal_init()
{
  Uint32 i;
  if(!power_conf->nosound)
  { iVolumeModule = MIX_MAX_VOLUME;
    iVolumeModule2 = MIX_MAX_VOLUME;
    iChangeSonMusique = 0;
    iFlagMusique = 1;                                          //1=joue un module
    iBruitagesVoix = iBRUITAGESVOIX;                           //nombre de voix pour les bruitages
    iNumeroModule = -1;
    iNumeroModule2 = 0;
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0)
    { fprintf(stderr, "sdl_mixer.cpp/seal_init(): Ainitialise() failed: %s\n",
        SDL_GetError());
      power_conf->nosound = 1;
      return 1;
    }
/*#ifdef _GP2X
    // We need a reduced audio rate for the GP2X to make sure sound doesn't lag
    int audio_rate = 22050;
    int audio_buffers = 64;
#else*/
    int audio_rate = 44100;
    int audio_buffers = 4096;
//#endif
    Uint16 audio_format = AUDIO_S16;
    if(Mix_OpenAudio(audio_rate, audio_format, 2, audio_buffers))
    { fprintf(stderr,"sdl_mixer.cpp/seal_init(): %s\n",SDL_GetError());
      power_conf->nosound = 1;
		  SDL_Quit();
		  return 1;
	  }
    Mix_AllocateChannels(16);
    //if(seal_initialise(iNumeroModule2) == FALSE)
    if(!seal_JoueModule(0))
      return FALSE;
    if(power_conf->nosound)
      return 1;

    //charge les fichiers "waves" ----------------------------------------------
    iTailleSons = 0;                                           //taille total des sons
    for(i = 0; i < iNOMBRESONS; i++)
    { char *_pNom = pFichiersSons[i];
      char *pathname = locate_data_file(_pNom);
      if(!pathname)
      { fprintf(stderr,
           "!sdl_mixer.cpp/seal_init() / error locating data file %s\n",
            _pNom);
        return FALSE;
      }
      Mix_Chunk *_pSon = Mix_LoadWAV(pathname);
      if(!_pSon) {
        afficheErreur(pathname,
          "! sdl_mixer.cpp/seal_init() / Mix_LoadWAV(),  wave file no found ");
        return FALSE;
      }
      pSons[i] = _pSon;
      iTailleSons += _pSon->alen;                              //calcul taille total des sons
      fSons[i] = 1;                                            //ok, son charge
    }
  }
  if(power_conf->verbose)
    printf("> sdl_mixer.cpp/seal_init(): successful initialization!\n");
  return TRUE;
}

//------------------------------------------------------------------------------
// seal : initialisations / initializations
//------------------------------------------------------------------------------
int seal_initialise(int _iNumeroModule)
{
  if(seal_chargeModule(_iNumeroModule) == FALSE)
    return FALSE;
  return 1;
}

//------------------------------------------------------------------------------
// seal : chargement d'un module / loading a module
// input => _iNumeroModule : 0 a n
//------------------------------------------------------------------------------
int seal_chargeModule(int _iNumeroModule)
{
  char *_pNom;
  if(iFlagMusique)
  { if(iNumeroModule >= 0)
    { Mix_HaltMusic();
      Mix_FreeMusic(lpModule);
      iNumeroModule = -1;
    }
    _pNom = pFichiersModules[_iNumeroModule];
    char *pathname = locate_data_file(_pNom);
    if(!pathname)
    { fprintf(stderr,
        "! error locating data file %s\n",
        _pNom);
      return FALSE;
    }
	
	fprintf(stderr, "! seal_chargeModule() pathname %s\n", pathname);

    lpModule = Mix_LoadMUS(pathname);
    if(!lpModule)
    { fprintf(stderr,
        "! sdl_mixer.cpp/seal_chargeModule() erreur %s\n",
        SDL_GetError());
      return FALSE;
    }
#ifdef _VERBEUX_
    if(power_conf->verbose > 0)
      fprintf(stdout,
        "> sdl_mixer.cpp/seal_chargeModule() loaded module: %s\n",
        _pNom);
#endif
    iNumeroModule = _iNumeroModule;
  }
  return TRUE;
}

//------------------------------------------------------------------------------
// seal : joue un module / playing a module
// input => _iNumeroModule : 0 a n / 0 to n
//------------------------------------------------------------------------------
int seal_JoueModule(int _iNumeroModule)
{
  iNumeroModule2 = _iNumeroModule;
  if(iFlagMusique)
  {
    if(seal_chargeModule(_iNumeroModule)) {
      if(Mix_PlayMusic(lpModule, -1) == -1) {
        fprintf(
          stderr,
          "! sdl_mixer.cpp/seal_JoueModuleerreur %s\n",
          SDL_GetError());
      }
      return 1;
    }
  return 0;
  }
  return 1;
}

//------------------------------------------------------------------------------
// seal : execution / runtime
//------------------------------------------------------------------------------
void seal_exec()
{
  Uint32 i;
  if(!power_conf->nosound)
  {
    //regle volume du module
    if(iFlagMusique && etat_menu == MENU_OFF
       && _score::getActionExecution() == 0)
    {
      //on est dans le jeu ?
      if(!aff_game_over)
      {
        if(iVolumeModule2 != iVolumeModule / 2)
        { iVolumeModule2 = iVolumeModule / 2;
          Mix_VolumeMusic (iVolumeModule2);
        }
      }
      else
      {
        if(iVolumeModule2 != iVolumeModule)
        {
          iVolumeModule2 = iVolumeModule;
          Mix_VolumeMusic (iVolumeModule2);
        }
      }
    }
    for(i = 0; i < iNOMBRESONS; i++)
    {
      if(iFlagBruitages[i])
      { if (Mix_PlayChannel(-1,pSons[i], 0) == -1) {
          /*fprintf(
            stderr,
            "! sdl_mixer.cpp/seal_exec(): Mix_PlayChannel, %s\n",
            Mix_GetError());*/
        }
        iFlagBruitages[i] = 0;
      }
    }
    if(iChangeSonMusique && !touch[88] && !touch[60])          //touche [ctrl]+[s] relachee ?
    {
      iChangeSonMusique = 0;
      switch (iFlagMusique)
      {
        case 0:                                               //musique inactive, on l'active
          seal_restaure();
          iFlagMusique = 1;
          iBruitagesVoix = iBRUITAGESVOIX;                     //nombre de voix pour les bruitages
          if(seal_initialise(iNumeroModule2) == FALSE)
            quit_game = 1;
          Mix_PlayMusic(lpModule, -1);
          Mix_VolumeMusic(iVolumeModule);
          break;
        case 1:                                               //musique active, on la desactive
          seal_restaure();
          iFlagMusique = 0;
          iBruitagesVoix = iBRUITAGESVOIX2;                    //nombre de voix pour les bruitages
          if(seal_initialise(iNumeroModule2) == FALSE)
            quit_game = 1;
          Mix_VolumeMusic(0);
          break;
      }
    }
    if(touch[88] && touch[60])
      iChangeSonMusique = 1;                                   //touche [ctrl]+[s] pressee ?
  }
}

//------------------------------------------------------------------------------
// SDL_mixer: release sounds
//------------------------------------------------------------------------------
void seal_kill()
{
	Uint32 i;
	if(!power_conf->nosound)
	{	for(i = 0; i < iNOMBRESONS; i++)
		{	if(fSons[i] == 1)	//sound was loaded?
			{	Mix_FreeChunk(pSons[i]);
				fSons[i] = 0;	//ok, sound released
			}
		}
		Mix_CloseAudio();
		SDL_Quit();
	}
}

//------------------------------------------------------------------------------
// seal : restaure
//------------------------------------------------------------------------------
void seal_restaure()
{
}

//------------------------------------------------------------------------------
// seal : demande a jouer un son
// input => iNumeroSon : numero du son de 0 a iNOMBRESONS
//------------------------------------------------------------------------------
void seal_joue(Uint32 iNumeroSon)
{
  if(!aff_game_over && !player_pause && _score::getActionExecution() == 0
     && etat_menu == MENU_OFF)
    iFlagBruitages[iNumeroSon] = 1;
}

//------------------------------------------------------------------------------
// seal : display error message
// input => _pErreur1    : error message
//       => _pErreur2    : error title
//       => _iErreur     : error number
//------------------------------------------------------------------------------
void seal_error(char *_pErreur1, char *_pErreur2, int _iErreur)
{
  if(_iErreur < 0 || _iErreur > 13)
    _iErreur = 13;
  afficheErreur(_pErreur1, _pErreur2, SealErrorText[_iErreur]);
}
//..............................................................................
#endif

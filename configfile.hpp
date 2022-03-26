//*****************************************************************************
// copyright (c) 2004-2005 TLK Games 
//-----------------------------------------------------------------------------
// file         : "configfile.cpp"
// created      : 2004-12-12
// updates      : 2005-01-09
// id		: $Id
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA  02111-1307, USA.
//*****************************************************************************
#ifndef __CONFIGFILE__
#define __CONFIGFILE__
#include <SDL/SDL.h>

#include <sys/types.h>
#include <sys/stat.h>
#include "lispreader.hpp"

//..............................................................................
#ifdef _WIN32
#include <direct.h>
#define MKDIR(d, p) _mkdir(d)
#else
#include <dirent.h>
#define MKDIR mkdir
#endif
#ifdef _WIN32
#define CONFIG_DIR_NAME "tlkgames"
#else
#define CONFIG_DIR_NAME ".tlkgames"
#endif
#define CONFIG_FILE_NAME "powermanga.conf"
//..............................................................................



enum xlanguages {ENLANGUAGE, FRLANGUAGE, XXLANGUAGE};


class configfile 
{

	private:
		static		char* lang2text[XXLANGUAGE];

	public:
		bool		fullscreen;	//0=windowed mode / 1=full screen
		bool		nosound;	//1=force no sound
		bool		nosync;		//1=disable timer
		Sint32		resolution;	//320 or 640
		Sint32		verbose;	//1=verbose mode / 2=more messages
		Sint32		difficulty;	//0=easy / 1=normal / 2=hard
		Sint32		lang;		//0=en / 1=fr
	
	private:
		char		config_dir[512]; 
		char		configname[512];
	
	public:
				configfile();
				~configfile();
		void		configinfo();
		void		loadconfig();
		void		saveconfig();
		Sint32		scanZeArgs(Sint32 nbArg, char **ptArg); 
		char*		get_txtlang();

	private:
		Sint32		tocheckdir();
		void		resetvalue();
		FILE * 		fopen_data(const char * rel_filename, const char * mode);
		
};




#endif



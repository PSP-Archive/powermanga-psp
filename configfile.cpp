//******************************************************************************
// Copyright (C) 1998-2005 TLK Games all rights reserved.
//------------------------------------------------------------------------------
// file         : "configfile.cpp"
// created      : 2005-12-12
// updates      : 2005-01-26
// id		: $Id: configfile.cpp,v 1.7 2006/03/27 09:13:14 gurumeditation Exp $
//------------------------------------------------------------------------------
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
//******************************************************************************
#include "powermanga.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//..............................................................................
char* configfile::lang2text[XXLANGUAGE] = {"en", "fr"};

//------------------------------------------------------------------------------
// create object
//------------------------------------------------------------------------------
	configfile::configfile()
{
	resetvalue();
}


//------------------------------------------------------------------------------
// destroy object
//------------------------------------------------------------------------------
	configfile::~configfile()
{
}

//------------------------------------------------------------------------------
// reset all values
//------------------------------------------------------------------------------
void	configfile::resetvalue()
{
	fullscreen = 1;
	nosound = 0;
	resolution = 640;
	verbose = 0;
	difficulty = 1;
	lang = 0;
}

//------------------------------------------------------------------------------
// display values
//------------------------------------------------------------------------------
void configfile::configinfo()
{
	fprintf(stdout, "  <config info>\n"
			"- fullscreen: %i\n- nosound: %i\n- resolution:%i\n"
		"- verbose: %i\n - difficulty: %i\n - lang:%s\n",
		fullscreen, nosound, resolution, verbose, difficulty,
		lang2text[lang]);
}

//------------------------------------------------------------------------------
// check if config directory exists; if not create it and set config_dir
//------------------------------------------------------------------------------
Sint32 configfile::tocheckdir()
{

#ifdef _WIN32

	_snprintf(config_dir, sizeof(config_dir) - 1, "./%s", CONFIG_DIR_NAME );

	/* create directory if not exist */
	MKDIR(config_dir, S_IRWXU);
	fprintf(stderr, "ok\n");

#elif defined(_GP2X)
    snprintf(config_dir, sizeof(config_dir) - 1, ".");
#else

	snprintf(config_dir, sizeof(config_dir) - 1, "%s/%s",
		(getenv( "HOME" )?getenv( "HOME" ):"."), CONFIG_DIR_NAME );
	/* test and create .lgames */
	if(!opendir(config_dir)) {
		fprintf(stderr, "couldn't find/open config directory '%s'\n", config_dir);
		fprintf(stderr, "attempting to create it... " );
		MKDIR(config_dir, S_IRWXU );
		if(!opendir(config_dir))
		{	fprintf(stderr, "failed\n");
			return 0;
		}
		else
			fprintf(stderr, "ok\n");
	}
#endif

	return 1;
}


//------------------------------------------------------------------------------
// load config file "~/.tlkgames/powermanga.conf"
//------------------------------------------------------------------------------
void configfile::loadconfig()
{
	FILE *pfile = NULL;
	resetvalue();
	if (!tocheckdir()) return;
	sprintf(configname, "%s/%s", config_dir, CONFIG_FILE_NAME);
	pfile = fopen_data(configname, "r");
	if (pfile == NULL) return;
	lisp_stream_t stream;
	lisp_object_t *root_obj = NULL;
	lisp_stream_init_file (&stream, pfile);
	root_obj = lisp_read (&stream);

	fclose(pfile);

	if (root_obj->type == LISP_TYPE_EOF || root_obj->type == LISP_TYPE_PARSE_ERROR)
	{	fprintf(stderr, "configfile::loadconfig() / conf parsing failed\n");
		return;
	}

	if (strcmp(lisp_symbol(lisp_car(root_obj)), "powermanga-config") != 0)
	{	fprintf(stderr, "configfile::loadconfig() / conf parsing failed\n");
		return;
	}
	LispReader reader(lisp_cdr(root_obj));
	std::string ptStr;
	if (!reader.read_string ("lang", &ptStr))
		lang = ENLANGUAGE;
	 else
	{	if (ptStr == "fr")
			lang = FRLANGUAGE;
		else
			lang = ENLANGUAGE;
	}
	if (!reader.read_bool("fullscreen", &fullscreen))
		fullscreen = true;

	if (!reader.read_bool("nosound", &nosound))
		nosound = false;
	if (!reader.read_bool("nosync", &nosync))
		nosync = false;
	if (!reader.read_int ("verbose", &verbose))
		verbose = 0;


	if (!reader.read_int ("resolution", &resolution))
		resolution = 640;
	if (resolution != 320 && resolution != 640)
		resolution = 640;
	lisp_free(root_obj);
}

//------------------------------------------------------------------------------
// save config file "~/.tlkgames/powermanga.conf"
//------------------------------------------------------------------------------
void configfile::saveconfig()
{
	FILE * config = fopen_data(configname, "w");
	if(config)
	{
		fprintf(config, "(powermanga-config\n");
		fprintf(config, "\t;; the following options can be set to #t or #f:\n");
		fprintf(config, "\t(fullscreen %s)\n", fullscreen ? "#t" : "#f");
		fprintf(config, "\t(nosound %s)\n", nosound ? "#t" : "#f");
		fprintf(config, "\t(nosync %s)\n", nosync ? "#t" : "#f");

		fprintf(config, "\n\t;; window size (320 or 640):\n");
	       	fprintf(config, "\t(resolution  %d)\n", resolution);

		fprintf(config, "\n\t;; verbose mode 0 (disabled), 1 (enable) or 2 (more messages)\n");
      		fprintf(config, "\t(verbose   %d)\n", verbose);

		fprintf(config, "\n\t;; difficulty 0 (easy), 1 (normal) or 2 (hard)\n");
      		fprintf(config, "\t(difficulty   %d)\n", difficulty);

		fprintf(config, "\n\t;; langage en or fr\n");
		fprintf(config, "\t(lang      ");
		switch (lang)
		{	case FRLANGUAGE:
				fprintf(config, "\"fr\")\n");
				break;
			default:
				fprintf(config, "\"en\")\n");
				break;
		}
		fprintf(config, ")\n");

	}

	fclose(config);
}

//------------------------------------------------------------------------------
// open a file
//------------------------------------------------------------------------------
FILE * configfile::fopen_data(const char *fname, const char *fmode)
{
	FILE * fi;
	fi = fopen(fname, fmode);
	if (fi == NULL)
	{	fprintf(stderr, "configfile::fopen_data(): Warning: Unable "
			"to open the file \"%s\" ", fname);
		if (strcmp(fmode, "r") == 0)
			fprintf(stderr, "for read!!!\n");
		else if (strcmp(fmode, "w") == 0)
			fprintf(stderr, "for write!!!\n");
	}
	return(fi);
}


//------------------------------------------------------------------------------
// analyse command line paramaters
//------------------------------------------------------------------------------
Sint32 configfile::scanZeArgs(Sint32 nbArg, char **ptArg)
{
	for(Sint32 _iIndex = 1; _iIndex < nbArg; _iIndex++)
	{
		if(*ptArg[_iIndex] != '-')
			continue;

		//######################################################
		// help
		//######################################################
		if(!strcmp(ptArg[_iIndex], "-h") ||
			!strcmp(ptArg[_iIndex], "--help"))
		{
			fprintf(stdout, "\noptions:\n"
				"-h, --help     print Help (this message) and exit\n"
  				"--version      print version information and exit\n"
				"--320          game run in a 320*200 window (slow machine)\n");
#ifdef SDL_TLK
			fprintf(stdout,
				"--window       windowed mode (full screen by default) \n");
#endif
			fprintf(stdout,
				"-v             verbose mode\n"
				"--verbose      verbose mode (more messages)\n"
				"--nosound      force no sound\n"
				"--nosync       disable timer\n"
				"--easy         easy bonuses\n"
				"--hard         hard bonuses\n"
				"--------------------------------------------------------------\n"
				"keys recognized during the game:\n"
				"CTRL+S         enable/disable the music\n"
				"CTRL+Q         finish the play current\n"
				"CTRL+A         about Powermanga\n"
				"F10            quit Powermanga\n"
				"P              enable/disable pause\n");
#ifdef SDL_TLK
			fprintf(stdout,
				"F              switch between full screen and windowed mode\n");
#endif
return 0;

		}


		if(!strcmp(ptArg[_iIndex], "--version"))
		{	printf(POWERMANGA_VERSION);
			printf("\n");
			printf("copyright (c) 1998-2006 TLK Games\n");
			printf("website: http://linux.tlk.fr/\n");
			return 0;
		}

		//######################################################
		//force window mode
		//######################################################
		if(!strcmp(ptArg[_iIndex], "--window"))
		{	fullscreen = false;
			continue;
		}

		//######################################################
		//force fullscreen mode
		//######################################################
		if(!strcmp(ptArg[_iIndex], "--fullscreen"))
		{	fullscreen = true;
			continue;
		}

		//######################################################
		// resolution, low-res or high-res
		//######################################################
                if(!strcmp(ptArg[_iIndex], "--320"))
		{	resolution = 320;
			continue;
		}
                if(!strcmp(ptArg[_iIndex], "--640"))
		{	resolution = 640;
			continue;
		}

		//######################################################
		// enable verbose mode
		//######################################################
		if(!strcmp(ptArg[_iIndex], "-v"))
		{	verbose = 1;
			continue;
		}
		if(!strcmp(ptArg[_iIndex], "--verbose"))
		{	verbose = 2;
			continue;
		}

		//######################################################
		// disable sound
		//######################################################
		if(!strcmp(ptArg[_iIndex], "--nosound"))
		{	nosound = true;
			continue;
		}

		//######################################################
		// disable timer
		//######################################################
		if(!strcmp(ptArg[_iIndex], "--nosync"))
		{	nosync = true;
			continue;
		}

		//######################################################
		// difficulty: easy or hard (normal bu default)
		//######################################################
		if(!strcmp(ptArg[_iIndex], "--easy"))
		{	difficulty = 0;
			continue;
		}
		if(!strcmp(ptArg[_iIndex], "--hard"))
		{	difficulty = 2;
			continue;
		}
	}
	return 1;
}


//------------------------------------------------------------------------------
// return current language
//------------------------------------------------------------------------------
char* configfile::get_txtlang()
{
	return	lang2text[lang];
}

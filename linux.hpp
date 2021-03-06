//*****************************************************************************
// copyright (c) 1998-2003 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "linux..hpp"
// created      : 1999-08-17
// updates      : 2003-06-28
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
//
//*****************************************************************************
#ifndef __LINUXHPP__
#define __LINUXHPP__

//////////////////////////
#ifdef WIN32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include <math.h>

#else //WIN32

//..............................................................................
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
//#include <sys/utsname.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

#endif //WIN32

//..............................................................................
#define _VERBEUX_
#define TRUE                   1
#define FALSE                  0

//gestion fichier graphique en memoire ...........................................
typedef struct
{ unsigned int            iLargeur;                            //image width
  unsigned int            iHauteur;                            //image height
  unsigned int            iProfond;                            //bits per pixels
  unsigned int            iTaille;                             //memory size
  char*                   pAdresse;                            //pointer to the image data
  unsigned char           tPaletteGif[768] ;                   //palette
} sDescriptionGfx;

//"xwindow.cpp"....................................................................
int                    xw_init();
int                    reserve_ecran_ram();
int                    charge_palette(char* _pNomFichier);
int                    xw_exec();
int                    change_resolution();
int                    xw_kill();
char                  *Xload_pcx(char* _pNomFichier);
int                    Xload_pcx(char* _pNomFichier, char* _pMem);
char                  *convertit_16ou24(sDescriptionGfx* _pSrc);
void                   copie_gfx(sDescriptionGfx* _pGfx);
int                    reserve_ecranPlayanim();
void                   kill_ecranPlayanim();
#ifdef SDL_TLK
void                   verouille();
void                   deverouille();
int                    init_video_mode();
#endif

//linuxroutines.cpp ------------------------------------------------------------
int                    mem_initialise(unsigned int _iNombreZones);
char*                  reserveMemoire(unsigned int _iTailleMemoire);
void                   libereMemoire(char* _pMemoire);
int                    mem_retourneTaille(char* _pMem);
void                   mem_libereTous();
int                    load_pcx(char*, char*);
sDescriptionGfx*       load_pcx(char*);
unsigned int           littleWord(char* _pMem);
short                  litMot16bits(short* _pMem);
int                    litMot32bits(int* _pMem);
void                   convert32bits_2bigendian (unsigned char* memory);
int                    chaine_cmp(char* _pChaine1, char* _pChaine2, unsigned int _iTaille);
char*                  chargeFichier(char* _pNomFichier);
char*                  chargeFichier(char* _pNomFichier, unsigned int* pTaille);
int                    chargeFichier(char* _pMemoire, char* _pNomFichier);
void                   fps_init();
void                   fps_affiche();
void                   afficheMessage(char* _Message);
void                   afficheMessage(char* _Message1, char* _Message2);
void                   afficheErreur(char* _pErreur1, char* _pErreur2);
void                   afficheErreur(char* _pErreur1, char* _pErreur2, int _iErreur);
void                   afficheErreur(char* _pErreur1, char* _pErreur2, char* _pErreur3);
int                    synchro_processusPause(int _iTemps);
int                    synchro_CalculDifference();
int                    synchro_CalculDifference2();


#endif

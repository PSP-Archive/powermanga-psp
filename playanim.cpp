//*****************************************************************************
// copyright (c) 1998-2003 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "playanim.cpp"
// created      : ?
// updates      : 2003-07-20
//-----------------------------------------------------------------------------
// functions    : int            joueAnimation()
//                int            InitAnim(char *)
//                void           ReleaseAnim()
//                int            init(char*);
//                unsigned char* PlayAnim(int);
//                unsigned char* decompress(unsigned char*, unsigned char*,unsigned char*);
//-----------------------------------------------------------------------------
// descriptions         : play animation / 
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
#include "powermanga.hpp"
//..............................................................................
unsigned char          *image1 = 0;
static unsigned char   *image2 = 0;
static unsigned char   *icmpr = 0;
static unsigned char   *smage1 = 0;
static unsigned char   *smage2 = 0;
static unsigned char   *scmpr = 0;
static unsigned char   *pFichier = 0;                          //pointer to the file in memory

//..............................................................................
static unsigned char   *pcxpal = 0;                            //pointer to the colormap in the file
static int              ic = 0;
static int              images = 0;

//..............................................................................
extern unsigned int     iOctetsParPixel;                       //bytes per pixel "xwindow.cpp"
extern unsigned int     iCodeTouche;                           //key code pressed)
extern unsigned int     iJoueIntro;                            //(1=intro / 2=congratulation)
extern unsigned short  *pal16PlayAnim;                         //16 bits colormap (2 bytes par color)
extern unsigned int    *pal32PlayAnim;                         //32 bits colormap (4 bytes par color)

//locals functions .............................................................
int                     InitAnim(char *);
void                    ReleaseAnim();
int                     ainit(char *);
unsigned char          *PlayAnim(int);
unsigned char          *decompress(unsigned char *, unsigned char *, unsigned char *);

//------------------------------------------------------------------------------
// play animation compressed 
//------------------------------------------------------------------------------
int joueAnimation()
{
  switch (iJoueIntro)
  { //introduction animation
    case 1: 
      if(InitAnim("graphics/deb.gca"))
        iJoueIntro = 5;
      else
      { iJoueIntro = 0;
        afficheErreur("InitAnim(\"graphics/deb.gca\")", "playanim.cpp/joueAnimation()");
        return 0;
      }
      break;
    //congratulation animation
    case 2:
      if(InitAnim("graphics/fin.gca"))
        iJoueIntro = 5;
      else
      { iJoueIntro = 0;
        afficheErreur("InitAnim(\"graphics/fin.gca\")", "playanim.cpp/joueAnimation()");
        return 0;
      }
      break;
    //play animation compressed 
    case 5: 
    { unsigned char *_pScreen = PlayAnim(0);
      if(_pScreen != NULL && iCodeTouche == 0)
      {
      }
      else
      { ReleaseAnim();
        iJoueIntro = 0;
      }
    }
      break;
  }
  return 1;
}

//------------------------------------------------------------------------------
// initialize animation
// input => _pFileName : filename
//------------------------------------------------------------------------------
int InitAnim(char *_pFileName)
{
  int i = 0;
  smage1 = smage2 = scmpr = 0;
  smage1 = ((unsigned char *)reserveMemoire(64000));
  if(!smage1)
  { afficheErreur("'smage1' out of memory", "playanim.cpp/InitAnim()");
    return 0;
  }
  smage2 = ((unsigned char *)reserveMemoire(64000));
  if(!smage1)
  { afficheErreur("'smage2' out of memory", "playanim.cpp/InitAnim()");
    return 0;
  }
  pFichier = ((unsigned char *)reserveMemoire((1024 * 1024 * 4)));
  if(!pFichier)
  { afficheErreur("'pFichier' out of memory", "playanim.cpp/InitAnim");
    return 0;
  }
  for(i = 0; i < 64000; i++)
  { smage1[i] = 0;
    smage2[i] = 0;
  }
  if(!ainit(_pFileName))
  { afficheErreur("ainit() failed !", "playanim.cpp/InitAnim()");
    return 0;
  }
  image1 = smage1;
  image2 = smage2;
  icmpr = scmpr;
  ic = 0;
  if(!reserve_ecranPlayanim())
  {afficheErreur("reserve_ecranPlayanim() failed !", "playanim.cpp/InitAnim()");
    return 0;
  }
  return 1;
}

//------------------------------------------------------------------------------
// initialize animation
// input => _pFileName : filename
//------------------------------------------------------------------------------
int ainit(char *_pFileName)
{
  int *_p32;
  unsigned char *_p8;

  if(!chargeFichier(_pFileName, (char *)pFichier))
    return 0;
  _p32 = (int *)pFichier;
  images = litMot32bits(_p32++);
  _p8 = (unsigned char *)_p32;
  pcxpal = _p8;
  scmpr = _p8 + 768;
  if(iOctetsParPixel == 2)
  {
    if(!pal16PlayAnim)
      pal16PlayAnim = (unsigned short *)reserveMemoire(256 * 2);
    if(!pal16PlayAnim)
    {
      afficheErreur("'pal16PlayAnim' out of memory", "playanim.cpp/InitAnim()");
      return 0;
    }
    convertitPalette24_16(pcxpal, pal16PlayAnim);  //"assembler.S"
  }
  if(iOctetsParPixel > 2)
  {
    if(!pal32PlayAnim)
      pal32PlayAnim = (unsigned int *)reserveMemoire(256 * 4);
    if(!pal32PlayAnim)
    {
      afficheErreur("'pal32PlayAnim' out of memory", "playanim.cpp/InitAnim()");
      return 0;
    }
    unsigned char *_p = (unsigned char *)pal32PlayAnim;
    unsigned char *_pPal = pcxpal;
    for(int _iIndex = 0; _iIndex < 256; _iIndex++)
    {
#if __BYTE_ORDER == __BIG_ENDIAN
#ifdef SDL_TLK
      _p[2] = _pPal[2];
      _p[1] = _pPal[1];
      _p[0] = _pPal[0];
      _p[3] = 0;
#else     
      _p[3] = _pPal[2];
      _p[2] = _pPal[1];
      _p[1] = _pPal[0];
      _p[0] = 0;
#endif      
#else
#ifdef SDL_TLK
      _p[2] = _pPal[2];
      _p[1] = _pPal[1];
      _p[0] = _pPal[0];
      _p[3] = 0;
#else      
      _p[0] = _pPal[2];
      _p[1] = _pPal[1];
      _p[2] = _pPal[0];
      _p[3] = 0;
#endif      
#endif
      _p += 4;
      _pPal += 3;
    }
  }
  return 1;
}

//------------------------------------------------------------------------------
// playanim : free memory
//------------------------------------------------------------------------------
void ReleaseAnim()
{
  if(smage1)
  { libereMemoire((char *)smage1);
    smage1 = 0;
  }
  if(smage2)
  { libereMemoire((char *)smage2);
    smage2 = 0;
  }
  if(pal16PlayAnim)
  { libereMemoire((char *)pal16PlayAnim);
    pal16PlayAnim = 0;
  }
  kill_ecranPlayanim(); // "xwindow.cpp" free XImage
}

//------------------------------------------------------------------------------
// playanim : run-time
//------------------------------------------------------------------------------
unsigned char *PlayAnim(int bcl)
{
  int i;
  unsigned char *tmp; //images=64+16+2+1+1 ;
  ic++;
  if(ic == images - 1)
    if(bcl == 0)
      return 0;
    else
    { ic = 0;
      icmpr = scmpr;
      for(i = 0; i < 64000; i++)
      { smage1[i] = 0;
        smage2[i] = 0;
      }
    }
  icmpr = decompress(icmpr, image1, image2);
  tmp = image1;
  image1 = image2;
  image2 = tmp;
  return image1;
}

//------------------------------------------------------------------------------
// playanim : decompress 
//------------------------------------------------------------------------------
unsigned char *decompress(unsigned char *wsc, unsigned char *im1, unsigned char *im2)
{
  int i, j;
  unsigned char *idec;
  unsigned char *wdec;
  unsigned char c;
  unsigned char color;
  int longueur = 0;
  int retour = 0;
  int position = 0;
  i = 0;
  idec = im2;
  while (i < 64000)
  { int mode = 0;
    c = *wsc++; //read a byte
    if(c == 255)
    { color = *wsc++;
      *idec++ = color;
      i++;
      mode = 0;
    }
    else if((c & 0xc0) == 0)
    { unsigned int wr = 0;
      wr = c << 16;
      wr += (*wsc++) << 8;
      wr += *wsc++;
      longueur = wr & 63;
      position = (wr >> 6) & 65535;
      mode = 1;
    }
    else if((c & 0xc0) == 0x40)
    { unsigned int wr = 0;
      wr = c << 24;
      wr += (*wsc++) << 16;
      wr += (*wsc++) << 8;
      wr += *wsc++;
      longueur = wr & 16383;
      position = (wr >> 14) & 65535;
      mode = 1;
    }
    else if((c & 0xe0) == 0x80)
    { unsigned int wr = 0;
      wr = c << 8;
      wr += *wsc++;
      longueur = wr & 63;
      retour = (wr >> 6) & 255;
      mode = 2;
    }
    else if((c & 0xe0) == 0xa0)
    { unsigned int wr = 0;
      wr = c << 16;
      wr += (*wsc++) << 8;
      wr += *wsc++;
      longueur = wr & 255;
      retour = (wr >> 8) & 8191;
      mode = 2;
    }
    else if((c & 0xe0) == 0xc0)
    { unsigned int wr = 0;
      wr = c << 24;
      wr += (*wsc++) << 16;
      wr += (*wsc++) << 8;
      wr += *wsc++;
      longueur = wr & 8191;
      position = (wr >> 13) & 65535;
      mode = 3;
    }
    if(i + longueur > 64000)
    { longueur = 64000 - i;
    }
    if(mode == 1)
    { for(j = 0; j < longueur; j++)
        *idec++ = im1[position + j];
      i += longueur;
    }
    else if(mode == 2)
    { wdec = idec - retour;
      for(j = 0; j < longueur; j++)
        *idec++ = *wdec++;
      i += longueur;
    }
    else if(mode == 3)
    { for(j = 0; j < longueur; j++)
        *idec++ = im2[position + j];
      i += longueur;
    }
  }
  return wsc;
}

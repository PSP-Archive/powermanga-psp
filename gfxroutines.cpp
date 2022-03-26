//*****************************************************************************
// copyright (c) 2003 Samuel Hocevar <sam@zoy.org>
//-----------------------------------------------------------------------------
// file         : "gfxroutines.cpp"
// created      : 2003-04-15
// updates      : 2006-03-26
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


void _type_routine_gfx(int *adresse)
{
  *adresse = 20;
}

typedef struct _compress
{
  unsigned int            offset;                              //offset
  unsigned short          r1;                                  //
  unsigned short          r2;
}
_compress;

/* To test these functions: the intro animation */

void conv8_16(char *_pSource, char *_pDestination,
              unsigned short *_pPalette16, unsigned int _iTaille)
{
    unsigned short *pDestination = (unsigned short *)_pDestination;
    unsigned char *pSource = (unsigned char *)_pSource;

    while (_iTaille--)
        *pDestination++ = _pPalette16[*pSource++];
}

void conv8_24(char *_pSource, char *_pDestination,
              unsigned int *_pPalette24, unsigned int _iTaille)
{
    printf("conv8_24 not implemented\n");
}

void conv8_32(char *_pSource, char *_pDestination,
              unsigned int *_pPalette32, unsigned int _iTaille)
{
    unsigned int *pDestination = (unsigned int *)_pDestination;
    unsigned char *pSource = (unsigned char *)_pSource;

    while (_iTaille--)
        *pDestination++ = _pPalette32[*pSource++];
}

/* ??? */

void copie4octets(char *src, char *dest,
                  unsigned int _iLarge, unsigned int _iHaut,
                  unsigned int _iOffset, unsigned int _iOffset2)
{
    printf("copie4octets not implemented\n");
}

void copie8octets(char *src, char *dest, unsigned int _iLarge,
                  unsigned int _iHaut, unsigned int _iOffset)
{
    printf("copie8octets not implemented\n");
}

/* To test these functions: the whole display */

void raz_ecran(char *_pEcran, unsigned int _iLargeur,
               unsigned int _iHauteur, unsigned int _iOffset)
{
    _iLargeur *= 4;

    while(_iHauteur--)
    {
        memset(_pEcran, 0, _iLargeur);
        _pEcran += _iLargeur + _iOffset;
    }
}

/* To test these functions: the whole display */

void convertitPalette24_16(unsigned char *pal, unsigned short *pal16)
{
    int i = 256;
    while(i--)
    {
        unsigned short dest = 0;
	// fix bug, green value range 0 to 64 in 16 bits depth
	// 2006-03-26
	dest |= (unsigned short)((*pal++) & 0x000000f8) << 8;
        dest |= (unsigned short)((*pal++) & 0xfffffffc) << 3;
        dest |= (unsigned short)((*pal++) & 0x000000f8) >> 3;
	// dest |= (unsigned short)((*pal++) >> 3) << 11;
        // dest |= (unsigned short)((*pal++) >> 3) << 5;
        // dest |= (unsigned short)((*pal++) >> 3) << 0;
        *pal16++ = dest;
    }
}

void convertitPalette24_15(unsigned char *pal, unsigned short *pal16)
{
    int i = 256;

    while(i--)
    {
        unsigned short dest = 0;

        dest |= ((unsigned short)(*pal++) >> 3) << 10;
        dest |= ((unsigned short)(*pal++) >> 3) << 5;
        dest |= ((unsigned short)(*pal++) >> 3) << 0;

        *pal16++ = dest;
    }
}

/* To test these functions: the intro animation with the --640 flag */

#define COPY2X(TYPE) \
    TYPE *src = (TYPE *)_src; \
    TYPE *dest = (TYPE *)_dest; \
    \
    while(_iHaut--) \
    { \
        int i; \
        for(i = _iLarge / 8; i--;) \
        { \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
        } \
        for(i = _iLarge % 8; i--;) \
        { \
            dest[0] = dest[1] = *src++; dest += 2; \
        } \
        src += _iOffset / sizeof(TYPE); \
        dest += _iOffset2 / sizeof(TYPE); \
    }

void _COPY2X8BITS(char *_src, char *_dest,
                  unsigned int _iLarge, unsigned int _iHaut,
                  unsigned int _iOffset, unsigned int _iOffset2)
{
    COPY2X(unsigned char);
}

void _COPY2X16BITS(char *_src, char *_dest,
                   unsigned int _iLarge, unsigned int _iHaut,
                   unsigned int _iOffset, unsigned int _iOffset2)
{
    COPY2X(unsigned short);
}

void _COPY2X24BITS(char *_src, char *_dest,
                   unsigned int _iLarge, unsigned int _iHaut,
                   unsigned int _iOffset, unsigned int _iOffset2)
{
    printf("_COPY2X24BITS not implemented\n");
}

void _COPY2X32BITS(char *_src, char *_dest,
                   unsigned int _iLarge, unsigned int _iHaut,
                   unsigned int _iOffset, unsigned int _iOffset2)
{
    COPY2X(unsigned int);
}

/* ??? */

//-----------------------------------------------------------------------------
// display fontes 8*8 pixels (8 / 16 / 24 or 32 bits)
//-----------------------------------------------------------------------------
void _ASMCOPYFONTE8BITS(unsigned char *_pSource, unsigned char *_pDestination,
                        unsigned int _iLarge, unsigned int _iHaut,
                        unsigned int _iOffsetS, unsigned int _iOffsetD)
{
  unsigned char *s = _pSource;
  unsigned char *d = _pDestination;
  for(unsigned int h = 0; h < _iHaut; h++)
  { for(unsigned int l = 0; l < _iLarge; l++)
    { unsigned char p = *(s++);
      if(p != 0)
        *d = p;
      d++;
    }
    s = s + _iOffsetS;
    d = d + _iOffsetD;
  }
}
void _ASMCOPYFONTE16BITS(unsigned char *_pSource, unsigned char *_pDestination,
                         unsigned int _iLarge, unsigned int _iHaut,
                         unsigned int _iOffsetS, unsigned int _iOffsetD)
{
  unsigned short *s = (unsigned short *)_pSource;
  unsigned short *d = (unsigned short *)_pDestination;
  _iOffsetS = _iOffsetS / 2;
  _iOffsetD = _iOffsetD / 2;
  for(unsigned int h = 0; h < _iHaut; h++)
  { for(unsigned int l = 0; l < _iLarge; l++)
    { unsigned short p = *(s++);
      if(p != 0)
        *d = p;
      d++;
    }
    s = s + _iOffsetS;
    d = d + _iOffsetD;
  }
}
void _ASMCOPYFONTE24BITS(unsigned char *_pSource, unsigned char *_pDestination,
                         unsigned int _iLarge, unsigned int _iHaut,
                         unsigned int _iOffsetS, unsigned int _iOffsetD)
{
    printf("_ASMCOPYFONTE24BITS not implemented\n");
}
void _ASMCOPYFONTE32BITS(unsigned char *_pSource, unsigned char *_pDestination,
                         unsigned int _iLarge, unsigned int _iHaut,
                         unsigned int _iOffsetS, unsigned int _iOffsetD)
{
  unsigned int *s = (unsigned int *)_pSource;
  unsigned int *d = (unsigned int *)_pDestination;
  _iOffsetS = _iOffsetS / 4;
  _iOffsetD = _iOffsetD / 4;
  for(unsigned int h = 0; h < _iHaut; h++)
  { for(unsigned int l = 0; l < _iLarge; l++)
    { unsigned int p = *(s++);
      if(p != 0)
        *d = p;
      d++;
    }
    s = s + _iOffsetS;
    d = d + _iOffsetD;
  }
}

/* To test these functions: the main menu, the whole game */

#define PUTSPRITE(TYPE) \
    unsigned int *pCompression = (unsigned int *)_pCompression; \
    \
    while(_iTaille--) \
    { \
        _pDestination += *pCompression++; \
        \
        unsigned int data = *pCompression++; \
        unsigned int i = ((data & 0x0000ffff) << 2) /* 32 bits words */ \
                       | ((data & 0xffff0000) \
                           / (0x10000 / sizeof(TYPE))); /* 8 bits words */ \
        \
        memcpy( _pDestination, _pSource, i ); \
        _pDestination += i; \
        _pSource += i; \
    }

void putsprite3_8bits(char *_pSource, char *_pDestination,
                      char *_pCompression, unsigned int _iTaille)
{

  //PUTSPRITE(unsigned char);
  unsigned char  *s = (unsigned char *)_pSource;
  unsigned char  *p = (unsigned char *)_pDestination;
  _compress  *t = (_compress *) _pCompression;
  do
  { register unsigned int d = t->offset;
    //d = d >> 1;
    p = p + d;
    unsigned short z = t->r1;
    unsigned int *s2 = (unsigned int *)s;
    unsigned int *p2 = (unsigned int *)p;
    memcpy(p2, s2, z * 4);
    p2 = p2 + z;
    s2 = s2 + z;
    s = (unsigned char *)s2;
    p = (unsigned char *)p2;
    z = t->r2;
    memcpy(p2, s2, z);
    p = p + z;
    s = s + z;
    t++;
    _iTaille = _iTaille - 1;
  }
  while (_iTaille > 0);
}

void putsprite3_16bits(char *_pSource, char *_pDestination,
                       char *_pCompression, unsigned int _iTaille)
{ 
  unsigned short  *s = (unsigned short *)_pSource;
  unsigned short  *p = (unsigned short *)_pDestination;
  _compress  *t = (_compress *) _pCompression;
  do
  { register unsigned int d = t->offset;
    d = d >> 1;
    p = p + d;
    unsigned short z = t->r1;
    unsigned int *s2 = (unsigned int *)s;
    unsigned int *p2 = (unsigned int *)p;
    memcpy(p2, s2, z * 4);
    p2 = p2 + z;
    s2 = s2 + z;
    s = (unsigned short *)s2;
    p = (unsigned short *)p2;
    z = t->r2;
    memcpy(p2, s2, z * 2);
    p = p + z;
    s = s + z;
    t++;
    _iTaille = _iTaille - 1;
  }
  while (_iTaille > 0);
}

void putsprite3_24bits(char *_pSource, char *_pDestination,
                       char *_pCompression, unsigned int _iTaille)
{
    printf("putsprite3_24bits not implemented\n");
}

void putsprite3_32bits(char *_pSource, char *_pDestination,
                       char *_pCompression, unsigned int _iTaille)
{
    //PUTSPRITE(unsigned int);
  unsigned int  *s = (unsigned int *)_pSource;
  unsigned int  *p = (unsigned int *)_pDestination;
  _compress  *t = (_compress *) _pCompression;
  do
  { register unsigned int d = t->offset;
    d = d >> 2;
    p = p + d;
    register unsigned short z = t->r1;
    memcpy(p, s, z * 4);
    p = p + z;
    s = s + z;
    z = t->r2;
    memcpy(p, s, z * 4);
    p = p + z;
    s = s + z;
    t++;
    _iTaille = _iTaille - 1;
  }
  while (_iTaille > 0);
}


/* To test these functions: when an enemy gets killed */

#define PUTCOLOR(TYPE) \
    TYPE *pDestination = (TYPE *)_pDestination; \
    unsigned int *pCompression = (unsigned int *)_pCompression; \
    \
    while(_iTaille--) \
    { \
        pDestination += *pCompression++ / sizeof(TYPE); \
        \
        unsigned int data = *pCompression++; \
        unsigned int i = ((data & 0x0000ffff) \
                           * (4 / sizeof(TYPE))) /* 32 bits words */ \
                       | ((data & 0xffff0000) >> 16); /* remaining */ \
        \
        if( sizeof(TYPE) == 1 ) \
        { \
            memset(pDestination, _iColor, i); \
            pDestination += i; \
        } \
        else \
        { \
            while(i--) *pDestination++ = _iColor; \
        } \
    }

void putsprcolor_8bits(unsigned int _iColor, char *_pDestination,
                       char *_pCompression, unsigned int _iTaille)
{
    //PUTCOLOR(unsigned char);
  unsigned char *p = (unsigned char *)_pDestination;
  _compress *t = (_compress *) _pCompression;
  do
  {
    unsigned int d = t->offset;
    //d = d >> 1;
    p = p + d;
    unsigned short z = t->r1;
    unsigned int *p2 = (unsigned int *)p;
    memset(p2, _iColor, z * 4);
    p2 = p2 + z;
    p = (unsigned char *)p2;
    z = t->r2;
    memset(p2, _iColor, z);
    p = p + z;
    t++;
    _iTaille = _iTaille - 1;
  }
  while (_iTaille > 0);
}

void putsprcolor_16bits(unsigned int _iColor, char *_pDestination,
                        char *_pCompression, unsigned int _iTaille)
{
  //PUTCOLOR(unsigned short);
  //return ;
  unsigned int d = _iColor << 16;
  _iColor = _iColor | d;
  unsigned short *p = (unsigned short *)_pDestination;
  _compress *t = (_compress *) _pCompression;
  do
  {
    d = t->offset;
    d = d >> 1;
    p = p + d;
    unsigned short z = t->r1;
    unsigned int *p2 = (unsigned int *)p;
    memset(p2, _iColor, z * 4);
    p2 = p2 + z;
    p = (unsigned short *)p2;
    z = t->r2;
    memset(p2, _iColor, z * 2);
    p = p + z;
    t++;
    _iTaille = _iTaille - 1;
  }
  while (_iTaille > 0);

}

void putsprcolor_24bits(unsigned int _iColor, char *_pDestination,
                        char *_pCompression, unsigned int _iTaille)
{
    printf("putsprcolor_24bits not implemented\n");
}

void putsprcolor_32bits(unsigned int _iColor, char *_pDestination,
                        char *_pCompression, unsigned int _iTaille)
{
    PUTCOLOR(unsigned int);
}

/* To test these functions: the nuke (purple gem) */

#define POLY(TYPE) \
    TYPE *pDestination = (TYPE *)_pDestination; \
    \
    if(sizeof(TYPE) == 1) \
        memset(pDestination, _iCouleur, _iNombrePixel); \
    else \
        while(_iNombrePixel--) \
            *pDestination++ = _iCouleur

void poly8bits(char *_pDestination, int _iNombrePixel, int _iCouleur)
{
    POLY(unsigned char);
}

void poly16bits(char *_pDestination, int _iNombrePixel, int _iCouleur)
{
    POLY(unsigned short);
}

void poly24bits(char *_pDestination, int _iNombrePixel, int _iCouleur)
{
    printf("poly24bits not implemented\n");
}

void poly32bits(char *_pDestination, int _iNombrePixel, int _iCouleur)
{
    POLY(unsigned int);
}

/* To test these functions: the lightning (6 yellow gems) */

#define BRESENHAM(cur1,cur2,inc1,inc2,d1,d2) do { \
    int dp = d2 << 1; \
    int dpu = dp - (d1 << 1); \
    int p = dp - d1; \
    \
    while(d1--) \
    { \
        pDestination[cur1 + cur2 - inc2] = _pEclair->col2; \
        pDestination[cur1 + cur2] = _pEclair->col1; \
        pDestination[cur1 + cur2 + inc2] = _pEclair->col2; \
        cur1 += inc1; \
        if (p > 0) \
        { \
            cur2 += inc2; \
            p += dpu; \
        } \
        else \
            p += dp; \
    } \
} while(0)

#define DRAW_ECLAIR(TYPE,CALLBACK) \
    TYPE *pDestination = (TYPE *)_pDestination; \
    \
    if(_iIter--) \
    { \
        int dx, dy, midx, midy, r, oldx, oldy; \
        \
        dx = (_pEclair->dx - _pEclair->sx) / 2; \
        dy = (_pEclair->dy - _pEclair->sy) / 2; \
        \
        /* Add a little random normal deviation */ \
        r = randomize_eclair(_pEclair) / (1 << 24); \
        midx = _pEclair->sx + dx + ((dy * r) / (1 << 8)); \
        midy = _pEclair->sy + dy - ((dx * r) / (1 << 8)); \
        \
        /* Recurse on both halves */ \
        oldx = _pEclair->sx; _pEclair->sx = midx; \
        oldy = _pEclair->sy; _pEclair->sy = midy; \
        CALLBACK(_pDestination, _pEclair, _iIter); \
        _pEclair->sx = oldx; \
        _pEclair->sy = oldy; \
        \
        oldx = _pEclair->dx; _pEclair->dx = midx; \
        oldy = _pEclair->dy; _pEclair->dy = midy; \
        CALLBACK(_pDestination, _pEclair, _iIter); \
        _pEclair->dx = oldx; \
        _pEclair->dy = oldy; \
    } \
    else \
    { \
        /* Draw a line using Bresenham */ \
        int dx = abs(_pEclair->dx - _pEclair->sx); \
        int dy = abs(_pEclair->dy - _pEclair->sy); \
        \
        int xcur = _pEclair->sx; \
        int ycur = _pEclair->sy * 512; \
        \
        int xinc = _pEclair->sx > _pEclair->dx ? -1 : 1; \
        int yinc = _pEclair->sy > _pEclair->dy ? -512 : 512; \
        \
        if(dx >= dy) \
            BRESENHAM(xcur,ycur,xinc,yinc,dx,dy); \
        else \
            BRESENHAM(ycur,xcur,yinc,xinc,dy,dx); \
    }

static int randomize_eclair(Eclair *_pEclair)
{
    int a = _pEclair->r1, b = _pEclair->r2, c = _pEclair->r3;

    a = (a << 13) | (a >> 3);
    a = a ^ c;
    b = b ^ c;
    b = (b >> 7) | (b << 9);
    c += 27;
    a = a ^ c;
    b = b ^ c;

    _pEclair->r1 = c; _pEclair->r2 = a; _pEclair->r3 = b;

    return c;
}

static void draw_eclair8_in(char *_pDestination, Eclair *_pEclair, int _iIter)
{
    DRAW_ECLAIR(unsigned char, draw_eclair8_in);
}

static void draw_eclair16_in(char *_pDestination, Eclair *_pEclair, int _iIter)
{
    DRAW_ECLAIR(unsigned short, draw_eclair16_in);
}

static void draw_eclair32_in(char *_pDestination, Eclair *_pEclair, int _iIter)
{
    DRAW_ECLAIR(unsigned int, draw_eclair32_in);
}

void draw_eclair8(char *_pDestination, Eclair *_pEclair, int _iIter)
{
    int a = _pEclair->r1, b = _pEclair->r2, c = _pEclair->r3;
    draw_eclair8_in(_pDestination, _pEclair, _iIter);
    _pEclair->r1 = a; _pEclair->r2 = b; _pEclair->r3 = c;
}

void draw_eclair16(char *_pDestination, Eclair *_pEclair, int _iIter)
{
    int a = _pEclair->r1, b = _pEclair->r2, c = _pEclair->r3;
    draw_eclair16_in(_pDestination, _pEclair, _iIter);
    _pEclair->r1 = a; _pEclair->r2 = b; _pEclair->r3 = c;
}

void draw_eclair24(char *_pDestination, Eclair *_pEclair, int _iIter)
{
    printf("draw_eclair24 not implemented\n");
}

void draw_eclair32(char *_pDestination, Eclair *_pEclair, int _iIter)
{
    int a = _pEclair->r1, b = _pEclair->r2, c = _pEclair->r3;
    draw_eclair32_in(_pDestination, _pEclair, _iIter);
    _pEclair->r1 = a; _pEclair->r2 = b; _pEclair->r3 = c;
}

/* To test these functions: the main menu, the game */

#define COPY2X_512x440(TYPE) \
    TYPE *src = (TYPE *)_src; \
    TYPE *dest = (TYPE *)_dest; \
    \
    for(int i = hauteur; i--;) \
    { \
        for(int j = 256 / 8; j--;) \
        { \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
            dest[0] = dest[1] = *src++; dest += 2; \
        } \
        src += 256; \
        dest += 640 + (640 - 512); \
    }

void _COPY2X8BITS_512x440(char *_src, char *_dest, unsigned int hauteur)
{
    COPY2X_512x440(unsigned char);
}

void _COPY2X16BITS_512x440(char *_src, char *_dest, unsigned int hauteur)
{
    COPY2X_512x440(unsigned short);
}

void _COPY2X24BITS_512x440(char *_src, char *_dest, unsigned int hauteur)
{
    printf("_COPY2X24BITS_512x440 not implemented\n");
}

void _COPY2X32BITS_512x440(char *_src, char *_dest, unsigned int hauteur)
{
    COPY2X_512x440(unsigned int);
}

/* To test these functions: the cursor in the "ORDER" menu and name input */

#define PUTRECT(TYPE) \
    TYPE *dest = (TYPE *)adresse + (512 * y + x); \
    int i; \
    \
    /* Top line */ \
    i = large; while(i--) *dest++ = coul; \
    \
    dest += 512 - large; \
    haute -= 2; \
    \
    /* Side lines */ \
    while(haute-- > 0) \
    { \
        dest[0] = dest[large-1] = coul; \
        dest += 512; \
    } \
    \
    /* Bottom line */ \
    i = large; while(i--) *dest++ = coul

void _PUTRECTANGLEVIDE8BITS(char *adresse, int x, int y,
                            int coul, int large, int haute)
{
    PUTRECT(unsigned char);
}

void _PUTRECTANGLEVIDE16BITS(char *adresse, int x, int y,
                             int coul, int large, int haute)
{
    PUTRECT(unsigned short);
}

void _PUTRECTANGLEVIDE24BITS(char *adresse, int x, int y,
                             int coul, int large, int haute)
{
    printf("_PUTRECTANGLEVIDE24BITS not implemented\n");
}

void _PUTRECTANGLEVIDE32BITS(char *adresse, int x, int y,
                             int coul, int large, int haute)
{
    PUTRECT(unsigned int);
}


//*****************************************************************************
// copyright (c) 1998-2003 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "assembler.hpp"
// created      : 1999-08-24
// updates      : 2003-07-01
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
#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

void                    conv8to16(char *_pSource, char *_pDestination, unsigned short *_pPalette16, unsigned int _iTaille);
void                    conv8to24(char *_pSource, char *_pDestination, unsigned int *_pPalette24, unsigned int _iTaille);
void                    conv8to32(char *_pSource, char *_pDestination, unsigned int *_pPalette32, unsigned int _iTaille);

void                    _putRectangle8bits(char *adresse, int x, int y, int coul, int large, int haute);
void                    _putRectangle16bits(char *adresse, int x, int y, int coul, int large, int haute);
void                    _putRectangle24bits(char *adresse, int x, int y, int coul, int large, int haute);
void                    _putRectangle32bits(char *adresse, int x, int y, int coul, int large, int haute);
void                    _putsprite3_8bits(char *_pSource, char *_pDestination, char *_pCompression, unsigned int _iTaille);
void                    _putsprite3_16bits(char *_pSource, char *_pDestination, char *_pCompression, unsigned int _iTaille);
void                    _putsprite3_24bits(char *_pSource, char *_pDestination, char *_pCompression, unsigned int _iTaille);
void                    _putsprite3_32bits(char *_pSource, char *_pDestination, char *_pCompression, unsigned int _iTaille);
void                    _putsprcolor_8bits(unsigned int _iColor, char *_pDestination, char *_pCompression, unsigned int _iTaille);
void                    _putsprcolor_16bits(unsigned int _iColor, char *_pDestination, char *_pCompression, unsigned int _iTaille);
void                    _putsprcolor_24bits(unsigned int _iColor, char *_pDestination, char *_pCompression, unsigned int _iTaille);
void                    _putsprcolor_32bits(unsigned int _iColor, char *_pDestination, char *_pCompression, unsigned int _iTaille);
void                    _asmCopyFonte8Bits(unsigned char *_pSource, unsigned char *_pDestination, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffsetS, unsigned int _iOffsetD);
void                    _asmCopyFonte16Bits(unsigned char *_pSource, unsigned char *_pDestination, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffsetS, unsigned int _iOffsetD);
void                    _asmCopyFonte24Bits(unsigned char *_pSource, unsigned char *_pDestination, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffsetS, unsigned int _iOffsetD);
void                    _asmCopyFonte32Bits(unsigned char *_pSource, unsigned char *_pDestination, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffsetS, unsigned int _iOffsetD);
void                    _conv8_16(char *_pSource, char *_pDestination, unsigned short *_pPalette16, unsigned int _iTaille);
void                    _conv8_24(char *_pSource, char *_pDestination, unsigned int *_pPalette24, unsigned int _iTaille);
void                    _conv8_32(char *_pSource, char *_pDestination, unsigned int *_pPalette32, unsigned int _iTaille);

extern "C"
{ void                    _type_routine_gfx(int *adresse);
  void                    conv8_16(char *_pSource, char *_pDestination, unsigned short *_pPalette16, unsigned int _iTaille);
  void                    conv8_24(char *_pSource, char *_pDestination, unsigned int *_pPalette24, unsigned int _iTaille);
  void                    conv8_32(char *_pSource, char *_pDestination, unsigned int *_pPalette32, unsigned int _iTaille);
  unsigned int            pixel8_16(unsigned int _iIndex, unsigned short *_pPalette);
  unsigned int            pixel8_24(unsigned int _iIndex, char *_pPalette);
  unsigned int            pixel8_32(unsigned int _iIndex, char *_pPalette);
  void                    copie4octets(char *, char *, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffset, unsigned int _iOffset2);
  void                    copie8octets(char *, char *, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffset);
  void                    raz_ecran(char *_pEcran, unsigned int _iLargeur, unsigned int _iHauteur, unsigned int _iOffset);
  void                    convertit8_16(char *_pSource, char *_pDestination, unsigned char *_pPalette, unsigned int _iTaille);
  void                    convertitPalette24_16(unsigned char *pal, unsigned short *pal16);
  void                    convertitPalette24_15(unsigned char *pal, unsigned short *pal16);
  void                    _COPY2X8BITS(char *, char *, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffset, unsigned int _iOffset2);
  void                    _COPY2X16BITS(char *, char *, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffset, unsigned int _iOffset2);
  void                    _COPY2X24BITS(char *, char *, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffset, unsigned int _iOffset2);
  void                    _COPY2X32BITS(char *, char *, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffset, unsigned int _iOffset2);
  void                    _ASMCOPYFONTE8BITS(unsigned char *_pSource, unsigned char *_pDestination, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffsetS, unsigned int _iOffsetD);
  void                    _ASMCOPYFONTE16BITS(unsigned char *_pSource, unsigned char *_pDestination, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffsetS, unsigned int _iOffsetD);
  void                    _ASMCOPYFONTE24BITS(unsigned char *_pSource, unsigned char *_pDestination, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffsetS, unsigned int _iOffsetD);
  void                    _ASMCOPYFONTE32BITS(unsigned char *_pSource, unsigned char *_pDestination, unsigned int _iLarge, unsigned int _iHaut, unsigned int _iOffsetS, unsigned int _iOffsetD);
  void                    putsprite3_8bits(char *_pSource, char *_pDestination, char *_pCompression, unsigned int _iTaille);
  void                    putsprite3_16bits(char *_pSource, char *_pDestination, char *_pCompression, unsigned int _iTaille);
  void                    putsprite3_24bits(char *_pSource, char *_pDestination, char *_pCompression, unsigned int _iTaille);
  void                    putsprite3_32bits(char *_pSource, char *_pDestination, char *_pCompression, unsigned int _iTaille);
  void                    putsprcolor_8bits(unsigned int _iColor, char *_pDestination, char *_pCompression, unsigned int _iTaille);
  void                    putsprcolor_16bits(unsigned int _iColor, char *_pDestination, char *_pCompression, unsigned int _iTaille);
  void                    putsprcolor_24bits(unsigned int _iColor, char *_pDestination, char *_pCompression, unsigned int _iTaille);
  void                    putsprcolor_32bits(unsigned int _iColor, char *_pDestination, char *_pCompression, unsigned int _iTaille);
  void                    poly8bits(char *_pDestination, int _iNombrePixel, int _iCouleur);
  void                    poly16bits(char *_pDestination, int _iNombrePixel, int _iCouleur);
  void                    poly24bits(char *_pDestination, int _iNombrePixel, int _iCouleur);
  void                    poly32bits(char *_pDestination, int _iNombrePixel, int _iCouleur);
  void                    draw_eclair8(char *, Eclair *, int);
  void                    draw_eclair16(char *, Eclair *, int);
  void                    draw_eclair24(char *, Eclair *, int);
  void                    draw_eclair32(char *, Eclair *, int);
  void                    _COPY2X8BITS_512x440(char *, char *, unsigned int);
  void                    _COPY2X16BITS_512x440(char *, char *, unsigned int);
  void                    _COPY2X24BITS_512x440(char *, char *, unsigned int);
  void                    _COPY2X32BITS_512x440(char *, char *, unsigned int);
  void                    _PUTRECTANGLEVIDE8BITS(char *adresse, int, int, int, int, int);
  void                    _PUTRECTANGLEVIDE16BITS(char *adresse, int, int, int, int, int);
  void                    _PUTRECTANGLEVIDE24BITS(char *adresse, int, int, int, int, int);
  void                    _PUTRECTANGLEVIDE32BITS(char *adresse, int, int, int, int, int);
}
#endif


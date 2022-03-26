//*****************************************************************************
// copyright (c) 2003 Samuel Hocevar <sam@zoy.org>
//-----------------------------------------------------------------------------
// file         : "gfxroutines.hpp"
// created      : 2003-04-15
// updates      : 2003-04-15
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

#ifndef __PLAIN_C_H__
#define __PLAIN_C_H__

extern "C" {

void conv8_16(char *, char *, unsigned short *, unsigned int);
void conv8_24(char *, char *, unsigned int *, unsigned int);
void conv8_32(char *, char *, unsigned int *, unsigned int);

void copie4octets(char *, char *, unsigned int, unsigned int, unsigned int, unsigned int);
void copie8octets(char *, char *, unsigned int, unsigned int, unsigned int);
void raz_ecran(char *_pEcran, unsigned int, unsigned int, unsigned int);

void convertitPalette24_16(unsigned char *, unsigned short *);
void convertitPalette24_15(unsigned char *, unsigned short *);

void _COPY2X8BITS(char *, char *, unsigned int, unsigned int, unsigned int, unsigned int);
void _COPY2X16BITS(char *, char *, unsigned int, unsigned int, unsigned int, unsigned int);
void _COPY2X24BITS(char *, char *, unsigned int, unsigned int, unsigned int, unsigned int);
void _COPY2X32BITS(char *, char *, unsigned int, unsigned int, unsigned int, unsigned int);

void _ASMCOPYFONTE8BITS(unsigned char *, unsigned char *, unsigned int, unsigned int, unsigned int, unsigned int);
void _ASMCOPYFONTE16BITS(unsigned char *, unsigned char *, unsigned int, unsigned int, unsigned int, unsigned int);
void _ASMCOPYFONTE24BITS(unsigned char *, unsigned char *, unsigned int, unsigned int, unsigned int, unsigned int);
void _ASMCOPYFONTE32BITS(unsigned char *, unsigned char *, unsigned int, unsigned int, unsigned int, unsigned int);

void putsprite3_8bits(char *, char *, char *, unsigned int);
void putsprite3_16bits(char *, char *, char *, unsigned int);
void putsprite3_24bits(char *, char *, char *, unsigned int);
void putsprite3_32bits(char *, char *, char *, unsigned int);

void putsprcolor_8bits(unsigned int, char *, char *, unsigned int);
void putsprcolor_16bits(unsigned int, char *, char *, unsigned int);
void putsprcolor_24bits(unsigned int, char *, char *, unsigned int);
void putsprcolor_32bits(unsigned int, char *, char *, unsigned int);

void poly8bits(char *, int, int);
void poly16bits(char *, int, int);
void poly24bits(char *, int, int);
void poly32bits(char *, int, int);

void draw_eclair8(char *, Eclair *, int);
void draw_eclair16(char *, Eclair *, int);
void draw_eclair24(char *, Eclair *, int);
void draw_eclair32(char *, Eclair *, int);

void _COPY2X8BITS_512x440(char *, char *, unsigned int);
void _COPY2X16BITS_512x440(char *, char *, unsigned int);
void _COPY2X24BITS_512x440(char *, char *, unsigned int);
void _COPY2X32BITS_512x440(char *, char *, unsigned int);

void _PUTRECTANGLEVIDE8BITS(char *, int, int, int, int, int);
void _PUTRECTANGLEVIDE16BITS(char *, int, int, int, int, int);
void _PUTRECTANGLEVIDE24BITS(char *, int, int, int, int, int);
void _PUTRECTANGLEVIDE32BITS(char *, int, int, int, int, int);

} /* extern "C" */

#endif


//*****************************************************************************
// copyright (c) 1998-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "keymap.hpp"
// created      : ?
// updates      : 2002-08-11
//------------------------------------------------------------------------------
// functions    : - void Keys_Down(unsigned int wParam)
//                - void Keys_Up(unsigend int wParam)
//                - keytouch(unsigned int _iCodeTouche, unsigned int i)
//                - void Clear_Keyboard()
//------------------------------------------------------------------------------
//                CTRL+'Q' : force "GameOver"
//                CTRL+'A' : display "ABOUT"
//                CTRL+'S' : swicth no-music / music
//                'P'      : swicth pause on/off
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
// XFree86 keys codes for AZERTY french keyboard
// a=24 b=56 c=54 d=40 e=26 f=41 g=42 h=43 i=31 j=44 k=45 l=46 m=47 n=57 o=32
// p=33 q=38 r=27 s=39 t=28 u=30 v=55 w=52 x=53 y=29 z=25
// 0=19  1=10 2=11 3=12 4=13 5=14 6=15 7=16 8=17 9=18 
// numeric : 0=90 1=87 2=88 3=89 4=83 5=84 6=85 7=79 8=80 9=81
//------------------------------------------------------------------------------
#include "powermanga.hpp"
extern short            touch[500];                            //tableau des flags de toutes les touches clavier
#ifdef XFREE_TLK
#include <X11/keysym.h>
#include <X11/keysymdef.h>
//..............................................................................
extern unsigned int     iCodeTouche;                           // code touche pressee
extern _etbfonte1      *pEtbfonte1;

//..............................................................................
void                    keytouch(unsigned int _iCodeTouche, unsigned int i);

//------------------------------------------------------------------------------
// touche enfoncee / key pressed
//------------------------------------------------------------------------------
void Keys_Down(unsigned int _iCodeTouche)
{
  _chaine::setCodeTouchePressee(_iCodeTouche);                 //sauve le code de la touche pressee 
  iCodeTouche = _iCodeTouche;                                  //sauve le code touche
  keytouch(_iCodeTouche, 1);
}

//------------------------------------------------------------------------------
// touche relachee / key released
//------------------------------------------------------------------------------
void Keys_Up(unsigned int _iCodeTouche)
{
  _chaine::setCodeToucheRelachee(_iCodeTouche);                // sauve le code ASCII de la touche relachee
  if(iCodeTouche == _iCodeTouche) iCodeTouche = 0;             // remet a zero le code touche
  keytouch(_iCodeTouche, 0);
}

//------------------------------------------------------------------------------
// touche enfoncee ou relachee
//  i=0 : relachee/released  ou/or i=1 : enfoncee/pressed
//------------------------------------------------------------------------------
void keytouch(unsigned int _iCodeTouche, unsigned int i)
{
  if(_iCodeTouche >= 'a' && _iCodeTouche <= 'z')
  { _iCodeTouche = _iCodeTouche - 32;
  }
  switch (_iCodeTouche)
  { case XK_Escape    : touch[0]   = i; break;                 //ESC (ASCII=27)
    case XK_Control_L : touch[88]  = i; break;
    case XK_Control_R : touch[88]  = i; break;
    case XK_Return    : touch[50]  = i; break;                 //RETURN (ASCII=13)
    case XK_Pause     : touch[150] = i; break;
    case XK_Shift_L   : touch[74]  = i; break;
    case XK_Shift_R   : touch[74]  = i; break;
    case XK_F1        : touch[1]   = i; break;
    case XK_F2        : touch[2]   = i; break;
    case XK_F3        : touch[3]   = i; break;
    case XK_F4        : touch[4]   = i; break;
    case XK_F5        : touch[5]   = i; break;
    case XK_F6        : touch[6]   = i; break;
    case XK_F7        : touch[7]   = i; break;
    case XK_F8        : touch[8]   = i; break;
    case XK_F9        : touch[9]   = i; break;
    case XK_F10       : touch[10]  = i; break;
    case XK_F11       : touch[11]  = i; break;
    case XK_F12       : touch[12]  = i; break;
    case XK_Insert    : touch[13]  = i; break;
    case XK_space     : touch[90]  = i; break;
    case XK_Left      : touch[71]  = i; break;
    case XK_Right     : touch[73]  = i; break;
    case XK_Up        : touch[55]  = i; break;
    case XK_Down      : touch[87]  = i; break;
    case 'A'          : touch[38]  = i; break;                 //A CTRL+A=ABOUT
    case 'V'          : touch[79]  = i; break;                 //V
    case 'B'          : touch[80]  = i; break;                 //B
    case 'P'          : touch[47]  = i; break;                 //P P=PAUSE
    case 'Q'          : touch[58]  = i; break;                 //Q CTRL+Q gameover
    case 'S'          : touch[60]  = i; break;                 //S CTRL+S coupe la musique
  }
}
#endif
//------------------------------------------------------------------------------
// RAZ table clavier
//------------------------------------------------------------------------------
void Clear_Keyboard()
{
  for(int i = 0; i < 200; i++)
    touch[i] = 0;
}

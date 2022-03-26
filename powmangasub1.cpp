//*****************************************************************************
// copyright (c) 1998-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "powmangasub1.cpp"
// created      : ?
// updates      : 2002-08-02
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
//------------------------------------------------------------------------------
//routine qui calcul un angle a partir de deux points
//------------------------------------------------------------------------------
float Calc_Angle_Tir(short pxs, short pys, short pxd, short pyd)
{
  double                  longeur, resultat, dx, dy, angle;
  dx = pxd - pxs;
  dy = pyd - pys;                                              //calcul la distance entre l'origine et la destination.
  resultat = (dx * dx) + (dy * dy);
  longeur = sqrt(resultat);
  if(longeur == 0)
    return (0.0);                                              //eviter de generer des erreurs
  resultat = dx / longeur;
  angle = acos(resultat);
  resultat = dy / longeur;
  if(asin(resultat) < 0)
    angle = -angle;
  return (float)angle;
}

//------------------------------------------------------------------------------
// Routine qui calcul un nouvel angle par rapport à un autre avec une certaine agilité.
//------------------------------------------------------------------------------
float Calcul_New_Angle(float old_angle, float new_angle, float agilite)
{
  float                   delta_angle;
  delta_angle = old_angle - new_angle;
  if(((delta_angle < 0) && (delta_angle > -3.14)) || (delta_angle > 3.14))
  {
    old_angle = old_angle + agilite;
    if(old_angle > 3.14)
      old_angle = old_angle - 6.28f;
  }
  if(((delta_angle > 0) && (delta_angle < 3.14)) || (delta_angle < -3.14))
  {
    old_angle = old_angle - agilite;
    if(old_angle < -3.14)
      old_angle = old_angle + 6.28f;
  }

  return (old_angle);
}

//------------------------------------------------------------------------------
// Retourne -1, 0 ou 1 suivant la valeur passée en argument.
//------------------------------------------------------------------------------
short sign(float val)
{
  if(val == 0)
    return 0;
  if(val < 0)
    return -1;
  return 1;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void draw_carre(char *screen, short x, short y, short l, short h, unsigned char coul)
{
  int                     i;

  for(i = 0; i < l; i++)
  {
    putpixel(screen, x + i, y, (int)coul);
    putpixel(screen, x + i, y + (h - 1), (int)coul);
  }
  for(i = 0; i < h; i++)
  {
    putpixel(screen, x, y + i, (int)coul);
    putpixel(screen, x + (l - 1), y + i, (int)coul);
  }
}

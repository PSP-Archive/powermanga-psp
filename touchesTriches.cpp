//*****************************************************************************
// copyright (c) 1998-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "touchesTriches.cpp"
// created      : ?
// updates      : 2002-08-02
//-----------------------------------------------------------------------------
// descriptions : touches speciales pour les developpeurs
// [1]          : augmente tir avant
// [2]          : augmente tir gauche
// [3]          : augmente tir droite
// [4]          : augmente tir arriere
// [5]          : augmente tir special avant
// [6]          : augmente tir special gauche
// [7]          : augmente tir special droite
// [8]          : augmente tir special arriere
// [9]          : ajoute un vaisseau ennemi special
// [0]          : ajoute un vaisseau ennemi special
// [SHIFT]+[1]  : tir avant au minimum
// [SHIFT]+[2]  : tir gauche au minimum
// [SHIFT]+[3]  : tir droite au minimum
// [SHIFT]+[4]  : tir arriere au minimum
// [SHIFT]+[5]  : tir special avant au minimum
// [SHIFT]+[6]  : tir special gauche au minimum
// [SHIFT]+[7]  : tir special droite au minimum
// [SHIFT]+[8]  : tir special arriere au minimum
// [SHIFT]+[9]  : ajoute un vaisseau ennemi special
// [SHIFT]+[0]  : ajoute un vaisseau ennemi special
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
#ifdef DEVELOPPEMENT
//..............................................................................
extern vaisseau         vj[NBR_MAX_J];
extern short            touch[500];                            //tableau des flags de toutes les touches clavier
extern short            nbr_nmis;                              //nombre d'enemis actuellement geres.

short                   touch17, touch18, touch19, touch20, touch21, touch22,
                        touch23, touch24, touch25, touch26, touch50, touch57;
//..............................................................................

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void touchesTriches()
{
  // Touche [SHIFT] droite relachee --------------------------------------------
  if(!touch[74])
  {                                                            // [1]
    if(touch[17] && !touch17)
    {
      vj[J1].power_avant++;
      if(vj[J1].power_avant > 5)
        vj[J1].power_avant = 5;
    }
    touch17 = touch[17];
    // [2]
    if(touch[18] && !touch18)
    {
      vj[J1].power_gauche++;
      if(vj[J1].power_gauche > 5)
        vj[J1].power_gauche = 5;
    }
    touch18 = touch[18];
    // [3]
    if(touch[19] && !touch19)
    {
      vj[J1].power_droite++;
      if(vj[J1].power_droite > 5)
        vj[J1].power_droite = 5;
    }
    touch19 = touch[19];
    // [4]
    if(touch[20] && !touch20)
    {
      vj[J1].power_arriere++;
      if(vj[J1].power_arriere > 5)
        vj[J1].power_arriere = 5;
    }
    touch20 = touch[20];
    // [5]
    if(touch[21] && !touch21)
    {
      vj[J1].power_spec_avant++;
      if(vj[J1].power_spec_avant > 5)
        vj[J1].power_spec_avant = 5;
    }
    touch21 = touch[21];
    // [6]
    if(touch[22] && !touch22)
    {
      vj[J1].power_spec_gauche++;
      if(vj[J1].power_spec_gauche > 5)
        vj[J1].power_spec_gauche = 5;
    }
    touch22 = touch[22];
    // [7]
    if(touch[23] && !touch23)
    {
      vj[J1].power_spec_droite++;
      if(vj[J1].power_spec_droite > 5)
        vj[J1].power_spec_droite = 5;
    }
    touch23 = touch[23];
    // [8]
    if(touch[24] && !touch24)
    {
      vj[J1].power_spec_arriere++;
      if(vj[J1].power_spec_arriere > 5)
        vj[J1].power_spec_arriere = 5;
    }
    touch24 = touch[24];
    // [9]
    if(touch[25] && !touch25)
    {
      if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
        Add_Ve_Special(36);
    }
    touch25 = touch[25];
    // [0]
    if(touch[26] && !touch26)
    {
      if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
        Add_Ve_Special(37);
    }
    touch26 = touch[26];
  }
  // Touche [SHIFT] droite enfoncee --------------------------------------------
  else
  {
    // [SHIFT]+[1]
    if(touch[17] && !touch17)
    {
      vj[J1].power_avant--;
      if(vj[J1].power_avant < 0)
        vj[J1].power_avant = 0;
    }
    touch17 = touch[17];
    // [SHIFT]+[2]
    if(touch[18] && !touch18)
    {
      vj[J1].power_gauche--;
      if(vj[J1].power_gauche < 0)
        vj[J1].power_gauche = 0;
    }
    touch18 = touch[18];
    // [SHIFT]+[3]
    if(touch[19] && !touch19)
    {
      vj[J1].power_droite--;
      if(vj[J1].power_droite < 0)
        vj[J1].power_droite = 0;
    }
    touch19 = touch[19];
    // [SHIFT]+[4]
    if(touch[20] && !touch20)
    {
      vj[J1].power_arriere--;
      if(vj[J1].power_arriere < 0)
        vj[J1].power_arriere = 0;
    }
    touch20 = touch[20];
    // [SHIFT]+[5]
    if(touch[21] && !touch21)
    {
      vj[J1].power_spec_avant--;
      if(vj[J1].power_spec_avant < 0)
        vj[J1].power_spec_avant = 0;
    }
    touch21 = touch[21];
    // [SHIFT]+[6]
    if(touch[22] && !touch22)
    {
      vj[J1].power_spec_gauche--;
      if(vj[J1].power_spec_gauche < 0)
        vj[J1].power_spec_gauche = 0;
    }
    touch22 = touch[22];
    // [SHIFT]+[7]
    if(touch[23] && !touch23)
    {
      vj[J1].power_spec_droite--;
      if(vj[J1].power_spec_droite < 0)
        vj[J1].power_spec_droite = 0;
    }
    touch23 = touch[23];
    // [SHIFT]+[8]
    if(touch[24] && !touch24)
    {
      vj[J1].power_spec_arriere--;
      if(vj[J1].power_spec_arriere < 0)
        vj[J1].power_spec_arriere = 0;
    }
    touch24 = touch[24];
    // [SHIFT]+[9]
    if(touch[25] && !touch25)
    {
      if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
        Add_Ve_Special(18);
    }
    touch25 = touch[25];
    // [SHIFT]+[0]
    if(touch[26] && !touch26)
    {
      if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
        Add_Ve_Special(34);
    }
    touch26 = touch[26];
  }
}
#endif

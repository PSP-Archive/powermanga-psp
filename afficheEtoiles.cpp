//*****************************************************************************
// copyright (c) 1998-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "afficheEtoiles.cpp"
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
//*****************************************************************************
#include "linux.hpp"
#include "powermanga.hpp"
//..............................................................................
extern star             stars[NBR_OF_STARS];                    //tableau du fond etoiles defilant verticalement 
extern image            star_field[TYPE_OF_STARS][NBR_ETOILES]; //tableau des images des etoiles 
extern bool             aff_star_field;                         //1=affiche le champ d'etoile
extern float            vit_star_field;                         //vitesse du champs d'etoiles

//..............................................................................
void afficheEtoiles()
{
  unsigned int            i;

  if(aff_star_field)
  {
    if(vit_star_field >= 0.0)
    {
      for(i = 0; i < NBR_OF_STARS; i++)
      {
        stars[i].coor_y += stars[i].vitesse * (vit_star_field);
        if((stars[i].coor_y) >= HAUT_ECR_RAM_TOT - BANDE_DE_CLIP)
        {
          stars[i].coor_x = (float)(rand() % LARG_ECR_RAM + BANDE_DE_CLIP);
          stars[i].coor_y = (float)(BANDE_DE_CLIP - stars[i].img->haut);

        }
        stars[i].nbr_vbl_act++;
        if(!(stars[i].nbr_vbl_act &= (stars[i].nbr_tot_vbl - 1)))
          stars[i].img =
            (image *) & star_field[stars[i].type][rand() % NBR_ETOILES];
        putsprite3(stars[i].img, (unsigned int)stars[i].coor_x, (unsigned int)stars[i].coor_y);
      }
    }
    else
    {
      for(i = 0; i < NBR_OF_STARS; i++)
      {
        stars[i].coor_y += stars[i].vitesse * (vit_star_field);
        if((stars[i].coor_y + stars[i].img->haut) <= BANDE_DE_CLIP)
        {
          stars[i].coor_x = (float)(rand() % LARG_ECR_RAM + BANDE_DE_CLIP);
          stars[i].coor_y = (float)(HAUT_ECR_RAM_TOT - BANDE_DE_CLIP);
        }
        stars[i].nbr_vbl_act++;
        if(!(stars[i].nbr_vbl_act &= (stars[i].nbr_tot_vbl - 1)))
          stars[i].img =
            (image *) & star_field[stars[i].type][rand() % NBR_ETOILES];
        putsprite3(stars[i].img, (unsigned int)stars[i].coor_x, (unsigned int)stars[i].coor_y);
      }
    }
  }
}

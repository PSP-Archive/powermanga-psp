//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "initialiseGameOver.cpp"
// created      : ?
// updates      : 2005-01-08
// id		: $Id: initialiseGameOver.cpp,v 1.1.1.1 2005/01/25 05:53:24 gurumeditation Exp $
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
#include "linux.hpp"
#include "score.hpp"
#include "powermanga.hpp"
//..............................................................................
extern vaisseau         vj[NBR_MAX_J];                         //structure des vaisseaux des joueurs.
extern bool             aff_game_over;                         //1=affichage du "game over"
//..............................................................................
void _initialiseGameOver()
{
  unsigned int i;

  _score::setAction(1);                                        //trie les scores & prepare affichage

  Add_Serie_Of_Explosion(-1);                                  //ajoute une serie d'explosions (-1=vaisseau du joueur)

  Add_Eclats(vj[J1].spr.coor_x + vj[J1].spr.img[vj[J1].spr.img_act]->x_cg - 8, vj[J1].spr.coor_y + vj[J1].spr.img[vj[J1].spr.img_act]->y_cg - 8, 0.5, 5, 0, 3); //ajoute un eclat a la liste des explosions
  Add_Eclats(vj[J1].spr.coor_x + vj[J1].spr.img[vj[J1].spr.img_act]->x_cg - 8, vj[J1].spr.coor_y + vj[J1].spr.img[vj[J1].spr.img_act]->y_cg - 8, 1.0, 5, 0, 2);
  Add_Eclats(vj[J1].spr.coor_x + vj[J1].spr.img[vj[J1].spr.img_act]->x_cg - 8, vj[J1].spr.coor_y + vj[J1].spr.img[vj[J1].spr.img_act]->y_cg - 8, 1.5, 5, 0, 1);


  aff_game_over = 1;                                           //1=affichage du "game over"
  vj[J1].visible = 0;                                          //0=vaisseau du joueur est invisible
  for(i = 0; i < 11; i++)
    Init_Anim_Option(i, 1);                                    //ferme toutes les options
  vj[J1].nbr_bonus = 0;                                        //le joueur n'a plus de bonus (plus de clignotement sur le tableau des options)
  Clear_Keyboard();                                            //raz les flags claviers

#ifdef USE_SDLMIXER
  seal_JoueModule(0);                                          //joue musique de l'intro
#endif
}

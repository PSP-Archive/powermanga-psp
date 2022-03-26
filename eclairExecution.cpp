//*****************************************************************************
// copyright (c) 1998-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "eclairExecution.cpp"
// created      : ?
// updates      : 2002-08-02
//-----------------------------------------------------------------------------
// descriptions : lightning handling
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
extern char            *ecran_ram;                             //adresse ecran logique.
extern vaisseau         vj[NBR_MAX_J];
extern short            touch[500];                            //tableau des flags de toutes les touches clavier
extern short            nbr_nmis;                              //nombre d'enemis actuellement geres.
extern int              tmp_apparition_vj;                     //vaisseau joueur : compteur temps apparition 
extern bool             disparition_vj;                        //vaisseau joueur : 1=disparition 
extern short            mouse_b;
extern unsigned int     iBoutonFeu;                            //1=bouton de la manette de jeu enfoncee
extern short            nbr_tirs;                              //nombre de tirs actuellement geres
extern int              liste_ve[MAX_NMI_ON_SCREEN];           //liste de tous les vaisseaux ennemis a gerer
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //structure des vaisseaux ennemis
extern bool             eclair_actif;                          //1=joueur peut utiliser l'eclair
extern Eclair           eclair1;
extern int              etat_menu;                             //MENU_OFF(false)=menu inactif ; MENU_ON(true)=menu actif
extern bool             player_pause;                          //true=pause en cours
extern bool             aff_game_over;                         //true=affichage du game over en cours
extern unsigned char    color_eclair[2 + 1];                   //tableau des couleurs utilisees lors de la disparition d'un vaisseau
extern short            nbr_exps;                              // nombre d'explosions actuellement gerees
extern int              player_score;
extern int              bonusx;                                //multiplicateur de points (*1, *2, *4)
extern int              bool_et;
extern bool             bar_nrj_guardian;

//------------------------------------------------------------------------------
/*typedef struct Eclair
{	int 	sx ;		//0
	int		sy ;		//4
	int 	dx ;		//8
	int		dy  ;		//12
	int		col1 ;	//16
	int		col2 ;	//20
	int 	r1 ;		//24
	int		r2 ;		//28
	int		r3 ;		//32
} Eclair;
*/
void eclairExecution()
{
  if(nbr_nmis && (mouse_b == 1 || touch[90] || iBoutonFeu) && eclair_actif)
    //if (nbr_nmis && (mouse_b==1 || touch[90] || iBoutonFeu))
  {
    int                     val_tmp = liste_ve[0];
    int                     centre_x_ve =
      (int)(ve[val_tmp].spr.coor_x +
            ve[val_tmp].spr.img[ve[val_tmp].spr.img_act]->x_cg);
    int                     centre_y_ve =
      (int)(ve[val_tmp].spr.coor_y +
            ve[val_tmp].spr.img[ve[val_tmp].spr.img_act]->y_cg);
    // Test si l'enemi est en vie, qu'il n'y ait pas la pause ni de game over.
    if(!ve[val_tmp].dead && !aff_game_over && !player_pause
       && etat_menu == MENU_OFF && centre_x_ve > BANDE_DE_CLIP
       && centre_x_ve < (BANDE_DE_CLIP + LARG_ECR_RAM)
       && centre_y_ve > BANDE_DE_CLIP
       && centre_y_ve < (BANDE_DE_CLIP + HAUT_ECR_RAM))
    {
      eclair1.sx =
        (int)(vj[J1].spr.coor_x + vj[J1].spr.img[vj[J1].spr.img_act]->x_cg);
      eclair1.sy =
        (int)(vj[J1].spr.coor_y + vj[J1].spr.img[vj[J1].spr.img_act]->y_cg);
      eclair1.dx = centre_x_ve;
      eclair1.dy = centre_y_ve;
      eclair1.col1 = color_eclair[0];
      eclair1.col2 = color_eclair[1];
      if(bool_et == 0)
      {
        eclair1.r1 = (int)rand() * (int)rand();                //value random
        eclair1.r2 = (int)rand() * (int)rand();                //value random
        eclair1.r3 = (int)rand() * (int)rand();                //value random
        ve[val_tmp].spr.etat_dommages--;                       //decrease enemy's damage
        if(ve[val_tmp].deplacement == GARDIEN)                 //enemy is a big-boss ?
          bar_nrj_guardian = 1;                                //yes, update the energy bar-line
        ve[val_tmp].affich_white = 1;                          //1=display white mask of sprite
      }
      bool_et = (bool_et + 1) & 3;
      draw_eclair(ecran_ram, &eclair1, 4);                     //display lightning "gfxbase.cpp"
      if(ve[val_tmp].spr.etat_dommages <= 0)                   //enemy dead ?
      {
        if(ve[val_tmp].type >= BIGMETEOR)                      //enmey is a meteor or a big-boss ?
        {                                                      //Gene_Bonus_Malus_Meteor(ve[val_tmp].spr.coor_x+ve[val_tmp].spr.img[ve[val_tmp].spr.img_act]->x_cg - 8, ve[val_tmp].spr.coor_y+ve[val_tmp].spr.img[ve[val_tmp].spr.img_act]->y_cg - 8);
          Gene_Bonus_Malus_Meteor(&ve[val_tmp]);
          Add_Eclats(ve[val_tmp].spr.coor_x +
                     ve[val_tmp].spr.img[ve[val_tmp].spr.img_act]->x_cg - 8,
                     ve[val_tmp].spr.coor_y +
                     ve[val_tmp].spr.img[ve[val_tmp].spr.img_act]->y_cg - 8,
                     1.0, 5, 0, 2);
          Add_Serie_Of_Explosion(val_tmp);                     //ajoute explosions
        }
        else                                                   //is a "grid" or "curve" phase
        {                                                      //Gene_Bonus_Malus(ve[val_tmp].spr.coor_x, ve[val_tmp].spr.coor_y);
          Gene_Bonus_Malus(&ve[val_tmp]);
          if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
            Add_Explosion(ve[val_tmp].spr.coor_x, ve[val_tmp].spr.coor_y,
                          0.25, ve[val_tmp].type, 0);
          Add_Eclats(ve[val_tmp].spr.coor_x +
                     ve[val_tmp].spr.img[ve[val_tmp].spr.img_act]->x_cg - 8,
                     ve[val_tmp].spr.coor_y +
                     ve[val_tmp].spr.img[ve[val_tmp].spr.img_act]->y_cg - 8,
                     1.0, 3, 0, 1);
        }
        player_score += ve[val_tmp].spr.pow_of_dest << 2 << bonusx;     //increase player's score
        Clignote_Nmi_Avant_Disparition(val_tmp);
      }
    }
    else
    {
      bool_et = 0;
    }
  }
}


//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "vaisseau.cpp"
// created      : ?
// updates      : 2005-01-10
// id		: $Id: vaisseau.cpp,v 1.1.1.1 2005/01/25 05:55:32 gurumeditation Exp $
//-----------------------------------------------------------------------------
// functions   : void  vaisseau_restaure()
//               void  vaisseau_invincibilite()
//               void    boule_execution()
//               void    canon_execution()       
//               void Fill_Struct_Player_One()
//               void  Apparition_Vaisseau()
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
extern short            nbr_boules;                            //nombre de boules gerees
extern bool             player_pause;                          //1=pause en cours
extern str_boule        bou[MAX_BOULES_ON_SCREEN];             //structure des boules
extern image            boule[MAX_TYP_OF_BOULES][NBR_IMAGES_BOULES];    //images des boules
extern int              cmpt_boules;                           //compteur des "boule" dans les boucles
extern int              etat_menu;                             //MENU_OFF(0)=menu inactif ; MENU_ON(true)=menu actif
extern vaisseau         vj[NBR_MAX_J];                         //declaration de la structure des vaisseaux des joueurs.
extern image            vj1[NBR_MAX_VAIS_J][NBR_MAX_IMG_VAIS_J];        //structure des images des vaisseaux du joueur
extern tir              tirs[NBR_TIRS_MAX];                    //tableau de tous les objets tirs
extern int              cmpt_canons;                           //compteur des "can" dans les boucles
extern short            nbr_canons;                            //nombre de "can" actuellement geres
extern int              liste_can[MAX_CANONS_ON_SCREEN];       //liste de touts les "can" geres
extern str_canon        can[MAX_CANONS_ON_SCREEN];             //structure des "can"
extern int              new_tir;                               //indice sur un nouvel objet "tirs"
extern short            nbr_tirs;                              //nombre de "tirs" actuellement geres
extern char            *ecran_ram;                             //adresse ecran logique.
extern int              liste_bou[MAX_BOULES_ON_SCREEN];       //liste de tous les "bou" geres 
extern short            cercle_px[];
extern short            cercle_py[];
extern image            fire[MAX_TYP_OF_TIR][NBR_IMAGES_TIR];  //image des tirs
extern unsigned char    coulor[12];                            //couleurs choisies par l'utilisateur
extern short            nbr_exps;                              //nombre d'explosions actuellement gerees
extern bool             bar_nrj_player;
extern bool             aff_game_over;                         //1=affichage du game over en cours
extern int              cmpt_vbl_nrj_player;                   //compteur utilise pour augmenter automatiquement l'energie du vaisseau du joueur
extern int              tmp_apparition_vj;                     //appearance time counter of player's ship 
extern bool             disparition_vj;                        //1=disappearance of player's ship

//..............................................................................

//------------------------------------------------------------------------------
// augmente le niveau d'energie du joueur
//------------------------------------------------------------------------------
void vaisseau_restaure()
{
  if(!aff_game_over && !player_pause && etat_menu == MENU_OFF)
  {
    cmpt_vbl_nrj_player += (5 - vj[J1].type_vaisseau);         //compteur temporisation 
    if(cmpt_vbl_nrj_player >= 150)                             //augmente l'energie du vaisseau du joueur ?
    {
      cmpt_vbl_nrj_player = 0;                                 //oui, remet le compteur temporisation a zero
      if(vj[J1].spr.etat_dommages < vj[J1].spr.pow_of_dest)    //barre d'energie au maximum ?
      {
        vj[J1].spr.etat_dommages++;                            //non, augmente la barre d'erengie
        if(vj[J1].spr.etat_dommages >= vj[J1].spr.pow_of_dest) //energie au maximum ?
          Init_Anim_Option(1, 1);                              //animation de l'option dans le panneau a gauche
        bar_nrj_player = 1;                                    //1=reaffiche la barre d'energie
      }
    }
  }
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void vaisseau_invincibilite()
{
  if(vj[J1].invincible)
  {                                                            // Décrémente le compteur de VBLs.
    vj[J1].vbl_invincible--;
    // Test si le vaisseau du joueur est visible.
    if(vj[J1].visible)
    {                                                          // Test si on doit reinitialiser le compteur de VBLs.
      if(vj[J1].vbl_invincible <= 0)
      {                                                        // Indique que le vaisseau du joueur doit devenir invisible.
        vj[J1].visible = 0;
        vj[J1].vbl_invincible = vj[J1].invincible;
      }
    }
    else                                                       // Sinon, le vaisseau du joueur est invisible.
    {                                                          // Test si on doit reinitialiser le compteur de VBLs.
      if(vj[J1].vbl_invincible <= 0)
      {                                                        // Indique que le vaisseau du joueur doit devenir visible.
        vj[J1].visible = 1;
        // Décrémente le compteur d'invincibilité du joueur si la pause n'est pas activée.
        if(!player_pause && etat_menu == MENU_OFF)
          vj[J1].invincible--;
        // Test si le joueur est toujours invincible.
        if(vj[J1].invincible <= 0)
        {                                                      // Rend le vaisseau du joueur visible.
          vj[J1].visible = 1;
          vj[J1].metamorphose = 0;                             // Indique qu'aucune métamorphose du vaisseau ne doit s'oppérer.
        }
        else                                                   // Sinon, le joueur est toujours invincible.
        {                                                      // Donne la nouvelle valeur au compteur de VBLs.
          vj[J1].vbl_invincible = vj[J1].invincible;
        }
      }
      // On ajoute une étoile à la liste des explosions.
      int                     aff_etoil_x =
        (int)vj[J1].spr.coor_x +
        (int)(((long)rand() %
               (vj[J1].spr.img[vj[J1].spr.img_act]->larg + 16))) - 16;
      int                     aff_etoil_y =
        (int)vj[J1].spr.coor_y +
        (int)(((long)rand() %
               (vj[J1].spr.img[vj[J1].spr.img_act]->haut + 16))) - 8;
      if(aff_etoil_x >= BANDE_DE_CLIP
         && aff_etoil_x <= (BANDE_DE_CLIP + LARG_ECR_RAM)
         && vj[J1].metamorphose && aff_etoil_y >= BANDE_DE_CLIP
         && aff_etoil_y <= (BANDE_DE_CLIP + HAUT_ECR_RAM)
         && nbr_exps < (MAX_NMI_ON_SCREEN - 1))
        Add_Explosion((float)aff_etoil_x, (float)aff_etoil_y, 0.7f,
                      PLAYERSTAR, 0);
    }
  }
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void boule_execution()
{
  static int              i, k, l;

  for(i = 0; i < nbr_boules; i++)
  {                                                            // Retourne l'indice dans le tableau 'bou' du nième élément de la liste.
    cmpt_boules = liste_bou[i];
    // Test si la pause est activée ou pas.
    if(!player_pause && etat_menu == MENU_OFF)
    {                                                          // Déplacement de la boule sur la position suivante.
      bou[cmpt_boules].pos_pts_cercle++;
      // Test si la position n'est pas suppérieure à la position existante.
      if(bou[cmpt_boules].pos_pts_cercle >= bou[cmpt_boules].nbr_pts_cercle)
      {                                                        // Si on à dépassé le nombre de positions maximales, on remet la position à zéro.
        bou[cmpt_boules].pos_pts_cercle = 0;
      }
      // Mise à jour des coordonées de la boule.
      bou[cmpt_boules].coor_x =
        (short)(vj[J1].spr.coor_x) +
        vj[J1].spr.img[vj[J1].spr.img_act]->x_cg +
        cercle_px[bou[cmpt_boules].pos_pts_cercle] -
        bou[cmpt_boules].img[bou[cmpt_boules].img_act]->x_cg;
      bou[cmpt_boules].coor_y =
        (short)(vj[J1].spr.coor_y) +
        vj[J1].spr.img[vj[J1].spr.img_act]->y_cg +
        cercle_py[bou[cmpt_boules].pos_pts_cercle] -
        bou[cmpt_boules].img[bou[cmpt_boules].img_act]->y_cg;
      // Décompte le temps séparant deux tirs.
      bou[cmpt_boules].cad_tir--;
      // Test si la boule doit ou non lancer un tir.
      if(bou[cmpt_boules].cad_tir <= 0)
      {                                                        // Met la cadence du tir pour la boule.
        bou[cmpt_boules].cad_tir = bou[cmpt_boules].cad_tir_initiale;   // Corespond au temps qu'il faut à la boule avant de pouvoir tirer à nouveau.
        // Boucle sur tous les points de tirs existants.
        for(k = 0;
            k < bou[cmpt_boules].img[bou[cmpt_boules].img_act]->nbr_pts_tir;
            k++)
        {                                                      // Test s'il est possible d'ajouter un nouveau tir à la liste.
          if(nbr_tirs < (NBR_TIRS_MAX - 1))
          {                                                    // Recherche d'un nouveau tir.
            new_tir = New_Element_Tir();
            // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
            tirs[new_tir].clignote = 1;                        // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
            tirs[new_tir].spr.genre = AMI;                     // Indique que c'est un sprite ennemi.
            tirs[new_tir].spr.trajectoire = 0;                 // Indique que la trajectoire du sprite est calculée une fois pour toute.
            tirs[new_tir].spr.nbr_img = 32;                    // Indique le nombre d'images composant le sprite.
            tirs[new_tir].spr.pow_of_dest = 1;                 // Indique la puissance de destruction du sprite.
            // Donne l'adresse des images du sprite.
            for(l = 0; l < tirs[new_tir].spr.nbr_img; l++)
            {                                                  //Donne l'adresse des images du projectile.
              tirs[new_tir].spr.img[l] = (image *) & fire[V1TN1][l];    // Corespond au Tir 2 puissance 1.
            }
            tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
            //tirs[new_tir].spr.pilote = 0;                      // Indique par qui est dirigé le sprite.
            //tirs[new_tir].spr.mode_commande = 0;               // Indique le moyen par lequel on fait déplacer le sprite.
            tirs[new_tir].spr.img_act = 0;                     // Indique l'image actuelle.
            tirs[new_tir].spr.nbr_tot_vbl = 4;                 // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
            tirs[new_tir].spr.nbr_vbl_act = 0;                 // Indique le nombre de VBL comptées actuellement.
            // Donne les coordonnées d'affichage du sprite.
            tirs[new_tir].img_angle =
              bou[cmpt_boules].img[bou[cmpt_boules].img_act]->angle_tir[k];
            tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
            tirs[new_tir].spr.coor_x =
              (float)(bou[cmpt_boules].coor_x +
                      bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                      xy_pos_tir[k][X] -
                      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg);
            tirs[new_tir].spr.coor_y =
              (float)(bou[cmpt_boules].coor_y +
                      bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                      xy_pos_tir[k][Y] -
                      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg);
            tirs[new_tir].tmp_life_tir = 400;
            // Donne un angle vertical pour le tir.
            tirs[new_tir].angle_tir = PI_SUR_16 * tirs[new_tir].img_angle;
            tirs[new_tir].spr.vitesse = 9.0;                   // Donne la vitesse de déplacement du sprite.
          }
        }
      }
    }
    // Test si on doit ou non afficher la boule.
    if((bou[cmpt_boules].coor_x + bou[cmpt_boules].img[0]->larg) <
       DEBUT_ECRAN_X
       || (bou[cmpt_boules].coor_y + bou[cmpt_boules].img[0]->haut) <
       DEBUT_ECRAN_Y
       || bou[cmpt_boules].coor_x > (DEBUT_ECRAN_X + LARG_ECR_RAM - 1)
       || bou[cmpt_boules].coor_y > (DEBUT_ECRAN_Y + HAUT_ECR_RAM - 1))
    {                                                          // Indique que la boule n'est pas visible, on n'a donc pas besoin de faire de tests de collision avec elle.
      bou[cmpt_boules].visible = 0;
    }
    else                                                       // Sinon, on doit afficher la boule.
    {                                                          // Indique que la boule est visible, on devra donc faire les tests de collision avec elle.
      bou[cmpt_boules].visible = 1;
      // Incrémente le compteur de VBLs pour l'affichage de l'animation.
      bou[cmpt_boules].nbr_vbl_act++;
      // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
      if(bou[cmpt_boules].nbr_vbl_act >= bou[cmpt_boules].nbr_tot_vbl)
      {                                                        // On initialise le compteur de VBLs.
        bou[cmpt_boules].nbr_vbl_act = 0;
        // On passe à l'image suivante.
        bou[cmpt_boules].img_act++;
        // Test si le numéro de l'image actuelle n'est pas supérieur au nombre d'images totales.
        if(bou[cmpt_boules].img_act >= bou[cmpt_boules].nbr_img)
        {                                                      // On repositionne l'image actuelle sur la première image de l'animation.
          bou[cmpt_boules].img_act = 0;
        }
      }
      // Affichage de la boule.
      // Test si on doit afficher la boule en blanc ou non.
      if(bou[cmpt_boules].affich_white)
      {                                                        // Affichage de la boule en blanc.
        //putsprcolor(coulor[BLANC],ecran_ram+bou[cmpt_boules].coor_y*LARG_ECR_RAM_TOT+bou[cmpt_boules].coor_x,bou[cmpt_boules].img[bou[cmpt_boules].img_act]->compress,(short)(bou[cmpt_boules].img[bou[cmpt_boules].img_act]->nbr_data_comp>>2));
        putsprcolor(coulor[BLANC],
                    bou[cmpt_boules].img[bou[cmpt_boules].img_act],
                    bou[cmpt_boules].coor_x, bou[cmpt_boules].coor_y);
        bou[cmpt_boules].affich_white = 0;
      }
      else
      {                                                        // Affichage normal de la boule.
        //putsprite3(bou[cmpt_boules].img[bou[cmpt_boules].img_act]->img,ecran_ram+bou[cmpt_boules].coor_y*LARG_ECR_RAM_TOT+bou[cmpt_boules].coor_x,bou[cmpt_boules].img[bou[cmpt_boules].img_act]->compress,(short)(bou[cmpt_boules].img[bou[cmpt_boules].img_act]->nbr_data_comp>>2),"bou[cmpt_boules].img[bou[cmpt_boules].img_act]->img");
        putsprite3(bou[cmpt_boules].img[bou[cmpt_boules].img_act],
                   bou[cmpt_boules].coor_x, bou[cmpt_boules].coor_y);
      }
    }
  }
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void canon_execution()
{
  static int              i, k, l;

  // Gestion des canons autour du vaisseau.
  // Bcle sur tous les canons qui se toruvent autour du vaisseau.
  for(i = 0; i < nbr_canons; i++)
  {                                                            // Retourne l'indice dans le tableau 'can' du nième élément de la liste.
    cmpt_canons = liste_can[i];
    // Donne la position verticale des canons par rapport au vaisseau du joueur.
    can[cmpt_canons].coor_y =
      (short)(vj[J1].spr.coor_y) + vj[J1].spr.img[vj[J1].spr.img_act]->haut -
      can[cmpt_canons].img[can[cmpt_canons].img_act]->haut;
    // Test si on est en présence du canon de droite appartenant au joueur.
    if(can[cmpt_canons].position == 0)
    {                                                          // Donne la position horizontale du canon.
      can[cmpt_canons].coor_x =
        (short)(vj[J1].spr.coor_x) + vj[J1].spr.img[vj[J1].spr.img_act]->larg;
    }
    // Test si on est en présence du canon de gauche appartenant au joueur.
    if(can[cmpt_canons].position == 1)
    {                                                          // Mise à jour des coordonées du canon.
      can[cmpt_canons].coor_x =
        (short)(vj[J1].spr.coor_x) -
        can[cmpt_canons].img[can[cmpt_canons].img_act]->larg;
    }
    // Test si la pause est activée ou pas.
    if(!player_pause && etat_menu == MENU_OFF)
    {                                                          // Décompte le temps séparant deux tirs.
      can[cmpt_canons].cad_tir--;
      // Test si le canon doit ou non lancer un tir.
      if(can[cmpt_canons].cad_tir <= 0)
      {                                                        // Met la cadence du tir pour le canon.
        can[cmpt_canons].cad_tir = can[cmpt_canons].cad_tir_initiale;   // Corespond au temps qu'il faut au conon avant de pouvoir tirer à nouveau.
        // Bcle sur tous les points de tirs existants.
        for(k = 0;
            k < can[cmpt_canons].img[can[cmpt_canons].img_act]->nbr_pts_tir;
            k++)
        {                                                      // Test s'il est possible d'ajouter un nouveau tir à la liste.
          if(nbr_tirs < (NBR_TIRS_MAX - 1))
          {                                                    // Recherche d'un nouveau tir.
            new_tir = New_Element_Tir();
            // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
            tirs[new_tir].clignote = 1;                        // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
            tirs[new_tir].spr.genre = AMI;                     // Indique que c'est un sprite ennemi.
            tirs[new_tir].spr.trajectoire = 0;                 // Indique que la trajectoire du sprite est calculée une fois pour toute.
            tirs[new_tir].spr.nbr_img = 32;                    // Indique le nombre d'images composant le sprite.
            switch (vj[J1].type_vaisseau)
            {
              case V1J:
                tirs[new_tir].spr.pow_of_dest = 2;             // Indique la puissance de destruction du sprite.
                break;
              case V2J:
                tirs[new_tir].spr.pow_of_dest = 3;             // Indique la puissance de destruction du sprite.
                break;
              case V3J:
                tirs[new_tir].spr.pow_of_dest = 4;             // Indique la puissance de destruction du sprite.
                break;
              case V4J:
                tirs[new_tir].spr.pow_of_dest = 3;             // Indique la puissance de destruction du sprite.
                break;
              case V5J:
                tirs[new_tir].spr.pow_of_dest = 4;             // Indique la puissance de destruction du sprite.
                break;
            }
            // Donne l'adresse des images du sprite.
            for(l = 0; l < tirs[new_tir].spr.nbr_img; l++)
            {                                                  //Donne l'adresse des images du projectile.
              tirs[new_tir].spr.img[l] = (image *) & fire[V1TN1][l];    // Corespond au Tir 2 puissance 1.
            }
            tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
            //tirs[new_tir].spr.pilote = 0;                      // Indique par qui est dirigé le sprite.
            //tirs[new_tir].spr.mode_commande = 0;               // Indique le moyen par lequel on fait déplacer le sprite.
            tirs[new_tir].spr.img_act = 0;                     // Indique l'image actuelle.
            tirs[new_tir].spr.nbr_tot_vbl = 4;                 // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
            tirs[new_tir].spr.nbr_vbl_act = 0;                 // Indique le nombre de VBL comptées actuellement.
            // Donne les coordonnées d'affichage du sprite.
            tirs[new_tir].img_angle =
              can[cmpt_canons].img[can[cmpt_canons].img_act]->angle_tir[k];
            tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
            tirs[new_tir].spr.coor_x =
              (float)(can[cmpt_canons].coor_x +
                      can[cmpt_canons].img[can[cmpt_canons].img_act]->
                      xy_pos_tir[k][X] -
                      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg);
            tirs[new_tir].spr.coor_y =
              (float)(can[cmpt_canons].coor_y +
                      can[cmpt_canons].img[can[cmpt_canons].img_act]->
                      xy_pos_tir[k][Y] -
                      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg);
            tirs[new_tir].tmp_life_tir = 400;
            // Donne un angle vertical pour le tir.
            tirs[new_tir].angle_tir = PI_SUR_16 * tirs[new_tir].img_angle;
            tirs[new_tir].spr.vitesse = 9.0;                   // Donne la vitesse de déplacement du sprite.
          }
        }
      }
    }
    // Test si on doit ou non afficher le canon.
    if((can[cmpt_canons].coor_x + can[cmpt_canons].img[0]->larg) <
       DEBUT_ECRAN_X
       || (can[cmpt_canons].coor_y + can[cmpt_canons].img[0]->haut) <
       DEBUT_ECRAN_Y
       || can[cmpt_canons].coor_x > (DEBUT_ECRAN_X + LARG_ECR_RAM - 1)
       || can[cmpt_canons].coor_y > (DEBUT_ECRAN_Y + HAUT_ECR_RAM - 1))
    {                                                          // Indique que le cnone n'est pas visible, on n'a donc pas besoin de faire de tests de collision avec lui.
      can[cmpt_canons].visible = 0;
    }
    else                                                       // Sinon, on doit afficher le canon.
    {                                                          // Indique que le canon est visible, on devra donc faire les tests de collision avec lui.
      can[cmpt_canons].visible = 1;
      // Incrémente le compteur de VBLs pour l'affichage de l'animation.
      can[cmpt_canons].nbr_vbl_act++;
      // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
      if(can[cmpt_canons].nbr_vbl_act >= can[cmpt_canons].nbr_tot_vbl)
      {                                                        // On initialise le compteur de VBLs.
        can[cmpt_canons].nbr_vbl_act = 0;
        // On passe à l'image suivante.
        can[cmpt_canons].img_act++;
        // Test si le numéro de l'image actuelle n'est pas supérieur au nombre d'images totales.
        if(can[cmpt_canons].img_act >= can[cmpt_canons].nbr_img)
        {                                                      // On repositionne l'image actuelle sur la première image de l'animation.
          can[cmpt_canons].img_act = 0;
        }
      }
      // Affichage du canon.
      // Test si on doit afficher le canon en blanc ou non.
      if(can[cmpt_canons].affich_white)
      {                                                        // Affichage du canon en blanc.
        putsprcolor(coulor[BLANC],
                    can[cmpt_canons].img[can[cmpt_canons].img_act],
                    can[cmpt_canons].coor_x, can[cmpt_canons].coor_y);
        can[cmpt_canons].affich_white = 0;
      }
      else
      {                                                        // Affichage normal du canon.
        putsprite3(can[cmpt_canons].img[can[cmpt_canons].img_act],
                   can[cmpt_canons].coor_x, can[cmpt_canons].coor_y);
      }
    }
  }
}

//------------------------------------------------------------------------------
// initialise la structure du joueur
//------------------------------------------------------------------------------
void Fill_Struct_Player_One()
{
  int                     i;

  // Remplissage de la structure vaisseau du joueur 1.
  vj[J1].nbr_bonus = 4;                                        // Donne le nombre de bonus que détient le vaisseau.
  vj[J1].nbr_boules = 0;                                       // Donne le nombre de boules détenues par le vaisseau.
  vj[J1].type_vaisseau = V1J;                                  // Donne le type de vaisseau du joueur.
  //vj[J1].type_vaisseau = V5J ; // Donne le type de vaisseau du joueur.
  vj[J1].score = 0L;                                           // Initialise le score du joueur.
  vj[J1].cad_tir = 50 - (vj[J1].type_vaisseau * 5 + 5);        // Nombre de VBLs nécessaires avant que le vaisseau puisse à noubeau tirer.
	//vj[J1].freeze_tmps = 0;
  vj[J1].vit_dep_vaisseau = 2;                                 // Indique l'option vitesse actuellement sélectionné par le vaisseau.
  vj[J1].power_avant = 1;                                      // Indique l'option tir avant actuellement sélectionné par le vaisseau.
  vj[J1].power_gauche = 0;                                     // Indique l'option tir gauche actuellement sélectionné par le vaisseau.
  vj[J1].power_droite = 0;                                     // Indique l'option tir droite actuellement sélectionné par le vaisseau.
  vj[J1].power_arriere = 0;                                    // Indique l'option tir arriere actuellement sélectionné par le vaisseau.
  vj[J1].power_spec_avant = 0;                                 // Indique l'option tir special avant actuellement sélectionné par le vaisseau.
  vj[J1].power_spec_arriere = 0;                               // Indique l'option tir special arriere actuellement sélectionné par le vaisseau.
  vj[J1].power_spec_droite = 0;                                // Indique l'option tir special droite actuellement sélectionné par le vaisseau.
  vj[J1].power_spec_gauche = 0;                                // Indique l'option tir special gauche actuellement sélectionné par le vaisseau.
  // Remplissage de la structure sprite appartenant au vaisseau du joueur 1.
  vj[J1].spr.genre = AMI;                                      // Indique que c'est un sprite ami.
  vj[J1].spr.trajectoire = 1;                                  // Indique que la trajectoire du sprite est recalculée à chaque VBL.
  vj[J1].spr.pow_of_dest = (short)(vj[J1].type_vaisseau * 20 + 20);     // Indique la puissance de destruction du sprite.
  vj[J1].spr.etat_dommages = vj[J1].spr.pow_of_dest;           // Indique l'état de domages du sprite.
  //vj[J1].spr.pilote = 1;                                       // Indique par qui est dirigé le sprite.
  //vj[J1].spr.mode_commande = 0;                                // Indique le moyen par lequel on fait déplacer le sprite.
  vj[J1].spr.nbr_img = NBR_MAX_IMG_VAIS_J;                     // Indique le nombre d'images composant le sprite.
  vj[J1].spr.img_act = 2;                                      // Indique l'image actuelle.
  vj[J1].spr.nbr_tot_vbl = 10;                                 // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
  vj[J1].spr.nbr_vbl_act = 0;                                  // Indique le nombre de VBL comptées actuellement.
  vj[J1].metamorphose = 0;                                     // Indique qu'aucune métamorphose du vaisseau n'est en train.
  // Mise en place des images du vaisseau du joueur 1.
  for(i = 0; i < NBR_MAX_IMG_VAIS_J; i++)
  {
    vj[J1].spr.img[i] = (image *) & vj1[vj[J1].type_vaisseau][i];       // Donne l'adresse des images.
  }
}

//------------------------------------------------------------------------------
// player's ship appeared
//------------------------------------------------------------------------------
void Apparition_Vaisseau()
{
  vj[J1].spr.coor_x = (float)(LARG_ECR_RAM - vj[J1].spr.img[2]->larg / 2);
  vj[J1].spr.coor_y = (float)(DEBUT_ECRAN_Y - 32);
  vj[J1].vitesse_y = 1.2f;                                     //vertical speed
  vj[J1].vitesse_x = 0.0;                                      //horizontal speed
  tmp_apparition_vj = 90;
  disparition_vj = 0;
}

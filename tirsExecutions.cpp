//*****************************************************************************
// copyright (c) 1998-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "tirsExecutions.cpp"
// created      : ?
// updates      : 2002-08-02
//-----------------------------------------------------------------------------
// functions    : tirsExecutions()
//-----------------------------------------------------------------------------
// descriptions : gestion de tous les tirs / manage all shoots
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
//------------------------------------------------------------------------------
extern short            nbr_tirs;                              //nombre de tirs actuellement geres
extern tir              tirs[NBR_TIRS_MAX];                    //tableau de tous les objets tirs
extern int              liste_tirs[NBR_TIRS_MAX];              //liste des 'tirs' geres
extern int              cmpt_tirs;                             //compteur nombre de tirs.
extern int              etat_menu;                             //MENU_OFF(false)=menu inactif ; MENU_ON(true)=menu actif
extern bool             player_pause;                          //true=pause en cours
extern bool             aff_game_over;                         //true=affichage du game over en cours
extern short            nbr_nmis;                              //nombre d'ennemis actuellement gérés.
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //structure des vaisseaux ennemis
extern float            depix[13][32];                         //tableau de deplacement en x des tirs a trajectoire fixe
extern float            depiy[13][32];                         //tableau de deplacement en y des tirs a trajectoire fixe
extern int              liste_ve[MAX_NMI_ON_SCREEN];           //liste de tous les vaisseaux ennemis a gerer
extern int              tmp_tsts_x;
extern int              tmp_tsts_y;
extern int              tmp_tstd_x;
extern int              tmp_tstd_y;
extern curve            initial_curve[NBR_CURVE_MAX];          //structure des courbes de bezier
extern short            nbr_exps;                              //nombre d'explosions actuellement gerees
extern bool             bar_nrj_guardian;
extern int              cmpt_nmis;                             //compteur nombre d'ennemis
extern int              player_score;
extern int              bonusx;                                //multiplicateur de points (*1, *2, *4)
extern bool             apparition_gardian;                    //1=apparition d'un gardien.
extern vaisseau         vj[NBR_MAX_J];
extern short            nbr_boules;                            //nombre de boules actuellement gerees
extern int              liste_bou[MAX_BOULES_ON_SCREEN];       //liste de toutes les 'bou' gerees
extern str_boule        bou[MAX_BOULES_ON_SCREEN];             //structure des boules.
extern int              cmpt_boules;                           //compteur du nombre de boules
extern int              liste_can[MAX_CANONS_ON_SCREEN];       //liste de tous les 'can' geres
extern str_canon        can[MAX_CANONS_ON_SCREEN];             //structure des canons.
extern int              cmpt_canons;                           //compteur du nombre de canons.
extern bool             bar_nrj_player;
extern bool             game_over_player_one;                  //true=le joueur vient de perdre son vaisseau.(game over)
extern short            nbr_canons;                            //nombre de canons actuellement geres

//------------------------------------------------------------------------------
void tirsExecutions()
{
  int                     i, j, k, l, m;

  for(i = 0; i < nbr_tirs; i++)
  {                                                            // Retourne l'indice dans le tableau 'tirs' du nième élément de la liste.
    cmpt_tirs = liste_tirs[i];
    // Test si un projectile est en activité.
    if(tirs[cmpt_tirs].tmp_life_tir)
    {                                                          // Test si la pause est activée ou pas.
      if(!player_pause && etat_menu == MENU_OFF
         && _score::getActionExecution() == 0)
      {                                                        // Test la trajectoire du tir afin de calculer son déplacement.
        switch (tirs[cmpt_tirs].spr.trajectoire)
        {                                                      // Cas d'un tir à trajectoire fixe.
          case 0:
          {                                                    // Donne la nouvelle coordonée X du tir à trajectoire fixe.
            tirs[cmpt_tirs].spr.coor_x +=
              depix[(short)tirs[cmpt_tirs].spr.vitesse][tirs[cmpt_tirs].
                                                        img_angle];
            // Donne la nouvelle coordonée Y du tir à trajectoire fixe.
            tirs[cmpt_tirs].spr.coor_y +=
              depiy[(short)tirs[cmpt_tirs].spr.vitesse][tirs[cmpt_tirs].
                                                        img_angle];
          }
            break;
            // Cas d'un tir dont la trajectoire est calculée à chaque VBLs (du type missile téléguidé).
          case 1:
          {                                                    // Calcul du nouvel angle en fonction de l'ancien.
            if(nbr_nmis)
            {
              int                     val_tmp = liste_ve[0];

              tirs[cmpt_tirs].angle_tir =
                Calcul_New_Angle(tirs[cmpt_tirs].angle_tir,
                                 Calc_Angle_Tir((short)
                                                (tirs[cmpt_tirs].spr.coor_x +
                                                 tirs[cmpt_tirs].spr.
                                                 img[tirs[cmpt_tirs].
                                                     img_old_angle]->x_cg),
                                                (short)(tirs[cmpt_tirs].spr.
                                                        coor_y +
                                                        tirs[cmpt_tirs].spr.
                                                        img[tirs[cmpt_tirs].
                                                            img_old_angle]->
                                                        y_cg),
                                                (short)(ve[val_tmp].spr.
                                                        coor_x +
                                                        ve[val_tmp].spr.
                                                        img[ve[val_tmp].spr.
                                                            img_act]->x_cg),
                                                (short)(ve[val_tmp].spr.
                                                        coor_y +
                                                        ve[val_tmp].spr.
                                                        img[ve[val_tmp].spr.
                                                            img_act]->y_cg)),
                                 tirs[cmpt_tirs].agilite);
            }
            else
              tirs[cmpt_tirs].angle_tir =
                Calcul_New_Angle(tirs[cmpt_tirs].angle_tir,
                                 Calc_Angle_Tir((short)
                                                (tirs[cmpt_tirs].spr.coor_x +
                                                 tirs[cmpt_tirs].spr.
                                                 img[tirs[cmpt_tirs].
                                                     img_old_angle]->x_cg),
                                                (short)(tirs[cmpt_tirs].spr.
                                                        coor_y +
                                                        tirs[cmpt_tirs].spr.
                                                        img[tirs[cmpt_tirs].
                                                            img_old_angle]->
                                                        y_cg), 256, 0),
                                 tirs[cmpt_tirs].agilite);
            // Donne la nouvelle coordonée X du missile.
            tirs[cmpt_tirs].spr.coor_x =
              Deplacement_Tir_X(tirs[cmpt_tirs].angle_tir,
                                tirs[cmpt_tirs].spr.vitesse,
                                tirs[cmpt_tirs].spr.coor_x -
                                tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].
                                                        img_old_angle]->x_cg);
            // Donne la nouvelle coordonée Y du missile.
            tirs[cmpt_tirs].spr.coor_y =
              Deplacement_Tir_Y(tirs[cmpt_tirs].angle_tir,
                                tirs[cmpt_tirs].spr.vitesse,
                                tirs[cmpt_tirs].spr.coor_y -
                                tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].
                                                        img_old_angle]->y_cg);
          }
            break;
            // Cas d'un tir dont la trajectoire suit une courbe.
          case 2:
          {                                                    // On change deux fois de position dans la courbe pour accélérer le déplacement du tir.
            for(int z = 0; z < 2; z++)
            {                                                  // On change la position du tir dans la courbe.
              tirs[cmpt_tirs].pos_vaiss[POS_CURVE]++;
              // Test si on doit ou non supprimer le tir parce qu'il est sorti de la courbe.
              if(tirs[cmpt_tirs].pos_vaiss[POS_CURVE] >=
                 initial_curve[tirs[cmpt_tirs].num_courbe].nbr_pnt_curve)
              {                                                // On enlève le tir de la liste des tirs.
                Del_Element_Tir(cmpt_tirs, i);                 // Routine utilisée pour supprimer un tir du tableau des tirs.
                // On se positionne sur le tir que l'on vient de décaler.
                i--;
                // On sort de la bcle qui test les collisions des tirs amis avec les vaisseaux ennemis.
                goto fin_bcle_tst_col_ennemi;
              }
              // On donne les nouvelles coordonées du tir.
              tirs[cmpt_tirs].spr.coor_x += (float)initial_curve[tirs[cmpt_tirs].num_courbe].delta_x[tirs[cmpt_tirs].pos_vaiss[POS_CURVE]];     // Donne l'indice de progression du tir sur la courbe.
              tirs[cmpt_tirs].spr.coor_y += (float)initial_curve[tirs[cmpt_tirs].num_courbe].delta_y[tirs[cmpt_tirs].pos_vaiss[POS_CURVE]];     // Donne l'indice de progression du tir sur la courbe.
            }                                                  // Fin des deux passages sur la courbe.
          }
            break;
        }
      }
      // Test si on doit ou non afficher les projectiles.
      if((short)
         (tirs[cmpt_tirs].spr.coor_x + tirs[cmpt_tirs].spr.img[0]->haut - 1) <
         DEBUT_ECRAN_X
         || (short)(tirs[cmpt_tirs].spr.coor_y +
                    tirs[cmpt_tirs].spr.img[0]->larg - 1) < DEBUT_ECRAN_Y
         || (short)tirs[cmpt_tirs].spr.coor_y > DEBUT_ECRAN_Y + HAUT_ECR_RAM
         || (short)tirs[cmpt_tirs].spr.coor_x > DEBUT_ECRAN_X + LARG_ECR_RAM)
      {                                                        // Test si on doit ou non détruire le projectile à trajectoire fixe.
        if(tirs[cmpt_tirs].spr.trajectoire == 0)
        {
          Del_Element_Tir(cmpt_tirs, i);                       // Routine utilisée pour supprimer un tir du tableau des tirs.
          // On se positionne sur le tir que l'on vient de décaler.
          i--;
        }
        else                                                   // Le tir est sorti de l'écran, on doit donc tester s'il doit continuer à exister.
        {                                                      // Test si le tir à trajectoire calculée à chaque VBLs ou le tir qui suit une courbe doit continuer à vivre.
          if((short)(tirs[cmpt_tirs].spr.coor_x) < (BANDE_DE_CLIP - 32) ||
             (short)(tirs[cmpt_tirs].spr.coor_y) < (BANDE_DE_CLIP - 32) ||
             (short)tirs[cmpt_tirs].spr.coor_y >
             (BANDE_DE_CLIP + HAUT_ECR_RAM + 32)
             || (short)tirs[cmpt_tirs].spr.coor_x >
             (BANDE_DE_CLIP + LARG_ECR_RAM + 32))
          {                                                    // Routine utilisée pour supprimer un tir du tableau des tirs.
            Del_Element_Tir(cmpt_tirs, i);
            // On se positionne sur le tir que l'on vient de décaler.
            i--;
          }
          else                                                 // Le tir à trajectoire calculée peut encore continuer à vivre.
          {                                                    // Si c'est un tir à trajectoire calculée à chaque VBLs, on donne sa nouvelle position.
            if(tirs[cmpt_tirs].spr.trajectoire == 1)
            {                                                  // Donne la nouvelle coordonée X du tir.
              tirs[cmpt_tirs].spr.coor_x +=
                tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_old_angle]->x_cg;
              // Donne la nouvelle coordonée Y du tir.
              tirs[cmpt_tirs].spr.coor_y +=
                tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_old_angle]->y_cg;
              // Décrémente le temps de vie du projectile.
              tirs[cmpt_tirs].tmp_life_tir--;
            }
          }
        }
        // On sort de la bcle qui test les collisions des tirs amis avec les vaisseaux ennemis.
        goto fin_bcle_tst_col_ennemi;
      }
      else                                                     // Sinon, on doit l'afficher.
      {                                                        // Test la trajectoire du tir pour l'affichage du tir.
        switch (tirs[cmpt_tirs].spr.trajectoire)
        {                                                      // Cas d'un tir à trajectoire fixe.
          case 0:
          case 2:                                             // Cas d'un tir qui suit une courbe.
          {                                                    // Test si l'on est en présence d'un sprite qui clignote.
            if(tirs[cmpt_tirs].clignote)                       // Ceci afin de faire clignoter le sprite.
            {                                                  // Incrémente le compteur de VBLs pour l'affichage de l'animation.
              tirs[cmpt_tirs].spr.nbr_vbl_act++;
              // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
              if(tirs[cmpt_tirs].spr.nbr_vbl_act >=
                 tirs[cmpt_tirs].spr.nbr_tot_vbl)
              {                                                // On initialise le compteur de VBLs.
                tirs[cmpt_tirs].spr.nbr_vbl_act = 0;
                // On passe à l'image suivante.
                tirs[cmpt_tirs].spr.img_act++;
                // Test si le numéro de l'image actuelle n'est pas supérieur au nombre d'images totales.
                if(tirs[cmpt_tirs].spr.img_act >= tirs[cmpt_tirs].spr.nbr_img)
                {                                              // On repositionne l'image actuelle sur la première image de l'animation.
                  tirs[cmpt_tirs].spr.img_act = 0;
                }
              }
              // Affichage de l'image du tir.
              putsprite3(tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].spr.img_act],
                         (unsigned int)tirs[cmpt_tirs].spr.coor_x,
                         (unsigned int)tirs[cmpt_tirs].spr.coor_y);
            }
            else                                               // Cas d'un tir qui ne clignote pas.
            {                                                  // Affichage de l'image du tir.
              putsprite3(tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_angle],
                         (unsigned int)tirs[cmpt_tirs].spr.coor_x,
                         (unsigned int)tirs[cmpt_tirs].spr.coor_y);
            }
            // Test de collision du tir à trajectoire fixe.
            if(tirs[cmpt_tirs].spr.genre == AMI)               // Test si on est en présence d'un sprite AMI ou ENNEMI
            {                                                  // On traite tous les vaisseaux ennemis existants.
              for(j = 0; j < nbr_nmis; j++)
              {                                                // Retourne l'indice dans le tableau 've' du nième élément de la liste.
                cmpt_nmis = liste_ve[j];
                // Test si le vaisseau ennemi est visible et vivant, sinon, il n'est pas utile de faire le test des collisions.
                // Test aussi si on est en présence d'un gardien en train d'apparaitre et dans ce cas, on ne test pas non plus les collisions.
                if(ve[cmpt_nmis].visible && !ve[cmpt_nmis].dead
                   && !(apparition_gardian
                        && ve[cmpt_nmis].deplacement == GARDIEN))
                {                                              // Bcle sur tous les points de collision du tir.
                  for(k = 0;
                      k <
                      tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_angle]->
                      nbr_pts_col; k++)
                  {                                            // Bcle sur toutes les zones de collision de l'ennemi.
                    for(l = 0;
                        l <
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        nbr_zon_col; l++)
                    {                                          // Test si le point de collision du tir se trouve dans la zone de collision de l'ennemi.
                      if(((short)tirs[cmpt_tirs].spr.coor_x +
                          tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_angle]->
                          xy_pts_col[k][X]) >=
                         ((short)ve[cmpt_nmis].spr.coor_x +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          xy_zon_col[l][X])
                         && ((short)tirs[cmpt_tirs].spr.coor_x +
                             tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].
                                                     img_angle]->
                             xy_pts_col[k][X]) <
                         ((short)ve[cmpt_nmis].spr.coor_x +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          xy_zon_col[l][X] +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          lh_zon_col[l][L])
                         && ((short)tirs[cmpt_tirs].spr.coor_y +
                             tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].
                                                     img_angle]->
                             xy_pts_col[k][Y]) >=
                         ((short)ve[cmpt_nmis].spr.coor_y +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          xy_zon_col[l][Y])
                         && ((short)tirs[cmpt_tirs].spr.coor_y +
                             tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].
                                                     img_angle]->
                             xy_pts_col[k][Y]) <
                         ((short)ve[cmpt_nmis].spr.coor_y +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          xy_zon_col[l][Y] +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          lh_zon_col[l][H]))
                      {                                        // On décrémente l'état des domages du vaisseau touché par le tir.
                        ve[cmpt_nmis].spr.etat_dommages -=
                          tirs[cmpt_tirs].spr.pow_of_dest;
                        // On test si le vaisseau est détruit par le tir.
                        if(ve[cmpt_nmis].spr.etat_dommages <= 0)
                        {                                      // Test si le vaisseau ennemi était une météorite.
                          if(ve[cmpt_nmis].type >= BIGMETEOR)
                          {                                    // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                            //Gene_Bonus_Malus_Meteor(ve[cmpt_nmis].spr.coor_x, ve[cmpt_nmis].spr.coor_y);
                            Gene_Bonus_Malus_Meteor(&ve[cmpt_nmis]);
                          }
                          else                                 // Sinon, on appel la routine normale de génération de bonus, malus.
                          {                                    // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                            //Gene_Bonus_Malus(ve[cmpt_nmis].spr.coor_x, ve[cmpt_nmis].spr.coor_y);
                            Gene_Bonus_Malus(&ve[cmpt_nmis]);
                          }
                          // Test si l'on est en présence d'un vaisseau spécial ou d'un gardien.
                          if(ve[cmpt_nmis].deplacement == SPECIAUX
                             || ve[cmpt_nmis].deplacement == GARDIEN)
                          {
                            Add_Serie_Of_Explosion(cmpt_nmis); // Routine utilisée pour ajouter une série d'explosion à un gros vaisseau.
                            // On ajoute un eclat à la liste des explosions.
                            Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       x_cg - 8,
                                       ve[cmpt_nmis].spr.coor_y +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       y_cg - 8, 1.0, 5, 0, 2);
                          }
                          else                                 // On ajoute une explosion à la liste.
                          {
                            if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                              Add_Explosion(ve[cmpt_nmis].spr.coor_x,
                                            ve[cmpt_nmis].spr.coor_y, 0.25,
                                            ve[cmpt_nmis].type, 0);
                            // On ajoute un eclat à la liste des explosions.
                            Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       x_cg - 8,
                                       ve[cmpt_nmis].spr.coor_y +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       y_cg - 8, 1.0, 3, 0, 1);
                          }
                          // Mise à jour du score du joueur pour la destruction d'un ennemi
                          player_score +=
                            ve[cmpt_nmis].spr.pow_of_dest << 2 << bonusx;
                          // Indique que le vaisseau ennemi doit clignoter avant de disparaitre.
                          Clignote_Nmi_Avant_Disparition(cmpt_nmis);
                        }
                        else                                   // Cas ou le vaisseau ennemi n'est pas détruit.
                        {                                      // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                          ve[cmpt_nmis].affich_white = TRUE;
                          //On test si on est en presence d'un gardien afin de mettre a jour sa barre d'energie.
                          if(ve[cmpt_nmis].deplacement == GARDIEN)
                            bar_nrj_guardian = TRUE;
                          //On test si on est en presence d'un vaisseau qui doit s'enerver.
                          //Pour ca, on tests tous les vaisseaux qui ne doivent pas s'enerver car ils sont moins nombreux.
                          if(ve[cmpt_nmis].type != NAGGYS)
                          {                                    // On décrémente la cadence de tir de l'ennemi (il vient d'etre touché alors ça l'as énervé).
                            ve[cmpt_nmis].cad_tir =
                              ve[cmpt_nmis].cad_tir >> 1;
                          }
                        }
                        // On ajoute une petite explosion à la liste.
                        if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                          Add_Explosion(tirs[cmpt_tirs].spr.coor_x,
                                        tirs[cmpt_tirs].spr.coor_y, 0.35f,
                                        MINI_EXPLOSION, 0);
                        // On supprime le tir qui vient de toucher le vaisseau ennemi.
                        Del_Element_Tir(cmpt_tirs, i);         // Routine utilisée pour supprimer un tir du tableau des tirs.
                        // On se positionne sur le tir que l'on vient de décaler.
                        i--;
                        // On sort de la bcle qui test les collisions des tirs amis avec les vaisseaux ennemis.
                        goto fin_bcle_tst_col_ennemi;
                      }
                    }
                  }
                }
              }
            }
            else                                               // Cas d'un tir ennemi.
            {                                                  // Si le joueur n'est pas Game Over.
              if(!aff_game_over)
              {                                                // Bcle sur tous les points de collision du tir.
                for(k = 0;
                    k <
                    tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_angle]->
                    nbr_pts_col; k++)
                {                                              // Donne les coordonnée du point de collision actuel du tir.
                  tmp_tsts_x =
                    (short)tirs[cmpt_tirs].spr.coor_x +
                    tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_angle]->
                    xy_pts_col[k][X];
                  tmp_tsts_y =
                    (short)tirs[cmpt_tirs].spr.coor_y +
                    tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_angle]->
                    xy_pts_col[k][Y];
                  // Bcle sur toutes les zones de collision du vaisseau ami.
                  for(l = 0;
                      l < vj[J1].spr.img[vj[J1].spr.img_act]->nbr_zon_col;
                      l++)
                  {                                            // Test si le point de collision du tir se trouve dans la zone de collision de vaisseau ami.
                    if(tmp_tsts_x >=
                       ((short)vj[J1].spr.coor_x +
                        vj[J1].spr.img[vj[J1].spr.img_act]->xy_zon_col[l][X])
                       && tmp_tsts_x <
                       ((short)vj[J1].spr.coor_x +
                        vj[J1].spr.img[vj[J1].spr.img_act]->xy_zon_col[l][X] +
                        vj[J1].spr.img[vj[J1].spr.img_act]->lh_zon_col[l][L])
                       && tmp_tsts_y >=
                       ((short)vj[J1].spr.coor_y +
                        vj[J1].spr.img[vj[J1].spr.img_act]->xy_zon_col[l][Y])
                       && tmp_tsts_y <
                       ((short)vj[J1].spr.coor_y +
                        vj[J1].spr.img[vj[J1].spr.img_act]->xy_zon_col[l][Y] +
                        vj[J1].spr.img[vj[J1].spr.img_act]->lh_zon_col[l][H]))
                    {                                          // On décrémente l'état des domages du vaisseau touché par le tir.
                      // Test si le vaisseau du joueur est invincible dans le but de décendre son énergie.
                      if(!vj[J1].invincible)
                      {                                        // Test si l'énergie du joueur était au maximum.
                        if(vj[J1].spr.etat_dommages == vj[J1].spr.pow_of_dest)
                        {                                      // Indique que l'animation d'ouverture de l'option d'energie doit s'effectuer.
                          Init_Anim_Option(1, FALSE);
                        }
                        // Donne le nouvel état des dommages du vaisseau du joueur.
                        vj[J1].spr.etat_dommages -=
                          tirs[cmpt_tirs].spr.pow_of_dest;
                      }
                      // Demande un réaffichage de la barre d'énergie du joueur.
                      bar_nrj_player = TRUE;
                      // On test si le vaisseau est détruit.
                      if(vj[J1].spr.etat_dommages <= 0)
                      {                                        // Indique que le joueur est Game Over.
                        game_over_player_one = TRUE;
                      }
                      else
                      {                                        // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                        vj[J1].affich_white = TRUE;
                      }
                      // On ajoute une petite explosion à la liste.
                      if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                        Add_Explosion((float)tirs[cmpt_tirs].spr.coor_x,
                                      (float)tirs[cmpt_tirs].spr.coor_y,
                                      0.35f, MINI_EXPLOSION, 0);
                      // On supprime le tir qui vient de toucher le vaisseau du joueur.
                      Del_Element_Tir(cmpt_tirs, i);           // Routine utilisée pour supprimer un tir du tableau des tirs.
                      // On se positionne sur le tir que l'on vient de décaler.
                      i--;
                      // On sort de la bcle qui test les collisions des tirs.
                      goto fin_bcle_tst_col_ennemi;
                    }
                  }
                  // Test les collisions entre les boules et le tir actuel.
                  // Boucles sur toutes les boules du joueur.
                  for(l = 0; l < nbr_boules; l++)
                  {                                            // Retourne l'indice dans le tableau 'bou' du nième élément de la liste.
                    cmpt_boules = liste_bou[l];
                    // Test si la boule est visible et si oui, on peux tester les collisions.
                    if(bou[cmpt_boules].visible)
                    {                                          // Boucle sur toutes les zones de collision de la boule.
                      for(m = 0;
                          m <
                          bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                          nbr_zon_col; m++)
                      {                                        // Donne les coordonnée de la zone de collision actuelle de la boule.
                        tmp_tstd_x =
                          bou[cmpt_boules].coor_x +
                          bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                          xy_zon_col[m][X];
                        tmp_tstd_y =
                          bou[cmpt_boules].coor_y +
                          bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                          xy_zon_col[m][Y];
                        // Test s'il le point de collision de la boule se trouve à l'intérieur de la zone de collision du vaisseau ennemi.
                        if(tmp_tsts_x >= tmp_tstd_x &&
                           tmp_tsts_y >= tmp_tstd_y &&
                           tmp_tsts_x <
                           (tmp_tstd_x +
                            bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                            lh_zon_col[m][L])
                           && tmp_tsts_y <
                           (tmp_tstd_y +
                            bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                            lh_zon_col[m][H]))
                        {                                      // Donne le nouvel état des dommages de la boule du joueur.
                          bou[cmpt_boules].etat_dommages -=
                            tirs[cmpt_tirs].spr.pow_of_dest;
                          // Test si la boule est détruite.
                          if(bou[cmpt_boules].etat_dommages <= 0)
                          {                                    // On supprime la boule de la liste.
                            Del_Element_Bou(cmpt_boules, l);   // Routine utilisée pour supprimer une boule du tableau 'bou'.
                            // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                            l--;
                            // Positionnement des boules autour du vaisseau appartenant au joueur.
                            if(nbr_boules)
                            {
                              bou[liste_bou[0]].pos_pts_cercle = 0;     // Indique la position du pointeur sur le tableau des points du cercle.
                              switch (nbr_boules)
                              {
                                case 2:
                                  bou[liste_bou[1]].pos_pts_cercle = 40;        // Indique la position du pointeur sur le tableau des points du cercle.
                                  break;
                                case 3:
                                  bou[liste_bou[1]].pos_pts_cercle = 26;        // Indique la position du pointeur sur le tableau des points du cercle.
                                  bou[liste_bou[2]].pos_pts_cercle = 53;        // Indique la position du pointeur sur le tableau des points du cercle.
                                  break;
                                case 4:
                                  bou[liste_bou[1]].pos_pts_cercle = 20;        // Indique la position du pointeur sur le tableau des points du cercle.
                                  bou[liste_bou[2]].pos_pts_cercle = 40;        // Indique la position du pointeur sur le tableau des points du cercle.
                                  bou[liste_bou[3]].pos_pts_cercle = 60;        // Indique la position du pointeur sur le tableau des points du cercle.
                                  break;
                                case 5:
                                  bou[liste_bou[1]].pos_pts_cercle = 16;        // Indique la position du pointeur sur le tableau des points du cercle.
                                  bou[liste_bou[2]].pos_pts_cercle = 32;        // Indique la position du pointeur sur le tableau des points du cercle.
                                  bou[liste_bou[3]].pos_pts_cercle = 48;        // Indique la position du pointeur sur le tableau des points du cercle.
                                  bou[liste_bou[4]].pos_pts_cercle = 64;        // Indique la position du pointeur sur le tableau des points du cercle.
                                  break;
                              }
                            }
                            // La boule est en détruite, on peux passer à la boule suivante.
                            goto fin_test_col_bou_tirs_nmi;
                          }
                          else
                          {                                    // Si la boule n'est pas détruite, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                            bou[cmpt_boules].affich_white = TRUE;
                          }
                          // On ajoute une petite explosion à la liste.
                          if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                            Add_Explosion((float)tirs[cmpt_tirs].spr.coor_x,
                                          (float)tirs[cmpt_tirs].spr.coor_y,
                                          0.35f, MINI_EXPLOSION, 0);
                          // On supprime le tir qui vient de toucher le vaisseau du joueur.
                          Del_Element_Tir(cmpt_tirs, i);       // Routine utilisée pour supprimer un tir du tableau des tirs.
                          // On se positionne sur le tir que l'on vient de décaler.
                          i--;
                          // On sort de la boucle qui test les collisions des tirs.
                          goto fin_bcle_tst_col_ennemi;
                        }
                      }
                    }
                  fin_test_col_bou_tirs_nmi:;
                  }

                  // Test les collisions entre les canons et le tir actuel.
                  // Bcles sur tous les canons du joueur.
                  for(l = 0; l < nbr_canons; l++)
                  {                                            // Retourne l'indice dans le tableau 'can' du nième élément de la liste.
                    cmpt_canons = liste_can[l];
                    // Test si le canon est visible et si oui, on peux tester les collisions.
                    if(can[cmpt_canons].visible)
                    {                                          // Bcle sur toutes les zones de collision du canon.
                      for(m = 0;
                          m <
                          can[cmpt_canons].img[can[cmpt_canons].img_act]->
                          nbr_zon_col; m++)
                      {                                        // Donne les coordonnée de la zone de collision actuelle du canon.
                        tmp_tstd_x =
                          can[cmpt_canons].coor_x +
                          can[cmpt_canons].img[can[cmpt_canons].img_act]->
                          xy_zon_col[m][X];
                        tmp_tstd_y =
                          can[cmpt_canons].coor_y +
                          can[cmpt_canons].img[can[cmpt_canons].img_act]->
                          xy_zon_col[m][Y];
                        // Test s'il le point de collision du canon se trouve à l'intérieur de la zone de collision du vaisseau ennemi.
                        if(tmp_tsts_x >= tmp_tstd_x &&
                           tmp_tsts_y >= tmp_tstd_y &&
                           tmp_tsts_x <
                           (tmp_tstd_x +
                            can[cmpt_canons].img[can[cmpt_canons].img_act]->
                            lh_zon_col[m][L])
                           && tmp_tsts_y <
                           (tmp_tstd_y +
                            can[cmpt_canons].img[can[cmpt_canons].img_act]->
                            lh_zon_col[m][H]))
                        {                                      // Donne le nouvel état des dommages du canon.
                          can[cmpt_canons].etat_dommages -=
                            tirs[cmpt_tirs].spr.pow_of_dest;
                          // Test si le canon est détruit.
                          if(can[cmpt_canons].etat_dommages <= 0)
                          {                                    // On supprime le canon de la liste.
                            Del_Element_Can(cmpt_canons, l);   // Routine utilisée pour supprimer un canon du tableau 'can'.
                            // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                            l--;
                            // Le canon est en détruite, on peux passer à le canon suivante.
                            goto fin_test_col_can_tirs_nmi;
                          }
                          else
                          {                                    // Si le canon n'est pas détruite, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                            can[cmpt_canons].affich_white = TRUE;
                          }
                          // On ajoute une petite explosion à la liste.
                          if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                            Add_Explosion(tirs[cmpt_tirs].spr.coor_x,
                                          tirs[cmpt_tirs].spr.coor_y, 0.35f,
                                          MINI_EXPLOSION, 0);
                          // On supprime le tir qui vient de toucher le vaisseau du joueur.
                          Del_Element_Tir(cmpt_tirs, i);       // Routine utilisée pour supprimer un tir du tableau des tirs.
                          // On se positionne sur le tir que l'on vient de décaler.
                          i--;
                          // On sort de la bcle qui test les collisions des tirs.
                          goto fin_bcle_tst_col_ennemi;
                        }
                      }
                    }
                  fin_test_col_can_tirs_nmi:;
                  }



                }
              }                                                // Fin du test pour savoir si le joueur est Game Over ou non.
            }                                                  // Fin du test pour savoir si on est en présence d'un tir ennemi.
          }
            break;
            // Cas d'un tir dont la trajectoire est calculée à chaque VBLs (du type missile téléguidé).
          case 1:
          {                                                    // Recherche de l'image à afficher en fonction de l'angle.
            if(sign(tirs[cmpt_tirs].angle_tir) < 0)
              tirs[cmpt_tirs].img_angle =
                (short)((tirs[cmpt_tirs].angle_tir + DEUX_PI) / PI_SUR_16);
            else
              tirs[cmpt_tirs].img_angle =
                (short)(tirs[cmpt_tirs].angle_tir / PI_SUR_16);
            // Sauvegarde l'angle pour le calcul du prochain angle.
            tirs[cmpt_tirs].img_old_angle = tirs[cmpt_tirs].img_angle;
            // Pour éviter un indice de tableau négatif.
            tirs[cmpt_tirs].img_angle = (short)abs(tirs[cmpt_tirs].img_angle);
            // Pour éviter un angle de tir supérieur au nombre d'images du sprite.
            if(tirs[cmpt_tirs].img_angle >= tirs[cmpt_tirs].spr.nbr_img)
              tirs[cmpt_tirs].img_angle =
                (short)(tirs[cmpt_tirs].spr.nbr_img - 1);
            // Affichage de l'image du tir.
            putsprite3(tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_angle],
                       (unsigned int)tirs[cmpt_tirs].spr.coor_x,
                       (unsigned int)tirs[cmpt_tirs].spr.coor_y);

            // Test de collision du tir téléguidé.
            if(tirs[cmpt_tirs].spr.genre == AMI)               // Test si on est en présence d'un sprite AMI.
            {                                                  // On traite tous les vaisseaux ennemis existants.
              for(j = 0; j < nbr_nmis; j++)
              {                                                // Retourne l'indice dans le tableau 've' du nième élément de la liste.
                cmpt_nmis = liste_ve[j];
                // Test si le vaisseau ennemi est visible et vivant, sinon, il n'est pas utile de faire le test des collisions.
                // Test aussi si on est en présence d'un gardien en train d'apparaitre et dans ce cas, on ne test pas non plus les collisions.
                if(ve[cmpt_nmis].visible && !ve[cmpt_nmis].dead
                   && !(apparition_gardian
                        && ve[cmpt_nmis].deplacement == GARDIEN))
                {                                              // Bcle sur tous les points de collision du tir.
                  for(k = 0;
                      k <
                      tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_angle]->
                      nbr_pts_col; k++)
                  {                                            // Bcle sur toutes les zones de collision de l'ennemi.
                    for(l = 0;
                        l <
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        nbr_zon_col; l++)
                    {                                          // Test si le point de collision du tir se trouve dans la zone de collision de l'ennemi.
                      if(((short)tirs[cmpt_tirs].spr.coor_x +
                          tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_angle]->
                          x_cg +
                          tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_angle]->
                          xy_pts_col[k][X]) >=
                         ((short)ve[cmpt_nmis].spr.coor_x +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          xy_zon_col[l][X])
                         && ((short)tirs[cmpt_tirs].spr.coor_x +
                             tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].
                                                     img_angle]->x_cg +
                             tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].
                                                     img_angle]->
                             xy_pts_col[k][X]) <
                         ((short)ve[cmpt_nmis].spr.coor_x +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          xy_zon_col[l][X] +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          lh_zon_col[l][L])
                         && ((short)tirs[cmpt_tirs].spr.coor_y +
                             tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].
                                                     img_angle]->y_cg +
                             tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].
                                                     img_angle]->
                             xy_pts_col[k][Y]) >=
                         ((short)ve[cmpt_nmis].spr.coor_y +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          xy_zon_col[l][Y])
                         && ((short)tirs[cmpt_tirs].spr.coor_y +
                             tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].
                                                     img_angle]->y_cg +
                             tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].
                                                     img_angle]->
                             xy_pts_col[k][Y]) <
                         ((short)ve[cmpt_nmis].spr.coor_y +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          xy_zon_col[l][Y] +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          lh_zon_col[l][H]))
                      {                                        // On décrémente l'état des domages du vaisseau touché par le tir.
                        ve[cmpt_nmis].spr.etat_dommages -=
                          tirs[cmpt_tirs].spr.pow_of_dest;
                        // On test si le vaisseau ennemi est détruit.
                        if(ve[cmpt_nmis].spr.etat_dommages <= 0)
                        {                                      // Test si le vaisseau ennemi était une météorite.
                          if(ve[cmpt_nmis].type >= BIGMETEOR)
                          {                                    // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                            //Gene_Bonus_Malus_Meteor(ve[cmpt_nmis].spr.coor_x, ve[cmpt_nmis].spr.coor_y);
                            Gene_Bonus_Malus_Meteor(&ve[cmpt_nmis]);
                          }
                          else                                 // Sinon, on appel la routine normale de génération de bonus, malus.
                          {                                    // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                            //Gene_Bonus_Malus(ve[cmpt_nmis].spr.coor_x, ve[cmpt_nmis].spr.coor_y);
                            Gene_Bonus_Malus(&ve[cmpt_nmis]);
                          }
                          // Test si l'on est en présence d'un vaisseau spécial.
                          if(ve[cmpt_nmis].deplacement == SPECIAUX
                             || ve[cmpt_nmis].deplacement == GARDIEN)
                          {
                            Add_Serie_Of_Explosion(cmpt_nmis); // Routine utilisée pour ajouter une série d'explosion à un gros vaisseau.
                            // On ajoute un eclat à la liste des explosions.
                            Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       x_cg - 8,
                                       ve[cmpt_nmis].spr.coor_y +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       y_cg - 8, 1.0, 5, 0, 2);
                          }
                          else                                 // On ajoute une explosion à la liste.
                          {
                            if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                              Add_Explosion(ve[cmpt_nmis].spr.coor_x,
                                            ve[cmpt_nmis].spr.coor_y, 0.25,
                                            ve[cmpt_nmis].type, 0);
                            // On ajoute un eclat à la liste des explosions.
                            Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       x_cg - 8,
                                       ve[cmpt_nmis].spr.coor_y +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       y_cg - 8, 1.0, 3, 0, 1);
                          }
                          // Mise à jour du score du joueur pour la destruction d'un ennemi
                          player_score +=
                            ve[cmpt_nmis].spr.pow_of_dest << 2 << bonusx;
                          // Indique que le vaisseau ennemi doit clignoter avant de disparaitre.
                          Clignote_Nmi_Avant_Disparition(cmpt_nmis);
                        }
                        else
                        {                                      // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                          ve[cmpt_nmis].affich_white = TRUE;
                          //On test si on est en presence d'un gardien afin de mettre a jour sa barre d'energie.
                          if(ve[cmpt_nmis].deplacement == GARDIEN)
                            bar_nrj_guardian = TRUE;
                          //On test si on est en presence d'un vaisseau qui doit s'enerver.
                          //Pour ca, on tests tous les vaisseaux qui ne doivent pas s'enerver car ils sont moins nombreux.
                          if(ve[cmpt_nmis].type != NAGGYS)
                          {                                    // On décrémente la cadence de tir de l'ennemi (il vient d'etre touché alors ça l'as énervé).
                            ve[cmpt_nmis].cad_tir =
                              ve[cmpt_nmis].cad_tir >> 1;
                          }
                        }
                        // On ajoute une petite explosion à la liste.
                        if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                          Add_Explosion((float)tirs[cmpt_tirs].spr.coor_x,
                                        (float)tirs[cmpt_tirs].spr.coor_y,
                                        0.35f, MINI_EXPLOSION, 0);
                        // On supprime le tir qui vient de toucher le vaisseau ennemi.
                        Del_Element_Tir(cmpt_tirs, i);         // Routine utilisée pour supprimer un tir du tableau des tirs.
                        // On se positionne sur le tir que l'on vient de décaler.
                        i--;
                        // On sort de la bcle qui test les collisions des tirs.
                        goto fin_bcle_tst_col_ennemi;
                      }
                    }
                  }
                }
              }
            }
            // Test si la pause est activée ou pas.
            if(!player_pause && etat_menu == MENU_OFF)
            {                                                  // Donne la nouvelle coordonée X du tir.
              tirs[cmpt_tirs].spr.coor_x +=
                tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_old_angle]->x_cg;
              // Donne la nouvelle coordonée Y du tir.
              tirs[cmpt_tirs].spr.coor_y +=
                tirs[cmpt_tirs].spr.img[tirs[cmpt_tirs].img_old_angle]->y_cg;
            }
          }
            break;
        }                                                      // Fin du switch sur les différentes trajectoires de tir.
        // Test si la pause est activée ou pas.
        if(!player_pause && etat_menu == MENU_OFF)
          tirs[cmpt_tirs].tmp_life_tir--;                      // Décrémente le temps de vie du projectile.
        // Label utilisé pour sortir de la bcle qui test les collisions des tirs amis avec
        // les vaisseaux ennemis lorsqu'un tir à touché un des ennemis.
fin_bcle_tst_col_ennemi:;
      }                                                        // Fin du test pour savoir si on doit afficher le tir ou pas.
    }
    else                                                       // Sinon, le tir vient de terminer son temps de vie.
    {                                                          // On doit donc le supprimer du tableau.
      Del_Element_Tir(cmpt_tirs, i);                           // Routine utilisée pour supprimer un tir du tableau des tirs.
      // On se positionne sur le tir que l'on vient de décaler.
      i--;
    }
  }

}

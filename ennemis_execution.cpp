//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "ennemis_exection.cpp"
// created      : ?
// updates      : 2005-01-10
// id		: $Id: ennemis_execution.cpp,v 1.1.1.1 2005/01/25 05:52:39 gurumeditation Exp $
//-----------------------------------------------------------------------------
// functions    : void  ennemis_exection()
//                float Deplacement_Tir_X( float angle, float vitesse, float pos_x )
//                float Deplacement_Tir_Y( float angle, float vitesse, float pos_y )
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
extern short            nbr_nmis;                              //nombre d'enemis actuellement geres
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //structure des vaisseaux ennemis
extern int              liste_ve[MAX_NMI_ON_SCREEN];           //liste de tous les 've' geres actuellement
extern int              cmpt_nmis;                             //compteur nombre d'ennemis (dans les boucles)
extern curve            initial_curve[NBR_CURVE_MAX];          //structure des courbes de bezier
extern bool             aff_game_over;                         //1=affichage du game over en cours
extern short            nbr_boules;                            //nombre de boules actuellement geres
extern str_boule        bou[MAX_BOULES_ON_SCREEN];             // structure des boules
extern image            boule[MAX_TYP_OF_BOULES][NBR_IMAGES_BOULES];    //tableau des boules
extern int              cmpt_boules;                           //compteur du nombre de boules (dans les boucles)
extern bool             player_pause;                          //1=pause en cours
extern tir              tirs[NBR_TIRS_MAX];                    //tableau de tous les objets tirs
extern int              etat_menu;                             //MENU_OFF(false)=menu inactif ; MENU_ON(true)=menu actif
extern int              new_tir;                               //indice sur un nouvel objet 'tirs'
extern short            nbr_tirs;                              //nombre de tir actuellement geres
extern int              nbr_onde_choc;                         //nombre actuel d'onde de choc
extern int              num_level;                             //numero du niveau en cours
extern int              tmp_tsts_x;
extern int              tmp_tsts_y;
extern int              tmp_tstd_x;
extern int              tmp_tstd_y;
extern int              cmpt_exps;                             //compteur du nombre d'explosions (dans les boucles)
extern image            fire[MAX_TYP_OF_TIR][NBR_IMAGES_TIR];  //ableau des différents tirs
extern int              bonusx;                                //multiplicateur de points x2 ou x4 (bonus)
extern int              player_score;                          //
extern int              cmpt_canons;                           // Utilisé dans les bcles pour compter le nombre de canons.
extern short            nbr_canons;                            // Indique le nombre de canons actuellement gérés.
extern int              liste_can[MAX_CANONS_ON_SCREEN];       // Définit la liste de toutes les 'can' que l'on doit gérés à chaque VBLs.
extern str_boule        bou[MAX_BOULES_ON_SCREEN];             // Déclaration de la structure des boules.
extern image            boule[MAX_TYP_OF_BOULES][NBR_IMAGES_BOULES];    // Définit le tableau des différents ennemis.
extern int              cmpt_boules;                           // Utilisé dans les bcles pour compter le nombre de boules.
extern short            nbr_exps;                              // Indique le nombre d'explosions actuellement gérées.
extern int              liste_bou[MAX_BOULES_ON_SCREEN];       // Définit la liste de toutes les 'bou' que l'on doit gérés à chaque VBLs.
extern str_canon        can[MAX_CANONS_ON_SCREEN];             // Déclaration de la structure des canons.
extern vaisseau         vj[NBR_MAX_J];                         // declaration de la structure des vaisseaux des joueurs.
extern bool             game_over_player_one;                  // true=le joueur vient de perdre son vaisseau.(game over)
extern grid             grille;
extern bool             bar_nrj_player;
extern int              new_ve;                                // Indique l'indice du tableau corespondant à un nouvel objet 've'.
extern int              gardian_activity;
extern bool             bar_nrj_guardian;
extern image            enemi[MAX_TYP_OF_ENEMI + MAX_TYP_OF_S_ENEMI + MAX_TYP_OF_NMI_SPE + MAX_TYP_OF_OBJ_SPE][NBR_IMAGES_MAX]; // Définit le tableau des différents ennemis.
extern short            first_value, second_value, compteur, compteur1;
extern int              iNumeroGardien;                        // numero du gardien en cours

//..............................................................................
void ennemis_execution()
{
  static int              i, k, j, l, m;

  for(i = 0; i < nbr_nmis; i++)
  {
    cmpt_nmis = liste_ve[i];                                   //cmpt_nmis=index sur le tableau "v.........
    switch (ve[cmpt_nmis].deplacement)
    {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      //  deplacement=0 : phase courbe
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      case COURBE:
      {
        if(!player_pause && etat_menu == MENU_OFF
           && _score::getActionExecution() == 0)
        {
          ve[cmpt_nmis].pos_vaiss[POS_CURVE]++;                //change position du vaisseau dans la courbe
          if(ve[cmpt_nmis].pos_vaiss[POS_CURVE] >=
             initial_curve[ve[cmpt_nmis].num_courbe].nbr_pnt_curve)
          {
            Del_Element_Ve(cmpt_nmis, i);                      //supprimer un vaisseau ennemi du tableau des 've'
            i--;
            goto fin_dep_ve_courbe;
          }
          //nouvelles coordonees
          ve[cmpt_nmis].spr.coor_x += (float)initial_curve[ve[cmpt_nmis].num_courbe].delta_x[ve[cmpt_nmis].pos_vaiss[POS_CURVE]];       // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
          ve[cmpt_nmis].spr.coor_y += (float)initial_curve[ve[cmpt_nmis].num_courbe].delta_y[ve[cmpt_nmis].pos_vaiss[POS_CURVE]];       // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
        }
        //donne la valeur de l'image du vaisseau a afficher
        ve[cmpt_nmis].spr.img_act =
          initial_curve[ve[cmpt_nmis].num_courbe].angle[ve[cmpt_nmis].
                                                        pos_vaiss[POS_CURVE]];
        // Test si on doit ou non afficher le sprite.
        if(((short)ve[cmpt_nmis].spr.coor_x +
            ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->larg) <
           DEBUT_ECRAN_X
           || ((short)ve[cmpt_nmis].spr.coor_y +
               ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->haut) <
           DEBUT_ECRAN_Y
           || (short)ve[cmpt_nmis].spr.coor_y >= DEBUT_ECRAN_Y + HAUT_ECR_RAM
           || (short)ve[cmpt_nmis].spr.coor_x >= DEBUT_ECRAN_X + LARG_ECR_RAM)
        {                                                      // Indique que le sprite n'est pas visible, on n'a donc pas besoin de faire de tests de collision avec lui.
          ve[cmpt_nmis].visible = FALSE;
        }
        else                                                   // Sinon, on doit l'afficher.
        {
          if(ve[cmpt_nmis].dead)                               //ennemi est mort ?
          {
            if(Gere_Invincibilite_Ve(cmpt_nmis))               //vaisseau continue a clignoter ?
            {                                                  // RAZ de la variable de fin de vie du vaisseau ennemi.
              ve[cmpt_nmis].dead = FALSE;                      //enemy's really dead
              Del_Element_Ve(cmpt_nmis, i);                    // Routine utilisée pour supprimer un vaisseau ennemi du tableau des 've'.
              // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
              i--;
              goto fin_dep_ve_courbe;                          // On sort de la bcle directement.
            }
          }
          else                                                 // Sinon, l'ennemi est encore en vie.
          {                                                    // Indique que le sprite est visible, on devra donc faire les tests de collision avec lui.
            ve[cmpt_nmis].visible = TRUE;
            // Affichage de l'ennemi.
            Aff_Ve(cmpt_nmis);
            // Test s'il y a des ondes de choc en activité.
            if(nbr_onde_choc)
            {                                                  // Test si l'onde de choc est en collision avec l'ennemi actuel.
              if(Tst_Col_Onde_Choc(cmpt_nmis))
              {                                                // On fait clignoter l'ennemi actuel avant de le faire disparaitre.
                Clignote_Nmi_Avant_Disparition(cmpt_nmis);
                // On ajoute une explosion à la liste.
                if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                  Add_Explosion(ve[cmpt_nmis].spr.coor_x,
                                ve[cmpt_nmis].spr.coor_y, 0.25,
                                ve[cmpt_nmis].type, 0);
                // On ajoute un eclat à la liste des explosions.
                Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                           ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                           x_cg - 8,
                           ve[cmpt_nmis].spr.coor_y +
                           ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                           y_cg - 8, 1.0, 4, 0, 1);
                goto fin_dep_ve_courbe;                        // Sort de la gestion des vaisseau ennemis.
              }
            }

            // Test si la pause est activée ou pas.
            if(!player_pause && etat_menu == MENU_OFF
               && _score::getActionExecution() == 0)
            {                                                  // Décompte le temps séparant deux tirs.
              ve[cmpt_nmis].cad_tir--;

              //test si l'ennemi doit ou non lancer un tir ----------------------
              if(ve[cmpt_nmis].cad_tir <= 0
                 && nbr_tirs <
                 ((NBR_TIRS_MAX - 1) -
                  ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                  nbr_pts_tir))
              {                                                // Met la cadence du tir pour le vaisseau ennemi.
                ve[cmpt_nmis].cad_tir = ve[cmpt_nmis].cad_tir_initiale; // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
                for(k = 0;
                    (k <
                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                     nbr_pts_tir
                     && nbr_tirs <
                     ((NBR_TIRS_MAX - 1) -
                      ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                      nbr_pts_tir)); k++)
                {
                  if(nbr_tirs < (NBR_TIRS_MAX - 1))            //reste tirs ?
                  {
                    new_tir = new_element_tir();               //demande un tir libre
                    tirs[new_tir].clignote = 1;                //1=tir clignote
                    tirs[new_tir].spr.genre = ENNEMI;          // Indique que c'est un sprite ennemi.
                    tirs[new_tir].spr.trajectoire = FALSE;     // Indique que la trajectoire du sprite est calculée une fois pour toute.
                    tirs[new_tir].spr.nbr_img = 32;            // Indique le nombre d'images composant le sprite.
                    // Test si le type d'ennemi afin d'attribuer la puissance du tir.
                    switch (ve[cmpt_nmis].type)
                    {                                          // Cas d'un ennemi de 16x16.
                      case 0:
                        tirs[new_tir].spr.pow_of_dest = 2;     // Indique la puissance de destruction du sprite.
                        // Donne l'adresse des images du sprite.
                        for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
                        {                                      //Donne l'adresse des images du projectile.
                          tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P1E][j];      // Corespond au Tir 2 puissance 1.
                        }
                        break;
                        // Cas d'un ennemi de 32x32.
                      case 1:
                        tirs[new_tir].spr.pow_of_dest = 4;     // Indique la puissance de destruction du sprite.
                        // Donne l'adresse des images du sprite.
                        for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
                        {                                      //Donne l'adresse des images du projectile.
                          tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P2E][j];      // Corespond au Tir 2 puissance 2.
                        }
                        break;
                    }
                    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
                    //tirs[new_tir].spr.pilote = 0;              // Indique par qui est dirigé le sprite.
                    //tirs[new_tir].spr.mode_commande = 0;       // Indique le moyen par lequel on fait déplacer le sprite.
                    tirs[new_tir].spr.img_act = 0;             // Indique l'image actuelle.
                    tirs[new_tir].spr.nbr_tot_vbl = 1;         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
                    tirs[new_tir].spr.nbr_vbl_act = 0;         // Indique le nombre de VBL comptées actuellement.
                    // Donne les coordonnées d'affichage du sprite.
                    tirs[new_tir].img_angle =
                      ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                      angle_tir[k];
                    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
                    tirs[new_tir].spr.coor_x =
                      ve[cmpt_nmis].spr.coor_x +
                      ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                      xy_pos_tir[k][X] -
                      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
                    tirs[new_tir].spr.coor_y =
                      ve[cmpt_nmis].spr.coor_y +
                      ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                      xy_pos_tir[k][Y] -
                      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
                    tirs[new_tir].tmp_life_tir = 400;
                    // Donne un angle vertical pour le tir.
                    tirs[new_tir].angle_tir =
                      PI_SUR_16 * tirs[new_tir].img_angle;
                    tirs[new_tir].spr.vitesse = 1.0f + (float)num_level / 20.0f;        // Donne la vitesse de déplacement du sprite.
                  }
                }
              }
            }
            // Si le joueur n'est pas Game Over.
            if(!aff_game_over)
            {                                                  // Test les collisions entre les boules et le vaisseau ennemi.
              // Boucles sur toutes les boules du joueur.
              for(k = 0; k < nbr_boules; k++)
              {                                                // Retourne l'indice dans le tableau 'bou' du nième élément de la liste.
                cmpt_boules = liste_bou[k];
                // Test si la boule est visible et si oui, on peux tester les collisions.
                if(bou[cmpt_boules].visible)
                {                                              // Boucle sur tous les points de collision de la boule.
                  for(l = 0;
                      l <
                      bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                      nbr_pts_col; l++)
                  {                                            // Donne les coordonnée du point de collision actuel de la boule.
                    tmp_tsts_x =
                      bou[cmpt_boules].coor_x +
                      bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                      xy_pts_col[l][X];
                    tmp_tsts_y =
                      bou[cmpt_boules].coor_y +
                      bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                      xy_pts_col[l][Y];
                    // Boucle sur toutes les zones de collisions du vaisseau ennemi.
                    for(m = 0;
                        m <
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        nbr_zon_col; m++)
                    {                                          // Test si le point de collision de la boule se trouve à l'intérieur de la
                      // zone de collision du vaisseau ennemi.
                      tmp_tstd_x =
                        (int)ve[cmpt_nmis].spr.coor_x +
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        xy_zon_col[m][X];
                      tmp_tstd_y =
                        (int)ve[cmpt_nmis].spr.coor_y +
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        xy_zon_col[m][Y];
                      // Test s'il le point de collision de la boule se trouve à l'intérieur de la zone de collision du vaisseau ennemi.
                      if(tmp_tsts_x >= tmp_tstd_x &&
                         tmp_tsts_y >= tmp_tstd_y &&
                         tmp_tsts_x <
                         (tmp_tstd_x +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          lh_zon_col[m][L])
                         && tmp_tsts_y <
                         (tmp_tstd_y +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          lh_zon_col[m][H]))
                      {                                        // On doit décrémenter l'état des dommages du vaisseau ennemi et de la boule.
                        // Donne le nouvel état des dommages du vaisseau ennemi.
                        ve[cmpt_nmis].spr.etat_dommages -=
                          bou[cmpt_boules].pow_of_dest;
                        // Donne le nouvel état des dommages de la boule du joueur.
                        bou[cmpt_boules].etat_dommages -=
                          ve[cmpt_nmis].spr.pow_of_dest;
                        // Test si la boule est détruite.
                        if(bou[cmpt_boules].etat_dommages <= 0)
                        {                                      // On supprime la boule de la liste.
                          Del_Element_Bou(cmpt_boules, k);     // Routine utilisée pour supprimer une boule du tableau 'bou'.
                          // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                          k--;
                          // Positionnement des boules autour du vaisseau appartenant au joueur.
                          if(nbr_boules)
                          {
                            bou[liste_bou[0]].pos_pts_cercle = 0;       // Indique la position du pointeur sur le tableau des points du cercle.
                            switch (nbr_boules)
                            {
                              case 2:
                                bou[liste_bou[1]].pos_pts_cercle = 40;  // Indique la position du pointeur sur le tableau des points du cercle.
                                break;
                              case 3:
                                bou[liste_bou[1]].pos_pts_cercle = 26;  // Indique la position du pointeur sur le tableau des points du cercle.
                                bou[liste_bou[2]].pos_pts_cercle = 53;  // Indique la position du pointeur sur le tableau des points du cercle.
                                break;
                              case 4:
                                bou[liste_bou[1]].pos_pts_cercle = 20;  // Indique la position du pointeur sur le tableau des points du cercle.
                                bou[liste_bou[2]].pos_pts_cercle = 40;  // Indique la position du pointeur sur le tableau des points du cercle.
                                bou[liste_bou[3]].pos_pts_cercle = 60;  // Indique la position du pointeur sur le tableau des points du cercle.
                                break;
                              case 5:
                                bou[liste_bou[1]].pos_pts_cercle = 16;  // Indique la position du pointeur sur le tableau des points du cercle.
                                bou[liste_bou[2]].pos_pts_cercle = 32;  // Indique la position du pointeur sur le tableau des points du cercle.
                                bou[liste_bou[3]].pos_pts_cercle = 48;  // Indique la position du pointeur sur le tableau des points du cercle.
                                bou[liste_bou[4]].pos_pts_cercle = 64;  // Indique la position du pointeur sur le tableau des points du cercle.
                                break;
                            }
                          }
                          // La boule est en détruite, on peux passer à la boule suivante.
                          goto fin_test_col_bou_nmi_courbe;
                        }
                        else
                        {                                      // Si la boule n'est pas détruite, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                          bou[cmpt_boules].affich_white = TRUE;
                        }
                        // On test si le vaisseau ennemi est détruit.
                        if(ve[cmpt_nmis].spr.etat_dommages <= 0)
                        {                                      // Test si le vaisseau ennemi était une météorite.
                          if(ve[cmpt_nmis].type >= BIGMETEOR
                             && ve[cmpt_nmis].type <= SMALLMETEOR)
                          {
                            //Gene_Bonus_Malus_Meteor(ve[cmpt_nmis].spr.coor_x, ve[cmpt_nmis].spr.coor_y); 
                            Gene_Bonus_Malus_Meteor(&ve[cmpt_nmis]);
                            Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       x_cg - 8,
                                       ve[cmpt_nmis].spr.coor_y +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       y_cg - 8, 1.0, 4, 0, 2);
                          }
                          else                                 // Sinon, on appel la routine normale de génération de bonus, malus.
                          {                                    // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                            //Gene_Bonus_Malus(ve[cmpt_nmis].spr.coor_x, ve[cmpt_nmis].spr.coor_y);
                            Gene_Bonus_Malus(&ve[cmpt_nmis]);
                          }
                          // On ajoute une explosion à la liste.
                          if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                            Add_Explosion(ve[cmpt_nmis].spr.coor_x,
                                          ve[cmpt_nmis].spr.coor_y, 0.25,
                                          ve[cmpt_nmis].type, 0);
                          // On ajoute un eclat à la liste des explosions.
                          Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                           img_act]->x_cg - 8,
                                     ve[cmpt_nmis].spr.coor_y +
                                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                           img_act]->y_cg - 8,
                                     1.0, 3, 0, 1);
                          // Mise à jour du score du joueur pour la destruction d'un ennemi.
                          player_score +=
                            ve[cmpt_nmis].spr.pow_of_dest << 2 << bonusx;
                          // Indique que le vaisseau ennemi doit clignoter avant de disparaitre.
                          Clignote_Nmi_Avant_Disparition(cmpt_nmis);
                          goto fin_dep_ve_courbe;
                        }
                        else
                        {                                      // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                          ve[cmpt_nmis].affich_white = TRUE;
                        }
                        // On ajoute une mini explosion à la liste car une boule à eu une collision.
                        if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                          Add_Explosion((float)tmp_tsts_x, (float)tmp_tsts_y,
                                        0.3f, MINI_EXPLOSION, 0);
                        // La boule est en collision avec l'ennemi, on peux passer à la boule suivante.
                        goto fin_test_col_bou_nmi_courbe;
                      }
                    }
                  }
                }
              fin_test_col_bou_nmi_courbe:;
              }

              // Test les collisions entre les canons et le vaisseau ennemi.
              // Bcles sur tous les canons du joueur.
              for(k = 0; k < nbr_canons; k++)
              {                                                // Retourne l'indice dans le tableau 'can' du nième élément de la liste.
                cmpt_canons = liste_can[k];
                // Test si le canon est visible et si oui, on peux tester les collisions.
                if(can[cmpt_canons].visible)
                {                                              // Bcle sur tous les points de collision du canon.
                  for(l = 0;
                      l <
                      can[cmpt_canons].img[can[cmpt_canons].img_act]->
                      nbr_pts_col; l++)
                  {                                            // Donne les coordonnée du point de collision actuel du canon.
                    tmp_tsts_x =
                      can[cmpt_canons].coor_x +
                      can[cmpt_canons].img[can[cmpt_canons].img_act]->
                      xy_pts_col[l][X];
                    tmp_tsts_y =
                      can[cmpt_canons].coor_y +
                      can[cmpt_canons].img[can[cmpt_canons].img_act]->
                      xy_pts_col[l][Y];
                    // Bcle sur toutes les zones de collisions du vaisseau ennemi.
                    for(m = 0;
                        m <
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        nbr_zon_col; m++)
                    {                                          // Donne les coordonées du point supérieur gauche de la
                      // zone de collision du vaisseau ennemi.
                      tmp_tstd_x =
                        (int)ve[cmpt_nmis].spr.coor_x +
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        xy_zon_col[m][X];
                      tmp_tstd_y =
                        (int)ve[cmpt_nmis].spr.coor_y +
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        xy_zon_col[m][Y];
                      // Test s'il le point de collision du canon se trouve à l'intérieur de la zone de collision du vaisseau ennemi.
                      if(tmp_tsts_x >= tmp_tstd_x &&
                         tmp_tsts_y >= tmp_tstd_y &&
                         tmp_tsts_x <
                         (tmp_tstd_x +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          lh_zon_col[m][L])
                         && tmp_tsts_y <
                         (tmp_tstd_y +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          lh_zon_col[m][H]))
                      {                                        // On doit décrémenter l'état des dommages du vaisseau ennemi et du canon.
                        // Donne le nouvel état des dommages du vaisseau ennemi.
                        ve[cmpt_nmis].spr.etat_dommages -=
                          can[cmpt_canons].pow_of_dest;
                        // Donne le nouvel état des dommages du canon.
                        can[cmpt_canons].etat_dommages -=
                          ve[cmpt_nmis].spr.pow_of_dest;
                        // Test si le canon est détruit.
                        if(can[cmpt_canons].etat_dommages <= 0)
                        {                                      // On supprime le canon de la liste.
                          Del_Element_Can(cmpt_canons, k);     // Routine utilisée pour supprimer un canon du tableau 'can'.
                          // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                          k--;
                          // Le canon est en détruit, on peux passer au canon suivant.
                          goto fin_test_col_can_nmi_courbe;
                        }
                        else
                        {                                      // Si le canon n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                          can[cmpt_canons].affich_white = TRUE;
                        }
                        // On test si le vaisseau ennemi est détruit.
                        if(ve[cmpt_nmis].spr.etat_dommages <= 0)
                        {                                      // Test si le vaisseau ennemi était une météorite.
                          if(ve[cmpt_nmis].type >= BIGMETEOR
                             && ve[cmpt_nmis].type <= SMALLMETEOR)
                          {                                    // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                            //Gene_Bonus_Malus_Meteor(ve[cmpt_nmis].spr.coor_x, ve[cmpt_nmis].spr.coor_y);
                            Gene_Bonus_Malus_Meteor(&ve[cmpt_nmis]);
                            // On ajoute un eclat à la liste des explosions.
                            Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       x_cg - 8,
                                       ve[cmpt_nmis].spr.coor_y +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       y_cg - 8, 1.0, 4, 0, 2);
                          }
                          else                                 // Sinon, on appel la routine normale de génération de bonus, malus.
                          {                                    // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                            //Gene_Bonus_Malus(ve[cmpt_nmis].spr.coor_x, ve[cmpt_nmis].spr.coor_y);
                            Gene_Bonus_Malus(&ve[cmpt_nmis]);
                          }
                          // On ajoute une explosion à la liste.
                          if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                            Add_Explosion(ve[cmpt_nmis].spr.coor_x,
                                          ve[cmpt_nmis].spr.coor_y, 0.25,
                                          ve[cmpt_nmis].type, 0);
                          // On ajoute un eclat à la liste des explosions.
                          Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                           img_act]->x_cg - 8,
                                     ve[cmpt_nmis].spr.coor_y +
                                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                           img_act]->y_cg - 8,
                                     1.0, 3, 0, 1);
                          // Mise à jour du score du joueur pour la destruction d'un ennemi.
                          player_score +=
                            ve[cmpt_nmis].spr.pow_of_dest << 2 << bonusx;
                          // Indique que le vaisseau ennemi doit clignoter avant de disparaitre.
                          Clignote_Nmi_Avant_Disparition(cmpt_nmis);
                          goto fin_dep_ve_courbe;
                        }
                        else
                        {                                      // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                          ve[cmpt_nmis].affich_white = TRUE;
                        }
                        // On ajoute une mini explosion à la liste car un canon à eu une collision.
                        if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                          Add_Explosion((float)tmp_tsts_x, (float)tmp_tsts_y,
                                        0.3f, MINI_EXPLOSION, 0);
                        // Le canon est en collision avec l'ennemi, on peux passer au caono suivant.
                        goto fin_test_col_can_nmi_courbe;
                      }
                    }
                  }
                }
              fin_test_col_can_nmi_courbe:;
              }

              // Test si l'ennemi est en collision avec le joueur.
              // Bcle sur tous les points de collisions du vaisseau du joueur.
              for(k = 0; k < vj[J1].spr.img[vj[J1].spr.img_act]->nbr_pts_col;
                  k++)
              {                                                // Donne les coordonées du point de collision du vaisseau appartenant au joueur.
                tmp_tsts_x =
                  (int)vj[J1].spr.coor_x +
                  vj[J1].spr.img[vj[J1].spr.img_act]->xy_pts_col[k][X];
                tmp_tsts_y =
                  (int)vj[J1].spr.coor_y +
                  vj[J1].spr.img[vj[J1].spr.img_act]->xy_pts_col[k][Y];
                // Bcle sur toutes les zones de collisions du vaisseau ennemi.
                for(l = 0;
                    l <
                    ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                    nbr_zon_col; l++)
                {                                              // Donne les coordonées de la zone de collision du vaisseau ennemi.
                  tmp_tstd_x =
                    (int)ve[cmpt_nmis].spr.coor_x +
                    ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                    xy_zon_col[l][X];
                  tmp_tstd_y =
                    (int)ve[cmpt_nmis].spr.coor_y +
                    ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                    xy_zon_col[l][Y];
                  // Test si le point de collision du vaisseau du joueur se trouve à l'intérieur de la
                  // zone de collision du vaisseau ennemi.
                  if(tmp_tsts_x >= tmp_tstd_x &&
                     tmp_tsts_y >= tmp_tstd_y &&
                     tmp_tsts_x <
                     tmp_tstd_x +
                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                     lh_zon_col[l][L]
                     && tmp_tsts_y <
                     tmp_tstd_y +
                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                     lh_zon_col[l][H])
                  {                                            // On doit décrémenter l'état des dommages des deux vaisseaux.
                    // Donne le nouvel état des dommages du vaisseau ennemi.
                    ve[cmpt_nmis].spr.etat_dommages -=
                      (vj[J1].spr.pow_of_dest << 1);
                    // Test si le vaisseau du joueur est invincible dans le but de décendre son énergie.
                    if(!vj[J1].invincible)
                    {                                          // Test si l'énergie du joueur était au maximum.
                      if(vj[J1].spr.etat_dommages == vj[J1].spr.pow_of_dest)
                      {                                        // Indique que l'animation d'ouverture de l'option d'energie doit s'effectuer.
                        Init_Anim_Option(1, FALSE);
                      }
                      // Donne le nouvel état des dommages du vaisseau du joueur.
                      vj[J1].spr.etat_dommages -=
                        ve[cmpt_nmis].spr.pow_of_dest;
                    }
                    // Demande un réaffichage de la barre d'énergie du joueur.
                    bar_nrj_player = TRUE;
                    // Test si le vaisseau du joueur est détruit.
                    if(vj[J1].spr.etat_dommages <= 0)
                    {                                          // Indique que le joueur est Game Over.
                      game_over_player_one = TRUE;
                    }
                    else
                    {                                          // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                      vj[J1].affich_white = TRUE;
                    }
                    // On ajoute la vitesse de déplacement du vaisseau ennemi à celle du vaisseau ami.
                    vj[J1].vitesse_x += (float)initial_curve[ve[cmpt_nmis].num_courbe].delta_x[ve[cmpt_nmis].pos_vaiss[POS_CURVE]];     // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
                    vj[J1].vitesse_y += (float)initial_curve[ve[cmpt_nmis].num_courbe].delta_y[ve[cmpt_nmis].pos_vaiss[POS_CURVE]];     // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
                    // On test si le vaisseau ennemi est détruit.
                    if(ve[cmpt_nmis].spr.etat_dommages <= 0)
                    {                                          // On ajoute une explosion à la liste.
                      if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                        Add_Explosion(ve[cmpt_nmis].spr.coor_x,
                                      ve[cmpt_nmis].spr.coor_y, 0.25,
                                      ve[cmpt_nmis].type, 0);
                      // On ajoute un eclat à la liste des explosions.
                      Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                 ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                       img_act]->x_cg - 8,
                                 ve[cmpt_nmis].spr.coor_y +
                                 ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                       img_act]->y_cg - 8,
                                 1.0, 4, 0, 2);
                      // Mise à jour du score du joueur pour la destruction d'un ennemi
                      player_score +=
                        ve[cmpt_nmis].spr.pow_of_dest << 2 << bonusx;
                      // Indique que le vaisseau ennemi doit clignoter avant de disparaitre.
                      Clignote_Nmi_Avant_Disparition(cmpt_nmis);
                      goto fin_dep_ve_courbe;
                    }
                    else
                    {                                          // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                      ve[cmpt_nmis].affich_white = TRUE;
                    }
                    // Le vaisseau du joueur est entré en collision avec le vaisseau ennemi, on peux sortir du test.
                    goto fin_tst_col_vnmi_dep_ve_courbe;
                  }
                }
              }                                                // Fin du test pour savoir si le joueur est en collision avec un vaisseau ennemi.
            fin_tst_col_vnmi_dep_ve_courbe:;
            }                                                  // Fin du test si le joueur est Game Over.
          }                                                    // Fin du test pour savoir si l'ennemi est mort et si on est en train de le faire clignoter.
        }                                                      // Fin du test si on doit afficher le vaisseau ennemi ou non.
      fin_dep_ve_courbe:;
      }
        break;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        // deplacement=1 : phase grille
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      case GRILLE:
      {                                                        // Test si la pause est activée ou pas.
        if(!player_pause && etat_menu == MENU_OFF
           && _score::getActionExecution() == 0)
        {                                                      // Donne la nouvelle coordonée X du sprite ennemi.
          ve[cmpt_nmis].spr.coor_x =
            grille.coor_x + (ve[cmpt_nmis].pos_vaiss[X] << 4);
          // Donne la nouvelle coordonée Y du sprite ennemi.
          ve[cmpt_nmis].spr.coor_y =
            grille.coor_y + (ve[cmpt_nmis].pos_vaiss[Y] << 4);
        }
        // Test si on doit ou non afficher le sprite.
        if(((short)ve[cmpt_nmis].spr.coor_x +
            ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->larg) <
           DEBUT_ECRAN_X
           || ((short)ve[cmpt_nmis].spr.coor_y +
               ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->haut) <
           DEBUT_ECRAN_Y
           || (short)ve[cmpt_nmis].spr.coor_y >= DEBUT_ECRAN_Y + HAUT_ECR_RAM
           || (short)ve[cmpt_nmis].spr.coor_x >= DEBUT_ECRAN_X + LARG_ECR_RAM)
        {                                                      // Indique que le sprite n'est pas visible, on n'a donc pas besoin de faire de tests de collision avec lui.
          ve[cmpt_nmis].visible = FALSE;
          // Test si l'ennemi se trouve en dehors des coordonées de l'écran.
          if((short)ve[cmpt_nmis].spr.coor_y >= DEBUT_ECRAN_Y + HAUT_ECR_RAM)
          {                                                    // On enlève le vaisseau ennemi de la liste des vaisseaux ennemis.
            Del_Element_Ve(cmpt_nmis, i);                      // Routine utilisée pour supprimer un vaisseau ennemi du tableau des 've'.
            // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
            i--;
          }
        }
        else                                                   // Sinon, on doit l'afficher.
        {                                                      // Test si le vaisseau ennemi est mort.
          if(ve[cmpt_nmis].dead)
          {                                                    // Gestion de l'invincibilité d'un vaisseau ennemi.
            // Retourne Vrai si le vaisseau ennemi est détruit, Faux si on continu à le faire clignoter.
            if(Gere_Invincibilite_Ve(cmpt_nmis))
            {                                                  // RAZ de la variable de fin de vie du vaisseau ennemi.
              ve[cmpt_nmis].dead = FALSE;
              // On enlève le vaisseau ennemi de la liste des vaisseaux ennemis.
              Del_Element_Ve(cmpt_nmis, i);                    // Routine utilisée pour supprimer un vaisseau ennemi du tableau des 've'.
              // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
              i--;
              goto fin_dep_ve_grille;                          // On sort de la bcle directement.
            }
          }
          else                                                 // Sinon, l'ennemi est encore en vie.
          {                                                    // Indique que le sprite est visible, on devra donc faire les tests de collision avec lui.
            ve[cmpt_nmis].visible = TRUE;
            // Incrémente le compteur de VBLs pour l'affichage de l'animation.
            ve[cmpt_nmis].spr.nbr_vbl_act++;
            // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
            if(ve[cmpt_nmis].spr.nbr_vbl_act >= ve[cmpt_nmis].spr.nbr_tot_vbl)
            {                                                  // On initialise le compteur de VBLs.
              ve[cmpt_nmis].spr.nbr_vbl_act = 0;
              // On passe à l'image suivante.
              ve[cmpt_nmis].spr.img_act++;
              // Test si le numéro de l'image actuelle n'est pas supérieur au nombre d'images totales.
              if(ve[cmpt_nmis].spr.img_act >= ve[cmpt_nmis].spr.nbr_img)
              {                                                // On repositionne l'image actuelle sur la première image de l'animation.
                ve[cmpt_nmis].spr.img_act = 0;
              }
            }
            // Affichage de l'ennemi.
            Aff_Ve(cmpt_nmis);
            // Test s'il y a des ondes de choc en activité.
            if(nbr_onde_choc)
            {                                                  // Test si l'onde de choc est en collision avec l'ennemi actuel.
              if(Tst_Col_Onde_Choc(cmpt_nmis))
              {                                                // On fait clignoter l'ennemi actuel avant de le faire disparaitre.
                Clignote_Nmi_Avant_Disparition(cmpt_nmis);
                // On ajoute une explosion à la liste.
                if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                  Add_Explosion(ve[cmpt_nmis].spr.coor_x,
                                ve[cmpt_nmis].spr.coor_y, 0.25,
                                ve[cmpt_nmis].type, 0);
                // On ajoute un eclat à la liste des explosions.
                Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                           ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                           x_cg - 8,
                           ve[cmpt_nmis].spr.coor_y +
                           ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                           y_cg - 8, 1.5, 5, 0, 1);
                goto fin_dep_ve_grille;                        // Sort de la gestion des vaisseau ennemis.
              }
            }
            // Test si la pause est activée ou pas.
            if(!player_pause && etat_menu == MENU_OFF
               && _score::getActionExecution() == 0)
            {                                                  // Décompte le temps séparant deux tirs.
              ve[cmpt_nmis].cad_tir--;
              // Test si l'ennemi doit ou non lancer un tir.
              if(ve[cmpt_nmis].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
              {                                                // Met la cadence du tir pour le vaisseau ennemi.
                ve[cmpt_nmis].cad_tir =
                  grille.freq_tir[ve[cmpt_nmis].pos_vaiss[X]][ve[cmpt_nmis].
                                                              pos_vaiss[Y]];
                // Bcle sur tous les points de tirs existants.
                for(k = 0;
                    k <
                    ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                    nbr_pts_tir; k++)
                {                                              // Test s'il est possible d'ajouter un nouveau tir à la liste.
                  if(nbr_tirs < (NBR_TIRS_MAX - 1))
                  {                                            // Recherche d'un nouveau tir.
                    new_tir = new_element_tir();
                    // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
                    tirs[new_tir].clignote = TRUE;             // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
                    tirs[new_tir].spr.genre = ENNEMI;          // Indique que c'est un sprite ennemi.
                    tirs[new_tir].spr.trajectoire = FALSE;     // Indique que la trajectoire du sprite est calculée une fois pour toute.
                    tirs[new_tir].spr.nbr_img = 32;            // Indique le nombre d'images composant le sprite.
                    // Test si le type d'ennemi afin d'attribuer la puissance du tir.
                    switch (ve[cmpt_nmis].type)
                    {                                          // Cas d'un ennemi de 16x16.
                      case 0:
                        tirs[new_tir].spr.pow_of_dest = 2;     // Indique la puissance de destruction du sprite.
                        // Donne l'adresse des images du sprite.
                        for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
                        {                                      //Donne l'adresse des images du projectile.
                          tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P1E][j];      // Corespond au Tir 2 puissance 1.
                        }
                        break;
                        // Cas d'un ennemi de 32x32.
                      case 1:
                        tirs[new_tir].spr.pow_of_dest = 4;     // Indique la puissance de destruction du sprite.
                        // Donne l'adresse des images du sprite.
                        for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
                        {                                      //Donne l'adresse des images du projectile.
                          tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P2E][j];      // Corespond au Tir 2 puissance 2.
                        }
                        break;
                    }
                    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
                    //tirs[new_tir].spr.pilote = 0;              // Indique par qui est dirigé le sprite.
                    //tirs[new_tir].spr.mode_commande = 0;       // Indique le moyen par lequel on fait déplacer le sprite.
                    tirs[new_tir].spr.img_act = 0;             // Indique l'image actuelle.
                    tirs[new_tir].spr.nbr_tot_vbl = 1;         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
                    tirs[new_tir].spr.nbr_vbl_act = 0;         // Indique le nombre de VBL comptées actuellement.
                    // Donne les coordonnées d'affichage du sprite.
                    tirs[new_tir].img_angle =
                      ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                      angle_tir[k];
                    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
                    tirs[new_tir].spr.coor_x =
                      ve[cmpt_nmis].spr.coor_x +
                      ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                      xy_pos_tir[k][X] -
                      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
                    tirs[new_tir].spr.coor_y =
                      ve[cmpt_nmis].spr.coor_y +
                      ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                      xy_pos_tir[k][Y] -
                      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
                    tirs[new_tir].tmp_life_tir = 400;
                    // Donne un angle vertical pour le tir.
                    tirs[new_tir].angle_tir =
                      PI_SUR_16 * tirs[new_tir].img_angle;
                    tirs[new_tir].spr.vitesse = 1.0f + (float)num_level / 20.0f;        // Donne la vitesse de déplacement du sprite.
                  }
                }
              }
            }
            // Si le joueur n'est pas Game Over.
            if(!aff_game_over)
            {                                                  // Test les collisions entre les boules et le vaisseau ennemi.
              // Boucles sur toutes les boules du joueur.
              for(k = 0; k < nbr_boules; k++)
              {                                                // Retourne l'indice dans le tableau 'bou' du nième élément de la liste.
                cmpt_boules = liste_bou[k];
                // Test si la boule est visible et si oui, on peux tester les collisions.
                if(bou[cmpt_boules].visible)
                {                                              // Boucle sur tous les points de collision de la boule.
                  for(l = 0;
                      l <
                      bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                      nbr_pts_col; l++)
                  {                                            // Donne les coordonnée du point de collision actuel de la boule.
                    tmp_tsts_x =
                      bou[cmpt_boules].coor_x +
                      bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                      xy_pts_col[l][X];
                    tmp_tsts_y =
                      bou[cmpt_boules].coor_y +
                      bou[cmpt_boules].img[bou[cmpt_boules].img_act]->
                      xy_pts_col[l][Y];
                    // Boucle sur toutes les zones de collisions du vaisseau ennemi.
                    for(m = 0;
                        m <
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        nbr_zon_col; m++)
                    {                                          // Test si le point de collision de la boule se trouve à l'intérieur de la
                      // zone de collision du vaisseau ennemi.
                      tmp_tstd_x =
                        (int)ve[cmpt_nmis].spr.coor_x +
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        xy_zon_col[m][X];
                      tmp_tstd_y =
                        (int)ve[cmpt_nmis].spr.coor_y +
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        xy_zon_col[m][Y];
                      // Test s'il le point de collision de la boule se trouve à l'intérieur de la zone de collision du vaisseau ennemi.
                      if(tmp_tsts_x >= tmp_tstd_x &&
                         tmp_tsts_y >= tmp_tstd_y &&
                         tmp_tsts_x <
                         (tmp_tstd_x +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          lh_zon_col[m][L])
                         && tmp_tsts_y <
                         (tmp_tstd_y +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          lh_zon_col[m][H]))
                      {                                        // On doit décrémenter l'état des dommages du vaisseau ennemi et de la boule.
                        // Donne le nouvel état des dommages du vaisseau ennemi.
                        ve[cmpt_nmis].spr.etat_dommages -=
                          bou[cmpt_boules].pow_of_dest;
                        // Donne le nouvel état des dommages de la boule du joueur.
                        bou[cmpt_boules].etat_dommages -=
                          ve[cmpt_nmis].spr.pow_of_dest;
                        // Test si la boule est détruite.
                        if(bou[cmpt_boules].etat_dommages <= 0)
                        {                                      // On supprime la boule de la liste.
                          Del_Element_Bou(cmpt_boules, k);     // Routine utilisée pour supprimer une boule du tableau 'bou'.
                          // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                          k--;
                          // Positionnement des boules autour du vaisseau appartenant au joueur.
                          if(nbr_boules)
                          {
                            bou[liste_bou[0]].pos_pts_cercle = 0;       // Indique la position du pointeur sur le tableau des points du cercle.
                            switch (nbr_boules)
                            {
                              case 2:
                                bou[liste_bou[1]].pos_pts_cercle = 40;  // Indique la position du pointeur sur le tableau des points du cercle.
                                break;
                              case 3:
                                bou[liste_bou[1]].pos_pts_cercle = 26;  // Indique la position du pointeur sur le tableau des points du cercle.
                                bou[liste_bou[2]].pos_pts_cercle = 53;  // Indique la position du pointeur sur le tableau des points du cercle.
                                break;
                              case 4:
                                bou[liste_bou[1]].pos_pts_cercle = 20;  // Indique la position du pointeur sur le tableau des points du cercle.
                                bou[liste_bou[2]].pos_pts_cercle = 40;  // Indique la position du pointeur sur le tableau des points du cercle.
                                bou[liste_bou[3]].pos_pts_cercle = 60;  // Indique la position du pointeur sur le tableau des points du cercle.
                                break;
                              case 5:
                                bou[liste_bou[1]].pos_pts_cercle = 16;  // Indique la position du pointeur sur le tableau des points du cercle.
                                bou[liste_bou[2]].pos_pts_cercle = 32;  // Indique la position du pointeur sur le tableau des points du cercle.
                                bou[liste_bou[3]].pos_pts_cercle = 48;  // Indique la position du pointeur sur le tableau des points du cercle.
                                bou[liste_bou[4]].pos_pts_cercle = 64;  // Indique la position du pointeur sur le tableau des points du cercle.
                                break;
                            }
                          }
                          // La boule est en détruite, on peux passer à la boule suivante.
                          goto fin_test_col_bou_nmi_grille;
                        }
                        else
                        {                                      // Si la boule n'est pas détruite, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                          bou[cmpt_boules].affich_white = TRUE;
                        }
                        // On test si le vaisseau ennemi est détruit.
                        if(ve[cmpt_nmis].spr.etat_dommages <= 0)
                        {                                      // Test si le vaisseau ennemi était une météorite.
                          if(ve[cmpt_nmis].type >= BIGMETEOR
                             && ve[cmpt_nmis].type <= SMALLMETEOR)
                          {                                    // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                            //Gene_Bonus_Malus_Meteor(ve[cmpt_nmis].spr.coor_x, ve[cmpt_nmis].spr.coor_y);
                            Gene_Bonus_Malus_Meteor(&ve[cmpt_nmis]);
                            // On ajoute un eclat à la liste des explosions.
                            Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       x_cg - 8,
                                       ve[cmpt_nmis].spr.coor_y +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       y_cg - 8, 1.0, 2, 0, 2);
                          }
                          else                                 // Sinon, on appel la routine normale de génération de bonus, malus.
                          {                                    // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                            //Gene_Bonus_Malus(ve[cmpt_nmis].spr.coor_x, ve[cmpt_nmis].spr.coor_y);
                            Gene_Bonus_Malus(&ve[cmpt_nmis]);
                          }
                          // On ajoute une explosion à la liste.
                          if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                            Add_Explosion(ve[cmpt_nmis].spr.coor_x,
                                          ve[cmpt_nmis].spr.coor_y, 0.25,
                                          ve[cmpt_nmis].type, 0);
                          // On ajoute un eclat à la liste des explosions.
                          Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                           img_act]->x_cg - 8,
                                     ve[cmpt_nmis].spr.coor_y +
                                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                           img_act]->y_cg - 8,
                                     1.0, 3, 0, 1);
                          // Mise à jour du score du joueur pour la destruction d'un ennemi
                          player_score +=
                            ve[cmpt_nmis].spr.pow_of_dest << 2 << bonusx;
                          // Indique que le vaisseau ennemi doit clignoter avant de disparaitre.
                          Clignote_Nmi_Avant_Disparition(cmpt_nmis);
                          goto fin_dep_ve_grille;
                        }
                        else
                        {                                      // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                          ve[cmpt_nmis].affich_white = TRUE;
                        }
                        // On ajoute une mini explosion à la liste car une boule à eu une collision.
                        if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                          Add_Explosion((float)tmp_tsts_x, (float)tmp_tsts_y,
                                        0.3f, MINI_EXPLOSION, 0);
                        // La boule est en collision avec l'ennemi, on peux passer à la boule suivante.
                        goto fin_test_col_bou_nmi_grille;
                      }
                    }
                  }
                }
              fin_test_col_bou_nmi_grille:;
              }
              // Test les collisions entre les canons et le vaisseau ennemi.
              // Bcles sur toutes les canons du joueur.
              for(k = 0; k < nbr_canons; k++)
              {                                                // Retourne l'indice dans le tableau 'can' du nième élément de la liste.
                cmpt_canons = liste_can[k];
                // Test si le canon est visible et si oui, on peux tester les collisions.
                if(can[cmpt_canons].visible)
                {                                              // Bcle sur tous les points de collision du canon.
                  for(l = 0;
                      l <
                      can[cmpt_canons].img[can[cmpt_canons].img_act]->
                      nbr_pts_col; l++)
                  {                                            // Donne les coordonnée du point de collision actuel du canon.
                    tmp_tsts_x =
                      can[cmpt_canons].coor_x +
                      can[cmpt_canons].img[can[cmpt_canons].img_act]->
                      xy_pts_col[l][X];
                    tmp_tsts_y =
                      can[cmpt_canons].coor_y +
                      can[cmpt_canons].img[can[cmpt_canons].img_act]->
                      xy_pts_col[l][Y];
                    // Bcle sur toutes les zones de collisions du vaisseau ennemi.
                    for(m = 0;
                        m <
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        nbr_zon_col; m++)
                    {                                          // Donne les coordonées du coin supérieur gauche de la
                      // zone de collision du vaisseau ennemi.
                      tmp_tstd_x =
                        (int)ve[cmpt_nmis].spr.coor_x +
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        xy_zon_col[m][X];
                      tmp_tstd_y =
                        (int)ve[cmpt_nmis].spr.coor_y +
                        ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                        xy_zon_col[m][Y];
                      // Test s'il le point de collision du canon se trouve à l'intérieur de la zone de collision du vaisseau ennemi.
                      if(tmp_tsts_x >= tmp_tstd_x &&
                         tmp_tsts_y >= tmp_tstd_y &&
                         tmp_tsts_x <
                         (tmp_tstd_x +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          lh_zon_col[m][L])
                         && tmp_tsts_y <
                         (tmp_tstd_y +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          lh_zon_col[m][H]))
                      {                                        // On doit décrémenter l'état des dommages du vaisseau ennemi et du canon.
                        // Donne le nouvel état des dommages du vaisseau ennemi.
                        ve[cmpt_nmis].spr.etat_dommages -=
                          can[cmpt_canons].pow_of_dest;
                        // Donne le nouvel état des dommages du canon du joueur.
                        can[cmpt_canons].etat_dommages -=
                          ve[cmpt_nmis].spr.pow_of_dest;
                        // Test si le canon est détruite.
                        if(can[cmpt_canons].etat_dommages <= 0)
                        {                                      // On supprime le canon de la liste.
                          Del_Element_Can(cmpt_canons, k);     // Routine utilisée pour supprimer un canon du tableau 'can'.
                          // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                          k--;
                          // Le canon est en détruit, on peux passer au canon suivant.
                          goto fin_test_col_can_nmi_grille;
                        }
                        else
                        {                                      // Si le canon n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                          can[cmpt_canons].affich_white = TRUE;
                        }
                        // On test si le vaisseau ennemi est détruit.
                        if(ve[cmpt_nmis].spr.etat_dommages <= 0)
                        {                                      // Test si le vaisseau ennemi était une météorite.
                          if(ve[cmpt_nmis].type >= BIGMETEOR
                             && ve[cmpt_nmis].type <= SMALLMETEOR)
                          {                                    // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                            //Gene_Bonus_Malus_Meteor(ve[cmpt_nmis].spr.coor_x, ve[cmpt_nmis].spr.coor_y);
                            Gene_Bonus_Malus_Meteor(&ve[cmpt_nmis]);
                            //printf("725\n") ;
                            // On ajoute un eclat à la liste des explosions.
                            Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       x_cg - 8,
                                       ve[cmpt_nmis].spr.coor_y +
                                       ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                             spr.img_act]->
                                       y_cg - 8, 1.0, 2, 0, 2);
                          }
                          else                                 // Sinon, on appel la routine normale de génération de bonus, malus.
                          {                                    // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                            //Gene_Bonus_Malus(ve[cmpt_nmis].spr.coor_x, ve[cmpt_nmis].spr.coor_y);
                            Gene_Bonus_Malus(&ve[cmpt_nmis]);
                            //printf("732\n") ;
                          }
                          // On ajoute une explosion à la liste.
                          if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                            Add_Explosion(ve[cmpt_nmis].spr.coor_x,
                                          ve[cmpt_nmis].spr.coor_y, 0.25,
                                          ve[cmpt_nmis].type, 0);
                          // On ajoute un eclat à la liste des explosions.
                          Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                           img_act]->x_cg - 8,
                                     ve[cmpt_nmis].spr.coor_y +
                                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                           img_act]->y_cg - 8,
                                     1.0, 3, 0, 1);
                          // Mise à jour du score du joueur pour la destruction d'un ennemi
                          player_score +=
                            ve[cmpt_nmis].spr.pow_of_dest << 2 << bonusx;
                          // Indique que le vaisseau ennemi doit clignoter avant de disparaitre.
                          Clignote_Nmi_Avant_Disparition(cmpt_nmis);
                          goto fin_dep_ve_grille;
                        }
                        else
                        {                                      // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                          ve[cmpt_nmis].affich_white = TRUE;
                        }
                        // On ajoute une mini explosion à la liste car un canon à eu une collision.
                        if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                          Add_Explosion((float)tmp_tsts_x, (float)tmp_tsts_y,
                                        0.3f, MINI_EXPLOSION, 0);
                        // Le canon est en collision avec l'ennemi, on peux passer au canon suivant.
                        goto fin_test_col_can_nmi_grille;
                      }
                    }
                  }
                }
              fin_test_col_can_nmi_grille:;
              }

              // Test si l'ennemi est en collision avec le joueur.
              // Bcle sur tous les points de collisions du vaisseau du joueur.
              for(k = 0; k < vj[J1].spr.img[vj[J1].spr.img_act]->nbr_pts_col;
                  k++)
              {                                                // Donne les coordonées du point de collision du vaisseau appartenant au joueur.
                tmp_tsts_x =
                  (int)vj[J1].spr.coor_x +
                  vj[J1].spr.img[vj[J1].spr.img_act]->xy_pts_col[k][X];
                tmp_tsts_y =
                  (int)vj[J1].spr.coor_y +
                  vj[J1].spr.img[vj[J1].spr.img_act]->xy_pts_col[k][Y];
                // Bcle sur toutes les zones de collisions du vaisseau ennemi.
                for(l = 0;
                    l <
                    ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                    nbr_zon_col; l++)
                {                                              // Donne les coordonées de la zone de collision du vaisseau ennemi.
                  tmp_tstd_x =
                    (int)ve[cmpt_nmis].spr.coor_x +
                    ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                    xy_zon_col[l][X];
                  tmp_tstd_y =
                    (int)ve[cmpt_nmis].spr.coor_y +
                    ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                    xy_zon_col[l][Y];
                  // Test si le point de collision du vaisseau du joueur se trouve à l'intérieur de la
                  // zone de collision du vaisseau ennemi.
                  if(tmp_tsts_x >= tmp_tstd_x &&
                     tmp_tsts_y >= tmp_tstd_y &&
                     tmp_tsts_x <
                     tmp_tstd_x +
                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                     lh_zon_col[l][L]
                     && tmp_tsts_y <
                     tmp_tstd_y +
                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                     lh_zon_col[l][H])
                  {                                            // On doit décrémenter l'état des dommages des deux vaisseaux.
                    // Donne le nouvel état des dommages du vaisseau ennemi.
                    ve[cmpt_nmis].spr.etat_dommages -=
                      (vj[J1].spr.pow_of_dest << 1);
                    // Test si le vaisseau du joueur est invincible dans le but de décendre son énergie.
                    if(!vj[J1].invincible)
                    {                                          // Test si l'énergie du joueur était au maximum.
                      if(vj[J1].spr.etat_dommages == vj[J1].spr.pow_of_dest)
                      {                                        // Indique que l'animation d'ouverture de l'option d'energie doit s'effectuer.
                        Init_Anim_Option(1, FALSE);
                      }
                      // Donne le nouvel état des dommages du vaisseau du joueur.
                      vj[J1].spr.etat_dommages -=
                        ve[cmpt_nmis].spr.pow_of_dest;
                    }
                    // Demande un réaffichage de la barre d'énergie du joueur.
                    bar_nrj_player = TRUE;
                    // Test si le vaisseau du joueur est détruit.
                    if(vj[J1].spr.etat_dommages <= 0)
                    {                                          // Indique que le joueur est Game Over.
                      game_over_player_one = TRUE;
                    }
                    else
                    {                                          // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                      vj[J1].affich_white = TRUE;
                    }
                    // On ajoute la vitesse de déplacement du vaisseau ennemi à celle du vaisseau ami.
                    // Test le sens de déplacement de la grille.
                    if(grille.sens_dep == GAUCHE)
                    {                                          // Nouvelle vitesse du joueur après la collision avec un des vaisseaux de la grille.
                      vj[J1].vitesse_x -= grille.vit_dep_x;
                      vj[J1].vitesse_y += grille.vit_dep_y;
                    }
                    else                                       // Sinon, la grille se déplace vers la droite.
                    {                                          // Nouvelle vitesse du joueur après la collision avec un des vaisseaux de la grille.
                      vj[J1].vitesse_x += grille.vit_dep_x;
                      vj[J1].vitesse_y += grille.vit_dep_y;
                    }
                    // On test si le vaisseau ennemi est détruit du fait d'une collision avec le joueur.
                    if(ve[cmpt_nmis].spr.etat_dommages <= 0)
                    {                                          // On ajoute une explosion à la liste.
                      if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                        Add_Explosion(ve[cmpt_nmis].spr.coor_x,
                                      ve[cmpt_nmis].spr.coor_y, 0.25,
                                      ve[cmpt_nmis].type, 0);
                      // On ajoute un eclat à la liste des explosions.
                      Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                                 ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                       img_act]->x_cg - 8,
                                 ve[cmpt_nmis].spr.coor_y +
                                 ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                       img_act]->y_cg - 8,
                                 1.0, 3, 0, 1);
                      // Mise à jour du score du joueur pour la destruction d'un ennemi
                      player_score +=
                        ve[cmpt_nmis].spr.pow_of_dest << 2 << bonusx;
                      // Indique que le vaisseau ennemi doit clignoter avant de disparaitre.
                      Clignote_Nmi_Avant_Disparition(cmpt_nmis);
                      goto fin_dep_ve_grille;
                    }
                    else
                    {                                          // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                      ve[cmpt_nmis].affich_white = TRUE;
                    }
                    // Le vaisseau du joueur est entré en collision avec le vaisseau ennemi, on peux sortir du test.
                    goto fin_tst_col_vnmi_dep_ve_grille;
                  }
                }
              }                                                // Fin du test pour savoir si le joueur est en collision avec un vaisseau ennemi.
            fin_tst_col_vnmi_dep_ve_grille:;
            }                                                  // Fin du test si le joueur est Game Over.
          }                                                    // Fin du test pour savoir si l'ennemi est mort et si on est en train de le faire clignoter.
        }                                                      // Fin du test si on doit afficher le vaisseau ennemi ou non.
      fin_dep_ve_grille:;
      }
        break;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        // deplacement=2 : ennemis speciaux
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        // Cas des ennemis spéciaux qui traversent l'écran de part en part.
      case SPECIAUX:
      {                                                        // Test le type de vaisseau spéciaux.
        switch (ve[cmpt_nmis].type)
        {                                                      // Vaisseaux se déplacant comme un missile.
          case SOUKEE:
          {                                                    // Test si la pause est activée ou pas.
            if(!player_pause && etat_menu == MENU_OFF
               && _score::getActionExecution() == 0)
            {                                                  // Calcul du nouvel angle en fonction de l'ancien.
              ve[cmpt_nmis].angle_tir =
                Calcul_New_Angle(ve[cmpt_nmis].angle_tir,
                                 Calc_Angle_Tir((short)
                                                (ve[cmpt_nmis].spr.coor_x +
                                                 ve[cmpt_nmis].spr.
                                                 img[ve[cmpt_nmis].spr.
                                                     img_act]->x_cg),
                                                (short)(ve[cmpt_nmis].spr.
                                                        coor_y +
                                                        ve[cmpt_nmis].spr.
                                                        img[ve[cmpt_nmis].spr.
                                                            img_act]->y_cg),
                                                (short)(vj[J1].spr.coor_x +
                                                        vj[J1].spr.img[vj[J1].
                                                                       spr.
                                                                       img_act]->
                                                        x_cg),
                                                (short)(vj[J1].spr.coor_y +
                                                        vj[J1].spr.img[vj[J1].
                                                                       spr.
                                                                       img_act]->
                                                        y_cg)),
                                 ve[cmpt_nmis].agilite);

              // On donne les nouvelles coordonées du vaisseau ennemi.
              ve[cmpt_nmis].spr.coor_x =
                Deplacement_Tir_X(ve[cmpt_nmis].angle_tir,
                                  ve[cmpt_nmis].spr.vitesse,
                                  ve[cmpt_nmis].spr.coor_x -
                                  ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                        img_old_angle]->x_cg);
              ve[cmpt_nmis].spr.coor_y =
                Deplacement_Tir_Y(ve[cmpt_nmis].angle_tir,
                                  ve[cmpt_nmis].spr.vitesse,
                                  ve[cmpt_nmis].spr.coor_y -
                                  ve[cmpt_nmis].spr.img[ve[cmpt_nmis].
                                                        img_old_angle]->y_cg);

              // Recherche de l'image à afficher en fonction de l'angle.
              if(sign(ve[cmpt_nmis].angle_tir) < 0)
                ve[cmpt_nmis].img_angle =
                  (short)((ve[cmpt_nmis].angle_tir + DEUX_PI) / PI_SUR_16);
              else
                ve[cmpt_nmis].img_angle =
                  (short)(ve[cmpt_nmis].angle_tir / PI_SUR_16);
              // Sauvegarde l'angle pour le calcul du prochain angle.
              ve[cmpt_nmis].img_old_angle = ve[cmpt_nmis].img_angle;
              // Pour éviter un indice de tableau négatif.
              ve[cmpt_nmis].img_angle = (short)abs(ve[cmpt_nmis].img_angle);
              // Pour éviter un angle de tir supérieur au nombre d'images du sprite.
              if(ve[cmpt_nmis].img_angle >= ve[cmpt_nmis].spr.nbr_img)
                ve[cmpt_nmis].img_angle =
                  (short)(ve[cmpt_nmis].spr.nbr_img - 1);

              // Donne la nouvelle coordonée X du tir.
              ve[cmpt_nmis].spr.coor_x +=
                ve[cmpt_nmis].spr.img[ve[cmpt_nmis].img_old_angle]->x_cg;
              // Donne la nouvelle coordonée Y du tir.
              ve[cmpt_nmis].spr.coor_y +=
                ve[cmpt_nmis].spr.img[ve[cmpt_nmis].img_old_angle]->y_cg;
              //Donne la valeur de l'image a afficher.
              ve[cmpt_nmis].spr.img_act = ve[cmpt_nmis].img_angle;

            }

            // Test si on doit ou non afficher le sprite.
            if(((short)ve[cmpt_nmis].spr.coor_y +
                ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->haut) <
               DEBUT_ECRAN_Y
               || ((short)ve[cmpt_nmis].spr.coor_x +
                   ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->larg) <
               DEBUT_ECRAN_X
               || (short)ve[cmpt_nmis].spr.coor_y >=
               DEBUT_ECRAN_Y + HAUT_ECR_RAM
               || (short)ve[cmpt_nmis].spr.coor_x >=
               DEBUT_ECRAN_X + LARG_ECR_RAM)
            {                                                  // Indique que le sprite n'est pas visible, on n'a donc pas besoin de faire de tests de collision avec lui.
              ve[cmpt_nmis].visible = FALSE;
              // Test si l'ennemi se trouve en dehors des coordonées de l'écran.
              if((short)ve[cmpt_nmis].spr.coor_x < (BANDE_DE_CLIP - 64) ||
                 (short)ve[cmpt_nmis].spr.coor_y < (BANDE_DE_CLIP - 64) ||
                 (short)ve[cmpt_nmis].spr.coor_y >
                 (BANDE_DE_CLIP + HAUT_ECR_RAM + 64)
                 || (short)ve[cmpt_nmis].spr.coor_x >
                 (BANDE_DE_CLIP + LARG_ECR_RAM + 64))
              {                                                // On enlève le vaisseau ennemi de la liste des vaisseaux ennemis.
                Del_Element_Ve(cmpt_nmis, i);                  // Routine utilisée pour supprimer un vaisseau ennemi du tableau des 've'.
                // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                i--;
                // Le vaisseau est détruit, on peux sortir de la bcle.
                goto fin_dep_ve_speciaux;
              }
            }
            else                                               // Sinon, on doit l'afficher.
            {                                                  // Indique que le sprite est visible, on devra donc faire les tests de collision avec lui.
              ve[cmpt_nmis].visible = TRUE;
            }
          }
            break;


            // Vaisseaux se déplacant du haut de l'écran vers le bas et qui remontent.
          case VIONIEES:
          case ANGOUFF:
          case CLOWNIES:
          case EFFIES:
          case SAPOUCH:
          {                                                    // Test si la pause est activée ou pas.
            if(!player_pause && etat_menu == MENU_OFF
               && _score::getActionExecution() == 0)
            {                                                  // Test si le vaisseau ennemi est dans sa phase de retournement.
              if(ve[cmpt_nmis].retournement)
              {                                                // Incrémente le compteur de VBLs pour l'affichage de l'animation.
                ve[cmpt_nmis].spr.nbr_vbl_act++;
                // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
                if(ve[cmpt_nmis].spr.nbr_vbl_act >=
                   ve[cmpt_nmis].spr.nbr_tot_vbl)
                {                                              // On initialise le compteur de VBLs.
                  ve[cmpt_nmis].spr.nbr_vbl_act = 0;
                  // On passe à l'image suivante.
                  ve[cmpt_nmis].spr.img_act++;
                  // Test si le numéro de l'image actuelle n'est pas supérieur au nombre d'images totales.
                  if(ve[cmpt_nmis].spr.img_act >=
                     (ve[cmpt_nmis].spr.nbr_img - 1))
                  {                                            // On clip l'image de l'animation à afficher.
                    ve[cmpt_nmis].spr.img_act =
                      (short)(ve[cmpt_nmis].spr.nbr_img - 1);
                    // On indique que la phase de retournement du vaiseau ennemi est terminée.
                    ve[cmpt_nmis].retournement = FALSE;
                    // On indique que le vaisseau ennemi vient de changer de direction.
                    ve[cmpt_nmis].change_dir = TRUE;
                  }
                }
              }
              else                                             // Sinon, le vaisseau ennemi n'est pas dans sa phase de retournement.
              {                                                // Test si le vaisseau ennemi n'a pas encore changé de direction.
                if(!ve[cmpt_nmis].change_dir)
                {                                              // Test si la vitesse de déplacement est à zéro.
                  if(ve[cmpt_nmis].spr.vitesse >= -0.1
                     && ve[cmpt_nmis].spr.vitesse <= 0.1)
                  {                                            // On indique qu'il faut effectuer le retournement du vaisseau ennemi.
                    ve[cmpt_nmis].retournement = TRUE;
                  }
                }
              }
              // On décrémente la vitesse de déplacement du vaisseau.
              ve[cmpt_nmis].spr.vitesse -= 0.02f;
              // On donne les nouvelles coordonées du vaisseau ennemi.
              ve[cmpt_nmis].spr.coor_y += ve[cmpt_nmis].spr.vitesse;
            }
            // Test si on doit ou non afficher le sprite.
            if(((short)ve[cmpt_nmis].spr.coor_y +
                ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->haut) <
               DEBUT_ECRAN_Y
               || (short)ve[cmpt_nmis].spr.coor_y >=
               DEBUT_ECRAN_Y + HAUT_ECR_RAM)
            {                                                  // Indique que le sprite n'est pas visible, on n'a donc pas besoin de faire de tests de collision avec lui.
              ve[cmpt_nmis].visible = FALSE;
              // Test si l'ennemi se trouve en dehors des coordonées de l'écran.
              if((short)ve[cmpt_nmis].spr.coor_y < DEBUT_ECRAN_Y
                 && ve[cmpt_nmis].change_dir)
              {                                                // On enlève le vaisseau ennemi de la liste des vaisseaux ennemis.
                Del_Element_Ve(cmpt_nmis, i);                  // Routine utilisée pour supprimer un vaisseau ennemi du tableau des 've'.
                // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                i--;
                // Le vaisseau est détruit, on peux sortir de la bcle.
                goto fin_dep_ve_speciaux;
              }
            }
            else                                               // Sinon, on doit l'afficher.
            {                                                  // Indique que le sprite est visible, on devra donc faire les tests de collision avec lui.
              ve[cmpt_nmis].visible = TRUE;
            }
          }
            break;

          case ETB_SHURIKY:                                   //(ETB) Ennemi geree au gardien 12 (gardien12.cpp)
            // le gardien est mort ???
            if(ve[iNumeroGardien].type != KAMEAMEA
               || ve[iNumeroGardien].prev_object == MAX_NMI_ON_SCREEN)
            {
              ve[cmpt_nmis].type = SHURIKY;
            }
            break;
          case SAAKEY:                                        // (ETB) ennemi geree au gardien 14 (gardien14.cpp)
            //le gardien est mort ???
            if(ve[iNumeroGardien].type != GHOTTEN
               || ve[iNumeroGardien].prev_object == MAX_NMI_ON_SCREEN)
            {
              if(ve[cmpt_nmis].spr.img_act >= 31)
              {
                for(int _iImage = 0; _iImage < ve[cmpt_nmis].spr.nbr_img;
                    _iImage++)
                  ve[cmpt_nmis].spr.img[_iImage] =
                    (image *) & enemi[SAAKAMIN][_iImage];
                ve[cmpt_nmis].spr.img_act = 0;
                ve[cmpt_nmis].spr.nbr_tot_vbl = 3;
                ve[cmpt_nmis].type = SAAKAMIN;
              }
            }
            break;
          case SAAKAMIN:                                      // (ETB) ennemi geree au gardien 14 (gardien14.cpp)
            //le gardien est mort ???
            if(ve[iNumeroGardien].type != GHOTTEN
               || ve[iNumeroGardien].prev_object == MAX_NMI_ON_SCREEN)
            {
              ve[cmpt_nmis].type = SHURIKY;
            }
            break;

            // Vaisseaux se déplacant du haut de l'écran vers le bas.
          case MILLOUZ:
          case TOUBOUG:
          case BELCHOUTIES:
          case DEMONIANS:
          case FIDGETINIANS:
          case DIMITINIANS:
          case ENSLAVEERS:
          case DIVERTIZERS:
          case HORRIBIANS:
          case DEVILIANS:
          case ROUGHLEERS:
          case ABASCUSIANS:
          case PERTURBIANS:
          case EARTHINIANS:
          case DISGOOSTEES:
          case BIRIANSTEES:
          case TODHAIRIES:
          case DEFECTINIANS:
          case SOONIEES:
          case GAFFIES:
          case SHURIKY:
          case GOZUKY:
          case QUIBOULY:
          {                                                    // Test si la pause est activée ou pas.
            if(!player_pause && etat_menu == MENU_OFF
               && _score::getActionExecution() == 0)
            {                                                  // On donne les nouvelles coordonées du vaisseau ennemi.
              ve[cmpt_nmis].spr.coor_y += ve[cmpt_nmis].spr.vitesse;
            }
            // Test si on doit ou non afficher le sprite.
            if(((short)ve[cmpt_nmis].spr.coor_y +
                ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->haut) <
               DEBUT_ECRAN_Y
               || (short)ve[cmpt_nmis].spr.coor_y >=
               DEBUT_ECRAN_Y + HAUT_ECR_RAM)
            {                                                  // Indique que le sprite n'est pas visible, on n'a donc pas besoin de faire de tests de collision avec lui.
              ve[cmpt_nmis].visible = FALSE;
              // Test si l'ennemi se trouve en dehors des coordonées de l'écran.
              if((short)ve[cmpt_nmis].spr.coor_y >=
                 DEBUT_ECRAN_Y + HAUT_ECR_RAM)
              {                                                // On enlève le vaisseau ennemi de la liste des vaisseaux ennemis.
                Del_Element_Ve(cmpt_nmis, i);                  // Routine utilisée pour supprimer un vaisseau ennemi du tableau des 've'.
                // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                i--;
                // Le vaisseau est détruit, on peux sortir de la bcle.
                goto fin_dep_ve_speciaux;
              }
            }
            else                                               // Sinon, on doit l'afficher.
            {                                                  // Indique que le sprite est visible, on devra donc faire les tests de collision avec lui.
              ve[cmpt_nmis].visible = TRUE;
            }
          }
            break;
            // Vaisseaux se déplacant du bas de l'écran vers le haut.
          case SWORDINIANS:
          case HOCKYS:
          case CARRYONIANS:
          case ARCHINIANS:
          case TOURNADEE:
          {                                                    // Test si la pause est activée ou pas.
            if(!player_pause && etat_menu == MENU_OFF
               && _score::getActionExecution() == 0)
            {                                                  // On donne les nouvelles coordonées du vaisseau ennemi.
              ve[cmpt_nmis].spr.coor_y += ve[cmpt_nmis].spr.vitesse;
            }
            // Test si on doit ou non afficher le sprite.
            if(((short)ve[cmpt_nmis].spr.coor_y +
                ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->haut) <
               DEBUT_ECRAN_Y
               || (short)ve[cmpt_nmis].spr.coor_y >=
               DEBUT_ECRAN_Y + HAUT_ECR_RAM)
            {                                                  // Indique que le sprite n'est pas visible, on n'a donc pas besoin de faire de tests de collision avec lui.
              ve[cmpt_nmis].visible = FALSE;
              // Test si l'ennemi se trouve en dehors des coordonées de l'écran.
              if((short)ve[cmpt_nmis].spr.coor_y < DEBUT_ECRAN_Y)
              {                                                // On enlève le vaisseau ennemi de la liste des vaisseaux ennemis.
                Del_Element_Ve(cmpt_nmis, i);                  // Routine utilisée pour supprimer un vaisseau ennemi du tableau des 've'.
                // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                i--;
                // Le vaisseau est détruit, on peux sortir de la bcle.
                goto fin_dep_ve_speciaux;
              }
            }
            else                                               // Sinon, on doit l'afficher.
            {                                                  // Indique que le sprite est visible, on devra donc faire les tests de collision avec lui.
              ve[cmpt_nmis].visible = TRUE;
            }
          }
            break;
            // Vaisseaux se déplacant horizontalement de la droite vers la gauche.
          case FEABILIANS:
          case BITTERIANS:
          case TOUTIES:
          case SUBJUGANEERS:
          {                                                    // Test si la pause est activée ou pas.
            if(!player_pause && etat_menu == MENU_OFF
               && _score::getActionExecution() == 0)
            {                                                  // On donne les nouvelles coordonées du vaisseau ennemi.
              ve[cmpt_nmis].spr.coor_x += ve[cmpt_nmis].spr.vitesse;
            }
            // Test si on doit ou non afficher le sprite.
            if(((short)ve[cmpt_nmis].spr.coor_x +
                ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->larg) <
               DEBUT_ECRAN_X
               || (short)ve[cmpt_nmis].spr.coor_x >=
               DEBUT_ECRAN_X + LARG_ECR_RAM)
            {                                                  // Indique que le sprite n'est pas visible, on n'a donc pas besoin de faire de tests de collision avec lui.
              ve[cmpt_nmis].visible = FALSE;
              // Test si l'ennemi se trouve en dehors des coordonées de l'écran.
              if(((short)ve[cmpt_nmis].spr.coor_x +
                  ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->larg) <
                 DEBUT_ECRAN_X)
              {                                                // On enlève le vaisseau ennemi de la liste des vaisseaux ennemis.
                Del_Element_Ve(cmpt_nmis, i);                  // Routine utilisée pour supprimer un vaisseau ennemi du tableau des 've'.
                // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                i--;
                // Le vaisseau est détruit, on peux sortir de la bcle.
                goto fin_dep_ve_speciaux;
              }
            }
            else                                               // Sinon, on doit l'afficher.
            {                                                  // Indique que le sprite est visible, on devra donc faire les tests de collision avec lui.
              ve[cmpt_nmis].visible = TRUE;
            }
          }
            break;
            // Vaisseaux se déplacant horizontalement de la gauche vers la droite.
          case PAINIANS:
          case BAINIES:
          case NAGGYS:
          {                                                    // Test si la pause est activée ou pas.
            if(!player_pause && etat_menu == MENU_OFF
               && _score::getActionExecution() == 0)
            {                                                  // On donne les nouvelles coordonées du vaisseau ennemi.
              ve[cmpt_nmis].spr.coor_x += ve[cmpt_nmis].spr.vitesse;
            }
            // Test si on doit ou non afficher le sprite.
            if(((short)ve[cmpt_nmis].spr.coor_x +
                ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->larg) <
               DEBUT_ECRAN_X
               || (short)ve[cmpt_nmis].spr.coor_x >=
               DEBUT_ECRAN_X + LARG_ECR_RAM)
            {                                                  // Indique que le sprite n'est pas visible, on n'a donc pas besoin de faire de tests de collision avec lui.
              ve[cmpt_nmis].visible = FALSE;
              // Test si l'ennemi se trouve en dehors des coordonées de l'écran.
              if((short)ve[cmpt_nmis].spr.coor_x >=
                 DEBUT_ECRAN_X + LARG_ECR_RAM)
              {                                                // On enlève le vaisseau ennemi de la liste des vaisseaux ennemis.
                Del_Element_Ve(cmpt_nmis, i);                  // Routine utilisée pour supprimer un vaisseau ennemi du tableau des 've'.
                // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                i--;
                // Le vaisseau est détruit, on peux sortir de la bcle.
                goto fin_dep_ve_speciaux;
              }
            }
            else                                               // Sinon, on doit l'afficher.
            {                                                  // Indique que le sprite est visible, on devra donc faire les tests de collision avec lui.
              ve[cmpt_nmis].visible = TRUE;
            }
          }
            break;
            // Vaisseaux spéciaux se déplacant suivant une courbe.
          case ROTIES:
          case STENCHIES:
          case MADIRIANS:
          case BLAVIRTHE:
          case BLEUERCKS:
          {                                                    // Test si la pause est activée ou pas.
            if(!player_pause && etat_menu == MENU_OFF
               && _score::getActionExecution() == 0)
            {                                                  // On change la position du vaisseau dans la courbe.
              ve[cmpt_nmis].pos_vaiss[POS_CURVE]++;
              // Test si on doit ou non supprimer le sprite parce qu'il est sorti de la courbe.
              if(ve[cmpt_nmis].pos_vaiss[POS_CURVE] >=
                 initial_curve[ve[cmpt_nmis].num_courbe].nbr_pnt_curve)
              {                                                // On enlève le vaisseau ennemi de la liste des vaisseaux ennemis.
                Del_Element_Ve(cmpt_nmis, i);                  // Routine utilisée pour supprimer un vaisseau ennemi du tableau des 've'.
                // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                i--;
                // Le vaisseau est détruit, on peux sortir de la bcle.
                goto fin_dep_ve_speciaux;
              }
              // On donne les nouvelles coordonées du vaisseau ennemi.
              ve[cmpt_nmis].spr.coor_x += (float)initial_curve[ve[cmpt_nmis].num_courbe].delta_x[ve[cmpt_nmis].pos_vaiss[POS_CURVE]];   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
              ve[cmpt_nmis].spr.coor_y += (float)initial_curve[ve[cmpt_nmis].num_courbe].delta_y[ve[cmpt_nmis].pos_vaiss[POS_CURVE]];   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
            }
            // On donne la valeur de l'image du vaisseau à afficher.
            ve[cmpt_nmis].spr.img_act =
              initial_curve[ve[cmpt_nmis].num_courbe].angle[ve[cmpt_nmis].
                                                            pos_vaiss
                                                            [POS_CURVE]];
            // Test si on doit ou non afficher le sprite.
            if(((short)ve[cmpt_nmis].spr.coor_x +
                ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->larg) <
               DEBUT_ECRAN_X
               || (short)ve[cmpt_nmis].spr.coor_x >=
               DEBUT_ECRAN_X + LARG_ECR_RAM
               || ((short)ve[cmpt_nmis].spr.coor_y +
                   ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->haut) <
               DEBUT_ECRAN_Y
               || (short)ve[cmpt_nmis].spr.coor_y >=
               DEBUT_ECRAN_Y + HAUT_ECR_RAM)
            {                                                  // Indique que le sprite n'est pas visible, on n'a donc pas besoin de faire de tests de collision avec lui.
              ve[cmpt_nmis].visible = FALSE;
            }
            else                                               // Sinon, on doit l'afficher.
            {                                                  // Indique que le sprite est visible, on devra donc faire les tests de collision avec lui.
              ve[cmpt_nmis].visible = TRUE;
            }
          }
            break;
          case BIGMETEOR:
          case NORMALMETEOR:
          case SMALLMETEOR:
          {                                                    //printf("BIGMETEOR START : %i", ve[cmpt_nmis].type) ;
            // Test si la pause est activée ou pas.
            if(!player_pause && etat_menu == MENU_OFF
               && _score::getActionExecution() == 0)
            {                                                  // On donne les nouvelles coordonées du vaisseau ennemi.
              ve[cmpt_nmis].spr.coor_x += ve[cmpt_nmis].vitesse_x;
              ve[cmpt_nmis].spr.coor_y += ve[cmpt_nmis].vitesse_y;
              // On décompte le temps d'apparition du météor.
              ve[cmpt_nmis].temps_apparition--;
            }
            // Test si on doit ou non afficher le sprite.
            if(((short)ve[cmpt_nmis].spr.coor_y +
                ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->haut) <
               DEBUT_ECRAN_Y
               || (short)ve[cmpt_nmis].spr.coor_y >=
               DEBUT_ECRAN_Y + HAUT_ECR_RAM
               || ((short)ve[cmpt_nmis].spr.coor_x +
                   ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->larg) <
               DEBUT_ECRAN_X
               || (short)ve[cmpt_nmis].spr.coor_x >=
               DEBUT_ECRAN_X + LARG_ECR_RAM)
            {                                                  // Indique que le sprite n'est pas visible, on n'a donc pas besoin de faire de tests de collision avec lui.
              ve[cmpt_nmis].visible = FALSE;
              // Test si le temps d'apparition de la météorite est écoulé et si oui, elle peut-etre détruite.
              if(ve[cmpt_nmis].temps_apparition < 0)
              {                                                // On enlève la météorite de la liste des vaisseaux ennemis.
                Del_Element_Ve(cmpt_nmis, i);                  // Routine utilisée pour supprimer un vaisseau ennemi du tableau des 've'.
                // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                i--;
                // Le vaisseau est détruit, on peux sortir de la bcle.
                goto fin_dep_ve_speciaux;
              }
            }
            else                                               // Sinon, on doit l'afficher.
            {                                                  // Indique que le sprite est visible, on devra donc faire les tests de collision avec lui.
              ve[cmpt_nmis].visible = TRUE;
              // Incrémente le compteur de VBLs pour l'affichage de l'animation.
              ve[cmpt_nmis].spr.nbr_vbl_act++;
              // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
              if(ve[cmpt_nmis].spr.nbr_vbl_act >=
                 ve[cmpt_nmis].spr.nbr_tot_vbl)
              {                                                // On initialise le compteur de VBLs.
                ve[cmpt_nmis].spr.nbr_vbl_act = 0;
                // On passe à l'image suivante.
                ve[cmpt_nmis].spr.img_act += ve[cmpt_nmis].sens_anim;
                // Test si le numéro de l'image actuelle n'est pas supérieur au nombre d'images totales.
                if(ve[cmpt_nmis].spr.img_act >= ve[cmpt_nmis].spr.nbr_img)
                {                                              // On repositionne l'image actuelle sur la première image de l'animation.
                  ve[cmpt_nmis].spr.img_act = 0;
                }
                // Test si on doit mettre a jour l'image actuelle
                if(ve[cmpt_nmis].spr.img_act < 0)
                {                                              // On repositionne l'image actuelle sur la première image de l'animation.
                  ve[cmpt_nmis].spr.img_act = NBR_IMAGES_METEOR - 1;
                }
              }
            }
          }
            break;
        }
        // Test si le vaisseau ennemi spécial est visible.
        if(ve[cmpt_nmis].visible == TRUE)
        {                                                      // Test si on est en présence de vaisseaux spéciaux ayant une animation cyclique sur 32 images.
          // Si oui, on met à jour la prochaine image du sprite à afficher.
          // Pour cela, on test tous ceux qui n'ont pas une animation cyclique car il y en a moins que les autres.
          if(ve[cmpt_nmis].type != BIGMETEOR
             && ve[cmpt_nmis].type != NORMALMETEOR
             && ve[cmpt_nmis].type != SMALLMETEOR
             && ve[cmpt_nmis].type != BLAVIRTHE
             && ve[cmpt_nmis].type != BLEUERCKS
             && ve[cmpt_nmis].type != ROTIES
             && ve[cmpt_nmis].type != STENCHIES
             && ve[cmpt_nmis].type != MADIRIANS
             && ve[cmpt_nmis].type != PERTURBIANS
             && ve[cmpt_nmis].type != HOCKYS && ve[cmpt_nmis].type != BAINIES
             && ve[cmpt_nmis].type != TOUTIES
             && ve[cmpt_nmis].type != VIONIEES
             && ve[cmpt_nmis].type != ANGOUFF
             && ve[cmpt_nmis].type != CLOWNIES && ve[cmpt_nmis].type != EFFIES
             && ve[cmpt_nmis].type != SAPOUCH
             && ve[cmpt_nmis].type != DIMITINIANS
             && ve[cmpt_nmis].type != GAFFIES
             && ve[cmpt_nmis].type != ARCHINIANS
             && ve[cmpt_nmis].type != TODHAIRIES
             && ve[cmpt_nmis].type != DEFECTINIANS
             && ve[cmpt_nmis].type != SOONIEES
             && ve[cmpt_nmis].type != SOUKEE)
          {                                                    // Incrémente le compteur de VBLs pour l'affichage de l'animation.
            ve[cmpt_nmis].spr.nbr_vbl_act++;
            // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
            if(ve[cmpt_nmis].spr.nbr_vbl_act >= ve[cmpt_nmis].spr.nbr_tot_vbl)
            {                                                  // On initialise le compteur de VBLs.
              ve[cmpt_nmis].spr.nbr_vbl_act = 0;
              // On passe à l'image suivante.
              ve[cmpt_nmis].spr.img_act++;
              // Test si le numéro de l'image actuelle n'est pas supérieur au nombre d'images totales.
              if(ve[cmpt_nmis].spr.img_act >= ve[cmpt_nmis].spr.nbr_img)
              {                                                // On repositionne l'image actuelle sur la première image de l'animation.
                ve[cmpt_nmis].spr.img_act = 0;
              }
            }
          }
          // Test si le vaisseau ennemi va de droite vers la gauche et vice-versa.
          // Si oui, on met à jour la prochaine image du sprite à afficher.
          if(ve[cmpt_nmis].type == DIMITINIANS
             || ve[cmpt_nmis].type == TODHAIRIES
             || ve[cmpt_nmis].type == DEFECTINIANS
             || ve[cmpt_nmis].type == SOONIEES)
          {                                                    // Test si le sens de comptage de l'animation est décroissant.
            if(ve[cmpt_nmis].sens_anim)
            {                                                  // Incrémente le compteur de VBLs pour l'affichage de l'animation.
              ve[cmpt_nmis].spr.nbr_vbl_act++;
              // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
              if(ve[cmpt_nmis].spr.nbr_vbl_act >=
                 ve[cmpt_nmis].spr.nbr_tot_vbl)
              {                                                // On initialise le compteur de VBLs.
                ve[cmpt_nmis].spr.nbr_vbl_act = 0;
                // On passe à l'image suivante.
                ve[cmpt_nmis].spr.img_act--;
                // Test si le numéro de l'image actuelle n'est pas inférieur à zéro.
                if(ve[cmpt_nmis].spr.img_act < 0)
                {                                              // On repositionne l'image actuelle sur la première image de l'animation.
                  ve[cmpt_nmis].spr.img_act = 0;
                  ve[cmpt_nmis].sens_anim = 0;                 // Donne un sens croissant pour le comptage des images.
                }
              }
            }
            else                                               // Le sens de comptage des images de l'animation est croissant.
            {                                                  // Incrémente le compteur de VBLs pour l'affichage de l'animation.
              ve[cmpt_nmis].spr.nbr_vbl_act++;
              // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
              if(ve[cmpt_nmis].spr.nbr_vbl_act >=
                 ve[cmpt_nmis].spr.nbr_tot_vbl)
              {                                                // On initialise le compteur de VBLs.
                ve[cmpt_nmis].spr.nbr_vbl_act = 0;
                // On passe à l'image suivante.
                ve[cmpt_nmis].spr.img_act++;
                // Test si le numéro de l'image actuelle n'est pas supérieur au nombre d'images totales.
                if(ve[cmpt_nmis].spr.img_act >= ve[cmpt_nmis].spr.nbr_img)
                {                                              // On repositionne l'image actuelle sur la première image de l'animation.
                  ve[cmpt_nmis].spr.img_act =
                    (short)(ve[cmpt_nmis].spr.nbr_img - 1);
                  ve[cmpt_nmis].sens_anim = 1;                 // Donne un sens décroissant pour le comptage des images.
                }
              }
            }
          }
          // Test si on est en présence d'un vaisseau dont le cannon suit le joueur.
          // Si oui, on met à jour la prochaine image du sprite à afficher.
          if(ve[cmpt_nmis].type == PERTURBIANS ||
             ve[cmpt_nmis].type == GAFFIES ||
             ve[cmpt_nmis].type == HOCKYS ||
             ve[cmpt_nmis].type == BAINIES ||
             ve[cmpt_nmis].type == TOUTIES ||
             ve[cmpt_nmis].type == ARCHINIANS)
          {                                                    // Test si l'ancienne position corespond toujours à la bonne direction de tir.
            if(ve[cmpt_nmis].img_angle != ve[cmpt_nmis].spr.img_act)
            {                                                  // Test si le sens de comptage de l'animation est décroissant.
              if(ve[cmpt_nmis].sens_anim)
              {                                                // Incrémente le compteur de VBLs pour l'affichage de l'animation.
                ve[cmpt_nmis].spr.nbr_vbl_act++;
                // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
                if(ve[cmpt_nmis].spr.nbr_vbl_act >=
                   ve[cmpt_nmis].spr.nbr_tot_vbl)
                {                                              // On initialise le compteur de VBLs.
                  ve[cmpt_nmis].spr.nbr_vbl_act = 0;
                  // On passe à l'image suivante.
                  ve[cmpt_nmis].spr.img_act--;
                  // Test si le numéro de l'image actuelle n'est pas inférieur à zéro.
                  if(ve[cmpt_nmis].spr.img_act < 0)
                  {                                            // On repositionne l'image actuelle sur la première image de l'animation.
                    ve[cmpt_nmis].spr.img_act =
                      (short)(ve[cmpt_nmis].spr.nbr_img - 1);
                  }
                }
              }
              else                                             // Le sens de comptage des images de l'animation est croissant.
              {                                                // Incrémente le compteur de VBLs pour l'affichage de l'animation.
                ve[cmpt_nmis].spr.nbr_vbl_act++;
                // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
                if(ve[cmpt_nmis].spr.nbr_vbl_act >=
                   ve[cmpt_nmis].spr.nbr_tot_vbl)
                {                                              // On initialise le compteur de VBLs.
                  ve[cmpt_nmis].spr.nbr_vbl_act = 0;
                  // On passe à l'image suivante.
                  ve[cmpt_nmis].spr.img_act++;
                  // Test si le numéro de l'image actuelle n'est pas supérieur au nombre d'images totales.
                  if(ve[cmpt_nmis].spr.img_act >= ve[cmpt_nmis].spr.nbr_img)
                  {                                            // On repositionne l'image actuelle sur la première image de l'animation.
                    ve[cmpt_nmis].spr.img_act = 0;
                  }
                }
              }
              // On cherche le sens dans lequel doit tourner le cannon.
              // Initialisation du compteur pour la recherche dans le sens croissant.
              compteur = ve[cmpt_nmis].spr.img_act;
              first_value = 0;
              // Bcle sur toutes les positions d'angle possibles.
              for(compteur1 = 0; compteur1 < 32; compteur1++)
              {                                                // Incrémentation de la première valeur.
                first_value++;
                compteur++;
                if(compteur > 31)
                  compteur = 0;
                // Si le compteur à atteint la bonne valeur, on sort de la bcle.
                if(ve[cmpt_nmis].img_angle == compteur)
                  compteur1 = 32;
              }
              // Initialisation du compteur pour la recherche dans le sens décroissant.
              compteur = ve[cmpt_nmis].spr.img_act;
              second_value = 0;
              // Bcle sur toutes les positions d'angle possibles.
              for(compteur1 = 0; compteur1 < 32; compteur1++)
              {                                                // Incrémentation de la première valeur.
                second_value++;
                compteur--;
                if(compteur < 0)
                  compteur = 31;
                // Si le compteur à atteint la bonne valeur, on sort de la bcle.
                if(ve[cmpt_nmis].img_angle == compteur)
                  compteur1 = 32;
              }
              // Si la première valeur est plus petite que la seconde, on part dans le sens croissant.
              if(first_value < second_value)
                ve[cmpt_nmis].sens_anim = 0;
              else                                             // Sinon, le sens de comptage des animations du canon sera décroissant.
                ve[cmpt_nmis].sens_anim = 1;
            }
            // Recherche du sens dans lequel doit tourner le canon.
            // Recherche de la position du canon par rapport à la direction du joueur humain.
            float                   angl_canon_vj =
              Calc_Angle_Tir((short)
                             (ve[cmpt_nmis].spr.coor_x +
                              ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                    img_act]->x_cg),
                             (short)(ve[cmpt_nmis].spr.coor_y +
                                     ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                           img_act]->y_cg),
                             (short)(vj[J1].spr.coor_x +
                                     vj[J1].spr.img[vj[J1].spr.img_act]->
                                     x_cg),
                             (short)(vj[J1].spr.coor_y +
                                     vj[J1].spr.img[vj[J1].spr.img_act]->
                                     y_cg));
            // Recherche de l'image à afficher en fonction de l'angle.
            if(sign(angl_canon_vj < 0))
              ve[cmpt_nmis].img_angle =
                (short)((angl_canon_vj + DEUX_PI) / PI_SUR_16);
            else
              ve[cmpt_nmis].img_angle = (short)(angl_canon_vj / PI_SUR_16);
            // Pour éviter un indice de tableau négatif.
            ve[cmpt_nmis].img_angle = (short)abs(ve[cmpt_nmis].img_angle);
            // Pour éviter un angle de tir supérieur au nombre d'images du sprite.
            if(ve[cmpt_nmis].img_angle >= ve[cmpt_nmis].spr.nbr_img)
              ve[cmpt_nmis].img_angle =
                (short)(ve[cmpt_nmis].spr.nbr_img - 1);
          }
          // Test si le vaisseau ennemi est mort.
          if(ve[cmpt_nmis].dead)
          {                                                    // Gestion de l'invincibilité d'un vaisseau ennemi.
            // Retourne Vrai si le vaisseau ennemi est détruit, Faux si on continu à le faire clignoter.
            if(Gere_Invincibilite_Ve(cmpt_nmis))
            {                                                  // RAZ de la variable de fin de vie du vaisseau ennemi.
              ve[cmpt_nmis].dead = FALSE;
              // On enlève le vaisseau ennemi de la liste des vaisseaux ennemis.
              Del_Element_Ve(cmpt_nmis, i);                    // Routine utilisée pour supprimer un vaisseau ennemi du tableau des 've'.
              // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
              i--;
              goto fin_dep_ve_speciaux;                        // On sort de la bcle directement.
            }
          }
          else                                                 // Sinon, l'ennemi est encore en vie et visible.
          {                                                    // Affichage de l'ennemi.
            Aff_Ve(cmpt_nmis);
            // Test s'il y a des ondes de choc en activité.
            if(nbr_onde_choc)
            {                                                  // Test si l'onde de choc est en collision avec l'ennemi actuel.
              if(Tst_Col_Onde_Choc(cmpt_nmis))
              {                                                // On fait clignoter l'ennemi actuel avant de le faire disparaitre.
                Clignote_Nmi_Avant_Disparition(cmpt_nmis);
                // Routine utilisée pour ajouter une série d'explosion à un gros vaisseau.
                Add_Serie_Of_Explosion(cmpt_nmis);
                // On ajoute un eclat à la liste des explosions.
                Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                           ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                           x_cg - 8,
                           ve[cmpt_nmis].spr.coor_y +
                           ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                           y_cg - 8, 1.0, 4, 0, 1);
                goto fin_dep_ve_speciaux;                      // Sort de la gestion des vaisseau ennemis.
              }
            }
            // Test si la pause est activée ou pas.
            if(!player_pause && etat_menu == MENU_OFF
               && _score::getActionExecution() == 0)
            {                                                  // Décompte le temps séparant deux tirs.
              ve[cmpt_nmis].cad_tir--;
              // Test si l'ennemi doit ou non lancer un tir.
              if(ve[cmpt_nmis].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
              {                                                // Test si on est en présence d'un vaisseau dont le cannon suit le joueur.
                if(ve[cmpt_nmis].type == PERTURBIANS ||
                   ve[cmpt_nmis].type == GAFFIES ||
                   ve[cmpt_nmis].type == HOCKYS ||
                   ve[cmpt_nmis].type == BAINIES ||
                   ve[cmpt_nmis].type == TOUTIES ||
                   ve[cmpt_nmis].type == ARCHINIANS)
                {                                              // Test si la position du canon est dans la direction du joueur humain.
                  if(ve[cmpt_nmis].img_angle == ve[cmpt_nmis].spr.img_act)
                    goto Fait_Tirer_Le_Vaiso_Spe;
                }
                else                                           // Sinon, on est en présence d'un vaisseau spécial tout à fait normal.
                {
                Fait_Tirer_Le_Vaiso_Spe:;
                  // Met la cadence du tir pour le vaisseau ennemi.
                  ve[cmpt_nmis].cad_tir = ve[cmpt_nmis].cad_tir_initiale;
                  // Bcle sur tous les points de tirs existants.
                  for(k = 0;
                      k <
                      ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                      nbr_pts_tir; k++)
                  {                                            //Test si on est en presence d'un vaisseau qui envoi d'autres vaisseaux.
                    if(ve[cmpt_nmis].type == NAGGYS)
                    {
                      int                     tmp_pos_tst_x =
                        (int)(ve[cmpt_nmis].spr.coor_x +
                              ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                    img_act]->
                              xy_pos_tir[k][X]);
                      int                     tmp_pos_tst_y =
                        (int)(ve[cmpt_nmis].spr.coor_y +
                              ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.
                                                    img_act]->
                              xy_pos_tir[k][Y]);
                      // Test si le vaisseau que l'on doit creer se trouve a l'interieur de l'ecran visible.
                      if(tmp_pos_tst_x > DEBUT_ECRAN_X &&
                         tmp_pos_tst_x < (DEBUT_ECRAN_X + LARG_ECR_RAM - 1)
                         && (nbr_nmis < (MAX_NMI_ON_SCREEN - 2)))
                      {                                        // On rajoute un Vaisseau spécial GOZUKY dans la liste.
                        new_ve = New_Element_Ve();             // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
                        // Remplissage de la structure sprite appartenant au vaisseau ennemi.
                        ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + 10); // Indique la puissance de destruction du sprite.
                        ve[new_ve].spr.etat_dommages = ve[new_ve].spr.pow_of_dest;      // Indique l'état de domages du sprite.
                        ve[new_ve].spr.nbr_img = 32;           // Indique le nombre d'images composant le sprite.
                        ve[new_ve].spr.img_act = (short)(((long)rand() % NBR_IMAGES_NMI_S));    // Indique l'image actuelle.
                        ve[new_ve].spr.nbr_vbl_act = 0;        // RAZ du compteur de VBL pour le passage à l'image suivante.
                        ve[new_ve].spr.nbr_tot_vbl = 1;        //2; // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
                        // Donne l'adresse des images du sprite.
                        for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
                        {
                          ve[new_ve].spr.img[k] =
                            (image *) & enemi[GOZUKY][k];
                        }
                        ve[new_ve].cad_tir_initiale = 6000;    // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
                        ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;       // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
                        ve[new_ve].deplacement = SPECIAUX;     // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
                        // Donne les coordonées x et y de départ du vaisseau.
                        ve[new_ve].spr.coor_x =
                          (float)(tmp_pos_tst_x -
                                  ve[new_ve].spr.img[0]->x_cg);
                        ve[new_ve].spr.coor_y =
                          (float)(tmp_pos_tst_y -
                                  ve[new_ve].spr.img[0]->y_cg);
                        ve[new_ve].spr.vitesse = 0.2f;         // Donne la vitesse de déplacement du vaisseau ennemi spécial.
                        ve[new_ve].type = GOZUKY;              // Indique le type de vaisseau spécial.
                        ve[new_ve].dead = FALSE;               // Indique que l'ennemi est bien vivant.
                        ve[new_ve].visible = TRUE;             // Indique que l'ennemi est visible, on devra donc l'afficher.
                      }
                    }
                    else                                       //Sinon, on est en presence d'un vaisseau qui tire normalement.
                    {                                          // Test s'il est possible d'ajouter un nouveau tir à la liste.
                      if(nbr_tirs < (NBR_TIRS_MAX - 1))
                      {                                        // Recherche d'un nouveau tir.
                        new_tir = new_element_tir();
                        // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
                        tirs[new_tir].clignote = TRUE;         // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
                        tirs[new_tir].spr.genre = ENNEMI;      // Indique que c'est un sprite ennemi.
                        tirs[new_tir].spr.trajectoire = FALSE; // Indique que la trajectoire du sprite est calculée une fois pour toute.
                        tirs[new_tir].spr.nbr_img = 32;        // Indique le nombre d'images composant le sprite.
                        // Indique la puissance de destruction du sprite.
                        tirs[new_tir].spr.pow_of_dest = 4;
                        // Donne l'adresse des images du sprite.
                        for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
                        {                                      //Donne l'adresse des images du projectile.
                          if(ve[cmpt_nmis].type == QUIBOULY)   //Si c'est un QUIBOULY, on met des projectiles plus petits.
                          {
                            tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P1E][j];    // Corespond au Tir 2 puissance 2.
                            tirs[new_tir].spr.pow_of_dest = 2; // On change aussi la puissance du tir.
                          }
                          else                                 //Sinon, on met des projectiles de taille moyenne.
                            tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P2E][j];    // Corespond au Tir 2 puissance 2.
                        }
                        tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;        // Indique l'état de domages du sprite.
                        //tirs[new_tir].spr.pilote = 0;          // Indique par qui est dirigé le sprite.
                        //tirs[new_tir].spr.mode_commande = 0;   // Indique le moyen par lequel on fait déplacer le sprite.
                        tirs[new_tir].spr.img_act = 0;         // Indique l'image actuelle.
                        tirs[new_tir].spr.nbr_tot_vbl = 1;     // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
                        tirs[new_tir].spr.nbr_vbl_act = 0;     // Indique le nombre de VBL comptées actuellement.
                        // Donne les coordonnées d'affichage du sprite.
                        tirs[new_tir].img_angle =
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          angle_tir[k];
                        tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
                        tirs[new_tir].spr.coor_x =
                          ve[cmpt_nmis].spr.coor_x +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          xy_pos_tir[k][X] -
                          tirs[new_tir].spr.img[tirs[new_tir].img_angle]->
                          x_cg;
                        tirs[new_tir].spr.coor_y =
                          ve[cmpt_nmis].spr.coor_y +
                          ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                          xy_pos_tir[k][Y] -
                          tirs[new_tir].spr.img[tirs[new_tir].img_angle]->
                          y_cg;
                        tirs[new_tir].tmp_life_tir = 400;
                        // Donne un angle vertical pour le tir.
                        tirs[new_tir].angle_tir =
                          PI_SUR_16 * tirs[new_tir].img_angle;
                        tirs[new_tir].spr.vitesse = 1.0f + (float)num_level / 20.0f;    // Donne la vitesse de déplacement du sprite.
                      }
                    }
                  }
                }
              }
            }                                                  // Fin du test pour savoir si la pause est ou non activée.

            // Si le joueur n'est pas Game Over.
            if(!aff_game_over)
            {                                                  // Test les collisions entre les boules du joueur et le vaisseau ennemi.
              Test_Col_Bou_With_Nmi(cmpt_nmis);
              // Test les collisions entre les canons du joueur et le vaisseau ennemi.
              Test_Col_Can_With_Nmi(cmpt_nmis);
              // Test des collisions entre le joueur et l'ennemi spécial.
              Test_Col_Player_With_Spec_Nmi(cmpt_nmis);
            }                                                  // Fin du test si le joueur est Game Over.
          }                                                    // Fin du test pour savoir si l'ennemi est mort et si on est en train de le faire clignoter.
        }                                                      // Fin du test si on doit afficher le vaisseau ennemi ou non.
      fin_dep_ve_speciaux:
        //printf("fin_dep_ve_speciaux\n") ;
        ;
      }
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      // deplacement 3 : phase gardiens
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      case GARDIEN:
      { if(gardian_activity)                                   //gestion du gardien ?
        {
          if(ve[cmpt_nmis].dead)                               //oui, gardien est mort ?
          {
            if(Gere_Invincibilite_Ve(cmpt_nmis))               //oui, il clignote ?
            {
              ve[cmpt_nmis].dead = 0;                          //non, il est vraiment mort
              Del_Element_Ve(cmpt_nmis, i);                    //supprime le gardien de la liste "ve"
              i--;
              bar_nrj_guardian = 1;                            //1=reaffichage
              goto fin_dep_ve_gardien;                         //on sort de la bcle directement.
            }
          }
          else                                                 //le gardien est toujours vivant
          {
            if(nbr_onde_choc)                                  //il y a des onde(s) de choc ?
            {
              if(Tst_Col_Onde_Choc(cmpt_nmis))                 //oui, collision gardien/onde(s) de choc
              {
                Clignote_Nmi_Avant_Disparition(cmpt_nmis);
                Add_Serie_Of_Explosion(cmpt_nmis);             //on ajoute des explosions
                Add_Eclats(ve[cmpt_nmis].spr.coor_x +
                           ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                           x_cg - 8,
                           ve[cmpt_nmis].spr.coor_y +
                           ve[cmpt_nmis].spr.img[ve[cmpt_nmis].spr.img_act]->
                           y_cg - 8, 1.0, 4, 0, 1);
              }
            }
            Gere_gardien(cmpt_nmis);                           //gestion du gardien "gardien.cpp"
          }
        }
      fin_dep_ve_gardien:;
      }
        break;
    }
  }
}


float Deplacement_Tir_X(float angle, float vitesse, float pos_x)
{
  pos_x += (float)(cos(angle)) * vitesse;
  return (pos_x);
}
float Deplacement_Tir_Y(float angle, float vitesse, float pos_y)
{
  pos_y += (float)(sin(angle)) * vitesse;
  return (pos_y);
}

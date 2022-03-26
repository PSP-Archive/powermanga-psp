//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "bonus.cpp"
// created	: ?
// updates	: 2005-01-08
// id		: $Id: bonus.cpp,v 1.1.1.1 2005/01/25 05:52:09 gurumeditation Exp $
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
extern configfile*      power_conf;
extern short            nbr_nmis;                              // nombre d'enemis actuellement geres.
extern int              num_level;                             // level number
extern bool             player_pause;                          // 1=pause enable
extern int              etat_menu;                             // MENU_OFF=menu disable ; MENU_ON=menu enable
extern str_bonus        bon[MAX_BONUS_ON_SCREEN];              // bonus's structure
extern image            bonus[MAX_TYP_OF_BONUS][NBR_IMAGES_BONUS];      // Definit le tableau des differents ennemis.
extern int              tmp_tsts_x;
extern int              tmp_tsts_y;
extern int              tmp_tstd_x;
extern int              tmp_tstd_y;
extern bool             option_change;                         // 1=option change (udpate panel)
extern vaisseau         vj[NBR_MAX_J];                         // stucture's ship of player
extern char            *ecran_ram;                             // adresse ecran logique.
extern short            nbr_bonus;                             // indique le nombre de bonus actuellement geres.
extern bool             aff_game_over;                         // true=affichage du game over en cours
extern int              player_score;
extern int              bonusx;                                // variable utilisee pour multiplier les points si le joueur a pris le bonus.
extern short            nbr_boules;                            // indique le nombre de boules actuellement geres.
extern int              new_bou;                               // utilisee pour pointer sur un nouvel element 'bou'.
extern int              liste_bou[MAX_BOULES_ON_SCREEN];       // definit la liste de toutes les 'bou' que l'on doit geres a chaque VBLs.
extern str_boule        bou[MAX_BOULES_ON_SCREEN];             // declaration de la structure des boules.
extern image            boule[MAX_TYP_OF_BOULES][NBR_IMAGES_BOULES];    // Definit le tableau des differents ennemis.
extern bool             eclair_actif;                          // Indique si le joueur peut ou non utiliser l'eclair.
extern option           aff_option[MAX_TYP_OF_OPTIONS];                        // Tableau utilise pour l'affichage des options pendant le jeu.
extern int              nbr_onde_choc;                         // Defini le nombre actuel d'onde de choc.
extern int              cmpt_onde_choc;                        // Utilise pour compter les ondes de choc dans les bcles.
extern onde_de_choc     onde_choc[NBR_ONDE_CHOC_MAX];          // Defini le tableau des differentes ondes de choc.
extern bool             bar_nrj_player;

//..............................................................................
void                    Gene_Bonus_Malus(float pos_x, float pos_y);     //genere un bonus ou un malus
void                    Gene_Bonus_Malus_Meteor(float pos_x, float pos_y);      //genere un bonus ou un malus dans les phases meteors
int                     Calculate_Bonus(int value);            // Calcule le bonus en fonction de la difficulte de jeu
int                     Calculate_Bonus_Meteor(int value);     // Calcule le bonus des meteorites en fonction de la difficulte de jeu

//..............................................................................

//------------------------------------------------------------------------------
// gestion de tous les bonus
//------------------------------------------------------------------------------
void bonus_execution()
{
  static int              cmpt_bonus, o, n, m;
  for(cmpt_bonus = 0; cmpt_bonus < nbr_bonus; cmpt_bonus++)
  {                                                            // Test la trajectoire du bonus afin de le deplacer.
    switch (bon[cmpt_bonus].trajectoire)
    {                                                          // Cas d'un bonus qui avance tout droit.
      case 0:
      {                                                        // Test si la pause est activee ou pas.
        if(!player_pause && etat_menu == MENU_OFF
           && _score::getActionExecution() == 0)
        {                                                      // Deplacement du bonus vers le bas suivant sa vitesse.
          bon[cmpt_bonus].coor_y += bon[cmpt_bonus].vitesse;
        }
        // Test si le bonus ne sort pas de l'ecran.
        if(((short)bon[cmpt_bonus].coor_y >= (HAUT_ECR_RAM + BANDE_DE_CLIP))
           || ((short)bon[cmpt_bonus].coor_x +
               (short)bon[cmpt_bonus].img[bon[cmpt_bonus].img_act]->larg >=
               (LARG_ECR_RAM_TOT - 1))
           || ((short)bon[cmpt_bonus].coor_x +
               (short)bon[cmpt_bonus].img[bon[cmpt_bonus].img_act]->larg <
               BANDE_DE_CLIP))
        {                                                      // Le bonus sort de l'ecran, on l'enleve donc de la liste des bonus.
          // Bcle sur tous les bonus restants.
          for(n = cmpt_bonus; n < (nbr_bonus - 1); n++)
          {                                                    // Decalage du tableau des bonus.
            bon[n].genre = bon[n + 1].genre;                   // Indique le genre du bonus.
            bon[n].trajectoire = bon[n + 1].trajectoire;       // Donne la trajectoire du bonus.
            bon[n].nbr_img = bon[n + 1].nbr_img;               // Indique le nombre d'images composant le sprite.
            bon[n].img_act = bon[n + 1].img_act;               // Indique l'image actuelle.
            bon[n].nbr_tot_vbl = bon[n + 1].nbr_tot_vbl;       // Indique le nombre de VBL qu'il faut pour passer a l'image suivante.
            bon[n].nbr_vbl_act = bon[n + 1].nbr_vbl_act;       // Indique le nombre de VBL comptees actuellement.
            // Donne l'adresse des images du bonus.
            for(o = 0; o < bon[n].nbr_img; o++)
            {                                                  // Copie de la nouvelle image.
              bon[n].img[o] = bon[n + 1].img[o];
            }
            // Donne les coordonnees d'affichage du bonus.
            bon[n].coor_x = bon[n + 1].coor_x;
            bon[n].coor_y = bon[n + 1].coor_y;
            bon[n].vitesse = bon[n + 1].vitesse;               // Donne la vitesse de deplacement du sprite.
          }
          // On decremente le nombre de bonus actuellement geres.
          nbr_bonus--;
          cmpt_bonus--;
        }
        else                                                   // On doit afficher le bonus.
        {                                                      // Incremente le compteur de VBLs pour l'affichage de l'animation.
          bon[cmpt_bonus].nbr_vbl_act++;
          // Test si l'on a atteint le nombre de VBLs necessaires pour le changement d'image.
          if(bon[cmpt_bonus].nbr_vbl_act >= bon[cmpt_bonus].nbr_tot_vbl)
          {                                                    // On initialise le compteur de VBLs.
            bon[cmpt_bonus].nbr_vbl_act = 0;
            bon[cmpt_bonus].img_act++;                        // passe a l'image suivante.
            // Test si le numero de l'image actuelle n'est pas superieur au nombre d'images totales.
            if(bon[cmpt_bonus].img_act >= bon[cmpt_bonus].nbr_img)
            {                                                  // On repositionne l'image actuelle sur la premiere image de l'animation.
              bon[cmpt_bonus].img_act = 0;
            }
          }
          // Test s'il y a une collision entre le bonus et le vaisseau du joueur.
          if(!aff_game_over)                                   // Si le joueur n'est pas Game Over.
          {                                                    // Bcle sur tous les points de collision du vaisseau.
            for(m = 0; m < vj[J1].spr.img[vj[J1].spr.img_act]->nbr_pts_col;
                m++)
            {
              tmp_tsts_x =
                (int)vj[J1].spr.coor_x +
                vj[J1].spr.img[vj[J1].spr.img_act]->xy_pts_col[m][X];
              tmp_tsts_y =
                (int)vj[J1].spr.coor_y +
                vj[J1].spr.img[vj[J1].spr.img_act]->xy_pts_col[m][Y];
              tmp_tstd_x =
                (int)bon[cmpt_bonus].coor_x +
                bon[cmpt_bonus].img[bon[cmpt_bonus].img_act]->
                xy_zon_col[0][X];
              tmp_tstd_y =
                (int)bon[cmpt_bonus].coor_y +
                bon[cmpt_bonus].img[bon[cmpt_bonus].img_act]->
                xy_zon_col[0][Y];
              // Test si le point de collision se trouve a l'interieur de la zone de collision du bonus.
              if(tmp_tsts_x >= tmp_tstd_x &&
                 tmp_tsts_y >= tmp_tstd_y &&
                 tmp_tsts_x <
                 (tmp_tstd_x +
                  bon[cmpt_bonus].img[bon[cmpt_bonus].img_act]->
                  lh_zon_col[0][L])
                 && tmp_tsts_y <
                 (tmp_tstd_y +
                  bon[cmpt_bonus].img[bon[cmpt_bonus].img_act]->
                  lh_zon_col[0][H]))
              {                                                // Mise a jour du score du joueur pour le ramassage d'un bonus.
                player_score += 250 << bonusx;
                // Test le genre du bonus pour savoir ce que l'on doit donner au joueur.
                switch (bon[cmpt_bonus].genre)
                {                                              // Cas d'un bonus qui incremente de un la barre d'option.
                  case 0:
                  {                                            // Incremente le nombre de bonus du joueur.
                    vj[J1].nbr_bonus++;
                    option_change = TRUE;                      //indique que l'etat des options a change
#ifdef USE_SDLMIXER
                    seal_joue(BONUS1);                         //joue son du bonus 1
#endif
                  }
                    break;
                    // Cas d'un bonus qui incremente de deux la barre d'option.
                  case 1:
                  {                                            // Incremente le nombre de bonus du joueur.
                    vj[J1].nbr_bonus += 2;
                    option_change = TRUE;                      //indique que l'etat des options a change
#ifdef USE_SDLMIXER
                    seal_joue(BONUS2);                         //son "bonus2 
#endif
                  }
                    break;
                    // Cas d'un bonus qui donne une boule.
                  case 2:
                  {
                    ajoute_boule();
#ifdef USE_SDLMIXER
                    seal_joue(BONUS3);                         //son du bonus 3.
#endif
                  }
                    break;
                    // Cas d'un bonus qui remonte la barre d'energie du joueur.
                  case 3:
                  {                                            // Test si l'energie du joueur n'est pas au maximum.
                    if(vj[J1].spr.etat_dommages < vj[J1].spr.pow_of_dest)
                    {                                          // On indique que l'option d'energie est ouverte.
                      aff_option[1].close_option = FALSE;
#ifdef USE_SDLMIXER
                      seal_joue(BONUS4);                       //joue son bonus 4.
#endif
                    }
                    else                                       // Si l'energie du joueur est au maximum, on ajoute une onde de choc.
                    {
                      cmpt_onde_choc = New_Element_Onde_Choc();
                      onde_choc[cmpt_onde_choc].cmpt_cercle = 0;
                      onde_choc[cmpt_onde_choc].cmpt_color_aff = 0;
                      onde_choc[cmpt_onde_choc].orig_x =
                        (int)(vj[J1].spr.coor_x +
                              vj[J1].spr.img[vj[J1].spr.img_act]->x_cg);
                      onde_choc[cmpt_onde_choc].orig_y =
                        (int)(vj[J1].spr.coor_y +
                              vj[J1].spr.img[vj[J1].spr.img_act]->y_cg);
                      // Joue le son de l'onde de chocu tir 1.
#ifdef USE_SDLMIXER
                      seal_joue(ONDE_CHOC);                    //joue un son avec seal
#endif
                    }
                    // Incremente la barre d'energie du joueur.
                    vj[J1].spr.etat_dommages += 20;            // Augmente le niveau d'energie du joueur.
                    // Test si la barre d'energie n'est pas supperieur a sa barre de reference.
                    if(vj[J1].spr.etat_dommages >= vj[J1].spr.pow_of_dest)
                    {                                          // On clip l'energie du joueur.
                      vj[J1].spr.etat_dommages = vj[J1].spr.pow_of_dest;
                      // Test si l'energie n'etait pas avant cela au maximum.
                      if(!aff_option[1].close_option)
                      {                                        // Indique que l'animation de fermeture de l'option d'energie doit s'effectuer.
                        Init_Anim_Option(1, TRUE);
                      }
                    }
                     bar_nrj_player = TRUE;                   //indique que la barre d'energie du joueur a change.
                  }
                    break;
                    // Cas d'un bonus multiplicateur.
                  case 4:
                  {
                    bonusx++;                                  //incremente le nombre de bonus du joueur.
                    if(bonusx > 2) bonusx = 2;                 //test si depasse le maximum autorise (x4)
                  }
                    break;
                }
                // Le bonus vient d'être ramasse par le joueur, on doit donc le supprimer de la liste.
                // Bcle sur tous les bonus restants.
                for(n = cmpt_bonus; n < (nbr_bonus - 1); n++)
                {                                              // Decalage du tableau des bonus.
                  bon[n].genre = bon[n + 1].genre;             // Indique le genre du bonus.
                  bon[n].trajectoire = bon[n + 1].trajectoire; // Donne la trajectoire du bonus.
                  bon[n].nbr_img = bon[n + 1].nbr_img;         // Indique le nombre d'images composant le sprite.
                  bon[n].img_act = bon[n + 1].img_act;         // Indique l'image actuelle.
                  bon[n].nbr_tot_vbl = bon[n + 1].nbr_tot_vbl; // Indique le nombre de VBL qu'il faut pour passer a l'image suivante.
                  bon[n].nbr_vbl_act = bon[n + 1].nbr_vbl_act; // Indique le nombre de VBL comptees actuellement.
                  // Donne l'adresse des images du bonus.
                  for(o = 0; o < bon[n].nbr_img; o++)
                  {                                            // Copie de la nouvelle image.
                    bon[n].img[o] = bon[n + 1].img[o];
                  }
                  // Donne les coordonnees d'affichage du bonus.
                  bon[n].coor_x = bon[n + 1].coor_x;
                  bon[n].coor_y = bon[n + 1].coor_y;
                  bon[n].vitesse = bon[n + 1].vitesse;         // Donne la vitesse de deplacement du sprite.
                }
                // On decremente le nombre de bonus actuellement geres.
                nbr_bonus--;
                cmpt_bonus--;
                // On sort de la bcle de test des bonus.
                goto fin_tst_col_bonus_cur;
              }
            }
          }
          // Affichage du bonus.
          putsprite3(bon[cmpt_bonus].img[bon[cmpt_bonus].img_act], (unsigned int)bon[cmpt_bonus].coor_x, (unsigned int)bon[cmpt_bonus].coor_y);
          // Label utilise pour sortir de la bcle qui test si le
          // vaisseau du joueur est en collision avec un bonus.
        fin_tst_col_bonus_cur:;
        }
      }
        break;
    }
  }
}

//------------------------------------------------------------------------------
// ajoute une boule autour du vaisseau
//------------------------------------------------------------------------------
void ajoute_boule()
{
  int                     m;

  //reinitialise l'energie de toutes les boules
  for(m = 0; m < MAX_BOULES_ON_SCREEN; m++)
  {
    bou[m].etat_dommages = (short)(50 + vj[J1].type_vaisseau * 10);     // si =< 0 cela indique que la boule est detruite, si > 0 cela   indique l'etat des dommages.
    bou[m].pow_of_dest = (short)(15 + vj[J1].type_vaisseau * 5);        // puissance de destruction (utilise lors d'une collision).
  }

  if(nbr_boules < 5)
  {                                                            // On rajoute une boule dans la liste.
    new_bou = New_Element_Bou();                               // Retourne l'indice dans le tableau 'bou' d'un nouvel element de la liste.
	//bou[new_bou].app_vaisseau = 0;
    bou[new_bou].nbr_pts_cercle = 80;                          // Indique le nombre de points composant le cercle.
    bou[liste_bou[0]].pos_pts_cercle = 0;                      // Indique la position du pointeur sur le tableau des points du cercle.
    bou[new_bou].etat_dommages = (short)(50 + vj[J1].type_vaisseau * 10);       //si =< 0 cela indique que la boule est detruite, si > 0 cela   indique l'etat des dommages.
    bou[new_bou].pow_of_dest = (short)(15 + vj[J1].type_vaisseau * 5);  //puissance de destruction (utilise lors d'une collision).
    bou[new_bou].nbr_img = NBR_IMAGES_BOULES;                  //nombre d'images total composant le sprite
    bou[new_bou].img_act = 0;                                  //commence a l'image zero
    bou[new_bou].nbr_tot_vbl = 8;                              //vitesse d'animation entre chaque image
    bou[new_bou].nbr_vbl_act = 0;                              //tempo. d'animation
    bou[new_bou].cad_tir_initiale = 25;                        //cadence de tir de la boule (non utilise)
    bou[new_bou].cad_tir = bou[new_bou].cad_tir_initiale;      //tempo. entre deux tirs (non utilise)
    for(m = 0; m < bou[new_bou].nbr_img; m++)
      bou[new_bou].img[m] = (image *) & boule[vj[J1].type_vaisseau][m]; //calcul adresse de chaque image

    // Positionnement des boules autour du vaisseau appartenant au joueur.
    switch (nbr_boules)
    {                                                          // Test les cas suivant le nombre de boules presentes.
      case 2:
        bou[liste_bou[1]].pos_pts_cercle = 40;                 // Indique la position du pointeur sur le tableau des points du cercle.
        break;
      case 3:
        bou[liste_bou[1]].pos_pts_cercle = 26;                 // Indique la position du pointeur sur le tableau des points du cercle.
        bou[liste_bou[2]].pos_pts_cercle = 53;                 // Indique la position du pointeur sur le tableau des points du cercle.
        break;
      case 4:
        bou[liste_bou[1]].pos_pts_cercle = 20;                 // Indique la position du pointeur sur le tableau des points du cercle.
        bou[liste_bou[2]].pos_pts_cercle = 40;                 // Indique la position du pointeur sur le tableau des points du cercle.
        bou[liste_bou[3]].pos_pts_cercle = 60;                 // Indique la position du pointeur sur le tableau des points du cercle.
        break;
      case 5:
        bou[liste_bou[1]].pos_pts_cercle = 16;                 // Indique la position du pointeur sur le tableau des points du cercle.
        bou[liste_bou[2]].pos_pts_cercle = 32;                 // Indique la position du pointeur sur le tableau des points du cercle.
        bou[liste_bou[3]].pos_pts_cercle = 48;                 // Indique la position du pointeur sur le tableau des points du cercle.
        bou[liste_bou[4]].pos_pts_cercle = 64;                 // Indique la position du pointeur sur le tableau des points du cercle.
        break;
    }
  }
  else                                                         // Cas dans lequel le joueur a le maximum de boules.
  {                                                            // On ajoute le droit au joueur d'utiliser l'eclair.
    eclair_actif = TRUE;
  }
}

//------------------------------------------------------------------------------
// Genere un bonus soit un malus
//------------------------------------------------------------------------------
void Gene_Bonus_Malus(vaisseau_nmi *pve)
{
  image                  *i = pve->spr.img[pve->spr.img_act];
  Gene_Bonus_Malus(pve->spr.coor_x + i->x_cg - 8, pve->spr.coor_y + i->y_cg - 8);
}

//------------------------------------------------------------------------------
// Genere un bonus soit un malus
//------------------------------------------------------------------------------
void Gene_Bonus_Malus_Meteor(vaisseau_nmi *pve)
{
  image                  *i = pve->spr.img[pve->spr.img_act];
  Gene_Bonus_Malus_Meteor(pve->spr.coor_x + i->x_cg - 8, pve->spr.coor_y + i->y_cg - 8);
}

//------------------------------------------------------------------------------
// Genere un bonus soit un malus
//------------------------------------------------------------------------------
void Gene_Bonus_Malus(float pos_x, float pos_y)
{
  int                     m;
  int                     val_gene = 0;                        //anti-warning

  // Generation alleatoire de la valeur utilisee pour le choix du bonus ou du malus.
  if(num_level == 0) val_gene = (((long)rand() % ((vj[J1].type_vaisseau << 2) + 35)));
  if(num_level == 1) val_gene = (((long)rand() % ((vj[J1].type_vaisseau << 2) + 40)));
  if(num_level == 2) val_gene = (((long)rand() % ((vj[J1].type_vaisseau << 2) + 45)));
  if(num_level >= 3) val_gene = (((long)rand() % ((vj[J1].type_vaisseau << 2) + 50)));
  val_gene = Calculate_Bonus(val_gene);                        // (Laurent Duperval)
  switch (val_gene)
  {
    // Test si l'on doit ajouter un bonus qui incremente la barre des options.
    case BONUS_INCR_BY_1:
      if(nbr_bonus < (MAX_BONUS_ON_SCREEN - 1))                // On ajoute un bonus a la liste des bonus. 
      { bon[nbr_bonus].genre = 0;
        bon[nbr_bonus].trajectoire = 0;
        bon[nbr_bonus].nbr_img = NBR_IMAGES_BONUS;
        bon[nbr_bonus].img_act = 0;
        bon[nbr_bonus].nbr_tot_vbl = 4;
        bon[nbr_bonus].nbr_vbl_act = 0;
        // Donne l'adresse des images du bonus.
        for(m = 0; m < bon[nbr_bonus].nbr_img; m++)            // donne les adresses des images du bonus.
        {  bon[nbr_bonus].img[m] = (image *) & bonus[0][m];
        }
        bon[nbr_bonus].coor_x = pos_x;
        bon[nbr_bonus].coor_y = pos_y;
        bon[nbr_bonus].vitesse = 0.25;
        nbr_bonus++;                                           // Indique que l'on vient de rajouter un bonus a la liste.
      }
      break;

    // Test si l'on doit ajouter un bonus qui incrémente de deux la barre des options.
    case BONUS_INCR_BY_2:
      if(nbr_bonus < (MAX_BONUS_ON_SCREEN - 1))                // On ajoute un bonus a la liste des bonus.
      { bon[nbr_bonus].genre = 1;
        bon[nbr_bonus].trajectoire = 0;
        bon[nbr_bonus].nbr_img = NBR_IMAGES_BONUS;
        bon[nbr_bonus].img_act = 0;
        bon[nbr_bonus].nbr_tot_vbl = 4;
        bon[nbr_bonus].nbr_vbl_act = 0;
        for(m = 0; m < bon[nbr_bonus].nbr_img; m++)            // donne les adresses des images du bonus.
        { bon[nbr_bonus].img[m] = (image *) & bonus[1][m];
        }
        bon[nbr_bonus].coor_x = pos_x;
        bon[nbr_bonus].coor_y = pos_y;
        bon[nbr_bonus].vitesse = 0.35f;
        nbr_bonus++;                                           // Indique que l'on vient de rajouter un bonus a la liste.
      }
      break;

    // Test si l'on doit ajouter un bonus qui donne une boule.
    case BONUS_PROT_BALL:
    if(nbr_bonus < (MAX_BONUS_ON_SCREEN - 1))                  // on ajoute un bonus a la liste des bonus ?
    { bon[nbr_bonus].genre = 2;
      bon[nbr_bonus].trajectoire = 0;
      bon[nbr_bonus].nbr_img = NBR_IMAGES_BONUS;
      bon[nbr_bonus].img_act = 0;
      bon[nbr_bonus].nbr_tot_vbl = 4;
      bon[nbr_bonus].nbr_vbl_act = 0;
      for(m = 0; m < bon[nbr_bonus].nbr_img; m++)              // donne les adresses des images du bonus.
      { bon[nbr_bonus].img[m] = (image *) & bonus[2][m];
      }
      bon[nbr_bonus].coor_x = pos_x;
      bon[nbr_bonus].coor_y = pos_y;
      bon[nbr_bonus].vitesse = 0.55f;
      nbr_bonus++;                                             // Indique que l'on vient de rajouter un bonus a la liste.
    }
    break;

  // Test si l'on doit ajouter un bonus qui remonte l'energie du joueur.
    case BONUS_INCR_ENERGY:
      if(nbr_bonus < (MAX_BONUS_ON_SCREEN - 1))                // on ajoute un bonus a la liste des bonus ?
      { bon[nbr_bonus].genre = 3;
        bon[nbr_bonus].trajectoire = 0;
        bon[nbr_bonus].nbr_img = NBR_IMAGES_BONUS;
        bon[nbr_bonus].img_act = 0;
        bon[nbr_bonus].nbr_tot_vbl = 4;
        bon[nbr_bonus].nbr_vbl_act = 0;
        for(m = 0; m < bon[nbr_bonus].nbr_img; m++)            // donne les adresses des images du bonus.
        { bon[nbr_bonus].img[m] = (image *) & bonus[3][m];
        }
        bon[nbr_bonus].coor_x = pos_x;
        bon[nbr_bonus].coor_y = pos_y;
        bon[nbr_bonus].vitesse = 0.45f;
        nbr_bonus++;                                           // Indique que l'on vient de rajouter un bonus a la liste.
      }
      break;

    // Test si l'on doit ajouter un vaisseau special.
    case BONUS_SPECIAL_SHIP:
      Add_Ve_Special(-1);
      break;

    default:
      break;
  }
}

//------------------------------------------------------------------------------
// Selectionne le bonus genere par la destruction d'un vaisseau (Laurent Duperval)
//------------------------------------------------------------------------------
int Calculate_Bonus(int value)
{
  int retValue = -1;
  if (power_conf->difficulty == 2)
  {
    if (value < 5)      { retValue = BONUS_INCR_BY_1; }
    else if (value==10) { retValue = BONUS_INCR_BY_2; }
    else if (value==11) { retValue = BONUS_PROT_BALL; }
    else if (value==12) { retValue = BONUS_INCR_ENERGY; }
    else if (value==13 || value==26 || value == 33 || value == 40 || value==49)
    { if (nbr_nmis<(MAX_NMI_ON_SCREEN))
      { retValue = BONUS_SPECIAL_SHIP;
      }
    }
  }
  else if (power_conf->difficulty == 1)
  {
    if (value < 10)   { retValue = BONUS_INCR_BY_1; }
    else if (value==10) { retValue = BONUS_INCR_BY_2; }
    else if (value==11) { retValue = BONUS_PROT_BALL; }
    else if (value==12 || value==35) { retValue = BONUS_INCR_ENERGY; }
    else if (value==13 || value==26 || value==49)
    { if (nbr_nmis<(MAX_NMI_ON_SCREEN-2))
      { retValue = BONUS_SPECIAL_SHIP;
      }
    }
  }
  else if (power_conf->difficulty == 0)
  { if (value < 15)      { retValue = BONUS_INCR_BY_1; }
    else if (value < 22) { retValue = BONUS_INCR_BY_2; }
    else if (value < 25) { retValue = BONUS_PROT_BALL; }
    else if (value < 30) { retValue = BONUS_INCR_ENERGY; }
    else if (value < 32 || value == 49)
    { if (nbr_nmis<(MAX_NMI_ON_SCREEN-4))
      { retValue = BONUS_SPECIAL_SHIP;
      }
    }
  }
  return retValue;
}

//------------------------------------------------------------------------------
// genere un bonus ou malus dans une phase meteorites 
//------------------------------------------------------------------------------
void Gene_Bonus_Malus_Meteor(float pos_x, float pos_y)
{
  int                     m;
  int                     val_gene = 0;

  // Generation alleatoire de la valeur utilisee pour le choix du bonus ou du malus.
  if(num_level == 0) val_gene = (((long)rand() % ((vj[J1].type_vaisseau << 2) + 30)));
  if(num_level == 1) val_gene = (((long)rand() % ((vj[J1].type_vaisseau << 2) + 40)));
  if(num_level == 2) val_gene = (((long)rand() % ((vj[J1].type_vaisseau << 2) + 50)));
  if(num_level >= 3) val_gene = (((long)rand() % ((vj[J1].type_vaisseau << 2) + 60)));

  val_gene = Calculate_Bonus_Meteor(val_gene);                 // (Laurent Duperval)
  switch (val_gene)
  {
    // Test si l'on doit ajouter un bonus qui incremente la barre des options.
    case BONUS_INCR_BY_1:
      if(nbr_bonus < (MAX_BONUS_ON_SCREEN - 1))                // on ajoute un bonus a la liste des bonus ?
      { bon[nbr_bonus].genre = 0;
        bon[nbr_bonus].trajectoire = 0;
        bon[nbr_bonus].nbr_img = NBR_IMAGES_BONUS;
        bon[nbr_bonus].img_act = 0;
        bon[nbr_bonus].nbr_tot_vbl = 4;
        bon[nbr_bonus].nbr_vbl_act = 0;
        for(m = 0; m < bon[nbr_bonus].nbr_img; m++)
        { bon[nbr_bonus].img[m] = (image *) & bonus[0][m];
        }
        bon[nbr_bonus].coor_x = pos_x;
        bon[nbr_bonus].coor_y = pos_y;
        bon[nbr_bonus].vitesse = 0.25;
        nbr_bonus++;                                           // Indique que l'on vient de rajouter un bonus a la liste.
      }
      break;

    // Test si l'on doit ajouter un bonus qui incremente de deux la barre des options.
    case BONUS_INCR_BY_2:
      if(nbr_bonus < (MAX_BONUS_ON_SCREEN - 1))                // on ajoute un bonus a la liste des bonus ?
      { bon[nbr_bonus].genre = 1;
        bon[nbr_bonus].trajectoire = 0;
        bon[nbr_bonus].nbr_img = NBR_IMAGES_BONUS;
        bon[nbr_bonus].img_act = 0;
        bon[nbr_bonus].nbr_tot_vbl = 4;
        bon[nbr_bonus].nbr_vbl_act = 0;
        for(m = 0; m < bon[nbr_bonus].nbr_img; m++)
        { bon[nbr_bonus].img[m] = (image *) & bonus[1][m];     // donne les adresses des images du bonus.
        }
        bon[nbr_bonus].coor_x = pos_x;
        bon[nbr_bonus].coor_y = pos_y;
        bon[nbr_bonus].vitesse = 0.35f;
        nbr_bonus++;                                           // Indique que l'on vient de rajouter un bonus a la liste.
      }
      break;
      
      // Test si l'on doit ajouter un bonus qui donne une boule.
    case BONUS_PROT_BALL:
      if(nbr_bonus < (MAX_BONUS_ON_SCREEN - 1))                // on ajoute un bonus a la liste des bonus ?
      { bon[nbr_bonus].genre = 2;
        bon[nbr_bonus].trajectoire = 0;
        bon[nbr_bonus].nbr_img = NBR_IMAGES_BONUS;
        bon[nbr_bonus].img_act = 0;
        bon[nbr_bonus].nbr_tot_vbl = 4;
        bon[nbr_bonus].nbr_vbl_act = 0;
        for(m = 0; m < bon[nbr_bonus].nbr_img; m++)
        { bon[nbr_bonus].img[m] = (image *) & bonus[2][m];     // donne les adresses des images du bonus.
        }
        bon[nbr_bonus].coor_x = pos_x;
        bon[nbr_bonus].coor_y = pos_y;
        bon[nbr_bonus].vitesse = 0.55f;
        nbr_bonus++;                                           // Indique que l'on vient de rajouter un bonus a la liste.
      }
      break;

    // Test si l'on doit ajouter un bonus qui remonte l'energie du joueur.
    case BONUS_INCR_ENERGY:
      if(nbr_bonus < (MAX_BONUS_ON_SCREEN - 1))                // on ajoute un bonus a la liste des bonus ?
      { bon[nbr_bonus].genre = 3;
        bon[nbr_bonus].trajectoire = 0;
        bon[nbr_bonus].nbr_img = NBR_IMAGES_BONUS;
        bon[nbr_bonus].img_act = 0;
        bon[nbr_bonus].nbr_tot_vbl = 4;
        bon[nbr_bonus].nbr_vbl_act = 0;
        for(m = 0; m < bon[nbr_bonus].nbr_img; m++)
        { bon[nbr_bonus].img[m] = (image *) & bonus[3][m];     // donne les adresses des images du bonus.
        }
        bon[nbr_bonus].coor_x = pos_x;
        bon[nbr_bonus].coor_y = pos_y;
        bon[nbr_bonus].vitesse = 0.45f;
        nbr_bonus++;                                           // Indique que l'on vient de rajouter un bonus a la liste.
      }
      break;

  // Test si l'on doit ajouter le bonus x2.
    case BONUS_TIMES_2:
      if(nbr_bonus < (MAX_BONUS_ON_SCREEN - 1))                // on ajoute un bonus a la liste des bonus ?
      { bon[nbr_bonus].genre = 4;
        bon[nbr_bonus].trajectoire = 0;
        bon[nbr_bonus].nbr_img = NBR_IMAGES_BONUS;
        bon[nbr_bonus].img_act = 0;
        bon[nbr_bonus].nbr_tot_vbl = 4;
        bon[nbr_bonus].nbr_vbl_act = 0;
        for(m = 0; m < bon[nbr_bonus].nbr_img; m++)
        { bon[nbr_bonus].img[m] = (image *) & bonus[4][m];     // donne les adresses des images du bonus.
        }
        bon[nbr_bonus].coor_x = pos_x;
        bon[nbr_bonus].coor_y = pos_y;
        bon[nbr_bonus].vitesse = 0.65f;
        nbr_bonus++;                                           // Indique que l'on vient de rajouter un bonus a la liste.
      }
      break;

    // Test si l'on doit ajouter un vaisseau special.
    case BONUS_SPECIAL_SHIP:
      if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2)) Add_Ve_Special(-1);
      break;

    default:
      break;
  }
}

//------------------------------------------------------------------------------
// Selectionne le bonus genere par la destruction d'un meteor (Laurent Duperval)
//------------------------------------------------------------------------------
int Calculate_Bonus_Meteor(int value)
{
  int retValue = -1;
  switch (power_conf->difficulty)
  { case 0: 
      if (value < 10)                                                           { retValue = BONUS_INCR_BY_1; }
      if (value==10 || value == 11)                                             { retValue = BONUS_INCR_BY_2; }
      else if (value==6 || value == 12 || value == 36)                          { retValue = BONUS_PROT_BALL; }
      else if (value==7 || value == 21 || value==35 || value==50 || value== 56) { retValue = BONUS_INCR_ENERGY; }
      else if (value==40 || value == 20)                                        { retValue = BONUS_TIMES_2; }
      else if (value==8 || value==49)                                           { retValue = BONUS_SPECIAL_SHIP; }
      break;

    case 1:
      if (value < 5)                                                            { retValue = BONUS_INCR_BY_1; }
      if (value==5)                                                             { retValue = BONUS_INCR_BY_2; }
      else if (value==6)                                                        { retValue = BONUS_PROT_BALL; }
      else if (value==7 || value==35 || value==50)                              { retValue = BONUS_INCR_ENERGY; }
      else if (value==40 && !(rand()%3))                                        { retValue = BONUS_TIMES_2; }
      else if (value==8 || value==26 || value==49 || value==59)                 { retValue = BONUS_SPECIAL_SHIP; }
      break;

    case 2:
      if (value < 3)                                                            { retValue = BONUS_INCR_BY_1; }
      if (value==5 && (rand()%2))                                               { retValue = BONUS_INCR_BY_2; }
      else if (value==6 && (rand()%2))                                          { retValue = BONUS_PROT_BALL; }
      else if (value==7 || value==35 || value==50 && (rand() %2))               { retValue = BONUS_INCR_ENERGY; }
      else if (value==40 && !(rand()%6))                                        { retValue = BONUS_TIMES_2; }
      else if (!(rand()%6))                                                       { retValue = BONUS_SPECIAL_SHIP; }
  }
  return retValue;
}


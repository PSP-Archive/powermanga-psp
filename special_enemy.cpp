//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "special_enemy.cpp"
// created      : 09-09-1999
// updates      : 2005-01-05
// id		: $Id: special_enemy.cpp,v 1.1.1.1 2005/01/25 05:55:14 gurumeditation Exp $
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
extern image            enemi[MAX_TYP_OF_ENEMI + MAX_TYP_OF_S_ENEMI + MAX_TYP_OF_NMI_SPE + MAX_TYP_OF_OBJ_SPE][NBR_IMAGES_MAX]; // Définit le tableau des différents ennemis.
extern vaisseau         vj[NBR_MAX_J];                         //structure of player's ship
extern int              nbr_ve_spe_generes;                    //number of specials enemy
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //structure of enemys's ship
extern int              new_ve;                                //index to a new enemy
extern curve            initial_curve[NBR_CURVE_MAX];          //bezier'curves structure
extern char            *pEnnemisNoms[];

//------------------------------------------------------------------------------
// ajoute un vaisseau special a la liste des vaisseau ennemis
// entree => val_ve_special : numero de l'enemeni (ou -1 si aleatoire)
//------------------------------------------------------------------------------
void Add_Ve_Special(int val_ve_special)
{
  int                     k;
#ifdef USE_SDLMIXER
  seal_joue(SPECNMI);                                          //play sound with seal
#endif
  if(val_ve_special == -1)                                     //choisis un ennemi aleatoirement ?
  { nbr_ve_spe_generes++;                                      //incremente le nombre de vaisseaux specials generes
    // Clip le nombre de vaisseaux spéciaux générés.
    if(nbr_ve_spe_generes > MAX_TYP_OF_NMI_SPE)
    {
      nbr_ve_spe_generes = MAX_TYP_OF_NMI_SPE;                 //determine au hasard le vaiseau special qui va etre ajoute à la liste.
      val_ve_special = (int)(((long)rand() % nbr_ve_spe_generes));
    }
    else
    {                                                          // Détermine le vaiseau spécial qui va etre ajouté à la liste.
      val_ve_special = nbr_ve_spe_generes - 1;
      if(val_ve_special < 0)
        val_ve_special = 0;
    }
  }
  switch (val_ve_special)
  {                                                            // Vaisseau spécial SUBJUGANEERS.
    case 0:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + SUBJUGANEERS - 40);    // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[SUBJUGANEERS][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x = DEBUT_ECRAN_X + 64 + LARG_ECR_RAM;
      ve[new_ve].spr.coor_y =
        DEBUT_ECRAN_Y +
        (float)(((long)rand() %
                 (HAUT_ECR_RAM - ve[new_ve].spr.img[0]->haut)));
      ve[new_ve].spr.vitesse = -0.5;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = SUBJUGANEERS;                          // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial MILLOUZ.
    case 1:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + MILLOUZ - 40); // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 3;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[MILLOUZ][k];
      }
      ve[new_ve].cad_tir_initiale = 0;                         // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(DEBUT_ECRAN_X +
                ((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.4f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = MILLOUZ;                               // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial SWORDINIANS.
    case 2:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + SWORDINIANS - 40);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[SWORDINIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y = DEBUT_ECRAN_Y + 64 + HAUT_ECR_RAM;
      ve[new_ve].spr.vitesse = -0.3f;                          // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = SWORDINIANS;                           // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial TOUBOUG.
    case 3:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + TOUBOUG - 40); // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[TOUBOUG][k];
      }
      ve[new_ve].cad_tir_initiale = 0;                         // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(DEBUT_ECRAN_X +
                ((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.35f;                          // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = TOUBOUG;                               // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial DISGOOSTEES.
    case 4:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + DISGOOSTEES - 40);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[DISGOOSTEES][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.6f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = DISGOOSTEES;                           // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial EARTHINIANS.
    case 5:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + EARTHINIANS - 40);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[EARTHINIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.6f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = EARTHINIANS;                           // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial BIRIANSTEES.
    case 6:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + BIRIANSTEES - 40);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[BIRIANSTEES][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.6f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = BIRIANSTEES;                           // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial BELCHOUTIES.
    case 7:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + BELCHOUTIES - 40);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[BELCHOUTIES][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.6f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = BELCHOUTIES;                           // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial VIONIEES.
    case 8:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + VIONIEES - 40);        // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[VIONIEES][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 32 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 2.0f + (float)(((long)rand() % (100))) / 100.0f; // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = VIONIEES;                              // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
      ve[new_ve].retournement = 0;                             // Vaut 1 si le vaisseau ennemi est dans sa phase de retournement, sinon 0.
      ve[new_ve].change_dir = 0;                               // Vaut 1 si le vaisseau ennemi à changé de direction, sinon 0.
    }
      break;

      // Vaisseau spécial HOCKYS.
    case 9:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + HOCKYS - 40);  // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[HOCKYS][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y = DEBUT_ECRAN_Y + 64 + HAUT_ECR_RAM;
      ve[new_ve].spr.vitesse = -0.4f;                          // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = HOCKYS;                                // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial TODHAIRIES.
    case 10:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + TODHAIRIES - 40);      // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[TODHAIRIES][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (60)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.6f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = TODHAIRIES;                            // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial DEFECTINIANS.
    case 11:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + DEFECTINIANS - 40);    // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[DEFECTINIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (60)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.6f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = DEFECTINIANS;                          // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial BLAVIRTHE.
    case 12:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + BLAVIRTHE - 40);       // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].num_courbe = (short)(51 + (int)(((long)rand() % (4))));        // Donne le numéro de la courbe utilisée par le vaisseau ennemi.
      ve[new_ve].spr.img_act = initial_curve[ve[new_ve].num_courbe].angle[0];   // Indique l'image actuelle.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[BLAVIRTHE][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (60)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      ve[new_ve].spr.coor_x = (float)(initial_curve[ve[new_ve].num_courbe].pos_x + 128 - 32);   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].spr.coor_y = (float)(initial_curve[ve[new_ve].num_courbe].pos_y + 128 - 32);   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].pos_vaiss[POS_CURVE] = 0;                     // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].type = BLAVIRTHE;                             // Indique que l'on se trouve en présence d'un vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial SOONIEES.
    case 13:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + SOONIEES - 40);        // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[SOONIEES][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (60)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.6f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = SOONIEES;                              // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial ANGOUFF.
    case 14:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + ANGOUFF - 40); // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[ANGOUFF][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 32 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 2.0f + (float)(((long)rand() % (100))) / 100.0f; // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = ANGOUFF;                               // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
      ve[new_ve].retournement = 0;                             // Vaut 1 si le vaisseau ennemi est dans sa phase de retournement, sinon 0.
      ve[new_ve].change_dir = 0;                               // Vaut 1 si le vaisseau ennemi à changé de direction, sinon 0.
    }
      break;

      // Vaisseau spécial GAFFIES.
    case 15:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + GAFFIES - 40); // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 6;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[GAFFIES][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (60)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.2f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = GAFFIES;                               // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial BITTERIANS.
    case 16:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + BITTERIANS - 40);      // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[BITTERIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x = DEBUT_ECRAN_X + 64 + LARG_ECR_RAM;
      ve[new_ve].spr.coor_y =
        DEBUT_ECRAN_Y +
        (float)(((long)rand() %
                 (HAUT_ECR_RAM - ve[new_ve].spr.img[0]->haut)));
      ve[new_ve].spr.vitesse = -0.5;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = BITTERIANS;                            // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial BLEUERCKS.
    case 17:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + BLEUERCKS - 40);       // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].num_courbe = (short)(51 + (int)(((long)rand() % (4))));        // Donne le numéro de la courbe utilisée par le vaisseau ennemi.
      ve[new_ve].spr.img_act = initial_curve[ve[new_ve].num_courbe].angle[0];   // Indique l'image actuelle.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[BLEUERCKS][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      ve[new_ve].spr.coor_x = (float)(initial_curve[ve[new_ve].num_courbe].pos_x + 128 - 32);   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].spr.coor_y = (float)(initial_curve[ve[new_ve].num_courbe].pos_y + 128 - 32);   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].pos_vaiss[POS_CURVE] = 0;                     // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].type = BLEUERCKS;                             // Indique que l'on se trouve en présence d'un vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial ARCHINIANS.
    case 18:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + ARCHINIANS - 40);      // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 5;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[ARCHINIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y = DEBUT_ECRAN_Y + 64 + HAUT_ECR_RAM;
      ve[new_ve].spr.vitesse = -0.2f;                          // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = ARCHINIANS;                            // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial CLOWNIES.
    case 19:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + CLOWNIES - 40);        // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[CLOWNIES][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 2.5f + (float)(((long)rand() % (100))) / 100.0f; // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = CLOWNIES;                              // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
      ve[new_ve].retournement = 0;                             // Vaut 1 si le vaisseau ennemi est dans sa phase de retournement, sinon 0.
      ve[new_ve].change_dir = 0;                               // Vaut 1 si le vaisseau ennemi à changé de direction, sinon 0.
    }
      break;

      // Vaisseau spécial DEMONIANS.
    case 20:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + DEMONIANS - 40);       // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[DEMONIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = DEMONIANS;                             // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial TOUTIES.
    case 21:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + TOUTIES - 40); // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[TOUTIES][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x = DEBUT_ECRAN_X + 64 + LARG_ECR_RAM;
      ve[new_ve].spr.coor_y =
        DEBUT_ECRAN_Y +
        (float)(((long)rand() %
                 (HAUT_ECR_RAM - ve[new_ve].spr.img[0]->haut)));
      ve[new_ve].spr.vitesse = -0.35f;                         // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = TOUTIES;                               // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial FIDGETINIANS.
    case 22:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + FIDGETINIANS - 40);    // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[FIDGETINIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = FIDGETINIANS;                          // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial EFFIES.
    case 23:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + EFFIES - 40);  // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[EFFIES][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 2.5f + (float)(((long)rand() % (100))) / 100.0f; // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = EFFIES;                                // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
      ve[new_ve].retournement = 0;                             // Vaut 1 si le vaisseau ennemi est dans sa phase de retournement, sinon 0.
      ve[new_ve].change_dir = 0;                               // Vaut 1 si le vaisseau ennemi à changé de direction, sinon 0.
    }
      break;

      // Vaisseau spécial DIMITINIANS.
    case 24:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + DIMITINIANS - 40);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 6;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[DIMITINIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].sens_anim = 0;                                // Indique que le sens de comptage des images de l'animation est croissant.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.3f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = DIMITINIANS;                           // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseu spécial PAINIANS.
    case 25:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + PAINIANS - 40);        // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[PAINIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(DEBUT_ECRAN_X - 64 - ve[new_ve].spr.img[0]->larg);
      ve[new_ve].spr.coor_y =
        DEBUT_ECRAN_Y +
        (float)(((long)rand() %
                 (HAUT_ECR_RAM - ve[new_ve].spr.img[0]->haut)));
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = PAINIANS;                              // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial ENSLAVEERS.
    case 26:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + ENSLAVEERS - 40);      // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[ENSLAVEERS][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = +0.6f;                          // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = ENSLAVEERS;                            // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial FEABILIANS.
    case 27:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + FEABILIANS - 40);      // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[FEABILIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x = DEBUT_ECRAN_X + 64 + LARG_ECR_RAM;
      ve[new_ve].spr.coor_y =
        DEBUT_ECRAN_Y +
        (float)(((long)rand() %
                 (HAUT_ECR_RAM - ve[new_ve].spr.img[0]->haut)));
      ve[new_ve].spr.vitesse = -0.5;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = FEABILIANS;                            // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial DIVERTIZERS.
    case 28:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + DIVERTIZERS - 40);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 3;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[DIVERTIZERS][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)((((long)rand() %
                  (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg))));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.6f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = DIVERTIZERS;                           // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial SAPOUCH.
    case SAPOUCH:
    case 29:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + SAPOUCH - 40); // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[SAPOUCH][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 2.5f + (float)(((long)rand() % (100))) / 100.0f; // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = SAPOUCH;                               // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
      ve[new_ve].retournement = 0;                             // Vaut 1 si le vaisseau ennemi est dans sa phase de retournement, sinon 0.
      ve[new_ve].change_dir = 0;                               // Vaut 1 si le vaisseau ennemi à changé de direction, sinon 0.
    }
      break;

      // Vaisseau spécial HORRIBIANS.
    case 30:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + HORRIBIANS - 40);      // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 3;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[HORRIBIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)((((long)rand() %
                  (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg))));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.6f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = HORRIBIANS;                            // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial CARRYONIANS.
    case 31:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + CARRYONIANS - 40);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 5;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[CARRYONIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y = DEBUT_ECRAN_Y + 64 + HAUT_ECR_RAM;
      ve[new_ve].spr.vitesse = -0.2f;                          // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = CARRYONIANS;                           // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial DEVILIANS.
    case DEVILIANS:
    case 32:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + DEVILIANS - 40);       // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 5;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[DEVILIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 60 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)((((long)rand() %
                  (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg))));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = +0.5;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = DEVILIANS;                             // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial ROUGHLEERS.
    case 33:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + ROUGHLEERS - 40);      // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 6;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[ROUGHLEERS][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = ROUGHLEERS;                            // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial ABASCUSIANS.
    case 34:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + ABASCUSIANS - 40);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[ABASCUSIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = ABASCUSIANS;                           // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial ROTIES.
    case 35:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + ROTIES - 40);  // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].num_courbe = (short)(51 + (int)(((long)rand() % (4))));        // Donne le numéro de la courbe utilisée par le vaisseau ennemi.
      ve[new_ve].spr.img_act = initial_curve[ve[new_ve].num_courbe].angle[0];   // Indique l'image actuelle.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[ROTIES][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      ve[new_ve].spr.coor_x = (float)(initial_curve[ve[new_ve].num_courbe].pos_x + 128 - 32);   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].spr.coor_y = (float)(initial_curve[ve[new_ve].num_courbe].pos_y + 128 - 32);   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].pos_vaiss[POS_CURVE] = 0;                     // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].type = ROTIES;                                // Indique que l'on se trouve en présence d'un vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial STENCHIES.
    case 36:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + STENCHIES - 40);       // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].num_courbe = (short)(51 + (int)(((long)rand() % (4))));        // Donne le numéro de la courbe utilisée par le vaisseau ennemi.
      ve[new_ve].spr.img_act = initial_curve[ve[new_ve].num_courbe].angle[0];   // Indique l'image actuelle.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[STENCHIES][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      ve[new_ve].spr.coor_x = (float)(initial_curve[ve[new_ve].num_courbe].pos_x + 128 - 32);   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].spr.coor_y = (float)(initial_curve[ve[new_ve].num_courbe].pos_y + 128 - 32);   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].pos_vaiss[POS_CURVE] = 0;                     // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].type = STENCHIES;                             // Indique que l'on se trouve en présence d'un vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial PERTURBIANS.
    case 37:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + PERTURBIANS - 40);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 6;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[PERTURBIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        DEBUT_ECRAN_X +
        (float)(((long)rand() %
                 (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg)));
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - 64 - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.2f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = PERTURBIANS;                           // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial MADIRIANS.
    case 38:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + MADIRIANS - 40);       // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].num_courbe = (short)(51 + (int)(((long)rand() % (4))));        // Donne le numéro de la courbe utilisée par le vaisseau ennemi.
      ve[new_ve].spr.img_act = initial_curve[ve[new_ve].num_courbe].angle[0];   // Indique l'image actuelle.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[MADIRIANS][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (50)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      ve[new_ve].spr.coor_x = (float)(initial_curve[ve[new_ve].num_courbe].pos_x + 128 - 32);   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].spr.coor_y = (float)(initial_curve[ve[new_ve].num_courbe].pos_y + 128 - 32);   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].pos_vaiss[POS_CURVE] = 0;                     // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].type = MADIRIANS;                             // Indique que l'on se trouve en présence d'un vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseu spécial BAINIES.
    case 39:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + BAINIES - 40); // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 10); // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[BAINIES][k];
      }
      ve[new_ve].cad_tir_initiale = 50 + (int)(((long)rand() % (40)));  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(DEBUT_ECRAN_X - 64 - ve[new_ve].spr.img[0]->larg);
      ve[new_ve].spr.coor_y =
        DEBUT_ECRAN_Y +
        (float)(((long)rand() %
                 (HAUT_ECR_RAM - ve[new_ve].spr.img[0]->haut)));
      ve[new_ve].spr.vitesse = 0.4f;                           // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = BAINIES;                               // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;

      // Vaisseau spécial NAGGYS.
    case NAGGYS:
    {                                                          // On rajoute un Vaisseau spécial NAGGYS dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)(vj[J1].type_vaisseau << 2);  // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)(ve[new_ve].spr.pow_of_dest + 20);  // Indique l'état de domages du sprite.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 2;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[NAGGYS][k];
      }
      ve[new_ve].cad_tir_initiale = 15;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = SPECIAUX;                       // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(DEBUT_ECRAN_X - 64 - ve[new_ve].spr.img[0]->larg);
      ve[new_ve].spr.coor_y = DEBUT_ECRAN_Y + 48 + (float)(((long)rand() % 32));        //(float)(((long)rand()%(HAUT_ECR_RAM-ve[new_ve].spr.img[0]->haut)));
      ve[new_ve].spr.vitesse = 2.0;                            // Donne la vitesse de déplacement du vaisseau ennemi spécial.
      ve[new_ve].type = NAGGYS;                                // Indique le type de vaisseau spécial.
      ve[new_ve].dead = 0;                                     // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = 1;                                  // Indique que l'ennemi est visible, on devra donc l'afficher.
    }
      break;
  }
}

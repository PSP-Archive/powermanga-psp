//******************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//------------------------------------------------------------------------------
// functions    : Sint32   new_element_tir()
//                Sint32   New_Element_Tir()
//                void  Del_Element_Tir(Sint32 n, Sint32 o)
//                Sint32   New_Element_Ve()
//                Sint32   New_Element_Bou()
//                void  fichierFlag()
//                void  fichierFlag2() 
//                void  entierEnASCII(Uint32 _iNombre, char *_pASCII)
//                void  entier2ASCII(Sint32 _iNombre, char *_pASCII, Uint32 _iReste)
//------------------------------------------------------------------------------
// file         : "powmangasub2.cpp" 
// created      : ?
// updates      : 2005-01-08
// id		: $Id: powmangasub2.cpp,v 1.1.1.1 2005/01/25 05:54:55 gurumeditation Exp $
//------------------------------------------------------------------------------
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
//******************************************************************************
#include "powermanga.hpp"
//..............................................................................
extern Uint32     iCompteurGlobal;                       //compteur global a tout faire
extern bool             eclair_actif;                          //Indique si le joueur peut ou non utiliser l'eclair.
extern Sint32              nbchannel;                             //Utilise pour connaitre le nombre de voix utilisées pour le son.
extern Sint32              fxhandle_xplo1;                        //Handle du son de l'explosion 1.
extern bool             meteor_activity;                       //Utilisé pour savoir si on est dans une phase de météorites ou non.
extern bool             disparition_vj;                        //Utilisé pour indiquer la disparition du vaisseau du joueur.
extern curve_level      courbe;                                //Déclaration de la structure de niveaux courbe.
extern float            vit_star_field;                        //Vitesse du champs d'étoiles.
extern Sint32              num_level;                             //numero du niveau en cours
extern float            pos_x_level, pos_y_level;              // Coordonées x et y de la position d'affichage du niveau.
extern bool             move_level;                            // Variable utilisée pour le déplacement du niveau pendant le jeu.
extern char            *chaine;                                // Utilisé pour la conversion des chaines de caractères.
extern short            touch[];                               // Tableau utilisé pour recevoir les flag des touches du clavier.
extern tir              tirs[NBR_TIRS_MAX];                    // Définit le tableau de tous les objets tirs.
extern Sint32              liste_tirs[NBR_TIRS_MAX];              // Définit la liste de tous les 'tirs' que l'on doit gérés à chaque VBLs.
extern short            nbr_tirs;                              // Indique le nombre de tir actuellement gérés.
extern star             stars[NBR_OF_STARS];                   // Définit le tableau d'étoiles utilisées pour le star fiels.
extern image            star_field[TYPE_OF_STARS][NBR_ETOILES]; // Définit le tableau des différentes images d'étoiles utilisées pour le star field.
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //structure des vaisseaux ennemis
extern Sint32              liste_ve[MAX_NMI_ON_SCREEN];           // Définit la liste de tous les 've' que l'on doit gérés à chaque VBLs.
extern Sint32              cmpt_nmis;                             // Utilisé dans les bcles pour compter le nombre d'ennemis.
extern short            nbr_nmis;                              // Indique le nombre d'enemis actuellement gérés.
extern grid             grille;                                // Déclaration de la structure de grille des vaisseaux ennemis.

#ifdef DEVELOPPEMENT
extern short            mouse_x, mouse_y, mouse_here;
#endif
extern short            t_l, t_s;                              // Variables utilisées lors du chargement ou de la sauvegarde d'un fichier dans les éditeurs.
extern char            *ecran_ram;                             // Adresse de l'écran logique.
extern char            *ecran;                                 // Adresse de l'écran physique.
extern short            ge_act_pos_x, ge_act_pos_y;            // Variables indiquant la position du carré actuellement sélectionné dans la grille.
extern unsigned char    coulor[];                              // Tableau contenant les couleurs choisies par l'utilisateur.
extern image            meteo[MAX_TYP_OF_METEOR][NBR_IMAGES_METEOR];    // Définit le tableau des différentes météorite.
extern image            enemi[MAX_TYP_OF_ENEMI + MAX_TYP_OF_S_ENEMI + MAX_TYP_OF_NMI_SPE + MAX_TYP_OF_OBJ_SPE][NBR_IMAGES_MAX]; // Définit le tableau des différents ennemis.
extern Sint32              new_ve;                                //indice tableau sur un nouvel objet 've'
extern vaisseau         vj[NBR_MAX_J];                         //structure des vaisseaux des joueurs
extern image            fire[MAX_TYP_OF_TIR][NBR_IMAGES_TIR];  // Définit le tableau des différents tirs.
extern image            boule[MAX_TYP_OF_BOULES][NBR_IMAGES_BOULES];    // tableau des images des boules
extern image            canon[MAX_TYP_OF_CANONS][NBR_IMAGES_CANONS];    // tableau des images des canons
extern fonte            doublex[5];                            // Définit le tableau des différentes images des multiplicateurs.

extern short            nbr_boules;                            // Indique le nombre de boules actuellement gérés.
extern str_boule        bou[MAX_BOULES_ON_SCREEN];             // Déclaration de la structure des boules.
extern Sint32              liste_bou[MAX_BOULES_ON_SCREEN];       // Définit la liste de toutes les 'bou' que l'on doit gérés à chaque VBLs.

extern short            nbr_canons;                            // Indique le nombre de canons actuellement gérés.
extern Sint32              liste_can[MAX_CANONS_ON_SCREEN];       // Définit la liste de toutes les 'can' que l'on doit gérés à chaque VBLs.
extern str_canon        can[MAX_CANONS_ON_SCREEN];             // Déclaration de la structure des canons.
extern Sint32              new_can;                               // Utilisée pour pointer sur un nouvel élément 'can'.

short                   grid_number = 0;                       // Variable indiquant le numéro de grille actuelle.
short                   ge_vais_act = 0;                       // Variable utilisée pour indiquer le vaisseau actuellement sélectionné dans l'éditeur de grille.

//------------------------------------------------------------------------------
// ajoute un tir (avec son) / ajout a fire (with a sound)
// sortie <= index sur le tableau "tirs"
//------------------------------------------------------------------------------
Sint32 new_element_tir()
{
#ifdef USE_SDLMIXER
	seal_joue(TIR1NMI + (iCompteurGlobal & 1));
#endif
	return (New_Element_Tir());
}

//------------------------------------------------------------------------------
//ajoute un tir (sans son)
//sortie <= index sur le tableau "tirs"
//------------------------------------------------------------------------------
Sint32 New_Element_Tir()
{
  Sint32                     l, m;

  if(nbr_tirs)                                                 //il existe deja des tirs geres ?
  {
    for(m = 0; m < NBR_TIRS_MAX; m++)                          //oui, on recherche un 
    {                                                          // Si une valeur next_object est égale au nombre maximum d'objets du tableau, c'est qu'on tient un objet libre.
      if(tirs[m].next_object == NBR_TIRS_MAX)
        goto New_Element_Tir_Find;
    }
    //modifie la liste chainee
  New_Element_Tir_Find:;
    l = liste_tirs[nbr_tirs - 1];                              //l=ex-dernier element de la liste
    tirs[l].next_object = m;                                   //m=element suivant a l'ex-dernier element de la liste
    tirs[m].prev_object = l;                                   //l=element precedent au nouveau dernier element de la liste
    tirs[m].next_object = -1;                                  //-1=dernier element de la liste
    liste_tirs[nbr_tirs] = m;                                  //met a jour la liste des tirs
    nbr_tirs++;                                                //incremente le nombre d'objets tirs actuellement geres
    return (m);
  }

  //tableau 'tirs' vide : pas de tirs a l'ecran ----------------------------------
  tirs[0].prev_object = -1;
  tirs[0].next_object = -1;
  liste_tirs[nbr_tirs] = 0;                                    //met a jour la liste tirs
  nbr_tirs++;                                                  //incremente le nombre d'objets tirs actuellement geres
  return (0);                                                  //index zero : premier element du tableau
}

//------------------------------------------------------------------------------
// Efface le nième élément de la liste et fait les mises à jour nécessaire.
//entree        => n :
//                              => o :
//------------------------------------------------------------------------------
void Del_Element_Tir(Sint32 n, Sint32 o)
{
  Sint32                     m;

  // On doit à présent modifier les éléments suivants et précédent de la liste pour faire un chaînage correct.
  if(tirs[n].prev_object != -1)
    tirs[tirs[n].prev_object].next_object = tirs[n].next_object;        // Attribution de l'élément suivant de la liste.
  if(tirs[n].next_object != -1)
    tirs[tirs[n].next_object].prev_object = tirs[n].prev_object;        // Attribution de l'élément suivant de la liste.
  tirs[n].prev_object = NBR_TIRS_MAX;                          // Indique que l'objet actuel est libre.
  tirs[n].next_object = NBR_TIRS_MAX;                          // Indique que l'objet actuel est libre.
  // Met à jour la liste des 'tirs'.
  for(m = o; m < (nbr_tirs - 1); m++)
    liste_tirs[m] = liste_tirs[m + 1];
  nbr_tirs--;                                                  // Décrémente le nombre d'objets 'tirs' actuellement gérés.
}


// Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
//------------------------------------------------------------------------------
// demande un nouvel ennemi
// <=sortie : pointeur sur 
//------------------------------------------------------------------------------
Sint32 New_Element_Ve()
{
  Sint32                     l, m;

  // Test s'il y a plusieurs éléments dans le tableau.
  if(nbr_nmis)                                                 // Si oui, on recherche un objet libre pour attribution.
  {                                                            // Recherche d'un objet libre dans le tableau.
    for(m = 0; m < MAX_NMI_ON_SCREEN; m++)
    {                                                          // Si une valeur next_object est égale au nombre maximum d'objets du tableau, c'est qu'on tient un objet libre.
      if(ve[m].next_object == MAX_NMI_ON_SCREEN)
        goto New_Element_Ve_Find;
    }

  New_Element_Ve_Find:;

    // On doit à présent modifier le dernier élément de la liste pour le chaîner au nouvel élément.
    l = liste_ve[nbr_nmis - 1];                                // Donne le dernier élément de la liste.
    ve[l].next_object = m;                                     // Attribution de l'élément suivant à ce qui était le dernier élément de la liste.
    ve[m].prev_object = l;                                     // Attribution de l'élément précédent au dernier élément de la liste.
    ve[m].next_object = -1;                                    // Indique  que cet élément est le dernier de la liste.
    liste_ve[nbr_nmis] = m;                                    // Met à jour la liste des 've'.
    nbr_nmis++;                                                // Incrémente le nombre d'objets 've' actuellement gérés.
    //printf("nbr_nmis=%i\n", nbr_nmis) ;
    return (m);
  }

  // Si on arrive en ce point c'est que le tableau est actuellement vide. On peux alors attribuer le premier élément.
  ve[0].prev_object = -1;
  ve[0].next_object = -1;
  liste_ve[nbr_nmis] = 0;                                      // Met à jour la liste des 've'.
  nbr_nmis++;                                                  // Incrémente le nombre d'objets 've' actuellement gérés.
  return (0);                                                  // Indique que le nouvel élément de la liste est le premier élément du tableau.
}

// Efface le nième élément de la liste et fait les mises à jour nécessaire.
void Del_Element_Ve(Sint32 n, Sint32 o)
{
  Sint32                     m;

  // On doit à présent modifier les éléments suivants et précédent de la liste pour faire un chaînage correct.
  if(ve[n].prev_object != -1)
    ve[ve[n].prev_object].next_object = ve[n].next_object;     // Attribution de l'élément suivant de la liste.
  if(ve[n].next_object != -1)
    ve[ve[n].next_object].prev_object = ve[n].prev_object;     // Attribution de l'élément suivant de la liste.
  ve[n].prev_object = MAX_NMI_ON_SCREEN;                       // Indique que l'objet actuel est libre.
  ve[n].next_object = MAX_NMI_ON_SCREEN;                       // Indique que l'objet actuel est libre.
  // Met à jour la liste des 've'.
  for(m = o; m < (nbr_nmis - 1); m++)
    liste_ve[m] = liste_ve[m + 1];
  nbr_nmis--;                                                  // Décrémente le nombre d'objets 've' actuellement gérés.
}





//------------------------------------------------------------------------------
// Retourne l'indice dans le tableau 'bou' d'un nouvel élément de la liste.
//------------------------------------------------------------------------------
Sint32 New_Element_Bou()
{
  Sint32                     l, m;

  // Test s'il y a plusieurs éléments dans le tableau.
  if(nbr_boules)                                               // Si oui, on recherche un objet libre pour attribution.
  {                                                            // Recherche d'un objet libre dans le tableau.
    for(m = 0; m < MAX_BOULES_ON_SCREEN; m++)
    {                                                          // Si une valeur next_object est égale au nombre maximum d'objets du tableau, c'est qu'on tient un objet libre.
      if(bou[m].next_object == MAX_BOULES_ON_SCREEN)
        goto New_Element_Bou_Find;
    }

  New_Element_Bou_Find:;

    // On doit à présent modifier le dernier élément de la liste pour le chaîner au nouvel élément.
    l = liste_bou[nbr_boules - 1];                             // Donne le dernier élément de la liste.
    bou[l].next_object = m;                                    // Attribution de l'élément suivant à ce qui était le dernier élément de la liste.
    bou[m].prev_object = l;                                    // Attribution de l'élément précédent au dernier élément de la liste.
    bou[m].next_object = -1;                                   // Indique  que cet élément est le dernier de la liste.
    liste_bou[nbr_boules] = m;                                 // Met à jour la liste des 'bou'.
    nbr_boules++;                                              // Incrémente le nombre d'objets 'bou' actuellement gérés.
    return (m);
  }

  // Si on arrive en ce point c'est que le tableau est actuellement vide. On peux alors attribuer le premier élément.
  bou[0].prev_object = -1;
  bou[0].next_object = -1;
  liste_bou[nbr_boules] = 0;                                   // Met à jour la liste des 'bou'.
  nbr_boules++;                                                // Incrémente le nombre d'objets 'bou' actuellement gérés.
  return (0);                                                  // Indique que le nouvel élément de la liste est le premier élément du tableau.
}

//------------------------------------------------------------------------------
// Efface le nième élément de la liste et fait les mises à jour nécessaire.
//------------------------------------------------------------------------------
void Del_Element_Bou(Sint32 n, Sint32 o)
{
  Sint32                     m;

  eclair_actif = 0;                                            //Indique que le joueur ne peut plus utiliser l'eclair.
  // On doit à présent modifier les éléments suivants et précédent de la liste pour faire un chaînage correct.
  if(bou[n].prev_object != -1)
    bou[bou[n].prev_object].next_object = bou[n].next_object;  // Attribution de l'élément suivant de la liste.
  if(bou[n].next_object != -1)
    bou[bou[n].next_object].prev_object = bou[n].prev_object;  // Attribution de l'élément suivant de la liste.
  bou[n].prev_object = MAX_BOULES_ON_SCREEN;                   // Indique que l'objet actuel est libre.
  bou[n].next_object = MAX_BOULES_ON_SCREEN;                   // Indique que l'objet actuel est libre.
  // Met à jour la liste des 'bou'.
  for(m = o; m < (nbr_boules - 1); m++)
    liste_bou[m] = liste_bou[m + 1];
  nbr_boules--;                                                // Décrémente le nombre d'objets 'bou' actuellement gérés.
}

// Retourne l'indice dans le tableau 'can' d'un nouvel élément de la liste.
Sint32 New_Element_Can(void)
{
  Sint32                     l, m;

  // Test s'il y a plusieurs éléments dans le tableau.
  if(nbr_canons)                                               // Si oui, on recherche un objet libre pour attribution.
  {                                                            // Recherche d'un objet libre dans le tableau.
    for(m = 0; m < MAX_CANONS_ON_SCREEN; m++)
    {                                                          // Si une valeur next_object est égale au nombre maximum d'objets du tableau, c'est qu'on tient un objet libre.
      if(can[m].next_object == MAX_CANONS_ON_SCREEN)
        goto New_Element_Can_Find;
    }

  New_Element_Can_Find:;

    // On doit à présent modifier le dernier élément de la liste pour le chaîner au nouvel élément.
    l = liste_can[nbr_canons - 1];                             // Donne le dernier élément de la liste.
    can[l].next_object = m;                                    // Attribution de l'élément suivant à ce qui était le dernier élément de la liste.
    can[m].prev_object = l;                                    // Attribution de l'élément précédent au dernier élément de la liste.
    can[m].next_object = -1;                                   // Indique  que cet élément est le dernier de la liste.
    liste_can[nbr_canons] = m;                                 // Met à jour la liste des 'can'.
    nbr_canons++;                                              // Incrémente le nombre d'objets 'can' actuellement gérés.
    return (m);
  }

  // Si on arrive en ce point c'est que le tableau est actuellement vide. On peux alors attribuer le premier élément.
  can[0].prev_object = -1;
  can[0].next_object = -1;
  liste_can[nbr_canons] = 0;                                   // Met à jour la liste des 'can'.
  nbr_canons++;                                                // Incrémente le nombre d'objets 'can' actuellement gérés.
  return (0);                                                  // Indique que le nouvel élément de la liste est le premier élément du tableau.
}

//------------------------------------------------------------------------------
// Efface le nième élément de la liste et fait les mises à jour nécessaire.
//------------------------------------------------------------------------------
void Del_Element_Can(Sint32 n, Sint32 o)
{
  Sint32                     m;

  // On doit à présent modifier les éléments suivants et précédent de la liste pour faire un chaînage correct.
  if(can[n].prev_object != -1)
    can[can[n].prev_object].next_object = can[n].next_object;  // Attribution de l'élément suivant de la liste.
  if(can[n].next_object != -1)
    can[can[n].next_object].prev_object = can[n].prev_object;  // Attribution de l'élément suivant de la liste.
  can[n].prev_object = MAX_CANONS_ON_SCREEN;                   // Indique que l'objet actuel est libre.
  can[n].next_object = MAX_CANONS_ON_SCREEN;                   // Indique que l'objet actuel est libre.
  // Met à jour la liste des 'can'.
  for(m = o; m < (nbr_canons - 1); m++)
    liste_can[m] = liste_can[m + 1];
  nbr_canons--;                                                // Décrémente le nombre d'objets 'can' actuellement gérés.
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 1.
//------------------------------------------------------------------------------
void V1_Tir_Normal_Avant_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 3;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TN1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 2.
//------------------------------------------------------------------------------
void V1_Tir_Normal_Avant_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 3;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 23;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du second tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 3;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 25;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 3.
//------------------------------------------------------------------------------
void V1_Tir_Normal_Avant_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 3;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 3;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 23;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du second tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 3;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 25;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

/*
	// Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 1.
	V1_Tir_Normal_Avant_P1() ;
	// Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 2.
	V1_Tir_Normal_Avant_P2() ;
*/
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 4.
//------------------------------------------------------------------------------
void V1_Tir_Normal_Avant_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 3;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 23;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du second tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 3;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 25;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 3;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 22;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du second tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 3;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 26;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 5.
void V1_Tir_Normal_Avant_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 3;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 4.
  V1_Tir_Normal_Avant_P4();

}



//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 1.
//------------------------------------------------------------------------------
void V1_Tir_Normal_Arriere_P1()
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[TIR1P1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 2.
//------------------------------------------------------------------------------
void V1_Tir_Normal_Arriere_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[TIR1P1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 9;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du second tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[TIR1P1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 7;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 3.
//------------------------------------------------------------------------------
void V1_Tir_Normal_Arriere_P3(void)
{                                                              // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 1.
  V1_Tir_Normal_Arriere_P1();
  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 2.
  V1_Tir_Normal_Arriere_P2();
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 4.
//------------------------------------------------------------------------------
void V1_Tir_Normal_Arriere_P4(void)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 2.
  V1_Tir_Normal_Arriere_P2();

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[TIR1P1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 10;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du second tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[TIR1P1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 6;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 5.
//------------------------------------------------------------------------------
void V1_Tir_Normal_Arriere_P5(void)
{
  V1_Tir_Normal_Arriere_P1();                                  // Routine utilisee pour ajouter au vaisseau 1 le tir normal à l'arriere de puissance 1.
  V1_Tir_Normal_Arriere_P4();                                  // Routine utilisee pour ajouter au vaisseau 1 le tir normal à l'arriere de puissance 4.
}


//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 1.
//------------------------------------------------------------------------------
void V1_Tir_Normal_Droite_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TDN1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 2.
//------------------------------------------------------------------------------
void V1_Tir_Normal_Droite_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TDN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 31;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TDN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 1;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 3.
void V1_Tir_Normal_Droite_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TDN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 2.
  V1_Tir_Normal_Droite_P2();
}

// Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 4.
void V1_Tir_Normal_Droite_P4(void)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 2.
  V1_Tir_Normal_Droite_P2();

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TDN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 30;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TDN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 5.
void V1_Tir_Normal_Droite_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TDN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 4.
  V1_Tir_Normal_Droite_P4();
}

// Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 1.
void V1_Tir_Normal_Gauche_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TGN1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 2.
void V1_Tir_Normal_Gauche_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TGN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 17;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TGN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 15;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 3.
void V1_Tir_Normal_Gauche_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TGN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 2.
  V1_Tir_Normal_Gauche_P2();
}

// Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 4.
void V1_Tir_Normal_Gauche_P4(void)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 2.
  V1_Tir_Normal_Gauche_P2();

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TGN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 18;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TGN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 14;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 5.
void V1_Tir_Normal_Gauche_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V1TGN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 4.
  V1_Tir_Normal_Gauche_P4();
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 1 le tir special à l'avant de puissance 1.
//------------------------------------------------------------------------------
void V1_Tir_Special_Avant_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1V1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à l'avant de puissance 2.
void V1_Tir_Special_Avant_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 4;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1V2J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à l'avant de puissance 3.
void V1_Tir_Special_Avant_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 6;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1V3J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à l'avant de puissance 4.
void V1_Tir_Special_Avant_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1V4J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à l'avant de puissance 5.
void V1_Tir_Special_Avant_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1V5J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}



// Routine utilisée pour ajouter au vaisseau 1 le tir special à l'arrière de puissance 1.
void V1_Tir_Special_Arriere_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1V1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à l'arrière de puissance 2.
void V1_Tir_Special_Arriere_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 4;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1V2J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à l'arrière de puissance 3.
void V1_Tir_Special_Arriere_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 6;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1V3J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à l'arrière de puissance 4.
void V1_Tir_Special_Arriere_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1V4J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à l'arrière de puissance 5.
void V1_Tir_Special_Arriere_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1V5J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}


// Routine utilisée pour ajouter au vaisseau 1 le tir special à droite de puissance 1.
void V1_Tir_Special_Droite_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1H1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à droite de puissance 2.
void V1_Tir_Special_Droite_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 4;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1H2J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à droite de puissance 3.
void V1_Tir_Special_Droite_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 6;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1H3J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à droite de puissance 4.
void V1_Tir_Special_Droite_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1H4J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à droite de puissance 5.
void V1_Tir_Special_Droite_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1H5J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à gauche de puissance 1.
void V1_Tir_Special_Gauche_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1H1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à gauche de puissance 2.
void V1_Tir_Special_Gauche_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 4;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1H2J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à gauche de puissance 3.
void V1_Tir_Special_Gauche_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 6;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1H3J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à gauche de puissance 4.
void V1_Tir_Special_Gauche_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1H4J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 1 le tir special à gauche de puissance 5.
void V1_Tir_Special_Gauche_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP1H5J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 1.
//------------------------------------------------------------------------------
void V2_Tir_Normal_Avant_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2THN1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 2.
//------------------------------------------------------------------------------
void V2_Tir_Normal_Avant_P2(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 105;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2THN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 106;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2THN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 3.
//------------------------------------------------------------------------------
void V2_Tir_Normal_Avant_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2THN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 2.
  V2_Tir_Normal_Avant_P2(1);
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 4.
//------------------------------------------------------------------------------
void V2_Tir_Normal_Avant_P4(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 2.
  V2_Tir_Normal_Avant_P2(pow);                                 // Demande une puissance de tir de un.

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 108;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2THN1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[1][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[1][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du second tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 107;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2THN1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[2][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[2][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 5.
void V2_Tir_Normal_Avant_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2THN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 4.
  V2_Tir_Normal_Avant_P4(1);
}


//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 1.
//------------------------------------------------------------------------------
void V2_Tir_Normal_Arriere_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[T2NP1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 2.
//------------------------------------------------------------------------------
void V2_Tir_Normal_Arriere_P2(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 109;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[T2NP1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 110;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[T2NP1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 3.
//------------------------------------------------------------------------------
void V2_Tir_Normal_Arriere_P3(void)
{                                                              // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 1.
  V2_Tir_Normal_Arriere_P1();
  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 2.
  V2_Tir_Normal_Arriere_P2(1);
}

// Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 4.
void V2_Tir_Normal_Arriere_P4(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 2.
  V2_Tir_Normal_Arriere_P2(pow);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 111;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[T2NP1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[5][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[5][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 112;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[T2NP1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[4][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[4][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 5.
//------------------------------------------------------------------------------
void V2_Tir_Normal_Arriere_P5(void)
{                                                              // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 1.
  V2_Tir_Normal_Arriere_P1();
  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 4.
  V2_Tir_Normal_Arriere_P4(1);
}


//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 1.
//------------------------------------------------------------------------------
void V2_Tir_Normal_Droite_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 2.
//------------------------------------------------------------------------------
void V2_Tir_Normal_Droite_P2(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 113;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 114;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 3.
void V2_Tir_Normal_Droite_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 2.
  V2_Tir_Normal_Droite_P2(1);
}

// Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 4.
void V2_Tir_Normal_Droite_P4(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 2.
  V2_Tir_Normal_Droite_P2(pow);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 115;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[7][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[7][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 116;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[8][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[8][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 5.
void V2_Tir_Normal_Droite_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 4.
  V2_Tir_Normal_Droite_P4(1);
}

// Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 1.
void V2_Tir_Normal_Gauche_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 2.
void V2_Tir_Normal_Gauche_P2(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 117;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 118;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 3.
void V2_Tir_Normal_Gauche_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 2.
  V2_Tir_Normal_Gauche_P2(1);
}

// Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 4.
void V2_Tir_Normal_Gauche_P4(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 2.
  V2_Tir_Normal_Gauche_P2(pow);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 119;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[10][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[10][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 120;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[11][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[11][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 5.
void V2_Tir_Normal_Gauche_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 4.
  V2_Tir_Normal_Gauche_P4(1);
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir special à l'avant de puissance 1.
//------------------------------------------------------------------------------
void V2_Tir_Special_Avant_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 4;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx0][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir special à l'avant de puissance 2.
//------------------------------------------------------------------------------
void V2_Tir_Special_Avant_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 6;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à l'avant de puissance 3.
void V2_Tir_Special_Avant_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir special à l'avant de puissance 4.
//------------------------------------------------------------------------------
void V2_Tir_Special_Avant_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 2 le tir special à l'avant de puissance 5.
//------------------------------------------------------------------------------
void V2_Tir_Special_Avant_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP2MIJ][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}



// Routine utilisée pour ajouter au vaisseau 2 le tir special à l'arrière de puissance 1.
void V2_Tir_Special_Arriere_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 4;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx0][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à l'arrière de puissance 2.
void V2_Tir_Special_Arriere_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 6;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à l'arrière de puissance 3.
void V2_Tir_Special_Arriere_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à l'arrière de puissance 4.
void V2_Tir_Special_Arriere_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à l'arrière de puissance 5.
void V2_Tir_Special_Arriere_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP2MIJ][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}


// Routine utilisée pour ajouter au vaisseau 2 le tir special à droite de puissance 1.
void V2_Tir_Special_Droite_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 4;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx0][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à droite de puissance 2.
void V2_Tir_Special_Droite_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 6;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à droite de puissance 3.
void V2_Tir_Special_Droite_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à droite de puissance 4.
void V2_Tir_Special_Droite_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à droite de puissance 5.
void V2_Tir_Special_Droite_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP2MIJ][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à gauche de puissance 1.
void V2_Tir_Special_Gauche_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 4;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx0][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à gauche de puissance 2.
void V2_Tir_Special_Gauche_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 6;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à gauche de puissance 3.
void V2_Tir_Special_Gauche_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à gauche de puissance 4.
void V2_Tir_Special_Gauche_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 2 le tir special à gauche de puissance 5.
void V2_Tir_Special_Gauche_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP2MIJ][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 1.
//------------------------------------------------------------------------------
void V3_Tir_Normal_Avant_P1(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3THN1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 2.
//------------------------------------------------------------------------------
void V3_Tir_Normal_Avant_P2(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 59;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3THN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 61;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3THN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 3.
//------------------------------------------------------------------------------
void V3_Tir_Normal_Avant_P3(void)
{                                                              // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 1.
  V3_Tir_Normal_Avant_P1(3);
  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 1.
  V3_Tir_Normal_Avant_P2(2);
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 4.
void V3_Tir_Normal_Avant_P4(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 2.
  V3_Tir_Normal_Avant_P2(pow);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 60;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3THN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[2][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[2][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 62;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3THN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[1][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[1][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 5.
void V3_Tir_Normal_Avant_P5(void)
{                                                              // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 1.
  V3_Tir_Normal_Avant_P1(3);
  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 1.
  V3_Tir_Normal_Avant_P4(1);
}



// Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 1.
void V3_Tir_Normal_Arriere_P1(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TBB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 2.
void V3_Tir_Normal_Arriere_P2(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 85;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TBA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 86;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TBA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 3.
void V3_Tir_Normal_Arriere_P3(void)
{                                                              // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 1.
  V3_Tir_Normal_Arriere_P1(3);
  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 2.
  V3_Tir_Normal_Arriere_P2(2);
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 4.
void V3_Tir_Normal_Arriere_P4(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 2.
  V3_Tir_Normal_Arriere_P2(pow);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 87;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TBB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[5][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[5][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 88;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TBB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[4][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[4][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 5.
void V3_Tir_Normal_Arriere_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TBC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 4.
  V3_Tir_Normal_Arriere_P4(1);
}


// Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 1.
void V3_Tir_Normal_Droite_P1(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TDB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 2.
void V3_Tir_Normal_Droite_P2(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 77;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TDA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 78;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TDA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 3.
void V3_Tir_Normal_Droite_P3(void)
{                                                              // Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 1.
  V3_Tir_Normal_Droite_P1(3);
  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 2.
  V3_Tir_Normal_Droite_P2(2);
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 4.
void V3_Tir_Normal_Droite_P4(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 2.
  V3_Tir_Normal_Droite_P2(pow);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 79;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TDB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[7][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[7][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 80;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TDB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[8][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[8][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 5.
void V3_Tir_Normal_Droite_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TDC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 4.
  V3_Tir_Normal_Droite_P4(1);
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 1.
void V3_Tir_Normal_Gauche_P1(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TGB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 2.
void V3_Tir_Normal_Gauche_P2(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 81;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TGA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 82;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TGA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 3.
void V3_Tir_Normal_Gauche_P3(void)
{                                                              // Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 1.
  V3_Tir_Normal_Gauche_P1(3);
  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 2.
  V3_Tir_Normal_Gauche_P2(2);
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 4.
void V3_Tir_Normal_Gauche_P4(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 2.
  V3_Tir_Normal_Gauche_P2(pow);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 83;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TGB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[10][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[10][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 84;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TGB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[11][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[11][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 5.
void V3_Tir_Normal_Gauche_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TGC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 4.
  V3_Tir_Normal_Gauche_P4(1);
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 3 le tir special à l'avant de puissance 1.
//------------------------------------------------------------------------------
void V3_Tir_Special_Avant_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 6;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à l'avant de puissance 2.
void V3_Tir_Special_Avant_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à l'avant de puissance 3.
void V3_Tir_Special_Avant_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à l'avant de puissance 4.
void V3_Tir_Special_Avant_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSD][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à l'avant de puissance 5.
void V3_Tir_Special_Avant_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 14;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSE][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}



// Routine utilisée pour ajouter au vaisseau 3 le tir special à l'arrière de puissance 1.
void V3_Tir_Special_Arriere_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 6;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à l'arrière de puissance 2.
void V3_Tir_Special_Arriere_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à l'arrière de puissance 3.
void V3_Tir_Special_Arriere_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à l'arrière de puissance 4.
void V3_Tir_Special_Arriere_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSD][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à l'arrière de puissance 5.
void V3_Tir_Special_Arriere_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 14;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSE][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}


// Routine utilisée pour ajouter au vaisseau 3 le tir special à droite de puissance 1.
void V3_Tir_Special_Droite_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 6;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à droite de puissance 2.
void V3_Tir_Special_Droite_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à droite de puissance 3.
void V3_Tir_Special_Droite_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à droite de puissance 4.
void V3_Tir_Special_Droite_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSD][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à droite de puissance 5.
void V3_Tir_Special_Droite_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 14;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSE][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à gauche de puissance 1.
void V3_Tir_Special_Gauche_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 6;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à gauche de puissance 2.
void V3_Tir_Special_Gauche_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à gauche de puissance 3.
void V3_Tir_Special_Gauche_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à gauche de puissance 4.
void V3_Tir_Special_Gauche_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSD][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 3 le tir special à gauche de puissance 5.
void V3_Tir_Special_Gauche_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 14;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V3TSE][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

//------------------------------------------------------------------------------
// Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 1.
//------------------------------------------------------------------------------
void V4_Tir_Normal_Avant_P1(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4THB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 2.
void V4_Tir_Normal_Avant_P2(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 89;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4THA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 90;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4THA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 3.
void V4_Tir_Normal_Avant_P3(void)
{                                                              // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 1.
  V4_Tir_Normal_Avant_P1(3);
  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 2.
  V4_Tir_Normal_Avant_P2(3);
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 4.
void V4_Tir_Normal_Avant_P4(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 2.
  V4_Tir_Normal_Avant_P2(pow);                                 // Demande une puissance de tir de un.

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 92;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4THB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[1][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[1][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du second tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 91;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4THB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[2][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[2][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 5.
void V4_Tir_Normal_Avant_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4THC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 4.
  V4_Tir_Normal_Avant_P4(2);
}



// Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 1.
void V4_Tir_Normal_Arriere_P1(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TBB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 2.
void V4_Tir_Normal_Arriere_P2(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 101;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TBA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[5][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[5][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 102;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TBA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[4][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[4][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 3.
void V4_Tir_Normal_Arriere_P3(void)
{                                                              // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 1.
  V4_Tir_Normal_Arriere_P1(3);
  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 2.
  V4_Tir_Normal_Arriere_P2(3);
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 4.
void V4_Tir_Normal_Arriere_P4(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 2.
  V4_Tir_Normal_Arriere_P2(pow);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 103;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TBB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[5][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[5][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 104;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TBB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[4][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[4][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 5.
void V4_Tir_Normal_Arriere_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TBC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 4.
  V4_Tir_Normal_Arriere_P4(2);
}


// Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 1.
void V4_Tir_Normal_Droite_P1(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TDB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 2.
void V4_Tir_Normal_Droite_P2(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 97;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TDA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 98;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TDA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 3.
void V4_Tir_Normal_Droite_P3(void)
{                                                              // Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 1.
  V4_Tir_Normal_Droite_P1(3);
  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 2.
  V4_Tir_Normal_Droite_P2(3);
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 4.
void V4_Tir_Normal_Droite_P4(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 2.
  V4_Tir_Normal_Droite_P2(pow);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 99;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TDB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[7][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[7][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 100;                            // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TDB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[8][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[8][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 5.
void V4_Tir_Normal_Droite_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TDC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 4.
  V4_Tir_Normal_Droite_P4(2);
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 1.
void V4_Tir_Normal_Gauche_P1(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TGB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 2.
void V4_Tir_Normal_Gauche_P2(Sint32 pow)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 93;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TGA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 94;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(pow);              // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TGA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 3.
void V4_Tir_Normal_Gauche_P3(void)
{                                                              // Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 1.
  V4_Tir_Normal_Gauche_P1(3);
  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 2.
  V4_Tir_Normal_Gauche_P2(3);
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 4.
void V4_Tir_Normal_Gauche_P4(Sint32 power)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 2.
  V4_Tir_Normal_Gauche_P2(power);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 95;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TGB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[10][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[10][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 96;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 1;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TGB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[11][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[11][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 5.
void V4_Tir_Normal_Gauche_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 3;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TGC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 8.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 4.
  V4_Tir_Normal_Gauche_P4(2);
}








// Routine utilisée pour ajouter au vaisseau 4 le tir special à l'avant de puissance 1.
void V4_Tir_Special_Avant_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à l'avant de puissance 2.
void V4_Tir_Special_Avant_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à l'avant de puissance 3.
void V4_Tir_Special_Avant_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à l'avant de puissance 4.
void V4_Tir_Special_Avant_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 14;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSD][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à l'avant de puissance 5.
void V4_Tir_Special_Avant_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 16;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSE][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}



// Routine utilisée pour ajouter au vaisseau 4 le tir special à l'arrière de puissance 1.
void V4_Tir_Special_Arriere_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à l'arrière de puissance 2.
void V4_Tir_Special_Arriere_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à l'arrière de puissance 3.
void V4_Tir_Special_Arriere_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à l'arrière de puissance 4.
void V4_Tir_Special_Arriere_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 14;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSD][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à l'arrière de puissance 5.
void V4_Tir_Special_Arriere_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 16;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSE][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}


// Routine utilisée pour ajouter au vaisseau 4 le tir special à droite de puissance 1.
void V4_Tir_Special_Droite_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à droite de puissance 2.
void V4_Tir_Special_Droite_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à droite de puissance 3.
void V4_Tir_Special_Droite_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à droite de puissance 4.
void V4_Tir_Special_Droite_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 14;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSD][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à droite de puissance 5.
void V4_Tir_Special_Droite_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 16;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSE][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à gauche de puissance 1.
void V4_Tir_Special_Gauche_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 8;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSA][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à gauche de puissance 2.
void V4_Tir_Special_Gauche_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSB][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à gauche de puissance 3.
void V4_Tir_Special_Gauche_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSC][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à gauche de puissance 4.
void V4_Tir_Special_Gauche_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 14;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSD][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 4 le tir special à gauche de puissance 5.
void V4_Tir_Special_Gauche_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 16;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V4TSE][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}








// Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 1.
void V5_Tir_Normal_Avant_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 9;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TN1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 2.
void V5_Tir_Normal_Avant_P2(Sint32 power)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 55;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(power);            // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 57;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(power);            // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 3.
void V5_Tir_Normal_Avant_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 5;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 2.
  V5_Tir_Normal_Avant_P2(3);
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 4.
void V5_Tir_Normal_Avant_P4(Sint32 power)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 2.
  V5_Tir_Normal_Avant_P2(power);                               // Demande une puissance de tir de un.

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 58;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[1][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[1][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du second tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 56;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[2][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[2][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 5.
void V5_Tir_Normal_Avant_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 5;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 4.
  V5_Tir_Normal_Avant_P4(2);
}



// Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 1.
void V5_Tir_Normal_Arriere_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 9;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[T2NP1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 2.
void V5_Tir_Normal_Arriere_P2(Sint32 power)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 73;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(power);            // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[T2NP1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[5][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[5][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 74;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(power);            // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[T2NP1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[4][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[4][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 3.
void V5_Tir_Normal_Arriere_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 5;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[T2NP1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }
  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 2.
  V5_Tir_Normal_Arriere_P2(3);
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 4.
void V5_Tir_Normal_Arriere_P4(Sint32 power)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 2.
  V5_Tir_Normal_Arriere_P2(power);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 75;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[T2NP1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[5][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[5][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 76;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[T2NP1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[4][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[4][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 5.
void V5_Tir_Normal_Arriere_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 0;                         // Indique que la trajectoire du sprite est définie une fois pour toute.
    tirs[new_tir].spr.pow_of_dest = 5;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[T2NP1J][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].spr.vitesse = 9.0;                           // Donne la vitesse de déplacement du sprite.
  }

  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 4.
  V5_Tir_Normal_Arriere_P4(2);
}


// Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 1.
void V5_Tir_Normal_Droite_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 72;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 9;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 2.
void V5_Tir_Normal_Droite_P2(Sint32 power)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 71;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(power);            // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 70;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(power);            // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 3.
void V5_Tir_Normal_Droite_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 72;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 5;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 2.
  V5_Tir_Normal_Droite_P2(3);
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 4.
void V5_Tir_Normal_Droite_P4(Sint32 power)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 2.
  V5_Tir_Normal_Droite_P2(power);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 69;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 68;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 5.
void V5_Tir_Normal_Droite_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 72;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 5;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TDN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 4.
  V5_Tir_Normal_Droite_P4(2);
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 1.
void V5_Tir_Normal_Gauche_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 67;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 9;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 2.
void V5_Tir_Normal_Gauche_P2(Sint32 power)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 66;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(power);            // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 65;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = (short)(power);            // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 3.
void V5_Tir_Normal_Gauche_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 67;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 5;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 2.
  V5_Tir_Normal_Gauche_P2(3);
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 4.
void V5_Tir_Normal_Gauche_P4(Sint32 power)
{
  Sint32                     new_tir, i;

  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 2.
  V5_Tir_Normal_Gauche_P2(power);

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 64;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 63;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 2;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 5.
void V5_Tir_Normal_Gauche_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].clignote = 1;                                // Indique que l'on est en présence d'un sprite qui doit clignoter.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 2;                         // Indique que la trajectoire du sprite est définie par une courbe.
    tirs[new_tir].num_courbe = 67;                             // Indique le numéro de la courbe suivie par le sprite.
    tirs[new_tir].pos_vaiss[POS_CURVE] = 0;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
    tirs[new_tir].spr.pow_of_dest = 5;                         // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 4;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[V2TGN3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
  }
  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 4.
  V5_Tir_Normal_Gauche_P4(2);
}








// Routine utilisée pour ajouter au vaisseau 5 le tir special à l'avant de puissance 1.
void V5_Tir_Special_Avant_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx0][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à l'avant de puissance 2.
void V5_Tir_Special_Avant_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP2MIJ][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à l'avant de puissance 3.
void V5_Tir_Special_Avant_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 14;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à l'avant de puissance 4.
void V5_Tir_Special_Avant_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 16;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à l'avant de puissance 5.
void V5_Tir_Special_Avant_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du premier tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 18;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 24;
    tirs[new_tir].angle_tir = PI_PLUS_PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[0][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}



// Routine utilisée pour ajouter au vaisseau 5 le tir special à l'arrière de puissance 1.
void V5_Tir_Special_Arriere_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx0][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à l'arrière de puissance 2.
void V5_Tir_Special_Arriere_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP2MIJ][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à l'arrière de puissance 3.
void V5_Tir_Special_Arriere_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 14;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à l'arrière de puissance 4.
void V5_Tir_Special_Arriere_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 16;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à l'arrière de puissance 5.
void V5_Tir_Special_Arriere_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 18;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 8;
    tirs[new_tir].angle_tir = PI_SUR_2;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[3][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}


// Routine utilisée pour ajouter au vaisseau 5 le tir special à droite de puissance 1.
void V5_Tir_Special_Droite_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx0][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à droite de puissance 2.
void V5_Tir_Special_Droite_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP2MIJ][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à droite de puissance 3.
void V5_Tir_Special_Droite_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 14;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à droite de puissance 4.
void V5_Tir_Special_Droite_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 16;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à droite de puissance 5.
void V5_Tir_Special_Droite_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 18;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 0;
    tirs[new_tir].angle_tir = 0.0;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[6][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à gauche de puissance 1.
void V5_Tir_Special_Gauche_P1(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 10;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx0][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à gauche de puissance 2.
void V5_Tir_Special_Gauche_P2(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 12;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[SP2MIJ][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à gauche de puissance 3.
void V5_Tir_Special_Gauche_P3(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 14;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx1][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à gauche de puissance 4.
void V5_Tir_Special_Gauche_P4(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 16;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx2][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}

// Routine utilisée pour ajouter au vaisseau 5 le tir special à gauche de puissance 5.
void V5_Tir_Special_Gauche_P5(void)
{
  Sint32                     new_tir, i;

  // Test s'il est possible d'ajouter un nouveau tir à la liste.
  if(nbr_tirs < (NBR_TIRS_MAX - 1))
  {                                                            // Recherche d'un nouveau tir.
    new_tir = New_Element_Tir();
    // Remplissage de la structure du tir.
    tirs[new_tir].spr.genre = AMI;                             // Indique que c'est un sprite ami.
    tirs[new_tir].spr.trajectoire = 1;                         // Indique que la trajectoire du sprite est recalculée à chaque VLBs.
    tirs[new_tir].spr.pow_of_dest = 18;                        // Indique la puissance de destruction du sprite.
    tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
    //tirs[new_tir].spr.pilote = 0;                              // Indique par qui est dirigé le sprite.
    tirs[new_tir].spr.nbr_img = 32;                            // Indique le nombre d'images composant le sprite.
    tirs[new_tir].spr.nbr_tot_vbl = 0;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
    tirs[new_tir].spr.nbr_vbl_act = 0;                         // Indique le nombre de VBL comptées actuellement.
    // Donne l'adresse des images du sprite.
    for(i = 0; i < tirs[new_tir].spr.nbr_img; i++)
    {                                                          // Chargement de l'image du sprite.
      tirs[new_tir].spr.img[i] = (image *) & fire[MISSx3][i];
    }
    // Donne les coordonnées d'affichage du sprite.
    tirs[new_tir].img_angle = 16;
    tirs[new_tir].angle_tir = PI;
    tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
    tirs[new_tir].spr.coor_x =
      vj[J1].spr.coor_x +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][X] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
    tirs[new_tir].spr.coor_y =
      vj[J1].spr.coor_y +
      vj[J1].spr.img[vj[J1].spr.img_act]->xy_pos_tir[9][Y] -
      tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
    tirs[new_tir].tmp_life_tir = 500;
    tirs[new_tir].agilite = 0.04f;
    tirs[new_tir].spr.vitesse = 4.0;                           // Donne la vitesse de déplacement du sprite.
  }
}





// Initialisation du tableau 've'.
void Init_Tab_Ve(void)
{
Sint32                     i;

  for(i = 0; i < MAX_NMI_ON_SCREEN; i++)
  {
    ve[i].next_object = MAX_NMI_ON_SCREEN;                     // Indique que l'objet actuel est libre.
    ve[i].prev_object = MAX_NMI_ON_SCREEN;                     // Indique que l'objet actuel est libre.
    ve[i].dead = 0;
    ve[i].visible_dead = 0;
    ve[i].cmpt_color_dead = 0;                                 // Donne la couleur de départ dans le tableau des couleurs du vaisseau qui disparait.
    ve[i].invincible = 0;
    ve[i].retournement = 0;                                    // Vaut 1 si le vaisseau ennemi est dans sa phase de retournement, sinon 0.
    ve[i].change_dir = 0;                                      // Vaut 1 si le vaisseau ennemi à changé de direction, sinon 0.
  }
  nbr_nmis = 0;
}

// Initialisation du tableau 'tirs'.
void Init_Tab_Tirs(void)
{
  Sint32                     i;

  // Initialisation du tableau 'tirs'.
  for(i = 0; i < NBR_TIRS_MAX; i++)
  {
    tirs[i].next_object = NBR_TIRS_MAX;                        // Indique que l'objet actuel est libre.
    tirs[i].prev_object = NBR_TIRS_MAX;                        // Indique que l'objet actuel est libre.
  }
  nbr_tirs = 0;
}

// Initialisation du tableau 'bou'.
void Init_Tab_Boules(void)
{
  Sint32                     i;

  //Indique que le joueur ne peut pas utiliser l'eclair.
  eclair_actif = 0;
  // Initialisation du tableau 'bou'.
  for(i = 0; i < MAX_BOULES_ON_SCREEN; i++)
  {
    bou[i].next_object = MAX_BOULES_ON_SCREEN;                 // Indique que l'objet actuel est libre.
    bou[i].prev_object = MAX_BOULES_ON_SCREEN;                 // Indique que l'objet actuel est libre.
  }
  nbr_boules = 0;                                              // Initialise le nombre de boules autour du vaisseau.
}

// Initialisation du tableau 'can'.
void Init_Tab_Canons(void)
{
  Sint32                     i;

  // Initialisation du tableau 'can'.
  for(i = 0; i < MAX_CANONS_ON_SCREEN; i++)
  {
    can[i].next_object = MAX_CANONS_ON_SCREEN;                 // Indique que l'objet actuel est libre.
    can[i].prev_object = MAX_CANONS_ON_SCREEN;                 // Indique que l'objet actuel est libre.
  }
  nbr_canons = 0;                                              // Initialise le nombre de canons autour du vaisseau.
}


//------------------------------------------------------------------------------
// passer au niveau suivant sans gardiens
//------------------------------------------------------------------------------
Sint32 Meteor_Activity_Less_Guardians()
{
  disparition_vj = 1;                                          //1=disparition du vaisseau du joueur
  move_level = 1;                                              //1=texte indiquant le niveau en deplacement
  if(vit_star_field == 0.0 && pos_x_level == 0.0 && pos_y_level == 0.0)
  {
    num_level++;                                               //passage au niveau suivant.
    if(num_level > NBR_LEVEL_MAX)
      num_level = 0;
    if(!LoadGrid(num_level))                                   //chargement de la phase "grille"
      return 0;
    if(!Load_Curve_Level(num_level))                           //chargement de la phase "courbe"
      return 0;
    meteor_libere();                                           //libere memoire utilisee par les images des meteorites
    if(!Load_Meteor(num_level))                                //chargement des meteorites
      return 0;
    Active_Curve_Level();                                      //activation du fichier courbe charge precedement
    courbe.activity = 1;                                       //1=phase "courbe" activee
    grille.activity = 0;
    meteor_activity = 0;
    Apparition_Vaisseau();                                     //reapparition du vaisseau du joueur.
  }
  return 1;
}

// -----------------------------------------------------------------------------
// Creation d'un fichier Flag (test)
// -----------------------------------------------------------------------------
void fichierFlag()
{
/*MAKE LINUX PORT
  char* _pBuffer="ETB1" ;
  Sint32 _iHandle ;
  if ((_iHandle = _lcreat("etbdebug.txt", 0))!=-1)
  { _lwrite(_iHandle,_pBuffer,5);
    _lclose(_iHandle);
  }
*/
}

// -----------------------------------------------------------------------------
// Creation d'un fichier Flag (test)
// -----------------------------------------------------------------------------
void fichierFlag2(char *_pBuffer)
{
/*MAKE LINUX POR
  Sint32 _iHandle ;
  Uint32 _iTaille=strlen(_pBuffer) ;
  if ((_iHandle = _lcreat("etbdebug2.txt", 0))!=-1)
  { _lwrite(_iHandle,_pBuffer,_iTaille);
    _lclose(_iHandle);
  }
*/
}

// -----------------------------------------------------------------------------
// Convertit un nombre en chaine ASCII 
// input => _iNombre: nombre a convertit
//       => _pASCII : pointeur sunr une chaine ASCII (terminee par un zero)
// -----------------------------------------------------------------------------
void entierEnASCII(Uint32 _iNombre, char *_pASCII)
{
  Uint32            _iIndex = 0;
  Uint32            _iTaille = 1;
  Uint32            _iReste = strlen(_pASCII) - 1;
  for(_iIndex = 0; _iIndex < _iReste; _iIndex++)
  { _iTaille = _iTaille * 10;
  }
  _iIndex = 0;
  while (_iTaille > 0)
  { Uint32            _iReste = _iNombre / _iTaille;
    char                    _cASCII = (char)('0' + _iReste);
    if(_cASCII > '9' || _cASCII < '0') _cASCII = '?';
    _pASCII[_iIndex++] = _cASCII;
    _iNombre = _iNombre - (_iTaille * _iReste);
    _iTaille = _iTaille / 10;
  }
}

// -----------------------------------------------------------------------------
// Convertit un nombre en chaine ASCII 
// input => _iNombre: nombre a convertir / number to convert
//       => _pASCII : pointeur sunr une chaine ASCII (terminee par un zero)
// -----------------------------------------------------------------------------
void entier2ASCII(Sint32 _iNombre, char *_pASCII, Uint32 _iReste)
{
  Uint32            _iIndex = 0;
  Uint32            _iTaille = 1;
  if(_iNombre < 0)
  { _iNombre = -_iNombre;
    *(_pASCII++) = '-';
    if(_iReste > 0)
      _iReste--;
  }
  for(_iIndex = 0; _iIndex < _iReste; _iIndex++)
  { _iTaille = _iTaille * 10;
  }
  _iIndex = 0;
  while (_iTaille > 0)
  { Uint32            _iReste = _iNombre / _iTaille;
    char                    _cASCII = (char)('0' + _iReste);
    if(_cASCII > '9' || _cASCII < '0') _cASCII = '?';
    _pASCII[_iIndex++] = _cASCII;
    _iNombre = _iNombre - (_iTaille * _iReste);
    _iTaille = _iTaille / 10;
  }
}


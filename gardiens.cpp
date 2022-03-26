//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "gardiens.cpp"
// created	: 1998-04-21
// updates	: 2005-01-08
// id		: $Id: gardiens.cpp,v 1.1.1.1 2005/01/25 05:53:22 gurumeditation Exp $
//-----------------------------------------------------------------------------
// functions   : Meteor_Activity_With_Guardian(int num_gardien)
//               Gere_gardien(int num_gardien)
//               Load_Gardien(int numero_gardien)
//               Test_Col_Player_With_Spec_Nmi(int nmi_numero)
//               Test_Col_Bou_With_Nmi(int nmi_numero)
//               Free_Mem_Gardian()
//               Fill_Tab_Dep_Gard_Gauche()
//               Fill_Tab_Dep_Gard_Droite()
//               Fill_Tab_Dep_Gard_Haut()
//               Fill_Tab_Dep_Gard_Bas()
//               Fait_Apparaitre_Gardien_Haut(int num_gardien)
//               Move_Gardian_Line(int num_gardien)
//               Move_Gardian_Sin(int num_gardien)
//               Clignote_Nmi_Avant_Disparition(int numero_nmi)
//               gardien_testFin() 
//               gardienBarreNRJ()
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
//constantes de deplacement du gardien nuemro dix ----------------------------
#define HAUT						0
#define BAS							1
#define DRTE						2
#define GOCH						3
//..............................................................................
#define START_NAGGYS		16                                     //Utilise pour compter le nombre de tirs separant le lancement d'un NAGGYS dans la gestion du Gardien N°3.
#define START_SAPOUCH		8                                      //Utilise pour compter le nombre de tirs separant le lancement d'un SAPOUCH dans la gestion du Gardien N°4.
#define START_DEVILIANS	140                                    //Utilise pour compter le nombre de tirs separant le lancement d'un DEVILIANS dans la gestion du Gardien N°4.
#define MAX_DEVILIANS		3                                      //Utilise pour compter le nombre maximum de DEVILIANS lances par le Gardien N°4.
//..............................................................................
int                     send_dep_gard10;
int                     clip_gard10 = 16;
int                     cmpt_DEVILIANS;                        //compteur du nombre de "DEVILIANS" lances par le gardien4
int                     cmpt_tmps_DEVILIANS;                   //interval temps separant le lancement d'un "DEVILIANS" par le gardien4
short                   cmpt_vbl_trans;                        // Utiliser pour compter les vbl avant chaque changement d'images.
short                   ptr_sur_tab_gardien;                   // Pointeur sur le tableau du gardien.
short                   cmpt_tmps_dep_gardien;                 // Compteur utilisé pour décompter le temps entre chaque déplacments d'un gardien.
short                   nbr_dep_gardian_max;                   // Indique le nombre maximum de déplacments pour chaque gardien.
unsigned char           tab_traj_gardien[NBR_MAX_DEP_GARDIEN]; // Tableau des trajectoires que va suivre le gardien.
short                   tab_tmps_traj_gardien[NBR_MAX_DEP_GARDIEN];     // Tableau des temps avant chaque déplacmements du gardien.
unsigned char           tab_vit_dep_gardien[NBR_MAX_DEP_GARDIEN];       // Tableau des vitesses de déplacement du gardien.
bool                    change_direction;                      //1=vaisseau vient de changer de direction
bool                    vertical_trajectoire;                  //1=trajectoire verticale
bool                    aff_gard_1_sur_2;                      //clignotement gardien lors de son apparition
int                     cmpt_tir_gard_spec;                    //lance un tir special des gardiens
int                     compteur_shuriy;                       //donner le depart d'un "SHURIKY"
int                     compteur_quibouly;                     //donne le depart d'un "QUIBOULY"
int                     compteur_tournadee;                    //donne le depart d'un "TOURNADEE"
//..............................................................................
extern int              etat_menu;                             //etat du menu (actif, non actif, apparition, disparition)
extern image            enemi[MAX_TYP_OF_ENEMI + MAX_TYP_OF_S_ENEMI + MAX_TYP_OF_NMI_SPE + MAX_TYP_OF_OBJ_SPE][NBR_IMAGES_MAX]; //tableau des differents ennemis
extern image            fire[MAX_TYP_OF_TIR][NBR_IMAGES_TIR];  //tableau des differents tirs
extern short            nbr_tirs;                              //nombre de tirs actuellement geres
extern bool             player_pause;                          //1=pause dans le jeu
extern tir              tirs[NBR_TIRS_MAX];                    //tableau de tous les objets tirs
extern int              new_tir;                               //indice tableau corespondant sru un nouvel objet 'tirs'
extern bool             game_over_player_one;                  //1=le joueur a perdu son vaisseau
extern bool             bar_nrj_player;
extern bool             bar_nrj_guardian;
extern int              player_score;                          //score du joueur.
extern int              bonusx;                                //multiplier de points (x2 ou x4) 
extern unsigned char    coulor[12];                            //couleurs choisies par l'utilisateur
extern int              new_ve;                                //indice tableau sur un nouvel objet 've'.
extern image            gardi[NBR_ANIM_GARDIAN][NBR_IMAGES_NMI_S];      //tableau des images des differents gardiens
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //structure des vaisseaux ennemis.
extern unsigned char    color_dead[NBR_COLOR_DEAD + 1];        //tableau des couleurs utilisees pour la disparition d'un vaisseau
extern char            *ecran_ram;                             //adresse ecran logique
extern char            *ecran;                                 //adresse ecran physique
extern int              gardian_activity;                      //1=phase "gardien"
extern curve_level      courbe;                                //structure phase "courbe"
extern grid             grille;                                //structure phase "grille"
extern bool             meteor_activity;                       //1=phase "meteorites"
extern bool             apparition_gardian;                    //1=apparition d'un gardien
extern vaisseau         vj[NBR_MAX_J];                         //structure du vaisseau du joueur
extern bool             aff_game_over;                         //1=affichage du "game over"
extern short            nbr_boules;                            //nombre de boules actuellement gerees
extern int              liste_bou[MAX_BOULES_ON_SCREEN];       //liste de toutes les 'bou' gerees 
extern str_boule        bou[MAX_BOULES_ON_SCREEN];             //structure des boules
extern int              cmpt_boules;                           //compteur du nombre de boules
extern short            nbr_canons;                            //nombre de canons actuellement geres
extern int              liste_can[MAX_CANONS_ON_SCREEN];       //liste de toutes les 'can' geres.
extern str_canon        can[MAX_CANONS_ON_SCREEN];             //structure des canons
extern int              cmpt_canons;                           //compteur du nombre de canons
extern int              new_can;                               //pointeur sur un nouvel element 'can'
extern short            nbr_exps;                              //nombre d'explosions actuellement gerees
extern float            depix[13][32];                         //tableau pour le deplacement en x des tirs a trajectoire fixe
extern float            depiy[13][32];                         //tableau pour le deplacement en y des tirs a trajectoire fixe
extern short            first_value;
extern short            second_value;
extern short            compteur;
extern short            compteur1;
extern float            pre_sin[];
extern image            bar_nrj_blue;                          //utilise pour affichage barre d'energie
extern short            nbr_nmis;                              //nombre d'enemis actuellement geres
extern bool             disparition_vj;                        //1=disparition du vaisseau du joueur
extern bool             move_level;                            //1=deplacement du texte du numero du niveau
extern float            vit_star_field;                        //vitesse du champs d'etoiles.
extern float            pos_x_level;                           //abscisse affichage du texte du numero niveau
extern float            pos_y_level;                           //ordonnee affichage du texte du numero niveau
extern int              num_level;                             //numero du niveau
extern image            meteo[MAX_TYP_OF_METEOR][NBR_IMAGES_METEOR];    //tableau des differentes meteorites
extern image            bar_nrj_red;                           //affichage de la barre d'energie rouge
extern image            bar_nrj_green;                         //affichage de la barre d'energie verte
extern image            bar_nrj_blue;                          //affichage de la barre d'énergie bleue
extern int              iNumeroGardien;                        //(ETB) numero du gardien en cours 
extern int              liste_ve[MAX_NMI_ON_SCREEN];           //liste de tous les vaisseaux ennemis a gerer
extern int              cmpt_nmis;                             //compteur nombre d'ennemis
extern _etbfonte1*	pEtbfonte1;                            //(ETB)objet cpp geran affichage caracteres 8x8
extern configfile*	power_conf;
char                    pppTexte[] = { "IMAGE:000 X:000 Y:000\0" };

//..............................................................................
void                    _init_congra();
void                    _execute_congra(int);

//------------------------------------------------------------------------------
// guardian : initiliaze
// fin du champ de meteorites, on passe au gardien
// input => num_gardien : numero du gardien de 1 a 14 (15=congratulation)
//------------------------------------------------------------------------------
void Meteor_Activity_With_Guardian(int num_gardien)
{
  int                     k;

  gardian_activity = num_gardien;
  apparition_gardian = TRUE;                                   //1=demande apparition d'un gardien
  courbe.activity = FALSE;
  grille.activity = FALSE;
  meteor_activity = FALSE;
  bar_nrj_guardian = TRUE;                                     //1=mise a jour de la barre d'energie du gardien
  switch (num_gardien)
    //guardian 1 initialize ........................................................
  {
    case 1:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      iNumeroGardien = new_ve;
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + THANIKEE);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 30); // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve l'etat des dommages a la creation du gardien.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 15;                             // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
      }
      ve[new_ve].cad_tir_initiale = 85;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(LARG_ECR_RAM - ve[new_ve].spr.img[15]->larg / 2);
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - ve[new_ve].spr.img[15]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = THANIKEE;                              // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.
      // Initialisation du pointeur sur le tableau qui permet les déplacements du gardien.
      ptr_sur_tab_gardien = 0;
      // Initialisation du compteur de déplacements du gardien.
      nbr_dep_gardian_max = 0;                                 // Indique le nombre de déplacements effectués par le gardien.
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la droite.
      Fill_Tab_Dep_Gard_Droite();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
    }
      break;

      //guardian 2 initialize ........................................................
    case 2:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      iNumeroGardien = new_ve;
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + BARYBOOG);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 50); // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve l'etat des dommages a la creation du gardien.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 15;                             // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
      }
      ve[new_ve].cad_tir_initiale = 50;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(LARG_ECR_RAM - ve[new_ve].spr.img[15]->larg / 2);
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - ve[new_ve].spr.img[15]->haut);
      ve[new_ve].spr.vitesse = 1.0;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = BARYBOOG;                              // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.

      // Initialisation du pointeur sur le tableau qui permet les déplacements du gardien.
      ptr_sur_tab_gardien = 0;
      // Initialisation du compteur de déplacements du gardien.
      nbr_dep_gardian_max = 0;                                 // Indique le nombre de déplacements effectués par le gardien.
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la droite.
      Fill_Tab_Dep_Gard_Droite();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers le bas.
      Fill_Tab_Dep_Gard_Bas();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers le haut.
      Fill_Tab_Dep_Gard_Haut();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la droite.
      Fill_Tab_Dep_Gard_Droite();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers le bas.
      Fill_Tab_Dep_Gard_Bas();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers le haut.
      Fill_Tab_Dep_Gard_Haut();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la droite.
      Fill_Tab_Dep_Gard_Droite();
    }
      break;

      //guardian 3 initialize ........................................................
    case 3:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      iNumeroGardien = new_ve;
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + PIKKIOU);      // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 70); // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve l'etat des dommages a la creation du gardien.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 15;                             // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
      }
      ve[new_ve].cad_tir_initiale = 75;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(LARG_ECR_RAM - ve[new_ve].spr.img[15]->larg / 2);
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - ve[new_ve].spr.img[15]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = PIKKIOU;                               // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.

      // Initialisation du pointeur sur le tableau qui permet les déplacements du gardien.
      ptr_sur_tab_gardien = 0;
      // Initialisation du compteur de déplacements du gardien.
      nbr_dep_gardian_max = 0;                                 // Indique le nombre de déplacements effectués par le gardien.
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la droite.
      Fill_Tab_Dep_Gard_Droite();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
      //Demande de lancer un vaisseau NAGGYS.
      cmpt_tir_gard_spec = START_NAGGYS;
    }
      break;

      //guardian 4 initialize ........................................................
    case 4:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      iNumeroGardien = new_ve;
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + NEGDEIS);      // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 90); // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve l'etat des dommages a la creation du gardien.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 15;                             // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
      }
      ve[new_ve].cad_tir_initiale = 75;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(LARG_ECR_RAM - ve[new_ve].spr.img[15]->larg / 2);
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - ve[new_ve].spr.img[15]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = NEGDEIS;                               // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.

      // Initialisation du pointeur sur le tableau qui permet les déplacements du gardien.
      ptr_sur_tab_gardien = 0;
      // Initialisation du compteur de déplacements du gardien.
      nbr_dep_gardian_max = 0;                                 // Indique le nombre de déplacements effectués par le gardien.
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la droite.
      Fill_Tab_Dep_Gard_Droite();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers le bas.
      Fill_Tab_Dep_Gard_Bas();
      // Remplissage du tableau des trajectoires, du temps et la vitesse de déplacement du gardien.
      tab_traj_gardien[nbr_dep_gardian_max] = 24;              // Indique que l'on se déplace vers le haut.
      tab_tmps_traj_gardien[nbr_dep_gardian_max] = 400;        // Tableau des temps avant chaque déplacmements du gardien.
      tab_vit_dep_gardien[nbr_dep_gardian_max] = 3;            // Tableau des vitesses de déplacement du gardien.
      nbr_dep_gardian_max++;                                   // Indique le nombre de déplacements effectués par le gardien.
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la droite.
      Fill_Tab_Dep_Gard_Droite();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la droite.
      Fill_Tab_Dep_Gard_Droite();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers le bas.
      Fill_Tab_Dep_Gard_Bas();
      // Remplissage du tableau des trajectoires, du temps et la vitesse de déplacement du gardien.
      tab_traj_gardien[nbr_dep_gardian_max] = 24;              // Indique que l'on se déplace vers le haut.
      tab_tmps_traj_gardien[nbr_dep_gardian_max] = 400;        // Tableau des temps avant chaque déplacmements du gardien.
      tab_vit_dep_gardien[nbr_dep_gardian_max] = 3;            // Tableau des vitesses de déplacement du gardien.
      nbr_dep_gardian_max++;                                   // Indique le nombre de déplacements effectués par le gardien.
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
      //Mise a zero du compteur de lancement du SAPOUCH.
      cmpt_tir_gard_spec = 0;
      // On lance un NAGGYS pour occuper le joueur avant l'apparition du Gardien N°4.
      if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
        Add_Ve_Special(NAGGYS);
    }
      break;

      //guardian 5 initialize ........................................................
    case 5:
    {                                                          // On rajoute un nouveau gardien dans la liste des ennemis.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      iNumeroGardien = new_ve;
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + FLASHY);       // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 110);        // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve etat
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 15;                             // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
      }
      ve[new_ve].cad_tir_initiale = 80;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(LARG_ECR_RAM - ve[new_ve].spr.img[15]->larg / 2);
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - ve[new_ve].spr.img[15]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = FLASHY;                                // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.
      ptr_sur_tab_gardien = 0;
      nbr_dep_gardian_max = 0;                                 // Indique le nombre de déplacements effectués par le gardien.
      Fill_Tab_Dep_Gard_Droite();
      Fill_Tab_Dep_Gard_Gauche();
      Fill_Tab_Dep_Gard_Bas();
      tab_traj_gardien[nbr_dep_gardian_max] = 24;              // Indique que l'on se déplace vers le haut.
      tab_tmps_traj_gardien[nbr_dep_gardian_max] = 400;        // Tableau des temps avant chaque déplacmements du gardien.
      tab_vit_dep_gardien[nbr_dep_gardian_max] = 3;            // Tableau des vitesses de déplacement du gardien.
      nbr_dep_gardian_max++;                                   // Indique le nombre de déplacements effectués par le gardien.
      Fill_Tab_Dep_Gard_Droite();
      Fill_Tab_Dep_Gard_Gauche();
      Fill_Tab_Dep_Gard_Droite();
      Fill_Tab_Dep_Gard_Bas();
      tab_traj_gardien[nbr_dep_gardian_max] = 24;              // Indique que l'on se déplace vers le haut.
      tab_tmps_traj_gardien[nbr_dep_gardian_max] = 400;        // Tableau des temps avant chaque déplacmements du gardien.
      tab_vit_dep_gardien[nbr_dep_gardian_max] = 3;            // Tableau des vitesses de déplacement du gardien.
      nbr_dep_gardian_max++;                                   // Indique le nombre de déplacements effectués par le gardien.
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
      //Mise a zero du compteur de lancement de SOUKEE.
      cmpt_tir_gard_spec = 0;
    }
      break;

      //guardian 6 initialize ........................................................
    case 6:
    {                                                          // On rajoute un nouveau gardien dans la liste des ennemis.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      iNumeroGardien = new_ve;
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + MEECKY);       // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 130);        // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve etat
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 15;                             // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
      }
      ve[new_ve].cad_tir_initiale = 80;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(LARG_ECR_RAM - ve[new_ve].spr.img[15]->larg / 2);
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - ve[new_ve].spr.img[15]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = MEECKY;                                // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.

      // On rajoute la barre qui s'allonge dessous le gardien.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + MEECKY);       // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (ve[new_ve].spr.pow_of_dest << 3) / 3;     // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve etat
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 15;                             // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[1][k];
      }
      ve[new_ve].cad_tir_initiale = 80;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x = ve[0].spr.coor_x;
      ve[new_ve].spr.coor_y = ve[0].spr.coor_y + 63;
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = MEECKY;                                // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.

      // Initialisation du pointeur sur le tableau qui permet les déplacements du gardien.
      ptr_sur_tab_gardien = 0;
      // Initialisation du compteur de déplacements du gardien.
      nbr_dep_gardian_max = 0;                                 // Indique le nombre de déplacements effectués par le gardien.
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la droite.
      Fill_Tab_Dep_Gard_Droite();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
      //Mise a zero du compteur de lancement de SOUKEE.
      cmpt_tir_gard_spec = 0;
      // On lance un NAGGYS pour occuper le joueur avant l'apparition du Gardien N°6.
//                      if (nbr_nmis<(MAX_NMI_ON_SCREEN-2)) Add_Ve_Special(NAGGYS);
    }
      break;

      //guardian 7 initialize ........................................................
    case 7:
    {
      new_ve = New_Element_Ve();                               //get a new index
      iNumeroGardien = new_ve;
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + TYPYBOON);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 150);        // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve l'etat des dommages a la creation du gardien.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
      }
      ve[new_ve].cad_tir_initiale = 55;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      ve[new_ve].spr.coor_x = (float)(LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg / 2);  //x start
      ve[new_ve].spr.coor_y = (float)(DEBUT_ECRAN_Y - ve[new_ve].spr.img[0]->haut);     //y start
      ve[new_ve].spr.vitesse = 0.5;                            //move speed
      ve[new_ve].type = TYPYBOON;                              //type of guardian
      ve[new_ve].dead = FALSE;                                 //guardian is not dead
      ve[new_ve].visible = TRUE;                               //guardian is visible
      ptr_sur_tab_gardien = 0;                                 //
      nbr_dep_gardian_max = 0;                                 // Indique le nombre de déplacements effectués par le gardien.
      Fill_Tab_Dep_Gard_Droite();
      Fill_Tab_Dep_Gard_Gauche();
      //Mise a zero du compteur de lancement de QUIBOULY.
      compteur_quibouly = 0;
      // On lance un NAGGYS pour occuper le joueur avant l'apparition du Gardien N°6.
      if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
        Add_Ve_Special(NAGGYS);
    }
      break;

      //guardian 8 initialize ........................................................
    case 8:
    {                                                          // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      iNumeroGardien = new_ve;
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + MATHYDEE);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 170);        // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve l'etat des dommages a la creation du gardien.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 0;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
      }
      ve[new_ve].cad_tir_initiale = 65;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg / 2);
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - ve[new_ve].spr.img[0]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = MATHYDEE;                              // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.
      // Initialisation du pointeur sur le tableau qui permet les déplacements du gardien.
      ptr_sur_tab_gardien = 0;
      // Initialisation du compteur de déplacements du gardien.
      nbr_dep_gardian_max = 0;                                 // Indique le nombre de déplacements effectués par le gardien.
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la droite.
      Fill_Tab_Dep_Gard_Droite();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
      //Mise a zero du compteur de lancement de TOURNADEE.
      compteur_tournadee = 0;
      // On lance un NAGGYS pour occuper le joueur avant l'apparition du Gardien N°6.
      if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
        Add_Ve_Special(NAGGYS);
    }
      break;

      //guardian 9 initialize ........................................................
    case 9:
    {
      new_ve = New_Element_Ve();                               //nouvel element de la liste.
      iNumeroGardien = new_ve;
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + OVYDOON);      // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 190);        // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve etat
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 15;                             // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
      }
      ve[new_ve].cad_tir_initiale = 60;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(LARG_ECR_RAM - ve[new_ve].spr.img[15]->larg / 2);
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - ve[new_ve].spr.img[15]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = OVYDOON;                               // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.

      // On rajoute la barre qui s'allonge dessous le gardien.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + OVYDOON);      // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (ve[new_ve].spr.pow_of_dest * 3) / 2;      // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve etat
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 15;                             // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[1][k];
      }
      ve[new_ve].cad_tir_initiale = 75;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x = ve[0].spr.coor_x;
      ve[new_ve].spr.coor_y = ve[0].spr.coor_y;
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = OVYDOON;                               // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.

      // Initialisation du pointeur sur le tableau qui permet les déplacements du gardien.
      ptr_sur_tab_gardien = 0;
      // Initialisation du compteur de déplacements du gardien.
      nbr_dep_gardian_max = 0;                                 // Indique le nombre de déplacements effectués par le gardien.
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la droite.
      Fill_Tab_Dep_Gard_Droite();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
      //Mise a zero du compteur de lancement de SOUKEE.
      cmpt_tir_gard_spec = 0;
      // On lance un NAGGYS pour occuper le joueur avant l'apparition du Gardien N°6.
      if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
        Add_Ve_Special(NAGGYS);
    }
      break;

      //guardian 10 initialize ........................................................
    case 10:
    {                                                          // On rajoute un nouveau gardien dans la liste des ennemis.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      iNumeroGardien = new_ve;
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + GATLEENY);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 210);        // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve etat
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 15;                             // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 1;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
      }
      ve[new_ve].cad_tir_initiale = 50;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(LARG_ECR_RAM - ve[new_ve].spr.img[15]->larg / 2);
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - ve[new_ve].spr.img[15]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = GATLEENY;                              // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.

      // On rajoute la barre qui s'allonge dessous le gardien.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + GATLEENY);     // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 210);        // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve etat
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 8;                              // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 20;                         // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[1][k];
      }
      ve[new_ve].cad_tir_initiale = 65;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x = ve[0].spr.coor_x;
      ve[new_ve].spr.coor_y = ve[0].spr.coor_y;
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = GATLEENY;                              // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.

      // Initialisation du sens de deplacement du gardien.
      send_dep_gard10 = BAS;
      //Mise a zero du compteur de tir.
      cmpt_tir_gard_spec = 0;
      // On lance un NAGGYS pour occuper le joueur avant l'apparition du Gardien N°6.
      if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
        Add_Ve_Special(NAGGYS);
    }
      break;

      //guardian 11 initialize .......................................................
    case 11:
    {                                                          // On rajoute un nouveau gardien dans la liste des ennemis.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
      iNumeroGardien = new_ve;
      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + NAUTEE);       // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 230);        // Indique l'état de domages du sprite.
      ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;  // sauve etat
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = 15;                             // Indique l'image actuelle.
      ve[new_ve].spr.nbr_vbl_act = 0;                          // RAZ du compteur de VBL pour le passage à l'image suivante.
      ve[new_ve].spr.nbr_tot_vbl = 4;                          // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
      // Donne l'adresse des images du sprite.
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
      }
      ve[new_ve].cad_tir_initiale = 60;                        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
      ve[new_ve].deplacement = GARDIEN;                        // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      // Donne les coordonées x et y de départ du vaisseau.
      ve[new_ve].spr.coor_x =
        (float)(LARG_ECR_RAM - ve[new_ve].spr.img[15]->larg / 2);
      ve[new_ve].spr.coor_y =
        (float)(DEBUT_ECRAN_Y - ve[new_ve].spr.img[15]->haut);
      ve[new_ve].spr.vitesse = 0.5;                            // Donne la vitesse de déplacement du gardien.
      ve[new_ve].type = NAUTEE;                                // Indique le type de gardien.
      ve[new_ve].dead = FALSE;                                 // Indique que l'ennemi est bien vivant.
      ve[new_ve].visible = TRUE;                               // Indique que l'ennemi est visible, on devra donc l'afficher.

      // Initialisation du pointeur sur le tableau qui permet les déplacements du gardien.
      ptr_sur_tab_gardien = 0;
      // Initialisation du compteur de déplacements du gardien.
      nbr_dep_gardian_max = 0;                                 // Indique le nombre de déplacements effectués par le gardien.
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la droite.
      Fill_Tab_Dep_Gard_Droite();
      // Remplissage du tableau des déplacement des gardiens pour un déplacement vers la gauche.
      Fill_Tab_Dep_Gard_Gauche();
      //Mise a zero du compteur de lancement de SOUKEE.
      cmpt_tir_gard_spec = 0;
      // On lance un NAGGYS pour occuper le joueur avant l'apparition du Gardien N°6.
      if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
        Add_Ve_Special(NAGGYS);
    }
      break;

      //guardian 12 initialize .......................................................
    case 12:                                                  //(ETB)
      _init_gardien12();
      break;
      //guardian 13 initialize .......................................................
    case 13:                                                  //(ETB)
      _init_gardien13();
      break;
      //guardian 14 initialize .......................................................
    case 14:                                                  //(ETB)
      _init_gardien14();
      break;
      //guardian 15 initialize .......................................................
    case 15:                                                  //(ETB)
      _init_congra();
      break;
  }
}

//------------------------------------------------------------------------------
// run all guardians + congratulations
//------------------------------------------------------------------------------
void Gere_gardien(int num_gardien)
{
  int                     k, j;

  switch (gardian_activity)
  {   //##############################################################################
      //guardian 1 run ###############################################################
      //##############################################################################
    case 1:
    {                                                          // Test si on est dans la phase d'apparition du gardien.
      if(apparition_gardian)
      {                                                        // Routine utilisée pour faire l'apparition d'un gardien par le haut de l'écran.
        Fait_Apparaitre_Gardien_Haut(num_gardien);
      }
      else                                                     // Sinon, on doit gérer le gardien.
      {                                                        // Décompte du temps séparant le prochain déplacement du gardien.
        cmpt_tmps_dep_gardien--;
        // Test si l'on doit passer au prochain déplacement.
        if(cmpt_tmps_dep_gardien > 0)
        {                                                      // Test si le gardien est en déplacment actuellement.
          if(tab_traj_gardien[ptr_sur_tab_gardien] < 32)       // 32 étant la position de repos, tous les nombres en dessous étant des directions.
          {                                                    // Routine utilisée pour déplacer un gardien en suivant une droite.
            Move_Gardian_Line(num_gardien);
          }
          // Test le sens de déplacement du gardien.
          switch (tab_traj_gardien[ptr_sur_tab_gardien])
          {                                                    // Cas d'un déplacement vers la droite.
            case 0:
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 3)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act++;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act >= 31)
                  ve[num_gardien].spr.img_act = 31;
              }
              break;
              // Cas d'un déplacement vers la gauche.
            case 16:
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 3)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act--;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act <= 0)
                  ve[num_gardien].spr.img_act = 0;
              }
              break;
              // Cas de la position de repos.
            case 32:
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 3)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On met le vaisseau en position centrale.
                if(ve[num_gardien].spr.img_act > 15)
                  ve[num_gardien].spr.img_act--;
                if(ve[num_gardien].spr.img_act < 15)
                  ve[num_gardien].spr.img_act++;
              }
              break;
          }
        }
        else                                                   // Sinon, on doit passer au prochain déplacement.
        {                                                      // On passe au déplacement suivant.
          ptr_sur_tab_gardien++;
          // Test si on n'a pas dépasser le nombre maximum de déplacements.
          if(ptr_sur_tab_gardien >= nbr_dep_gardian_max)
          {                                                    // On indique qu'il faut faire disparaitre le gardien.
            ptr_sur_tab_gardien = 0;
          }
          // Remet à jour le compteur de temps avant chaque déplacments du gardien.
          cmpt_tmps_dep_gardien = tab_tmps_traj_gardien[ptr_sur_tab_gardien];
        }
      }

      // Test si la pause est activée ou pas.
      if(!player_pause && etat_menu == MENU_OFF
         && _score::getActionExecution() == 0)
      {                                                        // Décompte le temps séparant deux tirs.
        ve[num_gardien].cad_tir--;
        // Test si l'ennemi doit ou non lancer un tir.
        if(ve[num_gardien].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
        {                                                      // Met la cadence du tir pour le vaisseau ennemi.
          ve[num_gardien].cad_tir = ve[num_gardien].cad_tir_initiale;
          // Bcle sur tous les points de tirs existants.
          for(k = 0;
              k <
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
              nbr_pts_tir; k++)
          {                                                    // Test s'il est possible d'ajouter un nouveau tir à la liste.
            if(nbr_tirs < (NBR_TIRS_MAX - 1))
            {                                                  // Recherche d'un nouveau tir.
              new_tir = New_Element_Tir();
#ifdef USE_SDLMIXER
              seal_joue(TIR3NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
              tirs[new_tir].clignote = TRUE;                   // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
              tirs[new_tir].spr.genre = ENNEMI;                // Indique que c'est un sprite ennemi.
              tirs[new_tir].spr.trajectoire = FALSE;           // Indique que la trajectoire du sprite est calculée une fois pour toute.
              tirs[new_tir].spr.nbr_img = 32;                  // Indique le nombre d'images composant le sprite.
              // Indique la puissance de destruction du sprite.
              tirs[new_tir].spr.pow_of_dest = 8;
              // Donne l'adresse des images du sprite.
              for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
              {                                                // Chargement du missile.
                tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P3E][j];        // Corespond au Tir 2 puissance 2.
              }
              tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;  // Indique l'état de domages du sprite.
              //tirs[new_tir].spr.pilote = 0;                    // Indique par qui est dirigé le sprite.
              //tirs[new_tir].spr.mode_commande = 0;             // Indique le moyen par lequel on fait déplacer le sprite.
              tirs[new_tir].spr.img_act = 0;                   // Indique l'image actuelle.
              tirs[new_tir].spr.nbr_tot_vbl = 1;               // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              tirs[new_tir].spr.nbr_vbl_act = 0;               // Indique le nombre de VBL comptées actuellement.
              // Donne les coordonnées d'affichage du sprite.
              tirs[new_tir].img_angle =
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                angle_tir[k];
              tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
              tirs[new_tir].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              tirs[new_tir].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              tirs[new_tir].tmp_life_tir = 400;
              // Donne un angle vertical pour le tir.
              tirs[new_tir].angle_tir =
                (float)(PI_SUR_16 * tirs[new_tir].img_angle);
              tirs[new_tir].spr.vitesse = 2.0;                 // Donne la vitesse de déplacement du sprite.
            }
          }
        }                                                      // Fin du test pour savoir si l'ennemi doit ou non tirer.
      }                                                        // Fin du test pour savoir si la pause est ou non activée.
    }
      break;                                                   // Fin de la gestion du premier gardien.


      //##############################################################################
      //guardian 2 run ###############################################################
      //##############################################################################
    case 2:
    {                                                          // Test si on est dans la phase d'apparition du gardien.
      if(apparition_gardian)
      {                                                        // On fait avancer le gardien jusqu'au début de l'écran.
        ve[num_gardien].spr.coor_y += ve[num_gardien].spr.vitesse;
        // Test si la positon du gardien est arrivée au début de l'écran.
        if(ve[num_gardien].spr.coor_y >= (float)(BANDE_DE_CLIP - 15))
        {                                                      // Clip la position du gardien.
          ve[num_gardien].spr.coor_y = (float)(BANDE_DE_CLIP - 15);
          // Indique que la phase d'appariton du gardien est terminée.
          apparition_gardian = FALSE;
          // Initialisation du pointeur sur le tableau qui permet les déplacements du gardien.
          ptr_sur_tab_gardien = 0;
          // Initialisation du compteur temps avant le prochain déplacement du gardien.
          cmpt_tmps_dep_gardien = tab_tmps_traj_gardien[ptr_sur_tab_gardien];
        }
      }
      else                                                     // Sinon, on doit gérer le gardien.
      {                                                        // Décompte du temps séparant le prochain déplacement du gardien.
        cmpt_tmps_dep_gardien--;
        // Test si l'on doit passer au prochain déplacement.
        if(cmpt_tmps_dep_gardien > 0)
        {                                                      // Test si le gardien est en déplacment actuellement.
          if(tab_traj_gardien[ptr_sur_tab_gardien] < 32 && !player_pause && etat_menu == MENU_OFF && _score::getActionExecution() == 0) // 32 étant la position de repos, tous les nombres en dessous étant des directions.
          {                                                    // Donne la nouvelle coordonée X du gardien à trajectoire fixe.
            ve[num_gardien].spr.coor_x +=
              depix[tab_vit_dep_gardien[ptr_sur_tab_gardien]][tab_traj_gardien
                                                              [ptr_sur_tab_gardien]];
            // Donne la nouvelle coordonée Y du gardien à trajectoire fixe.
            ve[num_gardien].spr.coor_y +=
              depiy[tab_vit_dep_gardien[ptr_sur_tab_gardien]][tab_traj_gardien
                                                              [ptr_sur_tab_gardien]];
            // Clip les coordonées du gardien.
            if(ve[num_gardien].spr.coor_x < (float)(BANDE_DE_CLIP - 10))
            {
              ve[num_gardien].spr.coor_x = (float)(BANDE_DE_CLIP - 10);
              cmpt_tmps_dep_gardien = 0;
            }
            if((ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->larg) >
               (float)(BANDE_DE_CLIP + LARG_ECR_RAM + 40))
            {
              ve[num_gardien].spr.coor_x =
                (float)(BANDE_DE_CLIP + LARG_ECR_RAM + 40) -
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->larg;
              cmpt_tmps_dep_gardien = 0;
            }
            if(ve[num_gardien].spr.coor_y < (float)(BANDE_DE_CLIP - 15))
            {
              ve[num_gardien].spr.coor_y = (float)(BANDE_DE_CLIP - 15);
              cmpt_tmps_dep_gardien = 0;
            }
            if((ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->haut) >
               (float)(BANDE_DE_CLIP + HAUT_ECR_RAM + 40))
            {
              ve[num_gardien].spr.coor_y =
                (float)(BANDE_DE_CLIP + HAUT_ECR_RAM + 40) -
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->haut;
              cmpt_tmps_dep_gardien = 0;
            }
          }
          // Test le sens de déplacement du gardien.
          switch (tab_traj_gardien[ptr_sur_tab_gardien])
          {                                                    // Cas d'un déplacement vers la droite.
            case 0:
              // Test si le gardien vient de changer de direction.
              if(change_direction)
              {                                                // On réinitialise la position de l'image actuelle.
                ve[num_gardien].spr.img_act = 15;
                // RAZ du flag qui indique si un gardien vient de changer de direction.
                change_direction = FALSE;
                // Donne l'adresse des images du sprite.
                for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                {
                  ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                }
              }
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 4)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act++;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act >= 31)
                  ve[num_gardien].spr.img_act = 31;
              }
              break;
              // Cas d'un déplacement vers la gauche.
            case 16:
              // Test si le gardien vient de changer de direction.
              if(change_direction)
              {                                                // On réinitialise la position de l'image actuelle.
                ve[num_gardien].spr.img_act = 15;
                // RAZ du flag qui indique si un gardien vient de changer de direction.
                change_direction = FALSE;
                // Donne l'adresse des images du sprite.
                for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                {
                  ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                }
              }
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 4)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act--;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act <= 0)
                  ve[num_gardien].spr.img_act = 0;
              }
              break;
              // Cas d'un déplacement vers le haut ou vers le bas.
            case 24:
            case 8:
              // Test si le gardien vient de changer de direction.
              if(change_direction)
              {                                                // On réinitialise la position de l'image actuelle.
                ve[num_gardien].spr.img_act = 0;
                // RAZ du flag qui indique si un gardien vient de changer de direction.
                change_direction = FALSE;
                // Donne l'adresse des images du sprite.
                for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                {
                  ve[num_gardien].spr.img[k] = (image *) & gardi[1][k];
                }
              }
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 2)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act++;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act >= 31)
                  ve[num_gardien].spr.img_act = 31;
              }
              break;
              // Cas de la position de repos.
            case 32:
              // Test si le gardien vient de changer de direction.
              if(change_direction)
              {                                                // RAZ du flag qui indique si un gardien vient de changer de direction.
                change_direction = FALSE;
                // Donne l'adresse des images du sprite.
                for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                {
                  ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                }
                // Test si on était précédement sur une trajectoire verticale afin de se
                // positionner sur la bonne image.
                if(vertical_trajectoire)
                {                                              // On se positionne sur la bonne image.
                  ve[num_gardien].spr.img_act = 15;
                  // On réinitialise le flag indiquant si on est sur une trajectoire verticale ou pas.
                  vertical_trajectoire = FALSE;
                }
              }
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 3)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On met le vaisseau en position centrale.
                if(ve[num_gardien].spr.img_act > 15)
                  ve[num_gardien].spr.img_act--;
                if(ve[num_gardien].spr.img_act < 15)
                  ve[num_gardien].spr.img_act++;
              }
              break;
          }
        }
        else                                                   // Sinon, on doit passer au prochain déplacement.
        {                                                      // Test si avant de changer, on était sur une trajectoire verticale.
          if(tab_traj_gardien[ptr_sur_tab_gardien] == 8
             || tab_traj_gardien[ptr_sur_tab_gardien] == 24)
            vertical_trajectoire = TRUE;
          // On passe au déplacement suivant.
          ptr_sur_tab_gardien++;
          // Test si on n'a pas dépasser le nombre maximum de déplacements.
          if(ptr_sur_tab_gardien >= nbr_dep_gardian_max)
          {                                                    // On indique qu'il faut faire disparaitre le gardien.
            ptr_sur_tab_gardien = 0;
          }
          change_direction = TRUE;                             // Indique que l'on vient de changer de direction.
          // Remet à jour le compteur de temps avant chaque déplacments du gardien.
          cmpt_tmps_dep_gardien = tab_tmps_traj_gardien[ptr_sur_tab_gardien];
        }
      }

      // Test si la pause est activée ou pas.
      if(!player_pause && etat_menu == MENU_OFF
         && _score::getActionExecution() == 0)
      {                                                        // Décompte le temps séparant deux tirs.
        ve[num_gardien].cad_tir--;
        // Test si l'ennemi doit ou non lancer un tir.
        if(ve[num_gardien].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
        {                                                      // Met la cadence du tir pour le vaisseau ennemi.
          ve[num_gardien].cad_tir = ve[num_gardien].cad_tir_initiale;
          // Bcle sur tous les points de tirs existants.
          for(k = 0;
              k <
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
              nbr_pts_tir; k++)
          {                                                    // Test s'il est possible d'ajouter un nouveau tir à la liste.
            if(nbr_tirs < (NBR_TIRS_MAX - 1))
            {                                                  // Recherche d'un nouveau tir.
              new_tir = New_Element_Tir();
#ifdef USE_SDLMIXER
              seal_joue(TIR3NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
              tirs[new_tir].clignote = TRUE;                   // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
              tirs[new_tir].spr.genre = ENNEMI;                // Indique que c'est un sprite ennemi.
              tirs[new_tir].spr.trajectoire = FALSE;           // Indique que la trajectoire du sprite est calculée une fois pour toute.
              tirs[new_tir].spr.nbr_img = 32;                  // Indique le nombre d'images composant le sprite.
              // Indique la puissance de destruction du sprite.
              tirs[new_tir].spr.pow_of_dest = 10;
              // Donne l'adresse des images du sprite.
              for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
              {                                                // Chargement du missile.
                tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P3E][j];        // Corespond au Tir 2 puissance 2.
              }
              tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;  // Indique l'état de domages du sprite.
              //tirs[new_tir].spr.pilote = 0;                    // Indique par qui est dirigé le sprite.
              //tirs[new_tir].spr.mode_commande = 0;             // Indique le moyen par lequel on fait déplacer le sprite.
              tirs[new_tir].spr.img_act = 0;                   // Indique l'image actuelle.
              tirs[new_tir].spr.nbr_tot_vbl = 1;               // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              tirs[new_tir].spr.nbr_vbl_act = 0;               // Indique le nombre de VBL comptées actuellement.
              // Donne les coordonnées d'affichage du sprite.
              tirs[new_tir].img_angle =
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                angle_tir[k];
              tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
              tirs[new_tir].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              tirs[new_tir].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              tirs[new_tir].tmp_life_tir = 400;
              // Donne un angle vertical pour le tir.
              tirs[new_tir].angle_tir =
                (float)(PI_SUR_16 * tirs[new_tir].img_angle);
              tirs[new_tir].spr.vitesse = 3.0;                 // Donne la vitesse de déplacement du sprite.
            }
          }
          // Incrémente le compteur permettant d'ajouter un SHURIKY au gardien.
          compteur_shuriy++;
          // Test si on doit rajouter un SHURIKY.
          if(compteur_shuriy >= 4)
          {                                                    // On remet le compteur de SHURIKY à zéro.
            compteur_shuriy = 0;
            // On rajoute un Vaisseau spécial SHURIKY dans la liste.
            new_ve = New_Element_Ve();                         // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
#ifdef USE_SDLMIXER
            seal_joue(TIR4NMI);                                //joue un son avec seal
#endif
            // Remplissage de la structure sprite appartenant au vaisseau ennemi.
            ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + 10);     // Indique la puissance de destruction du sprite.
            ve[new_ve].spr.etat_dommages = ve[new_ve].spr.pow_of_dest;  // Indique l'état de domages du sprite.
            ve[new_ve].spr.nbr_img = 32;                       // Indique le nombre d'images composant le sprite.
            ve[new_ve].spr.img_act = 0;                        // Indique l'image actuelle.
            ve[new_ve].spr.nbr_vbl_act = 0;                    // RAZ du compteur de VBL pour le passage à l'image suivante.
            ve[new_ve].spr.nbr_tot_vbl = 2;                    // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
            // Donne l'adresse des images du sprite.
            for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
            {
              ve[new_ve].spr.img[k] = (image *) & enemi[SHURIKY][k];
            }
            ve[new_ve].cad_tir_initiale = 70;                  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
            ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;  // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
            ve[new_ve].deplacement = SPECIAUX;                 // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
            // Donne les coordonées x et y de départ du vaisseau.
            ve[new_ve].spr.coor_x =
              ve[num_gardien].spr.coor_x +
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->x_cg -
              ve[new_ve].spr.img[0]->larg / 2;
            ve[new_ve].spr.coor_y =
              ve[num_gardien].spr.coor_y +
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->y_cg -
              ve[new_ve].spr.img[0]->haut / 2;
            ve[new_ve].spr.vitesse = 0.3f;                     // Donne la vitesse de déplacement du vaisseau ennemi spécial.
            ve[new_ve].type = SHURIKY;                         // Indique le type de vaisseau spécial.
            ve[new_ve].dead = FALSE;                           // Indique que l'ennemi est bien vivant.
            ve[new_ve].visible = TRUE;                         // Indique que l'ennemi est visible, on devra donc l'afficher.
          }
        }                                                      // Fin du test pour savoir si l'ennemi doit ou non tirer.
      }                                                        // Fin du test pour savoir si la pause est ou non activée.
    }
      break;

      //##############################################################################
      //guardian 3 run ###############################################################
      //##############################################################################
    case 3:
    {                                                          // Test si on est dans la phase d'apparition du gardien.
      if(apparition_gardian)
      {                                                        // Routine utilisée pour faire l'apparition d'un gardien par le haut de l'écran.
        Fait_Apparaitre_Gardien_Haut(num_gardien);
      }
      else                                                     // Sinon, on doit gérer le gardien.
      {                                                        // Décompte du temps séparant le prochain déplacement du gardien.
        cmpt_tmps_dep_gardien--;
        // Test si l'on doit passer au prochain déplacement.
        if(cmpt_tmps_dep_gardien > 0)
        {                                                      // Test si le gardien est en déplacment actuellement.
          if(tab_traj_gardien[ptr_sur_tab_gardien] < 32)       // 32 étant la position de repos, tous les nombres en dessous étant des directions.
          {                                                    // Routine utilisée pour déplacer un gardien en suivant une droite.
            Move_Gardian_Line(num_gardien);
          }
          // Test le sens de déplacement du gardien.
          switch (tab_traj_gardien[ptr_sur_tab_gardien])
          {                                                    // Cas d'un déplacement vers la droite.
            case 0:
              // Cas d'un déplacement vers la gauche.
            case 16:
              // Cas de la position de repos.
            case 32:
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 3)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act++;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act >= 31)
                  ve[num_gardien].spr.img_act = 0;
              }
              break;
          }
        }
        else                                                   // Sinon, on doit passer au prochain déplacement.
        {                                                      // On passe au déplacement suivant.
          ptr_sur_tab_gardien++;
          // Test si on n'a pas dépasser le nombre maximum de déplacements.
          if(ptr_sur_tab_gardien >= nbr_dep_gardian_max)
          {                                                    // On indique qu'il faut faire disparaitre le gardien.
            ptr_sur_tab_gardien = 0;
          }
          // Remet à jour le compteur de temps avant chaque déplacments du gardien.
          cmpt_tmps_dep_gardien = tab_tmps_traj_gardien[ptr_sur_tab_gardien];
        }
      }

      // Test si la pause est activée ou pas.
      if(!player_pause && etat_menu == MENU_OFF
         && _score::getActionExecution() == 0)
      {                                                        // Décompte le temps séparant deux tirs.
        ve[num_gardien].cad_tir--;
        // Test si l'ennemi doit ou non lancer un tir.
        if(ve[num_gardien].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
        {                                                      // Met la cadence du tir pour le vaisseau ennemi.
          ve[num_gardien].cad_tir = ve[num_gardien].cad_tir_initiale;
          // Bcle sur tous les points de tirs existants.
          for(k = 0;
              k <
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
              nbr_pts_tir; k++)
          {                                                    // Test s'il est possible d'ajouter un nouveau tir à la liste.
            if(nbr_tirs < (NBR_TIRS_MAX - 1))
            {                                                  // Recherche d'un nouveau tir.
              new_tir = New_Element_Tir();
#ifdef USE_SDLMIXER
              seal_joue(TIR3NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
              tirs[new_tir].clignote = TRUE;                   // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
              tirs[new_tir].spr.genre = ENNEMI;                // Indique que c'est un sprite ennemi.
              tirs[new_tir].spr.trajectoire = FALSE;           // Indique que la trajectoire du sprite est calculée une fois pour toute.
              tirs[new_tir].spr.nbr_img = 32;                  // Indique le nombre d'images composant le sprite.
              // Indique la puissance de destruction du sprite.
              tirs[new_tir].spr.pow_of_dest = 8;
              // Donne l'adresse des images du sprite.
              for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
              {                                                // Chargement du missile.
                tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P3E][j];        // Corespond au Tir 2 puissance 2.
              }
              tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;  // Indique l'état de domages du sprite.
              //tirs[new_tir].spr.pilote = 0;                    // Indique par qui est dirigé le sprite.
              //tirs[new_tir].spr.mode_commande = 0;             // Indique le moyen par lequel on fait déplacer le sprite.
              tirs[new_tir].spr.img_act = 0;                   // Indique l'image actuelle.
              tirs[new_tir].spr.nbr_tot_vbl = 1;               // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              tirs[new_tir].spr.nbr_vbl_act = 0;               // Indique le nombre de VBL comptées actuellement.
              // Donne les coordonnées d'affichage du sprite.
              tirs[new_tir].img_angle =
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                angle_tir[k];
              tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
              tirs[new_tir].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              tirs[new_tir].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              tirs[new_tir].tmp_life_tir = 400;
              // Donne un angle vertical pour le tir.
              tirs[new_tir].angle_tir =
                (float)(PI_SUR_16 * tirs[new_tir].img_angle);
              tirs[new_tir].spr.vitesse = 2.0;                 // Donne la vitesse de déplacement du sprite.
            }
          }
          // Incrémente le compteur permettant d'ajouter un NAGGYS au gardien.
          cmpt_tir_gard_spec++;
          // Test si on doit rajouter un NAGGYS.
          if(cmpt_tir_gard_spec >= START_NAGGYS)
          {                                                    // On remet le compteur de NAGGYS à zéro.
#ifdef USE_SDLMIXER
            seal_joue(TIR1);
#endif
            cmpt_tir_gard_spec = 0;
            // On rajoute un ennemi dans la liste.
            if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
              Add_Ve_Special(NAGGYS);
          }
        }                                                      // Fin du test pour savoir si l'ennemi doit ou non tirer.
      }                                                        // Fin du test pour savoir si la pause est ou non activée.
    }
      break;                                                   // Fin de la gestion du troisieme gardien.

      //##############################################################################
      //guardian 4 run ###############################################################
      //##############################################################################
    case 4:
    {                                                          // Test si on est dans la phase d'apparition du gardien.
      if(apparition_gardian)
      {                                                        // Routine utilisée pour faire l'apparition d'un gardien par le haut de l'écran.
        Fait_Apparaitre_Gardien_Haut(num_gardien);
      }
      else                                                     // Sinon, on doit gérer le gardien.
      {                                                        // Décompte du temps séparant le prochain déplacement du gardien.
        cmpt_tmps_dep_gardien--;
        //Test si on doit lancer des DEVILIANS.
        if(cmpt_DEVILIANS > 0)
        {                                                      //Incremente le temps separant le lancement du prochain DEVILIANS.
          cmpt_tmps_DEVILIANS++;
          if(cmpt_tmps_DEVILIANS > START_DEVILIANS)
          {                                                    // On rajoute un vaisseau spécial DEVILIANS dans la liste.
            if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
              Add_Ve_Special(DEVILIANS);
            cmpt_tmps_DEVILIANS = 0;
            cmpt_DEVILIANS++;                                  //Incremente le nombre de DEVILIANS lances.
          }
          //Demande d'arreter le lancement des DEVILIANS.
          if(cmpt_DEVILIANS > MAX_DEVILIANS)
            cmpt_DEVILIANS = 0;
        }

        // Test si l'on doit passer au prochain déplacement.
        if(cmpt_tmps_dep_gardien > 0)
        {                                                      // Test si le gardien est en déplacment actuellement.
          if(tab_traj_gardien[ptr_sur_tab_gardien] == 0
             || tab_traj_gardien[ptr_sur_tab_gardien] == 16)
          {                                                    // Routine utilisée pour déplacer un gardien en suivant une droite.
            Move_Gardian_Line(num_gardien);
          }
          // Test le sens de déplacement du gardien.
          switch (tab_traj_gardien[ptr_sur_tab_gardien])
          {                                                    // Cas d'un déplacement vers la droite.
            case 0:
              // Test si le gardien vient de changer de direction.
              if(change_direction)
              {                                                // On réinitialise la position de l'image actuelle.
                ve[num_gardien].spr.img_act = 15;
                // RAZ du flag qui indique si un gardien vient de changer de direction.
                change_direction = FALSE;
                // Donne l'adresse des images du sprite.
                for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                {
                  ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                }
              }
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 4)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act++;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act >= 31)
                  ve[num_gardien].spr.img_act = 31;
              }
              break;
              // Cas d'un déplacement vers la gauche.
            case 16:
              // Test si le gardien vient de changer de direction.
              if(change_direction)
              {                                                // On réinitialise la position de l'image actuelle.
                ve[num_gardien].spr.img_act = 15;
                // RAZ du flag qui indique si un gardien vient de changer de direction.
                change_direction = FALSE;
                // Donne l'adresse des images du sprite.
                for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                {
                  ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                }
              }
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 4)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act--;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act <= 0)
                  ve[num_gardien].spr.img_act = 0;
              }
              break;
              // Cas d'un déplacement vers le bas.
            case 8:
              // Test si le gardien vient de changer de direction.
              if(change_direction)
              {                                                // On réinitialise la position de l'image actuelle.
                ve[num_gardien].spr.img_act = 0;
                // RAZ du flag qui indique si un gardien vient de changer de direction.
                change_direction = FALSE;
                // Donne l'adresse des images du sprite.
                for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                {
                  ve[num_gardien].spr.img[k] = (image *) & gardi[1][k];
                }
                //Demande le debut de l'envoi des DEVILIANS par le gardien N°4.
                cmpt_DEVILIANS = 1;
              }
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 2)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act++;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act >= 31)
                  ve[num_gardien].spr.img_act = 31;
              }
              break;
              // Cas d'un déplacement vers le haut.
            case 24:
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 2)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act--;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act < 0)
                  ve[num_gardien].spr.img_act = 0;
              }
              break;
              // Cas de la position de repos.
            case 32:
              //Test si le gardien vient de changer de direction.
              if(change_direction)
              {                                                // RAZ du flag qui indique si un gardien vient de changer de direction.
                change_direction = FALSE;
                // Donne l'adresse des images du sprite.
                for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                {
                  ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                }
                // Test si on était précédement sur une trajectoire verticale afin de se
                // positionner sur la bonne image.
                if(vertical_trajectoire)
                {                                              // On se positionne sur la bonne image.
                  ve[num_gardien].spr.img_act = 15;
                  // On réinitialise le flag indiquant si on est sur une trajectoire verticale ou pas.
                  vertical_trajectoire = FALSE;
                }
              }
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 3)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On met le vaisseau en position centrale.
                if(ve[num_gardien].spr.img_act > 15)
                  ve[num_gardien].spr.img_act--;
                if(ve[num_gardien].spr.img_act < 15)
                  ve[num_gardien].spr.img_act++;
              }
              break;
          }
        }
        else                                                   // Sinon, on doit passer au prochain déplacement.
        {                                                      // Test si avant de changer, on était sur une trajectoire verticale.
          if(tab_traj_gardien[ptr_sur_tab_gardien] == 8
             || tab_traj_gardien[ptr_sur_tab_gardien] == 24)
            vertical_trajectoire = TRUE;
          // On passe au déplacement suivant.
          ptr_sur_tab_gardien++;
          // Test si on n'a pas dépasser le nombre maximum de déplacements.
          if(ptr_sur_tab_gardien >= nbr_dep_gardian_max)
          {                                                    // On indique qu'il faut faire disparaitre le gardien.
            ptr_sur_tab_gardien = 0;
          }
          change_direction = TRUE;                             // Indique que l'on vient de changer de direction.
          // Remet à jour le compteur de temps avant chaque déplacments du gardien.
          cmpt_tmps_dep_gardien = tab_tmps_traj_gardien[ptr_sur_tab_gardien];
        }
      }


      // Test si la pause est activée ou pas.
      if(!player_pause && etat_menu == MENU_OFF
         && _score::getActionExecution() == 0)
      {                                                        // Décompte le temps séparant deux tirs.
        ve[num_gardien].cad_tir--;
        // Test si l'ennemi doit ou non lancer un tir.
        if(ve[num_gardien].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
        {                                                      // Met la cadence du tir pour le vaisseau ennemi.
          ve[num_gardien].cad_tir = ve[num_gardien].cad_tir_initiale;
          // Bcle sur tous les points de tirs existants.
          for(k = 0;
              k <
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
              nbr_pts_tir; k++)
          {                                                    // Test s'il est possible d'ajouter un nouveau tir à la liste.
            if(nbr_tirs < (NBR_TIRS_MAX - 1))
            {                                                  // Recherche d'un nouveau tir.
              new_tir = New_Element_Tir();
#ifdef USE_SDLMIXER
              seal_joue(TIR3NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
              tirs[new_tir].clignote = TRUE;                   // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
              tirs[new_tir].spr.genre = ENNEMI;                // Indique que c'est un sprite ennemi.
              tirs[new_tir].spr.trajectoire = FALSE;           // Indique que la trajectoire du sprite est calculée une fois pour toute.
              tirs[new_tir].spr.nbr_img = 32;                  // Indique le nombre d'images composant le sprite.
              // Indique la puissance de destruction du sprite.
              tirs[new_tir].spr.pow_of_dest = 8;
              // Donne l'adresse des images du sprite.
              for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
              {                                                // Chargement du missile.
                tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P3E][j];        // Corespond au Tir 2 puissance 2.
              }
              tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;  // Indique l'état de domages du sprite.
              //tirs[new_tir].spr.pilote = 0;                    // Indique par qui est dirigé le sprite.
              //tirs[new_tir].spr.mode_commande = 0;             // Indique le moyen par lequel on fait déplacer le sprite.
              tirs[new_tir].spr.img_act = 0;                   // Indique l'image actuelle.
              tirs[new_tir].spr.nbr_tot_vbl = 1;               // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              tirs[new_tir].spr.nbr_vbl_act = 0;               // Indique le nombre de VBL comptées actuellement.
              // Donne les coordonnées d'affichage du sprite.
              tirs[new_tir].img_angle =
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                angle_tir[k];
              tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
              tirs[new_tir].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              tirs[new_tir].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              tirs[new_tir].tmp_life_tir = 400;
              // Donne un angle vertical pour le tir.
              tirs[new_tir].angle_tir =
                (float)(PI_SUR_16 * tirs[new_tir].img_angle);
              tirs[new_tir].spr.vitesse = 2.0;                 // Donne la vitesse de déplacement du sprite.
            }
          }
          // Incrémente le compteur permettant d'ajouter un SAPOUCH au gardien.
          cmpt_tir_gard_spec++;
          // Test si on doit rajouter un SAPOUCH.
          if(cmpt_tir_gard_spec >= START_SAPOUCH)
          {                                                    // On remet le compteur de SAPOUCH à zéro.
            cmpt_tir_gard_spec = 0;
            // On rajoute un vaisseau spécial SAPOUCH dans la liste.
            if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
              Add_Ve_Special(SAPOUCH);
#ifdef USE_SDLMIXER
            seal_joue(TIR1);
#endif
          }
        }                                                      // Fin du test pour savoir si l'ennemi doit ou non tirer.
      }                                                        // Fin du test pour savoir si la pause est ou non activée.
    }
      break;                                                   // Fin de la gestion du troisieme gardien.

      //##############################################################################
      //guardian 5 run ###############################################################
      //##############################################################################
    case 5:
    {
      if(apparition_gardian)                                   //apparition du gardien ?
      {
        Fait_Apparaitre_Gardien_Haut(num_gardien);
      }
      else
      {
        if(!player_pause && etat_menu == MENU_OFF && _score::getActionExecution() == 0) //pause activee ?
        {
          cmpt_tmps_dep_gardien--;
          if(cmpt_tmps_dep_gardien > 0)
          {
            if(tab_traj_gardien[ptr_sur_tab_gardien] == 0
               || tab_traj_gardien[ptr_sur_tab_gardien] == 16)
            {
              Move_Gardian_Sin(num_gardien);                   //deplacemenet en sinus
            }
            switch (tab_traj_gardien[ptr_sur_tab_gardien])
            {                                                  //deplacement vers la droite ....................................................
              case 0:
                if(change_direction)                           //on vient de changer de direction ?
                {
                  ve[num_gardien].spr.img_act = 15;            //reinitialise l'image
                  change_direction = FALSE;
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                    ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                }
                cmpt_vbl_trans++;                              //tempo animation
                if(cmpt_vbl_trans >= 4)
                {
                  cmpt_vbl_trans = 0;
                  ve[num_gardien].spr.img_act++;
                  if(ve[num_gardien].spr.img_act >= 31)
                    ve[num_gardien].spr.img_act = 31;
                }
                break;

                //deplacement vers la gauche ....................................................
              case 16:
                if(change_direction)                           //on vient de changer de direction ?
                {
                  ve[num_gardien].spr.img_act = 15;
                  change_direction = FALSE;
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                    ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                }
                cmpt_vbl_trans++;                              //tempo animation
                if(cmpt_vbl_trans >= 4)
                {
                  cmpt_vbl_trans = 0;
                  ve[num_gardien].spr.img_act--;
                  if(ve[num_gardien].spr.img_act <= 0)
                    ve[num_gardien].spr.img_act = 0;
                }
                break;

                //deplacement vers le bas.........................................................
              case 8:
                if(change_direction)                           //on vient de changer de direction ?
                {
                  ve[num_gardien].spr.img_act = 0;
                  change_direction = FALSE;
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                    ve[num_gardien].spr.img[k] = (image *) & gardi[1][k];
                }
                cmpt_vbl_trans++;                              //tempo animation
                if(cmpt_vbl_trans >= 2)
                {
                  cmpt_vbl_trans = 0;
                  ve[num_gardien].spr.img_act++;
                  if(ve[num_gardien].spr.img_act > 31)
                    ve[num_gardien].spr.img_act = 0;
                }
                break;

                //position repos ... .............................................................
              case 32:
                if(change_direction)                           //on vient de changer de direction ?
                {
                  change_direction = FALSE;
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                    ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                  if(vertical_trajectoire)
                  {
                    ve[num_gardien].spr.img_act = 15;
                    vertical_trajectoire = FALSE;
                  }
                }
                cmpt_vbl_trans++;
                if(cmpt_vbl_trans >= 3)
                {
                  cmpt_vbl_trans = 0;
                  if(ve[num_gardien].spr.img_act > 15)
                    ve[num_gardien].spr.img_act--;
                  if(ve[num_gardien].spr.img_act < 15)
                    ve[num_gardien].spr.img_act++;
                }
                break;
            }
          }
          else                                                 // Sinon, on doit passer au prochain déplacement.
          {
            if(tab_traj_gardien[ptr_sur_tab_gardien] == 8
               || tab_traj_gardien[ptr_sur_tab_gardien] == 24)
              vertical_trajectoire = TRUE;
            ptr_sur_tab_gardien++;                             //deplacement suivant
            if(ptr_sur_tab_gardien >= nbr_dep_gardian_max)
              ptr_sur_tab_gardien = 0;
            change_direction = TRUE;                           // Indique que l'on vient de changer de direction.
            cmpt_tmps_dep_gardien =
              tab_tmps_traj_gardien[ptr_sur_tab_gardien];
          }
        }
      }

      // Test si la pause est activée ou pas.
      if(!player_pause && etat_menu == MENU_OFF
         && _score::getActionExecution() == 0)
      {                                                        // Décompte le temps séparant deux tirs.
        ve[num_gardien].cad_tir--;
        // Test si l'ennemi doit ou non lancer un tir.
        if(ve[num_gardien].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
        {                                                      // Met la cadence du tir pour le vaisseau ennemi.
          ve[num_gardien].cad_tir = ve[num_gardien].cad_tir_initiale;
          // Bcle sur tous les points de tirs existants.
          for(k = 0;
              k <
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
              nbr_pts_tir; k++)
          {                                                    // Test s'il est possible d'ajouter un nouveau tir à la liste.
            if(nbr_tirs < (NBR_TIRS_MAX - 1))
            {                                                  // Recherche d'un nouveau tir.
              new_tir = New_Element_Tir();
#ifdef USE_SDLMIXER
              seal_joue(TIR3NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
              tirs[new_tir].clignote = TRUE;                   // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
              tirs[new_tir].spr.genre = ENNEMI;                // Indique que c'est un sprite ennemi.
              tirs[new_tir].spr.trajectoire = FALSE;           // Indique que la trajectoire du sprite est calculée une fois pour toute.
              tirs[new_tir].spr.nbr_img = 32;                  // Indique le nombre d'images composant le sprite.
              // Indique la puissance de destruction du sprite.
              tirs[new_tir].spr.pow_of_dest = 8;
              // Donne l'adresse des images du sprite.
              for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
              {                                                // Chargement du missile.
                tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P3E][j];        // Corespond au Tir 2 puissance 2.
              }
              tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;  // Indique l'état de domages du sprite.
              //tirs[new_tir].spr.pilote = 0;                    // Indique par qui est dirigé le sprite.
              //tirs[new_tir].spr.mode_commande = 0;             // Indique le moyen par lequel on fait déplacer le sprite.
              tirs[new_tir].spr.img_act = 0;                   // Indique l'image actuelle.
              tirs[new_tir].spr.nbr_tot_vbl = 1;               // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              tirs[new_tir].spr.nbr_vbl_act = 0;               // Indique le nombre de VBL comptées actuellement.
              // Donne les coordonnées d'affichage du sprite.
              tirs[new_tir].img_angle =
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                angle_tir[k];
              tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
              tirs[new_tir].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              tirs[new_tir].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              tirs[new_tir].tmp_life_tir = 400;
              // Donne un angle vertical pour le tir.
              tirs[new_tir].angle_tir =
                (float)(PI_SUR_16 * tirs[new_tir].img_angle);
              tirs[new_tir].spr.vitesse = 2.0;                 // Donne la vitesse de déplacement du sprite.
            }

            // Incrémente le compteur permettant d'ajouter un SOUKEE au gardien.
            cmpt_tir_gard_spec++;
            // Test si on doit rajouter un SOUKEE.
            if(cmpt_tir_gard_spec >= 16 && !game_over_player_one)
            {                                                  // On remet le compteur de SOUKEE à zéro.
              cmpt_tir_gard_spec = 0;
              // On rajoute un Vaisseau spécial SOUKEE dans la liste.
              new_ve = New_Element_Ve();                       // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
#ifdef USE_SDLMIXER
              seal_joue(TIR1);
#endif
              // Remplissage de la structure sprite appartenant au vaisseau ennemi.
              ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + 14);   // Indique la puissance de destruction du sprite.
              ve[new_ve].spr.etat_dommages = ve[new_ve].spr.pow_of_dest;        // Indique l'état de domages du sprite.
              ve[new_ve].spr.nbr_img = 32;                     // Indique le nombre d'images composant le sprite.
              ve[new_ve].spr.img_act = 8;                      // Indique l'image actuelle.
              ve[new_ve].spr.nbr_vbl_act = 0;                  // RAZ du compteur de VBL pour le passage à l'image suivante.
              ve[new_ve].spr.nbr_tot_vbl = 2;                  // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              // Donne l'adresse des images du sprite.
              for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
              {
                ve[new_ve].spr.img[k] = (image *) & enemi[SOUKEE][k];
              }
              ve[new_ve].cad_tir_initiale = 70;                // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
              ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale; // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
              ve[new_ve].deplacement = SPECIAUX;               // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
              // Donne les coordonées x et y de départ du vaisseau.
              ve[new_ve].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->x_cg -
                ve[new_ve].spr.img[0]->larg / 2;
              ve[new_ve].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->y_cg -
                ve[new_ve].spr.img[0]->haut / 2;
              ve[new_ve].spr.vitesse = 2.0f;                   // Donne la vitesse de déplacement du vaisseau ennemi spécial.
              ve[new_ve].type = SOUKEE;                        // Indique le type de vaisseau spécial.
              ve[new_ve].dead = FALSE;                         // Indique que l'ennemi est bien vivant.
              ve[new_ve].visible = TRUE;                       // Indique que l'ennemi est visible, on devra donc l'afficher.
              ve[new_ve].img_angle = 8;
              ve[new_ve].angle_tir = PI_SUR_2;
              ve[new_ve].img_old_angle = ve[new_ve].img_angle;
              ve[new_ve].agilite = 0.018f;                     //Indique l'agilite du vaisseau ennemi.
            }
          }
        }                                                      // Fin du test pour savoir si l'ennemi doit ou non tirer.
      }                                                        // Fin du test pour savoir si la pause est ou non activée.
    }
      break;

      //##############################################################################
      //guardian 6 run ###############################################################
      //##############################################################################
    case 6:
    {                                                          // Test si on est dans la phase d'apparition du gardien.
      if(apparition_gardian)
      {                                                        // Test si on est bien sur le gardien et non sur le bras articulé.
        if(!num_gardien)
        {                                                      // Routine utilisée pour faire l'apparition d'un gardien par le haut de l'écran.
          Fait_Apparaitre_Gardien_Haut(num_gardien);
          // Donne la nouvelle coordonée X du bras articulé.
          ve[1].spr.coor_x = ve[0].spr.coor_x;
          // Donne la nouvelle coordonée Y du bras articulé.
          ve[1].spr.coor_y = ve[0].spr.coor_y + 63;
        }
      }
      else                                                     // Sinon, on doit gérer le gardien.
      {                                                        // Test si la pause est activée ou pas.
        if(!player_pause && etat_menu == MENU_OFF && !num_gardien
           && _score::getActionExecution() == 0)
        {                                                      // Décompte du temps séparant le prochain déplacement du gardien.
          cmpt_tmps_dep_gardien--;
          // Test si l'on doit passer au prochain déplacement.
          if(cmpt_tmps_dep_gardien > 0)
          {                                                    // Test si le gardien est en déplacment actuellement.
            if(tab_traj_gardien[ptr_sur_tab_gardien] < 32)     // 32 étant la position de repos, tous les nombres en dessous étant des directions.
            {                                                  // Routine utilisée pour déplacer un gardien en suivant une droite.
              Move_Gardian_Line(num_gardien);
            }
            // Test le sens de déplacement du gardien.
            switch (tab_traj_gardien[ptr_sur_tab_gardien])
            {                                                  // Cas d'un déplacement vers la droite.
              case 0:
                // Test si le gardien vient de changer de direction.
                if(change_direction)
                {                                              // On réinitialise la position de l'image actuelle.
                  ve[num_gardien].spr.img_act = 15;
                  // RAZ du flag qui indique si un gardien vient de changer de direction.
                  change_direction = FALSE;
                  // Donne l'adresse des images du sprite.
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                  {
                    ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                  }
                }
                // Incrémente le compteur séparant le changement d'images.
                cmpt_vbl_trans++;
                // Test s'il est temps de changer d'image.
                if(cmpt_vbl_trans >= 4)
                {                                              // RAZ du compteur séparant le changement d'images.
                  cmpt_vbl_trans = 0;
                  // On incrémente l'image actuelle.
                  ve[num_gardien].spr.img_act++;
                  // Test si on est arrivé à l'image maximum.
                  if(ve[num_gardien].spr.img_act >= 31)
                    ve[num_gardien].spr.img_act = 31;
                }
                break;
                // Cas d'un déplacement vers la gauche.
              case 16:
                // Test si le gardien vient de changer de direction.
                if(change_direction)
                {                                              // On réinitialise la position de l'image actuelle.
                  ve[num_gardien].spr.img_act = 15;
                  // RAZ du flag qui indique si un gardien vient de changer de direction.
                  change_direction = FALSE;
                  // Donne l'adresse des images du sprite.
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                  {
                    ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                  }
                }
                // Incrémente le compteur séparant le changement d'images.
                cmpt_vbl_trans++;
                // Test s'il est temps de changer d'image.
                if(cmpt_vbl_trans >= 4)
                {                                              // RAZ du compteur séparant le changement d'images.
                  cmpt_vbl_trans = 0;
                  // On incrémente l'image actuelle.
                  ve[num_gardien].spr.img_act--;
                  // Test si on est arrivé à l'image maximum.
                  if(ve[num_gardien].spr.img_act <= 0)
                    ve[num_gardien].spr.img_act = 0;
                }
                break;
                // Cas de la position de repos.
              case 32:
                // Test si le gardien vient de changer de direction.
                if(change_direction)
                {                                              // RAZ du flag qui indique si un gardien vient de changer de direction.
                  change_direction = FALSE;
                  // Donne l'adresse des images du sprite.
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                  {
                    ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                  }
                  // Test si on était précédement sur une trajectoire verticale afin de se
                  // positionner sur la bonne image.
                  if(vertical_trajectoire)
                  {                                            // On se positionne sur la bonne image.
                    ve[num_gardien].spr.img_act = 15;
                    // On réinitialise le flag indiquant si on est sur une trajectoire verticale ou pas.
                    vertical_trajectoire = FALSE;
                  }
                }
                // Incrémente le compteur séparant le changement d'images.
                cmpt_vbl_trans++;
                // Test s'il est temps de changer d'image.
                if(cmpt_vbl_trans >= 3)
                {                                              // RAZ du compteur séparant le changement d'images.
                  cmpt_vbl_trans = 0;
                  // On met le vaisseau en position centrale.
                  if(ve[num_gardien].spr.img_act > 15)
                    ve[num_gardien].spr.img_act--;
                  if(ve[num_gardien].spr.img_act < 15)
                    ve[num_gardien].spr.img_act++;
                }
                break;
            }
          }
          else                                                 // Sinon, on doit passer au prochain déplacement.
          {                                                    // On passe au déplacement suivant.
            ptr_sur_tab_gardien++;
            // Test si on n'a pas dépasser le nombre maximum de déplacements.
            if(ptr_sur_tab_gardien >= nbr_dep_gardian_max)
            {                                                  // On indique qu'il faut faire disparaitre le gardien.
              ptr_sur_tab_gardien = 0;
            }
            change_direction = TRUE;                           // Indique que l'on vient de changer de direction.
            // Remet à jour le compteur de temps avant chaque déplacments du gardien.
            cmpt_tmps_dep_gardien =
              tab_tmps_traj_gardien[ptr_sur_tab_gardien];
          }
        }
        //Test si le bras articulé est toujours présent.
        if(ve[1].deplacement == GARDIEN)
        {                                                      // Donne la nouvelle coordonée X du bras articulé.
          ve[1].spr.coor_x = ve[0].spr.coor_x;
          // Donne la nouvelle coordonée Y du bras articulé.
          ve[1].spr.coor_y = ve[0].spr.coor_y + 63;
          // On incrémente l'image actuelle.
          ve[1].spr.nbr_vbl_act++;
          if(!(ve[1].spr.nbr_vbl_act &= (ve[1].spr.nbr_tot_vbl - 1)))
          {                                                    // On passe à l'image suivante.
            ve[1].spr.img_act++;
            ve[1].spr.img_act &= 31;
          }
        }
      }

      // Test si la pause est activée ou pas.
      if(!player_pause && etat_menu == MENU_OFF
         && _score::getActionExecution() == 0)
      {                                                        // Décompte le temps séparant deux tirs.
        ve[num_gardien].cad_tir--;
        // Test si l'ennemi doit ou non lancer un tir.
        if(ve[num_gardien].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
        {                                                      // Met la cadence du tir pour le vaisseau ennemi.
          ve[num_gardien].cad_tir = ve[num_gardien].cad_tir_initiale;
          // Bcle sur tous les points de tirs existants.
          for(k = 0;
              k <
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
              nbr_pts_tir; k++)
          {                                                    // Test s'il est possible d'ajouter un nouveau tir à la liste.
            if(nbr_tirs < (NBR_TIRS_MAX - 1))
            {                                                  // Recherche d'un nouveau tir.
              new_tir = New_Element_Tir();
#ifdef USE_SDLMIXER
              seal_joue(TIR3NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
              tirs[new_tir].clignote = TRUE;                   // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
              tirs[new_tir].spr.genre = ENNEMI;                // Indique que c'est un sprite ennemi.
              tirs[new_tir].spr.trajectoire = FALSE;           // Indique que la trajectoire du sprite est calculée une fois pour toute.
              tirs[new_tir].spr.nbr_img = 32;                  // Indique le nombre d'images composant le sprite.
              // Indique la puissance de destruction du sprite.
              tirs[new_tir].spr.pow_of_dest = 8;
              // Donne l'adresse des images du sprite.
              for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
              {                                                // Chargement du missile.
                tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P3E][j];        // Corespond au Tir 2 puissance 2.
              }
              tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;  // Indique l'état de domages du sprite.
              //tirs[new_tir].spr.pilote = 0;                    // Indique par qui est dirigé le sprite.
              //tirs[new_tir].spr.mode_commande = 0;             // Indique le moyen par lequel on fait déplacer le sprite.
              tirs[new_tir].spr.img_act = 0;                   // Indique l'image actuelle.
              tirs[new_tir].spr.nbr_tot_vbl = 1;               // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              tirs[new_tir].spr.nbr_vbl_act = 0;               // Indique le nombre de VBL comptées actuellement.
              // Donne les coordonnées d'affichage du sprite.
              tirs[new_tir].img_angle =
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                angle_tir[k];
              tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
              tirs[new_tir].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              tirs[new_tir].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              tirs[new_tir].tmp_life_tir = 400;
              // Donne un angle vertical pour le tir.
              tirs[new_tir].angle_tir =
                (float)(PI_SUR_16 * tirs[new_tir].img_angle);
              tirs[new_tir].spr.vitesse = 2.0;                 // Donne la vitesse de déplacement du sprite.
            }

            //gestion missile a tete chercheuse ..........................................
            cmpt_tir_gard_spec++;
            if(cmpt_tir_gard_spec >= 8 && !game_over_player_one)        //ajoute missile a tete chercheuse ?
            {
              cmpt_tir_gard_spec = 0;
              new_ve = New_Element_Ve();
#ifdef USE_SDLMIXER
              seal_joue(TIR1);                                 //joue un son avec seal
#endif
              // Remplissage de la structure sprite appartenant au vaisseau ennemi.
              ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + 5);    // Indique la puissance de destruction du sprite.
              ve[new_ve].spr.etat_dommages = ve[new_ve].spr.pow_of_dest >> 2;   // Indique l'état de domages du sprite.
              ve[new_ve].spr.nbr_img = 32;                     // Indique le nombre d'images composant le sprite.
              ve[new_ve].spr.img_act = 16;                     // Indique l'image actuelle.
              ve[new_ve].spr.nbr_vbl_act = 0;                  // RAZ du compteur de VBL pour le passage à l'image suivante.
              ve[new_ve].spr.nbr_tot_vbl = 2;                  // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              // Donne l'adresse des images du sprite.
              for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
              {
                ve[new_ve].spr.img[k] = (image *) & fire[MISSx4][k];
              }
              ve[new_ve].cad_tir_initiale = 70;                // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
              ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale; // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
              ve[new_ve].deplacement = SPECIAUX;               // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
              // Donne les coordonées x et y de départ du vaisseau.
              ve[new_ve].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[0][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              ve[new_ve].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[0][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              ve[new_ve].spr.vitesse = 2.0f;                   // Donne la vitesse de déplacement du vaisseau ennemi spécial.
              ve[new_ve].type = SOUKEE;                        // Indique le type de vaisseau spécial.
              ve[new_ve].dead = FALSE;                         // Indique que l'ennemi est bien vivant.
              ve[new_ve].visible = TRUE;                       // Indique que l'ennemi est visible, on devra donc l'afficher.
              ve[new_ve].img_angle = 16;
              ve[new_ve].angle_tir = PI;
              ve[new_ve].img_old_angle = ve[new_ve].img_angle;
              ve[new_ve].agilite = 0.028f;                     //Indique l'agilite du vaisseau ennemi.

              // On rajoute un Vaisseau spécial SOUKEE dans la liste.
              new_ve = New_Element_Ve();                       // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
#ifdef USE_SDLMIXER
              seal_joue(TIR4NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite appartenant au vaisseau ennemi.
              ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + 5);    // Indique la puissance de destruction du sprite.
              ve[new_ve].spr.etat_dommages = ve[new_ve].spr.pow_of_dest >> 2;   // Indique l'état de domages du sprite.
              ve[new_ve].spr.nbr_img = 32;                     // Indique le nombre d'images composant le sprite.
              ve[new_ve].spr.img_act = 0;                      // Indique l'image actuelle.
              ve[new_ve].spr.nbr_vbl_act = 0;                  // RAZ du compteur de VBL pour le passage à l'image suivante.
              ve[new_ve].spr.nbr_tot_vbl = 2;                  // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              // Donne l'adresse des images du sprite.
              for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
              {
                ve[new_ve].spr.img[k] = (image *) & fire[MISSx4][k];
              }
              ve[new_ve].cad_tir_initiale = 70;                // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
              ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale; // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
              ve[new_ve].deplacement = SPECIAUX;               // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
              // Donne les coordonées x et y de départ du vaisseau.
              ve[new_ve].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[1][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              ve[new_ve].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[1][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              ve[new_ve].spr.vitesse = 2.0f;                   // Donne la vitesse de déplacement du vaisseau ennemi spécial.
              ve[new_ve].type = SOUKEE;                        // Indique le type de vaisseau spécial.
              ve[new_ve].dead = FALSE;                         // Indique que l'ennemi est bien vivant.
              ve[new_ve].visible = TRUE;                       // Indique que l'ennemi est visible, on devra donc l'afficher.
              ve[new_ve].img_angle = 0;
              ve[new_ve].angle_tir = 0.0;
              ve[new_ve].img_old_angle = ve[new_ve].img_angle;
              ve[new_ve].agilite = 0.028f;                     //Indique l'agilite du vaisseau ennemi.
            }
          }
        }                                                      // Fin du test pour savoir si l'ennemi doit ou non tirer.
      }                                                        // Fin du test pour savoir si la pause est ou non activée.
    }
      break;                                                   // Fin de la gestion du sixième gardien.

      //##############################################################################
      //guardian 7 run ###############################################################
      //##############################################################################
    case 7:
    {
      if(apparition_gardian)                                   //apparition ?
      {
        Fait_Apparaitre_Gardien_Haut(num_gardien);             //oui, fait apparaitre le gardien en haut de l'ecran
      }
      else
      {
        cmpt_tmps_dep_gardien--;                               //decrementre compteur avant prochain deplacement
        if(cmpt_tmps_dep_gardien > 0)                          //change de deplacement ?
        {
          if(tab_traj_gardien[ptr_sur_tab_gardien] < 32)       //32=position repos, les autres valeurs sont des deplacements  
          {
            Move_Gardian_Line(num_gardien);                    //deplacement lineaire
          }
          switch (tab_traj_gardien[ptr_sur_tab_gardien])
          {                                                    //deplacement a droite .........................................................
            case 0:
              cmpt_vbl_trans++;                                //compteur temporisation
              if(cmpt_vbl_trans >= 3)                          //change d'image ?
              {
                cmpt_vbl_trans = 0;
                ve[num_gardien].spr.img_act++;                 //image suivante
                if(ve[num_gardien].spr.img_act > 31)
                  ve[num_gardien].spr.img_act = 0;             //image maximum ?
              }
              break;
              //deplacement a gauche .........................................................
            case 16:
              cmpt_vbl_trans++;                                //compteur temporisation
              if(cmpt_vbl_trans >= 3)                          //change d'image ?
              {
                cmpt_vbl_trans = 0;                            //RAZ compteur
                ve[num_gardien].spr.img_act++;                 //image suivante
                if(ve[num_gardien].spr.img_act > 31)
                  ve[num_gardien].spr.img_act = 0;             //image maximum ?
              }
              break;
              //position repos ...............................................................
            case 32:
              cmpt_vbl_trans++;                                //incremente compteur temporisation
              if(cmpt_vbl_trans >= 3)                          //change d'image ?
              {
                cmpt_vbl_trans = 0;                            //RAZ compteur
                ve[num_gardien].spr.img_act++;                 //image suivante
                if(ve[num_gardien].spr.img_act > 31)
                  ve[num_gardien].spr.img_act = 0;             //image maximum ?
              }
              break;
          }
        }
        else
          //change le deplacement ........................................................
        {
          ptr_sur_tab_gardien++;                               //next movement
          if(ptr_sur_tab_gardien >= nbr_dep_gardian_max)       //movement max ?
          {
            ptr_sur_tab_gardien = 0;
          }
          cmpt_tmps_dep_gardien = tab_tmps_traj_gardien[ptr_sur_tab_gardien];
        }
      }
      // Test si la pause est activée ou pas.
      if(!player_pause && etat_menu == MENU_OFF
         && _score::getActionExecution() == 0)
      {                                                        // Décompte le temps séparant deux tirs.
        ve[num_gardien].cad_tir--;
        // Test si l'ennemi doit ou non lancer un tir.
        if(ve[num_gardien].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
        {                                                      // Met la cadence du tir pour le vaisseau ennemi.
          ve[num_gardien].cad_tir = ve[num_gardien].cad_tir_initiale;
          // Bcle sur tous les points de tirs existants.
          for(k = 0;
              k <
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
              nbr_pts_tir; k++)
          {
            if(nbr_tirs < (NBR_TIRS_MAX - 1))                  //il reste des tirs ?
            {
              new_tir = New_Element_Tir();
#ifdef USE_SDLMIXER
              seal_joue(TIR3NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
              tirs[new_tir].clignote = TRUE;                   // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
              tirs[new_tir].spr.genre = ENNEMI;                // Indique que c'est un sprite ennemi.
              tirs[new_tir].spr.trajectoire = FALSE;           // Indique que la trajectoire du sprite est calculée une fois pour toute.
              tirs[new_tir].spr.nbr_img = 32;                  // Indique le nombre d'images composant le sprite.
              // Indique la puissance de destruction du sprite.
              tirs[new_tir].spr.pow_of_dest = 8;
              // Donne l'adresse des images du sprite.
              for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
              {                                                // Chargement du missile.
                tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P3E][j];        // Corespond au Tir 2 puissance 2.
              }
              tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;  // Indique l'état de domages du sprite.
              //tirs[new_tir].spr.pilote = 0;                    // Indique par qui est dirigé le sprite.
              //tirs[new_tir].spr.mode_commande = 0;             // Indique le moyen par lequel on fait déplacer le sprite.
              tirs[new_tir].spr.img_act = 0;                   // Indique l'image actuelle.
              tirs[new_tir].spr.nbr_tot_vbl = 1;               // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              tirs[new_tir].spr.nbr_vbl_act = 0;               // Indique le nombre de VBL comptées actuellement.
              // Donne les coordonnées d'affichage du sprite.
              tirs[new_tir].img_angle =
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                angle_tir[k];
              tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
              tirs[new_tir].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              tirs[new_tir].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              tirs[new_tir].tmp_life_tir = 400;
              // Donne un angle vertical pour le tir.
              tirs[new_tir].angle_tir =
                (float)(PI_SUR_16 * tirs[new_tir].img_angle);
              tirs[new_tir].spr.vitesse = 2.0;                 // Donne la vitesse de déplacement du sprite.
            }
          }

          // Incrémente le compteur permettant d'ajouter un vaisseau special au gardien.
          cmpt_tir_gard_spec++;
          // Test si on doit rajouter un vaisseau special.
          if(cmpt_tir_gard_spec >= 50)
          {                                                    // On remet le compteur de vaisseaux speciaux à zéro.
            cmpt_tir_gard_spec = 0;
            //On envoi un vaisseau special.
            if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
              Add_Ve_Special(-1);
          }
          // Incrémente le compteur permettant d'ajouter un QUIBOULY au gardien.
          compteur_quibouly++;
          // Test si on doit rajouter un QUIBOULY.
          if(compteur_quibouly >= 6)
          {                                                    // On remet le compteur de QUIBOULY à zéro.
            compteur_quibouly = 0;
            // On rajoute un Vaisseau spécial QUIBOULY dans la liste.
            new_ve = New_Element_Ve();                         // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
#ifdef USE_SDLMIXER
            seal_joue(TIR1);
#endif
            // Remplissage de la structure sprite appartenant au vaisseau ennemi.
            ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + 10);     // Indique la puissance de destruction du sprite.
            ve[new_ve].spr.etat_dommages = QUIBOULY + ve[new_ve].spr.pow_of_dest;       // Indique l'état de domages du sprite.
            ve[new_ve].spr.nbr_img = 32;                       // Indique le nombre d'images composant le sprite.
            ve[new_ve].spr.img_act = 0;                        // Indique l'image actuelle.
            ve[new_ve].spr.nbr_vbl_act = 0;                    // RAZ du compteur de VBL pour le passage à l'image suivante.
            ve[new_ve].spr.nbr_tot_vbl = 2;                    // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
            // Donne l'adresse des images du sprite.
            for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
            {
              ve[new_ve].spr.img[k] = (image *) & enemi[QUIBOULY][k];
            }
            ve[new_ve].cad_tir_initiale = 50;                  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
            ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;  // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
            ve[new_ve].deplacement = SPECIAUX;                 // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
            // Donne les coordonées x et y de départ du vaisseau.
            ve[new_ve].spr.coor_x =
              ve[num_gardien].spr.coor_x +
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->x_cg -
              ve[new_ve].spr.img[0]->larg / 2;
            ve[new_ve].spr.coor_y = ve[num_gardien].spr.coor_y + 96;
            ve[new_ve].spr.vitesse = 0.25f;                    // Donne la vitesse de déplacement du vaisseau ennemi spécial.
            ve[new_ve].type = QUIBOULY;                        // Indique le type de vaisseau spécial.
            ve[new_ve].dead = FALSE;                           // Indique que l'ennemi est bien vivant.
            ve[new_ve].visible = TRUE;                         // Indique que l'ennemi est visible, on devra donc l'afficher.
          }
        }                                                      // Fin du test pour savoir si l'ennemi doit ou non tirer.
      }                                                        // Fin du test pour savoir si la pause est ou non activée.
    }
      break;

      //guardian 8 run ###############################################################
    case 8:
    {                                                          // Test si on est dans la phase d'apparition du gardien.
      if(apparition_gardian)
      {                                                        // Routine utilisée pour faire l'apparition d'un gardien par le haut de l'écran.
        Fait_Apparaitre_Gardien_Haut(num_gardien);
      }
      else                                                     // Sinon, on doit gérer le gardien.
      {                                                        // Décompte du temps séparant le prochain déplacement du gardien.
        cmpt_tmps_dep_gardien--;
        // Test si l'on doit passer au prochain déplacement.
        if(cmpt_tmps_dep_gardien > 0)
        {                                                      // Test si le gardien est en déplacment actuellement.
          if(tab_traj_gardien[ptr_sur_tab_gardien] < 32)       // 32 étant la position de repos, tous les nombres en dessous étant des directions.
          {                                                    // Routine utilisée pour déplacer un gardien en suivant une droite.
            Move_Gardian_Line(num_gardien);
          }
          // Test le sens de déplacement du gardien.
          switch (tab_traj_gardien[ptr_sur_tab_gardien])
          {                                                    // Cas d'un déplacement vers la droite.
            case 0:
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 3)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act++;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act > 31)
                  ve[num_gardien].spr.img_act = 0;
              }
              break;
              // Cas d'un déplacement vers la gauche.
            case 16:
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 3)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act++;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act > 31)
                  ve[num_gardien].spr.img_act = 0;
              }
              break;
              // Cas de la position de repos.
            case 32:
              // Incrémente le compteur séparant le changement d'images.
              cmpt_vbl_trans++;
              // Test s'il est temps de changer d'image.
              if(cmpt_vbl_trans >= 3)
              {                                                // RAZ du compteur séparant le changement d'images.
                cmpt_vbl_trans = 0;
                // On incrémente l'image actuelle.
                ve[num_gardien].spr.img_act++;
                // Test si on est arrivé à l'image maximum.
                if(ve[num_gardien].spr.img_act > 31)
                  ve[num_gardien].spr.img_act = 0;
              }
              break;
          }
        }
        else                                                   // Sinon, on doit passer au prochain déplacement.
        {                                                      // On passe au déplacement suivant.
          ptr_sur_tab_gardien++;
          // Test si on n'a pas dépasser le nombre maximum de déplacements.
          if(ptr_sur_tab_gardien >= nbr_dep_gardian_max)
          {                                                    // On indique qu'il faut faire disparaitre le gardien.
            ptr_sur_tab_gardien = 0;
          }
          // Remet à jour le compteur de temps avant chaque déplacments du gardien.
          cmpt_tmps_dep_gardien = tab_tmps_traj_gardien[ptr_sur_tab_gardien];
        }
      }

      // Test si la pause est activée ou pas.
      if(!player_pause && etat_menu == MENU_OFF
         && _score::getActionExecution() == 0)
      {                                                        // Décompte le temps séparant deux tirs.
        ve[num_gardien].cad_tir--;
        // Test si l'ennemi doit ou non lancer un tir.
        if(ve[num_gardien].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
        {                                                      // Met la cadence du tir pour le vaisseau ennemi.
          ve[num_gardien].cad_tir = ve[num_gardien].cad_tir_initiale;
          // Bcle sur tous les points de tirs existants.
          for(k = 0;
              k <
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
              nbr_pts_tir; k++)
          {                                                    // Test s'il est possible d'ajouter un nouveau tir à la liste.
            if(nbr_tirs < (NBR_TIRS_MAX - 1))
            {                                                  // Recherche d'un nouveau tir.
              new_tir = New_Element_Tir();
#ifdef USE_SDLMIXER
              seal_joue(TIR3NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
              tirs[new_tir].clignote = TRUE;                   // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
              tirs[new_tir].spr.genre = ENNEMI;                // Indique que c'est un sprite ennemi.
              tirs[new_tir].spr.trajectoire = FALSE;           // Indique que la trajectoire du sprite est calculée une fois pour toute.
              tirs[new_tir].spr.nbr_img = 32;                  // Indique le nombre d'images composant le sprite.
              // Indique la puissance de destruction du sprite.
              tirs[new_tir].spr.pow_of_dest = 8;
              // Donne l'adresse des images du sprite.
              for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
              {                                                // Chargement du missile.
                tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P3E][j];        // Corespond au Tir 2 puissance 2.
              }
              tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;  // Indique l'état de domages du sprite.
              //tirs[new_tir].spr.pilote = 0;                    // Indique par qui est dirigé le sprite.
              //tirs[new_tir].spr.mode_commande = 0;             // Indique le moyen par lequel on fait déplacer le sprite.
              tirs[new_tir].spr.img_act = 0;                   // Indique l'image actuelle.
              tirs[new_tir].spr.nbr_tot_vbl = 1;               // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              tirs[new_tir].spr.nbr_vbl_act = 0;               // Indique le nombre de VBL comptées actuellement.
              // Donne les coordonnées d'affichage du sprite.
              tirs[new_tir].img_angle =
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                angle_tir[k];
              tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
              tirs[new_tir].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              tirs[new_tir].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              tirs[new_tir].tmp_life_tir = 400;
              // Donne un angle vertical pour le tir.
              tirs[new_tir].angle_tir =
                (float)(PI_SUR_16 * tirs[new_tir].img_angle);
              tirs[new_tir].spr.vitesse = 2.0;                 // Donne la vitesse de déplacement du sprite.
            }
          }

          // Incrémente le compteur permettant d'ajouter un vaisseau special au gardien.
          cmpt_tir_gard_spec++;
          // Test si on doit rajouter un vaisseau special.
          if(cmpt_tir_gard_spec >= 40)
          {                                                    // On remet le compteur de vaisseaux speciaux à zéro.
            cmpt_tir_gard_spec = 0;
            //On envoi un vaisseau special.
            if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
              Add_Ve_Special(-1);
          }
          // Incrémente le compteur permettant d'ajouter un TOURNADEE au gardien.
          compteur_tournadee++;
          // Test si on doit rajouter un TOURNADEE.
          if(compteur_tournadee >= 12)
          {                                                    // On remet le compteur de TOURNADEE à zéro.
            static int              var_tournadee;             // Utilisé pour savoir de quel cote de l'ecran lancer un TOURNADEE.

            compteur_tournadee = 0;
            // On rajoute un Vaisseau spécial TOURNADEE dans la liste.
            new_ve = New_Element_Ve();                         // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
#ifdef USE_SDLMIXER
            seal_joue(TIR1);
#endif
            // Remplissage de la structure sprite appartenant au vaisseau ennemi.
            ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + 10);     // Indique la puissance de destruction du sprite.
            ve[new_ve].spr.etat_dommages = TOURNADEE + ve[new_ve].spr.pow_of_dest;      // Indique l'état de domages du sprite.
            ve[new_ve].spr.nbr_img = 32;                       // Indique le nombre d'images composant le sprite.
            ve[new_ve].spr.img_act = 0;                        // Indique l'image actuelle.
            ve[new_ve].spr.nbr_vbl_act = 0;                    // RAZ du compteur de VBL pour le passage à l'image suivante.
            ve[new_ve].spr.nbr_tot_vbl = 3;                    // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
            // Donne l'adresse des images du sprite.
            for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
            {
              ve[new_ve].spr.img[k] = (image *) & enemi[TOURNADEE][k];
            }
            ve[new_ve].cad_tir_initiale = 20;                  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
            ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;  // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
            ve[new_ve].deplacement = SPECIAUX;                 // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
            // Donne les coordonées x et y de départ du vaisseau.
            if(var_tournadee)
            {
              var_tournadee = 0;
              ve[new_ve].spr.coor_x = DEBUT_ECRAN_X - 2;
            }
            else
            {
              var_tournadee = 1;
              ve[new_ve].spr.coor_x =
                (float)(DEBUT_ECRAN_X + LARG_ECR_RAM -
                        ve[new_ve].spr.img[0]->larg + 2);
            }
            ve[new_ve].spr.coor_y = DEBUT_ECRAN_Y + 32 + HAUT_ECR_RAM;
            ve[new_ve].spr.vitesse = -0.2f;                    // Donne la vitesse de déplacement du vaisseau ennemi spécial.
            ve[new_ve].type = TOURNADEE;                       // Indique le type de vaisseau spécial.
            ve[new_ve].dead = FALSE;                           // Indique que l'ennemi est bien vivant.
            ve[new_ve].visible = TRUE;                         // Indique que l'ennemi est visible, on devra donc l'afficher.
          }
        }                                                      // Fin du test pour savoir si l'ennemi doit ou non tirer.
      }                                                        // Fin du test pour savoir si la pause est ou non activée.
    }
      break;                                                   // Fin de la gestion du premier gardien.


      //##############################################################################
      //guardian 9 run ###############################################################
      //##############################################################################
    case 9:
    {
      if(apparition_gardian)                                   //gardien apparait ?
      {
        if(!num_gardien)
        {
          Fait_Apparaitre_Gardien_Haut(num_gardien);
          ve[1].spr.coor_x = ve[0].spr.coor_x;                 //abscisse du nez du vaisseau
          ve[1].spr.coor_y = ve[0].spr.coor_y;                 //ordonnee du nez du vaisseau
        }
      }
      else                                                     // Sinon, on doit gérer le gardien.
      {                                                        // Test si la pause est activée ou pas.
        if(!player_pause && etat_menu == MENU_OFF && !num_gardien
           && _score::getActionExecution() == 0)
        {                                                      // Décompte du temps séparant le prochain déplacement du gardien.
          cmpt_tmps_dep_gardien--;
          // Test si l'on doit passer au prochain déplacement.
          if(cmpt_tmps_dep_gardien > 0)
          {                                                    // Test si le gardien est en déplacment actuellement.
            if(tab_traj_gardien[ptr_sur_tab_gardien] < 32)     // 32 étant la position de repos, tous les nombres en dessous étant des directions.
            {                                                  // Routine utilisée pour déplacer un gardien en suivant une droite.
              Move_Gardian_Line(num_gardien);
            }
            // Test le sens de déplacement du gardien.
            switch (tab_traj_gardien[ptr_sur_tab_gardien])
            {                                                  // Cas d'un déplacement vers la droite.
              case 0:
                // Test si le gardien vient de changer de direction.
                if(change_direction)
                {                                              // On réinitialise la position de l'image actuelle.
                  ve[num_gardien].spr.img_act = 15;
                  // RAZ du flag qui indique si un gardien vient de changer de direction.
                  change_direction = FALSE;
                  // Donne l'adresse des images du sprite.
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                  {
                    ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                  }
                }
                // Incrémente le compteur séparant le changement d'images.
                cmpt_vbl_trans++;
                // Test s'il est temps de changer d'image.
                if(cmpt_vbl_trans >= 4)
                {                                              // RAZ du compteur séparant le changement d'images.
                  cmpt_vbl_trans = 0;
                  // On incrémente l'image actuelle.
                  ve[num_gardien].spr.img_act++;
                  // Test si on est arrivé à l'image maximum.
                  if(ve[num_gardien].spr.img_act >= 31)
                    ve[num_gardien].spr.img_act = 31;
                }
                break;
                // Cas d'un déplacement vers la gauche.
              case 16:
                // Test si le gardien vient de changer de direction.
                if(change_direction)
                {                                              // On réinitialise la position de l'image actuelle.
                  ve[num_gardien].spr.img_act = 15;
                  // RAZ du flag qui indique si un gardien vient de changer de direction.
                  change_direction = FALSE;
                  // Donne l'adresse des images du sprite.
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                  {
                    ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                  }
                }
                // Incrémente le compteur séparant le changement d'images.
                cmpt_vbl_trans++;
                // Test s'il est temps de changer d'image.
                if(cmpt_vbl_trans >= 4)
                {                                              // RAZ du compteur séparant le changement d'images.
                  cmpt_vbl_trans = 0;
                  // On incrémente l'image actuelle.
                  ve[num_gardien].spr.img_act--;
                  // Test si on est arrivé à l'image maximum.
                  if(ve[num_gardien].spr.img_act <= 0)
                    ve[num_gardien].spr.img_act = 0;
                }
                break;
                // Cas de la position de repos.
              case 32:
                // Test si le gardien vient de changer de direction.
                if(change_direction)
                {                                              // RAZ du flag qui indique si un gardien vient de changer de direction.
                  change_direction = FALSE;
                  // Donne l'adresse des images du sprite.
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                  {
                    ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                  }
                  // Test si on était précédement sur une trajectoire verticale afin de se
                  // positionner sur la bonne image.
                  if(vertical_trajectoire)
                  {                                            // On se positionne sur la bonne image.
                    ve[num_gardien].spr.img_act = 15;
                    // On réinitialise le flag indiquant si on est sur une trajectoire verticale ou pas.
                    vertical_trajectoire = FALSE;
                  }
                }
                // Incrémente le compteur séparant le changement d'images.
                cmpt_vbl_trans++;
                // Test s'il est temps de changer d'image.
                if(cmpt_vbl_trans >= 3)
                {                                              // RAZ du compteur séparant le changement d'images.
                  cmpt_vbl_trans = 0;
                  // On met le vaisseau en position centrale.
                  if(ve[num_gardien].spr.img_act > 15)
                    ve[num_gardien].spr.img_act--;
                  if(ve[num_gardien].spr.img_act < 15)
                    ve[num_gardien].spr.img_act++;
                }
                break;
            }
          }
          else                                                 // Sinon, on doit passer au prochain déplacement.
          {                                                    // On passe au déplacement suivant.
            ptr_sur_tab_gardien++;
            // Test si on n'a pas dépasser le nombre maximum de déplacements.
            if(ptr_sur_tab_gardien >= nbr_dep_gardian_max)
            {                                                  // On indique qu'il faut faire disparaitre le gardien.
              ptr_sur_tab_gardien = 0;
            }
            change_direction = TRUE;                           // Indique que l'on vient de changer de direction.
            // Remet à jour le compteur de temps avant chaque déplacments du gardien.
            cmpt_tmps_dep_gardien =
              tab_tmps_traj_gardien[ptr_sur_tab_gardien];
          }
        }
        if(ve[1].deplacement == GARDIEN)                       //nez du vaisseau toujours present ?
        {
          ve[1].spr.coor_x = ve[0].spr.coor_x;                 //abscisse du nez du vaisseau
          ve[1].spr.coor_y = ve[0].spr.coor_y;                 //ordonnee du nez du vaisseau +128 ;
          ve[1].spr.nbr_vbl_act++;                             //incremente compteur animation
          if(!(ve[1].spr.nbr_vbl_act &= (ve[1].spr.nbr_tot_vbl - 1)))
            ve[1].spr.img_act++;
          ve[1].spr.img_act &= 31;                             //prochaine image de l'animation
        }
      }

      // Test si la pause est activée ou pas.
      if(!player_pause && etat_menu == MENU_OFF
         && _score::getActionExecution() == 0)
      {                                                        // Décompte le temps séparant deux tirs.
        ve[num_gardien].cad_tir--;
        // Test si l'ennemi doit ou non lancer un tir.
        if(ve[num_gardien].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
        {                                                      // Met la cadence du tir pour le vaisseau ennemi.
          ve[num_gardien].cad_tir = ve[num_gardien].cad_tir_initiale;
          // Bcle sur tous les points de tirs existants.
          for(k = 0;
              k <
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
              nbr_pts_tir; k++)
          {                                                    // Test s'il est possible d'ajouter un nouveau tir à la liste.
            if(nbr_tirs < (NBR_TIRS_MAX - 1))
            {                                                  // Recherche d'un nouveau tir.
              new_tir = New_Element_Tir();
#ifdef USE_SDLMIXER
              seal_joue(TIR3NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
              tirs[new_tir].clignote = TRUE;                   // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
              tirs[new_tir].spr.genre = ENNEMI;                // Indique que c'est un sprite ennemi.
              tirs[new_tir].spr.trajectoire = FALSE;           // Indique que la trajectoire du sprite est calculée une fois pour toute.
              tirs[new_tir].spr.nbr_img = 32;                  // Indique le nombre d'images composant le sprite.
              // Indique la puissance de destruction du sprite.
              tirs[new_tir].spr.pow_of_dest = 8;
              // Donne l'adresse des images du sprite.
              for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
              {                                                // Chargement du missile.
                tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P3E][j];        // Corespond au Tir 2 puissance 2.
              }
              tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;  // Indique l'état de domages du sprite.
              //tirs[new_tir].spr.pilote = 0;                    // Indique par qui est dirigé le sprite.
              //tirs[new_tir].spr.mode_commande = 0;             // Indique le moyen par lequel on fait déplacer le sprite.
              tirs[new_tir].spr.img_act = 0;                   // Indique l'image actuelle.
              tirs[new_tir].spr.nbr_tot_vbl = 1;               // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              tirs[new_tir].spr.nbr_vbl_act = 0;               // Indique le nombre de VBL comptées actuellement.
              // Donne les coordonnées d'affichage du sprite.
              tirs[new_tir].img_angle =
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                angle_tir[k];
              tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
              tirs[new_tir].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              tirs[new_tir].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              tirs[new_tir].tmp_life_tir = 400;
              // Donne un angle vertical pour le tir.
              tirs[new_tir].angle_tir =
                (float)(PI_SUR_16 * tirs[new_tir].img_angle);
              tirs[new_tir].spr.vitesse = 2.0;                 // Donne la vitesse de déplacement du sprite.
            }

            cmpt_tir_gard_spec++;
            // Test si on doit rajouter un vaisseau special.
            if(cmpt_tir_gard_spec >= 40)
            {                                                  // On remet le compteur de vaisseaux speciaux à zéro.
              cmpt_tir_gard_spec = 0;
              //On envoi un vaisseau special.
              if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
                Add_Ve_Special(-1);
#ifdef USE_SDLMIXER
              seal_joue(TIR1);
#endif
            }
          }
        }                                                      // Fin du test pour savoir si l'ennemi doit ou non tirer.
      }                                                        // Fin du test pour savoir si la pause est ou non activée.
    }
      break;                                                   // Fin de la gestion du neuvième gardien.


      //##############################################################################
      //guardian 10 run ##############################################################
      //##############################################################################
    case 10:
    {                                                          // Test si on est dans la phase d'apparition du gardien.
      if(apparition_gardian)
      {                                                        // Test si on est bien sur le gardien et non sur le bras articulé.
        if(!num_gardien)
        {                                                      // Routine utilisée pour faire l'apparition d'un gardien par le haut de l'écran.
          Fait_Apparaitre_Gardien_Haut(num_gardien);
          // Donne la nouvelle coordonée X du bras articulé.
          ve[1].spr.coor_x = ve[0].spr.coor_x;
          // Donne la nouvelle coordonée Y du bras articulé.
          ve[1].spr.coor_y = ve[0].spr.coor_y;
        }
      }
      else                                                     // Sinon, on doit gérer le gardien.
      {                                                        // Test si la pause est activée ou pas.
        if(!player_pause && etat_menu == MENU_OFF && !num_gardien
           && _score::getActionExecution() == 0)
        {                                                      // Donne la nouvelle coordonée du gardien.

          if(send_dep_gard10 == DRTE)
            ve[num_gardien].spr.coor_x++;
          if(send_dep_gard10 == BAS)
            ve[num_gardien].spr.coor_y++;
          if(send_dep_gard10 == GOCH)
            ve[num_gardien].spr.coor_x--;
          if(send_dep_gard10 == HAUT)
            ve[num_gardien].spr.coor_y--;

          // Clip les coordonées du gardien.
          if(ve[num_gardien].spr.coor_x < (float)BANDE_DE_CLIP - clip_gard10)
          {
            ve[num_gardien].spr.coor_x = (float)(BANDE_DE_CLIP - clip_gard10);
            send_dep_gard10 = HAUT;
          }
          if((ve[num_gardien].spr.coor_x +
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->larg) >
             (float)(BANDE_DE_CLIP + LARG_ECR_RAM + clip_gard10))
          {
            ve[num_gardien].spr.coor_x =
              (float)(BANDE_DE_CLIP + LARG_ECR_RAM + clip_gard10) -
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->larg;
            send_dep_gard10 = BAS;
          }
          if(ve[num_gardien].spr.coor_y < (float)BANDE_DE_CLIP - clip_gard10)
          {
            ve[num_gardien].spr.coor_y = (float)(BANDE_DE_CLIP - clip_gard10);
            send_dep_gard10 = DRTE;
          }
          if((ve[num_gardien].spr.coor_y +
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->haut) >
             (float)(BANDE_DE_CLIP + HAUT_ECR_RAM + clip_gard10))
          {
            ve[num_gardien].spr.coor_y =
              (float)(BANDE_DE_CLIP + HAUT_ECR_RAM + clip_gard10) -
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->haut;
            send_dep_gard10 = GOCH;
          }
        }

        //Test si la seconde partie du gardien est présente.
        if(ve[1].deplacement == GARDIEN)
        {                                                      // Donne la nouvelle coordonée X du bras articulé.
          ve[1].spr.coor_x = ve[0].spr.coor_x;
          // Donne la nouvelle coordonée Y du bras articulé.
          ve[1].spr.coor_y = ve[0].spr.coor_y;

          // Incrémente le compteur séparant le changement d'images.
          ve[num_gardien].spr.nbr_vbl_act++;
          // Test s'il est temps de changer d'image.
          if(ve[num_gardien].spr.nbr_vbl_act >=
             ve[num_gardien].spr.nbr_tot_vbl)
          {                                                    // RAZ du compteur séparant le changement d'images.
            ve[num_gardien].spr.nbr_vbl_act = 0;
            // On incrémente l'image actuelle.
            ve[num_gardien].spr.img_act++;
            // Test si on est arrivé à l'image maximum.
            if(ve[num_gardien].spr.img_act > 31)
              ve[num_gardien].spr.img_act = 0;
          }

          //Deplacement du canon de maniere a suivre le vaisseau du joueur.
          if(ve[1].img_angle != ve[1].spr.img_act)
          {                                                    // Test si le sens de comptage de l'animation est décroissant.
            if(ve[1].sens_anim)
            {                                                  // Incrémente le compteur de VBLs pour l'affichage de l'animation.
              ve[1].spr.nbr_vbl_act++;
              // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
              if(ve[1].spr.nbr_vbl_act >= ve[1].spr.nbr_tot_vbl)
              {                                                // On initialise le compteur de VBLs.
                ve[1].spr.nbr_vbl_act = 0;
                // On passe à l'image suivante.
                ve[1].spr.img_act--;
                // Test si le numéro de l'image actuelle n'est pas inférieur à zéro.
                if(ve[1].spr.img_act < 0)
                {                                              // On repositionne l'image actuelle sur la première image de l'animation.
                  ve[1].spr.img_act = (short)(ve[1].spr.nbr_img - 1);
                }
              }
            }
            else                                               // Le sens de comptage des images de l'animation est croissant.
            {                                                  // Incrémente le compteur de VBLs pour l'affichage de l'animation.
              ve[1].spr.nbr_vbl_act++;
              // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
              if(ve[1].spr.nbr_vbl_act >= ve[1].spr.nbr_tot_vbl)
              {                                                // On initialise le compteur de VBLs.
                ve[1].spr.nbr_vbl_act = 0;
                // On passe à l'image suivante.
                ve[1].spr.img_act++;
                // Test si le numéro de l'image actuelle n'est pas supérieur au nombre d'images totales.
                if(ve[1].spr.img_act >= ve[1].spr.nbr_img)
                {                                              // On repositionne l'image actuelle sur la première image de l'animation.
                  ve[1].spr.img_act = 0;
                }
              }
            }
            // On cherche le sens dans lequel doit tourner le cannon.
            // Initialisation du compteur pour la recherche dans le sens croissant.
            compteur = ve[1].spr.img_act;
            first_value = 0;
            // Bcle sur toutes les positions d'angle possibles.
            for(compteur1 = 0; compteur1 < 32; compteur1++)
            {                                                  // Incrémentation de la première valeur.
              first_value++;
              compteur++;
              if(compteur > 31)
                compteur = 0;
              // Si le compteur à atteint la bonne valeur, on sort de la bcle.
              if(ve[1].img_angle == compteur)
                compteur1 = 32;
            }
            // Initialisation du compteur pour la recherche dans le sens décroissant.
            compteur = ve[1].spr.img_act;
            second_value = 0;
            // Bcle sur toutes les positions d'angle possibles.
            for(compteur1 = 0; compteur1 < 32; compteur1++)
            {                                                  // Incrémentation de la première valeur.
              second_value++;
              compteur--;
              if(compteur < 0)
                compteur = 31;
              // Si le compteur à atteint la bonne valeur, on sort de la bcle.
              if(ve[1].img_angle == compteur)
                compteur1 = 32;
            }
            // Si la première valeur est plus petite que la seconde, on part dans le sens croissant.
            if(first_value < second_value)
              ve[1].sens_anim = 0;
            else                                               // Sinon, le sens de comptage des animations du canon sera décroissant.
              ve[1].sens_anim = 1;
          }
          // Recherche du sens dans lequel doit tourner le canon.
          // Recherche de la position du canon par rapport à la direction du joueur humain.
          float                   angl_canon_vj =
            Calc_Angle_Tir((short)
                           (ve[1].spr.coor_x +
                            ve[1].spr.img[ve[1].spr.img_act]->x_cg),
                           (short)(ve[1].spr.coor_y +
                                   ve[1].spr.img[ve[1].spr.img_act]->y_cg),
                           (short)(vj[J1].spr.coor_x +
                                   vj[J1].spr.img[vj[J1].spr.img_act]->x_cg),
                           (short)(vj[J1].spr.coor_y +
                                   vj[J1].spr.img[vj[J1].spr.img_act]->y_cg));
          // Recherche de l'image à afficher en fonction de l'angle.
          if(sign(angl_canon_vj < 0))
            ve[1].img_angle = (short)((angl_canon_vj + DEUX_PI) / PI_SUR_16);
          else
            ve[1].img_angle = (short)(angl_canon_vj / PI_SUR_16);
          // Pour éviter un indice de tableau négatif.
          ve[1].img_angle = (short)abs(ve[1].img_angle);
          // Pour éviter un angle de tir supérieur au nombre d'images du sprite.
          if(ve[1].img_angle >= ve[1].spr.nbr_img)
            ve[1].img_angle = (short)(ve[1].spr.nbr_img - 1);

        }
      }

      // Test si la pause est activée ou pas.
      if(!player_pause && etat_menu == MENU_OFF
         && _score::getActionExecution() == 0)
      {                                                        // Décompte le temps séparant deux tirs.
        ve[num_gardien].cad_tir--;
        // Test si l'ennemi doit ou non lancer un tir.
        if(ve[num_gardien].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
        {                                                      // Met la cadence du tir pour le vaisseau ennemi.
          ve[num_gardien].cad_tir = ve[num_gardien].cad_tir_initiale;
          // Bcle sur tous les points de tirs existants.
          for(k = 0;
              k <
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
              nbr_pts_tir; k++)
          {                                                    // Test s'il est possible d'ajouter un nouveau tir à la liste.
            if(nbr_tirs < (NBR_TIRS_MAX - 1))
            {                                                  // Recherche d'un nouveau tir.
              new_tir = New_Element_Tir();
#ifdef USE_SDLMIXER
              seal_joue(TIR3NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
              tirs[new_tir].clignote = TRUE;                   // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
              tirs[new_tir].spr.genre = ENNEMI;                // Indique que c'est un sprite ennemi.
              tirs[new_tir].spr.trajectoire = FALSE;           // Indique que la trajectoire du sprite est calculée une fois pour toute.
              tirs[new_tir].spr.nbr_img = 32;                  // Indique le nombre d'images composant le sprite.
              // Indique la puissance de destruction du sprite.
              tirs[new_tir].spr.pow_of_dest = 8;
              // Donne l'adresse des images du sprite.
              for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
              {                                                // Chargement du missile.
                tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P3E][j];        // Corespond au Tir 2 puissance 2.
              }
              tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;  // Indique l'état de domages du sprite.
              //tirs[new_tir].spr.pilote = 0;                    // Indique par qui est dirigé le sprite.
              //tirs[new_tir].spr.mode_commande = 0;             // Indique le moyen par lequel on fait déplacer le sprite.
              tirs[new_tir].spr.img_act = 0;                   // Indique l'image actuelle.
              tirs[new_tir].spr.nbr_tot_vbl = 1;               // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              tirs[new_tir].spr.nbr_vbl_act = 0;               // Indique le nombre de VBL comptées actuellement.
              // Donne les coordonnées d'affichage du sprite.
              tirs[new_tir].img_angle =
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                angle_tir[k];
              tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
              tirs[new_tir].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              tirs[new_tir].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              tirs[new_tir].tmp_life_tir = 400;
              // Donne un angle vertical pour le tir.
              tirs[new_tir].angle_tir =
                (float)(PI_SUR_16 * tirs[new_tir].img_angle);
              tirs[new_tir].spr.vitesse = 2.0;                 // Donne la vitesse de déplacement du sprite.
            }

            cmpt_tir_gard_spec++;
            // Test si on doit rajouter un vaisseau special.
            if(cmpt_tir_gard_spec >= 40)
            {                                                  // On remet le compteur de vaisseaux speciaux à zéro.
              cmpt_tir_gard_spec = 0;
              //On envoi un vaisseau special.
              if(nbr_nmis < (MAX_NMI_ON_SCREEN - 2))
                Add_Ve_Special(-1);
#ifdef USE_SDLMIXER
              seal_joue(TIR1);
#endif
            }
          }
        }                                                      // Fin du test pour savoir si l'ennemi doit ou non tirer.
      }                                                        // Fin du test pour savoir si la pause est ou non activée.
    }
      break;                                                   // Fin de la gestion du neuvième gardien.



      //##############################################################################
      //guardian 11 run ##############################################################
      //##############################################################################
    case 11:
    {                                                          // Test si on est dans la phase d'apparition du gardien.
      if(apparition_gardian)
      {                                                        // Test si on est bien sur le gardien et non sur le bras articulé.
        if(!num_gardien)
        {                                                      // Routine utilisée pour faire l'apparition d'un gardien par le haut de l'écran.
          Fait_Apparaitre_Gardien_Haut(num_gardien);
        }
      }
      else                                                     // Sinon, on doit gérer le gardien.
      {                                                        // Test si la pause est activée ou pas.
        if(!player_pause && etat_menu == MENU_OFF && !num_gardien
           && _score::getActionExecution() == 0)
        {                                                      // Décompte du temps séparant le prochain déplacement du gardien.
          cmpt_tmps_dep_gardien--;
          // Test si l'on doit passer au prochain déplacement.
          if(cmpt_tmps_dep_gardien > 0)
          {                                                    // Test si le gardien est en déplacment actuellement.
            if(tab_traj_gardien[ptr_sur_tab_gardien] < 32)     // 32 étant la position de repos, tous les nombres en dessous étant des directions.
            {                                                  // Routine utilisée pour déplacer un gardien en suivant une droite.
              Move_Gardian_Line(num_gardien);
            }
            // Test le sens de déplacement du gardien.
            switch (tab_traj_gardien[ptr_sur_tab_gardien])
            {                                                  // Cas d'un déplacement vers la droite.
              case 0:
                // Test si le gardien vient de changer de direction.
                if(change_direction)
                {                                              // On réinitialise la position de l'image actuelle.
                  ve[num_gardien].spr.img_act = 15;
                  // RAZ du flag qui indique si un gardien vient de changer de direction.
                  change_direction = FALSE;
                  // Donne l'adresse des images du sprite.
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                  {
                    ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                  }
                }
                // Incrémente le compteur séparant le changement d'images.
                cmpt_vbl_trans++;
                // Test s'il est temps de changer d'image.
                if(cmpt_vbl_trans >= 4)
                {                                              // RAZ du compteur séparant le changement d'images.
                  cmpt_vbl_trans = 0;
                  // On incrémente l'image actuelle.
                  ve[num_gardien].spr.img_act++;
                  // Test si on est arrivé à l'image maximum.
                  if(ve[num_gardien].spr.img_act >= 31)
                    ve[num_gardien].spr.img_act = 31;
                }
                break;
                // Cas d'un déplacement vers la gauche.
              case 16:
                // Test si le gardien vient de changer de direction.
                if(change_direction)
                {                                              // On réinitialise la position de l'image actuelle.
                  ve[num_gardien].spr.img_act = 15;
                  // RAZ du flag qui indique si un gardien vient de changer de direction.
                  change_direction = FALSE;
                  // Donne l'adresse des images du sprite.
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                  {
                    ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                  }
                }
                // Incrémente le compteur séparant le changement d'images.
                cmpt_vbl_trans++;
                // Test s'il est temps de changer d'image.
                if(cmpt_vbl_trans >= 4)
                {                                              // RAZ du compteur séparant le changement d'images.
                  cmpt_vbl_trans = 0;
                  // On incrémente l'image actuelle.
                  ve[num_gardien].spr.img_act--;
                  // Test si on est arrivé à l'image maximum.
                  if(ve[num_gardien].spr.img_act <= 0)
                    ve[num_gardien].spr.img_act = 0;
                }
                break;
                // Cas de la position de repos.
              case 32:
                // Test si le gardien vient de changer de direction.
                if(change_direction)
                {                                              // RAZ du flag qui indique si un gardien vient de changer de direction.
                  change_direction = FALSE;
                  // Donne l'adresse des images du sprite.
                  for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
                  {
                    ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
                  }
                  // Test si on était précédement sur une trajectoire verticale afin de se
                  // positionner sur la bonne image.
                  if(vertical_trajectoire)
                  {                                            // On se positionne sur la bonne image.
                    ve[num_gardien].spr.img_act = 15;
                    // On réinitialise le flag indiquant si on est sur une trajectoire verticale ou pas.
                    vertical_trajectoire = FALSE;
                  }
                }
                // Incrémente le compteur séparant le changement d'images.
                cmpt_vbl_trans++;
                // Test s'il est temps de changer d'image.
                if(cmpt_vbl_trans >= 3)
                {                                              // RAZ du compteur séparant le changement d'images.
                  cmpt_vbl_trans = 0;
                  // On met le vaisseau en position centrale.
                  if(ve[num_gardien].spr.img_act > 15)
                    ve[num_gardien].spr.img_act--;
                  if(ve[num_gardien].spr.img_act < 15)
                    ve[num_gardien].spr.img_act++;
                }
                break;
            }
          }
          else                                                 // Sinon, on doit passer au prochain déplacement.
          {                                                    // On passe au déplacement suivant.
            ptr_sur_tab_gardien++;
            // Test si on n'a pas dépasser le nombre maximum de déplacements.
            if(ptr_sur_tab_gardien >= nbr_dep_gardian_max)
            {                                                  // On indique qu'il faut faire disparaitre le gardien.
              ptr_sur_tab_gardien = 0;
            }
            change_direction = TRUE;                           // Indique que l'on vient de changer de direction.
            // Remet à jour le compteur de temps avant chaque déplacments du gardien.
            cmpt_tmps_dep_gardien =
              tab_tmps_traj_gardien[ptr_sur_tab_gardien];
          }
        }
      }

      // Test si la pause est activée ou pas.
      if(!player_pause && etat_menu == MENU_OFF
         && _score::getActionExecution() == 0)
      {                                                        // Décompte le temps séparant deux tirs.
        ve[num_gardien].cad_tir--;
        // Test si l'ennemi doit ou non lancer un tir.
        if(ve[num_gardien].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
        {                                                      // Met la cadence du tir pour le vaisseau ennemi.
          ve[num_gardien].cad_tir = ve[num_gardien].cad_tir_initiale;
          // Bcle sur tous les points de tirs existants.
          for(k = 0;
              k <
              ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
              nbr_pts_tir; k++)
          {                                                    // Test s'il est possible d'ajouter un nouveau tir à la liste.
            if(nbr_tirs < (NBR_TIRS_MAX - 1))
            {                                                  // Recherche d'un nouveau tir.
              new_tir = New_Element_Tir();
#ifdef USE_SDLMIXER
              seal_joue(TIR3NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
              tirs[new_tir].clignote = TRUE;                   // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
              tirs[new_tir].spr.genre = ENNEMI;                // Indique que c'est un sprite ennemi.
              tirs[new_tir].spr.trajectoire = FALSE;           // Indique que la trajectoire du sprite est calculée une fois pour toute.
              tirs[new_tir].spr.nbr_img = 32;                  // Indique le nombre d'images composant le sprite.
              // Indique la puissance de destruction du sprite.
              tirs[new_tir].spr.pow_of_dest = 8;
              // Donne l'adresse des images du sprite.
              for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
              {                                                // Chargement du missile.
                tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P3E][j];        // Corespond au Tir 2 puissance 2.
              }
              tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;  // Indique l'état de domages du sprite.
              //tirs[new_tir].spr.pilote = 0;                    // Indique par qui est dirigé le sprite.
              //tirs[new_tir].spr.mode_commande = 0;             // Indique le moyen par lequel on fait déplacer le sprite.
              tirs[new_tir].spr.img_act = 0;                   // Indique l'image actuelle.
              tirs[new_tir].spr.nbr_tot_vbl = 1;               // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              tirs[new_tir].spr.nbr_vbl_act = 0;               // Indique le nombre de VBL comptées actuellement.
              // Donne les coordonnées d'affichage du sprite.
              tirs[new_tir].img_angle =
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                angle_tir[k];
              tirs[new_tir].img_old_angle = tirs[new_tir].img_angle;
              tirs[new_tir].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              tirs[new_tir].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[k][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              tirs[new_tir].tmp_life_tir = 400;
              // Donne un angle vertical pour le tir.
              tirs[new_tir].angle_tir =
                (float)(PI_SUR_16 * tirs[new_tir].img_angle);
              tirs[new_tir].spr.vitesse = 2.0;                 // Donne la vitesse de déplacement du sprite.
            }

            // Incrémente le compteur permettant d'ajouter un SOUKEE au gardien.
            cmpt_tir_gard_spec++;
            // Test si on doit rajouter un SOUKEE.
            if(cmpt_tir_gard_spec >= 8 && !game_over_player_one)
            {                                                  // On remet le compteur de SOUKEE à zéro.
              cmpt_tir_gard_spec = 0;
              // On rajoute un Vaisseau spécial SOUKEE dans la liste.
              new_ve = New_Element_Ve();                       // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
#ifdef USE_SDLMIXER
              seal_joue(TIR1);
#endif
              // Remplissage de la structure sprite appartenant au vaisseau ennemi.
              ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + 5);    // Indique la puissance de destruction du sprite.
              ve[new_ve].spr.etat_dommages = ve[new_ve].spr.pow_of_dest >> 2;   // Indique l'état de domages du sprite.
              ve[new_ve].spr.nbr_img = 32;                     // Indique le nombre d'images composant le sprite.
              ve[new_ve].spr.img_act = 16;                     // Indique l'image actuelle.
              ve[new_ve].spr.nbr_vbl_act = 0;                  // RAZ du compteur de VBL pour le passage à l'image suivante.
              ve[new_ve].spr.nbr_tot_vbl = 2;                  // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              // Donne l'adresse des images du sprite.
              for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
              {
                ve[new_ve].spr.img[k] = (image *) & fire[MISSx4][k];
              }
              ve[new_ve].cad_tir_initiale = 70;                // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
              ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale; // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
              ve[new_ve].deplacement = SPECIAUX;               // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
              // Donne les coordonées x et y de départ du vaisseau.
              ve[new_ve].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[0][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              ve[new_ve].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[0][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              ve[new_ve].spr.vitesse = 2.0f;                   // Donne la vitesse de déplacement du vaisseau ennemi spécial.
              ve[new_ve].type = SOUKEE;                        // Indique le type de vaisseau spécial.
              ve[new_ve].dead = FALSE;                         // Indique que l'ennemi est bien vivant.
              ve[new_ve].visible = TRUE;                       // Indique que l'ennemi est visible, on devra donc l'afficher.
              ve[new_ve].img_angle = 16;
              ve[new_ve].angle_tir = PI;
              ve[new_ve].img_old_angle = ve[new_ve].img_angle;
              ve[new_ve].agilite = 0.028f;                     //Indique l'agilite du vaisseau ennemi.

              // On rajoute un Vaisseau spécial SOUKEE dans la liste.
              new_ve = New_Element_Ve();                       // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
#ifdef USE_SDLMIXER
              seal_joue(TIR4NMI);                              //joue un son avec seal
#endif
              // Remplissage de la structure sprite appartenant au vaisseau ennemi.
              ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + 5);    // Indique la puissance de destruction du sprite.
              ve[new_ve].spr.etat_dommages = ve[new_ve].spr.pow_of_dest >> 2;   // Indique l'état de domages du sprite.
              ve[new_ve].spr.nbr_img = 32;                     // Indique le nombre d'images composant le sprite.
              ve[new_ve].spr.img_act = 0;                      // Indique l'image actuelle.
              ve[new_ve].spr.nbr_vbl_act = 0;                  // RAZ du compteur de VBL pour le passage à l'image suivante.
              ve[new_ve].spr.nbr_tot_vbl = 2;                  // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
              // Donne l'adresse des images du sprite.
              for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
              {
                ve[new_ve].spr.img[k] = (image *) & fire[MISSx4][k];
              }
              ve[new_ve].cad_tir_initiale = 70;                // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
              ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale; // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
              ve[new_ve].deplacement = SPECIAUX;               // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
              // Donne les coordonées x et y de départ du vaisseau.
              ve[new_ve].spr.coor_x =
                ve[num_gardien].spr.coor_x +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[1][X] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->x_cg;
              ve[new_ve].spr.coor_y =
                ve[num_gardien].spr.coor_y +
                ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
                xy_pos_tir[1][Y] -
                tirs[new_tir].spr.img[tirs[new_tir].img_angle]->y_cg;
              ve[new_ve].spr.vitesse = 2.0f;                   // Donne la vitesse de déplacement du vaisseau ennemi spécial.
              ve[new_ve].type = SOUKEE;                        // Indique le type de vaisseau spécial.
              ve[new_ve].dead = FALSE;                         // Indique que l'ennemi est bien vivant.
              ve[new_ve].visible = TRUE;                       // Indique que l'ennemi est visible, on devra donc l'afficher.
              ve[new_ve].img_angle = 0;
              ve[new_ve].angle_tir = 0.0;
              ve[new_ve].img_old_angle = ve[new_ve].img_angle;
              ve[new_ve].agilite = 0.028f;                     //Indique l'agilite du vaisseau ennemi.
            }
          }
        }                                                      // Fin du test pour savoir si l'ennemi doit ou non tirer.
      }                                                        // Fin du test pour savoir si la pause est ou non activée.
    }
      break;

      //##############################################################################
      //guardians 12, 13, 14 and congratulations #####################################
      //##############################################################################
    case 15:
      _execute_congra(num_gardien);
      break;
    case 14:
      _execute_gardien14(num_gardien);
      break;
    case 13:
      _execute_gardien13(num_gardien);
      break;
    case 12:
      _execute_gardien12(num_gardien);
      break;
  }

  //partie commune ###############################################################
  if(apparition_gardian)                                       //apparition du gardien ?
  {
    if(!num_gardien)
    {                                                          //affichage du gardien une fois sur deux.
      if(aff_gard_1_sur_2)
      {                                                        //affichage de deux vaisseaux
        if(gardian_activity == 6 || 		   
		   gardian_activity == 9 ||
		   gardian_activity == 10)
        {
          Aff_Ve2(0);
          Aff_Ve2(1);
        }
        else
          Aff_Ve2(num_gardien);
        aff_gard_1_sur_2 = 0;
      }
      else
        aff_gard_1_sur_2 = 1;
    }
  }
  else
  {
    Test_Col_Bou_With_Nmi(num_gardien);                        //test collisions with balls
    Test_Col_Can_With_Nmi(num_gardien);                        //test collisions with gun's ship
    if(!vj[J1].invincible)
      Test_Col_Player_With_Spec_Nmi(num_gardien);              //test collisions with player's ship
    Aff_Ve2(num_gardien);                                      //display guardian
  }
}

//------------------------------------------------------------------------------
// guardian loading data in memory
// input => numero_gardien : guardian 1 to 15
//------------------------------------------------------------------------------
int Load_Gardien(int numero_gardien)
{
#ifdef _VERBEUX_
  if(power_conf->verbose > 0)
    fprintf(stdout,
      "> gardiens.cpp/Load_Gardien(%i)\n", numero_gardien);
#endif
  Free_Mem_Gardian();
  switch (numero_gardien)
  { case 1:
      if(!etb_read_image
         ("graphics/gardi_0.spr", &gardi[0][0], 1, NBR_IMAGES_NMI_S))
        return 0;
      break;
    case 2:
      if(!etb_read_image
         ("graphics/gardi_1.spr", &gardi[0][0], 2, NBR_IMAGES_NMI_S))
        return 0;
      break;
    case 3:
      if(!etb_read_image
         ("graphics/gardi_2.spr", &gardi[0][0], 1, NBR_IMAGES_NMI_S))
        return 0;
      break;
    case 4:
      if(!etb_read_image
         ("graphics/gardi_3.spr", &gardi[0][0], 2, NBR_IMAGES_NMI_S))
        return 0;
      break;
    case 5:
      if(!etb_read_image
         ("graphics/gardi_4.spr", &gardi[0][0], 2, NBR_IMAGES_NMI_S))
        return 0;
      break;
    case 6:
      if(!etb_read_image
         ("graphics/gardi_5.spr", &gardi[0][0], 2, NBR_IMAGES_NMI_S))
        return 0;
      break;                                                   //2 elements
    case 7:
      if(!etb_read_image
         ("graphics/gardi_6.spr", &gardi[0][0], 1, NBR_IMAGES_NMI_S))
        return 0;
      break;
    case 8:
      if(!etb_read_image
         ("graphics/gardi_7.spr", &gardi[0][0], 1, NBR_IMAGES_NMI_S))
        return 0;
      break;
    case 9:
      if(!etb_read_image
         ("graphics/gardi_8.spr", &gardi[0][0], 2, NBR_IMAGES_NMI_S))
        return 0;
      break;                                                   //2 elements
    case 10:
      if(!etb_read_image
         ("graphics/gardi_9.spr", &gardi[0][0], 2, NBR_IMAGES_NMI_S))
        return 0;
      break;                                                   //2 elements
    case 11:
      if(!etb_read_image
         ("graphics/gardi_10.spr", &gardi[0][0], 1, NBR_IMAGES_NMI_S))
        return 0;
      break;
    case 12:
      if(!etb_read_image
         ("graphics/gardi_11.spr", &gardi[0][0], 2, NBR_IMAGES_NMI_S))
        return 0;
      break;
    case 13:
      if(!etb_read_image
         ("graphics/gardi_12.spr", &gardi[0][0], 2, NBR_IMAGES_NMI_S))
        return 0;
      break;
    case 14:
      if(!etb_read_image
         ("graphics/gardi_13.spr", &gardi[0][0], 2, NBR_IMAGES_NMI_S))
        return 0;
      break;
  }
  return 1;
}

//------------------------------------------------------------------------------
// Test des collisions entre le joueur et l'ennemi spécial.
//------------------------------------------------------------------------------
void Test_Col_Player_With_Spec_Nmi(int nmi_numero)
{
  int                     k, l;
  int                     tmp_tsts_x, tmp_tsts_y, tmp_tstd_x, tmp_tstd_y;

  // Si le joueur n'est pas Game Over.
  if(!aff_game_over)
  {                                                            // Test si l'ennemi est en collision avec le joueur.
    // Bcle sur tous les points de collisions du vaisseau du joueur.
    for(k = 0; k < vj[J1].spr.img[vj[J1].spr.img_act]->nbr_pts_col; k++)
    {                                                          // Donne les coordonées du point de collision du vaisseau appartenant au joueur.
      tmp_tsts_x =
        (int)vj[J1].spr.coor_x +
        vj[J1].spr.img[vj[J1].spr.img_act]->xy_pts_col[k][X];
      tmp_tsts_y =
        (int)vj[J1].spr.coor_y +
        vj[J1].spr.img[vj[J1].spr.img_act]->xy_pts_col[k][Y];
      // Bcle sur toutes les zones de collisions du vaisseau ennemi.
      for(l = 0;
          l < ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->nbr_zon_col;
          l++)
      {                                                        // Donne les coordonées de la zone de collision du vaisseau ennemi.
        tmp_tstd_x =
          (int)ve[nmi_numero].spr.coor_x +
          ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
          xy_zon_col[l][X];
        tmp_tstd_y =
          (int)ve[nmi_numero].spr.coor_y +
          ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
          xy_zon_col[l][Y];
        // Test si le point de collision du vaisseau du joueur se trouve à l'intérieur de la
        // zone de collision du vaisseau ennemi.
        if(tmp_tsts_x >= tmp_tstd_x &&
           tmp_tsts_y >= tmp_tstd_y &&
           tmp_tsts_x <
           tmp_tstd_x +
           ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
           lh_zon_col[l][L]
           && tmp_tsts_y <
           tmp_tstd_y +
           ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
           lh_zon_col[l][H])
        {                                                      // On doit décrémenter l'état des dommages des deux vaisseaux.
          // Donne le nouvel état des dommages du vaisseau ennemi.
          ve[nmi_numero].spr.etat_dommages -= (vj[J1].spr.pow_of_dest << 1);
          // Test si le vaisseau du joueur est invincible dans le but de décendre son énergie.
          if(!vj[J1].invincible)
          {                                                    // Test si l'énergie du joueur était au maximum.
            if(vj[J1].spr.etat_dommages == vj[J1].spr.pow_of_dest)
            {                                                  // Indique que l'animation d'ouverture de l'option d'energie doit s'effectuer.
              Init_Anim_Option(1, FALSE);
            }
            // Donne le nouvel état des dommages du vaisseau du joueur.
            vj[J1].spr.etat_dommages -= ve[nmi_numero].spr.pow_of_dest;
          }
          // Demande un réaffichage de la barre d'énergie du joueur.
          bar_nrj_player = TRUE;
          // Test si le vaisseau du joueur est détruit.
          if(vj[J1].spr.etat_dommages <= 0)
          {                                                    // Indique que le joueur est Game Over.
            game_over_player_one = TRUE;
          }
          else
          {                                                    // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
            vj[J1].affich_white = TRUE;
          }
          // On ajoute la vitesse de déplacement du vaisseau ennemi à celle du vaisseau ami.
          vj[J1].vitesse_x += ve[nmi_numero].vitesse_x;
          vj[J1].vitesse_y += ve[nmi_numero].vitesse_y;

          // On test si le vaisseau ennemi est détruit du fait d'une collision avec le joueur.
          if(ve[nmi_numero].spr.etat_dommages <= 0)
          {                                                    // Routine utilisée pour ajouter une série d'explosion à un gros vaisseau.
            Add_Serie_Of_Explosion(nmi_numero);
            // On ajoute des eclats à la liste des explosions.
            Add_Eclats(ve[nmi_numero].spr.coor_x +
                       ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
                       x_cg - 8,
                       ve[nmi_numero].spr.coor_y +
                       ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
                       y_cg - 8, 0.5, 8, 0, 3);
            Add_Eclats(ve[nmi_numero].spr.coor_x +
                       ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
                       x_cg - 8,
                       ve[nmi_numero].spr.coor_y +
                       ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
                       y_cg - 8, 1.0, 8, 0, 2);
            Add_Eclats(ve[nmi_numero].spr.coor_x +
                       ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
                       x_cg - 8,
                       ve[nmi_numero].spr.coor_y +
                       ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
                       y_cg - 8, 1.5, 8, 0, 1);
            // Mise à jour du score du joueur pour la destruction d'un ennemi
            player_score += ve[nmi_numero].spr.pow_of_dest << 2 << bonusx;
            // On fait clignoter l'ennemi actuel avant de le faire disparaitre.
            Clignote_Nmi_Avant_Disparition(nmi_numero);
          }
          else
          {                                                    // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
            ve[nmi_numero].affich_white = TRUE;
          }
          //On test si on est en presence d'un gardien afin de mettre a jour sa barre d'energie.
          if(ve[nmi_numero].deplacement == GARDIEN)
            bar_nrj_guardian = TRUE;
          // Le vaisseau du joueur est entré en collision avec le vaisseau ennemi, on peux sortir du test.
          goto fin_tst_col_player_with_spec_nmi;
        }
      }
    }                                                          // Fin du test pour savoir si le joueur est en collision avec un vaisseau ennemi.
  fin_tst_col_player_with_spec_nmi:;
  }
}

//------------------------------------------------------------------------------
// Test des collisions entre les boules du joueur et l'ennemi.
//------------------------------------------------------------------------------
void Test_Col_Bou_With_Nmi(int nmi_numero)
{
  int                     k, l, m;
  int                     tmp_tsts_x, tmp_tsts_y, tmp_tstd_x, tmp_tstd_y;

  // Si le joueur n'est pas Game Over.
  if(!aff_game_over)
  {                                                            // Test les collisions entre les boules et le vaisseau ennemi.
    // Boucles sur toutes les boules du joueur.
    for(k = 0; k < nbr_boules; k++)
    {                                                          // Retourne l'indice dans le tableau 'bou' du nième élément de la liste.
      cmpt_boules = liste_bou[k];
      // Test si la boule est visible et si oui, on peux tester les collisions.
      if(bou[cmpt_boules].visible)
      {                                                        // Boucle sur tous les points de collision de la boule.
        for(l = 0;
            l < bou[cmpt_boules].img[bou[cmpt_boules].img_act]->nbr_pts_col;
            l++)
        {                                                      // Donne les coordonnée du point de collision actuel de la boule.
          tmp_tsts_x =
            bou[cmpt_boules].coor_x +
            bou[cmpt_boules].img[bou[cmpt_boules].img_act]->xy_pts_col[l][X];
          tmp_tsts_y =
            bou[cmpt_boules].coor_y +
            bou[cmpt_boules].img[bou[cmpt_boules].img_act]->xy_pts_col[l][Y];
          // Boucle sur toutes les zones de collisions du vaisseau ennemi.
          for(m = 0;
              m <
              ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->nbr_zon_col;
              m++)
          {                                                    // Test si le point de collision de la boule se trouve à l'intérieur de la
            // zone de collision du vaisseau ennemi.
            tmp_tstd_x =
              (int)ve[nmi_numero].spr.coor_x +
              ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
              xy_zon_col[m][X];
            tmp_tstd_y =
              (int)ve[nmi_numero].spr.coor_y +
              ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
              xy_zon_col[m][Y];
            // Test s'il le point de collision de la boule se trouve à l'intérieur de la zone de collision du vaisseau ennemi.
            if(tmp_tsts_x >= tmp_tstd_x &&
               tmp_tsts_y >= tmp_tstd_y &&
               tmp_tsts_x <
               (tmp_tstd_x +
                ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
                lh_zon_col[m][L])
               && tmp_tsts_y <
               (tmp_tstd_y +
                ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
                lh_zon_col[m][H]))
            {                                                  // On doit décrémenter l'état des dommages du vaisseau ennemi et de la boule.
              // Donne le nouvel état des dommages du vaisseau ennemi.
              ve[nmi_numero].spr.etat_dommages -=
                bou[cmpt_boules].pow_of_dest;
              // Donne le nouvel état des dommages de la boule du joueur.
              bou[cmpt_boules].etat_dommages -=
                ve[nmi_numero].spr.pow_of_dest;
              // Test si la boule est détruite.
              if(bou[cmpt_boules].etat_dommages <= 0)
              {                                                // On supprime la boule de la liste.
                Del_Element_Bou(cmpt_boules, k);               // Routine utilisée pour supprimer une boule du tableau 'bou'.
                // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                k--;
                // Positionnement des boules autour du vaisseau appartenant au joueur.
                if(nbr_boules)
                {
                  bou[liste_bou[0]].pos_pts_cercle = 0;        // Indique la position du pointeur sur le tableau des points du cercle.
                  switch (nbr_boules)
                  {
                    case 2:
                      bou[liste_bou[1]].pos_pts_cercle = 40;   // Indique la position du pointeur sur le tableau des points du cercle.
                      break;
                    case 3:
                      bou[liste_bou[1]].pos_pts_cercle = 26;   // Indique la position du pointeur sur le tableau des points du cercle.
                      bou[liste_bou[2]].pos_pts_cercle = 53;   // Indique la position du pointeur sur le tableau des points du cercle.
                      break;
                    case 4:
                      bou[liste_bou[1]].pos_pts_cercle = 20;   // Indique la position du pointeur sur le tableau des points du cercle.
                      bou[liste_bou[2]].pos_pts_cercle = 40;   // Indique la position du pointeur sur le tableau des points du cercle.
                      bou[liste_bou[3]].pos_pts_cercle = 60;   // Indique la position du pointeur sur le tableau des points du cercle.
                      break;
                    case 5:
                      bou[liste_bou[1]].pos_pts_cercle = 16;   // Indique la position du pointeur sur le tableau des points du cercle.
                      bou[liste_bou[2]].pos_pts_cercle = 32;   // Indique la position du pointeur sur le tableau des points du cercle.
                      bou[liste_bou[3]].pos_pts_cercle = 48;   // Indique la position du pointeur sur le tableau des points du cercle.
                      bou[liste_bou[4]].pos_pts_cercle = 64;   // Indique la position du pointeur sur le tableau des points du cercle.
                      break;
                  }
                }
              }                                                // Fin du test pour savoir si la boule est en détruite.
              else
              {                                                // Si la boule n'est pas détruite, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                bou[cmpt_boules].affich_white = TRUE;
              }
              // On test si le vaisseau ennemi est détruit.
              if(ve[nmi_numero].spr.etat_dommages <= 0)
              {                                                // Test si le vaisseau ennemi était une météorite.
                if(ve[nmi_numero].type >= BIGMETEOR)
                {                                              // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                  //Gene_Bonus_Malus_Meteor(ve[nmi_numero].spr.coor_x, ve[nmi_numero].spr.coor_y);
                  Gene_Bonus_Malus_Meteor(&ve[nmi_numero]);
                }
                else                                           // Sinon, on appel la routine normale de génération de bonus, malus.
                {                                              // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                  //Gene_Bonus_Malus(ve[nmi_numero].spr.coor_x, ve[nmi_numero].spr.coor_y);
                  Gene_Bonus_Malus(&ve[nmi_numero]);
                }
                // Routine utilisée pour ajouter une série d'explosion à un gros vaisseau.
                Add_Serie_Of_Explosion(nmi_numero);
                // On ajoute des eclats à la liste des explosions.
                Add_Eclats(ve[nmi_numero].spr.coor_x +
                           ve[nmi_numero].spr.img[ve[nmi_numero].spr.
                                                  img_act]->x_cg - 8,
                           ve[nmi_numero].spr.coor_y +
                           ve[nmi_numero].spr.img[ve[nmi_numero].spr.
                                                  img_act]->y_cg - 8, 0.5, 8,
                           0, 3);
                Add_Eclats(ve[nmi_numero].spr.coor_x +
                           ve[nmi_numero].spr.img[ve[nmi_numero].spr.
                                                  img_act]->x_cg - 8,
                           ve[nmi_numero].spr.coor_y +
                           ve[nmi_numero].spr.img[ve[nmi_numero].spr.
                                                  img_act]->y_cg - 8, 1.0, 8,
                           0, 2);
                Add_Eclats(ve[nmi_numero].spr.coor_x +
                           ve[nmi_numero].spr.img[ve[nmi_numero].spr.
                                                  img_act]->x_cg - 8,
                           ve[nmi_numero].spr.coor_y +
                           ve[nmi_numero].spr.img[ve[nmi_numero].spr.
                                                  img_act]->y_cg - 8, 1.5, 8,
                           0, 1);
                // Mise à jour du score du joueur pour la destruction d'un ennemi
                player_score += ve[nmi_numero].spr.pow_of_dest << 2 << bonusx;
                // On fait clignoter l'ennemi actuel avant de le faire disparaitre.
                Clignote_Nmi_Avant_Disparition(nmi_numero);
                // La boule est en collision avec l'ennemi, on peux passer à la boule suivante.
                goto fin_test_col_bou_nmi_act;
              }
              else
              {                                                // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                ve[nmi_numero].affich_white = TRUE;
              }
              // On ajoute une mini explosion à la liste car une boule à eu une collision.
              if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                Add_Explosion((float)tmp_tsts_x, (float)tmp_tsts_y, 0.3f,
                              MINI_EXPLOSION, 0);
              // La boule est en collision avec l'ennemi, on peux passer à la boule suivante.
              goto fin_test_col_bou_nmi_act;
            }
          }
        }
      }
    fin_test_col_bou_nmi_act:;
    }
  }
}

//------------------------------------------------------------------------------
// Test des collisions entre les canons du joueur et l'ennemi.
//------------------------------------------------------------------------------
void Test_Col_Can_With_Nmi(int nmi_numero)
{
  int k, l, m;
  int tmp_tsts_x, tmp_tsts_y, tmp_tstd_x, tmp_tstd_y;

  // Si le joueur n'est pas Game Over.
  if(!aff_game_over)
  {                                                            // Test les collisions entre les canons et le vaisseau ennemi.
    // Bcles sur tous les canons du joueur.
    for(k = 0; k < nbr_canons; k++)
    {                                                          // Retourne l'indice dans le tableau 'can' du nième élément de la liste.
      cmpt_canons = liste_can[k];
      // Test si le canon est visible et si oui, on peux tester les collisions.
      if(can[cmpt_canons].visible)
      {                                                        // Bcle sur tous les points de collision du canon.
        for(l = 0;
            l < can[cmpt_canons].img[can[cmpt_canons].img_act]->nbr_pts_col;
            l++)
        {                                                      // Donne les coordonnée du point de collision actuel de le canon.
          tmp_tsts_x =
            can[cmpt_canons].coor_x +
            can[cmpt_canons].img[can[cmpt_canons].img_act]->xy_pts_col[l][X];
          tmp_tsts_y =
            can[cmpt_canons].coor_y +
            can[cmpt_canons].img[can[cmpt_canons].img_act]->xy_pts_col[l][Y];
          // Bcle sur toutes les zones de collisions du vaisseau ennemi.
          for(m = 0;
              m <
              ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->nbr_zon_col;
              m++)
          {                                                    // Donne les coordonées du point supérieur gauche de la
            // zone de collision du vaisseau ennemi.
            tmp_tstd_x =
              (int)ve[nmi_numero].spr.coor_x +
              ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
              xy_zon_col[m][X];
            tmp_tstd_y =
              (int)ve[nmi_numero].spr.coor_y +
              ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
              xy_zon_col[m][Y];
            // Test s'il le point de collision du canon se trouve à l'intérieur de la zone de collision du vaisseau ennemi.
            if(tmp_tsts_x >= tmp_tstd_x &&
               tmp_tsts_y >= tmp_tstd_y &&
               tmp_tsts_x <
               (tmp_tstd_x +
                ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
                lh_zon_col[m][L])
               && tmp_tsts_y <
               (tmp_tstd_y +
                ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act]->
                lh_zon_col[m][H]))
            {                                                  // On doit décrémenter l'état des dommages du vaisseau ennemi et du canon.
              // Donne le nouvel état des dommages du vaisseau ennemi.
              ve[nmi_numero].spr.etat_dommages -=
                can[cmpt_canons].pow_of_dest;
              // Donne le nouvel état des dommages du canon.
              can[cmpt_canons].etat_dommages -=
                ve[nmi_numero].spr.pow_of_dest;
              // Test si le canon est détruite.
              if(can[cmpt_canons].etat_dommages <= 0)
              {                                                // On supprime le canon de la liste.
                Del_Element_Can(cmpt_canons, k);               // Routine utilisée pour supprimer un canon du tableau 'can'.
                // On se positionne sur le vaisseau ennemi que l'on vient de décaler.
                k--;
              }                                                // Fin du test pour savoir si le canon est en détruite.
              else
              {                                                // Si le canon n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                can[cmpt_canons].affich_white = TRUE;
              }
              // On test si le vaisseau ennemi est détruit.
              if(ve[nmi_numero].spr.etat_dommages <= 0)
              {                                                // Test si le vaisseau ennemi était une météorite.
                if(ve[nmi_numero].type >= BIGMETEOR)
                {                                              // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                  //Gene_Bonus_Malus_Meteor(ve[nmi_numero].spr.coor_x, ve[nmi_numero].spr.coor_y);
                  Gene_Bonus_Malus_Meteor(&ve[nmi_numero]);
                }
                else                                           // Sinon, on appel la routine normale de génération de bonus, malus.
                {                                              // Appel de la routine utilisée pour générer soit un bonus soit un malus.
                  //Gene_Bonus_Malus(ve[nmi_numero].spr.coor_x, ve[nmi_numero].spr.coor_y);
                  Gene_Bonus_Malus(&ve[nmi_numero]);
                }
                // Routine utilisée pour ajouter une série d'explosion à un gros vaisseau.
                Add_Serie_Of_Explosion(nmi_numero);
                // On ajoute des eclats à la liste des explosions.
                Add_Eclats(ve[nmi_numero].spr.coor_x +
                           ve[nmi_numero].spr.img[ve[nmi_numero].spr.
                                                  img_act]->x_cg - 8,
                           ve[nmi_numero].spr.coor_y +
                           ve[nmi_numero].spr.img[ve[nmi_numero].spr.
                                                  img_act]->y_cg - 8, 0.5, 8,
                           0, 3);
                Add_Eclats(ve[nmi_numero].spr.coor_x +
                           ve[nmi_numero].spr.img[ve[nmi_numero].spr.
                                                  img_act]->x_cg - 8,
                           ve[nmi_numero].spr.coor_y +
                           ve[nmi_numero].spr.img[ve[nmi_numero].spr.
                                                  img_act]->y_cg - 8, 1.0, 8,
                           0, 2);
                Add_Eclats(ve[nmi_numero].spr.coor_x +
                           ve[nmi_numero].spr.img[ve[nmi_numero].spr.
                                                  img_act]->x_cg - 8,
                           ve[nmi_numero].spr.coor_y +
                           ve[nmi_numero].spr.img[ve[nmi_numero].spr.
                                                  img_act]->y_cg - 8, 1.5, 8,
                           0, 1);
                // Mise à jour du score du joueur pour la destruction d'un ennemi
                player_score += ve[nmi_numero].spr.pow_of_dest << 2 << bonusx;
                // On fait clignoter l'ennemi actuel avant de le faire disparaitre.
                Clignote_Nmi_Avant_Disparition(nmi_numero);
                // Le canon est en collision avec l'ennemi, on peux passer au canon suivant.
                goto fin_test_col_can_nmi_act;
              }
              else
              {                                                // Si le vaisseau n'est pas détruit, on indique qu'il faudra l'afficher la prochaine fois en blanc.
                ve[nmi_numero].affich_white = TRUE;
              }
              // On ajoute une mini explosion à la liste car un canon à eu une collision.
              if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))
                Add_Explosion((float)tmp_tsts_x, (float)tmp_tsts_y, 0.3f,
                              MINI_EXPLOSION, 0);
              // Le canon est en collision avec l'ennemi, on peux passer au canon suivant.
              goto fin_test_col_can_nmi_act;
            }
          }
        }
      }
    fin_test_col_can_nmi_act:;
    }
  }
}

//------------------------------------------------------------------------------
// gestion de l'"invincibilite" d'un vaisseau ennemi
// Retourne Vrai si le vaisseau ennemi est détruit, Faux si on continu à le faire clignoter.
//------------------------------------------------------------------------------
bool Gere_Invincibilite_Ve(int numero_nmi)
{                                                              // Test si le vaisseau ennemi est visible.
  if(ve[numero_nmi].visible_dead)
  {                                                            // On rend le vaisseau ennemi invisible.
    ve[numero_nmi].visible_dead = FALSE;
    // Affichage du vaisseau ennemi dans une couleur sombre avant sa disparition totale.
    //putsprcolor(color_dead[ve[numero_nmi].cmpt_color_dead],ecran_ram+(short)ve[numero_nmi].spr.coor_y*512+(short)ve[numero_nmi].spr.coor_x,ve[numero_nmi].spr.img[ve[numero_nmi].spr.img_act]->compress,(short)(ve[numero_nmi].spr.img[ve[numero_nmi].spr.img_act]->nbr_data_comp>>2));
    putsprcolor(color_dead[ve[numero_nmi].cmpt_color_dead],
                ve[numero_nmi].spr.img[ve[numero_nmi].spr.img_act],
                (int)(ve[numero_nmi].spr.coor_x),
                (int)(ve[numero_nmi].spr.coor_y));
    // Passage à la couleur suivante.
    ve[numero_nmi].cmpt_color_dead++;
    // Test si la couleur d'affichage n'est pas dépassée.
    if(ve[numero_nmi].cmpt_color_dead >= NBR_COLOR_DEAD)
      ve[numero_nmi].cmpt_color_dead = NBR_COLOR_DEAD - 1;
  }
  else
  {
    ve[numero_nmi].visible_dead = 1;
    ve[numero_nmi].invincible--;                               //decremente compteur invincibilite
    if(ve[numero_nmi].invincible <= 0)                         //ennemi est toujours invincible ?
    {
      ve[numero_nmi].visible_dead = 1;                         //1=ennemi encore visible mais inactif
      return 1;                                                //1=supprime le vaisseau ennemi definitivement.
    }
  }
  return 0;                                                    //0=ennemi ne doit pas encore etre supprime
}

//------------------------------------------------------------------------------
// liberation de la memoire utilisee par les gardiens
//------------------------------------------------------------------------------
void Free_Mem_Gardian(void)
{
  int i, j;
  for(i = 0; i < NBR_ANIM_GARDIAN; i++)
  { for(j = 0; j < NBR_IMAGES_NMI_S; j++)
    { if(gardi[i][j].img)
      { libereMemoire(gardi[i][j].img);
        gardi[i][j].img = 0;
      }
      if(gardi[i][j].compress)
      { libereMemoire(gardi[i][j].compress);
        gardi[i][j].compress = 0;
      }
    }
  }
}

// Routine qui rempli le tableau des déplacements du gardien de manière
// à le faire se déplacer vers la gauche.
void Fill_Tab_Dep_Gard_Gauche(void)
{                                                              // Remplissage du tableau des trajectoires, du temps et la vitesse de déplacement du gardien.
  tab_traj_gardien[nbr_dep_gardian_max] = 32;                  // Indique que l'on attends.
  tab_tmps_traj_gardien[nbr_dep_gardian_max] = 100;            // Tableau des temps avant chaque déplacmements du gardien.
  tab_vit_dep_gardien[nbr_dep_gardian_max] = 0;                // Tableau des vitesses de déplacement du gardien.
  nbr_dep_gardian_max++;                                       // Indique le nombre de déplacements effectués par le gardien.
  tab_traj_gardien[nbr_dep_gardian_max] = 16;                  // Indique que l'on se déplace vers la gauche.
  tab_tmps_traj_gardien[nbr_dep_gardian_max] = 400;            // Tableau des temps avant chaque déplacmements du gardien.
  tab_vit_dep_gardien[nbr_dep_gardian_max] = 1;                // Tableau des vitesses de déplacement du gardien.
  nbr_dep_gardian_max++;                                       // Indique le nombre de déplacements effectués par le gardien.
}

// Routine qui rempli le tableau des déplacements du gardien de manière
// à le faire se déplacer vers la droite.
void Fill_Tab_Dep_Gard_Droite(void)
{                                                              // Remplissage du tableau des trajectoires, du temps et la vitesse de déplacement du gardien.
  tab_traj_gardien[nbr_dep_gardian_max] = 32;                  // Indique que l'on attends.
  tab_tmps_traj_gardien[nbr_dep_gardian_max] = 100;            // Tableau des temps avant chaque déplacmements du gardien.
  tab_vit_dep_gardien[nbr_dep_gardian_max] = 0;                // Tableau des vitesses de déplacement du gardien.
  nbr_dep_gardian_max++;                                       // Indique le nombre de déplacements effectués par le gardien.
  tab_traj_gardien[nbr_dep_gardian_max] = 0;                   // Indique que l'on se déplace vers la droite.
  tab_tmps_traj_gardien[nbr_dep_gardian_max] = 400;            // Tableau des temps avant chaque déplacmements du gardien.
  tab_vit_dep_gardien[nbr_dep_gardian_max] = 1;                // Tableau des vitesses de déplacement du gardien.
  nbr_dep_gardian_max++;                                       // Indique le nombre de déplacements effectués par le gardien.
}

// Routine qui rempli le tableau des déplacements du gardien de manière
// à le faire se déplacer vers le haut.
void Fill_Tab_Dep_Gard_Haut(void)
{                                                              // Remplissage du tableau des trajectoires, du temps et la vitesse de déplacement du gardien.
  tab_traj_gardien[nbr_dep_gardian_max] = 32;                  // Indique que l'on attends.
  tab_tmps_traj_gardien[nbr_dep_gardian_max] = 100;            // Tableau des temps avant chaque déplacmements du gardien.
  tab_vit_dep_gardien[nbr_dep_gardian_max] = 0;                // Tableau des vitesses de déplacement du gardien.
  nbr_dep_gardian_max++;                                       // Indique le nombre de déplacements effectués par le gardien.
  tab_traj_gardien[nbr_dep_gardian_max] = 24;                  // Indique que l'on se déplace vers le haut.
  tab_tmps_traj_gardien[nbr_dep_gardian_max] = 400;            // Tableau des temps avant chaque déplacmements du gardien.
  tab_vit_dep_gardien[nbr_dep_gardian_max] = 3;                // Tableau des vitesses de déplacement du gardien.
  nbr_dep_gardian_max++;                                       // Indique le nombre de déplacements effectués par le gardien.
}

// Routine qui rempli le tableau des déplacements du gardien de manière
// à le faire se déplacer vers le bas.
void Fill_Tab_Dep_Gard_Bas(void)
{                                                              // Remplissage du tableau des trajectoires, du temps et la vitesse de déplacement du gardien.
  tab_traj_gardien[nbr_dep_gardian_max] = 32;                  // Indique que l'on attends.
  tab_tmps_traj_gardien[nbr_dep_gardian_max] = 100;            // Tableau des temps avant chaque déplacmements du gardien.
  tab_vit_dep_gardien[nbr_dep_gardian_max] = 0;                // Tableau des vitesses de déplacement du gardien.
  nbr_dep_gardian_max++;                                       // Indique le nombre de déplacements effectués par le gardien.
  tab_traj_gardien[nbr_dep_gardian_max] = 8;                   // Indique que l'on se déplace vers le bas.
  tab_tmps_traj_gardien[nbr_dep_gardian_max] = 400;            // Tableau des temps avant chaque déplacmements du gardien.
  tab_vit_dep_gardien[nbr_dep_gardian_max] = 3;                // Tableau des vitesses de déplacement du gardien.
  nbr_dep_gardian_max++;                                       // Indique le nombre de déplacements effectués par le gardien.
}

// Routine utilisée pour faire l'apparition d'un gardien par le haut de l'écran.
void Fait_Apparaitre_Gardien_Haut(int num_gardien)
{                                                              // On fait avancer le gardien jusqu'au début de l'écran.
  ve[num_gardien].spr.coor_y += ve[num_gardien].spr.vitesse;
  // Test si la positon du gardien est arrivée au début de l'écran.
  if(ve[num_gardien].spr.coor_y >= BANDE_DE_CLIP)
  {                                                            // Clip la position du gardien.
    ve[num_gardien].spr.coor_y = (float)BANDE_DE_CLIP;
    // Indique que la phase d'appariton du gardien est terminée.
    apparition_gardian = FALSE;
    // Initialisation du pointeur sur le tableau qui permet les déplacements du gardien.
    ptr_sur_tab_gardien = 0;
    // Initialisation du compteur temps avant le prochain déplacement du gardien.
    cmpt_tmps_dep_gardien = tab_tmps_traj_gardien[ptr_sur_tab_gardien];
  }
}

// Routine utilisée pour déplacer un gardien en suivant une droite.
void Move_Gardian_Line(int num_gardien)
{                                                              // Test si la pause est activée ou pas.
  if(!player_pause && etat_menu == MENU_OFF
     && _score::getActionExecution() == 0)
  {                                                            // Donne la nouvelle coordonée X du gardien à trajectoire fixe.
    ve[num_gardien].spr.coor_x +=
      depix[tab_vit_dep_gardien[ptr_sur_tab_gardien]][tab_traj_gardien
                                                      [ptr_sur_tab_gardien]];
    // Donne la nouvelle coordonée Y du gardien à trajectoire fixe.
    ve[num_gardien].spr.coor_y +=
      depiy[tab_vit_dep_gardien[ptr_sur_tab_gardien]][tab_traj_gardien
                                                      [ptr_sur_tab_gardien]];

    //Traitement particulier du gardien N°6.
    if(gardian_activity == 6)
    {                                                          // Clip les coordonées du gardien.
      if(ve[num_gardien].spr.coor_x < (float)BANDE_DE_CLIP - 16)
      {
        ve[num_gardien].spr.coor_x = (float)BANDE_DE_CLIP - 16;
        cmpt_tmps_dep_gardien = 0;
        // Donne la nouvelle coordonée Y du gardien à trajectoire fixe.
        ve[num_gardien].spr.coor_y++;
        if(ve[num_gardien].spr.coor_y > (float)(BANDE_DE_CLIP + 40))
          ve[num_gardien].spr.coor_y = (float)(BANDE_DE_CLIP + 40);
      }
      if((ve[num_gardien].spr.coor_x +
          ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->larg) >
         (float)(BANDE_DE_CLIP + LARG_ECR_RAM + 48))
      {
        ve[num_gardien].spr.coor_x =
          (float)(BANDE_DE_CLIP + LARG_ECR_RAM) -
          ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->larg + 48;
        cmpt_tmps_dep_gardien = 0;
        // Donne la nouvelle coordonée Y du gardien à trajectoire fixe.
        ve[num_gardien].spr.coor_y++;
        if(ve[num_gardien].spr.coor_y > (float)(BANDE_DE_CLIP + 40))
          ve[num_gardien].spr.coor_y = (float)(BANDE_DE_CLIP + 40);
      }
    }
    else                                                       //Déplacement utilisé dans tous les autres cas de gardiens.
    {                                                          // Clip les coordonées du gardien.
      if(ve[num_gardien].spr.coor_x < (float)BANDE_DE_CLIP)
      {
        ve[num_gardien].spr.coor_x = (float)BANDE_DE_CLIP;
        cmpt_tmps_dep_gardien = 0;
      }
      if((ve[num_gardien].spr.coor_x +
          ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->larg) >
         (float)(BANDE_DE_CLIP + LARG_ECR_RAM))
      {
        ve[num_gardien].spr.coor_x =
          (float)(BANDE_DE_CLIP + LARG_ECR_RAM) -
          ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->larg;
        cmpt_tmps_dep_gardien = 0;
      }
    }

    if(ve[num_gardien].spr.coor_y < (float)BANDE_DE_CLIP)
    {
      ve[num_gardien].spr.coor_y = (float)BANDE_DE_CLIP;
      cmpt_tmps_dep_gardien = 0;
    }
    if((ve[num_gardien].spr.coor_y +
        ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->haut) >
       (float)(BANDE_DE_CLIP + HAUT_ECR_RAM))
    {
      ve[num_gardien].spr.coor_y =
        (float)(BANDE_DE_CLIP + HAUT_ECR_RAM) -
        ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->haut;
      cmpt_tmps_dep_gardien = 0;
    }
  }
}

// Routine utilisée pour déplacer un gardien en suivant un sinus.
void Move_Gardian_Sin(int num_gardien)
{                                                              // Test si la pause est activée ou pas.
  if(!player_pause && etat_menu == MENU_OFF
     && _score::getActionExecution() == 0)
  {                                                            // Donne la nouvelle coordonée X du gardien à trajectoire fixe.
    ve[num_gardien].spr.coor_x +=
      depix[tab_vit_dep_gardien[ptr_sur_tab_gardien]][tab_traj_gardien
                                                      [ptr_sur_tab_gardien]];

    // nouvelle ordonnée du gardien en fct de son abscisse
    ve[num_gardien].spr.coor_y += pre_sin[cmpt_tmps_dep_gardien & 0x001f] * 4;
    //Interdit au gardien de décendre plus bas qu'une certaine hauteur.
    //Ceci afin d'éviter qu'il ne traversse la RAM.
    if(ve[num_gardien].spr.coor_y > (float)(BANDE_DE_CLIP + 40))
      ve[num_gardien].spr.coor_y = (float)(BANDE_DE_CLIP + 40);

    // Clip les coordonées du gardien.
    if(ve[num_gardien].spr.coor_x < (float)BANDE_DE_CLIP)
    {
      ve[num_gardien].spr.coor_x = (float)BANDE_DE_CLIP;
      cmpt_tmps_dep_gardien = 0;
    }

    if((ve[num_gardien].spr.coor_x +
        ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->larg) >
       (float)(BANDE_DE_CLIP + LARG_ECR_RAM))
    {
      ve[num_gardien].spr.coor_x =
        (float)(BANDE_DE_CLIP + LARG_ECR_RAM) -
        ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->larg;
      cmpt_tmps_dep_gardien = 0;
    }
  }
}

// Routine qui indique au vaisseau ennemi de clignoter avant de disparaitre.
void Clignote_Nmi_Avant_Disparition(int numero_nmi)
{                                                              // Indique que le vaisseau ennemi doit clignoter avant de disparaitre.
  ve[numero_nmi].dead = TRUE;
  ve[numero_nmi].visible_dead = TRUE;
  ve[numero_nmi].cmpt_color_dead = 0;                          // Donne la couleur de départ dans le tableau des couleurs du vaisseau qui disparait.
  ve[numero_nmi].invincible = 20;
  ve[numero_nmi].spr.etat_dommages = 0;
  //Test si un gardien est présent, si c'est la première partie du gardien et si le gardien est en deux parties..
  //Dans le cas du bras articulé par exemple.
  if(gardian_activity && !numero_nmi && ve[1].deplacement == GARDIEN)
  {                                                            //Dans ce cas, il faut aussi faire disparaitre l'autre partie du gardien.
    ve[1].dead = TRUE;
    ve[1].visible_dead = TRUE;
    ve[1].cmpt_color_dead = 0;                                 // Donne la couleur de départ dans le tableau des couleurs du vaisseau qui disparait.
    ve[1].invincible = 20;
    ve[1].spr.etat_dommages = 0;
  }
}



//------------------------------------------------------------------------------
// gardien : fin du jeu (on a trois gradiens a la suite)
//------------------------------------------------------------------------------
int gardien_testFin()
{
  if(gardian_activity)
  { 
    // Gestion special pour les Gardiens 11, 12, 13, et 14
    if(num_level == NBR_LEVEL_MAX && gardian_activity >= 11
       && gardian_activity < 15)
    {
      disparition_vj = TRUE;
      if(vit_star_field == 0.0)
      { switch (gardian_activity)
        { case 11:
            if(!Load_Gardien(12))
              return 0;
            Meteor_Activity_With_Guardian(12);
            Apparition_Vaisseau(); // fait reapparaitre le vaisseau du joueur.
            break;
          case 12:
            if(!Load_Gardien(13))
              return 0;
            Meteor_Activity_With_Guardian(13);
            Apparition_Vaisseau(); // fait reapparaitre le vaisseau du joueur.
            break;
          case 13:
            if(!Load_Gardien(14))
              return 0;
            Meteor_Activity_With_Guardian(14);
            Apparition_Vaisseau(); // fait reapparaitre le vaisseau du joueur.
            break;
          case 14:
            //disparition_vj = TRUE ;
            Meteor_Activity_With_Guardian(15);
            break;
        }
      }
    }
    else
    {
      disparition_vj = TRUE;                                   //1=disparition du vaisseau du joueur
      move_level = TRUE;                                       //1=deplacement du texte relative au numero du niveau
      if(vit_star_field == 0.0 && pos_x_level == 0.0 && pos_y_level == 0.0)
      {
        num_level++;                                           //passage au niveau suivant
        if(num_level > NBR_LEVEL_MAX)
          num_level = 0;
        switch (num_level)                                     //chargement du gardien
        {
          case 4:
            if(!Load_Gardien(2))
              return 0;
            break;
          case 8:
            if(!Load_Gardien(3))
              return 0;
            break;
          case 12:
            if(!Load_Gardien(4))
              return 0;
            break;
          case 16:
            if(!Load_Gardien(5))
              return 0;
            break;
          case 20:
            if(!Load_Gardien(6))
              return 0;
            break;
          case 24:
            if(!Load_Gardien(7))
              return 0;
            break;
          case 28:
            if(!Load_Gardien(8))
              return 0;
            break;
          case 32:
            if(!Load_Gardien(9))
              return 0;
            break;
          case 36:
            if(!Load_Gardien(10))
              return 0;
            break;
          case 40:
            if(!Load_Gardien(11))
              return 0;
            break;
        }
        if(!LoadGrid(num_level))                               //chargement d'une grille et remplissage du tableau d'ennemis.
          return 0;
        if(!Load_Curve_Level(num_level))                       //chargement du niveau courbe actuel.
          return 0;
        meteor_libere();                                       //liberation de la memoire utilisee par les images des meteorites
        if(!Load_Meteor(num_level))                            //chargement des meteorites.
          return 0;
        Active_Curve_Level();                                  //activation du fichier courbe chargé précédement.
        courbe.activity = 1;                                   //indique que l'on doit gérer la courbe.
        grille.activity = 0;
        meteor_activity = 0;
        gardian_activity = 0;
        Apparition_Vaisseau();                                 //indique qu'il faut faire réapparaitre le vaisseau du joueur.
      }
    }
  }                                                            // Fin du test pour savoir si on était dans la phase des gardiens.
  return 1;
}

/*
gardien 1 : gauche a droite
gardien 2 : gauche a droite + animation descente
gardien 3 : gauche a droite
gardien 4 : gauche a droite + animation quand il pause
gardien 5 : gauche a droite (sinus vertical) + missile
gardien 6 : compose de 2 sprites : gauche a droite  (tube vertical)
gardien 7 : gauche a droite + mine (tir & anim si mort) + autres vaisseaux
gardien 8 : gauche a droite + autres vaisseaux
gardien 9 : compose de 2 sprites : gauche a droite (nez du vaisseau) + autres vaisseaux
gardien 10 : tourne en rectangle + autres vaisseaux
gardien 11 : gauche a droite + autres vaisseaux
*/

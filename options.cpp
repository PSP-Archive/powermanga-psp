//*****************************************************************************
// copyright (c) 1998-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "options.cpp"
// created      : ?
// updates      : 2002-08-02
//-----------------------------------------------------------------------------
// function     : void  option_execution() 
//-----------------------------------------------------------------------------
// descriptions : gere la bande verticale panneau des options a doite de l'ecran
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
extern char            *ecran_opt;                             //adresse barre options
extern short            touch[500];
extern unsigned int     iBoutonOption;
extern int              touche_ctrl;                           //true=touche ctrl presse
extern vaisseau         vj[NBR_MAX_J];                         //declaration de la structure des vaisseaux des joueurs.
extern bool             option_change;                         //true=changement d'options a lieur
extern bool             bar_nrj_player;
extern image            vj1[NBR_MAX_VAIS_J][NBR_MAX_IMG_VAIS_J];
extern option           aff_option[MAX_TYP_OF_OPTIONS];                        //tableau affichage des options
extern int              liste_bou[MAX_BOULES_ON_SCREEN];       //liste de touts les 'bou' geres actuellement
extern int              liste_can[MAX_CANONS_ON_SCREEN];       //liste de touts les 'can' geres actuellement
extern int              bonusx;                                //mutliplicatreur des points
extern bool             option_change;                         //1=un changement d'option a lieu
extern short            nbr_canons;                            //nombre de canons actuellement geres
extern int              new_can;                               //pointeur sur un nouvel element 'can'
extern str_canon        can[MAX_CANONS_ON_SCREEN];             //declaration de la structure des canons.
extern int              old_option;                            //ancien numero d'option (utilise pour l'effacement dans le panneau d'options)
extern image            canon[MAX_TYP_OF_CANONS][NBR_IMAGES_CANONS];    //tableau des différents ennemis
static short            pos_cadre;
extern fonte            doublex[5];                            //tableau des differentes images des multiplicateurs
extern char            *ecran;                                 //adresse de l'ecran physique
extern int              cmpt_vbls_x2, cmpt_vbls_x4, aff_x2_rj, aff_x4_rj;       //variables utilisees pour la gestion des multiplicateurs
extern fonte            options[MAX_TYP_OF_OPTIONS][NBR_IMAGES_OPTIONS];        // Définit le tableau des différentes images des options.
//extern option           aff_option[11];                        //Tableau utilise pour l'affichage des options pendant le jeu.
extern int              old_option;                            //Ancien numéro d'option, utilise pour l'effacement dans le panneau d'options.

//..............................................................................
unsigned int            pOptionChange[88][2] =
{ {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
};
int                     iOptionIndex = -1;
static unsigned int     iTempoCadre = 0;                       //tempo display for option selected

//position des options dans le panneau des options .............................
static unsigned int     pOptionsPositions[11][2] =
{ {260 - 256, 169 - 16},
  {290 - 256, 154 - 16},
  {260 - 256, 139 - 16},
  {290 - 256, 124 - 16},
  {260 - 256, 109 - 16},
  {290 - 256, 94 - 16},
  {260 - 256, 79 - 16},
  {290 - 256, 64 - 16},
  {260 - 256, 49 - 16},
  {290 - 256, 34 - 16},
  {260 - 256, 19 - 16}
};

//..............................................................................
static unsigned int     bonusx_clear = 0;
unsigned int            iBonusX2Affiche = 0;
unsigned int            iBonusX4Affiche = 0;

//------------------------------------------------------------------------------
// getion du tableau d'options a gauche
//------------------------------------------------------------------------------
void option_execution()
{
  static int              m, i;

  if(option_change)                                            //option change ?
  {
    Clear_Option(old_option);                                  //restauration background
    if(vj[J1].nbr_bonus > 11)                                  //out of option ?
    {
      vj[J1].nbr_bonus = 0;                                    //raz number of bonus
      if(nbr_canons < 2)                                       //
      {
        new_can = New_Element_Can();                           //retourne l'indice dans le tableau 'can' d'un nouvel élément de la liste.
        can[new_can].app_vaisseau = 0;                         //Indique le vaisseau à qui appartient ce canon.
        can[new_can].etat_dommages = (short)(100 + vj[J1].type_vaisseau * 10);  // si =< 0 cela indique que le canon est détruite, si > 0 cela   indique l'état des dommages.
        can[new_can].pow_of_dest = (short)(30 + vj[J1].type_vaisseau * 5);      // puissance de destruction (utilisé lors d'une collision).
        can[new_can].nbr_img = NBR_IMAGES_CANONS;              //indique le nombre d'images composant le sprite.
        can[new_can].img_act = 0;                              //indique le numéro de l'images actuelle.
        can[new_can].nbr_tot_vbl = 10;                         //indique le nombre de vbl qu'il faut avant de passer à l'image suivante.
        can[new_can].nbr_vbl_act = 0;                          //indique le compteur de vbl actuel pour l'animation de l'image.
        can[new_can].cad_tir_initiale = 25 - (vj[J1].type_vaisseau << 1);       // Corespond à la cadence de tir du canon.
        can[new_can].cad_tir = can[new_can].cad_tir_initiale;  // Initialise la variable utilisée pour compter le temps sépérant deux tirs.
        // Donne l'adresse des images du canon.
        for(m = 0; m < can[new_can].nbr_img; m++)
        {                                                      // Donne les images du canon.
          can[new_can].img[m] = (image *) & canon[vj[J1].type_vaisseau][m];
        }
        // Positionnement du caono autour du vaisseau appartenant au joueur.
        switch (nbr_canons)
        {                                                      // Dans le cas on est en présence du premier canon.
          case 1:
            can[new_can].position = 0;                         // Indique que le canon sera mis à droite.
            break;
            // Dans le cas ou en est en présence de plusieurs canons.
          case 2:
            if(can[liste_can[0]].position == 0)
              can[new_can].position = 1;
            else
              can[new_can].position = 0;
            break;
        }
      }
    }


    // Affecte la nouvelle valeur à la position du cadre.
    pos_cadre = vj[J1].nbr_bonus;
    // Indique que l'on vient de traiter le changement d'options.
    option_change = FALSE;
  }

  //**********************************************************************************************************************
  // Test si l'on doit jouer l'animation de fermeture ou d'ouverture d'une option.
  for(i = 0; i < 11; i++)
  {
    if(aff_option[i].anim_close)
      Play_Anim_Close_Option(i);
    if(aff_option[i].anim_open)
      Play_Anim_Open_Option(i);
  }

  //**********************************************************************************************************************
  // Affichage des options.
  switch (vj[J1].nbr_bonus)                                    // Test l'option du joueur.
  {                                                            // Choix de la vitesse de déplacement du vaisseau.
    case 1:
      // Test si aucune animation d'ouverture ou de fermeture de l'option est en cours.
      if(!aff_option[0].anim_close && !aff_option[0].anim_open)
      {                                                        // Test si la vitesse maximum du vaisseau n'est pas encore atteinte.
        if(vj[J1].vit_dep_vaisseau < 5)
        {                                                      // Dans ce cas, on peux joueur l'annimation de la vitesse du vaisseau.
          Aff_Options(vj[J1].nbr_bonus - 1);
        }
        else                                                   // Sinon, on doit afficher l'option non sélectionné.
        {                                                      // Affichage de l'option non sélectionnée.
          Clear_Option(0);
        }
      }
      // Affichage du cadre de sélection sur l'option du tableau de bord.
      Aff_Cadre_Selection(0);
      old_option = vj[J1].nbr_bonus - 1;                       // Attribut le numéro d'option.
      break;
      // Choix de l'augmentation de l'énergie du vaisseau.
    case 2:
      // Test si aucune animation d'ouverture ou de fermeture de l'option est en cours.
      if(!aff_option[1].anim_close && !aff_option[1].anim_open)
      {                                                        // Test si le joueur n'est pas au maximum de son energie.
        if(vj[J1].spr.etat_dommages != vj[J1].spr.pow_of_dest)
        {                                                      // Dans ce cas, on peux joueur l'annimation de l'énergie du vaisseau.
          Aff_Options(vj[J1].nbr_bonus - 1);
        }
        else                                                   // Sinon, on doit afficher l'option non sélectionné.
        {                                                      // Affichage de l'option non sélectionnée.
          Clear_Option(1);
        }
      }
      // Affichage du cadre de sélection sur l'option du tableau de bord.
      Aff_Cadre_Selection(1);
      old_option = vj[J1].nbr_bonus - 1;                       // Attribut le numéro d'option.
      break;
      // Choix de l'augmentation du power_avant du joueur.
    case 3:
      // Test si aucune animation d'ouverture ou de fermeture de l'option est en cours.
      if(!aff_option[2].anim_close && !aff_option[2].anim_open)
      {                                                        // Test si la puissance maximale du tir avant est atteinte.
        if(vj[J1].power_avant < 5)
        {                                                      // Dans ce cas, on peux joueur l'annimation du tir avant du vaisseau.
          Aff_Options(vj[J1].nbr_bonus - 1);
        }
        else                                                   // Sinon, on doit afficher l'option non sélectionné.
        {                                                      // Affichage de l'option non sélectionnée.
          Clear_Option(2);
        }
      }
      // Affichage du cadre de sélection sur l'option du tableau de bord.
      Aff_Cadre_Selection(2);
      old_option = vj[J1].nbr_bonus - 1;                       // Attribut le numéro d'option.
      break;
      // Choix de l'augmentation du power_spec_avant du joueur.
    case 4:
      // Test si aucune animation d'ouverture ou de fermeture de l'option est en cours.
      if(!aff_option[3].anim_close && !aff_option[3].anim_open)
      {                                                        // Test si la puissance maximale du tir spécial avant est atteinte.
        if(vj[J1].power_spec_avant < 5)
        {                                                      // Dans ce cas, on peux joueur l'annimation du tir spécial avant du vaisseau.
          Aff_Options(vj[J1].nbr_bonus - 1);
        }
        else                                                   // Sinon, on doit afficher l'option non sélectionné.
        {                                                      // Affichage de l'option non sélectionnée.
          Clear_Option(3);
        }
      }
      // Affichage du cadre de sélection sur l'option du tableau de bord.
      Aff_Cadre_Selection(3);
      old_option = vj[J1].nbr_bonus - 1;                       // Attribut le numéro d'option.
      break;
      // Choix de l'augmentation du power_gauche du joueur.
    case 5:
      // Test si aucune animation d'ouverture ou de fermeture de l'option est en cours.
      if(!aff_option[4].anim_close && !aff_option[4].anim_open)
      {                                                        // Test si la puissance maximale du tir gauche est atteinte.
        if(vj[J1].power_gauche < 5)
        {                                                      // Dans ce cas, on peux joueur l'annimation du tir gauche du vaisseau.
          Aff_Options(vj[J1].nbr_bonus - 1);
        }
        else                                                   // Sinon, on doit afficher l'option non sélectionné.
        {                                                      // Affichage de l'option non sélectionnée.
          Clear_Option(4);
        }
      }
      // Affichage du cadre de sélection sur l'option du tableau de bord.
      Aff_Cadre_Selection(4);
      old_option = vj[J1].nbr_bonus - 1;                       // Attribut le numéro d'option.
      break;
      // Choix de l'augmentation du power_spec_gauche du joueur.
    case 6:
      // Test si aucune animation d'ouverture ou de fermeture de l'option est en cours.
      if(!aff_option[5].anim_close && !aff_option[5].anim_open)
      {                                                        // Test si la puissance maximale du tir spécial gauche est atteinte.
        if(vj[J1].power_spec_gauche < 5)
        {                                                      // Dans ce cas, on peux joueur l'annimation du tir spécial gauche du vaisseau.
          Aff_Options(vj[J1].nbr_bonus - 1);
        }
        else                                                   // Sinon, on doit afficher l'option non sélectionné.
        {                                                      // Affichage de l'option non sélectionnée.
          Clear_Option(5);
        }
      }
      // Affichage du cadre de sélection sur l'option du tableau de bord.
      Aff_Cadre_Selection(5);
      old_option = vj[J1].nbr_bonus - 1;                       // Attribut le numéro d'option.
      break;
      // Choix de l'augmentation du power_droite du joueur.
    case 7:
      // Test si aucune animation d'ouverture ou de fermeture de l'option est en cours.
      if(!aff_option[6].anim_close && !aff_option[6].anim_open)
      {                                                        // Test si la puissance maximale du tir droit est atteinte.
        if(vj[J1].power_droite < 5)
        {                                                      // Dans ce cas, on peux joueur l'annimation du tir droit du vaisseau.
          Aff_Options(vj[J1].nbr_bonus - 1);
        }
        else                                                   // Sinon, on doit afficher l'option non sélectionné.
        {                                                      // Affichage de l'option non sélectionnée.
          Clear_Option(6);
        }
      }
      // Affichage du cadre de sélection sur l'option du tableau de bord.
      Aff_Cadre_Selection(6);
      old_option = vj[J1].nbr_bonus - 1;                       // Attribut le numéro d'option.
      break;
      // Choix de l'augmentation du power_spec_droite du joueur.
    case 8:
      // Test si aucune animation d'ouverture ou de fermeture de l'option est en cours.
      if(!aff_option[7].anim_close && !aff_option[7].anim_open)
      {                                                        // Test si la puissance maximale du tir spécial droit est atteinte.
        if(vj[J1].power_spec_droite < 5)
        {                                                      // Dans ce cas, on peux joueur l'annimation du tir spécial droit du vaisseau.
          Aff_Options(vj[J1].nbr_bonus - 1);
        }
        else                                                   // Sinon, on doit afficher l'option non sélectionné.
        {                                                      // Affichage de l'option non sélectionnée.
          Clear_Option(7);
        }
      }
      // Affichage du cadre de sélection sur l'option du tableau de bord.
      Aff_Cadre_Selection(7);
      old_option = vj[J1].nbr_bonus - 1;                       // Attribut le numéro d'option.
      break;
      // Choix de l'augmentation du power_arriere du joueur.
    case 9:
      // Test si aucune animation d'ouverture ou de fermeture de l'option est en cours.
      if(!aff_option[8].anim_close && !aff_option[8].anim_open)
      {                                                        // Test si la puissance maximale du tir arrière est atteinte.
        if(vj[J1].power_arriere < 5)
        {                                                      // Dans ce cas, on peux joueur l'annimation du tir arrière du vaisseau.
          Aff_Options(vj[J1].nbr_bonus - 1);
        }
        else                                                   // Sinon, on doit afficher l'option non sélectionné.
        {                                                      // Affichage de l'option non sélectionnée.
          Clear_Option(8);
        }
      }
      // Affichage du cadre de sélection sur l'option du tableau de bord.
      Aff_Cadre_Selection(8);
      old_option = vj[J1].nbr_bonus - 1;                       // Attribut le numéro d'option.
      break;
      // Choix de l'augmentation du power_spec_arriere du joueur.
    case 10:
      // Test si aucune animation d'ouverture ou de fermeture de l'option est en cours.
      if(!aff_option[9].anim_close && !aff_option[9].anim_open)
      {                                                        // Test si la puissance maximale du tir spécial arrière est atteinte.
        if(vj[J1].power_spec_arriere < 5)
        {                                                      // Dans ce cas, on peux joueur l'annimation du tir spécial arrière du vaisseau.
          Aff_Options(vj[J1].nbr_bonus - 1);
        }
        else                                                   // Sinon, on doit afficher l'option non sélectionné.
        {                                                      // Affichage de l'option non sélectionnée.
          Clear_Option(9);
        }
      }
      // Affichage du cadre de sélection sur l'option du tableau de bord.
      Aff_Cadre_Selection(9);
      old_option = vj[J1].nbr_bonus - 1;                       // Attribut le numéro d'option.
      break;
      // Choix de vaisseau supérieur pour le joueur.
    case 11:
      // Test si aucune animation d'ouverture ou de fermeture de l'option est en cours.
      if(!aff_option[10].anim_close && !aff_option[10].anim_open)
      {                                                        // Test si le vaisseau maximum du joueur est atteint.
        if(vj[J1].type_vaisseau < (NBR_MAX_VAIS_J - 1))
        {                                                      // Dans ce cas, on peux joueur l'annimation du vaisseau supérieur pour le joueur.
          Aff_Options(vj[J1].nbr_bonus - 1);
        }
        else                                                   // Sinon, on doit afficher l'option non sélectionné.
        {                                                      // Affichage de l'option non sélectionnée.
          Clear_Option(10);
        }
      }
      // Affichage du cadre de sélection sur l'option du tableau de bord.
      Aff_Cadre_Selection(10);
      old_option = vj[J1].nbr_bonus - 1;                       // Attribut le numéro d'option.
      break;
  }                                                            // Fin de l'affichage des options du joueur.


}

//------------------------------------------------------------------------------
// test button option is pressed
//------------------------------------------------------------------------------
void testBoutonOption()
{
  static int              i;

  if((touch[88] || iBoutonOption) && pos_cadre && !touche_ctrl)
  {                                                            // Attribution de l'option au vaisseau.
    switch (pos_cadre)
    {                                                          //increase speed ...............................................................
      case 1:
        if(vj[J1].vit_dep_vaisseau < 5)
        {
          vj[J1].vit_dep_vaisseau++;                           //increase ship's speed
          vj[J1].nbr_bonus = 0;
          option_change = 1;
          if(vj[J1].vit_dep_vaisseau >= 5)                     //speed maxi ?
            Init_Anim_Option(0, 1);                            //yes, close option
#ifdef USE_SDLMIXER
          seal_joue(SELECT);                                   //play sound with seal library
#endif
        }
        else
        {
#ifdef USE_SDLMIXER
          seal_joue(NOSELECT);
#endif
        }
        break;

        //increase energy ..............................................................
      case 2:
        if(vj[J1].spr.etat_dommages != vj[J1].spr.pow_of_dest) //energy maxi ?
        {
          vj[J1].spr.etat_dommages += 20;                      //increase ship's energy
          if(vj[J1].spr.etat_dommages > vj[J1].spr.pow_of_dest)
            vj[J1].spr.etat_dommages = vj[J1].spr.pow_of_dest; //set maxi
          bar_nrj_player = 1;                                  //1=display energy barline
          vj[J1].nbr_bonus = 0;
          option_change = 1;                                   //1=display options
          if(vj[J1].spr.etat_dommages == vj[J1].spr.pow_of_dest)
          {
            Init_Anim_Option(1, 1);
          }
#ifdef USE_SDLMIXER
          seal_joue(SELECT);
#endif
        }
        else
        {
#ifdef USE_SDLMIXER
          seal_joue(NOSELECT);                                 //joue un son avec seal
#endif
        }
        break;

        //increase power up ............................................................
      case 3:
        // Test si la puissance maximale du tir avant est atteinte.
        if(vj[J1].power_avant < 5)
        {                                                      // Incrément du power_avant du joueur 1.
          vj[J1].power_avant++;
          // Remet à zéro le nombre de bonus du joueur.
          vj[J1].nbr_bonus = 0;
          // Indique que l'on doit mettre à jour la barre des options.
          option_change = TRUE;
          // Test si la puissance maximale du tir avant est atteinte.
          if(vj[J1].power_avant >= 5)
          {                                                    // Demande que l'animation de fermeture de l'option tir avant s'effectue.
            Init_Anim_Option(2, TRUE);
          }
#ifdef USE_SDLMIXER
          seal_joue(SELECT);                                   //joue un son avec seal
#endif
        }
        else
        {
#ifdef USE_SDLMIXER
          seal_joue(NOSELECT);                                 //joue un son avec seal
#endif
        }
        break;
        // Choix de l'augmentation du power_spec_avant du joueur.
      case 4:
        // Test si la puissance maximale du tir spécial avant est atteinte.
        if(vj[J1].power_spec_avant < 5)
        {                                                      // Incrément du power_avant du joueur 1.
          vj[J1].power_spec_avant++;
          // Remet à zéro le nombre de bonus du joueur.
          vj[J1].nbr_bonus = 0;
          // Indique que l'on doit mettre à jour la barre des options.
          option_change = TRUE;
          // Test si la puissance maximale du tir spécial avant est atteinte.
          if(vj[J1].power_spec_avant >= 5)
          {                                                    // Demande que l'animation de fermeture de l'option tir special avant s'effectue.
            Init_Anim_Option(3, TRUE);
          }
#ifdef USE_SDLMIXER
          seal_joue(SELECT);                                   //joue un son avec seal
#endif
        }
        else
        {
#ifdef USE_SDLMIXER
          seal_joue(NOSELECT);                                 //joue un son avec seal
#endif
        }
        break;
        // Choix de l'augmentation du power_gauche du joueur.
      case 5:
        // Test si la puissance maximale du tir gauche est atteinte.
        if(vj[J1].power_gauche < 5)
        {                                                      // Incrément du power_gauche du joueur 1.
          vj[J1].power_gauche++;
          // Remet à zéro le nombre de bonus du joueur.
          vj[J1].nbr_bonus = 0;
          // Indique que l'on doit mettre à jour la barre des options.
          option_change = TRUE;
          // Test si la puissance maximale du tir gauche est atteinte.
          if(vj[J1].power_gauche >= 5)
          {                                                    // Demande que l'animation de fermeture de l'option tir gauche s'effectue.
            Init_Anim_Option(4, TRUE);
          }
#ifdef USE_SDLMIXER
          seal_joue(SELECT);                                   //joue un son avec seal
#endif
        }
        else
        {                                                      // Joue le son qui indique que la selection de l'option est impossible.
//                                                      if (sndfx) wt_ms_playeffect (fxhandle_impsel1,nbchannel+3,64L,PAN_RIGHT,15000) ;
#ifdef USE_SDLMIXER
          seal_joue(NOSELECT);                                 //joue un son avec seal
#endif
        }
        break;
        // Choix de l'augmentation du power_spec_gauche du joueur.
      case 6:
        // Test si la puissance maximale du tir spécial gauche est atteinte.
        if(vj[J1].power_spec_gauche < 5)
        {                                                      // Incrément du power_gauche du joueur 1.
          vj[J1].power_spec_gauche++;
          // Remet à zéro le nombre de bonus du joueur.
          vj[J1].nbr_bonus = 0;
          // Indique que l'on doit mettre à jour la barre des options.
          option_change = TRUE;
          // Test si la puissance maximale du tir spécial gauche est atteinte.
          if(vj[J1].power_spec_gauche >= 5)
          {                                                    // Demande que l'animation de fermeture de l'option tir spécial gauche s'effectue.
            Init_Anim_Option(5, TRUE);
          }
#ifdef USE_SDLMIXER
          seal_joue(SELECT);                                   //joue un son avec seal
#endif
        }
        else
        {
#ifdef USE_SDLMIXER
          seal_joue(NOSELECT);                                 //joue un son avec seal
#endif
        }
        break;
        // Choix de l'augmentation du power_droite du joueur.
      case 7:
        // Test si la puissance maximale du tir droit est atteinte.
        if(vj[J1].power_droite < 5)
        {                                                      // Incrément du power_droite du joueur 1.
          vj[J1].power_droite++;
          // Remet à zéro le nombre de bonus du joueur.
          vj[J1].nbr_bonus = 0;
          // Indique que l'on doit mettre à jour la barre des options.
          option_change = TRUE;
          // Test si la puissance maximale du tir droit est atteinte.
          if(vj[J1].power_droite >= 5)
          {                                                    // Demande que l'animation de fermeture de l'option tir droit s'effectue.
            Init_Anim_Option(6, TRUE);
          }
#ifdef USE_SDLMIXER
          seal_joue(SELECT);                                   //joue un son avec seal
#endif
        }
        else
        {
#ifdef USE_SDLMIXER
          seal_joue(NOSELECT);                                 //joue un son avec seal
#endif
        }
        break;
        // Choix de l'augmentation du power_spec_droite du joueur.
      case 8:
        // Test si la puissance maximale du tir spécial droit est atteinte.
        if(vj[J1].power_spec_droite < 5)
        {                                                      // Incrément du power_spec_droite du joueur 1.
          vj[J1].power_spec_droite++;
          // Remet à zéro le nombre de bonus du joueur.
          vj[J1].nbr_bonus = 0;
          // Indique que l'on doit mettre à jour la barre des options.
          option_change = TRUE;
          // Test si la puissance maximale du tir spécial droit est atteinte.
          if(vj[J1].power_spec_droite >= 5)
          {                                                    // Demande que l'animation de fermeture de l'option tir spécial droit s'effectue.
            Init_Anim_Option(7, TRUE);
          }
#ifdef USE_SDLMIXER
          seal_joue(SELECT);                                   //joue un son avec seal
#endif
        }
        else
        {
#ifdef USE_SDLMIXER
          seal_joue(NOSELECT);                                 //joue un son avec seal
#endif
        }
        break;
        // Choix de l'augmentation du power_arriere du joueur.
      case 9:
        // Test si la puissance maximale du tir arrière est atteinte.
        if(vj[J1].power_arriere < 5)
        {                                                      // Incrément du power_arrière du joueur 1.
          vj[J1].power_arriere++;
          // Remet à zéro le nombre de bonus du joueur.
          vj[J1].nbr_bonus = 0;
          // Indique que l'on doit mettre à jour la barre des options.
          option_change = TRUE;
          // Test si la puissance maximale du tir arrière est atteinte.
          if(vj[J1].power_arriere >= 5)
          {                                                    // Demande que l'animation de fermeture de l'option tir arrière s'effectue.
            Init_Anim_Option(8, TRUE);
          }
#ifdef USE_SDLMIXER
          seal_joue(SELECT);                                   //joue un son avec seal
#endif
        }
        else
        {
#ifdef USE_SDLMIXER
          seal_joue(NOSELECT);                                 //joue un son avec seal
#endif
        }
        break;
        // Choix de l'augmentation du power_spec_arriere du joueur.
      case 10:
        // Test si la puissance maximale du tir spécial arrière est atteinte.
        if(vj[J1].power_spec_arriere < 5)
        {                                                      // Incrément du power_spec_arrière du joueur 1.
          vj[J1].power_spec_arriere++;
          // Remet à zéro le nombre de bonus du joueur.
          vj[J1].nbr_bonus = 0;
          // Indique que l'on doit mettre à jour la barre des options.
          option_change = TRUE;
          // Test si la puissance maximale du tir spécial arrière est atteinte.
          if(vj[J1].power_spec_arriere >= 5)
          {                                                    // Demande que l'animation de fermeture de l'option tir spécial arrière s'effectue.
            Init_Anim_Option(9, TRUE);
          }
#ifdef USE_SDLMIXER
          seal_joue(SELECT);                                   //joue un son avec seal
#endif
        }
        else
        {
#ifdef USE_SDLMIXER
          seal_joue(NOSELECT);                                 //joue un son avec seal
#endif
        }
        break;
        //option vaisseau superieur
      case 11:
        // Test si le vaisseau maximum du joueur est atteint.
        if(vj[J1].type_vaisseau < (NBR_MAX_VAIS_J - 1))
        {
          // Donne le vaisseau supérieur au joueur.
          vj[J1].type_vaisseau++;
          vj[J1].metamorphose = TRUE;                          // Indique qu'une métamorphose du vaisseau doit s'opérer.
#ifdef USE_SDLMIXER
          seal_joue(VCHANGE1);                                 //joue un son avec seal
#endif
          vj[J1].invincible = TEMPS_INVINCIBLE;                // Indique le temps pendant lequel le joueur est invincible.
          vj[J1].cad_tir = 50 - (vj[J1].type_vaisseau * 5 + 5); // Nombre de VBLs nécessaires avant que le vaisseau puisse à noubeau tirer.
          // Mise en place des images du vaisseau du joueur 1.
          for(i = 0; i < NBR_MAX_IMG_VAIS_J; i++)
          {
            vj[J1].spr.img[i] = (image *) & vj1[vj[J1].type_vaisseau][i];       // Donne l'adresse des images.
          }
          // Initialisation du tableau 'bou'.
          Init_Tab_Boules();
          for(i = 0; i < MAX_BOULES_ON_SCREEN; i++)
            liste_bou[i] = 0;
          // Initialisation du tableau 'can'.
          Init_Tab_Canons();
          for(i = 0; i < MAX_CANONS_ON_SCREEN; i++)
            liste_can[i] = 0;
          // Test si l'énergie du joueur n'est pas au maximum.
          if(vj[J1].spr.etat_dommages < vj[J1].spr.pow_of_dest)
          {                                                    // Demande que l'animation de fermeture de l'option énergie s'effectue.
            Init_Anim_Option(1, TRUE);
          }
          vj[J1].spr.pow_of_dest = (short)(vj[J1].type_vaisseau * 20 + 20);     // Indique la puissance de destruction du sprite.
          vj[J1].spr.etat_dommages = vj[J1].spr.pow_of_dest;   // Indique l'état de domages du sprite.
          // Indique que la barre d'énergie du joueur à changé.
          bar_nrj_player = TRUE;
          // Remet à zéro le nombre de bonus du joueur.
          vj[J1].nbr_bonus = 0;
          // Indique que l'on doit mettre à jour la barre des options.
          option_change = TRUE;
          // Test si le vaisseau maximum du joueur est atteint.
          if(vj[J1].type_vaisseau >= (NBR_MAX_VAIS_J - 1))
          {                                                    // Demande que l'animation de fermeture de l'option transformation s'effectue.
            Init_Anim_Option(10, TRUE);
          }
          // Décrémente de 1 toutes les autres options du joueur.
          // Test si l'option vitesse était fermée et si oui, on l'ouvre.
          if(aff_option[0].close_option)
            Init_Anim_Option(0, FALSE);
          vj[J1].vit_dep_vaisseau--;                           // Décrémente de 1 la vitesse du joueur.
          if(vj[J1].vit_dep_vaisseau < 1)
            vj[J1].vit_dep_vaisseau = 1;                       // Clip la vitesse du joueur afin qu'il puisse se déplacer.
          // Test si l'option tir avant était fermée et si oui, on l'ouvre.
          if(aff_option[2].close_option)
            Init_Anim_Option(2, FALSE);
          vj[J1].power_avant--;                                // Décrémente de 1 le tir avant du joueur.
          if(vj[J1].power_avant < 1)
            vj[J1].power_avant = 1;                            // Clip le tir avant du joueur afin qu'il puisse tirer.
          // Test si l'option tir spécial avant était fermée et si oui, on l'ouvre.
          if(aff_option[3].close_option)
            Init_Anim_Option(3, FALSE);
          vj[J1].power_spec_avant--;                           // Décrémente de 1 le tir spécal avant du joueur.
          if(vj[J1].power_spec_avant < 0)
            vj[J1].power_spec_avant = 0;                       // Clip le tir spécial avant du joueur.
          // Test si l'option tir gauche était fermée et si oui, on l'ouvre.
          if(aff_option[4].close_option)
            Init_Anim_Option(4, FALSE);
          vj[J1].power_gauche--;                               // Décrémente de 1 le tir gauche du vaisseau du joueur.
          if(vj[J1].power_gauche < 0)
            vj[J1].power_gauche = 0;                           // Clip le tir gauche du joueur.
          // Test si l'option tir spécial gauche était fermée et si oui, on l'ouvre.
          if(aff_option[5].close_option)
            Init_Anim_Option(5, FALSE);
          vj[J1].power_spec_gauche--;                          // Décrémente de 1 le tir spécial gauche du joueur.
          if(vj[J1].power_spec_gauche < 0)
            vj[J1].power_spec_gauche = 0;                      // Clip le tir spécial gauche du joueur.
          // Test si l'option tir droite était fermée et si oui, on l'ouvre.
          if(aff_option[6].close_option)
            Init_Anim_Option(6, FALSE);
          vj[J1].power_droite--;                               // Décrémente de 1 le tir droite du vaisseau du joueur.
          if(vj[J1].power_droite < 0)
            vj[J1].power_droite = 0;                           // Clip le tir droite du joueur.
          // Test si l'option tir spécial droite était fermée et si oui, on l'ouvre.
          if(aff_option[7].close_option)
            Init_Anim_Option(7, FALSE);
          vj[J1].power_spec_droite--;                          // Décrémente de 1 le tir spécial droite du joueur.
          if(vj[J1].power_spec_droite < 0)
            vj[J1].power_spec_droite = 0;                      // Clip le tir spécial droite du joueur.
          // Test si l'option tir arriere était fermée et si oui, on l'ouvre.
          if(aff_option[8].close_option)
            Init_Anim_Option(8, FALSE);
          vj[J1].power_arriere--;                              // Décrémente de 1 le tir arriere du vaisseau du joueur.
          if(vj[J1].power_arriere < 0)
            vj[J1].power_arriere = 0;                          // Clip le tir arriere du joueur.
          // Test si l'option tir spécial arriere était fermée et si oui, on l'ouvre.
          if(aff_option[9].close_option)
            Init_Anim_Option(9, FALSE);
          vj[J1].power_spec_arriere--;                         // Décrémente de 1 le tir spécial arriere du joueur.
          if(vj[J1].power_spec_arriere < 0)
            vj[J1].power_spec_arriere = 0;                     // Clip le tir spécial arriere du joueur.
          //On RAZ la variable utilisee pour doubler ou quadrupler le score du joueur.
          bonusx = 0;
#ifdef USE_SDLMIXER
          seal_joue(SELECT);                                   //joue un son avec seal
#endif
        }
        else
        {
#ifdef USE_SDLMIXER
          seal_joue(NOSELECT);                                 //joue un son avec seal
#endif
        }
        break;
    }
  }
  touche_ctrl = (touch[88] | iBoutonOption);

  //**********************************************************************************************************************
  //Test si on doit ou non afficher les bonus.
  switch (bonusx)                                              // Test l'option du joueur.
  {
    //pas de bonus..................................................................
    case 0:
      if(bonusx_clear--)
      {
        putsprite3_opt(&doublex[0], 41, 171);                  //clear X2
        iBonusX2Affiche = 1;
      }
      if(bonusx_clear--)
      {
        putsprite3_opt(&doublex[0], 41, 5);                    //clear X4
        iBonusX4Affiche = 1;
      }
      break;

    case 2:                                                   //display X4
      if(!(cmpt_vbls_x4 & 15))
      {
        iBonusX4Affiche = 1;
        if(aff_x4_rj & 1)
          putsprite3_opt(&doublex[3], 41, 5);                  //display in red
        else
          putsprite3_opt(&doublex[4], 41, 5);                  //display in yellow
        aff_x4_rj++;
      }
      cmpt_vbls_x4++;

    case 1:                                                   //display X2
      if(!(cmpt_vbls_x2 & 15))
      {
        iBonusX2Affiche = 1;
        if(aff_x2_rj & 1)
          putsprite3_opt(&doublex[1], 41, 171);                //display in red
        else
          putsprite3_opt(&doublex[2], 41, 171);                //display in yellow
        aff_x2_rj++;
        if(bonusx == 1)
          putsprite3_opt(&doublex[0], 41, 5);                  //clear X4
      }
      cmpt_vbls_x2++;
      break;

  }
}

//------------------------------------------------------------------------------
// Affichage des options.
// input        => num_option : options number (0 to 10)
//------------------------------------------------------------------------------
void Aff_Options(int num_option)
{
  int                     _iOptionX;
  int                     _iOptionY;

  aff_option[num_option].nbr_vbl_act += aff_option[num_option].add_vbl;
  if(aff_option[num_option].nbr_vbl_act >= aff_option[num_option].nbr_tot_vbl)
  {
    aff_option[num_option].nbr_vbl_act =
      (aff_option[num_option].nbr_vbl_act -
       aff_option[num_option].nbr_tot_vbl);
    aff_option[num_option].img_act++;
    if(aff_option[num_option].img_act >= 32)
      aff_option[num_option].img_act = 0;
  }
  fonte                  *_pFonte = &options[num_option][aff_option[num_option].img_act];       //data option close

  _iOptionX = pOptionsPositions[num_option][0];
  _iOptionY = pOptionsPositions[num_option][1];
  putsprite3_opt(_pFonte, _iOptionX, _iOptionY);
  pOptionChange[++iOptionIndex][0] = _iOptionX;
  pOptionChange[iOptionIndex][1] = _iOptionY;
}

//------------------------------------------------------------------------------
// close an option 
// input        => num_option : option number (0 to 10)
//------------------------------------------------------------------------------
void Clear_Option(int num_option)
{
  fonte                  *_pFonte = &options[12][32];          //data option close
  int                     _iOptionX;
  int                     _iOptionY;
  //test if the option is open
  switch (num_option)
  {
    case 0:
      if(vj[J1].vit_dep_vaisseau < 5)
        _pFonte = &options[num_option][32];
      break;
    case 1:
      if(vj[J1].spr.etat_dommages != vj[J1].spr.pow_of_dest)
        _pFonte = &options[num_option][32];
      break;
    case 2:
      if(vj[J1].power_avant < 5)
        _pFonte = &options[num_option][32];
      break;
    case 3:
      if(vj[J1].power_spec_avant < 5)
        _pFonte = &options[num_option][32];
      break;
    case 4:
      if(vj[J1].power_gauche < 5)
        _pFonte = &options[num_option][32];
      break;
    case 5:
      if(vj[J1].power_spec_gauche < 5)
        _pFonte = &options[num_option][32];
      break;
    case 6:
      if(vj[J1].power_droite < 5)
        _pFonte = &options[num_option][32];
      break;
    case 7:
      if(vj[J1].power_spec_droite < 5)
        _pFonte = &options[num_option][32];
      break;
    case 8:
      if(vj[J1].power_arriere < 5)
        _pFonte = &options[num_option][32];
      break;
    case 9:
      if(vj[J1].power_spec_arriere < 5)
        _pFonte = &options[num_option][32];
      break;
    case 10:
      if(vj[J1].type_vaisseau < (NBR_MAX_VAIS_J - 1))
        _pFonte = &options[num_option][32];
      break;
  }
  _iOptionX = pOptionsPositions[num_option][0];
  _iOptionY = pOptionsPositions[num_option][1];
  putsprite3_opt(_pFonte, _iOptionX, _iOptionY);
  pOptionChange[++iOptionIndex][0] = _iOptionX;
  pOptionChange[iOptionIndex][1] = _iOptionY;
}

//------------------------------------------------------------------------------
// display close animation option
// input        => num_option : option number (0 to 10)
//------------------------------------------------------------------------------
void Play_Anim_Close_Option(int num_option)
{
  int                     _iOptionX;
  int                     _iOptionY;

  aff_option[num_option].nbr_vbl_act += aff_option[num_option].add_vbl;
  if(aff_option[num_option].nbr_vbl_act >= aff_option[num_option].nbr_tot_vbl)
  {
    aff_option[num_option].nbr_vbl_act =
      (aff_option[num_option].nbr_vbl_act -
       aff_option[num_option].nbr_tot_vbl);
    aff_option[num_option].img_act++;
    if(aff_option[num_option].img_act >= 31)
      aff_option[num_option].anim_close = 0;
  }
  aff_option[num_option].close_option = 1;                     //1=option is closed
  _iOptionX = pOptionsPositions[num_option][0];
  _iOptionY = pOptionsPositions[num_option][1];
  putsprite3_opt(&options[11][aff_option[num_option].img_act], _iOptionX, _iOptionY);   // "gfxbase.cpp"
  pOptionChange[++iOptionIndex][0] = _iOptionX;
  pOptionChange[iOptionIndex][1] = _iOptionY;
}

//------------------------------------------------------------------------------
// display open animation option
// input        => num_option : option number (0 to 10)
//------------------------------------------------------------------------------
void Play_Anim_Open_Option(int num_option)
{
  int                     num_ouverture;
  int                     _iOptionX;
  int                     _iOptionY;

  num_ouverture = 12;
  aff_option[num_option].nbr_vbl_act += aff_option[num_option].add_vbl; //increase tempo counter
  if(aff_option[num_option].nbr_vbl_act >= aff_option[num_option].nbr_tot_vbl)
  {
    aff_option[num_option].nbr_vbl_act =
      (aff_option[num_option].nbr_vbl_act -
       aff_option[num_option].nbr_tot_vbl);
    aff_option[num_option].img_act++;                          //next image
    if(aff_option[num_option].img_act >= 32)                   //last image ?
    {
      aff_option[num_option].anim_open = 0;                    //option is open
      num_ouverture = num_option;
    }
  }
  aff_option[num_option].close_option = 0;                     //option is opening
  _iOptionX = pOptionsPositions[num_option][0];
  _iOptionY = pOptionsPositions[num_option][1];
  putsprite3_opt(&options[num_ouverture][aff_option[num_option].img_act], _iOptionX, _iOptionY);
  pOptionChange[++iOptionIndex][0] = _iOptionX;
  pOptionChange[iOptionIndex][1] = _iOptionY;
}

//------------------------------------------------------------------------------
// initialize options animations
// input        => num_option : option number (0 to 10)
//                              => val_option   : 0=open / 1=close
//------------------------------------------------------------------------------
void Init_Anim_Option(int num_option, bool val_option)
{
  if(val_option == 1)
  {
    aff_option[num_option].anim_open = 0;
    aff_option[num_option].anim_close = 1;
    aff_option[num_option].img_act = 0;
    aff_option[num_option].nbr_vbl_act = 0;
  }
  else
  {
    aff_option[num_option].anim_open = 1;
    aff_option[num_option].anim_close = 0;
    aff_option[num_option].img_act = 0;
    aff_option[num_option].nbr_vbl_act = 0;
  }
}

//------------------------------------------------------------------------------
// display selection
// input        => num_option : options number (0 to 10)
//------------------------------------------------------------------------------
void Aff_Cadre_Selection(int num_option)
{
  int                     _iOptionX;
  int                     _iOptionY;

  if(((iTempoCadre++) & 7) > 4)
  {
    _iOptionX = pOptionsPositions[num_option][0];
    _iOptionY = pOptionsPositions[num_option][1];
    putsprite3_opt(&options[11][32], _iOptionX, _iOptionY);    //"gfxbase.cpp"
    pOptionChange[++iOptionIndex][0] = _iOptionX;
    pOptionChange[iOptionIndex][1] = _iOptionY;
  }
}

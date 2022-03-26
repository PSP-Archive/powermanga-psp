//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "gardien12.cpp"
// created      : 1999-03-23
// updates      : 2005-01-08
// id		: $Id: gardien12.cpp,v 1.1.1.1 2005/01/25 05:52:49 gurumeditation Exp $
//-----------------------------------------------------------------------------
// infos        : Guardian number 12 : 2*32 images
//                1 to 8  flip
//                9 to 16 bottom to up
//                17 to31 up to bottom
//-----------------------------------------------------------------------------
// functions    : void _init_gardien12()
//                void _execute_gardien12()
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
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];
extern short            nbr_nmis;
extern int              new_ve;
extern vaisseau         vj[NBR_MAX_J];                         // Declaration de la structure des vaisseaux des joueurs.
extern image            gardi[NBR_ANIM_GARDIAN][NBR_IMAGES_NMI_S];      // Definit le tableau pour les images des differents gardiens.
extern bool             apparition_gardian;                    // Utilise pour indiquer si on doit ou non faire l'apparition d'un gardien.
extern float            pSin[128];
extern float            pCos[128];
extern bool             player_pause;                          // true=pause en cours
extern int              etat_menu;                             // MENU_OFF(false)=menu inactif ; MENU_ON(true)=menu actif
extern short            nbr_tirs;                              // Indique le nombre de tir actuellement gérés.
extern int              new_tir;                               // Indique l'indice du tableau corespondant à un nouvel objet 'tirs'.
extern tir              tirs[NBR_TIRS_MAX];                    // Définit le tableau de tous les objets tirs.
extern image            fire[MAX_TYP_OF_TIR][NBR_IMAGES_TIR];  // Définit le tableau des différents tirs.
extern unsigned int     iCompteurGlobal;
extern image            enemi[MAX_TYP_OF_ENEMI + MAX_TYP_OF_S_ENEMI + MAX_TYP_OF_NMI_SPE + MAX_TYP_OF_OBJ_SPE][NBR_IMAGES_MAX]; // Définit le tableau des différents ennemis.
extern _etbfonte1      *pEtbfonte1;
extern int              iNumeroGardien;                        //numero du gardien en cours (ETB)

//..............................................................................
int                     iAngleG1;
int                     iAngleG2;
int                     iAngleG3;
int                     iAngleG1Inc;
int                     iPositionXSauve;
int                     iTypedeplacement;
int                     iPosYbase;
int                     iTempo3;
int                     iTempo4;
int                     pShuriy[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int                     iAngleShury;
int                     iAngleShury2;
int                     iCentreX;
int                     iCentreY;

//..............................................................................
extern int              compteur_shuriy;
void                    ajoute_shuriy(int);
void                    execute_shuriy(int);

//------------------------------------------------------------------------------
// Execution du gardien 12
//------------------------------------------------------------------------------
void _execute_gardien12(int num_gardien)
{
  int                     k, j;

  if(apparition_gardian)
  {
    if(--iTempo4 < 0)
    {
      Fait_Apparaitre_Gardien_Haut(num_gardien);
      iPositionXSauve = (int)ve[num_gardien].spr.coor_x;
    }
  }
  else
  {                                                            //if(--iTempo4<=0) iTempo4=5 ;
    //else return ;
    if(!player_pause && etat_menu == MENU_OFF
       && _score::getActionExecution() == 0)
    {                                                          //Move_Gardian_Sin(num_gardien) ;
      execute_shuriy(num_gardien);
      switch (iTypedeplacement)

      {
        //------------------------------------------------------------------------------
        case 0:                                               // deplacement de gauche a droite (animation 1)
        {
          iAngleG1 = (iAngleG1 + iAngleG1Inc) & 127;
          iAngleG2 = (iAngleG2 + 2) & 127;
          float                   _fSin = pSin[iAngleG1];
          int                     _iRayon = int (50 * pSin[iAngleG2] + 50);
          int                     _iPositionX =
            (int)(_fSin * _iRayon) + (256 -
                                      ve[num_gardien].spr.img[15]->larg / 2);
          ve[num_gardien].spr.coor_x = (float)_iPositionX;
          if(iPositionXSauve < _iPositionX)
          {
            if(ve[num_gardien].spr.img_act > 0)
              ve[num_gardien].spr.img_act--;
          }
          else
          {
            if(iPositionXSauve > _iPositionX)
            {
              if(ve[num_gardien].spr.img_act < 31)
                ve[num_gardien].spr.img_act++;
            }
            else
            {
              if(ve[num_gardien].spr.img_act < 15)
                ve[num_gardien].spr.img_act++;
              if(ve[num_gardien].spr.img_act > 15)
                ve[num_gardien].spr.img_act--;
            }
          }
          iPositionXSauve = _iPositionX;
          if(_iRayon == 0)
          {
            if((--iTempo3) <= 0)
            {
              iTypedeplacement = 1;
              iPosYbase = (int)ve[num_gardien].spr.coor_y;
            }
          }
        }
          break;
        //------------------------------------------------------------------------------
        case 1:                                               // le vaisseau devient droit
        {
          if(ve[num_gardien].spr.img_act == 15)
          {
            iTypedeplacement = 2;
            for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
            {
              ve[num_gardien].spr.img[k] = (image *) & gardi[1][k];
            }
            ve[num_gardien].spr.img_act = 0;
            iTypedeplacement = 2;
            ajoute_shuriy(num_gardien);
            iTempo3 = 10;
          }

          if(ve[num_gardien].spr.img_act < 15)
            ve[num_gardien].spr.img_act++;
          if(ve[num_gardien].spr.img_act > 15)
            ve[num_gardien].spr.img_act--;
        }
          break;
        //------------------------------------------------------------------------------
        case 2:                                               // les lampes du vaisseau clignotent (animation 2)
        {
          if(ve[num_gardien].spr.img_act < 7)
            ve[num_gardien].spr.img_act++;
          else
          {
            if(--iTempo3 <= 0)
            {
              ve[num_gardien].spr.img_act = 8;
              iTypedeplacement = 3;
              iTempo3 = 0;
              iAngleG3 = 0;
            }
            else
              ve[num_gardien].spr.img_act = 0;
          }
        }
          break;
        //------------------------------------------------------------------------------
        case 3:                                               // deplacement verticale (animation 2)
        {
          if(--iTempo3 <= 0)
          {
            iAngleG3 = (iAngleG3 + 1) & 127;
            iTempo3 = 0;
          }
          float                   _fSin = pSin[iAngleG3];

          if(_fSin == 0.0)
          {
            iAngleG3 = 0;
            iTypedeplacement = 4;
          }
          int                     _iPositionY =
            int (80 * pSin[iAngleG3]) + iPosYbase;
          ve[num_gardien].spr.coor_y = (float)_iPositionY;
          if(iAngleG3 >= 33 && ve[num_gardien].spr.img_act < 31)
            ve[num_gardien].spr.img_act++;
          else if(iAngleG3 < 33 && ve[num_gardien].spr.img_act < 17)
            ve[num_gardien].spr.img_act++;
        }
          break;
        //------------------------------------------------------------------------------
        case 4:                                               // retour a l'etat 1
        {
          if(ve[num_gardien].spr.img_act < 31)
            ve[num_gardien].spr.img_act++;
          else
          {
            iTypedeplacement = 0;
            iTempo3 = (iCompteurGlobal + (int)(vj[J1].spr.coor_x) & 31) + 5;
            for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
            {
              ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
            }
            ve[num_gardien].spr.img_act = 15;
            iPositionXSauve = (int)ve[num_gardien].spr.coor_x;
          }
        }
          break;
      }

      ve[num_gardien].cad_tir--;
      // Test si l'ennemi doit ou non lancer un tir.
      if(ve[num_gardien].cad_tir <= 0 && nbr_tirs < (NBR_TIRS_MAX - 1))
      {                                                        // Met la cadence du tir pour le vaisseau ennemi.
        ve[num_gardien].cad_tir = ve[num_gardien].cad_tir_initiale;
        // Bcle sur tous les points de tirs existants.
        for(k = 0;
            k <
            ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->nbr_pts_tir;
            k++)
        {                                                      // Test s'il est possible d'ajouter un nouveau tir à la liste.
          if(nbr_tirs < (NBR_TIRS_MAX - 1))
          {                                                    // Recherche d'un nouveau tir.
            new_tir = New_Element_Tir();
#ifdef USE_SDLMIXER
            seal_joue(TIR3NMI);                                //joue un son avec seal
#endif
            // Remplissage de la structure sprite pour le tir du vaisseau ennemi.
            tirs[new_tir].clignote = TRUE;                     // Indique que l'on est en présence d'un sprite ennemi qui doit clignoter.
            tirs[new_tir].spr.genre = ENNEMI;                  // Indique que c'est un sprite ennemi.
            tirs[new_tir].spr.trajectoire = FALSE;             // Indique que la trajectoire du sprite est calculée une fois pour toute.
            tirs[new_tir].spr.nbr_img = 32;                    // Indique le nombre d'images composant le sprite.
            tirs[new_tir].spr.pow_of_dest = 8;                // Indique la puissance de destruction du sprite.
            // Donne l'adresse des images du sprite.
            for(j = 0; j < tirs[new_tir].spr.nbr_img; j++)
            {                                                  // Chargement du missile.
              tirs[new_tir].spr.img[j] = (image *) & fire[TIR1P3E][j];  // Corespond au Tir 2 puissance 2.
            }
            tirs[new_tir].spr.etat_dommages = tirs[new_tir].spr.pow_of_dest;    // Indique l'état de domages du sprite.
            //tirs[new_tir].spr.pilote = 0;                      // Indique par qui est dirigé le sprite.
            //tirs[new_tir].spr.mode_commande = 0;               // Indique le moyen par lequel on fait déplacer le sprite.
            tirs[new_tir].spr.img_act = 0;                     // Indique l'image actuelle.
            tirs[new_tir].spr.nbr_tot_vbl = 1;                 // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
            tirs[new_tir].spr.nbr_vbl_act = 0;                 // Indique le nombre de VBL comptées actuellement.
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
            tirs[new_tir].spr.vitesse = 2.0;                   // Donne la vitesse de déplacement du sprite.
          }
        }
      }                                                        // Fin du test pour savoir si l'ennemi doit ou non tirer.
    }                                                          // Fin du test pour savoir si la pause est ou non activée.
  }
}

//------------------------------------------------------------------------------
// Initialise le gardien 12
//------------------------------------------------------------------------------
void _init_gardien12()
{
  int                     k;

  iTempo3 = (iCompteurGlobal + (int)(vj[J1].spr.coor_x) & 31) + 10;
  iTempo4 = 200;
  iAngleG1 = 0;
  iAngleG2 = 0;
  iAngleG3 = 0;
  iAngleG1Inc = 1;
  compteur_shuriy = 0;
  iAngleShury = 0;
  iAngleShury2 = 5;
  iTypedeplacement = 0;
  new_ve = New_Element_Ve();                                   // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
  iNumeroGardien = new_ve;
  ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + KAMEAMEA); // Indique la puissance de destruction du sprite.
  ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 250);    // Indique l'état de domages du sprite.
  ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;      // sauve etat
  ve[new_ve].spr.nbr_img = 32;                                 // Indique le nombre d'images composant le sprite.
  ve[new_ve].spr.img_act = 15;                                 // Indique l'image actuelle.
  ve[new_ve].spr.nbr_vbl_act = 0;                              // RAZ du compteur de VBL pour le passage à l'image suivante.
  ve[new_ve].spr.nbr_tot_vbl = 4;                              // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
  // Donne l'adresse des images du sprite.
  for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
  {
    ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
  }
  iCentreX = (ve[new_ve].spr.img[ve[new_ve].spr.img_act]->x_cg - 16);
  iCentreY = (ve[new_ve].spr.img[ve[new_ve].spr.img_act]->y_cg - 16);
  ve[new_ve].cad_tir_initiale = 60;                            // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
  ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;            // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
  ve[new_ve].deplacement = GARDIEN;                            // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
  // Donne les coordonées x et y de départ du vaisseau.
  ve[new_ve].spr.coor_x =
    (float)(LARG_ECR_RAM - ve[new_ve].spr.img[15]->larg / 2);
  iPositionXSauve = (int)ve[new_ve].spr.coor_x;
  ve[new_ve].spr.coor_y =
    (float)(DEBUT_ECRAN_Y - ve[new_ve].spr.img[15]->haut);
  ve[new_ve].spr.vitesse = 0.5;                                // Donne la vitesse de déplacement du gardien.
  ve[new_ve].type = KAMEAMEA;                                  // Indique le type de gardien.
  ve[new_ve].dead = FALSE;                                     // Indique que l'ennemi est bien vivant.
  ve[new_ve].visible = TRUE;                                   // Indique que l'ennemi est visible, on devra donc l'afficher.
}

//------------------------------------------------------------------------------
// add 5 news "ETB_SHURIKY" sprites
//------------------------------------------------------------------------------
void ajoute_shuriy(int num_gardien)
{
  int                     k, i;

  if(compteur_shuriy == 0)
  {
    compteur_shuriy = 1;
    iAngleShury = 0;
#ifdef USE_SDLMIXER
    seal_joue(TIR1);                                           //joue un son avec seal
#endif
    for(i = 0; i < 5; i++)
    {
      new_ve = New_Element_Ve();
      pShuriy[i] = new_ve;
      ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + 5);
      ve[new_ve].spr.etat_dommages = ve[new_ve].spr.pow_of_dest - 2;
      ve[new_ve].spr.nbr_img = 32;
      ve[new_ve].spr.img_act = 0;
      ve[new_ve].spr.nbr_vbl_act = 0;
      ve[new_ve].spr.nbr_tot_vbl = 2;
      for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
      {
        ve[new_ve].spr.img[k] = (image *) & enemi[SHURIKY][k];
      }
      ve[new_ve].cad_tir_initiale = 70 * 4;
      ve[new_ve].cad_tir = (i * 10);
      ve[new_ve].deplacement = SPECIAUX;
      ve[new_ve].spr.coor_x =
        ve[num_gardien].spr.coor_x +
        ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->x_cg -
        ve[new_ve].spr.img[0]->larg / 2;
      ve[new_ve].spr.coor_y =
        ve[num_gardien].spr.coor_y +
        ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->y_cg -
        ve[new_ve].spr.img[0]->haut / 2;
      ve[new_ve].spr.vitesse = 0.3f;
      ve[new_ve].type = ETB_SHURIKY;
      ve[new_ve].dead = FALSE;
      ve[new_ve].visible = TRUE;
    }
  }
}

//------------------------------------------------------------------------------
// run the 5  "ETB_SHURIKY" sprites
//------------------------------------------------------------------------------
void execute_shuriy(int num_gardien)
{
  int                     k, i;
  k = 0;
  iAngleShury = (iAngleShury + 3) & 127;
  if(compteur_shuriy > 0)
  {
    iAngleShury2 = (iAngleShury2 + 1) & 127;
    int                     _iAngleShury = iAngleShury;
    int                     _iAngleShury2 = iAngleShury2;

    for(i = 0; i < 5; i++)
    {
      new_ve = pShuriy[i];
      _iAngleShury = (_iAngleShury + 24) & 127;
      if(ve[new_ve].type == ETB_SHURIKY
         && ve[new_ve].prev_object != MAX_NMI_ON_SCREEN)
      {
        float                   _fSin = pSin[_iAngleShury];
        float                   _fCos = pCos[_iAngleShury];
        int                     _iRayon1 = (int)(pSin[_iAngleShury2] * 70);
        int                     _iRayon2 = (int)(pSin[_iAngleShury2] * 70);

        ve[new_ve].spr.coor_x =
          (_fSin * _iRayon1 + ve[num_gardien].spr.coor_x + iCentreX);
        ve[new_ve].spr.coor_y =
          (_fCos * _iRayon2 + ve[num_gardien].spr.coor_y + iCentreY);
        k++;
      }
    }
    if(k == 0)
      compteur_shuriy = 0;
  }
}

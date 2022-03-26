//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "gardien14.cpp"
// created      : 1999-03-26
// updates      : 2005-01-08
// id		: $Id: gardien14.cpp,v 1.1.1.1 2005/01/25 05:52:54 gurumeditation Exp $
//-----------------------------------------------------------------------------
// functions    : void _init_gardien14()
//                void _execute_gardien14()
//                void ajoute_saakey(int num_gardien)
//                void execute_saakey(int num_gardien)
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
extern int              iNumeroGardien;                        // numero du gardien en cours (ETB)
//..............................................................................
extern int              iAngleG1;
extern int              iAngleG2;
extern int              iAngleG3;
extern int              iAngleG1Inc;
extern int              iPositionXSauve;
extern int              iTypedeplacement;
extern int              iPosYbase;
extern int              iTempo3;
extern int              iTempo4;
extern int              pShuriy[10];
extern int              iAngleShury;
extern int              iAngleShury2;
extern int              iCentreX;
extern int              iCentreY;
int                     iNumeroImageGardien;
//..............................................................................
extern int              compteur_shuriy;
void                    execute_saakey(int num_gardien);
void                    ajoute_saakey(int num_gardien);
void                    change_image_gardien(int num_gardien);
//..............................................................................
typedef struct structGhotten
{
  int                     iNumero;                             // numero dans la structure 've' de JMM
  int                     iRayon;                              // rayon par rapport au centre de la trompe
  int                     iAngleh;                             // angle par rapport au centre
  int                     iAnglev;                             // angle par rapport au centre
  int                     iAngle2h;                            // angle par rapport au centre
  int                     iAngle2v;                            // angle par rapport au centre
  int                     iRayon2h;
  int                     iRayon2v;
  int                     iAction;                             // action 0, 1 ou 2
  int                     iSens;                               // sens increment
  int                     iTempo1;
  int                     iTempo2;
  int                     iCentreX;
  int                     iCentreY;
  int                     iMaxiRayon2h;
  int                     iMaxiRayon;
}
structGhotten;
//..............................................................................
const int               iNGHOTTEN = 10;
structGhotten           pGhotten[iNGHOTTEN];
int                     iFrequenceGhotten;

//------------------------------------------------------------------------------
// Execution du gardien 14
//------------------------------------------------------------------------------
void _execute_gardien14(int num_gardien)
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
    if(!player_pause && etat_menu == MENU_OFF
       && _score::getActionExecution() == 0)
    {                                                          //Move_Gardian_Sin(num_gardien) ;
      execute_saakey(num_gardien);
      switch (iTypedeplacement)

      {
        //------------------------------------------------------------------------------
        case 0:                                               // deplacement de gauche a droite (animation 1)
        {
          iAngleG1 = (iAngleG1 + iAngleG1Inc) & 127;
          iAngleG2 = (iAngleG2 + 2) & 127;
          float                   _fSin = pSin[iAngleG1];
          int                     _iPositionX =
            (int)(_fSin * 77) + (256 - ve[num_gardien].spr.img[15]->larg / 2);
          ve[num_gardien].spr.coor_x = (float)_iPositionX;
          if(++iNumeroImageGardien >= 64)
            iNumeroImageGardien = 0;
          iPositionXSauve = _iPositionX;
          ajoute_saakey(num_gardien);
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
            ajoute_saakey(num_gardien);
            iTempo3 = 10;
          }

          if(ve[num_gardien].spr.img_act < 15)
            ve[num_gardien].spr.img_act++;
          if(ve[num_gardien].spr.img_act > 15)
            ve[num_gardien].spr.img_act--;
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
            // Indique la puissance de destruction du sprite.
            tirs[new_tir].spr.pow_of_dest = 8;
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
    change_image_gardien(num_gardien);                         // gestion de 64 images du gardien
  }
}

//------------------------------------------------------------------------------
// Initialise le gardien 14
//------------------------------------------------------------------------------
void _init_gardien14()
{
  int                     k;
  iNumeroImageGardien = 0;
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
  ve[new_ve].spr.pow_of_dest = (short)((vj[J1].type_vaisseau << 1) + GHOTTEN);  // Indique la puissance de destruction du sprite.
  ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + (ve[new_ve].spr.pow_of_dest << 3) / 3 + 250);    // Indique l'état de domages du sprite.
  ve[new_ve].spr.etat_max = ve[new_ve].spr.etat_dommages;      // sauve etat
  ve[new_ve].spr.nbr_img = 32;                                 // Indique le nombre d'images composant le sprite.
  ve[new_ve].spr.img_act = 31;                                 // Indique l'image actuelle.
  ve[new_ve].spr.nbr_vbl_act = 0;                              // RAZ du compteur de VBL pour le passage à l'image suivante.
  ve[new_ve].spr.nbr_tot_vbl = 4;                              // Indique le nombre de VBL qu'il faut pour passer à l'image suivante.
  // Donne l'adresse des images du sprite.
  for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
  {
    ve[new_ve].spr.img[k] = (image *) & gardi[0][k];
  }
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
  ve[new_ve].type = GHOTTEN;                                   // Indique le type de gardien.
  ve[new_ve].dead = FALSE;                                     // Indique que l'ennemi est bien vivant.
  ve[new_ve].visible = TRUE;                                   // Indique que l'ennemi est visible, on devra donc l'afficher.
  for(k = 0; k < iNGHOTTEN; k++)
  {
    pGhotten[k].iNumero = -1;
  }
  iFrequenceGhotten = 0;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void change_image_gardien(int num_gardien)
{
  int                     k;
  if(iNumeroImageGardien < 32)
  {
    for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
    {
      ve[num_gardien].spr.img[k] = (image *) & gardi[0][k];
    }
    ve[num_gardien].spr.img_act = iNumeroImageGardien;
  }
  else
  {
    for(k = 0; k < ve[num_gardien].spr.nbr_img; k++)
    {
      ve[num_gardien].spr.img[k] = (image *) & gardi[1][k];
    }
    ve[num_gardien].spr.img_act = iNumeroImageGardien - 32;
  }
}

//------------------------------------------------------------------------------
// gardien 14 : ajoute un "saakey" à la trompe du gardien 14
//------------------------------------------------------------------------------
void ajoute_saakey(int num_gardien)
{
  if(--iFrequenceGhotten < 0)                                  // on ajoute un saakey ?
  {
    int                     _iNumero;

#ifdef USE_SDLMIXER
    seal_joue(TIR1);                                           //joue un son avec seal
#endif
    for(int _iCompteur = 0; _iCompteur < iNGHOTTEN; _iCompteur++)
    {
      if(pGhotten[_iCompteur].iNumero == -1)
      {
        iFrequenceGhotten = 100;
        iAngleG3 = (iAngleG3 + 13) & 127;
        _iNumero = New_Element_Ve();                           // on reserve un nouveau sprite !
        pGhotten[_iCompteur].iNumero = _iNumero;
        pGhotten[_iCompteur].iRayon = 0;
        pGhotten[_iCompteur].iMaxiRayon = (iCompteurGlobal & 7) + 30;
        pGhotten[_iCompteur].iAngleh = iAngleG3;
        pGhotten[_iCompteur].iAnglev = (iAngleG3 + iAngleG1) & 127;
        pGhotten[_iCompteur].iRayon2h = 0;
        pGhotten[_iCompteur].iMaxiRayon2h = (iCompteurGlobal & 3) + 5;
        pGhotten[_iCompteur].iRayon2v = 0;
        pGhotten[_iCompteur].iAngle2h = iCompteurGlobal & 127;
        pGhotten[_iCompteur].iAngle2v = (iCompteurGlobal + iAngleG3) & 127;
        pGhotten[_iCompteur].iSens = 1;
        pGhotten[_iCompteur].iAction = 0;
        pGhotten[_iCompteur].iTempo1 = 0;
        pGhotten[_iCompteur].iTempo2 = 0;
        ve[_iNumero].spr.pow_of_dest =
          (short)((vj[J1].type_vaisseau << 1) + 5);
        ve[_iNumero].spr.etat_dommages = ve[_iNumero].spr.pow_of_dest - 2;
        ve[_iNumero].spr.nbr_img = 32;
        ve[_iNumero].spr.img_act = 0;
        ve[_iNumero].spr.nbr_vbl_act = 0;
        ve[_iNumero].spr.nbr_tot_vbl = 6;
        for(int _iImage = 0; _iImage < ve[_iNumero].spr.nbr_img; _iImage++)
        {
          ve[_iNumero].spr.img[_iImage] = (image *) & enemi[SAAKEY][_iImage];
        }
        ve[_iNumero].cad_tir_initiale = 70 * 4;
        ve[_iNumero].cad_tir = (_iCompteur * 10);
        ve[_iNumero].deplacement = SPECIAUX;
        ve[_iNumero].spr.coor_x =
          (ve[num_gardien].spr.coor_x +
           ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
           xy_pos_tir[0][X]) - 16;
        ve[_iNumero].spr.coor_y =
          (ve[num_gardien].spr.coor_y +
           ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
           xy_pos_tir[0][Y]) - 16;
        ve[_iNumero].spr.vitesse = 0.3f;
        ve[_iNumero].type = SAAKEY;
        ve[_iNumero].dead = FALSE;
        ve[_iNumero].visible = TRUE;
        goto finajout;
      }
    }
  }
finajout:;
}

//------------------------------------------------------------------------------
// gardien 14 : deplacement "saakey" 
//------------------------------------------------------------------------------
void execute_saakey(int num_gardien)
{
  int                     _iNumero;

  iCentreX =
    (int)(ve[num_gardien].spr.coor_x +
          ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
          xy_pos_tir[0][X]) - 16;
  iCentreY =
    (int)(ve[num_gardien].spr.coor_y +
          ve[num_gardien].spr.img[ve[num_gardien].spr.img_act]->
          xy_pos_tir[0][Y]) - 16;

  for(int _iCompteur = 0; _iCompteur < iNGHOTTEN; _iCompteur++)
  {
    _iNumero = pGhotten[_iCompteur].iNumero;
    if(_iNumero > -1)
    {
      if((ve[_iNumero].type == SAAKEY || ve[_iNumero].type == SAAKAMIN)
         && ve[_iNumero].prev_object != MAX_NMI_ON_SCREEN)
      {
        pGhotten[_iCompteur].iAngle2h =
          (pGhotten[_iCompteur].iAngle2h - 4) & 127;
        pGhotten[_iCompteur].iAngle2v =
          (pGhotten[_iCompteur].iAngle2v + 1) & 127;
        int                     _iCentreX = 0;
        int                     _iCentreY = 0;

        switch (pGhotten[_iCompteur].iAction)
        {
            //-------------------------------------
          case 0:                                             // le saakey sort de la trompe
          {
            _iCentreX = iCentreX;
            _iCentreY = iCentreY;
            if(ve[_iNumero].spr.img_act >= 31)
            {
              for(int _iImage = 0; _iImage < ve[_iNumero].spr.nbr_img;
                  _iImage++)
              {
                ve[_iNumero].spr.img[_iImage] =
                  (image *) & enemi[SAAKAMIN][_iImage];
              }
              ve[_iNumero].spr.img_act = 0;
              ve[_iNumero].spr.nbr_tot_vbl = 3;
              pGhotten[_iCompteur].iAction = 1;
              ve[_iNumero].type = SAAKAMIN;
            }
          }
            break;
            //-------------------------------------
          case 1:                                             // rotation autour de la trompe
          {
            if(pGhotten[_iCompteur].iRayon2h <
               pGhotten[_iCompteur].iMaxiRayon2h)
              pGhotten[_iCompteur].iRayon2h++;
            pGhotten[_iCompteur].iRayon2v = pGhotten[_iCompteur].iRayon2h;
            _iCentreX = iCentreX;
            _iCentreY = iCentreY;
            if(--pGhotten[_iCompteur].iTempo1 < 0)
            {
              pGhotten[_iCompteur].iRayon = pGhotten[_iCompteur].iRayon + 1;
              pGhotten[_iCompteur].iTempo1 = 5;
              if(pGhotten[_iCompteur].iRayon >
                 pGhotten[_iCompteur].iMaxiRayon)
              {
                pGhotten[_iCompteur].iRayon = pGhotten[_iCompteur].iMaxiRayon;
                pGhotten[_iCompteur].iAction = 2;
                pGhotten[_iCompteur].iCentreX = iCentreX;
                pGhotten[_iCompteur].iCentreY = iCentreY;
              }
            }
            if(--pGhotten[_iCompteur].iTempo2 < 0)
            {
              pGhotten[_iCompteur].iAngleh =
                (pGhotten[_iCompteur].iAngleh + 1) & 127;
              pGhotten[_iCompteur].iAnglev =
                (pGhotten[_iCompteur].iAnglev - 1) & 127;
              pGhotten[_iCompteur].iTempo2 = 1;
            }
          }
            break;
            //-------------------------------------
          case 2:                                             // le saakey decend
          {
            _iCentreX = pGhotten[_iCompteur].iCentreX;
            _iCentreY = pGhotten[_iCompteur].iCentreY;
            if(pGhotten[_iCompteur].iRayon2v < 50)
              pGhotten[_iCompteur].iRayon2v++;
            pGhotten[_iCompteur].iRayon =
              pGhotten[_iCompteur].iRayon + pGhotten[_iCompteur].iSens;
            if(pGhotten[_iCompteur].iRayon > pGhotten[_iCompteur].iMaxiRayon)
              pGhotten[_iCompteur].iSens = -1;
            if(pGhotten[_iCompteur].iRayon < 5)
              pGhotten[_iCompteur].iSens = 1;
            if(--pGhotten[_iCompteur].iTempo1 < 0)
            {
              pGhotten[_iCompteur].iTempo1 = 5;
              pGhotten[_iCompteur].iCentreY++;
              if(pGhotten[_iCompteur].iCentreY >=
                 DEBUT_ECRAN_Y + HAUT_ECR_RAM)
                ve[_iNumero].type = SHURIKY;
            }

            pGhotten[_iCompteur].iAngleh =
              (pGhotten[_iCompteur].iAngleh - 2) & 127;
            if(--pGhotten[_iCompteur].iTempo2 < 0)
            {
              pGhotten[_iCompteur].iAnglev =
                (pGhotten[_iCompteur].iAnglev - 1) & 127;
              pGhotten[_iCompteur].iTempo2 = 1;
            }

          }
            break;
        }

        float                   _fSin = pSin[pGhotten[_iCompteur].iAngleh];
        float                   _fCos = pCos[pGhotten[_iCompteur].iAnglev];
        float                   _fX = ve[_iNumero].spr.coor_x =
          (_fSin * pGhotten[_iCompteur].iRayon + _iCentreX);
        float                   _fY = ve[_iNumero].spr.coor_y =
          (_fCos * pGhotten[_iCompteur].iRayon + _iCentreY);
        _fSin = pSin[pGhotten[_iCompteur].iAngle2h];
        _fCos = pCos[pGhotten[_iCompteur].iAngle2v];
        ve[_iNumero].spr.coor_x =
          (_fSin * pGhotten[_iCompteur].iRayon2h + _fX);
        ve[_iNumero].spr.coor_y =
          (_fCos * pGhotten[_iCompteur].iRayon2v + _fY);
        // 
        if(((short)ve[_iNumero].spr.coor_x +
            ve[_iNumero].spr.img[ve[_iNumero].spr.img_act]->larg) <
           DEBUT_ECRAN_X
           || ((short)ve[_iNumero].spr.coor_y +
               ve[_iNumero].spr.img[ve[_iNumero].spr.img_act]->haut) <
           DEBUT_ECRAN_Y
           || (short)ve[_iNumero].spr.coor_y >= DEBUT_ECRAN_Y + HAUT_ECR_RAM
           || (short)ve[_iNumero].spr.coor_x >= DEBUT_ECRAN_X + LARG_ECR_RAM)
        {
          ve[_iNumero].visible = FALSE;
        }
        else
        {
          ve[_iNumero].visible = TRUE;
        }

      }
      else
      {
        pGhotten[_iCompteur].iNumero = -1;
      }
    }
  }
}

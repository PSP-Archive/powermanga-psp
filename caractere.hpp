//*****************************************************************************
// copyright (c) 1998-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "caractere.hpp"
// created      : 1998-04-21
// updates      : 2002-08-11
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
//******************************************************************************
#ifndef __CARACTERE__
#define __CARACTERE__

const  int iANGLEMAXI=127;

class _caractere
{ static char*          pChaine ;

  // membres
  private :
    int                  iAffiche;                             // 0=n'affiche pas / 1=affiche
    int                  iIndexCaractere;                      // index sur le caractere a afficher
    int                  iIndexImage;                          // index sur le numero de l'image
    int                  iPositionX;                           // abscisse
    int                  iPositionY;                           // ordonnee
    int                  iCentreX;
    int                  iCentreY;
    int                  iRayonX;
    int                  iRayonY;
    int                  iAngle;                               // Angle utilise le calcul des sin et cos
    int                  iAngleInc;                            // Increment de l'angle
    int                  iLargeur;                             // largeur d'une fonte
    int                  iHauteur;                             // hauteur d'une fonte
    int                  iNombreImages ;                       // nombres d'images pour l'animation
    int                  iTypeFonte;                           // 0=fonte jeu; 1=big_fonte 
    int                  iTempoAnimation;                      // compteur temporisation
    int                  iTempoInit;                           // valeur initialede iTempoAnimation

  // methodes
  public :
                         _caractere();
                         ~_caractere();
    void                 initialise_caractere(char _cAscii, int _iPositionX, int _iPositionY, int _iTypeFonte);
    void                 convertit_caractere(char _cAscii);
    void                 changePosition(int _iPosX, int _iPosY);
    void                 changePositionY(int _iPosY);
    void                 changePositionX(int _iPosX);
    void                 affiche();
    void                 animation();
    int                  animation2();
    void                 debuteAnimation();
    int                  rotation1(int _iDecX, int _iDecY, int _iMin);
    int                  rotation2(int _iIncX, int _iIncY, int _iMaxX, int _iMaxY);
    void                 initRotation(int _iAngle, int _iAngleInc, int _iRayonX, int _iRayonY);
};
#endif

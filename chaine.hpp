//*****************************************************************************
// copyright (c) 1998-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "_chaine.hpp"
// created      : 1998-05-21
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
//*****************************************************************************
#ifndef __CHAINE__
#define __CHAINE__

#include "caractere.hpp"

class _chaine
{
  private :
  // Membres statiques
    static int            iCodeTouchePressee;     // code touche pressee
    static int            iCodeTouchePressee2;    // code touche pressee precedement
    static int            iCodeToucheRelachee;    // code tocuhe relachee
    static unsigned char  pCycle[];               // couleurs du cycling du curseur
    static int            iTempoClavier ;         // temporisation du clavier
    static unsigned char  pCodesClavier[];

  // Methodes statiques
  public :
    static void            setCodeTouchePressee(int _iCodeTouchePresse);
    static void            setCodeToucheRelachee(int _iCodeToucheRelachee);
    static void            entierEnASCII(int _iNombre, char* _pChaine);

  // Membres
  private :
    int                    iAngle;                // Angle utilise le calcul des sin et cos
    int                    iAngleInc;             // Increment de l'angle
    int                    iCentreX;              // Abscisse du centre
    int                    iCentreY;              // Ordonnee du centre
    int                    iCurseurPosition;      // position du cuseur dans la chaine
    int                    iIndexCycle;           // index sur le tableau pCycle
    int                    iNombreCaracteres;     // nombre de caracteres utilises
    int                    iOffsetH;              // espace entre les caracteres
    int                    iPositionX;            // Abscisse du premier caractere de la chaine
    int                    iPositionY;            // Ordonnee du premier caractere de la chaine
    int                    iRayonX;
    int                    iRayonY;
    int                    iTempoCycle;           // temporisation du cyle couleur
    int                    iCurseurAction;
    char*                  pAscii;                // pointeur sur la chaine ASCII
	int                    pAscii_allocted;       // 1=pAscii was allocated, else copied
    _caractere*            pCaracteres;           // pointeur sur un taleau d'objets de type _caractere

  // Methodes
  public :
                          _chaine();
                          _chaine(char* _pChaine, int _iPositionX, int _iPositionY, int _iOffsetH);
                          ~_chaine();
  int                     getNombreCaracteres();
  _caractere*             getCaracteres();
  void                    affiche();
  void                    affiche2();
  void                    animationTous();
  int                     animation2Tous();
  void                    changePosition(int _iPosX, int _iPosY);
  void                    changePositionX(int _iPosX);
  void                    changePositionY(int _iPosY);

  protected :
    void                  razMembres();
    void                  init(int _iPositionX, int _iPositionY, int _iOffsetH, int _iTypeFonte);
  public :
    void                  initialise(char* _pChaine, int _iPositionX, int _iPositionY, int _iOffsetH, int _iTypeFonte);
    void                  initialise(char* _pChaine, int _iNombreCaracteres, int _iPositionX, int _iPositionY, int _iOffsetH, int _iTypeFonte);
    void                  initialiseChaine(char* _pChaine) ;
    void                  initialiseSprite();
    void                  initialiseSpritePos();
    int                   getPositionX();
    int                   getPositionY();
    void                  setCurseurPosition(int _iCurseurPosition);
    int                   getCurseurPosition() const;
    int                   entreeClavier();
    void                  entreeClavier(int _iCodeTouchePressee);
    unsigned char         getCouleur();
    int                   rotation1(int _iDecX, int _iDecY, int _iMin);
    int                   rotation2(int _iIncX, int _iIncY, int _iMaxX, int _iMaxY);
    void                  initRotation(int _iAngle, int _iAngleInc, int _iRayonX, int _iRayonY);
    int                   getCurseurAction();
};
#endif

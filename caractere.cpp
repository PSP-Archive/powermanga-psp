//*****************************************************************************
// copyright (c) 1998-2003 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "caractere.cpp"
// created      : 1998-04-21
// updates      : 2003-04-07
//-----------------------------------------------------------------------------
// description  : Implementation de la classe _caractere
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
#include 	"linux.hpp"
#include	"caractere.hpp"
#include	"powermanga.hpp"
//..............................................................................
extern float            pSin[128];
extern float            pCos[128];
extern fonte            fnt_game[NBR_FONTES_GAME][NBR_IMAGES_FONTE];
extern fonte            fnt_big[NBR_FONTES_BIG][NBR_IMAGES_FONTE];
extern fonte            fnt_scroll[NBR_FONTES_SCROLL];
extern char            *ecran_ram;                             // Adresse de l'ecran logique.
char                   *_caractere::pChaine = "(c) 2003 TLK GAMES";
//..............................................................................

// METHODES ====================================================================

// Constructeur ----------------------------------------------------------------
        _caractere::_caractere()
{
  iAngle = 0;
  iAngleInc = 1;
  iCentreX = 0;
  iCentreY = 0;
  iIndexCaractere = 0;
  iIndexImage = 0;
  iPositionX = 0;
  iPositionY = 0;
  iRayonX = 0;
  iRayonY = 0;
  iTypeFonte = 0;
  iLargeur = LARG_FONTE_GAME;
  iHauteur = HAUT_FONTE_GAME;
  iNombreImages = NBR_IMAGES_FONTE;
  iAffiche = 0;
  iTempoAnimation = 0;
  iTempoInit = 5;
}

// Destructeur -----------------------------------------------------------------
        _caractere::~_caractere()
{
}


//------------------------------------------------------------------------------
// Intialise les caracteres
// Entree => _cAscii     : caractere ASCII
//        => _iPositionX : Abscisse
//        => _iPositionY : Ordonnee
//        => _iTypeFonte : Type de police utilisee
//------------------------------------------------------------------------------
void _caractere::initialise_caractere(char _cAscii, int _iPositionX,
                                      int _iPositionY, int _iTypeFonte)
{
  iTypeFonte = _iTypeFonte;
  if(iTypeFonte < 0)
    _iTypeFonte = 0;
  if(iTypeFonte > 2)
    _iTypeFonte = 2;
  switch (iTypeFonte)
  {
    case 0:                                                   // fontes 'game'
      iLargeur = LARG_FONTE_GAME;
      iHauteur = HAUT_FONTE_GAME;
      iNombreImages = NBR_IMAGES_FONTE;
      break;
    case 1:                                                   // fontes 'big'
      iLargeur = LARG_FONTE_GAME;
      iHauteur = HAUT_FONTE_GAME;
      iNombreImages = NBR_IMAGES_FONTE;
      break;
    case 2:                                                   // fontes 'scroll'
      iLargeur = LARG_FONTE_GAME;
      iHauteur = HAUT_FONTE_GAME;
      iNombreImages = NBR_IMAGES_FONTE;
      break;
  }
  convertit_caractere(_cAscii);
  iPositionX = _iPositionX;
  iPositionY = _iPositionY;
  iCentreX = iPositionX;
  iCentreY = iPositionY;
  iIndexImage = 0;
}

//------------------------------------------------------------------------------
// Convertit le caractere ASCII en index sur la structure fonte
// input => _cAscii : ASCII code
//------------------------------------------------------------------------------
void _caractere::convertit_caractere(char _cAscii)
{                                                              // Il n'ya pas de caractere representant l'espace (Jean-Mi, tu manques d'inspiration ?)
  if(_cAscii == 32)
    iAffiche = 0;
  else
    iAffiche = 1;
  // Convertit le caractere ASCII en index sur la structure fonte de JMM
  switch (iTypeFonte)
  {
    case 0:                                                   // fontes 'game'
      iIndexCaractere = (int)_cAscii - 33;
      break;
    case 1:                                                   // fontes 'big'
      iIndexCaractere = (int)_cAscii - 65;
      break;
    case 2:                                                   // fontes 'scroll'
      iIndexCaractere = (int)_cAscii - 33;
      break;
  }
}

//------------------------------------------------------------------------------
// Change la position ecran
// input => _iPosX : Abscisse
//       => _iPosY : Ordonnee
//------------------------------------------------------------------------------
void _caractere::changePosition(int _iPosX, int _iPosY)
{
  iPositionX = _iPosX;
  iPositionY = _iPosY;
}

// Change l'ordonnee ecran -----------------------------------------------------
void _caractere::changePositionY(int _iPosY)
//      Entree =>        _iPosY : Ordonnee
{
  iPositionY = _iPosY;
}

// Change l'abscisse ecran -----------------------------------------------------
void _caractere::changePositionX(int _iPosX)
//      Entree =>        _iPosX : Abscisse
{
  iPositionX = _iPosX;
}


// Affiche ---------------------------------------------------------------------
void _caractere::affiche()
{
  if(iAffiche == 1)
  { if(iPositionY > (BANDE_DE_CLIP - 32) && iPositionY < (BANDE_DE_CLIP + HAUT_ECR_RAM))
    { fonte                  *_pSource = 0;
      switch (iTypeFonte)
      { case 0:                                               // fontes 'game'
          _pSource = &fnt_game[iIndexCaractere][iIndexImage];
          break;
        case 1:                                               // fontes 'big'
          _pSource = &fnt_big[iIndexCaractere][iIndexImage];
          break;
        case 2:                                               // fontes 'scroll'
          _pSource = &fnt_scroll[iIndexCaractere];
          break;
      }
      putsprite3(_pSource, iPositionX, iPositionY);
    }
  }
}


// Animation du caractere en boucle ------------------------------------------
void _caractere::animation()
{
  if(iTempoAnimation == 0)
  { iIndexImage++;
    iTempoAnimation = 5;
    if(iIndexImage >= NBR_IMAGES_FONTE) iIndexImage = 0;
    iTempoAnimation = iTempoInit;
  }
  else iTempoAnimation--;
}

// Animation du caractere une seule fois -------------------------------------
// sortie <= 0=animation terminee
int _caractere::animation2()
{
  int _iFin = 0;

  if(iIndexImage > 0)
  { if(iTempoAnimation == 0)
    { iIndexImage++;
      iTempoAnimation = iTempoInit;
      if(iIndexImage >= NBR_IMAGES_FONTE) iIndexImage = 0;
      else                                _iFin = 1;
    }
    else
    { iTempoAnimation--;
      _iFin = 1;
    }
  }
  return _iFin;
}

// Force la deuxieme image de l'Animation ------------------------------------
void _caractere::debuteAnimation()
{
  if(iIndexImage == 0)
  { iIndexImage = 1;
    iTempoAnimation = iTempoInit;
  }
}

// Rotation du caractere
// sortie <= 0=rotation non terminee / 1=rotation terminee
int _caractere::rotation1(int _iDecX, int _iDecY, int _iMin)
{
  int _iRetour = 0;
  iAngle = (iAngle + iAngleInc) & iANGLEMAXI;
  if(iRayonX > _iMin) iRayonX -= _iDecX;
  if(iRayonY > _iMin) iRayonY -= _iDecY;
  if(iRayonX < _iMin) iRayonX = _iMin;
  if(iRayonY < _iMin) iRayonY = _iMin;
  if(iRayonX == _iMin && iRayonY == _iMin) _iRetour = 1;
  float _fSin = pSin[iAngle];
  float _fCos = pCos[iAngle];
  iPositionX = (int)(_fSin * iRayonX) + iCentreX;
  iPositionY = (int)(_fCos * iRayonY) + iCentreY;
  return _iRetour;
}

// Rotation du caractere autour de son centre avec elargissement du rayon
// sortie <= 0=rotation non terminee / 1=rotation terminee
int _caractere::rotation2(int _iIncX, int _iIncY, int _iMaxX, int _iMaxY)
{
  int _iRetour = 0;
  iAngle = (iAngle + iAngleInc) & iANGLEMAXI;
  float _fSin = pSin[iAngle];
  float _fCos = pCos[iAngle];
  if(iRayonX < _iMaxX)
    iRayonX += _iIncX;
  if(iRayonY < _iMaxY)
    iRayonY += _iIncY;
  if(iRayonX > _iMaxX)
    iRayonX = _iMaxX;
  if(iRayonY > _iMaxY)
    iRayonY = _iMaxY;
  if(iRayonX == _iMaxX && iRayonY == _iMaxY)
    _iRetour = 1;
  iPositionX = (int)(_fSin * iRayonX) + iCentreX;
  iPositionY = (int)(_fCos * iRayonY) + iCentreY;
  return _iRetour;
}

void _caractere::initRotation(int _iAngle, int _iAngleInc, int _iRayonX, int _iRayonY)
{
  iAngle = _iAngle;
  iAngleInc = _iAngleInc;
  iRayonX = _iRayonX;
  iRayonY = _iRayonY;
}

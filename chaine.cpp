//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "_chaine.cpp"
// created      : 1998-05-21
// updates      : 2005-01-11
//-----------------------------------------------------------------------------
// description  : Implementation de la classe _chaine
//                gere des chaines de caractere, avec un affichage 'sprite'
//-----------------------------------------------------------------------------
// Note         : le point tout en haut a gauche de l'ecran correspond a la
//                coordonnee (128, 128)
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
#include "linux.hpp"
#include "chaine.hpp"
#include "powermanga.hpp"

#ifndef SDL_TLK
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#endif
//..............................................................................
extern char            *ecran_ram;                             // adresse de l'ecran logique.
float                   pSin[128]={0.0000f, 0.0491f, 0.0980f, 0.1467f, 0.1951f, 0.2430f, 0.2903f, 0.3369f, 0.3827f, 0.4275f, 0.4714f, 0.5141f, 0.5556f, 0.5957f, 0.6344f, 0.6716f, 0.7071f, 0.7409f, 0.7730f, 0.8032f, 0.8315f, 0.8577f, 0.8819f, 0.9040f, 0.9239f, 0.9415f, 0.9569f, 0.9700f, 0.9808f, 0.9892f, 0.9952f, 0.9988f, 1.0000f, 0.9988f, 0.9952f, 0.9892f, 0.9808f, 0.9700f, 0.9569f, 0.9416f, 0.9239f, 0.9040f, 0.8819f, 0.8577f, 0.8315f, 0.8032f, 0.7730f, 0.7410f, 0.7071f, 0.6716f, 0.6344f, 0.5957f, 0.5556f, 0.5141f, 0.4714f, 0.4276f, 0.3827f, 0.3369f, 0.2903f, 0.2430f, 0.1951f, 0.1468f, 0.0981f, 0.0491f, 0.0000f, -0.0490f, -0.0980f, -0.1467f, -0.1950f, -0.2429f, -0.2902f, -0.3368f, -0.3826f, -0.4275f, -0.4714f, -0.5141f, -0.5555f, -0.5957f, -0.6344f, -0.6715f, -0.7071f, -0.7409f, -0.7730f, -0.8032f, -0.8314f, -0.8577f, -0.8819f, -0.9040f, -0.9239f, -0.9415f, -0.9569f, -0.9700f, -0.9808f, -0.9892f, -0.9952f, -0.9988f, -1.0000f, -0.9988f, -0.9952f, -0.9892f, -0.9808f, -0.9700f, -0.9570f, -0.9416f, -0.9239f, -0.9040f, -0.8820f, -0.8578f, -0.8315f, -0.8033f, -0.7731f, -0.7410f, -0.7072f, -0.6716f, -0.6345f, -0.5958f, -0.5556f, -0.5142f, -0.4715f, -0.4276f, -0.3828f, -0.3370f, -0.2904f, -0.2431f, -0.1952f, -0.1468f, -0.0981f, -0.0492f};
float                   pCos[128]={1.0000f, 0.9988f, 0.9952f, 0.9892f, 0.9808f, 0.9700f, 0.9569f, 0.9415f, 0.9239f, 0.9040f, 0.8819f, 0.8577f, 0.8315f, 0.8032f, 0.7730f, 0.7410f, 0.7071f, 0.6716f, 0.6344f, 0.5957f, 0.5556f, 0.5141f, 0.4714f, 0.4276f, 0.3827f, 0.3369f, 0.2903f, 0.2430f, 0.1951f, 0.1467f, 0.0980f, 0.0491f, 0.0000f, -0.0490f, -0.0980f, -0.1467f, -0.1951f, -0.2430f, -0.2903f, -0.3369f, -0.3827f, -0.4275f, -0.4714f, -0.5141f, -0.5555f, -0.5957f, -0.6344f, -0.6715f, -0.7071f, -0.7409f, -0.7730f, -0.8032f, -0.8315f, -0.8577f, -0.8819f, -0.9040f, -0.9239f, -0.9415f, -0.9569f, -0.9700f, -0.9808f, -0.9892f, -0.9952f, -0.9988f, -1.0000f, -0.9988f, -0.9952f, -0.9892f, -0.9808f, -0.9700f, -0.9570f, -0.9416f, -0.9239f, -0.9040f, -0.8819f, -0.8578f, -0.8315f, -0.8032f, -0.7730f, -0.7410f, -0.7071f, -0.6716f, -0.6344f, -0.5957f, -0.5556f, -0.5142f, -0.4714f, -0.4276f, -0.3827f, -0.3369f, -0.2903f, -0.2430f, -0.1952f, -0.1468f, -0.0981f, -0.0491f, 0.0000f, 0.0490f, 0.0980f, 0.1467f, 0.1950f, 0.2429f, 0.2902f, 0.3368f, 0.3826f, 0.4275f, 0.4713f, 0.5140f, 0.5555f, 0.5956f, 0.6343f, 0.6715f, 0.7071f, 0.7409f, 0.7730f, 0.8032f, 0.8314f, 0.8577f, 0.8819f, 0.9040f, 0.9238f, 0.9415f, 0.9569f, 0.9700f, 0.9808f, 0.9892f, 0.9952f, 0.9988f} ;

// MEMBRES STATIQUES ===========================================================
int                     _chaine::iCodeTouchePressee = 0;      // code touche pressee
int                     _chaine::iCodeTouchePressee2 = 0;     // code touche pressee precedement
int                     _chaine::iCodeToucheRelachee = 0;     // code tocuhe relachee
int                     _chaine::iTempoClavier = 0;
unsigned char           _chaine::pCycle[]={64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64, 0} ;

// METHODES STATIQUES ==========================================================


// Convertit un nombre en chaine ASCII ----------------------------------------
// entree => _iNombre   :       nombre a convertir
//                              => _pASCII      : pointeur sur une chaine ASCII (terminee par un zero)
void _chaine::entierEnASCII(int _iNombre, char *_pASCII)
{
  int _iIndex = 0;
  int _iTaille = 1;
  int _iReste = strlen(_pASCII) - 1;

  for(_iIndex = 0; _iIndex < _iReste; _iIndex++)
  {
    _iTaille = _iTaille * 10;
  }
  _iIndex = 0;
  while (_iTaille > 0)
  {
    int _iReste = _iNombre / _iTaille;
    char _cASCII = (char)('0' + _iReste);

    if(_cASCII > '9' || _cASCII < '0')
      _cASCII = '9';
    _pASCII[_iIndex++] = _cASCII;
    _iNombre = _iNombre - (_iTaille * _iReste);
    _iTaille = _iTaille / 10;
  }
}

// donne le code touche pressee (appellee depuis KeyMap.cpp) --------------------
void _chaine::setCodeTouchePressee(int _iCodeTouchePressee)
{
  iCodeTouchePressee = _iCodeTouchePressee;
  iTempoClavier = 0;
}

// donne le code touche relachee (appellee depuis KeyMap.cpp) --------------------
void _chaine::setCodeToucheRelachee(int _iCodeToucheRelachee)
{
  iCodeToucheRelachee = _iCodeToucheRelachee;
  if(iCodeToucheRelachee == iCodeTouchePressee)
    iCodeTouchePressee = 0;
  {
    iTempoClavier = 0;
    iCodeTouchePressee2 = 0;
  }
}

// *****************************************************************************
// METHODES
// *****************************************************************************

// Constructeurs ---------------------------------------------------------------
        _chaine::_chaine()
{
  razMembres();
}
        _chaine::_chaine(char *_pChaine, int _iPositionX, int _iPositionY,
                 int _iOffsetH)
{
  razMembres();
  initialise(_pChaine, _iPositionX, _iPositionY, _iOffsetH, 0);
}

// Initialise tous les membres a zero ------------------------------------------
void _chaine::razMembres()
{
  iAngle = 0;
  iAngleInc = 1;
  iCentreX = 0;
  iCentreY = 0;
  iCurseurPosition = 0;
  iIndexCycle = 0;
  iNombreCaracteres = 0;
  iOffsetH = 16;
  iPositionX = 0;
  iPositionY = 0, iRayonX = 0;
  iRayonY = 0;
  iTempoClavier = 0;
  iTempoCycle = 0;
  pCaracteres = (_caractere *)NULL;
  pAscii = (char *)NULL;
  iCurseurAction = 0;
  pAscii_allocted = 0;
  pCaracteres = 0;
  pAscii = 0;
}

// Destructeur -----------------------------------------------------------------
_chaine::~_chaine()
{
  if(pCaracteres != 0)
    delete [] pCaracteres;
  //printf("_chaine::~_chaine() %i %s 2\n", pAscii_allocted, pAscii);
  if(pAscii && pAscii_allocted)
    delete [] pAscii;
  pCaracteres = (_caractere *)NULL;
  pAscii = (char *)NULL;
  pAscii_allocted = 0;
}

// Retourne le nombre de caracteres  -------------------------------------------
int _chaine::getNombreCaracteres()
{
  return iNombreCaracteres;
}

// Retourne le pointeur sur le tableau des objets '_caractere' -----------------
_caractere             *_chaine::getCaracteres()
{
  return pCaracteres;
}

// Initialise une nouvelle chaine de caracteres --------------------------------
void _chaine::initialise(char *_pChaine, int _iPositionX, int _iPositionY,
                         int _iOffsetH, int _iTypeFonte)
{
	iNombreCaracteres = strlen(_pChaine);
	if(pAscii == (char *)0x0) {
		pAscii_allocted = 1;
		pAscii = new char[iNombreCaracteres + 1];
	}
	strcpy(pAscii, _pChaine);
	init(_iPositionX, _iPositionY, _iOffsetH, _iTypeFonte);
}

// Initialise une nouvelle chaine de caracteres --------------------------------
void _chaine::initialise(char *_pChaine, int _iNombreCaractere,
                         int _iPositionX, int _iPositionY, int _iOffsetH,
                         int _iTypeFonte)
{
  iNombreCaracteres = _iNombreCaractere;
  pAscii = _pChaine;
  init(_iPositionX, _iPositionY, _iOffsetH, _iTypeFonte);
}

// Initialise une nouvelle chaine de caracteres --------------------------------
void _chaine::init(int _iPositionX, int _iPositionY, int _iOffsetH,
                   int _iTypeFonte)
{
	iCurseurPosition = 0;
	iPositionX = _iPositionX;
	iPositionY = _iPositionY;
	iCentreX = _iPositionX;
	iCentreY = _iPositionY;
	iOffsetH = _iOffsetH;
	if(pCaracteres == (_caractere *) 0x0)
		pCaracteres = new _caractere[iNombreCaracteres];
	for(int _iIndex = 0; _iIndex < iNombreCaracteres; _iIndex++)
	{	_caractere *_oCaractere = &pCaracteres[_iIndex];
		char _cAscii = pAscii[_iIndex];
		_oCaractere->initialise_caractere(_cAscii,
			iPositionX + _iIndex * iOffsetH,
			iPositionY, _iTypeFonte);
	}
}

void _chaine::initialiseChaine(char *_pChaine)
{
  pAscii = _pChaine;
}

// Cette methode reinitialise la chaine de caracteres en caracteres affichables --
void _chaine::initialiseSprite()
{
  for(int _iIndex = 0; _iIndex < iNombreCaracteres; _iIndex++)
  {
    _caractere             *_oCaractere = &pCaracteres[_iIndex];
    char _cAscii = pAscii[_iIndex];

    _oCaractere->convertit_caractere(_cAscii);
  }
}

// Cette methode reinitialise la chaine de caracteres en caracteres affichables --
void _chaine::initialiseSpritePos()
{
  for(int _iIndex = 0; _iIndex < iNombreCaracteres; _iIndex++)
  {
    _caractere             *_oCaractere = &pCaracteres[_iIndex];

    _oCaractere->changePosition(iPositionX + _iIndex * iOffsetH, iPositionY);
  }
}

// Retourne l'abscisse de la chaine -----------------------------------------------
int _chaine::getPositionX()
{
  return iPositionX;
}

// Retourne l'ordonnee de la chaine -----------------------------------------------
int _chaine::getPositionY()
{
  return iPositionY;
}

// Donne la position du curseur ------------------------------------------------
void _chaine::setCurseurPosition(int _iCurseurPosition)
{
  iCurseurPosition = _iCurseurPosition;
  if(iCurseurPosition < 0)
    iCurseurPosition = 0;
  if(iCurseurPosition > iNombreCaracteres - 1)
    iCurseurPosition = iNombreCaracteres - 1;
}

// Retourne la position du curseur ---------------------------------------------
int _chaine::getCurseurPosition() const
{
  return iCurseurPosition;
}

// Entre une chaine de caractere au clavier ------------------------------------
// sortie <= 1 : _iCodeTouchePressee
int _chaine::entreeClavier()
{
  int _iCodeTouchePressee = 0;

  if(iTempoClavier < 1)
  {
    _iCodeTouchePressee = iCodeTouchePressee;
    if(iCodeTouchePressee > 0)
    {
      if(iCodeTouchePressee2 != iCodeTouchePressee) // 1ere fois pressee ?
      {
iCodeTouchePressee2 = iCodeTouchePressee;
        iTempoClavier = 2000;
      }
      else
      { // L'utilisateur avertit se doit de controler la vitesse de  frappe des caracteres
        iTempoClavier = 60;
      }
    }
  }
  else
  {
    _iCodeTouchePressee = 0;
    iTempoClavier--;
  }
  entreeClavier(_iCodeTouchePressee);
  return _iCodeTouchePressee;
}

// Entre une chaine de caractere au clavier ------------------------------------
// entree => _iCodeTouchePressee : code de la touche clavier pressee
void _chaine::entreeClavier(int _iCodeTouchePressee)
{
  int _iIndex;
  _caractere *_oCaractere;

  iCurseurAction = 0;
  // Affiche le curseur clignotant
  unsigned char _cCouleur = getCouleur();
  iAngleInc = 1;
  iAngle = (iAngle + iAngleInc) & iANGLEMAXI;
  float _fSinus = pSin[iAngle];
  int _iTailleCurseur = (int)(_fSinus * iOffsetH / 2);
  if(_iTailleCurseur < 0)
    _iTailleCurseur = -_iTailleCurseur;
  _iTailleCurseur++;
  int _iPosX =
    iPositionX + (iCurseurPosition * iOffsetH) + iOffsetH / 2 -
    _iTailleCurseur;
  int _iPosY = iPositionY + iOffsetH / 2 - _iTailleCurseur;
  _iTailleCurseur += _iTailleCurseur;;
  _putRectangleVide(ecran_ram, _iPosX, _iPosY - 2, _cCouleur, _iTailleCurseur,
                    _iTailleCurseur);

  // Test la touche pressee
  switch (_iCodeTouchePressee)
  {
#ifdef SDL_TLK
    case SDLK_LEFT:
#else
    case XK_Left:
#endif
      iCurseurPosition--;
      break;
#ifdef SDL_TLK
    case SDLK_RIGHT:
#else
    case XK_Right:
#endif
      iCurseurPosition++;
      break;
      // Test de la touche 'backspace'
#ifdef SDL_TLK
    case SDLK_BACKSPACE:
#else
    case XK_BackSpace:
#endif
      /*for(_iIndex = iCurseurPosition; _iIndex < iNombreCaracteres; _iIndex++)
      { pAscii[_iIndex - 1] = pAscii[_iIndex];
      }*/

	  int j;
	  if (iCurseurPosition > 0)
		  j = iCurseurPosition;
	  else
		  j = 1;

	  for(_iIndex = j; _iIndex < iNombreCaracteres; _iIndex++)
      {
		  pAscii[_iIndex - 1] = pAscii[_iIndex];
      }

      pAscii[iNombreCaracteres - 1] = ' ';
      iCurseurPosition--;
      initialiseSprite();
      break;
      // Test de la touche 'delete'
#ifdef SDL_TLK
    case SDLK_DELETE:
#else
    case XK_Delete:
#endif
      for(_iIndex = iCurseurPosition; _iIndex < iNombreCaracteres - 2;	// - 1
          _iIndex++)
      { pAscii[_iIndex] = pAscii[_iIndex + 1];
      }
      pAscii[iNombreCaracteres - 1] = ' ';
      initialiseSprite();
      break;
#ifdef SDL_TLK
    case SDLK_RETURN:
      break;                                                   //ASCII code=13
    case SDLK_UP:
      break;
    case SDLK_DOWN:
      break;
    case SDLK_LSHIFT:
      break;
    case SDLK_RSHIFT:
      break;
    case SDLK_LCTRL:
      break;
    case SDLK_RCTRL:
      break;
    case SDLK_F1:
      break;
    case SDLK_F2:
      break;
    case SDLK_F3:
      break;
    case SDLK_F4:
      break;
    case SDLK_F5:
      break;
    case SDLK_F6:
      break;
    case SDLK_F7:
      break;
    case SDLK_F8:
      break;
    case SDLK_F9:
      break;
    case SDLK_F10:
      break;
    case SDLK_F11:
      break;
    case SDLK_F12:
      break;
#else
    case XK_Return:
      break;                                                   //ASCII code=13
    case XK_Up:
      break;
    case XK_Down:
      break;
    case XK_Shift_L:
      break;
    case XK_Shift_R:
      break;
    case XK_Control_L:
      break;
    case XK_Control_R:
      break;
    case XK_F1:
      break;
    case XK_F2:
      break;
    case XK_F3:
      break;
    case XK_F4:
      break;
    case XK_F5:
      break;
    case XK_F6:
      break;
    case XK_F7:
      break;
    case XK_F8:
      break;
    case XK_F9:
      break;
    case XK_F10:
      break;
    case XK_F11:
      break;
    case XK_F12:
      break;
#endif
    default:
      _iCodeTouchePressee = _iCodeTouchePressee & 127;
      if(_iCodeTouchePressee >= ' ' && _iCodeTouchePressee <= 'z')
      {
#ifndef _GP2X
        // Don't scroll characters if running on the GP2X - overwrite them
        // instead
        for(_iIndex = iNombreCaracteres - 1; _iIndex > iCurseurPosition;
            _iIndex--)
        { pAscii[_iIndex] = pAscii[_iIndex - 1];
        }
#endif
        pAscii[iCurseurPosition] = _iCodeTouchePressee;
        _oCaractere = &pCaracteres[iCurseurPosition];
#ifndef _GP2X
        // Don't move the cursor if running on the GP2X - we manually move it
        // with the joystick
        iCurseurPosition++;
#endif
        initialiseSprite();
      }
  }
  // Test les depassements position curseur
  if(iCurseurPosition < 0)
  { iCurseurPosition = 0;
    iCurseurAction = 1;                                        //
  }
  if(iCurseurPosition > iNombreCaracteres - 1)
  {
    iCurseurPosition = iNombreCaracteres - 1;
    iCurseurAction = 2;
  }
  _oCaractere = &pCaracteres[iCurseurPosition];
  _oCaractere->debuteAnimation();
}

// Retourne une des 9 couleurs du cycle  ---------------------------------------
unsigned char _chaine::getCouleur()
{
  unsigned char _cCouleur;
  if(iTempoCycle++ > 1)                                        // On change la couleur ? (temporisation)
  { iTempoCycle = 0;                                           // oui, remet la temporisation a zero
    iIndexCycle++;                                             // augmente l'index sur la table des couleurs
    _cCouleur = 100;
    if(_cCouleur == 0)                                         // code special 0=fin de la table
    { _cCouleur = pCycle[0];
      iIndexCycle = 0;
    }
  }
  else
  { _cCouleur = pCycle[iIndexCycle];
  }
  return _cCouleur;
}

// Rotation de la chaine a
// sortie <= 0=rotation non terminee / 1=rotation terminee
int _chaine::rotation1(int _iDecX, int _iDecY, int _iMin)
{
  int _iRetour = 0;

  iAngle = (iAngle + iAngleInc) & iANGLEMAXI;
  float _fSin = pSin[iAngle];
  float _fCos = pCos[iAngle];

  iPositionX = (int)(_fSin * iRayonX) + iCentreX;
  iPositionY = (int)(_fCos * iRayonY) + iCentreY;
  if(iRayonX > _iMin)
    iRayonX -= _iDecX;
  if(iRayonY > _iMin)
    iRayonY -= _iDecY;
  if(iRayonX < _iMin)
    iRayonX = _iMin;
  if(iRayonY < _iMin)
    iRayonY = _iMin;
  if(iRayonX == _iMin && iRayonY == _iMin)
    _iRetour = 1;
  return _iRetour;
}

// Rotation de la chaine autour de son centre avec elargissement du rayon
// sortie <= 0=rotation non terminee / 1=rotation terminee
int _chaine::rotation2(int _iIncX, int _iIncY, int _iMaxX, int _iMaxY)
{
  int _iRetour = 0;

  iAngle = (iAngle + iAngleInc) & iANGLEMAXI;
  float _fSin = pSin[iAngle];
  float _fCos = pCos[iAngle];

  iPositionX = (int)(_fSin * iRayonX) + iCentreX;
  iPositionY = (int)(_fCos * iRayonY) + iCentreY;
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
  return _iRetour;
}

void _chaine::initRotation(int _iAngle, int _iAngleInc, int _iRayonX,
                           int _iRayonY)
{
  iAngle = _iAngle;
  iAngleInc = _iAngleInc;
  iRayonX = _iRayonX;
  iRayonY = _iRayonY;
}

// Affiche les sprites caracteres  ---------------------------------------------
void _chaine::affiche()
{
  for(int _iIndex = 0; _iIndex < iNombreCaracteres; _iIndex++)
  { _caractere *_oCaractere = &pCaracteres[_iIndex];
    _oCaractere->affiche();
  }
}

// Affiche les sprites caracteres en ajustant leurs positions ------------------
void _chaine::affiche2()
{
  for(int _iIndex = 0; _iIndex < iNombreCaracteres; _iIndex++)
  { _caractere *_oCaractere = &pCaracteres[_iIndex];
    _oCaractere->changePosition(iPositionX + _iIndex * iOffsetH, iPositionY);
    _oCaractere->affiche();
  }
}

// Animation des caracteres ----------------------------------------------------
void _chaine::animationTous()
{
  for(int _iIndex = 0; _iIndex < iNombreCaracteres; _iIndex++)
  { _caractere *_oCaractere = &pCaracteres[_iIndex];
    _oCaractere->animation();
  }
}

// Change les coordonnees de tous les caracteres  ------------------------------
void _chaine::changePosition(int _iPosX, int _iPosY)
{
  for(int _iIndex = 0; _iIndex < iNombreCaracteres; _iIndex++)
  { _caractere *_oCaractere = &pCaracteres[_iIndex];
    _oCaractere->changePosition(_iPosX, _iPosY);
    _iPosX += iOffsetH;
  }
}

// Decremente l'ordonnee de tous les caracteres  -------------------------------
void _chaine::changePositionX(int _iPosX)
{
  for(int _iIndex = 0; _iIndex < iNombreCaracteres; _iIndex++)
  { _caractere *_oCaractere = &pCaracteres[_iIndex];
    _oCaractere->changePositionX(_iPosX);
    _iPosX += iOffsetH;
  }
}

// Decremente l'ordonnee de tous les caracteres  -------------------------------
void _chaine::changePositionY(int _iPosY)
{
  for(int _iIndex = 0; _iIndex < iNombreCaracteres; _iIndex++)
  { _caractere *_oCaractere = &pCaracteres[_iIndex];
    _oCaractere->changePositionY(_iPosY);
  }
}

// Animation des caracteres une seule fois -------------------------------------
// sortie <= 0 : plus aucun caractere est anime
int _chaine::animation2Tous()
{
  int _iFin = 0;
  for(int _iIndex = 0; _iIndex < iNombreCaracteres; _iIndex++)
  { _caractere *_oCaractere = &pCaracteres[_iIndex];
   if(_oCaractere->animation2() != 0)
      _iFin = 1;
  }
  return _iFin;
}

int _chaine::getCurseurAction()
{
  return iCurseurAction;
}

//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "score.cpp"
// created      : 1998-06-29
// updates      : 2005-01-07
// id           : $Id: score.cpp,v 1.5 2005/01/31 09:44:33 patriceduhamel Exp $
//-----------------------------------------------------------------------------
// descriptions : manage scores / about / credits
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
//*****************************************************************************
#include "powermanga.hpp"
#include "score.hpp"
#include "configfile.hpp"
//#ifdef SDL_TLK
#include <SDL/SDL_endian.h>
//#else
//#include <X11/keysym.h>
//#include <X11/keysymdef.h>
//#endif
#include <errno.h>
//..............................................................................
extern configfile *power_conf;
extern const char *nomfichierscore[];
//..............................................................................
// Scores
char
_score::cScoresNoms[iNOMBRE_SCORES][4] = { {'T', 'L', 'K', 0},
{'J', 'M', 'M', 0},
{'E', 'T', 'B', 0},
{'D', 'A', 'V', 0},
{'F', 'L', 'O', 0}
};

//..............................................................................
Sint32
_score::iScores[iNOMBRE_SCORES] = { 10, 90, 80, 7, 5 };
_chaine *
  _score::pScoresObj = (_chaine *) 0x0;
_chaine *
  _score::pChaineJoueur = (_chaine *) 0x0;	// objet de type '_chaine' reservee a l'entree du nom du joueur
_chaine *
  _score::pGame = (_chaine *) 0x0;	// objet de type '_chaine'
_chaine *
  _score::pOver = (_chaine *) 0x0;	// objet de type '_chaine'
Sint32
  _score::iAction = 0;
Sint32
  _score::iActionExecution = 0;
Uint32
  _score::iTempo = 0;		// tempo durant lequel le score reste affiche
Sint32
  _score::iLettreAnime = 0;	// numero de la lettre anime
_caractere *
  _score::pLettreActuelle = (_caractere *) 0x0;
_caractere *
  _score::pTexteAbout = (_caractere *) 0x0;
Uint32
  _score::iAnimIndex = 0;	// Numero du caractere a anime
Uint32
  _score::iAboutNombreCaracteres = 0;
Uint32
  _score::iIndexAboutTable = 0;
Uint32
  _score::iAnimIndexTempo = 0;
_caractere *
  _score::pTableau[iTOTALCARACTERES];
_chaine *
  _score::pTxtChaines = 0x0;
Sint32
  _score::iTxtPosX = 0;
Uint32
  _score::iTxtPosY = 0;
char *
  _score::pTxtTableau = 0x0;
Uint16 *
  _score::pTableCommande = 0x0;
Uint32
  _score::iTableIndex = 0;	// index sur le tableau pointee par 'pTableCommande'
Sint32
  _score::iTableTempo = 0;	// temporisation

Uint16
  _score::last_valeur = 0;

//..............................................................................
char
  _score::pScoreTempo[32] = { 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};


// --> DisplayFonte.cpp
//char _score::pGameTexte[] = { "GAME" };
//char _score::pOverTexte[] = { "OVER" };
extern char
  pASCIIgame[32];
extern char
  pASCIIover[32];


//..............................................................................
char **
  _score::pTableAbout = NULL;
char *
  _score::pTextData = NULL;
Sint32
  _score::iSizeTableAbout = 0;

// Variables declaree dans le fichier "powermanga.cpp"
extern Sint32
  player_score;			//player's score
extern Sint32
  pos_y_menu;			// position Y d'affichage du menu.
extern Uint32
  iCodeTouche;			// code touche pressee
extern Sint32
  etat_menu;			// indique l'etat du menu, on, off, apparition, disparition.
extern _etbfonte1 *
  pEtbfonte1;			// objet '_etbfonte1' gestion affichage de caractere 8x8
extern bool
  aff_game_over;		// 1=display "GAME OVER"


//------------------------------------------------------------------------------
// load about text file
//
// format:
//
//  0,MAIN,       0,PROGRAMMING, 1,JEAN MICHEL, 1,MARTIN
//  0,LINUX,      0,VERSION,     1,BRUNO,       1,ETHVIGNOT
//------------------------------------------------------------------------------
Sint32
_score::loadTextAbout ()
{
  //##############################################################
  // load file text
  //##############################################################
  char *pLang = power_conf->get_txtlang ();
  char fname[256];
  sprintf (fname, "texts/about_%s.txt", pLang);
  Uint32 bsize;
  char *pData = chargeFichier (fname, &bsize);
  if (!pData)
    return 0;

  //##############################################################
  // determine memory size of the strings
  //##############################################################
  iSizeTableAbout = 0;		//number of strings
  Uint32 iText = 0;		//size of the strings in bytes
  Uint32 index = 0;		//file index
  Uint32 nword = 0;		//number of words
  while (index < bsize)
    {
      switch (pData[index])
	{
	case ',':
	  if (nword & 1)
	    iText++;
	  nword++;
	  break;
	case '\n':
		iSizeTableAbout += ((nword + 1) / 2) + 1;
		iText += 3;
		nword = 0;
		break;
	default:
	  if (pData[index] >= ' ')
	    {
	      if (nword & 1)
		iText++;
	      else
		{
		  if (pData[index] != ' ')
		    iText++;
		}
	    }
	  break;
	}
      index++;
  }

  iText += 2;
  iSizeTableAbout += 2;

  //##############################################################
  // allocate memory
  //##############################################################
  pTextData = reserveMemoire (bsize);
  if (!pTextData)
    return 0;
  pTableAbout = (char **) reserveMemoire (iSizeTableAbout * sizeof (char *));

  //##############################################################
  // create the strings
  //##############################################################
  Uint32 idTab = 0;
  nword = 0;
  index = 0;
  iText = 0;
  pTableAbout[idTab++] = &pTextData[0];
  while (index < bsize)
    {
      switch (pData[index])
	{
	case ',':
	  if (nword & 1)
	    {
	      pTextData[iText++] = '\0';
	      pTableAbout[idTab++] = &pTextData[iText];
	    }
	  nword++;
	  break;

	case '\n':
	  pTextData[iText++] = '\0';
	  pTableAbout[idTab++] = &pTextData[iText];
	  pTextData[iText++] = '@';
	  pTextData[iText++] = '\0';
	  pTableAbout[idTab++] = &pTextData[iText];
	  nword = 0;
	  break;

	default:
	  if (pData[index] >= ' ')
	    {
	      if (nword & 1)
		pTextData[iText++] = pData[index];
	      else
		{
		  if (pData[index] != ' ')
		    pTextData[iText++] = pData[index];

		}
	    }
	  break;
	}
      index++;
  }


  // end of text
  pTableAbout[idTab++] = &pTextData[iText];
  pTextData[iText++] = '#';
  pTextData[iText++] = '\0';

  libereMemoire (pData);

  return 1;
}


//******************************************************************************
// Detruit le texte du menu about
//******************************************************************************
void
_score::freeTextAbout ()
{
  if (pTableAbout)
    {
      libereMemoire ((char *) pTableAbout);
      pTableAbout = NULL;
    }
  if (pTextData)
    {
      libereMemoire (pTextData);
      pTextData = NULL;
    }
}


//******************************************************************************
// Chargement du texte du menu order
//******************************************************************************
Sint32
_score::loadTextOrder ()
{

  //##############################################################
  // load file text
  //##############################################################
  char *pLang = power_conf->get_txtlang ();
  char fname[256];
  sprintf (fname, "texts/order_%s.txt", pLang);
  Uint32 bsize;
  char *pData = chargeFichier (fname, &bsize);
  if (!pData)
    return 0;

  _score::pTableCommande = (Uint16 *) reserveMemoire (bsize * 2 * sizeof (Uint16));
  if (!pTableCommande)
    return 0;
  memset (pTableCommande, 0, bsize * 2 * sizeof (Uint16));

  Uint32 zePos = 0;
  for (Uint32 i = 0; i < bsize; i++)
    {
      if (pData[i] == '"')
	continue;
      if ((pData[i] == 10) || (pData[i] >= ' '))
	{
	  pTableCommande[zePos++] = (unsigned char) pData[i];
	  pTableCommande[zePos++] = iTXT_PAUSE;
	}
    }
  libereMemoire (pData);
  return 1;
}

//******************************************************************************
// detruit le texte du menu order
//******************************************************************************
void
_score::freeTextOrder ()
{
  if (pTableCommande)
    {
      libereMemoire ((char *) pTableCommande);
      pTableCommande = NULL;
    }

}



//******************************************************************************
// Retourne le type d'action en cours
//******************************************************************************
Sint32
_score::getActionExecution ()
{
  return iActionExecution;
}

//******************************************************************************
// Routine d'execution principale / main loop
// input => _iFlag      : 0 main menu's disable (etat_menu==MENU_OFF)
//                      : 1 main menu's enable
//******************************************************************************
void
_score::execution (Sint32 _iFlag)
{
  switch (iActionExecution)
    {
      // "Game Over" : le joueur a perdu son dernier vaisseau
    case 1:
      executeGameOver (_iFlag);
      break;

      // affiche la table des scores : le joueur presse ESC dans le menu
    case 2:
      executeTableScore (_iFlag);
      break;

      // affiche les 'abouts' : qui fait quoi
    case 3:
      executeAbout (_iFlag);
      break;

      // affiche le 'order' : comment commander
    case 4:
      executeOrder (_iFlag);
      break;

    }
}

//******************************************************************************
// Initialize
// input => _iCodeAction : 1 = GAME OVER
//                         2 = SCORES
//                         3 = ABOUT
//                         4 = ORDER
//******************************************************************************
Sint32
_score::setAction (Sint32 _iCodeAction)
{
  switch (_iCodeAction)
    {
    case 0:
      {
	iActionExecution = 0;
	iAction = 0;
	break;
      }
      // initialize the 'GAME OVER' ----------
    case 1:
      {
	chargeScores ();
	Sint32 _iPosition = trieScores ();	// _iPosition=position du joueur dans la table des scores
	if (_iPosition == -1)
	  iTempo = iTEMPOTABLE;	// si pas de classement la table apparait un instant
	else
	  iTempo = 0;		//no tempo: player name input
	creeScores ();
	initialiseGameOver (_iPosition);
	iActionExecution = 1;
	iAction = 1;
      }
      break;

      // initialize the 'SCORE TABLE' --------
    case 2:
      chargeScores ();
      creeScores ();
      iActionExecution = 2;
      iAction = 0;
      break;

      // initialize the 'ABOUT' --------------
    case 3:
      creeAbout ();
      iActionExecution = 3;
      iAction = 0;
      break;

      // initialize the 'ORDER' --------------
    case 4:
      if (!creeOrder ())
	return 0;
      iActionExecution = 4;
      break;
    }
  return 1;
}



//******************************************************************************
// test if current input string
//******************************************************************************
Sint32
_score::isInputName ()
{
  if (pChaineJoueur)
    return 1;
  else
    return 0;
}

//******************************************************************************
// release objects
//******************************************************************************
void
_score::destroy ()
{
  if (pScoresObj)
    delete[]pScoresObj;
  if (pGame)
    delete pGame;
  if (pOver)
    delete pOver;
  if (pTxtChaines)
    delete[]pTxtChaines;
  if (pTexteAbout)
    delete[]pTexteAbout;

  if (pTxtTableau)
	  delete [] pTxtTableau;

  pGame = (_chaine *) NULL;
  pOver = (_chaine *) NULL;
  pScoresObj = (_chaine *) NULL;
  pTxtChaines = NULL;
  pTxtTableau = NULL;
  pTexteAbout = NULL;
}


//=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=
// GAME OVER : initialize / initialise
//  - trie des scores
//  - affiche table des scores
//  - affiche "Game Over"
//=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=
Sint32
_score::initialiseGameOver (Sint32 _iPosition)
{
  // Cree deux objets chaine pour les textes 'GAME' et 'OVER'
  if (pGame == (_chaine *) 0x0)
    pGame = new _chaine ();
  if (pOver == (_chaine *) 0x0)
    pOver = new _chaine ();
  //pGame->initialise(pGameTexte, 4, 182, POS_Y_GAME, 38, 1);
  //pOver->initialise(pOverTexte, 4, 182, POS_Y_OVER, 38, 1);
  //256 - 38 * 4 / 2 + 128 = 180 (182 a l'orgine)

  Sint32 l = 32;
  Sint32 x = (LARG_ECR_RAM - l * strlen (pASCIIgame)) / 2 + DEBUT_ECRAN_X;
  pGame->initialise (pASCIIgame, strlen (pASCIIgame), x, POS_Y_GAME, l, 1);
  x = (LARG_ECR_RAM - l * strlen (pASCIIover)) / 2 + DEBUT_ECRAN_X;
  pOver->initialise (pASCIIover, strlen (pASCIIover), x, POS_Y_OVER, l, 1);
  pGame->initRotation (64, 0, 0, 120);
  pOver->initRotation (0, 0, 0, 120);

  //sort the scores
  pChaineJoueur = (_chaine *) NULL;	// pointeur sur la chaine a rentrer

  // Initialise l'arrivee rotative des noms et des scores
  Sint32 _iIndexScore = 0;
  Sint32 _iAngle = 0;
  Sint32 _iAngleInc = 1;
  Sint32 _iRayonX = 200;
  Sint32 _iRayonY = 160;
  Uint32 _iIndex1 = 0;

  // initialise les noms
  for (_iIndex1 = 0; _iIndex1 < iNOMBRE_SCORES; _iIndex1++)
    {
      _chaine *_pChaine = &pScoresObj[_iIndexScore++];
      _pChaine->initRotation (_iAngle, _iAngleInc, _iRayonX, _iRayonY);
      if (_iPosition == (int) (_iIndex1))
	pChaineJoueur = _pChaine;
      _iAngle = (_iAngle + 1) & iANGLEMAXI;
      _iAngleInc = -_iAngleInc;
    }

  // initialise les scores
  for (_iIndex1 = 0; _iIndex1 < iNOMBRE_SCORES; _iIndex1++)
    {
      _chaine *_pChaine = &pScoresObj[_iIndexScore++];
      _pChaine->initRotation (_iAngle, _iAngleInc, _iRayonX, _iRayonY);
      _iAngle = (_iAngle + 1) & iANGLEMAXI;
      _iAngleInc = -_iAngleInc;
    }
  return 1;
}

// ------------------------------------------------------------------------------
// cree les chaines de caractere des scores -------------------------------------
// ------------------------------------------------------------------------------
Sint32
_score::creeScores ()
{
  Sint32 _iIndexScore = 0;
  if (pScoresObj == 0x0)
    pScoresObj = new _chaine[iNOMBRE_SCORES * 2];	// Cree les objets '_chaine'
  Sint32 _iPositionY = iTABLEY;
  Uint32 _iIndex1;
  Sint32 _iIndexTableau = 0;
  Sint32 _iIndex3;

  // Convertit les noms en sprites
  for (_iIndex1 = 0; _iIndex1 < iNOMBRE_SCORES; _iIndex1++)
    {
      _chaine *_pChaine = &pScoresObj[_iIndexScore++];
      char *_pNom = &cScoresNoms[_iIndex1][0];

      _pChaine->initialise (_pNom, iLONGEURNOM, iTABLEX1, _iPositionY, 16, 0);
      _iPositionY += iTABLEOFFSET;

      // Recopie les pointeurs sur les objets caracteres dans un tableau
      Sint32 _iNombre = _pChaine->getNombreCaracteres ();
      _caractere *_pCaracteres = _pChaine->getCaracteres ();
      for (_iIndex3 = 0; _iIndex3 < _iNombre; _iIndex3++)
	pTableau[_iIndexTableau++] = &_pCaracteres[_iIndex3];

    }

  // Convertit les scores en sprites
  _iPositionY = iTABLEY;
  for (_iIndex1 = 0; _iIndex1 < iNOMBRE_SCORES; _iIndex1++)
    {
      _chaine *_pChaine = &pScoresObj[_iIndexScore++];

      Sint32 _iScore = iScores[_iIndex1];

      for (_iIndex3 = 0; _iIndex3 <= 7; _iIndex3++)
	pScoreTempo[_iIndex3] = '0';

      entierEnASCII (_iScore, pScoreTempo);

      _pChaine->initialise (pScoreTempo, iTABLEX2, _iPositionY,
			    LARG_FONTE_GAME, 0);
      _iPositionY += iTABLEOFFSET;


      // Recopie les pointeurs sur les objets caracteres dans un tableau

      Sint32 _iNombre = _pChaine->getNombreCaracteres ();

      _caractere *_pCaracteres = _pChaine->getCaracteres ();

      for (_iIndex3 = 0; _iIndex3 < _iNombre; _iIndex3++)
	pTableau[_iIndexTableau++] = &_pCaracteres[_iIndex3];
    }

  return 1;
}

// ------------------------------------------------------------------------------
// affiche les chaines de caractere des scores
// <=   _iRetour : 1=touche entree pressee
// ------------------------------------------------------------------------------
Sint32
_score::affScores ()
{
  Sint32 _iRetour = 0;
  Sint32 _iRotation = 1;
  Uint32 _iIndex1 = 0;
  for (_iIndex1 = 0; _iIndex1 < iNOMBRE_SCORES * 2; _iIndex1++)
    {
      _chaine *_pChaine = &pScoresObj[_iIndex1];
      if (_pChaine->rotation1 (2, 2, 0) == 0)
	_iRotation = 0;		// _iRotation=1 alors rotation terminee
    }
  // Gestion entree des caracteres + clignotement du curseur
  if (pChaineJoueur != 0 && _iRotation == 1)
    {
      _iRetour = pChaineJoueur->entreeClavier ();
      //   RETURN pressed ? (ASCII=13)
#ifdef SDL_TLK
      if (_iRetour == SDLK_RETURN)
#else
      if (_iRetour == XK_Return)
#endif
	{
	  pChaineJoueur = (_chaine *) 0x0;
	  sauveScores ();
	  _iRetour = 1;
	}
      else
	{
	  _iRetour = 0;
	}
    }
  afficheScores ();
  return _iRetour;
}

// ------------------------------------------------------------------------------
// Affichage de la table des scores
// ------------------------------------------------------------------------------
void
_score::afficheScores ()
{
  Uint32 _iIndex1;
  for (_iIndex1 = 0; _iIndex1 < iNOMBRE_SCORES * 2; _iIndex1++)
    {
      _chaine *_pChaine = &pScoresObj[_iIndex1];
      _pChaine->affiche2 ();
    }
}


// Trie des scores  -----------------------------------------------------------
// Sortie <=    position dans la table des scores, de 0 a 4 ou -1 si non classe
Sint32
_score::trieScores ()
{
  Sint32 _iPosition = -1;

  for (Uint32 _iIndex = 0; _iIndex < iNOMBRE_SCORES; _iIndex++)
    {
      if (player_score > iScores[_iIndex])
	{
	  for (Uint32 _iIndex2 = iNOMBRE_SCORES - 1; _iIndex2 > _iIndex;
	       _iIndex2--)
	    {
	      iScores[_iIndex2] = iScores[_iIndex2 - 1];
	      cScoresNoms[_iIndex2][0] = cScoresNoms[_iIndex2 - 1][0];
	      cScoresNoms[_iIndex2][1] = cScoresNoms[_iIndex2 - 1][1];
	      cScoresNoms[_iIndex2][2] = cScoresNoms[_iIndex2 - 1][2];
	    }
	  iScores[_iIndex] = player_score;
	  cScoresNoms[_iIndex][0] = ' ';
	  cScoresNoms[_iIndex][1] = ' ';
	  cScoresNoms[_iIndex][2] = ' ';
	  _iPosition = _iIndex;
	  break;
	}
    }
  return _iPosition;
}

//------------------------------------------------------------------------------
// load score table
//------------------------------------------------------------------------------
Sint32
_score::chargeScores ()
{
  Sint32 _iHandle;
  struct stat sDescriptionFichier;
  char *_pMemoire = NULL;
#ifdef WIN32
  _iHandle = open (nomfichierscore[power_conf->difficulty],
		   O_RDONLY | O_BINARY, 0);
#else
  _iHandle = open (nomfichierscore[power_conf->difficulty], O_RDONLY, 0);
#endif
  if (_iHandle == -1)
    fprintf (stderr, "_score::chargeScores() %s: %s\n",
	     nomfichierscore[power_conf->difficulty], strerror (errno));
  else
    {
      if (fstat (_iHandle, &sDescriptionFichier))
	fprintf (stderr, "_score::chargeScores() %s: %s\n",
		 nomfichierscore[power_conf->difficulty], strerror (errno));
      else
	{
	  //allocate memory for the file
	  _pMemoire = reserveMemoire (sDescriptionFichier.st_size);
	  if (!_pMemoire)
	    fprintf (stderr, "_score::chargeScores() out of memory\n");
	  else
	    {
	      if (read (_iHandle, _pMemoire,
			sDescriptionFichier.st_size) !=
		  sDescriptionFichier.st_size)
		{
		  libereMemoire (_pMemoire);
		  _pMemoire = 0;
		  fprintf (stderr, "_score::chargeScores()%s: %s\n",
			   nomfichierscore[power_conf->difficulty],
			   strerror (errno));
		}
	      else
		{
#ifdef _VERBEUX_
		  if (power_conf->verbose)
		    fprintf (stdout,
			     "_score::chargeScores()"
			     "file \"%s\" was "
			     "loaded in memory",
			     nomfichierscore[power_conf->difficulty]);
#endif
		}
	    }
	}
      close (_iHandle);
    }

  //##############################################################
  // copy data file into memory structure
  //##############################################################
    if (_pMemoire)
    {
        SDL_RWops * rwops = SDL_RWFromMem(_pMemoire, iTAILLEBUFFER);
        // Copy players names
        for (Uint32 _iIndex = 0; _iIndex < iNOMBRE_SCORES; _iIndex++)
        {
            SDL_RWread(rwops, &cScoresNoms[_iIndex], 1, 3);
        }
        // Copy players scores
        for (Uint32 _iIndex = 0; _iIndex < iNOMBRE_SCORES; _iIndex++)
        {
            Sint32 score;
            SDL_RWread(rwops, &score, 4, 1);
            score = SDL_SwapLE32(score);
            iScores[_iIndex] = score;
        }

        Sint32 _iSomme2;
        SDL_RWread(rwops, &_iSomme2, 4, 1);    // somme d'auto-controle du fichier
        _iSomme2 = SDL_SwapLE32(_iSomme2);
        Sint32 _iSomme =
            calculSomme ((Sint32 *) _pMemoire, (iTAILLEBUFFER / 4) - 1);

      //le fichier charge n'est pas bon (on cree une nouvelle table des scores)
        if (_iSomme2 != _iSomme) {
            creeNouvelleTable ();
        }
        SDL_FreeRW(rwops);
        libereMemoire (_pMemoire);
    }
    else
    {
        creeNouvelleTable ();	//la tables des scores n'a pu etre chargee
    }
    return 1;
}

// Creation d'une nouvelle table des scores  ----------------------------------
void
_score::creeNouvelleTable ()
{
  Uint32 _iIndex;
  Uint32 _iIndex2;
  char _pNom[iLONGEURNOM] = { 'T', 'L', 'K' };

  // Cree les noms
  for (_iIndex = 0; _iIndex < iNOMBRE_SCORES; _iIndex++)
    {
      for (_iIndex2 = 0; _iIndex2 < iLONGEURNOM; _iIndex2++)
	cScoresNoms[_iIndex][_iIndex2] = _pNom[_iIndex2];
    }
  // Cree les scores

  Sint32 _iScore = 10000;

  for (_iIndex = 0; _iIndex < iNOMBRE_SCORES; _iIndex++)
    {
      iScores[_iIndex] = _iScore;
      _iScore = _iScore / 2;
    }
}

// Sauvegarde des scores  -----------------------------------------------------
void
_score::sauveScores ()
{				//printf("%d %d \n", getgid(), getegid() ) ;
    char *_pBuffer = new char[iTAILLEBUFFER];
    SDL_RWops * rwops = SDL_RWFromMem(_pBuffer, iTAILLEBUFFER);
    for (Uint32 _iIndex = 0; _iIndex < iNOMBRE_SCORES; _iIndex++)
    {
        SDL_RWwrite(rwops, &cScoresNoms[_iIndex], 1, 3);
    }
    for (Uint32 _iIndex = 0; _iIndex < iNOMBRE_SCORES; _iIndex++)
    {
        Sint32 score = iScores[_iIndex];
        score = SDL_SwapLE32(score);
        SDL_RWwrite(rwops, &score, 4, 1);
    }
    Sint32 _iSomme = calculSomme ((Sint32 *) _pBuffer, (iTAILLEBUFFER / 4) - 1);
    _iSomme = SDL_SwapLE32(_iSomme);
    SDL_RWwrite(rwops, &_iSomme, 4, 1);

    Sint32 _iHandle;
#ifdef WIN32
    _umask (0002);		//set umask so that files are group-writable
#else
    //umask (0002);
#endif
    _iHandle = open (nomfichierscore[power_conf->difficulty], O_WRONLY | O_CREAT, 00666);	//ouvre le fichier
    if (_iHandle == -1)
    {
        printf ("%s: %s\n", nomfichierscore[power_conf->difficulty],
        strerror (errno));
    }
    else
    {
#ifdef WIN32
        _write (_iHandle, _pBuffer, iTAILLEBUFFER);
#else
        write (_iHandle, _pBuffer, iTAILLEBUFFER);
#endif
        if (close (_iHandle) != 0)
        {
            printf ("%s: %s\n", nomfichierscore[power_conf->difficulty],
            strerror (errno));
        }
    }
    SDL_FreeRW(rwops);
    delete[]_pBuffer;
}

// Calcul la somme d'auto-controle des scores ---------------------------------
// entree => _pBuffer   : pointeur sur le buffer
//                              => _iTaille     : nombre de mots de 32 bits
// sortie <= _iValeur   :       somme d'auto-controle
Sint32
_score::calculSomme (Sint32 * _pBuffer, Sint32 _iTaille)
{
  Sint32 _iValeur = 0;

  for (Sint32 _iIndex = 0; _iIndex < _iTaille; _iIndex++)
    _iValeur |= *(_pBuffer++);
  return _iValeur;
}

//--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--
// GAME OVER : apparition table des scores, entre le nom, affiche "GAME OVER"
// input => _iFlag      : 0 main menu's disable (etat_menu==MENU_OFF)
//                                                                      : 1 main menu's enable
//--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--
void
_score::executeGameOver (Sint32 _iFlag)
{
  switch (iAction)
    {				// Apparition des scores
    case 1:
      {
	Sint32 _iCode = affScores ();	// 1=le joueur a presse la touche RETURN

	if (iTempo == 0)	// si iTempo==0 alors le joueur entre son nom
	  {
	    if (_iCode == 1)	// touche RETURN pressee ?
	      {
		iAction = 2;	// oui, les scores vont disparaitres
	      }
	  }
	else
	  {
	    iTempo--;		// iTempo>0 : le joueur n'est pas dans les scores
	    if (iTempo == 0
		|| (iCodeTouche != 0 && iTempo < iTEMPOTABLE - 10))
	      iAction = 2;	// on affiche un court moment la table des scores
	  }

      }
      break;

      // Disparition des scores
    case 2:
      {
	Uint32 _iIndex1;
	Sint32 _iRotation = 1;

	for (_iIndex1 = 0; _iIndex1 < iNOMBRE_SCORES * 2; _iIndex1++)
	  {
	    _chaine *_pChaine = &pScoresObj[_iIndex1];

	    if (_pChaine->rotation2 (2, 2, 250, 210) == 0)
	      _iRotation = 0;	// _iRotation=1 alors rotation terminee
	  }
	if (_iRotation == 1)
	  iAction = 3;
	afficheScores ();
      }
      break;

      // Le "GAME OVER" apparait les lettres tournent toutes
    case 3:
      {
	Sint32 _iRotation = 1;

	afficheScores ();
	if (pGame->rotation1 (2, 2, 0) == 0)
	  _iRotation = 0;	// _iRotation=1 alors rotation terminee
	if (pOver->rotation1 (2, 2, 0) == 0)
	  _iRotation = 0;	// _iRotation=1 alors rotation terminee
	pGame->affiche2 ();
	pOver->affiche2 ();
	pGame->animationTous ();
	pOver->animationTous ();
	if (_iRotation == 1)
	  iAction = 4;
      }
      break;

      // Le "GAME OVER" est fixe les lettres tournent toutes encore une fois
    case 4:
      {
	Sint32 _iFin = 0;

	pGame->affiche2 ();
	pOver->affiche2 ();
	if (pGame->animation2Tous () != 0)
	  _iFin = 1;
	if (pOver->animation2Tous () != 0)
	  _iFin = 1;
	if (_iFin == 0)
	  iAction = 5;
	iLettreAnime = 7;
	_caractere *_pListe = pOver->getCaracteres ();

	pLettreActuelle = &_pListe[3];
	iTempo = 0;
      }
      break;


      // Le "GAME OVER" est fixe
    case 5:
      {
	{
	  if (pLettreActuelle->animation2 () == 0)	// L'animation de la lettre est terminee ?
	    {
	      if (iTempo == 0)
		{
		  iLettreAnime++;	// oui, passe a la lettre suivante
		  if (iLettreAnime >= 8)
		    {
		      iLettreAnime = 0;
		    }
		  if (iLettreAnime == 7)
		    iTempo = 200;
		  if (iLettreAnime < 4)
		    {
		      _caractere *_pListe = pGame->getCaracteres ();

		      pLettreActuelle = &_pListe[iLettreAnime];
		    }
		  else
		    {
		      _caractere *_pListe = pOver->getCaracteres ();

		      pLettreActuelle = &_pListe[iLettreAnime - 4];
		    }
		  pLettreActuelle->debuteAnimation ();
		}
	      else
		iTempo--;
	    }
	}
	pGame->affiche ();
	pOver->affiche ();
	if (etat_menu == MENU_ON)
	  {
	    iActionExecution = 0;
	    //aff_game_over=0 ;
	  }
	if (_iFlag == 1)
	  {
	    pGame->changePositionY (POS_Y_GAME + pos_y_menu -
				    (BANDE_DE_CLIP + HAUT_ECR_RAM));
	    pOver->changePositionY (POS_Y_OVER + pos_y_menu -
				    (BANDE_DE_CLIP + HAUT_ECR_RAM));
	  }
	testApparitionMenu ();
      }
      break;
    }
}

//------------------------------------------------------------------------------
// Animations de tous les caracteres du scores
//------------------------------------------------------------------------------
void
_score::animationScore ()
{
  if (iAnimIndexTempo == 0)
    {
      _caractere *_oCaractere = pTableau[iAnimIndex++];

      _oCaractere->debuteAnimation ();
      if (iAnimIndex >= iTOTALCARACTERES)
	iAnimIndex = 0;
      iAnimIndexTempo = 25;
    }
  else
    iAnimIndexTempo--;

  for (Uint32 _iIndex = 0; _iIndex < iTOTALCARACTERES; _iIndex++)
    {
      _caractere *_oCaractere = pTableau[_iIndex];

      _oCaractere->animation2 ();
    }

}

//--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--
// TABLE SCORE : apparition table des scores lorsque le joueur presse ESC
// input => _iFlag      : 0 main menu's disable (etat_menu==MENU_OFF)
//                                                                      : 1 main menu's enable
//--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--
void
_score::executeTableScore (Sint32 _iFlag)
{
  Uint32 _iIndexScore = 0;
  Uint32 _iIndex1;

  switch (iAction)
    {				// Initialise l'apparition
    case 0:
      {
	iAction = 1;
	iAnimIndexTempo = 0;
	iAnimIndex = 0;
      }
      break;

      // Les caracteres descendent ou montent
    case 1:
      {				//animationScore() ;
	Sint32 _iPositionY = iTABLEY;

	for (_iIndex1 = 0; _iIndex1 < iNOMBRE_SCORES; _iIndex1++)
	  {
	    _chaine *_pChaine = &pScoresObj[_iIndexScore++];

	    _pChaine->changePositionY (_iPositionY + pos_y_menu -
				       (BANDE_DE_CLIP + HAUT_ECR_RAM));
	    _pChaine->affiche ();
	    _pChaine->animationTous ();
	    _iPositionY += iTABLEOFFSET;
	  }
	_iPositionY = iTABLEY;
	for (_iIndex1 = 0; _iIndex1 < iNOMBRE_SCORES; _iIndex1++)
	  {
	    _chaine *_pChaine = &pScoresObj[_iIndexScore++];

	    _pChaine->changePositionY (_iPositionY + pos_y_menu -
				       (BANDE_DE_CLIP + HAUT_ECR_RAM));
	    _pChaine->affiche ();
	    _pChaine->animationTous ();
	    _iPositionY += iTABLEOFFSET;
	  }
	if (pos_y_menu >= BANDE_DE_CLIP + HAUT_ECR_RAM)
	  iAction = 2;		// (menu totalement disparu ?)
	testApparitionMenu ();
	if (etat_menu == MENU_ON)
	  iActionExecution = 0;
      }
      break;

      // Les caracteres sont fixes et s'arretent de tourner
    case 2:
      {
	Sint32 _iFin = 0;

	for (_iIndex1 = 0; _iIndex1 < iNOMBRE_SCORES; _iIndex1++)
	  {
	    _chaine *_pChaine = &pScoresObj[_iIndexScore++];

	    if (_pChaine->animation2Tous () != 0)
	      _iFin = 1;
	    _pChaine->affiche ();
	  }
	for (_iIndex1 = 0; _iIndex1 < iNOMBRE_SCORES; _iIndex1++)
	  {
	    _chaine *_pChaine = &pScoresObj[_iIndexScore++];

	    if (_pChaine->animation2Tous () != 0)
	      _iFin = 1;
	    _pChaine->affiche ();
	  }
	if (_iFin == 0)
	  iAction = 3;
	testApparitionMenu ();
      }
      break;

      // Les caracters sont fixes et s'animent les uns apres les autres
    case 3:
      {
	animationScore ();
	afficheScores ();
	if (pos_y_menu < BANDE_DE_CLIP + HAUT_ECR_RAM)
	  iAction = 1;		// (tout tourne en motant)
	testApparitionMenu ();
      }
      break;
    }
}

//=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=
// ABOUT : initialize credits / initialise credit
//=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=
Sint32
_score::creeAbout ()
{
  if (pTexteAbout == 0x0)
    pTexteAbout = new _caractere[iMAXICARACTERES];
  iAboutNombreCaracteres = 0;
  iIndexAboutTable = 0;
  return 1;
}

//--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--
// ABOUT : credit du jeu (qui fait quoi ?) execution
// input => _iFlag      : 0 main menu's disable (etat_menu==MENU_OFF)
//                                                                      : 1 main menu's enable
//--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--
void
_score::executeAbout (Sint32 _iFlag)
{
  Uint32 _iIndex;
  Sint32 _iPosY;
  Sint32 _iPosX;
  Sint32 _iType;
  char _cAscii;
  Sint32 _iFin;
  _caractere *_oCaractere;
  char *_pLigne;
  Sint32 _iAngleInc;

  switch (iAction)
    {
    case 0:
      {
	_iPosY = 158;
	_iAngleInc = 1;
	iAboutNombreCaracteres = 0;
	_pLigne = pTableAbout[iIndexAboutTable++];
	if (*(_pLigne) == '#')
	  {
	    _pLigne = pTableAbout[0];
	    iIndexAboutTable = 1;
	  }

	while (*(_pLigne) != '@')
	  {
	    _iPosX = 160;
	    if (iAboutNombreCaracteres >= iMAXICARACTERES)
	      break;
	    _cAscii = *(_pLigne++);
	    if (_cAscii == '0')
	      _iType = 0;	// premier caractere determine la police utilisee
	    else
	      _iType = 1;

	    if (_iType == 0)
	      _iPosX = 128 + (256 - ((strlen (_pLigne)) * 16)) / 2 - 8;
	    else
	      _iPosX = 128 + (256 - ((strlen (_pLigne)) * 22)) / 2 - 11;

	    _cAscii = *(_pLigne++);
	    while (_cAscii != 0)
	      {
		if (iAboutNombreCaracteres >= iMAXICARACTERES)
		  break;
		_oCaractere = &pTexteAbout[iAboutNombreCaracteres++];
		_oCaractere->initialise_caractere (_cAscii, _iPosX, _iPosY,
						   _iType);
		_oCaractere->initRotation (0, _iAngleInc, 200, 200);
		if (_iAngleInc > 0)
		  {
		    _iAngleInc = -_iAngleInc;
		  }
		else
		  {
		    _iAngleInc = -_iAngleInc;
		    _iAngleInc++;
		    if (_iAngleInc > 2)
		      _iAngleInc = 1;
		  }
		_cAscii = *(_pLigne++);
		if (_iType == 0)
		  _iPosX += 16;
		else
		  _iPosX += 22;
	      }
	    _iPosY += 30;
	    _pLigne = pTableAbout[iIndexAboutTable++];
	  }
	iAction = 1;
	testApparitionMenu ();
      }
      break;

      // Les lettres apparaissent (rotation)
    case 1:
      {
	_iFin = 1;
	for (_iIndex = 0; _iIndex < iAboutNombreCaracteres; _iIndex++)
	  {
	    _oCaractere = &pTexteAbout[_iIndex];
	    if (_oCaractere->rotation1 (1, 1, 0) == 0)
	      _iFin = 0;
	    _oCaractere->animation ();
	    _oCaractere->affiche ();
	  }
	if (_iFin == 1)
	  iAction = 2;
	testApparitionMenu ();
      }
      break;

      // Les caracteres sont arrives (termine l'animation)
    case 2:
      {
	_iFin = 0;
	for (_iIndex = 0; _iIndex < iAboutNombreCaracteres; _iIndex++)
	  {
	    _oCaractere = &pTexteAbout[_iIndex];
	    if (_oCaractere->animation2 () != 0)
	      _iFin = 1;
	    _oCaractere->affiche ();
	  }
	if (_iFin == 0)
	  {
	    iAction = 3;
	    iAnimIndex = 0;
	    iAnimIndexTempo = 0;
	    iTempo = 300;
	  }
	testApparitionMenu ();
      }
      break;

      // Les caracteres sont statiques
    case 3:
      {
	if (iAnimIndexTempo == 0)
	  {
	    _oCaractere = &pTexteAbout[iAnimIndex++];
	    _oCaractere->debuteAnimation ();
	    if (iAnimIndex >= iAboutNombreCaracteres)
	      iAnimIndex = 0;
	    iAnimIndexTempo = 25;
	  }
	else
	  {
	    iAnimIndexTempo--;
	  }
	for (_iIndex = 0; _iIndex < iAboutNombreCaracteres; _iIndex++)
	  {
	    _oCaractere = &pTexteAbout[_iIndex];
	    _oCaractere->animation2 ();
	    _oCaractere->affiche ();
	  }
	if (--iTempo <= 0)
	  iAction = 4;
	if (etat_menu == MENU_ON || pos_y_menu < BANDE_DE_CLIP + HAUT_ECR_RAM)
	  iAction = 4;
	testApparitionMenu ();
      }
      break;

    case 4:
      {
	_iFin = 0;
	for (_iIndex = 0; _iIndex < iAboutNombreCaracteres; _iIndex++)
	  {
	    _oCaractere = &pTexteAbout[_iIndex];
	    if (_oCaractere->animation2 () != 0)
	      _iFin = 1;
	    _oCaractere->affiche ();
	  }
	if (_iFin == 0)
	  iAction = 5;
	testApparitionMenu ();
      }
      break;

      // Les lettres disparaissent (rotation)
    case 5:
      {
	_iFin = 1;
	for (_iIndex = 0; _iIndex < iAboutNombreCaracteres; _iIndex++)
	  {
	    _oCaractere = &pTexteAbout[_iIndex];
	    if (_oCaractere->rotation2 (3, 3, 300, 300) == 0)
	      _iFin = 0;
	    _oCaractere->animation ();
	    _oCaractere->affiche ();
	  }
	if (_iFin == 1)
	  {
	    iAction = 0;
	    if (etat_menu == MENU_ON
		|| pos_y_menu < BANDE_DE_CLIP + HAUT_ECR_RAM)
	      iActionExecution = 0;
	  }
	testApparitionMenu ();
      }
      break;
    }
}



//=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=
// ORDER : initialize / initialise
//=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=!=
Sint32
_score::creeOrder ()
{
  char *_pTxt;
  Uint32 _iIndex;

  if (pTxtChaines == 0x0)
    pTxtChaines = new _chaine[iTXT_NBRELIGNES];
  // Reserve le tableau de caractere
  if (pTxtTableau == 0x0)
    {
      pTxtTableau = new char[iTXT_NBRELIGNES * iTXT_NBRECOL];

      _pTxt = pTxtTableau;
      for (_iIndex = 0; _iIndex < iTXT_NBRELIGNES * iTXT_NBRECOL; _iIndex++)
	{
	  _pTxt[_iIndex] = ' ';
	}

    }
  Sint32 _iPosY = 128 + 7;

  _pTxt = pTxtTableau;
  for (_iIndex = 0; _iIndex < iTXT_NBRELIGNES; _iIndex++)
    {
      _chaine *_oChaine = &pTxtChaines[_iIndex];

      _oChaine->initialise (_pTxt, iTXT_NBRECOL, 128, _iPosY, 16, 0);
      _iPosY += 16;
      _pTxt += iTXT_NBRECOL;
    }
  iTxtPosX = 0;
  iTxtPosY = 0;
  if (!chargeTable ())
    {
      afficheErreur ("_score::ChargeTable()", "score.cpp/creeOrder()");
      return 0;
    }
  //iTableIndex = 2;                                             // premier element de la table est le 2 (avant c'est la somme de controle)
  iTableIndex = 0;		// premier element de la table est le 2 (avant c'est la somme de controle)
  iTableTempo = 0;

  last_valeur = 0;

  effaceCaracteres ();

  return 1;
}

//--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--
// ORDER : Affiche le texte a la Commodore 64 / display text
// iTXT_PAUSE : attente de la valeur suivante (1 a 32767)
// iTXT_FIN : fin du fichier
// sinon code clavier
// input => _iFlag      : 0 main menu's disable (etat_menu==MENU_OFF)
//                                                                      : 1 main menu's enable
//--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--
void
_score::executeOrder (Sint32 _iFlag)
{
  Uint32 _iIndex;
  _chaine *_oChaine;
  Sint32 _iCodeTouchePressee = 0;
  Sint32 _iCurseurAction = 0;

  // Donne l'ordonnee de toutes les chaines
  Uint32 _iPosY = pos_y_menu - HAUT_ECR_RAM + 7;

  for (_iIndex = 0; _iIndex < iTXT_NBRELIGNES; _iIndex++)
    {
      _oChaine = &pTxtChaines[_iIndex];
      _oChaine->changePositionY (_iPosY);
      _iPosY += 16;
    }
  _oChaine = &pTxtChaines[iTxtPosY];	// objet '_chaine' ou est le curseur

  switch (iAction)
    {
      // ORDER = entre le texte
/*    case 0:
    {
      _iCodeTouchePressee = _oChaine->entreeClavier();         //0=pas de code touche
      iTxtPosX = _oChaine->getCurseurPosition();
      _iCurseurAction = _oChaine->getCurseurAction();          //1=depassement a gauche / 2=depassement a droite
      if(_iCodeTouchePressee == 0)
      {
        iTableTempo++;
        if(iTableTempo >= 32767)
        {
          entreTableValeur(iTXT_PAUSE);                        // code attente
          entreTableValeur(iTableTempo);                       // delai attente
          iTableTempo = 0;
        }
      }
      else
      {
        if(iTableTempo > 0)
        {
          entreTableValeur(iTXT_PAUSE);                        // code attente
          entreTableValeur(iTableTempo);                       // delai attente
          iTableTempo = 0;
        }
        entreTableValeur(_iCodeTouchePressee);
      }
      if(etat_menu == MENU_ON || etat_menu == APPARITION_MENU)
      {
        iActionExecution = 0;
        entreTableValeur(iTXT_FIN);                            // fin du fichier
        sauveTable();
      }
    }
      break;
	  */

      // ORDER : affiche le texte a partir du fichier charge
    case 1:
      {
	_iCodeTouchePressee = 0;
	if (--iTableTempo <= 0)
	  {
	    Uint16 _valeur = sortTableValeur ();

	    if (_valeur == iTXT_PAUSE)
	      {

		if (last_valeur == (Uint16) ' ')
		  {

		    iTableTempo = 10;	// fast  spaces

		  }
		else
		  {

		    iTableTempo = 30 + rand () % 30;	//sortTableValeur();
		    if ((rand () % 100) < 25)
		      iTableTempo += 20;
		    iTableTempo = iTableTempo / 3;
		  }

	      }
	    else
	      {
		_iCodeTouchePressee = _valeur;	//force unsigned

		last_valeur = _valeur;

#ifdef SDL_TLK

/*       printf("XK_BackSpace=%x, XK_Delete=%x, XK_Right=%x, XK_Left=%x, XK_Up=%x, XK_Down=%x, XK_Return=%x, XK_F3=%x, XK_F4=%x, XK_F5=%x, XK_Shift_L=%x, XK_Shift_R=%x\n",
         XK_BackSpace, XK_Delete, XK_Right, XK_Left, XK_Up, XK_Down, XK_Return, XK_F3, XK_F4, XK_F5, XK_Shift_L, XK_Shift_R);
*/
		switch (_iCodeTouchePressee)
		  {
		  case 0xff08:	//XK_BackSpace
		    _iCodeTouchePressee = SDLK_BACKSPACE;
		    break;
		  case 0xffff:	//XK_Delete
		    _iCodeTouchePressee = SDLK_DELETE;
		    break;
		  case 0xff53:	//XK_Right
		    _iCodeTouchePressee = SDLK_RIGHT;
		    break;
		  case 0xff51:	//XK_Left
		    _iCodeTouchePressee = SDLK_LEFT;
		    break;
		  case 0xff52:	//XK_Up
		    _iCodeTouchePressee = SDLK_UP;
		    break;
		  case 0xff54:	//XK_Down
		    _iCodeTouchePressee = SDLK_DOWN;
		    break;
		  case 0xff0d:	//XK_Return
		    _iCodeTouchePressee = SDLK_RETURN;
		    break;
		  case 0xffc0:	//XK_F3
		    _iCodeTouchePressee = SDLK_F3;
		    break;
		  case 0xffc1:	//XK_F4
		    _iCodeTouchePressee = SDLK_F4;
		    break;
		  case 0xffc2:	//XK_F5
		    _iCodeTouchePressee = SDLK_F5;
		    break;
		  case 0xffe1:	//XK_Shift_L
		    _iCodeTouchePressee = SDLK_LSHIFT;
		    break;
		  case 0xffe2:	//XK_Shift_R
		    _iCodeTouchePressee = SDLK_RSHIFT;
		    break;
		  }
#endif

	      }
	  }
	_oChaine->entreeClavier (_iCodeTouchePressee);
	iTxtPosX = _oChaine->getCurseurPosition ();
	_iCurseurAction = _oChaine->getCurseurAction ();	//1=depassement a gauche / 2=depassement a droite
	if (etat_menu == MENU_ON || etat_menu == APPARITION_MENU)
	  {
	    iAction = 2;
	  }
	testApparitionMenu ();
      }
      break;

      // le menu monte
    case 2:
      {
	_iCodeTouchePressee = 0;
	_iCurseurAction = 0;
	if (etat_menu == MENU_ON)
	  {
	    iActionExecution = 0;
	  }
      }
      break;
    }

  // test les touches speciales
  switch (_iCodeTouchePressee)
    {
#ifdef SDL_TLK
    case SDLK_DOWN:
#else
    case XK_Down:
#endif
      if (++iTxtPosY >= iTXT_NBRELIGNES)
	{
	  iTxtPosY = iTXT_NBRELIGNES - 1;
	  supprimeLigne (0);
	}
      break;
#ifdef SDL_TLK
    case SDLK_UP:
#else
    case XK_Up:
#endif
      if (iTxtPosY == 0)
	{
	  insertLigne (iTxtPosY);
	}
      else
	{
	  iTxtPosY--;
	}
      break;
#ifdef SDL_TLK
    case SDLK_INSERT:
#else
    case XK_Insert:		// touche INSERT pressee
#endif
      insertLigne (iTxtPosY);
      break;
#ifdef SDL_TLK
    case SDLK_RETURN:
#else
    case XK_Return:		// touche RETURN pressed ? (ASCII=13)
#endif
      if (++iTxtPosY >= iTXT_NBRELIGNES)
	{
	  iTxtPosY = iTXT_NBRELIGNES - 1;
	  supprimeLigne (0);
	}
      iTxtPosX = 0;
      break;
#ifdef SDL_TLK
    case SDLK_F3:
#else
    case XK_F3:		// curseur en haut a gauche
#endif
      iTxtPosX = 0;
      iTxtPosY = 0;
      break;
#ifdef SDL_TLK
    case SDLK_F4:
#else
    case XK_F4:		// curseur en haut a gauche & efface l'ecran
#endif
      effaceCaracteres ();
      break;
#ifdef SDL_TLK
    case SDLK_F5:
#else
    case XK_F5:		// supprime la ligne en cours
#endif
      supprimeLigne (iTxtPosY);
      break;
    }				// fin test des touches speciales

  // test les depassements horizontaux du curseur
  switch (_iCurseurAction)
    {
    case 1:			// Curseur depassement gauche
      if (iTxtPosY == 0)
	{
	  insertLigne (iTxtPosY);
	}
      else
	{
	  iTxtPosY--;
	}
      iTxtPosX = iTXT_NBRECOL - 1;
      break;
    case 2:			// curseur depassement a droite
      if (++iTxtPosY >= iTXT_NBRELIGNES)
	{
	  iTxtPosY = iTXT_NBRELIGNES - 1;
	  supprimeLigne (0);
	  iTxtPosX = 0;
	}
      else
	iTxtPosX = 0;
      break;
    }
  // donne la nouvelle position du cuseur
  _oChaine = &pTxtChaines[iTxtPosY];
  _oChaine->setCurseurPosition (iTxtPosX);
  // affiche toutes les chaines
  for (_iIndex = 0; _iIndex < iTXT_NBRELIGNES; _iIndex++)
    {
      _oChaine = &pTxtChaines[_iIndex];
      _oChaine->affiche ();
      _oChaine->animation2Tous ();
    }
}

//------------------------------------------------------------------------------
// display order: clear all screen
//------------------------------------------------------------------------------
void
_score::effaceCaracteres ()
{
  char *_pDestination;
  Uint32 _iIndex;
  iTxtPosX = 0;
  iTxtPosY = 0;
  _pDestination = pTxtTableau;
  for (_iIndex = 0; _iIndex < iTXT_NBRELIGNES * iTXT_NBRECOL; _iIndex++)
    *(_pDestination++) = ' ';
  for (_iIndex = 0; _iIndex < iTXT_NBRELIGNES; _iIndex++)
    {
      _chaine *_oChaine = &pTxtChaines[_iIndex];
      _oChaine->initialiseSprite ();
    }
}

//------------------------------------------------------------------------------
// display order : insert text line
// input => _iLigne: index 0 to 11
//------------------------------------------------------------------------------
void
_score::insertLigne (Sint32 _iLigne)
{
  char *_pDestination = pTxtTableau + (iTXT_NBRECOL * iTXT_NBRELIGNES) - 1;
  char *_pSource = _pDestination - iTXT_NBRECOL;
  Uint32 _iIndex;
  for (_iIndex = 0;
       _iIndex < (iTXT_NBRELIGNES - 1 - _iLigne) * iTXT_NBRECOL; _iIndex++)
    *(_pDestination--) = *(_pSource--);
  _pSource++;
  for (_iIndex = 0; _iIndex < iTXT_NBRECOL; _iIndex++)
    *(_pSource++) = ' ';
  for (_iIndex = 0; _iIndex < iTXT_NBRELIGNES; _iIndex++)
    {
      _chaine *_oChaine = &pTxtChaines[_iIndex];
      _oChaine->initialiseSprite ();
    }
}

//------------------------------------------------------------------------------
// display order: remove text line
// input => _iLigne: index 0 to 10
//------------------------------------------------------------------------------
void
_score::supprimeLigne (Sint32 _iLigne)
{
  char *_pDestination = pTxtTableau + (_iLigne * iTXT_NBRECOL);
  char *_pSource = _pDestination + iTXT_NBRECOL;
  Uint32 _iIndex;
  for (_iIndex = 0;
       _iIndex < (iTXT_NBRELIGNES - 1 - _iLigne) * iTXT_NBRECOL; _iIndex++)
    *(_pDestination++) = *(_pSource++);

  for (_iIndex = 0; _iIndex < iTXT_NBRECOL; _iIndex++)
    *(--_pSource) = ' ';

  for (_iIndex = 0; _iIndex < iTXT_NBRELIGNES; _iIndex++)
    {
      _chaine *_oChaine = &pTxtChaines[_iIndex];
      _oChaine->initialiseSprite ();
    }
}

// ------------------------------------------------------------------------------
// order :
// ------------------------------------------------------------------------------
// Charge la table de commande  -----------------------------------------------
Sint32
_score::chargeTable ()
{

  if (pTableCommande == 0)
    {
      pTableCommande = (Uint16 *) reserveMemoire (iTXT_FICHIERTAILLE);
      if (!pTableCommande)
	return 0;
    }


  // detruit le texte s'il ets deja charg�  _score::freeTextOrder ();

  // charge le texte
  _score::loadTextOrder ();
  iAction = 1;			// on se contente d'afficher le texte


/*
  if(chargeFichier("graphics/pmorder.xxx", (char *)pTableCommande))
  {
    Sint32 _iSomme2 = *pTableCommande;                            // somme d'auto-controle du fichier
    Sint32 _iSomme =
      calculSomme((Sint32 *)pTableCommande, (iTXT_FICHIERTAILLE / 4) - 1);
    if(_iSomme2 != _iSomme)
      iAction = 0;
    iAction = 1;                                               // on se contente d'afficher le texte
  }
  //le fichier n'existe pas / file not found
  else
  {
    for(Uint32 _iIndex = 0; _iIndex < iTXT_FICHIERTAILLE / 2; _iIndex++)
    { pTableCommande[_iIndex] = 0;
    }
    iAction = 0;                                               // on rentre un nouveau texte
  }
*/

  return 1;
}

// Sauvegarde la table de commande  -------------------------------------------
/*void _score::sauveTable()
{
  Sint32 _iSomme =calculSomme((Sint32 *)pTableCommande, (iTXT_FICHIERTAILLE / 4) - 1);
  *pTableCommande = _iSomme;
  Sint32 _iHandle = open("graphics/pmorder.xxx", O_WRONLY | O_CREAT, 0666);        //ouvre le fichier
  if(_iHandle == -1)
  { printf("_score::sauveTable() : can't open file : \"pmorder.xxx\"\n\n");
  }
  else
  {
    write(_iHandle, pTableCommande, iTXT_FICHIERTAILLE);
    close(_iHandle);
  }
}*/

// Entre une valeur dans la table de commande  --------------------------------
void
_score::entreTableValeur (Uint16 _sValeur)
{
  if (iTableIndex < iTXT_FICHIERTAILLE - 1)
    {
      pTableCommande[iTableIndex++] = _sValeur;	// sauve code
      //printf("entreTableValeur : %i", _sValeur) ;
      //if(_sValeur>32 && _sValeur <= 'z') printf(" ==> %c", (char)(_sValeur)) ;
      //printf("\n") ;
    }
}

//------------------------------------------------------------------------------
// display order: read value from command table
//------------------------------------------------------------------------------
Uint16
_score::sortTableValeur ()
{
  Uint16 _sValeur =
    (Uint16) litMot16bits ((short *) &pTableCommande[iTableIndex++]);
/*  Uint16 _sValeur2 = (Uint16) litMot16bits(
    (short*) &pTableCommande[iTableIndex]
  );*/

  if (iTableIndex >= iTXT_FICHIERTAILLE || _sValeur == 0)
//     || _sValeur2 == iTXT_FIN)
    {
      effaceCaracteres ();

      //iTableIndex = 2;
      iTableIndex = 0;

      afficheMessage ("Fin du fichier pmorder.xxx");
    }
  //printf("sortTableValeur : %i", _sValeur) ;
  //if(_sValeur>32 && _sValeur <= 'z') printf(" ==> %c", (char)(_sValeur)) ;
  //printf("\n") ;
  return _sValeur;
}

// Test si quitte le menu -----------------------------------------------------
void
_score::testApparitionMenu ()
{
  if (iCodeTouche != 0 && etat_menu == MENU_OFF)
    {
      etat_menu = APPARITION_MENU;	//met le flag apparition du menu a 1
      Init_Scroll_Present ();	//initialise le defilement du texte en bas
    }
}

// F3: // curseur en haut a gauche
// F4: // curseur en haut a gauche & efface l'ecran
// F5: // supprime la ligne en cours
/*
.THIS..PROGRAM..
IS FREE SOFTWARE
....YOU..CAN....
..REDISTRIBUTE..
...IT..AND/OR...
...MODIFY..IT...
...UNDER..THE...
TERMS OF THE GNU
.GENERAL PUBLIC.
...LICENSE AS...
..PUBLISHED.BY..
....THE FREE....
....SOFTWARE....
...FOUNDATION...
EITHER VERSION 2
.OF.THE LICENSE.
.......OR.......
(AT.YOUR.OPTION)
...ANY..LATER...
....VERSION.....
................
................
SEE THE GNU.....
GENERAL PUBLIC..
LICENSE FOR MORE
DETAILS.........
................
....(c) 2002....
...TLK..GAMES...
................
.LINUX..VERSION.
.......BY.......
BRUNO..ETHVIGNOT
......AND.......
.ANDRE..MAJOREL.
................
................
..Greetings.to..

.Alex...Senesse.
.Andre..Majorel.
Aurelien.Anselme
.David..Alardet.
..David.Igreja..
Delph..Ethvignot
Didier..Florentz
Emeric.Pourcelot
.Etienne.Sobole.
.Gaelle.Richard.
.Gautier.Portet.
..Hafida.Bouih..
J-Michel..Martin
J-Francois.Kabak
Karine.Philippon
Laurent.Guibelin
.Laurent..Tenza.
.M-J.Robertella.
.Nicolas.Arnaud.
Olivier..Simonin
Pascal.Ethvignot
..Pascal.Lauly..
Philip..Bouveret
Philip.Jacquemet
.Roderic.Moitie.
Sandrine..Berger
.Sophie.Bibanov.
..Sylvain.Cote..
..Yoan.Daubeze..
.Valerie.Arnaud.

*/

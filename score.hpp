//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "score.hpp"
// created	: 1998-06-29
// updates	: 2005-01-07
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSEn. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA 02111-1307, USA.
//*****************************************************************************
#ifndef __SCORE__
#define __SCORE__
//..............................................................................
#include "powermanga.hpp"
#include "chaine.hpp"
#include "SDL/SDL.h"
//..............................................................................
// texte a la commodore 64 (pour le ORDER)
const Uint32	iTXT_NBRELIGNES = 11;
const Uint32	iTXT_NBRECOL = 16;
const Uint32	iTXT_FICHIERTAILLE = 20480;	//taille utilise pour le fichier de commande (comportement du curseur)
const Uint32	iTXT_PAUSE = 0x4c1;		//pause code
const Uint32	iTXT_FIN = 0x4c2;		//end code
// pour les scores proprement dit
const Uint32	iMAXICARACTERES = 100;		// nombre maxium de caraceteres pour l'about
const Uint32	iTABLEX1 = 128 + 32;		// abscisse des noms
const Uint32	iTABLEX2 = 128 + 96;		// abscisse des scores
const Uint32	iTABLEY = 128 + 50;		// ordonnee du premier nom
const Uint32	iTABLEOFFSET = 20;		// espacement entre deux noms
const Uint32	iNOMBRE_SCORES = 5;		// On garde cinq noms & cinq scores
const Uint32	iLONGEURNOM = 3;		// Longeur nom du joueur
const Uint32	iTEMPOTABLE = 400;		// Temps d'apparition de la table avant d'afficher le GAME OVER
const Uint32	iTOTALCARACTERES=iNOMBRE_SCORES*(iLONGEURNOM+8);
// Taille du fichier
const Uint32	 iTAILLEBUFFER = iNOMBRE_SCORES * (iLONGEURNOM + sizeof(Sint32)) + sizeof(Sint32);

class _score: public _chaine
{

	//##############################################################
	// statics members
	//##############################################################
	private:

		// strings "GAME" and "OVER"
		static char		pGameTexte[];
		static char		pOverTexte[];
		static char		pScoreTempo[32];	// tableau de travail
		static _chaine*		pChaineJoueur;// pointeur de type '_chaine' reservee a l'entree du nom du joueur
		static char		cScoresNoms[iNOMBRE_SCORES][iLONGEURNOM+1];// tableau contenant les noms en ASCII
		static Sint32		iScores[iNOMBRE_SCORES];// tableau contenant les scores en entiers
		static _chaine*		pScoresObj;// tableau contenant les 10 chaines noms & scores
		//0: no action; 1: score input; 6: display scores
		static Sint32		iAction;

		//1:game-over 2:scores 3:about 4:order
		static Sint32		iActionExecution;

		static _chaine*		pGame;
		static _chaine*		pOver;
		static Uint32		iTempo;// >0 le joueur n'est pas dans les scores, la table apparait un instant
		static Sint32		iLettreAnime;// numero de la lettre anime !
		static _caractere*	pLettreActuelle;
		static _caractere*	pTableau[iTOTALCARACTERES];
		static _caractere*	pTexteAbout;

		//######################################################
		//strings of about texts
		//######################################################
		static char**		pTableAbout;
		static char*		pTextData;
		static Sint32		iSizeTableAbout;
		//number of chars current display
		static Uint32		iAboutNombreCaracteres;
		//list text index
		static Uint32		iIndexAboutTable;
		//anim char index
		static Uint32		iAnimIndex;
		static Uint32		iAnimIndexTempo;

		//######################################################
		//text of order menu
		//######################################################
		static _chaine*		pTxtChaines;
		static Sint32		iTxtPosX;
		static Uint32		iTxtPosY;
		static char*		pTxtTableau;
		static Uint16*		pTableCommande;	//tableau contenant le texte & mouvement du curseur
		static Uint32		iTableIndex;	//index sur le tableau 'pTableCommande'
		static Sint32		iTableTempo;	//compteur d'attente entre chaque action (maximum=255)
		static Uint16		last_valeur;


	//##############################################################
	// statics methods
	//##############################################################
	public :
		static Sint32		getActionExecution();
		static void		execution(Sint32 _bFlag);
		static Sint32		setAction(Sint32 _iCodeAction);
		static Sint32		isInputName();
        static const _chaine * const getScoreInput() { return pChaineJoueur; }
		static void		destroy();
	private :
		static Sint32		initialiseGameOver(Sint32 _iPosition);
		static Sint32		creeScores();
		static Sint32		creeAbout();
		static Sint32		creeOrder();
		static Sint32		affScores();
		static void		animationScore();
		static Sint32		trieScores();
		static Sint32		chargeScores();
		static void		sauveScores();
		static void		creeNouvelleTable();
		static Sint32		calculSomme(int* _pBuffer, Sint32 _iTaille);
		static void		afficheScores();
		static void		executeGameOver(Sint32 _iFlag);
		static void		executeTableScore(Sint32 _iFlag);
		static void		executeAbout(Sint32 _iFlag);
		static void		executeOrder(Sint32 _iFlag);
		static void		effaceCaracteres();
		static void		insertLigne(Sint32 _iLigne);
		static void		supprimeLigne(Sint32 _iLigne);
		static Sint32		chargeTable();
		//static void		sauveTable();
		static void		entreTableValeur(Uint16 cValeur);
		static Uint16		sortTableValeur();
		static void		testApparitionMenu();

	public:
		static Sint32		loadTextAbout();
		static void		freeTextAbout();
		static Sint32		loadTextOrder();
		static void		freeTextOrder();

};
#endif

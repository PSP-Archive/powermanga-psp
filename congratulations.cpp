//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "congratulations.cpp"
// created	: 1999-03-26
// updates	: 2005-01-08
// id		: $Id: congratulations.cpp,v 1.1.1.1 2005/01/25 05:52:19 gurumeditation Exp $
//-----------------------------------------------------------------------------
// fonctions    : void  _init_congra()
//                void  _execute_congratulations(int)
//                void  nouveau_ennemi()
//                void  animation_ennemi() 
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
#include "powermanga.hpp"
//------------------------------------------------------------------------------
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
extern short            touch[500];                            // Tableau utilisé pour recevoir les flag des touches du clavier.
extern image            enemi[MAX_TYP_OF_ENEMI + MAX_TYP_OF_S_ENEMI + MAX_TYP_OF_NMI_SPE + MAX_TYP_OF_OBJ_SPE][NBR_IMAGES_MAX]; // Définit le tableau des différents ennemis.
extern _etbfonte1      *pEtbfonte1;
extern int              iNumeroGardien;                        // numero du gardien en cours (ETB)
extern int              iAngleG1;
extern int              iAngleG2;
extern int              iTypedeplacement;
extern int              iTempo3;
extern int              iTempo4;
extern int              iCentreX;
extern int              iCentreY;
extern unsigned int     iCodeTouche;                           //(ETB) (code touche pressee)
extern int              iNumeroImageGardien;
extern float            vit_star_field;                        // vitesse du champs d'etoiles.
extern unsigned int     iCongratulation;                       //=1 congratulation running
extern unsigned int     iJoueIntro;                            //(ETB) (1=animation du debut / 2=animation de fin)
extern bool             aff_game_over;                         //1=display "GAME OVER"

//------------------------------------------------------------------------------
unsigned int            iChoixDeplacemement1;
unsigned int            iEnnemi;
unsigned int            iEnnemiType;
unsigned int            iEnnemiCompteur;
unsigned int            pEnnemis[] =
{ BOONIES, MOOARCKS, ACHEES, EVILIANS, RAGOONS,
  FLABBIES, BOWANS, HIJACKERS, DISTASIANS, BRITTERS,
  SICKIES, ARIANS, STINCKIES, BZEURCKS, WEAKIES,
  MASKIES, SCRAPIES, SCROLLIANS, SHAMISTEES, FOOLINIANS,
  GLAMOOSH,
  BIGBOONIES, BIGMOOARCKS, BIGACHEES, BIGEVILIANS, BIGRAGOONS,
  BIGFLABBIES, BIGBOWANS, BIGHIJACKERS, BIGDISTASIANS, BIGBRITTERS,
  BIGSICKIES, BIGARIANS, BIGSTINCKIES, BIGBZEURCKS, BIGWEAKIES,
  BIGMASKIES, BIGSCRAPIES, BIGSCROLLIANS, BIGSHAMISTEES, BIGFOOLINIANS,
  BIGGLAMOOSH,
  SUBJUGANEERS, MILLOUZ, SWORDINIANS, TOUBOUG, DISGOOSTEES,
  EARTHINIANS, BIRIANSTEES, BELCHOUTIES, VIONIEES, HOCKYS,
  TODHAIRIES, DEFECTINIANS, BLAVIRTHE, SOONIEES, ANGOUFF,
  GAFFIES, BITTERIANS, BLEUERCKS, ARCHINIANS, CLOWNIES,
  DEMONIANS, TOUTIES, FIDGETINIANS, EFFIES, DIMITINIANS,
  PAINIANS, ENSLAVEERS, FEABILIANS, DIVERTIZERS, SAPOUCH,
  HORRIBIANS, CARRYONIANS, DEVILIANS, ROUGHLEERS, ABASCUSIANS,
  ROTIES, STENCHIES, PERTURBIANS, MADIRIANS, BAINIES,
  SHURIKY, NAGGYS, GOZUKY, SOUKEE, QUIBOULY, TOURNADEE, SAAKAMIN
};
char                    pNomEnnemi[] ="                                      ";
char                   *pEnnemisNoms[] =
{ "BOONIES", "MOOARCKS", "ACHEES", "EVILIANS", "RAGOONS",
  "FLABBIES", "BOWANS", "HIJACKERS", "DISTASIANS", "BRITTERS",
  "SICKIES", "ARIANS", "STINCKIES", "BZEURCKS", "WEAKIES",
  "MASKIES", "SCRAPIES", "SCROLLIANS", "SHAMISTEES", "FOOLINIANS",
  "GLAMOOSH",
  "BIGBOONIES", "BIGMOOARCKS", "BIGACHEES", "BIGEVILIANS", "BIGRAGOONS",
  "BIGFLABBIES", "BIGBOWANS", "BIGHIJACKERS", "BIGDISTASIANS", "BIGBRITTERS",
  "BIGSICKIES", "BIGARIANS", "BIGSTINCKIES", "BIGBZEURCKS", "BIGWEAKIES",
  "BIGMASKIES", "BIGSCRAPIES", "BIGSCROLLIANS", "BIGSHAMISTEES",
    "BIGFOOLINIANS",
  "BIGGLAMOOSH",
  "SUBJUGANEERS", "MILLOUZ", "SWORDINIANS", "TOUBOUG", "DISGOOSTEES",
  "EARTHINIANS", "BIRIANSTEES", "BELCHOUTIES", "VIONIEES", "HOCKYS",
  "TODHAIRIES", "DEFECTINIANS", "BLAVIRTHE", "SOONIEES", "ANGOUFF",
  "GAFFIES", "BITTERIANS", "BLEUERCKS", "ARCHINIANS", "CLOWNIES",
  "DEMONIANS", "TOUTIES", "FIDGETINIANS", "EFFIES", "DIMITINIANS",
  "PAINIANS", "ENSLAVEERS", "FEABILIANS", "DIVERTIZERS", "SAPOUCH",
  "HORRIBIANS", "CARRYONIANS", "DEVILIANS", "ROUGHLEERS", "ABASCUSIANS",
  "ROTIES", "STENCHIES", "PERTURBIANS", "MADIRIANS", "BAINIES",
  "SHURIKY", "NAGGYS", "GOZUKY", "SOUKEE", "QUIBOULY", "TOURNADEE", "SAAKAMIN"
};
_chaine                *pChaineG = 0x0;

//------------------------------------------------------------------------------
void                    _init_congra();
void                    _execute_congra(int);
void                    nouveau_ennemi();
void                    animation_ennemi();

//------------------------------------------------------------------------------
// Initialise les congratulations
//------------------------------------------------------------------------------
void _init_congra()
{
	ve[iNumeroGardien].spr.coor_y =
		(float)(DEBUT_ECRAN_Y - ve[iNumeroGardien].spr.img[15]->haut);
	iEnnemiCompteur = 0;
	iTempo3 = 0;
	iTempo4 = 0;
	iAngleG1 = 0;
	iAngleG1 = 2;
	iTypedeplacement = 0;
	iEnnemi = New_Element_Ve();                                  // Retourne l'indice dans le tableau 've' d'un nouvel élément de la liste.
	iNumeroGardien = new_ve;
	if(!pChaineG)
		pChaineG = new _chaine(pNomEnnemi, 0, 0, LARG_FONTE_GAME);
#ifdef USE_SDLMIXER
	seal_JoueModule(2);                                          //joue musique des congratulations
#endif
}


//------------------------------------------------------------------------------
// release "pChaineG" object
//------------------------------------------------------------------------------
void _destroy_congra() 
{
	if(pChaineG)
		delete pChaineG;
	pChaineG = (_chaine *)NULL;
}


//------------------------------------------------------------------------------
// Execute les congratulations
//------------------------------------------------------------------------------
void _execute_congra(int num_gardien)
{ {
    switch (iTypedeplacement)
    {
      case 0:
        if(vit_star_field == 0.0)
        {
          iJoueIntro = 2;
          iTypedeplacement = 1;
          iAngleG1 = 0;
          iAngleG2 = 0;
        }
        break;
      case 1:
      {
        //if(iCodeTouche != 0 && _score::getActionExecution() == 0)
        if(_score::getActionExecution() == 0)
        {
          _score::setAction(1);
          aff_game_over = TRUE;
          for(int i = 0; i < 11; i++)
            Init_Anim_Option(i, TRUE);                         // ferme les options
          Clear_Keyboard();
          iTypedeplacement = 2;
        }
      }
        break;
      case 2:
      {
        iCongratulation = 1;
        if(iTempo3 <= 0)
        {
          iAngleG1 = (iAngleG1 + 1) & 127;
          if(iAngleG1 == 96 || iAngleG1 == 32)
            iTempo3 = 100;
        }
        else
          iTempo3--;
        nouveau_ennemi();
        float                   _fSin = pSin[iAngleG1];

        vit_star_field = (40 * _fSin) / 10;
        animation_ennemi();
        if(iTempo4 > 0)
        {
          switch (iChoixDeplacemement1)
          {
            case 0:
              iAngleG2++;
              if(iAngleG2 >= 32)
              {
                iAngleG2 = 32;
                iChoixDeplacemement1 = rand() % 2;
              };
              break;
            case 1:
              iAngleG2--;
              if(iAngleG2 <= 32)
              {
                iAngleG2 = 32;
                iChoixDeplacemement1 = rand() % 2;
              };
              break;
          }
        }
        else
        {
          switch (iChoixDeplacemement1)
          {
            case 0:                                           // l'ennemi part a droite
              iAngleG2++;
              if(iAngleG2 >= 64)
                iAngleG2 = 64;
              break;
            case 1:                                           // l'ennemi part a gauche
              iAngleG2--;
              if(iAngleG2 <= 0)
                iAngleG2 = 0;
              break;
          }
        }
        ve[iEnnemi].spr.coor_x = iCentreX + (pCos[iAngleG2] * 256);
        ve[iEnnemi].spr.coor_y = (float)iCentreY;
        Aff_Ve(iEnnemi);

        int                     _iPosX =
          (LARG_ECR_RAM_TOT -
           pChaineG->getNombreCaracteres() * LARG_FONTE_GAME) / 2;

        int                     _iAngle = (iAngleG2 + 64) & 127;
        int                     _iY;

        if(_score::getActionExecution() == 0)
          _iY = (int)(iCentreY + ve[iEnnemi].spr.img[0]->haut);
        else
          _iY = HAUT_ECR_RAM_TOT - BANDE_DE_CLIP - 16;
        pChaineG->changePosition((int)(pCos[_iAngle] * 256) + _iPosX, _iY);
        pChaineG->affiche();
        if(iTempo4 > 0 && iAngleG2 != 32)
          pChaineG->animationTous();
        else
          pChaineG->animation2Tous();
      }
        break;
    }
  }
}

//------------------------------------------------------------------------------
// Initialise la structure ennemi avec le prochain ennemi de la liste.
//------------------------------------------------------------------------------
void nouveau_ennemi()
{
  if(--iTempo4 <= 0 && (iAngleG2 == 64 || iAngleG2 == 0))
  {
    iTempo4 = 300;
    // initialise l'objet '_chaine' (qui affiche le nom)
    char                   *_pLigne = pEnnemisNoms[iEnnemiCompteur];
    int                     _iCaractere = 0;

    while (*(_pLigne) != 0)
    {
      pNomEnnemi[_iCaractere++] = *(_pLigne++);
    }
    pChaineG->initialise(pNomEnnemi, _iCaractere, 0, 0, LARG_FONTE_GAME, 0);
    iChoixDeplacemement1 = rand() % 2;
    if(iChoixDeplacemement1 == 1)
      iAngleG2 = 64;
    else
      iAngleG2 = 0;
    iEnnemiType = pEnnemis[iEnnemiCompteur++];
    if(iEnnemiType == SAAKAMIN)
      iEnnemiCompteur = 0;
    ve[iEnnemi].spr.pow_of_dest = 1000;
    ve[iEnnemi].spr.etat_dommages = 1000;
    ve[iEnnemi].spr.etat_max = 1000;
    if(iEnnemiType <= GLAMOOSH)
      ve[iEnnemi].spr.nbr_img = 32;
    else
    {
      if(iEnnemiType >= BIGBOONIES && iEnnemiType <= BIGGLAMOOSH)
        ve[iEnnemi].spr.nbr_img = 8;
      else
        ve[iEnnemi].spr.nbr_img = 32;
    }
    ve[iEnnemi].spr.img_act = 0;
    ve[iEnnemi].spr.nbr_vbl_act = 0;
    ve[iEnnemi].spr.nbr_tot_vbl = 4;
    for(int _iImage = 0; _iImage < ve[iEnnemi].spr.nbr_img; _iImage++)
      ve[iEnnemi].spr.img[_iImage] = (image *) & enemi[iEnnemiType][_iImage];
    ve[iEnnemi].cad_tir_initiale = 60;
    ve[iEnnemi].cad_tir = 60;
    ve[iEnnemi].deplacement = SPECIAUX;
    ve[iEnnemi].spr.coor_x =
      (float)(LARG_ECR_RAM_TOT - ve[iEnnemi].spr.img[0]->larg) / 2;
    ve[iEnnemi].spr.coor_y =
      (float)(HAUT_ECR_RAM_TOT - ve[iEnnemi].spr.img[0]->haut) / 2;
    iCentreX = (int)(LARG_ECR_RAM_TOT - ve[iEnnemi].spr.img[15]->larg) / 2;
    if(_score::getActionExecution() == 0)
      iCentreY = (int)(HAUT_ECR_RAM_TOT - ve[iEnnemi].spr.img[0]->haut) / 2;
    else
      iCentreY = BANDE_DE_CLIP;                                //+ve[iEnnemi].spr.img[0]->haut/2
    ve[iEnnemi].spr.vitesse = 0.5;
    ve[iEnnemi].type = iEnnemiType;
    ve[iEnnemi].dead = FALSE;
    ve[iEnnemi].visible = TRUE;

  }
}

//------------------------------------------------------------------------------
// Gestion de l'animation
//------------------------------------------------------------------------------
void animation_ennemi()
{
  if(ve[iEnnemi].sens_anim)
  {
    ve[iEnnemi].spr.nbr_vbl_act++;
    if(ve[iEnnemi].spr.nbr_vbl_act >= ve[iEnnemi].spr.nbr_tot_vbl)
    {
      ve[iEnnemi].spr.nbr_vbl_act = 0;
      ve[iEnnemi].spr.img_act--;
      if(ve[iEnnemi].spr.img_act < 0)
      {
        ve[iEnnemi].spr.img_act = 0;
        ve[iEnnemi].sens_anim = 0;
      }
    }
  }
  else
  {
    ve[iEnnemi].spr.nbr_vbl_act++;
    if(ve[iEnnemi].spr.nbr_vbl_act >= ve[iEnnemi].spr.nbr_tot_vbl)
    {
      ve[iEnnemi].spr.nbr_vbl_act = 0;
      ve[iEnnemi].spr.img_act++;
      if(ve[iEnnemi].spr.img_act >= ve[iEnnemi].spr.nbr_img)
      {
        ve[iEnnemi].spr.img_act = (short)(ve[iEnnemi].spr.nbr_img - 1);
        ve[iEnnemi].sens_anim = 1;
      }
    }
  }
}

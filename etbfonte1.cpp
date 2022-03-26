//*****************************************************************************
// copyright (c) 1998-2006 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "etbfonte.cpp"
// created	: ?
// updates	: 2006-03-27
// id		: $Id: etbfonte1.cpp,v 1.3 2006/03/27 09:10:18 gurumeditation Exp $
//-----------------------------------------------------------------------------
// description  : Implementation de la classe _etfonte1
//                display simples fontes in 8*8 pixels
//                CTRL+A : affiche les credits / display credits
//                CTRL+V : affiche des variables globales 
//                         display some variables (DEVELOPPEMENT2)
//                CTRL+B : affiche d'autres variables globales /
//                         display some other variables (DEVELOPPEMENT2)
//                CTRL+P : affiche & modifie les parametres 
//                         display menu's cheats (DEVELOPPEMENT2)
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
//#define               DEVELOPPEMENT2                           //if define then the keys : ctrl+v, ctrl+p and ctrl+b are enable
//------------------------------------------------------------------------------
extern configfile*      power_conf;
extern unsigned int     iOctetsParPixel;                       //nombre d'octets par pixels (1=256 couleurs / 2=65536 couleurs)
extern int              tmp_apparition_vj;                     //vaisseau joueur : compteur temps apparition 
extern bool             disparition_vj;                        //vaisseau joueur : 1=disparition 
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];
extern char            *ecran_ram;                             //adresse ecran logique.
extern short            touch[];                               //tableau des flags des touches du clavier.
extern int              num_level;                             //numero du niveau
extern int              etat_menu;                             //MENU_OFF(false)=menu inactif ; MENU_ON(true)=menu actif
extern bool             player_pause;                          //true=pause en cours
extern bool             aff_pause;                             //true=le texte "pause" est affiche
extern bool             game_over_player_one;                  //true=le joueur vient de perdre son vaisseau.(game over)
extern bool             aff_game_over;                         //true=affichage du game over en cours
extern int              quit_game;                             //true=demande a quitter le jeu
extern bool             apparition_gardian;
extern int              gardian_activity;
extern bool             meteor_activity;
extern grid             grille;
extern curve_level      courbe;                                //declaration de la structure de niveaux courbe.
extern short            nbr_nmis;                              //nombre enemis actuellement geres
extern unsigned int     iFlipFlag;                             //si=1 alors on utilise 2 surfaces directdraw !       
extern int              vmode;                                 //0=resolution de 320x200 / 1=640*480
extern image            meteo[MAX_TYP_OF_METEOR][NBR_IMAGES_METEOR];    //tableau des differentes meteorite
extern bool             move_level;                            //1=deplace le texte du niveau
extern int              liste_ve[MAX_NMI_ON_SCREEN];           //liste des 've' geres actuellement
extern int              liste_tirs[NBR_TIRS_MAX];              //liste des'tirs' geres actuellement
extern int              liste_bou[MAX_BOULES_ON_SCREEN];       //liste de toutes les 'bou' geres actuellement
extern int              liste_can[MAX_CANONS_ON_SCREEN];       //liste de toutes les 'can' geres actuellement
extern int              liste_onde_choc[NBR_ONDE_CHOC_MAX];    //liste de tous les 'tirs' geres actuellement
extern float            pos_x_level;
extern vaisseau         vj[NBR_MAX_J];
extern image            vj1[NBR_MAX_VAIS_J][NBR_MAX_IMG_VAIS_J];        //structure des images des vaisseaux du joueur 1
extern bool             bar_nrj_player;
extern short            nbr_boules;                            //nombre de boules actuellement geres
extern bool             eclair_actif;                          //1=joueur peut ou non utiliser l'eclair.
extern int              iNumeroGardien;                        //numero du gardien en cours (ETB)
extern unsigned int     iNombreZones;                          //nombre total d'adresse memoire reservee
extern unsigned int     iOctetsTotal;                          //memoire totale reservee
extern int              taille_mem_glob;                       //taille memoire maximum utilisee

#ifdef USE_SDLMIXER2
extern HAC              pBruitages[];
extern LPAUDIOWAVE      pSons[];
extern unsigned int     iFlagBruitages[];
extern unsigned int     iBruitagesVoix;                        //nombre de voix pour les bruitages
extern unsigned int     iTailleSons;                           //taille des sons

#ifdef DEVELOPPEMENT2
int                     iBruitage = 0;                         //numero d'un bruitage a jouer
#endif
#endif
//------------------------------------------------------------------------------

char _etbfonte1::pTexteAbout[]=
{
  "********************************@"\
  "*                              *@* "\
  POWERMANGA_VERSION\
  "*@*                LINUX VERSION *@"\
  "* (c) 1998-2006 TLK-GAMES      *@"\
  "*                              *@"\
  "* LINUX.TLK.FR                 *@"\
  "*                              *@"\
  "* BY :                         *@"\
  "* ANDRE MAJOREL                *@"\
  "* BRUNO ETHVIGNOT              *@"\
  "* DAVID IGREJA                 *@"\
  "* EMMANUEL FOUNAUD             *@"\
  "* ETIENNE SOBOLE               *@"\
  "* GUILLAUME COTTENCEAU         *@"\
  "* JEAN MICHEL MARTIN DE SANTERO*@"\
  "* LAURENT DUPERVAL             *@"\
  "* PATRICE DUHAMEL              *@"\
  "* MICHEL DOGNIAUX              *@"\
  "* SAMUEL HOCEVAR               *@"\
  "*                              *@"\
  "*  > PRESS CTRL-A TO CANCEL <  *@"\
  "********************************@"
} ;
//------------------------------------------------------------------------------
char	_etbfonte1::pTexteVariables[]=
{
  " * * * * * * * * * * * * * * * *@"\
  "* num-level:??    etat-menu:?   @"\
  "  player-pause:?  aff-pause:?  *@"\
  "* game-over-player-one:?        @"\
  "  aff-game-over:?  quit-game:? *@"\
  "* apparition-gardian:?          @"\
  "  gardian-activity:??          *@"\
  "* meteor-activity:?             @"\
  "  grille.activity:?            *@"\
  "* courbe.activity:?             @"\
  "  touch-90-:?                  *@"\
  "* nbr-nmis:???                  @"\
  "  score::getActionExecution():?*@"\
  "* iFlipFlag:?      vmode:?      @"\
  "  tmp_apparition-vj:           *@"\
  "* disparition-vj:               @"\
  "  vj-J1- cad-tir:              *@"\
  "* iNombreZones:?????            @"\
  "  iOctetsTotal  :???????? ?????*@"\
  "* taille-mem-glob:????????      @"\
  "                               *@"\
  "*  > PRESS CTRL-V TO CANCEL <   @"\
  " * * * * * * * * * * * * * * * *@"
} ;

//------------------------------------------------------------------------------
char	_etbfonte1::pTexteVariables2[]=
{
  " * * * * * * * * * * * * * * * *@"\
  "* seal : AGetVoiceStatus        @"\
  "  ????????????????             *@"\
  "* iBruitage:?                   @"\
  "  iFlagBruitages :             *@"\
  "* ???????????????????????       @"\
  "  iTailleSons:                 *@"\
  "* gameLevel:                    @"\
  "                               *@"\
  "*                               @"\
  "                               *@"\
  "*                               @"\
  "                               *@"\
  "*                               @"\
  "                               *@"\
  "*                               @"\
  "                               *@"\
  "*                               @"\
  "                               *@"\
  "*                               @"\
  "                               *@"\
  "*  > PRESS CTRL-B TO CANCEL <   @"\
  " * * * * * * * * * * * * * * * *@"
} ;
//------------------------------------------------------------------------------
char	_etbfonte1::pTexteParametres[]=
{
  "################################@"\
  "# ] num-level          :??     #@"\
  "#   allez au gardien   :??     #@"\
  "#   vaisseau maximum           #@"\
  "#   detruit gardien    :??     #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#                              #@"\
  "#  > PRESS CTRL-P TO CANCEL <  #@"\
  "################################@" 
} ;

// constructeur ****************************************************************
_etbfonte1::_etbfonte1()
{
	iAction = 0;
	iTempo = 0;
	iIndexTouch = -1;
	iParamPosY = 0;
	iTempoClavier = 0;
	num_level_sauve = 0;
	allez_gardien = 14;
	pFonte = NULL;
}

// destructeur *****************************************************************
_etbfonte1::~_etbfonte1()
{
	if(pFonte)
	{	libereMemoire((char *)pFonte);
		pFonte = NULL;
	}
}

//******************************************************************************
int _etbfonte1::initialise()
{
  pFonte = (unsigned char *)Xload_pcx("graphics/etbfonte1.pcx");
  if(!pFonte)
    return 0;
  return 1;
}

//******************************************************************************
void _etbfonte1::execution()
{
  switch (iAction)
  {
    case 1:
      afficheAbout();
      break;

#ifdef DEVELOPPEMENT2
    case 2:
      afficheVariables();
      break;
    case 3:
      afficheParametres();
      break;
    case 4:
      afficheVariables2();
      break;
#endif
  }

  if(iIndexTouch != -1)
  {
    if(touch[iIndexTouch] == 0)
      iIndexTouch = -1;
  }
  else
  {
    if(touch[88] == 1)                                         //touche control pressee ?
    {
      if(touch[38] == 1)                                       //touche [a] ?
      { iIndexTouch = 38;
        if(iAction == 1)
          iAction = 0;
        else if(iAction == 0)
          iAction = 1;
      }
#ifdef DEVELOPPEMENT2
      if(touch[79] == 1)                                       //touche [v] ?
      {
        iIndexTouch = 79;
        if(iAction == 2)      iAction = 0;
        else if(iAction == 0) iAction = 2;
      }
      if(touch[80] == 1)                                       //touche [b] ?
      {
        iIndexTouch = 80;
        if(iAction == 4)
          iAction = 0;
        else if(iAction == 0)
          iAction = 4;
      }
      if(touch[47] == 1 && etat_menu == MENU_OFF)              //touche [p] ?
      {
        iIndexTouch = 47;
        if(iAction == 3)
        {
          iAction = 0;
        }
        else
        {
          if(iAction == 0)
          {
            iAction = 3;
            num_level_sauve = num_level;
          }
        }
      }
#endif
    }
#ifdef DEVELOPPEMENT2
    else
    {
      if(touch[50] == 1)                                       // touche RETURN pressee ?
      {
        switch (iAction)
        {
          case 3:                                             // parametres (CTRL+P)
          {
            int i;

            switch (iParamPosY)
            {
              case 0:                                         // change le numero du niveau
                if(num_level_sauve != num_level)
                {
                  num_level = num_level_sauve;
                  change_niveau();
                  move_level = TRUE;
                  pos_x_level = 0.0;
                }
                player_pause = 0;
                aff_pause = FALSE;
                iAction = 0;
                break;
              case 1:                                         // change le numero du gardien
                init_gardien();
                player_pause = 0;
                aff_pause = FALSE;
                iAction = 0;
                break;
              case 2:                                         // vaisseau maximum
                vj[J1].type_vaisseau = V5J;                    // type de vaisseau 
                vj[J1].power_avant = 5;                        // Indique l'option tir avant actuellement sélectionné par le vaisseau.
                vj[J1].power_gauche = 5;                       // Indique l'option tir gauche actuellement sélectionné par le vaisseau.
                vj[J1].power_droite = 5;                       // Indique l'option tir droite actuellement sélectionné par le vaisseau.
                vj[J1].power_arriere = 5;                      // Indique l'option tir arriere actuellement sélectionné par le vaisseau.
                vj[J1].power_spec_avant = 5;                   // Indique l'option tir special avant actuellement sélectionné par le vaisseau.
                vj[J1].power_spec_arriere = 5;                 // Indique l'option tir special arriere actuellement sélectionné par le vaisseau.
                vj[J1].power_spec_droite = 5;                  // Indique l'option tir special droite actuellement sélectionné par le vaisseau.
                vj[J1].power_spec_gauche = 5;                  // Indique l'option tir special gauche actuellement sélectionné par le vaisseau.
                vj[J1].spr.etat_dommages = vj[J1].spr.pow_of_dest;      // Indique l'état de domages du sprite.
                bar_nrj_player = TRUE;
                while (nbr_boules < 5)
                {
                  ajoute_boule();
                }
                eclair_actif = TRUE;
                for(i = 0; i < NBR_MAX_IMG_VAIS_J; i++)
                {
                  vj[J1].spr.img[i] = (image *) & vj1[vj[J1].type_vaisseau][i]; // Donne l'adresse des images.
                }
                player_pause = 0;
                aff_pause = FALSE;
                iAction = 0;
                break;
              case 3:                                         // detruit gardien
                if(gardian_activity)
                {
                  if(ve[iNumeroGardien].prev_object != MAX_NMI_ON_SCREEN)
                  {
                    ve[iNumeroGardien].spr.etat_dommages = -1;
                  }
                }
                player_pause = 0;
                aff_pause = FALSE;
                iAction = 0;
            }
          }
            break;
        }
      }
    }
#endif
  }
}

//******************************************************************************
void _etbfonte1::afficheParametres()
{
#ifdef DEVELOPPEMENT2
  player_pause = 1;
  aff_pause = 1;
  char                   *_pTexte = pTexteParametres;
  int _iChange = 0;

  *(_pTexte + (33 * (1 + iParamPosY) + 2)) = ' ';
  if(iTempoClavier < 1)
  {
    if(touch[87]) { iParamPosY++; iTempoClavier = 8; }
    else
    { if(touch[55]) { iParamPosY--; iTempoClavier = 8; }
      else
      {
        if(touch[71])                                          // gauche
        { _iChange = -1; iTempoClavier = 8;
        }
        else
        { if(touch[73])                                        // droite 
          { _iChange = 1; iTempoClavier = 8;
          }
        }
      }
    }
  }
  else
    iTempoClavier--;
  if(iParamPosY < 0) iParamPosY = 3;
  if(iParamPosY > 3) iParamPosY = 0;
  *(_pTexte + (33 * (1 + iParamPosY) + 2)) = ']';
  if(_iChange != 0)
  {
    switch (iParamPosY)
    {
      case 0:                                                 // change le numero du niveau
        num_level_sauve = num_level_sauve + _iChange;
        if(num_level_sauve < 0) num_level_sauve = NBR_LEVEL_MAX;
        if(num_level_sauve > NBR_LEVEL_MAX) num_level_sauve = 0;
        break;
      case 1:                                                 // change le numero du gadien
        allez_gardien = allez_gardien + _iChange;
        if(allez_gardien < 1) allez_gardien = 14;
        if(allez_gardien > 14) allez_gardien = 1;
        break;
      case 2:                                                 // active le vaisseau maximum
        break;
      case 3:                                                 // detruit le gardien
        break;
    }
  }
  entier2ASCII(num_level_sauve, _pTexte + (33 * 1) + 24, 1);
  entier2ASCII(allez_gardien, _pTexte + (33 * 2) + 24, 1);
  entier2ASCII(gardian_activity, _pTexte + (33 * 4) + 24, 1);
  afficheTexte(0, 0, _pTexte);
#endif
}

//******************************************************************************
void _etbfonte1::afficheAbout()
{
  afficheTexte(0, 0, pTexteAbout);
}

//******************************************************************************
void _etbfonte1::afficheVariables()
{
#ifdef DEVELOPPEMENT2
  char                   *_pTexte = pTexteVariables;
  entier2ASCII(num_level, _pTexte + (33 * 1) + 12, 1);
  entier2ASCII(etat_menu, _pTexte + (33 * 1) + 28, 0);
  entier2ASCII(player_pause, _pTexte + (33 * 2) + 15, 0);
  entier2ASCII(aff_pause, _pTexte + (33 * 2) + 28, 0);
  entier2ASCII(game_over_player_one, _pTexte + (33 * 3) + 23, 0);
  entier2ASCII(aff_game_over, _pTexte + (33 * 4) + 16, 0);
  entier2ASCII(quit_game, _pTexte + (33 * 4) + 29, 0);
  entier2ASCII(apparition_gardian, _pTexte + (33 * 5) + 21, 0);
  entier2ASCII(gardian_activity, _pTexte + (33 * 6) + 19, 1);
  entier2ASCII(meteor_activity, _pTexte + (33 * 7) + 18, 0);
  entier2ASCII(grille.activity, _pTexte + (33 * 8) + 18, 0);
  entier2ASCII(courbe.activity, _pTexte + (33 * 9) + 18, 0);
  entier2ASCII(touch[90], _pTexte + (33 * 10) + 12, 0);
  entier2ASCII(nbr_nmis, _pTexte + (33 * 11) + 11, 2);
  entier2ASCII(_score::getActionExecution(), _pTexte + (33 * 12) + 30, 0);
  entier2ASCII(vmode, _pTexte + (33 * 13) + 25, 0);
  entier2ASCII(tmp_apparition_vj, _pTexte + (33 * 14) + 20, 3);
  entier2ASCII(disparition_vj, _pTexte + (33 * 15) + 18, 3);
  entier2ASCII(vj[J1].cad_tir, _pTexte + (33 * 16) + 17, 3);
  entier2ASCII(iNombreZones, _pTexte + (33 * 17) + 17, 4);
  entier2ASCII(iOctetsTotal, _pTexte + (33 * 18) + 17, 7);
  entier2ASCII(iOctetsTotal / 1024, _pTexte + (33 * 18) + 26, 4);
  entier2ASCII(taille_mem_glob, _pTexte + (33 * 19) + 18, 7);
  afficheTexte(0, 0, _pTexte);
#endif
}

//affiche valeurs relatives au son (seal) **************************************
#ifdef DEVELOPPEMENT2
void _etbfonte1::afficheVariables2()
{
  char                   *_pTexte = pTexteVariables2;

#ifdef USE_SDLMIXER2
  unsigned int i;
  char                   *_p = _pTexte + (33 * 2) + 2;
  int iEtatVoix = 0;
  for(i = 0; i < iBruitagesVoix; i++)
  { AGetVoiceStatus(pBruitages[i], &iEtatVoix);
    entier2ASCII(iEtatVoix, _p, 0);
    _p += 1;
  }
  if(iBruitagesVoix != 16)
  { for(i = 0; i < (16 - iBruitagesVoix); i++)
    { *(_p++) = 'x';
    }
  }
  entier2ASCII(iBruitage, _pTexte + (33 * 3) + 12, 1);
  _p = _pTexte + (33 * 5) + 2;
  for(i = 0; i < 26; i++)
  { entier2ASCII(iFlagBruitages[i], _p, 0);
    _p += 1;
  }
  entier2ASCII(iTailleSons, _pTexte + (33 * 6) + 14, 7);
  entier2ASCII(power_conf->difficulty, _pTexte + (33 * 7) + 12, 0);
  //joue un bruitage par touche
  int _iCanal = -1;
  int _iOffset = 0;

  if(touch[74]) _iOffset = 10;                                             //[shift] presse
  if(touch[88]) _iOffset = 20;                                             //[ctrl] presse

  if(touch[17]) { _iCanal = 0; iBruitage = 00 + _iOffset; }               //on joue un son avec les touches 
  if(touch[18]) { _iCanal = 1; iBruitage = 01 + _iOffset; }
  if(touch[19]) { _iCanal = 2; iBruitage = 02 + _iOffset; }
  if(touch[20]) { _iCanal = 3; iBruitage = 3 + _iOffset; }
  if(touch[21]) { _iCanal = 0; iBruitage = 4 + _iOffset; }
  if(touch[22]) { _iCanal = 1; iBruitage = 5 + _iOffset; }
  if(touch[23]) { _iCanal = 2; iBruitage = 6 + _iOffset; }
  if(_iOffset < 20)
  { if(touch[24]) { _iCanal = 3; iBruitage = 7 + _iOffset; }
    if(touch[25]) { _iCanal = 0; iBruitage = 8 + _iOffset; }
    if(touch[26]) { _iCanal = 1; iBruitage = 9 + _iOffset; }
  }
  if(_iCanal >= 0)
  { AGetVoiceStatus(pBruitages[_iCanal], &iEtatVoix);
    if(iEtatVoix == 1) { APlayVoice(pBruitages[_iCanal], pSons[iBruitage]); }
  }
#endif
  afficheTexte(0, 0, _pTexte);
}
#endif
//******************************************************************************
void _etbfonte1::afficheTexte(int _iPosX, int _iPosY, char *_pTexte)
{
  unsigned char          *_pEcran = (unsigned char *)
    (ecran_ram +
     ((DEBUT_ECRAN_Y + _iPosY) * LARG_ECR_RAM_TOT +
      DEBUT_ECRAN_X + _iPosX) * iOctetsParPixel);

  unsigned char          *_pDestination = _pEcran;
  unsigned char _cCaractere = *(_pTexte++);

  while (_cCaractere != 0)
  {
    if(_cCaractere == '@')
    {
      _pEcran = _pEcran + (8 * LARG_ECR_RAM_TOT * iOctetsParPixel);
      _pDestination = _pEcran;
    }
    else
    {
      unsigned int _iOffset = decode(_cCaractere);

      if(_iOffset != 26 * 8)
      {
        unsigned char          *_pSource =
          pFonte + _iOffset * iOctetsParPixel;
        //printf("%c", _cCaractere) ;
        _ASMCOPYFONTE(_pDestination, _pSource);
      }
      _pDestination = _pDestination + (8 * iOctetsParPixel);
    }
    _cCaractere = *(_pTexte++);
  }
}

//******************************************************************************
unsigned int _etbfonte1::decode(unsigned char _cCaractere)
{
  if(_cCaractere >= 'A' && _cCaractere <= 'Z')
  {
    _cCaractere = _cCaractere - 'A';
  }
  else
  {
    if(_cCaractere >= 'a' && _cCaractere <= 'z')
    {
      _cCaractere = _cCaractere - 'a';
    }
    else
    {
      if(_cCaractere >= '0' && _cCaractere <= '9')
      {
        _cCaractere = _cCaractere - '0' + 27;
      }
      else
      {
        switch (_cCaractere)
        {
          case ' ':
            _cCaractere = 26;
            break;
          case '!':
            _cCaractere = 37;
            break;
          case ';':
            _cCaractere = 38;
            break;
          case ':':
            _cCaractere = 39;
            break;
          case 39:                                            // (apostrophe)
            _cCaractere = 40;
            break;
          case '-':
            _cCaractere = 41;
            break;
          case '.':
            _cCaractere = 42;
            break;
          case '?':
            _cCaractere = 43;
            break;
          case '<':
            _cCaractere = 44;
            break;
          case '>':
            _cCaractere = 45;
            break;
          case '&':
            _cCaractere = 46;
            break;
          case '*':
            _cCaractere = 47;
            break;
          case '(':
            _cCaractere = 48;
            break;
          case ')':
            _cCaractere = 49;
            break;
          case '#':
            _cCaractere = 50;
            break;
          case '_':
            _cCaractere = 51;
            break;
          case '=':
            _cCaractere = 52;
            break;
          case '[':
            _cCaractere = 53;
            break;
          case ']':
            _cCaractere = 54;
            break;
          case '|':
            _cCaractere = 55;
            break;
          default:
            _cCaractere = 43;
            break;
        }
      }

    }
  }

  return _cCaractere * 8;
}

// -----------------------------------------------------------------------------
// Convertit un nombre en chaine ASCII 
// entree => _iNombre   :       nombre a convertit
//                              => _pASCII      :pointeur sunr une chaine ASCII (terminee par un zero)
// -----------------------------------------------------------------------------
void _etbfonte1::entier2ASCII(int _iNombre, char *_pASCII,
                              unsigned int _iReste)
{
  unsigned int _iIndex = 0;
  unsigned int _iTaille = 1;

  if(_iNombre < 0)
  {
    _iNombre = -_iNombre;
    *(_pASCII++) = '-';
    if(_iReste > 0)
      _iReste--;
  }
  for(_iIndex = 0; _iIndex < _iReste; _iIndex++)
  {
    _iTaille = _iTaille * 10;
  }
  _iIndex = 0;
  while (_iTaille > 0)
  {
    unsigned int _iReste = _iNombre / _iTaille;
    char _cASCII = (char)('0' + _iReste);

    if(_cASCII > '9' || _cASCII < '0')
      _cASCII = '?';
    _pASCII[_iIndex++] = _cASCII;
    _iNombre = _iNombre - (_iTaille * _iReste);
    _iTaille = _iTaille / 10;
  }
}

// -----------------------------------------------------------------------------
// Change de niveau
// -----------------------------------------------------------------------------
void _etbfonte1::change_niveau()
{
  if(num_level >= 0 && num_level < 4)
    Load_Gardien(1);
  if(num_level > 3 && num_level < 8)
    Load_Gardien(2);
  if(num_level > 7 && num_level < 12)
    Load_Gardien(3);
  if(num_level > 11 && num_level < 16)
    Load_Gardien(4);
  if(num_level > 15 && num_level < 20)
    Load_Gardien(5);
  if(num_level > 19 && num_level < 24)
    Load_Gardien(6);
  if(num_level > 23 && num_level < 28)
    Load_Gardien(7);
  if(num_level > 27 && num_level < 32)
    Load_Gardien(8);
  if(num_level > 31 && num_level < 36)
    Load_Gardien(9);
  if(num_level > 35 && num_level < 40)
    Load_Gardien(10);
  if(num_level > 39 && num_level < 42)
    Load_Gardien(11);
  LoadGrid(num_level);
  Load_Curve_Level(num_level);
  for(unsigned int i = 0; i < (MAX_TYP_OF_METEOR); i++)
  {
    for(unsigned int j = 0; j < NBR_IMAGES_METEOR; j++)
    {
      if(meteo[i][j].img)
      {
        libereMemoire(meteo[i][j].img);
        meteo[i][j].img = 0;
      }
      if(meteo[i][j].compress)
      {
        libereMemoire(meteo[i][j].compress);
        meteo[i][j].compress = 0;
      }
    }
  }
  Load_Meteor(num_level);
  Active_Curve_Level();
  courbe.activity = TRUE;
  grille.activity = FALSE;
  meteor_activity = FALSE;
  gardian_activity = FALSE;
}

// -----------------------------------------------------------------------------
// va directement a un gardien
// -----------------------------------------------------------------------------
void _etbfonte1::init_gardien()
{
  unsigned int i;

  switch (allez_gardien)
  {
    case 1:
      num_level_sauve = 3;
      break;
    case 2:
      num_level_sauve = 7;
      break;
    case 3:
      num_level_sauve = 11;
      break;
    case 4:
      num_level_sauve = 15;
      break;
    case 5:
      num_level_sauve = 19;
      break;
    case 6:
      num_level_sauve = 23;
      break;
    case 7:
      num_level_sauve = 27;
      break;
    case 8:
      num_level_sauve = 31;
      break;
    case 9:
      num_level_sauve = 35;
      break;
    case 10:
      num_level_sauve = 39;
      break;
    case 11:
      num_level_sauve = 41;
      break;
    case 12:
      num_level_sauve = 41;
      break;
    case 13:
      num_level_sauve = 41;
      break;
    case 14:
      num_level_sauve = 41;
      break;
  }
  num_level = num_level_sauve;
  Init_Tab_Ve();
  for(i = 0; i < MAX_NMI_ON_SCREEN; i++)
    liste_ve[i] = 0;
  Init_Tab_Tirs();
  for(i = 0; i < NBR_TIRS_MAX; i++)
    liste_tirs[i] = 0;
  Init_Tab_OndeChoc();
  for(i = 0; i < NBR_ONDE_CHOC_MAX; i++)
    liste_onde_choc[i] = 0;
  Load_Gardien(allez_gardien);
  Meteor_Activity_With_Guardian(allez_gardien);
}

//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "displayFonte.cpp"
// created      : 1999-09-05
// updates      : 2005-01-12
// id           : $Id: displayFonte.cpp,v 1.2 2005/01/26 08:36:51 patriceduhamel Exp $
//-----------------------------------------------------------------------------
// functions    : void  Aff_Score_Player_One()
//              : void  Aff_Level_Player_One(int dec_x, int dec_y, int numero_level)
//              : void Aff_Game_Pause_Player_One()
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
#include "configfile.hpp"
//
// player's score ..............................................................
extern configfile *power_conf;
fonte fnt_score[NBR_FONTES_SCORE][NBR_IMAGES_FONTE];	//fontes's data structure
int player_score;		//new score value
int old_player_score;		//old score value
play_score aff_player_score[10];	//score's fontes structure

// level number ................................................................
static const Uint32 MAXIOFCHARS = 32;
static const Uint32 NBR_CHIF_LEVEL = MAXIOFCHARS; //number of chars 
bool move_level = 0;		//1=text of level number move
play_score aff_player_level[NBR_CHIF_LEVEL];	//level's fontes structure
int num_level = -1;		//level number
float pos_x_level = 0;
float pos_y_level = 0;

//..............................................................................

fonte fnt_game[NBR_FONTES_GAME][NBR_IMAGES_FONTE];	//fontes's data
play_score aff_player_pause[NBR_CHIF_PAUSE];
fonte fnt_big[NBR_FONTES_BIG][NBR_IMAGES_FONTE];

//..............................................................................
const int iBAR_NRJ_PLAYER_LARGE = 100;
int num_let_game_over;		//numero de la lettre a animer 
int cmpt_vbl_let_game_over;	//temps qu'il faut avant de faire tourner une lettre du game over.
int nbr_vbl_game_over;		//compteur de temps qu'il faut avant de faire tourner une lettre du game over.
image bar_nrj_red;		//affichage de la barre d'energie rouge
image bar_nrj_green;		//affichage de la barre d'energie verte
image bar_nrj_blue;		//affichage de la barre d'énergie bleue
extern bool bar_nrj_player;
extern bool bar_nrj_guardian;
extern char *ecran_scr;
extern Uint32 iOctetsParPixel;	//number of bytes per pixels (1=256 colors / 2=65536 colors)
extern vaisseau vj[NBR_MAX_J];	//player's ship structure
extern int gardian_activity;	//1="gardien" phase (big boss)
extern bool meteor_activity;	//1="meteors" phase
extern vaisseau_nmi ve[MAX_NMI_ON_SCREEN];	//structure des vaisseaux ennemis.
extern int liste_ve[MAX_NMI_ON_SCREEN];	//liste de tous les vaisseaux ennemis a gerer
extern short nbr_nmis;		//nombre d'enemis actuellement geres

//..............................................................................
void update_nrj (Uint32 _iBarreLargeMax,
		 int _iDommage, Uint32 _iPosX, Uint32 _iBarreLarge);
Uint32 iBarreNrjJoueurXWindow = 0;	//1=update player's enerny bar-line 
Uint32 iBarreNrjGardienXWindow = 0;	//1=update bigboss's energy bar-line
Uint32 iScoreXWindow = 0;	//1=update player's score points
static char pASCIIscore[10] = { "????????\0" };

//static char             pASCIIlevel[32] = { "LEVEL??\0\0" };
char pASCIIlevel[32];

// game texts
static const Uint32 NUMBEROFTEXTS = 4;
char pASCIIleveltxt[MAXIOFCHARS] = "level\0";
char pASCIIpause[MAXIOFCHARS] = "pause\0";
char pASCIIgame[MAXIOFCHARS] = "game\0";
char pASCIIover[MAXIOFCHARS] = "over\0";
char *pASCIItext[NUMBEROFTEXTS] = { pASCIIleveltxt,
  pASCIIpause, pASCIIgame, pASCIIover
};


//------------------------------------------------------------------------------
// display score number
//------------------------------------------------------------------------------
void
Aff_Score_Player_One ()
{
  Uint32 _iIndex;
  Uint32 _iOffset;
  Uint32 i;

  if (player_score != old_player_score)	//score update ?
    for (i = 0; i < NBR_CHIF_SCORE; i++)
      aff_player_score[i].anim_font = 1;	//1=update display
  old_player_score = player_score;
  iScoreXWindow = 1;		//1=update player's score points
  for ( i = 0; i < NBR_CHIF_SCORE; i++)
    {
      if (aff_player_score[i].anim_font)
	{
	  entier2ASCII (player_score, pASCIIscore, NBR_CHIF_SCORE - 1);
	  for (_iIndex = 0; _iIndex < NBR_CHIF_SCORE; _iIndex++)
	    {
	      if (aff_player_score[_iIndex].anim_font)
		{
		  aff_player_score[_iIndex].nbr_vbl_act += aff_player_score[_iIndex].add_vbl;	//increase counter tempo
		  if (aff_player_score[_iIndex].nbr_vbl_act >= aff_player_score[_iIndex].nbr_tot_vbl)	//change image?
		    {
		      aff_player_score[_iIndex].nbr_vbl_act =
			(aff_player_score[_iIndex].nbr_vbl_act -
			 aff_player_score[_iIndex].nbr_tot_vbl);
		      aff_player_score[_iIndex].img_act++;	//next image
		      if (aff_player_score[_iIndex].img_act >= NBR_IMAGES_FONTE)	//last image ?
			{
			  aff_player_score[_iIndex].img_act = 0;	//set first image
			  aff_player_score[_iIndex].anim_font = 0;
			}
		    }
		  _iOffset = pASCIIscore[_iIndex] - 48;
		  putsprite3_scr (&fnt_score[_iOffset]
				  [aff_player_score[_iIndex].img_act],
				  68 + (_iIndex * 16), 0);
		  aff_player_score[_iIndex].old_val_font = _iOffset;
		  aff_player_score[_iIndex].old_img_font =
		    aff_player_score[_iIndex].img_act;
		}
	    }
	  return;
	}
    }
}

//------------------------------------------------------------------------------
// display the level number
//------------------------------------------------------------------------------
void
Aff_Level_Player_One ()
{
  Uint32 i, j;
  Uint32 val_atoi;
  Uint32 _iNombreCaracteres;
  Uint32 pos_x = DEBUT_ECRAN_X + ((LARG_ECR_RAM - (8 * LARG_FONTE_GAME)) / 2);
  Uint32 pos_y = DEBUT_ECRAN_Y + ((HAUT_ECR_RAM - HAUT_FONTE_GAME) / 2);
  Uint32 dec_x;
  Uint32 dec_y;
  Uint32 numero_level;

  if (!move_level)
    return;
      
      
      if (meteor_activity || gardian_activity)	//phase "meteor" ou "gardien" ?
	{
	  numero_level = num_level + 1;
	  pos_x_level -= 2;	//oui, deplace le texte horizontalement
	  if (pos_x_level < 0.0)
	    pos_x_level = 0.0;	//bloque l'abscisse a zero
	  dec_x = (Uint32) pos_x_level;
	  dec_y = (Uint32) pos_y_level;
	}
      else
	{
	  numero_level = num_level;
	  pos_y_level += 0.4f;	//sinon, deplace le texte verticalement
	  for (i = 0; i < NBR_CHIF_LEVEL; i++)
	    aff_player_level[i].anim_font = 1;	//flag d'animation des caracteres a 1
	  dec_x = (Uint32) pos_x_level;
	  dec_y = (Uint32) pos_y_level;
	}

          sprintf (pASCIIlevel, "%s%d", pASCIIleveltxt, numero_level);
          _iNombreCaracteres = strlen (pASCIIlevel);


      j = strlen (pASCIIleveltxt) - 1;
      for (i = 0; i < _iNombreCaracteres; i++)
	{
	  if (aff_player_level[i].anim_font)	//change fonte ?
	    {
	      aff_player_level[i].nbr_vbl_act += aff_player_level[i].add_vbl;
	      if (aff_player_level[i].nbr_vbl_act >=
		  aff_player_level[i].nbr_tot_vbl)
		{
		  aff_player_level[i].nbr_vbl_act =
		    (short) (aff_player_level[i].nbr_vbl_act -
			     aff_player_level[i].nbr_tot_vbl);
		  aff_player_level[i].img_act++;
		  if (aff_player_level[i].img_act >= NBR_IMAGES_FONTE)
		    {
		      aff_player_level[i].img_act = 0;
		      aff_player_level[i].anim_font = 0;
		    }
		}
	    }
	  val_atoi = pASCIIlevel[i] - 33;
	  if ((pos_x + dec_x) <= (LARG_ECR_RAM + BANDE_DE_CLIP)
	      && (pos_y + dec_y) <= (HAUT_ECR_RAM + BANDE_DE_CLIP))
	    putsprite3 (&fnt_game[val_atoi][aff_player_level[i].img_act],
			pos_x + dec_x + (i * LARG_FONTE_GAME), pos_y + dec_y);
	  if (i == j)
	    pos_x += LARG_FONTE_GAME;
	}

      if (pos_y_level > 100)	//stop display ? 
	{
	  move_level = 0;	//yes
	  pos_x_level = 128.0;	//
	  pos_y_level = 0.0;	//
	  for (i = 0; i < _iNombreCaracteres; i++)
	    {
	      aff_player_level[i].img_act = 0;
	      aff_player_level[i].anim_font = 1;
	    }
	}
}

//------------------------------------------------------------------------------
// display "PAUSE"
//------------------------------------------------------------------------------
void
Aff_Game_Pause_Player_One (void)
{
  //char                    string[6] = "PAUSE";
  Uint32 i, val_atoi;

  for (i = 0; i < 5; i++)
    {
      val_atoi = pASCIIpause[i] - 65;	//offset
      if (aff_player_pause[i].anim_font)	//display fonte ?
	{
	  aff_player_pause[i].nbr_vbl_act += aff_player_pause[i].add_vbl;
	  if (aff_player_pause[i].nbr_vbl_act >=
	      aff_player_pause[i].nbr_tot_vbl)
	    {
	      aff_player_pause[i].nbr_vbl_act =
		(short) (aff_player_pause[i].nbr_vbl_act -
			 aff_player_pause[i].nbr_tot_vbl);
	      aff_player_pause[i].img_act++;
	      if (aff_player_pause[i].img_act >= NBR_IMAGES_FONTE)
		{
		  aff_player_pause[i].img_act = 0;
		  aff_player_pause[i].anim_font = 0;
		}
	    }
	}
      putsprite3 (&fnt_big[val_atoi][aff_player_pause[i].img_act],
		  175 + (i * 33), 205);
    }
  cmpt_vbl_let_game_over++;	//increase counter
  if (cmpt_vbl_let_game_over > nbr_vbl_game_over)	//change image ?
    {
      cmpt_vbl_let_game_over = 0;	//yes, raz counter
      switch (num_let_game_over)
	{
	case 0:
	  aff_player_pause[0].anim_font = 1;
	  break;
	case 1:
	  aff_player_pause[1].anim_font = 1;
	  break;
	case 2:
	  aff_player_pause[2].anim_font = 1;
	  break;
	case 3:
	  aff_player_pause[3].anim_font = 1;
	  break;
	case 4:
	  aff_player_pause[4].anim_font = 1;
	  cmpt_vbl_let_game_over = -200;
	  break;
	}
      num_let_game_over++;
      if (num_let_game_over > 4)
	num_let_game_over = 0;	//set next image
    }
}

//------------------------------------------------------------------------------
// initialize fontes's structures
//------------------------------------------------------------------------------
void
initializeFonte ()
{
  Uint32 i;

  //player's score
  for (i = 0; i < NBR_CHIF_SCORE; i++)
    {
      aff_player_score[i].old_val_font = 0;
      aff_player_score[i].old_img_font = 0;
      aff_player_score[i].anim_font = 1;
      aff_player_score[i].img_act = 0;
      aff_player_score[i].nbr_tot_vbl = 1000;
      switch (i)
	{
	case 0:
	  aff_player_score[i].add_vbl = 550;
	  break;
	case 1:
	  aff_player_score[i].add_vbl = 525;
	  break;
	case 2:
	  aff_player_score[i].add_vbl = 500;
	  break;
	case 3:
	  aff_player_score[i].add_vbl = 475;
	  break;
	case 4:
	  aff_player_score[i].add_vbl = 450;
	  break;
	case 5:
	  aff_player_score[i].add_vbl = 425;
	  break;
	case 6:
	  aff_player_score[i].add_vbl = 400;
	  break;
	case 7:
	  aff_player_score[i].add_vbl = 375;
	  break;
	}
      aff_player_score[i].nbr_vbl_act = 0;
    }
  player_score = 0;
  old_player_score = -1;
  //text "PAUSE"
  for (i = 0; i < NBR_CHIF_LEVEL; i++)
    {
      aff_player_level[i].anim_font = 1;
      aff_player_level[i].nbr_tot_vbl = 1000;
      aff_player_level[i].add_vbl = 500;
    }
  for (i = 0; i < 100; i++)
    putsprite3_scr (&bar_nrj_blue, 210 + i, 3);

}

//------------------------------------------------------------------------------
// display player's ernergy
//------------------------------------------------------------------------------
void
update_bar_nrj_player ()
{
  if (bar_nrj_player)
    {
      update_nrj (iBAR_NRJ_PLAYER_LARGE, vj[J1].spr.etat_dommages, 210,
		  vj[J1].type_vaisseau * 20 + 20);
      bar_nrj_player = 0;
      iBarreNrjJoueurXWindow = 1;	//1=update player's enerny bar-line 
    }
}

//------------------------------------------------------------------------------
// display bigboss's energy
//------------------------------------------------------------------------------
void
gardienBarreNRJ ()
{
  int i;
  Uint32 _iIndex, _iDommages;

  if (bar_nrj_guardian)
    {
      iBarreNrjGardienXWindow = 1;	//1=update bigboss's energy bar-line
      bar_nrj_guardian = 0;
      for (i = 0; i < nbr_nmis; i++)	//search the big boss
	{
	  _iIndex = liste_ve[i];
	  if (ve[_iIndex].deplacement == GARDIEN)	//big boss found ?
	    {
	      _iDommages =
		(ve[_iIndex].spr.etat_dommages * 45) /
		ve[_iIndex].spr.etat_max;
	      update_nrj (45, _iDommages, 10, 45);
	      return;
	    }
	  update_nrj (45, 0, 10, 45);	//big boss no found
	}
    }
}

//------------------------------------------------------------------------------
// generic display ernergy
// input => _iBarreLargeMax : maximum width of energy barline
//       => _iDommage       :  damage value (0=damage maxi)
//       => _iPosX          : abcissa
//       => _iBarreLarge    : width of energy barline
//------------------------------------------------------------------------------
void
update_nrj (Uint32 _iBarreLargeMax, int _iDommage, Uint32 _iPosX,
	    Uint32 _iBarreLarge)
{
  int i, j, k;
  image *_pImage;
  char *_pSource;
  char *_pDestination;
  char *_pCompression;
  Uint32 _iTaille;
  _pDestination = ecran_scr + (3 * SCR_LARGE + _iPosX) * iOctetsParPixel;
  _pImage = &bar_nrj_green;
  _pSource = _pImage->img;
  _pCompression = _pImage->compress;
  _iTaille = _pImage->nbr_data_comp >> 2;
  k = _iDommage;
  switch (iOctetsParPixel)
    {
    case 1:
      for (i = 0; i < k; i++)
	{
	  putsprite3_8bits (_pSource, _pDestination, _pCompression, _iTaille);
	  _pDestination += 1;
	}
      break;
    case 2:
      for (i = 0; i < k; i++)
	{
	  putsprite3_16bits (_pSource, _pDestination, _pCompression,
			     _iTaille);
	  _pDestination += 2;
	}
      break;
    case 3:
      for (i = 0; i < k; i++)
	{
	  putsprite3_24bits (_pSource, _pDestination, _pCompression,
			     _iTaille);
	  _pDestination += 3;
	}
      break;
    case 4:
      for (i = 0; i < k; i++)
	{
	  putsprite3_32bits (_pSource, _pDestination, _pCompression,
			     _iTaille);
	  _pDestination += 4;
	}
      break;
    }

  if (_iDommage > 0)
    {
      _pImage = &bar_nrj_red;
      _pSource = _pImage->img;
      _pCompression = _pImage->compress;
      _iTaille = _pImage->nbr_data_comp >> 2;
      j = _iBarreLarge;
      k = j - _iDommage;
      switch (iOctetsParPixel)
	{
	case 1:
	  for (i = 0; i < k; i++)
	    {
	      putsprite3_8bits (_pSource, _pDestination, _pCompression,
				_iTaille);
	      _pDestination += 1;
	    }
	  break;
	case 2:
	  for (i = 0; i < k; i++)
	    {
	      putsprite3_16bits (_pSource, _pDestination, _pCompression,
				 _iTaille);
	      _pDestination += 2;
	    }
	  break;
	case 3:
	  for (i = 0; i < k; i++)
	    {
	      putsprite3_24bits (_pSource, _pDestination, _pCompression,
				 _iTaille);
	      _pDestination += 3;
	    }
	  break;
	case 4:
	  for (i = 0; i < k; i++)
	    {
	      putsprite3_32bits (_pSource, _pDestination, _pCompression,
				 _iTaille);
	      _pDestination += 4;
	    }
	  break;
	}
    }
  else
    {
      j = 0;
      _pDestination = ecran_scr + (3 * SCR_LARGE + _iPosX) * iOctetsParPixel;
    }

  _pImage = &bar_nrj_blue;
  _pSource = _pImage->img;
  _pCompression = _pImage->compress;
  _iTaille = _pImage->nbr_data_comp >> 2;
  k = _iBarreLargeMax;
  switch (iOctetsParPixel)
    {
    case 1:
      for (i = j; i < k; i++)
	{
	  putsprite3_8bits (_pSource, _pDestination, _pCompression, _iTaille);
	  _pDestination += 1;
	}
      break;
    case 2:
      for (i = j; i < k; i++)
	{
	  putsprite3_16bits (_pSource, _pDestination, _pCompression,
			     _iTaille);
	  _pDestination += 2;
	}
      break;
    case 3:
      for (i = j; i < k; i++)
	{
	  putsprite3_24bits (_pSource, _pDestination, _pCompression,
			     _iTaille);
	  _pDestination += 3;
	}
      break;
    case 4:
      for (i = j; i < k; i++)
	{
	  putsprite3_32bits (_pSource, _pDestination, _pCompression,
			     _iTaille);
	  _pDestination += 4;
	}
      break;
    }
}


//------------------------------------------------------------------------------
// load game texts
//------------------------------------------------------------------------------
int
LoadGameText ()
{
  char *pLang = power_conf->get_txtlang ();
  char fname[256];
  sprintf (fname, "texts/text_%s.txt", pLang);
  Uint32 bsize;
  char *pTemp = chargeFichier (fname, &bsize);
  if (!pTemp)
    return 0;

  //##############################################################
  // read line by line    
  //##############################################################
  Uint32 vcount = 0;
  Uint32 i = 0;
  Uint32 j = 0;
  
  //char *p = pASCIItext[i];

  while (vcount < bsize)
  {
    char c = pTemp[vcount++];
    if (c == '\n')
	{	  
		if (i >= NUMBEROFTEXTS)
	    {
	      fprintf (stderr,
		       "LoadGameText() too many lines in %s\n", fname);
	      break;
	    }
		else
	    {
		  //p[j] = '\0';
		  pASCIItext[i][j] = '\0';
	      
	      j = 0;
		  i++;
	  
		  //p = pASCIItext[i];
	    }	  
	}
    else
	{
	  if ((i < NUMBEROFTEXTS) && 
		  (j < (MAXIOFCHARS - 1) && c >= ' '))
		pASCIItext[i][j++] = c;		// p[j++] = c;
	}
  }
  
  libereMemoire (pTemp);
  return 1;
}

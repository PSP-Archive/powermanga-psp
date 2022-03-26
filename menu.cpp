//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "menu.cpp"
// created      : ?
// updates      : 2005-01-08
// id		: $Id: menu.cpp,v 1.1.1.1 2005/01/25 05:53:36 gurumeditation Exp $
//-----------------------------------------------------------------------------
// functions  : void Gestion_Menu()
//            : void Affich_Menu(int pos_y, int num_option)
//            : void Init_Val_Player_One()
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
#define HAUT_SPR_MENU				41             //hauteur d'un sprite texte du menu
#define POS_AFF_MENU				512+128+32     //ordoneee affichage du menu
#define OPTION_PLAY					0              //numero option PLAY.
#define OPTION_ORDER				1              //numero option ORDER.
#define OPTION_ABOUT				2              //numero option ABOUT.
#define OPTION_QUIT					3              //numero option QUIT.
#define POS_AFF_MENU_NORMAL	136            //ordonnee du menu.
//..............................................................................
extern fonte            menu_spr[NBR_OPTION_MENU][NBR_IMAGES_MENU];
extern fonte            logotlk[NBR_IMAGES_LOGOTLK];
extern int              etat_menu;                             //etat actuel du menu (on, off, apparition, disparition)
extern short            touch[500];                            //tableau des flags des touches du clavier
extern int              quit_game;                             //1=demande a quitter le jeu
extern int              pos_y_menu;                            //ordonnee affichage du menu
extern bool             aff_game_over;                         //1=affichage du game over en cours
extern bool             bool_logo;                             //utilise pour l'animation du LOGOTLK.
extern int              posx_logo;                             //abscisse affichage du logo
extern int              posy_logo;                             //ordonnee affichage du logo.
extern bool             move_logo;                             //utilise pour indiquer si on doit ou non deplacer le logo.

//..............................................................................
static int              _cmpt_image_menu[NBR_OPTION_MENU] = { 0, 0, 0, 0 };
static short            touch_up = 0;
static short            touch_down = 0;
static int              num_option = 0;

//..............................................................................
extern int              liste_bou[MAX_BOULES_ON_SCREEN];       //liste des 'bou' a gerer
extern int              liste_can[MAX_CANONS_ON_SCREEN];       //liste des 'can' a gerer
extern vaisseau         vj[NBR_MAX_J];                         //structure vaisseaux joueurs
extern unsigned int     iCongratulation;                       //1=congratulation running 
extern int              bonusx;                                //multiplicateur score X2 ou X4
extern float            pos_x_level;                           //abscisse du texte donnant le niveau 
extern float            pos_y_level;                           //ordonnee du texte donnant le niveau
extern bool             option_change;                         //1=changement d'option a lieu
extern bool             bar_nrj_player;                        //1=reaffiche barre d'energie du joueur
extern bool             bar_nrj_guardian;                      //1=eraffiche la barre d'energie du gardien.
extern bool             apparition_gardian;                    //1=apparition gardien
extern int              gardian_activity;                      //1=phase "gardien"
extern int              nbr_ve_spe_generes;                    //nombre de vaisseaux speciaux deja utilise
extern bool             game_over_player_one;                  //1=le joueur vient de perdre son vaisseau
extern bool             aff_game_over;                         //1=affichage du game over
extern bool             meteor_activity;
extern int              num_let_game_over;                     //numero de la lettre a animer
extern curve_level      courbe;                                //structure de phase 'courbe'
extern int              nbr_meteor_generer;
extern int              num_level;                             //level number
extern grid             grille;                                //structure grille 
extern short            nbr_bonus;                             //nombre de bonus geres
extern int              liste_tirs[NBR_TIRS_MAX];              //liste de tous les 'tirs' a gerer
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //structure if enemy's ships.
extern int              liste_ve[MAX_NMI_ON_SCREEN];           //liste de tous les 've'

//------------------------------------------------------------------------------
// Routine utilisee pour la gestion du menu.
//------------------------------------------------------------------------------
int Gestion_Menu()
{
  //apparition disparition ........................................................
  if(etat_menu == APPARITION_MENU)                             //apparition du menu ?
  {
    if(pos_y_menu > POS_AFF_MENU_NORMAL)
      pos_y_menu--;                                            //oui, on deplace le menu vers le haut
  }
  if(etat_menu == DISPARITION_MENU)                            //disparition du menu ?
  {
    if(pos_y_menu < BANDE_DE_CLIP + HAUT_ECR_RAM)
      pos_y_menu++;                                            //oui, on deplace le menu vers le bas
  }
  if(pos_y_menu <= POS_AFF_MENU_NORMAL)                        //menu est dans sa position final ?
  {
    etat_menu = MENU_ON;                                       //ok, menu actif
  }
  if(pos_y_menu >= BANDE_DE_CLIP + HAUT_ECR_RAM)               //menu est sorti de l'ecran ?
  {
    etat_menu = MENU_OFF;                                      //ok, menu inactif
  }
  if(etat_menu == MENU_ON)                                     //menu actif ?
  {
    //navigation dans les options du menu ...........................................
    if(touch[55] && !touch_up)                                 //touche du haut pressee ?
    {
      num_option--;                                            //passe a l'option precedente.
      if(num_option < 0)
        num_option = NBR_OPTION_MENU - 1;
    }
    touch_up = touch[55];
    if(touch[87] && !touch_down)                               //fleche du bas pressee ?
    {
      num_option++;                                            //passe a l'option suivante.
      if(num_option >= NBR_OPTION_MENU)
        num_option = 0;
    }
    touch_down = touch[87];

    //deplacement du logo TLK ........................................................
    if(!move_logo && (rand() % 2500) == 500)                   //apparition du logo TLK ?
    {
      if(rand() % 2)
        posx_logo = 120;
      else
        posx_logo = 310;
      posy_logo = 328;
      move_logo = 1;
    }
    if(touch[50] || touch[90])                                 //barre d'espace ou entree pressee ?
    {
      switch (num_option)
      {                                                        //nouvelle partie  ..............................................................
        case OPTION_PLAY:                                      //commence une nouvelle partie ?
          Init_Val_Player_One();                               //raz les parametres du joueur
          etat_menu = DISPARITION_MENU;                        //demande la disparition du menu
          Destruct_Scroll_Font(SCROLL_PRESENT);                //faire disparaitre le texte qui defile
          _score::setAction(0);
#ifdef USE_SDLMIXER
          seal_JoueModule(1);                                  //joue musique du jeu
#endif
          break;

          //comment commander ? ...........................................................
        case OPTION_ORDER:
          _score::setAction(4);                                //prepare l'affichage du "order"
          etat_menu = DISPARITION_MENU;
          Destruct_Scroll_Font(SCROLL_PRESENT);                //fait disparaitre le texte qui defile
          break;

          //credits du jeu ..............................................................
        case OPTION_ABOUT:
          _score::setAction(3);                                //prepare l'affichage du "about"
          etat_menu = DISPARITION_MENU;
          Destruct_Scroll_Font(SCROLL_PRESENT);                //fait disparaitre le texte qui defile
          break;

          //quitte le jeu  ................................................................
        case OPTION_QUIT:
          quit_game = 1;                                       //1=demande a quitter le jeu
          break;
      }
    }
    if(touch[0])                                               //touche [esc] quitte le menu ?
    {
      if(aff_game_over == 1)
        _score::setAction(2);                                  //prepare l'affichage des scores
      etat_menu = DISPARITION_MENU;
      Destruct_Scroll_Font(SCROLL_PRESENT);                    //fait disparaitre le texte qui defile
    }
  }
  Affich_Menu(pos_y_menu, num_option);                         //affiche le texte du menu
  return 1;
}


//------------------------------------------------------------------------------
// routine utilisee pour l'affichage du menu.
// entre pos_y : ordonnee  (minimum=136, maximum 312)
//------------------------------------------------------------------------------
void Affich_Menu(int pos_y, int num_option)
{
  int                     minspeed = 0;
  int                     i;

  for(i = 0; i < NBR_OPTION_MENU; i++)
  {
    if(minspeed == 0)
    {
      if(i == num_option)
      {
        _cmpt_image_menu[i] = (_cmpt_image_menu[i] + 1) % NBR_IMAGES_MENU;
      }
      else if(_cmpt_image_menu[i] != 0)
        _cmpt_image_menu[i] = (_cmpt_image_menu[i] + 1) % NBR_IMAGES_MENU;
    }
    if((pos_y + i * HAUT_SPR_MENU) >= (BANDE_DE_CLIP - HAUT_SPR_MENU)
       && (pos_y + i * HAUT_SPR_MENU) < (BANDE_DE_CLIP + HAUT_ECR_RAM))
      putsprite3(&menu_spr[i][_cmpt_image_menu[i]], BANDE_DE_CLIP + 32, pos_y + (HAUT_SPR_MENU * i));   //affiche le texte 
  }
  minspeed = 1 - minspeed;
}

//------------------------------------------------------------------------------
// initialize values before beging the game
//------------------------------------------------------------------------------
void Init_Val_Player_One()
{
  int                     i;                                   //pos_cmpt_nrj;

  srand(1);                                                    //initialise random generator
  initializeFonte();                                           //"displayFonte.cpp"
  for(i = 0; i < 11; i++)
    if(i != 1)
      Init_Anim_Option(i, 0);                                  //open all options
  Fill_Struct_Player_One();                                    //initialize player's structure
  bonusx = 0;                                                  //no score muliplicateur 
  vj[J1].invincible = TEMPS_INVINCIBLE;                        //
  Init_Tab_Ve();
  for(i = 0; i < MAX_NMI_ON_SCREEN; i++)
    liste_ve[i] = 0;
  Init_Tab_Tirs();
  for(i = 0; i < NBR_TIRS_MAX; i++)
    liste_tirs[i] = 0;
  Init_Tab_Boules();
  for(i = 0; i < MAX_BOULES_ON_SCREEN; i++)
    liste_bou[i] = 0;
  Init_Tab_Canons();
  for(i = 0; i < MAX_CANONS_ON_SCREEN; i++)
    liste_can[i] = 0;
  Init_Tab_OndeChoc();                                         //init. shock wave table 
  meteor_activity = 1;
  nbr_meteor_generer = 1000;
  courbe.activity = 0;
  grille.activity = 0;
  gardian_activity = 0;
  apparition_gardian = 0;
  game_over_player_one = 0;
  aff_game_over = 0;
  num_level = -1;                                              //numero du niveau (-1= debut du jeu, sinon de 0 a 41) 
  num_let_game_over = 0;
  bar_nrj_player = 1;                                          //updates player's energy barline 
  bar_nrj_guardian = 1;                                        //updates big-boss's energy barline
  nbr_ve_spe_generes = 0;
  option_change = 1;                                           //updates option panel
  nbr_bonus = 0;
  Apparition_Vaisseau();
  pos_x_level = 128.0;
  pos_y_level = 0.0;
  iCongratulation = 0;                                         //0=no congratulation
}

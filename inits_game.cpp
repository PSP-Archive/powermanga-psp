//*****************************************************************************
// copyright (c) 1998-2003 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "init_game.cpp"
// created      : ?
// updates      : 2003-07-20
//-----------------------------------------------------------------------------
// functions    :  int   inits_game()
//                 int   etb_read_image(char* _pNomFichier, image* _pImage, unsigned int _iNombreType, unsigned _iNombreImage) ;
//                 char* etb_read_image(image* _pImage, char* _pFichier)  ;
//                 int   etb_read_image(char* _pNomFichier, image* _pImage) ;
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
extern char            *ecran_opt;                             //adresse barre options
extern char            *ecran_scr;                             //adresse bande scores
extern image            gardi[NBR_ANIM_GARDIAN][NBR_IMAGES_NMI_S];      //images des differents gardiens
extern fntscroll        fnt_scr[NBR_FNT_SCROLL_MAX];           //tableau de tous les objets caracteres qui defilent
extern int              liste_fnt_scroll[NBR_FNT_SCROLL_MAX];  //liste des fontes qui defilent actuellement
extern short            nbr_fnt_scroll;                        //nombre de fontes qui scrollent actuellement g��.
extern option           aff_option[MAX_TYP_OF_OPTIONS];                        // Tableau utilis�pour l'affichage des options pendant le jeu.
extern int              num_let_game_over;                     // Utilis�pour savoir quelle lettre du game over il faut faire tourner.
extern int              cmpt_vbl_let_game_over;                // Utilis�pour compter le temps qu'il faut avant de faire tourner une lettre du game over.
extern int              nbr_vbl_game_over;                     // Utilis�pour compter le temps qu'il faut avant de faire tourner une lettre du game over.
extern play_score       aff_player_pause[NBR_CHIF_PAUSE];      // Tableau utilis�pour l'affichage du mot pause pendant le jeu.
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 // D�laration de la structure des vaisseaux ennemis.
extern explosion        xpl[MAX_NMI_ON_SCREEN];                // D�laration de la structure utilis� par les explosions.
extern float            depix[13][32];                         //deplacement en x des missiles
extern float            depiy[13][32];                         //deplacement en y des missiles
extern float            pre_sin[];                             //table sinus precalcules
extern float            pre_cos[];                             //table cosinus precalcules

#ifdef DEVELOPPEMENT
extern short            mouse_x, mouse_y, mouse_here;
#endif
extern short            mouse_b;                               //1=bouton gauche souris presse
extern image            bar_nrj_red;                           // Utilis�pour l'affichage de la barre d'�ergie.
extern image            bar_nrj_green;                         // Utilis�pour l'affichage de la barre d'�ergie.
extern image            bar_nrj_blue;                          // Utilis�pour l'affichage de la barre d'�ergie.
extern char             str_file[50];                          // D�laration d'une chaine pour le chargement de fichiers.
extern char             str_tmp[40];                           // Chaine temporaire utilis� pour le chargement de fichiers.
extern image            vj1[NBR_MAX_VAIS_J][NBR_MAX_IMG_VAIS_J];        // D�laration de la structure des images des vaisseaux du joueur 1.
extern vaisseau         vj[NBR_MAX_J];                         //D�laration de la structure des vaisseaux des joueurs.
extern char            *chaine;                                //Utilis�pour la conversion des chaines de caract�es.
extern int              taille_mem_glob;                       //Donne la taille memoire maximum utilisee.
extern unsigned char    coulor[12];                            //Tableau contenant les couleurs choisies par l'utilisateur.
extern unsigned char   *pal;                                   //palette de couleurs 24 bits (3 octets par couleur)
extern unsigned char    color_eclair[2 + 1];                   //Tableau contenant les couleurs utilis�s lors de la disparition d'un vaisseau.
extern unsigned char    color_dead[NBR_COLOR_DEAD + 1];        //Tableau contenant les couleurs utilis�s lors de la disparition d'un vaisseau.
extern unsigned char    color_onde_choc[NBR_COLOR_ONDE_CHOC + 1];       //Tableau contenant les couleurs utilis�s pour l'affichage d'une onde de choc.
extern int              etat_menu;                             //MENU_OFF(false)=menu inactif ; MENU_ON(true)=menu actif.
extern image            meteo[MAX_TYP_OF_METEOR][NBR_IMAGES_METEOR];
extern int              iErreurInitialisation;                 //1=erreur a l'initialisation
extern image            enemi[MAXTYPENMI][NBR_IMAGES_MAX];     //tableau des images des vaisseaux ennemis
extern image            fire[MAX_TYP_OF_TIR][NBR_IMAGES_TIR];  //tableau des images des tirs
extern image            explo[MAX_TYP_OF_EXPLOS][NBR_IMAGES_EXP];       //tableau des images des explosions
extern image            eclat[MAX_TYP_OF_ECLATS][NBR_IMAGES_ECLATS];    //tableau des images les eclats
extern image            bonus[MAX_TYP_OF_BONUS][NBR_IMAGES_BONUS];      //tableau des images des bonus
extern image            boule[MAX_TYP_OF_BOULES][NBR_IMAGES_BOULES];    //tableau des images des boules
extern image            canon[MAX_TYP_OF_CANONS][NBR_IMAGES_CANONS];    //tableau des images des canons
extern image            star_field[TYPE_OF_STARS][NBR_ETOILES]; //tableau des images des etoiles
extern fonte            options[MAX_TYP_OF_OPTIONS][NBR_IMAGES_OPTIONS];        //tableau des images des options
extern fonte            fnt_scroll[NBR_FONTES_SCROLL];         //images des fontes du defilement
extern fonte            logotlk[NBR_IMAGES_LOGOTLK];           //images du logo TLK
extern fonte            menu_spr[NBR_OPTION_MENU][NBR_IMAGES_MENU];     //images des textes du menu
extern fonte            fnt_score[NBR_FONTES_SCORE][NBR_IMAGES_FONTE];
extern fonte            fnt_game[NBR_FONTES_GAME][NBR_IMAGES_FONTE];
extern fonte            fnt_big[NBR_FONTES_BIG][NBR_IMAGES_FONTE];
extern fonte            doublex[5];                            //tableau des differentes images des multiplicateurs
extern _etbfonte1      *pEtbfonte1;
extern star             stars[NBR_OF_STARS];                   //etoiles du champ d'etoiles
sDescriptionGfx         sGfxTempo;
extern unsigned int     iOctetsParPixel;                       //nombre d'octets par pixels (1=256 couleurs / 2=65536 couleurs)
extern unsigned short  *pal16;                                 //palette 16 bits
extern unsigned int    *pal32;                                 //palette 16 bits
extern int              vmode;                                 //0=resolution de 320x200 / 1=640*400 / 2=640*400 (double pixels)

//fonctions utlisees en local ..................................................
void                    initialise_etoiles();
int                     charge_courbes();
int                     charge_etoiles();
int                     charge_ennemis();
int                     etb_read_fonte(char *_pNomFichier, fonte * _pFonte, unsigned int _iNombreType, unsigned _iNombreImage, unsigned int _iLargeurSprite);
char                   *etb_read_image(image * _pImage, unsigned int _iNombreType, unsigned int _iNombreImage, char *_pMem, unsigned int _iMaxImage);
char                   *etb_read_fonte(fonte * _pFonte, unsigned int _iNombreType, unsigned _iNombreImage, char *_pMem, unsigned int _iMaxImage, unsigned int _iLargeurSprite);
int                     etb_read_image(char *_pNomFichier, image * _pImage);
char                   *etb_read_image(image * _pImage, char *_pFichiere);      //decodage
char                   *etb_read_fonte(fonte * _pFonte, char *_pFichier, unsigned int _iLargeurSprite); //decodage
char                   *lecture_pixels(unsigned int _iNbr_pts_img, char *_pFichier, char *_pPixels);
char                   *lecture_compress(unsigned int _iNbr_data_comp, char *_pFichier, char *_pCompress, unsigned int _iLargeurSprite);
int                     load_options();

//------------------------------------------------------------------------------
// initialisation du jeu (se fait une seule et une seule fois)
// output <= 0=erreur
//           1=tout c'est bien passee
//------------------------------------------------------------------------------
int inits_game()
{
  int i, j;
  iErreurInitialisation = 0;
  chaine = reserveMemoire(2000);                               //zone pour les chaines de caracteres
  if(!chaine)
  { afficheErreur("'chaine' out of memory", "inits_game.cpp/inits_game()");
    return 0;
  }

  //chargement des fichiers ----------------------------------------------------
  if(!charge_palette("graphics/image.pal")) return 0;
  if(!charge_ennemis()) return 0;
  if(!etb_read_image("graphics/tirs.spr",     &fire[0][0], MAX_TYP_OF_TIR, NBR_IMAGES_TIR))  return 0;              //996131 octets
  if(!load_options()) return 0;
  if(!etb_read_fonte("graphics/menu.spr",     &menu_spr[0][0], NBR_OPTION_MENU, NBR_IMAGES_MENU, 32)) return 0;     //452598 octets
  if(!etb_read_fonte("graphics/fontes.spr",   &fnt_game[0][0], NBR_FONTES_GAME, NBR_IMAGES_FONTE, 16)) return 0;    //159654 octets
  if(!etb_read_fonte("graphics/big.spr",      &fnt_big[0][0], NBR_FONTES_BIG, NBR_IMAGES_FONTE, 32)) return 0;      //149950 octets
  if(!etb_read_fonte("graphics/logotlk.spr",  &logotlk[0], 1, NBR_IMAGES_LOGOTLK, 48))return 0;                     //92949 octets
  if(!etb_read_image("graphics/bonus.spr",    &bonus[0][0], MAX_TYP_OF_BONUS, NBR_IMAGES_BONUS)) return 0;          //80868 octets
  if(!etb_read_image("graphics/explos.spr",   &explo[0][0], MAX_TYP_OF_EXPLOS, NBR_IMAGES_EXP))return 0;            //61750 octets
  if(!etb_read_fonte("graphics/scores.spr",   &fnt_score[0][0], NBR_FONTES_SCORE, NBR_IMAGES_FONTE, 16)) return 0;  //52480 octets
  if(!etb_read_image("graphics/eclats.spr",   &eclat[0][0], MAX_TYP_OF_ECLATS, NBR_IMAGES_ECLATS)) return 0;        //34089 octets
  if(!etb_read_image("graphics/canons.spr",   &canon[0][0], MAX_TYP_OF_CANONS, NBR_IMAGES_CANONS)) return 0;        //28450 octets
  if(!etb_read_image("graphics/boules.spr",   &boule[0][0], MAX_TYP_OF_BOULES, NBR_IMAGES_BOULES)) return 0;        //26191 octets
  if(!etb_read_fonte("graphics/fntscrol.spr", &fnt_scroll[0], 1, NBR_FONTES_SCROLL, 16)) return 0;                  //12143 octets
  if(!charge_etoiles()) return 0;
  initialise_etoiles();                                                                                             //initialise le champ d'etoile / initialize starfield

  //initialisation des adresse memoires utilisees par les gardiens -------------
  for(i = 0; i < NBR_ANIM_GARDIAN; i++)
  { for(j = 0; j < NBR_IMAGES_NMI_S; j++)
    { gardi[i][j].img = 0;
      gardi[i][j].compress = 0;
    }
  }
  //initialisation de la memoire utilisee par les images des meteorites --------
  for(i = 0; i < (MAX_TYP_OF_METEOR); i++)
  { for(j = 0; j < NBR_IMAGES_METEOR; j++)
    { meteo[i][j].img = 0;
      meteo[i][j].compress = 0;
    }
  }

  //initialisation du tableau 'fnt_scr' ----------------------------------------
  for(i = 0; i < NBR_FNT_SCROLL_MAX; i++)
  { fnt_scr[i].next_object = NBR_FNT_SCROLL_MAX; //objet actuel est libre
    fnt_scr[i].prev_object = NBR_FNT_SCROLL_MAX; //objet actuel est libre
    liste_fnt_scroll[i] = 0;
  }
  nbr_fnt_scroll = 0;

  //initialisation du tableau 'tirs' ainsi que du nombre de tirs ---------------
  Init_Tab_Tirs();  // "powermanga sub2.cpp"

  //initialisation des valeurs utilisees pour l'affichage des options ----------
  for(i = 0; i < MAX_TYP_OF_OPTIONS; i++)
  { aff_option[i].nbr_tot_vbl = 1000;
    aff_option[i].add_vbl = 550;
    aff_option[i].img_act = 0;
    aff_option[i].nbr_vbl_act = 0;
  }

  //initialisation des valeurs utilis�s pour l'affichage de la pause ------------
  num_let_game_over = 0;                                       //numero de la lettre de "pause" qu'il faut faire tourner
  cmpt_vbl_let_game_over = 0;                                  //temporisation avant de faire tourner une lettre de "pause"
  nbr_vbl_game_over = 50;                                      //temporisation avant de faire tourner une lettre de "pause"
  for(i = 0; i < NBR_CHIF_PAUSE; i++)
  {
    aff_player_pause[i].anim_font = 0;
    aff_player_pause[i].nbr_tot_vbl = 1000;
    aff_player_pause[i].add_vbl = 250;
  }

  //initialisation du tableau 've' (vaisseaux ennemis) & 'xpl' (explosions) ------
  for(i = 0; i < MAX_NMI_ON_SCREEN; i++)
  {
    ve[i].next_object = MAX_NMI_ON_SCREEN;                     //objet actuel est libre.
    ve[i].prev_object = MAX_NMI_ON_SCREEN;                     //objet actuel est libre.
    xpl[i].next_object = MAX_NMI_ON_SCREEN;                    //objet actuel est libre.
    xpl[i].prev_object = MAX_NMI_ON_SCREEN;                    //objet actuel est libre.
  }

  //initialisation tableaux  des ondes de choc, des boules, et des canons --------
  Init_Tab_OndeChoc();                                         //'onde_choc'
  Init_Tab_Boules();                                           //'bou'.
  Init_Tab_Canons();                                           //'can'.

  //remplissage du tableau des deplacements utilises pour le missile teleguide ---
  for(i = 0; i < 32; i++)
  {
    depix[0][i] = pre_cos[i] * 0.5f;
    depix[1][i] = pre_cos[i] * 1.0f;
    depix[2][i] = pre_cos[i] * 1.5f;
    depix[3][i] = pre_cos[i] * 2.0f;
    depix[4][i] = pre_cos[i] * 2.5f;
    depix[5][i] = pre_cos[i] * 3.0f;
    depix[6][i] = pre_cos[i] * 3.5f;
    depix[7][i] = pre_cos[i] * 4.0f;
    depix[8][i] = pre_cos[i] * 4.5f;
    depix[9][i] = pre_cos[i] * 5.0f;
    depix[10][i] = pre_cos[i] * 5.5f;
    depix[11][i] = pre_cos[i] * 6.0f;
    depix[12][i] = pre_cos[i] * 6.5f;
    depiy[0][i] = pre_sin[i] * 0.5f;
    depiy[1][i] = pre_sin[i] * 1.0f;
    depiy[2][i] = pre_sin[i] * 1.5f;
    depiy[3][i] = pre_sin[i] * 2.0f;
    depiy[4][i] = pre_sin[i] * 2.5f;
    depiy[5][i] = pre_sin[i] * 3.0f;
    depiy[6][i] = pre_sin[i] * 3.5f;
    depiy[7][i] = pre_sin[i] * 4.0f;
    depiy[8][i] = pre_sin[i] * 4.5f;
    depiy[9][i] = pre_sin[i] * 5.0f;
    depiy[10][i] = pre_sin[i] * 5.5f;
    depiy[11][i] = pre_sin[i] * 6.0f;
    depiy[12][i] = pre_sin[i] * 6.5f;
  }

  //initialisation de la souris --------------------------------------------------
#ifdef DEVELOPPEMENT
  mouse_here = TRUE;
#endif

  //chargement des sprites utilises pour la mise a jour de la barre d'energie du joueur.
  if(!etb_read_image("graphics/nrjred.spr", &bar_nrj_red))
    return 0;
  if(!etb_read_image("graphics/nrjgreen.spr", &bar_nrj_green))
    return 0;
  if(!etb_read_image("graphics/nrjblue.spr", &bar_nrj_blue))
    return 0;

  //chargement vaisseaux du joueur / load all player's ships ---------------------
  if(!etb_read_image("graphics/v1j1i_0.spr", &vj1[0][0])) return 0;
  if(!etb_read_image("graphics/v1j1i_1.spr", &vj1[0][1])) return 0;
  if(!etb_read_image("graphics/v1j1i_2.spr", &vj1[0][2])) return 0;
  if(!etb_read_image("graphics/v1j1i_3.spr", &vj1[0][3])) return 0;
  if(!etb_read_image("graphics/v1j1i_4.spr", &vj1[0][4])) return 0;
  if(!etb_read_image("graphics/v2j1i_0.spr", &vj1[1][0])) return 0;
  if(!etb_read_image("graphics/v2j1i_1.spr", &vj1[1][1])) return 0;
  if(!etb_read_image("graphics/v2j1i_2.spr", &vj1[1][2])) return 0;
  if(!etb_read_image("graphics/v2j1i_3.spr", &vj1[1][3])) return 0;
  if(!etb_read_image("graphics/v2j1i_4.spr", &vj1[1][4])) return 0;
  if(!etb_read_image("graphics/v3j1i_0.spr", &vj1[2][0])) return 0;
  if(!etb_read_image("graphics/v3j1i_1.spr", &vj1[2][1])) return 0;
  if(!etb_read_image("graphics/v3j1i_2.spr", &vj1[2][2])) return 0;
  if(!etb_read_image("graphics/v3j1i_3.spr", &vj1[2][3])) return 0;
  if(!etb_read_image("graphics/v3j1i_4.spr", &vj1[2][4])) return 0;
  if(!etb_read_image("graphics/v4j1i_0.spr", &vj1[3][0])) return 0;
  if(!etb_read_image("graphics/v4j1i_1.spr", &vj1[3][1])) return 0;
  if(!etb_read_image("graphics/v4j1i_2.spr", &vj1[3][2])) return 0;
  if(!etb_read_image("graphics/v4j1i_3.spr", &vj1[3][3])) return 0;
  if(!etb_read_image("graphics/v4j1i_4.spr", &vj1[3][4])) return 0;
  if(!etb_read_image("graphics/v5j1i_0.spr", &vj1[4][0])) return 0;
  if(!etb_read_image("graphics/v5j1i_1.spr", &vj1[4][1])) return 0;
  if(!etb_read_image("graphics/v5j1i_2.spr", &vj1[4][2])) return 0;
  if(!etb_read_image("graphics/v5j1i_3.spr", &vj1[4][3])) return 0;
  if(!etb_read_image("graphics/v5j1i_4.spr", &vj1[4][4]))  return 0;

  Fill_Struct_Player_One();                                    //initialise la structure joueur / initialize structure's player
  if(!charge_courbes())
    return 0;                                                  //charge les courbes / load all curves
  for(i = 0; i < 11; i++)
    Init_Anim_Option(i, 1);                                    //ferme les options / close all options
  vj[J1].nbr_bonus = 0;

  //recherche de couleurs dans la palette / search colors
  coulor[0] = recherche_couleur(0, 0, 0);                      //noir / black
  coulor[1] = recherche_couleur(85, 85, 85);                   //gris fonce
  coulor[2] = recherche_couleur(128, 128, 128);                //gris
  coulor[3] = recherche_couleur(212, 212, 212);                //gris clair.
  coulor[4] = recherche_couleur(255, 255, 255);                //blanche / white
  coulor[5] = recherche_couleur(255, 0, 0);                    //rouge / red
  coulor[6] = recherche_couleur(0, 255, 0);                    //verte / green
  coulor[7] = recherche_couleur(255, 255, 0);                  //jaune / yellow
  coulor[8] = recherche_couleur(55, 55, 255);                  //bleu / blue
  coulor[9] = recherche_couleur(180, 150, 0);                  //rouge fonce / drak red
  //couleurs utilisees pour l'eclair
  color_eclair[0] = recherche_couleur(50, 150, 250);           //centre de l'eclair
  color_eclair[1] = recherche_couleur(50, 0, 180);             //bords l'eclair
  //couleurs disparition d'un vaisseau / colors if a ship's dead
  color_dead[0] = recherche_couleur(250, 250, 0);
  color_dead[1] = recherche_couleur(250, 200, 0);
  color_dead[2] = recherche_couleur(200, 200, 0);
  color_dead[3] = recherche_couleur(200, 150, 0);
  color_dead[4] = recherche_couleur(200, 150, 0);
  color_dead[5] = recherche_couleur(150, 150, 0);
  color_dead[6] = recherche_couleur(150, 100, 0);
  color_dead[7] = recherche_couleur(100, 100, 0);
  color_dead[8] = recherche_couleur(100, 50, 0);
  color_dead[9] = recherche_couleur(50, 50, 0);
  color_dead[10] = recherche_couleur(50, 0, 0);
  //couleurs utilisees pour une onde de choc
  color_onde_choc[0] = recherche_couleur(255, 255, 0);
  color_onde_choc[1] = recherche_couleur(248, 248, 0);
  color_onde_choc[2] = recherche_couleur(241, 241, 0);
  color_onde_choc[3] = recherche_couleur(234, 234, 0);
  color_onde_choc[4] = recherche_couleur(227, 227, 0);
  color_onde_choc[5] = recherche_couleur(220, 220, 0);
  color_onde_choc[6] = recherche_couleur(213, 213, 0);
  color_onde_choc[7] = recherche_couleur(206, 206, 0);
  color_onde_choc[8] = recherche_couleur(199, 199, 0);
  color_onde_choc[9] = recherche_couleur(192, 192, 0);
  color_onde_choc[10] = recherche_couleur(185, 185, 0);
  color_onde_choc[11] = recherche_couleur(178, 178, 0);
  color_onde_choc[12] = recherche_couleur(171, 171, 0);
  color_onde_choc[13] = recherche_couleur(164, 164, 0);
  color_onde_choc[14] = recherche_couleur(157, 157, 0);
  color_onde_choc[15] = recherche_couleur(150, 150, 0);
  color_onde_choc[16] = recherche_couleur(143, 143, 0);
  color_onde_choc[17] = recherche_couleur(136, 136, 0);
  color_onde_choc[18] = recherche_couleur(129, 129, 0);
  color_onde_choc[19] = recherche_couleur(122, 122, 0);
  color_onde_choc[20] = recherche_couleur(115, 115, 0);
  color_onde_choc[21] = recherche_couleur(108, 108, 0);
  color_onde_choc[22] = recherche_couleur(101, 101, 0);
  color_onde_choc[23] = recherche_couleur(94, 94, 0);
  color_onde_choc[24] = recherche_couleur(87, 87, 0);

  etat_menu = APPARITION_MENU;                                 //menu apparition / main menu enable
  Init_Scroll_Present();                                       //initialise le defilement de texte / initialize the scrolltext
  if(!pEtbfonte1->initialise())
  {
    afficheErreur("pEtbfonte1->initialise() failed", "inits_game.cpp/inits_game()");
    return 0;
  }
  if(!reserve_ecran_ram())
    return 0;                                                  //reserve "XImage" / initialize "XImage" ("xwindow.cpp")
  if(!(Xload_pcx("graphics/tab_bord1.pcx", ecran_opt)))
    return 0;                                                  //charge la bande des options a droite / load right option panel ("xwindow.cpp")
  if(!(Xload_pcx("graphics/tab_bord2.pcx", ecran_scr)))
    return 0;                                                  //charge la bande des scores / load up score panel ("xwindow.cpp")
  return 1;
}

//------------------------------------------------------------------------------
// cherche une couleur dans la palette / search a color in the palette
// input => r : rouge / red
//       => g : vert  / green
//       => b :  bleu / blue
//------------------------------------------------------------------------------
unsigned char recherche_couleur(unsigned char r, unsigned char g,
                                unsigned char b)
{
  unsigned char           cr, cg, cb, indice_couleur;
  unsigned int            i;
  double                  norme[256], min_norme;
  for(i = 0; i < 767; i += 3)
  {
    cr = pal[i];
    cg = pal[i + 1];
    cb = pal[i + 2];
    norme[i / 3] = sqrt(pow(r - cr, 2) + pow(g - cg, 2) + pow(b - cb, 2));
  }
  min_norme = norme[0];
  indice_couleur = 0;
  for(i = 1; i < 256; i++)
  {
    if(norme[i] < min_norme)
    {
      min_norme = norme[i];
      indice_couleur = (unsigned char)i;
    }
  }
  return indice_couleur;
}

//------------------------------------------------------------------------------
// charge toutes les etoiles / load all stars (4 types d'etoiles de 8 images chacun)
// - "BISTAR" : champ d'etoile / starfield
// - "STAR"   : champ d'etoile / starfield
// - "LISTAR" : chanp d'etoile / starfield
// - "ETOILE" : etoile sur le vaisseau du joueur (invincibilite) / ship's stars
//------------------------------------------------------------------------------
static char             pNomFichierStar1[] = "graphics/bigstar?.spr";
static char             pNomFichierStar2[] = "graphics/star?.spr";
static char             pNomFichierStar3[] = "graphics/litstar?.spr";
static char             pNomFichierStar4[] = "graphics/etoile_?.spr";
int charge_etoiles()
{
  unsigned int            i;

  //grosses etoiles / big stars
  for(i = 0; i < NBR_ETOILES; i++)
  {
    entier2ASCII(i, pNomFichierStar1 + 16, 0);
    if(!etb_read_image(pNomFichierStar1, &star_field[BIGSTAR][i], 1, 1))
      return 0;
  }
  //moyennes etoiles / normal stars
  for(i = 0; i < NBR_ETOILES; i++)
  {
    entier2ASCII(i, pNomFichierStar2 + 13, 0);
    if(!etb_read_image(pNomFichierStar2, &star_field[STAR][i], 1, 1))
      return 0;
  }
  //petites etoiles / little stars
  for(i = 0; i < NBR_ETOILES; i++)
  {
    entier2ASCII(i, pNomFichierStar3 + 16, 0);
    if(!etb_read_image(pNomFichierStar3, &star_field[LITSTAR][i], 1, 1))
      return 0;
  }
  // etoiles du vaisseau / ship's stars
  for(i = 0; i < NBR_ETOILES; i++)
  {
    entier2ASCII(i, pNomFichierStar4 + 16, 0);
    if(!etb_read_image(pNomFichierStar4, &star_field[PLAYERSTAR][i], 1, 1))
      return 0;
  }
  return 1;
}

//------------------------------------------------------------------------------
// initialise le champ d'etoile / initialize the starfield
//------------------------------------------------------------------------------
void initialise_etoiles()
{
  unsigned int            j, i;

  //remplissage de la structure 'stars' des grosses etoiles
  j = 0;
  for(i = 0; i < NBR_STARS_BY_TYPE; i++)
  {
    stars[i + 48].coor_x = (float)(rand() % LARG_ECR_RAM + BANDE_DE_CLIP);
    stars[i + 48].coor_y = (float)(rand() % 10 + i * 15);
    stars[i + 48].vitesse = (float)((float)(rand() % 8 / (float)100.0 + 0.8));
    stars[i + 48].img = (image *) & star_field[BIGSTAR][j];
    stars[i + 48].type = BIGSTAR;
    stars[i + 48].nbr_tot_vbl = 16;
    stars[i + 48].nbr_vbl_act = (rand() * 8) / (RAND_MAX + 1);
    j++;
    if(j == NBR_ETOILES) j = 0;
  }
  //remplissage de la structure 'stars' des etoiles normales
  j = 0;
  for(i = 0; i < NBR_STARS_BY_TYPE; i++)
  {
    stars[i + 24].coor_x = (float)(rand() % LARG_ECR_RAM + BANDE_DE_CLIP);
    stars[i + 24].coor_y = (float)(rand() % 10 + i * 15);
    stars[i + 24].vitesse = (float)((float)(rand() % 4 / (float)100.0 + 0.4));
    stars[i + 24].img = (image *) & star_field[STAR][j];
    stars[i + 24].type = STAR;
    stars[i + 24].nbr_tot_vbl = 16;
    stars[i + 24].nbr_vbl_act = (rand() * 8) / (RAND_MAX + 1);
    j++;
    if(j == NBR_ETOILES) j = 0;
  }
  //remplissage de la structure 'stars' des petites etoiles
  j = 0;
  for(i = 0; i < NBR_STARS_BY_TYPE; i++)
  {
    stars[i].coor_x = (float)(rand() % LARG_ECR_RAM + BANDE_DE_CLIP);
    stars[i].coor_y = (float)(rand() % 10 + i * 15);
    stars[i].vitesse = (float)((float)(rand() % 2 / (float)100.0 + 0.2));
    stars[i].img = (image *) & star_field[LITSTAR][j];
    stars[i].type = LITSTAR;
    stars[i].nbr_tot_vbl = 8;
    stars[i].nbr_vbl_act = (rand() * 8) / (RAND_MAX + 1);
    j++;
    if(j == NBR_ETOILES) j = 0;
  }
}

//------------------------------------------------------------------------------
// charge tous les vaisseaux ennemis / load all aliens
// - 21 petits ennemis pour la phase des grilles        : 32 animations pour la rotation, et 8 pour la position fixe
// - 21 supers-ennemis pour la phase des grilles        : 8 animations
// - 40 ennemis speciaux apparaissant comme malus : 32 animations
// - 8 objets speciaux utlises pour les gardiens        : 32 animations
// sortie : 1 = pas d'erreur
//------------------------------------------------------------------------------
int charge_ennemis()
{
  char                   *_pMem;
  image                  *_pImg;
  char                   *_pFichier = chargeFichier("graphics/allnmis.spr");

  if(!_pFichier)
    return 0;
  _pMem = _pFichier;
  //charge les 21 petits ennemis de 40 images chacun (phase grille)
  _pImg = &enemi[0][0];
  _pMem =
    etb_read_image(_pImg, MAX_TYP_OF_ENEMI, NBR_IMAGES_MAX, _pMem,
                   NBR_IMAGES_MAX);
  if(!_pMem)
    return 0;
  //charge les 21 supers ennemis de 8 images chacun (phase grille)
  _pImg = _pImg + (MAX_TYP_OF_ENEMI * NBR_IMAGES_MAX);
  _pMem =
    etb_read_image(_pImg, MAX_TYP_OF_S_ENEMI, NBR_IMAGES_S_NMI, _pMem,
                   NBR_IMAGES_MAX);
  if(!_pMem)
    return 0;
  //charge les 40 ennemis speciaux + les 8 objets speciaux de 32 images chacun
  _pImg = _pImg + (MAX_TYP_OF_S_ENEMI * NBR_IMAGES_MAX);
  _pMem =
    etb_read_image(_pImg, MAX_TYP_OF_NMI_SPE + MAX_TYP_OF_OBJ_SPE,
                   NBR_IMAGES_NMI_S, _pMem, NBR_IMAGES_MAX);
  if(!_pMem)
    return 0;
  libereMemoire(_pFichier);
  return 1;
}

//------------------------------------------------------------------------------
// charge un fichier 'sprite' et le decode dans une structure 'image'
// input  => _pNomFichier  : nom du fichier
//        => _pImage       : pointeur sur la premiere structure image
//        => _iNombreType  : nombre de sprites differents
//        => _iNombreImage : nombre d'animations pour un meme sprite
//        => _pMem         : pointeur sur le fichier en memoire
// output <= 1             : pas d'erreur
//------------------------------------------------------------------------------
int etb_read_image(char *_pNomFichier, image * _pImage, unsigned int _iNombreType, unsigned int _iNombreImage)
{
  char                   *_pMem;
  char                   *_pFichier = chargeFichier(_pNomFichier);
  if(!_pFichier) return 0;
  _pMem = _pFichier;
  _pMem = etb_read_image(_pImage, _iNombreType, _iNombreImage, _pMem, _iNombreImage);
  if(!_pMem)
  {
    libereMemoire(_pFichier);
    return 0;
  }
  libereMemoire(_pFichier);
  return 1;
}

//------------------------------------------------------------------------------
// charge un fichier 'sprite' et le decode dans une structure 'fonte'
// input  => _pNomFichier  : nom du fichier
//        => _pFonte       : pointeur sur la premiere structure fonte
//        => _iNombreType  : nombre de sprites differents
//        => _iNombreImage : nombre d'animations pour un meme sprite
//        => _pMem         : pointeur sur le fichier en memoire
// output <= 1             : pas d'erreur
//------------------------------------------------------------------------------
int etb_read_fonte(char *_pNomFichier, fonte * _pFonte, unsigned int _iNombreType, unsigned _iNombreImage, unsigned int _iLargeurSprite)
{
  char                   *_pMem;
  char                   *_pFichier = chargeFichier(_pNomFichier);
  if(!_pFichier) return 0;
  _pMem = _pFichier;
  _pMem = etb_read_fonte(_pFonte, _iNombreType, _iNombreImage, _pMem, _iNombreImage, _iLargeurSprite);
  if(!_pMem)
  {
    libereMemoire(_pFichier);
    afficheErreur("'chaine' out of memory", "inits_game.cpp/etb_read_fonte()");
    return 0;
  }
  libereMemoire(_pFichier);
  return 1;
}

//------------------------------------------------------------------------------
// decode un fichier sprite dans une structure 'image'
// input  => _pImage       : pointeur sur la premiere structure sprite
//        => _iNombreType  : nombre de sprites differents
//        => _iNombreImage : nombre d'animations pour un meme sprite
//        => _pMem         : pointeur sur le fichier en memoire
//        => _iMaxImage    : nombre d'images maximum par type de sprite
// output <=_pMem          : nouveau pointeur sur le fichier en memoire
//------------------------------------------------------------------------------
char *etb_read_image(image * _pImage, unsigned int _iNombreType, unsigned int _iNombreImage, char *_pMem, unsigned int _iMaxImage)
{
  unsigned int            _iCompteurType;
  unsigned int            _iCompteurImage;
  image                  *_pImg;
  for(_iCompteurType = 0; _iCompteurType < _iNombreType; _iCompteurType++)
  {
    for(_iCompteurImage = 0; _iCompteurImage < _iNombreImage;
        _iCompteurImage++)
    {
      _pImg = _pImage + (_iCompteurType * _iMaxImage) + _iCompteurImage;
      _pMem = etb_read_image(_pImg, _pMem);
      if(!_pMem)
      {
        afficheErreur("etb_read_image(image*, char*) failed", "inits_game.cpp/etb_read_image(image*, int, int, char*, int)");
        return 0;
      }
    }
  }
  return _pMem;
}

//------------------------------------------------------------------------------
// decode un fichier sprite dans un structure 'fonte'
// input  => _pFonte       : pointeur sur une structure 'fonte'
//        => _iNombreType  : nombre de sprites differents
//        => _iNombreImage : nombre d'animations pour un meme sprite
//        => _pMem         : pointeur sur le fichier en memoire
//        => _iMaxImage    : nombre d'images maximum par type de sprite
// output <= _pMem         : nouveau pointeur sur le fichier en memoire
//------------------------------------------------------------------------------
char *etb_read_fonte(fonte * _pFonte, unsigned int _iNombreType, unsigned _iNombreImage, char *_pMem, unsigned int _iMaxImage, unsigned int _iLargeurSprite)
{
  unsigned int            _iCompteurType;
  unsigned int            _iCompteurImage;
  fonte                  *_pFnt;
  for(_iCompteurType = 0; _iCompteurType < _iNombreType; _iCompteurType++)
  {
    for(_iCompteurImage = 0; _iCompteurImage < _iNombreImage;
        _iCompteurImage++)
    {
      _pFnt = _pFonte + (_iCompteurType * _iMaxImage) + _iCompteurImage;
      _pMem = etb_read_fonte(_pFnt, _pMem, _iLargeurSprite);
      if(!_pMem) return 0;
    }
  }
  return _pMem;
}

//------------------------------------------------------------------------------
// input  => _pNomFichier : nom du fichier/filename (.spr)
//        => _pImage      : pointeur sur une structure 'image'
// output <= 1            : pas d'erreur
//------------------------------------------------------------------------------
int etb_read_image(char *_pNomFichier, image * _pImage)
{
  char                   *_pFichier = chargeFichier(_pNomFichier);
  if(!_pFichier) return 0;
  char                   *_pMem = etb_read_image(_pImage, _pFichier);
  if(!_pMem)
  {
    afficheErreur("etb_read_image(char*, char*) failed", "inits_game.cpp/etb_read_image(char*, image*)");
    libereMemoire(_pFichier);
    return 0;
  }
  libereMemoire(_pFichier);
  return 1;
}

//------------------------------------------------------------------------------
// decodage d'un fichier sprite (read sprite file)
// input => _pFichier  : adresse  du fichier en memoire / filename in memory (.spr)
//          => _pImage : pointeur sur une structure 'image'
// notes :
// les valeurs du fichier sont stockees en little-endian
// contenu : abscisse, ordonnee, largeur, hauteur du sprite
//         : table des point(s) de collision et de zone(s) de collison
//         : table des position(s) des sortie(s) des tir(s), et table des angle(s) des tir(s)
//         : table des pixels en 8 bits (256 couleurs) du sprite, sans les pixels transparents (valeur zero)
//         : table 'compress'  : offset et repetition des pixels optimise pour architecture 80x86
//         : table caculee pour un ecran de 512 pixels de large en 8 bits
//           structure t+0 (short)         offset destination
//                     t+2 (unsigned char) nombre de donnees 32 bits consecutives (rep;stosb)
//                     t+3 (unsigned char) nombre de donnees 8 bits consecutives (rep;stosl)
//------------------------------------------------------------------------------
char *etb_read_image(image * _pImage, char *_pFichier)
{
  unsigned int            i;
  short                  *_pMem;                               //pointeurs sur les tables
  char                   *_p8;
  short                  *_p16;
  int                    *_p32;                                //pointeurs sur le fichier (acces 8/16/32 bits)

  //lecture position & taille du sprite ..........................................
  _p16 = (short *)_pFichier;                                   //acces sur 16 bits
  _pImage->x_cg = litMot16bits(_p16++);                        //abscisse centre gravite (litMot16bits() lit un mot en little-endian)
  _pImage->y_cg = litMot16bits(_p16++);                        //ordonnee centre gravite
  _pImage->larg = litMot16bits(_p16++);                        //largeur de l'image
  _pImage->haut = litMot16bits(_p16++);                        //hauteur de l'image
  unsigned                _iLargeurSprite = _pImage->larg;

  //lecture des points et des zones de collisions ................................
  _pImage->nbr_pts_col = litMot16bits(_p16++);                 //nombre de points de collision
  _pMem = &_pImage->xy_pts_col[0][0];
  for(i = 0; i < NBR_PTS_COL_MAX * 2; i++)
    *(_pMem++) = litMot16bits(_p16++);
  _pImage->nbr_zon_col = litMot16bits(_p16++);                 //nombre de zones de collisions
  _pMem = &_pImage->xy_zon_col[0][0];
  for(i = 0; i < NBR_ZON_COL_MAX * 2; i++)
    *(_pMem++) = litMot16bits(_p16++);
  _pMem = &_pImage->lh_zon_col[0][0];
  for(i = 0; i < NBR_ZON_COL_MAX * 2; i++)
    *(_pMem++) = litMot16bits(_p16++);

  //lecture des coordonnees et angles des tirs ...................................
  _pImage->nbr_pts_tir = litMot16bits(_p16++);                 //nombre de sorties pour les tirs
  _pMem = &_pImage->xy_pos_tir[0][0];
  for(i = 0; i < NBR_PTS_TIR_MAX * 2; i++)
    *(_pMem++) = litMot16bits(_p16++);
  for(i = 0; i < NBR_PTS_TIR_MAX; i++)
    _pImage->angle_tir[i] = litMot16bits(_p16++);


  //lecture des pixels de l'images (pixels 8 bits pour le 256 couleurs) ..........
  _p32 = (int *)(_p16);                                        //acces sur 32 bits
  _pImage->nbr_pts_img = litMot32bits(_p32++);                 //nombre de pixels composant l'image
  _pImage->img = reserveMemoire(_pImage->nbr_pts_img * iOctetsParPixel);
  if(!_pImage->img)
    return 0;
  _p8 = (char *)_p32;                                          //acces des pixels sur 8 bits
  _p8 = lecture_pixels(_pImage->nbr_pts_img, _p8, _pImage->img);

  //lecture de la table 'compress' ...............................................
  _p32 = (int *)(_p8);                                         //acces 32 bits
  _pImage->nbr_data_comp = litMot32bits(_p32++);               //taille de la table 'compress' en octets
  _pImage->compress = reserveMemoire(_pImage->nbr_data_comp * 2);
  if(!_pImage->compress)
    return 0;
  _p8 = (char *)_p32;                                          //acces des pixels sur 8 bits
  _p8 =
    lecture_compress(_pImage->nbr_data_comp, _p8, _pImage->compress,
                     _iLargeurSprite);
  return _p8;
}

//------------------------------------------------------------------------------
// decodage d'un fichier "fonte" (.spr aussi)
// input  => _pFonte         : pointeur sur une structure 'fonte'
//        => _pFichier       : adresse memoire du fichier
//        => _iLargeurSprite : largeur
// output <= _p8             : pointeur
//------------------------------------------------------------------------------
char *etb_read_fonte(fonte * _pFonte, char *_pFichier, unsigned int _iLargeurSprite)
{ char                   *_p8;
  int                    *_p32;                                //pointeurs sur le fichier (acces 8/16/32 bits)
  _p8 = _pFichier;

  //lecture des pixels de l'images (pixels 8 bits pour le 256 couleurs) ..........
  _p32 = (int *)(_p8);                                         //acces sur 32 bits
  _pFonte->nbr_pts_img = litMot32bits(_p32++);                 //nombre de pixels composant l'image
  _pFonte->img = reserveMemoire(_pFonte->nbr_pts_img * iOctetsParPixel);
  if(!_pFonte->img)
  {
    afficheErreur("out of memory", "inits_game.cpp/etb_read_fonte()");
    return 0;
  }
  _p8 = (char *)_p32;                                          //acces des pixels sur 8 bits
  _p8 = lecture_pixels(_pFonte->nbr_pts_img, _p8, _pFonte->img);

  //lecture de la table 'compress' ...............................................
  _p32 = (int *)(_p8);                                         //acces 32 bits
  _pFonte->nbr_data_comp = litMot32bits(_p32++);               //taille de la table 'compress' en octets
  _pFonte->compress = reserveMemoire(_pFonte->nbr_data_comp * 2);
  if(!_pFonte->compress)
  {
    printf
      ("inits_game.cpp/etb_read_fontefonte(fonte*, char*) : error out of memory \n");
    return 0;
  }
  _p8 = (char *)_p32;                                          //acces des pixels sur 8 bits
  _p8 = lecture_compress(_pFonte->nbr_data_comp, _p8, _pFonte->compress, _iLargeurSprite);
  return _p8;
}

//------------------------------------------------------------------------------
// lecture des pixels 8 bits d'un fichier "sprite" (.spr)
// utilise pour une structure "image" ou "fonte"
// input  => _iNbr_pts_img : nombre de pixels dans le fichier
//        => _pFichier     : adresse memoire du fichier
//        => _pPixels      : buffer memoire utilise pour la structure "image" ou "fonte"
// sortie <= nouveau pointeur sur le fichier
//------------------------------------------------------------------------------
char *lecture_pixels(unsigned int _iNbr_pts_img, char *_pFichier, char *_pPixels)
{                                                              //printf("  lecture_pixels\n") ;
  unsigned int            i;
  unsigned short         *_pMem2;
  unsigned short          _wOctet;
  unsigned char           _bOctet;                             //valeur de travail
  unsigned char          *_pMem3;

  //lecture des pixels de l'images (pixels 8 bits pour le 256 couleurs) .........
  switch (iOctetsParPixel)
  {
    case 1:                                                   //mode ecran 256 couleurs
    {
      for(i = 0; i < _iNbr_pts_img; i++)
        _pPixels[i] = *(_pFichier++);
    }
      break;
      //..............................................................................
    case 2:                                                   //mode ecran 16 bits : 65,536 couleurs
    {
      _pMem2 = (unsigned short *)_pPixels;
      for(i = 0; i < _iNbr_pts_img; i++)
      {
        _bOctet = (unsigned char)*(_pFichier++);
        _wOctet = pal16[_bOctet];
        *(_pMem2++) = _wOctet;
      }
    }
      break;
      //..............................................................................
    case 3:                                                   //mode ecran 24 bits : 16,777,216 couleurs
      _pMem3 = (unsigned char *)_pPixels;
      for(i = 0; i < _iNbr_pts_img; i++)
      {
        _bOctet = (unsigned char)*(_pFichier++);
        unsigned char          *_p = (unsigned char *)&(pal32[_bOctet]);

        *(_pMem3++) = _p[0];                                   //blue
        *(_pMem3++) = _p[1];                                   //green
        *(_pMem3++) = _p[2];                                   //red
      }
      break;
      //..............................................................................
    case 4:                                                   //mode ecran 32 bits : 16,777,216 couleurs + couche alpha
      _pMem3 = (unsigned char *)_pPixels;
      for(i = 0; i < _iNbr_pts_img; i++)
      {
        _bOctet = (unsigned char)*(_pFichier++);
        unsigned char          *_p = (unsigned char *)&(pal32[_bOctet]);

        *(_pMem3++) = _p[0];
        *(_pMem3++) = _p[1];
        *(_pMem3++) = _p[2];
        *(_pMem3++) = _p[3];
      }
      break;
  }
  return _pFichier;
}

//------------------------------------------------------------------------------
// lecture des offsets & valeurs de repetitions (rep;stos) des fichiers "sprite" (.spr)
// utilise pour une structure "image" ou "fonte"
// la table 'compress' est prevu pour un affichage dans un ecran de 512 pixels de large en 256 couleurs
// - structure : _pFichier  (short)         offset destination
// -            _pFichier+2 (unsigned char) nombre de donnees 32 bits consecutives (rep;stosl)
// -            _pFichier+3 (unsigned char) nombre de donnees 8 bits consecutives (rep;stosb)
// -            _pFichier+4 (short)         offset destination
//                                                                      ...
// input  => _iNbr_data_comp : taille de la table 'compress' en octets
//        => _pFichier       : adresse memoire du fichier
//        => _pPixels        : buffer memoire utilise pour la structure "image" ou "fonte"
// output <= _pFichier       : nouveau pointeur sur le fichier
//------------------------------------------------------------------------------
char *lecture_compress(unsigned int _iNbr_data_comp, char *_pFichier, char *_pCompress, unsigned int _iLargeurSprite)
{                                                              //printf("  lecture_compress\n") ;
  unsigned int            i = 0;
  unsigned int            _iOffset;
  char                   *_p8;                                 //acces 8 bits au fichier
  short                  *_p16;                                //acces 16 bits au fichier
  unsigned int           *_pMem32;                             //acces 32 bits a la table "compress"
  unsigned short         *_pMem16;                             //acces 16 bits a la table "compress"
  unsigned short          _iRepeat;

  _pMem32 = (unsigned int *)_pCompress;
  _p16 = (short *)_pFichier;                                   //acces 16 bits sur le fichier
  _p8 = (char *)_p16;

  // mode 8 bits (256 couleurs) ..................................................
  switch (iOctetsParPixel)
  {
    case 1:                                                   //mode ecran 256 couleurs (on touche rien on laisse comme c'est)
      while (i < _iNbr_data_comp)
      {
        _iOffset = (unsigned short)litMot16bits(_p16++);         //lecture de l'offset (pour un ecran de 512 pixels de large)
        i += 2;                                                //on a lu deux octets
        *(_pMem32++) = _iOffset;                               //offset pour un ecran de 512 pixels en "iOctetsParPixel" octets par pixel
        _p8 = (char *)_p16;                                    //acces 8 bits sur le fichier
        _pMem16 = (unsigned short *)_pMem32;                   //on accede a la table sur 16 bits
        _iRepeat = (unsigned short)*(_p8++);
        i++;                                                   //on a lu un octet
        *(_pMem16++) = _iRepeat;                               //sauve nouvelle valeur
        _iRepeat = (unsigned short)*(_p8++);
        i++;                                                   //on a lu un octet
        *(_pMem16++) = _iRepeat;                               //sauve nouvelle valeur
        _p16 = (short *)_p8;                                   //acces 16 bits sur le fichier
        _pMem32 = (unsigned int *)_pMem16;                     //acces 16 bits sur la table
      }
      _pFichier = (char *)_p8;
      break;
      //..............................................................................
    case 2:                                                   //mode ecran 65536 couleurs (on recaclul la table pour un affichage 16 bits)
      while (i < _iNbr_data_comp)
      {
        _iOffset = (unsigned short)litMot16bits(_p16++);       //lecture de l'offset
        _iOffset = _iOffset * 2;                               //double offset (taille ligne trois fois plus grande)
        *(_pMem32++) = _iOffset;                               //offset pour un ecran de 512 pixels en 24 bits
        _p8 = (char *)_p16;                                    //acces 8 bits sur le fichier
        _pMem16 = (unsigned short *)_pMem32;                   //on accede a la table sur 8 bits
        unsigned int            _iRepeat1 = (unsigned short)*(_p8++);   //nombre de fois en 4 octets

        _iRepeat1 = _iRepeat1 * 2 * 4;                         //nombre de fois en 12 octets
        unsigned int            _iRepeat2 = (unsigned short)*(_p8++);   //nombre de fois en 1 octet

        _iRepeat2 = _iRepeat2 * 2;                             //nombre de fois en 3 octets
        _iRepeat1 = _iRepeat1 + _iRepeat2;
        _iRepeat2 = _iRepeat1;
        _iRepeat1 = _iRepeat1 / 4;
        _iRepeat2 = _iRepeat2 % 4;
        *(_pMem16++) = _iRepeat1;                              //sauve nouvelle valeur (nombre de mots de 32 bits a copier)
        *(_pMem16++) = _iRepeat2 / 2;                          //sauve la nouvelle valeur (nombre de mots de 16 bits a copier)
        _p16 = (short *)_p8;                                   //acces 16 bits sur le fichier
        _pMem32 = (unsigned int *)_pMem16;                     //acces 16 bits sur la table
        i += 4;                                                //on a lu 4 octets
      }
      _pFichier = (char *)_p8;
      break;

      break;
      //..............................................................................
    case 3:                                                   //mode ecran 16777216 couleurs
      while (i < _iNbr_data_comp)
      {
        _iOffset = (unsigned short)litMot16bits(_p16++);       //lecture de l'offset
        _iOffset = _iOffset * 3;                               //triple offset (taille ligne trois fois plus grande)
        *(_pMem32++) = _iOffset;                               //offset pour un ecran de 512 pixels en 24 bits
        _p8 = (char *)_p16;                                    //acces 8 bits sur le fichier
        _pMem16 = (unsigned short *)_pMem32;                   //on accede a la table sur 8 bits
        unsigned int            _iRepeat1 = (unsigned short)*(_p8++);   //nombre de fois en 4 octets

        _iRepeat1 = _iRepeat1 * 3 * 4;                         //nombre de fois en 12 octets
        unsigned int            _iRepeat2 = (unsigned short)*(_p8++);   //nombre de fois en 1 octet

        _iRepeat2 = _iRepeat2 * 3;                             //nombre de fois en 3 octets
        _iRepeat1 = _iRepeat1 + _iRepeat2;
        _iRepeat2 = _iRepeat1;
        _iRepeat1 = _iRepeat1 / 4;
        _iRepeat2 = _iRepeat2 % 4;
        *(_pMem16++) = _iRepeat1;                              //sauve nouvelle valeur
        *(_pMem16++) = _iRepeat2;                              //sauve la nouvelle valeur
        _p16 = (short *)_p8;                                   //acces 16 bits sur le fichier
        _pMem32 = (unsigned int *)_pMem16;                     //acces 16 bits sur la table
        i += 4;                                                //on a lu 4 octets
      }
      _pFichier = (char *)_p8;
      break;
      //..............................................................................
    case 4:                                                   //mode ecran 32 bits
    {
      while (i < _iNbr_data_comp)
      {

		// remarque : il faut caster litMot16bits() en unsigned short
		// sinon il arrive que la fonction signed soit appell�,
		// ca renvoit une valeur negative cast�en unsigned,
		// et fait planter la fonction d'affichage du sprite.
		// (exemple dans le fichier gardi_8.spr)
		//
        _iOffset = (unsigned short)litMot16bits(_p16++);         //lecture de l'offset (pour un ecran de 512 pixels de large)

        i += 2;                                                //on a lu deux octets
        _iOffset = _iOffset * 4;                               //quadruple offset (taille ligne trois fois plus grande)
        *(_pMem32++) = _iOffset;                               //offset pour un ecran de 512 pixels en 24 bits
        _p8 = (char *)_p16;                                    //acces 8 bits sur le fichier
        _pMem16 = (unsigned short *)_pMem32;                   //on accede a la table sur 8 bits
        _iRepeat = (unsigned short)*(_p8++);
        _iRepeat = _iRepeat * 4;
        i++;                                                   //on a lu un octet
        *(_pMem16++) = _iRepeat;                               //sauve nouvelle valeur
        _iRepeat = (unsigned short)*(_p8++);                   //nombre de donnees en 8 bits
        i++;                                                   //on a lu un octet
        *(_pMem16++) = _iRepeat;                               //sauve la nouvelle valeur
        _p16 = (short *)_p8;                                   //acces 16 bits sur le fichier
        _pMem32 = (unsigned int *)_pMem16;                     //acces 16 bits sur la table
      }
      _pFichier = (char *)_p8;
    }
      break;
  }
  return _pFichier;
}

//------------------------------------------------------------------------------
// load all images options
// les offsets d'affichage sont prevus a l'origine pour un ecran de 320 pixels
// 13 options differentes de 33 images :
//------------------------------------------------------------------------------
int load_options()
{
  if(!etb_read_fonte("graphics/option.spr", &options[0][0], MAX_TYP_OF_OPTIONS, NBR_IMAGES_OPTIONS, 32)) return 0; //387761 octets
  unsigned int            i, j;
  unsigned int            _iOffset;
  unsigned int            _iNbr_data_comp;
  unsigned int           *_pCompress;
  fonte                  *_pFonte;

  for(i = 0; i < MAX_TYP_OF_OPTIONS; i++)
  {
    for(j = 0; j < NBR_IMAGES_OPTIONS; j++)
    {
      _pFonte = &options[i][j];
      _pCompress = (unsigned int *)_pFonte->compress;
      _iNbr_data_comp = _pFonte->nbr_data_comp >> 2;
      do
      {
        _iOffset = *_pCompress;
        if(_iOffset > (26 * iOctetsParPixel))                  //64
        {
          _iOffset -= (320 * iOctetsParPixel);                 //original width screen's 320 pixels
          _iOffset += (OPT_LARGE * iOctetsParPixel);           //actual width screen's 64 pixels
        }
        *_pCompress = _iOffset;
        _pCompress += 2;                                       //+8 bytes
      } while (--_iNbr_data_comp > 0);
    }
  }
  if(!etb_read_fonte("graphics/double.spr", &doublex[0], 1, 5, 1))
    return 0;                                                  //   760 octets
  for(j = 0; j < 5; j++)
  {
    _pFonte = &doublex[j];
    _pCompress = (unsigned int *)_pFonte->compress;
    _iNbr_data_comp = _pFonte->nbr_data_comp >> 2;
    do
    {
      _iOffset = *_pCompress;
      if(_iOffset > (16 * iOctetsParPixel))
      {
        _iOffset -= (320 * iOctetsParPixel);                   //original width screen's 320 pixels
        _iOffset += (OPT_LARGE * iOctetsParPixel);             //actual width screen's 64 pixels
      }
      *_pCompress = _iOffset;
      _pCompress += 2;
    } while (--_iNbr_data_comp > 0);
  }
  return 1;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void print_val_image(image * _pImage)
{
  printf("\nImage->nbr_data_comp : %i\n", _pImage->nbr_data_comp);
  int                     l, k = _pImage->nbr_data_comp >> 2;
  unsigned short         *pp = (unsigned short *)_pImage->img;

  for(l = 0; l < k; l++)
    printf("%X, ", *(pp++));
  printf("\n******************************\n");
  pp = (unsigned short *)_pImage->compress;
  unsigned char          *oo;

  for(l = 0; l < k; l++)
  {
    printf("offset=%i; ", *(pp++));
    oo = (unsigned char *)pp;
    printf("rep32=%i; ", *(oo++));
    printf("rep8=%i; \n", *(oo++));
    pp = (unsigned short *)oo;
  }
  printf("\n******************************\n");
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void print_val_fonte(fonte * _pFonte)
{
  unsigned short         *oo;

  printf("\n_pFonte->nbr_data_comp : %i\n", _pFonte->nbr_data_comp);
  int                     l, k = _pFonte->nbr_data_comp >> 2;
  unsigned int           *pp = (unsigned int *)_pFonte->img;

  for(l = 0; l < k; l++)
  {
    printf("%X, ", *(pp++));
    oo = (unsigned short *)pp;
    printf("%X, ", *(oo++));
    pp = (unsigned int *)oo;

  }
  printf("\n******************************\n");

  pp = (unsigned int *)_pFonte->compress;
  for(l = 0; l < k; l++)
  {
    printf("offset=%i; ", *(pp++));
    oo = (unsigned short *)pp;
    printf("rep32=%i; ", *(oo++));
    printf("rep8=%i; \n", *(oo++));
    pp = (unsigned int *)oo;
  }
  printf("\n******************************\n");
}

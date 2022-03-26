//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "powermanga.cpp"
// created      : ?
// updates      : 2005-01-08
// id		: $Id: powermanga.cpp,v 1.1.1.1 2005/01/25 05:53:49 gurumeditation Exp $
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
//..............................................................................
#include "powermanga.hpp"
extern configfile*      power_conf;
//..............................................................................
unsigned int            iJoueIntro = 1;                        //1=start anim / 2= end anom 
unsigned int            iCodeTouche = 0;                       //code key pressed / code touche pressee
unsigned int            iBoutonFeu = 0;                        //1=bouton de la manette de jeu enfonce
unsigned int            iBoutonOption = 0;                     //1=bouton option de la manette de jeu enfonce
unsigned int            iManetteGauche = 0;
unsigned int            iManetteDroite = 0;
unsigned int            iManetteHaut = 0;
unsigned int            iManetteBas = 0;
unsigned int            iManettePosX = 0;
unsigned int            iManettePosY = 0;
unsigned int            iCompteurGlobal;                       //(ETB)compteur global a tout faire
unsigned int            iCongratulation = 0;                   //(ETB)1=congratulations sont en cours
int                     iNumeroGardien;
void                    _execute_congra(int);
int                     iErreurInitialisation;                 //1=erreur a l'initialisation
extern unsigned int     iOctetsParPixel;                       //nombre d'octets par pixels (1=256 couleurs / 2=65536 couleurs)
extern _etbfonte1             *pEtbfonte1;         //(ETB)objet cpp geran affichage caracteres 8x8

//..............................................................................
// Variables utilisees pour la gestion generale du jeu *************************
int                     etat_menu;                             //MENU_OFF(0)=menu inactif ; MENU_ON(1)=menu actif
bool                    player_pause;                          //1=pause en cours
bool                    aff_pause;                             //1=le texte "pause" est affiche
bool                    game_over_player_one;                  //1=le joueur vient de perdre son vaisseau.(game over)
bool                    aff_game_over = 1;                     //1=affichage du game over en cours

//..............................................................................
unsigned int            bActive;                               //1=application activee
char                    szMsg[1000];
char                    g_sError[2048];
unsigned int            g_bErrorOccured = 0;
short                   touch[500];                            //tableau flags des touches du clavier
short                   mouse_b;                               //1=bouton gauche souris presse

// donnees utiles pour PowerManga ----------------------------------------------
Eclair                  eclair1;
bool                    eclair_actif;                          //1=joueur peut utiliser l'eclair
int                     bool_et = 0;
int                     pos_y_menu = BANDE_DE_CLIP + HAUT_ECR_RAM;      //ordonnee affichage du menu
int                     taille_mem_glob;                       //taille memoire maximum utilisee

//..............................................................................
#ifdef DEVELOPPEMENT
short                   mouse_x;
short                   mouse_y;
short                   mouse_here;
bool                    curv_editor = 0;                       // Flag indiquant si l'on se trouve dans l'éditeur de courbe ou non.
#endif
//..............................................................................
char                   *ecran_ram;                             //adresse ecran logique
char                   *chaine;                                //conversion des chaines de caracteres
bool                    Inits_Made = 0;

//..............................................................................
int                     touche_ctrl;                           //1=touche ctrl ou bouton option de la manette de jeu a ete enfoncee
int                     num_image_logo;                        //logotlk :
bool                    bool_logo;                             //logotlk :
int                     posx_logo;                             //logotlk : abscisse
int                     posy_logo;                             //logotlk : ordonnee
bool                    move_logo;                             //logotlk : 1=deplacement du logo

// defilement de texte en bas de l'ecran du menu ..............................
fonte                   fnt_scroll[NBR_FONTES_SCROLL];         //tableau de variables utilise pour l'affichage 
int                     liste_fnt_scroll[NBR_FNT_SCROLL_MAX];  //liste de toutes les fontes gerees
int                     cmpt_fnt_scroll;                       //compteur des fontes
short                   nbr_fnt_scroll;                        //nombre de fontes gerees actuellement
int                     new_fnt_scroll;                        //indice du tableau sur la prochaine objet fonte
fntscroll               fnt_scr[NBR_FNT_SCROLL_MAX];           //tableau de tous les objets fontes 

//..............................................................................
int                     cmpt_vbls_x2, cmpt_vbls_x4, aff_x2_rj, aff_x4_rj;       //variables utilisees pour la gestion des multiplicateurs.
short                   first_value, second_value, compteur, compteur1;
int                     old_option;                            // Ancien numéro d'option, utilisé pour l'effacement dans le panneau d'options.
image                   fire[MAX_TYP_OF_TIR][NBR_IMAGES_TIR];  // Définit le tableau des différents tirs.
fonte                   menu_spr[NBR_OPTION_MENU][NBR_IMAGES_MENU];
fonte                   logotlk[NBR_IMAGES_LOGOTLK];
vaisseau                vj[NBR_MAX_J];                         //structure des vaisseaux des joueurs.
short                   ge_act_pos_x = 0, ge_act_pos_y = 0;    // Variables indiquant la position du carré actuellement sélectionné dans la grille.
short                   t_l, t_s;                              // Variables utilisées lors du chargement ou de la sauvegarde d'un fichier dans les éditeurs.
grid                    grille;                                // Déclaration de la structure de grille des vaisseaux ennemis.
image                   meteo[MAX_TYP_OF_METEOR][NBR_IMAGES_METEOR];    // Définit le tableau des différentes météorite.
image                   enemi[MAX_TYP_OF_ENEMI + MAX_TYP_OF_S_ENEMI + MAX_TYP_OF_NMI_SPE + MAX_TYP_OF_OBJ_SPE][NBR_IMAGES_MAX]; // Définit le tableau des différents ennemis.
image                   explo[MAX_TYP_OF_EXPLOS][NBR_IMAGES_EXP];       // Défini le tableau des différentes images utilisées pour les explosions.
image                   eclat[MAX_TYP_OF_ECLATS][NBR_IMAGES_ECLATS];    // Défini le tableau des différentes images utilisées pour les explosions.
image                   boule[MAX_TYP_OF_BOULES][NBR_IMAGES_BOULES];    // Définit le tableau des différents ennemis.
image                   canon[MAX_TYP_OF_CANONS][NBR_IMAGES_CANONS];    // Définit le tableau des différents ennemis.
curve                   initial_curve[NBR_CURVE_MAX];          // structure des courbes de bezier
curve_level             courbe;                                // Déclaration de la structure de niveaux courbe.
unsigned char           coulor[12];                            // Tableau contenant les couleurs choisies par l'utilisateur.
unsigned char           color_dead[NBR_COLOR_DEAD + 1];        // Tableau contenant les couleurs utilisées lors de la disparition d'un vaisseau.
unsigned char           color_eclair[2 + 1];                   // tableau des couleurs utilisees lors de la disparition d'un vaisseau
tir                     tirs[NBR_TIRS_MAX];                    //tableau de tous les objets tirs
int                     liste_tirs[NBR_TIRS_MAX];              //liste des 'tirs' geres
int                     cmpt_tirs;                             //compteur nombre de tirs.
short                   nbr_tirs;                              //nombre de tirs actuellement geres
int                     new_tir;                               // Indique l'indice du tableau corespondant à un nouvel objet 'tirs'.
star                    stars[NBR_OF_STARS];                   //tableau du fond etoiles defilant verticalement 
image                   star_field[TYPE_OF_STARS][NBR_ETOILES]; //tableau des images des etoiles 
bool                    aff_star_field = 1;                    //1=affiche le champ d'etoile
float                   vit_star_field = 2.0;                  //vitesse du champs d'etoiles
vaisseau_nmi            ve[MAX_NMI_ON_SCREEN];                 // Déclaration de la structure des vaisseaux ennemis.
int                     liste_ve[MAX_NMI_ON_SCREEN];           // liste de tous les vaisseaux ennemis a gerer
int                     cmpt_nmis;                             //compteur nombre d'ennemis
short                   nbr_nmis = 0;                          // Indique le nombre d'enemis actuellement gérés.
int                     new_ve;                                // Indique l'indice du tableau corespondant à un nouvel objet 've'.
explosion               xpl[MAX_NMI_ON_SCREEN];                //structure utilisee par les explosions
int                     liste_xpl[MAX_NMI_ON_SCREEN];          //liste de tous les 'xpl' geres
int                     cmpt_exps;                             //compteur du nombre d'explosions
short                   nbr_exps = 0;                          //nombre d'explosions actuellement gerees
char                    str_file[50];                          // Déclaration d'une chaine pour le chargement de fichiers.
char                    str_tmp[40];                           // Chaine temporaire utilisée pour le chargement de fichiers.

//..............................................................................
char                   *nom_vaisso[MAX_TYP_OF_ENEMI+MAX_TYP_OF_S_ENEMI+4][20]={ //les differents vaisseaux ennemis
                          {"BOONIES"},{"MOOARCKS"},{"ACHEES"},{"EVILIANS"},
                          {"RAGOONS"},{"FLABBIES"},{"BOWANS"},{"HIJACKERS"},
                          {"DISTASIANS"},{"BRITTERS"},{"SICKIES"},{"ARIANS"},
                          {"STINCKIES"},{"BZEURCKS"},{"WEAKIES"},{"MASKIES"},
                          {"SCRAPIES"},{"SCROLLIANS"},{"SHAMISTEES"},{"FOOLINIANS"},
                          {"GLAMOOSH"},
                          {"BIGBOONIES"},{"BIGMOOARCKS"},{"BIGACHEES"},{"BIGEVILIANS"},
                          {"BIGRAGOONS"},{"BIGFLABBIES"},{"BIGBOWANS"},{"BIGHIJACKERS"},
													{"BIGDISTASIANS"},{"BIGBRITTERS"},{"BIGSICKIES"},{"BIGARIANS"},
                          {"BIGSTINCKIES"},{"BIGBZEURCKS"},{"BIGWEAKIES"},{"BIGSUPER MASKIES"},
                          {"BIGSCRAPIES"},{"BIGSCROLLIANS"},{"BIGSHAMISTEES"},{"BIGFOOLINIANS"},
                          {"BIGGLAMOOSH"}} ; 

//..............................................................................
short                   nbr_boules = 0;                        //nombre de boules actuellement gerees
int                     liste_bou[MAX_BOULES_ON_SCREEN];       //liste de toutes les 'bou' gerees
str_boule               bou[MAX_BOULES_ON_SCREEN];             //structure des boules.
int                     new_bou;                               //pointeur sur un nouvel element 'bou'
int                     cmpt_boules;                           //compteur du nombre de boules
short                   nbr_canons = 0;                        //nombre de canons actuellement geres
int                     liste_can[MAX_CANONS_ON_SCREEN];       //liste de tous les 'can' geres
str_canon               can[MAX_CANONS_ON_SCREEN];             //structure des canons.
int                     new_can;                               //pointeur sur un nouvel element 'can'.
int                     cmpt_canons;                           //compteur du nombre de canons
option                  aff_option[MAX_TYP_OF_OPTIONS];                        //tableau pour afficher options 
int                     nbr_ve_spe_generes;                    //nombre de vaisseaux spéciaux déjà utilisés dans le jeu.
short                   nbr_bonus = 0;                         //nombre de bonus actuellement gérés.
image                   bonus[MAX_TYP_OF_BONUS][NBR_IMAGES_BONUS];      //tableau des différentes images des bonus.
fonte                   options[MAX_TYP_OF_OPTIONS][NBR_IMAGES_OPTIONS];        //tableau des différentes images des options.
str_bonus               bon[MAX_BONUS_ON_SCREEN];              //structure des bonus.
image                   vj1[NBR_MAX_VAIS_J][NBR_MAX_IMG_VAIS_J];        //structure des images des vaisseaux du joueur 1.
bool                    bar_nrj_player = 1;
bool                    bar_nrj_guardian = 1;
int                     tmp_apparition_vj;                     //vaisseau joueur : compteur temps apparition 
bool                    disparition_vj;                        //vaisseau joueur : 1=disparition 
int                     cmpt_vbl_nrj_player;                   //compteur augmentation automatique de l'energie du vaisseau appartenant au joueur.
int                     bonusx;                                //multiplicateur de points (*1, *2, *4)
fonte                   doublex[5];                            //tableau des differentes images des multiplicateurs

//guardians ....................................................................
bool                    apparition_gardian;                    //1=apparition d'un gardien.
int                     gardian_activity;                      //1=guardian running
image                   gardi[NBR_ANIM_GARDIAN][NBR_IMAGES_NMI_S];      //images des differents gardiens

// meteors .....................................................................
bool                    meteor_activity;
int                     nbr_meteor_generer;
int                     cmpt_vbl_app_meteor;
int                     taille_meteor;
int                     numero_du_meteor;

//..............................................................................
bool                    option_change;                         //Indique si un changement d'option à lieu ou pas.
float                   depix[13][32];                         //tableau de deplacement en x des tirs a trajectoire fixe
float                   depiy[13][32];                         //tableau de deplacement en y des tirs a trajectoire fixe
struct poly             polyg;                                 //defini un polygone pour la routine d'affichage d'Etienne.
float                   pre_sin[] = { 0.0f, 0.19509032202f, 0.38268343237f, 0.55557023302f, 0.70710678119f, 0.83146961230f, 0.92387953251f, 0.98078528040f, 1.0f, 0.98078528040f, 0.92387953251f, 0.83146961230f, 0.70710678119f, 0.55557023302f, 0.38268343237f, 0.19509032202f, 0.0f, -0.19509032202f, -0.38268343237f, -0.55557023302f, -0.70710678119f, -0.83146961230f, -0.92387953251f, -0.98078528040f, -1.0f, -0.98078528040f, -0.92387953251f, -0.83146961230f, -0.70710678119f, -0.55557023302f, -0.38268343237f, -0.19509032202f };  // Tableau de sinus précalculés.
float                   pre_cos[] = { 1.0f, 0.98078528040f, 0.92387953251f, 0.83146961230f, 0.70710678119f, 0.55557023302f, 0.38268343236f, 0.19509032201f, 0.0f, -0.19509032201f, -0.38268343236f, -0.55557023302f, -0.70710678119f, -0.83146961230f, -0.92387953251f, -0.98078528040f, -1.0f, -0.98078528040f, -0.92387953251f, -0.83146961230f, -0.70710678119f, -0.55557023302f, -0.38268343236f, -0.19509032201f, 0.0f, 0.19509032201f, 0.38268343236f, 0.55557023302f, 0.70710678119f, 0.83146961230f, 0.92387953251f, 0.98078528040f };  // Tableau de cosinus précalculés.
short                   curv_number;                           //numero de courbe actuelle.
bool                    all_nmi_killed;                        //1=tous les ennemis sont morts
int                     tmp_tsts_x;
int                     tmp_tsts_y;
int                     tmp_tstd_x;
int                     tmp_tstd_y;
short                   ce_vais_act = 0;                       // Variable utilisée pour indiquer le vaisseau actuellement sélectionné dans l'éditeur de courbe.
short						        cercle_px[] = {30,29,29,29,28,27,26,25,24,22,21,19,17,15,13,11,9,7,4,2,0,-2,-4,-7,-9,-11,-13,-15,-17,-19,-21,-22,-24,-25,-26,-27,-28,-29,-29,-29,-30,-29,-29,-29,-28,-27,-26,-25,-24,-22,-21,-19,-17,-15,-13,-11,-9,-7,-4,-2,0,2,4,7,9,11,13,15,17,19,21,22,24,25,26,27,28,29,29,29};
short						        cercle_py[] = {0,2,4,7,9,11,13,15,17,19,21,22,24,25,26,27,28,29,29,29,30,29,29,29,28,27,26,25,24,22,21,19,17,15,13,11,9,7,4,2,0,-2,-4,-7,-9,-11,-13,-15,-17,-19,-21,-22,-24,-25,-26,-27,-28,-29,-29,-29,-30,-29,-29,-29,-28,-27,-26,-25,-24,-22,-21,-19,-17,-15,-13,-11,-9,-7,-4,-2};


// game language
char			Lang[8];


//******************************************************************************
// "upDateFrame" : the main loop of game / boucle principale du jeu
//******************************************************************************
int UpdateFrame()
{                                                              //static int                    i, j, k, l, m, n, o ; // variables utilisees pour les boucles
  static int i;

//..............................................................................
#ifdef DEVELOPPEMENT
  static bool  grid_editor = 0; //1=editeur de grille en cours
#endif
//..............................................................................
  iCompteurGlobal++; // global counter / compteur global 
  ce_vais_act = 0;

  //joue les animations d'etienne ----------------------------------------------
  if(iJoueIntro != 0)
  {
    if(!joueAnimation())
    { afficheErreur("joueAnimation() failed !",
                    "powermanaga.cpp/UpdateFrame()");
      return 0;
    }
    else
      return 1;
  }

  //efface l'ecran logique -----------------------------------------------------
  raz_ecran(ecran_ram + (BANDE_DE_CLIP * LARG_ECR_RAM_TOT * iOctetsParPixel) +
            (BANDE_DE_CLIP * iOctetsParPixel),
            (LARG_ECR_RAM * iOctetsParPixel) >> 2, HAUT_ECR_RAM,
            (LARG_ECR_RAM_TOT - LARG_ECR_RAM) * iOctetsParPixel);

  //restauration de l'energie du vaisseau du joueur ----------------------------
  vaisseau_restaure();

  //editeurs de "grilles" et de "courbes" --------------------------------------
#ifdef DEVELOPPEMENT
  if(grid_editor || curv_editor)
  {
    if(grid_editor)
      Grid_Edit();
    else
      courbe_editeur();
  }
  else
#endif

  //routines du jeu ************************************************************
  { 
    //pause inactive et menu inactif ?
    if(!player_pause && etat_menu == MENU_OFF &&
      _score::getActionExecution() == 0)
    { grille_execution(); //phase 2 : execution des "grilles" de vaisseaux
      courbe_execution(); //phase 1 : execution des "courbes" de vaisseaux
      meteor_execution(); //phase 3 : execution des vagues de meteores 
    }
  }

  //affiche le fond d'etoile defilant verticalemenent --------------------------
  afficheEtoiles();

  //gestion des bonus ----------------------------------------------------------
  bonus_execution();

  //gestion des canons & des boules autour du vaisseau du joueur ---------------
  if(!aff_game_over && _score::getActionExecution() == 0)
  {
    boule_execution();
    canon_execution();
  }

  //gestion des ennemis --------------------------------------------------------
  if(iCongratulation == 0)
    ennemis_execution(); //gestion de tous les ennemis existants
  else
    _execute_congra(iNumeroGardien); //routine special "congratulations"

  //gestion de l'invincibilite du vaisseau -------------------------------------
  vaisseau_invincibilite();

  //gestion de l'eclair (arme du vaisseau joueur) ------------------------------
  eclairExecution();

  //affichage du vaisseau du joueur --------------------------------------------
  if(!aff_game_over && vj[J1].visible)
  {
    if(((short)vj[J1].spr.coor_y +
        vj[J1].spr.img[vj[J1].spr.img_act]->haut) >= BANDE_DE_CLIP)
    {
      if(vj[J1].affich_white)                                  // affichage en blanc ?
      {
        putsprcolor(coulor[BLANC], vj[J1].spr.img[vj[J1].spr.img_act],
                    (int)(vj[J1].spr.coor_x), (int)(vj[J1].spr.coor_y));
        vj[J1].affich_white = 0;
      }
      else                                                     //sinon affichage normal
      {
        putsprite3(vj[J1].spr.img[vj[J1].spr.img_act], (unsigned int)vj[J1].spr.coor_x,
                   (unsigned int)vj[J1].spr.coor_y);
      }
    }
  }

  //gestion des explosions ------------------------------------------------------
  explosionExecution();

  //gestion des tirs ------------------------------------------------------------
  tirsExecutions();

  //si plus d'ennemi a l'ecran, on passe a la phase suivante --------------------
  if(!nbr_nmis && !player_pause && etat_menu == MENU_OFF
     && _score::getActionExecution() == 0)
  {
    if(!gardien_testFin())                                     // on etait dans une phase "gardien" ?
    {
      afficheErreur("gardien_testFin failed", "powermanga.cpp/UpdateFrame()");
      return 0;
    }
    courbe_testFin();                                          // on etait dans une phase "courbe" ?
    grille_testFin();                                          // on etait dans une phase "grille" ?
    if(!meteor_initialise())                                   // on etait dans une phase "meteor" ?
    {
      afficheErreur("meteor_initialise failed !",
                    "powermanga.cpp/UpdateFrame()");
      return 0;
    }
  }

  //deplace un pixel a l'ecran, en guise de cuseur souris  ----------------------
#ifdef DEVELOPPEMENT
  if(mouse_here)
    putpixel(ecran_ram, mouse_x, mouse_y, 5);
#endif

  //affichage des ondes de choc -------------------------------------------------
  Draw_Onde_De_Choc();

  // test si l'etat des options a change, animation des options ------------------
  option_execution();

  //affichage scores, "game over", "about" & order (ETB) -------------------------
  //if (aff_game_over)//"game over" en cours ?
  {
    if(etat_menu == MENU_OFF)
      _score::execution(0);                                    // gestion menu desactive
    else
      _score::execution(1);                                    // gestion menu active
  }

  //affiche le mot "pause" a l'ecran, si la pause est active ---------------------
  if(aff_pause)
    Aff_Game_Pause_Player_One();

  //apparition du vaisseau du joueur ---------------------------------------------
  if(tmp_apparition_vj > 0)
  {
    tmp_apparition_vj--;                                       //decrementation du compteur
    vit_star_field += 0.028f;                                  //augmentation de la vitesse du champ d'etoile
    if(vit_star_field > 2.0)
      vit_star_field = 2.0;                                    //bloque a la vitesse maximum
    vj[J1].invincible = TEMPS_INVINCIBLE / 3;                  //temps d'invinsiblite du vaisseau
  }

  //disapparition du vaisseau du joueur ------------------------------------------
  if(disparition_vj && iCongratulation == 0)
  {
    vit_star_field -= 0.02f;                                   //diminue la vitesse du champ d'etoile
    if(vit_star_field <= 0.0)                                  //bloque la vitesse a zero
    {
      vit_star_field = 0.0;
      vj[J1].vitesse_y = 0.0;
    }
    else
      vj[J1].vitesse_y += -0.15f;                              //augmente vitesse vaisseau, pour qu'il sorte de l'ecran
  }

  //deplacement des caracteres affichant le numero du niveau----------------------
  Aff_Level_Player_One();

  //gestion du defilemenet de texte en bas de l'ecran du menu --------------------
  Gestion_Fontes_Ki_Scrollent();

  //gestion du menu principal de powermanga --------------------------------------
  if(etat_menu != MENU_OFF)
  {
    if(!Gestion_Menu())                                        //routine gerant le menu
      return 0;
  }
  else
  {
    if(touch[0])                                               //touche [ESC] pressee ?
    {
      etat_menu = APPARITION_MENU;                             //met le flag apparition du menu a 1
      Init_Scroll_Present();                                   //initialise le defilement du texte en bas
    }
  }

  //[f1]=apparition du vaisseau ; [f2]=disparition du vaisseau -------------------
#ifdef DEVELOPPEMENT
  if(touch[2])
    disparition_vj = 1;
  if(touch[1] && vit_star_field == 0.0)
    Apparition_Vaisseau();
#endif

  //gestion du "logotlk"----------------------------------------------------------
  if(move_logo)                                                //on gere le logo ?
  {
    putsprite3(&logotlk[num_image_logo], posx_logo, posy_logo);
    if(bool_logo)                                              //on passe a l'animation suivante ?
    {
      num_image_logo++;
      if(num_image_logo >= NBR_IMAGES_LOGOTLK)
        num_image_logo = 0;
      bool_logo = 0;
      posy_logo--;                                             //deplacement du logo vers le haut
    }
    else
      bool_logo = 1;
    if(posy_logo <= 32)
      move_logo = 0;                                           //logo sort en haut, on stop la gestion
  }

  //(ETB) gestion affichage caractere 8x8 (about, parametres, variables) -------
  pEtbfonte1->execution();

  //test si le joueur perd son vaisseau, et si la partie est terminee ------------
  if(game_over_player_one && !aff_game_over
     && _score::getActionExecution() == 0)
  {

    if(vj[J1].type_vaisseau != V1J)                            //le vaisseau n'est pas le premier vaisseau ?
    {
      vj[J1].type_vaisseau--;                                  //non, on regresse au vaisseau inferieur
#ifdef USE_SDLMIXER
      seal_joue(VCHANGE2);                                     //joue un son avec seal
#endif
      vj[J1].metamorphose = 1;                                 //indique une metamorphose du vaisseau
      vj[J1].cad_tir = 50 - (vj[J1].type_vaisseau * 5 + 5);    //nombre de vbls necessaires avant que le vaisseau puisse a nouveau tirer
      vj[J1].spr.pow_of_dest = (short)(vj[J1].type_vaisseau * 20 + 20); //indique la puissance de destruction du sprite
      vj[J1].spr.etat_dommages = vj[J1].spr.pow_of_dest;       //indique l'atat de dommages du sprite
      bonusx = 0;                                              //raz le multiplicateur de points 
      if(!aff_option[1].close_option)
        Init_Anim_Option(1, 1);                                //ferme l'option ernergie
      if(aff_option[10].close_option)
        Init_Anim_Option(10, 0);                               //ouverture option transformation
      vj[J1].invincible = TEMPS_INVINCIBLE;                    //temps d'invincibilite
      bar_nrj_player = 1;                                      //demande reaffichage barre d'energie du joueur
      game_over_player_one = 0;
      for(i = 0; i < NBR_MAX_IMG_VAIS_J; i++)
        vj[J1].spr.img[i] = (image *) & vj1[vj[J1].type_vaisseau][i];   //donne adresse des images du vaisseau du joueur
      Init_Tab_Boules();
      for(i = 0; i < MAX_BOULES_ON_SCREEN; i++)
        liste_bou[i] = 0;                                      //initialise tableau 'bou'
      Init_Tab_Canons();
      for(i = 0; i < MAX_CANONS_ON_SCREEN; i++)
        liste_can[i] = 0;                                      //initialise tableau "can'
    }
    else
    {
      _initialiseGameOver();                                   //sinon, c'etait le vaisseau 1, la partie est terminee
    }

  }

  //gestion de la barre d'energie du joueur --------------------------------------
  update_bar_nrj_player();

  //gestion de la barre d'energie du gardien -------------------------------------
  gardienBarreNRJ();

  //gestion du score du joueur ---------------------------------------------------
  Aff_Score_Player_One();                                      //oui, saut a la routine d'affichage du score

  //gestion du score du joueur ----------------------------------------------------
#ifdef DEVELOPPEMENT
  if(touch[40] && touch[63])
    grid_editor = 1;                                           //[e]+[g] active editeur de grille
  if(touch[40] && touch[78])
    curv_editor = 1;                                           //[e]+[c] active editeur de courbes
  if(touch[47] && touch[63])                                   //[p]+[g] ?
  {
    grid_editor = 0;
    curv_editor = 0;
  }                                                            //oui, retourne dans le jeu
#endif

  //deplacement du vaisseau du joueur ---------------------------------------------     
  if(!player_pause && !aff_game_over && etat_menu == MENU_OFF
     && _score::getActionExecution() == 0)
  {
    if(tmp_apparition_vj <= 0 && !disparition_vj)              // phase de d'apparition ou disparition ?
    {
      if(vj[J1].vitesse_x >= 0.5f)
        vj[J1].vitesse_x -= 0.2f;                              // non, decremente vitesse de deplacement
      if(vj[J1].vitesse_x <= -0.5f)
        vj[J1].vitesse_x += 0.2f;
      if(vj[J1].vitesse_y >= 0.5f)
        vj[J1].vitesse_y -= 0.2f;
      if(vj[J1].vitesse_y <= -0.5f)
        vj[J1].vitesse_y += 0.2f;
      if(!game_over_player_one)                                //le joueur est mort ?
      {
        if(touch[71] || iManetteGauche == 1)
          vj[J1].vitesse_x -= 0.5f;                            //test touches deplacement
        if(touch[73] || iManetteDroite == 1)
          vj[J1].vitesse_x += 0.5f;
        if(touch[55] || iManetteHaut == 1)
          vj[J1].vitesse_y -= 0.5f;
        if(touch[87] || iManetteBas == 1)
          vj[J1].vitesse_y += 0.5f;
        if(touch[88] && touch[58])
          _initialiseGameOver();                               //(ETB)[ctrl]+[q] force "game over"
      }
    }
    else                                                       //oui, phase d'apparition ou de disparition du vaisseau du joueur
    {
      if(vj[J1].vitesse_x >= 0.5f)
        vj[J1].vitesse_x -= 0.4f;                              //decremente vitesse
      if(vj[J1].vitesse_x <= -0.5f)
        vj[J1].vitesse_x += 0.4f;
      if(vj[J1].vitesse_y >= 0.5f)
        vj[J1].vitesse_y -= 0.01f;
      if(vj[J1].vitesse_y <= -0.5f)
        vj[J1].vitesse_y += 0.01f;
    }
  }

  //touche [p] ou [pause] pressee : active / desactive la pause -------------------
  if((touch[47] || touch[150]) && !game_over_player_one
     && tmp_apparition_vj <= 0 && !disparition_vj && etat_menu == MENU_OFF
     && _score::getActionExecution() == 0)
  {
    if(player_pause)
      player_pause = 0;
    else
      player_pause = 1;
    if(aff_pause)
      aff_pause = 0;
    else
      aff_pause = 1;
  }
  touch[47] = 0;
  touch[150] = 0;                                              //raz les touches de pause

  
#ifdef SDL_TLK
	//if ((touch[39]) && (touche_ctrl))
	if((touch[39]) && !_score::isInputName())
	{	if(power_conf->fullscreen)
			power_conf->fullscreen = 0;
		else
			power_conf->fullscreen = 1;
		init_video_mode();
	}
	touch[39] = 0;
#endif
  
  
  //clip la vitesse de deplacement du vaisseau du joueur -------------------------
  if(vj[J1].vitesse_x >= (float)vj[J1].vit_dep_vaisseau)       //depasse vitesse droite ?
  {
    vj[J1].vitesse_x = (float)vj[J1].vit_dep_vaisseau;         //oui, bloque la vitesse du vaisseau
    vj[J1].spr.img_act = 4;                                    //image la plus a droite du vaisseau
  }
  else
  {
    if(vj[J1].vitesse_x > 0)
      vj[J1].spr.img_act = 3;                                  //image de droite
  }
  if(vj[J1].vitesse_x <= (float)(-vj[J1].vit_dep_vaisseau))    //depasse vitesse gauche ?
  {
    vj[J1].vitesse_x = (float)(-vj[J1].vit_dep_vaisseau);      //oui; bloque la vitesse              
    vj[J1].spr.img_act = 0;                                    //image la plus a gauche du vaisseau.
  }
  else
  {
    if(vj[J1].vitesse_x < 0)
      vj[J1].spr.img_act = 1;                                  //image de gauche
  }
  if(tmp_apparition_vj <= 0 && !disparition_vj)                //phase d'apparition ou disparition ?
  {
    if(vj[J1].vitesse_y > vj[J1].vit_dep_vaisseau)
      vj[J1].vitesse_y = (float)vj[J1].vit_dep_vaisseau;       //non clip la vitesse verticale
    if(vj[J1].vitesse_y < (-vj[J1].vit_dep_vaisseau))
      vj[J1].vitesse_y = (float)(-vj[J1].vit_dep_vaisseau);
  }
  if(!player_pause && etat_menu == MENU_OFF
     && _score::getActionExecution() == 0)
  {
    vj[J1].spr.coor_x += vj[J1].vitesse_x;                     //calcul nouvelle abscisse
    vj[J1].spr.coor_y += vj[J1].vitesse_y;                     //calcul nouvelle ordonnee
    if(vj[J1].spr.coor_y < (float)(DEBUT_ECRAN_Y - 100))
      vj[J1].spr.coor_y = (float)(DEBUT_ECRAN_Y - 100);        //test si le vaisseau du joueur ne va pas trop loin dans le haut de l'ecran
  }
  if(fabs(vj[J1].vitesse_x) < 0.5)                             //met la vitesse horizontal du vaisseau a zero
  {
    vj[J1].vitesse_x = 0.0;
    vj[J1].spr.img_act = 2;                                    //image centrale du vaisseau.
  }

  //fait descendre le vaisseau tout seul & test limite ecran --------------------
  if(tmp_apparition_vj <= 0 && !disparition_vj)                //on est pas dans une phase d'apparition ou disparition du vaisseau du joueur?
  {
    if(!player_pause && etat_menu == MENU_OFF && _score::getActionExecution() == 0)     //pause & menu non actifs
    {
      {                                                        //incrementation automatique de la vitesse y du vaisseau afin qu'il descende automatiquement
        if(!touch[87] && !touch[55] && !iManetteHaut && !iManetteBas)
          if(vj[J1].vitesse_y <= 0.2f)
            vj[J1].vitesse_y += 0.025f;
      }
    }
    //empeche les vaisseaux des joueurs de sortir des limites de l'ecran.
    if((short)vj[J1].spr.coor_x < 128)
    {
      vj[J1].spr.coor_x = 128.0;
      vj[J1].vitesse_x = 0.0;
    }
    if(((short)vj[J1].spr.coor_x +
        (short)vj[J1].spr.img[vj[J1].spr.img_act]->larg) > 128 + LARG_ECR_RAM)
    {
      vj[J1].spr.coor_x =
        (float)(LARG_ECR_RAM + 128.0f -
                vj[J1].spr.img[vj[J1].spr.img_act]->larg);
      vj[J1].vitesse_x = 0.0f;
    }
    if((short)vj[J1].spr.coor_y < 128)
    {
      vj[J1].spr.coor_y = 128.0;
      vj[J1].vitesse_y = 0.0;
    }
    if(((short)vj[J1].spr.coor_y +
        (short)vj[J1].spr.img[vj[J1].spr.img_act]->haut) > 128 + HAUT_ECR_RAM)
    {
      vj[J1].spr.coor_y =
        (float)(HAUT_ECR_RAM + 128.0f -
                vj[J1].spr.img[vj[J1].spr.img_act]->haut);
      vj[J1].vitesse_y = 0.0f;
    }
  }

  //Test codes touches triches (JMM) --------------------------------------------
#ifdef DEVELOPPEMENT
  touchesTriches();
#endif

	//handle weapon's player ship
#ifdef DEVELOPPEMENT
	if(!grid_editor && !curv_editor && !game_over_player_one && !player_pause
		&& etat_menu == MENU_OFF && _score::getActionExecution() == 0)
#else
	if(!game_over_player_one && !player_pause && etat_menu == MENU_OFF
		&& _score::getActionExecution() == 0)
#endif
		tirsJoueur();
	return 1;
}

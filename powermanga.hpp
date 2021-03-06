//*****************************************************************************
// copyright (c) 1998-2006 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "powermanga.hpp"
// created	: ?
// updates	: 2006-03-27
// id		: $Id: powermanga.hpp,v 1.4 2006/03/27 09:03:53 gurumeditation Exp $
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
#ifndef __POWER_MANGA__
#define __POWER_MANGA__
//..............................................................................
#ifndef SCOREFILE
#define SCOREFILE "powermanga.hiscore"
#endif
//..............................................................................
#ifndef PREFIX
#define PREFIX ""
#endif
//..............................................................................
#define SDL_TLK		//use SDL
//..............................................................................
#ifndef SDL_TLK
#define XFREE_TLK
#else
#include "SDL/SDL.h"
#endif
//..............................................................................
//#define DEVELOPPEMENT	//devel flag
#define USE_SDLMIXER		//use SDL_mixer (sounds and musics)
#define SCROLL_PRESENT		1
#define POWERMANGA_VERSION	"POWERMANGA 0.80 (2006-03-27)"
//..............................................................................
#define DEUX_PI			6.28318530718
#define PI			3.14159265359f
#define PI_SUR_2		1.57079632679f
#define PI_PLUS_PI_SUR_2	4.712388980385f
#define PI_SUR_16		0.19634954085f
#define PI_SUR_32		0.19634954085
//..............................................................................


//######################################################################
// index of the fires images in the table of the fires (fire[][])
//######################################################################
#define SP2MIJ		0	//vertical shot force 2 power 1 from spaceship
#define TIR1P1J		1	//shot force 1 power 1 from spaceship
#define TIR1P1E		2	//shot power 1 from enemies
#define TIR1P2E		3	//shot power 2 from enemies
#define TIR1P3E		4	//shot power 3 from enemies
#define SP1V1J		5	//vertical shot force 2 power 1 from spaceship
#define SP1V2J		6	//vertical shot force 2 power 2 from spaceship
#define SP1V3J		7	//vertical shot force 2 power 3 from spaceship
#define SP1V4J		8	//vertical shot force 2 power 4 from spaceship
#define SP1V5J		9	//vertical shot force 2 power 5 from spaceship
#define SP1H1J		10	//horizontal shot force 2 power 1 from spaceship
#define SP1H2J		11	//horizontal shot force 2 power 2 from spaceship
#define SP1H3J		12	//horizontal shot force 2 power 3 from spaceship
#define SP1H4J		13	//horizontal shot force 2 power 4 from spaceship
#define SP1H5J		14	//horizontal shot force 2 power 5 from spaceship
#define T2NP1J		15	//shot force 1 (2) power 1 from spaceship
#define T2NP2J		16	//shot force 1 (2) power 1 from spaceship
#define V1TN1		17	//front shot force 1 (1) from spaceship
#define V1TN2		18	//front shot force 1 (2) from spaceship
#define V1TN3		19	//front shot force 1 (3) from spaceship
#define V1TGN1		20	//left shot force 1 (1) from spaceship 1
#define V1TGN2		21	//left shot force 1 (2) from spaceship 1
#define V1TGN3		22	//left shot force 1 (3) from spaceship 1
#define V1TDN1		23	//left shot force 1 (1) from spaceship 1
#define V1TDN2		24	//left shot force 1 (2) from spaceship 1
#define V1TDN3		25	//left shot force 1 (3) from spaceship 1
#define V2TN1		26	//shot force 1 (1) power 1 from spaceship 1
#define V2TN2		27	//shot force 1 (1) power 1 from spaceship 1
#define V2TN3		28	//shot force 1 (3) power 1 from spaceship 1
#define V2TGN1		29	//left shot force 1 (1) from spaceship 1
#define V2TGN2		30	//left shot force 1 (2) from spaceship 1
#define V2TGN3		31	//left shot force 1 (3) from spaceship 1
#define V2TDN1		32	//left shot force 1 (1) from spaceship 1
#define V2TDN2		33	//left shot force 1 (2) from spaceship 1
#define V2TDN3		34	//left shot force 1 (3) from spaceship 1
#define V2THN1		35
#define V2THN2		36
#define V2THN3		37
#define V3THN1		38
#define V3THN2		39
#define V3THN3		40
#define MISSx0		41	//missile x0
#define MISSx1		42	//missile x1
#define MISSx2		43	//missile x2
#define MISSx3		44	//missile x3
#define MISSx4		45	//missile x4 from guard 6
#define MISSx5		46	//missile x5
//..............................................................................
#define V3TBA		47
#define V3TBB		48
#define V3TBC		49
#define V3TDA		50
#define V3TDB		51
#define V3TDC		52
#define V3TGA		53
#define V3TGB		54
#define V3TGC		55
#define V3TSA		56
#define V3TSB		57
#define V3TSC		58
#define V3TSD		59
#define V3TSE		60
//..............................................................................
#define V4THA		61
#define V4THB		62
#define V4THC		63
#define V4TBA		64
#define V4TBB		65
#define V4TBC		66
#define V4TDA		67
#define V4TDB		68
#define V4TDC		69
#define V4TGA		70
#define V4TGB		71
#define V4TGC		72
#define V4TSA		73
#define V4TSB		74
#define V4TSC		75
#define V4TSD		76
#define V4TSE		77
//..............................................................................
#define BIG_EXPLOSION	1
#define NORM_EXPLOSION	0
#define MINI_EXPLOSION	2
#define AMI		0
#define ENNEMI		1
#define GAUCHE		0
#define DROITE		1

//######################################################################
// size of the surface, screen, and pannels
//######################################################################
#define LARG_ECR_RAM_TOT	512	//width of the main surface
#define HAUT_ECR_RAM_TOT	440	//height of the main surface
#define LARG_ECRAN_PHY		320	//width of the window
#define HAUT_ECRAN_PHY		200	//height of the window
#define LARG_ECR_RAM		256	//width of the visible surface area
#define HAUT_ECR_RAM		184	//height of the visible surface area
#define BANDE_DE_CLIP		128	//size of clipping regions
#define DEBUT_ECRAN_X		128	//abscissa visible surface area
#define DEBUT_ECRAN_Y		128	//ordinate visible surface area
#define OFFSETHAUTGAUCHE       (DEBUT_ECRAN_Y * LARG_ECR_RAM_TOT) + DEBUT_ECRAN_X
#define OPT_LARGE		64	//width of option right side panel
#define OPT_HAUTE		184	//height of option right side panel
#define SCR_LARGE		320     //width of scores top side panel
#define SCR_HAUTE		16	//height of scores top side panel
//..............................................................................
#define LARG_GRILLE            16			          // Largeur de la grille en nombre de positions pour l'affichage des sprites enemis.
#define HAUT_GRILLE            10			          // Hauteur de la grille en nombre de positions pour l'affichage des sprites enemis.
#define X			  0                               // Indice des coordonnees X.
#define Y			  1                               // Indice des coordonnees Y.
#define L			  0                               // Indice des largeurs.
#define H			  1                               // Indice des hauteurs.
#define POS_CURVE              0			           // Indice de progression dans une courbe.
#define MAX_TYP_OF_EXPLOS      3			           // Defini le nombre maximum d'explosions differentes.
#define MAX_TYP_OF_ECLATS      5			           // Defini le nombre maximum d'eclats differents.
#define MAX_TYP_OF_METEOR      3			           // Defini le nombre maximum de metorites differentes (grosses, normales et petites).
#define NBR_IMAGES_METEOR      32			          // Defini le nombre maximum d'images pour les meteorites.
#define NBR_ANGLES             32			          // Defini le nombre d'angles differents.
//..............................................................................
#define MAX_TYP_OF_ENEMI       21			          //nombre maximum d'enemis
#define MAX_TYP_OF_S_ENEMI     21			          //nombre maximum de super-enemis
#define MAX_TYP_OF_NMI_SPE     40			          //nombre maximum d'enemis speciaux
#define MAX_TYP_OF_OBJ_SPE     8			           //nombre maximum d'objets speciaux
#define MAXTYPENMI             MAX_TYP_OF_ENEMI+MAX_TYP_OF_S_ENEMI+MAX_TYP_OF_NMI_SPE+MAX_TYP_OF_OBJ_SPE
//..............................................................................
#define MAX_TYP_OF_BONUS       6			           // Defini le nombre maximum de bonus differents.
#define MAX_TYP_OF_OPTIONS     13			          // Defini le nombre maximum d'animations pour les differentes options.
#define MAX_TYP_OF_BOULES      5			           // Defini le nombre maximum de boules differentes.
#define MAX_TYP_OF_CANONS      5			           // Defini le nombre maximum de canons differents.
#define MAX_NMI_ON_SCREEN      180			         // Defini le nombre maximum d'enemis geres au m???e instant.
#define MAX_BONUS_ON_SCREEN    20			          // Defini le nombre maximum de bonus geres au m???e instant.
#define MAX_BOULES_ON_SCREEN   20			          // Defini le nombre maximum de boules geres au m???e instant.
#define MAX_CANONS_ON_SCREEN   10			          // Defini le nombre maximum de canons geres au m???e instant.
#define MAX_PNT_OF_CURVE       2000			        // Defini le nombre maximum de points d'une courbe.
#define MAX_NBR_OF_SCR_CURV    5			           // Defini le nombre maximum de courbes simultanees  l'ecran.
#define NUM_VAI_BY_CURVE       16			          // Defini le nombre maximum de vaisseaux par courbe  l'ecran.
#define MAX_TYP_OF_TIR         78			          // Defini le nombre maximum de types de tirs differents.
#define NBR_PTS_COL_MAX        12			          // Defini le nombre de points de collision maximum.
#define NBR_PTS_TIR_MAX        12			          // Defini le nombre maximum de points de tir sur un sprite.
#define NBR_ZON_COL_MAX        6			           // Defini le nombre de zone de collision maximum.
#define NBR_IMAGES_MAX         40			          // Defini le nombre maximum d'images composant un sprite.
#define NBR_IMAGES_TIR         32			          // Defini le nombre maximum d'images pour les tirs.
#define NBR_IMAGES_EXP         32			          // Defini le nombre maximum d'images pour une explosion.
#define NBR_IMAGES_ECLATS      32			          // Defini le nombre maximum d'images pour un eclat.
#define NBR_IMAGES_S_NMI       8			           // Defini le nombre maximum d'images maximum pour les s_ennemi.
#define NBR_IMAGES_NMI_S       32			          // Defini le nombre maximum d'images maximum pour les ennemi speciaux.
#define NBR_IMAGES_BONUS       32			          // Defini le nombre maximum d'images composant un bonus.
#define NBR_IMAGES_OPTIONS     33			          // Defini le nombre maximum d'images composant une option.
#define NBR_IMAGES_BOULES      16			          // Defini le nombre maximum d'images composant une boule.
#define NBR_IMAGES_CANONS      16			          // Defini le nombre maximum d'images composant une canon.
#define NBR_MAX_VAIS_J         5			           // Defini le nombre maximum de vaisseaux par joueur.
#define NBR_MAX_IMG_VAIS_J     5			           // Defini le nombre maximum d'images de vaisseaux par joueur.
#define NBR_MAX_J              2			           // Defini le nombre maximum de joueurs.
#define TYPE_OF_STARS          4			           // Nombre de types d'etoiles different.
#define NBR_ETOILES            8			           // Nombre d'etoiles, utilise pour le chargement des fichiers graphique.
#define NBR_STARS_BY_TYPE      24			          // Nombre d'etoiles maximum  afficher par types d'etoile.
#define NBR_OF_STARS           72			          // Defini le nombre maximum d'etoiles  afficher.
#define BIGSTAR                0			           // Defini la constante utilisee pour pointer sur les grosses etoiles du starfield.
#define STAR                   1			           // Defini la constante utilisee pour pointer sur les etoiles du starfield.
#define LITSTAR                2			           // Defini la constante utilisee pour pointer sur les petites etoiles du starfield.
#define PLAYERSTAR             3			           // Defini la constante utilisee pour pointer sur les etoiles du joueur.
#define NBR_TIRS_MAX           400			         // Defini le nombre de tirs maximum.
#define NBR_FNT_SCROLL_MAX     400			         // Defini le nombre de fontes qui scrollent maximum.
#define NBR_GRID_MAX           3			           // Defini le nombre de grilles maximum.
#define NBR_CURVE_MAX          122			         // Defini le nombre maximum de courbes de bezier chargees au depart du jeu.
#define NBR_LEVEL_MAX          41			          // Defini le nombre maximum de niveaux du jeu.
#define NBR_COLOR_DEAD         11			          // Defini le nombre de couleurs utilisees lors de la disparition d'un vaisseau.
#define NBR_COLOR_ONDE_CHOC    25			          // Defini le nombre de couleurs utilisees pour l'affichage d'une onde de choc.
#define NBR_ANIM_GARDIAN       5			           // Defini le nombre d'animations maximum par gardien.
//..............................................................................
#define MENU_OFF               0
#define MENU_ON                1
#define APPARITION_MENU        2
#define DISPARITION_MENU       3
#define NBR_OPTION_MENU        4			           // Defini le nombre d'options du menu.
#define NBR_IMAGES_MENU        32			          // Defini le nombre d'images utilisees pour un sprite du menu.
#define NBR_IMAGES_LOGOTLK     64			          // Defini le nombre d'images utilisees pour le logo.
#define POS_Y_GAME             179			         // Defini la position Y d'affichage du mot GAME.
#define POS_Y_OVER             222			         // Defini la position Y d'affichage du mot OVER.
//..............................................................................
#define NBR_IMAGES_FONTE       16			          // Defini le nombre d'images utilisees pour l'affichage d'une lettre.
#define LARG_FONTE_GAME        16			          // Largeur d'une fonte pour le jeu.
#define HAUT_FONTE_GAME        16			          // Hauteur d'une fonte pour le jeu.
#define NBR_FONTES_SCORE       10			          // Defini le nombre de lettres utilisees pour l'affichage des scores pendant le jeu.
#define NBR_FONTES_GAME        90			          // Defini le nombre de lettres utilisees pour les differents affichage pendant le jeu.
#define NBR_FONTES_BIG         27			          // Defini le nombre de lettres utilisees pour l'affichage en grosse lettres pendant le jeu.
#define NBR_FONTES_SCROLL      58			          // Defini le nombre de lettres utilisees pour l'affichage avec scrolling.
#define NBR_CHIF_SCORE         8			           // Defini le nombre de lettres utilisees pour le comptage du score du joueur.
#define NBR_CHIF_GAME_OVER     4			           // Defini le nombre de lettres utilisees pour le comptage du game over.
#define NBR_CHIF_PAUSE         5			           // Defini le nombre de lettres utilisees pour le comptage de la pause.
#define TEMPS_INVINCIBLE       15			          // Defini le temps pendant lequel le joueur est invincible.
#define NBR_MAX_DEP_GARDIEN    100			         // Defini le nombre maximum de deplacements pour un gardien.
#define NBR_ONDE_CHOC_MAX      20			          // Defini le nombre maximum d'ondes de choc.
#define NBR_ANO_ONDE_CHOC      55			          // Defini le nombre maximum d'anneaux qui compose une onde de choc.
#define NBR_PTS_ONDE_CHOC      129			         // Defini le nombre maximum de points qui compose une onde de choc.
#define BONUS_INCR_BY_1        0			           // (Laurent Duperval)
#define BONUS_INCR_BY_2        1			           // (Laurent Duperval)
#define BONUS_PROT_BALL        2			           // (Laurent Duperval)
#define BONUS_INCR_ENERGY      3			           // (Laurent Duperval)
#define BONUS_SPECIAL_SHIP     4			           // (Laurent Duperval)
#define BONUS_TIMES_2          5			           // (Laurent Duperval)
//#define debug			                              // pour lancer le programme en mode DEBOGAGE
//..............................................................................
enum DEPLACEMENT
{ COURBE, GRILLE, SPECIAUX, GARDIEN };			     // Definition des constantes pour la gestion des ennemis.
enum JOUEUR
{ J1, J2, J3 };			                            // Defini les constantes designant les joueurs.
enum TYPE_VAIS_J
{ V1J, V2J, V3J, V4J, V5J };			               // Defini les constantes designant les differents types de vaisseaux par joueur.
//enemy vessels
enum TYPE_VAIS_E
{	BOONIES, MOOARCKS, ACHEES, EVILIANS, RAGOONS,   FLABBIES, BOWANS,
 	HIJACKERS, DISTASIANS, BRITTERS,   SICKIES, ARIANS, STINCKIES, BZEURCKS, WEAKIES,
 	MASKIES, SCRAPIES, SCROLLIANS, SHAMISTEES, FOOLINIANS, GLAMOOSH,
 	BIGBOONIES, BIGMOOARCKS, BIGACHEES, BIGEVILIANS, BIGRAGOONS,
	BIGFLABBIES, BIGBOWANS, BIGHIJACKERS, BIGDISTASIANS, BIGBRITTERS,
	BIGSICKIES, BIGARIANS, BIGSTINCKIES, BIGBZEURCKS, BIGWEAKIES,
	BIGMASKIES, BIGSCRAPIES, BIGSCROLLIANS, BIGSHAMISTEES, BIGFOOLINIANS,
	BIGGLAMOOSH,
	//special vessels
	SUBJUGANEERS, MILLOUZ, SWORDINIANS, TOUBOUG, DISGOOSTEES, EARTHINIANS,
	BIRIANSTEES, BELCHOUTIES, VIONIEES, HOCKYS, TODHAIRIES, DEFECTINIANS,
	BLAVIRTHE, SOONIEES, ANGOUFF, GAFFIES, BITTERIANS, BLEUERCKS, ARCHINIANS,
	CLOWNIES, DEMONIANS, TOUTIES, FIDGETINIANS, EFFIES, DIMITINIANS,
	PAINIANS, ENSLAVEERS, FEABILIANS, DIVERTIZERS, SAPOUCH, HORRIBIANS,
	CARRYONIANS, DEVILIANS, ROUGHLEERS, ABASCUSIANS, ROTIES, STENCHIES,
	PERTURBIANS, MADIRIANS, BAINIES,
	//special
	SHURIKY, NAGGYS, GOZUKY, SOUKEE, QUIBOULY, TOURNADEE, SAAKEY, SAAKAMIN,
	BIGMETEOR, NORMALMETEOR, SMALLMETEOR, ETB_SHURIKY,
	//guards
	THANIKEE, BARYBOOG, PIKKIOU, NEGDEIS, FLASHY, MEECKY, TYPYBOON, MATHYDEE,
	OVYDOON, GATLEENY, NAUTEE, KAMEAMEA, SUPRALIS, GHOTTEN
};
enum COULEUR
{	NOIR, GRIS_FONCE, GRIS, GRIS_CLAIR, BLANC, ROUGE, VERT, JAUNE, BLEUE, ROUGE_FONCE
};
//..............................................................................
typedef struct pnt2D
{	Sint16		x;
	Sint16		y;
} pnt2D;
//..............................................................................
typedef struct Eclair
{	Sint32		sx, sy;
	Sint32		dx, dy;
	Sint32		col1, col2;
	Sint32		r1, r2, r3;
} Eclair;
//..............................................................................
typedef struct poly
{	char		taille;
	char		zero;
	unsigned char	coul;
	char		nb_pnt;
	pnt2D		P[5];
	Sint32		fin;
} poly;
// circular shock wave structure from purple gem (onde.cpp) ....................
typedef struct onde_de_choc
{	Sint32		cmpt_cercle;		//circular shock wave number
	Sint32		next_object;		//previous element
	Sint32		prev_object;		//next element
	Sint32		orig_x, orig_y;		//circular shock wave center
	Sint32		cmpt_color_aff;		//color palette index
} onde_de_choc;
//..............................................................................
/*
typedef struct pcxheader
{
	unsigned char	griffe;
	unsigned char	version;
	unsigned char	codage;
	unsigned char	bits_par_pxl;
	Uint16		x1;
	Uint16		y1;
	Uint16		x2;
	Uint16		y2;
	Uint16		hres;
	Uint16		vres;
	unsigned char	pal[48];
	char		bidon;
	char		nbr_planes;
	Uint16		bytes_par_lgn;
	Uint16		type_pal;
	Uint16		shres;
	Uint16		svres;
	char		rabdeplace[54];
} pcxheader;
*/
//use for fontes, TLK's logo, main menu text, scrolltext, options ..............
typedef struct fonte
{
	Sint32		nbr_pts_img;	//number of pixels
	char*		img;		//memory address
	Sint32		nbr_data_comp;	//table compression size
	char*		compress;	//table compression data (repeat values)
} fonte;
//use for fontes animated: score, level number (displayFonte.cpp)...............
typedef struct play_score
{
	Sint32		old_val_font;	//fonte's number
	Sint32		old_img_font;
	bool		anim_font;	//1=fonte is animated
	Sint16		img_act;	//number of animations
	Sint16		nbr_tot_vbl;	//tempo
	Sint16		add_vbl;
	Sint16		nbr_vbl_act;	//tempo's counter
}
play_score;
// right side panel: options (options.cpp, bonus.cpp) ...........................
typedef struct option
{
	Sint32		num_option;
	bool		anim_open;
	bool		anim_close;
	bool		close_option;
	Sint16		img_act;	//current image number
	Sint16		nbr_tot_vbl;	//tempo before next image
	Sint16		add_vbl;	//additional tempo before next image
	Sint16		nbr_vbl_act;	//current tempo counter
}
option;
// use for all sprites .........................................................
typedef struct image
{
	Sint16 		x_cg, y_cg;	//centre of gravity of the image
	Sint16		larg, haut;	//image width and height

	//list of the collision coordinates
	Sint16		nbr_pts_col;	//number of collision coordinates
	Sint16		xy_pts_col[NBR_PTS_COL_MAX][2];

	//lists of the areas coordinates and areas sizes (width/height)
	Sint16		nbr_zon_col;	//number of collision areas
	Sint16		xy_zon_col[NBR_ZON_COL_MAX][2];
	Sint16		lh_zon_col[NBR_ZON_COL_MAX][2];

	//lists of tbe cannons coordinates and firing angles
	Sint16		nbr_pts_tir;	//number of cannons coordinates
	Sint16		xy_pos_tir[NBR_PTS_TIR_MAX][2];
	Sint16		angle_tir[NBR_PTS_TIR_MAX];

	//image data
	Sint32		nbr_pts_img;	//image number of pixels
	char*		img;		//image address
	Sint32		nbr_data_comp;	//size of compress table
	char*		compress;	//compress data
}
image;
//bonus structure (bonus.cpp) --------------------------------------------------
typedef struct str_bonus
{
	Sint32		genre;		//type of bonus
	Sint16		trajectoire;	//0=linear trajectory
	Sint16		nbr_img;	//number of images in the sprite
	Sint16		img_act;	//current image index
	Sint16		nbr_tot_vbl;	//tempo before next image
	Sint16		nbr_vbl_act;	//tempo's counter
	//structures of images bonuses
	image*		img[NBR_IMAGES_MAX];
	float		coor_x;	        //abscissa
	float		coor_y;	        //ordinate
	float		vitesse;	//speed of the sprite
}
str_bonus;
// structure of the orbital satellite protections ------------------------------
typedef struct str_boule
{
	bool		visible;	//1=display satellite
	bool		affich_white;	//1=display satellite's white mask
	Sint32		next_object;
	Sint32		prev_object;
	Sint16		app_vaisseau;	//*not used*
	Sint16		nbr_pts_cercle;	//number of points of the circle
	Sint16		pos_pts_cercle;	//index area on the circle table
	Sint16		pow_of_dest;	//power of destruction (collision)
	Sint16		etat_dommages;	//damage status <= 0 then then destroyed
	Sint16		nbr_img;	//number of images of the sprite
	Sint16		img_act;	//current image index
	Sint16		nbr_tot_vbl;	//tempo before next image
	Sint16		nbr_vbl_act;	//tempo's counter
	//structures of images satellites
	image*		img[NBR_IMAGES_MAX];
	Sint32		coor_x;		//abscissa
	Sint32		coor_y;		//ordinate
	Sint32		cad_tir;
	Sint32		cad_tir_initiale;
}
str_boule;
//side extra gun ---------------------------------------------------------------
typedef struct str_canon
{
	bool		visible;	//1=display extra gun
	bool		affich_white;	//1=display extra gun's white mask
	Sint32		next_object;
	Sint32		prev_object;
	Sint16		app_vaisseau;	//*not used*
	Sint16		pow_of_dest;	//power of destruction (collision)
	Sint16		etat_dommages;	//damage status  (<= 0 destroyed gun)
	Sint16		nbr_img;	//number of images of the sprite
	Sint16		img_act;	//current image index
	Sint16		nbr_tot_vbl;	//tempo before next image
	Sint16		nbr_vbl_act;	//tempo's counter
	//structures of images extra gun
	image*		img[NBR_IMAGES_MAX];
	Sint32          coor_x;         //abscissa
	Sint32          coor_y;         //ordinate
	Sint32		cad_tir;	//shot tempo counter
	Sint32		cad_tir_initiale;
	Sint32		position;	//0=right side / 1=left side
}
str_canon;
//------------------------------------------------------------------------------
typedef struct sprite
{
	bool		genre;		//0=friend sprite / 1=ennemy sprite
	Sint16		trajectoire;	//trajectory: 0=linear/1=compute/2=curve
	Sint16		pow_of_dest;	//power of destruction (in collision)
	Sint16		etat_dommages;	//damage status  (<= 0 destroyed sprite)
	Sint16		etat_max;	//damage initiale
/*
	Sint16		pilote;		// *not used* (0=computer control)
	Sint16		mode_commande;	// *not used* (0=key/1=mouse/2=joy)
*/
	Sint16		nbr_img;	//number of images of the sprite
	Sint16		img_act;	//current image index
	Sint16		nbr_tot_vbl;	//tempo before next image
	Sint16		nbr_vbl_act;	//tempo's counter
	//structures of the sprite images
	image*		img[NBR_IMAGES_MAX];
	float		coor_x;		//abscissa
	float		coor_y;		//ordinate
	float		vitesse;	//speed
} sprite;
// star background structure (afficheEtoiles.cpp) ------------------------------
typedef struct star
{
	image*		img;		//structures of the star images
	float		coor_x, coor_y;	//abscissa and ordinate
	float		vitesse;	//speed
	Sint32		type;
	Sint32		nbr_tot_vbl;	//tempo before next image
	Sint32		nbr_vbl_act;	//tempo's counter
} star;
//------------------------------------------------------------------------------
typedef struct explosion
{
	Sint16		img_act;	//current image index
	Sint16		nbr_tot_vbl;	//tempo before next image
	Sint16		nbr_vbl_act;	//tempo's counter
	Sint16		img_angle;
	//structures of the explosions images
	image*		img[NBR_IMAGES_EXP];
	float		coor_x, coor_y;	///abscissa and ordinate
	float		vitesse;	//speed
	Sint32		next_object;
	Sint32		prev_object;
	Sint32		nbr_vbl_av_exp;	//time before start
} explosion;
//------------------------------------------------------------------------------
typedef struct tir
{
	sprite		spr;		//sprite structure
	Sint16		pos_vaiss[2];	//index of progression
	Sint16		num_courbe;	//curve number used
	Sint16		tmp_life_tir;	//time of life
	float		angle_tir;	//angle of the projectile
	float		agilite;	//rotation speed
	Sint16		img_angle;
	Sint16		img_old_angle;
	Sint32		next_object;
	Sint32		prev_object;
	bool		clignote;	//flicker the sprite
} tir;
//------------------------------------------------------------------------------
typedef struct vaisseau_nmi
{
	sprite		spr;		//sprite structure
	bool		dead;		//1=ennemy's dead
	bool		visible_dead;	//enemy is still visible but inactive
	Sint16		cmpt_color_dead;//mask color
	bool		affich_white;	//1=display ennemy's white mask
	bool		visible;	//1=display spaceship
	Sint16		invincible;	//time during which vessel's invincible
	bool		retournement;
	bool		change_dir;                          // Vaut 1 si le vaisseau ennemi  change de direction, sinon 0.
	Sint32		 cad_tir;                             // Variable utilisee pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer  nouveau.
	Sint32		 cad_tir_initiale;                    // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer  nouveau.
	Sint16		deplacement;	//0:curve/1=grid/2=guard
	Sint16		pos_vaiss[2];	//curve or grid index
	Sint16		num_courbe;	//curve number used
	Sint32		next_object;
	Sint32		prev_object;
	Sint32		type;		//0=16*16/1=32*32/2=special
	Sint32		sens_anim;	//anim index offset (0=increase/1=decreasing)
	float		vitesse_x;	//horizontal speed
	float		vitesse_y;	//vertical speed
	Sint32		temps_apparition;                    // Utilisee par les vaisseaux speciaux pour savoir si l'on peux supprimer ou non le sprite.
	float		angle_tir;			       // Angle de deplacement du vaisseau pour les SOUKEE par exemple.
	Sint16		img_angle;
	Sint16		img_old_angle;
	float		agilite;                            // Permet de prendre les courbes plus ou moins rapidement.
} vaisseau_nmi;
//------------------------------------------------------------------------------
typedef struct grid
{
	Sint16		nbr_vaisseaux;			   // Donne le nombre de vaisseaux de la grille.
	bool		activity;                            // Indique si on doit faire la gestion de la grille.
	bool		apparition;                          // Indique si la grille est en phase d'appariton ou pas.
	Sint32		sens_dep;                            // Indique si la grille se deplace  gauche ou bien  droite.
	Sint16		nom_vaisseau[LARG_GRILLE][HAUT_GRILLE];       // Numero du vaisseau ennemi dans la grille.
	Sint16		freq_tir[LARG_GRILLE][HAUT_GRILLE];  // VBLs corespondant au temps qu'il faut au vaisseau avant de pouvoir tirer  nouveau.
	float		coor_x, coor_y;
	float		vit_dep_x;
	float		vit_dep_y;
} grid;
//------------------------------------------------------------------------------
typedef struct curve
{
	Sint16		nbr_pnt_curve;	//number of points on the curve
	Sint16		pos_x, pos_y;	//curve start coordinate into screen
	//x offsets
	signed char	delta_x[MAX_PNT_OF_CURVE];
	//y offsets
	signed char	delta_y[MAX_PNT_OF_CURVE];
	//vessel angles
	signed char	angle[MAX_PNT_OF_CURVE];
} curve;

//"curve_level" structure: curve phase -----------------------------------------
//(5 courbes maxium, 16 vaisseaux par courbes maximum)
typedef struct curve_level
{
	bool		activity;	//1=curve phase in progress
	Sint16		num_courbe_act;	//number of the current curve
	//number of current vessels (16 maximum)
	Sint16		nbr_vaisseaux_act[MAX_NBR_OF_SCR_CURV];
	Sint16		nbr_vbl_app_act[MAX_NBR_OF_SCR_CURV];//nombre de boucles comptees entre l'apparition de chaque vaisseau
	Sint16		nbr_tot_courbes;			 //nombre total de courbes
	Sint16		num_courbe[MAX_NBR_OF_SCR_CURV];     //tableau des differents numeros de courbes.
	Sint16		nbr_vaisseaux_tot[MAX_NBR_OF_SCR_CURV];       //nombre de vaisseaux  sur chaque courbes
	Sint16		nbr_vbl_apparition[MAX_NBR_OF_SCR_CURV];      //nombre de boucles entre l'apparition de chaque vaisseaux sur chaque courbe
	Sint16		num_vaisseau[MAX_NBR_OF_SCR_CURV][NUM_VAI_BY_CURVE];  //numeros des vaisseaux sur toutes les courbes
	Sint16		freq_tir[MAX_NBR_OF_SCR_CURV][NUM_VAI_BY_CURVE];      //delai entre les tirs des vaisseaux
} curve_level;

// structure of spaceship's player ---------------------------------------------
typedef struct vaisseau
{
	sprite		spr;		//structure of the sprite
	float		vitesse_x;	//horizontal speed
	float		vitesse_y;	//vertical speed
	bool		affich_white;	//1=display spaceship's white mask
	bool		visible;	//1=display spaceship
	Sint16		invincible;	//invinsibility time
	Sint16		vbl_invincible;	//invinsibility counter time
	Sint16		nbr_bonus;	//number of bonuses collected 0 to 11
	Sint16		nbr_boules;	//number of satellite protections 0 to 5
	Sint16		nbr_canons;	//number of extra gun 0 to 2
	Sint16		type_vaisseau;	//type of vessels  1 to 1O
	Sint32		score;        	//player's score
	Sint32		cad_tir;	//time before 2 shots
	Sint32		tir_spec;	//shot force 2 tempo counter
	Sint32		cad_tir_spec;	//shot force 2 tempo counter
/*
	Sint32		freeze_tmps;
*/
	//level of options, 0 (none) to 10 (maxi)
	Sint16		vit_dep_vaisseau;	//booster (0 = freezed)
	Sint16		power_avant;		//front shot force 1
	Sint16		power_arriere;		//rear shot force 1
	Sint16		power_droite;		//right shot force 1
	Sint16		power_gauche;		//left shot force 1
	Sint16		power_spec_avant;	//front shot force 2
	Sint16		power_spec_arriere;	//rear shot force 2
	Sint16		power_spec_droite;	//right shot force 2
	Sint16		power_spec_gauche;	//left shot force 2

/*
	//determine selected option in the right side panel
	bool		option_vitesse;		//booster selected
	bool		option_avant;		//front shot force 1 selected
	bool		option_arriere;		//rear shot force 1 selected
	bool		option_droite;		//right shot force 1 selected
	bool		option_gauche;		//left shot force 1 selected
	bool		option_spec_avant;	//front shot force 2 selected
	bool		option_spec_arriere; 	//rear shot force 2 selected
	bool		option_spec_droite;   	//left shot force 2 selected
	bool		option_spec_gauche;	//right shot force 2 selected
	bool		option_type_vaisseau;	//change of spaceship selected
*/

	bool		metamorphose;		//transformation of the vessel
} vaisseau;
// scrolltext of the menu (fntscrol.cpp) ---------------------------------------
typedef struct fntscroll
{
	sprite		spr;		//sprite structure
	Sint16		pos_fonte[2];	//curve index
	Sint16		num_courbe;	//curve number
	Sint16		num_image;	//image number
	Sint16		typ_of_scroll;	//type of scrolltext
	Sint32		next_object;
	Sint32		prev_object;
	bool		clignote;	//flicker the sprite
	bool		dir_x;		//1=x displacement
	bool		dir_y;		//1=y displacement
	Sint32		coor_x, coor_y;
	Sint32		vitesse;
} fntscroll;

//inits_game.cpp -----------------------------------------------
Sint32			inits_game();                          //initialisations unique a la creation de notre application
Sint32			etb_read_image(char *_pNomFichier, image * _pImage, Uint32 _iNombreType, Uint32 _iNombreImage);
void                   etb_affiche_images();
void                   setclav(void);
void                   resetclav(void);
void                   setint21(void);
void                   resetint21(void);
void                   Add_Ve_Special(Sint32 val_ve_special);    // Routine qui ajoute un vaisseau special  la liste des vaisseau ennemis.
void                   Gene_Bonus_Malus(vaisseau_nmi * pve);  //genere un bonus
void                   Gene_Bonus_Malus_Meteor(vaisseau_nmi * pve);    //genere un bonus ou un malus
float                  Calc_Angle_Tir(Sint16 pxs, Sint16 pys, Sint16 pxd, Sint16 pyd);     // Calcul un angle en fonction de deux points.
float                  Calcul_New_Angle(float old_angle, float new_angle, float agilite);
Sint16                  sign(float);			       // Retourne -1, 0 ou 1 suivant la valeur passee en argument.
void                   caraxy(unsigned char caract, Sint16 x, Sint16 y, unsigned char color, Sint16 fond, char *adr, Sint32 lar_buf);
void                   textxy(char *chaine, Sint16 x, Sint16 y, unsigned char color, Sint16 fond, char *adr, Sint32 lar_buf);
unsigned char          recherche_couleur(unsigned char r, unsigned char g, unsigned char b);
void                   draw_carre(char *screen, Sint16 x, Sint16 y, Sint16 l, Sint16 h, unsigned char coul);
Sint32			Load_Curve_Level(int);                 // Chargement d'un fichier courbe.
void                   Find_Tot_Nbr_Vais_Of_Curve(void);      // Recherche du nombre total de vaisseaux de la courbe.
void                   Active_Curve_Level(void);              // Activation d'un fichier courbe.
void                   read_sprites(char *fichier, Sint32 handle, image & rd_img);        // Lecture du fichier des petits enemis en 16x16.
void                   Add_Serie_Of_Explosion(Sint32 num_nmi);   // Routine utilisee pour ajouter une serie d'explosion  un gros vaisseau.
bool                   Anim_Player_Level(void);               // Test si l'affichage du niveau du joueur est dans une phase d'animation.
void                   Clear_Keyboard(void);                  // Routine utilisee afin de mettre a zero toutes les touches du clavier.
void                   Init_Val_Player_One(void);             // Routine utilisee afin de mettre tous les parametres du joueur  zero.
Sint32			Load_Gardien(int);                     // Routine permettant le chargement d'un gardien.
void                   Apparition_Vaisseau(void);             // Routine permettant l'apparition a l'ecan du vaisseau appartenant au joueur.
void                   Init_Tab_Ve(void);			 // Initialisation du tableau 've'.
void                   Init_Tab_Tirs(void);                   // Initialisation du tableau 'tirs'.
void                   Init_Tab_Boules(void);                 // Initialisation du tableau 'bou'.
void                   Init_Tab_Canons(void);                 // Initialisation du tableau 'can'.
void                   Aff_Options(int);			  // Routine permettant l'affichage des options.
void                   Clear_Option(int);			 // Routine permettant l'effacement d'une option.
void                   Play_Anim_Close_Option(int);           // Routine qui affiche l'animation de fermeture d'une option du tableau de bord.
void                   Play_Anim_Open_Option(int);            // Routine qui affiche l'animation d'ouverture d'une option du tableau de bord.
void                   Init_Anim_Option(Sint32, bool);           // Routine utilisee pour ouvrir ou fermer une option.
void                   Aff_Cadre_Selection(int);              // Affichage du cadre de selection sur l'option selectionnee.
void                   Gere_gardien(int);			 // Gestion des gardiens de fin de niveaux.
bool                   Gere_Invincibilite_Ve(int);            // Gestion de l'invincibilite d'un vaisseau ennemi.
void                   Aff_Ve(Sint32 nmi_numero);                //afficher un vaisseau ennemi.
void                   Aff_Ve2(Sint32 nmi_numero);               //affiche le vaisseau du gardien
void                   Test_Col_Bou_With_Nmi(int);            // Test les collisions entre les boules du joueur et le vaisseau ennemi.
void                   Test_Col_Can_With_Nmi(int);            // Test les collisions entre les canons du joueur et le vaisseau ennemi.
void                   Test_Col_Player_With_Spec_Nmi(int);    // Test des collisions entre le joueur et l'ennemi special.
void                   Clignote_Nmi_Avant_Disparition(int);   // Routine qui indique au vaisseau ennemi de clignoter avant de disparaitre.
void                   Fill_Struct_Player_One(void);          //Routine qui rempli la structure du joueur.

//######################################################################
// guards
//######################################################################
//next level when there are no guards
Sint32			Meteor_Activity_Less_Guardians();
//next level when there are guards
void			Meteor_Activity_With_Guardian(int);
void                   Fill_Tab_Dep_Gard_Droite(void);        // Routine qui rempli le tableau des deplacements du gardien de manire  le faire se deplacer vers la droite.
void                   Fill_Tab_Dep_Gard_Gauche(void);        // Routine qui rempli le tableau des deplacements du gardien de manire  le faire se deplacer vers la gauche.
void                   Fill_Tab_Dep_Gard_Haut(void);          // Routine qui rempli le tableau des deplacements du gardien de manire  le faire se deplacer vers le haut.
void                   Fill_Tab_Dep_Gard_Bas(void);           // Routine qui rempli le tableau des deplacements du gardien de manire  le faire se deplacer vers le bas.
void                   Free_Mem_Gardian(void);                // Liberation de la memoire utilisee par les gardiens.
void                   Fait_Apparaitre_Gardien_Haut(int);     // Routine utilisee pour faire l'apparition d'un gardien par le haut de l'ecran.
void                   Move_Gardian_Line(int);                // Routine utilisee pour deplacer un gardien en suivant une droite.
void                   Move_Gardian_Sin(int);                 // Routine utilisee pour deplacer un gardien selon un sinus
Sint32			gardien_testFin();
void                   gardienBarreNRJ();
void                   _init_gardien12();
void                   _init_gardien13();
void                   _init_gardien14();
void                   _execute_gardien12(int);
void                   _execute_gardien13(int);
void                   _execute_gardien14(int);

// "menu.cpp" ..................................................................
Sint32			 Gestion_Menu();                        //main menu "menu.cpp"
void			Affich_Menu(Sint32, int);                 //display menu text
// Routines se trouvant  l'origine dans des fichiers include.
#ifdef DEVELOPPEMENT
void			Grid_Edit(void);                       //editeur des "grilles"
#endif
Sint32			 LoadGrid(Sint32 num_grid);                // Routine permettant le chargement d'une grille.
void			Active_Grid(void);                     // Activation de la grille chargee precedement.
Sint32			 Load_Meteor(int);                      // Chargement d'un groupe de meteorites bien precis.
void			Add_Explosion(float, float, float, Sint32, int);   // Utiliser pour ajouter une explosion  la liste.
void			Add_Explosion_Guard (float c_x, float c_y);
void			Add_Eclats(float, float, float, Sint32, Sint32, short);       // Utiliser pour ajouter des eclats lors d'une explosion.

//routines utilisees pour la gestion des listes 'tirs'.
Sint32			 new_element_tir();                     //ajoute un tir (avec un son)
Sint32			 New_Element_Tir();                     //ajoute un tir (sans son)
void			Del_Element_Tir(Sint32 n, Sint32 o);         //Efface le nime element de la liste 'tirs' et fait les mises  jour necessaire.

// Routines utilisees pour la gestion des listes 've'.
Sint32			 New_Element_Ve(void);                  // Retourne l'indice dans le tableau 've' d'un nouvel element de la liste.
void			Del_Element_Ve(Sint32 n, Sint32 o);          // Efface le nime element de la liste 've' et fait les mises  jour necessaire.

// Routines utilisees pour la gestion des listes 'xpl'.
Sint32			 New_Element_Exp(void);                 // Retourne l'indice dans le tableau 'xpl' d'un nouvel element de la liste.
void			Del_Element_Exp(Sint32 n, Sint32 o);         // Efface le nime element de la liste 'xpl' et fait les mises  jour necessaire.

// Routines utilisees pour la gestion des listes 'bou'.
Sint32			 New_Element_Bou(void);                 // Retourne l'indice dans le tableau 'bou' d'un nouvel element de la liste.
void			Del_Element_Bou(Sint32 n, Sint32 o);         // Efface le nime element de la liste 'bou' et fait les mises  jour necessaire.

// Routines utilisees pour la gestion des listes 'can'.
Sint32			 New_Element_Can(void);                 // Retourne l'indice dans le tableau 'can' d'un nouvel element de la liste.
void			Del_Element_Can(Sint32 n, Sint32 o);         // Efface le nime element de la liste 'can' et fait les mises  jour necessaire.

//######################################################################
// spaceship 1: add shot force 1 (powmangasub2.cpp)
//######################################################################
//front shot force 1 (power 1 to 5)
void			V1_Tir_Normal_Avant_P1(void);
void			V1_Tir_Normal_Avant_P2(void);
void			V1_Tir_Normal_Avant_P3(void);
void			V1_Tir_Normal_Avant_P4(void);
void			V1_Tir_Normal_Avant_P5(void);
void			V1_Tir_Normal_Arriere_P1(void);
void			V1_Tir_Normal_Arriere_P2(void);
void			V1_Tir_Normal_Arriere_P3(void);
void			V1_Tir_Normal_Arriere_P4(void);
void			V1_Tir_Normal_Arriere_P5(void);
void			V1_Tir_Normal_Droite_P1(void);
void			V1_Tir_Normal_Droite_P2(void);
void			V1_Tir_Normal_Droite_P3(void);
void			V1_Tir_Normal_Droite_P4(void);
void			V1_Tir_Normal_Droite_P5(void);
void			V1_Tir_Normal_Gauche_P1(void);
void			V1_Tir_Normal_Gauche_P2(void);
void			V1_Tir_Normal_Gauche_P3(void);
void			V1_Tir_Normal_Gauche_P4(void);
void			V1_Tir_Normal_Gauche_P5(void);

//######################################################################
// spaceship 1: add shot force 2
//######################################################################
//front shot force 2 (power 1 to 5)
void			V1_Tir_Special_Avant_P1(void);
void			V1_Tir_Special_Avant_P2(void);
void			V1_Tir_Special_Avant_P3(void);
void			V1_Tir_Special_Avant_P4(void);
void			V1_Tir_Special_Avant_P5(void);
void			V1_Tir_Special_Arriere_P1(void);
void			V1_Tir_Special_Arriere_P2(void);
void			V1_Tir_Special_Arriere_P3(void);
void			V1_Tir_Special_Arriere_P4(void);
void			V1_Tir_Special_Arriere_P5(void);
void			V1_Tir_Special_Droite_P1(void);
void			V1_Tir_Special_Droite_P2(void);
void			V1_Tir_Special_Droite_P3(void);
void			V1_Tir_Special_Droite_P4(void);
void			V1_Tir_Special_Droite_P5(void);
void			V1_Tir_Special_Gauche_P1(void);
void			V1_Tir_Special_Gauche_P2(void);
void			V1_Tir_Special_Gauche_P3(void);
void			V1_Tir_Special_Gauche_P4(void);
void			V1_Tir_Special_Gauche_P5(void);


//######################################################################
// spaceship 2: add shot force 1
//######################################################################
//front shot force 1 (power 1 to 5)
void			V2_Tir_Normal_Avant_P1(void);
void			V2_Tir_Normal_Avant_P2(int);
void			V2_Tir_Normal_Avant_P3(void);
void			V2_Tir_Normal_Avant_P4(int);
void			V2_Tir_Normal_Avant_P5(void);
void			V2_Tir_Normal_Arriere_P1(void);
void			V2_Tir_Normal_Arriere_P2(int);
void			V2_Tir_Normal_Arriere_P3(void);
void			V2_Tir_Normal_Arriere_P4(int);
void			V2_Tir_Normal_Arriere_P5(void);
void			V2_Tir_Normal_Droite_P1(void);
void			V2_Tir_Normal_Droite_P2(int);
void			V2_Tir_Normal_Droite_P3(void);
void			V2_Tir_Normal_Droite_P4(int);
void			V2_Tir_Normal_Droite_P5(void);
void			V2_Tir_Normal_Gauche_P1(void);
void			V2_Tir_Normal_Gauche_P2(int);
void			V2_Tir_Normal_Gauche_P3(void);
void			V2_Tir_Normal_Gauche_P4(int);
void			V2_Tir_Normal_Gauche_P5(void);


//######################################################################
// spaceship 2: add shot force 1
//######################################################################
//front shot force 2 (power 1 to 5)
void			V2_Tir_Special_Avant_P1(void);
void			V2_Tir_Special_Avant_P2(void);
void			V2_Tir_Special_Avant_P3(void);
void			V2_Tir_Special_Avant_P4(void);
void			V2_Tir_Special_Avant_P5(void);
void			V2_Tir_Special_Arriere_P1(void);
void			V2_Tir_Special_Arriere_P2(void);
void			V2_Tir_Special_Arriere_P3(void);
void			V2_Tir_Special_Arriere_P4(void);
void			V2_Tir_Special_Arriere_P5(void);
void			V2_Tir_Special_Droite_P1(void);
void			V2_Tir_Special_Droite_P2(void);
void			V2_Tir_Special_Droite_P3(void);
void			V2_Tir_Special_Droite_P4(void);
void			V2_Tir_Special_Droite_P5(void);
void			V2_Tir_Special_Gauche_P1(void);
void			V2_Tir_Special_Gauche_P2(void);
void			V2_Tir_Special_Gauche_P3(void);
void			V2_Tir_Special_Gauche_P4(void);
void			V2_Tir_Special_Gauche_P5(void);


//######################################################################
// spaceship 3: add shot force 1
//######################################################################
//front shot force 1 (power 1 to 5)
void			V3_Tir_Normal_Avant_P1(int);
void			V3_Tir_Normal_Avant_P2(int);
void			V3_Tir_Normal_Avant_P3(void);
void			V3_Tir_Normal_Avant_P4(int);
void			V3_Tir_Normal_Avant_P5(void);
//rear shot force 1 (power 1 to 5)
void			V3_Tir_Normal_Arriere_P1(int);
void			V3_Tir_Normal_Arriere_P2(int);
void			V3_Tir_Normal_Arriere_P3(void);
void			V3_Tir_Normal_Arriere_P4(int);
void			V3_Tir_Normal_Arriere_P5(void);
//right shot force 1 (power 1 to 5)
void			V3_Tir_Normal_Droite_P1(int);
void			V3_Tir_Normal_Droite_P2(int);
void			V3_Tir_Normal_Droite_P3(void);
void			V3_Tir_Normal_Droite_P4(int);
void			V3_Tir_Normal_Droite_P5(void);
//left shot force 1 (power 1 to 5)
void			V3_Tir_Normal_Gauche_P1(int);
void			V3_Tir_Normal_Gauche_P2(int);
void			V3_Tir_Normal_Gauche_P3(void);
void			V3_Tir_Normal_Gauche_P4(int);
void			V3_Tir_Normal_Gauche_P5(void);

//######################################################################
// spaceship 3: add shot force 2
//######################################################################
//front shot force 2 (power 1 to 5)
void			V3_Tir_Special_Avant_P1(void);
void			V3_Tir_Special_Avant_P2(void);
void			V3_Tir_Special_Avant_P3(void);
void			V3_Tir_Special_Avant_P4(void);
void			V3_Tir_Special_Avant_P5(void);
//rear shot force 2 (power 1 to 5)
void			V3_Tir_Special_Arriere_P1(void);
void			V3_Tir_Special_Arriere_P2(void);
void			V3_Tir_Special_Arriere_P3(void);
void			V3_Tir_Special_Arriere_P4(void);
void			V3_Tir_Special_Arriere_P5(void);
//right shot force 2 (power 1 to 5)
void			V3_Tir_Special_Droite_P1(void);
void			V3_Tir_Special_Droite_P2(void);
void			V3_Tir_Special_Droite_P3(void);
void			V3_Tir_Special_Droite_P4(void);
void			V3_Tir_Special_Droite_P5(void);
void			V4_Tir_Normal_Droite_P5(void);
//left shot force 2 (power 1 to 5)
void			V3_Tir_Special_Gauche_P1(void);
void			V3_Tir_Special_Gauche_P2(void);
void			V3_Tir_Special_Gauche_P3(void);
void			V3_Tir_Special_Gauche_P4(void);
void			V3_Tir_Special_Gauche_P5(void);

//######################################################################
// spaceship 4: add shot force 1
//######################################################################
//front shot force 1 (power 1 to 5)
void			V4_Tir_Normal_Avant_P1(int);
void			V4_Tir_Normal_Avant_P2(int);
void			V4_Tir_Normal_Avant_P3(void);
void			V4_Tir_Normal_Avant_P4(int);
void			V4_Tir_Normal_Avant_P5(void);
//rear shot force 1 (power 1 to 5)
void			V4_Tir_Normal_Arriere_P1(int);
void			V4_Tir_Normal_Arriere_P2(int);
void			V4_Tir_Normal_Arriere_P3(void);
void			V4_Tir_Normal_Arriere_P4(int);
void			V4_Tir_Normal_Arriere_P5(void);
//right shot force 1 (power 1 to 5)
void			V4_Tir_Normal_Droite_P1(int);
void			V4_Tir_Normal_Droite_P2(int);
void			V4_Tir_Normal_Droite_P3(void);
void			V4_Tir_Normal_Droite_P4(int);
void			V4_Tir_Normal_Droite_P5(void);
//left shot force 1 (power 1 to 5)
void			V4_Tir_Normal_Gauche_P1(int);
void			V4_Tir_Normal_Gauche_P2(int);
void			V4_Tir_Normal_Gauche_P3(void);
void			V4_Tir_Normal_Gauche_P4(int);
void			V4_Tir_Normal_Gauche_P5(void);

//######################################################################
// spaceship 4: add shot force 2
//######################################################################
//front shot force 2 (power 1 to 5)
void			V4_Tir_Special_Avant_P1(void);
void			V4_Tir_Special_Avant_P2(void);
void			V4_Tir_Special_Avant_P3(void);
void			V4_Tir_Special_Avant_P4(void);
void			V4_Tir_Special_Avant_P5(void);
//rear shot force 2 (power 1 to 5)
void			V4_Tir_Special_Arriere_P1(void);
void			V4_Tir_Special_Arriere_P2(void);
void			V4_Tir_Special_Arriere_P3(void);
void			V4_Tir_Special_Arriere_P4(void);
void			V4_Tir_Special_Arriere_P5(void);
//right shot force 2 (power 1 to 5)
void			V4_Tir_Special_Droite_P1(void);
void			V4_Tir_Special_Droite_P2(void);
void			V4_Tir_Special_Droite_P3(void);
void			V4_Tir_Special_Droite_P4(void);
void			V4_Tir_Special_Droite_P5(void);
//left shot force 2 (power 1 to 5)
void			V4_Tir_Special_Gauche_P1(void);
void			V4_Tir_Special_Gauche_P2(void);
void			V4_Tir_Special_Gauche_P3(void);
void			V4_Tir_Special_Gauche_P4(void);
void			V4_Tir_Special_Gauche_P5(void);


//######################################################################
// spaceship 5: add shot force 1
//######################################################################
//front shot force 1 (power 1 to 5)
void			V5_Tir_Normal_Avant_P1();
void			V5_Tir_Normal_Avant_P2(int);
void			V5_Tir_Normal_Avant_P3(void);
void			V5_Tir_Normal_Avant_P4(int);
void			V5_Tir_Normal_Avant_P5(void);
//rear shot force 1 (power 1 to 5)
void			V4_Tir_Special_Arriere_P1(void);
void			V5_Tir_Normal_Arriere_P1();
void			V5_Tir_Normal_Arriere_P2(int);
void			V5_Tir_Normal_Arriere_P3(void);
void			V5_Tir_Normal_Arriere_P4(int);
void			V5_Tir_Normal_Arriere_P5(void);
//right shot force 1 (power 1 to 5)
void			V5_Tir_Normal_Droite_P1();
void			V5_Tir_Normal_Droite_P2(int);
void			V5_Tir_Normal_Droite_P3(void);
void			V5_Tir_Normal_Droite_P4(int);
void			V5_Tir_Normal_Droite_P5(void);
//left shot force 1 (power 1 to 5)
void			V5_Tir_Normal_Gauche_P1();
void			V5_Tir_Normal_Gauche_P2(int);
void			V5_Tir_Normal_Gauche_P3(void);
void			V5_Tir_Normal_Gauche_P4(int);
void			V5_Tir_Normal_Gauche_P5(void);

//######################################################################
// spaceship 5: add shot force 2
//######################################################################
//front shot force 2 (power 1 to 5)
void			V5_Tir_Special_Avant_P1(void);
void			V5_Tir_Special_Avant_P2(void);
void			V5_Tir_Special_Avant_P3(void);
void			V5_Tir_Special_Avant_P4(void);
void			V5_Tir_Special_Avant_P5(void);
void			V5_Tir_Special_Arriere_P1(void);
void			V5_Tir_Special_Arriere_P2(void);
void			V5_Tir_Special_Arriere_P3(void);
void			V5_Tir_Special_Arriere_P4(void);
void			V5_Tir_Special_Arriere_P5(void);
void			V5_Tir_Special_Droite_P1(void);
void			V5_Tir_Special_Droite_P2(void);
void			V5_Tir_Special_Droite_P3(void);
void			V5_Tir_Special_Droite_P4(void);
void			V5_Tir_Special_Droite_P5(void);
void			V5_Tir_Special_Gauche_P1(void);
void			V5_Tir_Special_Gauche_P2(void);
void			V5_Tir_Special_Gauche_P3(void);
void			V5_Tir_Special_Gauche_P4(void);
void			V5_Tir_Special_Gauche_P5(void);

//scrolltext handle ============================================================
Sint32			New_Element_FS(void);
void			Del_Element_FS(Sint32 n, Sint32 o);
void			Gestion_Fontes_Ki_Scrollent(void);
void			Init_Scroll_Present(void);
void			Destruct_Scroll_Font(Sint32 d);

// "meteors.cpp"
void			meteor_execution();
Sint32			 meteor_initialise();
void			meteor_libere();

// "options.cpp"
void			option_execution();
void			testBoutonOption();

// "courbe.cpp"
void			courbe_execution();
void			courbe_testFin();


#ifdef DEVELOPPEMENT
void			courbe_editeur();
#endif

//"grille.cpp" .................................................................
void			grille_execution();


void			grille_testFin();

//"bonus.cpp" ..................................................................
void			bonus_execution();
void			ajoute_boule();

//"vaisseau.cpp" ...............................................................
void			vaisseau_restaure();
void			vaisseau_invincibilite();
void			boule_execution();
void			canon_execution();

// "ennemis_execution.cpp" .....................................................
void			ennemis_execution();
float                   Deplacement_Tir_X(float angle, float vitesse, float x);
float                   Deplacement_Tir_Y(float angle, float vitesse, float y);
void			finiObjects();
void			Keys_Down(Uint32);
void			Keys_Up(Uint32);
void			draw_eclair(char *, Eclair *, int);
Uint32            doInit();
void			finiObjects();
void			_initialiseGameOver();
void			tirsJoueur();
void			afficheEtoiles();
void			eclairExecution();
void			explosionExecution();
void			tirsExecutions();

#ifdef DEVELOPPEMENT
void			touchesTriches();
#endif

//seal : synthetic audio library ...............................................
#ifdef USE_SDLMIXER
Sint32			seal_init();
void			seal_exec();
Sint32			seal_JoueModule(int);
void			seal_kill();
void			seal_joue(Uint32);

#define kbhit()                0
#define VCHANGE1               0			           //change de vaisseau (progression)
#define VCHANGE2               1			           //change de vaisseau (regression)
#define SELECT                 2			           //option selectionnee (dans le tableau)
#define NOSELECT               3			           //option selectionnee (mais il n'ya rien a selectionner)
#define BONUS4                 4			           //bonus violet
#define BONUS3                 5			           //bonus jaune
#define BONUS1                 6			           //bonus vert
#define BONUS2                 7			           //bonus rouge
#define SPECNMI                8			           //ennemi special arrive
#define ONDE_CHOC              9			           //onde de choc (bonus violet si l'energie du vaisseau est au maximum
#define TIR2                   10			          //tir vaisseau joueur
#define TIR1                   11			          //tir vaisseau joueur
#define XPLOC1                 12			          //explosion 64*64
#define XPLOC2                 13			          //explosion 64*64
#define XPLOC3                 14			          //explosion 64*64
#define XPLOC4                 15			          //explosion 64*64
#define XPLOB1                 16			          //explosion 32*32
#define XPLOB2                 17			          //explosion 32*32
#define XPLOB3                 18			          //explosion 32*32
#define XPLOB4                 19			          //explosion 32*32
#define XPLOA1                 20			          //explosion 16*16
#define XPLOA2                 21			          //explosion 16*16
#define XPLOA3                 22			          //explosion 16*16
#define XPLOA4                 23			          //explosion 16*16
#define TIR1NMI                24			          //tir ennemi 1
#define TIR2NMI                25			          //tir ennemi 2
#define TIR3NMI                26			          //tir ennemi 3 (gardien)
#define TIR4NMI                27			          //tir ennemi 4 (gardien)
#endif

// "gfxbase.cpp"  ...............................................................
void                   type_routine_gfx();
void                   putsprcolor(Uint32 _iColor, image * _pImage, Uint32 _iX, Uint32 _iY);
void                   putsprite3(image * _pImage, Uint32 _iX, Uint32 _iY);
void                   putsprite3(fonte * _pFonte, Uint32 _iX, Uint32 _iY);
void                   poke(char *adresse, unsigned char valeur);
void                   putpixel(char *_pAdresse, Sint32 x, Sint32 y, Sint32 coul);
void                   _putRectangleVide(char *adresse, Sint32 x, Sint32 y, Sint32 coul, Sint32 large, Sint32 haute);
void                   _ASMCOPYFONTE(unsigned char *dst, unsigned char *src);
void                   putsprite3_scr(fonte * _pFonte, Uint32 _iX, Uint32 _iY);
void                   putsprite3_opt(fonte * _pFonte, Uint32 _iX, Uint32 _iY);
void                   putsprite3_scr(image * _pImage, Uint32 _iX, Uint32 _iY);
void                   copy2X_512x440(char *, char *, Uint32);
void                   copy2X(char *, char *, Uint32 _iLarge, Uint32 _iHaut, Uint32 _iOffset, Uint32 _iOffset2);

// "powermanga.cpp" .............................................................
Sint32			 UpdateFrame();                         //main loop

// "playanim.cpp"  ..............................................................
Sint32			 joueAnimation();                       //play animation compressed by Etienne Sobole

// "displayFonte.cpp" ............................................................
void			Aff_Score_Player_One();                //display player's score
void			Aff_Level_Player_One();                //display level number
void			Aff_Game_Pause_Player_One();           //display "PAUSE"
void			initializeFonte();                     //initialise fontes's structures
void			update_bar_nrj_player();

// "onde.cpp" (shock wave) .......................................................
void                   Draw_Onde_De_Choc();                   //display shock wave to the screen
Sint32			New_Element_Onde_Choc();
void                   Del_Element_Onde_Choc(Sint32 n, Sint32 o);
void                   Init_Tab_OndeChoc();                   //initialize table "onde_choc"
bool                   Tst_Col_Onde_Choc(int);
void                   polygone(char *, Sint32 *, Sint32, Uint32);

void                   _joueAnimation() ;
void                   fichierFlag() ;
void                   fichierFlag2(char *_pBuffer) ;
void                   entierEnASCII(Uint32 _iNombre, char* _pASCII) ;
void                   entier2ASCII(Sint32 _iNombre,char* _pASCII, Uint32 _iReste) ;
char                  *copieChaine(char* _pSource, char* _pDestination) ;


//..............................................................................
#include "configfile.hpp"
#include "linux.hpp"
#include "score.hpp"
#include "assembler.h"
#include "etbfonte1.hpp"
#include "score.hpp"
#endif

//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "sdl_tlk.cpp"
// created	: 2003-07-09
// updates	: 2005-01-26
// id		: %Id$
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.t
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA  02111-1307, USA.
// Place - Suite 330, Boston, MA  02111-1307, USA.
//
//*****************************************************************************
#include "powermanga.hpp"
#ifdef SDL_TLK

#ifdef _GP2X
// GP2X button codes, as received through SDL joystick events
const int GP2X_BUTTON_UP        = 8;
const int GP2X_BUTTON_DOWN      = 6;
const int GP2X_BUTTON_LEFT      = 7;
const int GP2X_BUTTON_RIGHT     = 9;
//const int GP2X_BUTTON_UPLEFT    = 1;
//const int GP2X_BUTTON_UPRIGHT   = 7;
//const int GP2X_BUTTON_DOWNLEFT  = 3;
//const int GP2X_BUTTON_DOWNRIGHT = 5;
//const int GP2X_BUTTON_CLICK     = 13;
const int GP2X_BUTTON_A         = 2;
const int GP2X_BUTTON_B         = 1;
const int GP2X_BUTTON_X         = 3;
const int GP2X_BUTTON_Y         = 0;
const int GP2X_BUTTON_L         = 4;
const int GP2X_BUTTON_R         = 5;
const int GP2X_BUTTON_START     = 11;
const int GP2X_BUTTON_SELECT    = 10;
//const int GP2X_BUTTON_VOLUP     = 16;
//const int GP2X_BUTTON_VOLDOWN   = 17;

const int GP2X_CHARACTER_MAP[]  = {' ',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};
const int GP2X_CHARMAP_SIZE     = 26 + 10 + 1;

const int GP2X_FIRST_BUTTON     = GP2X_BUTTON_Y;
const int GP2X_LAST_BUTTON      = GP2X_BUTTON_START;

const int GP2X_NUM_BUTTONS      = (GP2X_LAST_BUTTON - GP2X_FIRST_BUTTON) + 1;

// The current state of all the GP2X buttons is stored in this array - used
// to handle multi-key actions
bool gp2x_buttons[GP2X_NUM_BUTTONS];

// The current indexes into the character map for all score characters
int gp2x_score_indexes[iLONGEURNOM];

// The resolution the GP2X runs at
const int GP2X_VIDEO_HEIGHT     = 200;

// We need to pull in SDL_Mixer as well so we can change the volume
#include <SDL/SDL_mixer.h>
#endif

//------------------------------------------------------------------------------
SDL_Surface           *pPowerMangaDisplay;
SDL_Surface            *pEcranXImage = 0;                      //(4 octets=pointeur) SDL_Surface de 512*440
unsigned int            display_offset_y = 0;
SDL_Surface            *pEcranXImage640 = 0;                   //(4 octets=pointeur) SDL_Surface de 640*400
SDL_Surface            *pEcranOptXImage = 0;                   //(4 octets=pointeur) SDL_Surface de 64*184
SDL_Surface            *pEcranScrXImage = 0;                   //(4 octets=pointeur) SDL_Surface de 320*16
unsigned int            iOctetsParPixel = 0;                   //nombre d'octets par pixels (1=256 couleurs / 2=32768 couleurs)
unsigned char          *pal = 0;                               //palette de couleurs 24 bits (3 octets par couleur)
unsigned int           *pal32 = 0;                             //palette de couleurs 24 bits (4 octets par couleur)
unsigned short         *pal16 = 0;                             //palette de couleurs 16 bits (2 octets par couleur)
unsigned short         *pal16PlayAnim = 0;                     //palette de couleurs 16 bits (2 octets par couleur)
unsigned int           *pal32PlayAnim = 0;                     //palette de couleurs 16 bits (2 octets par couleur)
SDL_Surface            *pEcranPlayanim = 0;                    //(4 octets=pointeur) SDL_Surface de 320*200
const unsigned int      iXIMAGEMAX = 100;
unsigned int            iNombreXImage = 0;
SDL_Surface            *pListeXImage[iXIMAGEMAX];

//...............................................................................
unsigned int            iProfondeur = 0;                       //profondeur en bits de l'ecran
unsigned int            iProfondeur2 = 0;                      //profondeur en bits de l'ecran
unsigned int            iLargeurEcran = 0;                     //largeur de notre fenetre
unsigned int            iHauteurEcran = 0;                     //hauteur de notre fenetre
unsigned int            iLargeurEcranTotal = 0;                //taille d'une ligne du buffer 'ecran_ram'
int                     vmode = 0;                             //0=resolution de 320x200 / 1=640*400 / 2=640*400 (double pixels)
#ifdef WIN32
Sint32			vmode2 = 1;				// si vmode = 1 ==> 0 = 640x400 / 1 = 640x480
#else
Sint32			vmode2 = 0;
#endif

//...............................................................................
char                   *ecran_playanim = 0;                    //memory buffer 320*200
char                   *ecran_ram640 = 0;                      //memory buffer 640*400
char                   *ecran_scr = 0;                         //memory buffer 320*016 (score bar-line)
char                   *ecran_opt = 0;                         //memory buffer 064*184 (option panel)
unsigned int            iAffiche = 0;                          //0=update display option panel and bareline's score

//..............................................................................
extern bool             bar_nrj_player;
extern char            *ecran_ram;                             //adresse du buffer de 512x440 (XImage)
extern short            touch[500];                            //tableau des flags de toutes les touches clavier
extern int              quit_game;                             //1=demande a quitter le jeu
extern unsigned int     iCodeTouche;                           // code touche pressee

//...............................................................................

//..............................................................................
void                    initialiseCurseurBlanc();
void                    affiche_ecran();
void                    afficheAnim();

void                    fenetre320x200();
void                    fenetre640x400();

void                    dga320x200();
void                    dga640x400();

SDL_Surface            *creeXImage(unsigned int _iLarge,
                                   unsigned int _iHaute);
void                    libereXImage(SDL_Surface *_pXImage);
void                    libereXImages();
void                    key_status(Uint8 *k);

//display optimisation for options .............................................
extern unsigned int     pOptionChange[22][2];                  //display options, positions in the window
extern int              iOptionIndex;                          //index to the "pOptionChange" table, display options
extern unsigned int     iBonusX2Affiche;                       //display bonusX2 in our window
extern unsigned int     iBonusX4Affiche;                       //display bonusX4 in out window
extern unsigned char   *image1;                                //pointer to the buffer for the full screen animations
extern unsigned int     iBarreNrjJoueurXWindow;                //1=update player's enerny bar-line
extern unsigned int     iBarreNrjGardienXWindow;               //1=update bigboss's energy bar-line
extern unsigned int     iScoreXWindow;                         //1=update player's score points
extern configfile*      power_conf;

//...............................................................................
SDL_Color              *pXColor = 0;                           //color table

//...............................................................................
char nomfenetre[] = "Powermanga by TLK Games (SDL version)\0";



//------------------------------------------------------------------------------
// SDL : initialise un affichage SDL / initialize SDL display
// sortie / output <= 0 : erreur
//------------------------------------------------------------------------------
int xw_init()
{
  for(unsigned int _iIndex = 0; _iIndex < iXIMAGEMAX; _iIndex++)
    pListeXImage[_iIndex] = 0;

//..............................................................................
  if(!vmode)
  { iLargeurEcran = LARG_ECRAN_PHY;
    iHauteurEcran = HAUT_ECRAN_PHY;
  }
  else
  { iLargeurEcran = LARG_ECRAN_PHY * 2;
    iHauteurEcran = HAUT_ECRAN_PHY * 2;
  }



  // initialize SDL screen .....................................................
    int init_flags = SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE;
#ifdef _GP2X
    init_flags |= SDL_INIT_JOYSTICK;
#endif

  //if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_NOPARACHUTE) <0 )
  if(SDL_Init(init_flags) < 0)
  { fprintf(stderr, "ecran_hard::ecran_init() : sdl_init: %s", SDL_GetError());
    return 0;
  }
#ifdef _GP2X
    // We'll need a joystick open on the GP2X
    SDL_JoystickOpen(0);

    // Force 8 bits per pixel if running on the GP2X
    iProfondeur = 8;
    iOctetsParPixel = 1;
#else
    const SDL_VideoInfo * vi = SDL_GetVideoInfo();

    iProfondeur = vi->vfmt->BitsPerPixel;
    iOctetsParPixel = vi->vfmt->BytesPerPixel;

    if(iProfondeur == 16) {
        iProfondeur = vi->vfmt->Rshift + vi->vfmt->Gshift + vi->vfmt->Bshift;
    }
#endif
  iProfondeur2 = iProfondeur;
  if(iProfondeur < 8)
  { afficheErreur("this game need 8 bits peer pixels minimum (256 colors)",
                  "sdl_tlk.cpp/xw_init()");
    return 0;
  }
#ifdef _VERBEUX_
  if(power_conf->verbose)
  { printf
      ("> sdl_tlk.cpp/xw_init(): depth of screen: %i; bytes per pixel: %i;\n",
       iProfondeur, iOctetsParPixel);
  }
#endif
  if(!init_video_mode())
    return 0;

  SDL_EnableUNICODE(1);
#ifdef _VERBEUX_
  if(power_conf->verbose)
    afficheMessage("> sdl_tlk.cpp/xw_init(): successful initialization!");
#endif
  return 1;
}

//------------------------------------------------------------------------------
// SDL: initialize video mode
// return: 0 = > error / 1 => no error
//------------------------------------------------------------------------------
int init_video_mode()
{
	Uint32 flag;
	Uint32 bpp;

	//##############################################################
	// 640x400 => 640x480 (win32 only)
	//##############################################################
	if ((vmode) && (vmode2)) {
		iHauteurEcran = 480;
	}

	//##############################################################
	// test video mode
	//##############################################################
#ifdef _GP2X
    // Ignore any video mode options if running on the GP2X - force a 320x240
    // full screen resolution, with 8 bits per pixel, as well as specific SDL
    // surface flags.
    flag = SDL_SWSURFACE | SDL_FULLSCREEN;
    iHauteurEcran = GP2X_VIDEO_HEIGHT;
    // Use a 20px y offset to center the output on the GP2X's screen
    display_offset_y = (GP2X_VIDEO_HEIGHT - HAUT_ECRAN_PHY) / 2;

    // Also, set the initial button & score charmap state
    // TODO: what is the sequence of SDL events when buttons are already held
    // down while entering the application?
    for (int i = GP2X_FIRST_BUTTON; i < GP2X_NUM_BUTTONS; gp2x_buttons[i++] = false);
    for (int i = 0; i < iLONGEURNOM; gp2x_score_indexes[i++] = 0);
#else
	flag = SDL_ANYFORMAT;
	if(iOctetsParPixel == 1)
		flag = flag | SDL_HWPALETTE;
	if(power_conf->fullscreen > 0)
		flag = flag | SDL_FULLSCREEN;
	bpp = SDL_VideoModeOK(iLargeurEcran, iHauteurEcran, iProfondeur2, flag);
	if(!bpp)
	{	if(!power_conf->fullscreen)
		{	fprintf(stderr,
				"sdl_tlk.cpp/init_video_mode(): SDL_VideoModeOK() return: %s",
				SDL_GetError());
			return 0;
		}
		else
		{
			// fullscreen fail, try in window mode
			power_conf->fullscreen = 0;
			flag = SDL_ANYFORMAT;
			if(iOctetsParPixel == 1)
				flag = flag | SDL_HWPALETTE;
			bpp = SDL_VideoModeOK(iLargeurEcran, iHauteurEcran, iProfondeur2, flag);
			if(!bpp)
			{	fprintf(stderr,
					"sdl_tlk.cpp/init_video_mode(): SDL_VideoModeOK() return: %s",
					SDL_GetError());
				return 0;
			}
		}
	}
#endif

	//##############################################################
	// initialize video mode
	//##############################################################
	pPowerMangaDisplay =
		SDL_SetVideoMode(iLargeurEcran, iHauteurEcran, iProfondeur2, flag);
	if(!pPowerMangaDisplay)
	{	fprintf(stderr,
			"sdl_tlk.cpp/init_video_mode(): SDL_SetVideoMode() return %s\n",
			SDL_GetError());
		return 0;
	}

#ifdef _GP2X
    // The native resolution is 320x200, so we scale up to 320x240 when updating
    // the screen
    iHauteurEcran = HAUT_ECRAN_PHY;
#endif
	//##############################################################
  	// restore valid height screen (win32 only)
	//##############################################################
	if ((vmode) && (vmode2)) {
		iHauteurEcran = HAUT_ECRAN_PHY * 2;
	}

	SDL_WM_SetCaption(nomfenetre, nomfenetre);
	iAffiche = 0; // force refresh all the screen
	if(power_conf->fullscreen > 0)
		SDL_ShowCursor(SDL_DISABLE);
	else
		SDL_ShowCursor(SDL_ENABLE);
	return 1;
}

//------------------------------------------------------------------------------
// SDL destroy surface / start anim and end anim / "playanim.cpp"
//------------------------------------------------------------------------------
void kill_ecranPlayanim()
{
  libereXImage(pEcranPlayanim);
  ecran_playanim = 0;
  pEcranPlayanim = 0;
}

//------------------------------------------------------------------------------
// SDL : create surface / start anim and end anim / "playanim.cpp"
//------------------------------------------------------------------------------
int reserve_ecranPlayanim()
{
  if(!(pEcranPlayanim = creeXImage(LARG_ECRAN_PHY, HAUT_ECRAN_PHY)))
    return 0;
  ecran_playanim = (char *)pEcranPlayanim->pixels;
  return 1;                                                    //no error
}

//------------------------------------------------------------------------------
// SDL : create 3 or 4 surfaces for the game
//------------------------------------------------------------------------------
int reserve_ecran_ram()
{
  //create surface "ecran_ram" 512*440
  if(!(pEcranXImage = creeXImage(LARG_ECR_RAM_TOT, HAUT_ECR_RAM_TOT)))
    return 0;
  ecran_ram = (char *)pEcranXImage->pixels;
  iLargeurEcranTotal = LARG_ECR_RAM_TOT * iOctetsParPixel;

  //create surface 640x400 (window 640x400)
  if(vmode == 1)
  {
    if(!(pEcranXImage640 = creeXImage(iLargeurEcran, iHauteurEcran)))
      return 0;
    ecran_ram640 = (char *)pEcranXImage640->pixels;
  }
  if(!(pEcranOptXImage = creeXImage(OPT_LARGE, OPT_HAUTE)))
    return 0;
  ecran_opt = (char *)pEcranOptXImage->pixels;
  if(!(pEcranScrXImage = creeXImage(SCR_LARGE, SCR_HAUTE)))
    return 0;
  ecran_scr = (char *)pEcranScrXImage->pixels;
  return 1;
}

//------------------------------------------------------------------------------
// SDL : create palette 16 bits and 24 bits
//       recopie la palette 24 bits et cree une palette 16 bits
// entree / input => _pNomFichier : filname's palette /  nom du fichier palette
//------------------------------------------------------------------------------
int charge_palette(char *_pNomFichier)
{
  unsigned int _iIndex;
  unsigned char *_pPal;
  if(!pal)
    pal = (unsigned char *)chargeFichier(_pNomFichier);
  if(!pal)
    return 0;

  //ecran 8 bits 256 couleurs  . . . . . . . . . . . . . . . . . . . . . . . . . .
  if(iOctetsParPixel == 1)
  { if(!pXColor)
      pXColor = (SDL_Color *) reserveMemoire(sizeof(SDL_Color) * 256);
    if(!pXColor)
    { afficheErreur("'pXColor' out of memory", "sdl_tlk.cpp/charge_palette()");
      return 0;
    }
    unsigned char *_p = (unsigned char *)pal;
    for(_iIndex = 0; _iIndex < 256; _iIndex++)
    { pXColor[_iIndex].r = _p[0] ;
      pXColor[_iIndex].g = _p[1] ;
      pXColor[_iIndex].b = _p[2] ;
      _p += 3;
    }
    SDL_SetPalette(pPowerMangaDisplay,
      SDL_PHYSPAL | SDL_LOGPAL, pXColor, 0, 256);
  }
  else

  //ecran 16 bits 65536 couleurs   . . . . . . . . . . . . . . . . . . . . . . . .
  { if(iOctetsParPixel == 2)
    { if(!pal16)
        pal16 = (unsigned short *)reserveMemoire(256 * 2);
      if(!pal16)
      { afficheErreur("'pal16' out of memory", "sdl_tlk.cpp/charge_palette()");
        return 0;
      }
      if(iProfondeur2 == 15)
        convertitPalette24_15(pal, pal16);
      else
        convertitPalette24_16(pal, pal16);
    }
    else

    //ecran 24 ou 32 bits  . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    { if(iOctetsParPixel > 2)
      { if(!pal32)
          pal32 = (unsigned int *)reserveMemoire(256 * 4);
        if(!pal32)
        { afficheErreur("'pal32' out of memory", "sdl_tlk.cpp/charge_palette()");
          return 0;
        }
        unsigned char *_p = (unsigned char *)pal32;
        _pPal = pal;
        //_p[3]=? / _p[2]=red / _p[1]=green / _p[0]=blue
        for(_iIndex = 0; _iIndex < 256; _iIndex++)
        { _p[2] = _pPal[2];
          _p[1] = _pPal[1];
          _p[0] = _pPal[0];
          _p[3] = 0;
          _p += 4;
          _pPal += 3;
        }
      }
    }
  }
  return 1;
}

#ifdef _GP2X
extern Uint32 iVolumeModule;
const Uint32      volume_increment  = MIX_MAX_VOLUME / 16;
void handleGP2XButtons()
{
    // This button mapping conforms to the GP2X Common User Interface
    // Recommendations, as of 2006-07-29, available from
    // http://wiki.gp2x.org/wiki/Common_User_Interface_Recommendations

    // Directions (UDLR)
    touch[55] = gp2x_buttons[GP2X_BUTTON_UP];
                //| gp2x_buttons[GP2X_BUTTON_UPLEFT]
                //| gp2x_buttons[GP2X_BUTTON_UPRIGHT];
    touch[87] = gp2x_buttons[GP2X_BUTTON_DOWN];
                //| gp2x_buttons[GP2X_BUTTON_DOWNLEFT]
                //| gp2x_buttons[GP2X_BUTTON_DOWNRIGHT];
    touch[71] = gp2x_buttons[GP2X_BUTTON_LEFT];
                //| gp2x_buttons[GP2X_BUTTON_UPLEFT]
                //| gp2x_buttons[GP2X_BUTTON_DOWNLEFT]
                //| gp2x_buttons[GP2X_BUTTON_L];
    touch[73] = gp2x_buttons[GP2X_BUTTON_RIGHT];
                //| gp2x_buttons[GP2X_BUTTON_UPRIGHT]
                //| gp2x_buttons[GP2X_BUTTON_DOWNRIGHT]
                //| gp2x_buttons[GP2X_BUTTON_R];

    // Pause (in game) / Return (in menu)
    touch[50] = touch[47] = gp2x_buttons[GP2X_BUTTON_START];

    // Escape (exit to menu)
    touch[0] = gp2x_buttons[GP2X_BUTTON_SELECT];

    // Volume control - very hacky for the moment, but PM doesn't support it natively
    if (gp2x_buttons[GP2X_BUTTON_L] || gp2x_buttons[GP2X_BUTTON_R]) {
        // Up
        if (gp2x_buttons[GP2X_BUTTON_L]) {
            iVolumeModule = (iVolumeModule + volume_increment > MIX_MAX_VOLUME)
                ? MIX_MAX_VOLUME : iVolumeModule + volume_increment;
        }
        // Down
        if (gp2x_buttons[GP2X_BUTTON_R]) {
            iVolumeModule = (iVolumeModule <= volume_increment)
                ? 0 : iVolumeModule - volume_increment;
        }
        // Set the volume of all channels, plus the music
        //Mix_Volume(-1, iVolumeModule);
        Mix_VolumeMusic(iVolumeModule);
    }

    // Fire
    touch[90] = gp2x_buttons[GP2X_BUTTON_A];
               // | gp2x_buttons[GP2X_BUTTON_B]
               // | gp2x_buttons[GP2X_BUTTON_CLICK];

    // Special (Powerup), also Control for multi-key commands
    touch[88] = gp2x_buttons[GP2X_BUTTON_X];
               // | gp2x_buttons[GP2X_BUTTON_Y];

    // Quit
    touch[58] = 0;
    //quit_game = gp2x_buttons[GP2X_BUTTON_START];

    // "Control" - needed for Quit, etc
    touch[88] |= touch[58];
}
#endif

//------------------------------------------------------------------------------
// SDL : gestion des evenements
//------------------------------------------------------------------------------
int xw_exec()
{
  SDL_Event event;
  SDL_KeyboardEvent *ke;
  //SDL_keysym *ks;
  while(SDL_PollEvent(&event) > 0)
  { switch (event.type)
    {
      // key down
      case SDL_KEYDOWN:
      { ke = (SDL_KeyboardEvent*) &event;
        //printf("sdl_tlk.cpp/xw_exec() SDL_KEYDOWN : %i %i %i %i\n",ke->type, ke->keysym.sym, ke->keysym.unicode, ke->state);
        Uint8 *keys;
        keys = SDL_GetKeyState(NULL);
        if ( keys[SDLK_F10] == SDL_PRESSED)
        { quit_game = 1;
        }
        key_status(keys);
        if(ke->keysym.unicode > 0)
          _chaine::setCodeTouchePressee(ke->keysym.unicode);
        else
          _chaine::setCodeTouchePressee(ke->keysym.sym);
        iCodeTouche = ke->keysym.sym;                         //sauve le code touche

      }
      break;

      case SDL_KEYUP:
      { ke = (SDL_KeyboardEvent*) &event;
        //printf("sdl_tlk.cpp/xw_exec() SDL_KEYUP : %i %i %i %i\n",ke->type, ke->keysym.sym, ke->keysym.unicode, ke->state);
        Uint8 *keys;
        keys = SDL_GetKeyState(NULL);
        if(ke->keysym.unicode > 0)
          _chaine::setCodeToucheRelachee(ke->keysym.unicode);
        else
          _chaine::setCodeToucheRelachee(ke->keysym.sym);
        if((int)iCodeTouche == ke->keysym.sym) iCodeTouche = 0;    // remet a zero le code touche
        key_status(keys);

      }
      break;

      case SDL_QUIT:
      { quit_game = 1;
      }
      break ;

#ifdef _GP2X
            // Handle GP2X-specific button presses
            	case SDL_JOYBUTTONUP:
		case SDL_JOYBUTTONDOWN:
                if (event.jbutton.button >= GP2X_FIRST_BUTTON
                    && event.jbutton.button <= GP2X_LAST_BUTTON)
                {
                    // We have a GP2X button event of some sort
                    if (event.type == SDL_JOYBUTTONDOWN) {
                        gp2x_buttons[event.jbutton.button] = true;

                        // Special case for entering scores
                        static bool prev_input_state = false;
                        bool is_input = _score::isInputName();
                        if (is_input) {
                            if (!prev_input_state) {
                                // Reset the character indexes if we're starting a new score entry
                                for (int i = 0; i < iLONGEURNOM; gp2x_score_indexes[i++] = 0);
                            }
                            const _chaine * const score_input = _score::getScoreInput();
                            int char_index = score_input ? score_input->getCurseurPosition() : 0;
                            int oldc = GP2X_CHARACTER_MAP[gp2x_score_indexes[char_index]];
                            int newc = oldc;
                            switch (event.jbutton.button) {
                            case GP2X_BUTTON_DOWN:
                                if (--gp2x_score_indexes[char_index] < 0) gp2x_score_indexes[char_index] = GP2X_CHARMAP_SIZE - 1;
                                newc = GP2X_CHARACTER_MAP[gp2x_score_indexes[char_index]];
                                break;
                            case GP2X_BUTTON_UP:
                                if (++gp2x_score_indexes[char_index] >= GP2X_CHARMAP_SIZE) gp2x_score_indexes[char_index] = 0;
                                newc = GP2X_CHARACTER_MAP[gp2x_score_indexes[char_index]];
                                break;
                            case GP2X_BUTTON_LEFT:
                            case GP2X_BUTTON_L:
                                newc = SDLK_LEFT;
                                break;
                            case GP2X_BUTTON_RIGHT:
                            case GP2X_BUTTON_R:
                                newc = SDLK_RIGHT;
                                break;
                            case GP2X_BUTTON_A:
                            case GP2X_BUTTON_B:
                            //case GP2X_BUTTON_CLICK:
                                newc = SDLK_RETURN;
                                break;
                            }
                            if (newc != oldc) {
                                _chaine::setCodeTouchePressee(newc);
                            }
                        }
                        prev_input_state = is_input;
                        iCodeTouche = 1;
                    } else {
                        gp2x_buttons[event.jbutton.button] = false;
                        iCodeTouche = 0;
                    }
                    handleGP2XButtons();
                }
                break;
#endif
    }
  }

  affiche_ecran();
  return 0;
}

void key_status(Uint8 *k)
{
  touch[0]   = k[SDLK_ESCAPE];                                //ESC (ASCII=27)
  touch[88]  = k[SDLK_LCTRL];
  touch[88] |= k[SDLK_RCTRL];
  touch[50]  = k[SDLK_RETURN];                                //RETURN (ASCII=13)
  touch[150] = k[SDLK_PAUSE];
  touch[74]  = k[SDLK_LSHIFT];
  touch[74] |= k[SDLK_RSHIFT];
  touch[1]   = k[SDLK_F1];
  touch[2]   = k[SDLK_F2];
  touch[3]   = k[SDLK_F3];
  touch[4]   = k[SDLK_F4];
  touch[5]   = k[SDLK_F5];
  touch[6]   = k[SDLK_F6];
  touch[7]   = k[SDLK_F7];
  touch[8]   = k[SDLK_F8];
  touch[9]   = k[SDLK_F9];
  touch[10]  = k[SDLK_F10];
  touch[11]  = k[SDLK_F11];
  touch[12]  = k[SDLK_F12];
  touch[13]  = k[SDLK_INSERT];
  touch[90]  = k[SDLK_SPACE];
  touch[71]  = k[SDLK_LEFT];
  touch[73]  = k[SDLK_RIGHT];
  touch[55]  = k[SDLK_UP];
  touch[87]  = k[SDLK_DOWN];
  touch[38]  = k[SDLK_a];                 //A CTRL+A=ABOUT
  touch[39]  = k[SDLK_f];                 //F F=FULL SCREEN
  touch[79]  = k[SDLK_v];                 //V
  touch[80]  = k[SDLK_b];                 //B
  touch[47]  = k[SDLK_p];                 //P P=PAUSE
  touch[58]  = k[SDLK_q];                 //Q CTRL+Q gameover
  touch[60]  = k[SDLK_s];                 //S CTRL+S coupe la musique

  touch[73]  |= k[SDLK_KP6];              //right arrow
  touch[71]  |= k[SDLK_KP4];              //left arrow
  touch[55]  |= k[SDLK_KP8];              //up arrow
  touch[87]  |= k[SDLK_KP5];              //down arrow
  touch[88]  |= k[SDLK_KP2];              //power-up (aka Ctrl key)
  touch[90]  |= k[SDLK_KP0];              //fire (aka space bar)
}


//------------------------------------------------------------------------------
// SDL : display screen
//------------------------------------------------------------------------------
// joue animation / play compressed ani
void affiche_ecran()
{
  if(pEcranPlayanim)                                           // movie ?
  { iAffiche = 0;
    afficheAnim();                                             // movie player.
  }
  else
  { switch (vmode)
    { case 0:
        fenetre320x200();
        break;                                               // window 320*200
      case 1:
        fenetre640x400();
        break;
    }
  }
}

//------------------------------------------------------------------------------
// SDL : affiche l'animation compactee / display start movie and end movie
//------------------------------------------------------------------------------
void afficheAnim()
{
  Uint32 v;
  SDL_Rect rsour, destrect;
  int i;

  switch (iOctetsParPixel)
  { case 1:
      for(i = 0; i < (LARG_ECRAN_PHY * HAUT_ECRAN_PHY); i++)
        ecran_playanim[i] = image1[i];
      break;
    case 2:
      conv8_16((char *)image1, ecran_playanim, pal16,
               LARG_ECRAN_PHY * HAUT_ECRAN_PHY);
      break;
    case 3:
      conv8_24((char *)image1, ecran_playanim, pal32PlayAnim,
               LARG_ECRAN_PHY * HAUT_ECRAN_PHY);
      break;
    case 4:
      conv8_32((char *)image1, ecran_playanim, pal32PlayAnim,
               LARG_ECRAN_PHY * HAUT_ECRAN_PHY);
      break;
  }

  //mode 320x200
  switch (vmode)
  { case 0:
    {
            rsour.x = rsour.y = 0;
            rsour.w = LARG_ECRAN_PHY;
            rsour.h = HAUT_ECRAN_PHY;
            destrect = rsour;
            destrect.y += display_offset_y;

      v = SDL_BlitSurface(pEcranPlayanim, &rsour, pPowerMangaDisplay, &destrect);
      if(v < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
      SDL_UpdateRect(pPowerMangaDisplay, 0, 0,
                     pPowerMangaDisplay->w, pPowerMangaDisplay->h);
    }
    break;

    //mode 640x400
    case 1:
      { copy2X(ecran_playanim, ecran_ram640, 320, 200, 0,
               iLargeurEcran * iOctetsParPixel * 2 -
               (iLargeurEcran * iOctetsParPixel));
      rsour.x = 0;

	  if (vmode2)
         rsour.y = 40;
	  else
	     rsour.y = 0;

      rsour.w = iLargeurEcran; rsour.h = iHauteurEcran;
      v = SDL_BlitSurface(pEcranXImage640, &rsour, pPowerMangaDisplay, &rsour);
      if(v < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

      }

	    if (vmode2)
			SDL_UpdateRect(pPowerMangaDisplay, 0, 40,
                     pPowerMangaDisplay->w, pPowerMangaDisplay->h - 40);
		else
			SDL_UpdateRect(pPowerMangaDisplay, 0, 0,
                     pPowerMangaDisplay->w, pPowerMangaDisplay->h);


        break;
    }
}


//------------------------------------------------------------------------------
// SDL : display window in 320*200
//------------------------------------------------------------------------------
void fenetre320x200()
{
  Uint32 v;
  SDL_Rect rdest;
  SDL_Rect rsour;
  rsour.x = BANDE_DE_CLIP;
  rsour.y = BANDE_DE_CLIP;
  rsour.w = LARG_ECR_RAM;
  rsour.h = HAUT_ECR_RAM;
  rdest.x = 0;
  rdest.y = 16;
  rdest.w = LARG_ECR_RAM;
  rdest.h = HAUT_ECR_RAM;

  rdest.y += display_offset_y;
  v = SDL_BlitSurface(pEcranXImage, &rsour, pPowerMangaDisplay, &rdest);
  if(v < 0)
    fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
  if(!iAffiche)
  { rsour.x = 0;
    rsour.y = 0;
    rsour.w = OPT_LARGE;
    rsour.h = OPT_HAUTE;
    rdest.x = LARG_ECR_RAM;
    rdest.y = 16;
    rdest.w = OPT_LARGE;
    rdest.h = OPT_HAUTE;
    rdest.y += display_offset_y;
    v = SDL_BlitSurface(pEcranOptXImage, &rsour, pPowerMangaDisplay, &rdest);
    if(v < 0)
      fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
  }

  //display score panel
  if(!iAffiche)
  { rsour.x = 0; rsour.y = 0; rsour.w = SCR_LARGE; rsour.h = SCR_HAUTE;
    rdest.x = 0; rdest.y = 0; rdest.w = SCR_LARGE; rdest.h = SCR_HAUTE;
    rdest.y += display_offset_y;
    v = SDL_BlitSurface(pEcranScrXImage, &rsour, pPowerMangaDisplay, &rdest);
    if(v < 0)
      fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
  }

  //display options from option panel
  while (iOptionIndex >= 0)
  { int _iOptionX = pOptionChange[iOptionIndex][0];
    int _iOptionY = pOptionChange[iOptionIndex--][1];
    rsour.x = _iOptionX; rsour.y = _iOptionY;
    rsour.w = 28; rsour.h = 28;
    rdest.x = LARG_ECR_RAM + _iOptionX; rdest.y = 16 + _iOptionY;
    rdest.w = 28; rdest.h = 28;
    rdest.y += display_offset_y;
    v = SDL_BlitSurface(pEcranOptXImage, &rsour, pPowerMangaDisplay, &rdest);
    if(v < 0)
      fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
  }
  if(iBonusX2Affiche || iAffiche)
  { rsour.x = 41;  rsour.y = 171; rsour.w = 14; rsour.h = 8;
    rdest.x = 297; rdest.y = 187; rdest.w = 14; rdest.h = 8;
    rdest.y += display_offset_y;
    v = SDL_BlitSurface(pEcranOptXImage, &rsour, pPowerMangaDisplay, &rdest);
    if(v < 0)
      fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    iBonusX2Affiche = 0;
  }
  if(iBonusX4Affiche || iAffiche)
  { rsour.x = 41;  rsour.y = 5;  rsour.w = 14; rsour.h = 8;
    rdest.x = 297; rdest.y = 21; rdest.w = 14; rdest.h = 8;
    rdest.y += display_offset_y;
    v = SDL_BlitSurface(pEcranOptXImage, &rsour, pPowerMangaDisplay, &rdest);
    if(v < 0)
      fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    iBonusX4Affiche = 0;
  }

  //display player's energy
  if(iBarreNrjJoueurXWindow)
  { rsour.x = 210; rsour.y = 3; rsour.w = 100; rsour.h = 9;
    rdest = rsour;
    rdest.y += display_offset_y;
    v = SDL_BlitSurface(pEcranScrXImage, &rsour, pPowerMangaDisplay, &rdest);
    if(v < 0)
      fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    iBarreNrjJoueurXWindow = 0;
  }

  //display big-boss's energy
  if(iBarreNrjGardienXWindow)
  { rsour.x = 10; rsour.y = 3; rsour.w = 45; rsour.h = 9;
    rdest = rsour;
    rdest.y += display_offset_y;
    v = SDL_BlitSurface(pEcranScrXImage, &rsour, pPowerMangaDisplay, &rdest);
    if(v < 0)
      fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    iBarreNrjGardienXWindow = 0;
  }

  //display score number
  if(iScoreXWindow)
  { rsour.x = 68; rsour.y = 0; rsour.w = 128; rsour.h = 16;
    rdest = rsour;
    rdest.y += display_offset_y;
    v = SDL_BlitSurface(pEcranScrXImage, &rsour, pPowerMangaDisplay, &rdest);
    if(v < 0)
      fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    iScoreXWindow = 0;
  }
  iOptionIndex = -1;
  iAffiche = 1;

    v = SDL_BlitSurface(pEcranPlayanim, &rsour, pPowerMangaDisplay, &rsour);

    SDL_UpdateRect(pPowerMangaDisplay, 0, 0,
                   pPowerMangaDisplay->w, pPowerMangaDisplay->h);
}

//------------------------------------------------------------------------------
// SDL : display window in 640*400
// playfield 512x440 ; score panel 320x16 ; option panel 64x184
// iAffiche=0 : display all window 640x400
//
//
//------------------------------------------------------------------------------
void fenetre640x400()
{
  Uint32 v;
  int		starty;
  SDL_Rect rdest;
  SDL_Rect rsour;
  char *_pSource =
    ecran_ram + (BANDE_DE_CLIP * iLargeurEcranTotal) +
    (BANDE_DE_CLIP * iOctetsParPixel);

  //recopie l'ecran de jeu en doublant / recopy the screen by it doubling
  //("assembler_opt.S" or "gfxroutine.cpp")
  copy2X_512x440(_pSource,
                 ecran_ram640 + (iLargeurEcran * iOctetsParPixel * 32),
                 HAUT_ECR_RAM
  );


  // mode 640x480
  if (vmode2)
	  starty = 40;
  else
	  starty = 0;



  if(iAffiche)
  { rsour.x = 0; rsour.y = 32;
    rsour.w = LARG_ECR_RAM * 2; rsour.h = HAUT_ECR_RAM * 2;
	rdest.x = 0; rdest.y = 32 + starty;
    rdest.w = LARG_ECR_RAM * 2; rdest.h = HAUT_ECR_RAM * 2;
    v = SDL_BlitSurface(pEcranXImage640, &rsour, pPowerMangaDisplay, &rdest);
    if(v < 0)
      fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

    //display score number ?
    if(iScoreXWindow)
    {
      iScoreXWindow = 0;
      copy2X(ecran_scr + (68 * iOctetsParPixel),
             ecran_ram640 + (68 * iOctetsParPixel * 2), 128, 16,
             SCR_LARGE * iOctetsParPixel - 128 * iOctetsParPixel,
             (iLargeurEcran * 2 * iOctetsParPixel) -
             128 * iOctetsParPixel * 2);
      rsour.x = 68 * 2; rsour.y = 0; rsour.w = 128 * 2; rsour.h = 16 * 2;
      rdest.x = 68 * 2; rdest.y = starty; rdest.w = 128 * 2; rdest.h = 16 * 2;
      v = SDL_BlitSurface(pEcranXImage640, &rsour, pPowerMangaDisplay, &rdest);
      if(v < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    }

    if(iBonusX2Affiche || iAffiche)
    {
      copy2X(ecran_opt + (41 * iOctetsParPixel) +
             (171 * OPT_LARGE * iOctetsParPixel),
             ecran_ram640 + ((LARG_ECR_RAM + 41) * iOctetsParPixel * 2) +
             ((SCR_HAUTE + 171) * iLargeurEcran * iOctetsParPixel * 2), 14, 8,
             OPT_LARGE * iOctetsParPixel - 14 * iOctetsParPixel,
             (iLargeurEcran * iOctetsParPixel * 2) -
             14 * iOctetsParPixel * 2);
      rsour.x = (LARG_ECR_RAM + 41) * 2; rsour.y = (SCR_HAUTE + 171) * 2;
      rsour.w = 14 * 2; rsour.h = 8 * 2;
	  rdest.x = (LARG_ECR_RAM + 41) * 2; rdest.y = (SCR_HAUTE + 171) * 2 + starty;
      rdest.w = 14 * 2; rdest.h = 8 * 2;
      v = SDL_BlitSurface(pEcranXImage640, &rsour, pPowerMangaDisplay, &rdest);
      if(v < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
      iBonusX2Affiche = 0;
    }

    if(iBonusX4Affiche || iAffiche)
    {
      copy2X(ecran_opt + (41 * iOctetsParPixel) +
             (5 * OPT_LARGE * iOctetsParPixel),
             ecran_ram640 + ((LARG_ECR_RAM + 41) * iOctetsParPixel * 2) +
             ((SCR_HAUTE + 5) * iLargeurEcran * iOctetsParPixel * 2), 14, 8,
             OPT_LARGE * iOctetsParPixel - 14 * iOctetsParPixel,
             (iLargeurEcran * iOctetsParPixel * 2) -
             14 * iOctetsParPixel * 2);
      rsour.x = (LARG_ECR_RAM + 41) * 2; rsour.y = (SCR_HAUTE + 5) * 2;
      rsour.w = 14 * 2; rsour.h = 8 * 2;
	  rdest.x = (LARG_ECR_RAM + 41) * 2; rdest.y = (SCR_HAUTE + 5) * 2 + starty;
      rdest.w = 14 * 2; rdest.h = 8 * 2;
      v = SDL_BlitSurface(pEcranXImage640, &rsour, pPowerMangaDisplay, &rdest);
      if(v < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
      iBonusX4Affiche = 0;
    }

    //display options from option panel
    while (iOptionIndex >= 0)
    { int  _iOptionX = pOptionChange[iOptionIndex][0];
      int  _iOptionY = pOptionChange[iOptionIndex--][1];
      copy2X(ecran_opt + (_iOptionX * iOctetsParPixel) +
             (_iOptionY * OPT_LARGE * iOctetsParPixel),
             ecran_ram640 +
             ((LARG_ECR_RAM + _iOptionX) * iOctetsParPixel * 2) +
             ((SCR_HAUTE + _iOptionY) * iLargeurEcran * iOctetsParPixel * 2),
             28, 28, OPT_LARGE * iOctetsParPixel - 28 * iOctetsParPixel,
             (iLargeurEcran * iOctetsParPixel * 2) -
             28 * iOctetsParPixel * 2);
      rsour.x = (LARG_ECR_RAM + _iOptionX) * 2; rsour.y = (SCR_HAUTE + _iOptionY) * 2;
      rsour.w = 28 * 2; rsour.h = 28 * 2;
	  rdest.x = (LARG_ECR_RAM + _iOptionX) * 2; rdest.y = (SCR_HAUTE + _iOptionY) * 2 + starty;
      rdest.w = 28 * 2; rdest.h = 28 * 2;
      v = SDL_BlitSurface(pEcranXImage640, &rsour, pPowerMangaDisplay, &rdest);
      if(v < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    }

    //display player's ernergy
    if(iBarreNrjJoueurXWindow)
    {
      iBarreNrjJoueurXWindow = 0;
      copy2X(ecran_scr + (210 * iOctetsParPixel) +
             (3 * SCR_LARGE * iOctetsParPixel),
             ecran_ram640 + (210 * iOctetsParPixel * 2) +
             (3 * iLargeurEcran * iOctetsParPixel * 2), 100, 9,
             SCR_LARGE * iOctetsParPixel - 100 * iOctetsParPixel,
             (iLargeurEcran * iOctetsParPixel * 2) -
             100 * iOctetsParPixel * 2);
      rsour.x = 210 * 2; rsour.y = 3 * 2;
      rsour.w = 100 * 2; rsour.h = 9 * 2;
	  rdest.x = 210 * 2; rdest.y = 3 * 2 + starty;
      rdest.w = 100 * 2; rdest.h = 9 * 2;
      v = SDL_BlitSurface(pEcranXImage640, &rsour, pPowerMangaDisplay, &rdest);
      if(v < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    }
    if(iBarreNrjGardienXWindow)                                //display bigboss's energy
    {
      iBarreNrjGardienXWindow = 0;
      copy2X(ecran_scr + (10 * iOctetsParPixel) +
             (3 * SCR_LARGE * iOctetsParPixel),
             ecran_ram640 + (10 * iOctetsParPixel * 2) +
             (3 * iLargeurEcran * iOctetsParPixel * 2), 45, 9,
             SCR_LARGE * iOctetsParPixel - 45 * iOctetsParPixel,
             (iLargeurEcran * iOctetsParPixel * 2) -
             45 * iOctetsParPixel * 2);
      rsour.x = 10 * 2; rsour.y = 3 * 2;
      rsour.w = 45 * 2; rsour.h = 9 * 2;
      rdest.x = 10 * 2; rdest.y = 3 * 2 + starty;
      rdest.w = 45 * 2; rdest.h = 9 * 2;
      v = SDL_BlitSurface(pEcranXImage640, &rsour, pPowerMangaDisplay, &rdest);
      if(v < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
      /*XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
                pEcranXImage640, 10 * 2, 3 * 2, 10 * 2, 3 * 2, 45 * 2, 9 * 2);*/
    }


  }
  else
  { copy2X(ecran_scr, ecran_ram640, SCR_LARGE, SCR_HAUTE, 0,
           iLargeurEcran * iOctetsParPixel * 2 -
           (SCR_LARGE * 2 * iOctetsParPixel));
    copy2X(ecran_opt,
           ecran_ram640 + (LARG_ECR_RAM * 2 * iOctetsParPixel) +
           (iLargeurEcran * iOctetsParPixel * 32), OPT_LARGE, OPT_HAUTE, 0,
           iLargeurEcran * iOctetsParPixel * 2 -
           (OPT_LARGE * 2 * iOctetsParPixel));
    rsour.x = 0; rsour.y = 0; rsour.w = iLargeurEcran; rsour.h = iHauteurEcran;
    rdest.x = 0; rdest.y = starty; rdest.w = iLargeurEcran; rdest.h = iHauteurEcran;
    v = SDL_BlitSurface(pEcranXImage640, &rsour, pPowerMangaDisplay, &rdest);
    if(v < 0)
      fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    iAffiche = 1;
    iOptionIndex = -1;
  }

  SDL_UpdateRect(pPowerMangaDisplay, 0, starty, pPowerMangaDisplay->w, pPowerMangaDisplay->h - starty);
}




//------------------------------------------------------------------------------
// SDL : ferme l'affichage sous SDL
//------------------------------------------------------------------------------
int xw_kill()
{
  libereXImages();
  ecran_ram = 0;
  pEcranXImage = 0;
  ecran_ram640 = 0;
  pEcranXImage640 = 0;
  ecran_scr = 0;
  pEcranScrXImage = 0;
  ecran_opt = 0;
  pEcranOptXImage = 0;
  SDL_Quit();
#ifdef _VERBEUX_
  if(power_conf->verbose)
    afficheMessage("> sdl_tlk.cpp/xw_kill() / SDL_Quit");
#endif
  return 1;
}


//------------------------------------------------------------------------------
// decompress pcx file and convert to 8 bits (no change), or 16 bits or 24 bits
// input  => _pFichier      : filename
// output <= _pStructureGfx : pointer to the "sDescriptionGfx" structure / 0=error
//------------------------------------------------------------------------------
char *Xload_pcx(char *_pNomFichier)
{
  sDescriptionGfx *_pGfx = load_pcx(_pNomFichier);
  if(!_pGfx)
    return 0;
  if(iOctetsParPixel > 0)
  { if(!convertit_16ou24(_pGfx))
    { afficheErreur("convertit_16ou24() failed\n", "sdl_tlk.cpp/Xload_pcx()");
      return 0;
    }
  }
  char *_pMem = _pGfx->pAdresse;
  libereMemoire((char *)_pGfx);
  return _pMem;
}

//------------------------------------------------------------------------------
// decompress pcx file and convert to 8 bits (no change), or 16 bits or 24 bits
// input => _pFichier : filename
//       => _pMem     : memory (XImage)
// output <= _pStructureGfx : pointer to the "sDescriptionGfx" structure / 0=error
//------------------------------------------------------------------------------
int Xload_pcx(char *_pNomFichier, char *_pMem)
{
  sDescriptionGfx *_pGfx = load_pcx(_pNomFichier);
  if(!_pGfx)
    return 0;
  if(iOctetsParPixel > 0)
  { if(!convertit_16ou24(_pGfx))
    { afficheErreur("convertit_16ou24() failed\n", "sdl_tlk.cpp/Xload_pcx()");
      return 0;
    }
  }
  char *_pMemBase = _pGfx->pAdresse;
  for(unsigned _iIndex = 0; _iIndex < _pGfx->iTaille; _iIndex++)
    _pMem[_iIndex] = _pMemBase[_iIndex];
  libereMemoire(_pMemBase);
  libereMemoire((char *)_pGfx);
  return 1;
}

//------------------------------------------------------------------------------
// SDL : converti une image en 256 couleurs en millier ou millions
// entree       =>  sDescriptionGfx : description de l'image
//------------------------------------------------------------------------------
char *convertit_16ou24(sDescriptionGfx * _pSrc)
{
  if(iOctetsParPixel == 1)
    return _pSrc->pAdresse;
  unsigned int _iTaille = (_pSrc->iLargeur * _pSrc->iHauteur);
  char *_pBuffer =
    reserveMemoire(_iTaille * iOctetsParPixel);
  if(!_pBuffer)
  { afficheErreur("out of memory", "sdl_tlk.cpp/convertit_16ou24()");
    return 0;
  }
  switch (iOctetsParPixel)
  { case 2:
      conv8_16(_pSrc->pAdresse, _pBuffer, pal16, _iTaille);
      break;
    case 3:
      conv8_24(_pSrc->pAdresse, _pBuffer, pal32, _iTaille);
      break;
    case 4:
      conv8_32(_pSrc->pAdresse, _pBuffer, pal32, _iTaille);
      break;
  }
  libereMemoire(_pSrc->pAdresse);
  _pSrc->pAdresse = _pBuffer;
  _pSrc->iTaille = _iTaille * iOctetsParPixel;
  return _pBuffer;
}

//------------------------------------------------------------------------------
// SDL : create a SDL surface
//------------------------------------------------------------------------------
SDL_Surface *creeXImage(unsigned int _iLarge, unsigned int _iHaute)
{
  Uint32 rmask, gmask, bmask;
  int _iXimageLibre = -1;
  SDL_Surface *_pXImage;
  unsigned int _iIndex;
  for(_iIndex = 0; _iIndex < iXIMAGEMAX; _iIndex++)
  { if(pListeXImage[_iIndex] == 0)
    { _iXimageLibre = _iIndex;
      break;
    }
  }
  if(_iXimageLibre < 0)
  { afficheErreur("out of pListeXImage list", "sdl_tlk.cpp/creeXImage()");
    return 0;
  }
  switch(iProfondeur2)
  { case 15:
      rmask = 0x7c00;
      gmask = 0x03e0;
      bmask = 0x001f;
      break;
    case 16:
      rmask = 0xf800;
      gmask = 0x03e0;
      bmask = 0x001f;
      break;
    default:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
      rmask = 0xff000000;
      gmask = 0x00ff0000;
      bmask = 0x0000ff00;
#else
      rmask = 0x000000ff;
      gmask = 0x0000ff00;
      bmask = 0x00ff0000;
#endif
      break;
  }
  _pXImage = SDL_CreateRGBSurface(
    SDL_ANYFORMAT, _iLarge, _iHaute, iProfondeur2, rmask, gmask, bmask, 0x00
  );
  if(!_pXImage)
  { fprintf(
      stderr, "! sdl_tlk.cpp/creeXImage() : SDL_CreateRGBSurface() %s\n",
      SDL_GetError()
    );
    return 0;
  }
  if(iOctetsParPixel == 1)
    SDL_SetPalette(_pXImage, SDL_PHYSPAL | SDL_LOGPAL, pXColor, 0, 256);
  pListeXImage[_iXimageLibre] = _pXImage;
  iNombreXImage++;
#ifdef _VERBEUX_
  if(power_conf->verbose)
    fprintf(stdout,
      "> sdl_tlk.cpp/creeXImage(): SDL_CreateRGBSurface(%i,%i,%i)\n",
      _iLarge,_iHaute,iProfondeur2);
#endif
  return _pXImage;
}

//------------------------------------------------------------------------------
// SDL : free a SDL surface
//------------------------------------------------------------------------------
void libereXImage(SDL_Surface *_pXImage)
{
  Uint32 w;
  Uint32 h;
  unsigned int _iIndex;
  for(_iIndex = 0; _iIndex < iXIMAGEMAX; _iIndex++)
  { if(pListeXImage[_iIndex] == _pXImage)
    { w = _pXImage->w;
      h = _pXImage->h;
      SDL_FreeSurface(_pXImage);
      pListeXImage[_iIndex] = 0;
      iNombreXImage--;
#ifdef _VERBEUX_
      if(power_conf->verbose)
        fprintf(stdout,
          "> sdl_tlk.cpp/libereXImage(): SDL_FreeSurface; %i,%i\n", w, h);
#endif
      break;
    }
  }
}

//------------------------------------------------------------------------------
// SDL : free all surfaces
//------------------------------------------------------------------------------
void libereXImages()
{
  unsigned int _iIndex;
  Uint32 w;
  Uint32 h;
  for(_iIndex = 0; _iIndex < iXIMAGEMAX; _iIndex++)
  { SDL_Surface *_pXImage = pListeXImage[_iIndex];
    if(_pXImage)
    { w = _pXImage->w;
      h = _pXImage->h;
      SDL_FreeSurface(_pXImage);
      pListeXImage[_iIndex] = 0;
      iNombreXImage--;
#ifdef _VERBEUX_
      if(power_conf->verbose)
        fprintf(stdout,
          "> sdl_tlk.cpp/libereXImage(): SDL_FreeSurface; %i,%i\n", w, h);
#endif
    }
  }
}

void verouille()
{ if(SDL_LockSurface(pEcranXImage))
  { fprintf(stderr, "> sdl_tlk.cpp/verouiller(): Impossible de verouiller la surface  %s\n", SDL_GetError());
  }
}

void deverouille()
{ SDL_UnlockSurface(pEcranXImage);
}
#endif

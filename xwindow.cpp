//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "xwindow.cpp"
// created      : 1999-08-17
// updates      : 2005-01-08
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
#ifdef XFREE_TLK
#define UTILISE_XSHM	1                                      //use XSHM Ximage
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/extensions/xf86dga.h>
#ifdef UTILISE_XSHM
#include <X11/extensions/XShm.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#endif
//------------------------------------------------------------------------------
Display                *pPowerMangaDisplay = 0;                //pointeur 
Window                  pFenetreRacine = 0;                    //pointeur fenetre racine (4 octets=pointeur)
Window                  pFenetreJeu = 0;                       //pointeur fenetre de notre jeu (4 octets=pointeur)
XSetWindowAttributes    sAttributFenetre;
GC                      pContexteGraphique = 0;                //(4 octets=pointeur)
XGCValues               sValeursGC;
XEvent                  sEvenements;
XImage                 *pEcranXImage = 0;                      //(4 octets=pointeur) XImage de 512*440
XImage                 *pEcranXImage640 = 0;                   //(4 octets=pointeur) XImage de 640*400
XImage                 *pEcranOptXImage = 0;                   //(4 octets=pointeur) XImage de 64*184
XImage                 *pEcranScrXImage = 0;                   //(4 octets=pointeur) XImage de 320*16
unsigned int            iOctetsParPixel = 0;                   //nombre d'octets par pixels (1=256 couleurs / 2=32768 couleurs)
XSizeHints              sTailleIndications;
unsigned char          *pal = 0;                               //palette de couleurs 24 bits (3 octets par couleur)
unsigned int           *pal32 = 0;                             //palette de couleurs 24 bits (4 octets par couleur) 
unsigned short         *pal16 = 0;                             //palette de couleurs 16 bits (2 octets par couleur)
unsigned short         *pal16PlayAnim = 0;                     //palette de couleurs 16 bits (2 octets par couleur)
unsigned int           *pal32PlayAnim = 0;                     //palette de couleurs 16 bits (2 octets par couleur)
XImage                 *pEcranPlayanim = 0;                    //(4 octets=pointeur) XImage de 320*200
XPixmapFormatValues     pixmapFormat;
const unsigned int      iXIMAGEMAX = 100;
unsigned int            iNombreXImage = 0;
XImage                 *pListeXImage[iXIMAGEMAX];
Atom                    protocols;
#ifdef UTILISE_XSHM
XShmSegmentInfo         SHMInfo[iXIMAGEMAX];
#endif

//...............................................................................
unsigned int            iLargeurRacine = 0;                    //largeur de l'ecran x-window
unsigned int            iHauteurRacine = 0;                    //hauteur de l'ecran x-window
unsigned int            iProfondeur = 0;                       //profondeur en bits de l'ecran x-window
unsigned int            iProfondeur2 = 0;                      //profondeur en bits de l'ecran x-window
unsigned int            iLargeurEcran = 0;                     //largeur de notre fenetre
unsigned int            iHauteurEcran = 0;                     //hauteur de notre fenetre
int                     iCouleurNoire = 0;
int                     iCouleurBlanche = 0;
unsigned int            iLargeurEcranTotal = 0;                //taille d'une ligne du buffer 'ecran_ram'
int                     vmode = 1;                             //0=resolution de 320x200 / 1=640*400 / 2=640*400 (double pixels)


//..............................................  ................................
int                     fullscreen = 0;                        //1=marche en plein ecran
int                     iDGAFlags;
char                   *pDGAmemoire;
int                     iDGAtailleLigne;                       //taille d'une ligne en pixels
int                     iDGAbanksize;                          //taille d'un banc en octets
int                     iDGAtailleMemoire;                     //taille memoire en kilo-octets
int                     iDGALargeur;                           //xwindow's screen width in pixels
int                     iDGAHauteur;                           //xwindow's screen height in pixels 
char                   *ecran_playanim = 0;                    //memory buffer 320*200
char                   *ecran_ram640 = 0;                      //memory buffer 640*400
char                   *ecran_scr = 0;                         //memory buffer 320*016 (score bar-line)
char                   *ecran_opt = 0;                         //memory buffer 064*184 (option panel)
unsigned int            iAffiche = 0;                          //1=update display option panel and bareline's score

//..............................................................................
extern bool             bar_nrj_player;
extern char            *ecran_ram;                             //adresse du buffer de 512x440 (XImage)
extern short            touch[500];                            //tableau des flags de toutes les touches clavier
extern int              quit_game;                             //1=demande a quitter le jeu

//..............................................................................
void                    initialiseCurseurBlanc();
void                    affiche_ecran();
void                    afficheAnim();
void                    fenetre320x200();
void                    fenetre640x400();
void                    dga320x200();
void                    dga640x400();
XImage                 *creeXImage(unsigned int _iLarge,
                                   unsigned int _iHaute);
void                    libereXImage(XImage * _pXImage);
void                    libereXImages();

//display optimisation for options .............................................
extern unsigned int     pOptionChange[22][2];                  //display options, positions in the XWindow
extern int              iOptionIndex;                          //index to the "pOptionChange" table, display options
extern unsigned int     iBonusX2Affiche;                       //display bonusX2 in our XWindow
extern unsigned int     iBonusX4Affiche;                       //display bonusX4 in out XWindow
extern unsigned char   *image1;                                //pointer to the buffer for the full screen animations
extern unsigned int     iBarreNrjJoueurXWindow;                //1=update player's enerny bar-line 
extern unsigned int     iBarreNrjGardienXWindow;               //1=update bigboss's energy bar-line
extern unsigned int     iScoreXWindow;                         //1=update player's score points
extern configfile*	power_conf;

//..............................................................................
const int               IMASQUE_EVENEMENTS =
  KeyPressMask | KeyReleaseMask | ExposureMask | FocusChangeMask | StructureNotifyMask;
Colormap                pTableCouleur;
XColor                  sCouleur;
XColor                 *pXColor = 0;                           //tableau XColor

//XF86VidModeModeInfo   VidModeInfo_sauve;                      //(32 octets=structure)
/*typedef struct {
    CARD32	dotclock B32;							4				//pixel clock
    CARD16	hdisplay B16;							2				//number of display pixels horizontally 
    CARD16	hsyncstart B16;						2				//horizontal sync start
    CARD16	hsyncend B16;							2				//horizontal sync end 
    CARD16	htotal B16;								2				//total horizontal pixels 
    CARD16	vdisplay B16;							2				//number of display pixels vertically
    CARD16	vsyncstart B16;						2				//Vertical sync start
    CARD16	vsyncend B16;							2				//vertical sync start 
    CARD16	vtotal B16;								2				//total vertical pixels 
    CARD32	flags B32;								4				//mode flags 
    CARD32	privsize B32;											//Size of private 
} xXF86VidModeModeInfo;
*/

//------------------------------------------------------------------------------
// xwindow : initialise un affichage sous xwindow / initialize xfree86
// sortie       <= 0 : erreur
//------------------------------------------------------------------------------
int xw_init()
{
  for(unsigned int _iIndex = 0; _iIndex < iXIMAGEMAX; _iIndex++)
    pListeXImage[_iIndex] = 0;

  //..............................................................................
#ifdef _VERBEUX_
  if(power_conf->verbose)
    afficheMessage("xwindow/xw_init() : start : xwindow intialize");
#endif
  if(!vmode)
  {
    iLargeurEcran = LARG_ECRAN_PHY;
    iHauteurEcran = HAUT_ECRAN_PHY;
  }
  else
  {
    iLargeurEcran = LARG_ECRAN_PHY * 2;
    iHauteurEcran = HAUT_ECRAN_PHY * 2;
  }
  //ouvertue d'un "display" (connexion socket au serveur X) ......................
  if(!pPowerMangaDisplay)
  {
    pPowerMangaDisplay = XOpenDisplay(0);
    if(!pPowerMangaDisplay)
    {
      afficheErreur("XOpenDisplay failed !", "xwindow.cpp/xw_init()");
      return 0;
    }
  }

  //demande la fenetre racine ....................................................
  if(!pFenetreRacine)
  {
    pFenetreRacine = DefaultRootWindow(pPowerMangaDisplay);
    if(!pFenetreRacine)
    {
      afficheErreur("DefaultRootWindow failed !", "xwindow.cpp/xw_init()");
      return 0;
    }
  }

  //..............................................................................
  int _iEcran = DefaultScreen(pPowerMangaDisplay);

  iCouleurNoire = BlackPixel(pPowerMangaDisplay, _iEcran);
  iCouleurBlanche = WhitePixel(pPowerMangaDisplay, _iEcran);
  iProfondeur = DefaultDepth(pPowerMangaDisplay, _iEcran);
  if(iProfondeur < 8)
  {
    afficheErreur("this game need 8 bits peer pixels minimum (256 colors)",
                  "xwindow.cpp/xw_init()");
    return 0;
  }
  iProfondeur2 = iProfondeur;
  int formatOk, formatCount;
  XPixmapFormatValues    *_pFormatListe;

  _pFormatListe = XListPixmapFormats(pPowerMangaDisplay, &formatCount);
  if(_pFormatListe == NULL)
  {
    afficheErreur("cannot get pixmap list format", "xwindow.cpp/xw_init()");
    return 0;
  }
  formatOk = -1;
  for(int i = 0; i < formatCount; i++)
    if(_pFormatListe[i].depth == (int)(iProfondeur))
      formatOk = i;
  if(formatOk == -1)
  {
    afficheErreur("erreur profondeur ecran", "xwindow.cpp/xw_init()");
    return 0;
  }
  memcpy(&pixmapFormat, &_pFormatListe[formatOk], sizeof(pixmapFormat));
  XFree(_pFormatListe);
  if(iProfondeur == 24)
    iProfondeur = pixmapFormat.bits_per_pixel;
  switch (iProfondeur)
  {
    case 8:
      iOctetsParPixel = 1;
      break;
    case 15:
      iOctetsParPixel = 2;
      break;
    case 16:
      iOctetsParPixel = 2;
      break;
    case 24:
      iOctetsParPixel = 3;
      break;
    case 32:
      iOctetsParPixel = 4;
      break;
  }
  iLargeurRacine = DisplayWidth(pPowerMangaDisplay, _iEcran);
  iHauteurRacine = DisplayHeight(pPowerMangaDisplay, _iEcran);
#ifdef _VERBEUX_
  if(power_conf->verbose)
  {
    printf
      ("depth of screen : %i ; bytes per pixel:%i ; size screen : (%i,%i) ;\n",
       iProfondeur, iOctetsParPixel, iLargeurRacine, iHauteurRacine);
    printf
      ("iProfondeur=%i formatOk=%i formatCount=%i pixmapFormat.scanline_pad=%i\n",
       iProfondeur, formatOk, formatCount, pixmapFormat.scanline_pad);
  }
#endif

  //creation d'un contexte graphique --------------------------------------------
  if(!fullscreen)
  {
    if(!pContexteGraphique)
    {
      sValeursGC.foreground = iCouleurBlanche;
      sValeursGC.background = iCouleurNoire;
      pContexteGraphique = XCreateGC(pPowerMangaDisplay,       //specifie la connection au serveur x
                                     pFenetreRacine,           //
                                     (GCForeground | GCBackground),     //
                                     &sValeursGC               //
        );
      if(!pContexteGraphique)
      {
        afficheErreur("XCreateGC failed failed !", "xwindow.cpp/xw_init()");
        return 0;
      }
    }
  }
  //attributs de la fenetre ------------------------------------------------------
  sAttributFenetre.background_pixel = iCouleurNoire;
  sAttributFenetre.border_pixel = iCouleurNoire;
  sAttributFenetre.event_mask = IMASQUE_EVENEMENTS;

  //creation d'une fenetre -------------------------------------------------------
  pFenetreJeu = XCreateWindow(pPowerMangaDisplay,              //pointeur sur un 'Display'
                              pFenetreRacine, (iLargeurRacine - iLargeurEcran) >> 1,    //abscisse de la fenetre
                              (iHauteurRacine - iHauteurEcran) >> 1,    //ordonnee de la fenetre
                              iLargeurEcran,                   //largeur de la fenetre
                              iHauteurEcran,                   //hauteur de la fenetre
                              5,                               //largeur du bord de la fenetre de zero pixel
                              iProfondeur2,                    //prondeur ecran en bits (on prend celle par defaut)
                              InputOutput,                     //la fenetre peut recevoir entrees et afficher des resultats
                              CopyFromParent, CWEventMask | CWBackPixel | CWBorderPixel, &sAttributFenetre      //structure "XSetWindowAttributes"
    );
  if(!pFenetreJeu)
  {
    afficheErreur("XCreateWindow failed", "xwindow.cpp/xw_init()");
    return 0;
  }
  XStoreName(pPowerMangaDisplay, pFenetreJeu, "PowerManga by TLK Games");
  XSelectInput(pPowerMangaDisplay, pFenetreJeu, IMASQUE_EVENEMENTS);

 
  protocols = XInternAtom(pPowerMangaDisplay, "WM_DELETE_WINDOW", 0);
  XSetWMProtocols(pPowerMangaDisplay, pFenetreJeu, &protocols, 1);
  
  //donne les specificites de notre fenetre  -------------------------------------
  sTailleIndications.flags = PPosition | PMinSize | PMaxSize;
  sTailleIndications.x = (iLargeurRacine - iLargeurEcran) >> 1;
  sTailleIndications.y = (iHauteurRacine - iHauteurEcran) >> 1;
  sTailleIndications.x = (iLargeurRacine - iLargeurEcran) >> 1;
  sTailleIndications.y = (iHauteurRacine - iHauteurEcran) >> 1;
  sTailleIndications.min_width = iLargeurEcran;
  sTailleIndications.min_height = iHauteurEcran;
  sTailleIndications.max_width = iLargeurEcran;
  sTailleIndications.max_height = iHauteurEcran;
  XSetWMNormalHints(pPowerMangaDisplay, pFenetreJeu, &sTailleIndications);
  XClearWindow(pPowerMangaDisplay, pFenetreJeu);               //efface la fenetre entierement / clear the window
  XMapRaised(pPowerMangaDisplay, pFenetreJeu);                 //affiche la fenetre / display the window
  XFlush(pPowerMangaDisplay);

  // enable DGA mode / active le mode DGA ........................................
  if(fullscreen == 1)                                          //marche en plein ecran / fullscreen DGA ?
  {
    XF86DGAQueryDirectVideo(pPowerMangaDisplay, _iEcran, &iDGAFlags);
    if(!(iDGAFlags & XF86DGADirectPresent))
    {
      afficheErreur("DGA's not available !", "xwindow/xw_init()");
      return 0;
    }
    XF86DGAGetVideo(pPowerMangaDisplay, _iEcran, &pDGAmemoire,
                    &iDGAtailleLigne, &iDGAbanksize, &iDGAtailleMemoire);
    XF86DGAGetViewPortSize(pPowerMangaDisplay, _iEcran, &iDGALargeur,
                           &iDGAHauteur);
#ifdef _VERBEUX_
    if(power_conf->verbose)
    {
      printf
        ("pDGAmemoire=%X ; iDGAtailleLigne=%i ; iDGAbanksize=%i  ; iDGAtailleMemoire=%i ; iDGALargeur=%i ; iDGAHauteur=%i\n",
         (int)(pDGAmemoire), iDGAtailleLigne, iDGAbanksize, iDGAtailleMemoire,
         iDGALargeur, iDGAHauteur);
    }
#endif
    XF86DGADirectVideo(pPowerMangaDisplay, _iEcran,
                       XF86DGADirectGraphics | XF86DGADirectKeyb);
    memset(pDGAmemoire, 0, iDGALargeur * iDGAHauteur * iOctetsParPixel);

  }

  initialiseCurseurBlanc();
#ifdef _VERBEUX_
  if(power_conf->verbose)
    printf
      ("xwindow/xw_init() : end xwindow intialisation fullscreen=%i vmode=%i  iLargeurEcran=%i\n",
       fullscreen, vmode, iLargeurEcran);
#endif

  // profondeur ecran = 8 bits / screen depth = 8 bits ...........................
  if(iOctetsParPixel == 1)
  {
    pTableCouleur =
      XCreateColormap(pPowerMangaDisplay, pFenetreJeu,
                      DefaultVisual(pPowerMangaDisplay, _iEcran), AllocAll);
    XSetWindowColormap(pPowerMangaDisplay, pFenetreJeu, pTableCouleur);
  }

  //printf("==> %i\n", sizeof(XF86VidModeModeInfo)) ;
  //XF86VidModeModeInfo* ii=&VidModeInfo_sauve ;
  //XF86VidModeModeLine *l = (XF86VidModeModeLine*)((char*)(ii + sizeof ii->dotclock));
  //unsigned int _iErreur=XF86VidModeGetModeLine(pPowerMangaDisplay, _iEcran, (int*)&ii->dotclock, l);
  //printf("==> erreur=%i\n", _iErreur) ;
  //printf("dotclock=%i ; hdisplay=%i ; hsyncend=%i ; htotal=%i ; vdisplay=%i ; vsyncstart=%i ; vsyncend=%i ; vtotal=%i ; flags=%i ; privsize=%i", 
  //ii->dotclock, ii->hdisplay, ii->hsyncend, ii->htotal, ii->vdisplay, ii->vsyncstart, ii->vsyncend, ii->vtotal, ii->flags, ii->privsize) ;  

  return 1;
}

//------------------------------------------------------------------------------
// xwindow destroy XImage / start anim and end anim / "playanim.cpp"
//------------------------------------------------------------------------------
void kill_ecranPlayanim()
{
  libereXImage(pEcranPlayanim);
  ecran_playanim = 0;
  pEcranPlayanim = 0;
}

//------------------------------------------------------------------------------
// xwindow : create XImage / start anim and end anim / "playanim.cpp"
//------------------------------------------------------------------------------
int reserve_ecranPlayanim()
{
  if(!(pEcranPlayanim = creeXImage(LARG_ECRAN_PHY, HAUT_ECRAN_PHY)))
    return 0;
  ecran_playanim = pEcranPlayanim->data;
  return 1;                                                    //no error
}

//------------------------------------------------------------------------------
// xwindow : create 3 or 4 XImages for the game 
//------------------------------------------------------------------------------
int reserve_ecran_ram()
{                                                              //create XImage "ecran_ram" 512*440
  if(!(pEcranXImage = creeXImage(LARG_ECR_RAM_TOT, HAUT_ECR_RAM_TOT)))
    return 0;
  ecran_ram = pEcranXImage->data;
  iLargeurEcranTotal = LARG_ECR_RAM_TOT * iOctetsParPixel;
  //create XImage 640x400 (window 640x400)
  if(vmode == 1 && !fullscreen)
  {
    if(!(pEcranXImage640 = creeXImage(iLargeurEcran, iHauteurEcran)))
      return 0;
    ecran_ram640 = pEcranXImage640->data;
  }
  if(!(pEcranOptXImage = creeXImage(OPT_LARGE, OPT_HAUTE)))
    return 0;
  ecran_opt = pEcranOptXImage->data;
  if(!(pEcranScrXImage = creeXImage(SCR_LARGE, SCR_HAUTE)))
    return 0;
  ecran_scr = pEcranScrXImage->data;
  return 1;
}

//------------------------------------------------------------------------------
// xwindow :    create palette 16 bits and 24 bits
//                                              recopie la palette 24 bits et cree une palette 16 bits
// entree               => _pNomFichier : filname's palette /  nom du fichier palette
//------------------------------------------------------------------------------
int charge_palette(char *_pNomFichier)
{
  unsigned int            _iIndex;
  unsigned char          *_pPal;

  if(!pal)
    pal = (unsigned char *)chargeFichier(_pNomFichier);
  if(!pal)
    return 0;

  //ecran 8 bits 256 couleurs  . . . . . . . . . . . . . . . . . . . . . . . . . . 
  if(iOctetsParPixel == 1)
  {
    if(!pXColor)
      pXColor = (XColor *) reserveMemoire(sizeof(XColor) * 256);
    if(!pXColor)
    {
      afficheErreur("'pXColor' out of memory", "xwindow/charge_palette()");
      return 0;
    }
    unsigned char          *_p = (unsigned char *)pal;

    for(_iIndex = 0; _iIndex < 256; _iIndex++)
    {
      pXColor[_iIndex].red = _p[0] << 8;
      pXColor[_iIndex].green = _p[1] << 8;
      pXColor[_iIndex].blue = _p[2] << 8;
      pXColor[_iIndex].pixel = _iIndex;
      pXColor[_iIndex].flags = DoRed | DoGreen | DoBlue;
      _p += 3;
    }
    XStoreColors(pPowerMangaDisplay, pTableCouleur, pXColor, 256);
    XInstallColormap(pPowerMangaDisplay, pTableCouleur);
  }
  else
    //ecran 16 bits 65536 couleurs   . . . . . . . . . . . . . . . . . . . . . . . . 
  {
    if(iOctetsParPixel == 2)
    {
      if(!pal16)
        pal16 = (unsigned short *)reserveMemoire(256 * 2);
      if(!pal16)
      {
        afficheErreur("'pal16' out of memory", "xwindow/charge_palette()");
        return 0;
      }
      if(iProfondeur2 == 15)
        convertitPalette24_15(pal, pal16);
      else
        convertitPalette24_16(pal, pal16);
    }
    else
      //ecran 24 ou 32 bits  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
    {
      if(iOctetsParPixel > 2)
      {
        if(!pal32)
          pal32 = (unsigned int *)reserveMemoire(256 * 4);
        if(!pal32)
        {
          afficheErreur("'pal32' out of memory", "xwindow/charge_palette()");
          return 0;
        }
        unsigned char          *_p = (unsigned char *)pal32;

        _pPal = pal;
        for(_iIndex = 0; _iIndex < 256; _iIndex++)             //_p[3]=? / _p[2]=red / _p[1]=green / _p[0]=blue
        {

#if __BYTE_ORDER == __BIG_ENDIAN
          _p[3] = _pPal[2];
          _p[2] = _pPal[1];
          _p[1] = _pPal[0];
          _p[0] = 0;
#else          
          _p[0] = _pPal[2];
          _p[1] = _pPal[1];
          _p[2] = _pPal[0];
          _p[3] = 0;
#endif
          _p += 4;
          _pPal += 3;
        }
      }
    }
  }
  return 1;
}

//------------------------------------------------------------------------------
// xwindow : gestion des evenements
//------------------------------------------------------------------------------
int xw_exec()
{
  int num_events;
  XKeyEvent *_pXKeyEvent;

  XFlush(pPowerMangaDisplay);
  num_events = XPending(pPowerMangaDisplay);
  //boucle des evenements ........................................................
  while ((num_events != 0))
  {
    num_events--;
    XNextEvent(pPowerMangaDisplay, &sEvenements);
    int                     _iAsciiKey;
    char                    _cASCII;
    KeySym                  _iKeySym;

    switch (sEvenements.type)
    {                                                          //touches enfoncees  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
      case KeyPress:
      {
        _pXKeyEvent = (XKeyEvent *) & sEvenements;
        _iAsciiKey =
          XLookupString((XKeyEvent *) & sEvenements, (char *)&_cASCII, 1,
                        &_iKeySym, NULL);
        //printf("KeyPress : _iAsciiKey=%i _cASCII=%c _iKeySym=%i cASCII=%i\n ", _iAsciiKey, _cASCII, _iKeySym, _cASCII) ;
        if(_iKeySym == XK_F10)
          quit_game = 1;                                       //touche F10
        //Keys_Down(_pXKeyEvent->keycode) ;
        //if(_iAsciiKey>0)  i_iAsciiKey=(int)_cASCII ;
        //else _iAsciiKey=_iKeySym ;
        //Keys_Down(_pXKeyEvent->keycode) ;
        //Keys_Down(_iAsciiKey) ;
        Keys_Down(_iKeySym);
      }
        break;
        //touches relachees  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
      case KeyRelease:
      {
        _pXKeyEvent = (XKeyEvent *) & sEvenements;
        _iAsciiKey =
          XLookupString((XKeyEvent *) & sEvenements, (char *)&_cASCII, 1,
                        &_iKeySym, NULL);
        Keys_Up(_iKeySym);
      }
        break;
        // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
      case Expose:
        iAffiche = 0;
        //if(iOctetsParPixel==1)        XInstallColormap(pPowerMangaDisplay, pTableCouleur) ;
        break;
        // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
      case FocusIn:
        break;
        // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
      case FocusOut:
        if(fullscreen == 1)
          quit_game = 1;                                       //in  DGA if leave the focus then quit the game !
        break;

      case ClientMessage:
        protocols = XInternAtom(pPowerMangaDisplay, "WM_PROTOCOLS", FALSE);
        if (sEvenements.xclient.message_type == protocols) {
          quit_game = 1;
         }
        break;

       // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
      default:
#ifdef _VERBEUX_
        if(power_conf->verbose)
          printf("xwindow.cpp / xw_exec() : evenement type=%i\n",
                 sEvenements.type);
#endif
        break;
    }
  }
  affiche_ecran();
  return 0;
}

//------------------------------------------------------------------------------
//****  xwindow : display scren ,window or full screen                      ****
//------------------------------------------------------------------------------
// joue animation / play compressed ani
void affiche_ecran()
{
  if(pEcranPlayanim)                                           // movie ?
  {
    iAffiche = 0;
    afficheAnim();                                             // movie player.
  }
  else
  {                                                            //affichage directement a l'ecran / display with DGA) --------------------------
    if(fullscreen == 1)
    {
      switch (vmode)
      {
        case 0:
          dga320x200();
          break;                                               // DGA 320*200
        case 1:
          dga640x400();
          break;                                               // DGA 640*400
      }
    }
    else
      //affichage dans une fenetre / display in a window -----------------------------
    {
      switch (vmode)
      {
        case 0:
          fenetre320x200();
          break;                                               // window 320*200
        case 1:
          fenetre640x400();
          break;                                               // window 640x480
      }
    }
  }
}

//------------------------------------------------------------------------------
// xwindow : display in DGA
//------------------------------------------------------------------------------
void dga320x200()
{
  char                   *_pSource;
  char                   *_pDestination;

  _pSource =
    ecran_ram + (BANDE_DE_CLIP * iLargeurEcranTotal) +
    (BANDE_DE_CLIP * iOctetsParPixel);
  int                     _iOffset =
    ((iDGALargeur - LARG_ECRAN_PHY) / 2) * iOctetsParPixel;
  _pDestination =
    pDGAmemoire + _iOffset +
    ((iDGAHauteur - HAUT_ECRAN_PHY) / 2 * iDGALargeur) * iOctetsParPixel;
  char                   *_pDestination2 =
    _pDestination + 16 * iDGALargeur * iOctetsParPixel;
  copie4octets(_pSource, _pDestination2, (LARG_ECR_RAM * iOctetsParPixel) / 4, HAUT_ECR_RAM, iLargeurEcranTotal - (LARG_ECR_RAM * iOctetsParPixel), iDGALargeur * iOctetsParPixel - (LARG_ECR_RAM * iOctetsParPixel));  //copie simple "assembler.S"
  if(iAffiche)
  {
    if(iScoreXWindow)                                          //display score number.........................................
    {
      iScoreXWindow = 0;
      copie4octets(ecran_scr + (68 * iOctetsParPixel),
                   _pDestination + (68 * iOctetsParPixel),
                   128 / 4 * iOctetsParPixel, 16,
                   SCR_LARGE * iOctetsParPixel - 128 * iOctetsParPixel,
                   (iDGALargeur * iOctetsParPixel) - 128 * iOctetsParPixel);
    }
    if(iBonusX2Affiche || iAffiche)                            //display bonus X2 .............................
    {
      copie4octets(ecran_opt + (41 * iOctetsParPixel) +
                   (171 * OPT_LARGE * iOctetsParPixel),
                   _pDestination + ((LARG_ECR_RAM + 41) * iOctetsParPixel) +
                   ((SCR_HAUTE + 171) * iDGALargeur * iOctetsParPixel),
                   16 / 4 * iOctetsParPixel, 8,
                   OPT_LARGE * iOctetsParPixel - 16 * iOctetsParPixel,
                   (iDGALargeur * iOctetsParPixel) - 16 * iOctetsParPixel);
      iBonusX2Affiche = 0;
    }
    if(iBonusX4Affiche || iAffiche)                            //display bonus x4 .............................
    {
      copie4octets(ecran_opt + (41 * iOctetsParPixel) +
                   (5 * OPT_LARGE * iOctetsParPixel),
                   _pDestination + ((LARG_ECR_RAM + 41) * iOctetsParPixel) +
                   ((SCR_HAUTE + 5) * iDGALargeur * iOctetsParPixel),
                   16 / 4 * iOctetsParPixel, 8,
                   OPT_LARGE * iOctetsParPixel - 16 * iOctetsParPixel,
                   (iDGALargeur * iOctetsParPixel) - 16 * iOctetsParPixel);
      iBonusX4Affiche = 0;
    }
    while (iOptionIndex >= 0)                                  //display options from option panel ....................
    {
      int                     _iOptionX = pOptionChange[iOptionIndex][0];
      int                     _iOptionY = pOptionChange[iOptionIndex--][1];

      copie4octets(ecran_opt + (_iOptionX * iOctetsParPixel) + (_iOptionY * OPT_LARGE * iOctetsParPixel), _pDestination + ((LARG_ECR_RAM + _iOptionX) * iOctetsParPixel) + ((SCR_HAUTE + _iOptionY) * iDGALargeur * iOctetsParPixel), 28 / 4 * iOctetsParPixel, 28, (OPT_LARGE - 28) * iOctetsParPixel, iDGALargeur * iOctetsParPixel - (28 * iOctetsParPixel));        //copie simple "assembler.S"
    }
    if(iBarreNrjJoueurXWindow)                                 //display player's energy .........................
    {
      iBarreNrjJoueurXWindow = 0;
      copie4octets(ecran_scr + (210 * iOctetsParPixel) +
                   (3 * SCR_LARGE * iOctetsParPixel),
                   _pDestination + (210 * iOctetsParPixel) +
                   (3 * iDGALargeur * iOctetsParPixel),
                   100 / 4 * iOctetsParPixel, 9,
                   SCR_LARGE * iOctetsParPixel - 100 * iOctetsParPixel,
                   (iDGALargeur * iOctetsParPixel) - 100 * iOctetsParPixel);
    }
    if(iBarreNrjGardienXWindow)                                //display bigboss's energy ........................
    {
      iBarreNrjGardienXWindow = 0;
      copie4octets(ecran_scr + (10 * iOctetsParPixel) +
                   (3 * SCR_LARGE * iOctetsParPixel),
                   _pDestination + (10 * iOctetsParPixel) +
                   (3 * iDGALargeur * iOctetsParPixel),
                   48 / 4 * iOctetsParPixel, 9,
                   SCR_LARGE * iOctetsParPixel - 48 * iOctetsParPixel,
                   (iDGALargeur * iOctetsParPixel) - 48 * iOctetsParPixel);
    }
  }
  else
  {
    copie4octets(ecran_scr, _pDestination, SCR_LARGE / 4 * iOctetsParPixel,
                 SCR_HAUTE, 0,
                 iDGALargeur * iOctetsParPixel -
                 (SCR_LARGE * iOctetsParPixel));
    copie4octets(ecran_opt,
                 _pDestination + (LARG_ECR_RAM * iOctetsParPixel) +
                 (iDGALargeur * iOctetsParPixel * 16),
                 OPT_LARGE / 4 * iOctetsParPixel, OPT_HAUTE, 0,
                 iDGALargeur * iOctetsParPixel -
                 (OPT_LARGE * iOctetsParPixel));
    iAffiche = 1;
    iOptionIndex = -1;
  }
}

//------------------------------------------------------------------------------
// xwindow : affiche en mode DGA 640*400 / display in DGA mode 640*400 
//------------------------------------------------------------------------------
void dga640x400()
{
  char                   *_pSource;
  char                   *_pDestination;

  _pSource =
    ecran_ram + (BANDE_DE_CLIP * iLargeurEcranTotal) +
    (BANDE_DE_CLIP * iOctetsParPixel);
  _pSource =
    ecran_ram + (BANDE_DE_CLIP * iLargeurEcranTotal) +
    (BANDE_DE_CLIP * iOctetsParPixel);
  {                                                            //int   _iOffset=((iDGALargeur-LARG_ECRAN_PHY*2)/2)*iOctetsParPixel ;
    _pDestination =
      pDGAmemoire + (((iDGALargeur - LARG_ECRAN_PHY * 2) / 2) +
                     (iDGAHauteur -
                      HAUT_ECRAN_PHY * 2) / 2 * iDGALargeur) *
      iOctetsParPixel;
    char                   *_pDestination2 =
      _pDestination + 32 * iDGALargeur * iOctetsParPixel;
    copy2X(_pSource, _pDestination2, LARG_ECR_RAM, HAUT_ECR_RAM, iLargeurEcranTotal - (LARG_ECR_RAM * iOctetsParPixel), iDGALargeur * iOctetsParPixel * 2 - (LARG_ECR_RAM * 2 * iOctetsParPixel));      // copie en doublant les pixels ("assembler.S")
    if(iAffiche)
    {
      if(iScoreXWindow)                                        //display score number .......................................
      {
        iScoreXWindow = 0;
        copy2X(ecran_scr + (68 * iOctetsParPixel),
               _pDestination + (68 * iOctetsParPixel * 2), 128, 16,
               SCR_LARGE * iOctetsParPixel - 128 * iOctetsParPixel,
               (iDGALargeur * 2 * iOctetsParPixel) -
               128 * iOctetsParPixel * 2);
      }
      if(iBonusX2Affiche || iAffiche)                          //display bonus X2 .............................
      {
        copy2X(ecran_opt + (41 * iOctetsParPixel) +
               (171 * OPT_LARGE * iOctetsParPixel),
               _pDestination + ((LARG_ECR_RAM + 41) * iOctetsParPixel * 2) +
               ((SCR_HAUTE + 171) * iDGALargeur * iOctetsParPixel * 2), 14, 8,
               OPT_LARGE * iOctetsParPixel - 14 * iOctetsParPixel,
               (iDGALargeur * iOctetsParPixel * 2) -
               14 * iOctetsParPixel * 2);
        iBonusX2Affiche = 0;
      }
      if(iBonusX4Affiche || iAffiche)                          //display bonus x4 .............................
      {
        copy2X(ecran_opt + (41 * iOctetsParPixel) +
               (5 * OPT_LARGE * iOctetsParPixel),
               _pDestination + ((LARG_ECR_RAM + 41) * iOctetsParPixel * 2) +
               ((SCR_HAUTE + 5) * iDGALargeur * iOctetsParPixel * 2), 14, 8,
               OPT_LARGE * iOctetsParPixel - 14 * iOctetsParPixel,
               (iDGALargeur * iOctetsParPixel * 2) -
               14 * iOctetsParPixel * 2);
        iBonusX4Affiche = 0;
      }
      while (iOptionIndex >= 0)                                //display options from option panel ....................
      {
        int                     _iOptionX = pOptionChange[iOptionIndex][0];
        int                     _iOptionY = pOptionChange[iOptionIndex--][1];

        copy2X(ecran_opt + (_iOptionX * iOctetsParPixel) +
               (_iOptionY * OPT_LARGE * iOctetsParPixel),
               _pDestination +
               ((LARG_ECR_RAM + _iOptionX) * iOctetsParPixel * 2) +
               ((SCR_HAUTE + _iOptionY) * iDGALargeur * iOctetsParPixel * 2),
               28, 28, OPT_LARGE * iOctetsParPixel - 28 * iOctetsParPixel,
               (iDGALargeur * iOctetsParPixel * 2) -
               28 * iOctetsParPixel * 2);
      }
      if(iBarreNrjJoueurXWindow)                               //display player's energy .........................
      {
        iBarreNrjJoueurXWindow = 0;
        copy2X(ecran_scr + (210 * iOctetsParPixel) +
               (3 * SCR_LARGE * iOctetsParPixel),
               _pDestination + (210 * iOctetsParPixel * 2) +
               (3 * iDGALargeur * iOctetsParPixel * 2), 100, 9,
               SCR_LARGE * iOctetsParPixel - 100 * iOctetsParPixel,
               (iDGALargeur * iOctetsParPixel * 2) -
               100 * iOctetsParPixel * 2);
      }
      if(iBarreNrjGardienXWindow)                              //display bigboss's energy ........................
      {
        iBarreNrjGardienXWindow = 0;
        copy2X(ecran_scr + (10 * iOctetsParPixel) +
               (3 * SCR_LARGE * iOctetsParPixel),
               _pDestination + (10 * iOctetsParPixel * 2) +
               (3 * iDGALargeur * iOctetsParPixel * 2), 45, 9,
               SCR_LARGE * iOctetsParPixel - 45 * iOctetsParPixel,
               (iDGALargeur * iOctetsParPixel * 2) -
               45 * iOctetsParPixel * 2);
      }
    }
    else
    {
      copy2X(ecran_scr, _pDestination, SCR_LARGE, SCR_HAUTE, 0,
             iDGALargeur * iOctetsParPixel * 2 -
             (SCR_LARGE * 2 * iOctetsParPixel));
      copy2X(ecran_opt,
             _pDestination + (LARG_ECR_RAM * 2 * iOctetsParPixel) +
             (iDGALargeur * iOctetsParPixel * 32), OPT_LARGE, OPT_HAUTE, 0,
             iDGALargeur * iOctetsParPixel * 2 -
             (OPT_LARGE * 2 * iOctetsParPixel));
      iAffiche = 1;
      iOptionIndex = -1;
    }
  }
}

//------------------------------------------------------------------------------
// xwindow : affiche l'animation compactee / display start movie and end movie
//------------------------------------------------------------------------------
void afficheAnim()
{
  switch (iOctetsParPixel)
  {
    case 1:
      for(int i = 0; i < (LARG_ECRAN_PHY * HAUT_ECRAN_PHY); i++)
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
  //mode plein ecran .............................................................
  if(fullscreen == 1)
  {
    char                   *_pDestination;

    switch (vmode)
    {
      case 0:                                                 //mode 320x200
      {
        int                     _iOffset =
          ((iDGALargeur - LARG_ECRAN_PHY) / 2) * iOctetsParPixel;
        _pDestination =
          pDGAmemoire + _iOffset +
          ((iDGAHauteur -
            HAUT_ECRAN_PHY) / 2 * iDGALargeur) * iOctetsParPixel;
        copie4octets(ecran_playanim, _pDestination, 320 / 4 * iOctetsParPixel,
                     200, 0,
                     iDGALargeur * iOctetsParPixel -
                     (iLargeurEcran * iOctetsParPixel));
      }
        break;
      case 1:                                                 //mode 640x400
      {
        _pDestination =
          pDGAmemoire + (((iDGALargeur - LARG_ECRAN_PHY * 2) / 2) +
                         (iDGAHauteur -
                          HAUT_ECRAN_PHY * 2) / 2 * iDGALargeur) *
          iOctetsParPixel;
        copy2X(ecran_playanim, _pDestination, 320, 200, 0,
               iDGALargeur * iOctetsParPixel * 2 -
               (iLargeurEcran * iOctetsParPixel));
      }
        break;
    }
  }
  //mode fenetree.................................................................
  else
  {
    switch (vmode)
    {
      case 0:                                                 //mode 320x200
      {
        XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
                  pEcranPlayanim, 0, 0, 0, 0, LARG_ECRAN_PHY, HAUT_ECRAN_PHY);
      }
        break;
      case 1:                                                 //mode 640x400
      {
        copy2X(ecran_playanim, ecran_ram640, 320, 200, 0,
               iLargeurEcran * iOctetsParPixel * 2 -
               (iLargeurEcran * iOctetsParPixel));
        XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
                  pEcranXImage640, 0, 0, 0, 0, iLargeurEcran, iHauteurEcran);
      }
        break;
    }
  }
}

//------------------------------------------------------------------------------
// xwindow : display window in 320*200
//------------------------------------------------------------------------------
void fenetre320x200()
{
  XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique, pEcranXImage,  //buffer source
            BANDE_DE_CLIP, BANDE_DE_CLIP,                      //coordonnees sources
            0, 16,                                             //coordonnees destination
            LARG_ECR_RAM, HAUT_ECR_RAM);
  if(!iAffiche)
    XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
              pEcranOptXImage, 0, 0, LARG_ECR_RAM, 16, OPT_LARGE, OPT_HAUTE);

  while (iOptionIndex >= 0)                                    //display options from option panel
  {
    int                     _iOptionX = pOptionChange[iOptionIndex][0];
    int                     _iOptionY = pOptionChange[iOptionIndex--][1];

    XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
              pEcranOptXImage, _iOptionX, _iOptionY, LARG_ECR_RAM + _iOptionX,
              16 + _iOptionY, 28, 28);
  }
  if(!iAffiche)                                                //display score panel
    XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
              pEcranScrXImage, 0, 0, 0, 0, SCR_LARGE, SCR_HAUTE);

  if(iBonusX2Affiche || iAffiche)
  {
    XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
              pEcranOptXImage, 41, 171, 297, 187, 14, 8);
    iBonusX2Affiche = 0;
  }
  if(iBonusX4Affiche || iAffiche)
  {
    XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
              pEcranOptXImage, 41, 5, 297, 21, 14, 8);
    iBonusX4Affiche = 0;
  }
  if(iBarreNrjJoueurXWindow)                                   //display player's energy
  {
    iBarreNrjJoueurXWindow = 0;
    XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
              pEcranScrXImage, 210, 3, 210, 3, 100, 9);
  }
  if(iBarreNrjGardienXWindow)                                  //display big-boss's energy
  {
    iBarreNrjGardienXWindow = 0;
    XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
              pEcranScrXImage, 10, 3, 10, 3, 45, 9);
  }
  if(iScoreXWindow)                                            //display score number
  {
    iScoreXWindow = 0;
    XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
              pEcranScrXImage, 68, 0, 68, 0, 128, 16);
  }
  iOptionIndex = -1;
  iAffiche = 1;
}

//------------------------------------------------------------------------------
// xwindow : display window in 640*400
// playfield 512x440 ; score panel 320x16 ; option panel 64x184
// iAffiche=0 : display all window 640x400
// 
// 
//------------------------------------------------------------------------------
void fenetre640x400()
{
  char *_pSource =
    ecran_ram + (BANDE_DE_CLIP * iLargeurEcranTotal) +
    (BANDE_DE_CLIP * iOctetsParPixel);

  //recopie l'ecran de jeu en doublant / recopy the screen by it doubling
  // ("assembler_opt.S")
  copy2X_512x440(_pSource, 
                 ecran_ram640 + (iLargeurEcran * iOctetsParPixel * 32),
                 HAUT_ECR_RAM
  );

  //affiche dans la fenetre ......................................................
  //affiche l'ecran de jeu 
  if(iAffiche)                                                 //iAffiche=1 pas besoin de reafficher tout l'ecran
  {
    XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique, pEcranXImage640,     //buffer source
              0, 32,                                           //coordonnees sources
              0, 32,                                           //coordonnees destination
              LARG_ECR_RAM * 2, HAUT_ECR_RAM * 2);             //affiche l'ecran de jeu de 512*368
 
    //display score number ?
    if(iScoreXWindow)
    {
      iScoreXWindow = 0;
      copy2X(ecran_scr + (68 * iOctetsParPixel),
             ecran_ram640 + (68 * iOctetsParPixel * 2), 128, 16,
             SCR_LARGE * iOctetsParPixel - 128 * iOctetsParPixel,
             (iLargeurEcran * 2 * iOctetsParPixel) -
             128 * iOctetsParPixel * 2);
      XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique, pEcranXImage640,   //buffer source
                68 * 2, 0,                                     //coordonnees sources
                68 * 2, 0,                                     //coordonnees destination
                128 * 2, 16 * 2);
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
      XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
                pEcranXImage640, (LARG_ECR_RAM + 41) * 2,
                (SCR_HAUTE + 171) * 2, (LARG_ECR_RAM + 41) * 2,
                (SCR_HAUTE + 171) * 2, 14 * 2, 8 * 2);
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
      XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
                pEcranXImage640, (LARG_ECR_RAM + 41) * 2, (SCR_HAUTE + 5) * 2,
                (LARG_ECR_RAM + 41) * 2, (SCR_HAUTE + 5) * 2, 14 * 2, 8 * 2);
      iBonusX4Affiche = 0;
    }

    while (iOptionIndex >= 0)                                  //display options from option panel
    {
      int                     _iOptionX = pOptionChange[iOptionIndex][0];
      int                     _iOptionY = pOptionChange[iOptionIndex--][1];

      copy2X(ecran_opt + (_iOptionX * iOctetsParPixel) +
             (_iOptionY * OPT_LARGE * iOctetsParPixel),
             ecran_ram640 +
             ((LARG_ECR_RAM + _iOptionX) * iOctetsParPixel * 2) +
             ((SCR_HAUTE + _iOptionY) * iLargeurEcran * iOctetsParPixel * 2),
             28, 28, OPT_LARGE * iOctetsParPixel - 28 * iOctetsParPixel,
             (iLargeurEcran * iOctetsParPixel * 2) -
             28 * iOctetsParPixel * 2);

      XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
                pEcranXImage640, (LARG_ECR_RAM + _iOptionX) * 2,
                (SCR_HAUTE + _iOptionY) * 2, (LARG_ECR_RAM + _iOptionX) * 2,
                (SCR_HAUTE + _iOptionY) * 2, 28 * 2, 28 * 2);
    }

    if(iBarreNrjJoueurXWindow)                                 //display player's ernergy
    {
      iBarreNrjJoueurXWindow = 0;
      copy2X(ecran_scr + (210 * iOctetsParPixel) +
             (3 * SCR_LARGE * iOctetsParPixel),
             ecran_ram640 + (210 * iOctetsParPixel * 2) +
             (3 * iLargeurEcran * iOctetsParPixel * 2), 100, 9,
             SCR_LARGE * iOctetsParPixel - 100 * iOctetsParPixel,
             (iLargeurEcran * iOctetsParPixel * 2) -
             100 * iOctetsParPixel * 2);

      XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
                pEcranXImage640, 210 * 2, 3 * 2, 210 * 2, 3 * 2, 100 * 2,
                9 * 2);
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
      XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique,
                pEcranXImage640, 10 * 2, 3 * 2, 10 * 2, 3 * 2, 45 * 2, 9 * 2);
    }


  }
  //reaffiche toute la fenetre .......................................................
  else
  {
    copy2X(ecran_scr, ecran_ram640, SCR_LARGE, SCR_HAUTE, 0,
           iLargeurEcran * iOctetsParPixel * 2 -
           (SCR_LARGE * 2 * iOctetsParPixel));
    copy2X(ecran_opt,
           ecran_ram640 + (LARG_ECR_RAM * 2 * iOctetsParPixel) +
           (iLargeurEcran * iOctetsParPixel * 32), OPT_LARGE, OPT_HAUTE, 0,
           iLargeurEcran * iOctetsParPixel * 2 -
           (OPT_LARGE * 2 * iOctetsParPixel));
    XPutImage(pPowerMangaDisplay, pFenetreJeu, pContexteGraphique, pEcranXImage640,     //buffer source
              0, 0,                                            //coordonnees sources
              0, 0,                                            //coordonnees destination
              iLargeurEcran, iHauteurEcran);                   //640x400
    iAffiche = 1;
    iOptionIndex = -1;
  }
}

//------------------------------------------------------------------------------
// xwindow : ferme l'affichage sous xwindow
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
  if(pFenetreJeu)
  {
    XDestroyWindow(pPowerMangaDisplay, pFenetreJeu);
    pFenetreJeu = 0;
#ifdef _VERBEUX_
    if(power_conf->verbose)
      afficheMessage("XDestroyWindow");
#endif
  }
  if(pContexteGraphique)
  {
    XFreeGC(pPowerMangaDisplay, pContexteGraphique);
    pContexteGraphique = 0;
#ifdef _VERBEUX_
    if(power_conf->verbose)
      afficheMessage("XFreeGC");
#endif
  }
  if(pPowerMangaDisplay)
  {
    XCloseDisplay(pPowerMangaDisplay);
    pPowerMangaDisplay = 0;
#ifdef _VERBEUX_
    if(power_conf->verbose)
      afficheMessage("XCloseDisplay");
#endif
  }
  return 1;
}

//------------------------------------------------------------------------------
// xwindow : initialise un curseur blanc / initialize small cursor (one pixel)
//------------------------------------------------------------------------------
void initialiseCurseurBlanc()
{
  Cursor                  cursor;
  XColor                  color;
  Pixmap                  cursorPixmap;
  int                     _iEcran = DefaultScreen(pPowerMangaDisplay);

  color.pixel = WhitePixel(pPowerMangaDisplay, _iEcran);
  cursorPixmap = XCreatePixmap(pPowerMangaDisplay, pFenetreJeu, 1, 1, 1);
  cursor =
    XCreatePixmapCursor(pPowerMangaDisplay, cursorPixmap, cursorPixmap,
                        &color, &color, 0, 0);
  if(cursorPixmap)
    XFreePixmap(pPowerMangaDisplay, cursorPixmap);
  XDefineCursor(pPowerMangaDisplay, pFenetreJeu, cursor);
}

//------------------------------------------------------------------------------
// decompress pcx file and convert to 8 bits (no change), or 16 bits or 24 bits
// input        => _pFichier                            : filename
// output <= _pStructureGfx             : pointer to the "sDescriptionGfx" structure / 0=error 
//------------------------------------------------------------------------------
char                   *Xload_pcx(char *_pNomFichier)
{
  sDescriptionGfx        *_pGfx = load_pcx(_pNomFichier);

  if(!_pGfx)
    return 0;
  if(iOctetsParPixel > 0)
  {
    if(!convertit_16ou24(_pGfx))
    {
      afficheErreur("convertit_16ou24() failed\n", "xwindow.cpp/Xload_pcx()");
      return 0;
    }
  }
  char                   *_pMem = _pGfx->pAdresse;

  libereMemoire((char *)_pGfx);
  return _pMem;
}

//------------------------------------------------------------------------------
// decompress pcx file and convert to 8 bits (no change), or 16 bits or 24 bits
// input        => _pFichier                            : filename
//                              => _pMem                                                : memory (XImage)
// output <= _pStructureGfx             : pointer to the "sDescriptionGfx" structure / 0=error 
//------------------------------------------------------------------------------
int Xload_pcx(char *_pNomFichier, char *_pMem)
{
  sDescriptionGfx        *_pGfx = load_pcx(_pNomFichier);

  if(!_pGfx)
    return 0;
  if(iOctetsParPixel > 0)
  {
    if(!convertit_16ou24(_pGfx))
    {
      afficheErreur("convertit_16ou24() failed\n", "xwindow.cpp/Xload_pcx()");
      return 0;
    }
  }
  char                   *_pMemBase = _pGfx->pAdresse;

  for(unsigned _iIndex = 0; _iIndex < _pGfx->iTaille; _iIndex++)
    _pMem[_iIndex] = _pMemBase[_iIndex];
  libereMemoire(_pMemBase);
  libereMemoire((char *)_pGfx);
  return 1;
}

//------------------------------------------------------------------------------
// xwindow : converti une image en 256 couleurs en millier ou millions
// entree       =>  sDescriptionGfx : description de l'image
//------------------------------------------------------------------------------
char                   *convertit_16ou24(sDescriptionGfx * _pSrc)
{
  if(iOctetsParPixel == 1)
    return _pSrc->pAdresse;
  unsigned int            _iTaille = (_pSrc->iLargeur * _pSrc->iHauteur);
  char                   *_pBuffer =
    reserveMemoire(_iTaille * iOctetsParPixel);
  if(!_pBuffer)
  {
    afficheErreur("out of memory", "xwindow.cpp/convertit_16ou24()");
    return 0;
  }
  switch (iOctetsParPixel)
  {
    case 2:
      conv8_16(_pSrc->pAdresse, _pBuffer, pal16, _iTaille);
      break;
    case 3:
      conv8_24(_pSrc->pAdresse, _pBuffer, pal32, _iTaille);
      break;
    case 4:
      conv8_32(_pSrc->pAdresse, _pBuffer, pal32, _iTaille);
      break;
  }
  libereMemoire(_pSrc->pAdresse);                              //libere le graphisme 8  bits
  _pSrc->pAdresse = _pBuffer;
  _pSrc->iTaille = _iTaille * iOctetsParPixel;
  return _pBuffer;
}

//------------------------------------------------------------------------------
// xwindow : create a XImage
//------------------------------------------------------------------------------
#ifdef UTILISE_XSHM
XImage                 *creeXImage(unsigned int _iLarge, unsigned int _iHaute)
{
  int                     _iXimageLibre = -1;

  for(unsigned int _iIndex = 0; _iIndex < iXIMAGEMAX; _iIndex++)
  {
    if(pListeXImage[_iIndex] == 0)
    {
      _iXimageLibre = _iIndex;
      break;
    }
  }
  if(_iXimageLibre < 0)
  {
    afficheErreur("pListeXImage is full", "xwindow.cpp/creeXImage()");
    return 0;
  }
  XImage                 *_pXImage =
    XShmCreateImage(pPowerMangaDisplay, CopyFromParent, iProfondeur2, ZPixmap,
                    0, &SHMInfo[_iXimageLibre], _iLarge, _iHaute);
  if(!_pXImage)
  {
    afficheErreur("pEcranXImage XShmCreateImage() failed",
                  "xwindow.cpp/creeXImage()");
    return 0;
  }
  SHMInfo[_iXimageLibre].shmid =
    shmget(IPC_PRIVATE, _pXImage->bytes_per_line * _pXImage->height,
           IPC_CREAT | 0777);
  _pXImage->data = SHMInfo[_iXimageLibre].shmaddr =
    (char *)shmat(SHMInfo[_iXimageLibre].shmid, 0, 0);
  SHMInfo[_iXimageLibre].readOnly = False;
  XShmAttach(pPowerMangaDisplay, &SHMInfo[_iXimageLibre]);
  pListeXImage[_iXimageLibre] = _pXImage;
  iNombreXImage++;
#ifdef _VERBEUX_
  if(power_conf->verbose)
    afficheMessage("xwindow.cpp/creeXImage() : XShmCreateImage");
#endif
  return _pXImage;
}
#else
XImage                 *creeXImage(unsigned int _iLarge, unsigned int _iHaute)
{
  int                     _iXimageLibre = -1;

  for(unsigned int _iIndex = 0; _iIndex < iXIMAGEMAX; _iIndex++)
  {
    if(pListeXImage[_iIndex] == 0)
    {
      _iXimageLibre = _iIndex;
      break;
    }
  }
  if(_iXimageLibre < 0)
  {
    afficheErreur("pListeXImage is full", "xwindow.cpp/creeXImage1()");
    return 0;
  }
  int                     _iScanLinePad = pixmapFormat.scanline_pad;
  int                     _iScanLineLength =
    pixmapFormat.bits_per_pixel * LARG_ECR_RAM_TOT / 8;
  if((_iScanLineLength & (pixmapFormat.scanline_pad / 8 - 1)) != 0)
  {
    _iScanLineLength &= ~(pixmapFormat.scanline_pad / 8 - 1);
    _iScanLineLength += pixmapFormat.scanline_pad / 8;
  }
  char                   *_pMemoire = reserveMemoire(_iLarge * _iHaute * iOctetsParPixel);      //reserve la memoire

  if(!_pMemoire)
  {
    afficheErreur("out of memory !", "xwindow.cpp/xw_init()");
    return 0;
  }
  XImage                 *_pXImage =
    XCreateImage(pPowerMangaDisplay, CopyFromParent, iProfondeur2, ZPixmap, 0,
                 _pMemoire, _iLarge, _iHaute, _iScanLinePad,
                 _iLarge * iOctetsParPixel);
  if(!_pXImage)
  {
    afficheErreur("XCreateImage() failed", "xwindow.cpp/creeXImage()");
    return 0;
  }
  pListeXImage[_iXimageLibre] = _pXImage;
  iNombreXImage++;
#ifdef _VERBEUX_
  if(power_conf->verbose)
    afficheMessage("xwindow.cpp/creeXImage() : XCreateImage\n");
#endif
  return _pXImage;
}
#endif

//------------------------------------------------------------------------------
// xwindow : free a XImage
//------------------------------------------------------------------------------
#ifdef UTILISE_XSHM
void libereXImage(XImage * _pXImage)
{
  for(unsigned int _iIndex = 0; _iIndex < iXIMAGEMAX; _iIndex++)
  {
    if(pListeXImage[_iIndex] == _pXImage)
    {
      XShmDetach(pPowerMangaDisplay, &SHMInfo[_iIndex]);
      if(SHMInfo[_iIndex].shmaddr)
        shmdt(SHMInfo[_iIndex].shmaddr);
      if(SHMInfo[_iIndex].shmid >= 0)
        shmctl(SHMInfo[_iIndex].shmid, IPC_RMID, 0);
      XDestroyImage(_pXImage);
      pListeXImage[_iIndex] = 0;
      iNombreXImage--;
#ifdef _VERBEUX_
      if(power_conf->verbose)
        afficheMessage
          ("xwindow.cpp/libereXImage() : XShmDetach and XDestroyImage");
#endif
      break;
    }
  }
}
#else
void libereXImage(XImage * _pXImage)
{
  for(unsigned int _iIndex = 0; _iIndex < iXIMAGEMAX; _iIndex++)
  {
    if(pListeXImage[_iIndex] == _pXImage)
    {
      char *_pMemoire = _pXImage->data;
      libereMemoire(_pMemoire);
      _pXImage->data = 0;
      XDestroyImage(_pXImage);
      pListeXImage[_iIndex] = 0;
      iNombreXImage--;
#ifdef _VERBEUX_
      if(power_conf->verbose)
        afficheMessage("xwindow.cpp/libereXImage() : XDestroyImage");
#endif
      break;
    }
  }
}
#endif

//------------------------------------------------------------------------------
// xwindow : free all XImages
//------------------------------------------------------------------------------
#ifdef UTILISE_XSHM
void libereXImages()
{
  unsigned int            _iIndex;

  for(_iIndex = 0; _iIndex < iXIMAGEMAX; _iIndex++)
  {
    XImage                 *_pXImage = pListeXImage[_iIndex];

    if(_pXImage)
    {
      XShmDetach(pPowerMangaDisplay, &SHMInfo[_iIndex]);
      if(SHMInfo[_iIndex].shmaddr)
        shmdt(SHMInfo[_iIndex].shmaddr);
      if(SHMInfo[_iIndex].shmid >= 0)
        shmctl(SHMInfo[_iIndex].shmid, IPC_RMID, 0);
      XDestroyImage(_pXImage);
      pListeXImage[_iIndex] = 0;
      iNombreXImage--;
#ifdef _VERBEUX_
      if(power_conf->verbose)
        afficheMessage("xwindow.cpp/libereXImages() : XDestroyImage");
#endif
    }
  }
}
#else
void libereXImages()
{
  unsigned int _iIndex;
  for(_iIndex = 0; _iIndex < iXIMAGEMAX; _iIndex++)
  { XImage *_pXImage = pListeXImage[_iIndex];
    if(_pXImage)
    { char *_pMemoire = _pXImage->data;
      libereMemoire(_pMemoire);
      _pXImage->data = 0;
      XDestroyImage(_pXImage);
      pListeXImage[_iIndex] = 0;
      iNombreXImage--;
#ifdef _VERBEUX_
      if(power_conf->verbose)
        afficheMessage
          ("xwindow.cpp/libereXImages() : XShmDetach and XDestroyImage");
#endif
    }
  }
}
#endif


/*
fullscreen=0 && vmode=0
- display sprites in "ecran_ram" (512*440)
- put "ecran_ram" in window (256*184) with XPutImage function

fullscreen=0 && vmode=1
- display sprites in "ecran_ram" (512*440)
- copy "ecran_ram" in "ecran_ram640" (double pixels horizontal and skip a line)
*/
#endif

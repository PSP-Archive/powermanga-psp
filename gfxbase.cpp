//******************************************************************************
// copyright (c) 1998-2003 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "gfxbase.cpp"
// created      : 1999-08-30
// updates      : 2003-07-20
//-----------------------------------------------------------------------------
// description  : paserelle sur les routines en langage machine
//                gatway to the assembler code
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
//******************************************************************************
#include "powermanga.hpp"
extern char            *pEnnemisNoms[];
extern image            enemi[MAXTYPENMI][NBR_IMAGES_MAX];     //tableau des images des vaisseaux ennemis
void                    print_val_image(image * _pImage);

//..............................................................................
extern char            *ecran_opt;                             //adresse barre options
extern char            *ecran_scr;                             //adresse bande scores
extern unsigned int     iLargeurEcranTotal;                    //taille d'une ligne du buffer 'ecran_ram'
extern char            *ecran_ram;                             //ecran logique (buffer 512*440)
extern unsigned int     iOctetsParPixel;                       //nombre d'octets par pixels (1=256 couleurs / 2=32768 couleurs)
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //structure des vaisseaux ennemis.
extern int              gardian_activity;                      //1=phase "gardien"
extern short            nbr_exps;                              //nombre d'explosions actuellement gerees
extern unsigned char    coulor[12];                            //couleurs choisies par l'utilisateur
extern unsigned short  *pal16;                                 //palette 16 bits
extern unsigned int    *pal32;                                 //palette 32 bits
unsigned int            iOne = 0;

//..............................................................................

void type_routine_gfx()
{ int flagcode = 0;
  _type_routine_gfx (&flagcode);
  switch(flagcode)
  { case 10:
      printf("> gfxbase.cpp/type_routine_gfx(): optimized by assembler routines\n");
      break;
    case 20:
      printf("> gfxbase.cpp/type_routine_gfx(): optimized by C routines\n");
      break;
    default:
      printf("> gfxbase.cpp/type_routine_gfx(): type_routine_gfx() error\n");
      break;
  }
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void poke(char *adresse, unsigned char valeur)
{
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void putpixel(char *_pAdresse, int x, int y, int coul)
{
}

//------------------------------------------------------------------------------
// affichage du masque du sprite / display sprite's mask
// input => _iColor     : couleur de la palette / color index (0 to 255)
//        => _pImage    : pointeur sur une structure "image" 
//        => _iX        : abscisse / x position
//        => _iY        : ordonnee / y position
//------------------------------------------------------------------------------
void putsprcolor(unsigned int _iColor, image * _pImage, unsigned int _iX,
                 unsigned int _iY)
{
  char  *_pDestination =
    ecran_ram + (_iY * LARG_ECR_RAM_TOT + _iX) * iOctetsParPixel;
  char *_pCompression = _pImage->compress;
  unsigned int _iTaille = _pImage->nbr_data_comp >> 2;

  
  
  switch (iOctetsParPixel)
  {
    case 1:
      putsprcolor_8bits(_iColor, _pDestination, _pCompression, _iTaille);
      break;
    case 2:
      putsprcolor_16bits((int)pal16[_iColor], _pDestination, _pCompression,
                         _iTaille);
      break;
    case 3:
      putsprcolor_24bits((int)pal32[_iColor], _pDestination, _pCompression,
                         _iTaille);
      break;
    case 4:
      putsprcolor_32bits((int)pal32[_iColor], _pDestination, _pCompression,
                         _iTaille);
      break;
  }
}

// -----------------------------------------------------------------------------
// affichage d'un sprite a partir d'une structure "image" / display sprite
// input => _pImage : pointeur sur une structure "image"
//       => _iX     : abscisse
//       => _iY     :  ordonnee
// -----------------------------------------------------------------------------
void putsprite3(image * _pImage, unsigned int _iX, unsigned int _iY)
{
  char           *_pSource      = _pImage->img;
  char           *_pDestination = ecran_ram + (_iY * iLargeurEcranTotal + _iX * iOctetsParPixel);
  char           *_pCompression = _pImage->compress;
  unsigned int    _iTaille      = _pImage->nbr_data_comp >> 2;
  switch (iOctetsParPixel)
  {
    case 1:
      putsprite3_8bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 2:
      putsprite3_16bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 3:
      putsprite3_24bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 4:
      putsprite3_32bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
  }
}

// -----------------------------------------------------------------------------
// affichage d'un sprite a partir d'une structure "fonte"
// input => _pFonte    : pointeur sur une structure "fonte"
//       => _iX        : abscisse
//       => _iY        : ordonnee
// -----------------------------------------------------------------------------
void putsprite3(fonte * _pFonte, unsigned int _iX, unsigned int _iY)
{
  char                   *_pSource = _pFonte->img;
  char                   *_pDestination =
    ecran_ram + (_iY * LARG_ECR_RAM_TOT + _iX) * iOctetsParPixel;
  char                   *_pCompression = _pFonte->compress;
  unsigned int            _iTaille = _pFonte->nbr_data_comp >> 2;

  switch (iOctetsParPixel)
  {
    case 1:
      putsprite3_8bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 2:
      putsprite3_16bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 3:
      putsprite3_24bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 4:
      putsprite3_32bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
  }
}

// -----------------------------------------------------------------------------
// affichage d'un sprite a partir d'une structure "fonte" dans la barre des scores
// input => _pFonte    : pointeur sur une structure "fonte"
//       => _iX        : abscisse
//       => _iY        : ordonnee
// -----------------------------------------------------------------------------
void putsprite3_scr(fonte * _pFonte, unsigned int _iX, unsigned int _iY)
{
  char                   *_pSource = _pFonte->img;
  char                   *_pDestination =
    ecran_scr + (_iY * SCR_LARGE + _iX) * iOctetsParPixel;
  char                   *_pCompression = _pFonte->compress;
  unsigned int            _iTaille = _pFonte->nbr_data_comp >> 2;

  switch (iOctetsParPixel)
  {
    case 1:
      putsprite3_8bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 2:
      putsprite3_16bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 3:
      putsprite3_24bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 4:
      putsprite3_32bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
  }
}

// -----------------------------------------------------------------------------
// affichage d'un sprite a partir d'une structure "fonte" dans la bande d'options
// input => _pFonte    : pointeur sur une structure "fonte"
//       => _iX        : abscisse
//       => _iY        : ordonnee
// -----------------------------------------------------------------------------
void putsprite3_opt(fonte * _pFonte, unsigned int _iX, unsigned int _iY)
{
  char                   *_pSource = _pFonte->img;
  char                   *_pDestination =
    ecran_opt + (_iY * OPT_LARGE + _iX) * iOctetsParPixel;
  char                   *_pCompression = _pFonte->compress;
  unsigned int            _iTaille = _pFonte->nbr_data_comp >> 2;

  switch (iOctetsParPixel)
  {
    case 1:
      putsprite3_8bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 2:
      putsprite3_16bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 3:
      putsprite3_24bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 4:
      putsprite3_32bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
  }
}

// -----------------------------------------------------------------------------
// affichage d'un sprite a partir d'une structure "image" dans la barre des scores
// input => _pImage    : pointeur sur une structure "image"
//       => _iX        : abscisse
//       => _iY        : ordonnee
// -----------------------------------------------------------------------------
void putsprite3_scr(image * _pImage, unsigned int _iX, unsigned int _iY)
{
  char                   *_pSource = _pImage->img;
  char                   *_pDestination =
    ecran_scr + (_iY * SCR_LARGE + _iX) * iOctetsParPixel;
  char                   *_pCompression = _pImage->compress;
  unsigned int            _iTaille = _pImage->nbr_data_comp >> 2;

  switch (iOctetsParPixel)
  {
    case 1:
      putsprite3_8bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 2:
      putsprite3_16bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 3:
      putsprite3_24bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
    case 4:
      putsprite3_32bits(_pSource, _pDestination, _pCompression, _iTaille);
      break;
  }
}

//------------------------------------------------------------------------------
// display lightning
//------------------------------------------------------------------------------
void draw_eclair(char *ecran, Eclair * eclair1, int recval)
{
  switch (iOctetsParPixel)
  {
    case 1:
      draw_eclair8(ecran, eclair1, recval);
      break;
    case 2:
      eclair1->col1 = (int)pal16[eclair1->col1];
      eclair1->col2 = (int)pal16[eclair1->col2];
      draw_eclair16(ecran, eclair1, recval);
      break;
    case 3:
      draw_eclair24(ecran, eclair1, recval);
      break;
    case 4:
      eclair1->col1 = (int)pal32[eclair1->col1];
      eclair1->col2 = (int)pal32[eclair1->col2];
      draw_eclair32(ecran, eclair1, recval);
      break;
  }
}

//------------------------------------------------------------------------------
// display en empty box (cursor text)
// input => adresse      : memory screen
//       => x     : x position
//       => y     : y position
//       => coul  : color index (0 to 255)
//       => large : width in pixels
//       => haute : height in pixels
//------------------------------------------------------------------------------
void _putRectangleVide(char *adresse, int x, int y, int coul, int large,
                       int haute)
{
  switch (iOctetsParPixel)
  {
    case 1:
      _PUTRECTANGLEVIDE8BITS(adresse, x, y, coul, large, haute);
      break;
    case 2:
      _PUTRECTANGLEVIDE16BITS(adresse, x, y, (int)(pal16[coul]), large,
                              haute);
      break;
    case 3:
      _PUTRECTANGLEVIDE24BITS(adresse, x, y, (int)(pal32[coul]), large,
                              haute);
      break;
    case 4:
      _PUTRECTANGLEVIDE32BITS(adresse, x, y, (int)(pal32[coul]), large,
                              haute);
      break;
  }
}

//------------------------------------------------------------------------------
// display fonte 8*8 pixels
//  input => _pDestination  : buffer destination address
//       => _pSource        : buffer source address
//------------------------------------------------------------------------------
void _ASMCOPYFONTE(unsigned char *_pDestination, unsigned char *_pSource)
{
  switch (iOctetsParPixel)
  {
    case 1:
      _ASMCOPYFONTE8BITS(_pSource, _pDestination, 8, 8, iLARGEURFONTE - 8,
                         iLargeurEcranTotal - 8);
      break;
    case 2:
      _ASMCOPYFONTE16BITS(_pSource, _pDestination, 8, 8,
                          iLARGEURFONTE * 2 - 16, iLargeurEcranTotal - 16);
      break;
    case 3:
      _ASMCOPYFONTE24BITS(_pSource, _pDestination, 8, 8,
                          iLARGEURFONTE * 3 - 24, iLargeurEcranTotal - 24);
      break;
    case 4:
      _ASMCOPYFONTE32BITS(_pSource, _pDestination, 8, 8,
                          iLARGEURFONTE * 4 - 32, iLargeurEcranTotal - 32);
      break;
  }
}

//-----------------------------------------------------------------------------
// display a ship / affichage d'un vaisseau :
// input => nmi_numero : ship number
// -----------------------------------------------------------------------------
void Aff_Ve(int nmi_numero)
{
  if(ve[nmi_numero].affich_white)                              //affichage en blanc, display mask ?
  {
    putsprcolor(coulor[BLANC],
                ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act],
                (int)(ve[nmi_numero].spr.coor_x),
                (int)(ve[nmi_numero].spr.coor_y));
    ve[nmi_numero].affich_white = 0;
  }
  else
  {
    putsprite3(ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act],
               (unsigned int)ve[nmi_numero].spr.coor_x,
               (unsigned int)ve[nmi_numero].spr.coor_y);
  }
}

//-----------------------------------------------------------------------------
// display a guardian's ship / affichage d'un vaisseau gardien
// input => nmi_numero : ship number
// -----------------------------------------------------------------------------
void Aff_Ve2(int nmi_numero)
{
  sprite ve_spr = ve[nmi_numero].spr;

  //affichage en blanc, display mask ?
  if(ve[nmi_numero].affich_white)
  { putsprcolor(coulor[BLANC],
                ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act],
                (int)(ve[nmi_numero].spr.coor_x),
                (int)(ve[nmi_numero].spr.coor_y));
    ve[nmi_numero].affich_white = 0;
  }
  else
  { putsprite3(ve[nmi_numero].spr.img[ve[nmi_numero].spr.img_act],
               (unsigned int)ve[nmi_numero].spr.coor_x, 
               (unsigned int)ve[nmi_numero].spr.coor_y);
    if(nbr_exps < MAX_NMI_ON_SCREEN - 1 && rand() % 2
       && rand() % (ve_spr.etat_max + 1) >
       ve_spr.etat_dommages + (ve_spr.etat_max >> 3))
    {
      const short zon_col = (short)(rand() % ((int)ve_spr.img[ve_spr.img_act]->nbr_zon_col));
      const float x_expl  = (float)(ve_spr.coor_x + ve_spr.img[ve_spr.img_act]->xy_zon_col[zon_col][X] +
                            rand() % ((int)ve_spr.img[ve_spr.img_act]->lh_zon_col[zon_col][X] +1));
      const float y_expl  = (float)(ve_spr.coor_y + ve_spr.img[ve_spr.img_act]->xy_zon_col[zon_col][Y] +
                            rand() % ((int)ve_spr.img[ve_spr.img_act]->lh_zon_col[zon_col][Y] + 1));
      Add_Explosion_Guard(x_expl, y_expl);
      //Add_Explosion(x_expl, y_expl, -.5, NORM_EXPLOSION, 0);
    }
  }
}

//------------------------------------------------------------------------------
// copie une zone de 256*184 d un ecran de 512*440 dans un ecran de 640*400 
// en doublant les pixels horizontalement et en sautant une ligne sur deux
// input  => _pSource      : adresse source
//        => _pDestination : adresse destination
//        => _iHauteur     : 184 lignes
//------------------------------------------------------------------------------
void copy2X_512x440(char *_pSource, char *_pDestination,
                    unsigned int _iHauteur)
{
  switch (iOctetsParPixel)
  {
    case 1:
      _COPY2X8BITS_512x440(_pSource, _pDestination, _iHauteur);
      break;
    case 2:
      _COPY2X16BITS_512x440(_pSource, _pDestination, _iHauteur);
      break;
    case 3:
      _COPY2X24BITS_512x440(_pSource, _pDestination, _iHauteur);
      break;
    case 4:
      _COPY2X32BITS_512x440(_pSource, _pDestination, _iHauteur);
      break;
  }
}

//------------------------------------------------------------------------------
// entree => _pSource      : adresse source
//        => _pDestination : adresse destination
//        => _iLarge       : largeur
//        => _iHaut        : hauteur
//        => _iOffset      : offset source (prochaine ligne)
//        => _iOffset2     : offset destination (prochaine ligne)
//------------------------------------------------------------------------------
void copy2X(char *_pSource, char *_pDestination, unsigned int _iLarge,
            unsigned int _iHaut, unsigned int _iOffset,
            unsigned int _iOffset2)
{
  switch (iOctetsParPixel)
  {
    case 1:
      _COPY2X8BITS(_pSource, _pDestination, _iLarge, _iHaut, _iOffset,
                   _iOffset2);
      break;
    case 2:
      _COPY2X16BITS(_pSource, _pDestination, _iLarge, _iHaut, _iOffset,
                    _iOffset2);
      break;
    case 3:
      _COPY2X24BITS(_pSource, _pDestination, _iLarge, _iHaut, _iOffset,
                    _iOffset2);
      break;
    case 4:
      _COPY2X32BITS(_pSource, _pDestination, _iLarge, _iHaut, _iOffset,
                    _iOffset2);
      break;
  }
}

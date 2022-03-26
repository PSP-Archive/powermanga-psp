//*****************************************************************************
// copyright (c) 1998-2003 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "grille.cpp"
// created      : ?
// updates      : 2003-06-28
//-----------------------------------------------------------------------------
// functions    : void  grille_execution()
//                void  grille_testFin()
//                int   LoadGrid(int num_grid)
//                void  Active_Grid()
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
extern image            enemi[MAX_TYP_OF_ENEMI + MAX_TYP_OF_S_ENEMI +
                              MAX_TYP_OF_NMI_SPE +
                              MAX_TYP_OF_OBJ_SPE][NBR_IMAGES_MAX];
extern vaisseau         vj[NBR_MAX_J];                         //structure des vaisseaux des joueurs
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //structure des vaisseaux ennemis
extern curve_level      courbe;                                // declaration de la structure de niveaux courbe.
extern bool             meteor_activity;
extern int              gardian_activity;                      // utilise pour savoir si on est dans une phase de gardien ou non.
extern grid             grille;                                // declaration de la structure de grille des vaisseaux ennemis.
extern int              numero_du_meteor;
extern short            nbr_nmis;                              // nombre d'enemis actuellement geres.
extern int              nbr_meteor_generer;
extern int              new_ve;                                //indice tableau sur un nouvel objet 've'

//------------------------------------------------------------------------------
// phase "grille" : gestion de la phase "grille"
//------------------------------------------------------------------------------
void grille_execution()
{
  if(grille.activity)
  { if(!grille.apparition)                                     //phase d'apparition ?
    { if(!grille.sens_dep)                                     //0=deplacement a gauche ?
      { grille.coor_x -= grille.vit_dep_x;                     //oui
        grille.coor_y += grille.vit_dep_y;
        if(grille.coor_x < BANDE_DE_CLIP - 32)                 //maximum a gauche ? change le sens de deplacement ?
        { grille.coor_x = BANDE_DE_CLIP - 32;                  //abscisse minimum
          grille.sens_dep = 1;                                 //1=deplacement a droite
        }
      }
      else
      { grille.coor_x += grille.vit_dep_x;                     //sinon deplacement a droite
        grille.coor_y += grille.vit_dep_y;
        if(grille.coor_x > BANDE_DE_CLIP + 32)                 //maximum a droite ? change le sens de deplacement ?
        { grille.coor_x = BANDE_DE_CLIP + 32;                  //abscisse maximum
          grille.sens_dep = 0;                                 //0=deplacement a droite
        }
      }
    }
    else
    //sinon, on se trouve dans une phase d'appariton de la grille
    { grille.coor_y++;
      if(grille.coor_y >= BANDE_DE_CLIP - 4)                   //grille arrivee a destination ?
        grille.apparition = 0;                                 //oui phase d'apparition terminee
    }
  }
}

//------------------------------------------------------------------------------
// test la fin de la phase "grille" / test if the end of the grid phase
//------------------------------------------------------------------------------
void grille_testFin()
{
  if(grille.activity)
  { if((short)grille.coor_y > 184 || !nbr_nmis)                //grille est sortie de l'ecran ? / grid's out of screen ?
    { meteor_activity = 1;                                     //1=meteors phase
      nbr_meteor_generer = 0;
      numero_du_meteor = rand() % (MAX_TYP_OF_METEOR / 3);
      courbe.activity = 0;
      gardian_activity = 0;
      grille.activity = 0;
    }
  }
}

//------------------------------------------------------------------------------
// chargement d'un fichier grille / load a grid file
//------------------------------------------------------------------------------
static char             _pNomFichier[] = "graphics/grille.??";
int LoadGrid(int num_grid)
{
  short                  *_p16;
  float                  *_f32;
  //unsigned int           *_p32;
  //unsigned char          *_p8;
  short                  *_pMem;
  unsigned int            i;
  char                   *_pFichier ;
  meteor_libere();
  if(num_grid > NBR_LEVEL_MAX || num_grid < 0)
    num_grid = 0;
  entier2ASCII(num_grid, _pNomFichier + 16, 1);
  _pFichier = chargeFichier(_pNomFichier);                     //charge le fichier en memoire
  if(!_pFichier) return 0;
  _f32 = (float *)_pFichier;
  //_p32 = (unsigned int *)_pFichier;
  //_p8 = (unsigned char *)_pFichier;

  // * windows : il ne faut pas les convertir...
/*#if __BYTE_ORDER == __BIG_ENDIAN
  convert32bits_2bigendian ((unsigned char *)_f32);
#endif*/

  grille.vit_dep_x = (float)*(_f32++);                         //lit un type 'float' (32 bits sur 80x86)

  // * windows : il ne faut pas les convertir...
/*#if __BYTE_ORDER == __BIG_ENDIAN
  convert32bits_2bigendian ((unsigned char *)_f32);
#endif*/

  grille.vit_dep_y = (float)*(_f32++);

  //printf("grille.vit_dep_x=%f  = grille.vit_dep_y=%f \n", grille.vit_dep_x, grille.vit_dep_y);

  _p16 = (short *)_f32;
  _pMem = &grille.nom_vaisseau[0][0];
  for(i = 0; i < LARG_GRILLE * HAUT_GRILLE; i++)
    *(_pMem++) = litMot16bits(_p16++);
  _pMem = &grille.freq_tir[0][0];
  for(i = 0; i < LARG_GRILLE * HAUT_GRILLE; i++)
    *(_pMem++) = litMot16bits(_p16++);
  libereMemoire(_pFichier);                                    //(!) bug found by Sam Hocevar
  return 1;
}

//------------------------------------------------------------------------------
// initialize "grid" phase
//------------------------------------------------------------------------------
void Active_Grid()
{
  int                     i, j, k;
  for(i = 0; i < LARG_GRILLE; i++)
  {
    for(j = 0; j < HAUT_GRILLE; j++)
    {                                                          // Test si un vaisseau est pr�ent dans la grille.
      if(grille.nom_vaisseau[i][j] != -1)                      // Remplissage de la structure sprite appartenant au vaisseau ennemi.
      {                                                        // On rajoute un ennemi dans la liste.
        new_ve = New_Element_Ve();                             // Retourne l'indice dans le tableau 've' d'un nouvel ��ent de la liste.
        // Test si l'on est en pr�ence d'un ennemi ou d'un s_ennemi.
        if(grille.nom_vaisseau[i][j] < MAX_TYP_OF_ENEMI)
        {
          ve[new_ve].spr.pow_of_dest = (short)(1 + grille.nom_vaisseau[i][j]);  // Indique la puissance de destruction du sprite.
          ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 1) + ve[new_ve].spr.pow_of_dest);     // Indique l'�at de domages du sprite.
          ve[new_ve].type = 0;                                 // Indique que l'on se trouve en pr�ence d'un ennemi de 16x16.
        }
        else                                                   // On est donc en pr�ence d'un s_enemi.
        {
          ve[new_ve].spr.pow_of_dest = (short)(6 + (grille.nom_vaisseau[i][j] - MAX_TYP_OF_ENEMI) * 2); // Indique la puissance de destruction du sprite.
          ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 2) + ve[new_ve].spr.pow_of_dest + 10);        // Indique l'�at de domages du sprite.
          ve[new_ve].type = 1;                                 // Indique que l'on se trouve en pr�ence d'un ennemi de 32x32.
        }
        ve[new_ve].spr.nbr_img = 8;                            //nombre d'images composant du sprite
        ve[new_ve].spr.img_act = rand() % 8;                   //image actuelle de 0 a 7
        ve[new_ve].spr.nbr_tot_vbl = 10;                       //vitesse d'animation
        ve[new_ve].spr.nbr_vbl_act = 0;                        //initialise la tempo d'animations

        //initialise les adresses des images
        for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
        {
          if(grille.nom_vaisseau[i][j] < MAX_TYP_OF_ENEMI)
            ve[new_ve].spr.img[k] = (image *) & enemi[grille.nom_vaisseau[i][j]][32 + k];       //ennemi 16*16
          else
            ve[new_ve].spr.img[k] = (image *) & enemi[grille.nom_vaisseau[i][j]][k];           //super-ennemi 32*32
        }
        ve[new_ve].cad_tir = grille.freq_tir[i][j];            // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer �nouveau.
        ve[new_ve].deplacement = 1;                            //0="courbe" ; 1="grille" ;  2="special" ; 3="gardien"
        ve[new_ve].pos_vaiss[X] = (short)i;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
        ve[new_ve].pos_vaiss[Y] = (short)j;                    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      }
    }
  }
  grille.coor_x = 128;                                         //abscisse de la grille
  grille.coor_y = -176;                                        //ordonnee de la grille
  grille.apparition = 1;                                       //1=phase d'apparition de la grille
  grille.activity = 0;                                         //0=gestion grille
  grille.sens_dep = rand() % 2;                                //deplacement de la grille 0=gauche / 1=droite
}

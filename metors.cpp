//*****************************************************************************
// copyright (c) 1998-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "metors.cpp"
// created      : ?
// updates      : 2002-08-02
//-----------------------------------------------------------------------------
// functions    : void  meteor_execution() 
//                int   meteor_initialise()
//                void  meteor_libere()
//                int   Load_Meteor(int num_meteor)
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
extern image            meteo[MAX_TYP_OF_METEOR][NBR_IMAGES_METEOR];    //tableau des images des meteorites
extern int              num_level;                             //numero du niveau
extern bool             meteor_activity;                       //1=meteorites actives
extern int              nbr_meteor_generer;
extern int              cmpt_vbl_app_meteor;
extern int              taille_meteor;
extern int              numero_du_meteor;
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //structure des vaisseaux ennemis.
extern int              new_ve;                                //indice d'un nouvel objet 've'.
extern vaisseau         vj[NBR_MAX_J];
extern image            meteo[MAX_TYP_OF_METEOR][NBR_IMAGES_METEOR];
extern curve_level      courbe;                                //structure de niveaux courbe.
extern grid             grille;                                //structure de grille des vaisseaux ennemis.
extern int              gardian_activity;                      //1=phase "gardien"
//..............................................................................
void meteor_execution()
{
  static int              k;
  if(meteor_activity)                                          //phase "meteor" ?
  {
    if(cmpt_vbl_app_meteor > (12 + (NBR_LEVEL_MAX - num_level)))        //nouvelle meteorite apparait ?
    {
      cmpt_vbl_app_meteor = 0;
      if(nbr_meteor_generer < (40 + (num_level << 1)))         //nombre de meterorites atteinds ?
      {
        taille_meteor = rand() % 3;                            //taille meterorites 0, 1 ou 2
        new_ve = New_Element_Ve();                             //nouvel ennemi a l'ecran 
        switch (taille_meteor)
        {                                                      // Donne la puissance de destruction du météor et le nombre de VBL qu'il faut pour passer à l'image suivante.
          case 0:
            ve[new_ve].spr.pow_of_dest = (short)(6 + (num_level >> 1)); // 24
            ve[new_ve].vitesse_y = 0.7f + (float)((float)num_level / 25.0);     // Donne la vitesse de déplacement du vaisseau ennemi spécial.
            ve[new_ve].spr.nbr_tot_vbl = 5;
            break;                                             // 4
          case 1:
            ve[new_ve].spr.pow_of_dest = (short)(4 + (num_level >> 1)); // 12
            ve[new_ve].vitesse_y = 0.6f + (float)((float)num_level / 30.0);     // Donne la vitesse de déplacement du vaisseau ennemi spécial.
            ve[new_ve].spr.nbr_tot_vbl = 4;
            break;                                             // 6
          case 2:
            ve[new_ve].spr.pow_of_dest = (short)(2 + (num_level >> 1)); // 6
            ve[new_ve].vitesse_y = 0.5f + (float)((float)num_level / 35.0);     // Donne la vitesse de déplacement du vaisseau ennemi spécial.
            ve[new_ve].spr.nbr_tot_vbl = 3;
            break;                                             // 8
        }
        ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 1) + (ve[new_ve].spr.pow_of_dest << 3) / 5);    // Indique l'état de domages du sprite.
        ve[new_ve].spr.nbr_img = NBR_IMAGES_METEOR;            // Indique le nombre d'images composant le sprite.
        ve[new_ve].spr.img_act = rand() % NBR_IMAGES_METEOR;   // Indique l'image actuelle.
        ve[new_ve].spr.nbr_vbl_act = 0;                        // RAZ du compteur de VBL pour le passage à l'image suivante.
        // Donne l'adresse des images du sprite.
        for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
        {
          ve[new_ve].spr.img[k] = (image *) & meteo[taille_meteor][k];
        }
        ve[new_ve].cad_tir_initiale = 10000;                   // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
        ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;      // Initialise la variable utilisée pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer à nouveau.
        ve[new_ve].deplacement = SPECIAUX;                     // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
        // Donne les coordonées x et y de départ du vaisseau.
        ve[new_ve].spr.coor_x =
          DEBUT_ECRAN_X +
          (float)(rand() % (LARG_ECR_RAM - ve[new_ve].spr.img[0]->larg));
        ve[new_ve].spr.coor_y = DEBUT_ECRAN_Y - 64;
        ve[new_ve].vitesse_x = 0.0;                            // Donne la vitesse de déplacement du vaisseau ennemi spécial.
        ve[new_ve].type = BIGMETEOR + taille_meteor;           // Indique le type de vaisseau spécial.
        ve[new_ve].temps_apparition = 210;                     // Donne le temps d'apparition du météor.

        if(rand() % 2)                                         //0 ou 1 (animation aleatoire)
          ve[new_ve].sens_anim = -1;                           //1=animation decroissante
        else
          ve[new_ve].sens_anim = 1;                            //0=animation croissante
        nbr_meteor_generer++;                                  //nombre de meteorites pour ce niveau
      }
    }
    cmpt_vbl_app_meteor++;
  }
}

//------------------------------------------------------------------------------
// prepare la phase "meteors"
//------------------------------------------------------------------------------
int meteor_initialise()
{                                                              //static int i, j;
  if(meteor_activity)                                          //phase "meteors" active
  {                                                            //printf("meteor_initialise start ") ;
    if(num_level != -1)                                        //debut du jeu ?
    {
      if(nbr_meteor_generer >= (40 + (num_level << 1)))        //nombre meteorites atteint ?
      {
        switch (num_level)
        {
          case 3:
            Meteor_Activity_With_Guardian(1);
            break;
          case 7:
            Meteor_Activity_With_Guardian(2);
            break;
          case 11:
            Meteor_Activity_With_Guardian(3);
            break;
          case 15:
            Meteor_Activity_With_Guardian(4);
            break;
          case 19:
            Meteor_Activity_With_Guardian(5);
            break;
          case 23:
            Meteor_Activity_With_Guardian(6);
            break;
          case 27:
            Meteor_Activity_With_Guardian(7);
            break;
          case 31:
            Meteor_Activity_With_Guardian(8);
            break;
          case 35:
            Meteor_Activity_With_Guardian(9);
            break;
          case 39:
            Meteor_Activity_With_Guardian(10);
            break;
          case 41:
            Meteor_Activity_With_Guardian(11);
            break;
          default:
            if(!Meteor_Activity_Less_Guardians())
              return 0;
            break;
        }
      }
    }
    else                                                       //premier niveau du jeu.
    {
      if(nbr_meteor_generer >= (40 + (num_level << 1)))
      {
        num_level++;
        if(num_level > NBR_LEVEL_MAX)
          num_level = 0;
        if(!Load_Gardien(1))                                   //charge gardien 1
          return 0;
        if(!LoadGrid(num_level))                               //charge la phase "grille"
          return 0;
        if(!Load_Curve_Level(num_level))
          return 0;
        meteor_libere();
        if(!Load_Meteor(num_level))                            //chargement des meteorites.
          return 0;
        Active_Curve_Level();                                  //active la phase "courbe"
        courbe.activity = 1;                                   //1=phase "courbe" active
        grille.activity = 0;
        meteor_activity = 0;
        gardian_activity = 0;
      }
    }
  }
  return 1;
}

//------------------------------------------------------------------------------
// meteors : liberation de la memoire utilisee par les images des meteorites
//------------------------------------------------------------------------------
void meteor_libere()
{
  unsigned int            i, j;

  for(i = 0; i < (MAX_TYP_OF_METEOR); i++)
  {
    for(j = 0; j < NBR_IMAGES_METEOR; j++)
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
}

//------------------------------------------------------------------------------
// Chargement de tous les sprites des météorites.
//------------------------------------------------------------------------------
static char             pNomFichierMeteor[] = "graphics/meteor??.spr";
int Load_Meteor(int num_meteor)
{
  meteor_libere();
  if(num_meteor > NBR_LEVEL_MAX || num_meteor < 0) num_meteor = 0;
  entier2ASCII(num_meteor, pNomFichierMeteor + 15, 1);
  char                   *_pFichier = chargeFichier(pNomFichierMeteor); //charge le fichier en memoire
  if(!_pFichier)
    return 0;
  if(!etb_read_image(pNomFichierMeteor, &meteo[0][0], MAX_TYP_OF_METEOR, NBR_IMAGES_METEOR))
    return 0;
  libereMemoire(_pFichier);
  return 1;
}

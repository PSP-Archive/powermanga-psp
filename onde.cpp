//*****************************************************************************
// copyright (c) 1998-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "onde.cpp"
// created      : 1999-09-05
// updates      : 2002-08-11
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
#include "cercle.hpp"
//.............................................................................
onde_de_choc            onde_choc[NBR_ONDE_CHOC_MAX];          // tableau des differentes ondes de choc
int                     liste_onde_choc[NBR_ONDE_CHOC_MAX];    // liste des "tirs" geres 
int                     nbr_onde_choc;                         // nombre actuel d'onde(s) de choc
int                     cmpt_onde_choc;                        // compteur des ondes de choc 
unsigned char           color_onde_choc[NBR_COLOR_ONDE_CHOC + 1];       // Tableau contenant les couleurs utilisees pour l'affichage d'une onde de choc.
static int              tableau[10];                           //utilise pour l'affichage des polygones

//.............................................................................
extern unsigned short  *pal16;                                 //16 bits palette
extern unsigned int    *pal32;                                 //32 bits palette
extern char            *ecran_ram;                             //pointer to the buffer (XImage 512*440)
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //ship enemy alien structure
extern unsigned int     iOctetsParPixel;                       //number of bytes per pixel (1=256 colors / 2=32768 colors)
extern unsigned int     iLargeurEcranTotal;                    //sizeline of screen buffer 'ecran_ram'

//------------------------------------------------------------------------------
// display shock-wave / affiche l'onde de choc (bonus violet)
//------------------------------------------------------------------------------
void Draw_Onde_De_Choc()
{
  int                     i, j;
  struct poly             poly_onde_choc;                      // Definition d'un polygone pour la routine d'affichage d'Etienne.
  bool                    un_polygone_de_visible;              //1=un polygone de visible au moins
  poly_onde_choc.taille = 24;
  poly_onde_choc.zero = 0;
  poly_onde_choc.nb_pnt = 4;
  poly_onde_choc.fin = -1;

  //boucle traitant tous les polygones . . . . . . . . . . . . . . . . . . . . . 
  for(i = 0; i < nbr_onde_choc; i++)
  { cmpt_onde_choc = liste_onde_choc[i];                       //indince dans le tableau 'onde_choc'
    poly_onde_choc.coul = color_onde_choc[onde_choc[cmpt_onde_choc].cmpt_color_aff];    //couleur d'affichage
    un_polygone_de_visible = FALSE;                            //polygone non visible
    if((onde_choc[cmpt_onde_choc].cmpt_cercle + 2) <= (NBR_ANO_ONDE_CHOC - 2))  //reste des anneaux ?
    { for(j = 0; j < (NBR_PTS_ONDE_CHOC - 1); j++)             //creation de tous les polygones de l'onde de choc
      { // Remplissage du polygone pour l'affichage. 
        tableau[2] = (int)(onde_choc_px[onde_choc[cmpt_onde_choc].cmpt_cercle][j] + onde_choc[cmpt_onde_choc].orig_x);
        tableau[3] = (int)(onde_choc_py[onde_choc[cmpt_onde_choc].cmpt_cercle][j] + onde_choc[cmpt_onde_choc].orig_y);
        // Test si on doit afficher ou non le polygone.
        if(tableau[2] >= (DEBUT_ECRAN_X - 5)
           && tableau[2] <= (DEBUT_ECRAN_X + LARG_ECR_RAM + 5)
           && tableau[3] >= (DEBUT_ECRAN_Y - 5)
           && tableau[3] <= (DEBUT_ECRAN_Y + HAUT_ECR_RAM + 5))
        {                                                      // Remplissage du reste de la structure.
          tableau[0] = (int)(onde_choc_px[onde_choc[cmpt_onde_choc].cmpt_cercle + 2][j] + onde_choc[cmpt_onde_choc].orig_x);
          tableau[1] = (int)(onde_choc_py[onde_choc[cmpt_onde_choc].cmpt_cercle + 2][j] + onde_choc[cmpt_onde_choc].orig_y);
          tableau[4] = (int)(onde_choc_px[onde_choc[cmpt_onde_choc].cmpt_cercle][j + 1] + onde_choc[cmpt_onde_choc].orig_x);
          tableau[5] = (int)(onde_choc_py[onde_choc[cmpt_onde_choc].cmpt_cercle][j + 1] + onde_choc[cmpt_onde_choc].orig_y);
          tableau[6] = (int)(onde_choc_px[onde_choc[cmpt_onde_choc].cmpt_cercle + 2][j + 1] + onde_choc[cmpt_onde_choc].orig_x);
          tableau[7] = (int)(onde_choc_py[onde_choc[cmpt_onde_choc].cmpt_cercle + 2][j + 1] + onde_choc[cmpt_onde_choc].orig_y);
          tableau[8] = tableau[0];
          tableau[9] = tableau[1];
          // on affiche le polygone.
          polygone(ecran_ram, tableau, 4, color_onde_choc[onde_choc[cmpt_onde_choc].cmpt_color_aff]);
          un_polygone_de_visible = TRUE;                       // On indique qu'il y a au moins un polygone de visible.
        }
      }                                                        // Fin de la bcle qui cree tous les polygones de l'onde de choc.
      // Test s'il y a eu au moins un affichage de polygone.
      if(!un_polygone_de_visible)
      { Del_Element_Onde_Choc(cmpt_onde_choc, i);              // Il n'y a eu aucun affichage de polygone, on peux donc supprimer l'onde de choc.
        i--;                                                   // On se positionne sur l'onde de choc que l'on vient de supprimer.
      }
    }                                                          // Fin du test pour savoir s'il reste encore des anneaux.
    else                                                       // Plus d'anneaux disponibles, on doit donc supprimer l'onde de choc.
    { Del_Element_Onde_Choc(cmpt_onde_choc, i);                // On enleve l'onde de choc de la liste des ondes de choc.
      i--;                                                     // On se positionne sur l'onde de choc que l'on vient de supprimer.
    }
    // On effectue le passage a l'anneau suivant de l'onde de choc.
    onde_choc[cmpt_onde_choc].cmpt_cercle++;                   // Passage a la couleur d'affichage suivante.
    onde_choc[cmpt_onde_choc].cmpt_color_aff++;                // Test s'il reste encore des couleurs dans le tableau des couleurs.
    if(onde_choc[cmpt_onde_choc].cmpt_color_aff >= (NBR_COLOR_ONDE_CHOC - 1))
      onde_choc[cmpt_onde_choc].cmpt_color_aff = NBR_COLOR_ONDE_CHOC - 1;
  }
}

//------------------------------------------------------------------------------
// Retourne l'indice dans le tableau 'onde_choc' d'un nouvel element de la liste.
//------------------------------------------------------------------------------
int New_Element_Onde_Choc(void)
{
  int                     l, m;
  // Test s'il y a plusieurs elements dans le tableau.
  if(nbr_onde_choc)                                            // Si oui, on recherche un objet libre pour attribution.
  { for(m = 0; m < NBR_ONDE_CHOC_MAX; m++)                     // Recherche d'un objet libre dans le tableau.
    { if(onde_choc[m].next_object == NBR_ONDE_CHOC_MAX)        // Si une valeur next_object est egale au nombre maximum d'objets du tableau, c'est qu'on tient un objet libre.
      goto New_Element_Onde_Choc_Find;
    }
New_Element_Onde_Choc_Find:;

    // On doit a present modifier le dernier element de la liste pour le chaîner au nouvel element.
    l = liste_onde_choc[nbr_onde_choc - 1];                    // Donne le dernier element de la liste.
    onde_choc[l].next_object = m;                              // Attribution de l'element suivant a ce qui etait le dernier element de la liste.
    onde_choc[m].prev_object = l;                              // Attribution de l'element precedent au dernier element de la liste.
    onde_choc[m].next_object = -1;                             // Indique  que cet element est le dernier de la liste.
    liste_onde_choc[nbr_onde_choc] = m;                        // Met a jour la liste des 'onde_choc'.
    nbr_onde_choc++;                                           // Incremente le nombre d'objets 'onde_choc' actuellement geres.
    return (m);
  }

  // Si on arrive en ce point c'est que le tableau est actuellement vide. On peux alors attribuer le premier element.
  onde_choc[0].prev_object = -1;
  onde_choc[0].next_object = -1;
  liste_onde_choc[nbr_onde_choc] = 0;                          // Met a jour la liste des 'onde_choc'.
  nbr_onde_choc++;                                             // Incremente le nombre d'objets 'onde_choc' actuellement geres.
  return (0);                                                  // Indique que le nouvel element de la liste est le premier element du tableau.
}

//------------------------------------------------------------------------------
// Efface le nieme element de la liste et fait les mises a jour necessaire.
//------------------------------------------------------------------------------
void Del_Element_Onde_Choc(int n, int o)
{
  int                     m;
  // On doit a present modifier les elements suivants et precedent de la liste pour faire un chaînage correct.
  if(onde_choc[n].prev_object != -1) onde_choc[onde_choc[n].prev_object].next_object = onde_choc[n].next_object; // Attribution de l'element suivant de la liste.
  if(onde_choc[n].next_object != -1) onde_choc[onde_choc[n].next_object].prev_object = onde_choc[n].prev_object; // Attribution de l'element suivant de la liste.
  onde_choc[n].prev_object = NBR_ONDE_CHOC_MAX;                // Indique que l'objet actuel est libre.
  onde_choc[n].next_object = NBR_ONDE_CHOC_MAX;                // Indique que l'objet actuel est libre.
  for(m = o; m < (nbr_onde_choc - 1); m++)                     // Met a jour la liste des 'onde_choc'.
    liste_onde_choc[m] = liste_onde_choc[m + 1];
  nbr_onde_choc--;                                             // Decremente le nombre d'objets 'onde_choc' actuellement geres.
}

//------------------------------------------------------------------------------
// Collision de l'onde de choc avec un enemi ?
//------------------------------------------------------------------------------
bool Tst_Col_Onde_Choc(int numero_nmi)
{
  int                     i, centre_nmi_x, centre_nmi_y, dist_centre_centre ;
  int                     dx, dy, resultat;
  centre_nmi_x = (int)(ve[numero_nmi].spr.coor_x + ve[numero_nmi].spr.img[ve[numero_nmi].spr.img_act]->x_cg);
  centre_nmi_y = (int)(ve[numero_nmi].spr.coor_y + ve[numero_nmi].spr.img[ve[numero_nmi].spr.img_act]->y_cg);
  for(i = 0; i < nbr_onde_choc; i++)                           // On traite toutes les ondes de choc existantes.
  { cmpt_onde_choc = liste_onde_choc[i];                       // retourne l'indice dans le tableau 'onde_choc' du nieme element de la liste.
    dx = centre_nmi_x - onde_choc[cmpt_onde_choc].orig_x;      // Calcul de la distance entre le centre de l'ennemi et le centre de l'onde de choc.
    dy = centre_nmi_y - onde_choc[cmpt_onde_choc].orig_y;
    resultat = (dx * dx) + (dy * dy);
    dist_centre_centre = (int)sqrt(resultat);
    // Test si l'anneau est en collision avec le vaisseau ennemi.
    if(dist_centre_centre >= (27 + (onde_choc[cmpt_onde_choc].cmpt_cercle * 5))
       && dist_centre_centre <= (43 + (onde_choc[cmpt_onde_choc].cmpt_cercle * 5)))
    { Gene_Bonus_Malus(&ve[numero_nmi]);                       // appel de la routine utilisee pour generer soit un bonus soit un malus.
      return TRUE;                                             // indique que l'ennemi est en collision avec l'onde de choc.
    }
  }
  return FALSE;                                                // Indique que l'ennemi n'est pas en collision avec l'onde de choc.
}

//------------------------------------------------------------------------------
// Initialisation du tableau 'onde_choc' ainsi que du nombre d'ondes de choc.
//------------------------------------------------------------------------------
void Init_Tab_OndeChoc(void)
{
  int                     i;
  for(i = 0; i < NBR_ONDE_CHOC_MAX; i++)                       // boucle sur le nombre d'ondes de choc maximums.
  { onde_choc[i].next_object = NBR_ONDE_CHOC_MAX;              // indique que l'objet actuel est libre.
    onde_choc[i].prev_object = NBR_ONDE_CHOC_MAX;              // indique que l'objet actuel est libre.
    onde_choc[i].cmpt_color_aff = 0;                           // RAZ de la couleur d'affichage de l'onde de choc.
  }
  nbr_onde_choc = 0;                                           // initialise le nombre d'ondes de choc.
  for(i = 0; i < NBR_ONDE_CHOC_MAX; i++) liste_onde_choc[i] = 0;
}

//------------------------------------------------------------------------------
// draw one polygon
// input => adrecr    : pointer to the screen buffer  
//       => tableau   : table of coordinates
//       => numpts    : number of points
//       => _iCouleur
// Copyright E.SOBOLE - EKO System
// une super routine de polygone
// j'ai ultra commente. comme ca tu pourras meme apprendre quelques chose.
// j'ai pas compiler. donc j'espere que toute les variable sont definies.
// j'ai (par consequent) pas tester... si ca c'est pas de la confiance en soit!!!
// a bientot.
// PS : si t'oublie ma tondeuse je te casse la tete.
// Etienne
//------------------------------------------------------------------------------
void polygone(char *adrecr, int *tableau, int numpts, unsigned int _iCouleur)
{
  int                     i, j, x;
  int                     x1, x2, y1, y2;
  int                     min, max;
  int                     pas;
  static int              gch[HAUT_ECR_RAM_TOT];               // static evite les malloc pas trop glop
  static int              drt[HAUT_ECR_RAM_TOT];
  static int              dy;
  static int              dx;
  int                     mondx;
  char                   *work;
  char                   *tmpwork;
  switch (iOctetsParPixel)
  { case 2:
      _iCouleur = pal16[_iCouleur];
      break;
    case 3:
      _iCouleur = pal32[_iCouleur];
      break;
    case 4:
      _iCouleur = pal32[_iCouleur];
      break;
  }
  min = 10000000;                                              // je pense pas qu'il soit possible de depasser
  max = -10000000;                                             // ces valeurs avant quelques millenaires
  for(i = 0; i < numpts * 2; i += 2)
  { y1 = tableau[i + 1];
    y2 = tableau[i + 3];
    dy = y2 - y1;
    if(dy != 0)                                                // si le segment est plat on s'en occupe meme pas
    { if(y1 < min) min = y1;
      if(y1 > max) max = y1;
      x1 = tableau[i];
      x2 = tableau[i + 2];
      dx = x2 - x1;
      if(dy > 0)                                               // buffer de gauche
      { pas = (dx << 16) / dy;                                 // le pas
        x = x1 << 16;                                          // 16 bit de poids faible
        for(j = y1; j <= y2; j++)                              // y1 < y2 donc c'est bon
        { gch[j] = x;                                          // ces 2 lignes peuvent passer en assembleur
          x += pas;                                            // mais pour les petits polygone c'est bof...
        }
      }
      else                                                     // buffer de droite
      { pas = (dx << 16) / dy;                                 // le pas
        x = x2 << 16;                                          // 16 bit de poids faible
        for(j = y2; j <= y1; j++)                              // y2 < y1 donc c'est toujours bon.
        { drt[j] = x;                                          // voir plus haut pour le passage en asm
          x += pas;
        }
      }
    }
  }
  tmpwork = adrecr + min * LARG_ECR_RAM_TOT * iOctetsParPixel; // on pointe sur le debut de la bonne ligne
  work = tmpwork;                                              // work est le pointeur de travail
  for(i = min; i < max; i++)                                   // une ligne de moins pour une belle jointure
  {
    x1 = gch[i] >> 16;                                         // on vire le bit de partie flottante
    x2 = drt[i] >> 16;
    mondx = x2 - x1;                                           // nombre de poxel a tracer.
    work += (x1 * iOctetsParPixel);                            // on pointe sur le premier pixel a afficher
    switch (iOctetsParPixel)
    { case 1:
        poly8bits(work, mondx, _iCouleur);
        break;
      case 2:
        poly16bits(work, mondx, _iCouleur);
        break;
      case 3:
        poly24bits(work, mondx, _iCouleur);
        break;
      case 4:
        poly32bits(work, mondx, _iCouleur);
        break;
    }
    //for (j = 0 ; j < dx ; j++)                                 // un pixel de moins pour une belle jointure
    // *work++ = _iCouleur;                                      // ces 2 lignes en assembleur et ca va 3 fois plus vite
    //*(pp++)=kk ;
    tmpwork += iLargeurEcranTotal;                             // on pointe sur la ligne suivante
    work = tmpwork;
  }
}

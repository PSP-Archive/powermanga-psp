//******************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//------------------------------------------------------------------------------
// file         : "fntscrol.cpp" 
// created      : ?
// updates      : 2005-01-07
//------------------------------------------------------------------------------
// functions    : Init_Scroll_Present()
//              : Destruct_Scroll_Font(int)
//              : Load_Font_Scroll()
//              : New_Element_FS()
//              : Del_Element_FS(int, int)
//              : Gestion_Fontes_Ki_Scrollent()
//------------------------------------------------------------------------------
// descriptions : scroll-text's menu
//------------------------------------------------------------------------------
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
//..............................................................................
extern configfile*	power_conf;
extern fonte            fnt_scroll[NBR_FONTES_SCROLL];         //Tableau de variables utilise pour l'affichage des caracteres qui scrollent.
extern int              liste_fnt_scroll[NBR_FNT_SCROLL_MAX];  // Définit la liste de toutes les fontes qui scrollent que l'on doit gérés à chaque VBLs.
extern int              cmpt_fnt_scroll;                       // Utilisé dans les bcles pour compter le nombre de fontes qui scrollent.
extern short            nbr_fnt_scroll;                        // Indique le nombre de fontes qui scrollent actuellement gérés.
extern int              new_fnt_scroll;                        // Indique l'indice du tableau corespondant à un nouvel objet fonte qui scrolle.
extern fntscroll        fnt_scr[NBR_FNT_SCROLL_MAX];           // Définit le tableau de tous les objets fontes qui scrollent.
extern char             szMsg[1000];
extern char            *ecran_ram;                             //adresse ecran logique.
extern curve            initial_curve[NBR_CURVE_MAX];          // Déclaration de la structure des courbes de bézier.
char                    s_big_util[3000];                      // Tableau utilise pour l'affichage du scrolling.
bool                    scroll_text;
short                   cmpt_util_fnt;
short                   len_s_util;
short                   cmpt_vbl_util;
short                   nbr_vbl_2_change;
short                   num_crb_scroll;
short                   type_de_scrolling;
short                   pos_x_scroll, pos_y_scroll;
float                   vit_scroll_txt;
bool                    dir_font_x;
bool                    dir_font_y;                            // Indique le sens des deplacements.
char                   *s_empty ="                                                                   ";

// scroll text (now loaded from a file in Load_Scroll_Text())
char*	s_present = NULL;


//------------------------------------------------------------------------------
// load scroll-text
//------------------------------------------------------------------------------	
int Load_Scroll_Text()
{
	char* pLang = power_conf->get_txtlang();
	char fname[256];
	sprintf(fname, "texts/scroll_%s.txt", pLang);
	Uint32 bsize;
	char *pTemp = chargeFichier(fname, &bsize);
	if (!pTemp) return 0;
	
	s_present = reserveMemoire(bsize);
	if (!s_present)
	{	libereMemoire(pTemp);
		return 0;
	}
	
	Uint32 j = 0;
	for(Uint32 i = 0; i < bsize; i++)
	{	if(pTemp[i] >= ' ' && pTemp[i] != '"')
			s_present[j++] = pTemp[i];
	}
	libereMemoire(pTemp);
	return 1;
}

//------------------------------------------------------------------------------
// Free scroll-text
//------------------------------------------------------------------------------	
void Free_Scroll_Text()
{
	if (s_present) 
	{	libereMemoire (s_present);
		s_present = NULL;
	}
}

	
//------------------------------------------------------------------------------
// initialize scroll-text
//------------------------------------------------------------------------------
void Init_Scroll_Present()
{
  scroll_text = 1;                                             // Indique que le scroll texte doit etre actif.
  strcpy(s_big_util, s_present);                               // Copy la chaine de presentation dans la chaine utilisee pour le scrolling.
  len_s_util = (short)strlen(s_big_util);                      // Recherche la longueur de la chaine.
  cmpt_util_fnt = 0;                                           // Initialise le compteur temporaire.
  nbr_vbl_2_change = 16;                                       // Initialise le nombre de vbls entre les ajouts de lettres.
  cmpt_vbl_util = 0;                                           // Initialise le compteur de vbls.
  num_crb_scroll = 121;                                        // Donne le numero de la courbe utilisse pour le scrolling.
  pos_x_scroll = DEBUT_ECRAN_X + LARG_ECR_RAM;                 // Donne la position X du scrolling.
  pos_y_scroll = 291;                                          // Donne la position Y du scrolling.
  vit_scroll_txt = -1.25;                                      // Donne la vitesse de deplacement des caracteres.
  type_de_scrolling = SCROLL_PRESENT;                          // Defini le type de scrolling utiliser.
  dir_font_x = 1;                                              // Indique que les fontes vont se deplacees en x.
  dir_font_y = 0;                                              // Indique que les fontes ne vont pas se deplacees en y.
}

// Routine utilisee pour faire disparaitre un scrolling de texte.
void Destruct_Scroll_Font(int scroll_2_destruct)
{
  strcpy(s_big_util, s_empty);                                 // Copy la chaine de presentation dans la chaine utilisee pour le scrolling.
  len_s_util = (short)strlen(s_big_util);                      // Recherche la longueur de la chaine.
  cmpt_util_fnt = 0;                                           // Initialise le compteur temporaire.
  // On traite tous les fontes qui scrollent existantes.
  for(int i = 0; i < nbr_fnt_scroll; i++)
  {                                                            // Retourne l'indice dans le tableau 'fnt_scr' du nième élément de la liste.
    cmpt_fnt_scroll = liste_fnt_scroll[i];
    switch (scroll_2_destruct)
    {                                                          // Cas du scrolling de presentation.
      case SCROLL_PRESENT:
        // Test si on est en presence d'une fonte du scrolling de presentation.
        if(fnt_scr[cmpt_fnt_scroll].typ_of_scroll == SCROLL_PRESENT)
        {                                                      // Changement de la vitesse du caractere afin qu'il disparaisse vers le bas.
          fnt_scr[cmpt_fnt_scroll].vitesse = 1;                // Donne la vitesse de deplacement des caracteres.
          fnt_scr[cmpt_fnt_scroll].dir_x = 0;                  // Indique que l'on ne se deplace pas sur les x.
          fnt_scr[cmpt_fnt_scroll].dir_y = 1;                  // Indique que l'on se deplace sur les y.
        }
        break;
    }
  }                                                            // Fin de la boucle sur toutes les fontes en train de scroller.
}

//------------------------------------------------------------------------------
// scroll-text : get a new element from list
//------------------------------------------------------------------------------
int New_Element_FS()
{
  int                     l, m;

  // Test s'il y a plusieurs éléments dans le tableau.
  if(nbr_fnt_scroll)                                           // Si oui, on recherche un objet libre pour attribution.
  {                                                            // Recherche d'un objet libre dans le tableau.
    for(m = 0; m < NBR_FNT_SCROLL_MAX; m++)
    {                                                          // Si une valeur next_object est égale au nombre maximum d'objets du tableau, c'est qu'on tient un objet libre.
      if(fnt_scr[m].next_object == NBR_FNT_SCROLL_MAX)
        goto New_Element_Fnt_Find;
    }
  New_Element_Fnt_Find:;

    // On doit à présent modifier le dernier élément de la liste pour le chaîner au nouvel élément.
    l = liste_fnt_scroll[nbr_fnt_scroll - 1];                  // Donne le dernier élément de la liste.
    fnt_scr[l].next_object = m;                                // Attribution de l'élément suivant à ce qui était le dernier élément de la liste.
    fnt_scr[m].prev_object = l;                                // Attribution de l'élément précédent au dernier élément de la liste.
    fnt_scr[m].next_object = -1;                               // Indique  que cet élément est le dernier de la liste.
    liste_fnt_scroll[nbr_fnt_scroll] = m;                      // Met à jour la liste des 'fnt_scr'.
    nbr_fnt_scroll++;                                          // Incrémente le nombre d'objets 'fnt_scr' actuellement gérés.
    return (m);
  }

  // Si on arrive en ce point c'est que le tableau est actuellement vide. On peux alors attribuer le premier élément.
  fnt_scr[0].prev_object = -1;
  fnt_scr[0].next_object = -1;
  liste_fnt_scroll[nbr_fnt_scroll] = 0;                        // Met à jour la liste des fontes qui scrollent.
  nbr_fnt_scroll++;                                            // Incrémente le nombre d'objets fontes qui scrollent actuellement gérés.
  return (0);                                                  // Indique que le nouvel élément de la liste est le premier élément du tableau.
}

//------------------------------------------------------------------------------
// scroll-text : remove element
//------------------------------------------------------------------------------
void Del_Element_FS(int n, int o)
{
  int                     m;

  // On doit à présent modifier les éléments suivants et précédent de la liste pour faire un chaînage correct.
  if(fnt_scr[n].prev_object != -1)
    fnt_scr[fnt_scr[n].prev_object].next_object = fnt_scr[n].next_object;       // Attribution de l'élément precedent de la liste.
  if(fnt_scr[n].next_object != -1)
    fnt_scr[fnt_scr[n].next_object].prev_object = fnt_scr[n].prev_object;       // Attribution de l'élément suivant de la liste.
  fnt_scr[n].prev_object = NBR_FNT_SCROLL_MAX;                 // Indique que l'objet actuel est libre.
  fnt_scr[n].next_object = NBR_FNT_SCROLL_MAX;                 // Indique que l'objet actuel est libre.
  // Met à jour la liste des 'fnt_scr'.
  for(m = o; m < (nbr_fnt_scroll - 1); m++)
    liste_fnt_scroll[m] = liste_fnt_scroll[m + 1];
  nbr_fnt_scroll--;                                            // Décrémente le nombre d'objets 'fnt_scr' actuellement gérés.
}

//------------------------------------------------------------------------------
// scroll-text : run-time
//------------------------------------------------------------------------------
// Routine utilisee pour la gestion des fontes qui scrollent.
void Gestion_Fontes_Ki_Scrollent()
{                                                              //static char t_go, val_fonte;
  static char             val_fonte;
  int                     i;

  // Test si on doit ou non gerer le scrolling de texte.
  if(scroll_text)
  {                                                            // Test si l'on doit ou non passer a la lettre suivante.
    if(cmpt_vbl_util >= nbr_vbl_2_change)
    {                                                          // Remet le compteur de vbls a zero pour les prochains passages.
      cmpt_vbl_util = 0;
      // Test s'il est possible d'ajouter une fonte qui scrolle à la liste.
      if(nbr_fnt_scroll < (NBR_FNT_SCROLL_MAX - 1))
      {                                                        // Test si on est en presence d'une fonte connue.
        val_fonte = (char)(s_big_util[cmpt_util_fnt] - 33);
        if(val_fonte >= 0 && val_fonte < NBR_FONTES_SCROLL)
        {                                                      // Recherche d'une nouvelle fonte.
          new_fnt_scroll = New_Element_FS();
          // Remplissage de la structure de la fonte.
          fnt_scr[new_fnt_scroll].clignote = 0;                // Indique que l'on est en présence d'un sprite qui doit clignoter.
          fnt_scr[new_fnt_scroll].spr.trajectoire = 2;         // Indique que la trajectoire du sprite est définie par une courbe.
          fnt_scr[new_fnt_scroll].num_courbe = num_crb_scroll; // Indique le numéro de la courbe suivie par le sprite.
          fnt_scr[new_fnt_scroll].pos_fonte[POS_CURVE] = 0;    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
          fnt_scr[new_fnt_scroll].spr.nbr_img = 1;             // Indique le nombre d'images composant le sprite.
          // Donne les coordonnées d'affichage du sprite.
          fnt_scr[new_fnt_scroll].coor_x = pos_x_scroll;
          fnt_scr[new_fnt_scroll].coor_y = pos_y_scroll;
          // Donne la vitesse de deplacement du sprite.
          fnt_scr[new_fnt_scroll].vitesse = -1;                //vit_scroll_txt ;
          // Donne l'adresse de l'image de la fonte.
          fnt_scr[new_fnt_scroll].num_image = val_fonte;
          fnt_scr[new_fnt_scroll].typ_of_scroll = type_de_scrolling;
          fnt_scr[new_fnt_scroll].dir_x = dir_font_x;          // Indique le deplacement en x.
          fnt_scr[new_fnt_scroll].dir_y = dir_font_y;          // Indique le deplacement en x.
        }
        cmpt_util_fnt++;
        if(cmpt_util_fnt >= len_s_util)
          cmpt_util_fnt = 0;
      }
    }
    cmpt_vbl_util++;                                           // Incremente le compteur utilise pour changer de lettre.
  }

  // On traite tous les fontes qui scrollent existantes.
  for(i = 0; i < nbr_fnt_scroll; i++)
  {                                                            // Retourne l'indice dans le tableau 'fnt_scr' du nième élément de la liste.
    cmpt_fnt_scroll = liste_fnt_scroll[i];
    // Test si on doit incrementer les X.
    if(fnt_scr[cmpt_fnt_scroll].dir_x)
      fnt_scr[cmpt_fnt_scroll].coor_x += fnt_scr[cmpt_fnt_scroll].vitesse;      // Incrementation de la position du caractere a faire scroller.
    // Test si on doit incrementer les Y.
    if(fnt_scr[cmpt_fnt_scroll].dir_y)
      fnt_scr[cmpt_fnt_scroll].coor_y += fnt_scr[cmpt_fnt_scroll].vitesse;      // Incrementation de la position du caractere a faire scroller.

    // Test si on doit ou non afficher les fontes.
    if((short)(fnt_scr[cmpt_fnt_scroll].coor_x + 32) < DEBUT_ECRAN_X ||
       (short)(fnt_scr[cmpt_fnt_scroll].coor_y + 32) < DEBUT_ECRAN_Y ||
       (short)fnt_scr[cmpt_fnt_scroll].coor_y > DEBUT_ECRAN_Y + HAUT_ECR_RAM
       || (short)fnt_scr[cmpt_fnt_scroll].coor_x >
       DEBUT_ECRAN_X + LARG_ECR_RAM)
    {                                                          // On enlève la fonte de la liste des 'fnt_scr'.
      Del_Element_FS(cmpt_fnt_scroll, i);                      // Routine utilisée pour supprimer une fonte du tableau des 'fnt_scr'.
      // On se positionne sur la fonte que l'on vient de décaler.
      i--;
      // On sort de la bcle de gestion des fontes qui scrollent.
      goto fin_bcle_gestion_fonte;
    }
    else                                                       // Sinon, on doit l'afficher.
    {                                                          // Test si l'on est en présence d'un sprite qui clignote.
      if(fnt_scr[cmpt_fnt_scroll].clignote)                    // Ceci afin de faire clignoter le sprite.
      {                                                        // Incrémente le compteur de VBLs pour l'affichage de l'animation.
        fnt_scr[cmpt_fnt_scroll].spr.nbr_vbl_act++;
        // Test si l'on à atteint le nombre de VBLs nécessaires pour le changement d'image.
        if(fnt_scr[cmpt_fnt_scroll].spr.nbr_vbl_act > 1)
        {                                                      // On initialise le compteur de VBLs.
          fnt_scr[cmpt_fnt_scroll].spr.nbr_vbl_act = 0;
          // Affichage de l'image de la fonte.
          putsprite3(&fnt_scroll[fnt_scr[cmpt_fnt_scroll].num_image], fnt_scr[cmpt_fnt_scroll].coor_x, fnt_scr[cmpt_fnt_scroll].coor_y);
        }
      }
      else                                                     // Cas d'une fonte qui ne clignote pas.
      {                                                        // Affichage de l'image de la fonte.
        //putsprite3(fnt_scroll[fnt_scr[cmpt_fnt_scroll].num_image].img,ecran_ram+((short)fnt_scr[cmpt_fnt_scroll].coor_y*LARG_ECR_RAM_TOT)+(short)fnt_scr[cmpt_fnt_scroll].coor_x,fnt_scroll[fnt_scr[cmpt_fnt_scroll].num_image].compress,(short)(fnt_scroll[fnt_scr[cmpt_fnt_scroll].num_image].nbr_data_comp>>2),"fnt_scroll[fnt_scr[cmpt_fnt_scroll].num_image].img");
        putsprite3(&fnt_scroll[fnt_scr[cmpt_fnt_scroll].num_image],
                   fnt_scr[cmpt_fnt_scroll].coor_x,
                   fnt_scr[cmpt_fnt_scroll].coor_y);
      }
    }                                                          // Fin du test pour savoir si on doit afficher la fonte ou pas.
// Label utilisé pour sortir de la bcle de gestion des fontes qui scrollent.
  fin_bcle_gestion_fonte:;
  }
}

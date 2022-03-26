//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "explosionExecution.cpp"
// created	: ?
// updates	: 2005-01-09
// id		: $Id: explosionExecution.cpp,v 1.1.1.1 2005/01/25 05:52:46 gurumeditation Exp $
//-----------------------------------------------------------------------------
// functions : void   explosionExecution()
//             void   Add_Explosion(float c_x, float c_y, float vitesse, int type, int nbr_vbl )
//             void   Add_Eclats(float c_x, float c_y, float vitesse, int nbre, int nbr_vbl, short nbr_tot_vbl)
//             void   Del_Element_Exp(int n, int o)
//             int    New_Element_Exp()
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
//------------------------------------------------------------------------------
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //structure des vaisseaux ennemis
extern vaisseau         vj[NBR_MAX_J];                         //structure des vaisseau joueur
extern char            *ecran_ram;                             //adresse ecran logique.
extern explosion        xpl[MAX_NMI_ON_SCREEN];                //structure utilisee par les explosions
extern int              liste_xpl[MAX_NMI_ON_SCREEN];          //liste de tous les 'xpl' geres
extern int              cmpt_exps;                             //compteur du nombre d'explosions
extern short            nbr_exps;                              //nombre d'explosions actuellement gerees
extern float            depix[13][32];                         //tableau de deplacement en X
extern float            depiy[13][32];                         //tableau de deplacement en Y
extern image            star_field[TYPE_OF_STARS][NBR_ETOILES]; //images utilisees par les etoiles
extern image            eclat[MAX_TYP_OF_ECLATS][NBR_IMAGES_ECLATS];    //images utilisees pour les eclats
extern image            explo[MAX_TYP_OF_EXPLOS][NBR_IMAGES_EXP];       //images utilisees pour les explosions
extern unsigned int     iCompteurGlobal;                       //global counter

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void explosionExecution()
{
  int i;

  //gestion des explosions (grosses, moyennes, petites, et speciales) -----------
  for(i = 0; i < nbr_exps; i++)
  {
    cmpt_exps = liste_xpl[i];                                  //index sur le tableau "xpl"
    xpl[cmpt_exps].coor_x += depix[(short)xpl[cmpt_exps].vitesse][xpl[cmpt_exps].img_angle];    //nouvelle abscisse
    xpl[cmpt_exps].coor_y += depiy[(short)xpl[cmpt_exps].vitesse][xpl[cmpt_exps].img_angle];    //nouvelle ordonnee

    // Test si le nombre de vbl avant l'explosion est superieur à zéro.
    if(xpl[cmpt_exps].nbr_vbl_av_exp > 0)
      xpl[cmpt_exps].nbr_vbl_av_exp--;
    else                                                       // Sinon, on peux effectuer le traitement de l'explosion.
    {
      xpl[cmpt_exps].nbr_vbl_act++;                            //incremente le compteur de tempo de l'animation
      if(xpl[cmpt_exps].nbr_vbl_act >= xpl[cmpt_exps].nbr_tot_vbl)      //passe a l'image suivante ?
      {
        xpl[cmpt_exps].nbr_vbl_act = 0;
        xpl[cmpt_exps].img_act++;                              // passe a l'image suivante
      }
      // Test si l'explosion doit être affichée.
      if(                                                      // Si le nombre maximum d'images est atteint pour une explosion.
          (xpl[cmpt_exps].nbr_tot_vbl < 5
           && xpl[cmpt_exps].img_act >= NBR_IMAGES_EXP) ||
          // Si le nombre maximum d'images est atteint pour une étoile.
          (xpl[cmpt_exps].nbr_tot_vbl > 3
           && xpl[cmpt_exps].img_act >= NBR_ETOILES) ||
          // Si l'explosion est dépasse en bas de l'écran virtuel.
          (((short)xpl[cmpt_exps].coor_y +
            xpl[cmpt_exps].img[xpl[cmpt_exps].img_act]->haut) >=
           (HAUT_ECR_RAM_TOT - 1)) ||
          // Si l'explosion est dépasse à droite de l'écran virtuel.
          (((short)xpl[cmpt_exps].coor_x +
            xpl[cmpt_exps].img[xpl[cmpt_exps].img_act]->larg) >=
           (LARG_ECR_RAM_TOT - 1)))
      {                                                        // On doit supprimer l'explosion de la liste des explosions.
        Del_Element_Exp(cmpt_exps, i);                         // Routine utilisée pour supprimer une explosion du tableau des 'xpl'.
        // On se positionne sur l'explosion que l'on vient de décaler.
        i--;
      }
      else                                                     // On doit afficher l'explosion.
      {                                                        // Affichage de l'explosion.
        putsprite3(xpl[cmpt_exps].img[xpl[cmpt_exps].img_act],
                   (unsigned int)xpl[cmpt_exps].coor_x, 
                   (unsigned int)xpl[cmpt_exps].coor_y);
      }
    }
  }
}


//------------------------------------------------------------------------------
// ajoute une explosion a la liste
// input => c_x     : abscisse de l'explosion
//       => c_y     : ordonnee de l'explosion
//       => vitesse : vitesse de deplacement de l'explosion
//       => type    : 0=32*32 / 1=64*14 / 2=16*16 / 3=etoile sur le vaisseau joueur
//       => nbr_vbl : duree d'affichage de l'explosion
//------------------------------------------------------------------------------
void Add_Explosion(float c_x, float c_y, float vitesse, int type, int nbr_vbl)
{
  int k, new_xpl;
  switch (type)
  {
    //explosion de taille 32*32 ------------------------------------------------
    case 0:
      new_xpl = New_Element_Exp();                             //demande un nouvel element 'xpl' 
      if(new_xpl >= 0)                                         //nouvel element trouve ?
      {
        xpl[new_xpl].img_act = 0;                              //commence image 0 
        xpl[new_xpl].nbr_tot_vbl = 1;                          //vitesse d'animation
        xpl[new_xpl].nbr_vbl_act = 0;                          //raz le compteur d'animation
        for(k = 0; k < NBR_IMAGES_EXP; k++)                    //calcul adresses des images 
        {
          xpl[new_xpl].img[k] = (image *) & explo[1][k];
        }
        xpl[new_xpl].coor_x = c_x - 8;                         //abscisse d'affichage
        xpl[new_xpl].coor_y = c_y - 8;                         //ordonnee d'affichage
        xpl[new_xpl].vitesse = vitesse;                        //vitesse de deplacement
        xpl[new_xpl].img_angle = 8;
        xpl[new_xpl].nbr_vbl_av_exp = nbr_vbl;                 //duree d'affichage de l'explosion
#ifdef USE_SDLMIXER
        seal_joue(XPLOB1 + (iCompteurGlobal & 3));             //joue un son avec seal
#endif
      }
      break;

      //explosion de taille 64*64 ----------------------------------------------
    case 1:
      new_xpl = New_Element_Exp();                             //demande un nouvel element 'xpl' 
      if(new_xpl >= 0)                                         //nouvel element trouve ?
      {
        xpl[new_xpl].img_act = 0;                              //commence image 0 
        xpl[new_xpl].nbr_tot_vbl = 1;                          //vitesse d'animation
        xpl[new_xpl].nbr_vbl_act = 0;                          //raz le compteur d'animation
        for(k = 0; k < NBR_IMAGES_EXP; k++)
        {
          xpl[new_xpl].img[k] = (image *) & explo[2][k];
        }
        xpl[new_xpl].coor_x = c_x - 16;                        //abscisse d'affichage
        xpl[new_xpl].coor_y = c_y - 16;                        //ordonnee d'affichage
        xpl[new_xpl].vitesse = vitesse;                        //vitesse de deplacement
        xpl[new_xpl].img_angle = 8;
        xpl[new_xpl].nbr_vbl_av_exp = nbr_vbl;                 //duree d'affichage de l'explosion
#ifdef USE_SDLMIXER
        seal_joue(XPLOC1 + (iCompteurGlobal & 3));             //joue explosion un son avec seal
#endif
      }
      break;
      //explosion de taille 16*16 ----------------------------------------------------
    case 2:
      new_xpl = New_Element_Exp();    //demande un nouvel element 'xpl' 
      if(new_xpl >= 0)                //nouvel element trouve ?
      { xpl[new_xpl].img_act = 0;     //commence image 0 
        xpl[new_xpl].nbr_tot_vbl = 1; //vitesse d'animation
        xpl[new_xpl].nbr_vbl_act = 0; //raz compteur d'animation
        for(k = 0; k < NBR_IMAGES_EXP; k++)
        { xpl[new_xpl].img[k] = (image *) & explo[0][k];
        }
        xpl[new_xpl].coor_x = c_x - 4;  //abscisse
        xpl[new_xpl].coor_y = c_y - 4;  //ordonnee
        xpl[new_xpl].vitesse = vitesse; //vitesse deplacement
        xpl[new_xpl].img_angle = 8;
        xpl[new_xpl].nbr_vbl_av_exp = nbr_vbl; //duree d'affichage explosion
#ifdef USE_SDLMIXER
        seal_joue(XPLOA1 + (iCompteurGlobal & 3));  //son explosion
#endif
      }
      break;
      //etoile si le vaisseau du joueur change --------------------------------------- 
    case 3:
      new_xpl = New_Element_Exp();                             //demande un nouvel element 'xpl' 
      if(new_xpl >= 0)                                         //nouvel element trouve ?
      { xpl[new_xpl].img_act = 0;                              //commence image 0 
        xpl[new_xpl].nbr_tot_vbl = 6;                          //vitesse d'animation
        xpl[new_xpl].nbr_vbl_act = 0;                          //raz le compteur d'animation
        for(k = 0; k < NBR_ETOILES; k++)
        { xpl[new_xpl].img[k] = (image *) & star_field[PLAYERSTAR][k];
        }
        xpl[new_xpl].coor_x = c_x;                             //abscisse d'affichage
        xpl[new_xpl].coor_y = c_y;                             //ordonnee d'affichage
        xpl[new_xpl].vitesse = vitesse;                        //vitesse de deplacement
        xpl[new_xpl].img_angle = 8;
        xpl[new_xpl].nbr_vbl_av_exp = nbr_vbl;                 //duree d'affichage de l'explosion
      }
      break;
  }
}

//------------------------------------------------------------------------------
// ajoute une explosion a la liste (routine special gardien)
// input => c_x : abscisse de l'explosion
//       => c_y : ordonnee de l'explosion
//------------------------------------------------------------------------------
void Add_Explosion_Guard(float c_x, float c_y)
{
  int k, new_xpl;
  new_xpl = New_Element_Exp();    //demande un nouvel element 'xpl' 
  if(new_xpl >= 0)                //nouvel element trouve ?
  { xpl[new_xpl].img_act = 0;     //commence image 0 
    xpl[new_xpl].nbr_tot_vbl = 1; //vitesse d'animation
    xpl[new_xpl].nbr_vbl_act = 0; //raz compteur d'animation
    for(k = 0; k < NBR_IMAGES_EXP; k++)
      xpl[new_xpl].img[k] = (image *) & explo[0][k];
    xpl[new_xpl].coor_x = c_x - 4;  //abscisse
    xpl[new_xpl].coor_y = c_y - 4;  //ordonnee
    xpl[new_xpl].vitesse = -.5;     //vitesse deplacement
    xpl[new_xpl].img_angle = 8;
    xpl[new_xpl].nbr_vbl_av_exp = 0; //duree d'affichage explosion
#ifdef USE_SDLMIXER
    if(!(rand() % 8))
    { seal_joue(XPLOA1 + (iCompteurGlobal & 3));  //son explosion
    }
#endif
  }
}

//------------------------------------------------------------------------------
// ajouter des eclat(s) a la liste
// input => c_x     : abscisse de l'explosion
//       => c_y     : ordonnee de l'explosion
//       => vitesse : vitesse de deplacement de l'explosion
//       => nbre    : nombre d'eclats a ajouter
//       => nbr_vbl : duree d'affichage de l'explosion
//------------------------------------------------------------------------------
void Add_Eclats(float c_x, float c_y, float vitesse, int nbre, int nbr_vbl,
                short nbr_tot_vbl)
{
  int                     i, k, new_xpl;

  for(i = 0; i < nbre; i++)
  {
    if(nbr_exps < (MAX_NMI_ON_SCREEN - 1))                     //maximum d'eclats ?
    {
      new_xpl = New_Element_Exp();                             //demande un nouvel element "exp"
      if(new_xpl >= 0)                                         //element "exp" disponible ?
      {
        xpl[new_xpl].img_act = 0;                              //numero de l'image actuelle
        xpl[new_xpl].nbr_tot_vbl = nbr_tot_vbl;                //temporisation animation
        xpl[new_xpl].nbr_vbl_act = 0;                          //compteur temporisation animation
        int                     num_eclat = rand() % MAX_TYP_OF_ECLATS; //choisis un eclat au hasard

        for(k = 0; k < NBR_IMAGES_ECLATS; k++)
          xpl[new_xpl].img[k] = (image *) & eclat[num_eclat][k];
        xpl[new_xpl].coor_x = c_x;                             //abscisse d'affichage
        xpl[new_xpl].coor_y = c_y;                             //ordonnee d'affcihage
        xpl[new_xpl].vitesse = vitesse;                        //vitesse de deplacement
        xpl[new_xpl].img_angle = rand() % NBR_ANGLES;
        xpl[new_xpl].nbr_vbl_av_exp = nbr_vbl;                 //temps avant affichage
      }
    }
  }
}

//------------------------------------------------------------------------------
// retourne un indice sur un objet "xpl" libre dans le tableau "xpl"
// ouput <= indice de 1 a ?
//------------------------------------------------------------------------------
int New_Element_Exp()
{
  int                     l, m;

  if(nbr_exps)                                                 // Si oui, on recherche un objet libre pour attribution.
  {                                                            // Recherche d'un objet libre dans le tableau.
    for(m = 0; m < MAX_NMI_ON_SCREEN; m++)
    {                                                          // Si une valeur next_object est égale au nombre maximum d'objets du tableau, c'est qu'on tient un objet libre.
      if(xpl[m].next_object == MAX_NMI_ON_SCREEN)
        goto New_Element_Exp_Find;
    }
    // Indique qu'aucune valeur n'à été trouvée alors, on sort avec une erreur.
    return -1;
    // On est dans le cas ou une valeur libre à été trouvée.
New_Element_Exp_Find:;

    // On doit à présent modifier le dernier élément de la liste pour le chaîner au nouvel élément.
    l = liste_xpl[nbr_exps - 1];                               // Donne le dernier élément de la liste.
    xpl[l].next_object = m;                                    // Attribution de l'élément suivant à ce qui était le dernier élément de la liste.
    xpl[m].prev_object = l;                                    // Attribution de l'élément précédent au dernier élément de la liste.
    xpl[m].next_object = -1;                                   // Indique  que cet élément est le dernier de la liste.
    liste_xpl[nbr_exps] = m;                                   // Met à jour la liste des 'xpl'.
    nbr_exps++;                                                // Incrémente le nombre d'objets 'xpl' actuellement gérés.
    return (m);
  }
  xpl[0].prev_object = -1;
  xpl[0].next_object = -1;
  liste_xpl[nbr_exps] = 0;                                     // Met à jour la liste des 'xpl'.
  nbr_exps++;                                                  // Incrémente le nombre d'objets 'xpl' actuellement gérés.
  return (0);                                                  // Indique que le nouvel élément de la liste est le premier élément du tableau.
}

//------------------------------------------------------------------------------
// efface un element de la liste 'xpl'
// input => n : index de l'objet sur le tableau 'xpl'
//       => o : index sur la liste "liste_xpl"
//------------------------------------------------------------------------------
void Del_Element_Exp(int n, int o)
{
  int                     m;

  //modifie elements suivants & precedents (liste chainee)
  if(xpl[n].prev_object != -1)
    xpl[xpl[n].prev_object].next_object = xpl[n].next_object;
  if(xpl[n].next_object != -1)
    xpl[xpl[n].next_object].prev_object = xpl[n].prev_object;
  xpl[n].prev_object = MAX_NMI_ON_SCREEN;                      //indique que l'objet actuel est libre
  xpl[n].next_object = MAX_NMI_ON_SCREEN;                      //indique que l'objet actuel est libre
  //met a jour la liste des 'xpl'.
  for(m = o; m < (nbr_exps - 1); m++)
    liste_xpl[m] = liste_xpl[m + 1];
  nbr_exps--;                                                  //decremente le nombre d'objets 'xpl' actuellement geres.
}


//------------------------------------------------------------------------------
// ajouter une serie d'explosions
// input => _iNumeroEnnemi : index sur la liste "ve" / -1=vaisseau joueur
//------------------------------------------------------------------------------
void Add_Serie_Of_Explosion(int _iNumeroEnnemi)
{
  unsigned int            _iTypeVaisseau;
  unsigned int            _iX, _iY, _iL, _iH;
  int                     _iMax;
  int                     _iH32, _iH16, _iH8, _iL32, _iL16, _iL8;

  _iMax = MAX_NMI_ON_SCREEN - 1;
  if(_iNumeroEnnemi >= 0)                                      //vaisseau ennemi
  {
    _iTypeVaisseau = 0;
    if(ve[_iNumeroEnnemi].spr.img[ve[_iNumeroEnnemi].spr.img_act]->larg > 32)
      _iTypeVaisseau += 2;
    if(ve[_iNumeroEnnemi].spr.img[ve[_iNumeroEnnemi].spr.img_act]->haut > 32)
      _iTypeVaisseau += 4;
    _iX = (unsigned int)ve[_iNumeroEnnemi].spr.coor_x;
    _iY = (unsigned int)ve[_iNumeroEnnemi].spr.coor_y;
    _iL = ve[_iNumeroEnnemi].spr.img[ve[_iNumeroEnnemi].spr.img_act]->larg;
    _iH = ve[_iNumeroEnnemi].spr.img[ve[_iNumeroEnnemi].spr.img_act]->haut;
  }
  else
  {
    _iTypeVaisseau = _iNumeroEnnemi;
    _iX = (unsigned int)vj[J1].spr.coor_x;
    _iY = (unsigned int)vj[J1].spr.coor_y;
    _iL = vj[J1].spr.img[vj[J1].spr.img_act]->larg;
    _iH = vj[J1].spr.img[vj[J1].spr.img_act]->haut;
  }
  //force un largeur & hauteur de 1 minimum
  _iH32 = _iH - 32;
  if(_iH32 < 1) _iH32 = 1;                                                 //minimum a 1 (evite division par zero)
  _iH16 = _iH - 16;
  if(_iH16 < 1) _iH16 = 1;
  _iH8 = _iH - 8;
  if(_iH8 < 1) _iH8 = 1;
  _iL32 = _iL - 32;
  if(_iL32 < 1) _iL32 = 1;
  _iL16 = _iL - 16;
  if(_iL16 < 1) _iL16 = 1;
  _iL8 = _iL - 8;
  if(_iL8 < 1) _iL8 = 1;

  switch (_iTypeVaisseau)
  {                                                            //vaisseau du joueur
    case -1:
      if(nbr_exps < _iMax)
        Add_Explosion(_iX, _iY, 0.3f, BIG_EXPLOSION, 0);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL16), _iY + (rand() % _iH16), 0.3f,
                      NORM_EXPLOSION, 20);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL16), _iY + (rand() % _iH16), 0.3f,
                      NORM_EXPLOSION, 30);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL16), _iY + (rand() % _iH16), 0.3f,
                      NORM_EXPLOSION, 40);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL16), _iY + (rand() % _iH16), 0.3f,
                      NORM_EXPLOSION, 50);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 30);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 40);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 50);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 60);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 70);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 80);
      break;
    // ennemi inferieur a 32 pixels en largeur & hauteur
    case 0:
      if(nbr_exps < _iMax)
        Add_Explosion(_iX, _iY, 0.3f, BIG_EXPLOSION, 0);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL16), _iY + (rand() % _iH16), 0.3f,
                      NORM_EXPLOSION, 10);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL16), _iY + (rand() % _iH16), 0.3f,
                      NORM_EXPLOSION, 20);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 30);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 40);
      break;
    //ennemi superieur a 32 pixels de large 
    case 2:
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL32), _iY, 0.3f, BIG_EXPLOSION, 0);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL32), _iY, 0.3f, BIG_EXPLOSION, 10);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL16), _iY + (rand() % _iH16), 0.3f,
                      NORM_EXPLOSION, 20);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL16), _iY + (rand() % _iH16), 0.3f,
                      NORM_EXPLOSION, 30);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 40);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 50);
      break;
    //ennemi superieur a 32 pixels de haut 
    case 4:
      if(nbr_exps < _iMax)
        Add_Explosion(_iX, _iY + (rand() % _iH32), 0.3f, BIG_EXPLOSION, 0);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX, _iY + (rand() % _iH32), 0.3f, BIG_EXPLOSION, 10);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL16), _iY + (rand() % _iH16), 0.3f,
                      NORM_EXPLOSION, 20);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL16), _iY + (rand() % _iH16), 0.3f,
                      NORM_EXPLOSION, 30);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 40);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 50);
      break;
    //ennemi superieur a 32 pixels de large et de haut
    case 6:
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL32), _iY + (rand() % _iH32), 0.3f,
                      BIG_EXPLOSION, 0);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL32), _iY + (rand() % _iH32), 0.3f,
                      BIG_EXPLOSION, 10);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL16), _iY + (rand() % _iH16), 0.3f,
                      NORM_EXPLOSION, 20);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL16), _iY + (rand() % _iH16), 0.3f,
                      NORM_EXPLOSION, 30);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 40);
      if(nbr_exps < _iMax)
        Add_Explosion(_iX + (rand() % _iL8), _iY + (rand() % _iH8), 0.3f,
                      MINI_EXPLOSION, 50);
      break;
  }
}

//*****************************************************************************
// copyright (c) 1998-2003 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "courbe.cpp"
// created      : ?
// updates      : 2003-06-28
//-----------------------------------------------------------------------------
// functions    : void courbe_execution()
//                void courbe_testFin()
//                void courbe_editeur()
//                int  charge_courbes()
//                int  Load_Curve_Level(int numero_level)
//                void Active_Curve_Level()
//                void Find_Tot_Nbr_Vais_Of_Curve()
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
extern curve_level      courbe;                                //structure de niveaux "courbe"
extern vaisseau_nmi     ve[MAX_NMI_ON_SCREEN];                 //structure des vaisseaux ennemis
extern curve            initial_curve[NBR_CURVE_MAX];          //structure des courbes de bezier.
extern image            enemi[MAX_TYP_OF_ENEMI + MAX_TYP_OF_S_ENEMI + MAX_TYP_OF_NMI_SPE + MAX_TYP_OF_OBJ_SPE][NBR_IMAGES_MAX]; // D�init le tableau des diff�ents ennemis.
extern int              new_ve;                                //indice du tableau corespondant �un nouvel objet 've'.
extern vaisseau         vj[NBR_MAX_J];                         // declaration de la structure des vaisseaux des joueurs.
extern bool             all_nmi_killed;
extern bool             meteor_activity;
extern int              gardian_activity;                      // utilise pour savoir si on est dans une phase de gardien ou non.
extern grid             grille;                                // declaration de la structure de grille des vaisseaux ennemis.
extern short            mouse_x;
extern short            mouse_y;
extern short            mouse_b;
extern short            ge_act_pos_x;
extern short            ge_act_pos_y;
extern unsigned char    coulor[12];                            //  couleurs choisies par l'utilisateur.
extern short            touch[];                               // tableau des flags des touches du clavier.
extern char            *ecran_ram;                             // adresse ecran logique.
extern char            *chaine;                                // utilise pour la conversion des chaines de caracteres.
extern int              tmp_tsts_x;
extern int              tmp_tsts_y;
extern short            t_l;                                   // variables utilisees lors du chargement d'un fichier dans les editeurs
extern short            t_s;                                   // variables utilisees lors de la sauvegarde d'un fichier dans les editeurs
extern char             str_file[50];                          // declaration d'une chaine pour le chargement de fichiers.
extern char             str_tmp[40];                           // Chaine temporaire utilis� pour le chargement de fichiers.
extern int              handle;                                // Utilise pour le chargement de fichiers.
extern short            ce_vais_act;                           // Variable utilis� pour indiquer le vaisseau actuellement s�ectionn�dans l'�iteur de courbe.
extern short            curv_number;                           // variable indiquant le numero de courbe actuelle.

//..............................................................................

//-----------------------------------------------------------------------------
// runtime
//-----------------------------------------------------------------------------
void courbe_execution()
{
  static int              i, k;

  if(courbe.activity)
  {
    // traite courbe par courbe
    for(i = 0; i < courbe.nbr_tot_courbes; i++)
    {
      // test si on fait apparaitre un nouveau vaisseau sur la courbe
      courbe.nbr_vbl_app_act[i]++;
      if(courbe.nbr_vaisseaux_act[i] < courbe.nbr_vaisseaux_tot[i]
         && courbe.nbr_vbl_app_act[i] > courbe.nbr_vbl_apparition[i])
      {                                                        // On rajoute un ennemi dans la liste.
        new_ve = New_Element_Ve();                             // Retourne l'indice dans le tableau 've' d'un nouvel ��ent de la liste.
        // Remplissage de la structure sprite appartenant au vaisseau ennemi.
        ve[new_ve].spr.pow_of_dest = (short)(1 + courbe.num_vaisseau[i][courbe.nbr_vaisseaux_act[i]]);  // Indique la puissance de destruction du sprite.
        ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 1) + ve[new_ve].spr.pow_of_dest);       // Indique l'�at de domages du sprite.
        ve[new_ve].spr.nbr_img = (short)32;                    // Indique le nombre d'images composant le sprite.
        ve[new_ve].spr.img_act = initial_curve[courbe.num_courbe[i]].angle[0];  // Indique l'image actuelle.
        // Donne l'adresse des images du sprite.
        for(k = 0; k < ve[new_ve].spr.nbr_img; k++)
        {
          ve[new_ve].spr.img[k] =
            (image *) & enemi[courbe.
                              num_vaisseau[i][courbe.
                                              nbr_vaisseaux_act[i]]][k];
        }
        ve[new_ve].cad_tir_initiale = courbe.freq_tir[i][courbe.nbr_vaisseaux_act[i]];  // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer �nouveau.
        ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;      // Initialise la variable utilis� pour compter le temps qu'il faut au vaisseau avant de pouvoir tirer �nouveau.
        ve[new_ve].deplacement = 0;                            // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.

        ve[new_ve].spr.coor_x =
          (float)(initial_curve[courbe.num_courbe[i]].pos_x + 128 - 32);
        ve[new_ve].spr.coor_y =
          (float)(initial_curve[courbe.num_courbe[i]].pos_y + 128 - 32);
        ve[new_ve].pos_vaiss[POS_CURVE] = 0;                   // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
        ve[new_ve].num_courbe = courbe.num_courbe[i];          // Donne le num�o de la courbe utilis� par le vaisseau ennemi.
        ve[new_ve].type = 0;                                   // Indique que l'on se trouve en pr�ence d'un ennemi de 16x16.
        courbe.nbr_vaisseaux_act[i]++;                         // Donne le nombre de vaisseaux mis dans la courbe actuellement.
        courbe.nbr_vbl_app_act[i] = 0;                         // On r�nitialise le compteur de VBLs pour l'apparition des vaisseaux.
      }
    }
  }
}

//------------------------------------------------------------------------------
// courbe : test si la phase "courbe" est terminee
//------------------------------------------------------------------------------
void courbe_testFin()
{
  int                     i;
  if(courbe.activity)                                          //phase "courbe" ?
  {
    all_nmi_killed = 1;
    for(i = 0; i < courbe.nbr_tot_courbes; i++)
    {
      if(courbe.nbr_vaisseaux_act[i] < courbe.nbr_vaisseaux_tot[i])
      {
        all_nmi_killed = 0;                                    //il reste encore des ennemis
      }
    }
    if(all_nmi_killed)                                         //plus d'ennemis a l'ecran ?
    {
      Active_Grid();                                           //oui, active la phase "grille"
      courbe.activity = 0;
      meteor_activity = 0;
      gardian_activity = 0;
      grille.activity = 1;                                     //1=phase grille
    }
  }
}

//------------------------------------------------------------------------------
// courbe : editeur
//------------------------------------------------------------------------------
#ifdef DEVELOPPEMENT
void courbe_editeur()
{
  static int              i, j;
  if(!courbe.nbr_tot_courbes)
  {
    courbe.nbr_tot_courbes = 1;
    for(i = 0; i < MAX_NBR_OF_SCR_CURV; i++)
      courbe.nbr_vbl_apparition[i] = 10;
  }

  // Dessine les lignes de la grille.
  for(i = 128; i <= (128 + LARG_GRILLE * 15); i += 16)
  {
    if(courbe.nbr_tot_courbes)
      line_v(ecran_ram + ((128 * LARG_ECR_RAM_TOT) + i), 512,
             (short)(16 * courbe.nbr_tot_courbes), coulor[GRIS]);
  }
  for(i = 128; i <= (128 + courbe.nbr_tot_courbes * 16); i += 16)
  {
    line_h(ecran_ram + ((i * LARG_ECR_RAM_TOT) + 128), 256, coulor[GRIS]);
  }

  // On change les coordon�s de la souris pour l'�iteur.
  mouse_x -= 128;
  mouse_y -= 128;

  // Test si l'utilisateur est en train de modifier la fr�uence du tir, le num�o de courbe ou la vitesse d'apparition.
  if(!touch[62] && !touch[42] && !touch[81] && !touch[80] && !touch[79] && !touch[38])  // Si les touches "f" et "t" sont s�ectionn�s.
  {                                                            // Donne les coordon�s x et y du carr�actuellement s�ectionn�dans la grille.
    ge_act_pos_x = (short)(mouse_x >> 4);
    ge_act_pos_y = (short)(mouse_y >> 4);
  }

  // On change les coordon�s de la souris pour l'�iteur.
  mouse_x += 128;
  mouse_y += 128;

  // Clipping des coordonn�s du carr�actuellement s�ectionn�dans la grille.
  if(ge_act_pos_x < 0)
    ge_act_pos_x = 0;
  if(ge_act_pos_x > (LARG_GRILLE - 1))
    ge_act_pos_x = LARG_GRILLE - 1;
  if(ge_act_pos_y < 0)
    ge_act_pos_y = 0;
  if(ge_act_pos_y > (courbe.nbr_tot_courbes - 1))
    ge_act_pos_y = (short)(courbe.nbr_tot_courbes - 1);
  // Conversion et affichage du num�o du niveau courbe actuel.
  ltoa((long)curv_number, chaine, 10);
  textxy("curv_number :", 128, 194 + 128 - 32, coulor[GRIS_CLAIR], 0,
         ecran_ram, LARG_ECR_RAM_TOT);
  textxy("c    n       ", 128, 194 + 128 - 32, coulor[ROUGE], -1, ecran_ram,
         LARG_ECR_RAM_TOT);
  textxy(chaine, 86 + 128 - 32, 194 + 128 - 32, coulor[BLANC], 0, ecran_ram,
         LARG_ECR_RAM_TOT);
  // Affichage du vaisseau qui sera affich�dans les cases �chaque clic souris.
  textxy("ce_vais_act :", 128, 201 + 128 - 32, coulor[GRIS_CLAIR], 0,
         ecran_ram, LARG_ECR_RAM_TOT);
  textxy(" e v         ", 128, 201 + 128 - 32, coulor[ROUGE], -1, ecran_ram,
         LARG_ECR_RAM_TOT);
  itoa((int)ce_vais_act, chaine, 10);
  textxy(chaine, 86 + 128 - 32, 201 + 128 - 32, coulor[BLANC], 0, ecran_ram,
         LARG_ECR_RAM_TOT);
  // Affichage de la frequence de tir du vaisseau.
  ltoa((long)courbe.freq_tir[ge_act_pos_y][ge_act_pos_x], chaine, 10);
  textxy("ce_freq_tir :", 128, 208 + 128 - 32, coulor[GRIS_CLAIR], 0,
         ecran_ram, LARG_ECR_RAM_TOT);
  textxy("   f    t    ", 128, 208 + 128 - 32, coulor[ROUGE], -1, ecran_ram,
         LARG_ECR_RAM_TOT);
  textxy(chaine, 86 + 128 - 32, 208 + 128 - 32, coulor[BLANC], 0, ecran_ram,
         LARG_ECR_RAM_TOT);
  // Affichage de la position actuelle du carr�s�ectionn�
  textxy("ge_act_pos :", 100 + 128 - 32, 194 + 128 - 32, coulor[GRIS_CLAIR],
         0, ecran_ram, LARG_ECR_RAM_TOT);
  ltoa((long)ge_act_pos_x, chaine, 10);
  textxy(chaine, 154 + 128 - 32, 194 + 128 - 32, coulor[BLANC], 0, ecran_ram,
         LARG_ECR_RAM_TOT);
  ltoa((long)ge_act_pos_y, chaine, 10);
  textxy(chaine, 162 + 128 - 32, 194 + 128 - 32, coulor[BLANC], 0, ecran_ram,
         LARG_ECR_RAM_TOT);
  // Affichage du nombre total de courbes utilis�s dans le niveau.
  textxy("nbr_tot_courbes :", 100 + 128 - 32, 201 + 128 - 32,
         coulor[GRIS_CLAIR], 0, ecran_ram, LARG_ECR_RAM_TOT);
  textxy("    t   c        ", 100 + 128 - 32, 201 + 128 - 32, coulor[ROUGE],
         -1, ecran_ram, LARG_ECR_RAM_TOT);
  ltoa((long)(courbe.nbr_tot_courbes), chaine, 10);
  textxy(chaine, 170 + 128 - 32, 201 + 128 - 32, coulor[BLANC], 0, ecran_ram,
         LARG_ECR_RAM_TOT);
  // Affichage du num�o de courbe actuellement s�ectionn�.
  textxy("num_courbe :", 100 + 128 - 32, 208 + 128 - 32, coulor[GRIS_CLAIR],
         0, ecran_ram, LARG_ECR_RAM_TOT);
  textxy("n       b   ", 100 + 128 - 32, 208 + 128 - 32, coulor[ROUGE], -1,
         ecran_ram, LARG_ECR_RAM_TOT);
  ltoa((long)(courbe.num_courbe[ge_act_pos_y]), chaine, 10);
  textxy(chaine, 154 + 128 - 32, 208 + 128 - 32, coulor[BLANC], 0, ecran_ram,
         LARG_ECR_RAM_TOT);


  // Affichage de la position actuelle du carr�s�ectionn�
  textxy("nbr_vaisseaux_tot :", 178 + 128 - 32, 194 + 128 - 32,
         coulor[GRIS_CLAIR], 0, ecran_ram, LARG_ECR_RAM_TOT);
  ltoa((long)courbe.nbr_vaisseaux_tot[ge_act_pos_y], chaine, 10);
  textxy(chaine, 258 + 128 - 32, 194 + 128 - 32, coulor[BLANC], 0, ecran_ram,
         LARG_ECR_RAM_TOT);

  // Affichage du nombre de VBLs avant chaque apparition de vaisseaux.
  textxy("nbr_vbl_apparition :", 178 + 128 - 32, 201 + 128 - 32,
         coulor[GRIS_CLAIR], 0, ecran_ram, LARG_ECR_RAM_TOT);
  textxy("    v   a           ", 178 + 128 - 32, 201 + 128 - 32,
         coulor[ROUGE], -1, ecran_ram, LARG_ECR_RAM_TOT);
  ltoa((long)(courbe.nbr_vbl_apparition[ge_act_pos_y]), chaine, 10);
  textxy(chaine, 262 + 128 - 32, 201 + 128 - 32, coulor[BLANC], 0, ecran_ram,
         LARG_ECR_RAM_TOT);

  // Affichage du carr�actuellement s�ectionn�dans la grille.
  line_v(ecran_ram +
         ((((ge_act_pos_y << 4) + 128) * LARG_ECR_RAM_TOT) +
          ((ge_act_pos_x << 4) + 128)), LARG_ECR_RAM_TOT, 16, coulor[ROUGE]);
  line_v(ecran_ram +
         ((((ge_act_pos_y << 4) + 128) * LARG_ECR_RAM_TOT) +
          ((ge_act_pos_x << 4) + 128 + 16)), LARG_ECR_RAM_TOT, 16,
         coulor[ROUGE]);
  line_h(ecran_ram +
         ((((ge_act_pos_y << 4) + 128) * LARG_ECR_RAM_TOT) +
          ((ge_act_pos_x << 4) + 128)), 16, coulor[ROUGE]);
  line_h(ecran_ram +
         ((((ge_act_pos_y << 4) + 128 + 16) * LARG_ECR_RAM_TOT) +
          ((ge_act_pos_x << 4) + 128)), 16, coulor[ROUGE]);


  // Recherche du nombre total de vaisseaux de la courbe.
  Find_Tot_Nbr_Vais_Of_Curve();

  // Affichage des vaisseaux de toutes les courbes.
  for(i = 0; i < courbe.nbr_tot_courbes; i++)
  {
    for(j = 0; j < courbe.nbr_vaisseaux_tot[i]; j++)
    {                                                          // Affichage d'un vaisseau.
      putsprite3(enemi[courbe.num_vaisseau[i][j]][15].img,
                 ecran_ram + ((128 + i * 16) * LARG_ECR_RAM_TOT) + (128 +
                                                                    16 * j),
                 enemi[courbe.num_vaisseau[i][j]][15].compress,
                 (short)(enemi[courbe.num_vaisseau[i][j]][15].
                         nbr_data_comp >> 2),
                 "enemi[courbe.num_vaisseau[i][j]][15].img");
    }
  }

  // Affichage de la courbe actuelle.
  tmp_tsts_x = initial_curve[courbe.num_courbe[ge_act_pos_y]].pos_x + 128 - 32; // Donne la valeur x du d�ut de la courbe.
  tmp_tsts_y = initial_curve[courbe.num_courbe[ge_act_pos_y]].pos_y + 128 - 32; // Donne la valeur y du d�ut de la courbe.
  // Pour tous les points de la courbe.
  for(i = 0; i < initial_curve[courbe.num_courbe[ge_act_pos_y]].nbr_pnt_curve;
      i++)
  {                                                            // Test si le pixel est dans la m�oire d'affichage.
    if(tmp_tsts_x >= 0 && tmp_tsts_x < LARG_ECR_RAM_TOT && tmp_tsts_y >= 0
       && tmp_tsts_y < HAUT_ECR_RAM_TOT)
      putpixel(ecran_ram, tmp_tsts_x, tmp_tsts_y, coulor[VERT]);        // Si oui, affichage du point actuel de la courbe.
    // Donne les coordon�s du point suivant.
    tmp_tsts_x += initial_curve[courbe.num_courbe[ge_act_pos_y]].delta_x[i];
    tmp_tsts_y += initial_curve[courbe.num_courbe[ge_act_pos_y]].delta_y[i];
  }


  // Test si l'utilisateur veux sauver un fichier courbe.
  if(touch[60] && touch[38] && !t_s)                           // Touches "s" et "a" s�ectionn�s ?
  {                                                            // Convertion du num�o de COURBE en nom de fichier.
    strcpy(str_file, "Graphics\\CURVLEV.");
    itoa((int)curv_number, str_tmp, 10);
    strcat(str_file, str_tmp);
    // Ecriture du fichier grille.
    _fmode = O_BINARY;                                         // Demande une �riture de fichier binaire.
    // Cr�tion et �riture du fichier grille.
    if((handle = _lcreat(str_file, NULL)) != -1)
    {
      _lwrite(handle, (const char *)&courbe.nbr_tot_courbes, sizeof(short));    // Indique le nombre total de courbes utilis�s dans ce niveau.
      _lwrite(handle, (const char *)courbe.num_courbe, sizeof(short) * MAX_NBR_OF_SCR_CURV);    // Tableau des diff�ents num�os de courbes utilis�s.
      _lwrite(handle, (const char *)courbe.nbr_vaisseaux_tot, sizeof(short) * MAX_NBR_OF_SCR_CURV);     // Donne le nombre de vaisseaux que la courbe doit avoir et ce pour chaque courbe.
      _lwrite(handle, (const char *)courbe.nbr_vbl_apparition, sizeof(short) * MAX_NBR_OF_SCR_CURV);    // Donne le nombre de VBLs qu'il faux pour l'apparition de chaque vaisseaux dans chaque courbe.
      _lwrite(handle, (const char *)courbe.num_vaisseau, sizeof(short) * MAX_NBR_OF_SCR_CURV * NUM_VAI_BY_CURVE);       // Num�os des vaisseaus utilis� dans chaque courbe.
      _lwrite(handle, (const char *)courbe.freq_tir, sizeof(short) * MAX_NBR_OF_SCR_CURV * NUM_VAI_BY_CURVE);   // Fr�uence de tir de tous les vaisseaux de chaque courbe.
      _lclose(handle);
    }
    else
      MessageBox(hWndMain, "Cannot create curve file !", "ERROR", MB_OK);
  }
  t_s = touch[60];

  // Test si l'utilisateur veux charger un fichier niveau courbe.
  if(touch[67] && touch[46] && !t_l)                           // Touches "l" et "o" s�ectionn�s ?
  {                                                            // Chargement d'un niveau courbe.
    Load_Curve_Level((int)curv_number);
    // Activation du fichier courbe charg�pr��ement.
    Active_Curve_Level();
  }
  t_l = touch[67];

  if(mouse_b)
  {                                                            // Test si l'utilisateur veux changer le num�o de la courbe.
    if(touch[78] && touch[81])                                 // Touches "c" et "n" s�ectionn�s ?
    {
      curv_number = (short)(mouse_y - 128 + mouse_x - 128);
      // V�ifi que la variable ne pointe pas sur une grille inexistante.
      if(curv_number < 0)
        curv_number = 0;
      goto fin_curv_editor;
    }
    // Test si l'utilisateur veux s�ectionner un vaisseau ennemi (touche "v" et "e").
    if(touch[79] && touch[40])
    {
      ce_vais_act = (short)(mouse_x - 128);
      // V�ifi que la variable ne pointe pas sur un vaisseau qui n'existe pas.
      if(ce_vais_act > (MAX_TYP_OF_ENEMI - 1))
        ce_vais_act = (MAX_TYP_OF_ENEMI - 1);
      if(ce_vais_act < 0)
        ce_vais_act = 0;
      goto fin_curv_editor;
    }
    // Test si l'utilisateur veux modifier la fr�uence du tir.
    if(touch[62] && touch[42])                                 // Si les touches "f" et "t" sont s�ectionn�s.
    {
      courbe.freq_tir[ge_act_pos_y][ge_act_pos_x] =
        (short)((mouse_y - 128) * 4 + mouse_x - 128);
      // V�ifi que la variable ne pointe pas sur un vaisseau qui n'existe pas.
      if(courbe.freq_tir[ge_act_pos_y][ge_act_pos_x] < 0)
        courbe.freq_tir[ge_act_pos_y][ge_act_pos_x] = 0;
      goto fin_curv_editor;
    }
    // Test si l'utilisateur veux modifier le nombre total de courbes utilis�s dans le niveau.
    if(touch[78] && touch[42])                                 // Si les touches "t" et "c" sont s�ectionn�s.
    {
      courbe.nbr_tot_courbes = (short)(mouse_y - 128 + mouse_x - 128);
      // V�ifi que la variable ne d�asse pas le nombre maximum de courbes autoris�s.
      if(courbe.nbr_tot_courbes < 1)
        courbe.nbr_tot_courbes = 1;
      if(courbe.nbr_tot_courbes > MAX_NBR_OF_SCR_CURV)
        courbe.nbr_tot_courbes = MAX_NBR_OF_SCR_CURV;
      goto fin_curv_editor;
    }
    // Test si l'utilisateur veux modifier la courbe actuellement s�ectionn�.
    if(touch[81] && touch[80])                                 // Si les touches "n" et "b" sont s�ectionn�s.
    {
      courbe.num_courbe[ge_act_pos_y] =
        (short)(mouse_y - 128 + mouse_x - 128);
      // V�ifi que la variable ne d�asse pas le nombre maximum de courbes autoris�s.
      if(courbe.num_courbe[ge_act_pos_y] < 0)
        courbe.num_courbe[ge_act_pos_y] = 0;
      if(courbe.num_courbe[ge_act_pos_y] > (NBR_CURVE_MAX - 1))
        courbe.num_courbe[ge_act_pos_y] = NBR_CURVE_MAX - 1;
      goto fin_curv_editor;
    }
    // Test si l'utilisateur veux modifier la vitesse d'apparition des vaisseaux.
    if(touch[79] && touch[38])                                 // Si les touches "v" et "a" sont s�ectionn�s.
    {
      courbe.nbr_vbl_apparition[ge_act_pos_y] =
        (short)((mouse_y - 128) * 4 + mouse_x - 128);
      // V�ifi que la variable ne d�asse pas les limites autoris�s.
      if(courbe.nbr_vbl_apparition[ge_act_pos_y] < 10)
        courbe.nbr_vbl_apparition[ge_act_pos_y] = 10;
      goto fin_curv_editor;
    }
    // Test si le btn droit de la souris est s�ectionn�
    if(mouse_b == 2)                                           // Si oui, on efface la case de la grille.
    {
      courbe.num_vaisseau[ge_act_pos_y][ge_act_pos_x] = -1;
      courbe.freq_tir[ge_act_pos_y][ge_act_pos_x] = 0;
    }
    else                                                       // Sinon, on met le vaisseau actuel dans la case de la grille actuellement s�ectionn�.
    {
      courbe.num_vaisseau[ge_act_pos_y][ge_act_pos_x] = ce_vais_act;
      courbe.freq_tir[ge_act_pos_y][ge_act_pos_x] = 210;
    }
  }
fin_curv_editor:;
}
#endif

//------------------------------------------------------------------------------
// charge toutes les courbes / load all curves
// sortie : 1 = pas d'erreur
//------------------------------------------------------------------------------
static char _pNomFichier[] = "graphics/courbe.???";
int charge_courbes()
{

  int i, j;
  char *_pFichier;
  signed char *_p8;
  short *_p16;
  for(i = 0; i < NBR_CURVE_MAX; i++)
  {
    entier2ASCII(i, _pNomFichier + 16, 2);
    _pFichier = chargeFichier(_pNomFichier);
    if(!_pFichier)
      return 0;
    _p16 = (short *)_pFichier;                                 //acces sur 16 bits
    initial_curve[i].nbr_pnt_curve = litMot16bits(_p16++);     //nombre de points de la courbe
    initial_curve[i].pos_x = litMot16bits(_p16++);             //abscisse de depart
    initial_curve[i].pos_y = litMot16bits(_p16++);             //ordonnee de depart
    //printf("%i %i %i\n",i , initial_curve[i].pos_x, initial_curve[i].pos_x);
    _p8 = (signed char *)_p16;                                        //acces 8 bits
    for(j = 0; j < initial_curve[i].nbr_pnt_curve; j++)
    {
      initial_curve[i].delta_x[j] = *(_p8++);
      initial_curve[i].delta_y[j] = *(_p8++);
      initial_curve[i].angle[j] = *(_p8++);
      //printf("%i %i %i %i\n", j, initial_curve[i].delta_x[j], initial_curve[i].delta_y[j], initial_curve[i].angle[j]);
    }
    _p16 = (short *)_p8;                                       //acces sur 16 bits
    libereMemoire(_pFichier);
  }
  return 1;
}

//------------------------------------------------------------------------------
// chargement d'un fichier "courbe" / load a file "curve"
// input => numero_level : 0 to 41
//------------------------------------------------------------------------------
static char pNomFichierCurveLevel[] = "graphics/curvlev.??";
int Load_Curve_Level(int numero_level)
{
  short *_p16;
  short *_pMem;
  unsigned int i;
  if(numero_level > NBR_LEVEL_MAX || numero_level < 0)
    numero_level = 0;
  entier2ASCII(numero_level, pNomFichierCurveLevel + 17, 1);
  char *_pFichier = chargeFichier(pNomFichierCurveLevel);     //load file in memory
  if(!_pFichier)
    return 0;
  _p16 = (short *)_pFichier;                                   //access sur 16 bits
  courbe.nbr_tot_courbes = litMot16bits(_p16++);               //nombre total de courbes utilisees dans ce niveau
  for(i = 0; i < MAX_NBR_OF_SCR_CURV; i++)
    courbe.num_courbe[i] = litMot16bits(_p16++);               //tableau des differents num�os de courbes utilisees
  for(i = 0; i < MAX_NBR_OF_SCR_CURV; i++)
    courbe.nbr_vaisseaux_tot[i] = litMot16bits(_p16++);        //nombre de vaisseaux sur chaque courbes
  for(i = 0; i < MAX_NBR_OF_SCR_CURV; i++)
    courbe.nbr_vbl_apparition[i] = litMot16bits(_p16++);       //temps d'appparition des vaisseaux sur les courbes
  //-
  _pMem = &courbe.num_vaisseau[0][0];
  for(i = 0; i < MAX_NBR_OF_SCR_CURV * NUM_VAI_BY_CURVE; i++)
    *(_pMem++) = litMot16bits(_p16++);                         //numeros des vaisseaus utilises sur les courbes
  //-
  _pMem = &courbe.freq_tir[0][0];
  for(i = 0; i < MAX_NBR_OF_SCR_CURV * NUM_VAI_BY_CURVE; i++)
    *(_pMem++) = litMot16bits(_p16++);                         //frequence de tir de tous les vaisseaux de chaque courbe
  libereMemoire(_pFichier);                                    //(!) bug found by Sam Hocevar
  return 1;
}

//------------------------------------------------------------------------------
//initialize phase "curve"
//------------------------------------------------------------------------------
void Active_Curve_Level()
{
  int                     i;

  Find_Tot_Nbr_Vais_Of_Curve();                                //search the total number of ships
  for(i = 0; i < courbe.nbr_tot_courbes; i++)
  {
    courbe.nbr_vaisseaux_act[i] = 0;                           // Donne le nombre de vaisseaux mis dans la courbe actuellement.
    courbe.nbr_vbl_app_act[i] = 0;                             // Donne le nombre de VBLs compt�s avant l'apparition de chaque vaisseaux dans la courbe.

#ifdef DEVELOPPEMENT
    if(curv_editor)
    {
      int                     j;

      // On rajoute un ennemi dans la liste.
      new_ve = New_Element_Ve();                               // Retourne l'indice dans le tableau 've' d'un nouvel ��ent de la liste.
      // Mise en place des ennemis par remplissage de la structure sprite appartenant au vaisseau ennemi.
      ve[new_ve].spr.pow_of_dest = (short)(5 + 1 + courbe.num_vaisseau[i][courbe.nbr_vaisseaux_act[i]]);        // Indique la puissance de destruction du sprite.
      ve[new_ve].spr.etat_dommages = (short)((vj[J1].type_vaisseau << 1) + ve[new_ve].spr.pow_of_dest - 5);     // Indique l'�at de domages du sprite.
      ve[new_ve].type = 0;                                     // Indique que l'on se trouve en pr�ence d'un ennemi de 16x16.
      ve[new_ve].spr.nbr_img = 32;                             // Indique le nombre d'images composant le sprite.
      ve[new_ve].spr.img_act = initial_curve[courbe.num_courbe[i]].angle[0];    // Indique l'image actuelle.
      // Donne l'adresse des images du sprite.
      for(j = 0; j < ve[new_ve].spr.nbr_img; j++)
      {
        ve[new_ve].spr.img[j] =
          (image *) & enemi[courbe.
                            num_vaisseau[i][courbe.nbr_vaisseaux_act[i]]][j];
      }
      ve[new_ve].cad_tir_initiale = courbe.freq_tir[i][courbe.nbr_vaisseaux_act[i]];    // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer �nouveau.
      ve[new_ve].cad_tir = ve[new_ve].cad_tir_initiale;        // Corespond au temps qu'il faut au vaisseau avant de pouvoir tirer �nouveau.
      ve[new_ve].deplacement = 0;                              // 0 indique une courbe, 1 indique une grille, 2 indique un special, 3 indique un gardien.
      ve[new_ve].spr.coor_x = (float)(initial_curve[courbe.num_courbe[i]].pos_x + 128 - 32);    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].spr.coor_y = (float)(initial_curve[courbe.num_courbe[i]].pos_y + 128 - 32);    // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].pos_vaiss[POS_CURVE] = 0;                     // Donne l'indice de progression du vaisseau soit sur la courbe, soit sur la grille.
      ve[new_ve].num_courbe = courbe.num_courbe[i];            // Donne le num�o de la courbe utilis� par le vaisseau ennemi.
      courbe.nbr_vaisseaux_act[i]++;                           // Donne le nombre de vaisseaux mis dans la courbe actuellement.
    }
#endif

  }
}

//------------------------------------------------------------------------------
// search the total number of ships in the phase "curve"
//------------------------------------------------------------------------------
void Find_Tot_Nbr_Vais_Of_Curve()
{
  int                     i, j;

Find_Nbr_Vais_By_Crb:;
  for(i = 0; i < courbe.nbr_tot_courbes; i++)
  {
    courbe.nbr_vaisseaux_tot[i] = 0;
    for(j = 0; j < NUM_VAI_BY_CURVE; j++)
    {
      if(courbe.num_vaisseau[i][j] != -1)                      //ship exist?
      {
        courbe.nbr_vaisseaux_tot[i]++;                         //increase the numebr of ships
        if(j && courbe.num_vaisseau[i][j - 1] == -1)           //the curve's empty ?
        {
          courbe.num_vaisseau[i][j - 1] = courbe.num_vaisseau[i][j];
          courbe.freq_tir[i][j - 1] = courbe.freq_tir[i][j];
          courbe.num_vaisseau[i][j] = -1;
          courbe.freq_tir[i][j] = 0;
          goto Find_Nbr_Vais_By_Crb;
        }
      }
    }
  }
}

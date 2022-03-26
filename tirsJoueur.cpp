//*****************************************************************************
// copyright (c) 1998-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "tirsJoueurs.cpp"
// created      : ?
// updates      : 2005-01-08
// id		: $Id: tirsJoueur.cpp,v 1.1.1.1 2005/01/25 05:55:27 gurumeditation Exp $
//-----------------------------------------------------------------------------
// description  : ship's fires run-time 
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
extern vaisseau         vj[NBR_MAX_J];
extern short            touch[500];                            //tableau des flags de toutes les touches clavier
extern short            nbr_nmis;                              //nombre d'enemis actuellement geres.
extern int              tmp_apparition_vj;                     //vaisseau joueur : compteur temps apparition 
extern bool             disparition_vj;                        //vaisseau joueur : 1=disparition 
extern short            mouse_b;
extern unsigned int     iBoutonFeu;                            //1=bouton de la manette de jeu enfoncee
extern short            nbr_tirs;                              //nombre de tirs actuellement geres

//------------------------------------------------------------------------------
void tirsJoueur()
{
  if(tmp_apparition_vj <= 0 && !disparition_vj)                //phase d'apparition/disparition du vaisseau ?
  {
    if((mouse_b == 1 || touch[90] || iBoutonFeu)
       && nbr_tirs < (NBR_TIRS_MAX - 1) && vj[J1].cad_tir <= 0)
    {
      vj[J1].cad_tir = 50 - (vj[J1].type_vaisseau * 5 + 5);    // nombre de VBLs necessaires avant que le vaisseau puisse a nouveau tirer
      vj[J1].cad_tir_spec = vj[J1].cad_tir >> 1;               // nombre de VBLs necessaires avant que le vaisseau puisse a nouveau tirer
#ifdef USE_SDLMIXER
      seal_joue(TIR2);                                         //joue un son avec seal
#endif
      // Test suivant le type de vaisseau utilisé par le joueur.
      switch (vj[J1].type_vaisseau)
      {                                                        // Cas du premier vaisseau du joueur 1.
        case V1J:
        {                                                      // Test suivant le type de tir avant du joueur 1.
          switch (vj[J1].power_avant)
          {                                                    // Cas du tir avant de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 1.
              V1_Tir_Normal_Avant_P1();
            }
              break;
              // Cas du tir avant de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 2.
              V1_Tir_Normal_Avant_P2();
            }
              break;
              // Cas du tir avant de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 3.
              V1_Tir_Normal_Avant_P3();
            }
              break;
              // Cas du tir avant de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 4.
              V1_Tir_Normal_Avant_P4();
            }
              break;
              // Cas du tir avant de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'avant de puissance 5.
              V1_Tir_Normal_Avant_P5();
            }
              break;
          }
          // Test suivant le type de tir arrière du joueur 1.
          switch (vj[J1].power_arriere)
          {                                                    // Cas du tir arrière de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 1.
              V1_Tir_Normal_Arriere_P1();
            }
              break;
              // Cas du tir arrière de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 2.
              V1_Tir_Normal_Arriere_P2();
            }
              break;
              // Cas du tir arrière de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 3.
              V1_Tir_Normal_Arriere_P3();
            }
              break;
              // Cas du tir arrière de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 4.
              V1_Tir_Normal_Arriere_P4();
            }
              break;
              // Cas du tir arrière de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à l'arrière de puissance 5.
              V1_Tir_Normal_Arriere_P5();
            }
              break;
          }
          // Test suivant le type de tir à droite du joueur 1.
          switch (vj[J1].power_droite)
          {                                                    // Cas du tir à droite de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 1.
              V1_Tir_Normal_Droite_P1();
            }
              break;
              // Cas du tir à droite de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 2.
              V1_Tir_Normal_Droite_P2();
            }
              break;
              // Cas du tir à droite de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 3.
              V1_Tir_Normal_Droite_P3();
            }
              break;
              // Cas du tir à droite de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 4.
              V1_Tir_Normal_Droite_P4();
            }
              break;
              // Cas du tir à droite de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à droite de puissance 5.
              V1_Tir_Normal_Droite_P5();
            }
              break;
          }
          // Test suivant le type de tir à gauche du joueur 1.
          switch (vj[J1].power_gauche)
          {                                                    // Cas du tir à gauche de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 1.
              V1_Tir_Normal_Gauche_P1();
            }
              break;
              // Cas du tir à gauche de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 2.
              V1_Tir_Normal_Gauche_P2();
            }
              break;
              // Cas du tir à gauche de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 3.
              V1_Tir_Normal_Gauche_P3();
            }
              break;
              // Cas du tir à gauche de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 4.
              V1_Tir_Normal_Gauche_P4();
            }
              break;
              // Cas du tir à gauche de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir normal à gauche de puissance 5.
              V1_Tir_Normal_Gauche_P5();
            }
              break;
          }
        }
          break;
        case V2J:
        {                                                      // Test suivant le type de tir avant du joueur 1.
          switch (vj[J1].power_avant)
          {                                                    // Cas du tir avant de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 1.
              V2_Tir_Normal_Avant_P1();
            }
              break;
              // Cas du tir avant de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 2.
              V2_Tir_Normal_Avant_P2(2);                       // L'argument défini la puissance de chaque tirs.
            }
              break;
              // Cas du tir avant de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 3.
              V2_Tir_Normal_Avant_P3();
            }
              break;
              // Cas du tir avant de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 4.
              V2_Tir_Normal_Avant_P4(2);                       // L'argument défini la puissance des deux premiers tirs.
            }
              break;
              // Cas du tir avant de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'avant de puissance 5.
              V2_Tir_Normal_Avant_P5();
            }
              break;
          }
          // Test suivant le type de tir arrière du joueur 1.
          switch (vj[J1].power_arriere)
          {                                                    // Cas du tir arrière de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 1.
              V2_Tir_Normal_Arriere_P1();
            }
              break;
              // Cas du tir arrière de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 2.
              V2_Tir_Normal_Arriere_P2(2);
            }
              break;
              // Cas du tir arrière de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 3.
              V2_Tir_Normal_Arriere_P3();
            }
              break;
              // Cas du tir arrière de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 4.
              V2_Tir_Normal_Arriere_P4(2);
            }
              break;
              // Cas du tir arrière de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à l'arrière de puissance 5.
              V2_Tir_Normal_Arriere_P5();
            }
              break;
          }
          // Test suivant le type de tir à droite du joueur 1.
          switch (vj[J1].power_droite)
          {                                                    // Cas du tir à droite de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 1.
              V2_Tir_Normal_Droite_P1();
            }
              break;
              // Cas du tir à droite de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 2.
              V2_Tir_Normal_Droite_P2(2);
            }
              break;
              // Cas du tir à droite de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 3.
              V2_Tir_Normal_Droite_P3();
            }
              break;
              // Cas du tir à droite de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 4.
              V2_Tir_Normal_Droite_P4(2);
            }
              break;
              // Cas du tir à droite de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à droite de puissance 5.
              V2_Tir_Normal_Droite_P5();
            }
              break;
          }
          // Test suivant le type de tir à gauche du joueur 1.
          switch (vj[J1].power_gauche)
          {                                                    // Cas du tir à gauche de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 1.
              V2_Tir_Normal_Gauche_P1();
            }
              break;
              // Cas du tir à gauche de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 2.
              V2_Tir_Normal_Gauche_P2(2);
            }
              break;
              // Cas du tir à gauche de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 3.
              V2_Tir_Normal_Gauche_P3();
            }
              break;
              // Cas du tir à gauche de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 4.
              V2_Tir_Normal_Gauche_P4(2);
            }
              break;
              // Cas du tir à gauche de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir normal à gauche de puissance 5.
              V2_Tir_Normal_Gauche_P5();
            }
              break;
          }
        }
          break;
        case V3J:
        {                                                      // Test suivant le type de tir avant du joueur 1.
          switch (vj[J1].power_avant)
          {                                                    // Cas du tir avant de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 1.
              V3_Tir_Normal_Avant_P1(5);
            }
              break;
              // Cas du tir avant de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 2.
              V3_Tir_Normal_Avant_P2(3);                       // L'argument défini la puissance de chaque tirs.
            }
              break;
              // Cas du tir avant de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 3.
              V3_Tir_Normal_Avant_P3();
            }
              break;
              // Cas du tir avant de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 4.
              V3_Tir_Normal_Avant_P4(2);                       // L'argument défini la puissance des deux premiers tirs.
            }
              break;
              // Cas du tir avant de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'avant de puissance 5.
              V3_Tir_Normal_Avant_P5();
            }
              break;
          }
          // Test suivant le type de tir arrière du joueur 1.
          switch (vj[J1].power_arriere)
          {                                                    // Cas du tir arrière de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 1.
              V3_Tir_Normal_Arriere_P1(5);
            }
              break;
              // Cas du tir arrière de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 2.
              V3_Tir_Normal_Arriere_P2(3);
            }
              break;
              // Cas du tir arrière de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 3.
              V3_Tir_Normal_Arriere_P3();
            }
              break;
              // Cas du tir arrière de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 4.
              V3_Tir_Normal_Arriere_P4(2);
            }
              break;
              // Cas du tir arrière de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à l'arrière de puissance 5.
              V3_Tir_Normal_Arriere_P5();
            }
              break;
          }
          // Test suivant le type de tir à droite du joueur 1.
          switch (vj[J1].power_droite)
          {                                                    // Cas du tir à droite de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 1.
              V3_Tir_Normal_Droite_P1(5);
            }
              break;
              // Cas du tir à droite de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 2.
              V3_Tir_Normal_Droite_P2(3);
            }
              break;
              // Cas du tir à droite de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 3.
              V3_Tir_Normal_Droite_P3();
            }
              break;
              // Cas du tir à droite de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 4.
              V3_Tir_Normal_Droite_P4(2);
            }
              break;
              // Cas du tir à droite de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à droite de puissance 5.
              V3_Tir_Normal_Droite_P5();
            }
              break;
          }
          // Test suivant le type de tir à gauche du joueur 1.
          switch (vj[J1].power_gauche)
          {                                                    // Cas du tir à gauche de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 1.
              V3_Tir_Normal_Gauche_P1(5);
            }
              break;
              // Cas du tir à gauche de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 2.
              V3_Tir_Normal_Gauche_P2(3);
            }
              break;
              // Cas du tir à gauche de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 3.
              V3_Tir_Normal_Gauche_P3();
            }
              break;
              // Cas du tir à gauche de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 4.
              V3_Tir_Normal_Gauche_P4(2);
            }
              break;
              // Cas du tir à gauche de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir normal à gauche de puissance 5.
              V3_Tir_Normal_Gauche_P5();
            }
              break;
          }
        }
          break;
        case V4J:
        {                                                      // Test suivant le type de tir avant du joueur 1.
          switch (vj[J1].power_avant)
          {                                                    // Cas du tir avant de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 1.
              V4_Tir_Normal_Avant_P1(7);
            }
              break;
              // Cas du tir avant de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 2.
              V4_Tir_Normal_Avant_P2(4);                       // L'argument défini la puissance de chaque tirs.
            }
              break;
              // Cas du tir avant de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 3.
              V4_Tir_Normal_Avant_P3();
            }
              break;
              // Cas du tir avant de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 4.
              V4_Tir_Normal_Avant_P4(3);                       // L'argument défini la puissance des deux premiers tirs.
            }
              break;
              // Cas du tir avant de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'avant de puissance 5.
              V4_Tir_Normal_Avant_P5();
            }
              break;
          }
          // Test suivant le type de tir arrière du joueur 1.
          switch (vj[J1].power_arriere)
          {                                                    // Cas du tir arrière de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 1.
              V4_Tir_Normal_Arriere_P1(7);
            }
              break;
              // Cas du tir arrière de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 2.
              V4_Tir_Normal_Arriere_P2(4);
            }
              break;
              // Cas du tir arrière de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 3.
              V4_Tir_Normal_Arriere_P3();
            }
              break;
              // Cas du tir arrière de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 4.
              V4_Tir_Normal_Arriere_P4(3);
            }
              break;
              // Cas du tir arrière de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à l'arrière de puissance 5.
              V4_Tir_Normal_Arriere_P5();
            }
              break;
          }
          // Test suivant le type de tir à droite du joueur 1.
          switch (vj[J1].power_droite)
          {                                                    // Cas du tir à droite de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 1.
              V4_Tir_Normal_Droite_P1(7);
            }
              break;
              // Cas du tir à droite de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 2.
              V4_Tir_Normal_Droite_P2(4);
            }
              break;
              // Cas du tir à droite de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 3.
              V4_Tir_Normal_Droite_P3();
            }
              break;
              // Cas du tir à droite de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 4.
              V4_Tir_Normal_Droite_P4(3);
            }
              break;
              // Cas du tir à droite de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à droite de puissance 5.
              V4_Tir_Normal_Droite_P5();
            }
              break;
          }
          // Test suivant le type de tir à gauche du joueur 1.
          switch (vj[J1].power_gauche)
          {                                                    // Cas du tir à gauche de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 1.
              V4_Tir_Normal_Gauche_P1(7);
            }
              break;
              // Cas du tir à gauche de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 2.
              V4_Tir_Normal_Gauche_P2(4);
            }
              break;
              // Cas du tir à gauche de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 3.
              V4_Tir_Normal_Gauche_P3();
            }
              break;
              // Cas du tir à gauche de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 4.
              V4_Tir_Normal_Gauche_P4(3);
            }
              break;
              // Cas du tir à gauche de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir normal à gauche de puissance 5.
              V4_Tir_Normal_Gauche_P5();
            }
              break;
          }
        }
          break;
        case V5J:
        {                                                      // Test suivant le type de tir avant du joueur 1.
          switch (vj[J1].power_avant)
          {                                                    // Cas du tir avant de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 1.
              V5_Tir_Normal_Avant_P1();
            }
              break;
              // Cas du tir avant de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 2.
              V5_Tir_Normal_Avant_P2(5);                       // L'argument défini la puissance de chaque tirs.
            }
              break;
              // Cas du tir avant de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 3.
              V5_Tir_Normal_Avant_P3();
            }
              break;
              // Cas du tir avant de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 4.
              V5_Tir_Normal_Avant_P4(4);                       // L'argument défini la puissance des deux premiers tirs.
            }
              break;
              // Cas du tir avant de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'avant de puissance 5.
              V5_Tir_Normal_Avant_P5();
            }
              break;
          }
          // Test suivant le type de tir arrière du joueur 1.
          switch (vj[J1].power_arriere)
          {                                                    // Cas du tir arrière de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 1.
              V5_Tir_Normal_Arriere_P1();
            }
              break;
              // Cas du tir arrière de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 2.
              V5_Tir_Normal_Arriere_P2(5);
            }
              break;
              // Cas du tir arrière de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 3.
              V5_Tir_Normal_Arriere_P3();
            }
              break;
              // Cas du tir arrière de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 4.
              V5_Tir_Normal_Arriere_P4(4);
            }
              break;
              // Cas du tir arrière de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à l'arrière de puissance 5.
              V5_Tir_Normal_Arriere_P5();
            }
              break;
          }
          // Test suivant le type de tir à droite du joueur 1.
          switch (vj[J1].power_droite)
          {                                                    // Cas du tir à droite de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 1.
              V5_Tir_Normal_Droite_P1();
            }
              break;
              // Cas du tir à droite de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 2.
              V5_Tir_Normal_Droite_P2(5);
            }
              break;
              // Cas du tir à droite de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 3.
              V5_Tir_Normal_Droite_P3();
            }
              break;
              // Cas du tir à droite de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 4.
              V5_Tir_Normal_Droite_P4(4);
            }
              break;
              // Cas du tir à droite de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à droite de puissance 5.
              V5_Tir_Normal_Droite_P5();
            }
              break;
          }
          // Test suivant le type de tir à gauche du joueur 1.
          switch (vj[J1].power_gauche)
          {                                                    // Cas du tir à gauche de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 1.
              V5_Tir_Normal_Gauche_P1();
            }
              break;
              // Cas du tir à gauche de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 2.
              V5_Tir_Normal_Gauche_P2(5);
            }
              break;
              // Cas du tir à gauche de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 3.
              V5_Tir_Normal_Gauche_P3();
            }
              break;
              // Cas du tir à gauche de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 4.
              V5_Tir_Normal_Gauche_P4(4);
            }
              break;
              // Cas du tir à gauche de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir normal à gauche de puissance 5.
              V5_Tir_Normal_Gauche_P5();
            }
              break;
          }
        }
          break;
      }
    }
    // Décrémente la cadence de tir du joueur 1.
    vj[J1].cad_tir--;


    // Si la btn gauche de la souris ou la barre d'espace est enfoncé, on initialise les données du tir.
    if(!vj[J1].cad_tir_spec && vj[J1].cad_tir > 0)
    {

      // Test suivant le type de vaisseau utilisé par le joueur.
      switch (vj[J1].type_vaisseau)
      {                                                        // Cas du premier vaisseau du joueur 1.
        case V1J:
        {                                                      // Test suivant le type de tir spécial avant du joueur 1.
          switch (vj[J1].power_spec_avant)
          {                                                    // Cas du tir avant special de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à l'avant de puissance 1.
              V1_Tir_Special_Avant_P1();
            }
              break;
              // Cas du tir avant de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à l'avant de puissance 2.
              V1_Tir_Special_Avant_P2();
            }
              break;
              // Cas du tir avant de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à l'avant de puissance 3.
              V1_Tir_Special_Avant_P3();
            }
              break;
              // Cas du tir avant de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à l'avant de puissance 4.
              V1_Tir_Special_Avant_P4();
            }
              break;
              // Cas du tir avant de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à l'avant de puissance 5.
              V1_Tir_Special_Avant_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial arrière du joueur 1.
          switch (vj[J1].power_spec_arriere)
          {                                                    // Cas du tir arrière de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à l'arrière de puissance 1.
              V1_Tir_Special_Arriere_P1();
            }
              break;
              // Cas du tir arrière de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à l'arrière de puissance 2.
              V1_Tir_Special_Arriere_P2();
            }
              break;
              // Cas du tir arrière de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à l'arrière de puissance 3.
              V1_Tir_Special_Arriere_P3();
            }
              break;
              // Cas du tir arrière de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à l'arrière de puissance 4.
              V1_Tir_Special_Arriere_P4();
            }
              break;
              // Cas du tir arrière de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à l'arrière de puissance 5.
              V1_Tir_Special_Arriere_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial à droite du joueur 1.
          switch (vj[J1].power_spec_droite)
          {                                                    // Cas du tir à droite de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à droite de puissance 1.
              V1_Tir_Special_Droite_P1();
            }
              break;
              // Cas du tir à droite de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à droite de puissance 2.
              V1_Tir_Special_Droite_P2();
            }
              break;
              // Cas du tir à droite de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à droite de puissance 3.
              V1_Tir_Special_Droite_P3();
            }
              break;
              // Cas du tir à droite de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à droite de puissance 4.
              V1_Tir_Special_Droite_P4();
            }
              break;
              // Cas du tir à droite de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à droite de puissance 5.
              V1_Tir_Special_Droite_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial à gauche du joueur 1.
          switch (vj[J1].power_spec_gauche)
          {                                                    // Cas du tir à gauche de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à gauche de puissance 1.
              V1_Tir_Special_Gauche_P1();
            }
              break;
              // Cas du tir à gauche de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à gauche de puissance 2.
              V1_Tir_Special_Gauche_P2();
            }
              break;
              // Cas du tir à gauche de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à gauche de puissance 3.
              V1_Tir_Special_Gauche_P3();
            }
              break;
              // Cas du tir à gauche de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à gauche de puissance 4.
              V1_Tir_Special_Gauche_P4();
            }
              break;
              // Cas du tir à gauche de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 1 le tir special à gauche de puissance 5.
              V1_Tir_Special_Gauche_P5();
            }
              break;
          }
        }
          break;
        case V2J:
        {                                                      // Test suivant le type de tir spécial avant du joueur 1.
          switch (vj[J1].power_spec_avant)
          {                                                    // Cas du tir avant special de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à l'avant de puissance 1.
              V2_Tir_Special_Avant_P1();
            }
              break;
              // Cas du tir avant de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à l'avant de puissance 2.
              V2_Tir_Special_Avant_P2();
            }
              break;
              // Cas du tir avant de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à l'avant de puissance 3.
              V2_Tir_Special_Avant_P3();
            }
              break;
              // Cas du tir avant de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à l'avant de puissance 4.
              V2_Tir_Special_Avant_P4();
            }
              break;
              // Cas du tir avant de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à l'avant de puissance 5.
              V2_Tir_Special_Avant_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial arrière du joueur 1.
          switch (vj[J1].power_spec_arriere)
          {                                                    // Cas du tir arrière de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à l'arrière de puissance 1.
              V2_Tir_Special_Arriere_P1();
            }
              break;
              // Cas du tir arrière de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à l'arrière de puissance 2.
              V2_Tir_Special_Arriere_P2();
            }
              break;
              // Cas du tir arrière de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à l'arrière de puissance 3.
              V2_Tir_Special_Arriere_P3();
            }
              break;
              // Cas du tir arrière de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à l'arrière de puissance 4.
              V2_Tir_Special_Arriere_P4();
            }
              break;
              // Cas du tir arrière de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à l'arrière de puissance 5.
              V2_Tir_Special_Arriere_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial à droite du joueur 1.
          switch (vj[J1].power_spec_droite)
          {                                                    // Cas du tir à droite de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à droite de puissance 1.
              V2_Tir_Special_Droite_P1();
            }
              break;
              // Cas du tir à droite de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à droite de puissance 2.
              V2_Tir_Special_Droite_P2();
            }
              break;
              // Cas du tir à droite de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à droite de puissance 3.
              V2_Tir_Special_Droite_P3();
            }
              break;
              // Cas du tir à droite de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à droite de puissance 4.
              V2_Tir_Special_Droite_P4();
            }
              break;
              // Cas du tir à droite de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à droite de puissance 5.
              V2_Tir_Special_Droite_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial à gauche du joueur 1.
          switch (vj[J1].power_spec_gauche)
          {                                                    // Cas du tir à gauche de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à gauche de puissance 1.
              V2_Tir_Special_Gauche_P1();
            }
              break;
              // Cas du tir à gauche de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à gauche de puissance 2.
              V2_Tir_Special_Gauche_P2();
            }
              break;
              // Cas du tir à gauche de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à gauche de puissance 3.
              V2_Tir_Special_Gauche_P3();
            }
              break;
              // Cas du tir à gauche de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à gauche de puissance 4.
              V2_Tir_Special_Gauche_P4();
            }
              break;
              // Cas du tir à gauche de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 2 le tir special à gauche de puissance 5.
              V2_Tir_Special_Gauche_P5();
            }
              break;
          }
        }
          break;
        case V3J:
        {                                                      // Test suivant le type de tir spécial avant du joueur 1.
          switch (vj[J1].power_spec_avant)
          {                                                    // Cas du tir avant special de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à l'avant de puissance 1.
              V3_Tir_Special_Avant_P1();
            }
              break;
              // Cas du tir avant de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à l'avant de puissance 2.
              V3_Tir_Special_Avant_P2();
            }
              break;
              // Cas du tir avant de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à l'avant de puissance 3.
              V3_Tir_Special_Avant_P3();
            }
              break;
              // Cas du tir avant de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à l'avant de puissance 4.
              V3_Tir_Special_Avant_P4();
            }
              break;
              // Cas du tir avant de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à l'avant de puissance 5.
              V3_Tir_Special_Avant_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial arrière du joueur 1.
          switch (vj[J1].power_spec_arriere)
          {                                                    // Cas du tir arrière de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à l'arrière de puissance 1.
              V3_Tir_Special_Arriere_P1();
            }
              break;
              // Cas du tir arrière de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à l'arrière de puissance 2.
              V3_Tir_Special_Arriere_P2();
            }
              break;
              // Cas du tir arrière de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à l'arrière de puissance 3.
              V3_Tir_Special_Arriere_P3();
            }
              break;
              // Cas du tir arrière de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à l'arrière de puissance 4.
              V3_Tir_Special_Arriere_P4();
            }
              break;
              // Cas du tir arrière de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à l'arrière de puissance 5.
              V3_Tir_Special_Arriere_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial à droite du joueur 1.
          switch (vj[J1].power_spec_droite)
          {                                                    // Cas du tir à droite de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à droite de puissance 1.
              V3_Tir_Special_Droite_P1();
            }
              break;
              // Cas du tir à droite de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à droite de puissance 2.
              V3_Tir_Special_Droite_P2();
            }
              break;
              // Cas du tir à droite de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à droite de puissance 3.
              V3_Tir_Special_Droite_P3();
            }
              break;
              // Cas du tir à droite de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à droite de puissance 4.
              V3_Tir_Special_Droite_P4();
            }
              break;
              // Cas du tir à droite de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à droite de puissance 5.
              V3_Tir_Special_Droite_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial à gauche du joueur 1.
          switch (vj[J1].power_spec_gauche)
          {                                                    // Cas du tir à gauche de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à gauche de puissance 1.
              V3_Tir_Special_Gauche_P1();
            }
              break;
              // Cas du tir à gauche de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à gauche de puissance 2.
              V3_Tir_Special_Gauche_P2();
            }
              break;
              // Cas du tir à gauche de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à gauche de puissance 3.
              V3_Tir_Special_Gauche_P3();
            }
              break;
              // Cas du tir à gauche de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à gauche de puissance 4.
              V3_Tir_Special_Gauche_P4();
            }
              break;
              // Cas du tir à gauche de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 3 le tir special à gauche de puissance 5.
              V3_Tir_Special_Gauche_P5();
            }
              break;
          }
        }
          break;
        case V4J:
        {                                                      // Test suivant le type de tir spécial avant du joueur 1.
          switch (vj[J1].power_spec_avant)
          {                                                    // Cas du tir avant special de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à l'avant de puissance 1.
              V4_Tir_Special_Avant_P1();
            }
              break;
              // Cas du tir avant de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à l'avant de puissance 2.
              V4_Tir_Special_Avant_P2();
            }
              break;
              // Cas du tir avant de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à l'avant de puissance 3.
              V4_Tir_Special_Avant_P3();
            }
              break;
              // Cas du tir avant de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à l'avant de puissance 4.
              V4_Tir_Special_Avant_P4();
            }
              break;
              // Cas du tir avant de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à l'avant de puissance 5.
              V4_Tir_Special_Avant_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial arrière du joueur 1.
          switch (vj[J1].power_spec_arriere)
          {                                                    // Cas du tir arrière de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à l'arrière de puissance 1.
              V4_Tir_Special_Arriere_P1();
            }
              break;
              // Cas du tir arrière de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à l'arrière de puissance 2.
              V4_Tir_Special_Arriere_P2();
            }
              break;
              // Cas du tir arrière de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à l'arrière de puissance 3.
              V4_Tir_Special_Arriere_P3();
            }
              break;
              // Cas du tir arrière de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à l'arrière de puissance 4.
              V4_Tir_Special_Arriere_P4();
            }
              break;
              // Cas du tir arrière de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à l'arrière de puissance 5.
              V4_Tir_Special_Arriere_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial à droite du joueur 1.
          switch (vj[J1].power_spec_droite)
          {                                                    // Cas du tir à droite de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à droite de puissance 1.
              V4_Tir_Special_Droite_P1();
            }
              break;
              // Cas du tir à droite de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à droite de puissance 2.
              V4_Tir_Special_Droite_P2();
            }
              break;
              // Cas du tir à droite de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à droite de puissance 3.
              V4_Tir_Special_Droite_P3();
            }
              break;
              // Cas du tir à droite de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à droite de puissance 4.
              V4_Tir_Special_Droite_P4();
            }
              break;
              // Cas du tir à droite de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à droite de puissance 5.
              V4_Tir_Special_Droite_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial à gauche du joueur 1.
          switch (vj[J1].power_spec_gauche)
          {                                                    // Cas du tir à gauche de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à gauche de puissance 1.
              V4_Tir_Special_Gauche_P1();
            }
              break;
              // Cas du tir à gauche de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à gauche de puissance 2.
              V4_Tir_Special_Gauche_P2();
            }
              break;
              // Cas du tir à gauche de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à gauche de puissance 3.
              V4_Tir_Special_Gauche_P3();
            }
              break;
              // Cas du tir à gauche de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à gauche de puissance 4.
              V4_Tir_Special_Gauche_P4();
            }
              break;
              // Cas du tir à gauche de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 4 le tir special à gauche de puissance 5.
              V4_Tir_Special_Gauche_P5();
            }
              break;
          }
        }
          break;
        case V5J:
        {                                                      // Test suivant le type de tir spécial avant du joueur 1.
          switch (vj[J1].power_spec_avant)
          {                                                    // Cas du tir avant special de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à l'avant de puissance 1.
              V5_Tir_Special_Avant_P1();
            }
              break;
              // Cas du tir avant de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à l'avant de puissance 2.
              V5_Tir_Special_Avant_P2();
            }
              break;
              // Cas du tir avant de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à l'avant de puissance 3.
              V5_Tir_Special_Avant_P3();
            }
              break;
              // Cas du tir avant de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à l'avant de puissance 4.
              V5_Tir_Special_Avant_P4();
            }
              break;
              // Cas du tir avant de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à l'avant de puissance 5.
              V5_Tir_Special_Avant_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial arrière du joueur 1.
          switch (vj[J1].power_spec_arriere)
          {                                                    // Cas du tir arrière de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à l'arrière de puissance 1.
              V5_Tir_Special_Arriere_P1();
            }
              break;
              // Cas du tir arrière de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à l'arrière de puissance 2.
              V5_Tir_Special_Arriere_P2();
            }
              break;
              // Cas du tir arrière de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à l'arrière de puissance 3.
              V5_Tir_Special_Arriere_P3();
            }
              break;
              // Cas du tir arrière de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à l'arrière de puissance 4.
              V5_Tir_Special_Arriere_P4();
            }
              break;
              // Cas du tir arrière de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à l'arrière de puissance 5.
              V5_Tir_Special_Arriere_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial à droite du joueur 1.
          switch (vj[J1].power_spec_droite)
          {                                                    // Cas du tir à droite de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à droite de puissance 1.
              V5_Tir_Special_Droite_P1();
            }
              break;
              // Cas du tir à droite de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à droite de puissance 2.
              V5_Tir_Special_Droite_P2();
            }
              break;
              // Cas du tir à droite de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à droite de puissance 3.
              V5_Tir_Special_Droite_P3();
            }
              break;
              // Cas du tir à droite de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à droite de puissance 4.
              V5_Tir_Special_Droite_P4();
            }
              break;
              // Cas du tir à droite de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à droite de puissance 5.
              V5_Tir_Special_Droite_P5();
            }
              break;
          }
          // Test suivant le type de tir spécial à gauche du joueur 1.
          switch (vj[J1].power_spec_gauche)
          {                                                    // Cas du tir à gauche de puissance 1 du joueur 1.
            case 1:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à gauche de puissance 1.
              V5_Tir_Special_Gauche_P1();
            }
              break;
              // Cas du tir à gauche de puissance 2 du joueur 1.
            case 2:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à gauche de puissance 2.
              V5_Tir_Special_Gauche_P2();
            }
              break;
              // Cas du tir à gauche de puissance 3 du joueur 1.
            case 3:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à gauche de puissance 3.
              V5_Tir_Special_Gauche_P3();
            }
              break;
              // Cas du tir à gauche de puissance 4 du joueur 1.
            case 4:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à gauche de puissance 4.
              V5_Tir_Special_Gauche_P4();
            }
              break;
              // Cas du tir à gauche de puissance 5 du joueur 1.
            case 5:
            {                                                  // Routine utilisée pour ajouter au vaisseau 5 le tir special à gauche de puissance 5.
              V5_Tir_Special_Gauche_P5();
            }
              break;
          }
        }
          break;
      }
    }
    // Décrémente la cadence de tir spécial du joueur 1.
    vj[J1].cad_tir_spec--;

    testBoutonOption();                                        //test option button
  }
}

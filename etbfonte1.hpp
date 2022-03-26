//*****************************************************************************
// copyright (c) 1998-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "etbfonte1.hpp"
// created      : ?
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
#ifndef __ETBFONTE1__
#define __ETBFONTE1__
//..............................................................................
#include "powermanga.hpp"
//..............................................................................
const long    iLARGEURFONTE=448;
const long    iHAUTEURFONTE=8;
//..............................................................................
class _etbfonte1 
{ private :              
    static char         pTexteAbout[];
    static char         pTexteVariables[];
    static char         pTexteVariables2[];
    static char         pTexteParametres[];
  //..............................................................................
  private :
    unsigned char*      pFonte;
    unsigned int        iAction;
    unsigned int        iTempo;
    int                 iParamPosY;
    int                 iIndexTouch;
    unsigned int        iTempoClavier;
    int                 num_level_sauve; 
    unsigned int        allez_gardien;
    //..............................................................................
  public :
                        _etbfonte1();
                        ~_etbfonte1();
    void                execution();
    int                 initialise();
    void                afficheTexte(int _iPosX, int _iPosY, char* _pTexte);
    void                afficheAbout();
    void                afficheVariables();
    void                afficheVariables2();
    void                afficheParametres();
    unsigned int        decode(unsigned char _cCaractere);
    void                entier2ASCII(int _iNombre, char* _pASCII, unsigned int _iReste);
    void                change_niveau();
    void                init_gardien();
};
#endif

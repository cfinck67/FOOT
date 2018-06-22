#ifndef _TABMntuRaw_HXX
#define _TABMntuRaw_HXX
/*!
  \file
  \version $Id: TABMntuRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TABMntuRaw.
*/
/*------------------------------------------+---------------------------------*/

#include "TObject.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"
#include "TABMparGeo.hxx"
#include "TABMntuHit.hxx"

#include <TRandom3.h>
#include "TString.h"

#include <string.h>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <vector>

using namespace std;

//##############################################################################

class TABMntuRaw : public TAGdata {
  public:

                    TABMntuRaw();
    virtual         ~TABMntuRaw();

    TABMntuHit*       Hit(Int_t i_ind);
    const TABMntuHit* Hit(Int_t i_ind) const;

    virtual void    SetupClones();

    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TABMntuRaw,1)

  public:
    Int_t           nhit;		    // 
    TClonesArray*   h;			    // hits
};

//------------------------------------------+-----------------------------------
//! Access \a i 'th hit

inline TABMntuHit* TABMntuRaw::Hit(Int_t i)
{
  return (TABMntuHit*) ((*h)[i]);;
}

//------------------------------------------+-----------------------------------
//! Read-only access \a i 'th hit

inline const TABMntuHit* TABMntuRaw::Hit(Int_t i) const
{
  return (const TABMntuHit*) ((*h)[i]);;
}


#endif

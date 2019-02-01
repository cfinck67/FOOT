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

    Int_t             GetHitsN() const;

    TABMntuHit*       Hit(Int_t i_ind);
    const TABMntuHit* Hit(Int_t i_ind) const;

    TABMntuHit*       NewHit(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t r, Double_t t, Double_t s);

    virtual void      SetupClones();

    virtual void      Clear(Option_t* opt="");

    virtual void      ToStream(ostream& os=cout, Option_t* option="") const;

  public:
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }

  private:
    TClonesArray*   fListOfHits;			    // hits
   
  private:
   static TString fgkBranchName;    // Branch name in TTree

   ClassDef(TABMntuRaw,1)
};

//------------------------------------------+-----------------------------------
//! Access \a i 'th hit

inline TABMntuHit* TABMntuRaw::Hit(Int_t i)
{
  return (TABMntuHit*) ((*fListOfHits)[i]);;
}

//------------------------------------------+-----------------------------------
//! Read-only access \a i 'th hit

inline const TABMntuHit* TABMntuRaw::Hit(Int_t i) const
{
  return (const TABMntuHit*) ((*fListOfHits)[i]);;
}


#endif

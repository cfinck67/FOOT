/*!
  \file
  \version $Id: TATWactNtuRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TATWactNtuRaw.
*/

#include "TATWparMap.hxx"
#include "TATWactNtuRaw.hxx"

/*!
  \class TATWactNtuRaw TATWactNtuRaw.hxx "TATWactNtuRaw.hxx"
  \brief Get Beam Monitor raw data from WD. **
*/

ClassImp(TATWactNtuRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATWactNtuRaw::TATWactNtuRaw(const char* name,
			     TAGdataDsc* p_datraw, 
			     TAGdataDsc* p_nturaw,
			     TAGparaDsc* p_pargeo)
  : TAGaction(name, "TATWactNtuRaw - Unpack TW raw data"),
    fpDatRaw(p_datraw),
    fpNtuRaw(p_nturaw),
    fpParGeo(p_pargeo)
{
  AddDataOut(p_nturaw, "TATWdatRaw");
  AddDataIn(p_datraw, "TATWntuRaw");
  AddPara(p_pargeo, "TATWparGeo");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TATWactNtuRaw::~TATWactNtuRaw()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TATWactNtuRaw::Action() {

   TATWdatRaw*    p_datraw = (TATWdatRaw*)   fpDatRaw->Object();
   TATWntuRaw*   p_nturaw = (TATWntuRaw*)  fpNtuRaw->Object();
   TATWparGeo*   p_pargeo = (TATWparGeo*)  fpParGeo->Object();

   p_nturaw->SetupClones();

   int nhit = p_datraw->nirhit;

   for(int ih = 0; ih< nhit; ih++) {
     TATWrawHit *aHi = p_datraw->Hit(ih);
     //Use mapping to retrieve the layer and bar ID
     int lay(0), bar(0);
     //If needed convert the charge into an eloss
     double pos(0);
     //Use geoMap to retrieve the position
     p_nturaw->NewHit(lay, bar, aHi->Charge(), aHi->Time(), pos);
     
   }
   
   fpNtuRaw->SetBit(kValid);
   return kTRUE;
}

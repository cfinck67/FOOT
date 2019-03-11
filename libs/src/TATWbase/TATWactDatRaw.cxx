/*!
  \file
  \version $Id: TATWactDatRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TATWactDatRaw.
*/

#include "TATWparMap.hxx"

#include "WDEvent.hh"
#include "TAGdaqEvent.hxx"
#include "TATWdatRaw.hxx"
#include "TATWactDatRaw.hxx"

/*!
  \class TATWactDatRaw TATWactDatRaw.hxx "TATWactDatRaw.hxx"
  \brief Get Beam Monitor raw data from WD. **
*/

ClassImp(TATWactDatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATWactDatRaw::TATWactDatRaw(const char* name,
			     TAGdataDsc* p_datraw, 
			     TAGdataDsc* p_datdaq,
			     TAGparaDsc* p_parmap)
  : TAGaction(name, "TATWactDatRaw - Unpack TW raw data"),
    fpDatRaw(p_datraw),
    fpDatDaq(p_datdaq),
    fpParMap(p_parmap)
{
  AddDataOut(p_datraw, "TATWdatRaw");
  AddDataIn(p_datdaq, "TAGdaqEvent");
  AddPara(p_parmap, "TATWparMap");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TATWactDatRaw::~TATWactDatRaw()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TATWactDatRaw::Action() {

   TATWdatRaw*    p_datraw = (TATWdatRaw*)   fpDatRaw->Object();
   TAGdaqEvent*   p_datdaq = (TAGdaqEvent*)  fpDatDaq->Object();
   TATWparMap*    p_parmap = (TATWparMap*)   fpParMap->Object();
  
   p_datraw->SetupClones();
   
   Int_t nFragments = p_datdaq->GetFragmentsN();

   for (Int_t i = 0; i < nFragments; ++i) {

      TString type = p_datdaq->GetClassType(i);
      if (type.Contains("WDEvent")) {
        const WDEvent* evt = static_cast<const WDEvent*> (p_datdaq->GetFragment(i));
        DecodeHits(evt);
      }
   }
   
   fpDatRaw->SetBit(kValid);
   
  return kTRUE;
}

//------------------------------------------+-----------------------------------
//! Decoding

Bool_t TATWactDatRaw::DecodeHits(const WDEvent* /*evt*/)
{
   return true;
}


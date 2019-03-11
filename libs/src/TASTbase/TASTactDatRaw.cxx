/*!
  \file
  \version $Id: TASTactDatRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TASTactDatRaw.
*/

#include "TASTparMap.hxx"

#include "WDEvent.hh"
#include "TAGdaqEvent.hxx"
#include "TASTdatRaw.hxx"
#include "TASTactDatRaw.hxx"

/*!
  \class TASTactDatRaw TASTactDatRaw.hxx "TASTactDatRaw.hxx"
  \brief Get Beam Monitor raw data from WD. **
*/

ClassImp(TASTactDatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TASTactDatRaw::TASTactDatRaw(const char* name,
			     TAGdataDsc* p_datraw, 
			     TAGdataDsc* p_datdaq,
			     TAGparaDsc* p_parmap)
  : TAGaction(name, "TASTactDatRaw - Unpack ST raw data"),
    fpDatRaw(p_datraw),
    fpDatDaq(p_datdaq),
    fpParMap(p_parmap)
{
  AddDataOut(p_datraw, "TASTdatRaw");
  AddDataIn(p_datdaq, "TAGdaqEvent");
  AddPara(p_parmap, "TASTparMap");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TASTactDatRaw::~TASTactDatRaw()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TASTactDatRaw::Action() {

   TASTdatRaw*    p_datraw = (TASTdatRaw*)   fpDatRaw->Object();
   TAGdaqEvent*   p_datdaq = (TAGdaqEvent*)  fpDatDaq->Object();
   TASTparMap*    p_parmap = (TASTparMap*)   fpParMap->Object();
  
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

Bool_t TASTactDatRaw::DecodeHits(const WDEvent* /*evt*/)
{
   return true;
}


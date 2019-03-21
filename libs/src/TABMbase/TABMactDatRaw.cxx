/*!
  \file
  \version $Id: TABMactDatRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TABMactDatRaw.
*/

#include "TDCEvent.hh"

#include "TABMparMap.hxx"
#include "TABMparCon.hxx"
#include "TAGdaqEvent.hxx"
#include "TABMdatRaw.hxx"
#include "TABMrawHit.hxx"
#include "TASTdatRaw.hxx"

#include "TABMactDatRaw.hxx"
#include <iomanip>

/*!
  \class TABMactDatRaw TABMactDatRaw.hxx "TABMactDatRaw.hxx"
  \brief Get Beam Monitor raw data from MBS. **
*/

ClassImp(TABMactDatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMactDatRaw::TABMactDatRaw(const char* name,
                             TAGdataDsc* p_datraw,
                             TAGdataDsc* p_datdaq,
                             TAGparaDsc* p_parmap,
                             TAGparaDsc* p_parcon,
                             TAGparaDsc* p_pargeo,
                             TAGdataDsc* p_timraw
                             )
  : TAGaction(name, "TABMactDatRaw - Unpack BM raw data"),
    fpDatRaw(p_datraw),
    fpDatDaq(p_datdaq),
    fpParMap(p_parmap),
    fpParCon(p_parcon),
    fpParGeo(p_pargeo),
    fpTimRaw(p_timraw)
{
  AddDataOut(p_datraw, "TABMdatRaw");
  AddPara(p_parmap, "TABMparMap");
  AddPara(p_parcon, "TABMparCon");
  AddPara(p_pargeo, "TABMparGeo");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMactDatRaw::~TABMactDatRaw()
{
}

//------------------------------------------+-----------------------------------
//! Action.
Bool_t TABMactDatRaw::Action() {
  
   TAGdaqEvent*   p_datqevt = (TAGdaqEvent*)  fpDatDaq->Object();

   Int_t nFragments = p_datqevt->GetFragmentsN();
   
   for (Int_t i = 0; i < nFragments; ++i) {
      
       TString type = p_datqevt->GetClassType(i);
       if (type.Contains("TDCEvent")) {
         const TDCEvent* evt = static_cast<const TDCEvent*> (p_datqevt->GetFragment(i));
         DecodeHits(evt);
       }
   }
   
   fpDatRaw->SetBit(kValid);
   
   return kTRUE;
}
   
//------------------------------------------+-----------------------------------
//! Action.
Bool_t TABMactDatRaw::DecodeHits(const TDCEvent* evt) {
   
   // Fill BM_struct with TDCEvent output
   TABMdatRaw*    p_datraw = (TABMdatRaw*)    fpDatRaw->Object();

   //From there we get the Mapping of the wires into the Chamber to the TDC channels
   TABMparMap*    p_parmap = (TABMparMap*)    fpParMap->Object();
   TABMparCon*    p_parcon = (TABMparCon*)    fpParCon->Object();
   TABMparGeo*    p_pargeo = (TABMparGeo*)    fpParGeo->Object();
   TASTdatRaw*    p_timraw = (TASTdatRaw*)    fpTimRaw->Object();
   

   Int_t size = (int)evt->measurement.size();
   
   for (Int_t i = 0; i < size; ++i) {
      printf("%08x\n", evt->measurement[i]);
   }
   return true;
}


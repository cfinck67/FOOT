/*!
  \file
  \version $Id: TATRactDatRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TATRactDatRaw.
*/

#include "TATRparMap.hxx"
#include "TAGdaqEvent.hxx"
#include "TATRdatRaw.hxx"
#include "TATRactDatRaw.hxx"

#include <iomanip>


/*!
  \class TATRactDatRaw TATRactDatRaw.hxx "TATRactDatRaw.hxx"
  \brief Get Beam Monitor raw data from MBS. **
*/

ClassImp(TATRactDatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATRactDatRaw::TATRactDatRaw(const char* name,
			     TAGdataDsc* p_datraw, 
			     TAGdataDsc* p_datqevt,
			     TAGparaDsc* p_parmap)
  : TAGaction(name, "TATRactDatRaw - Unpack BM raw data"),
    fpDatRaw(p_datraw),
    fpDaqEvent(p_datqevt),
    fpParMap(p_parmap)
{
  AddDataOut(p_datraw, "TATRdatRaw");
  AddDataIn(p_datqevt, "TAGdaqEvent");
  AddPara(p_parmap, "TATRparMap");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TATRactDatRaw::~TATRactDatRaw()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TATRactDatRaw::Action() {

  TATRdatRaw*    p_datraw = (TATRdatRaw*)    fpDatRaw->Object();
  TAGdaqEvent*   p_datqevt = (TAGdaqEvent*)  fpDaqEvent->Object();
  
  //From there we get the Mapping of the wires into the Chamber to the VUL channels
  TATRparMap*    p_parmap = (TATRparMap*)    fpParMap->Object();
  
  p_datraw->SetupClones();

   Int_t nFragements = p_datqevt->GetFragmentsN();
   
   for (Int_t i = 0; i < nFragements; ++i) {
      
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
//! Fill hits.

void TATRactDatRaw::DecodeHits(const TDCEvent* /*evt*/)
{
   int type;
   int channel;
   double charge;
   double time;
   
   TATRdatRaw* p_datraw = (TATRdatRaw*)    fpDatRaw->Object();
   TATRrawHit* mytmp    = p_datraw->NewHit(type, channel, charge, time);
}

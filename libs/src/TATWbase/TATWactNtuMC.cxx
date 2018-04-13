/*!
  \file
  \version $Id: TATWactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TATWactNtuMC.
*/

#include "TATWdatRaw.hxx"
#include "TATWactNtuMC.hxx"

/*!
  \class TATWactNtuMC TATWactNtuMC.hxx "TATWactNtuMC.hxx"
  \brief NTuplizer for BM raw hits. **
*/

ClassImp(TATWactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATWactNtuMC::TATWactNtuMC(const char* name,
			 TAGdataDsc* p_datraw, 
			 EVENT_STRUCT* evStr)
  : TAGaction(name, "TATWactNtuMC - NTuplize ToF raw data"),
    fpNtuMC(p_datraw),
    fpEvtStr(evStr)
{
  Info("Action()"," Creating the Beam Monitor MC tuplizer action\n");
  AddDataOut(p_datraw, "TATWdatRaw");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TATWactNtuMC::~TATWactNtuMC()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TATWactNtuMC::Action()
{

  TATWdatRaw* p_nturaw = (TATWdatRaw*) fpNtuMC->Object();
  Int_t nhits(0);
  if (!p_nturaw->hir) p_nturaw->SetupClones();

  //The number of hits inside the Start Counter is stn
  Info("Action()","Processing n Scint :: %2d hits \n",fpEvtStr->SCNn);
  for (Int_t i = 0; i < fpEvtStr->SCNn; i++) {
    //First two numbers make sense only for data (typ, channel)
    
     TATWrawHit *mytmp = new((*(p_nturaw->hir))[i])
     TATWrawHit(0,0,fpEvtStr->SCNde[i],fpEvtStr->SCNtim[i]);
    
     mytmp->SetMCID(fpEvtStr->SCNid[i]);
     nhits++;
  }
  
  p_nturaw->nirhit  = nhits;

  fpNtuMC->SetBit(kValid);
  return kTRUE;
}


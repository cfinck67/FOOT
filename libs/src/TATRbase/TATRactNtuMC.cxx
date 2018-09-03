/*!
  \file
  \version $Id: TATRactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TATRactNtuMC.
*/

#include "TATRdatRaw.hxx"
#include "TATRactNtuMC.hxx"

/*!
  \class TATRactNtuMC TATRactNtuMC.hxx "TATRactNtuMC.hxx"
  \brief NTuplizer for BM raw hits. **
*/

ClassImp(TATRactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATRactNtuMC::TATRactNtuMC(const char* name,
			 TAGdataDsc* p_datraw, 
			 EVENT_STRUCT* evStr)
  : TAGaction(name, "TATRactNtuMC - NTuplize ToF raw data"),
    fpNtuMC(p_datraw),
    fpEvtStr(evStr)
{
  Info("Action()"," Creating the Beam Monitor MC tuplizer action\n");
  AddDataOut(p_datraw, "TATRdatRaw");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TATRactNtuMC::~TATRactNtuMC()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TATRactNtuMC::Action()
{

  TATRdatRaw* p_nturaw = (TATRdatRaw*) fpNtuMC->Object();
  Int_t nhits(0);
  if (!p_nturaw->h) p_nturaw->SetupClones();

  //The number of hits inside the Start Counter is stn
  Info("Action()","Processing n SC :: %2d hits \n",fpEvtStr->stn);
  for (Int_t i = 0; i < fpEvtStr->stn; i++) {
    TATRrawHit *mytmp = new((*(p_nturaw->h))[i]) 
      TATRrawHit(0,1,fpEvtStr->stSigAmp,fpEvtStr->stSigTime);
    nhits++;
  }
  
  //The number of hits inside the Start Counter is stn
  Info("Action()","Processing n Onion :: %2d hits \n",fpEvtStr->onSigN);
  for (Int_t i = 0; i < fpEvtStr->onSigN; i++) {
    TATRrawHit *mytmp = new((*(p_nturaw->h))[i]) 
      TATRrawHit(1,fpEvtStr->onSigReg[i],fpEvtStr->onSigAmp[i],fpEvtStr->onSigTim[i]);
    nhits++;
  }
  
  p_nturaw->nhit  = nhits;

  fpNtuMC->SetBit(kValid);
  return kTRUE;
}


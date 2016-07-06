/*!
  \file
  \version $Id: TAIRactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TAIRactNtuMC.
*/

#include "TAIRdatRaw.hxx"
#include "TAIRactNtuMC.hxx"

/*!
  \class TAIRactNtuMC TAIRactNtuMC.hxx "TAIRactNtuMC.hxx"
  \brief NTuplizer for BM raw hits. **
*/

ClassImp(TAIRactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAIRactNtuMC::TAIRactNtuMC(const char* name,
			 TAGdataDsc* p_datraw, 
			 EVENT_STRUCT* evStr)
  : TAGaction(name, "TAIRactNtuMC - NTuplize ToF raw data"),
    fpNtuMC(p_datraw),
    fpEvtStr(evStr)
{
  Info("Action()"," Creating the Beam Monitor MC tuplizer action\n");
  AddDataOut(p_datraw, "TAIRdatRaw");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAIRactNtuMC::~TAIRactNtuMC()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TAIRactNtuMC::Action()
{

  TAIRdatRaw* p_nturaw = (TAIRdatRaw*) fpNtuMC->Object();
  Int_t nhits(0);
  if (!p_nturaw->hir) p_nturaw->SetupClones();

  //The number of hits inside the Start Counter is stn
  Info("Action()","Processing n SC :: %2d hits \n",fpEvtStr->stn);
  for (Int_t i = 0; i < fpEvtStr->stn; i++) {
    //TAIRrawHit *mytmp = 
	new((*(p_nturaw->hir))[i]) TAIRrawHit(0,0,fpEvtStr->stSigAmp,fpEvtStr->stSigTime);
    nhits++;
  }
  
  //The number of hits inside the Start Counter is stn
  Info("Action()","Processing n Onion :: %2d hits \n",fpEvtStr->onSigN);
  for (Int_t i = 0; i < fpEvtStr->onSigN; i++) {
    //TAIRrawHit *mytmp = 
	new((*(p_nturaw->hir))[i]) TAIRrawHit(1,fpEvtStr->onSigReg[i],fpEvtStr->onSigAmp[i],fpEvtStr->onSigTim[i]);
    nhits++;
  }
  
  double meantime(0), firsttime(-10000);
  Int_t i_nhit = p_nturaw->nirhit;
  for (Int_t i = 0; i < i_nhit; i++) {
    const TAIRrawHit* aHi = p_nturaw->Hit(i);
    if(aHi->Type() == 0) {
      if(aHi->ChID() == 0) firsttime = aHi->Time();
      meantime += aHi->Time();
    }
  }
  if(i_nhit) meantime /= i_nhit;
  
  //Set up of the Trigger Time
  p_nturaw->SetTrigTime(firsttime);

  p_nturaw->nirhit  = nhits;

  fpNtuMC->SetBit(kValid);
  return kTRUE;
}


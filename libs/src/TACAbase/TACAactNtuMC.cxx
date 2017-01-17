/*!
  \file
  \version $Id: TACAactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TACAactNtuMC.
*/

#include "TACAdatRaw.hxx"
#include "TACAactNtuMC.hxx"

/*!
  \class TACAactNtuMC TACAactNtuMC.hxx "TACAactNtuMC.hxx"
  \brief NTuplizer for Calo raw hits. **
*/

ClassImp(TACAactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TACAactNtuMC::TACAactNtuMC(const char* name,
			 TAGdataDsc* p_datraw, 
			 EVENT_STRUCT* evStr)
  : TAGaction(name, "TACAactNtuMC - NTuplize ToF raw data"),
    fpNtuMC(p_datraw),
    fpEvtStr(evStr)
{
  Info("Action()"," Creating the Beam Monitor MC tuplizer action\n");
  AddDataOut(p_datraw, "TACAdatRaw");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TACAactNtuMC::~TACAactNtuMC()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TACAactNtuMC::Action()
{

  TACAdatRaw* p_nturaw = (TACAdatRaw*) fpNtuMC->Object();
  Int_t nhits(0);
  if (!p_nturaw->hir) p_nturaw->SetupClones();

  //The number of hits inside the Calo is CALn
  Info("Action()","Processing n Calo :: %2d hits \n",fpEvtStr->CALn);
  for (Int_t i = 0; i < fpEvtStr->CALn; i++) {
    //First two numbers make sense only for data (typ, channel)
    new((*(p_nturaw->hir))[i]) TACArawHit(0,0,fpEvtStr->CALde[i],fpEvtStr->CALtim[i]);
    nhits++;
  }
  
  double meantime(0), firsttime(-10000);
  Int_t i_nhit = p_nturaw->nirhit;
  for (Int_t i = 0; i < i_nhit; i++) {
    const TACArawHit* aHi = p_nturaw->Hit(i);
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


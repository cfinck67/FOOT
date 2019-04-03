/*!
  \file
  \version $Id: TASTactNtuRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TASTactNtuRaw.
*/

#include "TASTparMap.hxx"
#include "TASTactNtuRaw.hxx"

/*!
  \class TASTactNtuRaw TASTactNtuRaw.hxx "TASTactNtuRaw.hxx"
  \brief Get Beam Monitor raw data from WD. **
*/

ClassImp(TASTactNtuRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TASTactNtuRaw::TASTactNtuRaw(const char* name,
			     TAGdataDsc* p_datraw, 
			     TAGdataDsc* p_nturaw)
  : TAGaction(name, "TASTactNtuRaw - Unpack ST raw data"),
    fpDatRaw(p_datraw),
    fpNtuRaw(p_nturaw)
{
  AddDataIn(p_datraw, "TASTdatRaw");
  AddDataOut(p_nturaw, "TASTntuRaw");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TASTactNtuRaw::~TASTactNtuRaw()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TASTactNtuRaw::Action() {

   TASTdatRaw*    p_datraw = (TASTdatRaw*)   fpDatRaw->Object();
   TASTntuRaw*   p_nturaw = (TASTntuRaw*)  fpNtuRaw->Object();

   p_nturaw->SetupClones();

   vector<TASTrawHit*> myHits = p_datraw->GetHits();
   int nHit = myHits.size(); 

   for(int ih = 0; ih< nHit; ih++) {
     int ch_num = myHits.at(ih)->GetChannel();
     double q =  myHits.at(ih)->GetCharge();
     double time = myHits.at(ih)->GetArrivalTime();
     
     p_nturaw->NewHit(ch_num, q, time);
   }

   p_nturaw->SetCharge(p_datraw->Charge());
   p_nturaw->SetTriggerTime(p_datraw->TrigTime());
   
   fpNtuRaw->SetBit(kValid);
   return kTRUE;
}

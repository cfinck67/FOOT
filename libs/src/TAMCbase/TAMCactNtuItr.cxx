/*!
 \file
 \version $Id: TAMCactNtuItr.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
 \brief   Implementation of TAMCactNtuItr.
 */

#include "TAMCntuHit.hxx"
#include "TAMCactNtuItr.hxx"

//FOOT
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"

/*!
 \class TAMCactNtuItr TAMCactNtuItr.hxx "TAMCactNtuItr.hxx"
 \brief NTuplizer for MC events. **
 */

ClassImp(TAMCactNtuItr);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAMCactNtuItr::TAMCactNtuItr(const char* name, TAGdataDsc* p_nturaw, EVENT_STRUCT* evStr)
 : TAGaction(name, "TAMCactNtuItr - NTuplize MC evt raw data"),
   fpNtuMC(p_nturaw),
   fpEvtStr(evStr)
{
   AddDataOut(p_nturaw, "TAMCntuHit");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAMCactNtuItr::~TAMCactNtuItr()
{
}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TAMCactNtuItr::Action() {
   
   TAMCntuHit* p_nturaw = (TAMCntuHit*) fpNtuMC->Object();
   
   p_nturaw->Clear();
   
   for (Int_t i = 0; i < fpEvtStr->ITRn; i++) {
      
      Int_t sensorId = fpEvtStr->ITRilay[i]*16 +  fpEvtStr->ITRiplume[i]*4 + fpEvtStr->ITRimimo[i];
      
      TVector3 pos( fpEvtStr->ITRxin[i], fpEvtStr->ITRyin[i], fpEvtStr->ITRzin[i]);
      TVector3 mom( fpEvtStr->ITRpxin[i], fpEvtStr->ITRpyin[i], fpEvtStr->ITRpzin[i]);
      
      p_nturaw->NewHit(sensorId, pos, mom, fpEvtStr->ITRde[i], fpEvtStr->ITRtim[i]);
   }
   
   fpNtuMC->SetBit(kValid);
   return kTRUE;
}


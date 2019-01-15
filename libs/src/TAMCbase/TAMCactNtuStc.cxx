/*!
 \file
 \version $Id: TAMCactNtuStc.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
 \brief   Implementation of TAMCactNtuStc.
 */

#include "TAMCntuHit.hxx"
#include "TAMCactNtuStc.hxx"

//FOOT
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"

/*!
 \class TAMCactNtuStc TAMCactNtuStc.hxx "TAMCactNtuStc.hxx"
 \brief NTuplizer for MC events. **
 */

ClassImp(TAMCactNtuStc);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAMCactNtuStc::TAMCactNtuStc(const char* name, TAGdataDsc* p_nturaw, EVENT_STRUCT* evStr)
 : TAGaction(name, "TAMCactNtuStc - NTuplize MC start counter evt"),
   fpNtuMC(p_nturaw),
   fpEvtStr(evStr)
{
   AddDataOut(p_nturaw, "TAMCntuHit");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAMCactNtuStc::~TAMCactNtuStc()
{
}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TAMCactNtuStc::Action() {
   
   TAMCntuHit* p_nturaw = (TAMCntuHit*) fpNtuMC->Object();
   
   p_nturaw->Clear();
   
   for (Int_t i = 0; i < fpEvtStr->STCn; i++) {
      
      Int_t id      = 0;
      Int_t trackId = fpEvtStr->STCid[i] - 1;

      TVector3 pos( fpEvtStr->STCxin[i], fpEvtStr->STCyin[i], fpEvtStr->STCzin[i]);
      TVector3 mom( fpEvtStr->STCpxin[i], fpEvtStr->STCpyin[i], fpEvtStr->STCpzin[i]);
      
      p_nturaw->NewHit(id, pos, mom, fpEvtStr->STCde[i], fpEvtStr->STCtim[i], trackId);
   }
   
   fpNtuMC->SetBit(kValid);
   return kTRUE;
}


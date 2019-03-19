/*!
 \file
 \version $Id: TAVTactNtuRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
 \brief   Implementation of TAVTactNtuRaw.
 */

#include "TH2F.h"

#include "DECardEvent.hh"
#include "TAGdaqEvent.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"

#include "TAVTdatRaw.hxx"
#include "TAVTactNtuRaw.hxx"

/*!
 \class TAVTactNtuRaw TAVTactNtuRaw.hxx "TAVTactNtuRaw.hxx"
 \brief Get vertex raw data from MBS. **
 */

ClassImp(TAVTactNtuRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAVTactNtuRaw::TAVTactNtuRaw(const char* name, TAGdataDsc* pNtuRaw, TAGdataDsc* pDatDaq, TAGparaDsc* pGeoMap, TAGparaDsc* pConfig)
: TAVTactBaseRaw(name, pNtuRaw, pGeoMap, pConfig),
  fpDatDaq(pDatDaq)
{
   AddDataIn(pDatDaq, "TAGdaqEvent");
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTactNtuRaw::~TAVTactNtuRaw()
{   
}

//------------------------------------------+-----------------------------------
//! Action.
Bool_t TAVTactNtuRaw::Action() {
   
   TAGdaqEvent* datDaq = (TAGdaqEvent*)  fpDatDaq->Object();
   
   Int_t nFragments = datDaq->GetFragmentsN();
   
   for (Int_t i = 0; i < nFragments; ++i) {
      
       TString type = datDaq->GetClassType(i);
       if (type.Contains("DECardEvent")) {
         const DECardEvent* evt = static_cast<const DECardEvent*> (datDaq->GetFragment(i));
          DecodeEvent(evt);
       }
   }
   
   fpNtuRaw->SetBit(kValid);
   
   return kTRUE;
}

// --------------------------------------------------------------------------------------
Bool_t TAVTactNtuRaw::DecodeEvent(const DECardEvent* evt)
{
   fData      = evt->values;
   fEventSize = evt->evtSize;
   fIndex     = 0;
   MI26_FrameRaw* data = new MI26_FrameRaw;

   
   TAVTparGeo*  pGeoMap = (TAVTparGeo*)  fpGeoMap->Object();
   
   // Vertex header
   if (!GetVtxHeader()) return false;
   
   // loop over boards
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
      
      if (!GetSensorHeader(i)) return false;
      
      fTriggerNumber = -1;
      
      // loop over frame (3 max)
      while (GetFrame(data)) {
         DecodeFrame(i, data);
      }
   }
   
   if(fDebugLevel>3)
      for (Int_t i = 0; i < fEventSize; ++i)
         printf("Data %x\n", fData[i]);
   
   delete data;

   return true;
}


// private method
// --------------------------------------------------------------------------------------
Bool_t TAVTactNtuRaw::GetVtxHeader()
{
   do {
      if (fData[fIndex] == DECardEvent::GetVertexHeader()) {
         return true;
      }
   } while (fIndex++ < fEventSize);
   
   return false;
}

// --------------------------------------------------------------------------------------
Bool_t TAVTactNtuRaw::GetSensorHeader(Int_t iSensor)
{
   UInt_t fakeTrigger   = 0;
   UInt_t fakeTimeStamp = 0;
   
   do {
      if (fData[fIndex] == GetKeyHeader(iSensor)) {
         fEventNumber  = fData[++fIndex];
         fakeTrigger   = fData[++fIndex];
         fakeTimeStamp = fData[++fIndex];
         return true;
      }
   } while (fIndex++ < fEventSize);
   
   return false;
}

// --------------------------------------------------------------------------------------
Bool_t TAVTactNtuRaw::GetFrame(MI26_FrameRaw* data)
{
   Bool_t ok = false;
   
   // check frame header
   if (fData[fIndex] ==  (GetFrameHeader() & 0xFFF)) { // protection against wrong header !!!
      memcpy(data, &fData[fIndex], sizeof(MI26_FrameRaw));
      if (fTriggerNumber == -1) fTriggerNumber = data->TriggerCnt;
      ok =  CheckTrigger(data);
      
   } else
      return false;
 
   // go to frame trailer
   do {
      if (fData[fIndex] == (GetFrameTail() & 0xFFF))
         break;
      
   } while (fIndex++ < fEventSize);
   
   if (fDebugLevel) {
      printf("%08x\n", data->Header);
      printf("%08x\n", data->TriggerCnt);
      printf("%08x\n", data->TimeStamp);
      printf("%08x\n", data->FrameCnt);
      printf("%08x\n", data->DataLength);
      Int_t dataLength    = ((data->DataLength & 0xFFFF0000)>>16);
      for (Int_t i = 0; i < dataLength; ++i)
         printf("%08x\n", data->ADataW16[i]);
   }
   return ok;
}


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
         if (DecodeEvent(evt))
            DecodeFrame();
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
   
   if (!GetEventHeader()) return false;
      
   if (!GetStart()) return false;
      
   MI26_FrameRaw* data = new MI26_FrameRaw;
   GetFrame(data);
   UInt_t trigger = data->TriggerCnt;
   GetNextFrames(trigger);
   delete data;
   
   
   if(fDebugLevel>3)
      for (Int_t i = 0; i < fEventSize; ++i)
         printf("Data %x\n", fData[i]);
   
   return true;   
}


// private method
// --------------------------------------------------------------------------------------
Bool_t TAVTactNtuRaw::GetEventHeader()
{
   do {
      if (fData[fIndex] == DECardEvent::GetVertexHeader()) {
         fEventNumber = fData[++fIndex];
         return true;
      }
   } while (fIndex++ < fEventSize);
   
   return false;
}

// --------------------------------------------------------------------------------------
Bool_t TAVTactNtuRaw::GetStart()
{   
   do {
      if (fData[fIndex] == fgkFrameHeader)
         return true;
   
   } while (fIndex++ < fEventSize);
   
   return false;
}

// --------------------------------------------------------------------------------------
void TAVTactNtuRaw::GetNextFrames(UInt_t trigger)
{
   MI26_FrameRaw* data = new MI26_FrameRaw;
   Bool_t sameTrigger = true;
   Int_t index = 0;
   do {
      index = fIndex;
      GetStart();
      GetFrame(data);
      if (data->TriggerCnt != trigger) {
         sameTrigger = false;
         break;
      }
   } while(sameTrigger);
   
   fIndex = index;
   fEventSize = fIndex;
   delete data;
}

// --------------------------------------------------------------------------------------
void TAVTactNtuRaw::GetFrame(MI26_FrameRaw* data)
{
   memcpy(data, &fData[fIndex-GetHeaderSize()], GetHeaderSize()*4);   
}


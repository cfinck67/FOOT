/*!
 \file
 \version $Id: TAITactDatRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
 \brief   Implementation of TAITactDatRaw.
 */

#include "TH2F.h"

#include "TAGmbsEvent.hxx"
#include "TAITparGeo.hxx"
#include "TAITparConf.hxx"

#include "TAITdatRaw.hxx"
#include "TAITactDatRaw.hxx"

/*!
 \class TAITactDatRaw TAITactDatRaw.hxx "TAITactDatRaw.hxx"
 \brief Get vertex raw data from MBS. **
 */

ClassImp(TAITactDatRaw);
const UChar_t TAITactDatRaw::fgkGeoId1     = 4;
const UChar_t TAITactDatRaw::fgkGeoId2     = 5;

//------------------------------------------+-----------------------------------
//! Default constructor.

TAITactDatRaw::TAITactDatRaw(const char* name, TAGdataDsc* pDatRaw, TAGdataDsc* pDatMbs, TAGparaDsc* pGeoMap, TAGparaDsc* pConfig)
: TAITactBaseRaw(name, pDatRaw, pGeoMap, pConfig),
  fpDatMbs(pDatMbs)
{
   AddDataIn(pDatMbs, "TAGmbsEvent");
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITactDatRaw::~TAITactDatRaw()
{   
}

//------------------------------------------+-----------------------------------
//! Action.
Bool_t TAITactDatRaw::Action() {
   
   TAGmbsEvent* pDatMbs = (TAGmbsEvent*) fpDatMbs->Object();
   //Select only Physics Triggers
   if(pDatMbs->EventTrigger()==1) {
	  Int_t nSubEvent = pDatMbs->NSubEvent();
	  for (Int_t i = 0; i < nSubEvent; ++i) {
		 if (fDebugLevel)
			Info("Action()"," ET, SubT, procid, procty, crate %d %d %d %d %d", pDatMbs->SubEventType(i), 
				 pDatMbs->SubEventSubType(i),  pDatMbs->SubEventProcId(i), 
				 pDatMbs->SubEventProcType(i), pDatMbs->SubEventCrate(i));
		 
		 fCurrentTriggerCnt = 0;
		 if (pDatMbs->SubEventType(i) == 42) {
			Int_t subEventSize  = pDatMbs->SubEventSize(i);
			const UInt_t* data  = pDatMbs->SubEventData(i);
			if (FindData(data, subEventSize)) DecodeFrame();
		 }
	  }
   } else {
	  Info("Action()","Cosmics Trigger :: %d . Not going to process the event",pDatMbs->EventTrigger());
   }
   fpDatRaw->SetBit(kValid);
   
   return kTRUE;
}

// --------------------------------------------------------------------------------------
Bool_t TAITactDatRaw::FindData(const UInt_t* data, Int_t subEventSize)
{
   /*
	UInt_t length = data[index]       & 0x3FFFF;
	UInt_t geoId  = data[index] >> 23 & 0x1F;
	UInt_t type   = data[index] >> 19 & 0x7;
	*/
   
   Int_t index = 0;
   do {
	  IR_Header* header =  (IR_Header*)(&data[index]);
	  UInt_t length  = header->Length;
	  UShort_t geoId = header->GeoId;
	  UChar_t type   = header->Type;

	  if (geoId == fgkGeoId1 || geoId == fgkGeoId2) {
		 if (fDebugLevel)
			Info("FindData", "Find Vertex Data with Geo/Id %d and type %d", geoId, type);
		 
		 if (header->Ovf) {
			Warning("FindData", "Find Vertex Data with overflow for Geo/Id %d", geoId);
			return false;
		 }
		 
    	 fEventSize = length;
     	 fData = &data[index+1];
		 return true;
	  }
	  index += length+1;
	  
   } while (index < subEventSize);
   
   return false;
}


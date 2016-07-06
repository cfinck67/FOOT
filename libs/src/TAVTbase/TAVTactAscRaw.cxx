/*!
 \file
 \version $Id: TAVTactAscRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
 \brief   Implementation of TAVTactAscRaw.
 */

#include "TH2F.h"

#include "TAVTrawEvent.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"

#include "TAVTdatRaw.hxx"
#include "TAVTactAscRaw.hxx"

/*!
 \class TAVTactAscRaw TAVTactAscRaw.hxx "TAVTactAscRaw.hxx"
 \brief Get vertex raw data from ascii files. **
 */

ClassImp(TAVTactAscRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAVTactAscRaw::TAVTactAscRaw(const char* name, TAGdataDsc* pDatRaw, TAGdataDsc* pDatAsc, TAGparaDsc* pGeoMap, TAGparaDsc* pConfig)
: TAVTactBaseRaw(name, pDatRaw, pGeoMap, pConfig),
  fpDatAsc(pDatAsc)
{
   AddDataIn(pDatAsc, "TAVTrawEvent");
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTactAscRaw::~TAVTactAscRaw()
{   
}

//------------------------------------------+-----------------------------------
//! Action.
Bool_t TAVTactAscRaw::Action() 
{
   TAVTrawEvent* pDatAsc = (TAVTrawEvent*) fpDatAsc->Object();
   
   fCurrentTriggerCnt = 0;
   fEventSize         = pDatAsc->EventSize();
   fData              = pDatAsc->EventData();
   
	 DecodeFrame();
	 fpDatRaw->SetBit(kValid);
   
   return true;
}

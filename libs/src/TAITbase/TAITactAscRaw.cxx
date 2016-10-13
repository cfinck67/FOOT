/*!
 \file
 \version $Id: TAITactAscRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
 \brief   Implementation of TAITactAscRaw.
 */

#include "TH2F.h"

#include "TAITrawEvent.hxx"
#include "TAITparGeo.hxx"
#include "TAITparConf.hxx"

#include "TAITdatRaw.hxx"
#include "TAITactAscRaw.hxx"

/*!
 \class TAITactAscRaw TAITactAscRaw.hxx "TAITactAscRaw.hxx"
 \brief Get vertex raw data from ascii files. **
 */

ClassImp(TAITactAscRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAITactAscRaw::TAITactAscRaw(const char* name, TAGdataDsc* pDatRaw, TAGdataDsc* pDatAsc, TAGparaDsc* pGeoMap, TAGparaDsc* pConfig)
: TAITactBaseRaw(name, pDatRaw, pGeoMap, pConfig),
  fpDatAsc(pDatAsc)
{
   AddDataIn(pDatAsc, "TAITrawEvent");
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITactAscRaw::~TAITactAscRaw()
{   
}

//------------------------------------------+-----------------------------------
//! Action.
Bool_t TAITactAscRaw::Action() 
{
   TAITrawEvent* pDatAsc = (TAITrawEvent*) fpDatAsc->Object();
   
   fCurrentTriggerCnt = 0;
   fEventSize         = pDatAsc->EventSize();
   fData              = pDatAsc->EventData();
   
	 DecodeFrame();
	 fpDatRaw->SetBit(kValid);
   
   return true;
}

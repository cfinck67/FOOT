/*!
 \file
 \version $Id: TAVTactBaseRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
 \brief   Implementation of TAVTactBaseRaw.
 */

#include "TH2F.h"

#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"

#include "TAVTntuRaw.hxx"
#include "TAVTactBaseRaw.hxx"

/*!
 \class TAVTactBaseRaw TAVTactBaseRaw.hxx "TAVTactBaseRaw.hxx"
 \brief Base class to get vertex raw data. **
 */

ClassImp(TAVTactBaseRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAVTactBaseRaw::TAVTactBaseRaw(const char* name, TAGdataDsc* pDatRaw, TAGparaDsc* pGeoMap, TAGparaDsc* pConfig)
: TAGactionFile(name, "TAVTactBaseRaw - Base action for unpack vertex raw data"),
  fpNtuRaw(pDatRaw),
  fpGeoMap(pGeoMap),
  fpConfig(pConfig),
  fData(0x0),
  fNSensors(8),
  fIndex(0),
  fCurrentTriggerCnt(0),
  fReadingEvent(0),
  fOverflow(0),
  fEventsOverflow(0), 
  fNStatesInLine(0)
{
   AddDataOut(pDatRaw, "TAVTntuRaw");
   AddPara(pGeoMap, "TAVTparGeo");
   AddPara(pConfig, "TAVTparConf");
   TAVTparGeo* geoMap = (TAVTparGeo*) fpGeoMap->Object();
   fNSensors = geoMap->GetNSensors();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTactBaseRaw::~TAVTactBaseRaw()
{   
}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAVTactBaseRaw::CreateHistogram()
{
   DeleteHistogram();
   TAVTparGeo* pGeoMap = (TAVTparGeo*) fpGeoMap->Object();
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
	  if (TAVTparConf::IsMapHistOn()) {
		 fpHisPixelMap[i] = new TH2F(Form("vtPixelMap%d", i+1), Form("Vertex - pixel map for sensor %d", i+1), 
									 pGeoMap->GetNPixelY(), 0, pGeoMap->GetNPixelY(), 
									 pGeoMap->GetNPixelX(), 0, pGeoMap->GetNPixelX());
		 fpHisPixelMap[i]->SetStats(kFALSE);
		 AddHistogram(fpHisPixelMap[i]);
	  }
	  fpHisRateMap[i] = new TH1F(Form("vtRateMap%d", i+1), Form("Vertex - rate per line for sensor %d", i+1), 
								 pGeoMap->GetNPixelX(), 0, pGeoMap->GetNPixelX());
	  AddHistogram(fpHisRateMap[i]);

	  fpHisRateMapQ[i] = new TH1F(Form("vtRateMapQ%d", i+1), Form("Vertex - rate per quadrant for sensor %d", i+1), 
								 10, 0, 5);
	  AddHistogram(fpHisRateMapQ[i]);
	  
	  fpHisEvtLength[i] = new TH1F(Form("vtEvtLength%d", i+1), Form("Vertex - event length sensor %d", i+1), 
								   1000, 0, 1000);
	  AddHistogram(fpHisEvtLength[i]);
   }
   
   SetValidHistogram(kTRUE);
   return;
}

// --------------------------------------------------------------------------------------
void TAVTactBaseRaw::AddPixel( Int_t iSensor, Int_t value, Int_t aLine, Int_t aColumn) 
{
   // Add a pixel to the vector of pixels
   // require the following info
   // - input = number of the sensors
   // - value = analog value of this pixel
   // - line & column = position of the pixel in the matrix
   
   TAVTntuRaw* pDatRaw = (TAVTntuRaw*) fpNtuRaw->Object();
   TAVTparGeo* pGeoMap = (TAVTparGeo*) fpGeoMap->Object();
   
   TAVTntuHit* pixel   = (TAVTntuHit*)pDatRaw->NewPixel(iSensor, value, aLine, aColumn);
   
   double v = pGeoMap->GetPositionV(aLine);
   double u = pGeoMap->GetPositionU(aColumn);
   TVector3 pos(u,v,0);
   pixel->SetPosition(pos);

   
   if (ValidHistogram()) {
	  if (TAVTparConf::IsMapHistOn()) 
		 fpHisPixelMap[iSensor]->Fill(aLine, aColumn);
	  
	  fpHisRateMap[iSensor]->Fill(aColumn);
	  
	  for (Int_t i = 0; i < 4; ++i) {
		 if (aColumn >= 258*i && aColumn < (i+1)*258)
			fpHisRateMapQ[iSensor]->Fill(i+1);
	  }
   }
}


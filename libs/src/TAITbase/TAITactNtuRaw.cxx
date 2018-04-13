/*!
 \file
 \version $Id: TAITactNtuRaw.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
 \brief   Implementation of TAITactNtuRaw.
 */
#include "TH2F.h"
#include "TClonesArray.h"

#include "TAITdatRaw.hxx"
#include "TAITparGeo.hxx"
#include "TAITparMap.hxx"
#include "TAITparConf.hxx"
#include "TAITntuRaw.hxx"
#include "TAITactNtuRaw.hxx"

/*!
 \class TAITactNtuRaw 
 \brief NTuplizer for vertex raw hits. **
 */

ClassImp(TAITactNtuRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAITactNtuRaw::TAITactNtuRaw(const char* name, TAGdataDsc* p_nturaw, TAGdataDsc* p_datraw,
							 TAGparaDsc* p_parmap, TAGparaDsc* p_geomap)
: TAGaction(name, "TAITactNtuRaw - NTuplize vertex raw data"),
  fpNtuRaw(p_nturaw),
  fpDatRaw(p_datraw),
  fpParMap(p_parmap),
  fpGeoMap(p_geomap)
{
   AddDataOut(p_nturaw, "TAITntuRaw");
   AddDataIn(p_datraw, "TAITdatRaw");
   AddPara(p_geomap, "TAITparGeo");
   AddPara(p_parmap, "TAITparMap");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAITactNtuRaw::~TAITactNtuRaw()
{}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAITactNtuRaw::CreateHistogram()
{
   // DeleteHistogram();
   // TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();
   // for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
	  // if (TAITparConf::IsMapHistOn()) {
		 // fpHisPosMap[i] = new TH2F(Form("vtPosMap%d", i+1), Form("Vertex - position map for sensor %d", i+1), 
			// 						 pGeoMap->GetNPixelY(), -pGeoMap->GetPitchY()/2*pGeoMap->GetNPixelY(), pGeoMap->GetPitchY()/2*pGeoMap->GetNPixelY(),
			// 						 pGeoMap->GetNPixelX(), -pGeoMap->GetPitchX()/2*pGeoMap->GetNPixelX(), pGeoMap->GetPitchX()/2*pGeoMap->GetNPixelX());
		 // fpHisPosMap[i]->SetStats(kFALSE);
		 // AddHistogram(fpHisPosMap[i]);
	  // }
   // }
   
   // SetValidHistogram(kTRUE);
   // return;
}

//------------------------------------------+-----------------------------------
//! Action.
Bool_t TAITactNtuRaw::Action()
{
   TAITdatRaw* pDatRaw = (TAITdatRaw*) fpDatRaw->Object();
   TAITntuRaw* pNtuRaw = (TAITntuRaw*) fpNtuRaw->Object();
   TAITparGeo* pGeoMap = (TAITparGeo*) fpGeoMap->Object();
   TAITparMap* pParMap = (TAITparMap*) fpParMap->Object();
   
   pNtuRaw->SetupClones();
   
   // for (Int_t iSensor = 0; iSensor < pGeoMap->GetSensorsN(); ++iSensor) {
	  // //Sort List of pixels
	  // TClonesArray* myList = pDatRaw->GetPixels(iSensor); 
	  // myList->Sort(myList->GetEntriesFast());
	  // DeleteDoublet(iSensor);
	  	  
	  // Int_t nPixels = pDatRaw->GetPixelsN(iSensor);
	  // for (Int_t iPixel = 0; iPixel < nPixels; ++iPixel) {
		 // TAITrawHit* rawPixel =  pDatRaw->GetPixel(iSensor, iPixel);
		 // TAITntuHit* pixel= pNtuRaw->NewPixel(iSensor, rawPixel);
		 // Float_t u = pParMap->GetPositionU(rawPixel->GetColumnNumber());
		 // Float_t v = pParMap->GetPositionV(rawPixel->GetLineNumber());
		 // Float_t w = 0;
		 // TVector3 pos(v,u,w);
		 // pixel->SetPosition(pos);
		 // if (ValidHistogram()) {
			// if (TAITparConf::IsMapHistOn()) 
			//    fpHisPosMap[iSensor]->Fill(v, u);
		 // }
	  // }
   // }
   
   fpNtuRaw->SetBit(kValid);
   return kTRUE;
}

//----------------------------------------------------------------
//! Delete the pixel that have the same line and the same column
void TAITactNtuRaw::DeleteDoublet(Int_t iSens)
{
   TAITdatRaw* pDatRaw = (TAITdatRaw*) fpDatRaw->Object();
   TClonesArray* myList = pDatRaw->GetPixels(iSens); 
   
   //loop on pixel
   for(int iPix = 0; iPix<myList->GetEntries(); iPix++) {
	  
	  if (myList->GetEntries() == 1)return;
	  
	  TAITrawHit* apixel = (TAITrawHit*)myList->At(iPix);
	  TAITrawHit* npixel = (iPix == myList->GetEntries()-1) ? (TAITrawHit*)myList->At(iPix-1) : (TAITrawHit*)myList->At(iPix+1);
	  
	  Int_t aline = apixel->GetLineNumber();
	  Int_t acolumn = apixel->GetColumnNumber();
	  Int_t nline = npixel->GetLineNumber();
	  Int_t ncolumn = npixel->GetColumnNumber();
	  
	  if (aline == nline && acolumn == ncolumn) {
		 myList->Remove(npixel);
		 myList->Compress();
	  }
   }
}

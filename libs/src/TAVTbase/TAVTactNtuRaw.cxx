/*!
 \file
 \version $Id: TAVTactNtuRaw.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
 \brief   Implementation of TAVTactNtuRaw.
 */
#include "TH2F.h"
#include "TClonesArray.h"

#include "TAVTdatRaw.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparMap.hxx"
#include "TAVTparConf.hxx"
#include "TAVTntuRaw.hxx"
#include "TAVTactNtuRaw.hxx"

/*!
 \class TAVTactNtuRaw 
 \brief NTuplizer for vertex raw hits. **
 */

ClassImp(TAVTactNtuRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAVTactNtuRaw::TAVTactNtuRaw(const char* name, TAGdataDsc* p_nturaw, TAGdataDsc* p_datraw,
							 TAGparaDsc* p_parmap, TAGparaDsc* p_geomap)
: TAGaction(name, "TAVTactNtuRaw - NTuplize vertex raw data"),
  fpNtuRaw(p_nturaw),
  fpDatRaw(p_datraw),
  fpParMap(p_parmap),
  fpGeoMap(p_geomap)
{
   AddDataOut(p_nturaw, "TAVTntuRaw");
   AddDataIn(p_datraw, "TAVTdatRaw");
   AddPara(p_geomap, "TAVTparGeo");
   AddPara(p_parmap, "TAVTparMap");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAVTactNtuRaw::~TAVTactNtuRaw()
{}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAVTactNtuRaw::CreateHistogram()
{
   // DeleteHistogram();
   // TAVTparGeo* pGeoMap  = (TAVTparGeo*) fpGeoMap->Object();
   // for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
	  // if (TAVTparConf::IsMapHistOn()) {
		 // fpHisPosMap[i] = new TH2F(Form("vtPosMap%d", i+1), Form("Vertex - position map for sensor %d", i+1), 
			// 						 pGeoMap->GetPixelsNv(), -pGeoMap->GetPitchV()/2*pGeoMap->GetPixelsNv(), pGeoMap->GetPitchV()/2*pGeoMap->GetPixelsNv(),
			// 						 pGeoMap->GetPixelsNu(), -pGeoMap->GetPitchU()/2*pGeoMap->GetPixelsNu(), pGeoMap->GetPitchU()/2*pGeoMap->GetPixelsNu());
		 // fpHisPosMap[i]->SetStats(kFALSE);
		 // AddHistogram(fpHisPosMap[i]);
	  // }
   // }
   
   // SetValidHistogram(kTRUE);
   return;
}

//------------------------------------------+-----------------------------------
//! Action.
Bool_t TAVTactNtuRaw::Action()
{
   TAVTdatRaw* pDatRaw = (TAVTdatRaw*) fpDatRaw->Object();
   TAVTntuRaw* pNtuRaw = (TAVTntuRaw*) fpNtuRaw->Object();
   TAVTparGeo* pGeoMap = (TAVTparGeo*) fpGeoMap->Object();
   TAVTparMap* pParMap = (TAVTparMap*) fpParMap->Object();
   
   pNtuRaw->SetupClones();
   
   // for (Int_t iSensor = 0; iSensor < pGeoMap->GetSensorsN(); ++iSensor) {
	  // //Sort List of pixels
	  // TClonesArray* myList = pDatRaw->GetPixels(iSensor); 
	  // myList->Sort(myList->GetEntriesFast());
	  // DeleteDoublet(iSensor);
	  	  
	  // Int_t nPixels = pDatRaw->GetPixelsN(iSensor);
	  // for (Int_t iPixel = 0; iPixel < nPixels; ++iPixel) {
		 // TAVTrawHit* rawPixel =  pDatRaw->GetPixel(iSensor, iPixel);
		 // TAVTntuHit* pixel= pNtuRaw->NewPixel(iSensor, rawPixel);
		 // Float_t u = pParMap->GetPositionU(rawPixel->GetColumnNumber());
		 // Float_t v = pParMap->GetPositionV(rawPixel->GetLineNumber());
		 // Float_t w = 0;
		 // TVector3 pos(v,u,w);
		 // pixel->SetPosition(pos);
		 // if (ValidHistogram()) {
			// if (TAVTparConf::IsMapHistOn()) 
			//    fpHisPosMap[iSensor]->Fill(v, u);
		 // }
	  // }
   // }
   
   fpNtuRaw->SetBit(kValid);
   return kTRUE;
}

//----------------------------------------------------------------
//! Delete the pixel that have the same line and the same column
void TAVTactNtuRaw::DeleteDoublet(Int_t iSens)
{
   TAVTdatRaw* pDatRaw = (TAVTdatRaw*) fpDatRaw->Object();
   TClonesArray* myList = pDatRaw->GetPixels(iSens); 
   
   //loop on pixel
   for(int iPix = 0; iPix<myList->GetEntries(); iPix++) {
	  
	  if (myList->GetEntries() == 1)return;
	  
	  TAVTrawHit* apixel = (TAVTrawHit*)myList->At(iPix);
	  TAVTrawHit* npixel = (iPix == myList->GetEntries()-1) ? (TAVTrawHit*)myList->At(iPix-1) : (TAVTrawHit*)myList->At(iPix+1);
	  
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

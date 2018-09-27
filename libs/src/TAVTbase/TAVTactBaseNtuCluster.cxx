/*!
 \file
 \version $Id: TAVTactBaseNtuCluster.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
 \brief   Implementation of TAVTactBaseNtuCluster.
 */
#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#include "TAVTdatRaw.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"
#include "TAVTntuRaw.hxx"
#include "TAVTntuCluster.hxx"
#include "TAVTactBaseNtuCluster.hxx"

/*!
 \class TAVTactBaseNtuCluster 
 \brief NTuplizer for vertex raw hits. **
 */

ClassImp(TAVTactBaseNtuCluster);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAVTactBaseNtuCluster::TAVTactBaseNtuCluster(const char* name, 
											 TAGparaDsc* pConfig, TAGparaDsc* pGeoMap)
: TAGaction(name, "TAVTactNtuCluster - NTuplize cluster"),
  fpConfig(pConfig),
  fpGeoMap(pGeoMap),
  fCurrentPosition(new TVector3(0., 0., 0.)), 
  fCurrentPosError(new TVector3(0., 0., 0.)), 
  fListOfPixels(0x0),
  fClustersN(0)
{
   AddPara(pGeoMap, "TAVTbaseParGeo");
   AddPara(pConfig, "TAVTbaseParConf");
   
   TString tmp(name);
   fPrefix = tmp(0,2);
   
   TAVTparGeo* geoMap = (TAVTparGeo*)fpGeoMap->Object();
   Int_t nLines = geoMap->GetNPixelY()+1;
   Int_t nCols  = geoMap->GetNPixelX()+1;
   fFlagMap.Set(nLines*nCols);
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTactBaseNtuCluster::~TAVTactBaseNtuCluster()
{
   delete fCurrentPosition;
   delete fCurrentPosError;
}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAVTactBaseNtuCluster::CreateHistogram()
{
   DeleteHistogram();
   fpHisPixelTot = new TH1F(Form("%sClusPixelTot", fPrefix.Data()), "Vertex - Total # pixels per clusters", 100, 0., 100.);
   AddHistogram(fpHisPixelTot);
   
   TAVTbaseParGeo* pGeoMap  = (TAVTbaseParGeo*) fpGeoMap->Object();
   
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
	  fpHisPixel[i] = new TH1F(Form("%sClusPixel%d",fPrefix.Data(), i+1), Form("Vertex - # pixels per clusters for sensor %d", i+1), 100, 0., 100.);
	  AddHistogram(fpHisPixel[i]);
   }
   
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
	  if (TAVTparConf::IsMapHistOn()) {
		 fpHisClusMap[i] = new TH2F(Form("%sClusMap%d", fPrefix.Data(), i+1), Form("Vertex - clusters map for sensor %d", i+1),
									100, -pGeoMap->GetPitchX()*pGeoMap->GetNPixelX()/2., pGeoMap->GetPitchY()*pGeoMap->GetNPixelX()/2.,
									100, -pGeoMap->GetPitchY()*pGeoMap->GetNPixelX()/2., pGeoMap->GetPitchY()*pGeoMap->GetNPixelX()/2.);
		 fpHisClusMap[i]->SetMarkerStyle(20);
		 fpHisClusMap[i]->SetMarkerSize(.6);
		 fpHisClusMap[i]->SetMarkerColor(1);
		 fpHisClusMap[i]->SetStats(kFALSE);
		 AddHistogram(fpHisClusMap[i]);
	  }
   }
   
   SetValidHistogram(kTRUE);
   return;
}

//______________________________________________________________________________
//
void TAVTactBaseNtuCluster::FillMaps(TAVTbaseParGeo* pGeoMap)
{
   Int_t nLine = pGeoMap->GetNPixelY()+1;
   Int_t nCol  = pGeoMap->GetNPixelX()+1;
   
   fPixelMap.clear();
   fIndexMap.clear();
   fFlagMap.Reset(-1);
   
   if (fListOfPixels->GetEntries() == 0) return;
   
   // fill maps for cluster
   for (Int_t i = 0; i < fListOfPixels->GetEntries(); i++) { // loop over hit pixels
      
      TAVTbaseNtuHit* pixel = (TAVTbaseNtuHit*)fListOfPixels->At(i);
      Int_t line = pixel->GetPixelLine();
      Int_t col  = pixel->GetPixelColumn();
      if (line >= nLine) continue;
      if (col  >= nCol)  continue;
      if (line < 0) continue;
      if (col  < 0)  continue;
      fPixelMap[line*nCol+col] = 1;
      fIndexMap[line*nCol+col] = i;
   }
}

//______________________________________________________________________________
//
void TAVTactBaseNtuCluster::SearchCluster(TAVTbaseParGeo* pGeoMap)
{
   Int_t nLine = pGeoMap->GetNPixelY()+1;
   Int_t nCol  = pGeoMap->GetNPixelX()+1;
   
   fClustersN = 0;
   // Search for cluster
   
   for (Int_t iPix = 0; iPix < fListOfPixels->GetEntries(); ++iPix) { // loop over hit pixels
      TAVTbaseNtuHit* pixel = (TAVTbaseNtuHit*)fListOfPixels->At(iPix);
      if (pixel->Found()) continue;
      Int_t line = pixel->GetPixelLine();
      Int_t col  = pixel->GetPixelColumn();
      if (line >= nLine) continue;
      if (col  >= nCol)  continue;
      if (line < 0) continue;
      if (col  < 0)  continue;
      
      // loop over lines & columns
      if ( ShapeCluster(fClustersN, line, col, pGeoMap) )
         fClustersN++;
   }
   
}

//______________________________________________________________________________
//
Bool_t TAVTactBaseNtuCluster::ShapeCluster(Int_t noClus, Int_t IndX, Int_t IndY, TAVTbaseParGeo* pGeoMap)
{
   
   Int_t nLine = pGeoMap->GetNPixelY()+1;
   Int_t nCol  = pGeoMap->GetNPixelX()+1;
   
   if ( fPixelMap[IndX*nCol+IndY] <= 0 ) return false;
   if ( fFlagMap[IndX*nCol+IndY] != -1 ) return false;
   fFlagMap[IndX*nCol+IndY] = noClus;
   
   TAVTbaseNtuHit* pixel = (TAVTbaseNtuHit*)GetListOfPixels()->At(fIndexMap[IndX*nCol+IndY]);
   pixel->SetFound(true);
   
   for(Int_t i = -1; i <= 1 ; ++i)
      if ( IndX+i >= 0 && IndX+i < nLine)
         ShapeCluster(noClus, IndX+i, IndY, pGeoMap);
   
   for(Int_t j = -1; j <= 1 ; ++j)
      if ( IndY+j >= 0 && IndY+j < nCol)
         ShapeCluster(noClus, IndX  , IndY+j, pGeoMap);
   
   return true;
}

//______________________________________________________________________________
//
Bool_t TAVTactBaseNtuCluster::ApplyCuts(TAVTbaseCluster* cluster)
{
   TAVTbaseParConf* pConfig = (TAVTbaseParConf*) fpConfig->Object();
   
   TClonesArray* list = cluster->GetListOfPixels();
   Int_t  entries = list->GetEntries();
   
   // cuts on pixels in cluster
   if(entries < pConfig->GetSensorPar(cluster->GetPlaneNumber()).MinNofPixelsInCluster ||
      entries > pConfig->GetSensorPar(cluster->GetPlaneNumber()).MaxNofPixelsInCluster)
      return kFALSE;
   
   return kTRUE;
}

//______________________________________________________________________________
//
void TAVTactBaseNtuCluster::ComputePosition()
{
   ComputeCoGPosition();
}

//______________________________________________________________________________
//
void TAVTactBaseNtuCluster::ComputeSeedPosition()
{
   GetCurrentPosition()->SetXYZ((fPSeed->GetPosition())(0), (fPSeed->GetPosition())(1), 0);   
}

//______________________________________________________________________________
//
void TAVTactBaseNtuCluster::ComputeCoGPosition()
{
   if (!fCurListOfPixels) return;
   
   TVector3 tCorrection, tCorrection2, tCorTemp;
   TVector3 pos, posErr;
   tCorrection.SetXYZ( 0., 0., 0.);
   tCorrection2.SetXYZ( 0., 0., 0.);
   
   Float_t tClusterPulseSum = 0.;
   
   
   for (Int_t i = 0; i < fCurListOfPixels->GetEntries(); ++i) {
      TAVTntuHit* pixel = (TAVTntuHit*)fCurListOfPixels->At(i);
      tCorTemp.SetXYZ(pixel->GetPosition()(0)*pixel->GetPulseHeight(), pixel->GetPosition()(1)*pixel->GetPulseHeight(), pixel->GetPosition()(2));
      tCorrection  += tCorTemp;
      tClusterPulseSum  += pixel->GetPulseHeight();
	  }
   
   pos = tCorrection*(1./tClusterPulseSum);
   
   for (Int_t i = 0; i < fCurListOfPixels->GetEntries(); ++i) {
	  TAVTntuHit* pixel = (TAVTntuHit*)fCurListOfPixels->At(i);
	  tCorrection2.SetXYZ(pixel->GetPulseHeight()*(pixel->GetPosition()(0)-(pos)(0))*(pixel->GetPosition()(0)-(pos)(0)), 
							pixel->GetPulseHeight()*(pixel->GetPosition()(1)-(pos)(1))*(pixel->GetPosition()(1)-(pos)(1)), 
							0);
	  posErr += tCorrection2;
   }
   
   posErr *= 1./tClusterPulseSum;
   
   // for cluster with a single pixel
   Float_t lim = 2.5e-3; // in cm !
   if (posErr(0) < lim) posErr(0) = lim; //(20/Sqrt(12)^2
   if (posErr(1) < lim) posErr(1) = lim; //(20/Sqrt(12)^2
   
   GetCurrentPosition()->SetXYZ((pos)(0), (pos)(1), 0);  
   GetCurrentPosError()->SetXYZ(TMath::Sqrt((posErr)(0)), TMath::Sqrt((posErr)(1)), 0);
}


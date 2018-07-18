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
#include "TAVTparMap.hxx"
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
											 TAGdataDsc* pNtuRaw, TAGdataDsc* pNtuClus,
											 TAGparaDsc* pConfig, TAGparaDsc* pGeoMap)
: TAGaction(name, "TAVTactNtuCluster - NTuplize cluster"),
  fpNtuRaw(pNtuRaw),
  fpNtuClus(pNtuClus),
  fpConfig(pConfig),
  fpGeoMap(pGeoMap),
  fCurrentPosition(new TVector3(0., 0., 0.)), 
  fCurrentPosError(new TVector3(0., 0., 0.)), 
  fListOfPixels(0x0),
  fDebugLevel(0)
{
   AddDataIn(pNtuRaw,   "TAVTntuRaw");
   AddDataOut(pNtuClus, "TAVTntuCluster");
   AddPara(pGeoMap, "TAVTparGeo");
   AddPara(pConfig, "TAVTparConf");
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
   fpHisPixelTot = new TH1F("vtClusPixelTot", "Vertex - Total # pixels per clusters", 100, 0., 100.);
   AddHistogram(fpHisPixelTot);
   
   TAVTparGeo* pGeoMap  = (TAVTparGeo*) fpGeoMap->Object();
   
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
	  fpHisPixel[i] = new TH1F(Form("vtClusPixel%d", i+1), Form("Vertex - # pixels per clusters for sensor %d", i+1), 100, 0., 100.);
	  AddHistogram(fpHisPixel[i]);
   }
   
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
	  if (TAVTparConf::IsMapHistOn()) {
		 fpHisClusMap[i] = new TH2F(Form("vtClusMap%d", i+1), Form("Vertex - clusters map for sensor %d", i+1), 
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
Bool_t TAVTactBaseNtuCluster::ApplyCuts(TAVTcluster* cluster)
{
   TAVTparConf* pConfig = (TAVTparConf*) fpConfig->Object();
   
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
Bool_t TAVTactBaseNtuCluster::Action()
{
   TAVTntuRaw* pNtuHit  = (TAVTntuRaw*) fpNtuRaw->Object();
   TAVTparConf* pConfig = (TAVTparConf*) fpConfig->Object();
   
   Bool_t ok = true;
   
   for (Int_t i = 0; i < pConfig->GetSensorsN(); ++i) {
	  fListOfPixels = pNtuHit->GetListOfPixels(i);
	  if (fListOfPixels->GetEntries() > pConfig->GetAnalysisPar().HitsInPlaneMaximum) continue; 
	  if (fListOfPixels->GetEntries() == 0) continue; 
	  ok += FindClusters(i);
   }
   
   if(ok)
	  fpNtuClus->SetBit(kValid);
   return ok;
}

//______________________________________________________________________________
//
void TAVTactBaseNtuCluster::ComputePosition()
{
   TAVTparConf* pConfig = (TAVTparConf*) fpConfig->Object();

   Int_t positionAlgorithm = pConfig->GetAnalysisPar().PosAlgo % 100;
   switch (positionAlgorithm) {
	  case 0:
		 ComputeSeedPosition();
		 break;
	  case 1:
	  case 11:
	  case 12:
		 ComputeCoGPosition();
		 break;
   }
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
   if (posErr(0) < 25) posErr(0) = 25; //(18/Sqrt(12)^2
   if (posErr(1) < 25) posErr(1) = 25; //(18/Sqrt(12)^2
   
   GetCurrentPosition()->SetXYZ((pos)(0), (pos)(1), 0);  
   GetCurrentPosError()->SetXYZ(TMath::Sqrt((posErr)(0)), TMath::Sqrt((posErr)(1)), 0);
}


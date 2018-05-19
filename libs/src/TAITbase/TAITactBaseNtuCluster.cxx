/*!
 \file
 \version $Id: TAITactBaseNtuCluster.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
 \brief   Implementation of TAITactBaseNtuCluster.
 */
#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#include "TAITdatRaw.hxx"
#include "TAITparGeo.hxx"
#include "TAITparMap.hxx"
#include "TAITparConf.hxx"
#include "TAITntuRaw.hxx"
#include "TAITntuCluster.hxx"
#include "TAITactBaseNtuCluster.hxx"

/*!
 \class TAITactBaseNtuCluster 
 \brief NTuplizer for vertex raw hits. **
 */

ClassImp(TAITactBaseNtuCluster);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAITactBaseNtuCluster::TAITactBaseNtuCluster(const char* name, 
											 TAGdataDsc* pNtuRaw, TAGdataDsc* pNtuClus,
											 TAGparaDsc* pConfig, TAGparaDsc* pGeoMap, string command)
: TAGaction(name, "TAITactNtuCluster - NTuplize cluster"),
  fpNtuRaw(pNtuRaw),
  fpNtuClus(pNtuClus),
  fpConfig(pConfig),
  fpGeoMap(pGeoMap),
  fCurrentPosition(TVector3(0., 0., 0.)), 
  fCurrentPosError(TVector3(0., 0., 0.)), 
  // fListOfPixels(0x0),
  fDebugLevel(0)
{
   AddDataIn(pNtuRaw,   "TAITntuRaw");
   AddDataOut(pNtuClus, "TAITntuCluster");
   AddPara(pGeoMap, "TAITparGeo");
   AddPara(pConfig, "TAITparConf");

   m_command = command;
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITactBaseNtuCluster::~TAITactBaseNtuCluster()
{
   // delete fCurrentPosition;
   // delete fCurrentPosError;
}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAITactBaseNtuCluster::CreateHistogram()
{
   // DeleteHistogram();
   // fpHisPixelTot = new TH1F("itClusPixelTot", "Vertex - Total # pixels per clusters", 100, 0., 100.);
   // AddHistogram(fpHisPixelTot);
   
   // TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();
   
   // for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
	  // fpHisPixel[i] = new TH1F(Form("itClusPixel%d", i+1), Form("Vertex - # pixels per clusters for sensor %d", i+1), 100, 0., 100.);
	  // AddHistogram(fpHisPixel[i]);
   // }
   
   // for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
	  // if (TAITparConf::IsMapHistOn()) {
		 // fpHisClusMap[i] = new TH2F(Form("itClusMap%d", i+1), Form("Vertex - clusters map for sensor %d", i+1), 
			// 						100, -pGeoMap->GetPitchX()*pGeoMap->GetNPixelX()/2., pGeoMap->GetPitchY()*pGeoMap->GetNPixelX()/2.,
			// 						100, -pGeoMap->GetPitchY()*pGeoMap->GetNPixelX()/2., pGeoMap->GetPitchY()*pGeoMap->GetNPixelX()/2.);
		 // fpHisClusMap[i]->SetMarkerStyle(20);
		 // fpHisClusMap[i]->SetMarkerSize(.6);
		 // fpHisClusMap[i]->SetMarkerColor(1);
		 // fpHisClusMap[i]->SetStats(kFALSE);
		 // AddHistogram(fpHisClusMap[i]);
	  // }
   // }
   
   SetValidHistogram(kTRUE);
   return;
}

//______________________________________________________________________________
//  
Bool_t TAITactBaseNtuCluster::ApplyCuts(TAITcluster* cluster)
{
   // TAITparConf* pConfig = (TAITparConf*) fpConfig->Object();
   
   // TClonesArray* list = cluster->GetListOfPixels();
   // Int_t  entries = list->GetEntries();
   
   // cuts on pixels in cluster
   // if(entries < pConfig->GetSensorPar(cluster->GetPlaneNumber()).MinNofPixelsInCluster || 
	  // entries > pConfig->GetSensorPar(cluster->GetPlaneNumber()).MaxNofPixelsInCluster)
	  // return kFALSE;
      
   return kTRUE;
}

//______________________________________________________________________________
//  
Bool_t TAITactBaseNtuCluster::Action()  {

    if ( GlobalPar::GetPar()->Debug() > 0 )         
        cout << "\tTAITactBaseNtuCluster::Action start " << endl;


    TAITntuRaw* pNtuHit  = (TAITntuRaw*) fpNtuRaw->Object();
    // TAITparConf* pConfig = (TAITparConf*) fpConfig->Object();

    Bool_t ok = true;

    for (Int_t i = 0; i < pNtuHit->GetNSensors(); ++i) {
        // fListOfPixels = pNtuHit->GetListOfPixels(i);
        // if (fListOfPixels->GetEntries() > pConfig->GetAnalysisPar().HitsInPlaneMaximum) continue;    // old
        if ( pNtuHit->GetListOfPixels(i)->GetEntries() == 0 ) continue; 
        ok += FindClusters(i);
    }

    if(ok)    fpNtuClus->SetBit(kValid);
    return ok;
}

//______________________________________________________________________________
//
void TAITactBaseNtuCluster::ComputePosition()   {
   // TAITparConf* pConfig = (TAITparConf*) fpConfig->Object();

   // Int_t positionAlgorithm = pConfig->GetAnalysisPar().PosAlgo % 100;
   // switch (positionAlgorithm) {
	  // case 0:
		 // ComputeSeedPosition();
		 // break;
	  // case 1:
	  // case 11:
	  // case 12:
		 ComputeCoGPosition();
		 // break;
   // }
}

// //______________________________________________________________________________
// //
// void TAITactBaseNtuCluster::ComputeSeedPosition()
// {
//     // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//    fCurrentPosition.SetXYZ((fPSeed->GetPosition())(0), (fPSeed->GetPosition())(1), 0);    
// }

//______________________________________________________________________________
// compute position in the detector local frame
void TAITactBaseNtuCluster::ComputeCoGPosition() {

    // fCurListOfPixels = pixels of the current cluster
    if (!fCurListOfPixels) return;   

    // TAITparConf* pConfig = (TAITparConf*) fpConfig->Object();

    // Int_t positionAlgorithm = pConfig->GetAnalysisPar().PosAlgo % 100;

    TVector3 tCorrection, tCorrection2, tCorTemp;
    TVector3 pos, posErr;
    tCorrection.SetXYZ( 0., 0., 0.);
    tCorrection2.SetXYZ( 0., 0., 0.);

    Float_t tClusterPulseSum = 0.;

    // GetPulseHeight = 1 by default (at least in MC)

    // center of mass
    // if (positionAlgorithm == 1) {
    for (Int_t i = 0; i < fCurListOfPixels->GetEntries(); ++i) {
        TAITntuHit* pixel = (TAITntuHit*)fCurListOfPixels->At(i);
        tCorTemp.SetXYZ( pixel->GetPixelPosition_detectorFrame().X()*pixel->GetPulseHeight(), pixel->GetPixelPosition_detectorFrame().Y()*pixel->GetPulseHeight(), pixel->GetPixelPosition_detectorFrame().Z() );
        tCorrection  += tCorTemp;    // sum of distance vectors
        tClusterPulseSum  += pixel->GetPulseHeight();    // num of cluster
    }
	  
   //    // useless
   // } 
   // else if (positionAlgorithm == 11) { // 3*3 pixels clusters
	  
	  // TAITntuHit* pixelSeed = (TAITntuHit*)fCurListOfPixels->At(0);
	  // for (Int_t i = 0; i < fCurListOfPixels->GetEntries(); ++i) {
		 // TAITntuHit* pixel = (TAITntuHit*)fCurListOfPixels->At(i);
		 
		 // if( TMath::Abs(pixelSeed->DistanceU(pixel->GetPosition())) <= pixelSeed->GetSize()(0) && 
			// TMath::Abs(pixelSeed->DistanceV(pixel->GetPosition())) <= pixelSeed->GetSize()(1) ) {
			
			// tCorTemp.SetXYZ(pixel->GetPosition()(0)*pixel->GetPulseHeight(), pixel->GetPosition()(1)*pixel->GetPulseHeight(), pixel->GetPosition()(2));
			// tCorrection  += tCorTemp;
			// tClusterPulseSum += pixel->GetPulseHeight();
		 // }
	  // }
	  
   //    // useless
   // } else if (positionAlgorithm == 12) { // 2*2 pixels clusters
	  
	  // for (Int_t i = 0; i < TMath::Min( 4, fCurListOfPixels->GetEntries()); ++i) {
		 // TAITntuHit* pixel = (TAITntuHit*)fCurListOfPixels->At(i);
		 // tCorTemp.SetXYZ(pixel->GetPosition()(0)*pixel->GetPulseHeight(), pixel->GetPosition()(1)*pixel->GetPulseHeight(), pixel->GetPosition()(2));
		 // tCorrection  += tCorTemp;
		 // tClusterPulseSum += pixel->GetPulseHeight();
	  // }
   // }
   
   // make the (weighted) center of mass
   pos = tCorrection*(1./tClusterPulseSum);

   // evaluate uncertainty... does not propagate uncertainties... why?
   for (Int_t i = 0; i < fCurListOfPixels->GetEntries(); ++i) {
	  TAITntuHit* pixel = (TAITntuHit*)fCurListOfPixels->At(i);
	  tCorrection2.SetXYZ(  pixel->GetPulseHeight() * (pixel->GetPixelPosition_detectorFrame().X()-pos.X()) * (pixel->GetPixelPosition_detectorFrame().X()-pos.X()), 
							pixel->GetPulseHeight() * (pixel->GetPixelPosition_detectorFrame().Y()-pos.Y()) * (pixel->GetPixelPosition_detectorFrame().Y()-pos.Y()), 
							pixel->GetPixelPosition_detectorFrame().Z());
	  posErr += tCorrection2;
   }
   
   posErr *= 1./tClusterPulseSum;
   
   // for cluster with a single pixel
   if (posErr(0) < 25) posErr(0) = 25; //(18/Sqrt(12)^2
   if (posErr(1) < 25) posErr(1) = 25; //(18/Sqrt(12)^2
   
   fCurrentPosition = pos;  
   fCurrentPosError.SetXYZ(TMath::Sqrt(posErr.X()), TMath::Sqrt(posErr.Y()), 0);

   // return local position

}













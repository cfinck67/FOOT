/*!
 \file
 \version $Id: TAMSDactNtuCluster.cxx $
 \brief   Implementation of TAMSDactNtuCluster.
 */
#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#include "TAVTdatRaw.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"
#include "TAMSDntuRaw.hxx"
#include "TAMSDntuRaw.hxx"
#include "TAMSDntuCluster.hxx"
#include "TAMSDactNtuCluster.hxx"

/*!
 \class TAMSDactNtuCluster 
 \brief NTuplizer for vertex raw hits. **
 */

ClassImp(TAMSDactNtuCluster);

//------------------------------------------+-----------------------------------
//! Default constructor.
TAMSDactNtuCluster::TAMSDactNtuCluster(const char* name, 
									 TAGdataDsc* pNtuRaw, TAGdataDsc* pNtuClus,
									 TAGparaDsc* pConfig, TAGparaDsc* pGeoMap)
 : TAGaction(name, "TAMSDactNtuCluster - NTuplize cluster"),
   fpNtuRaw(pNtuRaw),
   fpNtuClus(pNtuClus),
   fpConfig(pConfig),
   fpGeoMap(pGeoMap),
   fCurrentPosition(0.),
   fCurrentPosError(0.),
   fListOfStrips(0x0),
   fClustersN(0)
{
   AddPara(pGeoMap, "TAVTbaseParGeo");
   AddPara(pConfig, "TAVTbaseParConf");
   AddDataIn(pNtuRaw,   "TAMSDntuRaw");
   AddDataOut(pNtuClus, "TAMSDntuCluster");
   
   TAVTparGeo* geoMap = (TAVTparGeo*)fpGeoMap->Object();
   Int_t strips = geoMap->GetNPixelY()+1;
   fFlagMap.Set(strips);
}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAMSDactNtuCluster::CreateHistogram()
{
   DeleteHistogram();
   
   TString prefix = "ms";
   TString titleDev = "Multi Strip Detector";
   
   fpHisStripTot = new TH1F(Form("%sClusPixelTot", prefix.Data()), Form("%s - Total # strips per clusters", titleDev.Data()), 100, 0., 100.);
   AddHistogram(fpHisStripTot);
   
   TAVTbaseParGeo* pGeoMap  = (TAVTbaseParGeo*) fpGeoMap->Object();
   
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
      fpHisStrip[i] = new TH1F(Form("%sClusPixel%d",prefix.Data(), i+1), Form("%s - # strips per clusters for sensor %d", titleDev.Data(), i+1), 100, 0., 100.);
      AddHistogram(fpHisStrip[i]);
   }
   
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
         fpHisClusMap[i] = new TH1F(Form("%sClusMap%d", prefix.Data(), i+1), Form("%s - clusters map for sensor %d", titleDev.Data(), i+1),
                                    pGeoMap->GetNPixelX(), -pGeoMap->GetPitchX()*pGeoMap->GetNPixelX()/2., pGeoMap->GetPitchY()*pGeoMap->GetNPixelX()/2.);
    
      AddHistogram(fpHisClusMap[i]);
   }
   
   SetValidHistogram(kTRUE);
   return;
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAMSDactNtuCluster::~TAMSDactNtuCluster()
{
}

//______________________________________________________________________________
//
Bool_t TAMSDactNtuCluster::Action()
{
   TAMSDntuRaw* pNtuHit  = (TAMSDntuRaw*) fpNtuRaw->Object();
   TAMSDparGeo* pGeoMap  = (TAMSDparGeo*) fpGeoMap->Object();
   Bool_t ok = true;
   
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
      fListOfStrips = pNtuHit->GetListOfStrips(i);
      //if (fListOfStrips->GetEntries() > pConfig->GetAnalysisPar().HitsInPlaneMaximum) continue; // no config
      if (fListOfStrips->GetEntries() == 0) continue;
      ok += FindClusters(i);
   }
   
   if(ok)
      fpNtuClus->SetBit(kValid);
   return ok;
}

//______________________________________________________________________________
//  
Bool_t TAMSDactNtuCluster::FindClusters(Int_t iSensor)
{
   // Algo taking from Virgile BEKAERT (ImaBio @ IPHC-Strasbourg)
   // Look in a iterative way to next neighbour
   
   TAMSDntuCluster* pNtuClus = (TAMSDntuCluster*) fpNtuClus->Object();
   TAVTbaseParGeo* pGeoMap  = (TAVTbaseParGeo*)     fpGeoMap->Object();

   FillMaps(pGeoMap);
   SearchCluster(pGeoMap);
 
   return CreateClusters(iSensor, pNtuClus, pGeoMap);
}

//______________________________________________________________________________
//
void TAMSDactNtuCluster::SearchCluster(TAVTbaseParGeo* pGeoMap)
{
   Int_t nStrip  = pGeoMap->GetNPixelX()+1;
   
   fClustersN = 0;
   // Search for cluster
   
   for (Int_t iPix = 0; iPix < fListOfStrips->GetEntries(); ++iPix) { // loop over hit strips
      TAMSDntuHit* strip = (TAMSDntuHit*)fListOfStrips->At(iPix);
      if (strip->Found()) continue;
      
      Int_t stripId  = strip->GetStrip();
      if (stripId >= nStrip) continue;
      if (stripId < 0) continue;
      
      // loop over lines & columns
      if ( ShapeCluster(fClustersN, stripId, pGeoMap) )
         fClustersN++;
   }
}

//______________________________________________________________________________
//
void TAMSDactNtuCluster::FillMaps(TAVTbaseParGeo* pGeoMap)
{
   Int_t nStrip  = pGeoMap->GetNPixelX()+1;
   
   fStripMap.clear();
   fIndexMap.clear();
   fFlagMap.Reset(-1);
   
   if (fListOfStrips->GetEntries() == 0) return;
   
   // fill maps for cluster
   for (Int_t i = 0; i < fListOfStrips->GetEntries(); i++) { // loop over hit strips
      
      TAMSDntuHit* strip = (TAMSDntuHit*)fListOfStrips->At(i);
      Int_t stripId  = strip->GetStrip();
      if (stripId >= nStrip) continue;
      if (stripId < 0) continue;
      fStripMap[stripId] = 1;
      fIndexMap[stripId] = i;
   }
}

//______________________________________________________________________________
//
Bool_t TAMSDactNtuCluster::CreateClusters(Int_t iSensor, TAMSDntuCluster* pNtuClus, TAVTbaseParGeo* pGeoMap)
{
   Int_t nStrip = pGeoMap->GetNPixelX()+1;
   
   TAMSDcluster* cluster = 0x0;

   // create clusters
   for (Int_t i = 0; i< fClustersN; ++i)
      pNtuClus->NewCluster(iSensor);
   
   for (Int_t iPix = 0; iPix < fListOfStrips->GetEntries(); ++iPix) {
      TAMSDntuHit* strip = (TAMSDntuHit*)fListOfStrips->At(iPix);
      Int_t stripId = strip->GetStrip();
      if(stripId >= nStrip) continue;
      if( nStrip < 0) continue;
      
      Int_t clusterN = fFlagMap[stripId];
      if ( clusterN != -1 ) {
         cluster = pNtuClus->GetCluster(iSensor, clusterN);
         cluster->AddPixel(strip);
      }
   }
   
   // Compute position and fill clusters info
   for (Int_t i = 0; i< pNtuClus->GetClustersN(iSensor); ++i) {
      cluster = pNtuClus->GetCluster(iSensor, i);
      cluster->SetPlaneNumber(iSensor);
      fCurListOfStrips = cluster->GetListOfStrips();
      ComputePosition();
      cluster->SetNumber(i);
      
      TVector3 posG(GetCurrentPosition(), 0, 0);
      posG = pGeoMap->Sensor2Detector(iSensor, posG);
      //  pGeoMap->Local2Global(&posG);
      cluster->SetPositionG(&posG);
      cluster->SetPosition(GetCurrentPosition());
      cluster->SetPosError(GetCurrentPosError());
      
      if (ApplyCuts(cluster)) {
         // histogramms
         if (ValidHistogram()) {
            if (cluster->GetStripsN() > 0) {
               fpHisStripTot->Fill(cluster->GetStripsN());
               fpHisStrip[iSensor]->Fill(cluster->GetStripsN());
               // printf("sensor %d %d\n", iSensor, cluster->GetPixelsN());
               if (TAVTparConf::IsMapHistOn()) {
                  fpHisClusMap[iSensor]->Fill(cluster->GetPosition());
               }
            }
         }
      } else {
         pNtuClus->GetListOfClusters(iSensor)->Remove(cluster);
         pNtuClus->GetListOfClusters(iSensor)->Compress();
      }
   }
   if (pNtuClus->GetClustersN(iSensor))
      return true;
   
   return false;

}

//______________________________________________________________________________
//  
Bool_t TAMSDactNtuCluster::ShapeCluster(Int_t noClus, Int_t Ind, TAVTbaseParGeo* pGeoMap)
{

   Int_t nStrip = pGeoMap->GetNPixelX()+1;

   if ( fStripMap[Ind] <= 0 ) return false;
    if ( fFlagMap[Ind] != -1 ) return false;
   fFlagMap[Ind] = noClus;

   TAMSDntuHit* strip = (TAMSDntuHit*)GetListOfStrips()->At(fIndexMap[Ind]);
   strip->SetFound(true);
      
   for(Int_t i = -1; i <= 1 ; ++i)
	  if ( Ind+i >= 0 && Ind+i < nStrip)
		 ShapeCluster(noClus, Ind+i, pGeoMap);
   
   return true;
}

//______________________________________________________________________________
//
void TAMSDactNtuCluster::ComputePosition()
{
   if (!fCurListOfStrips) return;
   
   Float_t tCorrection, tCorrection2, tCorTemp;
   Float_t pos, posErr;
   tCorrection = 0.;
   tCorrection2 = 0.;
   
   Float_t tClusterPulseSum = 0.;
   
   for (Int_t i = 0; i < fCurListOfStrips->GetEntries(); ++i) {
      TAMSDntuHit* strip = (TAMSDntuHit*)fCurListOfStrips->At(i);
      tCorTemp = strip->GetPosition()*strip->GetValue();
      tCorrection  += tCorTemp;
      tClusterPulseSum  += strip->GetValue();
	  }
   
   pos = tCorrection*(1./tClusterPulseSum);
   
   for (Int_t i = 0; i < fCurListOfStrips->GetEntries(); ++i) {
      TAMSDntuHit* strip = (TAMSDntuHit*)fCurListOfStrips->At(i);
      tCorrection2 = strip->GetValue()*(strip->GetPosition()-pos)*(strip->GetPosition()-pos);
      posErr += tCorrection2;
   }
   
   posErr *= 1./tClusterPulseSum;
   
   // for cluster with a single strip
   Float_t lim = 2.5e-3; // in cm !
   if (posErr < lim) posErr = lim; //(20/Sqrt(12)^2
   
   fCurrentPosition = pos;
   fCurrentPosError = posErr;
}

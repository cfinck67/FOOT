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
: TAVTactBaseNtuCluster(name, pConfig, pGeoMap),
  fpNtuRaw(pNtuRaw),
  fpNtuClus(pNtuClus)
{
   AddDataIn(pNtuRaw,   "TAMSDntuRaw");
   AddDataOut(pNtuClus, "TAMSDntuCluster");
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
      fListOfPixels = pNtuHit->GetListOfPixels(i);
      //if (fListOfPixels->GetEntries() > pConfig->GetAnalysisPar().HitsInPlaneMaximum) continue; // no config
      if (fListOfPixels->GetEntries() == 0) continue;
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
Bool_t TAMSDactNtuCluster::CreateClusters(Int_t iSensor, TAMSDntuCluster* pNtuClus, TAVTbaseParGeo* pGeoMap)
{
   Int_t nLine = pGeoMap->GetNPixelY()+1;
   Int_t nCol  = pGeoMap->GetNPixelX()+1;
   
   TAMSDcluster* cluster = 0x0;

   // create clusters
   for (Int_t i = 0; i< fClustersN; ++i)
      pNtuClus->NewCluster(iSensor);
   
   for (Int_t iPix = 0; iPix < fListOfPixels->GetEntries(); ++iPix) {
      TAMSDntuHit* pixel = (TAMSDntuHit*)fListOfPixels->At(iPix);
      Int_t line = pixel->GetPixelLine();
      Int_t col  = pixel->GetPixelColumn();
      if(line >= nLine) continue;
      if(col >= nCol) continue;
      if( line < 0) continue;
      if( col < 0) continue;
      
      Int_t clusterN = fFlagMap[line*nCol+col];
      if ( clusterN != -1 ) {
         cluster = pNtuClus->GetCluster(iSensor, clusterN);
         cluster->AddPixel(pixel);
      }
   }
   
   // Compute position and fill clusters info
   for (Int_t i = 0; i< pNtuClus->GetClustersN(iSensor); ++i) {
      cluster = pNtuClus->GetCluster(iSensor, i);
      cluster->SetPlaneNumber(iSensor);
      fPSeed = cluster->GetPixel(0);
      fCurListOfPixels = cluster->GetListOfPixels();
      ComputePosition();
      cluster->SetNumber(i);
      TVector3 posG = *GetCurrentPosition();
      pGeoMap->Sensor2Detector_frame(iSensor, &posG);
      //  pGeoMap->Local2Global(&posG);
      cluster->SetPositionG(&posG);
      cluster->SetPosition(GetCurrentPosition());
      cluster->SetPosError(GetCurrentPosError());
      cluster->SetIndexSeed(fPSeed->GetPixelIndex());
      
      if (ApplyCuts(cluster)) {
         // histogramms
         if (ValidHistogram()) {
            if (cluster->GetPixelsN() > 0) {
               fpHisPixelTot->Fill(cluster->GetPixelsN());
               fpHisPixel[iSensor]->Fill(cluster->GetPixelsN());
               // printf("sensor %d %d\n", iSensor, cluster->GetPixelsN());
               if (TAVTparConf::IsMapHistOn()) {
                  fpHisClusMap[iSensor]->Fill(cluster->GetPosition()[0], cluster->GetPosition()[1]);
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
Bool_t TAMSDactNtuCluster::ShapeCluster(Int_t noClus, Int_t IndX, Int_t IndY, TAVTbaseParGeo* pGeoMap)
{
   Int_t nLine = pGeoMap->GetNPixelY()+1;
   Int_t nCol  = pGeoMap->GetNPixelX()+1;

   if ( fPixelMap[IndX*nCol+IndY] <= 0 ) return false;
    if ( fFlagMap[IndX*nCol+IndY] != -1 ) return false;
   fFlagMap[IndX*nCol+IndY] = noClus;

   TAVTntuHit* pixel = (TAVTntuHit*)GetListOfPixels()->At(fIndexMap[IndX*nCol+IndY]);
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
void TAMSDactNtuCluster::ComputePosition()
{
   TAVTactBaseNtuCluster::ComputePosition();
}

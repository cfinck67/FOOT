/*!
 \file
 \version $Id: TAITactNtuClusterF.cxx $
 \brief   Implementation of TAITactNtuClusterF.
 */
#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#include "TAVTdatRaw.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"
#include "TAITntuRaw.hxx"
#include "TAITntuRaw.hxx"
#include "TAITntuCluster.hxx"
#include "TAITactNtuClusterF.hxx"

/*!
 \class TAITactNtuClusterF 
 \brief NTuplizer for vertex raw hits. **
 */

ClassImp(TAITactNtuClusterF);

//------------------------------------------+-----------------------------------
//! Default constructor.
TAITactNtuClusterF::TAITactNtuClusterF(const char* name, 
									 TAGdataDsc* pNtuRaw, TAGdataDsc* pNtuClus,
									 TAGparaDsc* pConfig, TAGparaDsc* pGeoMap)
: TAVTactBaseNtuCluster(name, pConfig, pGeoMap),
  fpNtuRaw(pNtuRaw),
  fpNtuClus(pNtuClus)
{
   AddDataIn(pNtuRaw,   "TAITntuRaw");
   AddDataOut(pNtuClus, "TAITntuCluster");
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITactNtuClusterF::~TAITactNtuClusterF()
{
   
}

//______________________________________________________________________________
//
Bool_t TAITactNtuClusterF::Action()
{
   TAITntuRaw* pNtuHit  = (TAITntuRaw*) fpNtuRaw->Object();
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
Bool_t TAITactNtuClusterF::FindClusters(Int_t iSensor)
{
   // Algo taking from Virgile BEKAERT (ImaBio @ IPHC-Strasbourg)
   // Look in a iterative way to next neighbour
   
   TAITntuCluster* pNtuClus = (TAITntuCluster*) fpNtuClus->Object();
   TAITparGeo*     pGeoMap  = (TAITparGeo*)     fpGeoMap->Object();

   FillMaps(pGeoMap);
   SearchCluster(pGeoMap);
 
   return CreateClusters(iSensor, pNtuClus, pGeoMap);
}

//______________________________________________________________________________
//
Bool_t TAITactNtuClusterF::CreateClusters(Int_t iSensor, TAITntuCluster* pNtuClus, TAVTparGeo* pGeoMap)
{
   Int_t nLine = pGeoMap->GetNPixelY()+1;
   Int_t nCol  = pGeoMap->GetNPixelX()+1;
   
   TAITcluster* cluster = 0x0;

   // create clusters
   for (Int_t i = 0; i< fClustersN; ++i)
      pNtuClus->NewCluster(iSensor);
   
   for (Int_t iPix = 0; iPix < fListOfPixels->GetEntries(); ++iPix) {
      TAITntuHit* pixel = (TAITntuHit*)fListOfPixels->At(iPix);
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
Bool_t TAITactNtuClusterF::ShapeCluster(Int_t noClus, Int_t IndX, Int_t IndY, TAVTparGeo* pGeoMap)
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
void TAITactNtuClusterF::ComputePosition()
{
   TAVTactBaseNtuCluster::ComputePosition();
}

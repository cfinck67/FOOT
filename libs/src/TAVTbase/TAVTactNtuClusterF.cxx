/*!
 \file
 \version $Id: TAVTactNtuClusterF.cxx $
 \brief   Implementation of TAVTactNtuClusterF.
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
#include "TAVTactNtuClusterF.hxx"

/*!
 \class TAVTactNtuClusterF 
 \brief NTuplizer for vertex raw hits. **
 */

ClassImp(TAVTactNtuClusterF);

//------------------------------------------+-----------------------------------
//! Default constructor.
TAVTactNtuClusterF::TAVTactNtuClusterF(const char* name, 
									 TAGdataDsc* pNtuRaw, TAGdataDsc* pNtuClus,
									 TAGparaDsc* pConfig, TAGparaDsc* pGeoMap)
: TAVTactBaseNtuCluster(name, pNtuRaw, pNtuClus, pConfig, pGeoMap)
{
   TAVTparGeo* geoMap = (TAVTparGeo*)fpGeoMap->Object();
   Int_t nLines = geoMap->GetNPixelY()+1;
   Int_t nCols  = geoMap->GetNPixelX()+1;
   fFlagMap.Set(nLines*nCols);
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTactNtuClusterF::~TAVTactNtuClusterF()
{
   
}
										
//______________________________________________________________________________
//  
Bool_t TAVTactNtuClusterF::FindClusters(Int_t iSensor)
{
   // Algo taking from Virgile BEKAERT (ImaBio @ IPHC-Strasbourg)
   // Look in a iterative way to next neighbour
   
   TAVTntuCluster* pNtuClus = (TAVTntuCluster*) fpNtuClus->Object();
   TAVTparGeo*     pGeoMap  = (TAVTparGeo*)     fpGeoMap->Object();

   return FindClusters(iSensor, pNtuClus, pGeoMap);
}

//______________________________________________________________________________
//
Bool_t TAVTactNtuClusterF::FindClusters(Int_t iSensor, TAVTntuCluster* pNtuClus, TAVTparGeo* pGeoMap)
{
   Int_t nLine = pGeoMap->GetNPixelY()+1;
   Int_t nCol  = pGeoMap->GetNPixelX()+1;
   
   Int_t nClusters = 0;
   fPixelMap.clear();
   fIndexMap.clear();
   fFlagMap.Reset(-1);
   
   if (fListOfPixels->GetEntries() == 0) return true; 
   
   if (fDebugLevel)
	  printf("Sensor %d\n", iSensor);
   
   // fill maps for cluster
   for (Int_t i = 0; i < fListOfPixels->GetEntries(); i++) { // loop over hit pixels
	  
	  TAVTntuHit* pixel = (TAVTntuHit*)fListOfPixels->At(i);
	  Int_t line = pixel->GetPixelLine();
	  Int_t col  = pixel->GetPixelColumn();
	   if (line >= nLine) continue;
	   if (col  >= nCol)  continue;
	   if (line < 0) continue;
	   if (col  < 0)  continue;
	  fPixelMap[line*nCol+col] = 1;
	  fIndexMap[line*nCol+col] = i;
   }
   
   
   // Search for cluster
   TAVTcluster* cluster = 0x0;
   
   for (Int_t iPix = 0; iPix < fListOfPixels->GetEntries(); ++iPix) { // loop over hit pixels
	  TAVTntuHit* pixel = (TAVTntuHit*)fListOfPixels->At(iPix);
	  if (pixel->Found()) continue;
	  Int_t line = pixel->GetPixelLine();
	  Int_t col  = pixel->GetPixelColumn();
	  if (line >= nLine) continue;
	  if (col  >= nCol)  continue;
	  if (line < 0) continue;
	  if (col  < 0)  continue;

	  // loop over lines & columns
     if ( ShapeCluster(nClusters, line, col, pGeoMap) )
        nClusters++;
   }
   
   // create clusters
   for (Int_t i = 0; i< nClusters; ++i)
      pNtuClus->NewCluster(iSensor);
   
   for (Int_t iPix = 0; iPix < fListOfPixels->GetEntries(); ++iPix) {
	  TAVTntuHit* pixel = (TAVTntuHit*)fListOfPixels->At(iPix);
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
Bool_t TAVTactNtuClusterF::ShapeCluster(Int_t noClus, Int_t IndX, Int_t IndY, TAVTparGeo* pGeoMap)
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
void TAVTactNtuClusterF::ComputePosition()
{
   TAVTactBaseNtuCluster::ComputePosition();
}

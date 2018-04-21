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
   Int_t nLines = geoMap->GetNPixelY();
   Int_t nCols  = geoMap->GetNPixelX();
   fFlagMap.Set(nLines*nCols);
}




// //______________________________________________________________________________
// //  
// Bool_t TAVTactNtuClusterF::FindClusters(Int_t iSensor)	{




// }


/*
COMMENTI

- non ti fa scegliere quali pix usare. Da fare tramite flag esterna.

- non fa controlli su cluster aggregati o non-connessi. Solo su dimensioni max e min

- logica pericolosa (map) e contorta e lenta (troppi loop e passaggi)

- quadrato non serve

- controlla in sistema di riferimento

- non-connessi: ogni bin vuoto adiacente a uno pieno. 
				se lato libero e' up, solo bin a dx oppure(non entrambi) sx devono aver coordinata up maggiore 

- settare le dimensioni min e max

*/


//______________________________________________________________________________
//  
Bool_t TAVTactNtuClusterF::FindClusters(Int_t iSensor)
{
   // Algo taking from Virgile BEKAERT (ImaBio @ IPHC-Strasbourg)
   // Look in a iterative way to next neighbour
   
   TAVTntuCluster* pNtuClus = (TAVTntuCluster*) fpNtuClus->Object();
   TAVTparGeo*     pGeoMap  = (TAVTparGeo*)     fpGeoMap->Object();
   // TAVTparConf*    pConfig  = (TAVTparConf*)    fpConfig->Object();
   // Int_t clusterWidth = pConfig->GetAnalysisPar().SearchPixelDistance;
   Int_t clusterWidth = 10;  // in unit of number of pixels

   Int_t nLine = pGeoMap->GetNPixelY();  
   Int_t nCol  = pGeoMap->GetNPixelX();
   
   Int_t nClusters = 0;
   fPixelMap.clear();
   fIndexMap.clear();
   fFlagMap.Reset(-1);
   
   Int_t loLine = nLine, hiLine = 0;
   Int_t loCol  = nCol,  hiCol  = 0;
   
   // check if any pixel avalilable
   if (fListOfPixels->GetEntries() == 0) return true; 
   
   if ( GlobalPar::GetPar()->Debug() > 0 )         cout << "\tTAVTactNtuClusterF::FindClusters  :  Clustering on sensor " << iSensor << endl;
   
  //  // fill maps for cluster  --  fListOfPixels  filled in parent class action() with all pixels of the current sensor
   for (int i=0; i < fListOfPixels->GetEntries(); i++) { // loop over hit pixels
	  
		TAVTntuHit* pixel = (TAVTntuHit*)fListOfPixels->At(i);
		int line = pixel->GetPixelLine();
		int col  = pixel->GetPixelColumn();
		if (line >= nLine) continue;
		if (col  >= nCol)  continue;
		if (line < 0) continue;
		if (col  < 0)  continue;
		fPixelMap[line*nCol+col] = 1;
		fIndexMap[line*nCol+col] = i;
   }
   
	if (fDebugLevel)
	  printf("clus lim %3d %3d %3d %3d\n", loLine, hiLine, loCol, hiCol);

	// Search for cluster
	TAVTcluster* cluster = 0x0;

	for (Int_t iPix = 0; iPix < fListOfPixels->GetEntries(); ++iPix) { // loop over hit pixels
		
		TAVTntuHit* pixel = (TAVTntuHit*)fListOfPixels->At(iPix);
		
		if (pixel->Found()) continue;		// make a blacklist of the used pixels
		
		// check if line and col are correct
		int line = pixel->GetPixelLine();
		int col  = pixel->GetPixelColumn();
		if ( line < 0 || line >= nLine ) continue;
		if ( col  < 0 || col  >= nCol )  continue;

		// fPixelMap[line*nCol+col] = 1;	// only record the pixel, 1 is useless.
		// fIndexMap[line*nCol+col] = iPix;	// merge the 2 

	  	// define a square arownd the pixel 
		loLine =  pixel->GetPixelLine() - clusterWidth;
		if ( line - clusterWidth < 0) 	loLine = 0;
		hiLine =  pixel->GetPixelLine() + clusterWidth;
		if (hiLine > nLine) hiLine = nLine;

		loCol =  pixel->GetPixelColumn() - clusterWidth;
		if (loCol < 0) loCol = 0;
		hiCol =  pixel->GetPixelColumn() + clusterWidth;
		if (hiCol > nCol) hiCol = nCol;


		// loop over the square and sign each connected pix inside as found(change). 
		// Do it for each pixel in the square and count how many connected clusters are there
		// fill fFlagMap map with pix (lin,col) and the nuber of the cluster it belong
		for (int i = loLine; i < hiLine; ++i) { 
			for (int j = loCol; j < hiCol; ++j) { 
				// Clusterization
				if ( ShapeCluster(nClusters, i, j) )
					nClusters++;
			} 
		}
	}
   
   // create clusters
   for (Int_t i = 0; i< nClusters; ++i) 
	  pNtuClus->NewCluster(iSensor);
   
   // loop over pix and add them to its cluster. Why not before?
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
	  
	  // find center
	  ComputePosition();
	  
	  cluster->SetNumber(i);
	  // TVector3 posG = pGeoMap->Local2Global(iSensor, *GetCurrentPosition());
	  TVector3 posG = *GetCurrentPosition();
	  pGeoMap->Local2Global(&posG);	// sure???
	  cluster->SetPositionG(&posG);
	  cluster->SetPosition(GetCurrentPosition());
	  cluster->SetPosError(GetCurrentPosError());
	  cluster->SetIndexSeed(fPSeed->GetPixelIndex());
	  
	  // check max and min num of pix in cluster only
	  if (ApplyCuts(cluster)) {
		 // histogramms
		 if (ValidHistogram()) {
			if (cluster->GetPixelsN() > 0) {
			   fpHisPixelTot->Fill(cluster->GetPixelsN());
			   fpHisPixel[iSensor]->Fill(cluster->GetPixelsN());
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
Bool_t TAVTactNtuClusterF::ShapeCluster(Int_t noClus, Int_t IndX, Int_t IndY)
{
   TAVTparGeo* pGeoMap = (TAVTparGeo*)fpGeoMap->Object();
   
   Int_t nLine = pGeoMap->GetNPixelY();
   Int_t nCol  = pGeoMap->GetNPixelX();

   if ( fPixelMap[IndX*nCol+IndY] <= 0 ) return false;  // check if pix is on. element not initialised is <= 0. It allocate a new element. Better to use find()
   if ( fFlagMap[IndX*nCol+IndY] != -1 ) return false;	// check if already used
   fFlagMap[IndX*nCol+IndY] = noClus;

   TAVTntuHit* pixel = (TAVTntuHit*)GetListOfPixels()->At(fIndexMap[IndX*nCol+IndY]);
   pixel->SetFound(true);
     
    // nested to fill the neighborought pixels, and their nighborought and so on...
   for(Int_t i = -1; i <= 1 ; ++i)
	  if ( IndX+i >= 0 && IndX+i < nLine)
		 ShapeCluster(noClus, IndX+i, IndY);
   
   for(Int_t j = -1; j <= 1 ; ++j)
	  if ( IndY+j >= 0 && IndY+j < nCol)
		 ShapeCluster(noClus, IndX  , IndY+j);
   
   return true;
}

//______________________________________________________________________________
//
void TAVTactNtuClusterF::ComputePosition()
{
   TAVTactBaseNtuCluster::ComputePosition();
}



















/*!
 \file
 \version $Id: TAITactNtuCluster.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
 \brief   Implementation of TAITactNtuCluster.
 */
#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#include "TAITdatRaw.hxx"
#include "TAITparGeo.hxx"
#include "TAITparConf.hxx"
#include "TAITntuRaw.hxx"
#include "TAITntuCluster.hxx"
#include "TAITactNtuCluster.hxx"

/*!
 \class TAITactNtuCluster 
 \brief NTuplizer for vertex raw hits. **
 */

ClassImp(TAITactNtuCluster);

//------------------------------------------+-----------------------------------
//! Default constructor.
TAITactNtuCluster::TAITactNtuCluster(const char* name, 
									 TAGdataDsc* pNtuRaw, TAGdataDsc* pNtuClus,
									 TAGparaDsc* pConfig, TAGparaDsc* pGeoMap)
: TAITactBaseNtuCluster(name, pNtuRaw, pNtuClus, pConfig, pGeoMap),
  fIndexArray()
{
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITactNtuCluster::~TAITactNtuCluster()
{
   
}
										
//______________________________________________________________________________
//  
Bool_t TAITactNtuCluster::FindClusters(Int_t iSensor)
{
   // Valid for sparse data acquisition
   //
   // Construct a hit (or cluster) from a seed pixel:
   // 1) try to associate all neighbouring Pixels that pass selection cut
   // 2) check the build hit passes all selections
   // 3) order the array fPixelIndexArray of neighbours by decreasing pulseheight
   // 4) compute cluster charge, noise and position
   //
   // change: look for neighbouring pixel, do not use anymore clusterLimit->Length()
   
   Bool_t hitOK  = kFALSE;  // set false by default for this search
   fIndexArray.Set(0);
   TAITntuCluster* pNtuClus = (TAITntuCluster*) fpNtuClus->Object();
   TAITparGeo*     pGeoMap  = (TAITparGeo*)     fpGeoMap->Object();

   for (Int_t tci = 0; tci < fListOfPixels->GetEntries(); tci++){ // loop over hit pixels
	  
	  fPSeed = (TAITntuHit*)fListOfPixels->At(tci);
	  if (!fPSeed->Found()) {
		 fIndexArray.Set(fIndexArray.GetSize()+1);
		 fIndexArray.AddAt(tci, fIndexArray.GetSize()-1);
		 fPSeed->SetFound(kTRUE);                            
	  } else {
		 continue;
	  }
	  
	  if ( ((TAITntuHit*)fListOfPixels->At(tci))->GetPulseHeight() > 0) { //fCut->GetSeedPulseHeightToNoise() ) {
		 hitOK = AddPixels( tci);
		 
		 if ( hitOK == kTRUE ) {
			TAITcluster* cluster = pNtuClus->NewCluster(iSensor);
			cluster->SetPlaneNumber(iSensor);
			for (Int_t i = 0; i < GetIndexArray().GetSize(); ++i) {
			   TAITntuHit* pixel = (TAITntuHit*)GetListOfPixels()->At(GetIndexArray()[i]);
			   cluster->AddPixel(pixel);
			}
			cluster->SetNumber(pNtuClus->GetClustersN(iSensor));
			fCurListOfPixels = cluster->GetListOfPixels();
			ComputePosition();
			// TVector3 posG = pGeoMap->Local2Global(iSensor, *GetCurrentPosition());
			TVector3 posG = *GetCurrentPosition();
			pGeoMap->Local2Global(&posG);
			cluster->SetPositionG(&posG);
			cluster->SetPosition(GetCurrentPosition());
			cluster->SetPosError(GetCurrentPosError());
			cluster->SetIndexSeed(fPSeed->GetPixelIndex());

			//ReorderPixels();
			
			if (!ApplyCuts(cluster))  {
			   for (Int_t i = 1; i < GetIndexArray().GetSize(); ++i) {
				  TAITntuHit* pixel = (TAITntuHit*)GetListOfPixels()->At(GetIndexArray()[i]);
				  pixel->SetFound(kFALSE);
			   }
			   pNtuClus->GetListOfClusters(iSensor)->Remove(cluster);
			   pNtuClus->GetListOfClusters(iSensor)->Compress();
			} else {
			   if (ValidHistogram()) {
				  if (cluster->GetPixelsN() > 0) {
					 fpHisPixelTot->Fill(cluster->GetPixelsN());
					 fpHisPixel[iSensor]->Fill(cluster->GetPixelsN());
					 if (TAITparConf::IsMapHistOn()) {
						fpHisClusMap[iSensor]->Fill(cluster->GetPosition()[0], cluster->GetPosition()[1]);
					 }
				  }
			   }
			}

			fIndexArray.Set(0);
		 } // if hitOk
		 
	  } //02//06/09 ends
	  
   } // loop Pixel 
   
   if( fDebugLevel>2 ) printf("         %d hits found\n", pNtuClus->GetClustersN(iSensor));
   
   return true;
}

//______________________________________________________________________________
//
void TAITactNtuCluster::ComputePosition()
{
   TAITactBaseNtuCluster::ComputePosition();
}

//______________________________________________________________________________
//  
Bool_t TAITactNtuCluster::AddPixels(Int_t aPixelIndex)
{
   
   TAITntuHit   *aNeighbour;
   Bool_t    valid = true;
   
   // add pixels that are neighbor from neighbors
   
   //===============
   // associate neighbouring pixels to seed one
   //===============
   if(fDebugLevel>1) 
	  printf("  DClusterFinderComplex:Analyse seed pixel (%d in list) (q=%f)\n", aPixelIndex, fPSeed->GetPulseHeight());
   
   
   for (Int_t iPix = 0; iPix < fListOfPixels->GetEntries(); ++iPix) { // loop over hit pixels
	  aNeighbour = (TAITntuHit*)fListOfPixels->At(iPix);
	  if ( iPix != aPixelIndex ) {
		 if( TMath::Abs(fPSeed->GetPixelLine() - aNeighbour->GetPixelLine()) <= 1 &&  TMath::Abs(fPSeed->GetPixelColumn() - aNeighbour->GetPixelColumn()) <= 1 ) {
			if (!aNeighbour->Found()) {
			   fIndexArray.Set(fIndexArray.GetSize()+1);
			   fIndexArray.AddAt(iPix, fIndexArray.GetSize()-1);
			   aNeighbour->SetFound(kTRUE);
			}
			
			if( fDebugLevel>1)
			   printf("  neighbour %d at index %d found!\n", fIndexArray.GetSize(), aNeighbour->GetPixelIndex());			
		 }
	  } // found
	  
	  TArrayI tmp;
	  for (Int_t idx = 0; idx < fIndexArray.GetSize(); ++idx) {
		 TAITntuHit* pixelI = (TAITntuHit*)fListOfPixels->At(fIndexArray[idx]);
		 for (Int_t j = 0; j < fListOfPixels->GetEntries(); ++j) {
			TAITntuHit* pixelO = (TAITntuHit*)fListOfPixels->At(j);
			
			if( TMath::Abs(pixelI->GetPixelLine() - pixelO->GetPixelLine()) <= 1 &&  TMath::Abs(pixelI->GetPixelColumn() - pixelO->GetPixelColumn()) <= 1 ) {
			   if (!pixelO->Found()) {
				  tmp.Set(tmp.GetSize()+1);
				  tmp.AddAt(j, tmp.GetSize()-1);
				  pixelO->SetFound(kTRUE);
			   }
			}
		 }
	  }
	  for (Int_t idx = 0; idx < tmp.GetSize(); ++idx) {
		 fIndexArray.Set(fIndexArray.GetSize()+1);
		 fIndexArray.AddAt(tmp[idx], fIndexArray.GetSize()-1);
	  }
   } // end loop over hit pixels
   
   if(fDebugLevel>2) 
	  printf("TAITactNtuCluster:Analyse found %d valid neighbours\n", fIndexArray.GetSize());
      
   return valid;
}

//______________________________________________________________________________
//  
void TAITactNtuCluster::ReorderPixels()
{
   
   //=============
   // order pixels
   //=============
   // here, the neighbourhood of Pixels is assumed to be ordered, 
   // at index 0 is the seed, to higher indices
   // the neighbourPixels are further away from the seed.
   
   Int_t *nOfneighbours = new Int_t[fIndexArray.GetSize()];
   
   // For binary readout
   // redefine the seed as the one with the highest number of direct neighbours	  
   Int_t iSeed = 0; // init
   Int_t indexSeed  = fPSeed->GetPixelIndex();
   TAITntuHit *aPixel = 0x0;
   TAITntuHit* bPixel = 0x0;
   for( Int_t iPix = 0; iPix < fIndexArray.GetSize(); iPix++) { // loop on pixels in candidate hit
	  nOfneighbours[ iPix] = 0;
	  
	  for( Int_t jPix = 0; jPix < fIndexArray.GetSize(); jPix++) {
		 if(fDebugLevel>3) 
			printf("     comparing pixels %d[%d] and %d[%d] in list of %d pixels\n", fIndexArray[ iPix], iPix, 
				   fIndexArray[ jPix], jPix, fIndexArray.GetSize());
		 
		 aPixel = (TAITntuHit*)fListOfPixels->At( fIndexArray[ iPix] );
		 bPixel = (TAITntuHit*)fListOfPixels->At( fIndexArray[ jPix] );
		 if(fDebugLevel>3) 
			printf("     comparing pixels %d[%d] (%d-%d) and %d[%d] (%d-%d) in list of %d pixels\n", fIndexArray[ iPix], 
				   iPix, aPixel->GetPixelLine(), aPixel->GetPixelColumn(), fIndexArray[ jPix], jPix, bPixel->GetPixelLine(), 
				   bPixel->GetPixelColumn(), fIndexArray.GetSize());
		 
		 // direct neighbours are defines as
		 // abs(difference in line) + abs(difference in column) = 1
		 if( (abs(aPixel->GetPixelLine() - bPixel->GetPixelLine()) + 
			  abs(aPixel->GetPixelColumn() - bPixel->GetPixelColumn()) ) == 1 ) {
			nOfneighbours[ iPix] += 1;
		 }
	  }
	  if(fDebugLevel>3) printf("   DClusterFinderComplex::Analyse pixel %d has %d direct neighbours <?> max%d\n", 
								 iPix, nOfneighbours[ iPix], nOfneighbours[ iSeed]);
	  if( nOfneighbours[ iPix] > nOfneighbours[ iSeed] || 
		 (nOfneighbours[ iPix] == nOfneighbours[ iSeed] && (aPixel->GetPixelColumn()<((TAITntuHit*)fListOfPixels->At( fIndexArray[ iPix] ))->GetPixelColumn() || 
															aPixel->GetPixelLine()<((TAITntuHit*)fListOfPixels->At( fIndexArray[ iSeed] ))->GetPixelLine()) ) ) { // force lower lefter seed
		 iSeed = iPix;
	  }
	  
   } // end loop on pixels
   
   // swap previous info at index 0 with the one at index iSeed
   // so that all arrays have seed info at index 0
   Int_t iTemp;
   iTemp                = fIndexArray[iSeed];
   fIndexArray[iSeed]   = fIndexArray[0];
   fIndexArray[0]       = iTemp;
   
   // need to update some variables 
   // to take into account the new seed definition
   fPSeed     = (TAITntuHit*)fListOfPixels->At( fIndexArray[0] );
   
   
   if( fDebugLevel>0 ) {
	  printf("  DClusterFinderComplex:Analyse  seed is now pixel %d in list, index %d, with %d direct neighbours\n", 
			 fIndexArray[ 0], indexSeed, nOfneighbours[ iSeed]);
   }
   
   delete nOfneighbours;
   
}


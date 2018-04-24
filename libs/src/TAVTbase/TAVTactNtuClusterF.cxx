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
									 TAGparaDsc* pConfig, TAGparaDsc* pGeoMap, string command)
: TAVTactBaseNtuCluster(name, pNtuRaw, pNtuClus, pConfig, pGeoMap, command)
{
   TAVTparGeo* geoMap = (TAVTparGeo*)fpGeoMap->Object();
   m_nLine = geoMap->GetNPixelY();
   m_nCol  = geoMap->GetNPixelX();
}



/*
COMMENTI

0 non ti fa scegliere quali pix usare. Da fare tramite flag esterna.

- non fa controlli su cluster aggregati o non-connessi. Solo su dimensioni max e min
- non-connessi: ogni bin vuoto adiacente a uno pieno. 
				se lato libero e' up, solo bin a dx oppure(non entrambi) sx devono aver coordinata up maggiore 

0 logica pericolosa (map) e contorta e lenta (troppi loop e passaggi)

? quadrato non serve

/ controlla double counting

/ controlla in sistema di riferimento

- settare le dimensioni min e max

*/


//______________________________________________________________________________
//  
Bool_t TAVTactNtuClusterF::FindClusters(int iSensor)	{

	// if ( GlobalPar::GetPar()->Debug() > 0 )         
		cout << "\tTAVTactNtuClusterF::FindClusters  :  Clustering on sensor " << iSensor << endl;

    // Look in a iterative way to next neighbour
   	TAVTntuCluster* 	pNtuClus = (TAVTntuCluster*) fpNtuClus->Object();
	TAVTntuRaw* 		pNtuRaw  = (TAVTntuRaw*) 	 fpNtuRaw->Object();

	// TAVTparConf*    pConfig  = (TAVTparConf*)    fpConfig->Object();
	// Int_t clusterWidth = pConfig->GetAnalysisPar().SearchPixelDistance;
	int nPixOn = pNtuRaw->GetPixelsN( iSensor, m_command );  // number of pixels ON

	// init and clear
	m_pixelHitMap.clear();
	m_indexMap.clear();
	m_usedPixelMap.clear();	
	m_pixelIDMap.resize(nPixOn);		// = to clear(), resize(nPixOn)

	// map<int, int> m_pixelHitMap;        // map of pixels that are present in the ntuRaw (value is always 1), indexed by line*nLine+col algorithm: x[ line*nLine+col ] = 1
	// map<int, int> m_indexMap;           // map of the pixel index in ntuRaw, indexed by line*nLine+col algorithm: x[ line*nLine+col ] = ID
	// vector<int> m_pixelIDMap;           // map of the line*nLine+col algorithm, indexed by index in ntuRaw: x[ ID ] = line*nLine+col
	// map<int, int> m_usedPixelMap;       // map used pixels, store the belongning cluster else -1, indexed by line*nLine+col algorithm: x[ line*nLine+col ] = clusterID
	
	// fill maps for cluster  
	for ( int iPix=0; iPix < nPixOn; iPix++) { // loop over hit pixels

   	  	// sanity check
   		TAVTntuHit* pixel = (TAVTntuHit*) pNtuRaw->GetPixel( iSensor, iPix, m_command );

   		int line = pixel->GetPixelLine();
   		int col  = pixel->GetPixelColumn();
   		if ( line < 0 || line >= m_nLine || col  < 0 || col  >= m_nCol )  {
   			cout << "ERROR << TAVTactNtuClusterF::FindClusters  -->  pixel "<<iPix<<" in sensor "<<iSensor<<" gives line or column out of bounds: line= "<<line<<" col="<<col<<".  Max line="<<m_nLine<<" Max column="<<m_nCol;
   			exit(0);
   		}
   		int idAlgo = (line * m_nLine) + col;
   		m_pixelHitMap[ idAlgo ] = 1;
   		m_indexMap[ idAlgo ] = iPix;
   		m_usedPixelMap[ idAlgo ] = -1;
   		m_pixelIDMap[iPix] = idAlgo;
	}
      

   	// Search for cluster
	Bool_t fillNewCluster = true;
   	for (int iPix = 0; iPix < pNtuRaw->GetPixelsN( iSensor, m_command ); ++iPix) { // loop over hit pixels

   		// skip if pixel arready used
   		if ( m_usedPixelMap[ m_pixelIDMap.at(iPix) ] != -1 )	continue;

  		TAVTntuHit* pixel = (TAVTntuHit*) pNtuRaw->GetPixel( iSensor, iPix, m_command );

		// Clusterization
		if (fillNewCluster) 	{
			pNtuClus->NewCluster(iSensor);
		}
		fillNewCluster = ShapeCluster( iSensor, pNtuClus->GetClustersN(iSensor)-1, pixel->GetPixelLine(), pixel->GetPixelColumn() );
   	}
   

	// Compute position and fill clusters info
	for ( int i = 0; i< pNtuClus->GetClustersN(iSensor); ++i) {
	  
		TAVTcluster* cluster = pNtuClus->GetCluster(iSensor, i);
		cluster->SetSensorID(iSensor);
		fPSeed = cluster->GetPixel(0);	  // really...
		fCurListOfPixels = cluster->GetListOfPixels();	// needed by ComputePosition()

		// find center: fill global var fCurrentPosition, fCurrentPosError
		ComputePosition();

		cluster->SetClusterID(i);	  // check if correct
		cluster->SetPosition( fCurrentPosition );
		cluster->SetPosError( fCurrentPosError );
		// cluster->SetIndexSeed( fPSeed->GetPixelIndex() );  // not used yet
   	  
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
		} 
		else {
			pNtuClus->GetListOfClusters(iSensor)->Remove(cluster);
			pNtuClus->GetListOfClusters(iSensor)->Compress();
		}
	}
	if (pNtuClus->GetClustersN(iSensor))	return true;
      
	return false;
	
}





//______________________________________________________________________________
//  
Bool_t TAVTactNtuClusterF::ShapeCluster( int sensorID, int clusterID, int line, int col )	{
   
	int currentPixAlgorithm = (line * m_nLine) + col;
	// check if defined or pix is on. element not initialised is <= 0. It allocate a new element without find. Better to use find()
	if ( m_pixelHitMap.find( currentPixAlgorithm ) == m_pixelHitMap.end() )		return false; 
	if ( m_pixelHitMap[currentPixAlgorithm] <= 0 ) 								return false; 
	
	// check if not defined already used
	if ( m_usedPixelMap.find( currentPixAlgorithm ) == m_usedPixelMap.end() )		return false; 
	if ( m_usedPixelMap[currentPixAlgorithm] != -1 ) 								return false;		
   
	m_usedPixelMap[currentPixAlgorithm] = clusterID;  // fill list of used clusters
	
	TAVTntuHit* pixel = (TAVTntuHit*) ( (TAVTntuRaw*) fpNtuRaw->Object() )->GetPixel( sensorID, m_indexMap[currentPixAlgorithm], m_command );
	( (TAVTntuCluster*) fpNtuClus->Object() )->GetCluster(sensorID, clusterID)->AddPixel(pixel);
	
	// nested to fill the neighborought pixels, and their nighborought and so on...
	for( int i = -1; i <= 1 ; ++i )
		if ( line+i >= 0 && line+i < m_nLine )
			ShapeCluster( sensorID, clusterID, line+i, col );

	for( int j = -1; j <= 1 ; ++j )
		if ( col+j >= 0 && col+j < m_nCol)
			ShapeCluster( sensorID, clusterID, line, col+j );
   
	return true;
}




//______________________________________________________________________________
//
void TAVTactNtuClusterF::ComputePosition() {
   TAVTactBaseNtuCluster::ComputePosition();
}




/*
//______________________________________________________________________________
//  
Bool_t TAVTactNtuClusterF::FindClusters(Int_t iSensor)
{
   // Algo taking from Virgile BEKAERT (ImaBio @ IPHC-Strasbourg)
   // Look in a iterative way to next neighbour
   
   TAVTntuCluster* pNtuClus = (TAVTntuCluster*) fpNtuClus->Object();
   TAVTparGeo*     pGeoMap  = (TAVTparGeo*)     fpGeoMap->Object();
   TAVTparConf*    pConfig  = (TAVTparConf*)    fpConfig->Object();

   Int_t clusterWidth = pConfig->GetAnalysisPar().SearchPixelDistance;
   Int_t nLine = pGeoMap->GetNPixelY()+1;
   Int_t nCol  = pGeoMap->GetNPixelX()+1;
   
   Int_t nClusters = 0;
   fPixelMap.clear();
   fIndexMap.clear();
   fFlagMap.Reset(-1);
   Int_t loLine = nLine, hiLine = 0;
   Int_t loCol  = nCol,  hiCol  = 0;
   
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
   
   if (fDebugLevel)
	  printf("clus lim %3d %3d %3d %3d\n", loLine, hiLine, loCol, hiCol);
   
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

	  
	  loLine =  pixel->GetPixelLine() - clusterWidth;
	  if (loLine < 0) loLine = 0;
	  hiLine =  pixel->GetPixelLine() + clusterWidth;
	  if (hiLine > nLine) hiLine = nLine;
	  
	  loCol =  pixel->GetPixelColumn() - clusterWidth;
	  if (loCol < 0) loCol = 0;
	  hiCol =  pixel->GetPixelColumn() + clusterWidth;
	  if (hiCol > nCol) hiCol = nCol;
	  
	  // loop over lines & columns
	  for (Int_t i = loLine; i < hiLine; ++i) { 
		 for (Int_t j = loCol; j < hiCol; ++j) { 
			// Clusterization
			if ( ShapeCluster(nClusters, i, j) )
			   nClusters++;
		 } 
	  }
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
	  // TVector3 posG = pGeoMap->Local2Global(iSensor, *GetCurrentPosition());
	  TVector3 posG = *GetCurrentPosition();
	  pGeoMap->Local2Global(&posG);
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


*/















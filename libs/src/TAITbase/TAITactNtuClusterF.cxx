/*!
 \file
 \version $Id: TAITactNtuClusterF.cxx $
 \brief   Implementation of TAITactNtuClusterF.
 */
#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#include "TAITparGeo.hxx"
#include "TAITparConf.hxx"
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
: TAVTactNtuClusterF(name, pNtuRaw, pNtuClus, pConfig, pGeoMap)
{

}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITactNtuClusterF::~TAITactNtuClusterF()
{
   
}
										
//______________________________________________________________________________
//  
Bool_t TAITactNtuClusterF::FindClusters(Int_t iSensor)
{
   // Algo taking from Virgile BEKAERT (ImaBio @ IPHC-Strasbourg)
   // Look in a iterative way to next neighbour
   
   TAITntuCluster* pNtuClus = (TAITntuCluster*) fpNtuClus->Object();
   TAITparGeo*     pGeoMap  = (TAITparGeo*)     fpGeoMap->Object();
   
   return TAVTactNtuClusterF::FindClusters(iSensor, pNtuClus, pGeoMap);
}

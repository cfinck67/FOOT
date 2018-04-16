/*!
 \file
 \version $Id: TAITactNtuClusterF.cxx $
 \brief   Implementation of TAITactNtuClusterF.
 */

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
										
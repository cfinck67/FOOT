/*!
 \file
 \version $Id: TAVTparMap.cxx,v 1.5 2003/06/09 18:41:04 mueller Exp $
 \brief   Implementation of TAVTparMap.
 */

#include "TAVTparMap.hxx"

//##############################################################################

/*!
 \class TAVTparMap TAVTparMap.hxx "TAVTparMap.hxx"
 \brief Map and Geometry parameters for Tof wall. **
 */

ClassImp(TAVTparMap);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAVTparMap::TAVTparMap() 
 : TAVTbaseParMap()
{
   fkDefaultMapName = "./config/TAVTdetector.map";
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAVTparMap::~TAVTparMap()
{
}

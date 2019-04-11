/*!
 \file
 \version $Id: TAITparMap.cxx,v 1.5 2003/06/09 18:41:04 mueller Exp $
 \brief   Implementation of TAITparMap.
 */

#include "TAITparMap.hxx"

//##############################################################################

/*!
 \class TAITparMap TAITparMap.hxx "TAITparMap.hxx"
 \brief Map and Geometry parameters for Tof wall. **
 */

ClassImp(TAITparMap);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAITparMap::TAITparMap() 
: TAVTparMap()
{
   fkDefaultMapName = "./config/TAITdetector.map";
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAITparMap::~TAITparMap()
{}

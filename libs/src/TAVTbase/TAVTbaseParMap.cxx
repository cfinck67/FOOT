/*!
 \file
 \version $Id: TAVTbaseParMap.cxx,v 1.5 2003/06/09 18:41:04 mueller Exp $
 \brief   Implementation of TAVTbaseParMap.
 */

#include "TAVTbaseParMap.hxx"

//##############################################################################

/*!
 \class TAVTbaseParMap TAVTbaseParMap.hxx "TAVTbaseParMap.hxx"
 \brief Map and Geometry parameters for Tof wall. **
 */

ClassImp(TAVTbaseParMap);

const Int_t TAVTbaseParMap::fgkSensorsN = 4;
//------------------------------------------+-----------------------------------
//! Default constructor.

TAVTbaseParMap::TAVTbaseParMap() 
 : TAGparTools(),
   fSensorsN(0),
   fkDefaultMapName("")
{
   fPlaneId.reserve(fgkSensorsN);
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAVTbaseParMap::~TAVTbaseParMap()
{
}

//------------------------------------------+-----------------------------------
Bool_t TAVTbaseParMap::FromFile(const TString& name)
{
   // Reading mapping file
   TString nameExp;
   
   if (name.IsNull())
      nameExp = fkDefaultMapName;
   else
      nameExp = name;
   
   if (!Open(nameExp))
      return false;
   
   ReadItem(fSensorsN);
   
   printf("%d\n", fSensorsN);
   Int_t tmp;

   for (Int_t i = 0; i < fSensorsN; ++i) { // Loop on each charge
      
      ReadItem(tmp);
      
      fPlaneId.push_back(tmp);
      
      if(fDebugLevel)
         printf("SensorId: %d PlaneId: %d\n", i, fPlaneId[i]);
   }

   return true;
}
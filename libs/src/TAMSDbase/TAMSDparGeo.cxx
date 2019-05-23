/*!
  \file
  \version $Id: TAMSDparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TAMSDparGeo.
*/

#include <Riostream.h>

#include "TGeoBBox.h"
#include "TColor.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TROOT.h"

#include "TAGgeoTrafo.hxx" 

#include "TAMSDparGeo.hxx"

const TString TAMSDparGeo::fgkBaseName      = "MSD";
const TString TAMSDparGeo::fgkDefParaName   = "msdGeo";

//_____________________________________________________________________________
TAMSDparGeo::TAMSDparGeo()
: TAVTparGeo()
{
   fkDefaultGeoName = "./geomaps/TAMSDdetector.map";
}

//_____________________________________________________________________________
TAMSDparGeo::~TAMSDparGeo()
{
}


//_____________________________________________________________________________
//void TAMSDparGeo::DefineMaterial()
//{
//   // material for strip, same as M28 ?
//   TAVTbaseParGeo::DefineMaterial();
//}

//_____________________________________________________________________________
TGeoVolume* TAMSDparGeo::BuildMultiStripDetector(const char* basemoduleName, const char *msdName)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   // define box
   DefineMaxMinDimension();
   
   TGeoVolume* msd = gGeoManager->FindVolumeFast(msdName);
   if ( msd == 0x0 ) {
      TGeoMaterial* mat = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("AIR");
      TGeoMedium*   med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("AIR");
      msd = gGeoManager->MakeBox(msdName, med, fSizeBox.X()/2., fSizeBox.Y()/2., fSizeBox.Z()/2.); // volume corresponding to msd
   }
   
   TGeoVolume* msdMod = 0x0;

   for(Int_t iSensor = 0; iSensor < 6; iSensor++) {
      TGeoHMatrix* hm = GetTransfo(iSensor);
      msdMod = AddModule(Form("%s%d",basemoduleName, iSensor), msdName);
      
      TGeoHMatrix* transf = (TGeoHMatrix*)hm->Clone();

      msd->AddNode(msdMod, iSensor, transf);
   }
   
   return msd;
}

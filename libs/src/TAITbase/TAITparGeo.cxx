/*!
  \file
  \version $Id: TAITparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TAITparGeo.
*/

#include <Riostream.h>

#include "TGeoBBox.h"
#include "TColor.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoMaterial.h"

#include "TAGmaterials.hxx"
#include "TAGgeoTrafo.hxx"

#include "TAITparGeo.hxx"

//##############################################################################

const TString TAITparGeo::fgkBaseName      = "IT";
const TString TAITparGeo::fgkDefParaName   = "itGeo";

//_____________________________________________________________________________
TAITparGeo::TAITparGeo()
: TAVTparGeo()
{
   // Constructor
   fLayersN = 2;
   fgDefaultGeoName = "./geomaps/TAITdetector.map";
}

//______________________________________________________________________________
TAITparGeo::~TAITparGeo()
{
   // Destructor
}

//_____________________________________________________________________________
void TAITparGeo::DefineMaterial()
{
   // material for M28
   TAVTparGeo::DefineMaterial();
   
   TGeoMaterial* mat = 0x0;
   TGeoMixture*  mix = 0x0;
   
   // Foam SiC+Air
   mix = TAGmaterials::Instance()->CreateMixture(fFoamMat, fFoamMatDensities, fFoamMatProp, fFoamMatDensity);
   if (fDebugLevel) {
      printf("Foam material:\n");
      mix->Print();
   }

   // Kapton C22_H10_N2_O5
   mat = TAGmaterials::Instance()->CreateMaterial(fKaptonMat, fKaptonMatDensity);
   if (fDebugLevel) {
      printf("Kapton material:\n");
      mat->Print();
   }

   // Epoxy C18_H19_O3
   mat = TAGmaterials::Instance()->CreateMaterial(fEpoxyMat, fEpoxyMatDensity);
   if (fDebugLevel) {
      printf("Epoxy material:\n");
      mat->Print();
   }
   
   // Aluminum
   mat = TAGmaterials::Instance()->CreateMaterial(fAlMat, fAlMatDensity);
   if (fDebugLevel) {
      printf("Aluminum material:\n");
      mat->Print();
   }
}

//_____________________________________________________________________________
void TAITparGeo::ReadSupportInfo()
{
   ReadVector3(fFoamSize);
   if(fDebugLevel)
      cout  << endl << "  Foam size of support: "<< fFoamSize.X() << " " <<  fFoamSize.Y() << " "
      <<  fFoamSize.Z()  << endl;
   
   ReadStrings(fFoamMat);
   if(fDebugLevel)
      cout   << "  Foam material: "<< fFoamMat.Data() << endl;
   
   ReadStrings(fFoamMatDensities);
   if(fDebugLevel)
      cout  << "  Foam material component densities: "<< fFoamMatDensities.Data() << endl;
   
   ReadStrings(fFoamMatProp);
   if(fDebugLevel)
      cout  << "  Foam material proportion: "<< fFoamMatProp.Data() << endl;
   
   ReadItem(fFoamMatDensity);
   if(fDebugLevel)
      cout  << "  Foam material density:  "<< fFoamMatDensity << endl;

   
   ReadItem(fKaptonThickness);
   if(fDebugLevel)
      cout  << endl << "  Kapton thickness: "<< fKaptonThickness << endl;
   
   ReadStrings(fKaptonMat);
   if(fDebugLevel)
      cout   << "  Kapton material: "<< fKaptonMat.Data() << endl;
   
   ReadItem(fKaptonMatDensity);
   if(fDebugLevel)
      cout  << "  Kapton material density:  "<< fKaptonMatDensity << endl;
   
   
   ReadItem(fEpoxyThickness);
   if(fDebugLevel)
      cout  << endl << "  Epoxy thickness: "<< fEpoxyThickness << endl;
   
   ReadStrings(fEpoxyMat);
   if(fDebugLevel)
      cout   << "  Epoxy material: "<< fEpoxyMat.Data() << endl;
   
   ReadItem(fEpoxyMatDensity);
   if(fDebugLevel)
      cout  << "  Epoxy material density:  "<< fEpoxyMatDensity << endl;
   
   
   ReadItem(fAlThickness);
   if(fDebugLevel)
      cout  << endl << "  Alunimum thickness: "<< fAlThickness << endl;
   
   ReadStrings(fAlMat);
   if(fDebugLevel)
      cout   << "  Alunimum material: "<< fAlMat.Data() << endl;
   
   ReadItem(fAlMatDensity);
   if(fDebugLevel)
      cout  << "  Alunimum material density:  "<< fAlMatDensity << endl;

}

//_____________________________________________________________________________
TGeoVolume* TAITparGeo::BuildInnerTracker(const char *itName, const char* basemoduleName)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   // define box
   DefineMaxMinDimension();
   
   TGeoVolume* it = gGeoManager->FindVolumeFast(itName);
   if ( it == 0x0 ) {
      TGeoMaterial* mat = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("AIR");
      TGeoMedium*   med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("AIR");
      it = gGeoManager->MakeBox(itName,med,fSizeBox.X()/2.,fSizeBox.Y()/2.,fSizeBox.Z()/2.); // volume corresponding to vertex
   }
   
   TGeoVolume* itMod = 0x0;
   
   for(Int_t iSensor = 0; iSensor < GetNSensors(); iSensor++) {
      TGeoHMatrix* hm = GetTransfo(iSensor);
      itMod = AddModule(Form("%s%d",basemoduleName, iSensor), itName);
      
      TGeoHMatrix* transf = (TGeoHMatrix*)hm->Clone();
      double* trans = transf->GetTranslation();
      
      it->AddNode(itMod, iSensor, transf);
   }
   
   return it;
}

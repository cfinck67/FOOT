/*!
  \file
  \version $Id: TADIparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TADIparGeo.
*/

#include <Riostream.h>

#include "TGeoBBox.h"
#include "TGeoTube.h"
#include "TColor.h"
#include "TEveTrans.h"
#include "TGeoManager.h"
#include "TGeoCompositeShape.h"
#include "TList.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TROOT.h"
#include "TSystem.h"

#include "TAGgeoTrafo.hxx" 

#include "TADIparGeo.hxx"

#include "foot_geo.h"

//##############################################################################

/*!
  \class TADIparGeo TADIparGeo.hxx "TADIparGeo.hxx"
  \brief Map and Geometry parameters for Dipole. **
*/

ClassImp(TADIparGeo);

//______________________________________________________________________________
TADIparGeo::TADIparGeo()
: TAGpara(),
  // from gsi_geo.h (VM)
  fWidth(0.),
  fHeight(0.),
  fLength(0.),
  fDebugLevel(0)
{
   // Standard constructor
   DefineMaterial();
}

//______________________________________________________________________________
TADIparGeo::~TADIparGeo()
{
   // Destructor
}

//_____________________________________________________________________________
void TADIparGeo::InitGeo()
{
   fWidth  = MAG_PM_THICK;
   fHeight = MAG_PM_THICK;
   fLength = MAG_CV_LENGTH;
}

//_____________________________________________________________________________
void TADIparGeo::DefineMaterial()
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   TGeoElementTable* table = gGeoManager->GetElementTable();
   
   // create material
   TGeoMixture* mat = 0x0;;
   TGeoMedium*  med = 0x0;
   
   const Char_t* matNameAir = MAG_AIR_MEDIUM.Data();
   if ( (mat = (TGeoMixture*)gGeoManager->GetListOfMaterials()->FindObject(matNameAir)) == 0x0 ) {
      TGeoMaterial *matO = new TGeoMaterial("Oxygen",   16., 8., 1.41e-3);
      TGeoMaterial *matN = new TGeoMaterial("Nitrogen", 14., 7., 1.25e-3);
   
      mat = new TGeoMixture(matNameAir, 2, 1.29e-3);
      mat->AddElement(matN, 0.79);
      mat->AddElement(matO, 0.21);
   }
   if ( (med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matNameAir)) == 0x0 )
      med = new TGeoMedium(matNameAir,1,mat);
   
   // SmCo
   const Char_t* matName = MAG_PM_MEDIUM.Data();
   if ( (mat = (TGeoMixture*)gGeoManager->GetListOfMaterials()->FindObject(matName)) == 0x0 ) {
      
      TGeoElement* matSm = table->GetElement(62);
      TGeoElement* matCo = table->GetElement(27);
      
      mat =new TGeoMixture(matName,2, 8.3);
      mat->AddElement(matSm, 2);
      mat->AddElement(matCo, 17);
   }
   if ( (med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName)) == 0x0 )
      med = new TGeoMedium(matName,1,mat);
}

//_____________________________________________________________________________
TGeoVolume* TADIparGeo::BuildMagnet(const char* basemoduleName, const char *magnetName)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   TGeoVolume* magnet = gGeoManager->FindVolumeFast(magnetName);
   if ( magnet == 0x0 ) {
      
      const Char_t* matNameAir = MAG_AIR_MEDIUM.Data();
      TGeoMedium  *med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matNameAir);
      TGeoMixture *mat = (TGeoMixture*)gGeoManager->GetListOfMaterials()->FindObject(matNameAir);
     
      magnet = gGeoManager->MakeBox(magnetName,med,MAG_PM_THICK/2.,MAG_PM_THICK/2.,(MAG_PM_THICK+MAG_DIST)/2.); // volume corresponding to vertex
   }
   
   // create module
   const Char_t* matName = MAG_PM_MEDIUM.Data();
   TGeoMedium  *med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName);
   TGeoMixture *mat = (TGeoMixture*)gGeoManager->GetListOfMaterials()->FindObject(matName);
   
   TGeoVolume* magnetMod = 0x0;
   Int_t nMagnets = MAG_N;
   for(Int_t iMag = 0; iMag < nMagnets; iMag++) {
      
      TGeoHMatrix* transf = new TGeoHMatrix();
      Double_t vec[3] = {0, 0, -MAG_DIST/2. +iMag*MAG_DIST};
      transf->SetTranslation(vec);
      TGeoTube* tube = new TGeoTube(Form("TubeMagnet%d", iMag+1), MAG_PM1_INRAD, MAG_PM1_OUTRAD, MAG_PM_THICK/2.);
      
      TGeoVolume *magnetMod = new TGeoVolume(Form("%s_Magnet",basemoduleName), tube, med);
      magnetMod->SetLineColor(kBlue-3);
      magnetMod->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
      
      magnet->AddNode(magnetMod, iMag, transf);
   }
   
   return magnet;
}



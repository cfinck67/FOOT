/*!
  \file
  \version $Id: TADIparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TADIparGeo.
*/

#include <Riostream.h>

#include "TGeoBBox.h"
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

#include "gsi_geo.h"

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
  fWidth(AL_MF_WIDTH_OU),
  fHeight(AL_MF_HEIGHT),
  fLength(AL_THICK),
  fDebugLevel(0)
{
   // Standard constructor
}

//______________________________________________________________________________
TADIparGeo::~TADIparGeo()
{
   // Destructor
}

//_____________________________________________________________________________
TGeoVolume* TADIparGeo::AddDipole(const char *dipoleName)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
	  new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   // create module
   TGeoMaterial* matDipole;
   TGeoMedium*   medDipole;
   
   Float_t ring = 10;
   if ( (matDipole = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Vacuum")) == 0x0 )
	  matDipole = new TGeoMaterial("Vacuum", 0., 0., 0.);
   if ( (medDipole = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
	  medDipole = new TGeoMedium("Vacuum", 1, matDipole);
   
   const Char_t* outerName = Form("%s_outer", dipoleName);
   const Char_t* innerName = Form("%s_inner", dipoleName);
   new TGeoBBox(outerName, TAGgeoTrafo::CmToMu()*fWidth/2., TAGgeoTrafo::CmToMu()*fHeight/2., TAGgeoTrafo::CmToMu()*fLength/2.);
   new TGeoBBox(innerName, TAGgeoTrafo::CmToMu()*(fWidth-ring)/2., TAGgeoTrafo::CmToMu()*(fHeight-ring)/2., TAGgeoTrafo::CmToMu()*(fLength+ring)/2.);
      
   TGeoCompositeShape* shape = new TGeoCompositeShape("Composite", Form("%s-%s", outerName, innerName));
   TGeoVolume * dipole       = new TGeoVolume(dipoleName, shape, medDipole);
   
   dipole->SetVisibility(true);
   dipole->SetLineColor(17);
   dipole->SetTransparency( TAGgeoTrafo::GetDefaultTransp());
   
   return dipole;
}



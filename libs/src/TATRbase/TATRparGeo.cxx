/*!
  \file
  \version $Id: TATRparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TATRparGeo.
*/

#include <string.h>

#include <fstream>

#include "TSystem.h"
#include "TString.h"
#include "TGeoManager.h"
#include "TGeoBBox.h"

#include "TATRparGeo.hxx"
#include "TAGmaterials.hxx"
#include "TAGgeoTrafo.hxx"

//##############################################################################

/*!
  \class TATRparGeo TATRparGeo.hxx "TATRparGeo.hxx"
  \brief Map and Geometry parameters for the start counter. **
*/

ClassImp(TATRparGeo);

const TString TATRparGeo::fgkBaseName    = "ST";
const TString TATRparGeo::fgkDefParaName = "trGeo";

//------------------------------------------+-----------------------------------
//! Default constructor.
TATRparGeo::TATRparGeo()
 : TAGparTools(),
   fSize(),
   fMaterial(""),
   fDensity(0.)
{
}

//------------------------------------------+-----------------------------------
//! Destructor.

TATRparGeo::~TATRparGeo()
{
}

//_____________________________________________________________________________
void TATRparGeo::DefineMaterial()
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   // ST material
   TGeoMaterial* mat = TAGmaterials::Instance()->CreateMaterial(fMaterial, fDensity);
  if (fDebugLevel) {
      printf("Start counter material:\n");
      mat->Print();
   }
}

//______________________________________________________________________________
Bool_t TATRparGeo::FromFile(const TString& name)
{
   TString nameExp;
   
   if (name.IsNull())
      nameExp = fgDefaultGeoName;
   else
      nameExp = name;
   
   if (!Open(nameExp)) return false;
   
   ReadVector3(fSize);
   if(fDebugLevel)
      cout  << "  Size: "
      << Form("%f %f %f", fSize[0], fSize[1], fSize[2]) << endl;
   
   ReadStrings(fMaterial);
   if(fDebugLevel)
      cout  << "   ST material: " <<  fMaterial << endl;
   
   ReadItem(fDensity);
   if(fDebugLevel)
      cout  << "   ST density: " <<  fDensity << endl;

   // Define material
   DefineMaterial();

   return true;
}

//------------------------------------------+-----------------------------------
//! Clear geometry info.

void TATRparGeo::Clear(Option_t*)
{
   return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TATRparGeo::ToStream(ostream& os, Option_t*) const
{
  os << "TATRparGeo " << GetName() << endl;
  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}
//_____________________________________________________________________________
TGeoVolume* TATRparGeo::BuildStartCounter(const char *stName )
{
   // create counter
   const Char_t* matName = fMaterial.Data();
   TGeoMedium*  medST = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName);
   
   TGeoBBox* box = new TGeoBBox(stName, fSize[0]/2., fSize[1]/2., fSize[2]/2.);
   TGeoVolume *start = new TGeoVolume(Form("%s_Mod",stName), box, medST);

   start->SetVisibility(true);
   start->SetLineColor(kRed-5);
   start->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
   
   return start;
}


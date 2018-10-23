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
#include "TGeoTube.h"

#include "TATRparGeo.hxx"
#include "TAGgeoTrafo.hxx"

#include "foot_geo.h"

//##############################################################################

/*!
  \class TATRparGeo TATRparGeo.hxx "TATRparGeo.hxx"
  \brief Map and Geometry parameters for the start counter. **
*/

ClassImp(TATRparGeo);

const TString TATRparGeo::fgkDefParaName = "trGeo";

//------------------------------------------+-----------------------------------
//! Default constructor.
TATRparGeo::TATRparGeo()
 : TAGpara(),
   fThick(0.),
   fRadius(0.)
{
   DefineMaterial();
}

//_____________________________________________________________________________
void TATRparGeo::DefineMaterial()
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   TGeoElementTable* table = gGeoManager->GetElementTable();
   
   // create material
   TGeoMaterial* mat = 0x0;;
   TGeoMedium*   med = 0x0;
   TGeoMixture*  mix = 0x0;;
   
   // EJ-212 Scintillator material from eljen technology
   const Char_t* matName = SCN_MEDIUM.Data();
   if ( (mat = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject(matName)) == 0x0 ) {
      
      TGeoElement* matC = table->GetElement(6);
      TGeoElement* matH = table->GetElement(1);
      
      mix =new TGeoMixture(matName,2, 1.023);
      mix->AddElement(matC, 9);
      mix->AddElement(matH, 10);
   }
   if ( (med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName)) == 0x0 )
      med = new TGeoMedium(matName,1,mat);
}

//------------------------------------------+-----------------------------------
//! Destructor.

TATRparGeo::~TATRparGeo()
{
}

//------------------------------------------+-----------------------------------
void TATRparGeo::InitGeo()
{
   fThick  = STC_THICK;
   fRadius = STC_RAD;
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
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager(TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   // create module
   TGeoMaterial* matST;
   TGeoMedium*   medST;
   
   const Char_t* matName = SCN_MEDIUM.Data();

   if ( (matST = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject(matName)) == 0x0 )
      matST = new TGeoMaterial("Vacuum", 0., 0., 0.);;
   if ( (medST = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName)) == 0x0 )
      medST = new TGeoMedium("Vacuum", 1, matST);
   
   TGeoTube* tube = new TGeoTube(stName, 0, fRadius, fThick/2.);
   
   TGeoVolume *start = new TGeoVolume(Form("%s_Mod",stName), tube, medST);
   
   start->SetVisibility(true);
   start->SetLineColor(kRed-5);
   start->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
   
   return start;
}


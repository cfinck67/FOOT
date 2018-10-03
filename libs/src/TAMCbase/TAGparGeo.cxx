

#include <Riostream.h>

#include "TGeoBBox.h"
#include "TColor.h"
#include "TGeoManager.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TSystem.h"

#include "foot_geo.h"

#include "TAGgeoTrafo.hxx"

#include "TAGparGeo.hxx"

//##############################################################################

/*!
 \class TAGparGeo TAGparGeo.hxx "TAGparGeo.hxx"
 \brief Map and Geometry parameters for vertex. **
 */

ClassImp(TAGparGeo);

const TString TAGparGeo::fgkDevBaseName   = "G";


//______________________________________________________________________________
TAGparGeo::TAGparGeo()
: TAGparTools()
{
   // Standard constructor
}

//______________________________________________________________________________
TAGparGeo::~TAGparGeo()
{
   // Destructor
}

//______________________________________________________________________________
void TAGparGeo::InitGeo()
{
   fBeamParameter.Size = PRIM_RMAX;
   if(fDebugLevel)
      cout << "  Beam size:  "<< fBeamParameter.Size << endl;
   
   fBeamParameter.Shape = PRIM_DIST;
   if(fDebugLevel)
      cout  << "  Beam shape:  "<< fBeamParameter.Shape.Data() << endl;
   
   fBeamParameter.Energy = PRIM_T;
   if(fDebugLevel)
      cout  << "  Beam energy:  "<< fBeamParameter.Energy << endl;
   
   fBeamParameter.AtomicMass = PRIM_A;
   if(fDebugLevel)
      cout  << "  Beam atomic mass:  "<< fBeamParameter.AtomicMass << endl;
   
   fBeamParameter.AtomicNumber = PRIM_Z;
   if(fDebugLevel)
      cout  << "  Beam atomic number:  "<< fBeamParameter.AtomicNumber << endl;
   
   fBeamParameter.PartNumber = 1;
   if(fDebugLevel)
      cout  << "   Number of particles:  "<< fBeamParameter.PartNumber << endl;
	  
   fBeamParameter.Position[0] = PRIM_Pos_X;
   fBeamParameter.Position[1] = PRIM_Pos_Y;
   fBeamParameter.Position[2] = PRIM_Pos_Z;
   if(fDebugLevel)
      cout  << "  Position: "
      << Form("%f %f %f", fBeamParameter.Position[0], fBeamParameter.Position[1], fBeamParameter.Position[2]) << endl;
   
   fBeamParameter.AngSpread[0] = PRIM_Pos_CX;
   fBeamParameter.AngSpread[1] = PRIM_Pos_CY;
   fBeamParameter.AngSpread[2] = 0.;
   if(fDebugLevel)
      cout  << "  Angular spread: "
      << Form("%f %f %f", fBeamParameter.AngSpread[0], fBeamParameter.AngSpread[1], fBeamParameter.AngSpread[2]) << endl;
   
   fBeamParameter.AngDiv = PRIM_DIV;
   if(fDebugLevel)
      cout  << "   Angular divergence:  "<< fBeamParameter.AngDiv << endl;
   
   if(fDebugLevel)
      cout << endl << "Reading target Parameters " << endl;
   
   fTargetParameter.Shape = "cubic";
   if(fDebugLevel)
      cout  << "  Target shape:  "<< fTargetParameter.Shape.Data() << endl;
	  
   fTargetParameter.Size[0] = TG_WIDTH;
   fTargetParameter.Size[1] = TG_HEIGHT;
   fTargetParameter.Size[2] = TG_THICK;
   if(fDebugLevel)
      cout  << "  Size: "
      << Form("%f %f %f", fTargetParameter.Size[0], fTargetParameter.Size[1], fTargetParameter.Size[2]) << endl;
	  
   fTargetParameter.Material = TG_MEDIUM;
   if(fDebugLevel)
      cout  << "  Target material:  "<< fTargetParameter.Material.Data() << endl;
   
   fTargetParameter.Position[0] = TG_X;
   fTargetParameter.Position[1] = TG_Y;
   fTargetParameter.Position[2] = TG_Z;
   if(fDebugLevel)
      cout  << "  Position: "
      << Form("%f %f %f", fTargetParameter.Position[0], fTargetParameter.Position[1], fTargetParameter.Position[2]) << endl;
   
   fInsertsN = 0; // for the moment
   if(fDebugLevel)
      cout  << "Number of inserts:  "<< fInsertsN << endl;

   for (Int_t p = 0; p < fInsertsN; p++) { // Loop on each plane
      
      // read sensor index
      fInsertParameter[p].InsertIdx = 0;
      if(fDebugLevel)
         cout << endl << " - Parameters of Sensor " <<  fInsertParameter[p].InsertIdx << endl;
      
      fInsertParameter[p].Material = "";
      if(fDebugLevel)
         cout  << "  Insert material:  "<< fInsertParameter[p].Material.Data() << endl;
      
      fInsertParameter[p].Shape = "";
      if(fDebugLevel)
         cout  << "  Insert shape:  "<< fInsertParameter[p].Shape.Data() << endl;
      
      fInsertParameter[p].Size[0] = 0;
      fInsertParameter[p].Size[1] = 0;
      fInsertParameter[p].Size[2] = 0;
      
      if(fDebugLevel)
         cout  << "  Size: "
         << Form("%f %f %f", fInsertParameter[p].Size[0], fInsertParameter[p].Size[1], fInsertParameter[p].Size[2]) << endl;
      
      // read sensor position
      fInsertParameter[p].Position[0] = 0;
      fInsertParameter[p].Position[1] = 0;
      fInsertParameter[p].Position[2] = 0;
      if(fDebugLevel)
         cout << "   Position: "
         << Form("%f %f %f", fInsertParameter[p].Position[0], fInsertParameter[p].Position[1], fInsertParameter[p].Position[2]) << endl;
   }
}

//_____________________________________________________________________________
TGeoVolume* TAGparGeo::AddTarget(const char *targetName)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   TGeoVolume* target = 0x0;
   fTargetParameter.Shape.ToLower();
   if (fTargetParameter.Shape.Contains("cubic")) 
      target = AddCubicTarget(targetName);
   else if (fTargetParameter.Shape.Contains("cyl"))
      target =  AddCylindricTarget(targetName);
   else
      Warning("AddTarget", "No target in geometry");
   
   return target;
}

//_____________________________________________________________________________
TGeoVolume* TAGparGeo::BuildTarget(const char *targetName)
{
   return AddTarget(targetName);
}


//_____________________________________________________________________________
TGeoVolume* TAGparGeo::AddCubicTarget(const char *targetName)
{
   // get size
   Float_t dx = fTargetParameter.Size[0]/2.;
   Float_t dy = fTargetParameter.Size[1]/2.;
   Float_t dz = fTargetParameter.Size[2]/2.;
   
   // create module
   TGeoMaterial* matTarget;
   TGeoMedium*   medTarget;
   
   if ( (matTarget = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Vacuum")) == 0x0 )
      matTarget = new TGeoMaterial("Vacuum", 0., 0., 0.);
   if ( (medTarget = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
      medTarget = new TGeoMedium("Vacuum", 1, matTarget);
   
   TGeoVolume* target = gGeoManager->MakeBox(targetName, medTarget, dx, dy, dz);
   target->SetVisibility(true);
   target->SetTransparency( TAGgeoTrafo::GetDefaultTransp());
   
   return target;
}

//_____________________________________________________________________________
TGeoVolume* TAGparGeo::AddCylindricTarget(const char *targetName)
{
   // get size
   Float_t height  = fTargetParameter.Size[0]/2.;
   Float_t degrees = fTargetParameter.Size[1];
   Float_t radius  = fTargetParameter.Size[2]/2.;
   
   // create module
   TGeoMaterial* matTarget;
   TGeoMedium*   medTarget;
   
   if ( (matTarget = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Vacuum")) == 0x0 )
      matTarget = new TGeoMaterial("Vacuum", 0., 0., 0.);
   if ( (medTarget = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
      medTarget = new TGeoMedium("Vacuum", 1, matTarget);
   
   TGeoVolume* tubs = 0x0;
   if (TMath::Abs(degrees-360.) < 0.1)
      tubs = gGeoManager->MakeTube(Form("%s_tubs",targetName), medTarget, 0, radius, height);
   else
      tubs = gGeoManager->MakeTubs(Form("%s_tubs",targetName), medTarget, 0, radius, height, 0, degrees);
   
   TGeoVolume* target = gGeoManager->MakeBox(targetName,medTarget,radius,height,radius); // volume corresponding to vertex
   
   TGeoHMatrix hm;
   hm.RotateX(90);
   target->AddNode(tubs, 1, new TGeoHMatrix(hm));
   
   target->SetVisibility(true);
   target->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
   
   return target;
}

//_____________________________________________________________________________
void TAGparGeo::Print(Option_t* /*opt*/) const
{
   cout << endl << "Reading Beam Parameters " << endl;
   
   cout << "  Beam size:  "<< fBeamParameter.Size << " cm (FWHM)" << endl;
   
   cout  << "  Beam shape:  "<< fBeamParameter.Shape.Data() << endl;
   
   cout  << "  Beam energy:  "<< fBeamParameter.Energy << " MeV" <<endl;
   
   cout  << "  Beam atomic mass:  "<< fBeamParameter.AtomicMass << endl;
   
   cout  << "  Beam atomic number:  "<< fBeamParameter.AtomicNumber << endl;
   
   cout  << "  Number of particles:  "<< fBeamParameter.PartNumber << endl;
	  
   cout  << "  Position: "
   << Form("%f %f %f", fBeamParameter.Position[0], fBeamParameter.Position[1], fBeamParameter.Position[2]) << " cm" << endl;
   
   cout  << "  Angular spread: "
   << Form("%f %f %f", fBeamParameter.AngSpread[0], fBeamParameter.AngSpread[1], fBeamParameter.AngSpread[2]) << " degrees" <<  endl;
   
   cout << endl << "Reading target Parameters " << endl;
   
   cout  << "  Target shape:  "<< fTargetParameter.Shape.Data() << endl;
	  
   cout  << "  Size: "
   << Form("%f %f %f", fTargetParameter.Size[0], fTargetParameter.Size[1], fTargetParameter.Size[2]) << " cm" << endl;
	  
   cout  << "  Target material:  "<< fTargetParameter.Material.Data() << endl;
   
   cout << endl;
   cout  << "Number of inserts:  "<< fInsertsN << endl;
   
   for (Int_t p = 0; p < fInsertsN; p++) { // Loop on each plane
      
      cout << endl << " - Parameters of Sensor " <<  fInsertParameter[p].InsertIdx << endl;
      
      cout  << "  Insert material:  "<< fInsertParameter[p].Material.Data() << endl;
      
      cout  << "  Insert shape:  "<< fInsertParameter[p].Shape.Data() << endl;
      
      cout  << "  Size: "
      << Form("%f %f %f", fInsertParameter[p].Size[0], fInsertParameter[p].Size[1], fInsertParameter[p].Size[2]) << " cm" <<  endl;
      
      cout << "   Position: "
      << Form("%f %f %f", fInsertParameter[p].Position[0], fInsertParameter[p].Position[1], fInsertParameter[p].Position[2]) << " cm" << endl;
   }
   cout << endl;

}



#include "TCCAgeometryConstructor.hxx"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "TACAparGeo.hxx"
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"

#include "TCCAsensitiveDetector.hxx"

#include "TMath.h"
#include "TRandom3.h"
#include "TString.h"

using namespace CLHEP;

TString TCCAgeometryConstructor::fgkCalSDname  = "caSD";

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCCAgeometryConstructor::TCCAgeometryConstructor(TACAparGeo* pParGeo)
: TCGbaseConstructor("TCCAgeometryConstructor", "1.0"),
  fBoxLog(0x0),
  fpParGeo(pParGeo)
{
   DefineMaxMinDimension();
   DefineMaterial();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCCAgeometryConstructor::~TCCAgeometryConstructor()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* TCCAgeometryConstructor::Construct()
{
   Info("Construct()", "Construct Calorimeter");

   for(Int_t i = 0; i< 3; ++i)
      fSizeBoxCal[i] = (fMaxPosition[i] - fMinPosition[i]);
   
   // Vtx box
   G4Material* vacuum = G4NistManager::Instance()->FindOrBuildMaterial("Vacuum");
   G4Material* matBGO = G4NistManager::Instance()->FindOrBuildMaterial("BGO");
   G4Box* boxCal = new G4Box("boxMag", 0.5*fSizeBoxCal.X(), 0.5*fSizeBoxCal.Y(), 0.5*fSizeBoxCal.Z());
   
   fBoxLog = new G4LogicalVolume(boxCal, vacuum, "boxCaloLog");
   fBoxLog->SetVisAttributes(G4VisAttributes::Invisible);
   
   Float_t width    = fpParGeo->GetCrystalWidth()*cm;
   Float_t thick    = fpParGeo->GetCrystalThick()*cm;
   Int_t crystalsN = fpParGeo->GetCrystalsN();
   
   //logical
   G4Box* calo    = new G4Box("Crystal", width/2., width/2., thick/2.);
   fCalLog = new G4LogicalVolume(calo, matBGO, "CrystalLog");
   
   G4VisAttributes* targetLog_att = new G4VisAttributes(G4Colour(0.,0.3,1)); //light blue
   targetLog_att->SetForceWireframe(true);
   fCalLog->SetVisAttributes(targetLog_att);
   
   for (Int_t i = 0; i < crystalsN; ++i) {
      
      TVector3 pos = fpParGeo->GetCrystalPosition(i)*cm;
      new G4PVPlacement(0, G4ThreeVector(pos[0], pos[1], 0), fCalLog, "Calo", fBoxLog, false, i + 1);
   }
   
   DefineSensitive();

   return fBoxLog;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void  TCCAgeometryConstructor::DefineSensitive()
{
   // sensitive volume
   G4SDManager* SDman = G4SDManager::GetSDMpointer();
   
   G4String calSDname;
   calSDname = fgkCalSDname;
   Info("DefineSensitive()", "Define sensitive for Calorimeter");
  
   TCCAsensitiveDetector* calSensitive = new TCCAsensitiveDetector(calSDname);
   calSensitive->SetCopyLevel(0);
   SDman->AddNewDetector(calSensitive);
   fCalLog->SetSensitiveDetector(calSensitive);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCCAgeometryConstructor::DefineMaxMinDimension()
{

   TVector3 size  = fpParGeo->GetCrystalSize();
   Float_t  thick = fpParGeo->GetCrystalThick();

   TVector3 minPosition(10e10, 10e10, 10e10);
   TVector3 maxPosition(-10e10, -10e10, -10e10);
   
   
   minPosition[0] = -size[1]/2.; maxPosition[0] = +size[1]/2.;
   minPosition[1] = -size[1]/2.; maxPosition[1] = +size[1]/2.;
   minPosition[2] = -thick/2.;   maxPosition[2] = +thick/2.;

   fMinPosition = minPosition*cm;
   fMaxPosition = maxPosition*cm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCCAgeometryConstructor::DefineMaterial()
{
   G4double density;
   G4int    ncomponents;
   G4String name;
   
   G4double z, a;

   // BGO
   if (fpParGeo->GetCrystalMat().Contains("BGO")) {
      G4Element* O  = new G4Element("Oxygen"  , "O",  z= 8, a= 16.00*g/mole);
      G4Element* Ge = new G4Element("Germanum", "Ge", z=32, a= 72.64*g/mole);
      G4Element* Bi = new G4Element("Bismuth",  "Bi", z=83, a= 208.98*g/mole);

      G4Material* BGO = new G4Material(name = "BGO", density=7.13*g/cm3, ncomponents=3);
      BGO->AddElement(O,12);
      BGO->AddElement(Bi,4);
      BGO->AddElement(Ge,3);
   } else
      printf("\n\nCalorimeter crystal material %s not defined !!! \n\n",fpParGeo->GetCrystalMat().Data());
}



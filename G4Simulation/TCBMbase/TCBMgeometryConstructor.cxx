
#include "TCBMgeometryConstructor.hxx"

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

#include "TABMparGeo.hxx"
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"

#include "TCBMsensitiveDetector.hxx"

#include "TMath.h"
#include "TRandom3.h"
#include "TString.h"

using namespace CLHEP;

TString TCBMgeometryConstructor::fgkBmSDname  = "bmSD";

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCBMgeometryConstructor::TCBMgeometryConstructor(TABMparGeo* pParGeo)
: TCGbaseConstructor("TCBMgeometryConstructor", "1.0"),
  fBoxLog(0x0),
  fpParGeo(pParGeo)
{
   DefineMaxMinDimension();
   DefineMaterial();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCBMgeometryConstructor::~TCBMgeometryConstructor()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* TCBMgeometryConstructor::Construct()
{
   Info("Construct()", "Construct Beam Monitor");

   for(Int_t i = 0; i< 3; ++i)
      fSizeBoxBm[i] = (fMaxPosition[i] - fMinPosition[i]);
   
   // BM box
   G4Material* matSense = G4NistManager::Instance()->FindOrBuildMaterial("Tungsten");
   G4Material* matField = G4NistManager::Instance()->FindOrBuildMaterial("Aluminium");
   G4Material* matGas   = G4NistManager::Instance()->FindOrBuildMaterial("ArCO2");
   
   G4Box* boxBm         = new G4Box("boxBm", 0.5*fSizeBoxBm.X(), 0.5*fSizeBoxBm.Y(), 0.5*fSizeBoxBm.Z());
   fBoxLog              = new G4LogicalVolume(boxBm, matGas, "boxBmLog");
   
   G4VisAttributes* bmLog_att = new G4VisAttributes(G4Colour(0.3,0.,0.)); //light red
   bmLog_att->SetForceWireframe(true);
   fBoxLog->SetVisAttributes(bmLog_att);
   
   Float_t senseRad     = fpParGeo->GetSenseRad()*cm;
   Float_t fieldRad     = fpParGeo->GetFieldRad()*cm;
   Float_t width        = fpParGeo->GetWidth()*cm;
   
   Int_t layersN        = fpParGeo->GetLayersN();
   Int_t wiresN         = fpParGeo->GetWiresN();
   
   
   //logical
   G4Tubs* senseWire    = new G4Tubs("SenseWire", 0, senseRad, width/2., 0, 360.);
   G4LogicalVolume* senseWireLog = new G4LogicalVolume(senseWire, matSense, "SenseWireLog");
   
   G4Tubs* fieldWire    = new G4Tubs("FieldWire", 0, fieldRad, width/2., 0, 360.);
   G4LogicalVolume* fieldWireLog = new G4LogicalVolume(fieldWire, matField, "FieldWireLog");
   
   G4VisAttributes* senseLog_att = new G4VisAttributes(G4Colour(1.,0.,0.)); // red
   senseWireLog->SetVisAttributes(senseLog_att);
   
   G4VisAttributes* fieldLog_att = new G4VisAttributes(G4Colour(0.,0.,1)); // blue
   fieldWireLog->SetVisAttributes(fieldLog_att);
   
   G4RotationMatrix* rotX = new G4RotationMatrix;
   rotX->rotateZ(0);
   rotX->rotateY(90*TMath::DegToRad());
   rotX->rotateX(90*TMath::DegToRad());
   rotX->invert(); // inversion
   
   G4RotationMatrix* rotY = new G4RotationMatrix;
   rotY->rotateZ(0);
   rotY->rotateX(0);
   rotY->rotateY(90*TMath::DegToRad());
   rotY->invert(); // inversion


   // view 0
   Int_t view = 0;
   Int_t copy = 0;
   for (Int_t i = 0; i < layersN; ++i) {
      for (Int_t j = 0; j < wiresN; ++j) {
         TVector3 pos = fpParGeo->GetWirePos(view, i, j)*cm;
         
         if (j ==  fpParGeo->GetSenseId(0) || j ==  fpParGeo->GetSenseId(1) || j ==  fpParGeo->GetSenseId(2))
            new G4PVPlacement(rotX, G4ThreeVector(0, pos[1], pos[2]), senseWireLog, "BM", fBoxLog, false, copy++);
         else
            new G4PVPlacement(rotX, G4ThreeVector(0, pos[1], pos[2]), fieldWireLog, "BM", fBoxLog, false, copy++);
      }
   }
   
   view = 1;
   for (Int_t i = 0; i < layersN; ++i) {
      for (Int_t j = 0; j < wiresN; ++j) {
         TVector3 pos = fpParGeo->GetWirePos(view, i, j)*cm;

         if (j ==  fpParGeo->GetSenseId(0) || j ==  fpParGeo->GetSenseId(1) ||  j ==  fpParGeo->GetSenseId(2))
            new G4PVPlacement(rotY, G4ThreeVector(pos[0], 0, pos[2]), senseWireLog, "BM", fBoxLog, false, copy++);
         else
            new G4PVPlacement(rotY, G4ThreeVector(pos[0], 0, pos[2]), fieldWireLog, "BM", fBoxLog, false, copy++);
      }
   }

   
   DefineSensitive();

   return fBoxLog;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void  TCBMgeometryConstructor::DefineSensitive()
{
   // sensitive volume
   G4SDManager* SDman = G4SDManager::GetSDMpointer();
   
   G4String bmSDname;
   bmSDname = fgkBmSDname;
   Info("DefineSensitive()", "Define sensitive for Beam Monitor");
  
   TCBMsensitiveDetector* bmSensitive = new TCBMsensitiveDetector(bmSDname);
   SDman->AddNewDetector(bmSensitive);
   fBoxLog->SetSensitiveDetector(bmSensitive);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCBMgeometryConstructor::DefineMaxMinDimension()
{

   TVector3 size  = fpParGeo->GetSide();
   
   TVector3 minPosition(10e10, 10e10, 10e10);
   TVector3 maxPosition(-10e10, -10e10, -10e10);
   
   
   minPosition[0] = -size[0]/2.; maxPosition[0] = +size[0]/2.;
   minPosition[1] = -size[1]/2.; maxPosition[1] = +size[1]/2.;
   minPosition[2] = -size[2]/2.; maxPosition[2] = +size[2]/2.;

   fMinPosition = minPosition*cm;
   fMaxPosition = maxPosition*cm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCBMgeometryConstructor::DefineMaterial()
{
   G4double A, Z;
   G4double density, fractionmass;
   G4int ncomponents, natoms;
   G4String name;
   
   G4Element* C  = new G4Element("Carbon",    "C",  Z=6.,  A= 12.011*g/mole);
   G4Element* O  = new G4Element("Oxygen",    "O",  Z=8.,  A= 15.9994*g/mole);
   G4Element* Ar = new G4Element("Argon",     "Ar", Z=18., A= 39.948*g/mole);
   G4Element* Al = new G4Element("Aluminium", "Al", Z=18., A= 26.98*g/mole);
   G4Element* W  = new G4Element("Tungsten",  "W",  Z=74., A= 183.84*g/mole);

   TString senseWire  = fpParGeo->GetSenseMat();
   TString fieldWire  = fpParGeo->GetFieldMat();

   // Tungsten
   G4Material* Tungsten = 0x0;
   if (senseWire.Contains("W")) {
      Tungsten = new G4Material(name="Tungsten", density=19.3*g/cm3, ncomponents=1);
      Tungsten->AddElement(W, 1);
   }
   
   // Aluminium
   G4Material* Aluminium = 0x0;
   if (fieldWire.Contains("Al")) {
      Aluminium = new G4Material(name="Aluminium", density=2.70*g/cm3, ncomponents=1);
      Aluminium->AddElement(Al, 1);
   }
   
   // Mixture
   TString gasMixture = fpParGeo->GetGasMixture();
   Int_t pos    = gasMixture.First("/");
   Int_t len    = gasMixture.Length();
   TString gas1 = gasMixture(0, pos);
   TString gas2 = gasMixture(pos+1, len-pos);
   
   TString gasProp = fpParGeo->GetGasProp();
   pos = gasProp.First("/");
   len = gasProp.Length();
   
   TString gasProp1 = gasProp(0, pos);
   Float_t prop1    = gasProp1.Atof();
   TString gasProp2 = gasProp(pos+1, len-pos);
   Float_t prop2    = gasProp2.Atof();

   G4Material* Argon  = 0x0;
   if (gas1.Contains("Ar")) {
      // Argon
      Argon = new G4Material(name="Argon", density=1.7836*mg/cm3, ncomponents=1, kStateGas);
      Argon->AddElement(Ar, 1);
   } else
      printf("No gas %s defined for mixture", gas1.Data());
   
   G4Material* CO2 = 0x0;
   if (gas2.Contains("CO2")) {
      // CO2
      CO2 =  new G4Material("CO2", density=1.87*mg/cm3, ncomponents=2, kStateGas);
      CO2->AddElement(C,natoms = 1);
      CO2->AddElement(O,natoms = 2);

   } else
      printf("No gas %s defined for mixture", gas2.Data());

   if (gas1.Contains("Ar") && gas2.Contains("CO2")) {
   
      G4Material* ArCO2 = G4NistManager::Instance()->FindOrBuildMaterial("ArCO2");
      if (ArCO2 == 0x0) {
         density = (1.784*prop1 + 1.87*prop2)*mg/cm3; // 0.8Ar + 0.2CO2 = (1.784*0.8 + 1.87*0.2) = 1.801
         ArCO2 = new G4Material(name="ArCO2", density, ncomponents=2);
         ArCO2->AddMaterial(Argon, fractionmass = prop1);
         ArCO2->AddMaterial (CO2,  fractionmass = prop2);
      }
   }
}



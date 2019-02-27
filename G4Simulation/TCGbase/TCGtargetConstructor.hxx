//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef TCGtargetConstructor_h
#define TCGtargetConstructor_h 1

#include "TCGbaseConstructor.hxx"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4String.hh"

#include "TString.h"
#include "TVector3.h"

class G4LogicalVolume;
class G4Material;
class TAGparGeo;

//class TAGtargetMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TCGtargetConstructor : public TCGbaseConstructor
{
public:
   TCGtargetConstructor(TAGparGeo* parGeo);
   ~TCGtargetConstructor();
   
public:
   G4LogicalVolume* Construct();

   G4Material*  GetMaterial()       { return fTargetMat; }

   void SetInsertMaterialName(Int_t insert, const char* material) ;
   void SetInsertShape(Int_t insert, const char* shape);
   void SetInsertAttributes(G4int insert,G4String material) ;
   void SetInsertSize(Int_t insert, G4double v1,  G4double v2, G4double v3) ;
   void SetInsertPosition(Int_t insert, G4double v1,  G4double v2, G4double v3) ;
   void SetInsertNumber(G4int value);
   
public:
   static const Char_t* GetTargetName()    { return fgkTargetName.Data();  }

private:
   void BuildTarget();
   void BuildCubicTarget(); //in the file config 1
   void BuildCylindricTarget(); // in the file config 2
   void BuildInsert();

   void DefineMaterial();
   
private:
   TAGparGeo*          fpParGeo;
   G4VPhysicalVolume*  fTargetPhy;
   G4LogicalVolume*    fTargetLog;
   
//   TAGtargetMessenger* fTargetMessenger;
   G4Material*         fTargetMat;
      
   //Insert parameter
   G4LogicalVolume*     fInsertLog[20];
   
private:
   //Insert parameter
   static TString  fgkTargetName;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


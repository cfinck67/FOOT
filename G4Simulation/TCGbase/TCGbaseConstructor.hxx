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

#ifndef TCGbaseConstructor_h
#define TCGbaseConstructor_h 1

#include "TString.h"
#include "G4String.hh"

#include "TNamed.h"

class G4LogicalVolume;
class G4VPhysicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class TCGbaseConstructor : public TNamed
{
public:
   TCGbaseConstructor(const Char_t* name = "TCGbaseConstructor", const Char_t* version = "1.0");
   virtual ~TCGbaseConstructor();
	
	G4LogicalVolume*   GetLogMother()                      { return fLogMother;           }
	void               SetLogMother(G4LogicalVolume* w)    { fLogMother = w;              }
   
   G4VPhysicalVolume* GetPhysMother()                     { return fPhysMother;          }
   void               SetPhysMother(G4VPhysicalVolume* w) { fPhysMother = w;             }
   

   virtual G4LogicalVolume* Construct()                   { return 0x0;                  }
   virtual G4LogicalVolume* GetLogicVolume() const        { return fLogMother;           }// default wise
   
   virtual void             DefineMaterial()               { return;                     }

   
protected:
   G4LogicalVolume*    fLogMother;
   G4VPhysicalVolume*  fPhysMother;
   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


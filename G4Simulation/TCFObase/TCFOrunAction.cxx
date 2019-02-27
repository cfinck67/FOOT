//Run Action

#include "TFile.h"

#include "TCFOrunAction.hxx"

#include "TAGgeoTrafo.hxx"
#include "G4Run.hh"

#include "TAGactTreeWriter.hxx"

#include "TATRparGeo.hxx"
#include "TABMparGeo.hxx"
#include "TAVTparGeo.hxx"
#include "TAITparGeo.hxx"
#include "TAMSDparGeo.hxx"
#include "TATWparGeo.hxx"
#include "TACAparGeo.hxx"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCFOrunAction::TCFOrunAction()
: TCGbaseRunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCFOrunAction::~TCFOrunAction()
{
   delete fpEventMC;
   delete fpEveStruct;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOrunAction::SetContainers()
{
   //File for
   fpOutFile = new TFile(GetRootFileName(), "RECREATE");
   if (!fpOutFile) return;
   
   fpTree = new TTree("EventTree", "FOOT");

   fpEveStruct = new EVENT_STRUCT;
   
   fpEventMC = new Evento();
   fpEventMC->SetBranches(fpTree, fpEveStruct);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOrunAction::ClearContainers()
{
   fpEventMC->Clean();
}



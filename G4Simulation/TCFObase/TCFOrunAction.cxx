//Run Action

#include "TFile.h"

#include "TCFOrunAction.hxx"

#include "TAGgeoTrafo.hxx"
#include "G4Run.hh"

#include "TAGactTreeWriter.hxx"

#include "TASTparGeo.hxx"
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOrunAction::SetContainers()
{
   //File for
   fpOutFile = new TFile(GetRootFileName(), "RECREATE");
   if (!fpOutFile) return;
   fpOutFile->cd();
   
   fpTree      = new TTree("EventTree", "FOOT");
   fpEventMC   = new Evento();
   
   fpEventMC->SetBranches(fpTree);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOrunAction::ClearContainers()
{
   fpEventMC->Clean();
}



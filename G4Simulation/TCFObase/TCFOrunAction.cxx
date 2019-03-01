//Run Action

#include "TFile.h"

#include "TCFOrunAction.hxx"

#include "TAGgeoTrafo.hxx"
#include "G4Run.hh"

#include "TFile.h"
#include "TTree.h"

#include "TAGactTreeWriter.hxx"

#include "TASTparGeo.hxx"
#include "TABMparGeo.hxx"
#include "TAVTparGeo.hxx"
#include "TAITparGeo.hxx"
#include "TAMSDparGeo.hxx"
#include "TATWparGeo.hxx"
#include "TACAparGeo.hxx"

TString TCFOrunAction::fgRootFileName = "ionCa1000.root";

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCFOrunAction::TCFOrunAction()
: G4UserRunAction(),
fpOutFile(0x0),
fpTree(0x0),
fEventsNToBeProcessed(-1)
{
    G4cout<<"Construct Run Action"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCFOrunAction::~TCFOrunAction()
{
   G4cout<<"Distructor of my class run Action "<<G4endl;
   delete fpEventMC;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOrunAction::BeginOfRunAction(const G4Run* aRun)
{
    fEventsNToBeProcessed = aRun->GetNumberOfEventToBeProcessed();

    G4cout <<" starts run number "<< aRun->GetRunID() << G4endl;
    G4cout <<" number of events to process "<< fEventsNToBeProcessed << G4endl;

    if (gTAGroot == 0x0) {
        G4cerr << "not TAGroot defined" << G4endl;
        return;
    }

    fWatch.Start();

    gTAGroot->SetCampaignNumber(100);
    gTAGroot->SetRunNumber(aRun->GetRunID()); //number of run

    SetContainers();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOrunAction::EndOfRunAction(const G4Run* aRun)
{
    G4cout<<"The total number of events number of events is "<<aRun->GetNumberOfEvent()<<G4endl;

    //close file
    fpTree->Write();
    fpOutFile->Close();
    fWatch.Print();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOrunAction::FillAndClear()
{
    fpTree->Fill();
    ClearContainers();
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



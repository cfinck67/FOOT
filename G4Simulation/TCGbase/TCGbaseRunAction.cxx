//Run Action

#include "TCGbaseRunAction.hxx"
#include "G4Run.hh"

#include "TFile.h"
#include "TTree.h"

TString TCGbaseRunAction::fgRootFileName = "ionCa1000.root";

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCGbaseRunAction::TCGbaseRunAction()
:  G4UserRunAction(),
   fpOutFile(0x0),
   fpTree(0x0),
   fEventsNToBeProcessed(-1)
{
   G4cout<<"Construct Run Action"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCGbaseRunAction::~TCGbaseRunAction()
{
   G4cout<<"Distructor of my class run Action "<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCGbaseRunAction::BeginOfRunAction(const G4Run* aRun)
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
void TCGbaseRunAction::EndOfRunAction(const G4Run* aRun)
{
   G4cout<<"The total number of events number of events is "<<aRun->GetNumberOfEvent()<<G4endl;
   
   //close file
 //  fpOutFile->Print();
   fpOutFile->Close();
   fWatch.Print();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCGbaseRunAction::FillAndClear()
{
   ClearContainers();
}

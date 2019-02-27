
#include "TCGbaseEventAction.hxx"

#include "TVector3.h"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "Evento.hxx"
#include "TCGmcHit.hxx"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TAGeventInterruptHandler::TAGeventInterruptHandler()
: TSignalHandler(kSigInterrupt, kFALSE)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Bool_t TAGeventInterruptHandler::Notify()
{
   G4RunManager::GetRunManager()->AbortRun(false);
   return kTRUE;
}

//
//---------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCGbaseEventAction::TCGbaseEventAction()
: G4UserEventAction(),
  fDebugLevel(0)
{
   fEventInterruptHandler = new TAGeventInterruptHandler();
   fEventInterruptHandler->Add();
   
   if (fDebugLevel >0 )
      G4cout<<"Construct event action "<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCGbaseEventAction::~TCGbaseEventAction()
{
   if(fDebugLevel > 0)
      G4cout<<"Distructor Event Action "<<G4endl;
   
   delete fEventInterruptHandler;
   
   if(fDebugLevel > 0)
      G4cout<<"Out Destructor Event Action "<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCGbaseEventAction::BeginOfEventAction(const G4Event* evt)
{
   if(fDebugLevel > 0)
      G4cout<<"********************************************************************Begin event actions "<<evt->GetEventID()<<G4endl;
   
   static Int_t frequency = 1;
   static Int_t max       = 0;
   static Bool_t hasDigit = false;
   
   if(max == 0) {
      max = GetEventsNToBeProcessed();
      
      if( max > 100000)      frequency = 10000;
      else if( max >= 10000) frequency = 1000;
      else if( max >= 1000)  frequency = 100;
      else if( max >= 100)   frequency = 10;
   }
   
   if(evt->GetEventID() % frequency == 0)
      G4cout<<evt->GetEventID()<<G4endl;
   
   if (!hasDigit) {
      ConstructCollection();
      hasDigit = true;
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCGbaseEventAction::EndOfEventAction(const G4Event* evt)
{
   // digitize evt
   Collect(evt);
   
   //At the end of each EVENT
   FillAndClear();
}


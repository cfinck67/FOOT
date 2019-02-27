
#include "TCFOeventAction.hxx"

//include hit class
//Endo of event:
//collection hits: all hits of this collections contains the information about what happened in the sensitive volume (epitaxial layer)
//each hit represents a step and in the epi layer there may have been many
//than the position to send to CmosDigitizer is the PosIn of the first hits and the pos out of the last hits
//the energy deposited is the sum of the energy deposited in each step
//All positon in microns and all energy in eV


#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"

#include "TCFOgeometryConstructor.hxx"

#include "TCSTgeometryConstructor.hxx"
#include "TCBMgeometryConstructor.hxx"
#include "TCVTgeometryConstructor.hxx"
#include "TCITgeometryConstructor.hxx"
#include "TCMSDgeometryConstructor.hxx"
#include "TCTWgeometryConstructor.hxx"
#include "TCCAgeometryConstructor.hxx"

#include "TCFOrunAction.hxx"

#include "TATRparGeo.hxx"
#include "TABMparGeo.hxx"
#include "TAVTparGeo.hxx"
#include "TAITparGeo.hxx"
#include "TAMSDparGeo.hxx"
#include "TATWparGeo.hxx"
#include "TACAparGeo.hxx"

#include "Evento.hxx"
#include "TCGmcHit.hxx"

#include "TCGbaseRunAction.hxx"
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"

#include "TClonesArray.h"
#include "TRandom.h"
#include "TGeoMatrix.h"
#include "TFile.h"

//
//---------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCFOeventAction::TCFOeventAction(TCGbaseRunAction* runAction, TCGbaseGeometryConstructor* footGeomConstructor)
: TCGbaseEventAction(),
  fIrCollId(-1),
  fBmCollId(-1),
  fVtxCollId(-1),
  fItCollId(-1),
  fMsdCollId(-1),
  fTwCollId(-1),
  fCaCollId(-1)
{
   fFootGeomConstructor = (TCFOgeometryConstructor*)footGeomConstructor;
   fRunAction           = (TCFOrunAction*)runAction;
   fpGeoTrafo           = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCFOeventAction::~TCFOeventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOeventAction::ConstructCollection()
{
   G4SDManager * SDman = G4SDManager::GetSDMpointer();

   if (GlobalPar::GetPar()->IncludeST()) {
      fIrCollId = SDman->GetCollectionID(TCSTgeometryConstructor::GetSDname());
   }
   
   if (GlobalPar::GetPar()->IncludeBM()) {
      fBmCollId = SDman->GetCollectionID(TCBMgeometryConstructor::GetSDname());
   }
   
   if (GlobalPar::GetPar()->IncludeVertex()) {
      fVtxCollId = SDman->GetCollectionID(TCVTgeometryConstructor::GetSDname());
   }
   
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fItCollId = SDman->GetCollectionID(TCITgeometryConstructor::GetSDname());
   }
   
   if (GlobalPar::GetPar()->IncludeMSD()) {
      fMsdCollId = SDman->GetCollectionID(TCMSDgeometryConstructor::GetSDname());
   }
   
   if (GlobalPar::GetPar()->IncludeTW()) {
      fTwCollId = SDman->GetCollectionID(TCTWgeometryConstructor::GetSDname());
   }
   
   if (GlobalPar::GetPar()->IncludeCA()) {
      fCaCollId = SDman->GetCollectionID(TCCAgeometryConstructor::GetSDname());
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t TCFOeventAction::GetEventsNToBeProcessed()
{
   return fRunAction->GetEventsNToBeProcessed();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOeventAction::FillAndClear()
{
   return fRunAction->FillAndClear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOeventAction::Collect(const G4Event* evt)
{
   
   if (fIrCollId >= 0)
      GetHitPerPlane(evt, fIrCollId);
   
   if (fBmCollId >= 0)
      GetHitPerPlane(evt, fBmCollId);
   
   if (fVtxCollId >= 0)
      GetHitPerPlane(evt, fVtxCollId);
   
   if (fItCollId >= 0)
      GetHitPerPlane(evt, fItCollId);
   
   if (fMsdCollId >= 0)
      GetHitPerPlane(evt, fMsdCollId);
   
   if (fTwCollId >= 0)
      GetHitPerPlane(evt, fTwCollId);
   
   if (fCaCollId >= 0)
      GetHitPerPlane(evt, fCaCollId);
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOeventAction::GetHitPerPlane(const G4Event* evt, G4int idColl)
{
   G4HCofThisEvent*  hitCollEv = evt->GetHCofThisEvent();
   TCGmcCollections* hitList   = (TCGmcCollections*)(hitCollEv->GetHC(idColl));
   Evento*            hit      = fRunAction->GetEventMC();
   
   Int_t entries =  hitList->entries();

   if (fDebugLevel)
      printf("IdColl %d entries %d\n", idColl, entries);

   for (Int_t i = 0; i < entries; ++i) {
      
      TCGmcHit* mcHit = (*hitList)[i];
      
      FillHits(hit, mcHit);

      if (fDebugLevel) {
         Int_t  curSensor = mcHit->GetSensorId();
         printf("IdCool %d id: %d\n", idColl, curSensor);
     }
      
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOeventAction::FillHits(Evento* hit, TCGmcHit* mcHit)
{
   G4ThreeVector vin = mcHit->GetPosIn();
   G4ThreeVector vou = mcHit->GetPosOut();
   G4ThreeVector pin = mcHit->GetMomIn();
   G4ThreeVector pou = mcHit->GetMomOut();

   Int_t    sensorId = mcHit->GetSensorId();
   Int_t    trackId  = mcHit->GetTrackId();
   Double_t edep     = mcHit->GetEdep()*TAGgeoTrafo::MevToGev(); 
   Double_t time     = mcHit->GetGlobalTime()*TAGgeoTrafo::NsToSec();
   Double_t al       = 0;
   
   if (fIrCollId >= 0)
      hit->AddSTC(trackId, vin[0], vin[1], vin[2], vou[0], vou[1], vou[2], pin[0], pin[1], pin[2], pou[0], pou[1], pou[2],
                  edep, al, time);
   
   if (fBmCollId >= 0) {
      Int_t layer = -1;
      Int_t view   = -1;
      Int_t cell  = -1;
      hit->AddBMN(trackId,layer, view, cell,
                  vin[0], vin[1], vin[2], vou[0], vou[1], vou[2], pin[0], pin[1], pin[2], pou[0], pou[1], pou[2],
                  edep, al, time);
   }
   
   if (fVtxCollId >= 0) {
      Int_t layer = sensorId;
      Int_t row   = -1;
      Int_t line  = -1;
      hit->AddVTX(trackId, layer, row, line,
                  vin[0], vin[1], vin[2], vou[0], vou[1], vou[2], pin[0], pin[1], pin[2], pou[0], pou[1], pou[2],
                  edep, al, time);
   }
   
   if (fItCollId >= 0) {
      Int_t layer = sensorId;
      Int_t row   = -1;
      Int_t line  = -1;
      Int_t plume = -1;
      Int_t mimo  = -1;
      hit->AddITR(trackId, layer, row, line, plume, mimo,
                  vin[0], vin[1], vin[2], vou[0], vou[1], vou[2], pin[0], pin[1], pin[2], pou[0], pou[1], pou[2],
                  edep, al, time);
   }
   
   if (fMsdCollId >= 0) {
      Int_t layer  = sensorId;
      Int_t stripx = -1;
      Int_t stripy = -1;
      hit->AddMSD(trackId, layer, stripx, stripy,
                  vin[0], vin[1], vin[2], vou[0], vou[1], vou[2], pin[0], pin[1], pin[2], pou[0], pou[1], pou[2],
                  edep, al, time);
   }
   
   if (fTwCollId >= 0) {
      Int_t barId  = sensorId % TATWparGeo::GetLayerOffset();
      Int_t view  = sensorId /  TATWparGeo::GetLayerOffset();
      hit->AddSCN(trackId, barId, view,
                  vin[0], vin[1], vin[2], vou[0], vou[1], vou[2], pin[0], pin[1], pin[2], pou[0], pou[1], pou[2],
                  edep, al, time);
   }
   
   if (fCaCollId >= 0) {
      Int_t crystalId  = sensorId;
      hit->AddCAL(trackId, crystalId,
                  vin[0], vin[1], vin[2], vou[0], vou[1], vou[2], pin[0], pin[1], pin[2], pou[0], pou[1], pou[2],
                  edep, al, time);

   }

}





//
// Documentation : See doc.html (readme, main.cc, ...)
// Convention    : Root coding convention: TMyClass for classes, fMyVariable for class data members, kMyVariable for constants, gMyVariable for global variables
// Help          : http://www.lcsim.org/software/geant4/doxygen/html/classes.html
//


#include "TCFOtrackingAction.hxx"
#include "G4TrackingManager.hh"

#include "G4RunManager.hh"

#include "G4ParticleDefinition.hh"
#include "TVector3.h"

using namespace CLHEP;
using namespace std;

//
//---------------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCFOtrackingAction::TCFOtrackingAction(TCFOeventAction* aEventAction)
{
    fEventAction = aEventAction ;
}

//
//---------------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TCFOtrackingAction::~TCFOtrackingAction()
{
}

//
//---------------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOtrackingAction::PreUserTrackingAction(const G4Track*)
{
}

//
//---------------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TCFOtrackingAction::PostUserTrackingAction(const G4Track* aTrack){

//    TString partName = aTrack->GetDefinition()->GetParticleName() ;

    Int_t flukaID = -5 ;
    Int_t trackID = aTrack->GetTrackID() ;
    Int_t parentID = aTrack->GetParentID() ;
    Int_t charge = aTrack->GetDefinition()->GetAtomicNumber() ;
    Double_t mass = aTrack->GetDefinition()->GetPDGMass() ;
    Double_t tof = aTrack->GetLocalTime() ;
    Double_t time = aTrack->GetProperTime() ;
    Double_t length = aTrack->GetTrackLength() ;

    TVector3 vtxpos ; /// position of the vertex of the track
    vtxpos.SetX(aTrack->GetVertexPosition().getX());
    vtxpos.SetY(aTrack->GetVertexPosition().getY());
    vtxpos.SetZ(aTrack->GetVertexPosition().getZ());
    TVector3 vtxmom ; /// momentum of the track at the vertex
    vtxmom.SetX(aTrack->GetMomentum().getX());
    vtxmom.SetY(aTrack->GetMomentum().getY());
    vtxmom.SetZ(aTrack->GetMomentum().getZ());

    TVector3 xpos = TVector3(0,0,0);
    TVector3 xmom = TVector3(0,0,0);
    TVector3 xposparent = TVector3(0,0,0);  /// position of the parent particle - not needed, so initialized to zero

//    G4ThreeVector = aTrack->GetVertexPosition() ;
    fEventAction->GetTrackMc()->NewHit(flukaID,charge,trackID,-1,-1,-1,mass,parentID,time,tof,length,vtxpos,xpos,vtxmom,xmom,xposparent,xposparent,-1);
}






//my Sensitive detector

#ifndef TCGbaseSensitiveDetector_h
#define TCGbaseSensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "TCGmcHit.hxx" //my hit class

class G4Step; //step class
class G4HCofThisEvent;
class TCGmcHit;

class TCGbaseSensitiveDetector : public G4VSensitiveDetector
{
public:
   TCGbaseSensitiveDetector(G4String);
   virtual ~TCGbaseSensitiveDetector();
   
   void Initialize(G4HCofThisEvent* HCE);

   void EndOfEvent(G4HCofThisEvent* HCE);
   
   void SetCopyLevel(G4int level) { fCopyLevel = level; }
   
   G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
   
protected:
   G4int             fCopyLevel;
   TCGmcCollections* fCollections;

protected:
   void FillHits(G4Step* aStep, TCGmcHit* newHit);

};

#endif


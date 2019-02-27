//my Sensitive detector

#ifndef TCTWsensitiveDetector_h
#define TCTWsensitiveDetector_h 1

#include "TCGbaseSensitiveDetector.hxx"
#include "TCGmcHit.hxx" //my hit class

class G4Step; //step class
class G4HCofThisEvent;

class TCTWsensitiveDetector : public TCGbaseSensitiveDetector
{
public:
   TCTWsensitiveDetector(G4String);
   ~TCTWsensitiveDetector();
      
};

#endif


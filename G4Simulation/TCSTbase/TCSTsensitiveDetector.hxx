//my Sensitive detector

#ifndef TCSTsensitiveDetector_h
#define TCSTsensitiveDetector_h 1

#include "TCGbaseSensitiveDetector.hxx"

class TCSTsensitiveDetector : public TCGbaseSensitiveDetector
{
public:
   TCSTsensitiveDetector(G4String);
   ~TCSTsensitiveDetector();
   
};

#endif


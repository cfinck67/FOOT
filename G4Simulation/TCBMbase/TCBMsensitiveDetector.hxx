//my Sensitive detector

#ifndef TCBMsensitiveDetector_h
#define TCBMsensitiveDetector_h 1

#include "TCGbaseSensitiveDetector.hxx"

class TCBMsensitiveDetector : public TCGbaseSensitiveDetector
{
public:
   TCBMsensitiveDetector(G4String);
   ~TCBMsensitiveDetector();
   
};

#endif


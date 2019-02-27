//my Sensitive detector

#ifndef TCCAsensitiveDetector_h
#define TCCAsensitiveDetector_h 1

#include "TCGbaseSensitiveDetector.hxx"

class TCCAsensitiveDetector : public TCGbaseSensitiveDetector
{
public:
   TCCAsensitiveDetector(G4String);
   ~TCCAsensitiveDetector();
   
};

#endif


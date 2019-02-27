//my Sensitive detector

#ifndef TCMSDSsensitiveDetector_h
#define TCSDSsensitiveDetector_h 1

#include "TCGbaseSensitiveDetector.hxx"

class TCMSDsensitiveDetector : public TCGbaseSensitiveDetector
{
public:
   TCMSDsensitiveDetector(G4String);
   ~TCMSDsensitiveDetector();
};

#endif


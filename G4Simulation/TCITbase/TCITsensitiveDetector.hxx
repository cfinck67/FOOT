//my Sensitive detector

#ifndef TCITsensitiveDetector_h
#define TCITsensitiveDetector_h 1

#include "TCGbaseSensitiveDetector.hxx"

class TCITsensitiveDetector : public TCGbaseSensitiveDetector
{
public:
   TCITsensitiveDetector(G4String);
   ~TCITsensitiveDetector();

};

#endif


//my Sensitive detector

#ifndef TCVTsensitiveDetector_h
#define TCVTsensitiveDetector_h 1

#include "TCGbaseSensitiveDetector.hxx"


class TCVTsensitiveDetector : public TCGbaseSensitiveDetector
{
public:
   TCVTsensitiveDetector(G4String);
   ~TCVTsensitiveDetector();

};

#endif


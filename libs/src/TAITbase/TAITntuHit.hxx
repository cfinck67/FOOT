
#ifndef _TAITntuHit_HXX
#define _TAITntuHit_HXX

// ROOT classes
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TVector3.h"

#include "TAVTdatRaw.hxx"

// all 3 needed to take from gTagROOT
#include "TAGroot.hxx"
#include "TAITparGeo.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

#include "TAVTbaseNtuHit.hxx"


// class TAVTrawHit;

/** TAITntuHit class contains information respect to a pixel in cmos detectors
 index, position, noise, pulse height, size, etc...
 
    Revised in 2018 by Matteo Franchini franchinim@bo.infn.it
    Back to a class compliant with storing in a root file by Ch. Finck

    All the coordinates are in cm and in the detector reference frame, i.e. the center
    is the center of the detector.

*/
/*------------------------------------------+---------------------------------*/

class TAITntuHit : public TAVTbaseNtuHit {
   
protected:
   TAITparGeo*        fGeometry;                 //! do NOT stored such a useless pointer !

public:

    TAITntuHit() {};
    TAITntuHit( Int_t iSensor, const Int_t aIndex, Double_t aValue);
    TAITntuHit( Int_t iSensor, Double_t aValue, Int_t aLine, Int_t aColumn);
    virtual ~TAITntuHit() {};

   
    ClassDef(TAITntuHit,3)                            // Pixel or Pixel of a Detector Plane
};

//##############################################################################



#endif
























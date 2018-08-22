
#ifndef _TAMSDntuHit_HXX
#define _TAMSDntuHit_HXX

// ROOT classes
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TVector3.h"

#include "TAVTdatRaw.hxx"

#include "TAGroot.hxx"
#include "TAMSDparGeo.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

#include "TAVTbaseNtuHit.hxx"


// class TAVTrawHit;

/** TAMSDntuHit class contains information respect to a pixel in cmos detectors
 index, position, noise, pulse height, size, etc...
 
    Revised in 2018 by Matteo Franchini franchinim@bo.infn.it
    Back to a class compliant with storing in a root file by Ch. Finck

    All the coordinates are in cm and in the detector reference frame, i.e. the center
    is the center of the detector.

*/
/*------------------------------------------+---------------------------------*/

class TAMSDntuHit : public TAVTbaseNtuHit {
   
protected:
   TAMSDparGeo*        fGeometry;                 //! do NOT stored such a useless pointer !

public:
    TAMSDntuHit() {};
    TAMSDntuHit( Int_t iSensor, const Int_t aIndex, Double_t aValue);
    TAMSDntuHit( Int_t iSensor, Double_t aValue, Int_t aLine, Int_t aColumn);
    virtual ~TAMSDntuHit() {};

    void               Initialise();
    Bool_t	           IsEqual(const TObject* obj) const;

      // all the Set methods
    void               SetMCPosition(TVector3 a_pos)   { fGeometry->Global2Local( &a_pos ), fMCPos = a_pos; }    //! Set MC truth position
    void               SetMCMomentum(TVector3 a_mom)   { fGeometry->Global2Local_RotationOnly( &a_mom ), fMCP = a_mom; }    //! Set MC truth momentum
   
    TVector3          GetPixelPosition_sensorFrame()   { return fGeometry->GetPixelPos_sensorFrame(fSensorId, fPixelColumn, fPixelLine); };
    TVector3          GetPixelPosition_footFrame()     { return fGeometry->GetPixelPos_footFrame( fSensorId, fPixelColumn, fPixelLine ); };
    
    TVector3          GetMCPosition_sensorFrame();
    TVector3          GetMCPosition_footFrame();

    TVector3          GetMCMomentum_footFrame();

    //! Get pixel line
    Int_t             GetPixelView()            const  { return  fPixelLine;      }
    //! Get pixel line
    Int_t             GetPixelStrip()           const  { return  fPixelColumn;    }
   
    //! Set pixel line
    void              SetPixelView(Int_t aLin)         { fPixelLine = aLin;       }
    //! Set pixel column
    void              SetPixelStrip(Int_t aCol)        { fPixelColumn = aCol;     }
   
//    //! Compute distance from a given pixel
//    Double_t           Distance( TAMSDntuHit&         aPixel);
//    //! Compute distance in U direction from a given pixel
//    Double_t           DistanceU( TAMSDntuHit&        aPixel);
//    //! Compute distance in V direction from a given pixel
//    Double_t           DistanceV( TAMSDntuHit&         aPixel);
//   

   // this is a tmp solution
   // MUST BE REMOVED
   // no respect of C++ encapsulation basic rule
   
     // Frank  ->  bad hack
    void SetGeneratedParticleInfo ( int genPartID, int genPartFLUKAid, int genPartCharge,
                        int genPartBarionNum, float genPartMass,
                        TVector3 genPartPosition,
                        TVector3 genPartMomentum ) {
        m_genPartID = genPartID;
        m_genPartFLUKAid = genPartFLUKAid;
        m_genPartCharge = genPartCharge;
        m_genPartBarionNum = genPartBarionNum;
        m_genPartMass = genPartMass;
        m_genPartPosition = genPartPosition;
        m_genPartMomentum = genPartMomentum;
    };
    // Get generated particle quantities
    //  provvisorio
    int m_genPartID;
    int m_genPartFLUKAid;
    int m_genPartCharge;
    int m_genPartBarionNum;
    float m_genPartMass;
    TVector3 m_genPartPosition;
    TVector3 m_genPartMomentum;
   
    ClassDef(TAMSDntuHit,3)                            // Pixel or Pixel of a Detector Plane
};

//##############################################################################



#endif
























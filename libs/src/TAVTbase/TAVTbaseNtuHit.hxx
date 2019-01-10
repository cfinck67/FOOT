
#ifndef _TAVTbaseNtuHit_HXX
#define _TAVTbaseNtuHit_HXX

// ROOT classes
#include "TObject.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TVector3.h"

#include "TAVTdatRaw.hxx"

// all 3 needed to take from gTagROOT
#include "TAGroot.hxx"
#include "TAVTparGeo.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

// class TAVTrawHit;

/** TAVTbaseNtuHit class contains information respect to a pixel in cmos detectors
 index, position, noise, pulse height, size, etc...
 
    Revised in 2018 by Matteo Franchini franchinim@bo.infn.it
    Back to a class compliant with storing in a root file by Ch. Finck

    All the coordinates are in cm and in the detector reference frame, i.e. the center
    is the center of the detector.

*/
/*------------------------------------------+---------------------------------*/

class TAVTbaseNtuHit : public TAGobject {
   
protected:
   static const Int_t fgkMAXTRACK = 10;

	Int_t              fSensorId;                 // number of the sensor
	TVector3           fPosition;                 // pixel position in the detector frame
	// TVector3           fSize;                  // size in uvw directions

   Int_t              fPixelIndex;               // index of the pixel
	Int_t              fPixelLine;                // line in the matrix
	Int_t              fPixelColumn;              // column in the matrix
	int                fLayer;

	Double_t           fRawValue;                 // the rawvalue
	Double_t           fPulseHeight;              // pulseheight on pixel

   Bool_t             fFound;                    // flag, that pixel is found in hit

   Int_t              fMcTrackCount;                 // Variable that count the number of times a crystal is touched
   Int_t              fMCindex[fgkMAXTRACK];         // Id of the track created in the simulation
   Int_t              fMcTrackId[fgkMAXTRACK];         // Id of the track created in the simulation

public:

    TAVTbaseNtuHit() {};
    TAVTbaseNtuHit( Int_t iSensor, const Int_t aIndex, Double_t aValue);
    TAVTbaseNtuHit( Int_t iSensor, Double_t aValue, Int_t aLine, Int_t aColumn);
    virtual ~TAVTbaseNtuHit() {};

   
    void               Clear(Option_t* option = "C");

    Bool_t	           IsEqual(const TObject* obj) const;
   
    void               SetRawValue(Double_t aRV)       { fRawValue = aRV;         }
    void               SetPulseHeight(Double_t aPH)    { fPulseHeight = aPH;      }
    void               SetFound(Bool_t b)              { fFound = b;              }
   
    //    All the Get methods
    Int_t              GetPixelIndex()   const         { return  fPixelIndex;     }
    Int_t              GetPixelLine()    const         { return  fPixelLine;      }
    Int_t              GetPixelColumn()  const         { return  fPixelColumn;    }
    Int_t              GetLayer()        const         { return  fLayer;          }
    Int_t              GetSensorID()     const         { return  fSensorId;       }
   
   
    Double_t           GetRawValue()     const         { return  fRawValue;       }
    Double_t           GetPulseHeight()  const         { return  fPulseHeight;    }

    // TVector3&          GetSize()                       { return  fSize;           }
   
    Bool_t             Found()          const         { return  fFound;          }
   
    TVector3          GetPixelPosition_detectorFrame()      { return fPosition; };
    TVector3&         GetPosition()                         { return fPosition; };

    void              SetPosition(TVector3 aPosition)       { fPosition = aPosition;   }
   
    //! Compute distance from a given pixel
    Double_t           Distance( TAVTbaseNtuHit&         aPixel);
    //! Compute distance from a given position
    Double_t           Distance( const TVector3&     aPosition);
    //! Compute distance in U direction from a given pixel
    Double_t           DistanceU( TAVTbaseNtuHit&        aPixel);
    //! Compute distance in U direction from a given position
    Double_t           DistanceU( const TVector3&     aPosition);
    //! Compute distance in V direction from a given pixel
    Double_t           DistanceV( TAVTbaseNtuHit&         aPixel);
    //! Compute distance in V direction from a given position
    Double_t           DistanceV( const TVector3&     aPosition);


   // MC track id
   Int_t      GetMcIndex(Int_t index)   const   { return fMCindex[index];   }
   Int_t      GetMcTrackI(Int_t index)  const   { return fMcTrackId[index]; }
   Int_t      GetMcTrackCount()         const   { return fMcTrackCount;     }
   
   // Add MC track Id
   void       AddMcTrackId(Int_t trackId, Int_t mcId = -1);

    ClassDef(TAVTbaseNtuHit,3)                            // Pixel or Pixel of a Detector Plane
};

//##############################################################################



#endif

























#ifndef _TAITntuHit_HXX
#define _TAITntuHit_HXX

// ROOT classes
#include "TObject.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TVector3.h"

#include "TAITdatRaw.hxx"

// all 3 needed to take from gTagROOT
#include "TAGroot.hxx"
#include "TAITparGeo.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

#include "TAGntuMCeve.hxx"
// #include "TAGntuMCeveHit.hxx"


// class TAITrawHit;

/** TAITntuHit class contains information respect to a pixel in cmos detectors
 index, position, noise, pulse height, size, etc...
 
    Revised in 2018 by Matteo Franchini franchinim@bo.infn.it

    All the coordinates are in cm and in the detector reference frame, i.e. the center
    is the center of the detector.

*/
/*------------------------------------------+---------------------------------*/



class TAITntuHit : public TObject {
   
private:

	TAITparGeo* m_geometry;

	Int_t              m_sensorID;                 // number of the sensor
	TVector3           fPosition;                     // pixel position in the detector frame
	// TVector3           fSize;                         // size in uvw directions

	Int_t              fMCid;                         // MC index of the hit in the ntuple
	TVector3           fMCPos;                        // MC hit position = track hitting the pixel
	TVector3           fMCP;                          // MC momentum of the hit

	// Int_t              fPixelIndex;                   // index of the pixel
	Int_t              fPixelLine;                    // line in the matrix
	Int_t              fPixelColumn;                  // column in the matrix
    int                m_layer;
    int                m_plume;
	int                m_chip;

	Double_t           fRawValue;                     // the rawvalue
	Double_t           fPulseHeight;                  // pulseheight on pixel

	Double_t           fEneLoss;        // energy loss by MC particle VM 3/11/13
   
	//////////////////////////

	string 					  m_origins;               // how the hit is produced: mc, noise, digitalization, data, ...

	int 					  m_originalMC_HitID;         // index of the hit that has generated it, Different from -1 only for cluster pix
	TAITntuHit*			      m_originalMC_Hit;        // hit index, mcID
	
    int 					  m_genPartIndex;          // the index of the particle generating the hit = TRid in ntupla
	TAGntuMCeveHit* 		  m_genPartPointer;        // the particle generating the hit

    //! Set pixel position in detector frame
    void               SetPosition(TVector3 aPosition) { fPosition = aPosition; }


public:

	TAITntuHit() {};
	TAITntuHit( Int_t iSensor, TAITrawHit* pixel);
	TAITntuHit( Int_t iSensor, const Int_t aIndex, Double_t aValue, string aorigin);
	TAITntuHit( Int_t iSensor, Double_t aValue, Int_t aLine, Int_t aColumn, string aorigin); 
	~TAITntuHit() {};

	void Initialise();


    // all the Set methods 

    void               SetMCid(Int_t a_id)             { fMCid = a_id;            }    //MC index of the hit in the ntuple
    void               SetMCPosition(TVector3 a_pos)   { m_geometry->Global2Local( &a_pos ), fMCPos = a_pos; }    //! Set MC truth position
    void               SetMCMomentum(TVector3 a_mom)   { m_geometry->Global2Local_RotationOnly( &a_mom ), fMCP = a_mom; }    //! Set MC truth momentum

    void               SetGenPartID( int agenPartID );  // also the m_genPartPointer

    void               SetOriginalMC_HitID( int amcID )         { m_originalMC_HitID = amcID; };
    void               SetOriginalMC_Hit( TAITntuHit* aseedID ) { m_originalMC_Hit = aseedID; };

    void               SetRawValue(Double_t aRV)       { fRawValue = aRV;         }
    void               SetPulseHeight(Double_t aPH)    { fPulseHeight = aPH;      }
    void               SetEneLoss(Double_t de) { fEneLoss=de; }  

    
    


    //    All the Get methods
  
    // Int_t              GetPixelIndex()           { return  fPixelIndex;     }
    int                GetPixelLine()            { return  fPixelLine;      }
    int                GetPixelColumn()          { return  fPixelColumn;    }
    int                GetLayer()                { return  m_layer;         }
    int                GetPlume()                { return  m_plume;         }
    int                GetChip()                 { return  m_chip;         }
    int                GetSensorID()             { return  m_sensorID;   }
    // TVector3&          GetSize()                       { return  fSize;           }
    
    Int_t               GetMCid()                       { return  fMCid;           }  //MC index of the hit in the ntuple
    int                 GetGenPartID() {return m_genPartIndex;};
    TAGntuMCeveHit*     GetGenParticle() {return m_genPartPointer;};

    string              GetOrigin() {return m_origins;};
    int                 GetOriginalMC_HitID() {return m_originalMC_HitID;};
    TAITntuHit*         GetOriginalMC_Hit() {return m_originalMC_Hit;}; // danger, to fix

    
    TVector3          GetPixelPosition_sensorFrame()        { return m_geometry->GetPixelPos_sensorFrame(m_sensorID, fPixelColumn, fPixelLine); };
    TVector3          GetPixelPosition_detectorFrame()      { return fPosition; };
    TVector3          GetPixelPosition_footFrame()          { return m_geometry->GetPixelPos_footFrame( m_sensorID, fPixelColumn, fPixelLine ); };
    
    TVector3          GetMCPosition_sensorFrame();
    TVector3          GetMCPosition_detectorFrame()      { return  fMCPos;            }
    TVector3          GetMCPosition_footFrame();

    // TVector3          GetMCMomentum_sensorFrame()        { return  fMCP;            }
    TVector3           GetMCMomentum_detectorFrame()      { return  fMCP;            }
    TVector3           GetMCMomentum_footFrame();

    Double_t           GetRawValue()              { return  fRawValue;       }
    Double_t           GetPulseHeight()           { return  fPulseHeight;    }
    Double_t           GetEneLoss()               { return fEneLoss; };


   //  // old
   //  TVector3&          GetPosition()                   { return  fPosition;       }
   //  TVector3           GetPixelPos_Global()            { 
   //      TVector3 globPos = fPosition;
   //      m_geometry->Local2Global( &globPos ); 
   //      return globPos;     
   //  };
   //  TVector3&          GetMCPosition_Local()                 { return  fMCPos;          }
   //  TVector3           GetMCPosition_Global()          { 
   //      TVector3 globPos = fMCPos;
   //      m_geometry->Local2Global( &globPos ); 
   //      return globPos; 
   //  };
   // TVector3&          GetMCMomentum_Local()                 { return  fMCP;            }
   // TVector3           GetMCMomentum_Global()          { 
   //      TVector3 globP = fMCP;
   //      m_geometry->Local2Global_RotationOnly( &globP ); 
   //      return globP; 
   //  };


    
    //! Compute distance from a given pixel
    Double_t           Distance( TAITntuHit&         aPixel);
    //! Compute distance from a given position
    Double_t           Distance( const TVector3&     aPosition);
    //! Compute distance in U direction from a given pixel
    Double_t           DistanceU( TAITntuHit&        aPixel);
    //! Compute distance in U direction from a given position
    Double_t           DistanceU( const TVector3&     aPosition);
    //! Compute distance in V direction from a given pixel
    Double_t           DistanceV( TAITntuHit&         aPixel);
    //! Compute distance in V direction from a given position
    Double_t           DistanceV( const TVector3&     aPosition);
   


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


   
    ClassDef(TAITntuHit,3)                            // Pixel or Pixel of a Detector Plane
};

//##############################################################################



#endif
























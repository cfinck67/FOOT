#ifndef _TAMSDntuRaw_HXX
#define _TAMSDntuRaw_HXX
/*!
 \file
 \version $Id: TAMSDntuRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
 \brief   Declaration of TAMSDntuRaw.
 */
/*------------------------------------------+---------------------------------*/

// ROOT classes
#include "TObject.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TVector3.h"

#include "TAMSDparGeo.hxx"

#include "TAGdata.hxx"

class TAMSDrawHit;

/** TAMSDntuHit class contains information respect to a pixel in cmos detectors
 index, position, noise, pulse height, size, etc...
 
 */

class TAMSDntuHit : public TObject {
   
private:

  TAMSDparGeo* m_geometry;

   Int_t              fSensorNumber;                 // number of the plane
   TVector3           fPosition;                     // position in uvw coordinates in the plane
   TVector3           fSize;                         // size in uvw directions
   
   Int_t              fMCid;                         // MC index
   TVector3           fMCPos;                        // position in uvw coordinates in the plane
   TVector3           fMCP;                          // size in uvw directions
   
   Int_t              fPixelIndex;                   // index of the pixel
   Int_t              fPixelView;                    // line in the matrix
   Int_t              fPixelStrip;                  // column in the matrix
   int                m_layer;
   Double_t           fRawValue;                     // the rawvalue
   Double_t           fPulseHeight;                  // pulseheight on pixel
   
   Bool_t             fFound;                        // flag, that strip is found in hit
   
   Int_t              fDebugLevel;                   // debug level

  Double_t            fEneLoss;        // energy loss by MC particle VM 3/11/13
   
public:
   TAMSDntuHit();
   TAMSDntuHit( Int_t iSensor, TAMSDrawHit* pixel);
   TAMSDntuHit( Int_t iSensor, const Int_t aIndex, Double_t aValue);
   TAMSDntuHit( Int_t iSensor, Double_t aValue, Int_t aView, Int_t aStrip); 
   ~TAMSDntuHit();
   
   //! Compute distance from a given pixel
   Double_t           Distance( TAMSDntuHit&         aPixel);
   //! Compute distance from a given position
   Double_t           Distance( const TVector3&     aPosition);
   //! Compute distance in U direction from a given pixel
   Double_t           DistanceU( TAMSDntuHit&        aPixel);
   //! Compute distance in U direction from a given position
   Double_t           DistanceU( const TVector3&     aPosition);
   //! Compute distance in V direction from a given pixel
   Double_t           DistanceV( TAMSDntuHit&         aPixel);
   //! Compute distance in V direction from a given position
   Double_t           DistanceV( const TVector3&     aPosition);
   
   //! Set Plane number
   void               SetSensorNumber(Int_t aNumber)  { fSensorNumber = aNumber; }
   //! Set raw value
   void               SetRawValue(Double_t aRV)       { fRawValue = aRV;         }
   //! Set pulse height
   void               SetPulseHeight(Double_t aPH)    { fPulseHeight = aPH;      }
   //! Set pixel line
   void               SetPixelView(Int_t aLin)        { fPixelView = aLin;       }
   //! Set pixel column
   void               SetPixelStrip(Int_t aCol)      { fPixelStrip = aCol;     }

   void               SetLayer(Int_t aLay)            { m_layer = aLay;     }
   //! Set pixel position
   void               SetPosition(TVector3 aPosition) { fPosition = aPosition;   }
   //! Set pixel size
   void               SetSize(TVector3 aSize)         { fSize = aSize;           }
   //! Set found flag
   void               SetFound(Bool_t b)              { fFound = b;              }
   //! Set pixel index
   void               SetPixelIndex(Int_t px)         { fPixelIndex = px;        } 

   //! Set MC truth matching index
   void               SetMCid(Int_t a_id)             { fMCid = a_id;            }
   //! Set MC truth position
   void               SetMCPosition(TVector3 a_pos)   { fMCPos = a_pos;          }
   //! Set MC truth momentum
   void               SetMCMomentum(TVector3 a_mom)   { fMCP = a_mom;            }
   //
   void               SetMsdGeo( TAMSDparGeo* ageometry )  { m_geometry = ageometry; };

   // Frank
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


  // set MC energy loss (VM 3/11/13)
  void SetEneLoss(Double_t de) { fEneLoss=de; }  
   
   //! Get pixel index
   Int_t              GetPixelIndex()           const { return  fPixelIndex;     }
   //! Get pixel line
   Int_t              GetPixelView()            const { return  fPixelView;      }
   //! Get pixel line
   Int_t              GetPixelStrip()          const { return  fPixelStrip;    }
   //! Get raw value
   Double_t           GetRawValue()             const { return  fRawValue;       }
   //! Get pulse height
   Double_t           GetPulseHeight()          const { return  fPulseHeight;    }
   // Quik hack by Frank, put it better
   Int_t              GetLayer()               { return  m_layer;   }
   
   //! Get position
   TVector3&          GetPosition()                   { return  fPosition;       }
   //! Get Size
   TVector3&          GetSize()                       { return  fSize;           }
   //! Get found flag
   Bool_t             Found()                         { return  fFound;          }
   //! Get plane number
   Int_t              GetSensorNumber()               { return  fSensorNumber;   }
   
   //! Get MC truth matching index
   Int_t              GetMCid()                       { return  fMCid;           }
   //! Get MC truth position
   TVector3&          GetMCPosition_Local()                 { return  fMCPos;          }
   TVector3          GetMCPosition_Global()          { 
        TVector3 globPos = fMCPos;
        m_geometry->Local2Global( &globPos ); 
        return globPos; 
    };
   //! Get MC truth momentum
   TVector3&          GetMCMomentum_Local()                 { return  fMCP;            }
   TVector3          GetMCMomentum_Global()          { 
        TVector3 globP = fMCP;
        m_geometry->Local2Global_RotationOnly( &globP ); 
        return globP; 
    };

  // get MC energy loss (VM 3/11/13)
  Double_t           GetEneLoss() { return fEneLoss; }

   ClassDef(TAMSDntuHit,3)                            // Pixel or Pixel of a Detector Plane
};

//##############################################################################

class TAMSDntuRaw : public TAGdata {
   
private:
   //using TObjArray here
   TObjArray*        fListOfPixels; 
   static TString    fgkBranchName;    // Branch name in TTree
   
public:
   TAMSDntuRaw();
   virtual          ~TAMSDntuRaw();
   
   TAMSDntuHit*       GetPixel(Int_t iSensor, Int_t iPixel);
   const TAMSDntuHit* GetPixel(Int_t iSensor, Int_t iPixel) const;
   
   TClonesArray*     GetListOfPixels(Int_t iSensor);
   TClonesArray*     GetListOfPixels(Int_t iSensor) const;
   
   Int_t             GetPixelsN(Int_t iSensor) const; 
   
   TAMSDntuHit*       NewPixel(Int_t sensor, Double_t value, Int_t aLine, Int_t aColumn);
   TAMSDntuHit*       NewPixel(Int_t sensor, TAMSDrawHit* pixel);

   
   virtual void      SetupClones();
   
   virtual void      Clear(Option_t* opt="");
   
   virtual void      ToStream(ostream& os=cout, Option_t* option="") const;
 
public:   
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }
   
   ClassDef(TAMSDntuRaw,1)
};

#endif

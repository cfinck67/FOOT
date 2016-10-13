#ifndef _TAITparGeo_HXX
#define _TAITparGeo_HXX
/*!
  \file
  \version $Id: TAITparGeo.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TAITparGeo.
*/
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TEveGeoShapeExtract.h"

#include "TObject.h"
#include "TString.h"
#include "TVector3.h"

#include "TAITparTools.hxx"


class TGeoHMatrix;
class TGeoVolume;
//##############################################################################

class TAITparGeo : public TAITparTools {
      
private:
   TObjArray* fMatrixList;       //! list of transformation matrices  (rotation+translation for each sensor)
   TVector3*  fCurrentPosition;  // current position
   
   Int_t      fSensorsN;         // Number of sensors	 
   Int_t      fPixelsNu;         // Number of pixels in U direction
   Int_t      fPixelsNv;         // Number of pixels in V direction
   Float_t    fPitchU;           // Pitch value in U direction
   Float_t    fPitchV;           // Pitch value in U direction
   Float_t    fWidth;            // Width of the sensor (active aera)
   Float_t    fHeight;           // Height of the sensor
   Float_t    fThick;            // Thickness of the sensor

   // For simulation purpose
   TVector3   fTotalSize;        // Total size of sensor 
   Float_t    fPixThickness;     // Pixel Thickness 
   Float_t    fTotalThickness;   // Epitaxyal Thickness 
         
   struct SensorParameter_t : public  TObject {
	  Int_t     SensorIdx;   // sensor index
	  TVector3  Position;    // current position 
	  TVector3  Tilt;        // current tilt angles 
	  Float_t   AlignmentU;  // U alignment
	  Float_t   AlignmentV;  // V alignment
	  Float_t   TiltW;       // Tilted angle around beam axis
   };
   
   SensorParameter_t  fSensorParameter[10];

private: 
   static       TString fgkDefaultGeoName;    // default detector geomap file
   static       TString fgkDefaultGeoNameMC;  // default detector geomap file for MC
   static       Float_t fgTargetWidth;        // target width
   static       Float_t fgTargetSize;         // target size

private:   
   void         CheckGeo();
   
public:

   TAITparGeo();
    virtual ~TAITparGeo();

   //! Transform point from the global reference frame
   //! to the local reference frame of the detection id
   void            Global2Local(Int_t detID,
								Double_t xg, Double_t yg, Double_t zg, 
								Double_t& xl, Double_t& yl, Double_t& zl) const;
   
   TVector3        Global2Local(Int_t detID, TVector3& glob) const;
   TVector3        Global2LocalVect(Int_t detID, TVector3& glob) const;

   //! Transform point from the local reference frame
   //! of the detection id to the global reference frame 
   void            Local2Global(Int_t detID,
								Double_t xl, Double_t yl, Double_t zl, 
								Double_t& xg, Double_t& yg, Double_t& zg) const;
   
   TVector3        Local2Global(Int_t detID, TVector3& loc) const;
   TVector3        Local2GlobalVect(Int_t detID, TVector3& loc) const;

   
   Bool_t          FromFile(const TString& name = "");
	
   void            AddTransMatrix(TGeoHMatrix* mat, Int_t idx = -1);
  
   void            RemoveTransMatrix(TGeoHMatrix* mat);
   
   TGeoHMatrix*    GetTransfo(Int_t iSensor);
   
   TVector3*       GetPosition(Int_t iSensor);
   
   virtual void    Clear(Option_t* opt="");
   
   virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;
   
   Int_t  GetSensorsN()         const { return fSensorsN;       }
   Int_t  GetPixelsNu()         const { return fPixelsNu;       }
   Int_t  GetPixelsNv()         const { return fPixelsNv;       }
   
   Float_t GetPitchU()          const { return fPitchU;         }
   Float_t GetPitchV()          const { return fPitchV;         }
   
   Float_t GetWidth()           const { return fWidth;          }
   Float_t GetHeight()          const { return fHeight;         }
   Float_t GetThick()           const { return fThick;          }

   Float_t  GetPixThickness()   const { return fPixThickness;   }
   Float_t  GetTotalThickness() const { return fTotalThickness; }
   TVector3 GetTotalSize()      const { return fTotalSize;      }
   
      
   //! Add CMOS module geometry to world
   TGeoVolume* AddVertexModule(TGeoHMatrix* hm, const char* basemoduleName = "Module", const char *name = "Vertex");
   
   //! Add CMOS module geometry to world
   TEveGeoShapeExtract* AddExtractVertexModule(TGeoHMatrix* hm, const char* basemoduleName = "Module", const char *name = "Vertex");

   //! Add Target
   TGeoVolume* AddTarget(const Float_t dx = fgTargetSize/2., const Float_t dy = fgTargetSize/2., 
						 const Float_t dz = fgTargetWidth/2., const char *targetName = "Target");
   
   //! Add Extract Target
   TEveGeoShapeExtract* AddExtractTarget(const Float_t dx = fgTargetSize/2., const Float_t dy = fgTargetSize/2., 
										 const Float_t dz = fgTargetWidth/2., const char *targetName = "Target");
   
   //! Build Vertex
   TGeoVolume* BuildVertex(const char* basemoduleName = "Module", const char *name = "Vertex");
   
   //! Build Extract Vertex
   TEveGeoShapeExtract* BuildExtractVertex(const char* basemoduleName = "Module", const char *name = "Vertex");
   
   //! Get Sensor parameter
   SensorParameter_t& GetSensorPar(Int_t idx){return fSensorParameter[idx];}
   
public:
   
   static Float_t GetTargetWidth()            { return fgTargetWidth; }
   static void    SetTargetWidth(Float_t w)   { fgTargetWidth = w;    }
   
   static Float_t GetTargetSize()             { return fgTargetSize;  }
   static void    SetTargetSize(Float_t s)    { fgTargetSize = s;     }

   static const Char_t* GetDefaultGeoName()   { return fgkDefaultGeoName.Data();    }
   static const Char_t* GetDefaultGeoNameMC() { return fgkDefaultGeoNameMC.Data();  }

   ClassDef(TAITparGeo,1)
};

#endif

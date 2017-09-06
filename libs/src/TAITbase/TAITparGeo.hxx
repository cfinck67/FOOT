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

#include "IronPlate.hxx"
#include <vector>

class TGeoHMatrix;
class TGeoVolume;
//##############################################################################

class TAITparGeo : public TAITparTools {

// object to be used as 3D matrix of sensors
typedef vector< vector< vector< IronPlate* > > > SensorMatrix;

public:

    TAITparGeo() {};
    TAITparGeo( TAITparGeo* original );
    virtual ~TAITparGeo() {};

    void InitGeo();
    void InitMaterial();
    void PrintBodies( string geoFileName );

    //! Transform point from the global reference frame
    //! to the local reference frame of the detection id and vice versa
    void Global2Local( TVector3* glob );
    void Global2Local_RotationOnly( TVector3* glob );
    void Local2Global( TVector3* loc );
    void Local2Global_RotationOnly( TVector3* loc );

    TRotation GetRotationToGlobal() { return *m_rotation; };
    TRotation GetRotationToLocal() { return m_rotation->Inverse(); };

    // Return the pixel position  -->  change name! in GetPixelPos()
    //    it should be changed arrirdingly with the simulation choice when more than one sensors will be used
    TVector3 GetPosition( int layer, int col, int row );

    

    // Return Inner Trakcker center coord. in the global frame
    TVector3 GetCenter() { return m_center; };

    // Return Inner Trakcker full dimension.
    TVector3 GetDimension() { return m_dimension; };
    
    double GetSingleSensorThickness() { return m_siliconSensorThick_Lz; };
    
    // Return distance from center to center
    double GetLayerDistance() { return m_layerDistance; };
    double GetNPixelX() { return m_nPixel_X; };
    double GetNPixelY() { return m_nPixel_Y; };
    int GetNLayers() { return m_nSensors_Z; };
    
    
    // Return a vector with the number of sensors along the cartesian directions
    TVector3        GetNumberOfSensorAlongDirections() { return m_NSensors; };
    
    TGeoVolume*     GetVolume();     


    virtual void    Clear(Option_t* opt="");
    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;



private:

    SensorMatrix m_sensorMatrix;
    TRotation* m_rotation;

    TVector3  m_origin;  // current position in local coord.
    TVector3  m_center;  // current position in global coord.
    TVector3  m_dimension;  

    int m_nSensors_X;
    int m_nSensors_Y;
    int m_nSensors_Z;
    TVector3 m_NSensors;   

    vector<string> m_materialOrder;
    map<string, double> m_materialThick;
    map<string, string> m_materialType;


    double m_siliconSensorThick_Lz;
    double m_layerDistance;

    int m_nPixel_X;
    int m_nPixel_Y;



// public:

   // Int_t  GetSensorsN()         const { return fSensorsN;       }
   // Int_t  GetPixelsNu()         const { return fPixelsNu;       }
   // Int_t  GetPixelsNv()         const { return fPixelsNv;       }
   
   // Float_t GetPitchU()          const { return fPitchU;         }
   // Float_t GetPitchV()          const { return fPitchV;         }
   
   // Float_t GetWidth()           const { return fWidth;          }
   // Float_t GetHeight()          const { return fHeight;         }
   // Float_t GetThick()           const { return fThick;          }

   // Float_t  GetPixThickness()   const { return fPixThickness;   }
   // Float_t  GetTotalThickness() const { return fTotalThickness; }
   // TVector3 GetTotalSize()      const { return fTotalSize;      }
   


      
   // //! Add CMOS module geometry to world
   // TGeoVolume* AddVertexModule(TGeoHMatrix* hm, const char* basemoduleName = "Module", const char *name = "Vertex");
   
   // //! Add CMOS module geometry to world
   // TEveGeoShapeExtract* AddExtractVertexModule(TGeoHMatrix* hm, const char* basemoduleName = "Module", const char *name = "Vertex");

   // //! Add Target
   // TGeoVolume* AddTarget(const Float_t dx = fgTargetSize/2., const Float_t dy = fgTargetSize/2., 
			// 			 const Float_t dz = fgTargetWidth/2., const char *targetName = "Target");
   
   // //! Add Extract Target
   // TEveGeoShapeExtract* AddExtractTarget(const Float_t dx = fgTargetSize/2., const Float_t dy = fgTargetSize/2., 
			// 							 const Float_t dz = fgTargetWidth/2., const char *targetName = "Target");
   
   // //! Build Vertex
   // TGeoVolume* BuildVertex(const char* basemoduleName = "Module", const char *name = "Vertex");
   
   // //! Build Extract Vertex
   // TEveGeoShapeExtract* BuildExtractVertex(const char* basemoduleName = "Module", const char *name = "Vertex");
   
   // //! Get Sensor parameter
   // SensorParameter_t& GetSensorPar(Int_t idx){return fSensorParameter[idx];}
   

///////////////////////////////////////////////////////////////////////////////////////////////////////////



   // static Float_t GetTargetWidth()            { return fgTargetWidth; }
   // static void    SetTargetWidth(Float_t w)   { fgTargetWidth = w;    }
   
   // static Float_t GetTargetSize()             { return fgTargetSize;  }
   // static void    SetTargetSize(Float_t s)    { fgTargetSize = s;     }

   // static const Char_t* GetDefaultGeoName()   { return fgkDefaultGeoName.Data();    }
   // static const Char_t* GetDefaultGeoNameMC() { return fgkDefaultGeoNameMC.Data();  }

   ClassDef(TAITparGeo,1)
};

#endif

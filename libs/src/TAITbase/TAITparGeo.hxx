#ifndef _TAITparGeo_HXX
#define _TAITparGeo_HXX
/*!
  \version $Id: TAITparGeo.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  
    Fully revised in 2017 by Matteo Franchini franchinim@bo.infn.it

    Three reference frames are possible and all the transformation from one to another 
    are defined in this class:
        - sensor frame
        - detector frame
        - FOOT frame

    All the coordinates are in cm and in the detector reference frame, i.e. the center
    is the center of the detector.

*/
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TEveGeoShapeExtract.h"

#include "TObject.h"

#include "TAGparTools.hxx"

#include "IronPlate.hxx"
#include "FootBox.hxx"
#include "GlobalPar.hxx"
#include "FootField.hxx"

#include <FieldManager.h>



class TGeoHMatrix;
class TGeoVolume;
//##############################################################################

class TAITparGeo : public TAGparTools {

// object to be used as 3D matrix of sensors
typedef vector< vector< vector< IronPlate* > > > SensorMatrix;
typedef vector< vector< IronPlate* > > SensorPlane;
typedef vector< IronPlate* > SensorLine;

typedef vector< vector< vector< FootBox* > > > PassiveMatrix;
typedef vector< vector< FootBox* > > PassivePlane;
typedef vector< FootBox* > PassiveLine;

public:

    TAITparGeo();
    TAITparGeo( TAITparGeo* original );
    virtual ~TAITparGeo() {
      // sensor matrix
      for ( SensorMatrix::iterator itX = m_sensorMatrix.begin(); itX != m_sensorMatrix.end(); itX++ ) {
        for ( SensorPlane::iterator itY = (*itX).begin(); itY != (*itX).end(); itY++ ) {
            for ( SensorLine::iterator itZ = (*itY).begin(); itZ != (*itY).end(); itZ++ ) {
                delete (*itZ);
            }
            (*itY).clear();
        }
        (*itX).clear();
      }
      m_sensorMatrix.clear();
    };

    void InitGeo();
    void InitMaterial();

    TVector3 GetSensorPosition( int sensorID );

    // new
    TVector3 GetPixelPos_sensorFrame( int layer, int plume, int chip, int col, int row );
    TVector3 GetPixelPos_detectorFrame( int layer, int plume, int chip, int col, int row );
    TVector3 GetPixelPos_footFrame( int layer, int plume, int chip, int col, int row );

    TVector3 GetPixelPos_sensorFrame( int sensorID, int col, int row );
    TVector3 GetPixelPos_detectorFrame( int sensorID, int col, int row );
    TVector3 GetPixelPos_footFrame( int sensorID, int col, int row );

// new
    float GetColumnCenter_sensorFrame ( int col);
    float GetColumnCenter_detectorFrame ( int layer, int plume, int chip, int col);
    float GetColumnCenter_detectorFrame( int sensorID, int col );
    float GetColumnCenter_footFrame ( int layer, int plume, int chip, int col);
    float GetColumnCenter_footFrame ( int sensorID, int col);
    // new
    float GetRowCenter_sensorFrame ( int row);
    float GetRowCenter_detectorFrame ( int layer, int plume, int chip, int row);
    float GetRowCenter_detectorFrame ( int sensorID, int row);
    float GetRowCenter_footFrame ( int layer, int plume, int chip, int row);
    float GetRowCenter_footFrame ( int sensorID, int row);


    //! Transform point from the global reference frame
    //! to the local reference frame of the detection id and vice versa
    void Detector2Sensor_frame( int sensorID, TVector3* coord );
    void Sensor2Detector_frame( int sensorID, TVector3* coord );

    // foot to detector
    void Global2Local( TVector3* glob );
    void Global2Local_RotationOnly( TVector3* glob );
    
    // detector to foot
    void Local2Global( TVector3* loc );
    void Local2Global_RotationOnly( TVector3* loc );

    TRotation GetRotationToGlobal() { return *m_rotation; };
    TRotation GetRotationToLocal() { return m_rotation->Inverse(); };



    // Return Inner Trakcker center coord. in the global frame
    TVector3 GetCenter() { return m_center; };

    // Return Inner Trakcker full dimension.
    TVector3 GetDimension() { return m_dimension; };

    double GetSingleSensorThickness() { return m_siliconSensorThick_Lz; };

    // Return distance from center to center
    double GetLayerDistance() { return m_layerDistance; };
    double GetNPixelX() { return m_nPixel_X; };
    double GetNPixelY() { return m_nPixel_Y; };

    int GetNSensors()    { return m_nSensors.X()*m_nSensors.Y()*m_nSensors.Z(); };  // return tot number of sensors

    // Return a vector with the number of sensors along the cartesian directions 
    TVector3        GetNumberOfSensorAlongDirections() { return m_nSensors; };
    int GetNChip() { return m_nSensors.X(); }; 
    int GetNPlume() { return m_nSensors.Y(); }; 
    int GetNLayers() { return m_nSensors.Z(); }; 
    
    // define the agloritm to map the sensor with a single variable. 
    int GetSensorID( int layer, int col, int row )    { return layer*GetNPlume()*GetNChip() + col*GetNChip() + row; };
    int GetLayerFromSensorID( int sensID )            { return sensID/( GetNPlume()*GetNChip() ); };
    int GetPlumeFromSensorID( int sensID )            { 
        int singleLayerID = sensID - ( GetLayerFromSensorID( sensID ) * GetNPlume()*GetNChip() ); 
        return singleLayerID / GetNChip();
    };
    int GetChipFromSensorID( int sensID )            { 
        int singleLayerID = sensID - ( GetLayerFromSensorID( sensID ) * GetNPlume()*GetNChip() ); 
        return singleLayerID % GetNChip();
    };

    // pixel dimension
    // double GetPitchX()  { return m_Pitch_X; };
    // double GetPitchY()  { return m_Pitch_Y; };

    // function for the FRUKA geometry creation
    string PrintBodies();
    string PrintRegions();
    string PrintAssignMaterial();
    string PrintSubtractBodiesFromAir();
    string PrintParameters();


    TGeoVolume*     GetVolume();


    virtual void    Clear(Option_t* opt="");
    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;



private:

    SensorMatrix m_sensorMatrix;
    PassiveMatrix m_passiveMatrix;
    PassiveMatrix m_chipMatrix;
    TRotation* m_rotation;

    TGeoVolume* m_universe;

    TVector3  m_origin;  // current position in local coord.
    TVector3  m_center;  // current position in global coord.
    TVector3  m_dimension;

    int m_volumeCount;
    int m_passiveCount;

    TVector3 m_nSensors;

    double m_plumeDistace_Z;
    double m_plumeDistace_Y;
    double m_boardXDeadMin;
    double m_boardXDeadMax;
    double m_boardYDeadMin;
    double m_boardYDeadMax;

    vector<string> m_materialOrder;
    vector<string> m_passiveMaterial;

    map<string, double> m_materialThick;
    map<string, string> m_materialType;

    map<string, vector<string> > m_bodyPrintOut;
    map<string, vector<string> > m_regionPrintOut;
    map<string, vector<string> > m_regionName;
    map<string, vector<string> > m_bodyName;
    map<string, int > m_magneticRegion;
    

    int m_nPassiveLayersPerBoard_z;
    double m_passiveMaterialThick;
    double m_siliconSensorThick_Lz;
    double m_layerDistance;

    int m_nPixel_X;
    int m_nPixel_Y;

    vector<vector<double>> m_xmin;
    vector<vector<double>> m_ymin;

    int m_debug;
    int m_setW_0number;



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

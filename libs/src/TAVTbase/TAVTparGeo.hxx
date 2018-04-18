#ifndef _TAVTparGeo_HXX
#define _TAVTparGeo_HXX
/*!
  \file
  \version $Id: TAVTparGeo.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TAVTparGeo.
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

class TAVTparGeo : public TAGparTools {


typedef vector< vector< vector< IronPlate* > > > SensorMatrix;
typedef vector< vector< IronPlate* > > SensorPlane;
typedef vector< IronPlate* > SensorLine;

typedef vector< vector< vector< FootBox* > > > PassiveMatrix;
typedef vector< vector< FootBox* > > PassivePlane;
typedef vector< FootBox* > PassiveLine;

public:

    TAVTparGeo();
    TAVTparGeo( TAVTparGeo* original );
    virtual ~TAVTparGeo() {
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

    Float_t GetPositionU(Int_t column)         const;
    Float_t GetPositionV(Int_t line)           const;

    //! Transform point from the global reference frame
    //! to the local reference frame of the detection id
    void Global2Local( TVector3* glob );
    void Global2Local_TranslationOnly( TVector3* glob );
    void Global2Local_RotationOnly( TVector3* glob );
    void Local2Global( TVector3* loc );
    void Local2Global_TranslationOnly( TVector3* loc );
    void Local2Global_RotationOnly( TVector3* loc );


    TRotation GetRotationToGlobal() { return *m_rotation; };
    TRotation GetRotationToLocal() { return m_rotation->Inverse(); };

    // Return the pixel position  -->  change name! in GetPixelPos()
    //    it should be changed arrirdingly with the simulation choice when more than one sensors will be used
    TVector3 GetPosition( int layer, int col, int row );

    //  Return Vertex center coord. in the global frame
    TVector3 GetCenter() { return m_center; };

    // Return Vertex full dimension.
    TVector3 GetDimension() { return m_dimension; };

    double GetSingleSensorThickness() { return m_siliconSensorThick_Lz; };

    // Return distance from center to center
    // double GetLayerDistance() { return m_layerDistance; };

    double GetNPixelX() { return m_nPixel_X; };
    double GetNPixelY() { return m_nPixel_Y; };
    int GetNLayers()    { return m_nSensors_Z; };
    int GetNSensors()    { return m_numberOfSensors; };

    double GetPitchX()  { return m_Pitch_X; };
    double GetPitchY()  { return m_Pitch_Y; };
    
    string PrintBodies();
    string PrintRegions();
    string PrintAssignMaterial();
    string PrintSubtractBodiesFromAir();
    string PrintParameters();

    // Return a vector with the number of sensors along the cartesian directions
    TVector3        GetNumberOfSensorAlongDirections() { return m_NSensors; };

    TGeoVolume*     GetVolume();

    virtual void    Clear(Option_t* opt="");
    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;


private:

    SensorMatrix m_sensorMatrix;
    PassiveMatrix m_passiveMatrix;
    TRotation* m_rotation;

    TGeoVolume* m_universe;

    TVector3  m_origin;  // current position in local coord.
    TVector3  m_center;  // current position in global coord.
    TVector3  m_dimension;

    int m_volumeCount;
    int m_passiveCount;

    int m_nSensors_X;
    int m_nSensors_Y;
    int m_nSensors_Z;
    int m_numberOfSensors;
    TVector3 m_NSensors;

    vector<string> m_materialOrder;
    vector<string> m_passiveMaterial;

    map<string, double> m_materialThick;
    map<string, string> m_materialType;

    map<string, vector<string> > m_regionPrintOut;
    map<string, vector<string> > m_bodyPrintOut;
    map<string, vector<string> > m_regionName;
    map<string, vector<string> > m_bodyName;
    map<string, int > m_magneticRegion;

    int m_nPassiveLayersPerBoard_z;
    double m_passiveMaterialThick;
    double m_siliconSensorThick_Lz;
    double m_layerDistance_samePair;
    double m_layerDistance_interPair;

    int m_nPixel_X;
    int m_nPixel_Y;

    int m_debug;
    int m_setW_0number;
    double m_Pitch_X;
    double m_Pitch_Y;

    double m_xmin, m_ymin;





    // static Float_t GetTargetWidth()            { return fgTargetWidth; }
    // static void    SetTargetWidth(Float_t w)   { fgTargetWidth = w;    }

    // static Float_t GetTargetSize()             { return fgTargetSize;  }
    // static void    SetTargetSize(Float_t s)    { fgTargetSize = s;     }

    // static const Char_t* GetDefaultGeoName()   { return fgkDefaultGeoName.Data();    }
    // static const Char_t* GetDefaultGeoNameMC() { return fgkDefaultGeoNameMC.Data();  }



////////////////////////////////////////////////////////////////////////////////



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


   ClassDef(TAVTparGeo,1)
};

#endif

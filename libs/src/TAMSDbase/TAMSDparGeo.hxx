#ifndef _TAMSDparGeo_HXX
#define _TAMSDparGeo_HXX
/*!
  \file
  \version $Id: TAMSDparGeo.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TAMSDparGeo.
*/
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TEveGeoShapeExtract.h"

#include "TObject.h"
#include "TString.h"
#include "TVector3.h"
#include "TRotation.h"

#include "TAMSDparTools.hxx"
#include "IronPlate.hxx"
#include "FootField.hxx"

#include <FieldManager.h>

#include "foot_geo.h"
#include "GlobalPar.hxx"

class TGeoHMatrix;
class TGeoVolume;
//##############################################################################

class TAMSDparGeo : public TAMSDparTools {


typedef vector< vector< vector< IronPlate* > > > SensorMatrix;
typedef vector< vector< IronPlate* > > SensorPlane;
typedef vector< IronPlate* > SensorLine;
      

public:

    TAMSDparGeo();
    TAMSDparGeo( TAMSDparGeo* original );
    virtual ~TAMSDparGeo();

    void InitGeo();
    void InitMaterial();

    //! Transform point from the global reference frame
    //! to the local reference frame of the detection id
    void Global2Local( TVector3* glob );
    void Global2Local_RotationOnly( TVector3* glob );
    void Local2Global( TVector3* loc );
    void Local2Global_RotationOnly( TVector3* loc );


    TRotation GetRotationToGlobal() { return *m_rotation; };
    TRotation GetRotationToLocal() { return m_rotation->Inverse(); };

    // Return the pixel position  -->  change name! in GetPixelPos()
    //    it should be changed arrirdingly with the simulation choice when more than one sensors will be used
    TVector3 GetPosition( int layer, int col, int row );

    TVector3 GetLayerCenter( int layer );

    //  Return Vertex center coord. in the global frame
    TVector3 GetCenter() { return m_center; };

    // Return Inner Trakcker full dimension.
    TVector3 GetDimension() { return m_dimension; };

    double GetSingleSensorThickness() { return m_dimension.z(); };

    // Return distance from center to center
    double GetLayerDistance() { return m_layerDistance; };

    double GetNPixelX() { return m_nPixel_X; };
    double GetNPixelY() { return m_nPixel_Y; };
    int GetNLayers() { return m_nSensors_Z; };

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
    TRotation* m_rotation;

    TGeoVolume* m_universe;

    // TObjArray* fMatrixList;       //! list of transformation matrices  (rotation+translation for each sensor)
    TVector3  m_origin;  // current position
    TVector3  m_center;  // current position
    TVector3  m_dimension;  

    int m_nSensors_X;
    int m_nSensors_Y;
    int m_nSensors_Z;
    TVector3 m_NSensors;   

    int m_volumeCount;
    int m_passiveCount;

    map<string, vector<string> > m_regionPrintOut;
    map<string, vector<string> > m_bodyPrintOut;
    map<string, vector<string> > m_regionName;
    map<string, vector<string> > m_bodyName;
    map<string, int > m_magneticRegion;

    vector<string> m_materialOrder;
    map<string, double> m_materialThick;
    map<string, string> m_materialType;

    double m_layerDistance;
    double m_layerThick;
    double m_nSensor_X_Layer;

    int m_nPixel_X;
    int m_nPixel_Y;

    int m_debug;
    int m_setW_0number;



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

   
   ClassDef(TAMSDparGeo,1)
};

#endif































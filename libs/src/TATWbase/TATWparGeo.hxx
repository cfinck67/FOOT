#ifndef _TATWparGeo_HXX
#define _TATWparGeo_HXX
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TEveGeoShapeExtract.h"

#include "TObject.h"
#include "TString.h"
#include "TVector3.h"
#include "TRotation.h"

#include "TAGparTools.hxx"

#include "FootBox.hxx"
#include "GlobalPar.hxx"

#include <FieldManager.h>

#include "foot_geo.h"
#include "LightSabre.hxx"

class TGeoHMatrix;
class TGeoVolume;
//##############################################################################

class TATWparGeo : public TAGparTools {


  typedef vector< vector< LightSabre* > > BarMatrix;
  typedef vector< LightSabre* > BarPlane;
      

public:

    TATWparGeo();
    TATWparGeo( TATWparGeo* original );
    virtual ~TATWparGeo() {};

    void InitGeo();
    void InitMaterial();

    //! Transform point from the global reference frame
    //! to the local reference frame of the detection id
    void Detector2Sensor_frame( int view, int bar, TVector3* coord );
    void Sensor2Detector_frame( int view, int bar, TVector3* coord );
    // foot to detector
    void Global2Local( TVector3* glob );
    void Global2Local_TranslationOnly( TVector3* glob );
    void Global2Local_RotationOnly( TVector3* glob );
    // detector to foot
    void Local2Global( TVector3* loc );
    void Local2Global_TranslationOnly( TVector3* loc );
    void Local2Global_RotationOnly( TVector3* loc );



    TRotation GetRotationToGlobal() { return *m_rotation; };
    TRotation GetRotationToLocal() { return m_rotation->Inverse(); };

    
    //  Return Scintillator center coord. in the global frame
    TVector3 GetCenter() { return m_center; };

    // Return Scintillator full dimension.
    TVector3 GetDimension() { return m_dimension; };

    TVector3 GetBarDimension() { return m_dimensionBar; };   // ( shorter dim, longer dim, z dim )

    double GetSingleBarThickness() { return m_dimensionBar.z(); };

    // Return distance from center to center
    double GetLayerDistance() { return m_layerDistance; };

    int GetNBars() { return m_nBar; };
    int GetNLayers() { return m_nLayer; };

    // Return the bar center position 
    float GetCoordiante_sensorFrame( int view, int bar )    { return ( view == 0 ? m_barMatrix[view][bar]->GetPosition_local().X() : m_barMatrix[view][bar]->GetPosition_local().Y() ); };
    float GetCoordiante_detectorFrame( int view, int bar )  { return ( view == 0 ? m_barMatrix[view][bar]->GetPosition().X() : m_barMatrix[view][bar]->GetPosition().Y() ); };
    float GetCoordiante_footFrame( int view, int bar );

    float GetZ_sensorFrame( int view, int bar )    { return m_barMatrix[view][bar]->GetPosition_local().Z(); };
    float GetZ_detectorFrame( int view, int bar )  { return m_barMatrix[view][bar]->GetPosition().Z(); };
    float GetZ_footFrame( int view, int bar );

    string PrintBodies();
    string PrintRegions();
    string PrintAssignMaterial();
    string PrintSubtractBodiesFromAir();
    string PrintParameters();

    // Return a vector with the number of sensors along the cartesian directions
    TVector2        GetNumberOfSensorAlongDirections() { return m_NBar; };

    TGeoVolume*     GetVolume(); 

    virtual void    Clear(Option_t* opt="");
    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;


private:

  BarMatrix m_barMatrix;
  TRotation* m_rotation;

  TGeoVolume* m_universe;

  // TObjArray* fMatrixList;       //! list of transformation matrices  (rotation+translation for each sensor)
  TVector3  m_origin;  // current position
  TVector3  m_center;  // current position
  TVector3  m_dimension;  
  TVector3  m_dimensionBar;  

  int m_volumeCount;

  int m_nBar;
  int m_nLayer;
  TVector2 m_NBar;   

  double m_barLongDim;
  double m_barShortDim;
  double m_layerDistance;

  string m_material;

  map<string, vector<string> > m_regionPrintOut;
  map<string, vector<string> > m_bodyPrintOut;
  map<string, vector<string> > m_regionName;
  map<string, vector<string> > m_bodyName;
  map<string, int > m_magneticRegion;

  int m_debug;
   
  ClassDef(TATWparGeo,1)
};

#endif

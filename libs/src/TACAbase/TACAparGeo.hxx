#ifndef _TACAparGeo_HXX
#define _TACAparGeo_HXX

#include "Riostream.h"

#include "TEveGeoShapeExtract.h"

#include "TObject.h"
#include "TString.h"
#include "TVector3.h"
#include "TRotation.h"

#include "TACAparTools.hxx"

#include "GlobalPar.hxx"
#include "FootBox.hxx"
#include "FootField.hxx"

#include <FieldManager.h>

#include "foot_geo.h"
#include "LightSabre.hxx"

class TGeoHMatrix;
class TGeoVolume;
//##############################################################################

class TACAparGeo : public TACAparTools {

typedef vector< vector< vector< LightSabre* > > > BGO_SensorMatrix;

//typedef vector< vector< vector< FootBox* > > > PassiveMatrix;
 
public:

  TACAparGeo();
  // TACAparGeo( TACAparGeo* original );
  virtual ~TACAparGeo() {

  };

  void InitGeo();
  void InitMaterial();
  
  //! Transform point from the global reference frame
  //! to the local reference frame of the detection id and vice versa
  void Global2Local( TVector3* glob );
  void Global2Local_RotationOnly( TVector3* glob );
  void Local2Global( TVector3* loc );
  void Local2Global_RotationOnly( TVector3* loc );
  void Detector2Sensor_frame( int BGOz, int BGOx, int BGOy, TVector3* coord );
  void Sensor2Detector_frame( int BGOz, int BGOx, int BGOy, TVector3* coord );
  
  
  TRotation GetRotationToGlobal() { return *m_rotation; };
  TRotation GetRotationToLocal() { return m_rotation->Inverse(); };
  
  // Return the pixel position  -->  change name! in GetPixelPos()
  //    it should be changed arrirdingly with the simulation choice when more than one sensors will be used
  TVector3 GetPosition( int layer, int plume, int chip, int col, int row );

  // Return Inner Trakcker center coord. in the global frame
  TVector3 GetCenter() { return m_center; };

  // Return Inner Trakcker full dimension.
  TVector3 GetDimension() { return m_dimension; };

  double GetSingleSensorThickness() { return m_BGOSensorThick_Lz; };

  // Return distance from center to center
  // double GetLayerDistance() { return m_layerDistance; };
  // double GetNPixelX() { return m_nPixel_X; };
  // double GetNPixelY() { return m_nPixel_Y; };
  // int GetNLayers() { return m_nSensors_Z; };

  int GetNBGOx() { return m_nBGO_X; }
  int GetNBGOy() { return m_nBGO_Y; }
  int GetNBGO() {return m_nBGO_X*m_nBGO_Y;}
  
  void AssignMaterial() {};
  void AssignMagnetField() {};

  // define the agloritm to map the sensor with a single variable. 
  int GetSensorID( int BGOx, int BGOy )    { return BGOx*m_nSensors.X() + BGOy; };    //from x,y coordinates --> ID_BGO
  TVector3 GetBGOcoordinate( int ID_BGO );    //from ID_BGO --> x,y coordinates
     
  //TVector3 GetCoordiante_sensorFrame( int m_nSensors.X() , int m_nSensors.Y() )    { return  m_BGOsensorMatrix[m_nSensors.X()][m_nSensors.Y()][0]->GetPosition_local() ; };
  //TVector3 GetCoordiante_detectorFrame( int m_nSensors.X() , int m_nSensors.Y() )  { return  m_BGOsensorMatrix[m_nSensors.X()][m_nSensors.Y()][0]->GetPosition() ; };
  // float GetCoordiante_footFrame( int m_nSensors.X() , int m_nSensors.Y() , int m_nSensors.Z() );
  
  // TVector3 GetCoordiante_sensorFrame( int ID_BGO )    { return  m_coordinate -> GetPosition_local().X() ; };
  TVector3 GetCoordiante_detectorFrame( int ID_BGO );
  

  string PrintBodies();
  string PrintRegions();
  string PrintAssignMaterial();
  string PrintSubtractBodiesFromAir();
  string PrintParameters();

  // Return a vector with the number of sensors along the cartesian directions
  // TVector3        GetNumberOfSensorAlongDirections() { return m_NSensors; };

  TGeoVolume*     GetVolume();


  virtual void    Clear(Option_t* opt="");
  virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;



private:

  BGO_SensorMatrix m_BGOsensorMatrix;
  // PassiveMatrix m_passiveMatrix;
  // PassiveMatrix m_chipMatrix;
  TRotation* m_rotation;

  // TGeoVolume* m_universe;

  TVector3  m_origin;  // current position in local coord.
  TVector3  m_center;  // current position in global coord.
  TVector3  m_dimension;
  TVector3  m_coordinate;   // xyz BGO coordinates 
  TVector3* coordinate;

  vector<string> m_passiveMaterial;

  int m_nCry;
  int m_debug;

  int m_volumeCount;
  // int m_passiveCount;

  int m_nBGO_X;
  int m_nBGO_Y;
  //int m_nSensors_Z;
  TVector3 m_nSensors;    // BGO sensor numbers in each coordinate

  vector<string> m_materialOrder;

  map<string, double> m_materialThick;
  map<string, string> m_materialType;

  double m_BGOSensorThick_Lz;

  double m_BGO_Pitch_X;
  double m_BGO_Pitch_Y;
  // int m_debug;
  // int m_setW_0number;


  ClassDef(TACAparGeo,1)
};

#endif

#ifndef _TACAparGeo_HXX
#define _TACAparGeo_HXX

#include "Riostream.h"

#include "TEveGeoShapeExtract.h"

#include "TObject.h"

#include "TACAparTools.hxx"

#include "GlobalPar.hxx"
//#include "IronPlate.hxx"
#include "LightSabre.hxx"
#include "FootBox.hxx"
#include "FootField.hxx"

#include <FieldManager.h>




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
  // void Global2Local( TVector3* glob );
  // void Global2Local_RotationOnly( TVector3* glob );
  // void Local2Global( TVector3* loc );
  // void Local2Global_RotationOnly( TVector3* loc );
  
  /*
  TRotation GetRotationToGlobal() { return *m_rotation; };
  TRotation GetRotationToLocal() { return m_rotation->Inverse(); };
  
  // Return the pixel position  -->  change name! in GetPixelPos()
  //    it should be changed arrirdingly with the simulation choice when more than one sensors will be used
  TVector3 GetPosition( int layer, int plume, int chip, int col, int row );

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
  
  void AssignMaterial() {};
  void AssignMagnetField() {};
  */
  string PrintBodies();
  string PrintRegions();
  string PrintAssignMaterial();
  string PrintSubtractBodiesFromAir();
  string PrintParameters();

  // Return a vector with the number of sensors along the cartesian directions
  // TVector3        GetNumberOfSensorAlongDirections() { return m_NSensors; };

  // TGeoVolume*     GetVolume();


  virtual void    Clear(Option_t* opt="");
  virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;



private:

  BGO_SensorMatrix m_BGOsensorMatrix;
  //PassiveMatrix m_passiveMatrix;
  // PassiveMatrix m_chipMatrix;
  TRotation* m_rotation;

  // TGeoVolume* m_universe;

  TVector3  m_origin;  // current position in local coord.
  TVector3  m_center;  // current position in global coord.
  TVector3  m_dimension;

  vector<string> m_passiveMaterial;

  int m_nCry;
  int m_debug;

  int m_volumeCount;
  // int m_passiveCount;

  int m_nBGO_X;
  int m_nBGO_Y;
  //int m_nSensors_Z;
  TVector3 m_nSensors;

  // double m_plumeDistace_Z;
  // double m_plumeDistace_Y;
  // double m_boardDeadMin;
  // double m_boardDeadMax;

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

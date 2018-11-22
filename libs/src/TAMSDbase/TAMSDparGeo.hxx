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
#include "GlobalPar.hxx"
#include <FieldManager.h>

#include "foot_geo.h"


/*

	layer = consider a layer formd by 2 xy strip planes -> 3 layers
	view = colums or rows, 

*/


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
	void Detector2Sensor_frame( int layer, int view, int strip, TVector3* coord );
    void Sensor2Detector_frame( int layer, int view, int strip, TVector3* coord );
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

	// Return the pixel position  -->  change name! in GetPixelPos()
	//    it should be changed arrirdingly with the simulation choice when more than one sensors will be used
	// TVector3 GetPosition( int layer, int col, int row );


	//  Return MSD center coord. in the global frame
	TVector3 GetCenter() { return m_center; };

	// Return Inner Trakcker full dimension.
	TVector3 GetDimension() { return m_dimension; };

	TVector3 GetStripDimension() { return m_dimensionStrip; };   // ( shorter dim, longer dim, z dim )

	TVector3 GetLayerCenter( int layer );
	TVector3 GetViewLayerCenter( int layer, int view );
	double GetSingleSensorThickness() { return m_dimension.z(); };

	// Return distance from center to center
	double GetLayerDistance() { return m_layerDistance; };

	int GetNStrip() { return m_nStrip; };
	// int GetNView() { return m_nView; };
    int GetNLayers() { return m_nLayer; };
    int GetNSensors() { return m_nSensors.x() * m_nSensors.y() * m_nSensors.z(); };

	// double GetNPixelX() { return m_nPixel_X; };
	// double GetNPixelY() { return m_nPixel_Y; };
	// int GetNLayers() { return m_nSensors_Z; };


    int SensorZ_From_LayerView( int layer, int view ) { return layer*2+view; };

	// Return the strip center position 
    float GetStripCoordiante_sensorFrame( int layer, int view, int strip )    { return ( view == 0 ? m_sensorMatrix[SensorZ_From_LayerView(layer,view)][0][0]->GetPosition_local(strip,0).X() : m_sensorMatrix[SensorZ_From_LayerView(layer,view)][0][0]->GetPosition_local(0,strip).Y() ); };
    float GetStripCoordiante_detectorFrame( int layer, int view, int strip )  { return ( view == 0 ? m_sensorMatrix[SensorZ_From_LayerView(layer,view)][0][0]->GetPosition(strip,0).X() : m_sensorMatrix[SensorZ_From_LayerView(layer,view)][0][0]->GetPosition(0,strip).Y() ); };
    float GetStripCoordiante_footFrame( int layer, int view, int strip );

    float GetStripZ_sensorFrame( int layer, int view, int strip )    { return ( view == 0 ? m_sensorMatrix[SensorZ_From_LayerView(layer,view)][0][0]->GetPosition_local(strip,0).Z() : m_sensorMatrix[SensorZ_From_LayerView(layer,view)][0][0]->GetPosition_local(0,strip).Z() );  };
    float GetStripZ_detectorFrame( int layer, int view, int strip )  { return ( view == 0 ? m_sensorMatrix[SensorZ_From_LayerView(layer,view)][0][0]->GetPosition(strip,0).Z() : m_sensorMatrix[SensorZ_From_LayerView(layer,view)][0][0]->GetPosition(0,strip).Z() ); };
    float GetStripZ_footFrame( int layer, int view, int strip );



	string PrintBodies();
	string PrintRegions();
	string PrintAssignMaterial();
	string PrintSubtractBodiesFromAir();
	string PrintParameters();

	// Return a vector with the number of sensors along the cartesian directions
	TVector3        GetNumberOfSensorAlongDirections() { return m_nSensors; };

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
	TVector3  m_sensorDimension;  
	TVector3  m_dimensionStrip;  

	// int m_nSensors_X;
	// int m_nSensors_Y;
	// int m_nSensors_Z;
	int m_nStrip;
	int m_nView;
	int m_nLayer;
	int m_layer_EdgeToEdgeDistance;
	TVector3 m_nSensors;   

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

	// int m_layer_EdgeToEdgeDistancem_layer_EdgeToEdgeDistance;
	// int m_nPixel_Y;

	int m_debug;
	int m_setW_0number;


   
   ClassDef(TAMSDparGeo,1)
};

#endif































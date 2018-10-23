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

#include "TAVTbaseParGeo.hxx"

#include "IronPlate.hxx"
#include "FootBox.hxx"
#include "GlobalPar.hxx"

#include <FieldManager.h>



class TGeoHMatrix;
class TGeoVolume;
//##############################################################################

class TAITparGeo : public TAVTbaseParGeo {

public:

    TAITparGeo();
    TAITparGeo( TAITparGeo* original );

    virtual ~TAITparGeo() {}
   
    void InitGeo();
    void InitMaterial();
    void DefineMaterial();

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

    // Return a vector with the number of sensors along the cartesian directions 
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


    // function for the FRUKA geometry creation
    string PrintBodies();
    string PrintRegions();
    string PrintAssignMaterial();
    string PrintSubtractBodiesFromAir();
    string PrintParameters();
   
    TGeoVolume*     GetVolume();

   //! Add CMOS module geometry to world
   TGeoVolume* AddModule(const char* basemoduleName = "Module", const char *name = "IT");
   
   //! Build Vertex
   TGeoVolume* BuildInnerTracker(const char* basemoduleName = "Module", const char *name = "IT");

   
    virtual void    Clear(Option_t* opt="");
    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;

public:
   static const Char_t* GetDefParaName() { return fgkDefParaName.Data(); }

   
private:
   PassiveMatrix m_chipMatrix;

    double m_plumeDistace_Z;
    double m_plumeDistace_Y;
    double m_boardXDeadMin;
    double m_boardXDeadMax;
    double m_boardYDeadMin;
    double m_boardYDeadMax;

    double m_layerDistance;

    vector<vector<double>> m_xmin;
    vector<vector<double>> m_ymin;

private:
   static TString fgkDefParaName;


   ClassDef(TAITparGeo,1)
};

#endif

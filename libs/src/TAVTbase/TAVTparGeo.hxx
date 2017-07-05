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
#include "TString.h"
#include "TVector3.h"
#include "TRotation.h"

#include "TAVTparTools.hxx"

#include "IronPlate.hxx"

class TGeoHMatrix;
class TGeoVolume;
//##############################################################################

class TAVTparGeo : public TAVTparTools {


typedef vector< vector< vector< IronPlate* > > > SensorMatrix;
// typedef map< int, map< int, map< int, IronPlate* > > > SensorMatrix;
      

public:

    TAVTparGeo() {};
    TAVTparGeo( TAVTparGeo* original );
    virtual ~TAVTparGeo() {};

    void InitGeo();
    void InitMaterial();

    //! get position from pixel line/column
    Float_t GetPositionU(Int_t column)         const;
    Float_t GetPositionV(Int_t line)           const;

   
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

    //  Return Vertex center coord. in the global frame
    TVector3 GetCenter() { return m_center; };

    // Return Inner Trakcker full dimension.
    TVector3 GetDimension() { return m_dimension; };

    double GetSingleSensorThickness() { return m_siliconSensorThick_Lz; };

    // Return distance from center to center
    double GetLayerDistance() { return m_layerDistance; };

    double GetNPixelX() const { return m_nPixel_X; };
    double GetNPixelY() const { return m_nPixel_Y; };
   
    double GetPitchX()  const { return m_Pitch_X; };
    double GetPitchY()  const { return m_Pitch_Y; };

    int    GetNLayers() const { return m_nSensors_Z; };

    int    GetSensorsN() const { return m_nSensors_Z; };
   

    // Return a vector with the number of sensors along the cartesian directions
    TVector3        GetNumberOfSensorAlongDirections() { return m_NSensors; };

    TGeoVolume*     GetVolume(); 

    virtual void    Clear(Option_t* opt="");
    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;


private:

    SensorMatrix m_sensorMatrix;
    TRotation* m_rotation;


    // TObjArray* fMatrixList;       //! list of transformation matrices  (rotation+translation for each sensor)
    TVector3  m_origin;  // current position
    TVector3  m_center;  // current position
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

    double m_Pitch_X;
    double m_Pitch_Y;

   ClassDef(TAVTparGeo,1)
};

#endif































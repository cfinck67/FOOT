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

#include "TAVTbaseParGeo.hxx"
#include "IronPlate.hxx"

#include <FieldManager.h>

#include "foot_geo.h"
#include "GlobalPar.hxx"

class TGeoHMatrix;
class TGeoVolume;
//##############################################################################

class TAMSDparGeo : public TAVTbaseParGeo {

public:

    TAMSDparGeo();
    TAMSDparGeo( TAMSDparGeo* original );
    virtual ~TAMSDparGeo();

    void InitGeo();
    void InitMaterial();
    void DefineMaterial();


    // Return the pixel position  -->  change name! in GetPixelPos()
    //    it should be changed arrirdingly with the simulation choice when more than one sensors will be used
    TVector3 GetPosition( int layer, int col, int row );

    TVector3 GetLayerCenter( int layer );

    // Return distance from center to center
    double GetLayerDistance() { return m_layerDistance; };

   
    string PrintParameters();

private:
   static TString fgkDefParaName;
   
public:
   static const Char_t* GetDefParaName() { return fgkDefParaName.Data(); }

private:
    int m_nSensors_X;
    int m_nSensors_Y;
    int m_nSensors_Z;

    double m_layerDistance;
    double m_layerThick;
    double m_nSensor_X_Layer;

   
   ClassDef(TAMSDparGeo,1)
};

#endif































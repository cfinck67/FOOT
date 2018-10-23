#ifndef _TAVTparGeo_HXX
#define _TAVTparGeo_HXX
/*!
  \version $Id: TAVTparGeo.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  
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

#include "TObject.h"
#include "TString.h"
#include "TVector3.h"

#include "TAVTbaseParGeo.hxx"

#include <FieldManager.h>


class TGeoHMatrix;
class TGeoVolume;
//##############################################################################

class TAVTparGeo : public TAVTbaseParGeo {

public:
   TAVTparGeo();

   void   InitGeo();
   void   InitMaterial();
   void   DefineMaterial();
   
   string PrintParameters();

   //! Add CMOS module geometry to world
   TGeoVolume* AddVertexModule(const char* basemoduleName = "Module", const char *name = "Vertex");
   
   //! Build Vertex
   TGeoVolume* BuildVertex( const char *name = "Vertex", const char* basemoduleName = "Module");
   
private:
   static TString fgkDefParaName;
   
public:
   static const Char_t* GetDefParaName() { return fgkDefParaName.Data(); }
   
   
   ClassDef(TAVTparGeo,1)
};

#endif

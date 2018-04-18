#ifndef _TAIRparGeo_HXX
#define _TAIRparGeo_HXX
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TEveGeoShapeExtract.h"

#include "TObject.h"

#include "TAGparTools.hxx"

#include "GlobalPar.hxx"




class TGeoHMatrix;
class TGeoVolume;
//##############################################################################

class TAIRparGeo : public TAGparTools {
  
public:

  TAIRparGeo();
  // TAIRparGeo( TAIRparGeo* original );
  virtual ~TAIRparGeo() {};
  
  void InitGeo() {};
  // void InitMaterial();

  // //! Transform point from the global reference frame
  // //! to the local reference frame of the detection id
  // void Global2Local( TVector3* glob );
  // void Global2Local_TranslationOnly( TVector3* glob );
  // void Global2Local_RotationOnly( TVector3* glob );
  // void Local2Global( TVector3* loc );
  // void Local2Global_TranslationOnly( TVector3* loc );
  // void Local2Global_RotationOnly( TVector3* loc );


  // TRotation GetRotationToGlobal() { return *m_rotation; };
  // TRotation GetRotationToLocal() { return m_rotation->Inverse(); };


  
  string PrintBodies();
  string PrintRegions();
  string PrintAssignMaterial();
  
  // Return a vector with the number of sensors along the cartesian directions

  // TGeoVolume*     GetVolume();
  
  virtual void    Clear(Option_t* opt="");
  virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;
  

private:
  
  // TRotation* m_rotation;

  // TGeoVolume* m_universe;
  
  TVector3  m_origin;  // current position in local coord.
  TVector3  m_center;  // current position in global coord.
  TVector3  m_dimension;

  ClassDef(TAIRparGeo,1)
};

#endif

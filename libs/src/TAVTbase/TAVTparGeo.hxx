#ifndef _TAVTparGeo_HXX
#define _TAVTparGeo_HXX
/*!
  \file
  \brief   Declaration of TAVTparGeo.
 
  \author Ch. Finck
*/
/*------------------------------------------+---------------------------------*/

#include <map>
#include "Riostream.h"

#include "TObject.h"
#include "TString.h"

#include "TAVTbaseParGeo.hxx"

//##############################################################################

class TAVTparGeo : public TAVTbaseParGeo {
      

protected:
   static const TString fgkBaseName;   // Vtx base name
   static const TString fgkDefParaName;

public:
   TAVTparGeo();
    virtual ~TAVTparGeo();
  
   // Define materials
   virtual void  DefineMaterial();

   //! Add CMOS module geometry to world
   TGeoVolume* AddModule(const char* basemoduleName = "Module", const char *name = "Vertex");
   
   //! Build Vertex
   TGeoVolume* BuildVertex( const char *name = "Vertex", const char* basemoduleName = "M28");
   
public:
   static const Char_t* GetBaseName()    { return fgkBaseName.Data();    }
   static const Char_t* GetDefParaName() { return fgkDefParaName.Data(); }

   ClassDef(TAVTparGeo,1)
};

#endif

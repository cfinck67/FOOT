#ifndef _TAMSDparGeo_HXX
#define _TAMSDparGeo_HXX
/*!
  \file
  \version $Id: TAMSDparGeo.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TAMSDparGeo.
*/
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TAVTparGeo.hxx"

class TGeoHMatrix;
class TGeoVolume;
//##############################################################################

class TAMSDparGeo : public TAVTparGeo {

  
public:
   TAMSDparGeo();
   virtual ~TAMSDparGeo();

   // Define materials
   void        DefineMaterial();
   
   //! Build MSD
   TGeoVolume* BuildMultiStripDetector(const char* basemoduleName = "Module", const char *name = "MSD");

private:
   static const TString fgkBaseName;   // MSD base name
   static const TString fgkDefParaName;
   
public:
   static const Char_t* GetBaseName()    { return fgkBaseName.Data();    }
   static const Char_t* GetDefParaName() { return fgkDefParaName.Data(); }

   
   ClassDef(TAMSDparGeo,1)
};

#endif































#ifndef _TATRparGeo_HXX
#define _TATRparGeo_HXX
/*!
 \file
 \version $Id: TATRparGeo.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
 \brief   Declaration of TATRparGeo.
 */
/*------------------------------------------+---------------------------------*/
#include "TGeoVolume.h"
#include "TString.h"
#include "TVector3.h"

#include "TAGparTools.hxx"

//##############################################################################

class TATRparGeo : public TAGparTools {
public:
   
   TATRparGeo();
   virtual         ~TATRparGeo();
   
   TVector3        GetCenter()   const { return fCenter;     }
   TVector3        GetSize()      const { return fSize;     }
   TString         GetMaterial()  const { return fMaterial; }
   Float_t         GetDensity()   const { return fDensity;  }

   Bool_t          FromFile(const TString& name = "");
   void            DefineMaterial();

   virtual void    Clear(Option_t* opt="");
   
   virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;
   
   TGeoVolume*     BuildStartCounter(const char *bmName = "ST");

  string PrintBodies();
  string PrintRegions();
  string PrintAssignMaterial();

public:
   static const Char_t* GetBaseName()    { return fgkBaseName.Data();    }
   static const Char_t* GetDefParaName() { return fgkDefParaName.Data(); }
   
   ClassDef(TATRparGeo,1)
   
private:
   TVector3  fSize;
   TVector3  fCenter;
   TString   fMaterial;
   Float_t   fDensity;
   
private:
   static const TString fgkBaseName;
   static const TString fgkDefParaName;
};

#endif

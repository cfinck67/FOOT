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

#include "TAGpara.hxx"

//##############################################################################

class TATRparGeo : public TAGpara {
public:
   
   TATRparGeo();
   virtual         ~TATRparGeo();
   
   Double_t        GetThickness() const { return fThick;  }
   Double_t        GetRadius()    const { return fRadius; }
   
   void            InitGeo();
   void            DefineMaterial();

   
   virtual void    Clear(Option_t* opt="");
   
   virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;
   
   TGeoVolume*     BuildStartCounter(const char *bmName = "ST");
   
public:
   static const Char_t* GetDefParaName() { return fgkDefParaName.Data(); }
   
   ClassDef(TATRparGeo,1)
   
private:
   Double_t fThick;
   Double_t fRadius;
   
private:
   static const TString fgkDefParaName;
};

#endif

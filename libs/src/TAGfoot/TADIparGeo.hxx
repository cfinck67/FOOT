#ifndef _TADIparGeo_HXX
#define _TADIparGeo_HXX
/*!
  \file
  \version $Id: TADIparGeo.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TADIparGeo.
*/
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"


#include "TObject.h"

#include "TAGpara.hxx"


class TGeoVolume;
//##############################################################################

class TADIparGeo : public TAGpara {
      
private:
   Float_t    fWidth;            // Width of the dipole 
   Float_t    fHeight;           // Height of the dipole
   Float_t    fLength;           // Length of the dipole
   Int_t      fDebugLevel;       // debug level

public:

   TADIparGeo();
    virtual ~TADIparGeo();

     
   Float_t GetWidth()   const { return fWidth;          }
   Float_t GetHeight()  const { return fHeight;         }
   Float_t GetLength()  const { return fLength;         }

   //! Init geo (compliant with all par geo
   void InitGeo();
    
   //! Add Dipole
   TGeoVolume* AddDipole(const char *dipoleName = "Dipole");
   
   
   ClassDef(TADIparGeo,1)
};

#endif

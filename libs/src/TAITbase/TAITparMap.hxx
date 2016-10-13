#ifndef _TAITparMap_HXX
#define _TAITparMap_HXX
/*!
 \file
 \version $Id: TAITparMap.hxx,v 1.1 2001/11/05 23:13:57 mueller Exp $
 \brief   Declaration of TAITparMap.
 */
/*------------------------------------------+---------------------------------*/


#include "TAGpara.hxx"

//##############################################################################

class TAITparMap : public TAGpara {
   
public:
   TAITparMap();
   virtual ~TAITparMap();

   //! get position from pixel line/column
   Float_t GetPositionU(Int_t column)         const;
   Float_t GetPositionV(Int_t line)           const;
   Int_t   GetIndex(Int_t line, Int_t column) const;

   //! return number of pixels in U direction
   Int_t   GetPixelsNu()   const { return fPixelsNu;  }
   
   //! return number of pixels in V direction
   Int_t   GetPixelsNv()   const { return fPixelsNv;  }
   
   //!  return pitch value in U direction
   Float_t GetPitchU()     const { return fPitchU;    }
   
   //!  return pitch value in V direction 
   Float_t GetPitchV()     const { return fPitchV;    }
   
   //! return pixel size
   Float_t GetPixelSize()  const { return fPixelSize; }
   
public:   
   static Int_t GetSensorsN() { return fgkSensorsN; }
   static Int_t GetRealId(Int_t mcSensorId);

private:
  
   Int_t fPixelsNu;     // Number of pixels in U direction
   Int_t fPixelsNv;     // Number of pixels in V direction
   
   Float_t fPitchU;    // Pitch value in U direction
   Float_t fPitchV;    // Pitch value in U direction
   
   Float_t fPixelSize; // Pixel size (same for all direction)
   
private:
   static const Int_t fgkSensorsN;          // number of sensors
   
   ClassDef(TAITparMap,1)
  
};

#endif

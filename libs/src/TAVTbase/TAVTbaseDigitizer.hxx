#ifndef _TAVTbaseDigitizer_HXX
#define _TAVTbaseDigitizer_HXX
/*!
 \file
 \version $Id: TAVTbaseDigitizer.hxx,v $
 \brief   Declaration of TAVTbaseDigitizer.
 */
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"
#include <map>

#include "TH2F.h"
#include "TObject.h"
#include "TString.h"
#include "TList.h"

#include "TAGparaDsc.hxx"


class TF1;
class TAVTbaseDigitizer : public TObject {
   
public:
   TAVTbaseDigitizer(TAGparaDsc* parGeo);
   virtual ~TAVTbaseDigitizer();
   
   Double_t     FuncClusterSize(Double_t* x, Double_t* par);
   Bool_t       Process( Double_t edep, Double_t x0, Double_t y0, Double_t zin = 0, Double_t zout = 0);

   virtual Bool_t MakeCluster(Double_t x0, Double_t y0, Double_t zin = 0, Double_t zout = 0) = 0;

   Int_t        GetPixelsN() const           { return fPixelsN;    }
   
   void         SetRsPar(Double_t par)       { fRsPar = par;       }
   void         SetRsParErr(Double_t par)    { fRsParErr = par;    }
   void         SetThresPar(Double_t par)    { fThresPar = par;    }
   void         SetThresParErr(Double_t par) { fThresParErr = par; }
   
   
   Float_t      GetNPixelX() const           { return fPixelsNx;   }
   Float_t      GetNPixelY() const           { return fPixelsNy;   }
   
   Float_t      GetPitchX()  const           { return fPitchX;     }
   Float_t      GetPitchY()  const           { return fPitchX;     }

   //! Get Map
   std::map<int, int>  GetMap() const        { return fMap;        }
   
   //! Get column/line from x/y position
   Int_t        GetColumn(Float_t x) const;
   Int_t        GetLine(Float_t y)   const;
   Int_t        GetIndex(Int_t line, Int_t column) const;
   
   //! Get remainder of column/line
   Float_t      GetColRemainder(Float_t x)  const;
   Float_t      GetLineRemainder(Float_t y) const;
   
   //! Get pixel region of column/line
   Int_t        GetColRegion(Float_t x)  const;
   Int_t        GetLineRegion(Float_t y) const;
   
public:
   static Double_t GeV2keV()                 { return fgkGeV2keV;  }
   static Double_t Cm2Mu()                   { return fgkCm2Mu;    }
   
   static Int_t    GetRandom (Int_t i)       { return std::rand() % i;}

protected:
   TAGparaDsc* fpParGeo;
   TF1*        fFuncClusterSize; // cluster size function
   
   std::map<int, int> fMap;      // map of found pixels
   
   Int_t       fPixelsN;         // number of pixels for a given eloss
   Double_t    fRsPar;           // parameter r_s for the cluster size function
   Double_t    fRsParErr;        // parameter r_s for the cluster size function
   Double_t    fThresPar;        // parameter threshold for cluster size function
   Double_t    fThresParErr;     // parameter threshold for cluster size function

   Int_t       fPixelsNx;        // number of pixels in X (colummn)
   Int_t       fPixelsNy;        // number of pixels in Y (line)
   
   Float_t     fPitchX;          // pitch in X
   Float_t     fPitchY;          // pitch in Y
   
   Int_t*      fShel;

   Int_t       fDebugLevel;      // debug
   
protected:
   void        SetFunctions();
   Int_t       GetLastShell(Int_t* shell, Int_t maxTurn) const;
   
protected:
   static Double_t fgkGeV2keV;
   static Double_t fgkCm2Mu;
   

   ClassDef(TAVTbaseDigitizer,0)
};

#endif

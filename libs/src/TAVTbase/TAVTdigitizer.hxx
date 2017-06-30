#ifndef _TAVTdigitizer_HXX
#define _TAVTdigitizer_HXX
/*!
 \file
 \version $Id: TAVTdigitizer.hxx,v $
 \brief   Declaration of TAVTdigitizer.
 */
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"
#include <map>

#include "TH2F.h"
#include "TObject.h"
#include "TString.h"
#include "TList.h"

#include "TAGparaDsc.hxx"


struct RawPixel_t : public  TObject {
     RawPixel_t() {line = column = 0;}
	  Int_t line;    // pixel line id
	  Int_t column;  // pixel column id
};


class TF1;
class TAVTdigitizer : public TObject {
   
public:
   TAVTdigitizer(TAGparaDsc* parGeo);
   virtual ~TAVTdigitizer();
   
   Bool_t       Process(Double_t edep, Double_t x0, Double_t y0);
   Double_t     FuncClusterSize(Double_t* x, Double_t* par);
   
   Int_t        GetPixelsN() const           { return fPixelsN;    }
   
   void         SetRsPar(Double_t par)       { fRsPar = par;       }
   void         SetRsParErr(Double_t par)    { fRsParErr = par;    }
   void         SetThresPar(Double_t par)    { fThresPar = par;    }
   void         SetThresParErr(Double_t par) { fThresParErr = par; }
   
   
   Float_t      GetNPixelX() const           { return fPixelsNx;   }
   Float_t      GetNPixelY() const           { return fPixelsNy;   }
   
   Float_t      GetPitchX()  const           { return fPitchX;     }
   Float_t      GetPitchY()  const           { return fPitchX;     }

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
   
private:
   TAGparaDsc* fpParGeo;
   TList*      fListOfPixels;    // list of pixel influenced by the deposited charge
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
   
   Int_t       fDebugLevel;      // debug
   
private:
   void        SetFunctions();
   Bool_t      MakeCluster(Double_t x0, Double_t y0);
   
private:   
   static Double_t fgkGeV2keV;
   
   //00
   static Int_t   fgkShel00[];
   static Float_t fgkAngD00[];
   static Float_t fgkFacX00[];
   static Float_t fgkFacY00[];
   
   //55
   static Int_t   fgkShel55[];
   static Float_t fgkAngD55[];
   static Float_t fgkFacX55[];
   static Float_t fgkFacY55[];
   
   //05
   static Int_t   fgkShel50[];
   static Float_t fgkAngD50[];
   static Float_t fgkFacX50[];
   static Float_t fgkFacY50[];
   
   //50
   static Float_t fgkAngD05[];

   static Int_t   fgkMaxTurn;

   ClassDef(TAVTdigitizer,0)
};

#endif

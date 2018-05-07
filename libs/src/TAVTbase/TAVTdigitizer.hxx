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
#include "TAVTbaseDigitizer.hxx"


class TF1;
class TAVTdigitizer : public TAVTbaseDigitizer {
   
public:
   TAVTdigitizer(TAGparaDsc* parGeo);
   virtual ~TAVTdigitizer();
   
   Bool_t MakeCluster(Double_t x0, Double_t y0, Double_t zin = 0, Double_t zout = 0);

private:   
   std::map<int, int> fMapLast;  // map of found pixels of the outer shell
   
   Float_t*    fAngD;
   Float_t*    fFacX;
   Float_t*    fFacY;
   
private:
   void        RemovePixels(Int_t rpixels); // remove number of pixels from last shell randomly
   Bool_t      SetRegion(Int_t regX, Int_t regY);
   
private:
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

#ifndef _TAVTdigitizerE_HXX
#define _TAVTdigitizerE_HXX
/*!
 \file
 \version $Id: TAVTdigitizerE.hxx,v $
 \brief   Declaration of TAVTdigitizerE.
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
class TAVTdigitizerE : public TAVTbaseDigitizer {
   
public:
   TAVTdigitizerE(TAGparaDsc* parGeo);
   virtual ~TAVTdigitizerE();
   
   Bool_t MakeCluster(Double_t x0, Double_t y0, Double_t zin = 0, Double_t zout = 0);

private:
   std::vector<std::pair<int, float> > fDistance;
   Int_t*         fDeltaRad;
   
private:
   void            RemovePixels(Int_t rpixels); // remove number of pixels from last shell randomly
   Bool_t          SetRegion(Float_t x0, Float_t y0);
   
private:
   static Int_t    fgkMaxTurn;
   static  Float_t fgkFactorRad;
   
   //00
   static Int_t    fgkShel00[];
   static Int_t    fgkDeltaRad00[];
   
   //55
   static Int_t    fgkShel55[];
   static Int_t    fgkDeltaRad55[];
   
   //05
   static Int_t    fgkShel50[];
   static Int_t    fgkDeltaRad50[];
   
public:
   static Bool_t    SortBack (const std::pair<Int_t, Float_t>  x, const std::pair<Int_t, Float_t>  y) { return (x.second > y.second); }
   
   ClassDef(TAVTdigitizerE,0)
};

#endif

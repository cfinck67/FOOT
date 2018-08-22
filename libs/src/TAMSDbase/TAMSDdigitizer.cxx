#include "TAMSDdigitizer.hxx"
#include "TAVTparGeo.hxx"

#include <math.h>
#include "TVector2.h"
#include "TRandom.h"
#include "TList.h"

ClassImp(TAMSDdigitizer);

using namespace std;

Float_t TAMSDdigitizer::fgChargeGain      = 200;
Float_t TAMSDdigitizer::fgChargeFac       = 0.8;


// --------------------------------------------------------------------------------------
TAMSDdigitizer::TAMSDdigitizer(TAVTbaseParGeo* parGeo)
: TAVTbaseDigitizer(parGeo)
{
}

// --------------------------------------------------------------------------------------
TAMSDdigitizer::~TAMSDdigitizer() 
{   
}

//------------------------------------------+-----------------------------------
//! fill pixel signal
Bool_t TAMSDdigitizer::Process( Double_t edep, Double_t x0, Double_t y0,  Double_t zin, Double_t zout)
{
   if (fgSmearFlag) {
      Float_t dx = gRandom->Gaus(0, fPitchX/2.);
      Float_t dy = gRandom->Gaus(0, fPitchX/2.);
      x0 += dx;
      y0 += dy;
   }

   fMap.clear();
   
   Int_t line = GetLine(y0);
   Int_t col  = GetColumn(x0);
   
   Double_t value = edep*fgChargeGain*fgChargeFac;
   FillMap(line, col, value);
   
   if (line+1 < fPixelsNx) {
      Float_t value = edep*fgChargeGain*(1-fgChargeFac/2.);
      FillMap(line+1, col, value);
   }
   
   if (col+1 < fPixelsNy) {
      Float_t value = edep*fgChargeGain*(1-fgChargeFac/2.);
      FillMap(line, col+1, value);
   }
   
   if (line-1 >= 0) {
      Float_t value = edep*fgChargeGain*(1-fgChargeFac/2.);
      FillMap(line-1, col, value);
   }
   
   if (col-1 >= 0) {
      Float_t value = edep*fgChargeGain*(1-fgChargeFac/2.);
      FillMap(line, col-1, value);
   }
   
   return true;
}


// --------------------------------------------------------------------------------------
void TAMSDdigitizer::FillMap(Int_t line, Int_t col, Double_t value)
{
   Int_t idx  = GetIndex(line, col);
   if (idx < 0) return;
   fMap[idx] = value;

}



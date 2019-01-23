/*!
 \file
 \version $Id: TAVTbaseDigitizer.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
 \brief   Implementation of TAVTbaseDigitizer.
 */
#include "TMath.h"
#include "TGeoMatrix.h"
#include "TLegend.h"
#include "TList.h"
#include "TF1.h"
#include "TStyle.h"
#include "TRandom2.h"

#include "TAGgeoTrafo.hxx"

#include "TAVTparGeo.hxx"
#include "TAVTbaseDigitizer.hxx"

/*!
 \class TAVTbaseDigitizer 
 \brief digitizer for pixel **
 */

ClassImp(TAVTbaseDigitizer);


Bool_t   TAVTbaseDigitizer::fgSmearFlag       = true;
Float_t  TAVTbaseDigitizer::fgDefSmearPos     =  10.35;    // in micron

//------------------------------------------+-----------------------------------
//! Default constructor.
TAVTbaseDigitizer::TAVTbaseDigitizer(TAVTbaseParGeo* parGeo)
: TAGbaseDigitizer(),
  fpParGeo(parGeo),
  fPixelsN(-1),
  fRsPar(0.65),
  fRsParErr(0.05),
  fThresPar(885),
  fThresParErr(250)
{
   SetFunctions();
   fPitchX   = fpParGeo->GetPitchX()*TAGgeoTrafo::CmToMu();
   fPixelsNx = fpParGeo->GetNPixelX();
   fPitchY   = fpParGeo->GetPitchY()*TAGgeoTrafo::CmToMu();
   fPixelsNy = fpParGeo->GetNPixelY();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTbaseDigitizer::~TAVTbaseDigitizer()
{

}

//------------------------------------------+-----------------------------------
//! fill pixel signal
Bool_t TAVTbaseDigitizer::Process( Double_t edep, Double_t x0, Double_t y0, Double_t zin, Double_t zout, Double_t /*time*/, Int_t /*sensorId*/)
{
   x0 *= TAGgeoTrafo::CmToMu();
   y0 *= TAGgeoTrafo::CmToMu();
   
   Double_t deltaE = edep*TAGgeoTrafo::GevToKev();
   Double_t  smear = 0;
   
   smear = gRandom->Gaus(0, fRsParErr);
   fFuncClusterSize->SetParameter(0, fRsPar+smear);

   smear = gRandom->Gaus(0, fThresParErr);
   fFuncClusterSize->SetParameter(1, fThresPar+smear);
   
   fPixelsN = TMath::Nint(fFuncClusterSize->Eval(deltaE));
   
   if (fPixelsN <= 0) fPixelsN = 1;
   
   if (fDebugLevel) {
      printf("\nnext hit:\n");
      printf("eloss %6.1f pixels %d\n", deltaE, fPixelsN);
   }
   
   if (fgSmearFlag) {
      Float_t dx = gRandom->Gaus(0, fgDefSmearPos);
      Float_t dy = gRandom->Gaus(0, fgDefSmearPos);
      x0 += dx;
      y0 += dy;
   }
   
   return MakeCluster(x0, y0, zin, zout);
}


// --------------------------------------------------------------------------------------
void  TAVTbaseDigitizer::SetFunctions()
{
   // compute cluster size for a given Edep, x and y
   fFuncClusterSize = new TF1("ClusterSize", this, &TAVTbaseDigitizer::FuncClusterSize, 0, 2000, 2, "TAVTbaseDigitizer", "FuncClusterSize");
}

// --------------------------------------------------------------------------------------
Double_t TAVTbaseDigitizer::FuncClusterSize(Double_t* x, Double_t* par)
{
   Float_t xx = x[0];
   
   Float_t f = 2*TMath::Pi()*par[0]*TMath::Log(xx/(2*TMath::Pi()*3.6e-3*par[1]));
   
   return f;
}


//_____________________________________________________________________________
Int_t TAVTbaseDigitizer::GetColumn(Float_t x) const
{
   Float_t xmin = -fPixelsNx*fPitchX/2.;
   
   if (x < xmin || x > -xmin) {
      if (fDebugLevel)
         Warning("GetColumn()", "Value of X: %f out of range +/- %f\n", x, xmin);
      return -1;
   }
   
   Int_t col = floor((x-xmin)/fPitchX);
   return col;
}

//_____________________________________________________________________________
Int_t TAVTbaseDigitizer::GetLine(Float_t y) const
{
   // equivalent to  floor((-y-ymin)/fPitchV)-1
   Float_t ymin = -fPixelsNy*fPitchY/2.;
   
   if (y < ymin || y > -ymin) {
      if (fDebugLevel)
         Warning("GetLine()", "Value of Y: %f out of range +/- %f\n", y, ymin);
      return -1;
   }
   
   Int_t line = floor((y-ymin)/fPitchY);
   return fPixelsNy - line - 1;
}


//-----------------------------------------+---------------------------------
Int_t TAVTbaseDigitizer::GetIndex(Int_t line, Int_t column) const
{
   return line*fPixelsNx + column;
}

//_____________________________________________________________________________
Float_t TAVTbaseDigitizer::GetPositionU(Int_t column) const
{
   Float_t x = (float(2*column - fPixelsNx + 1) * fPitchX)/2.;
   return  x;
}

//_____________________________________________________________________________
Float_t TAVTbaseDigitizer::GetPositionV(Int_t line) const
{
   Float_t y = -(float(2*line - fPixelsNy + 1) * fPitchY)/2.;
   return  y;
}


//_____________________________________________________________________________
Float_t TAVTbaseDigitizer::GetColRemainder(Float_t x) const
{
   Float_t xmin = -fPixelsNx*fPitchX/2.;
   
   if (x < xmin || x > -xmin) {
      if (fDebugLevel)
         Warning("GetColumn()", "Value of X: %f out of range +/- %f\n", x, xmin);
      return -1;
   }
   
   Float_t col = (x-xmin)/fPitchX - floor((x-xmin)/fPitchX);
   return col;
}

//_____________________________________________________________________________
Float_t TAVTbaseDigitizer::GetLineRemainder(Float_t y) const
{
   // equivalent to  floor((-y-ymin)/fPitchV)-1
   Float_t ymin = -fPixelsNy*fPitchY/2.;
   
   if (y < ymin || y > -ymin) {
      if (fDebugLevel)
         Warning("GetLine()", "Value of Y: %f out of range +/- %f\n", y, ymin);
      return -1;
   }
   
   Float_t line = 1 -((y-ymin)/fPitchY - floor((y-ymin)/fPitchY));
   return line;
}


//_____________________________________________________________________________
Int_t TAVTbaseDigitizer::GetColRegion(Float_t x) const
{
   Float_t rem = GetColRemainder(x);
   
   if (rem < 0.33) // hit a corner
      return 0;
   else if (rem > 0.33 && rem < 0.66)
      return 5; // hit the middle of the pixel
   else if (rem > 0.66)
      return 1; // hit the upper cormer
   else
      return -99;
}

//_____________________________________________________________________________
Int_t TAVTbaseDigitizer::GetLineRegion(Float_t y) const
{
   Float_t rem = GetLineRemainder(y);
   
   if (rem < 0.33)
      return 0;
   else if (rem > 0.33 && rem < 0.66)
      return 5;
   else if (rem > 0.66)
      return 1;
   else
      return -99;
}

//_____________________________________________________________________________
Int_t TAVTbaseDigitizer::GetLastShell(Int_t* shell, Int_t maxTurn) const
{
   Int_t lastTurn = 0;
   
   while (fPixelsN > shell[lastTurn]) {
      lastTurn++;
      if (lastTurn >= maxTurn) {
         lastTurn = maxTurn;
         break;
      }
   }
   
   return lastTurn;
}

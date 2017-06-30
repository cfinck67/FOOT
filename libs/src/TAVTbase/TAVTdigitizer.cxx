/*!
 \file
 \version $Id: TAVTdigitizer.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
 \brief   Implementation of TAVTdigitizer.
 */
#include "TMath.h"
#include "TGeoMatrix.h"
#include "TLegend.h"
#include "TList.h"
#include "TF1.h"
#include "TStyle.h"
#include "TRandom2.h"

#include "TAVTparGeo.hxx"
#include "TAVTdigitizer.hxx"
#include "TAVTdigitizer.hxx"

/*!
 \class TAVTdigitizer 
 \brief digitizer for pixel **
 */

ClassImp(TAVTdigitizer);


Double_t TAVTdigitizer::fgkGeV2keV = 1e6;

//------------------------------------------+-----------------------------------
//! Default constructor.
TAVTdigitizer::TAVTdigitizer(TAGparaDsc* parGeo)
: TObject(),
  fpParGeo(parGeo),
  fListOfPixels(new TList()),
  fPixelsN(-1),
  fRsPar(1.6),
  fRsParErr(0.2),
  fThresPar(885),
  fThresParErr(250),
  fDebugLevel(0)
{
   SetFunctions();
   TAVTparGeo* pGeoMap  = (TAVTparGeo*) fpParGeo->Object();
   fPitchX   = pGeoMap->GetPitchX();
   fPixelsNx = pGeoMap->GetNPixelX();
   fPitchY   = pGeoMap->GetPitchY();
   fPixelsNy = pGeoMap->GetNPixelY();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTdigitizer::~TAVTdigitizer()
{
   delete fListOfPixels;
}

//------------------------------------------+-----------------------------------
//! fill pixel signal
Bool_t TAVTdigitizer::Process( Double_t edep, Double_t x0, Double_t y0)
{
   Double_t deltaE = edep*fgkGeV2keV;
   Double_t  smear = 0;
   
  // smear = gRandom->Gaus(0, fRsParErr);
   fFuncClusterSize->SetParameter(0, fRsPar+smear);

 //  smear = gRandom->Gaus(0, fThresParErr);
   fFuncClusterSize->SetParameter(1, fThresPar+smear);
   
   fPixelsN = TMath::Nint(fFuncClusterSize->Eval(deltaE));
   
   if (fPixelsN <= 0) fPixelsN = 1;
   printf("eloss %6.1f pixels %d\n", deltaE, fPixelsN);
   
   return true;
   return MakeCluster(x0, y0);
}

// --------------------------------------------------------------------------------------
Bool_t TAVTdigitizer::MakeCluster(Double_t x0, Double_t y0)
{
   fMap.clear();
   
   // seed
   Int_t line0     = GetLine(y0);
   Int_t col0      = GetColumn(x0);
   Int_t idx0      = GetIndex(line0, col0);
   
   // compute sigma
   Float_t sigma   = TMath::Sqrt(fPixelsN)*2.;
   Float_t sigmaX  = sigma;
   Float_t sigmaY  = sigma;

   Int_t npixels = fPixelsN;
   
   fMap[idx0] = 1;

   while(npixels != 1) {
      Float_t x = gRandom->Gaus(x0, sigmaX);
      Float_t y = gRandom->Gaus(y0, sigmaY);
      
      Int_t line = GetLine(y);
      Int_t col  = GetColumn(x);
      Int_t idx  = GetIndex(line, col);
      
      if (fMap[idx] != 1) {
     //    printf("%d %d\n", line, col);
         npixels--;
         if (npixels == 1 )
            break;
      }
      fMap[idx] = 1;
   }
   
   return true;
}


// --------------------------------------------------------------------------------------
void  TAVTdigitizer::SetFunctions()
{
   // compute cluster size for a given Edep, x and y
   fFuncClusterSize = new TF1("ClusterSize", this, &TAVTdigitizer::FuncClusterSize, 0, 2000, 2, "TAVTdigitizer", "FuncClusterSize");
}

// --------------------------------------------------------------------------------------
Double_t TAVTdigitizer::FuncClusterSize(Double_t* x, Double_t* par)
{
   Float_t xx = x[0];
   
   Float_t f = 2*TMath::Pi()*par[0]*TMath::Log(xx/(2*TMath::Pi()*3.6e-3*par[1]));
   
   return f;
}

//_____________________________________________________________________________
Int_t TAVTdigitizer::GetColumn(Float_t x) const
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
Int_t TAVTdigitizer::GetLine(Float_t y) const
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
Int_t TAVTdigitizer::GetIndex(Int_t line, Int_t column) const
{
   return line*fPixelsNx + column;
}

//_____________________________________________________________________________
Float_t TAVTdigitizer::GetColRemainder(Float_t x) const
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
Float_t TAVTdigitizer::GetLineRemainder(Float_t y) const
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
Int_t TAVTdigitizer::GetColRegion(Float_t x) const
{
   Float_t rem = GetColRemainder(x);
   
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
Int_t TAVTdigitizer::GetLineRegion(Float_t y) const
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


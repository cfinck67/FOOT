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
Int_t    TAVTdigitizer::fgkMaxTurn = 8;

//00
Int_t   TAVTdigitizer::fgkShel00[] = {    4,    12,    16,    32,    36,    52,    68,    80,    88 };
Float_t TAVTdigitizer::fgkAngD00[] = { 90.0,  45.0,  90.0,  22.5,  90.0,  11.3,  22.5,  22.5,  29.9 };
Float_t TAVTdigitizer::fgkFacX00[] = {  0.5,   0.5,   1.5,   0.5,   2.5,   0.5,   2.5,   1.5,   2.3 };
Float_t TAVTdigitizer::fgkFacY00[] = {  0.5,   1.5,   1.5,   2.5,   2.5,   3.5,   3.5,   4.3,   4.3 };

//55
Int_t   TAVTdigitizer::fgkShel55[] = {    5,     9,    13,    21,    25,    45,    69,    97,   101 };
Float_t TAVTdigitizer::fgkAngD55[] = { 90.0,  11.3,  90.0,  18.5,  90.0,  11.3,  11.3,   4.8,  90.0 };
Float_t TAVTdigitizer::fgkFacX55[] = {  0.0,   0.5,   0.0,   1.0,   2.3,   1.0,   3.0,   0.4,   0.0 };
Float_t TAVTdigitizer::fgkFacY55[] = {  0.6,   1.0,   2.1,   1.8,   2.3,   3.0,   3.0,   4.9,   6.0 };

//05
Int_t   TAVTdigitizer::fgkShel50[] = {    2,     8,    12,    26,    38,    44,    60,    78,    86 };
Float_t TAVTdigitizer::fgkAngD50[] = {180.0,  11.3,  89.0,  18.5,  11.3,  11.3,  11.3,   8.2,   8.2 };
Float_t TAVTdigitizer::fgkFacX50[] = {  0.5,   0.5,   1.4,   1.5,   0.5,   0.0,   0.3,   0.3,   0.3 };
Float_t TAVTdigitizer::fgkFacY50[] = {  0.0,   1.0,   1.0,   2.0,   2.8,   3.3,   3.8,   4.4,   4.8 };

//50
Float_t TAVTdigitizer::fgkAngD05[] = {180.0,  11.3,  91.0,  18.5,  11.3,  11.3,  11.3,   8.2,   8.2 };

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
  fDebugLevel(1)
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
   
   if (fDebugLevel) {
      printf("\nnext hit:\n");
      printf("eloss %6.1f pixels %d\n", deltaE, fPixelsN);
   }
   
   return MakeCluster(x0, y0);
}

// --------------------------------------------------------------------------------------
Bool_t TAVTdigitizer::MakeCluster(Double_t x0, Double_t y0)
{
   fMap.clear();
   
   Int_t line0 = GetLine(y0);
   Int_t col0  = GetColumn(x0);
   
   Float_t remX = GetColRemainder(x0);
   Float_t remY = GetLineRemainder(y0);

   Int_t regX  = GetColRegion(x0);
   Int_t regY = GetLineRegion(y0);
   
   if (fDebugLevel) {

      printf("%d %d %d\n", fPixelsN, line0, col0);

      printf("remX: %.2f\n", remX);
      printf("remY: %.2f\n", remY);
   
      printf("regX: %d\n", regX);
      printf("regY: %d\n", regY);
   }
   
   Float_t pitchX  = GetPitchX();
   Float_t pitchY  = GetPitchY();
   
   switch (regX) {
      case 0:
         x0 += -(GetColRemainder(x0))*pitchX;
         break;
      case 5:
         x0 += (0.5 - GetColRemainder(x0))*pitchX;
         break;
      case 1:
         x0 += (1 - GetColRemainder(x0))*pitchX;
         col0  = GetColumn(x0); // next colummn
         break;
      default:
         break;
   }
   
   switch (regY) {
      case 0:
         y0 +=  (GetLineRemainder(y0))*pitchY;
         break;
      case 5:
         y0 += -(0.5 - GetLineRemainder(y0))*pitchY;
         break;
      case 1:
         y0 += -(1 - GetLineRemainder(y0))*pitchY;
         line0 = GetLine(y0); // next line
         break;
      default:
         break;
   }
   
   Int_t turn = 0;
   
   Int_t idx0  = GetIndex(line0, col0);
   fMap[idx0] = 1;
   
   if (fDebugLevel)
      printf("%d %d %d\n", fPixelsN, line0, col0);

   Int_t*   shel;
   Float_t* angD;
   Float_t* facX;
   Float_t* facY;
   
   if (regX == 5 && regY == 5) {
      shel = fgkShel55;
      angD = fgkAngD55;
      facX = fgkFacX55;
      facY = fgkFacY55;
   } else if ((regX == 0 && regY == 0) || (regX == 1 && regY == 1) || (regX == 1 && regY == 0) || (regX == 0 && regY == 1)) {
      shel = fgkShel00;
      angD = fgkAngD00;
      facX = fgkFacX00;
      facY = fgkFacY00;
   } else if ((regX == 5 && regY == 0) || (regX == 5 && regY == 1)) {
      shel = fgkShel50;
      angD = fgkAngD50;
      facX = fgkFacX50;
      facY = fgkFacY50;
   } else if ((regX == 0 && regY == 5) || (regX == 1 && regY == 5)) {
      shel = fgkShel50;
      angD = fgkAngD05;
      facX = fgkFacY50;
      facY = fgkFacX50;
      
   } else
      return false;
   
   while(fPixelsN != 1) {
      
      if (fDebugLevel)
         printf("turn %d\n", turn);
      
      Float_t angL = TMath::ATan2(facX[turn], facY[turn])*TMath::RadToDeg();
      Float_t angU = angL+360.;
      
      if (fDebugLevel)
         printf("angL: %.1f\n", angL);
      
      for (Float_t ang = angL; ang < angU; ang += angD[turn]) {
         Float_t fcX = facX[turn];
         Float_t fcY = facY[turn];
         Float_t radius1 = TMath::Sqrt(pitchX*pitchX*fcX*fcX + pitchY*pitchY*fcY*fcY);
         Float_t x = radius1*TMath::Cos(ang*TMath::DegToRad()) + x0;
         Float_t y = radius1*TMath::Sin(ang*TMath::DegToRad()) + y0;
         
         Int_t line = GetLine(y);
         Int_t col  = GetColumn(x);
         Int_t idx  = GetIndex(line, col);
         
         if (fMap[idx] != 1) {
            if (fDebugLevel)
               printf("%d %d %d\n", fPixelsN-1, line, col);
            fPixelsN--;
            if (fPixelsN == 1 )
               break;
         }
         fMap[idx] = 1;
      }
      if (turn == fgkMaxTurn)
         break;
      turn++;
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


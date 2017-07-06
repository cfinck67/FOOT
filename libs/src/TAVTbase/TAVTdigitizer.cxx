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
Double_t TAVTdigitizer::fgkCm2Mu   = 10000;
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

}

//------------------------------------------+-----------------------------------
//! fill pixel signal
Bool_t TAVTdigitizer::Process( Double_t edep, Double_t x0, Double_t y0)
{
   x0 *= fgkCm2Mu;
   y0 *= fgkCm2Mu;
   
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
   fMapLast.clear();
   
   if (TMath::Abs(x0) > fPitchX*fPixelsNx/2.) return false;
   if (TMath::Abs(y0) > fPitchY*fPixelsNy/2.) return false;
   
   Int_t line0 = GetLine(y0);
   Int_t col0  = GetColumn(x0);
   
   
   // get region in pixel of the hit
   Int_t regX  = GetColRegion(x0);
   Int_t regY  = GetLineRegion(y0);
   
   if (fDebugLevel) {

      printf("%d %d %d\n", fPixelsN, line0, col0);
   
      printf("regX: %d\n", regX);
      printf("regY: %d\n", regY);
   }
   
   Float_t pitchX  = GetPitchX();
   Float_t pitchY  = GetPitchY();
   
   // reset position in one of the 9 remarkable position
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
   
   
   // choose the cluster version
   if (!SetRegion(regX, regY))
      return false;
   
   
   Int_t turn = 0;
   
   Int_t idx0  = GetIndex(line0, col0);
   fMap[idx0] = 1;
   
   if (fDebugLevel)
      printf("%d %d %d\n", fPixelsN, line0, col0);
   
   
   Int_t rpixels = fPixelsN;
   Int_t lastTurn = GetLastShell(fShel);
   Int_t npixels  = fShel[lastTurn];
   
   rpixels  = npixels - rpixels;

   // complete all shells
   while(npixels != 1) {
      
      if (fDebugLevel)
         printf("turn %d\n", turn);
      
      Float_t angL = TMath::ATan2(fFacX[turn], fFacY[turn])*TMath::RadToDeg();
      Float_t angU = angL+360.;
      
      if (fDebugLevel)
         printf("angL: %.1f\n", angL);
      
      for (Float_t ang = angL; ang < angU; ang += fAngD[turn]) {
         Float_t fcX = fFacX[turn];
         Float_t fcY = fFacY[turn];
         Float_t radius1 = TMath::Sqrt(pitchX*pitchX*fcX*fcX + pitchY*pitchY*fcY*fcY);
         Float_t x = radius1*TMath::Cos(ang*TMath::DegToRad()) + x0;
         Float_t y = radius1*TMath::Sin(ang*TMath::DegToRad()) + y0;
         
         Int_t line = GetLine(y);
         Int_t col  = GetColumn(x);
         Int_t idx  = GetIndex(line, col);
         
         if (fMap[idx] != 1) {
            if (turn == lastTurn)
               fMapLast[idx] = 1;
               
            if (fDebugLevel)
               printf("%d %d %d\n", fPixelsN-1, line, col);
               
            npixels--;
            if (npixels == 1 ) {
               fMap[idx] = 1;
               break;
            }
         }
         fMap[idx] = 1;
      }
      if (turn == fgkMaxTurn)
         break;
      turn++;
   }

   // remove pixels until the pixels number is reached
   RemovePixels(rpixels);
   
   if (fDebugLevel) {
      std::map<int,int>::iterator it;
      
      Int_t r = 0;
      for (it = fMap.begin(); it != fMap.end(); ++it) {
         if (fMap[it->first] == 1) {
            r++;
            Int_t l = it->first / fPixelsNx;
            Int_t c = it->first % fPixelsNx;
            printf("line/col %d  %d %d\n", r, l, c);
         }
      }
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
Bool_t TAVTdigitizer::SetRegion(Int_t regX, Int_t regY)
{
   if (regX == 5 && regY == 5) {
      fShel = fgkShel55;
      fAngD = fgkAngD55;
      fFacX = fgkFacX55;
      fFacY = fgkFacY55;
      
   } else if ((regX == 0 && regY == 0) || (regX == 1 && regY == 1) || (regX == 1 && regY == 0) || (regX == 0 && regY == 1)) {
      fShel = fgkShel00;
      fAngD = fgkAngD00;
      fFacX = fgkFacX00;
      fFacY = fgkFacY00;
      
   } else if ((regX == 5 && regY == 0) || (regX == 5 && regY == 1)) {
      fShel = fgkShel50;
      fAngD = fgkAngD50;
      fFacX = fgkFacX50;
      fFacY = fgkFacY50;
      
   } else if ((regX == 0 && regY == 5) || (regX == 1 && regY == 5)) {
      fShel = fgkShel50;
      fAngD = fgkAngD05;
      fFacX = fgkFacY50;
      fFacY = fgkFacX50;
      
   } else
      return false;
   
   return true;
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

//_____________________________________________________________________________
Int_t TAVTdigitizer::GetLastShell(Int_t* shell) const
{
   Int_t lastTurn = 0;
   
   while (fPixelsN > shell[lastTurn]) {
      lastTurn++;
      if (lastTurn >= fgkMaxTurn) {
         lastTurn = fgkMaxTurn;
         break;
      }
   }

   return lastTurn;
}

//_____________________________________________________________________________
void TAVTdigitizer::RemovePixels(Int_t rpixels)
{
   Int_t size = fMapLast.size();
   
   std::vector<int> rarray;
   
   for (Int_t i = 0; i < size; ++i) {
      if (i > rpixels-1)
         rarray.push_back(0);
      else
         rarray.push_back(1);
   }
   
   std::random_shuffle (rarray.begin(), rarray.end(), digit::myrandom);
   
   std::map<int,int>::iterator it;
   Int_t r = 0;
   for (it = fMapLast.begin(); it != fMapLast.end(); ++it) {
      if (rarray[r] == 1) {
         fMap[it->first] = 0;
      }
      r++;
   }

}



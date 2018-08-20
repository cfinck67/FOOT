
#include "TString.h"
#include "TVector3.h"
#include "TClonesArray.h"

#include "TAVTntuRaw.hxx"
#include "TAVTbaseNtuHit.hxx"

ClassImp(TAVTbaseNtuHit) // Description of Single Detector TAVTbaseNtuHit 


//______________________________________________________________________________
//  build the hit from the index
TAVTbaseNtuHit::TAVTbaseNtuHit( Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue)
: TObject(),
  fSensorId(aSensorNumber),
  fMCid(-1),
  fPixelIndex(aPixelIndex),
  fPixelLine(0),
  fPixelColumn(0),
  fRawValue(aValue),
  fFound(false)
{
    fPulseHeight = fRawValue;
}

//______________________________________________________________________________
// Build the pixel from its sensor, line and column// constructor of a Pixel with column and line 
TAVTbaseNtuHit::TAVTbaseNtuHit( Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn )
: TObject(),
  fSensorId(aSensorNumber),
  fMCid(-1),
  fPixelIndex(0),
  fPixelLine(aLine),
  fPixelColumn(aColumn),
  fRawValue(aValue),
  fFound(false)
{
    fPulseHeight = fRawValue;
}

//______________________________________________________________________________
//
void TAVTbaseNtuHit::Clear(Option_t* /*option*/)
{
}


//______________________________________________________________________________
//  
Double_t TAVTbaseNtuHit::Distance(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.Perp();
}

//______________________________________________________________________________
//  
Double_t TAVTbaseNtuHit::DistanceU(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.x();
}

//______________________________________________________________________________
//  
Double_t TAVTbaseNtuHit::DistanceV(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.y();
}


























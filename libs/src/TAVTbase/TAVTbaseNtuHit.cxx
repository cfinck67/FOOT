
#include "TString.h"
#include "TVector3.h"
#include "TClonesArray.h"

#include "TAVTntuRaw.hxx"
#include "TAVTbaseNtuHit.hxx"

ClassImp(TAVTbaseNtuHit) // Description of Single Detector TAVTbaseNtuHit 


//______________________________________________________________________________
//  build the hit from the index
TAVTbaseNtuHit::TAVTbaseNtuHit( Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue)
: TAGobject(),
  fSensorId(aSensorNumber),
  fPixelIndex(aPixelIndex),
  fPixelLine(0),
  fPixelColumn(0),
  fRawValue(aValue),
  fFound(false),
  fMcTrackCount(0)
{
    fPulseHeight = fRawValue;
}

//______________________________________________________________________________
// Build the pixel from its sensor, line and column// constructor of a Pixel with column and line 
TAVTbaseNtuHit::TAVTbaseNtuHit( Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn )
: TAGobject(),
  fSensorId(aSensorNumber),
  fPixelIndex(0),
  fPixelLine(aLine),
  fPixelColumn(aColumn),
  fRawValue(aValue),
  fFound(false),
  fMcTrackCount(0)
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

//______________________________________________________________________________
//
void TAVTbaseNtuHit::AddMcTrackId(Int_t trackId,Int_t mcId)
{
   fMcTrackId[fMcTrackCount]   = mcId;
   fMcTrackId[fMcTrackCount++] = trackId;
}

//______________________________________________________________________________
//
Bool_t TAVTbaseNtuHit::IsEqual(const TObject* hit) const
{
   return ((fSensorId    == ((TAVTbaseNtuHit*)hit)->fSensorId)    &&
           (fPixelLine   == ((TAVTbaseNtuHit*)hit)->fPixelLine)   &&
           (fPixelColumn == ((TAVTbaseNtuHit*)hit)->fPixelColumn)
           );
}

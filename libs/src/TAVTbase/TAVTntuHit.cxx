

#include "TAVTntuHit.hxx"


ClassImp(TAVTntuHit) // Description of Single Detector TAVTntuHit 


//______________________________________________________________________________
//  build the hit from the index
TAVTntuHit::TAVTntuHit( Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue)
: TAVTbaseNtuHit(aSensorNumber, aPixelIndex, aValue)
{
}

//______________________________________________________________________________
// Build the pixel from its sensor, line and column// constructor of a Pixel with column and line 
TAVTntuHit::TAVTntuHit( Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn )
: TAVTbaseNtuHit(aSensorNumber, aValue, aLine, aColumn)
{
}

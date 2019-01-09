
#include "TString.h"
#include "TClonesArray.h"

#include "TAGgeoTrafo.hxx"
#include "TAVTntuRaw.hxx"
#include "TAITntuHit.hxx"

ClassImp(TAITntuHit) // Description of Single Detector TAITntuHit 


//______________________________________________________________________________
//  build the hit from the index
TAITntuHit::TAITntuHit( Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue)
: TAVTbaseNtuHit(aSensorNumber, aPixelIndex, aValue)
{
}

//______________________________________________________________________________
// Build the pixel from its sensor, line and column// constructor of a Pixel with column and line 
TAITntuHit::TAITntuHit( Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn )
: TAVTbaseNtuHit(aSensorNumber, aValue, aLine, aColumn)
{
}

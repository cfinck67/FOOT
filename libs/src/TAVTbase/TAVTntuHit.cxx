



#include "TString.h"
#include "TClonesArray.h"

#include "TAVTntuRaw.hxx"
#include "TAVTparMap.hxx"



#include "TAVTntuHit.hxx"






ClassImp(TAVTntuHit) // Description of Single Detector TAVTntuHit 

//______________________________________________________________________________
//  
TAVTntuHit::TAVTntuHit(Int_t aSensorNumber, TAVTrawHit* pixel)
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1),
  fFound(kFALSE),
  fDebugLevel(0)
{
   // constructor of a TAVTntuHit from a base pixel
   
   fPixelLine    = pixel->GetLineNumber();
   fPixelColumn  = pixel->GetColumnNumber();
   fPixelIndex   = pixel->GetIndex();

   fRawValue     = pixel->GetValue();
   fPulseHeight  = fRawValue;   

   fPosition.SetXYZ(0, 0, 0);
   fSize.SetXYZ(0, 0, 0);
   
   fMCPos.SetXYZ(0, 0, 0);
   fMCP.SetXYZ(0, 0, 0);
   
   if(fDebugLevel > 1)
	  printf("TAVTntuHit: pixel %d from plane %d with value %f built\n", fPixelIndex, fSensorNumber, fRawValue);
}




//______________________________________________________________________________
//  
TAVTntuHit::TAVTntuHit(Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue)
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1),
  fPixelIndex(aPixelIndex),
  fPixelLine(0),
  fPixelColumn(0),
  fRawValue(aValue),
  fFound(kFALSE),
  fDebugLevel(0)
{
   // constructor of a Pixel 
   fPosition.SetXYZ(0, 0, 0);
   fSize.SetXYZ(0, 0, 0);
   
   fMCPos.SetXYZ(0, 0, 0);
   fMCP.SetXYZ(0, 0, 0);
   
   fPulseHeight    = fRawValue; 
   
   if(fDebugLevel>1)
	  printf("TAVTntuHit: pixel %d from plane %d with value %f built\n", fPixelIndex, fSensorNumber, fRawValue);
   
}




//______________________________________________________________________________
//  
TAVTntuHit::TAVTntuHit(Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn)
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1),
  fPixelIndex(0),
  fPixelLine(aLine),
  fPixelColumn(aColumn),
  fRawValue(aValue),
  fFound(kFALSE),
  fDebugLevel(0)
{
   // constructor of a Pixel with column and line 
   
   fPosition.SetXYZ(0, 0, 0);
   fSize.SetXYZ(0, 0, 0);
   
   fMCPos.SetXYZ(0, 0, 0);
   fMCP.SetXYZ(0, 0, 0);
   
   fPulseHeight    = fRawValue; 
   
   if(fDebugLevel > 1) 
	  printf("TAVTntuHit: pixel %d from plane %d with value %f built\n", fPixelIndex, fSensorNumber, fRawValue);
}




//______________________________________________________________________________
//  
Double_t TAVTntuHit::Distance(TAVTntuHit &aPixel)
{
   return Distance(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::Distance(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.Perp();
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceU(TAVTntuHit &aPixel)
{
   return DistanceU(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceU(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result(0);
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceV(TAVTntuHit &aPixel)
{
   return DistanceV(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceV(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result(1);
}




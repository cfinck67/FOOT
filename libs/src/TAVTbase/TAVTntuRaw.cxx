/*!
  \file
  \version $Id: TAVTntuRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TAVTntuRaw.
*/

#include "TString.h"
#include "TClonesArray.h"

#include "TAVTntuRaw.hxx"
#include "TAVTparMap.hxx"

/*!
  \class TAVTntuRaw TAVTntuRaw.hxx "TAVTntuRaw.hxx"
  \brief Container class for VTX ntu hit **
*/

////////////////////////////////////////////////////////////
// Class Description of TAVTntuHit                        //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////


#include "TAVTntuRaw.hxx"
#include "TAVTdatRaw.hxx"

ClassImp(TAVTntuHit) // Description of Single Detector TAVTntuHit 
//______________________________________________________________________________
//  
TAVTntuHit::TAVTntuHit()
{ 
   // TAVTntuHit default constructor
}

//______________________________________________________________________________
//  

TAVTntuHit::~TAVTntuHit()
{
   // TAVTntuHit default destructor
   
}
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

//##############################################################################

ClassImp(TAVTntuRaw);

TString TAVTntuRaw::fgkBranchName   = "vtrh.";

//------------------------------------------+-----------------------------------
//! Default constructor.
TAVTntuRaw::TAVTntuRaw() 
: TAGdata(),
  fListOfPixels(0x0)
{
   SetupClones();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTntuRaw::~TAVTntuRaw() 
{
   delete fListOfPixels;
}

//------------------------------------------+-----------------------------------
//! return number of pixels for a given sensor.
Int_t TAVTntuRaw::GetPixelsN(Int_t iSensor) const
{
   if (iSensor >= 0  || iSensor < TAVTparMap::GetSensorsN()) {
	  TClonesArray*list = GetListOfPixels(iSensor);
	  return list->GetEntries();
   } else  {
	  Error("GetPixelsN()", "Wrong sensor number %d\n", iSensor);
	  return -1;
   }
}

//------------------------------------------+-----------------------------------
TClonesArray* TAVTntuRaw::GetListOfPixels(Int_t iSensor)
{
   if (iSensor >= 0  || iSensor < TAVTparMap::GetSensorsN()) {
	  TClonesArray* list = (TClonesArray*)fListOfPixels->At(iSensor);
	  return list;
   } else {
	  Error("GetListOfPixels()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//------------------------------------------+-----------------------------------
TClonesArray* TAVTntuRaw::GetListOfPixels(Int_t iSensor) const
{
   if (iSensor >= 0  || iSensor < TAVTparMap::GetSensorsN()) {
	  TClonesArray* list = (TClonesArray*)fListOfPixels->At(iSensor);
	  return list;
   } else {
	  Error("GetListOfPixels()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
TAVTntuHit* TAVTntuRaw::GetPixel(Int_t iSensor, Int_t iPixel)
{
   if (iPixel >=0 || iPixel < GetPixelsN(iSensor)) {
	  TClonesArray* list = GetListOfPixels(iSensor);
	  return (TAVTntuHit*)list->At(iPixel);
   } else {
	  Error("GetPixel()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
const TAVTntuHit* TAVTntuRaw::GetPixel(Int_t iSensor, Int_t iPixel) const
{
   if (iPixel >=0 || iPixel < GetPixelsN(iSensor)) {
	  TClonesArray* list = GetListOfPixels(iSensor);
	  return (TAVTntuHit*)list->At(iPixel);
   } else {
	  Error("GetPixel()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//------------------------------------------+-----------------------------------
//! Setup clones.
void TAVTntuRaw::SetupClones()
{
   if (fListOfPixels) return;
   fListOfPixels = new TObjArray();
   
   for (Int_t i = 0; i < TAVTparMap::GetSensorsN(); ++i) {
	  TClonesArray* arr = new TClonesArray("TAVTntuHit", 500);
	  arr->SetOwner(true);
	  fListOfPixels->AddAt(arr, i);
   }
   fListOfPixels->SetOwner(true);
}

//------------------------------------------+-----------------------------------
//! Clear event.
void TAVTntuRaw::Clear(Option_t*)
{
   for (Int_t i = 0; i < TAVTparMap::GetSensorsN(); ++i) {
	  TClonesArray* list = GetListOfPixels(i);
	  list->Delete();
   }   
}

//______________________________________________________________________________
//  
TAVTntuHit* TAVTntuRaw::NewPixel(Int_t iSensor, Double_t value, Int_t aLine, Int_t aColumn)
{
   if (iSensor >= 0  || iSensor < TAVTparMap::GetSensorsN()) {	  
	  TClonesArray &pixelArray = *GetListOfPixels(iSensor);
	  TAVTntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAVTntuHit(iSensor, value, aLine, aColumn);
	  return pixel;
   } else {
	  Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }
}

//______________________________________________________________________________
//  
TAVTntuHit* TAVTntuRaw::NewPixel(Int_t iSensor, TAVTrawHit* rawPixel)
{
   if (iSensor >= 0  || iSensor < TAVTparMap::GetSensorsN()) {	  
	  TClonesArray &pixelArray = *GetListOfPixels(iSensor);
	  TAVTntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAVTntuHit(iSensor, rawPixel);
	  return pixel;
   } else {
	  Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAVTntuRaw::ToStream(ostream& os, Option_t* option) const
{
   for (Int_t i = 0; i < TAVTparMap::GetSensorsN(); ++i) {
	  
	  os << "TAVTntuRaw " << GetName()
	  << Form("  nPixels=%3d", GetPixelsN(i))
	  << endl;
	  
	  //TODO properly
	  //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
	  for (Int_t j = 0; j < GetPixelsN(i); j++) {
		 const TAVTntuHit*  pixel = GetPixel(i,j);
		 if (pixel)
			os << Form("%4d", pixel->GetPixelIndex());
		 os << endl;
	  }
   }
}

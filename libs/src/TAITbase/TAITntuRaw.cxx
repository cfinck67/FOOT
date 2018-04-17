/*!
  \file
  \version $Id: TAITntuRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TAITntuRaw.
*/

#include "TString.h"
#include "TClonesArray.h"

#include "TAITntuRaw.hxx"
#include "TAITparMap.hxx"

/*!
  \class TAITntuRaw TAITntuRaw.hxx "TAITntuRaw.hxx"
  \brief Container class for VTX ntu hit **
*/

////////////////////////////////////////////////////////////
// Class Description of TAITntuHit                        //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////


#include "TAITdatRaw.hxx"

ClassImp(TAITntuHit) // Description of Single Detector TAITntuHit 
//______________________________________________________________________________
//  
TAITntuHit::TAITntuHit()
{ 
   // TAITntuHit default constructor
}

//______________________________________________________________________________
//  

TAITntuHit::~TAITntuHit()
{
   // TAITntuHit default destructor
   
}
//______________________________________________________________________________
//  
TAITntuHit::TAITntuHit(Int_t aSensorNumber, TAITrawHit* pixel)
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1),
  fFound(kFALSE),
  fDebugLevel(0)
{
   // constructor of a TAITntuHit from a base pixel
   
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
	  printf("TAITntuHit: pixel %d from plane %d with value %f built\n", fPixelIndex, fSensorNumber, fRawValue);
}

//______________________________________________________________________________
//  
TAITntuHit::TAITntuHit(Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue)
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
	  printf("TAITntuHit: pixel %d from plane %d with value %f built\n", fPixelIndex, fSensorNumber, fRawValue);
   
}

//______________________________________________________________________________
//  
TAITntuHit::TAITntuHit(Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn)
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
	  printf("TAITntuHit: pixel %d from plane %d with value %f built\n", fPixelIndex, fSensorNumber, fRawValue);
}

//______________________________________________________________________________
//  
Double_t TAITntuHit::Distance(TAITntuHit &aPixel)
{
   return Distance(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t TAITntuHit::Distance(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.Perp();
}

//______________________________________________________________________________
//  
Double_t TAITntuHit::DistanceU(TAITntuHit &aPixel)
{
   return DistanceU(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t TAITntuHit::DistanceU(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result(0);
}

//______________________________________________________________________________
//  
Double_t TAITntuHit::DistanceV(TAITntuHit &aPixel)
{
   return DistanceV(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t TAITntuHit::DistanceV(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result(1);
}

//##############################################################################

ClassImp(TAITntuRaw);

TString TAITntuRaw::fgkBranchName   = "vtrh.";

//------------------------------------------+-----------------------------------
//! Default constructor.
TAITntuRaw::TAITntuRaw() 
: TAGdata(),
  fListOfPixels(0x0)
{
   SetupClones();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITntuRaw::~TAITntuRaw() 
{
   delete fListOfPixels;
}

//------------------------------------------+-----------------------------------
//! return number of pixels for a given sensor.
Int_t TAITntuRaw::GetPixelsN(Int_t iSensor) const
{
   if (iSensor >= 0  || iSensor < TAITparMap::GetSensorsN()) {
	  TClonesArray*list = GetListOfPixels(iSensor);
	  return list->GetEntries();
   } else  {
	  Error("GetPixelsN()", "Wrong sensor number %d\n", iSensor);
	  return -1;
   }
}

//------------------------------------------+-----------------------------------
TClonesArray* TAITntuRaw::GetListOfPixels(Int_t iSensor)
{
   if (iSensor >= 0  || iSensor < TAITparMap::GetSensorsN()) {
	  TClonesArray* list = (TClonesArray*)fListOfPixels->At(iSensor);
	  return list;
   } else {
	  Error("GetListOfPixels()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//------------------------------------------+-----------------------------------
TClonesArray* TAITntuRaw::GetListOfPixels(Int_t iSensor) const
{
   if (iSensor >= 0  || iSensor < TAITparMap::GetSensorsN()) {
	  TClonesArray* list = (TClonesArray*)fListOfPixels->At(iSensor);
	  return list;
   } else {
	  Error("GetListOfPixels()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
TAITntuHit* TAITntuRaw::GetPixel(Int_t iSensor, Int_t iPixel)
{
   if (iPixel >=0 || iPixel < GetPixelsN(iSensor)) {
	  TClonesArray* list = GetListOfPixels(iSensor);
	  return (TAITntuHit*)list->At(iPixel);
   } else {
	  Error("GetPixel()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
const TAITntuHit* TAITntuRaw::GetPixel(Int_t iSensor, Int_t iPixel) const
{
   if (iPixel >=0 || iPixel < GetPixelsN(iSensor)) {
	  TClonesArray* list = GetListOfPixels(iSensor);
	  return (TAITntuHit*)list->At(iPixel);
   } else {
	  Error("GetPixel()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//------------------------------------------+-----------------------------------
//! Setup clones.
void TAITntuRaw::SetupClones()
{
   if (fListOfPixels) return;
   fListOfPixels = new TObjArray( TAITparMap::GetSensorsN() );
   
   for (Int_t i = 0; i < TAITparMap::GetSensorsN(); ++i) {
	  TClonesArray* arr = new TClonesArray("TAITntuHit", 500);
	  arr->SetOwner(true);
	  fListOfPixels->AddAt(arr, i);
   }
   fListOfPixels->SetOwner(true);
}

//------------------------------------------+-----------------------------------
//! Clear event.
void TAITntuRaw::Clear(Option_t*)
{
   for (Int_t i = 0; i < TAITparMap::GetSensorsN(); ++i) {
	  TClonesArray* list = GetListOfPixels(i);
	  list->Delete();
   }   
}

//______________________________________________________________________________
//  
TAITntuHit* TAITntuRaw::NewPixel(Int_t iSensor, Double_t value, Int_t aLine, Int_t aColumn)
{
   if (iSensor >= 0  || iSensor < TAITparMap::GetSensorsN()) {	  
	  TClonesArray &pixelArray = *GetListOfPixels(iSensor);
	  TAITntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAITntuHit(iSensor, value, aLine, aColumn);
	  return pixel;
   } else {
	  Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }
}

//______________________________________________________________________________
//  
TAITntuHit* TAITntuRaw::NewPixel(Int_t iSensor, TAITrawHit* rawPixel)
{
   if (iSensor >= 0  || iSensor < TAITparMap::GetSensorsN()) {	  
	  TClonesArray &pixelArray = *GetListOfPixels(iSensor);
	  TAITntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAITntuHit(iSensor, rawPixel);
	  return pixel;
   } else {
	  Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAITntuRaw::ToStream(ostream& os, Option_t* option) const
{
   for (Int_t i = 0; i < TAITparMap::GetSensorsN(); ++i) {
	  
	  os << "TAITntuRaw " << GetName()
	  << Form("  nPixels=%3d", GetPixelsN(i))
	  << endl;
	  
	  //TODO properly
	  //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
	  for (Int_t j = 0; j < GetPixelsN(i); j++) {
		 const TAITntuHit*  pixel = GetPixel(i,j);
		 if (pixel)
			os << Form("%4d", pixel->GetPixelIndex());
		 os << endl;
	  }
   }
}

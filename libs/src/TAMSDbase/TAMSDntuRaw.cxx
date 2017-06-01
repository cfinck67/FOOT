/*!
  \file
  \version $Id: TAMSDntuRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TAMSDntuRaw.
*/

#include "TString.h"
#include "TClonesArray.h"

#include "TAMSDntuRaw.hxx"
#include "TAMSDparMap.hxx"

/*!
  \class TAMSDntuRaw TAMSDntuRaw.hxx "TAMSDntuRaw.hxx"
  \brief Container class for VTX ntu hit **
*/

////////////////////////////////////////////////////////////
// Class Description of TAMSDntuHit                        //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////


#include "TAMSDntuRaw.hxx"
#include "TAMSDdatRaw.hxx"

ClassImp(TAMSDntuHit) // Description of Single Detector TAMSDntuHit 
//______________________________________________________________________________
//  
TAMSDntuHit::TAMSDntuHit()
{ 
   // TAMSDntuHit default constructor
}

//______________________________________________________________________________
//  

TAMSDntuHit::~TAMSDntuHit()
{
   // TAMSDntuHit default destructor
   
}
//______________________________________________________________________________
//  
TAMSDntuHit::TAMSDntuHit(Int_t aSensorNumber, TAMSDrawHit* pixel)
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1),
  fFound(kFALSE),
  fDebugLevel(0)
{
   // constructor of a TAMSDntuHit from a base pixel
   
   fPixelView    = pixel->GetView();
   fPixelStrip   = pixel->GetStrip();
   fPixelIndex   = pixel->GetIndex();

   fRawValue     = pixel->GetValue();
   fPulseHeight  = fRawValue;   

   fPosition.SetXYZ(0, 0, 0);
   fSize.SetXYZ(0, 0, 0);
   
   fMCPos.SetXYZ(0, 0, 0);
   fMCP.SetXYZ(0, 0, 0);
   
   if(fDebugLevel > 1)
	  printf("TAMSDntuHit: pixel %d from plane %d with value %f built\n", fPixelIndex, fSensorNumber, fRawValue);
}

//______________________________________________________________________________
//  
TAMSDntuHit::TAMSDntuHit(Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue)
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1),
  fPixelIndex(aPixelIndex),
  fPixelView(0),
  fPixelStrip(0),
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
	  printf("TAMSDntuHit: pixel %d from plane %d with value %f built\n", fPixelIndex, fSensorNumber, fRawValue);
   
}

//______________________________________________________________________________
//  
TAMSDntuHit::TAMSDntuHit(Int_t aSensorNumber, Double_t aValue, Int_t aView, Int_t aStrip)
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1),
  fPixelIndex(0),
  fPixelView(aView),
  fPixelStrip(aStrip),
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
	  printf("TAMSDntuHit: pixel %d from plane %d with value %f built\n", fPixelIndex, fSensorNumber, fRawValue);
}

//______________________________________________________________________________
//  
Double_t TAMSDntuHit::Distance(TAMSDntuHit &aPixel)
{
   return Distance(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t TAMSDntuHit::Distance(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.Perp();
}

//______________________________________________________________________________
//  
Double_t TAMSDntuHit::DistanceU(TAMSDntuHit &aPixel)
{
   return DistanceU(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t TAMSDntuHit::DistanceU(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result(0);
}

//______________________________________________________________________________
//  
Double_t TAMSDntuHit::DistanceV(TAMSDntuHit &aPixel)
{
   return DistanceV(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t TAMSDntuHit::DistanceV(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result(1);
}

//##############################################################################

ClassImp(TAMSDntuRaw);

TString TAMSDntuRaw::fgkBranchName   = "vtrh.";

//------------------------------------------+-----------------------------------
//! Default constructor.
TAMSDntuRaw::TAMSDntuRaw() 
: TAGdata(),
  fListOfPixels(0x0)
{
   SetupClones();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAMSDntuRaw::~TAMSDntuRaw() 
{
   delete fListOfPixels;
}

//------------------------------------------+-----------------------------------
//! return number of pixels for a given sensor.
Int_t TAMSDntuRaw::GetPixelsN(Int_t iSensor) const
{
   if (iSensor >= 0  || iSensor < TAMSDparMap::GetSensorsN()) {
	  TClonesArray*list = GetListOfPixels(iSensor);
	  return list->GetEntries();
   } else  {
	  Error("GetPixelsN()", "Wrong sensor number %d\n", iSensor);
	  return -1;
   }
}

//------------------------------------------+-----------------------------------
TClonesArray* TAMSDntuRaw::GetListOfPixels(Int_t iSensor)
{
   if (iSensor >= 0  || iSensor < TAMSDparMap::GetSensorsN()) {
	  TClonesArray* list = (TClonesArray*)fListOfPixels->At(iSensor);
	  return list;
   } else {
	  Error("GetListOfPixels()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//------------------------------------------+-----------------------------------
TClonesArray* TAMSDntuRaw::GetListOfPixels(Int_t iSensor) const
{
   if (iSensor >= 0  || iSensor < TAMSDparMap::GetSensorsN()) {
	  TClonesArray* list = (TClonesArray*)fListOfPixels->At(iSensor);
	  return list;
   } else {
	  Error("GetListOfPixels()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
TAMSDntuHit* TAMSDntuRaw::GetPixel(Int_t iSensor, Int_t iPixel)
{
   if (iPixel >=0 || iPixel < GetPixelsN(iSensor)) {
	  TClonesArray* list = GetListOfPixels(iSensor);
	  return (TAMSDntuHit*)list->At(iPixel);
   } else {
	  Error("GetPixel()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
const TAMSDntuHit* TAMSDntuRaw::GetPixel(Int_t iSensor, Int_t iPixel) const
{
   if (iPixel >=0 || iPixel < GetPixelsN(iSensor)) {
	  TClonesArray* list = GetListOfPixels(iSensor);
	  return (TAMSDntuHit*)list->At(iPixel);
   } else {
	  Error("GetPixel()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//------------------------------------------+-----------------------------------
//! Setup clones.
void TAMSDntuRaw::SetupClones()
{
   if (fListOfPixels) return;
   fListOfPixels = new TObjArray();
   
   for (Int_t i = 0; i < TAMSDparMap::GetSensorsN(); ++i) {
	  TClonesArray* arr = new TClonesArray("TAMSDntuHit", 500);
	  arr->SetOwner(true);
	  fListOfPixels->AddAt(arr, i);
   }
   fListOfPixels->SetOwner(true);
}

//------------------------------------------+-----------------------------------
//! Clear event.
void TAMSDntuRaw::Clear(Option_t*)
{
   for (Int_t i = 0; i < TAMSDparMap::GetSensorsN(); ++i) {
	  TClonesArray* list = GetListOfPixels(i);
	  list->Delete();   
   list->Clear();
 }
}

//______________________________________________________________________________
//  
TAMSDntuHit* TAMSDntuRaw::NewPixel(Int_t iSensor, Double_t value, Int_t aLine, Int_t aColumn)
{
   if (iSensor >= 0  || iSensor < TAMSDparMap::GetSensorsN()) {	  
	  TClonesArray &pixelArray = *GetListOfPixels(iSensor);
	  TAMSDntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAMSDntuHit(iSensor, value, aLine, aColumn);
	  return pixel;
   } else {
	  Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }
}

//______________________________________________________________________________
//  
TAMSDntuHit* TAMSDntuRaw::NewPixel(Int_t iSensor, TAMSDrawHit* rawPixel)
{
   if (iSensor >= 0  || iSensor < TAMSDparMap::GetSensorsN()) {	  
	  TClonesArray &pixelArray = *GetListOfPixels(iSensor);
	  TAMSDntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAMSDntuHit(iSensor, rawPixel);
	  return pixel;
   } else {
	  Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAMSDntuRaw::ToStream(ostream& os, Option_t* option) const
{
   for (Int_t i = 0; i < TAMSDparMap::GetSensorsN(); ++i) {
	  
	  os << "TAMSDntuRaw " << GetName()
	  << Form("  nPixels=%3d", GetPixelsN(i))
	  << endl;
	  
	  //TODO properly
	  //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
	  for (Int_t j = 0; j < GetPixelsN(i); j++) {
		 const TAMSDntuHit*  pixel = GetPixel(i,j);
		 if (pixel)
			os << Form("%4d", pixel->GetPixelIndex());
		 os << endl;
	  }
   }
}

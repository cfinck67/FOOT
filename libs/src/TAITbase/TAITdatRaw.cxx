////////////////////////////////////////////////////////////
// Class Description of TAITrawHit                        //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////
#include "TClonesArray.h"

#include "TAITdatRaw.hxx"

ClassImp(TAITrawHit) 

//______________________________________________________________________________
//
TAITrawHit::TAITrawHit()
: TObject(),
   fInput(0), 
   fValue(0),
   fIndex(0),
   fLineNumber(0),
   fColumnNumber(0)
{
   
}

//______________________________________________________________________________
//
TAITrawHit::TAITrawHit( Int_t input, Int_t value, Int_t lineNumber, Int_t columnNumber)
: TObject(),
   fInput(input), 
   fValue(value),
   fIndex(0),
   fLineNumber(lineNumber),
   fColumnNumber(columnNumber)
{

}

//_______________________________________
//!Compare return 1 if grater 0 if equal -1 if lower
Int_t TAITrawHit::Compare(const TObject* obj) const
{
   Int_t aLine = fLineNumber;
   Int_t nLine = ((TAITrawHit *)obj)->GetLineNumber();
   Int_t aCol = fColumnNumber;
   Int_t nCol = ((TAITrawHit *)obj)->GetColumnNumber();
   
   if (aLine == nLine) { //Order ok then order for column
      if(aCol<nCol)
		 return -1;
      else
		 return 1;
   }else if(aLine > nLine)
      return 1;
   else
      return -1;
   
}

//####################################################################################

  ////////////////////////////////////////////////////////////
  // Class Description of TAITdatRaw                        //
  //                                                        //
  //                                                        //
  ////////////////////////////////////////////////////////////

#include "TAITparMap.hxx"

ClassImp(TAITdatRaw) 

TString TAITdatRaw::fgkBranchName   = "vtrd.";

//______________________________________________________________________________
//
TAITdatRaw::TAITdatRaw() 
: TAGdata(),
  fListOfPixels(0x0)
{
   SetupClones();
}

//______________________________________________________________________________
//  
TAITdatRaw::~TAITdatRaw()
{
  // TAITdatRaw default destructor
   delete fListOfPixels; 
}

//------------------------------------------+-----------------------------------
//! return number of pixels for a given sensor.
Int_t TAITdatRaw::GetPixelsN(Int_t iSensor) const
{
   if (iSensor >= 0  || iSensor < TAITparMap::GetSensorsN())
	  return GetPixels(iSensor)->GetEntries();
   else {
	  Error("GetPixelsN()","Wrong sensor number %d\n", iSensor);
	  return -1;
   }
}

//______________________________________________________________________________
//
TClonesArray* TAITdatRaw::GetPixels(Int_t iSensor) const      
{ 
   if (iSensor >= 0  || iSensor < TAITparMap::GetSensorsN()) 	  
	  return (TClonesArray*)fListOfPixels->At(iSensor);
   else {
	  Error("GetPixels()","Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
   
}
   
//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
const TAITrawHit* TAITdatRaw::GetPixel(Int_t iSensor, Int_t iPixel) const
{
   if (iPixel >=0 || iPixel < GetPixelsN(iSensor))
	  return (TAITrawHit*)GetPixels(iSensor)->At(iPixel);
   else {
	  Error("GetPixels()","Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }
}
 
//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
TAITrawHit* TAITdatRaw::GetPixel(Int_t iSensor, Int_t iPixel)
{
   if (iPixel >=0 || iPixel < GetPixelsN(iSensor))
	  return (TAITrawHit*)GetPixels(iSensor)->At(iPixel);
   else {
	  Error("GetPixel()","Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }
}
   
//______________________________________________________________________________
//  
void TAITdatRaw::AddPixel(Int_t sensor, Int_t value, Int_t aLine, Int_t aColumn)
{
   if (sensor >= 0  || sensor < TAITparMap::GetSensorsN()) {	  
	  TClonesArray &pixelArray = *GetPixels(sensor);
	  new(pixelArray[pixelArray.GetEntriesFast()]) TAITrawHit(sensor, value, aLine, aColumn);
   } else {
	  Error("AddPixel()","Wrong sensor number %d\n", sensor);
   }
}

//______________________________________________________________________________
//  
void TAITdatRaw::SetupClones()
{
   if (fListOfPixels) return;
   fListOfPixels = new TObjArray();
   for (Int_t i = 0; i < TAITparMap::GetSensorsN(); ++i) {
	  TClonesArray* arr = new TClonesArray("TAITrawHit");
	  arr->SetOwner(true); 
	  fListOfPixels->AddAt(arr,i);
   }
   fListOfPixels->SetOwner(true);    
}

//______________________________________________________________________________
//  
void TAITdatRaw::Clear(Option_t* /*opt*/)
{
   TAGdata::Clear();
   for (Int_t i = 0; i < TAITparMap::GetSensorsN(); ++i) 
	  GetPixels(i)->Delete();
}

//______________________________________________________________________________
//  
void TAITdatRaw::ToStream(ostream& os, Option_t* option) const
{
   for (Int_t i = 0; i < TAITparMap::GetSensorsN(); ++i) {
	  os << "TAITdatRaw " << GetName()
	     << Form("  nhit=%3d", GetPixelsN(i))
	     << endl;
	  
	  for (Int_t k = 0; k < GetPixelsN(i); k++) {
		 const TAITrawHit* hit = GetPixel(i,k);
		 os << Form("%4d\n", hit->GetInput());
		 os << Form("%4d\n", hit->GetValue());
		 os << Form("%4d\n", hit->GetLineNumber());
		 os << Form("%4d\n", hit->GetColumnNumber());
		 os << Form("%4d\n", hit->GetIndex());
		 os << endl;
	  }
   }
}

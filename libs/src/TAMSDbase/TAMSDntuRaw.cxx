/*!
 \file
 \version $Id: TAMSDntuRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
 \brief   Implementation of TAMSDntuRaw.
 */

#include "TString.h"
#include "TClonesArray.h"

#include "TAMSDntuRaw.hxx"

/*!
 \class TAMSDntuRaw TAMSDntuRaw.hxx "TAMSDntuRaw.hxx"
 \brief Container class for VTX ntu hit **
 */

////////////////////////////////////////////////////////////
// Class Description of TAMSDntuHit                        //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////


#include "TAGroot.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTdatRaw.hxx"
#include "TAVTdatRaw.hxx"


//##############################################################################

ClassImp(TAMSDntuRaw);

TString TAMSDntuRaw::fgkBranchName   = "msdrh.";

//------------------------------------------+-----------------------------------
//! Default constructor.
TAMSDntuRaw::TAMSDntuRaw()
: TAGdata(),
  fListOfPixels(0x0),
  fpGeoMap(0x0)
{
   fpGeoMap = (TAMSDparGeo*) gTAGroot->FindParaDsc(TAMSDparGeo::GetDefParaName(), "TAMSDparGeo")->Object();
   if (!fpGeoMap) {
      Error("TAMSDntuRaw()", "Para desciptor vtGeo does not exist");
      exit(0);
   }

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
   if (iSensor >= 0  || iSensor < fpGeoMap->GetNSensors()) {
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
   if (iSensor >= 0  || iSensor < fpGeoMap->GetNSensors()) {
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
   if (iSensor >= 0  || iSensor < fpGeoMap->GetNSensors()) {
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
   
   for (Int_t i = 0; i < fpGeoMap->GetNSensors(); ++i) {
      TClonesArray* arr = new TClonesArray("TAMSDntuHit", 500);
      arr->SetOwner(true);
      fListOfPixels->AddAt(arr, i);
   }
   fListOfPixels->SetOwner(true);
   fMap.clear();
}

//------------------------------------------+-----------------------------------
//! Clear event.
void TAMSDntuRaw::Clear(Option_t*)
{
   for (Int_t i = 0; i < fpGeoMap->GetNSensors(); ++i) {
      TClonesArray* list = GetListOfPixels(i);
      list->Clear("C");
   }
   fMap.clear();
}

//______________________________________________________________________________
//
TAMSDntuHit* TAMSDntuRaw::NewPixel(Int_t iSensor, Double_t value, Int_t aView, Int_t aStrip)
{
   if (iSensor >= 0  || iSensor < fpGeoMap->GetNSensors()) {
      TClonesArray &pixelArray = *GetListOfPixels(iSensor);
      std::pair<int, int> idx(aView, aStrip);
      
      // check if pixel already exists
      if ( fMap[idx] == iSensor+1) {
         TAMSDntuHit* pixel = new TAMSDntuHit(iSensor, value, aView, aStrip);
         TAMSDntuHit* curPixel = (TAMSDntuHit*)pixelArray.FindObject(pixel);
         delete pixel;
         return curPixel;
         
      } else {
         fMap[idx] = iSensor+1;
         TAMSDntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAMSDntuHit(iSensor, value, aView, aStrip);
         return pixel;
      }
   } else {
      Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
      return 0x0;
   }
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAMSDntuRaw::ToStream(ostream& os, Option_t* option) const
{
   for (Int_t i = 0; i < fpGeoMap->GetNSensors(); ++i) {
      
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


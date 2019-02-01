/*!
  \file
  \version $Id: TABMntuRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TABMntuRaw.
*/

#include "TABMntuRaw.hxx"

using namespace std;

/*!
  \class TABMntuRaw TABMntuRaw.hxx "TABMntuRaw.hxx"
  \brief Mapping and Geometry parameters for Tof wall. **
*/


//##############################################################################

ClassImp(TABMntuRaw);

TString TABMntuRaw::fgkBranchName   = "bmrh.";

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMntuRaw::TABMntuRaw() :
  fListOfHits(0x0) {}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMntuRaw::~TABMntuRaw() {
  delete fListOfHits;
}

//------------------------------------------+-----------------------------------
// ! Get number of hits
Int_t TABMntuRaw::GetHitsN() const
{
   return fListOfHits->GetEntries();
}


//______________________________________________________________________________
//! new hit

TABMntuHit* TABMntuRaw::NewHit(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t r, Double_t t, Double_t s)
{
   TClonesArray &pixelArray = *fListOfHits;
   
   TABMntuHit* hit = new(pixelArray[pixelArray.GetEntriesFast()]) TABMntuHit(id, iv, il, ic, r, t,  s);
   
   return hit;
   
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TABMntuRaw::SetupClones()
{
  if (!fListOfHits) fListOfHits = new TClonesArray("TABMntuHit");
  return;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TABMntuRaw::Clear(Option_t*)
{
  if (fListOfHits)
    fListOfHits->Clear("C");
   
  return;
}

/*------------------------------------------+---------------------------------*/
//! helper to format one converter value

static void print_value(ostream& os, Int_t i_val)
{
  char c_ran = (i_val & 0x1000) ? 'h' : 'l';
  os << Form("%4d %s %d", i_val & 0x0fff, "d", 4) << " " << c_ran;
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TABMntuRaw::ToStream(ostream& os, Option_t* option) const
{
  os << "TABMntuRaw " << GetName()
     << Form("  nhit=%3d", fListOfHits->GetEntries())
     << endl;
  
  os << "slat stat    adct    adcb    tdct    tdcb" << endl;
  for (Int_t i = 0; i < fListOfHits->GetEntries(); i++) {
    const TABMntuHit*  hit = Hit(i);
    os << Form("%4d", hit->Cell());
    os << "  "; print_value(os, hit->Plane());
    os << endl;
  }
  return;
}

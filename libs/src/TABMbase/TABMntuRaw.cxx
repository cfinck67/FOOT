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

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMntuRaw::TABMntuRaw() :
  nhit(0), h(0) {}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMntuRaw::~TABMntuRaw() {
  delete h;
}


//------------------------------------------+-----------------------------------
//! Setup clones.

void TABMntuRaw::SetupClones()
{
  if (!h) h = new TClonesArray("TABMntuHit");
  return;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TABMntuRaw::Clear(Option_t*)
{
  nhit = 0;
  if (h) 
    h->Clear();
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
     << Form("  nhit=%3d", nhit)
     << endl;
  
  os << "slat stat    adct    adcb    tdct    tdcb" << endl;
  for (Int_t i = 0; i < nhit; i++) {
    const TABMntuHit*  hit = Hit(i);
    os << Form("%4d", hit->Cell());
    os << "  "; print_value(os, hit->Plane());
    os << endl;
  }
  return;
}

/*!
  \file
  \version $Id: TABMntuRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TABMntuRaw.
*/

#include <string.h>

#include <fstream>
#include <bitset>
using namespace std;
#include <algorithm>

#include "TString.h"

#include "TABMntuRaw.hxx"

/*!
  \class TABMntuRaw TABMntuRaw.hxx "TABMntuRaw.hxx"
  \brief Mapping and Geometry parameters for Tof wall. **
*/

ClassImp(TABMntuHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TABMntuHit::~TABMntuHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMntuHit::TABMntuHit()
  : idmon(0),    iview(0),    ilayer(0),    icell(0), itrkass(0), ichi2(0),
    xcamon(0.),    ycamon(0.),    zcamon(0.),
    pxcamon(0.),   pycamon(0.),   pzcamon(0.),
    rdrift(0.),    tdrift(0.),    timmon(0.)
{
}

TABMntuHit::TABMntuHit(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t tm) {

  idmon = id;  iview = iv;  ilayer = il;      icell = ic;  ichi2 = 0;
  xcamon = x;   ycamon = y;  zcamon = z;
  pxcamon = px;   pycamon = py;   pzcamon = pz;
  rdrift = r;   tdrift = t;    timmon = tm;
  
}

void TABMntuHit::SetData(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t tm) {
  
  idmon = id;  iview = iv;  ilayer = il;      icell = ic;  ichi2 = 0;
  xcamon = x;   ycamon = y;  zcamon = z;
  pxcamon = px;   pycamon = py;   pzcamon = pz;
  rdrift = r;   tdrift = t;    timmon = tm;

  return;
}

void TABMntuHit::SetAW(TABMparGeo* f_bmgeo) {

  int idfilo = f_bmgeo->GetID(Cell());
  /*  vista U */
  Int_t tmp_fview = 0;
  if(View()<0) tmp_fview = 1;

  if(idfilo <0 || idfilo>2) idfilo = 2;

  A0.SetXYZ(f_bmgeo->GetX(idfilo,Plane(),tmp_fview),
	    f_bmgeo->GetY(idfilo,Plane(),tmp_fview),
	    f_bmgeo->GetZ(idfilo,Plane(),tmp_fview));
  Wvers.SetXYZ(f_bmgeo->GetCX(idfilo,Plane(),tmp_fview),
	       f_bmgeo->GetCY(idfilo,Plane(),tmp_fview),
	       f_bmgeo->GetCZ(idfilo,Plane(),tmp_fview));
  
  Double_t modulo = Wvers.Mag();
  if(modulo!=0.) {
    Wvers = (1./modulo)*Wvers;
  }
  else{
    cout<<"Track::AddWire-> ERROR Wvers modulo: "<<modulo<<endl;
    return;
  }
  
  return;
}

//##############################################################################

ClassImp(TABMntuRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMntuRaw::TABMntuRaw() :
  nhit(0), h(0) {
}

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
  if (h) h->Clear();
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

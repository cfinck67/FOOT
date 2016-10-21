/*!
  \file
  \version $Id: adapted from TABMntuRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TADCntuRaw.
*/

#include <string.h>

#include <fstream>
#include <bitset>
using namespace std;
#include <algorithm>

#include "TString.h"

#include "TADCntuRaw.hxx"

/*!
  \class TADCntuRaw TADCntuRaw.hxx "TADCntuRaw.hxx"
  \brief Mapping and Geometry parameters for Tof wall. **
*/

ClassImp(TADCntuHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TADCntuHit::~TADCntuHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TADCntuHit::TADCntuHit()
  : iddc(0),    iview(0),    ilayer(0),    icell(0), itrkass(0), ichi2(0),
    xcadc(0.),    ycadc(0.),    zcadc(0.),
    pxcadc(0.),   pycadc(0.),   pzcadc(0.),
    rdrift(0.),    tdrift(0.),    timdc(0.)
{
}

TADCntuHit::TADCntuHit(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t tm) {

  iddc = id;  iview = iv;  ilayer = il;      icell = ic;  ichi2 = 0;
  xcadc = x;   ycadc = y;  zcadc = z;
  pxcadc = px;   pycadc = py;   pzcadc = pz;
  rdrift = r;   tdrift = t;    timdc = tm;
  
}

void TADCntuHit::SetData(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t tm) {
  
  iddc = id;  iview = iv;  ilayer = il;      icell = ic;  ichi2 = 0;
  xcadc = x;   ycadc = y;  zcadc = z;
  pxcadc = px;   pycadc = py;   pzcadc = pz;
  rdrift = r;   tdrift = t;    timdc = tm;

  return;
}

void TADCntuHit::SetAW(TADCparGeo* f_dcgeo) {

  int idfilo = f_dcgeo->GetID(Cell());
  /*  vista U */
  Int_t tmp_fview = 0;
  if(View()<0) tmp_fview = 1;

  //Temporary FIX
  if(idfilo <0 || idfilo>2) idfilo = 2;
 
  A0.SetXYZ(f_dcgeo->GetX(idfilo,Plane(),tmp_fview),
	    f_dcgeo->GetY(idfilo,Plane(),tmp_fview),
	    f_dcgeo->GetZ(idfilo,Plane(),tmp_fview));
  
  Wvers.SetXYZ(f_dcgeo->GetCX(idfilo,Plane(),tmp_fview),
	       f_dcgeo->GetCY(idfilo,Plane(),tmp_fview),
	       f_dcgeo->GetCZ(idfilo,Plane(),tmp_fview));
  
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

ClassImp(TADCntuRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TADCntuRaw::TADCntuRaw() :
  nhit(0), h(0) {
}

//------------------------------------------+-----------------------------------
//! Destructor.

TADCntuRaw::~TADCntuRaw() {
  delete h;
}


//------------------------------------------+-----------------------------------
//! Setup clones.

void TADCntuRaw::SetupClones()
{
  if (!h) h = new TClonesArray("TADCntuHit");
  return;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TADCntuRaw::Clear(Option_t*)
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

void TADCntuRaw::ToStream(ostream& os, Option_t* option) const
{
  os << "TADCntuRaw " << GetName()
     << Form("  nhit=%3d", nhit)
     << endl;
  
  os << "slat stat    adct    adcb    tdct    tdcb" << endl;
  for (Int_t i = 0; i < nhit; i++) {
    const TADCntuHit*  hit = Hit(i);
    os << Form("%4d", hit->Cell());
    os << "  "; print_value(os, hit->Plane());
    os << endl;
  }
  return;
}

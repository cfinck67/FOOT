/*!
  \file
  \version $Id: TATWdatRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TATWdatRaw.
*/

#include <string.h>

#include <fstream>
#include <bitset>
using namespace std;
#include <algorithm>

#include "TString.h"

#include "TATWdatRaw.hxx"

/*!
  \class TATWdatRaw TATWdatRaw.hxx "TATWdatRaw.hxx"
  \brief Mapping and Geometry parameters for IR detectors. **
*/

ClassImp(TATWrawHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TATWrawHit::~TATWrawHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TATWrawHit::TATWrawHit()
  : ir_time(999999.), ir_chg(0.), ir_typ(0), ir_chid(0)
{
}

TATWrawHit::TATWrawHit(int typ, int cha, double charge, double time) {

  ir_time = time;
  ir_chg  = charge;
  ir_typ  = typ;
  ir_chid   = cha;

}

void TATWrawHit::SetData(Int_t type, Int_t id, Double_t time, Double_t charge) {

  ir_time = time;
  ir_chg  = charge;
  ir_typ  = type;
  ir_chid   = id;
  return;
}
//##############################################################################

ClassImp(TATWdatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATWdatRaw::TATWdatRaw() :
  nirhit(0), hir(0) {
}


//------------------------------------------+-----------------------------------
//! Destructor.

TATWdatRaw::~TATWdatRaw() {
  delete hir;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TATWdatRaw::SetupClones()
{
  if (!hir) hir = new TClonesArray("TATWrawHit");
  return;
}


/*------------------------------------------+---------------------------------*/
//! Set statistics counters.

void TATWdatRaw::SetCounter(Int_t i_ntdc, Int_t i_nadc, Int_t i_ndrop)
{
  fiNTdc  = i_ntdc;
  fiNAdc  = i_nadc;
  fiNDrop = i_ndrop;
  return;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TATWdatRaw::Clear(Option_t*)
{
  TAGdata::Clear();

  nirhit = 0;
  if (hir) hir->Clear();

  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TATWdatRaw::ToStream(ostream& os, Option_t* option) const
{
  os << "TATWdatRaw " << GetName()
     << endl;
  return;
}

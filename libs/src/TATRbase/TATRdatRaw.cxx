/*!
  \file
  \version $Id: TATRdatRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TATRdatRaw.
*/

#include <string.h>

#include <fstream>
#include <bitset>
using namespace std;
#include <algorithm>

#include "TString.h"

#include "TATRdatRaw.hxx"

/*!
  \class TATRdatRaw TATRdatRaw.hxx "TATRdatRaw.hxx"
  \brief Mapping and Geometry parameters for IR detectors. **
*/

ClassImp(TATRrawHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TATRrawHit::~TATRrawHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TATRrawHit::TATRrawHit()
  : ir_time(999999.), ir_chg(0.), ir_typ(0), ir_chid(0)
{
}

TATRrawHit::TATRrawHit(int typ, int cha, double charge, double time) {

  ir_time = time;
  ir_chg  = charge;
  ir_typ  = typ;
  ir_chid   = cha;

}

void TATRrawHit::SetData(Int_t type, Int_t id, Double_t time, Double_t charge) {

  ir_time = time;
  ir_chg  = charge;
  ir_typ  = type;
  ir_chid   = id;
  return;
}
//##############################################################################

ClassImp(TATRdatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATRdatRaw::TATRdatRaw() :
  nirhit(0), hir(0) {
}


//------------------------------------------+-----------------------------------
//! Destructor.

TATRdatRaw::~TATRdatRaw() {
  delete hir;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TATRdatRaw::SetupClones()
{
  if (!hir) hir = new TClonesArray("TATRrawHit");
  return;
}


/*------------------------------------------+---------------------------------*/
//! Set statistics counters.

void TATRdatRaw::SetCounter(Int_t i_ntdc, Int_t i_nadc, Int_t i_ndrop)
{
  fiNTdc  = i_ntdc;
  fiNAdc  = i_nadc;
  fiNDrop = i_ndrop;
  return;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TATRdatRaw::Clear(Option_t*)
{
  TAGdata::Clear();

  nirhit = 0;
  if (hir) hir->Clear();

  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TATRdatRaw::ToStream(ostream& os, Option_t* option) const
{
  os << "TATRdatRaw " << GetName()
     << endl;
  return;
}

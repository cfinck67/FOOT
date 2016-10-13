/*!
  \file
  \version $Id: TACAdatRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TACAdatRaw.
*/

#include <string.h>

#include <fstream>
#include <bitset>
using namespace std;
#include <algorithm>

#include "TString.h"

#include "TACAdatRaw.hxx"

/*!
  \class TACAdatRaw TACAdatRaw.hxx "TACAdatRaw.hxx"
  \brief Mapping and Geometry parameters for CA detectors. **
*/

ClassImp(TACArawHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TACArawHit::~TACArawHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TACArawHit::TACArawHit()
  : ir_time(999999.), ir_chg(0.), ir_typ(0), ir_chid(0)
{
}

TACArawHit::TACArawHit(int typ, int cha, double charge, double time) {

  ir_time = time;
  ir_chg  = charge;
  ir_typ  = typ;
  ir_chid   = cha;

}

void TACArawHit::SetData(Int_t type, Int_t id, Double_t time, Double_t charge) {

  ir_time = time;
  ir_chg  = charge;
  ir_typ  = type;
  ir_chid   = id;
  return;
}
//##############################################################################

ClassImp(TACAdatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TACAdatRaw::TACAdatRaw() :
  nirhit(0), hir(0) {
}


//------------------------------------------+-----------------------------------
//! Destructor.

TACAdatRaw::~TACAdatRaw() {
  delete hir;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TACAdatRaw::SetupClones()
{
  if (!hir) hir = new TClonesArray("TACArawHit");
  return;
}


/*------------------------------------------+---------------------------------*/
//! Set statistics counters.

void TACAdatRaw::SetCounter(Int_t i_ntdc, Int_t i_nadc, Int_t i_ndrop)
{
  fiNTdc  = i_ntdc;
  fiNAdc  = i_nadc;
  fiNDrop = i_ndrop;
  return;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TACAdatRaw::Clear(Option_t*)
{
  TAGdata::Clear();

  nirhit = 0;
  if (hir) hir->Clear();

  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TACAdatRaw::ToStream(ostream& os, Option_t* option) const
{
  os << "TACAdatRaw " << GetName()
     << endl;
  return;
}

/*!
  \file
  \version $Id: TAIRdatRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TAIRdatRaw.
*/

#include <string.h>

#include <fstream>
#include <bitset>
using namespace std;
#include <algorithm>

#include "TString.h"

#include "TAIRdatRaw.hxx"

/*!
  \class TAIRdatRaw TAIRdatRaw.hxx "TAIRdatRaw.hxx"
  \brief Mapping and Geometry parameters for IR detectors. **
*/

ClassImp(TAIRrawHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TAIRrawHit::~TAIRrawHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TAIRrawHit::TAIRrawHit()
  : ir_time(999999.), ir_chg(0.), ir_typ(0), ir_chid(0)
{
}

TAIRrawHit::TAIRrawHit(int det, int cha, double charge, double time) {

  ir_time = time;
  ir_chg  = charge;
  ir_typ  = det;
  ir_chid   = cha;

}

void TAIRrawHit::SetData(Int_t type, Int_t id, Double_t time, Double_t charge) {

  ir_time = time;
  ir_chg  = charge;
  ir_typ  = type;
  ir_chid   = id;
  return;
}
//##############################################################################

ClassImp(TAIRdatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAIRdatRaw::TAIRdatRaw() :
  nirhit(0), hir(0) {
}


//------------------------------------------+-----------------------------------
//! Destructor.

TAIRdatRaw::~TAIRdatRaw() {
  delete hir;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TAIRdatRaw::SetupClones()
{
  if (!hir) hir = new TClonesArray("TAIRrawHit");
  return;
}


/*------------------------------------------+---------------------------------*/
//! Set statistics counters.

void TAIRdatRaw::SetCounter(Int_t i_ntdc, Int_t i_nadc, Int_t i_ndrop)
{
  fiNTdc  = i_ntdc;
  fiNAdc  = i_nadc;
  fiNDrop = i_ndrop;
  return;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TAIRdatRaw::Clear(Option_t*)
{
  TAGdata::Clear();

  nirhit = 0;
  if (hir) hir->Clear();

  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TAIRdatRaw::ToStream(ostream& os, Option_t* option) const
{
  os << "TAIRdatRaw " << GetName()
     << endl;
  return;
}

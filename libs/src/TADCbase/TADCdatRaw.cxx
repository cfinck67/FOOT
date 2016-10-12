/*!
  \file
  \version $Id: adapted from TABMdatRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TADCdatRaw.
*/

#include <string.h>

#include <fstream>
#include <bitset>
using namespace std;
#include <algorithm>

#include "TString.h"

#include "TADCdatRaw.hxx"

/*!
  \class TADCdatRaw TADCdatRaw.hxx "TADCdatRaw.hxx"
  \brief Mapping and Geometry parameters for Tof wall. **
*/

ClassImp(TADCrawHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TADCrawHit::~TADCrawHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TADCrawHit::TADCrawHit()
  : iview(0),    ilayer(0),    icell(0),  tdctime(0.)
{
}

void TADCrawHit::SetData(Int_t view, Int_t lay, Int_t cell, double time) {

  iview = view;
  ilayer = lay;
  icell = cell;
  tdctime = time;
  return;
}
//##############################################################################

ClassImp(TADCdatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TADCdatRaw::TADCdatRaw() {
}

//------------------------------------------+-----------------------------------
//! Destructor.

TADCdatRaw::~TADCdatRaw() {}

//------------------------------------------+-----------------------------------
//! Setup mapping data for a single slat.

void TADCdatRaw::SetHitData(Int_t view, Int_t lay, Int_t cell, double time)
{

  Int_t i_ind = (Int_t) fHitList.size();
  fHitList.push_back(TADCrawHit());
 
  fHitList[i_ind].SetData(view,lay,cell,time);
  
  return;
}

/*------------------------------------------+---------------------------------*/
//! Set statistics counters.

void TADCdatRaw::SetCounter(Int_t i_ntdc, Int_t i_ndrop)
{
  fiNTdc  = i_ntdc;
  fiNDrop = i_ndrop;
  return;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TADCdatRaw::Clear(Option_t*)
{
  TAGdata::Clear();

  fHitList.clear();
  return;
}

/*------------------------------------------+---------------------------------*/
//! helper to format one converter value

static void print_value(ostream& os, Int_t i_val)
{
  char c_ran = (i_val & 0x1000) ? 'h' : 'l';
  os << Form("%4d", i_val & 0x0fff, "d", 4) << " " << c_ran;
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TADCdatRaw::ToStream(ostream& os, Option_t* option) const
{
  os << "TADCdatRaw " << GetName()
     << Form("  nhit=%3d", NHit())
     << Form("  ntdc=%3d", NTdc())
     << Form("  ndrop=%3d", NDrop())
     << endl;
  
  os << "slat stat    adct    adcb    tdct    tdcb" << endl;
  for (Int_t i = 0; i < NHit(); i++) {
    const TADCrawHit& hit = Hit(i);
    os << Form("%4d", hit.Cell());
    os << "  "; print_value(os, hit.Plane());
    os << endl;
  }
  return;
}

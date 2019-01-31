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


void TATRrawHit::Clear(Option_t* /*option*/)
{
   ir_time = 0.;
   ir_chg  = 0.;
   ir_typ  = 0;
   ir_chid = 0;
}

//##############################################################################

ClassImp(TATRdatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATRdatRaw::TATRdatRaw()
: TAGdata(),
  fListOfHits(0)
{
}


//------------------------------------------+-----------------------------------
//! Destructor.

TATRdatRaw::~TATRdatRaw()
{
  delete fListOfHits;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TATRdatRaw::SetupClones()
{
  if (!fListOfHits) fListOfHits = new TClonesArray("TATRrawHit");
  return;
}

//______________________________________________________________________________
//
TATRrawHit* TATRdatRaw::NewHit(int type, int channel, double charge, double time)
{
   TClonesArray &pixelArray = *fListOfHits;
   
   TATRrawHit* hit = new(pixelArray[pixelArray.GetEntriesFast()]) TATRrawHit(type, channel, charge, time);
   
   return hit;
   
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
  if (fListOfHits) fListOfHits->Clear("C");

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

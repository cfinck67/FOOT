/*!
  \file
  \version $Id: TASTdatRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TASTdatRaw.
*/

#include <string.h>

#include <fstream>
#include <bitset>
using namespace std;
#include <algorithm>

#include "TString.h"

#include "TASTdatRaw.hxx"

/*!
  \class TASTdatRaw TASTdatRaw.hxx "TASTdatRaw.hxx"
  \brief Mapping and Geometry parameters for IR detectors. **
*/

ClassImp(TASTrawHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TASTrawHit::~TASTrawHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TASTrawHit::TASTrawHit()
  : ir_time(999999.), ir_chg(0.), ir_typ(0), ir_chid(0)
{
}

TASTrawHit::TASTrawHit(int typ, int cha, double charge, double time) {

  ir_time = time;
  ir_chg  = charge;
  ir_typ  = typ;
  ir_chid   = cha;

}

void TASTrawHit::SetData(Int_t type, Int_t id, Double_t time, Double_t charge) {

  ir_time = time;
  ir_chg  = charge;
  ir_typ  = type;
  ir_chid   = id;
  return;
}


void TASTrawHit::Clear(Option_t* /*option*/)
{
   ir_time = 0.;
   ir_chg  = 0.;
   ir_typ  = 0;
   ir_chid = 0;
}

//##############################################################################

ClassImp(TASTdatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TASTdatRaw::TASTdatRaw()
: TAGdata(),
  fListOfHits(0)
{
}


//------------------------------------------+-----------------------------------
//! Destructor.

TASTdatRaw::~TASTdatRaw()
{
  delete fListOfHits;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TASTdatRaw::SetupClones()
{
  if (!fListOfHits) fListOfHits = new TClonesArray("TASTrawHit");
  return;
}

//______________________________________________________________________________
//
TASTrawHit* TASTdatRaw::NewHit(int type, int channel, double charge, double time)
{
   TClonesArray &pixelArray = *fListOfHits;
   
   TASTrawHit* hit = new(pixelArray[pixelArray.GetEntriesFast()]) TASTrawHit(type, channel, charge, time);
   
   return hit;
   
}


/*------------------------------------------+---------------------------------*/
//! Set statistics counters.

void TASTdatRaw::SetCounter(Int_t i_ntdc, Int_t i_nadc, Int_t i_ndrop)
{
  fiNTdc  = i_ntdc;
  fiNAdc  = i_nadc;
  fiNDrop = i_ndrop;
  return;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TASTdatRaw::Clear(Option_t*)
{
  TAGdata::Clear();
  if (fListOfHits) fListOfHits->Clear("C");

  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TASTdatRaw::ToStream(ostream& os, Option_t* option) const
{
  os << "TASTdatRaw " << GetName()
     << endl;
  return;
}

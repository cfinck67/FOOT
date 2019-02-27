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
  : st_time(999999.), st_chg(0.), st_chid(0)
{
}

TASTrawHit::TASTrawHit(int cha, double charge, double time) {

  st_time = time;
  st_chg  = charge;
  st_chid   = cha;

}

void TASTrawHit::SetData(Int_t id, Double_t time, Double_t charge) {

  st_time = time;
  st_chg  = charge;
  st_chid   = id;
  return;
}
//##############################################################################

ClassImp(TASTdatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TASTdatRaw::TASTdatRaw() :
  nsthit(0), hst(0) {
}


//------------------------------------------+-----------------------------------
//! Destructor.

TASTdatRaw::~TASTdatRaw() {
  delete hst;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TASTdatRaw::SetupClones()
{
  if (!hst) hst = new TClonesArray("TASTrawHit");
  return;
}



//------------------------------------------+-----------------------------------
//! Clear event.

void TASTdatRaw::Clear(Option_t*)
{
  TAGdata::Clear();

  nsthit = 0;
  if (hst) hst->Clear();

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

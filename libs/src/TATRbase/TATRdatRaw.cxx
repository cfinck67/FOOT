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
  : vu_stat(-1), trg_pat(-1), trg_count(-1)
{
  trg_tim.clear();    trg_tim.resize(7);
  trg_mul.clear();    trg_mul.resize(7);
  for(int i=0; i<trg_tim.size(); i++) {trg_tim.at(i) =  10000;}
  for(int i=0; i<trg_mul.size(); i++) {trg_mul.at(i) =  0;}
  for(int i=0; i<16; i++) { sca_cnt[i] = -1 ;}
  for(int i=0; i<16; i++) { 
    trg_bf_lmu[i]=-1;
    trg_bf_dti[i]=-1;
    trg_af_dti[i]=-1;
    trg_af_red[i]=-1;
  }
  trg_vul_time=-1;
}

TATRrawHit::TATRrawHit(int sta, int pat, int cnt) {

  vu_stat = sta;
  trg_pat  = pat;
  trg_count  = cnt;
  for(int i=0; i<16; i++) { sca_cnt[i] = -1 ;}

}

void TATRrawHit::SetTrigBefLMU(int id, int val) {

  if(id>=0 && id<16) {
    trg_bf_lmu[id] = val;
  } else {
    Error("Action()","SetTrigBef LMU %d Array out of size",id);
  }
  return;

}

int TATRrawHit::GetTrigBefLMU(int id) {

  int val = -1000;
  if(id>=0 && id<16) {
    val = trg_bf_lmu[id];
  } else {
    Error("Action()","SetTrigBef LMU %d Array out of size",id);
  }
  return val;

}


void TATRrawHit::SetTrigBefDT(int id, int val) {

  if(id>=0 && id<16) {
    trg_bf_dti[id] = val;
  } else {
    Error("Action()","SetTrigBef DT %d Array out of size",id);
  }
  return;

}

int TATRrawHit::GetTrigBefDT(int id) {

  int val = -1000;
  if(id>=0 && id<16) {
    val = trg_bf_dti[id];
  } else {
    Error("Action()","SetTrigBef DT %d Array out of size",id);
  }
  return val;

}

void TATRrawHit::SetTrigAftDT(int id, int val) {

  if(id>=0 && id<16) {
    trg_af_dti[id] = val;
  } else {
    Error("Action()","SetTrigAft DT %d Array out of size",id);
  }
  return;

}

int TATRrawHit::GetTrigAftDT(int id) {

  int val = -1000;
  if(id>=0 && id<16) {
    val = trg_af_dti[id];
  } else {
    Error("Action()","SetTrigAft DT %d Array out of size",id);
  }
  return val;

}

void TATRrawHit::SetTrigAftRed(int id, int val) {

  if(id>=0 && id<16) {
    trg_af_red[id] = val;
  } else {
    Error("Action()","SetTrigAft Red %d Array out of size",id);
  }
  return;

}

int TATRrawHit::GetTrigAftRed(int id) {

  int val = -1000;
  if(id>=0 && id<16) {
    val = trg_af_red[id];
  } else {
    Error("Action()","SetTrigAft Red %d Array out of size",id);
  }
  return val;

}


void TATRrawHit::SetData(int sta, int pat, int cnt) {

  vu_stat = sta;
  trg_pat  = pat;
  trg_count  = cnt;

  return;
}

void TATRrawHit::Clear(Option_t* /*option*/)
{
   trg_tim.clear();
   trg_mul.clear();
}

//##############################################################################

ClassImp(TATRdatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATRdatRaw::TATRdatRaw()
: TAGdata(),
 fListOfHits(0)
{
   SetupClones();
}


//------------------------------------------+-----------------------------------
//! Destructor.

TATRdatRaw::~TATRdatRaw()
{
  delete fListOfHits;
}

//------------------------------------------+-----------------------------------
Int_t TATRdatRaw::GetHitsN() const
{
   return fListOfHits->GetEntries();
}

//______________________________________________________________________________
//
TATRrawHit* TATRdatRaw::NewHit(int channel, double charge, double time)
{
   TClonesArray &pixelArray = *fListOfHits;
   
   TATRrawHit* hit = new(pixelArray[pixelArray.GetEntriesFast()]) TATRrawHit(channel, charge, time);
   
   return hit;
   
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TATRdatRaw::SetupClones()
{
  if (!fListOfHits) fListOfHits = new TClonesArray("TATRrawHit");
   
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


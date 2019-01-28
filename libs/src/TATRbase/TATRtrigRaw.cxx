/*!
  \file
  \version $Id: TATRtrigRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TATRtrigRaw.
*/

#include <string.h>

#include <fstream>
#include <bitset>
using namespace std;
#include <algorithm>

#include "TString.h"

#include "TATRtrigRaw.hxx"

/*!
  \class TATRtrigRaw TATRtrigRaw.hxx "TATRtrigRaw.hxx"
  \brief Mapping and Geometry parameters for IR detectors. **
*/

ClassImp(TATRtrigHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TATRtrigHit::~TATRtrigHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TATRtrigHit::TATRtrigHit()
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

TATRtrigHit::TATRtrigHit(int sta, int pat, int cnt) {

  vu_stat = sta;
  trg_pat  = pat;
  trg_count  = cnt;
  for(int i=0; i<16; i++) { sca_cnt[i] = -1 ;}

}

void TATRtrigHit::SetTrigBefLMU(int id, int val) {

  if(id>=0 && id<16) {
    trg_bf_lmu[id] = val;
  } else {
    Error("Action()","SetTrigBef LMU %d Array out of size",id);
  }
  return;

}

int TATRtrigHit::GetTrigBefLMU(int id) {

  int val = -1000;
  if(id>=0 && id<16) {
    val = trg_bf_lmu[id];
  } else {
    Error("Action()","SetTrigBef LMU %d Array out of size",id);
  }
  return val;

}


void TATRtrigHit::SetTrigBefDT(int id, int val) {

  if(id>=0 && id<16) {
    trg_bf_dti[id] = val;
  } else {
    Error("Action()","SetTrigBef DT %d Array out of size",id);
  }
  return;

}

int TATRtrigHit::GetTrigBefDT(int id) {

  int val = -1000;
  if(id>=0 && id<16) {
    val = trg_bf_dti[id];
  } else {
    Error("Action()","SetTrigBef DT %d Array out of size",id);
  }
  return val;

}

void TATRtrigHit::SetTrigAftDT(int id, int val) {

  if(id>=0 && id<16) {
    trg_af_dti[id] = val;
  } else {
    Error("Action()","SetTrigAft DT %d Array out of size",id);
  }
  return;

}

int TATRtrigHit::GetTrigAftDT(int id) {

  int val = -1000;
  if(id>=0 && id<16) {
    val = trg_af_dti[id];
  } else {
    Error("Action()","SetTrigAft DT %d Array out of size",id);
  }
  return val;

}

void TATRtrigHit::SetTrigAftRed(int id, int val) {

  if(id>=0 && id<16) {
    trg_af_red[id] = val;
  } else {
    Error("Action()","SetTrigAft Red %d Array out of size",id);
  }
  return;

}

int TATRtrigHit::GetTrigAftRed(int id) {

  int val = -1000;
  if(id>=0 && id<16) {
    val = trg_af_red[id];
  } else {
    Error("Action()","SetTrigAft Red %d Array out of size",id);
  }
  return val;

}


void TATRtrigHit::SetData(int sta, int pat, int cnt) {

  vu_stat = sta;
  trg_pat  = pat;
  trg_count  = cnt;

  return;
}

void TATRtrigHit::Clear(Option_t* /*option*/)
{
   trg_tim.clear();
   trg_mul.clear();
}

//##############################################################################

ClassImp(TATRtrigRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATRtrigRaw::TATRtrigRaw()
: TAGdata(),
 fListOfHits(0)
{
   SetupClones();
}


//------------------------------------------+-----------------------------------
//! Destructor.

TATRtrigRaw::~TATRtrigRaw()
{
  delete fListOfHits;
}

//------------------------------------------+-----------------------------------
Int_t TATRtrigRaw::GetHitsN() const
{
   return fListOfHits->GetEntries();
}

//______________________________________________________________________________
//
TATRtrigHit* TATRtrigRaw::NewHit(int channel, double charge, double time)
{
   TClonesArray &pixelArray = *fListOfHits;
   
   TATRtrigHit* hit = new(pixelArray[pixelArray.GetEntriesFast()]) TATRtrigHit(channel, charge, time);
   
   return hit;
   
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TATRtrigRaw::SetupClones()
{
  if (!fListOfHits) fListOfHits = new TClonesArray("TATRtrigHit");
   
  return;
}


//------------------------------------------+-----------------------------------
//! Clear event.

void TATRtrigRaw::Clear(Option_t*)
{
  TAGdata::Clear();
  if (fListOfHits) fListOfHits->Clear("C");

  return;
}


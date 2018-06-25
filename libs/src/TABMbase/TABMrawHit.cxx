#include "TABMrawHit.hxx"

using namespace std;

ClassImp(TABMrawHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TABMrawHit::~TABMrawHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMrawHit::TABMrawHit()
  : iview(0),    ilayer(0),    icell(0),  tdctime(0.)
{
}

void TABMrawHit::SetData(Int_t lay, Int_t view, Int_t cell, Double_t time) {

  iview = view;
  ilayer = lay;
  icell = cell;
  tdctime = time;
  return;
}

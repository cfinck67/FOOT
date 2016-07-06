#ifndef _ChargeBetheBlock_HXX
#define _ChargeBetheBlock_HXX

#include "TVector3.h"
#include "GlobalTrack.hxx"
#include "TrackableParticle.hxx"
#include "TAGntuGlbTracks.hxx"
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"
#include "TATntuRaw.hxx"
#include "gsi_geo.h"

using namespace std;


class ChargeBetheBlock : public TObject {
  
 public:

  ChargeBetheBlock(Bool_t,TAGntuGlbTrk *);
  ~ChargeBetheBlock();
  Int_t GetCharge() { return Charge_bb; }

 private:

  Double_t deltaE_z2_tof(Double_t deltaX, Double_t beta_t, Double_t mass);
  Int_t find_z_mv(Bool_t mc, Double_t ene, Double_t ebb);

  Bool_t isMC;
  Int_t Charge_bb;

};


#endif // _ChargeBetheBlock_HXX

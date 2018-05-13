/*!
  \file
  \version $Id: TAGactNtuMCeve.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TAGactNtuMCeve.
*/

#include "TAGntuMCeve.hxx"
#include "TAGactNtuMCeve.hxx"

//First
#include "TAGroot.hxx"

#include "foot_geo.h"
#include "math.h"

/*!
  \class TAGactNtuMCeve TAGactNtuMCeve.hxx "TAGactNtuMCeve.hxx"
  \brief NTuplizer for MC events. **
*/

ClassImp(TAGactNtuMCeve);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGactNtuMCeve::TAGactNtuMCeve(const char* name,
			       TAGdataDsc* p_nturaw, 
			       EVENT_STRUCT* evStr)
  : TAGaction(name, "TAGactNtuMCeve - NTuplize MC evt raw data"),
    fpNtuMC(p_nturaw),
    fpEvtStr(evStr)
{
  cout<<" Entering TAGactNtuMCeve::TAGactNtuMCeve()"<<endl;
  AddDataOut(p_nturaw, "TAGntuMCeve");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGactNtuMCeve::~TAGactNtuMCeve()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TAGactNtuMCeve::Action() {

  cout << " Entering TAGactNtuMCeve" << endl;
  TAGntuMCeve* p_nturaw = (TAGntuMCeve*) fpNtuMC->Object();

  // Int_t nh(0);
  p_nturaw->Clear();

  //Number of hits/tracks
  // nh = fpEvtStr->TRn;

  for (Int_t i = 0; i < fpEvtStr->TRn; i++) {
    Double_t i_mass = fpEvtStr->TRmass[i];
    Int_t i_id = fpEvtStr->TRfid[i];
    Int_t i_mid = fpEvtStr->TRpaid[i];  //mother id
    Int_t i_chg = fpEvtStr->TRcha[i];
    Int_t i_reg = fpEvtStr->TRreg[i];
    Int_t i_bar = fpEvtStr->TRbar[i];
    Int_t i_dead = fpEvtStr->TRdead[i];
    Int_t i_moth = i_mid-1;
    Int_t i_type = -999;//useless for now
    Double_t i_time = fpEvtStr->TRtime[i];
    Double_t i_tof = fpEvtStr->TRtof[i];
    Double_t i_trlen = fpEvtStr->TRtrlen[i];

    TVector3 ipos = TVector3(fpEvtStr->TRix[i],fpEvtStr->TRiy[i],fpEvtStr->TRiz[i]);
    TVector3 fpos = TVector3(fpEvtStr->TRfx[i],fpEvtStr->TRfy[i],fpEvtStr->TRfz[i]);
    TVector3 ip = TVector3(fpEvtStr->TRipx[i],fpEvtStr->TRipy[i],fpEvtStr->TRipz[i]);
    TVector3 fp = TVector3(fpEvtStr->TRfpx[i],fpEvtStr->TRfpy[i],fpEvtStr->TRfpz[i]);
    //decide if propagate also final momentum

    TVector3 mothip = TVector3 (0,0,0);
    if(i_moth>=0) mothip = TVector3(fpEvtStr->TRipx[i_moth],fpEvtStr->TRipy[i_moth],fpEvtStr->TRipz[i_moth]);
    TVector3 mothfp = TVector3 (0,0,0);
    if(i_moth>=0) mothfp = TVector3(fpEvtStr->TRfpx[i_moth],fpEvtStr->TRfpy[i_moth],fpEvtStr->TRfpz[i_moth]);

    //    Int_t i_pileup = fpEvtStr->TRpileup[i]; // VM added 17/11/13 for pileup
    Int_t i_pileup = 0; // VM added 17/11/13 for pileup

    Info("Action()","MCeve : %d %d %lf ",i_id,i_chg,i_mass);

    // VM changed 17/11/13   -->  male!
    new((*(p_nturaw->h))[i])    // to be changed!!!!!!!!!!!!!!!
      TAGntuMCeveHit(i_id,i_chg,i_type,i_reg,i_bar,i_dead,i_mass,i_moth,i_time,i_tof,i_trlen,ipos,fpos,ip,fp,mothip,mothfp,i_pileup);
    
  }
  cout << "TRn = " << fpEvtStr->TRn << "  MChit = "<< p_nturaw->GetHitN() << endl;

  // p_nturaw->nhit  = nh;    // to be changed!!!!!!!!!!!!!!!

  fpNtuMC->SetBit(kValid);
  return kTRUE;
}


















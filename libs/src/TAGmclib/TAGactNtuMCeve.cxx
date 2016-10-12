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

  Info("Action()"," Entering TAGactNtuMCeve");
  TAGntuMCeve* p_nturaw = (TAGntuMCeve*) fpNtuMC->Object();

  Int_t nh(0);
  p_nturaw->Clear();

  //Number of hits/tracks
  nh = fpEvtStr->trn;

  for (Int_t i = 0; i < fpEvtStr->trn; i++) {
    Double_t i_mass = fpEvtStr->trmass[i];
    Int_t i_id = fpEvtStr->trfid[i];
    Int_t i_mid = fpEvtStr->trpaid[i];  //mother id
    Int_t i_chg = fpEvtStr->trcha[i];
    Int_t i_reg = fpEvtStr->trreg[i];
    Int_t i_bar = fpEvtStr->trbar[i];
    Int_t i_dead = fpEvtStr->trdead[i];
    Int_t i_moth = i_mid-1;
    Int_t i_type = -999;//useless for now
    Double_t i_time = fpEvtStr->trtime[i];
    Double_t i_tof = fpEvtStr->tof[i];
    Double_t i_trlen = fpEvtStr->trlen[i];

    TVector3 ipos = TVector3(fpEvtStr->trix[i],fpEvtStr->triy[i],fpEvtStr->triz[i]);
    TVector3 fpos = TVector3(fpEvtStr->trfx[i],fpEvtStr->trfy[i],fpEvtStr->trfz[i]);
    TVector3 ip = TVector3(fpEvtStr->tripx[i],fpEvtStr->tripy[i],fpEvtStr->tripz[i]);
    TVector3 fp = TVector3(fpEvtStr->trfpx[i],fpEvtStr->trfpy[i],fpEvtStr->trfpz[i]);
    //decide if propagate also final momentum

    TVector3 mothip = TVector3 (0,0,0);
    if(i_moth>=0) mothip = TVector3(fpEvtStr->tripx[i_moth],fpEvtStr->tripy[i_moth],fpEvtStr->tripz[i_moth]);
    TVector3 mothfp = TVector3 (0,0,0);
    if(i_moth>=0) mothfp = TVector3(fpEvtStr->trfpx[i_moth],fpEvtStr->trfpy[i_moth],fpEvtStr->trfpz[i_moth]);

    //    Int_t i_pileup = fpEvtStr->trpileup[i]; // VM added 17/11/13 for pileup
    Int_t i_pileup = 0; // VM added 17/11/13 for pileup

    Info("Action()","MCeve : %d %d %lf ",i_id,i_chg,i_mass);

    // VM changed 17/11/13
    new((*(p_nturaw->h))[i]) 
      TAGntuMCeveHit(i_id,i_chg,i_type,i_reg,i_bar,i_dead,i_mass,i_moth,i_time,i_tof,i_trlen,ipos,fpos,ip,fp,mothip,mothfp,i_pileup);
    
  }

  p_nturaw->nhit  = nh;

  fpNtuMC->SetBit(kValid);
  return kTRUE;
}


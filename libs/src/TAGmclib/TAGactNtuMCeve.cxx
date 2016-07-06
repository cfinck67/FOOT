/*!
  \file
  \version $Id: TAGactNtuMCeve.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TAGactNtuMCeve.
*/

#include "TAGntuMCeve.hxx"
#include "TAGactNtuMCeve.hxx"

//First
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"

#include "gsi_geo.h"
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

  TAGgeoTrafo* fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

  Int_t nh(0);
  p_nturaw->Clear();

  //Number of hits/tracks
  nh = fpEvtStr->trn;

  for (Int_t i = 0; i < fpEvtStr->trn; i++) {
    
    Double_t i_mass = fpEvtStr->trmass[i];
    Int_t i_id = fpEvtStr->trid[i];
    Int_t i_type = fpEvtStr->trtype[i];
    Int_t i_mid = fpEvtStr->trpaid[i];
    Int_t i_chg = fpEvtStr->trcha[i];
    Int_t i_reg = fpEvtStr->trreg[i];
    Int_t i_bar = fpEvtStr->trbar[i];
    Int_t i_dead = fpEvtStr->trdead[i];

    TVector3 ipos = TVector3(fpEvtStr->trix[i],fpEvtStr->triy[i],fpEvtStr->triz[i]);
    TVector3 fpos = TVector3(fpEvtStr->trfx[i],fpEvtStr->trfy[i],fpEvtStr->trfz[i]);
    TVector3 ip = TVector3(fpEvtStr->tripx[i],fpEvtStr->tripy[i],fpEvtStr->tripz[i]);

    TVector3 mothp = TVector3(fpEvtStr->trpapx[i],fpEvtStr->trpapy[i],fpEvtStr->trpapz[i]);

    Int_t i_pileup = fpEvtStr->trpileup[i]; // VM added 17/11/13 for pileup

    Info("Action()","MCeve : %d %d %lf ",i_id,i_chg,i_mass);

    // VM changed 17/11/13
    new((*(p_nturaw->h))[i]) 
      TAGntuMCeveHit(i_id,i_chg,i_type,i_reg,i_bar,i_dead,i_mass,ipos,fpos,ip,i_mid,mothp,i_pileup);
    
  }

  p_nturaw->nhit  = nh;

  fpNtuMC->SetBit(kValid);
  return kTRUE;
}


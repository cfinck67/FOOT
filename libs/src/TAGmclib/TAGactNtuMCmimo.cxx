/*!
  \file
  \version $Id: TAGactNtuMCmimo.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TAGactNtuMCmimo.
*/

#include "TAGntuMCmimo.hxx"
#include "TAGactNtuMCmimo.hxx"

//First
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"

#include "gsi_geo.h"
#include "math.h"

/*!
  \class TAGactNtuMCmimo TAGactNtuMCmimo.hxx "TAGactNtuMCmimo.hxx"
  \brief NTuplizer for MC events. **
*/

ClassImp(TAGactNtuMCmimo);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGactNtuMCmimo::TAGactNtuMCmimo(const char* name,
				 TAGdataDsc* p_nturaw, 
				 EVENT_STRUCT* evStr)
  : TAGaction(name, "TAGactNtuMCmimo - NTuplize MC evt raw data"),
    fpNtuMC(p_nturaw),
    fpEvtStr(evStr)
{
  cout<<" Entering TAGactNtuMCmimo::TAGactNtuMCmimo()"<<endl;
  AddDataOut(p_nturaw, "TAGntuMCmimo");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGactNtuMCmimo::~TAGactNtuMCmimo()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TAGactNtuMCmimo::Action() {

  Info("Action()"," Entering TAGactNtuMCmimo");
  TAGntuMCmimo* p_nturaw = (TAGntuMCmimo*) fpNtuMC->Object();

  TAGgeoTrafo* fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

  Int_t nh(0);
  p_nturaw->Clear();

 //Number of hits/tracks
  nh = fpEvtStr->miSigN;

  for (Int_t i = 0; i < fpEvtStr->miSigN; i++) {
    
    Int_t i_id = fpEvtStr->miSigId[i];

    TVector3 ipos = TVector3(fpEvtStr->miSigX[i],fpEvtStr->miSigY[i],fpEvtStr->miSigZ[i]);
    TVector3 ip = TVector3(fpEvtStr->miSigpX[i],fpEvtStr->miSigpY[i],fpEvtStr->miSigpZ[i]);
    // VM added 3/11/13 energy loss
    Double_t de= fpEvtStr->miSigDE[i];
    if (de<0) de=0;

    Info("Action()","MCmimo : %d %d %lf ",i_id);

    //new((*(p_nturaw->h))[i]) 
    //  TAGntuMCmimoHit(i_id,ipos,ip);
    // VM (3/11/13) changed to save de info
    new((*(p_nturaw->h))[i]) 
      TAGntuMCmimoHit(i_id,ipos,ip,de);

    
  }

  p_nturaw->nhit  = nh;

  fpNtuMC->SetBit(kValid);
  return kTRUE;
}


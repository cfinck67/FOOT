/*!
  \file
  \version $Id: TAGactNtuMCtof.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TAGactNtuMCtof.
*/

#include "TAGntuMCtof.hxx"
#include "TAGactNtuMCtof.hxx"

//First
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"

#include "gsi_geo.h"
#include "math.h"

/*!
  \class TAGactNtuMCtof TAGactNtuMCtof.hxx "TAGactNtuMCtof.hxx"
  \brief NTuplizer for MC events. **
*/

ClassImp(TAGactNtuMCtof);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGactNtuMCtof::TAGactNtuMCtof(const char* name,
				 TAGdataDsc* p_nturaw, 
				 EVENT_STRUCT* evStr)
  : TAGaction(name, "TAGactNtuMCtof - NTuplize MC evt raw data"),
    fpNtuMC(p_nturaw),
    fpEvtStr(evStr)
{
  cout<<" Entering TAGactNtuMCtof::TAGactNtuMCtof()"<<endl;
  AddDataOut(p_nturaw, "TAGntuMCtof");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGactNtuMCtof::~TAGactNtuMCtof()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TAGactNtuMCtof::Action() {

  Info("Action()"," Entering TAGactNtuMCtof");
  TAGntuMCtof* p_nturaw = (TAGntuMCtof*) fpNtuMC->Object();

  TAGgeoTrafo* fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

  Int_t nh(0);
  bool tobeAdded(kFALSE);
  p_nturaw->Clear();

  //In the TOF wall things are hard!
  //First of all we find from the list of scored hits what are the hits that the ToF really sees.
  vector<int> hitIDs; hitIDs.clear();
  for (Int_t i = 0; i < fpEvtStr->twn; i++) { 
    //ID of the Hit that passed trough the front slat
    if(fpEvtStr->twSlID[0][i]>0 && fpEvtStr->twSlID[0][i]<=96) {
      if(hitIDs.size() == 0) {
	hitIDs.push_back(fpEvtStr->twplid[i]);
      } else {
	tobeAdded = kTRUE;
	for(int it =0; it<(int) hitIDs.size(); it++) {
	  if(fpEvtStr->twplid[i] == hitIDs.at(it)) {
	    tobeAdded = kFALSE;
	    break;
	  }
	}
	if(tobeAdded) hitIDs.push_back(fpEvtStr->twplid[i]);
      }
    }
    //ID of the Hit that passed trough the rear slat
    if(fpEvtStr->twSlID[2][i]>0 && fpEvtStr->twSlID[2][i]<=96) {
      if(hitIDs.size() == 0) {
	hitIDs.push_back(fpEvtStr->twplid[i]);
      } else {
	tobeAdded = kTRUE;
	for(int it =0; it<(int) hitIDs.size(); it++) {
	  if(fpEvtStr->twplid[i] == hitIDs.at(it)) {
	    tobeAdded = kFALSE;
	    break;
	  }
	}
	if(tobeAdded) hitIDs.push_back(fpEvtStr->twplid[i]);
      }
    }
  }

  //Number of all ToF hits
  nh = fpEvtStr->twn;
  int toth(0);
  for (Int_t i = 0; i <nh; i++) {
    
    Int_t i_id = fpEvtStr->twplid[i];

    //Select the hit to be added based on the scroing info on the ToF planes
    //Assuming that the i_id and hitsIDs info is the same!
    tobeAdded = kFALSE;
    for(int id=0; id<(int)hitIDs.size(); id++) {
      if(hitIDs.at(id) == i_id) {
	tobeAdded = kTRUE;
	break;
      }
    }

    TVector3 ipos = TVector3(fpEvtStr->twplinx[i],fpEvtStr->twpliny[i],fpEvtStr->twplinz[i]);
    TVector3 ip = TVector3(fpEvtStr->twplinpx[i],fpEvtStr->twplinpy[i],fpEvtStr->twplinpz[i]);
    Double_t tof = Double_t(fpEvtStr->twpltim[i]);

    Info("Action()","MCtof : %d %d %lf ",i_id);
    if(tobeAdded) {
      new((*(p_nturaw->h))[toth]) 
	TAGntuMCtofHit(i_id,ipos,ip,tof);
      toth++;
    }
    
  }

  p_nturaw->nhit  = toth;

  fpNtuMC->SetBit(kValid);
  return kTRUE;
}


#include <stdio.h>

#include "gsi_geo.h"

#include "Evento.h"
#include "Segnale.h"

#include "TDirectory.h"
#include "TFile.h"
#include "TSystem.h"

#include <iostream>
#include <fstream>

#include <queue> // VM added for pile up 14/11/13


using namespace std;

ClassImp( Evento);

/*-----------------------------------------------------------------*/

Evento::Evento(Segnale *TheSegnale, Geometry *TheGeometry) :
  fSeg(TheSegnale), fGeo(TheGeometry) {
  //  printf("enter\n");
  //initialize
  ar3 = new TRandom3();
  ar3->SetSeed(12345);
  
  TFile *prevFile = gFile;
  /*
  string c_dir = gSystem->Getenv("FLUKAGSI");
  c_dir = c_dir + "/config/bmreso_vs_r.root";
  TFile *f = new TFile(c_dir.data(),"READ");
  f->cd();
  myreso = ((TH1D*)gDirectory->Get("myHp"));
  cout<<"BM:: ENT:: "<<myreso->GetEntries()<<endl;
  */
  prevFile->cd();

  SetToMax(&eve);
  //TODO: won't work for readout since is with Event->mu
  //  pmu= &(eve.mu);
  
  //   printf("set\n");
  BOEvent();
  //   printf("exit\n");

  count_skip=0; // VM pile-up


}

/*-----------------------------------------------------------------*/
Evento::~Evento() {

}

/*-----------------------------------------------------------------*/
Int_t Evento::SetEvent(Int_t numero_evento) {
	//  cout <<" Entro in SetEvent"<<endl;
	eve.EventNumber = numero_evento;
	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Evento::BOEvent() {
	//  cout <<" Entro in clean"<<endl;

	//clean signal
	fSeg->BOEvent();
	//clean event structure
	CleanEvent(&eve);

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Evento::SetToMax(EVENT_STRUCT *Event) {

	//initialize
	Event->trn = MAXTR;
	Event->stn = MAXST;
	Event->nmon = MAXMON;
	Event->tgn = MAXTG;
	Event->min = MAXMI;
	Event->miSigN = MAXMISIG;


	return 0;
}

/*-----------------------------------------------------------------*/

Int_t Evento::CleanEvent(EVENT_STRUCT *Event) {
  //  cout <<" Entro in clean"<<endl;


	for (int kk = 0; kk < Event->trn; kk++) {
  	        Event->trpileup[kk] = 0;  // VM added 17/11/13 for pile-up
		Event->trid[kk] = 0;
		Event->trpaid[kk] = 0;
		Event->trtype[kk] = -100;
		Event->trmass[kk] = 0.;
		Event->trcha[kk] = -100;
		Event->trbar[kk] = 0;
		Event->trreg[kk] = 0;
		Event->trgen[kk] = 0;
		Event->trdead[kk] = 0;
		Event->tritime[kk] = 0.;
		Event->trix[kk] = 0.;
		Event->triy[kk] = 0.;
		Event->triz[kk] = 0.;
		Event->trfx[kk] = 0.;
		Event->trfy[kk] = 0.;
		Event->trfz[kk] = 0.;
		Event->tripx[kk] = 0.;
		Event->tripy[kk] = 0.;
		Event->tripz[kk] = 0.;
		Event->trpapx[kk] = 0.;
		Event->trpapy[kk] = 0.;
		Event->trpapz[kk] = 0.;
	}
	//START
	for (int kk = 0; kk < Event->stn; kk++) {
		Event->stid[kk] = 0;
		Event->stflg[kk] = 0;
		Event->stinx[kk] = 0.;
		Event->stiny[kk] = 0.;
		Event->stinz[kk] = 0.;
		Event->stoutx[kk] = 0.;
		Event->stouty[kk] = 0.;
		Event->stoutz[kk] = 0.;
		Event->stinpx[kk] = 0.;
		Event->stinpy[kk] = 0.;
		Event->stinpz[kk] = 0.;
		Event->stoutpx[kk] = 0.;
		Event->stoutpy[kk] = 0.;
		Event->stoutpz[kk] = 0.;
		Event->stde[kk] = 0.;
		Event->stal[kk] = 0.;
		Event->sttim[kk] = 0.;

		Event->stSigAmp = 0.;
		Event->stSigTime = 0.;
		Event->stSigED = 0.;

		Event->stSigAmpDig = 0;
		Event->stSigTimeDig = 0;
	}
	//BMON
	for (int kk = 0; kk < Event->nmon; kk++) {
		Event->idmon[kk] = 0;
		Event->iview[kk] = 0;
		Event->icell[kk] = 0;
		Event->ilayer[kk] = 0;
		Event->xcamon[kk] = 0.;
		Event->ycamon[kk] = 0.;
		Event->zcamon[kk] = 0.;
		Event->pxcamon[kk] = 0.;
		Event->pycamon[kk] = 0.;
		Event->pzcamon[kk] = 0.;
	}
	//TARGET
	for (int kk = 0; kk < Event->tgn; kk++) {
		Event->tgid[kk] = 0;
		Event->tgflg[kk] = 0;
		Event->tginx[kk] = 0.;
		Event->tginy[kk] = 0.;
		Event->tginz[kk] = 0.;
		Event->tgoutx[kk] = 0.;
		Event->tgouty[kk] = 0.;
		Event->tgoutz[kk] = 0.;
		Event->tginpx[kk] = 0.;
		Event->tginpy[kk] = 0.;
		Event->tginpz[kk] = 0.;
		Event->tgoutpx[kk] = 0.;
		Event->tgoutpy[kk] = 0.;
		Event->tgoutpz[kk] = 0.;
		Event->tgtim[kk] = 0.;

	}

	//vertex tracker block (MIMOSA)
	for (Int_t kk = 0; kk < Event->min; kk++) {
		Event->miid[kk] = 0;
		Event->michip[kk] = 0;
		Event->mincol[kk] = 0;
		Event->minrow[kk] = 0;
		Event->mix[kk] = 0.;
		Event->miy[kk] = 0.;
		Event->miz[kk] = 0.;
		Event->mipx[kk] = 0.;
		Event->mipy[kk] = 0.;
		Event->mipz[kk] = 0.;
		Event->mide[kk] = 0.;
		Event->mitim[kk] = 0.;
	}
	//signal of MIMOSA26
	for (int kk = 0; kk < Event->miSigN; kk++) {
		Event->miSigChip[kk] = 0;
		Event->miSigIndex[kk] = 0;
		Event->miSigCol[kk] = 0;
		Event->miSigRow[kk] = 0;
		Event->miSigId[kk] = 0;
		Event->miSigTim[kk] = 0.;
		Event->miSigX[kk] = 0.;
		Event->miSigY[kk] = 0.;
		Event->miSigZ[kk] = 0.;
		Event->miSigpX[kk] = 0.;
		Event->miSigpY[kk] = 0.;
		Event->miSigpZ[kk] = 0.;
		Event->miSigPedest[kk] = 0.;
		Event->miSigDE[kk] = 0.; // VM added 3/11/13
	}


	//set entry counter to zero
	Event->EventNumber = -1;
	Event->trn = 0;
	Event->stn = 0;
	Event->nmon = 0;
	Event->tgn = 0;
	Event->min = 0;
	Event->miSigN = 0;

	return 0;
}

/*-----------------------------------------------------------------*/

Int_t Evento::InitBranches(TTree *RootTree, EVENT_STRUCT *Event) {

	RootTree->Branch("EventNumber", &(Event->EventNumber), "EventNumber/I");

	/*  tracks  */
	RootTree->Branch("trn", &(Event->trn), "trn/I");
	RootTree->Branch("trpileup", &(Event->trpileup), "trpileup[trn]/I");
	RootTree->Branch("trid", &(Event->trid), "trid[trn]/I");
	RootTree->Branch("trpaid", &(Event->trpaid), "trpaid[trn]/I");
	RootTree->Branch("trtype", &(Event->trtype), "trtype[trn]/I");
	RootTree->Branch("trmass", &(Event->trmass), "trmass[trn]/D");
	RootTree->Branch("trcha", &(Event->trcha), "trcha[trn]/I");
	RootTree->Branch("trbar", &(Event->trbar), "trbar[trn]/I");
	RootTree->Branch("trgen", &(Event->trgen), "trgen[trn]/I");
	RootTree->Branch("trdead", &(Event->trdead), "trdead[trn]/I");
	RootTree->Branch("trreg", &(Event->trreg), "trreg[trn]/I");
	RootTree->Branch("tritime", &(Event->tritime), "tritime[trn]/D");
	RootTree->Branch("trix", &(Event->trix), "trix[trn]/D");
	RootTree->Branch("triy", &(Event->triy), "triy[trn]/D");
	RootTree->Branch("triz", &(Event->triz), "triz[trn]/D");
	RootTree->Branch("trfx", &(Event->trfx), "trfx[trn]/D");
	RootTree->Branch("trfy", &(Event->trfy), "trfy[trn]/D");
	RootTree->Branch("trfz", &(Event->trfz), "trfz[trn]/D");
	RootTree->Branch("tripx", &(Event->tripx), "tripx[trn]/D");
	RootTree->Branch("tripy", &(Event->tripy), "tripy[trn]/D");
	RootTree->Branch("tripz", &(Event->tripz), "tripz[trn]/D");
	RootTree->Branch("trpapx", &(Event->trpapx), "trpapx[trn]/D");
	RootTree->Branch("trpapy", &(Event->trpapy), "trpapy[trn]/D");
	RootTree->Branch("trpapz", &(Event->trpapz), "trpapz[trn]/D");

	/*  start counter  */
	RootTree->Branch("stn", &(Event->stn), "stn/I");
	RootTree->Branch("stid", &(Event->stid), "idst[stn]/I");
	RootTree->Branch("stflg", &(Event->stflg), "stflg[stn]/I");
	RootTree->Branch("stinx", &(Event->stinx), "stinx[stn]/D");
	RootTree->Branch("stiny", &(Event->stiny), "stiny[stn]/D");
	RootTree->Branch("stinz", &(Event->stinz), "stinz[stn]/D");
	RootTree->Branch("stinpx", &(Event->stinpx), "stinpx[stn]/D");
	RootTree->Branch("stinpy", &(Event->stinpy), "stinpy[stn]/D");
	RootTree->Branch("stinpz", &(Event->stinpz), "stinpz[stn]/D");
	RootTree->Branch("stoutx", &(Event->stoutx), "stoutx[stn]/D");
	RootTree->Branch("stouty", &(Event->stouty), "stouty[stn]/D");
	RootTree->Branch("stoutz", &(Event->stoutz), "stoutz[stn]/D");
	RootTree->Branch("stoutpx", &(Event->stoutpx), "stoutpx[stn]/D");
	RootTree->Branch("stoutpy", &(Event->stoutpy), "stoutpy[stn]/D");
	RootTree->Branch("stoutpz", &(Event->stoutpz), "stoutpz[stn]/D");
	RootTree->Branch("stde", &(Event->stde), "stde[stn]/D");
	RootTree->Branch("stal", &(Event->stal), "stal[stn]/D");
	RootTree->Branch("sttim", &(Event->sttim), "sttim[stn]/D");
	//
	RootTree->Branch("stSigAmp", &(Event->stSigAmp), "stSigAmp/D");
	RootTree->Branch("stSigTime", &(Event->stSigTime), "stSigTime/D");
	RootTree->Branch("stSigED", &(Event->stSigED), "stSigED/D");
	RootTree->Branch("stSigAmpDig", &(Event->stSigAmpDig), "stSigAmpDig/I");
	RootTree->Branch("stSigTimeDig", &(Event->stSigTimeDig), "stSigTimeDig/I");

	/*  beam monitor  */
	RootTree->Branch("nmon", &(Event->nmon), "nmon/I");
	RootTree->Branch("idmon", &(Event->idmon), "idmon[nmon]/I");
	RootTree->Branch("iview", &(Event->iview), "iview[nmon]/I");
	RootTree->Branch("icell", &(Event->icell), "icell[nmon]/I");
	RootTree->Branch("ilayer", &(Event->ilayer), "ilayer[nmon]/I");
	RootTree->Branch("xcamon", &(Event->xcamon), "xcamon[nmon]/D");
	RootTree->Branch("ycamon", &(Event->ycamon), "ycamon[nmon]/D");
	RootTree->Branch("zcamon", &(Event->zcamon), "zcamon[nmon]/D");
	RootTree->Branch("pxcamon", &(Event->pxcamon), "pxcamon[nmon]/D");
	RootTree->Branch("pycamon", &(Event->pycamon), "pycamon[nmon]/D");
	RootTree->Branch("pzcamon", &(Event->pzcamon), "pzcamon[nmon]/D");
	RootTree->Branch("rdrift", &(Event->rdrift), "rdrift[nmon]/D");
	RootTree->Branch("tdrift", &(Event->tdrift), "tdrift[nmon]/D");
	RootTree->Branch("timmon", &(Event->timmon), "timmon[nmon]/D");

	/*  target  */
	RootTree->Branch("tgn", &(Event->tgn), "tgn/I");
	RootTree->Branch("tgid", &(Event->tgid), "tgid[tgn]/I");
	RootTree->Branch("tgflg", &(Event->tgflg), "tgflg[tgn]/I");
	RootTree->Branch("tginx", &(Event->tginx), "tginx[tgn]/D");
	RootTree->Branch("tginy", &(Event->tginy), "tginy[tgn]/D");
	RootTree->Branch("tginz", &(Event->tginz), "tginz[tgn]/D");
	RootTree->Branch("tginpx", &(Event->tginpx), "tginpx[tgn]/D");
	RootTree->Branch("tginpy", &(Event->tginpy), "tginpy[tgn]/D");
	RootTree->Branch("tginpz", &(Event->tginpz), "tginpz[tgn]/D");
	RootTree->Branch("tgoutx", &(Event->tgoutx), "tgoutx[tgn]/D");
	RootTree->Branch("tgouty", &(Event->tgouty), "tgouty[tgn]/D");
	RootTree->Branch("tgoutz", &(Event->tgoutz), "tgoutz[tgn]/D");
	RootTree->Branch("tgoutpx", &(Event->tgoutpx), "tgoutpx[tgn]/D");
	RootTree->Branch("tgoutpy", &(Event->tgoutpy), "tgoutpy[tgn]/D");
	RootTree->Branch("tgoutpz", &(Event->tgoutpz), "tgoutpz[tgn]/D");
	RootTree->Branch("tgtim", &(Event->tgtim), "tgtim[tgn]/D");

	/*  vertex tracker (MIMOSA)  */
	RootTree->Branch("min", &(Event->min), "min/I");
	RootTree->Branch("miid", &(Event->miid), "miid[min]/I");
	RootTree->Branch("michip", &(Event->michip), "michip[min]/I");
	RootTree->Branch("mincol", &(Event->mincol), "mincol[min]/I");
	RootTree->Branch("minrow", &(Event->minrow), "minrow[min]/I");
	RootTree->Branch("mix", &(Event->mix), "mix[min]/D");
	RootTree->Branch("miy", &(Event->miy), "miy[min]/D");
	RootTree->Branch("miz", &(Event->miz), "miz[min]/D");
	RootTree->Branch("mipx", &(Event->mipx), "mipx[min]/D");
	RootTree->Branch("mipy", &(Event->mipy), "mipy[min]/D");
	RootTree->Branch("mipz", &(Event->mipz), "mipz[min]/D");
	RootTree->Branch("mide", &(Event->mide), "mide[min]/D");
	RootTree->Branch("mitim", &(Event->mitim), "mitim[min]/D");
	//signal of MIMOSA26
	RootTree->Branch("miSigN", &(Event->miSigN), "miSigN/I");
	RootTree->Branch("miSigChip", &(Event->miSigChip), "miSigChip[miSigN]/I");
	RootTree->Branch("miSigIndex", &(Event->miSigIndex), "miSigIndex[miSigN]/I");
	RootTree->Branch("miSigCol", &(Event->miSigCol), "miSigCol[miSigN]/I");
	RootTree->Branch("miSigRow", &(Event->miSigRow), "miSigRow[miSigN]/I");
	RootTree->Branch("miSigId", &(Event->miSigId), "miSigId[miSigN]/I");
	RootTree->Branch("miSigTim", &(Event->miSigTim), "miSigTim[miSigN]/D");
	RootTree->Branch("miSigX", &(Event->miSigX), "miSigX[miSigN]/D");
	RootTree->Branch("miSigY", &(Event->miSigY), "miSigY[miSigN]/D");
	RootTree->Branch("miSigZ", &(Event->miSigZ), "miSigZ[miSigN]/D");
	RootTree->Branch("miSigpX", &(Event->miSigpX), "miSigpX[miSigN]/D");
	RootTree->Branch("miSigpY", &(Event->miSigpY), "miSigpY[miSigN]/D");
	RootTree->Branch("miSigpZ", &(Event->miSigpZ), "miSigpZ[miSigN]/D");
	RootTree->Branch("miSigPedest", &(Event->miSigPedest),
			"miSigPedest[miSigN]/D");
	// VM added 3/11/13
	RootTree->Branch("miSigDE", &(Event->miSigDE), "miSigDE[miSigN]/D");

	return 0;
}

/*-----------------------------------------------------------------*/

Int_t Evento::FindBranches(TTree *RootTree, EVENT_STRUCT *Event) {

  /*  tracks  */

  RootTree->SetBranchAddress("trn", &(Event->trn));
  RootTree->SetBranchAddress("trpileup", &(Event->trpileup));
  RootTree->SetBranchAddress("trid", &(Event->trid));
  RootTree->SetBranchAddress("trpaid", &(Event->trpaid));
  RootTree->SetBranchAddress("trtype", &(Event->trtype));
  RootTree->SetBranchAddress("trmass", &(Event->trmass));
  RootTree->SetBranchAddress("trcha", &(Event->trcha));
  RootTree->SetBranchAddress("trbar", &(Event->trbar));
  RootTree->SetBranchAddress("trgen", &(Event->trgen));
  RootTree->SetBranchAddress("trdead", &(Event->trdead));
  RootTree->SetBranchAddress("trreg", &(Event->trreg));
  RootTree->SetBranchAddress("tritime", &(Event->tritime));
  RootTree->SetBranchAddress("trix", &(Event->trix));
  RootTree->SetBranchAddress("triy", &(Event->triy));
  RootTree->SetBranchAddress("triz", &(Event->triz));
  RootTree->SetBranchAddress("trfx", &(Event->trfx));
  RootTree->SetBranchAddress("trfy", &(Event->trfy));
  RootTree->SetBranchAddress("trfz", &(Event->trfz));
  RootTree->SetBranchAddress("tripx", &(Event->tripx));
  RootTree->SetBranchAddress("tripy", &(Event->tripy));
  RootTree->SetBranchAddress("tripz", &(Event->tripz));
  RootTree->SetBranchAddress("trpapx", &(Event->trpapx));
  RootTree->SetBranchAddress("trpapy", &(Event->trpapy));
  RootTree->SetBranchAddress("trpapz", &(Event->trpapz));

  /*  start counter  */
  RootTree->SetBranchAddress("stn", &(Event->stn));
  RootTree->SetBranchAddress("stid", &(Event->stid));
  RootTree->SetBranchAddress("stflg", &(Event->stflg));
  RootTree->SetBranchAddress("stinx", &(Event->stinx));
  RootTree->SetBranchAddress("stiny", &(Event->stiny));
  RootTree->SetBranchAddress("stinz", &(Event->stinz));
  RootTree->SetBranchAddress("stinpx", &(Event->stinpx));
  RootTree->SetBranchAddress("stinpy", &(Event->stinpy));
  RootTree->SetBranchAddress("stinpz", &(Event->stinpz));
  RootTree->SetBranchAddress("stoutx", &(Event->stoutx));
  RootTree->SetBranchAddress("stouty", &(Event->stouty));
  RootTree->SetBranchAddress("stoutz", &(Event->stoutz));
  RootTree->SetBranchAddress("stoutpx", &(Event->stoutpx));
  RootTree->SetBranchAddress("stoutpy", &(Event->stoutpy));
  RootTree->SetBranchAddress("stoutpz", &(Event->stoutpz));
  RootTree->SetBranchAddress("stde", &(Event->stde));
  RootTree->SetBranchAddress("stal", &(Event->stal));
  RootTree->SetBranchAddress("sttim", &(Event->sttim));
  // signal
  RootTree->SetBranchAddress("stSigAmp", &(Event->stSigAmp));
  RootTree->SetBranchAddress("stSigTime", &(Event->stSigTime));
  RootTree->SetBranchAddress("stSigED", &(Event->stSigED));
  RootTree->SetBranchAddress("stSigAmpDig", &(Event->stSigAmpDig));
  RootTree->SetBranchAddress("stSigTimeDig", &(Event->stSigTimeDig));

  /*  beam monitor  */
  RootTree->SetBranchAddress("nmon", &(Event->nmon));
  RootTree->SetBranchAddress("idmon", &(Event->idmon));
  RootTree->SetBranchAddress("iview", &(Event->iview));
  RootTree->SetBranchAddress("icell", &(Event->icell));
  RootTree->SetBranchAddress("ilayer", &(Event->ilayer));
  RootTree->SetBranchAddress("xcamon", &(Event->xcamon));
  RootTree->SetBranchAddress("ycamon", &(Event->ycamon));
  RootTree->SetBranchAddress("zcamon", &(Event->zcamon));
  RootTree->SetBranchAddress("pxcamon", &(Event->pxcamon));
  RootTree->SetBranchAddress("pycamon", &(Event->pycamon));
  RootTree->SetBranchAddress("pzcamon", &(Event->pzcamon));
  RootTree->SetBranchAddress("rdrift", &(Event->rdrift));
  RootTree->SetBranchAddress("tdrift", &(Event->tdrift));
  RootTree->SetBranchAddress("timmon", &(Event->timmon));

  /*  target  */
  RootTree->SetBranchAddress("tgn", &(Event->tgn));
  RootTree->SetBranchAddress("tgid", &(Event->tgid));
  RootTree->SetBranchAddress("tgflg", &(Event->tgflg));
  RootTree->SetBranchAddress("tginx", &(Event->tginx));
  RootTree->SetBranchAddress("tginy", &(Event->tginy));
  RootTree->SetBranchAddress("tginz", &(Event->tginz));
  RootTree->SetBranchAddress("tginpx", &(Event->tginpx));
  RootTree->SetBranchAddress("tginpy", &(Event->tginpy));
  RootTree->SetBranchAddress("tginpz", &(Event->tginpz));
  RootTree->SetBranchAddress("tgoutx", &(Event->tgoutx));
  RootTree->SetBranchAddress("tgouty", &(Event->tgouty));
  RootTree->SetBranchAddress("tgoutz", &(Event->tgoutz));
  RootTree->SetBranchAddress("tgoutpx", &(Event->tgoutpx));
  RootTree->SetBranchAddress("tgoutpy", &(Event->tgoutpy));
  RootTree->SetBranchAddress("tgoutpz", &(Event->tgoutpz));
  RootTree->SetBranchAddress("tgtim", &(Event->tgtim));

  /*  vertex tracker (MIMOSA)  */
  RootTree->SetBranchAddress("min", &(Event->min));
  RootTree->SetBranchAddress("miid", &(Event->miid));
  RootTree->SetBranchAddress("michip", &(Event->michip));
  RootTree->SetBranchAddress("mincol", &(Event->mincol));
  RootTree->SetBranchAddress("minrow", &(Event->minrow));
  RootTree->SetBranchAddress("mix", &(Event->mix));
  RootTree->SetBranchAddress("miy", &(Event->miy));
  RootTree->SetBranchAddress("miz", &(Event->miz));
  RootTree->SetBranchAddress("mipx", &(Event->mipx));
  RootTree->SetBranchAddress("mipy", &(Event->mipy));
  RootTree->SetBranchAddress("mipz", &(Event->mipz));
  RootTree->SetBranchAddress("mide", &(Event->mide));
  RootTree->SetBranchAddress("mitim", &(Event->mitim));
  //signal of MIMOSA26
  RootTree->SetBranchAddress("miSigN", &(Event->miSigN));
  RootTree->SetBranchAddress("miSigChip", &(Event->miSigChip));
  RootTree->SetBranchAddress("miSigIndex", &(Event->miSigIndex));
  RootTree->SetBranchAddress("miSigCol", &(Event->miSigCol));
  RootTree->SetBranchAddress("miSigRow", &(Event->miSigRow));
  RootTree->SetBranchAddress("miSigId", &(Event->miSigId));
  RootTree->SetBranchAddress("miSigTim", &(Event->miSigTim));
  RootTree->SetBranchAddress("miSigX", &(Event->miSigX));
  RootTree->SetBranchAddress("miSigY", &(Event->miSigY));
  RootTree->SetBranchAddress("miSigZ", &(Event->miSigZ));
  RootTree->SetBranchAddress("miSigpX", &(Event->miSigpX));
  RootTree->SetBranchAddress("miSigpY", &(Event->miSigpY));
  RootTree->SetBranchAddress("miSigpZ", &(Event->miSigpZ));
  RootTree->SetBranchAddress("miSigPedest", &(Event->miSigPedest));
  RootTree->SetBranchAddress("miSigDE", &(Event->miSigDE)); // VM 3/11/13


  return 0;
}
/*-----------------------------------------------------------------*/
Int_t Evento::EOEvent() {
	//TODO: IMPLEMENT the ED variables!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Int_t Count;
	TVector3 MiPos;
	Int_t skip=0;  // VM 14/11/13 for pileup

	//signal calculation
	fSeg->EOEvent();

	//store analog and digititsed signal in event structure

	//Start Counter
	eve.stSigAmp = fSeg->SC_GetAmplitude();
	eve.stSigTime = fSeg->SC_GetTime();
	eve.stSigED = fSeg->SC_GetED();

	eve.stSigAmpDig = fSeg->SC_GetAmplitudeDig();
	eve.stSigTimeDig = fSeg->SC_GetTimeDig();

	// VM 14/11/2013 changed for pile-up simulation
	// parameters in gsi_geo.h
	MIMO_STRUCT * mimo = new MIMO_STRUCT();
	MIMO_STRUCT * mimo2;
	mimo->min=eve.min;
	for (Int_t i0=0;i0<eve.min;i0++) {
	  mimo->miid[i0]=eve.miid[i0];
	  mimo->michip[i0]=eve.michip[i0];
	  mimo->mincol[i0]=eve.mincol[i0];
	  mimo->minrow[i0]=eve.minrow[i0];
	  mimo->mix[i0]=eve.mix[i0];
	  mimo->miy[i0]=eve.miy[i0];
	  mimo->miz[i0]=eve.miz[i0];
	  mimo->mipx[i0]=eve.mipx[i0];
	  mimo->mipy[i0]=eve.mipy[i0];
	  mimo->mipz[i0]=eve.mipz[i0];
	  mimo->mide[i0]=eve.mide[i0];
	  mimo->mitim[i0]=eve.mitim[i0];
	}
	mimo->miSigN = 0;
	for (Int_t i0 = 0; i0 < fSeg->MI_GetHitPads(); i0++) {
	  if (fSeg->MI_GetActive(i0)) {
	    mimo->miSigChip[mimo->miSigN] = fSeg->MI_GetChip(i0);
	    //AS:: indexing in MC does has to be fixed at reco level
	    //See please, TAVTactNtuMC for guidance
	    mimo->miSigIndex[mimo->miSigN] = 
	      fGeo->MI_IndexRec(fSeg->MI_GetColumn(i0),fSeg->MI_GetRow(i0));
	    //this is to get index for reconstruction
	    mimo->miSigCol[mimo->miSigN] = fSeg->MI_GetColumn(i0);
	    mimo->miSigRow[mimo->miSigN] = fSeg->MI_GetRow(i0);
	    mimo->miSigId[mimo->miSigN] = fSeg->MI_GetID(i0);
	    mimo->miSigTim[mimo->miSigN] = fSeg->MI_GetTime(i0);
	    //MiPos=fGeo->MI_Coord(fSeg->MI_GetColumn(i0),fSeg->MI_GetRow(i0),fSeg->MI_GetChip(i0)-1);//this is to get FLUKA coordinates
	    //AS:: indexing in MC does has to be fixed at reco level
	    //See please, TAVTactNtuMC for guidance
	    MiPos = fGeo->MI_CoordRec(fSeg->MI_GetColumn(i0),
				      fSeg->MI_GetRow(i0), fSeg->MI_GetChip(i0) - 1);//this is to get coordinates for reconstruction
	    mimo->miSigX[mimo->miSigN] = fSeg->MI_GetPosition(i0).X();
	    mimo->miSigY[mimo->miSigN] = fSeg->MI_GetPosition(i0).Y();
	    mimo->miSigZ[mimo->miSigN] = fSeg->MI_GetPosition(i0).Z();
	    mimo->miSigpX[mimo->miSigN] = fSeg->MI_GetMomentum(i0).X();
	    mimo->miSigpY[mimo->miSigN] = fSeg->MI_GetMomentum(i0).Y();
	    mimo->miSigpZ[mimo->miSigN] = fSeg->MI_GetMomentum(i0).Z();
	    mimo->miSigPedest[mimo->miSigN] = 0.; //for the moment a dummy value
	    mimo->miSigDE[mimo->miSigN] = fSeg->MI_GetEDtot(i0); // VM 3/11/13
	    mimo->miSigN++;
	  }
	  if (mimo->miSigN == MAXMISIG) {
	    cerr
	      << "WARNING: Evento::EOEvent(): array overflow: mimo->miSigN==MAXMISIG "
	      << MAXMISIG << " !" << endl;
	    break;
	  }
	}

   	MC_STRUCT * mcblk = new MC_STRUCT();
	MC_STRUCT * mcblk2;
	mcblk->trn=eve.trn;
	for (int i0=0;i0<mcblk->trn;i0++) {
	  mcblk->trpileup[i0]=eve.trpileup[i0];
	  if (eve.trid[i0]>=5000) cout << "WARNING: trid=" << eve.trid[i0] << endl; 
	  mcblk->trid[i0] = eve.trid[i0];
	  if (eve.trpaid[i0]>=5000) cout << "WARNING: trpaid=" << eve.trpaid[i0] << endl; 
	  mcblk->trpaid[i0] = eve.trpaid[i0];
	  mcblk->trtype[i0] =eve.trtype[i0];
	  mcblk->trbar[i0] = eve.trbar[i0];
	  mcblk->trcha[i0] = eve.trcha[i0];
	  mcblk->trmass[i0] = eve.trmass[i0];
	  mcblk->trreg[i0] = eve.trreg[i0];
	  mcblk->trgen[i0] = eve.trgen[i0];
	  mcblk->trdead[i0] = eve.trdead[i0];
	  mcblk->tritime[i0] =eve.tritime[i0];
	  mcblk->trix[i0] = eve.trix[i0];
	  mcblk->triy[i0] = eve.triy[i0];
	  mcblk->triz[i0] = eve.triz[i0];
	  mcblk->trfx[i0] =  eve.trfx[i0];
	  mcblk->trfy[i0] = eve.trfy[i0];
	  mcblk->trfz[i0] = eve.trfz[i0];
	  mcblk->tripx[i0] =  eve.tripx[i0];
	  mcblk->tripy[i0] = eve.tripy[i0];
	  mcblk->tripz[i0] = eve.tripz[i0];
	  mcblk->trpapx[i0] = eve.trpapx[i0];
	  mcblk->trpapy[i0] =eve.trpapy[i0];
	  mcblk->trpapz[i0] = eve.trpapz[i0];
	}



	Int_t n_vtx=1;
	if (MI_PILE_UP) {
	  do {
	    n_vtx = rnd.Poisson(MI_MEAN_NVTX);
	  } while (!n_vtx);
	  if (n_vtx>1 && mimo_queue.size()<(n_vtx-1)) {
	    skip=1;
	    if (!MI_PILEUP_SKIP_EV) n_vtx=mimo_queue.size()+1;
	  }
	  if (count_skip>0) {
	    count_skip--;
	    skip=1;
	    if (MI_PILEUP_SKIP_EV) n_vtx=1;
	  } else if (mimo_queue.size()<MI_LOW_QUEUE) {
	    skip=1;
	    if (MI_PILEUP_SKIP_EV) n_vtx=1;
	    //cout << "queue small sime=" << mimo_queue.size() << " min=" << MI_LOW_QUEUE << endl;
	    count_skip=MI_HIGH_QUEUE-MI_LOW_QUEUE;
	  } else 
	    skip=0;
	} else { // no pile-up
	  n_vtx=1;
	}

	// fill eve structure with mimosa infoes
	eve.miSigN=0;
	eve.min=0;
	eve.trn = 0;

	for (int i=0;i<n_vtx;i++) {
	  if (!i) mimo2=mimo;
	  else mimo2=mimo_queue.front();
	  if (!i) mcblk2=mcblk;
	  else mcblk2=mc_queue.front();
	  for (Int_t i0 = 0; i0 < mimo2->min; i0++) {
	    if (eve.min < MAXMI) {
	      eve.miid[eve.min] = mimo2->miid[i0];
	      eve.michip[eve.min] = mimo2->michip[i0];
	      eve.mincol[eve.min] = mimo2->mincol[i0];
	      eve.minrow[eve.min] = mimo2->minrow[i0];
	      eve.mix[eve.min] = mimo2->mix[i0];
	      eve.miy[eve.min] = mimo2->miy[i0];
	      eve.miz[eve.min] = mimo2->miz[i0];
	      eve.mipx[eve.min] = mimo2->mipx[i0];
	      eve.mipy[eve.min] = mimo2->mipy[i0];
	      eve.mipz[eve.min] = mimo2->mipz[i0];
	      eve.mide[eve.min] = mimo2->mide[i0];
	      eve.mitim[eve.min] = mimo2->mitim[i0];
	      eve.min++;
	    } 
	  }
	  for (Int_t i0 = 0; i0 < mimo2->miSigN; i0++) {
	    if (eve.miSigN<MAXMISIG) {
	      eve.miSigChip[eve.miSigN] = mimo2->miSigChip[i0];
	      eve.miSigIndex[eve.miSigN] = mimo2->miSigIndex[i0];
	      eve.miSigCol[eve.miSigN] = mimo2->miSigCol[i0];
	      eve.miSigRow[eve.miSigN] = mimo2->miSigRow[i0];
	      eve.miSigId[eve.miSigN] = mimo2->miSigId[i0]+eve.trn;
	      eve.miSigTim[eve.miSigN] = mimo2->miSigTim[i0];
	      eve.miSigX[eve.miSigN] = mimo2->miSigX[i0] ;
	      eve.miSigY[eve.miSigN] = mimo2->miSigY[i0] ;
	      eve.miSigZ[eve.miSigN] = mimo2->miSigZ[i0];
	      eve.miSigpX[eve.miSigN] = mimo2->miSigpX[i0];
	      eve.miSigpY[eve.miSigN] = mimo2->miSigpY[i0];
	      eve.miSigpZ[eve.miSigN] = mimo2->miSigpZ[i0] ;
	      eve.miSigPedest[eve.miSigN] = 0.; //for the moment a dummy value
	      eve.miSigDE[eve.miSigN] = mimo2->miSigDE[i0];
	      eve.miSigN++;
	    }
	  }

	  for (Int_t i0 = 0; i0 < mcblk2->trn; i0++) {
	    if (eve.trn < MAXTR) {
	      eve.trpileup[eve.trn]= i;
	      eve.trid[eve.trn]= mcblk2->trid[i0]+PILEUP_ROUND*i;
	      eve.trpaid[eve.trn]= mcblk2->trpaid[i0]+PILEUP_ROUND*i;
	      eve.trtype[eve.trn]= mcblk2->trtype[i0];
	      eve.trbar[eve.trn]= mcblk2->trbar[i0];
	      eve.trcha[eve.trn]= mcblk2->trcha[i0];
	      eve.trmass[eve.trn]= mcblk2->trmass[i0];
	      eve.trreg[eve.trn]= mcblk2->trreg[i0];
	      eve.trgen[eve.trn]= mcblk2->trgen[i0];
	      eve.trdead[eve.trn]= mcblk2->trdead[i0];
	      eve.tritime[eve.trn]= mcblk2->tritime[i0];
	      eve.trix[eve.trn]= mcblk2->trix[i0];
	      eve.triy[eve.trn]= mcblk2->triy[i0];
	      eve.triz[eve.trn]= mcblk2->triz[i0];
	      eve.trfx[eve.trn]= mcblk2->trfx[i0];
	      eve.trfy[eve.trn]= mcblk2->trfy[i0];
	      eve.trfz[eve.trn]= mcblk2->trfz[i0];
	      eve.tripx[eve.trn]= mcblk2->tripx[i0];
	      eve.tripy[eve.trn]= mcblk2->tripy[i0];
	      eve.tripz[eve.trn]= mcblk2->tripz[i0];
	      eve.trpapx[eve.trn]= mcblk2->trpapx[i0];
	      eve.trpapy[eve.trn]= mcblk2->trpapy[i0];
	      eve.trpapz[eve.trn]= mcblk2->trpapz[i0];
	      eve.trn++;
	    }
	  }

	  if (i) {
	    if (mimo_queue.size()>0) mimo_queue.pop();
	    delete mimo2;
	    if (mc_queue.size()>0) mc_queue.pop();
	    delete mcblk2;
	  }
	}
	if (MI_PILE_UP) {
	  if (skip || !MI_PILEUP_SKIP_EV) mimo_queue.push(mimo);
	  else delete mimo;
	  // check if the fifo has too many elements
	  if (mimo_queue.size()>MI_HIGH_QUEUE) {
	    for (Int_t j=(MI_HIGH_QUEUE+1);j<=mimo_queue.size();j++) {
              mimo2=mimo_queue.front();
	      mimo_queue.pop();
	      delete mimo2;
	    }
	  }
	} else {
	  delete mimo;
	}

	if (MI_PILE_UP) {
	  if (skip || !MI_PILEUP_SKIP_EV) mc_queue.push(mcblk);
	  else delete mcblk;
	  // check if the fifo has too many elements
	  if (mc_queue.size()>MI_HIGH_QUEUE) {
	    for (Int_t j=(MI_HIGH_QUEUE+1);j<=mc_queue.size();j++) {
              mcblk2=mc_queue.front();
	      mc_queue.pop();
	      delete mcblk2;
	    }
	  }
	} else {
	  delete mcblk;
	}
	return skip;
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddPart(Int_t numtrack, Int_t idpa, Int_t igen, Int_t icha,
		Int_t numreg, Int_t iba, Int_t idead, Int_t jpa, Double_t vxi,
		Double_t vyi, Double_t vzi, Double_t vxf, Double_t vyf, Double_t vzf,
		Double_t px, Double_t py, Double_t pz, Double_t pxpa, Double_t pypa,
		Double_t pzpa, Double_t amass, Double_t tempo) {
	//  cout<<"Entro in Evento::AddPart "<<endl;
	if (eve.trn < MAXTR) {
		eve.trpileup[eve.trn]=0;
		eve.trid[eve.trn] = numtrack;
		eve.trpaid[eve.trn] = idpa;
		eve.trtype[eve.trn] = jpa;
		eve.trbar[eve.trn] = iba;
		eve.trcha[eve.trn] = icha;
		eve.trmass[eve.trn] = amass;
		eve.trreg[eve.trn] = numreg;
		eve.trgen[eve.trn] = igen;
		eve.trdead[eve.trn] = idead;
		eve.tritime[eve.trn] = tempo;
		eve.trix[eve.trn] = vxi;
		eve.triy[eve.trn] = vyi;
		eve.triz[eve.trn] = vzi;
		eve.trfx[eve.trn] = vxf;
		eve.trfy[eve.trn] = vyf;
		eve.trfz[eve.trn] = vzf;
		eve.tripx[eve.trn] = px;
		eve.tripy[eve.trn] = py;
		eve.tripz[eve.trn] = pz;
		eve.trpapx[eve.trn] = pxpa;
		eve.trpapy[eve.trn] = pypa;
		eve.trpapz[eve.trn] = pzpa;
		eve.trn++;
		return 0;
	} else {
		return -1;
	}
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddStart(Int_t idst, Int_t flgst, Double_t xinst, Double_t yinst,
		Double_t zinst, Double_t xoutst, Double_t youtst, Double_t zoutst,
		Double_t pxinst, Double_t pyinst, Double_t pzinst, Double_t pxoutst,
		Double_t pyoutst, Double_t pzoutst, Double_t dest, Double_t alst,
		Double_t timst) {
	//  cout<<"Entro in Evento::AddStart "<<endl;
	if (eve.stn < MAXST) {

		eve.stid[eve.stn] = idst;
		eve.stflg[eve.stn] = flgst;
		eve.stinx[eve.stn] = xinst;
		eve.stiny[eve.stn] = yinst;
		eve.stinz[eve.stn] = zinst;
		eve.stinpx[eve.stn] = pxinst;
		eve.stinpy[eve.stn] = pyinst;
		eve.stinpz[eve.stn] = pzinst;
		eve.stoutx[eve.stn] = xoutst;
		eve.stouty[eve.stn] = youtst;
		eve.stoutz[eve.stn] = zoutst;
		eve.stoutpx[eve.stn] = pxoutst;
		eve.stoutpy[eve.stn] = pyoutst;
		eve.stoutpz[eve.stn] = pzoutst;
		eve.stde[eve.stn] = dest;
		eve.stal[eve.stn] = alst;
		eve.sttim[eve.stn] = timst;
		eve.stn++;

		fSeg->SC_Score(
				0 //charge not needed currently, if needed: eve.icha[fidst]
				, dest, alst, xinst, yinst, zinst, xoutst, youtst, zoutst,
				timst);

		return 0;
	} else {
		return -1;
	}
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddBMON(Int_t idmon, Int_t iview, Int_t icell, Int_t ilayer,
		      Double_t xcamon, Double_t ycamon, Double_t zcamon, Double_t pxcamon,
		      Double_t pycamon, Double_t pzcamon, Double_t rdrift, Double_t tdrift,
		      Double_t timmon) {
  //  cout<<"Entro in Evento::AddMon "<<endl;
  double sigma;
  Double_t myrdrift;
  int abin;
  if (eve.nmon < MAXMON) {
    eve.nmon++;
    eve.idmon[eve.nmon - 1] = idmon;
    eve.iview[eve.nmon - 1] = iview;
    eve.icell[eve.nmon - 1] = icell;
    eve.ilayer[eve.nmon - 1] = ilayer;
    eve.xcamon[eve.nmon - 1] = xcamon;
    eve.ycamon[eve.nmon - 1] = ycamon;
    eve.zcamon[eve.nmon - 1] = zcamon;
    eve.pxcamon[eve.nmon - 1] = pxcamon;
    eve.pycamon[eve.nmon - 1] = pycamon;
    eve.pzcamon[eve.nmon - 1] = pzcamon;
    //Here we have to add the resolution
    abin = myreso->FindBin(rdrift);
    sigma = myreso->GetBinContent(abin);
    //    cout<<"BM:: sigma "<<sigma<<endl;
    myrdrift = fabs(ar3->Gaus(rdrift,sigma/10000));

    eve.rdrift[eve.nmon - 1] = myrdrift;
    eve.tdrift[eve.nmon - 1] = tdrift;
    eve.timmon[eve.nmon - 1] = timmon;
    return 0;
  } else {
    return -1;
  }

  return -1;
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddTarget(Int_t idtg, Int_t flgtg, Double_t xintg,
		Double_t yintg, Double_t zintg, Double_t xouttg, Double_t youttg,
		Double_t zouttg, Double_t pxintg, Double_t pyintg, Double_t pzintg,
		Double_t pxouttg, Double_t pyouttg, Double_t pzouttg, Double_t timtg) {
	//  cout<<"Entro in Evento::AddTgart "<<endl;
	if (eve.tgn < MAXTG) {
		eve.tgid[eve.tgn] = idtg;
		eve.tgflg[eve.tgn] = flgtg;
		eve.tginx[eve.tgn] = xintg;
		eve.tginy[eve.tgn] = yintg;
		eve.tginz[eve.tgn] = zintg;
		eve.tginpx[eve.tgn] = pxintg;
		eve.tginpy[eve.tgn] = pyintg;
		eve.tginpz[eve.tgn] = pzintg;
		eve.tgoutx[eve.tgn] = xouttg;
		eve.tgouty[eve.tgn] = youttg;
		eve.tgoutz[eve.tgn] = zouttg;
		eve.tgoutpx[eve.tgn] = pxouttg;
		eve.tgoutpy[eve.tgn] = pyouttg;
		eve.tgoutpz[eve.tgn] = pzouttg;
		eve.tgtim[eve.tgn] = timtg;
		eve.tgn++;

		return 0;
	} else {
		return -1;
	}
}

/*-----------------------------------------------------------------*/


Int_t Evento::AddMaps(Int_t idmaps, Int_t chipmaps, Int_t ncolmaps,
		Int_t nrowmaps, Double_t xmaps, Double_t ymaps, Double_t zmaps,
		Double_t pxmaps, Double_t pymaps, Double_t pzmaps, Double_t demaps,
		Double_t timmaps) {
  // VM 30/1/14 particle charge needed for the VTX
  // cluster size in Segnale
  Int_t chg;
  // look for particle charge
  for (int i=0;i<eve.trn;i++) {
    if (eve.trid[i]==idmaps) {
      chg=eve.trcha[i];
      break;
    }
  }

         if (eve.min < MAXMI) {
		eve.miid[eve.min] = idmaps;
		eve.michip[eve.min] = chipmaps;
		eve.mincol[eve.min] = ncolmaps;
		eve.minrow[eve.min] = nrowmaps;
		eve.mix[eve.min] = xmaps;
		eve.miy[eve.min] = ymaps;
		eve.miz[eve.min] = zmaps;
		eve.mipx[eve.min] = pxmaps;
		eve.mipy[eve.min] = pymaps;
		eve.mipz[eve.min] = pzmaps;
		eve.mide[eve.min] = demaps;
		eve.mitim[eve.min] = timmaps;
		eve.min++;

		fSeg->MI_Score(chipmaps, ncolmaps, nrowmaps, 
			       idmaps, chg, demaps, xmaps, ymaps, zmaps
			       , pxmaps, pymaps, pzmaps
			       , timmaps);

		return 0;
	} else {
		return -1;
	}
}

/*-----------------------------------------------------------------*/

Int_t Evento::Dump() {
	cout << "----------Dump event structure: -------------" << endl << endl;

	cout << " Interaction structure:  trn= " << eve.trn << endl;
	for (int ii = 0; ii < eve.trn; ii++) {
		cout << "particle# " << ii << " id particle= " << eve.trid[ii]
				<< " trgen= " << eve.trgen[ii] << " chrge= " << eve.trcha[ii]
				<< " numreg= " << eve.trreg[ii] << " trtype=" << eve.trtype[ii]
				<< endl;
		cout << "vert in= " << eve.trix[ii] << " " << eve.triy[ii] << " "
				<< eve.triz[ii] << "vert out= " << eve.trfx[ii] << " "
				<< eve.trfy[ii] << " " << eve.trfz[ii] << endl << "p= "
				<< eve.tripx[ii] << " " << eve.tripy[ii] << " "
				<< eve.tripz[ii] << endl;
	}

	cout << "MON structure:  nmon= " << eve.nmon << endl;
	for (int ii = 0; ii < eve.nmon; ii++) {
		cout << "hit " << ii << " idt= " << eve.idmon[ii] << " lay= "
				<< eve.ilayer[ii] << " view= " << eve.iview[ii] << " cell= "
				<< eve.icell[ii] << " rdrift=" << eve.rdrift[ii] << endl;
		cout << " xyz= " << eve.xcamon[ii] << " " << eve.ycamon[ii] << " "
				<< eve.zcamon[ii] << " tdrift=" << eve.tdrift[ii] << endl;
	}

	return 0;
}

/*-----------------------------------------------------------------*/

EVENT_STRUCT Evento::Output() {
	return eve;
}

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************



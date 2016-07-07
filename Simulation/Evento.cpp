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
  string c_dir = gSystem->Getenv("FLUKAGSI");
  c_dir = c_dir + "/config/bmreso_vs_r.root";
  TFile *f = new TFile(c_dir.data(),"READ");
  f->cd();
  myreso = ((TH1D*)gDirectory->Get("myHp"));
  cout<<"BM:: ENT:: "<<myreso->GetEntries()<<endl;

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
	Event->ken = MAXKE;
	Event->keSigN = MAXKE;
	Event->keSigDigN = MAXKE;

	for (Int_t i0 = 0; i0 < MU_PRO_Nb; i0++) {
		Event->mun[i0] = MAXMU;
		Event->muSigN[i0] = MAXMU;
		Event->muSigDigN[i0] = MAXMU;

		//    Event->mu.n[i0] = MAXMU;
		//    Event->mu.SigN[i0] = MAXMU;
		//    Event->mu.SigDigN[i0] = MAXMU;
	}

	Event->twn = MAXTW;

	Event->onn = MAXON;
	Event->onSigN = MAXON;
	Event->onSigDigN = MAXON;

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

	//KENTROS
	for (int kk = 0; kk < Event->ken; kk++) {
		Event->keid[kk] = 0;
		Event->kereg[kk] = 0;
		Event->keregtype[kk] = 0;
		Event->keinx[kk] = 0.;
		Event->keiny[kk] = 0.;
		Event->keinz[kk] = 0.;
		Event->keoutx[kk] = 0.;
		Event->keouty[kk] = 0.;
		Event->keoutz[kk] = 0.;
		Event->keinpx[kk] = 0.;
		Event->keinpy[kk] = 0.;
		Event->keinpz[kk] = 0.;
		Event->keoutpx[kk] = 0.;
		Event->keoutpy[kk] = 0.;
		Event->keoutpz[kk] = 0.;
		Event->kede[kk] = 0.;
		Event->keal[kk] = 0.;
		Event->ketim[kk] = 0.;
	}
	//KENTROS signal
	for (int kk = 0; kk < Event->keSigN; kk++) {
		Event->keSigReg[kk] = 0;
		Event->keSigAmp[kk] = 0.;
		Event->keSigTim[kk] = 0.;
		Event->keSigED[kk] = 0.;
		Event->keSigX[kk] = 0.;
		Event->keSigY[kk] = 0.;
		Event->keSigZ[kk] = 0.;
		Event->keSigpX[kk] = 0.;
		Event->keSigpY[kk] = 0.;
		Event->keSigpZ[kk] = 0.;
	}
	for (int kk = 0; kk < Event->keSigDigN; kk++) {
		Event->keSigRegDig[kk] = 0;
		Event->keSigAmpDig[kk] = 0;
		Event->keSigTimDig[kk] = 0;
	}

	//TP-MUSIC IV CLASS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	/*  for(int jj=0;jj<MU_PRO_Nb;jj++){
	 Event->mu.id[jj].clear();
	 Event->mu.inx[jj].clear();
	 Event->mu.iny[jj].clear();
	 Event->mu.inz[jj].clear();
	 Event->mu.outx[jj].clear();
	 Event->mu.outy[jj].clear();
	 Event->mu.outz[jj].clear();
	 Event->mu.inpx[jj].clear();
	 Event->mu.inpy[jj].clear();
	 Event->mu.inpz[jj].clear();
	 Event->mu.outpx[jj].clear();
	 Event->mu.outpy[jj].clear();
	 Event->mu.outpz[jj].clear();
	 Event->mu.de[jj].clear();
	 Event->mu.al[jj].clear();
	 Event->mu.tim[jj].clear();
	 //
	 for(int kk=0;kk<Event->mu.SigN[jj];kk++){
	 Event->mu.SigTim[jj][kk]=0.;
	 Event->mu.SigAmp[jj][kk]=0.;
	 Event->mu.SigHei[jj][kk]=0.;
	 }
	 for(int kk=0;kk<Event->mu.SigDigN[jj];kk++){
	 Event->mu.SigTimDig[jj][kk]=0;
	 Event->mu.SigAmpDig[jj][kk]=0;
	 Event->mu.SigHeiDig[jj][kk]=0;

	 }
	 }
	 */

	//TP-MUSIC IV
	for (int jj = 0; jj < MU_PRO_Nb; jj++) {

		for (int kk = 0; kk < Event->mun[jj]; kk++) {
			Event->muid[jj][kk] = 0;
			Event->muinx[jj][kk] = 0.;
			Event->muiny[jj][kk] = 0.;
			Event->muinz[jj][kk] = 0.;
			Event->muoutx[jj][kk] = 0.;
			Event->muouty[jj][kk] = 0.;
			Event->muoutz[jj][kk] = 0.;
			Event->muinpx[jj][kk] = 0.;
			Event->muinpy[jj][kk] = 0.;
			Event->muinpz[jj][kk] = 0.;
			Event->muoutpx[jj][kk] = 0.;
			Event->muoutpy[jj][kk] = 0.;
			Event->muoutpz[jj][kk] = 0.;
			Event->mude[jj][kk] = 0.;
			Event->mual[jj][kk] = 0.;
			Event->mutim[jj][kk] = 0.;
		}
		//
		for (int kk = 0; kk < Event->muSigN[jj]; kk++) {
			Event->muSigTim[jj][kk] = 0.;
			Event->muSigAmp[jj][kk] = 0.;
			Event->muSigHei[jj][kk] = 0.;
			Event->muSigED[jj][kk] = 0.;
		}
		for (int kk = 0; kk < Event->muSigDigN[jj]; kk++) {
			Event->muSigTimDig[jj][kk] = 0;
			Event->muSigAmpDig[jj][kk] = 0;
			Event->muSigHeiDig[jj][kk] = 0;

		}
	}

	//TOF-Wall
	for (int kk = 0; kk < Event->twn; kk++) {
		Event->twplid[kk] = 0;
		Event->twplflg[kk] = 0;
		Event->twplinx[kk] = 0.;
		Event->twpliny[kk] = 0.;
		Event->twplinz[kk] = 0.;
		Event->twplinxTF[kk] = 0.;
		Event->twplinyTF[kk] = 0.;
		Event->twplinzTF[kk] = 0.;
		Event->twploutx[kk] = 0.;
		Event->twplouty[kk] = 0.;
		Event->twploutz[kk] = 0.;
		Event->twplinpx[kk] = 0.;
		Event->twplinpy[kk] = 0.;
		Event->twplinpz[kk] = 0.;
		Event->twploutpx[kk] = 0.;
		Event->twploutpy[kk] = 0.;
		Event->twploutpz[kk] = 0.;
		Event->twplde[kk] = 0.;
		Event->twplal[kk] = 0.;
		Event->twpltim[kk] = 0.;
		Event->twSlNbS[kk] = 0;
		//
		Event->twSlNbF[kk] = 0;
		Event->twSlNbB[kk] = 0;
		for (int jj = 0; jj < 4; jj++) {
			Event->twSlID[jj][kk] = 0;
			Event->twInX[jj][kk] = 0.;
			Event->twInY[jj][kk] = 0.;
			Event->twInZ[jj][kk] = 0.;
			Event->twOutX[jj][kk] = 0.;
			Event->twOutY[jj][kk] = 0.;
			Event->twOutZ[jj][kk] = 0.;
			Event->twInPX[jj][kk] = 0.;
			Event->twInPY[jj][kk] = 0.;
			Event->twInPZ[jj][kk] = 0.;
			Event->twOutPX[jj][kk] = 0.;
			Event->twOutPY[jj][kk] = 0.;
			Event->twOutPZ[jj][kk] = 0.;
			Event->twER[jj][kk] = 0.;
			Event->twQER[jj][kk] = 0.;
			Event->twTime[jj][kk] = 0.;
		}

	}
	for (int kk = 0; kk < TW_NbSlatFront; kk++) {
		Event->twSigAmpFT[kk] = 0.;
		Event->twSigAmpFB[kk] = 0.;
		Event->twSigTimFT[kk] = 0.;
		Event->twSigTimFB[kk] = 0.;
		Event->twSigEDF[kk] = 0.;

		Event->twSigAmpDigFT[kk] = 0;
		Event->twSigAmpDigFB[kk] = 0;
		Event->twSigTimDigFT[kk] = 0;
		Event->twSigTimDigFB[kk] = 0;
	}
	for (int kk = 0; kk < TW_NbSlatBack; kk++) {
		Event->twSigAmpRT[kk] = 0.;
		Event->twSigAmpRB[kk] = 0.;
		Event->twSigTimRT[kk] = 0.;
		Event->twSigTimRB[kk] = 0.;
		Event->twSigEDR[kk] = 0.;

		Event->twSigAmpDigRT[kk] = 0;
		Event->twSigAmpDigRB[kk] = 0;
		Event->twSigTimDigRT[kk] = 0;
		Event->twSigTimDigRB[kk] = 0;
	}

	//ONION
	for (int kk = 0; kk < Event->onn; kk++) {
		Event->onid[kk] = 0;
		Event->onreg[kk] = 0;
		Event->oninx[kk] = 0.;
		Event->oniny[kk] = 0.;
		Event->oninz[kk] = 0.;
		Event->onoutx[kk] = 0.;
		Event->onouty[kk] = 0.;
		Event->onoutz[kk] = 0.;
		Event->oninpx[kk] = 0.;
		Event->oninpy[kk] = 0.;
		Event->oninpz[kk] = 0.;
		Event->onoutpx[kk] = 0.;
		Event->onoutpy[kk] = 0.;
		Event->onoutpz[kk] = 0.;
		Event->onde[kk] = 0.;
		Event->onal[kk] = 0.;
		Event->ontim[kk] = 0.;
	}
	//ONION signal
	for (int kk = 0; kk < Event->onSigN; kk++) {
		Event->onSigReg[kk] = 0;
		Event->onSigAmp[kk] = 0.;
		Event->onSigTim[kk] = 0.;
		Event->onSigED[kk] = 0.;
	}
	for (int kk = 0; kk < Event->onSigDigN; kk++) {
		Event->onSigRegDig[kk] = 0;
		Event->onSigAmpDig[kk] = 0;
		Event->onSigTimDig[kk] = 0;
	}

	//set entry counter to zero
	Event->EventNumber = -1;
	Event->trn = 0;
	Event->stn = 0;
	Event->nmon = 0;
	Event->tgn = 0;
	Event->min = 0;
	Event->miSigN = 0;
	Event->ken = 0;
	Event->keSigN = 0;
	Event->keSigDigN = 0;


	for (Int_t i0 = 0; i0 < MU_PRO_Nb; i0++) {
		//    Event->mu.n[i0] = 0;
		//    Event->mu.SigN[i0] = 0;
		//    Event->mu.SigDigN[i0] = 0;

		Event->mun[i0] = 0;
		Event->muSigN[i0] = 0;
		Event->muSigDigN[i0] = 0;
	}

	Event->twn = 0;

	Event->onn = 0;
	Event->onSigN = 0;
	Event->onSigDigN = 0;

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

	/* KENTROS */
	RootTree->Branch("ken", &(Event->ken), "ken/I");
	RootTree->Branch("keid", &(Event->keid), "keid[ken]/I");
	RootTree->Branch("kereg", &(Event->kereg), "kereg[ken]/I");
	RootTree->Branch("keregtype", &(Event->keregtype), "keregtype[ken]/I");
	RootTree->Branch("keinx", &(Event->keinx), "keinx[ken]/D");
	RootTree->Branch("keiny", &(Event->keiny), "keiny[ken]/D");
	RootTree->Branch("keinz", &(Event->keinz), "keinz[ken]/D");
	RootTree->Branch("keinpx", &(Event->keinpx), "keinpx[ken]/D");
	RootTree->Branch("keinpy", &(Event->keinpy), "keinpy[ken]/D");
	RootTree->Branch("keinpz", &(Event->keinpz), "keinpz[ken]/D");
	RootTree->Branch("keoutx", &(Event->keoutx), "keoutx[ken]/D");
	RootTree->Branch("keouty", &(Event->keouty), "keouty[ken]/D");
	RootTree->Branch("keoutz", &(Event->keoutz), "keoutz[ken]/D");
	RootTree->Branch("keoutpx", &(Event->keoutpx), "keoutpx[ken]/D");
	RootTree->Branch("keoutpy", &(Event->keoutpy), "keoutpy[ken]/D");
	RootTree->Branch("keoutpz", &(Event->keoutpz), "keoutpz[ken]/D");
	RootTree->Branch("kede", &(Event->kede), "kede[ken]/D");
	RootTree->Branch("keal", &(Event->keal), "keal[ken]/D");
	RootTree->Branch("ketim", &(Event->ketim), "ketim[ken]/D");
	//signal
	RootTree->Branch("keSigN", &(Event->keSigN), "keSigN/I");
	RootTree->Branch("keSigReg", &(Event->keSigReg), "keSigReg[keSigN]/I");
	RootTree->Branch("keSigAmp", &(Event->keSigAmp), "keSigAmp[keSigN]/D");
	RootTree->Branch("keSigTim", &(Event->keSigTim), "keSigTim[keSigN]/D");
	RootTree->Branch("keSigED", &(Event->keSigED), "keSigED[keSigN]/D");
	RootTree->Branch("keSigX", &(Event->keSigX), "keSigX[keSigN]/D");
	RootTree->Branch("keSigY", &(Event->keSigY), "keSigY[keSigN]/D");
	RootTree->Branch("keSigZ", &(Event->keSigZ), "keSigZ[keSigN]/D");
	RootTree->Branch("keSigpX", &(Event->keSigpX), "keSigpX[keSigN]/D");
	RootTree->Branch("keSigpY", &(Event->keSigpY), "keSigpY[keSigN]/D");
	RootTree->Branch("keSigpZ", &(Event->keSigpZ), "keSigpZ[keSigN]/D");
	//digitised signal
	RootTree->Branch("keSigDigN", &(Event->keSigDigN), "keSigDigN/I");
	RootTree->Branch("keSigRegDig", &(Event->keSigRegDig),
			"keSigRegDig[keSigDigN]/I");
	RootTree->Branch("keSigAmpDig", &(Event->keSigAmpDig),
			"keSigAmpDig[keSigDigN]/I");
	RootTree->Branch("keSigTimDig", &(Event->keSigTimDig),
			"keSigTimDig[keSigDigN]/I");

	//    TBranch *MyBranchA = RootTree->Branch("mu","MU_STRUCT",&Event->mu);
	//  MyBranchA->SetFile("TESTFile.root");

	//  Int_t split = 0;
	//  Int_t bsize = 64000;

	//  MU_STRUCT vmu= Event->mu;
	//  MU_STRUCT* pmu= 0;
	//  pmu= &vmu;

	//  cout<<"DEBUG: register MUSIC"<<endl;

	/* TP-MUSIC IV CLASS */
	//  RootTree->Branch("mu","MU_STRUCT",&pmu);//, bsize, split
	//  cout<<"DEBUG: register MUSIC OK!"<<endl;


	/* TP-MUSIC IV */
	for (Int_t i0 = 0; i0 < MU_PRO_Nb; i0++) {
		RootTree->Branch(TString::Format("mun%d", i0), &(Event->mun[i0]),
				TString::Format("mun%d/I", i0));
		RootTree->Branch(TString::Format("muid%d", i0), &(Event->muid[i0]),
				TString::Format("muid[mun%d]/I", i0));
		RootTree->Branch(TString::Format("muinx%d", i0), &(Event->muinx[i0]),
				TString::Format("muinx[mun%d]/D", i0));
		RootTree->Branch(TString::Format("muiny%d", i0), &(Event->muiny[i0]),
				TString::Format("muiny[mun%d]/D", i0));
		RootTree->Branch(TString::Format("muinz%d", i0), &(Event->muinz[i0]),
				TString::Format("muinz[mun%d]/D", i0));
		RootTree->Branch(TString::Format("muinpx%d", i0), &(Event->muinpx[i0]),
				TString::Format("muinpx[mun%d]/D", i0));
		RootTree->Branch(TString::Format("muinpy%d", i0), &(Event->muinpy[i0]),
				TString::Format("muinpy[mun%d]/D", i0));
		RootTree->Branch(TString::Format("muinpz%d", i0), &(Event->muinpz[i0]),
				TString::Format("muinpz[mun%d]/D", i0));
		RootTree->Branch(TString::Format("muoutx%d", i0), &(Event->muoutx[i0]),
				TString::Format("muoutx[mun%d]/D", i0));
		RootTree->Branch(TString::Format("muouty%d", i0), &(Event->muouty[i0]),
				TString::Format("muouty[mun%d]/D", i0));
		RootTree->Branch(TString::Format("muoutz%d", i0), &(Event->muoutz[i0]),
				TString::Format("muoutz[mun%d]/D", i0));
		RootTree->Branch(TString::Format("muoutpx%d", i0),
				&(Event->muoutpx[i0]), TString::Format("muoutpx[mun%d]/D", i0));
		RootTree->Branch(TString::Format("muoutpy%d", i0),
				&(Event->muoutpy[i0]), TString::Format("muoutpy[mun%d]/D", i0));
		RootTree->Branch(TString::Format("muoutpz%d", i0),
				&(Event->muoutpz[i0]), TString::Format("muoutpz[mun%d]/D", i0));
		RootTree->Branch(TString::Format("mude%d", i0), &(Event->mude[i0]),
				TString::Format("mude[mun%d]/D", i0));
		RootTree->Branch(TString::Format("mual%d", i0), &(Event->mual[i0]),
				TString::Format("mual[mun%d]/D", i0));
		RootTree->Branch(TString::Format("mutim%d", i0), &(Event->mutim[i0]),
				TString::Format("mutim[mun%d]/D", i0));
		//
		RootTree->Branch(TString::Format("muSigN%d", i0), &(Event->muSigN[i0]),
				TString::Format("muSigN%d/I", i0));
		RootTree->Branch(TString::Format("muSigTim%d", i0),
				&(Event->muSigTim[i0]),
				TString::Format("muSigTim[muSigN%d]/D", i0));
		RootTree->Branch(TString::Format("muSigAmp%d", i0),
				&(Event->muSigAmp[i0]),
				TString::Format("muSigAmp[muSigN%d]/D", i0));
		RootTree->Branch(TString::Format("muSigHei%d", i0),
				&(Event->muSigHei[i0]),
				TString::Format("muSigHei[muSigN%d]/D", i0));
		RootTree->Branch(TString::Format("muSigED%d", i0),
				&(Event->muSigED[i0]),
				TString::Format("muSigED[muSigN%d]/D", i0));

		RootTree->Branch(TString::Format("muSigDigN%d", i0),
				&(Event->muSigDigN[i0]), TString::Format("muSigDigN%d/I", i0));
		RootTree->Branch(TString::Format("muSigTimDig%d", i0),
				&(Event->muSigTimDig[i0]),
				TString::Format("muSigTimDig[muSigDigN%d]/I", i0));
		RootTree->Branch(TString::Format("muSigAmpDig%d", i0),
				&(Event->muSigAmpDig[i0]),
				TString::Format("muSigAmpDig[muSigDigN%d]/I", i0));
		RootTree->Branch(TString::Format("muSigHeiDig%d", i0),
				&(Event->muSigHeiDig[i0]),
				TString::Format("muSigHeiDig[muSigDigN%d]/I", i0));
	}

	//  RootTree->Branch(TString::Format("muid"),&(Event->muid),TString::Format("muid[8][mun0]/I"));


	/*  TOFW detector  */
	RootTree->Branch("twn", &(Event->twn), "twn/I");
	RootTree->Branch("twplid", &(Event->twplid), "twplid[twn]/I");
	RootTree->Branch("twplflg", &(Event->twplflg), "twplflg[twn]/I");
	RootTree->Branch("twplinx", &(Event->twplinx), "twplinx[twn]/D");
	RootTree->Branch("twpliny", &(Event->twpliny), "twpliny[twn]/D");
	RootTree->Branch("twplinz", &(Event->twplinz), "twplinz[twn]/D");
	RootTree->Branch("twplinxTF", &(Event->twplinxTF), "twplinxTF[twn]/D");
	RootTree->Branch("twplinyTF", &(Event->twplinyTF), "twplinyTF[twn]/D");
	RootTree->Branch("twplinzTF", &(Event->twplinzTF), "twplinzTF[twn]/D");
	RootTree->Branch("twplinpx", &(Event->twplinpx), "twplinpx[twn]/D");
	RootTree->Branch("twplinpy", &(Event->twplinpy), "twplinpy[twn]/D");
	RootTree->Branch("twplinpz", &(Event->twplinpz), "twplinpz[twn]/D");
	RootTree->Branch("twploutx", &(Event->twploutx), "twploutx[twn]/D");
	RootTree->Branch("twplouty", &(Event->twplouty), "twplouty[twn]/D");
	RootTree->Branch("twploutz", &(Event->twploutz), "twploutz[twn]/D");
	RootTree->Branch("twploutpx", &(Event->twploutpx), "twploutpx[twn]/D");
	RootTree->Branch("twploutpy", &(Event->twploutpy), "twploutpy[twn]/D");
	RootTree->Branch("twploutpz", &(Event->twploutpz), "twploutpz[twn]/D");
	RootTree->Branch("twplde", &(Event->twplde), "twplde[twn]/D");
	RootTree->Branch("twplal", &(Event->twplal), "twplal[twn]/D");
	RootTree->Branch("twpltim", &(Event->twpltim), "twpltim[twn]/D");
	RootTree->Branch("twSlNbS", &(Event->twSlNbS), "twSlNbS[twn]/I");
	//
	RootTree->Branch("twSlNbF", &(Event->twSlNbF), "twSlNbF[twn]/I");
	RootTree->Branch("twSlNbB", &(Event->twSlNbB), "twSlNbB[twn]/I");
	RootTree->Branch("twSlID1", &(Event->twSlID[0]), "twSlID1[twn]/I");
	RootTree->Branch("twSlID2", &(Event->twSlID[1]), "twSlID2[twn]/I");
	RootTree->Branch("twSlID3", &(Event->twSlID[2]), "twSlID3[twn]/I");
	RootTree->Branch("twSlID4", &(Event->twSlID[3]), "twSlID4[twn]/I");

	RootTree->Branch("twInX1", &(Event->twInX[0]), "twInX1[twn]/D");
	RootTree->Branch("twInX2", &(Event->twInX[1]), "twInX2[twn]/D");
	RootTree->Branch("twInX3", &(Event->twInX[2]), "twInX3[twn]/D");
	RootTree->Branch("twInX4", &(Event->twInX[3]), "twInX4[twn]/D");

	RootTree->Branch("twInY1", &(Event->twInY[0]), "twInY1[twn]/D");
	RootTree->Branch("twInY2", &(Event->twInY[1]), "twInY2[twn]/D");
	RootTree->Branch("twInY3", &(Event->twInY[2]), "twInY3[twn]/D");
	RootTree->Branch("twInY4", &(Event->twInY[3]), "twInY4[twn]/D");

	RootTree->Branch("twInZ1", &(Event->twInZ[0]), "twInZ1[twn]/D");
	RootTree->Branch("twInZ2", &(Event->twInZ[1]), "twInZ2[twn]/D");
	RootTree->Branch("twInZ3", &(Event->twInZ[2]), "twInZ3[twn]/D");
	RootTree->Branch("twInZ4", &(Event->twInZ[3]), "twInZ4[twn]/D");

	RootTree->Branch("twOutX1", &(Event->twOutX[0]), "twOutX1[twn]/D");
	RootTree->Branch("twOutX2", &(Event->twOutX[1]), "twOutX2[twn]/D");
	RootTree->Branch("twOutX3", &(Event->twOutX[2]), "twOutX3[twn]/D");
	RootTree->Branch("twOutX4", &(Event->twOutX[3]), "twOutX4[twn]/D");

	RootTree->Branch("twOutY1", &(Event->twOutY[0]), "twOutY1[twn]/D");
	RootTree->Branch("twOutY2", &(Event->twOutY[1]), "twOutY2[twn]/D");
	RootTree->Branch("twOutY3", &(Event->twOutY[2]), "twOutY3[twn]/D");
	RootTree->Branch("twOutY4", &(Event->twOutY[3]), "twOutY4[twn]/D");

	RootTree->Branch("twOutZ1", &(Event->twOutZ[0]), "twOutZ1[twn]/D");
	RootTree->Branch("twOutZ2", &(Event->twOutZ[1]), "twOutZ2[twn]/D");
	RootTree->Branch("twOutZ3", &(Event->twOutZ[2]), "twOutZ3[twn]/D");
	RootTree->Branch("twOutZ4", &(Event->twOutZ[3]), "twOutZ4[twn]/D");

	RootTree->Branch("twInPX1", &(Event->twInPX[0]), "twInPX1[twn]/D");
	RootTree->Branch("twInPX2", &(Event->twInPX[1]), "twInPX2[twn]/D");
	RootTree->Branch("twInPX3", &(Event->twInPX[2]), "twInPX3[twn]/D");
	RootTree->Branch("twInPX4", &(Event->twInPX[3]), "twInPX4[twn]/D");

	RootTree->Branch("twInPY1", &(Event->twInPY[0]), "twInPY1[twn]/D");
	RootTree->Branch("twInPY2", &(Event->twInPY[1]), "twInPY2[twn]/D");
	RootTree->Branch("twInPY3", &(Event->twInPY[2]), "twInPY3[twn]/D");
	RootTree->Branch("twInPY4", &(Event->twInPY[3]), "twInPY4[twn]/D");

	RootTree->Branch("twInPZ1", &(Event->twInPZ[0]), "twInPZ1[twn]/D");
	RootTree->Branch("twInPZ2", &(Event->twInPZ[1]), "twInPZ2[twn]/D");
	RootTree->Branch("twInPZ3", &(Event->twInPZ[2]), "twInPZ3[twn]/D");
	RootTree->Branch("twInPZ4", &(Event->twInPZ[3]), "twInPZ4[twn]/D");

	RootTree->Branch("twOutPX1", &(Event->twOutPX[0]), "twOutPX1[twn]/D");
	RootTree->Branch("twOutPX2", &(Event->twOutPX[1]), "twOutPX2[twn]/D");
	RootTree->Branch("twOutPX3", &(Event->twOutPX[2]), "twOutPX3[twn]/D");
	RootTree->Branch("twOutPX4", &(Event->twOutPX[3]), "twOutPX4[twn]/D");

	RootTree->Branch("twOutPY1", &(Event->twOutPY[0]), "twOutPY1[twn]/D");
	RootTree->Branch("twOutPY2", &(Event->twOutPY[1]), "twOutPY2[twn]/D");
	RootTree->Branch("twOutPY3", &(Event->twOutPY[2]), "twOutPY3[twn]/D");
	RootTree->Branch("twOutPY4", &(Event->twOutPY[3]), "twOutPY4[twn]/D");

	RootTree->Branch("twOutPZ1", &(Event->twOutPZ[0]), "twOutPZ1[twn]/D");
	RootTree->Branch("twOutPZ2", &(Event->twOutPZ[1]), "twOutPZ2[twn]/D");
	RootTree->Branch("twOutPZ3", &(Event->twOutPZ[2]), "twOutPZ3[twn]/D");
	RootTree->Branch("twOutPZ4", &(Event->twOutPZ[3]), "twOutPZ4[twn]/D");

	RootTree->Branch("twER1", &(Event->twER[0]), "twER1[twn]/D");
	RootTree->Branch("twER2", &(Event->twER[1]), "twER2[twn]/D");
	RootTree->Branch("twER3", &(Event->twER[2]), "twER3[twn]/D");
	RootTree->Branch("twER4", &(Event->twER[3]), "twER4[twn]/D");

	RootTree->Branch("twQER1", &(Event->twQER[0]), "twQER1[twn]/D");
	RootTree->Branch("twQER2", &(Event->twQER[1]), "twQER2[twn]/D");
	RootTree->Branch("twQER3", &(Event->twQER[2]), "twQER3[twn]/D");
	RootTree->Branch("twQER4", &(Event->twQER[3]), "twQER4[twn]/D");

	RootTree->Branch("twTime1", &(Event->twTime[0]), "twTime1[twn]/D");
	RootTree->Branch("twTime2", &(Event->twTime[1]), "twTime2[twn]/D");
	RootTree->Branch("twTime3", &(Event->twTime[2]), "twTime3[twn]/D");
	RootTree->Branch("twTime4", &(Event->twTime[3]), "twTime4[twn]/D");

	RootTree->Branch("twSigNF", &(Event->twSigNF), "twSigNF/I");
	RootTree->Branch("twSigNR", &(Event->twSigNR), "twSigNR/I");
	RootTree->Branch("twSigIDF", &(Event->twSigIDF), "twSigIDF[twSigNF]/I");
	RootTree->Branch("twSigIDR", &(Event->twSigIDR), "twSigIDB[twSigNR]/I");
	//signal of TOF-Wall
	//  RootTree->Branch("twSigTimFT" ,&(Event->twSigTimFT ,Form("twSigTimFT[%i]/D",TW_NbSlatFront) );
	RootTree->Branch("twSigTimFT", &(Event->twSigTimFT),
			"twSigTimFT[twSigNF]/D");
	RootTree->Branch("twSigTimFB", &(Event->twSigTimFB),
			"twSigTimFB[twSigNF]/D");
	RootTree->Branch("twSigAmpFT", &(Event->twSigAmpFT),
			"twSigAmpFT[twSigNF]/D");
	RootTree->Branch("twSigAmpFB", &(Event->twSigAmpFB),
			"twSigAmpFB[twSigNF]/D");
	RootTree->Branch("twSigEDF", &(Event->twSigEDF), "twSigEDF[twSigNF]/D");

	RootTree->Branch("twSigTimRT", &(Event->twSigTimRT),
			"twSigTimRT[twSigNR]/D");
	RootTree->Branch("twSigTimRB", &(Event->twSigTimRB),
			"twSigTimRB[twSigNR]/D");
	RootTree->Branch("twSigAmpRT", &(Event->twSigAmpRT),
			"twSigAmpRT[twSigNR]/D");
	RootTree->Branch("twSigAmpRB", &(Event->twSigAmpRB),
			"twSigAmpRB[twSigNR]/D");
	RootTree->Branch("twSigEDR", &(Event->twSigEDR), "twSigEDR[twSigNR]/D");

	//digitized signal of TOF-Wall
	RootTree->Branch("twSigTimDigFT", &(Event->twSigTimDigFT),
			"twSigTimDigFT[twSigNF]/I");
	RootTree->Branch("twSigTimDigFB", &(Event->twSigTimDigFB),
			"twSigTimDigFB[twSigNF]/I");
	RootTree->Branch("twSigAmpDigFT", &(Event->twSigAmpDigFT),
			"twSigAmpDigFT[twSigNF]/I");
	RootTree->Branch("twSigAmpDigFB", &(Event->twSigAmpDigFB),
			"twSigAmpDigFB[twSigNF]/I");

	RootTree->Branch("twSigTimDigRT", &(Event->twSigTimDigRT),
			"twSigTimDigRT[twSigNR]/I");
	RootTree->Branch("twSigTimDigRB", &(Event->twSigTimDigRB),
			"twSigTimDigRB[twSigNR]/I");
	RootTree->Branch("twSigAmpDigRT", &(Event->twSigAmpDigRT),
			"twSigAmpDigRT[twSigNR]/I");
	RootTree->Branch("twSigAmpDigRB", &(Event->twSigAmpDigRB),
			"twSigAmpDigRB[twSigNR]/I");

	/* ONION */
	RootTree->Branch("onn", &(Event->onn), "onn/I");
	RootTree->Branch("onid", &(Event->onid), "onid[onn]/I");
	RootTree->Branch("onreg", &(Event->onreg), "onreg[onn]/I");
	RootTree->Branch("oninx", &(Event->oninx), "oninx[onn]/D");
	RootTree->Branch("oniny", &(Event->oniny), "oniny[onn]/D");
	RootTree->Branch("oninz", &(Event->oninz), "oninz[onn]/D");
	RootTree->Branch("oninpx", &(Event->oninpx), "oninpx[onn]/D");
	RootTree->Branch("oninpy", &(Event->oninpy), "oninpy[onn]/D");
	RootTree->Branch("oninpz", &(Event->oninpz), "oninpz[onn]/D");
	RootTree->Branch("onoutx", &(Event->onoutx), "onoutx[onn]/D");
	RootTree->Branch("onouty", &(Event->onouty), "onouty[onn]/D");
	RootTree->Branch("onoutz", &(Event->onoutz), "onoutz[onn]/D");
	RootTree->Branch("onoutpx", &(Event->onoutpx), "onoutpx[onn]/D");
	RootTree->Branch("onoutpy", &(Event->onoutpy), "onoutpy[onn]/D");
	RootTree->Branch("onoutpz", &(Event->onoutpz), "onoutpz[onn]/D");
	RootTree->Branch("onde", &(Event->onde), "onde[onn]/D");
	RootTree->Branch("onal", &(Event->onal), "onal[onn]/D");
	RootTree->Branch("ontim", &(Event->ontim), "ontim[onn]/D");
	//signal
	RootTree->Branch("onSigN", &(Event->onSigN), "onSigN/I");
	RootTree->Branch("onSigReg", &(Event->onSigReg), "onSigReg[onSigN]/I");
	RootTree->Branch("onSigAmp", &(Event->onSigAmp), "onSigAmp[onSigN]/D");
	RootTree->Branch("onSigTim", &(Event->onSigTim), "onSigTim[onSigN]/D");
	RootTree->Branch("onSigED", &(Event->onSigED), "onSigED[onSigN]/D");
	//digitised signal
	RootTree->Branch("onSigDigN", &(Event->onSigDigN), "onSigDigN/I");
	RootTree->Branch("onSigRegDig", &(Event->onSigRegDig),
			"onSigRegDig[onSigDigN]/I");
	RootTree->Branch("onSigAmpDig", &(Event->onSigAmpDig),
			"onSigAmpDig[onSigDigN]/I");
	RootTree->Branch("onSigTimDig", &(Event->onSigTimDig),
			"onSigTimDig[onSigDigN]/I");

	// printf("Sono in Evento::InitRootTree(): ho scritto i branches \n");
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

  /* KENTROS */
  RootTree->SetBranchAddress("ken", &(Event->ken));
  RootTree->SetBranchAddress("keid", &(Event->keid));
  RootTree->SetBranchAddress("kereg", &(Event->kereg));
  RootTree->SetBranchAddress("keregtype", &(Event->keregtype));
  RootTree->SetBranchAddress("keinx", &(Event->keinx));
  RootTree->SetBranchAddress("keiny", &(Event->keiny));
  RootTree->SetBranchAddress("keinz", &(Event->keinz));
  RootTree->SetBranchAddress("keinpx", &(Event->keinpx));
  RootTree->SetBranchAddress("keinpy", &(Event->keinpy));
  RootTree->SetBranchAddress("keinpz", &(Event->keinpz));
  RootTree->SetBranchAddress("keoutx", &(Event->keoutx));
  RootTree->SetBranchAddress("keouty", &(Event->keouty));
  RootTree->SetBranchAddress("keoutz", &(Event->keoutz));
  RootTree->SetBranchAddress("keoutpx", &(Event->keoutpx));
  RootTree->SetBranchAddress("keoutpy", &(Event->keoutpy));
  RootTree->SetBranchAddress("keoutpz", &(Event->keoutpz));
  RootTree->SetBranchAddress("kede", &(Event->kede));
  RootTree->SetBranchAddress("keal", &(Event->keal));
  RootTree->SetBranchAddress("ketim", &(Event->ketim));
  //signal
  RootTree->SetBranchAddress("keSigN", &(Event->keSigN));
  RootTree->SetBranchAddress("keSigReg", &(Event->keSigReg));
  RootTree->SetBranchAddress("keSigAmp", &(Event->keSigAmp));
  RootTree->SetBranchAddress("keSigTim", &(Event->keSigTim));
  RootTree->SetBranchAddress("keSigED", &(Event->keSigED));
  RootTree->SetBranchAddress("keSigX", &(Event->keSigX));
  RootTree->SetBranchAddress("keSigY", &(Event->keSigY));
  RootTree->SetBranchAddress("keSigZ", &(Event->keSigZ));
  RootTree->SetBranchAddress("keSigpX", &(Event->keSigpX));
  RootTree->SetBranchAddress("keSigpY", &(Event->keSigpY));
  RootTree->SetBranchAddress("keSigpZ", &(Event->keSigpZ));
  //digitised signal
  RootTree->SetBranchAddress("keSigDigN", &(Event->keSigDigN));
  RootTree->SetBranchAddress("keSigRegDig", &(Event->keSigRegDig));
  RootTree->SetBranchAddress("keSigAmpDig", &(Event->keSigAmpDig));
  RootTree->SetBranchAddress("keSigTimDig", &(Event->keSigTimDig));

  //  cout<<"DEBUG: read MUSIC CLASS ...";
  /* TP-MUSIC IV CLASS */
  //  RootTree->SetBranchAddress("mu",&(Event->mu));
  //  cout<<"NOT IMPLEMENTED DONE"<<endl;

  /* TP-MUSIC IV */
  for (Int_t i0 = 0; i0 < MU_PRO_Nb; i0++) {
    RootTree->SetBranchAddress(TString::Format("mun%d", i0),
			       &(Event->mun[i0]));
    RootTree->SetBranchAddress(TString::Format("muid%d", i0),
			       &(Event->muid[i0]));
    RootTree->SetBranchAddress(TString::Format("muinx%d", i0),
			       &(Event->muinx[i0]));
    RootTree->SetBranchAddress(TString::Format("muiny%d", i0),
			       &(Event->muiny[i0]));
    RootTree->SetBranchAddress(TString::Format("muinz%d", i0),
			       &(Event->muinz[i0]));
    RootTree->SetBranchAddress(TString::Format("muinpx%d", i0),
			       &(Event->muinpx[i0]));
    RootTree->SetBranchAddress(TString::Format("muinpy%d", i0),
			       &(Event->muinpy[i0]));
    RootTree->SetBranchAddress(TString::Format("muinpz%d", i0),
			       &(Event->muinpz[i0]));
    RootTree->SetBranchAddress(TString::Format("muoutx%d", i0),
			       &(Event->muoutx[i0]));
    RootTree->SetBranchAddress(TString::Format("muouty%d", i0),
			       &(Event->muouty[i0]));
    RootTree->SetBranchAddress(TString::Format("muoutz%d", i0),
			       &(Event->muoutz[i0]));
    RootTree->SetBranchAddress(TString::Format("muoutpx%d", i0),
			       &(Event->muoutpx[i0]));
    RootTree->SetBranchAddress(TString::Format("muoutpy%d", i0),
			       &(Event->muoutpy[i0]));
    RootTree->SetBranchAddress(TString::Format("muoutpz%d", i0),
			       &(Event->muoutpz[i0]));
    RootTree->SetBranchAddress(TString::Format("mude%d", i0),
			       &(Event->mude[i0]));
    RootTree->SetBranchAddress(TString::Format("mual%d", i0),
			       &(Event->mual[i0]));
    RootTree->SetBranchAddress(TString::Format("mutim%d", i0),
			       &(Event->mutim[i0]));
    //
    RootTree->SetBranchAddress(TString::Format("muSigN%d", i0),
			       &(Event->muSigN[i0]));
    RootTree->SetBranchAddress(TString::Format("muSigTim%d", i0),
			       &(Event->muSigTim[i0]));
    RootTree->SetBranchAddress(TString::Format("muSigAmp%d", i0),
			       &(Event->muSigAmp[i0]));
    RootTree->SetBranchAddress(TString::Format("muSigHei%d", i0),
			       &(Event->muSigHei[i0]));
    RootTree->SetBranchAddress(TString::Format("muSigED%d", i0),
			       &(Event->muSigED[i0]));
    //
    RootTree->SetBranchAddress(TString::Format("muSigDigN%d", i0),
			       &(Event->muSigDigN[i0]));
    RootTree->SetBranchAddress(TString::Format("muSigTimDig%d", i0),
			       &(Event->muSigTimDig[i0]));
    RootTree->SetBranchAddress(TString::Format("muSigAmpDig%d", i0),
			       &(Event->muSigAmpDig[i0]));
    RootTree->SetBranchAddress(TString::Format("muSigHeiDig%d", i0),
			       &(Event->muSigHeiDig[i0]));
  }

  /*  TOFW detector  */
  RootTree->SetBranchAddress("twn", &(Event->twn));
  RootTree->SetBranchAddress("twplid", &(Event->twplid));
  RootTree->SetBranchAddress("twplflg", &(Event->twplflg));
  RootTree->SetBranchAddress("twplinx", &(Event->twplinx));
  RootTree->SetBranchAddress("twpliny", &(Event->twpliny));
  RootTree->SetBranchAddress("twplinz", &(Event->twplinz));
  RootTree->SetBranchAddress("twplinxTF", &(Event->twplinxTF));
  RootTree->SetBranchAddress("twplinyTF", &(Event->twplinyTF));
  RootTree->SetBranchAddress("twplinzTF", &(Event->twplinzTF));
  RootTree->SetBranchAddress("twplinpx", &(Event->twplinpx));
  RootTree->SetBranchAddress("twplinpy", &(Event->twplinpy));
  RootTree->SetBranchAddress("twplinpz", &(Event->twplinpz));
  RootTree->SetBranchAddress("twploutx", &(Event->twploutx));
  RootTree->SetBranchAddress("twplouty", &(Event->twplouty));
  RootTree->SetBranchAddress("twploutz", &(Event->twploutz));
  RootTree->SetBranchAddress("twploutpx", &(Event->twploutpx));
  RootTree->SetBranchAddress("twploutpy", &(Event->twploutpy));
  RootTree->SetBranchAddress("twploutpz", &(Event->twploutpz));
  RootTree->SetBranchAddress("twplde", &(Event->twplde));
  RootTree->SetBranchAddress("twplal", &(Event->twplal));
  RootTree->SetBranchAddress("twpltim", &(Event->twpltim));
  RootTree->SetBranchAddress("twSlNbS", &(Event->twSlNbS));
  //
  RootTree->SetBranchAddress("twSlNbF", &(Event->twSlNbF));
  RootTree->SetBranchAddress("twSlNbB", &(Event->twSlNbB));
  RootTree->SetBranchAddress("twSlID1", &(Event->twSlID[0]));
  RootTree->SetBranchAddress("twSlID2", &(Event->twSlID[1]));
  RootTree->SetBranchAddress("twSlID3", &(Event->twSlID[2]));
  RootTree->SetBranchAddress("twSlID4", &(Event->twSlID[3]));

  RootTree->SetBranchAddress("twInX1", &(Event->twInX[0]));
  RootTree->SetBranchAddress("twInX2", &(Event->twInX[1]));
  RootTree->SetBranchAddress("twInX3", &(Event->twInX[2]));
  RootTree->SetBranchAddress("twInX4", &(Event->twInX[3]));

  RootTree->SetBranchAddress("twInY1", &(Event->twInY[0]));
  RootTree->SetBranchAddress("twInY2", &(Event->twInY[1]));
  RootTree->SetBranchAddress("twInY3", &(Event->twInY[2]));
  RootTree->SetBranchAddress("twInY4", &(Event->twInY[3]));

  RootTree->SetBranchAddress("twInZ1", &(Event->twInZ[0]));
  RootTree->SetBranchAddress("twInZ2", &(Event->twInZ[1]));
  RootTree->SetBranchAddress("twInZ3", &(Event->twInZ[2]));
  RootTree->SetBranchAddress("twInZ4", &(Event->twInZ[3]));

  RootTree->SetBranchAddress("twOutX1", &(Event->twOutX[0]));
  RootTree->SetBranchAddress("twOutX2", &(Event->twOutX[1]));
  RootTree->SetBranchAddress("twOutX3", &(Event->twOutX[2]));
  RootTree->SetBranchAddress("twOutX4", &(Event->twOutX[3]));

  RootTree->SetBranchAddress("twOutY1", &(Event->twOutY[0]));
  RootTree->SetBranchAddress("twOutY2", &(Event->twOutY[1]));
  RootTree->SetBranchAddress("twOutY3", &(Event->twOutY[2]));
  RootTree->SetBranchAddress("twOutY4", &(Event->twOutY[3]));

  RootTree->SetBranchAddress("twOutZ1", &(Event->twOutZ[0]));
  RootTree->SetBranchAddress("twOutZ2", &(Event->twOutZ[1]));
  RootTree->SetBranchAddress("twOutZ3", &(Event->twOutZ[2]));
  RootTree->SetBranchAddress("twOutZ4", &(Event->twOutZ[3]));

  RootTree->SetBranchAddress("twInPX1", &(Event->twInPX[0]));
  RootTree->SetBranchAddress("twInPX2", &(Event->twInPX[1]));
  RootTree->SetBranchAddress("twInPX3", &(Event->twInPX[2]));
  RootTree->SetBranchAddress("twInPX4", &(Event->twInPX[3]));

  RootTree->SetBranchAddress("twInPY1", &(Event->twInPY[0]));
  RootTree->SetBranchAddress("twInPY2", &(Event->twInPY[1]));
  RootTree->SetBranchAddress("twInPY3", &(Event->twInPY[2]));
  RootTree->SetBranchAddress("twInPY4", &(Event->twInPY[3]));

  RootTree->SetBranchAddress("twInPZ1", &(Event->twInPZ[0]));
  RootTree->SetBranchAddress("twInPZ2", &(Event->twInPZ[1]));
  RootTree->SetBranchAddress("twInPZ3", &(Event->twInPZ[2]));
  RootTree->SetBranchAddress("twInPZ4", &(Event->twInPZ[3]));

  RootTree->SetBranchAddress("twOutPX1", &(Event->twOutPX[0]));
  RootTree->SetBranchAddress("twOutPX2", &(Event->twOutPX[1]));
  RootTree->SetBranchAddress("twOutPX3", &(Event->twOutPX[2]));
  RootTree->SetBranchAddress("twOutPX4", &(Event->twOutPX[3]));

  RootTree->SetBranchAddress("twOutPY1", &(Event->twOutPY[0]));
  RootTree->SetBranchAddress("twOutPY2", &(Event->twOutPY[1]));
  RootTree->SetBranchAddress("twOutPY3", &(Event->twOutPY[2]));
  RootTree->SetBranchAddress("twOutPY4", &(Event->twOutPY[3]));

  RootTree->SetBranchAddress("twOutPZ1", &(Event->twOutPZ[0]));
  RootTree->SetBranchAddress("twOutPZ2", &(Event->twOutPZ[1]));
  RootTree->SetBranchAddress("twOutPZ3", &(Event->twOutPZ[2]));
  RootTree->SetBranchAddress("twOutPZ4", &(Event->twOutPZ[3]));

  RootTree->SetBranchAddress("twER1", &(Event->twER[0]));
  RootTree->SetBranchAddress("twER2", &(Event->twER[1]));
  RootTree->SetBranchAddress("twER3", &(Event->twER[2]));
  RootTree->SetBranchAddress("twER4", &(Event->twER[3]));

  RootTree->SetBranchAddress("twQER1", &(Event->twQER[0]));
  RootTree->SetBranchAddress("twQER2", &(Event->twQER[1]));
  RootTree->SetBranchAddress("twQER3", &(Event->twQER[2]));
  RootTree->SetBranchAddress("twQER4", &(Event->twQER[3]));

  RootTree->SetBranchAddress("twTime1", &(Event->twTime[0]));
  RootTree->SetBranchAddress("twTime2", &(Event->twTime[1]));
  RootTree->SetBranchAddress("twTime3", &(Event->twTime[2]));
  RootTree->SetBranchAddress("twTime4", &(Event->twTime[3]));

  RootTree->SetBranchAddress("twSigNF", &(Event->twSigNF));
  RootTree->SetBranchAddress("twSigNR", &(Event->twSigNR));
  RootTree->SetBranchAddress("twSigIDF", &(Event->twSigIDF));
  RootTree->SetBranchAddress("twSigIDR", &(Event->twSigIDR));
  //signal of TOF-Wall
  RootTree->SetBranchAddress("twSigTimFT", &(Event->twSigTimFT));
  RootTree->SetBranchAddress("twSigTimFB", &(Event->twSigTimFB));
  RootTree->SetBranchAddress("twSigAmpFT", &(Event->twSigAmpFT));
  RootTree->SetBranchAddress("twSigAmpFB", &(Event->twSigAmpFB));
  RootTree->SetBranchAddress("twSigEDF", &(Event->twSigEDF));

  RootTree->SetBranchAddress("twSigTimRT", &(Event->twSigTimRT));
  RootTree->SetBranchAddress("twSigTimRB", &(Event->twSigTimRB));
  RootTree->SetBranchAddress("twSigAmpRT", &(Event->twSigAmpRT));
  RootTree->SetBranchAddress("twSigAmpRB", &(Event->twSigAmpRB));
  RootTree->SetBranchAddress("twSigEDR", &(Event->twSigEDR));

  //digitized signal of TOF-Wall
  RootTree->SetBranchAddress("twSigTimDigFT", &(Event->twSigTimDigFT));
  RootTree->SetBranchAddress("twSigTimDigFB", &(Event->twSigTimDigFB));
  RootTree->SetBranchAddress("twSigAmpDigFT", &(Event->twSigAmpDigFT));
  RootTree->SetBranchAddress("twSigAmpDigFB", &(Event->twSigAmpDigFB));

  RootTree->SetBranchAddress("twSigTimDigRT", &(Event->twSigTimDigRT));
  RootTree->SetBranchAddress("twSigTimDigRB", &(Event->twSigTimDigRB));
  RootTree->SetBranchAddress("twSigAmpDigRT", &(Event->twSigAmpDigRT));
  RootTree->SetBranchAddress("twSigAmpDigRB", &(Event->twSigAmpDigRB));

  /* ONION */
  RootTree->SetBranchAddress("onn", &(Event->onn));
  RootTree->SetBranchAddress("onid", &(Event->onid));
  RootTree->SetBranchAddress("onreg", &(Event->onreg));
  RootTree->SetBranchAddress("oninx", &(Event->oninx));
  RootTree->SetBranchAddress("oniny", &(Event->oniny));
  RootTree->SetBranchAddress("oninz", &(Event->oninz));
  RootTree->SetBranchAddress("oninpx", &(Event->oninpx));
  RootTree->SetBranchAddress("oninpy", &(Event->oninpy));
  RootTree->SetBranchAddress("oninpz", &(Event->oninpz));
  RootTree->SetBranchAddress("onoutx", &(Event->onoutx));
  RootTree->SetBranchAddress("onouty", &(Event->onouty));
  RootTree->SetBranchAddress("onoutz", &(Event->onoutz));
  RootTree->SetBranchAddress("onoutpx", &(Event->onoutpx));
  RootTree->SetBranchAddress("onoutpy", &(Event->onoutpy));
  RootTree->SetBranchAddress("onoutpz", &(Event->onoutpz));
  RootTree->SetBranchAddress("onde", &(Event->onde));
  RootTree->SetBranchAddress("onal", &(Event->onal));
  RootTree->SetBranchAddress("ontim", &(Event->ontim));
  //signal
  RootTree->SetBranchAddress("onSigN", &(Event->onSigN));
  RootTree->SetBranchAddress("onSigReg", &(Event->onSigReg));
  RootTree->SetBranchAddress("onSigAmp", &(Event->onSigAmp));
  RootTree->SetBranchAddress("onSigTim", &(Event->onSigTim));
  RootTree->SetBranchAddress("onSigED", &(Event->onSigED));
  //digitised signal
  RootTree->SetBranchAddress("onSigDigN", &(Event->onSigDigN));
  RootTree->SetBranchAddress("onSigRegDig", &(Event->onSigRegDig));
  RootTree->SetBranchAddress("onSigAmpDig", &(Event->onSigAmpDig));
  RootTree->SetBranchAddress("onSigTimDig", &(Event->onSigTimDig));

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


	//KENTROS
	//signal
	eve.keSigN = fSeg->KE_GetEntries();
	for (Int_t i0 = 0; i0 < eve.keSigN; i0++) {
		eve.keSigReg[i0] = fSeg->KE_GetReg(i0); //ID of the detector segment from 1 ...
		eve.keSigTim[i0] = fSeg->KE_GetTime(i0);
		eve.keSigAmp[i0] = fSeg->KE_GetAmp(i0);
		eve.keSigED[i0] = fSeg->KE_GetED(i0);
		eve.keSigX[i0] = fSeg->KE_GetPosition(i0).X();
		eve.keSigY[i0] = fSeg->KE_GetPosition(i0).Y();
		eve.keSigZ[i0] = fSeg->KE_GetPosition(i0).Z();
		eve.keSigpX[i0] = fSeg->KE_GetMomentum(i0).X();
		eve.keSigpY[i0] = fSeg->KE_GetMomentum(i0).Y();
		eve.keSigpZ[i0] = fSeg->KE_GetMomentum(i0).Z();
	}
	//digitised signal
	eve.keSigDigN = fSeg->KE_GetEntriesDig();
	for (Int_t i0 = 0; i0 < eve.keSigDigN; i0++) {
		eve.keSigRegDig[i0] = fSeg->KE_GetRegDig(i0); //ID of the detector segment from 1 ...
		eve.keSigTimDig[i0] = fSeg->KE_GetTimeDig(i0);
		eve.keSigAmpDig[i0] = fSeg->KE_GetAmpDig(i0);
	}

	//TOF-Wall
	// score only TOF-Wall slats with signal in a numbered scheme
	Count = 0;
	for (Int_t i0 = 0; i0 < TW_NbSlatFront; i0++) {
		//if top or bottom amplitude !=0. then score
		if ((fSeg->TW_GetAmplitude(0, 0, i0) != 0.) || (fSeg->TW_GetAmplitude(
				0, 1, i0) != 0.)) {
			eve.twSigIDF[Count] = i0;

			eve.twSigAmpFT[Count] = fSeg->TW_GetAmplitude(0, 0, i0);
			eve.twSigAmpFB[Count] = fSeg->TW_GetAmplitude(0, 1, i0);
			eve.twSigTimFT[Count] = fSeg->TW_GetTime(0, 0, i0);
			eve.twSigTimFB[Count] = fSeg->TW_GetTime(0, 1, i0);
			eve.twSigEDF[Count] = fSeg->TW_GetED(0, i0);

			eve.twSigAmpDigFT[Count] = fSeg->TW_GetAmplitudeDig(0, 0, i0);
			eve.twSigAmpDigFB[Count] = fSeg->TW_GetAmplitudeDig(0, 1, i0);
			eve.twSigTimDigFT[Count] = fSeg->TW_GetTimeDig(0, 0, i0);
			eve.twSigTimDigFB[Count] = fSeg->TW_GetTimeDig(0, 1, i0);

			Count++;
		}
	}
	eve.twSigNF = Count;

	Count = 0;
	for (Int_t i0 = 0; i0 < TW_NbSlatBack; i0++) {
		//if top or bottom amplitude !=0. then score
		if ((fSeg->TW_GetAmplitude(1, 0, i0) != 0.) || (fSeg->TW_GetAmplitude(
				1, 1, i0) != 0.)) {
			eve.twSigIDR[Count] = i0;

			eve.twSigAmpRT[Count] = fSeg->TW_GetAmplitude(1, 0, i0);
			eve.twSigAmpRB[Count] = fSeg->TW_GetAmplitude(1, 1, i0);
			eve.twSigTimRT[Count] = fSeg->TW_GetTime(1, 0, i0);
			eve.twSigTimRB[Count] = fSeg->TW_GetTime(1, 1, i0);
			eve.twSigEDR[Count] = fSeg->TW_GetED(1, i0);

			eve.twSigAmpDigRT[Count] = fSeg->TW_GetAmplitudeDig(1, 0, i0);
			eve.twSigAmpDigRB[Count] = fSeg->TW_GetAmplitudeDig(1, 1, i0);
			eve.twSigTimDigRT[Count] = fSeg->TW_GetTimeDig(1, 0, i0);
			eve.twSigTimDigRB[Count] = fSeg->TW_GetTimeDig(1, 1, i0);

			Count++;
		}
	}
	eve.twSigNR = Count;

	//ONION
	//signal
	eve.onSigN = fSeg->ON_GetEntries();
	for (Int_t i0 = 0; i0 < eve.onSigN; i0++) {
		eve.onSigReg[i0] = fSeg->ON_GetReg(i0); //ID of the detector segment from 1 ...
		eve.onSigTim[i0] = fSeg->ON_GetTime(i0);
		eve.onSigAmp[i0] = fSeg->ON_GetAmp(i0);
		eve.onSigED[i0] = fSeg->ON_GetED(i0);
	}
	//digitised signal
	eve.onSigDigN = fSeg->ON_GetEntriesDig();
	for (Int_t i0 = 0; i0 < eve.onSigDigN; i0++) {
		eve.onSigRegDig[i0] = fSeg->ON_GetRegDig(i0); //ID of the detector segment from 1 ...
		eve.onSigTimDig[i0] = fSeg->ON_GetTimeDig(i0);
		eve.onSigAmpDig[i0] = fSeg->ON_GetAmpDig(i0);
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

Int_t Evento::AddKENT(Int_t keid, Int_t kereg, Int_t keregtype, Double_t keinx,
		Double_t keiny, Double_t keinz, Double_t keoutx, Double_t keouty,
		Double_t keoutz, Double_t keinpx, Double_t keinpy, Double_t keinpz,
		Double_t keoutpx, Double_t keoutpy, Double_t keoutpz, Double_t kede,
		Double_t keal, Double_t ketim) {
	//  cout<<"Entro in Evento::AddStart "<<endl;
	if (eve.ken < MAXKE) {
		eve.keid[eve.ken] = keid;
		eve.kereg[eve.ken] = kereg;
		eve.keregtype[eve.ken] = keregtype;
		eve.keinx[eve.ken] = keinx;
		eve.keiny[eve.ken] = keiny;
		eve.keinz[eve.ken] = keinz;
		eve.keinpx[eve.ken] = keinpx;
		eve.keinpy[eve.ken] = keinpy;
		eve.keinpz[eve.ken] = keinpz;
		eve.keoutx[eve.ken] = keoutx;
		eve.keouty[eve.ken] = keouty;
		eve.keoutz[eve.ken] = keoutz;
		eve.keoutpx[eve.ken] = keoutpx;
		eve.keoutpy[eve.ken] = keoutpy;
		eve.keoutpz[eve.ken] = keoutpz;
		eve.kede[eve.ken] = kede;
		eve.keal[eve.ken] = keal;
		eve.ketim[eve.ken] = ketim;

		eve.ken++;

		//send information for signal generation
		fSeg->KE_Score(kereg, keregtype, 
			       keinpx, keinpy, keinpz, 
			       keinx, keiny, keinz, 
			       keoutx, keouty, keoutz,
			       kede, keal, ketim);

		return 0;
	} else {
		return -1;
	}
}

/*-----------------------------------------------------------------*/


/*-----------------------------------------------------------------*/

Int_t Evento::AddTOFW(Int_t idtw, Int_t flgtw, Double_t xintw, Double_t yintw,
		Double_t zintw, Double_t xinTFtw, Double_t yinTFtw, Double_t zinTFtw,
		Double_t xouttw, Double_t youttw, Double_t zouttw, Double_t pxintw,
		Double_t pyintw, Double_t pzintw, Double_t pxouttw, Double_t pyouttw,
		Double_t pzouttw, Double_t detw, Double_t altw, Double_t timtw,
		Int_t twSlNbS, Int_t twSlNbF, Int_t twSlNbB, Int_t twSlID[4],
		Double_t twInX[4], Double_t twInY[4], Double_t twInZ[4],
		Double_t twOutX[4], Double_t twOutY[4], Double_t twOutZ[4],
		Double_t twInPX[4], Double_t twInPY[4], Double_t twInPZ[4],
		Double_t twOutPX[4], Double_t twOutPY[4], Double_t twOutPZ[4],
		Double_t twER[4], Double_t twQER[4], Double_t twTime[4]) {
	//  cout<<"Entro in Evento::AddTOFW "<<endl;

	if (eve.twn < MAXTW) {
		eve.twn++;
		eve.twplid[eve.twn - 1] = idtw;
		eve.twplflg[eve.twn - 1] = flgtw;
		eve.twplinx[eve.twn - 1] = xintw;
		eve.twpliny[eve.twn - 1] = yintw;
		eve.twplinz[eve.twn - 1] = zintw;
		eve.twplinxTF[eve.twn - 1] = xinTFtw;
		eve.twplinyTF[eve.twn - 1] = yinTFtw;
		eve.twplinzTF[eve.twn - 1] = zinTFtw;
		eve.twplinpx[eve.twn - 1] = pxintw;
		eve.twplinpy[eve.twn - 1] = pyintw;
		eve.twplinpz[eve.twn - 1] = pzintw;
		eve.twploutx[eve.twn - 1] = xouttw;
		eve.twplouty[eve.twn - 1] = youttw;
		eve.twploutz[eve.twn - 1] = zouttw;
		eve.twploutpx[eve.twn - 1] = pxouttw;
		eve.twploutpy[eve.twn - 1] = pyouttw;
		eve.twploutpz[eve.twn - 1] = pzouttw;
		eve.twplde[eve.twn - 1] = detw;
		eve.twplal[eve.twn - 1] = altw;
		eve.twpltim[eve.twn - 1] = timtw;
		eve.twSlNbS[eve.twn - 1] = twSlNbS;
		//
		eve.twSlNbF[eve.twn - 1] = twSlNbF;
		eve.twSlNbB[eve.twn - 1] = twSlNbB;
		for (int jj = 0; jj < 4; jj++) {
			eve.twSlID[jj][eve.twn - 1] = twSlID[jj];
			eve.twInX[jj][eve.twn - 1] = twInX[jj];
			eve.twInY[jj][eve.twn - 1] = twInY[jj];
			eve.twInZ[jj][eve.twn - 1] = twInZ[jj];
			eve.twOutX[jj][eve.twn - 1] = twOutX[jj];
			eve.twOutY[jj][eve.twn - 1] = twOutY[jj];
			eve.twOutZ[jj][eve.twn - 1] = twOutZ[jj];
			eve.twInPX[jj][eve.twn - 1] = twInPX[jj];
			eve.twInPY[jj][eve.twn - 1] = twInPY[jj];
			eve.twInPZ[jj][eve.twn - 1] = twInPZ[jj];
			eve.twOutPX[jj][eve.twn - 1] = twOutPX[jj];
			eve.twOutPY[jj][eve.twn - 1] = twOutPY[jj];
			eve.twOutPZ[jj][eve.twn - 1] = twOutPZ[jj];
			eve.twER[jj][eve.twn - 1] = twER[jj];
			eve.twQER[jj][eve.twn - 1] = twQER[jj];
			eve.twTime[jj][eve.twn - 1] = twTime[jj];
			// segnale dello TOF-Wall
			if (twSlID[jj] != 0) {//if there is an entry
				//WATCHOUT: Slat ID runs for C++ from 0 NOT from 1 as for FORTRAN
				if (jj < 2) {//Front Wall
					fSeg->TW_Score(0,
							twSlID[jj] - 1,
							0 //charge not needed currently, if needed: eve.icha[fidtw]
							, twER[jj], twQER[jj], twInX[jj], twInY[jj],
							twInZ[jj], twOutX[jj], twOutY[jj], twOutZ[jj],
							twTime[jj]);
				} else {//Read Wall
					fSeg->TW_Score(1,
							twSlID[jj] - 1,
							0 //charge not needed currently, if needed: eve.icha[fidtw]
							, twER[jj], twQER[jj], twInX[jj], twInY[jj],
							twInZ[jj], twOutX[jj], twOutY[jj], twOutZ[jj],
							twTime[jj]);
				}
			}
		}

		return 0;
	} else {
		return -1;
	}
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddONIO(Int_t onid, Int_t onreg, Double_t oninx, Double_t oniny,
		Double_t oninz, Double_t onoutx, Double_t onouty, Double_t onoutz,
		Double_t oninpx, Double_t oninpy, Double_t oninpz, Double_t onoutpx,
		Double_t onoutpy, Double_t onoutpz, Double_t onde, Double_t onal,
		Double_t ontim) {
	//  cout<<"Entro in Evento::AddStart "<<endl;
	if (eve.onn < MAXON) {
		eve.onn++;
		eve.onid[eve.onn - 1] = onid;
		eve.onreg[eve.onn - 1] = onreg;
		eve.oninx[eve.onn - 1] = oninx;
		eve.oniny[eve.onn - 1] = oniny;
		eve.oninz[eve.onn - 1] = oninz;
		eve.oninpx[eve.onn - 1] = oninpx;
		eve.oninpy[eve.onn - 1] = oninpy;
		eve.oninpz[eve.onn - 1] = oninpz;
		eve.onoutx[eve.onn - 1] = onoutx;
		eve.onouty[eve.onn - 1] = onouty;
		eve.onoutz[eve.onn - 1] = onoutz;
		eve.onoutpx[eve.onn - 1] = onoutpx;
		eve.onoutpy[eve.onn - 1] = onoutpy;
		eve.onoutpz[eve.onn - 1] = onoutpz;
		eve.onde[eve.onn - 1] = onde;
		eve.onal[eve.onn - 1] = onal;
		eve.ontim[eve.onn - 1] = ontim;

		//send information for signal generation
		fSeg->ON_Score(onreg, oninx, oniny, oninz, onoutx, onouty, onoutz,
				onde, onal, ontim);

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



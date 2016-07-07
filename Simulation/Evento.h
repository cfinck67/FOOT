#ifndef ROOT_Evento
#define ROOT_Evento

#include <iostream>
#include <fstream>
#include <vector>

#include "TObject.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TROOT.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TFile.h"

//#include "MU_STRUCT.h"

#include "Geometry.h"
#include "Segnale.h"
#include "gsi_geo.h"

// VM 14/11/13 added to save mimosa events in the queue for pileup simulation
#include <queue>

// VM 14/11/13 terms added to trid and trpaid of each pile-up event
// to separate the navigations of MC track lists corresponding to
// different events
const Int_t PILEUP_ROUND = 10000;


using namespace std;


// VM 14/11/13 structure containing mimosa infoes for each event
// to save in the queue for pile-up simulation
typedef struct {
  Int_t    miSigN; //*
  Int_t    miSigChip[MAXMISIG]; //*chipnumber
  Int_t    miSigIndex[MAXMISIG]; //*pixel index in the chip
  Int_t    miSigCol[MAXMISIG]; //*column
  Int_t    miSigRow[MAXMISIG]; //*row
  Int_t    miSigId[MAXMISIG]; //*particle ID in particle_common  
  Double_t miSigTim[MAXMISIG]; //*time when pixel activated
  Double_t miSigX[MAXMISIG]; //*(global) coordinates of pixel
  Double_t miSigY[MAXMISIG]; //*(global) coordinates of pixel
  Double_t miSigZ[MAXMISIG]; //*(global) coordinates of pixel
  Double_t miSigpX[MAXMISIG]; //*momentum
  Double_t miSigpY[MAXMISIG]; //*momentum
  Double_t miSigpZ[MAXMISIG]; //*momentum
  Double_t miSigPedest[MAXMISIG]; //*pedestal oqn pixel
  Double_t miSigDE[MAXMISIG];  // *energy loss (VM 3/11/2013)
  Int_t    min;
  Int_t    miid[MAXMI];
  Int_t    michip[MAXMI];
  Int_t    mincol[MAXMI];
  Int_t    minrow[MAXMI];
  Double_t mix[MAXMI];
  Double_t miy[MAXMI];
  Double_t miz[MAXMI];
  Double_t mipx[MAXMI];
  Double_t mipy[MAXMI];
  Double_t mipz[MAXMI];
  Double_t mide[MAXMI];
  Double_t mitim[MAXMI];
} MIMO_STRUCT;

typedef struct {
  Int_t trn; //number of tracks for current event
  Int_t trpileup[MAXTR]; // 0=current event >0 pile-up 
  Int_t trid[MAXTR]; //generation number of the track (FLUKA intern)
                     // + PILEUP_ROUND*trpileup (VM)
  Int_t trpaid[MAXTR]; //generation number of the track parent (FLUKA intern)
                       // + PILEUP_ROUND*trpileup (VM)
  Int_t trtype[MAXTR]; //particle type (FLUKA encoding)
  Double_t trmass[MAXTR]; // particles mass [GeVc^2]
  Int_t trcha[MAXTR]; //charge of particle
  Int_t trbar[MAXTR]; //barionic number
  Int_t trreg[MAXTR]; //production region of particle (FLUKA region number)
  Int_t trgen[MAXTR]; //type of generation interaction (FLUKA encoding)
  Int_t trdead[MAXTR]; // with which interaction the particle dies (FLUKA encoding)
  Double_t tritime[MAXTR];//production time of the track
  Double_t trix[MAXTR]; //track initial position [cm]
  Double_t triy[MAXTR];
  Double_t triz[MAXTR];
  Double_t trfx[MAXTR]; //track final position
  Double_t trfy[MAXTR];
  Double_t trfz[MAXTR];
  Double_t tripx[MAXTR]; //track initial momentum [GeV/c]
  Double_t tripy[MAXTR];
  Double_t tripz[MAXTR];
  Double_t trpapx[MAXTR];//momentum of parent particle at production [GeV/c]
  Double_t trpapy[MAXTR];
  Double_t trpapz[MAXTR];
} MC_STRUCT;

typedef struct {
  Int_t EventNumber;
  
  /*
    track common
  */
  Int_t trn; //number of tracks for current event
  Int_t trpileup[MAXTR]; // 0=current event >0 pile-up (VM added)
  Int_t trid[MAXTR]; //generation number of the track (FLUKA intern)
                     // + PILEUP_ROUND*trpileup (VM)
  Int_t trpaid[MAXTR]; //generation number of the track parent (FLUKA intern)
                       // + PILEUP_ROUND*trpileup (VM)
  Int_t trtype[MAXTR]; //particle type (FLUKA encoding)
  Double_t trmass[MAXTR]; // particles mass [GeVc^2]
  Int_t trcha[MAXTR]; //charge of particle
  Int_t trbar[MAXTR]; //barionic number
  Int_t trreg[MAXTR]; //production region of particle (FLUKA region number)
  Int_t trgen[MAXTR]; //type of generation interaction (FLUKA encoding)
  Int_t trdead[MAXTR]; // with which interaction the particle dies (FLUKA encoding)
  Double_t tritime[MAXTR];//production time of the track
  Double_t trix[MAXTR]; //track initial position [cm]
  Double_t triy[MAXTR];
  Double_t triz[MAXTR];
  Double_t trfx[MAXTR]; //track final position
  Double_t trfy[MAXTR];
  Double_t trfz[MAXTR];
  Double_t tripx[MAXTR]; //track initial momentum [GeV/c]
  Double_t tripy[MAXTR];
  Double_t tripz[MAXTR];
  Double_t trpapx[MAXTR];//momentum of parent particle at production [GeV/c]
  Double_t trpapy[MAXTR];
  Double_t trpapz[MAXTR];

  
// GENERAL SYNTAX FOR DETECTOR VARIABLES
// tr...: Track
// st...: MAGARITA start counter 
// tg...: Target
// mi...: MIMOSA-26
// ke...: KENTROS
// mu...: TP-MUSIC IV
// tw...: TOF-Wall
// on...: ONION veto detector

// variables with raw Monte Carlo data
// ...n: number of particles entering the sensitive region for this event
// ...id: index of particle in "track common"
// ...reg: sensitive region ID
// ...inx,y,z: position where particles enters the sensitive region [cm]
// ...outx,y,z: position where particles exits the sensitive region [cm]
// ...inpx,py,pz: momentum when particles enters the sensitive region [GeV/c]
// ...outpx,py,pz: momentum when particles exits the sensitive region [GeV/c]
// ...de: energy deposition in sensitive region [GeV/cm^3]
// ...al: quenched energy deposition in sensitive region (with Birk's law) [GeV/cm^3]
// ...tim: time when particle enters the sensitive region [s]

// ...Sig...: signal variables  
// ...Amp...: amplitude of signal
// ...Time...: time of signal
// ...Dig...: Digitized

  /*
    start counter block
*/
  Int_t stn; //number particles entering the start counter for current event
  Int_t stid[MAXST];
  Int_t stflg[MAXST];
  Double_t stinx[MAXST];
  Double_t stiny[MAXST];
  Double_t stinz[MAXST];
  Double_t stoutx[MAXST];
  Double_t stouty[MAXST];
  Double_t stoutz[MAXST];
  Double_t stinpx[MAXST];
  Double_t stinpy[MAXST];
  Double_t stinpz[MAXST];
  Double_t stoutpx[MAXST];
  Double_t stoutpy[MAXST];
  Double_t stoutpz[MAXST];
  Double_t stde[MAXST];
  Double_t stal[MAXST];
  Double_t sttim[MAXST];
//signal  
  Double_t stSigAmp;
  Double_t stSigTime;
  Double_t stSigED;

  Int_t    stSigAmpDig;
  Int_t    stSigTimeDig;

  /*
    beam monitor chamber block
*/
  Int_t nmon; //number particles entering the beam monitor for current event
  Int_t idmon[MAXMON];
  Int_t iview[MAXMON];
  Int_t icell[MAXMON];
  Int_t ilayer[MAXMON];
  Double_t xcamon[MAXMON];
  Double_t ycamon[MAXMON];
  Double_t zcamon[MAXMON];
  Double_t pxcamon[MAXMON];
  Double_t pycamon[MAXMON];
  Double_t pzcamon[MAXMON];
  Double_t rdrift[MAXMON];
  Double_t tdrift[MAXMON];
  Double_t timmon[MAXMON];


  /*
    target block
*/
  Int_t tgn;
  Int_t tgid[MAXTG];
  Int_t tgflg[MAXTG];
  Double_t tginx[MAXTG];
  Double_t tginy[MAXTG];
  Double_t tginz[MAXTG];
  Double_t tgoutx[MAXTG];
  Double_t tgouty[MAXTG];
  Double_t tgoutz[MAXTG];
  Double_t tginpx[MAXTG];
  Double_t tginpy[MAXTG];
  Double_t tginpz[MAXTG];
  Double_t tgoutpx[MAXTG];
  Double_t tgoutpy[MAXTG];
  Double_t tgoutpz[MAXTG];
  Double_t tgtim[MAXTG];

  /*
    vertex tracker block (MIMOSA 26)
*/
  Int_t    min;
  Int_t    miid[MAXMI];
  Int_t    michip[MAXMI];
  Int_t    mincol[MAXMI];
  Int_t    minrow[MAXMI];
  Double_t mix[MAXMI];
  Double_t miy[MAXMI];
  Double_t miz[MAXMI];
  Double_t mipx[MAXMI];
  Double_t mipy[MAXMI];
  Double_t mipz[MAXMI];
  Double_t mide[MAXMI];
  Double_t mitim[MAXMI];
  //signal of MIMOSA26 (activated maps)
  Int_t    miSigN;
  Int_t    miSigChip[MAXMISIG]; //chipnumber
  Int_t    miSigIndex[MAXMISIG]; //pixel index in the chip
  Int_t    miSigCol[MAXMISIG]; //column
  Int_t    miSigRow[MAXMISIG]; //row
  Int_t    miSigId[MAXMISIG]; //particle ID in particle_common  
  Double_t miSigTim[MAXMISIG]; //time when pixel activated
  Double_t miSigX[MAXMISIG]; //(global) coordinates of pixel
  Double_t miSigY[MAXMISIG]; //(global) coordinates of pixel
  Double_t miSigZ[MAXMISIG]; //(global) coordinates of pixel
  Double_t miSigpX[MAXMISIG]; //momentum
  Double_t miSigpY[MAXMISIG]; //momentum
  Double_t miSigpZ[MAXMISIG]; //momentum
  Double_t miSigPedest[MAXMISIG]; //pedestal oqn pixel
  Double_t miSigDE[MAXMISIG];  // energy loss (VM 3/11/2013)


} EVENT_STRUCT;

class Evento  {
  
public:
  
  Evento(Segnale *TheSegnale,Geometry *TheGeometry);
  
  virtual ~Evento();   
 
  Int_t BOEvent();
  
  Int_t CleanEvent(EVENT_STRUCT *Event);
  
  Int_t SetToMax(EVENT_STRUCT *Event);
  
  Int_t InitBranches(TTree *RootTree,EVENT_STRUCT *Event);
  
  Int_t FindBranches(TTree *RootTree,EVENT_STRUCT *Event);

  Int_t EOEvent();
  
  Int_t SetEvent(Int_t numero_evento);

  Int_t AddPart(Int_t numtrack, Int_t idpa, Int_t igen, 
		Int_t icha, Int_t numreg, Int_t iba,
		Int_t idead, Int_t jpa,
		Double_t vxi,  Double_t vyi,  Double_t vzi,
		Double_t vxf,  Double_t vyf,  Double_t vzf,
		Double_t px,   Double_t py,   Double_t pz,
		Double_t pxpa, Double_t pypa, Double_t pzpa,
		Double_t amass, Double_t tempo);

  Int_t AddStart(Int_t idst, Int_t flgst,
		Double_t xinst,   Double_t yinst,   Double_t zinst,
		Double_t xoutst,  Double_t youtst,  Double_t zoutst,
		Double_t pxinst,  Double_t pyinst,  Double_t pzinst,
		Double_t pxoutst, Double_t pyoutst, Double_t pzoutst,
		Double_t dest, Double_t alst, Double_t timst);


  Int_t AddBMON(Int_t idmon, Int_t iview, Int_t icell, Int_t ilayer,
		Double_t xcamon, Double_t ycamon, Double_t zcamon,
		Double_t pxcamon, Double_t pycamon, Double_t pzcamon,
		Double_t rdrift, Double_t tdrift, Double_t timmon);

  Int_t AddTarget(Int_t idtg, Int_t flgtg,
               Double_t xintg,   Double_t yintg,   Double_t zintg,
               Double_t xouttg,  Double_t youttg,  Double_t zouttg,
               Double_t pxintg,  Double_t pyintg,  Double_t pzintg,
               Double_t pxouttg, Double_t pyouttg, Double_t pzouttg,
               Double_t timtg);

  Int_t AddMaps(Int_t idmaps, Int_t chipmaps, Int_t ncolmaps, Int_t nrowmaps,
		Double_t xmaps,  Double_t ymaps,  Double_t zmaps,
		Double_t pxmaps, Double_t pymaps, Double_t pzmaps,
		Double_t demaps, Double_t timmaps);
		


  Int_t Dump();
  //  void Dump() const { return TObject::Dump(); }
	  
  EVENT_STRUCT Output();

private:

  TRandom3 *ar3;
  TH1D *myreso;
  Segnale    *fSeg;
  Geometry   *fGeo;
  EVENT_STRUCT eve;
//  MU_STRUCT* pmu;

// VM 14/11/13 added to store mimosa hits for pile-up simulation
  std::queue <MIMO_STRUCT *> mimo_queue;
  std::queue <MC_STRUCT *> mc_queue;  
  TRandom3 rnd;
  Int_t count_skip;


protected:

  ClassDef(Evento,1);
  
};

#endif


///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************

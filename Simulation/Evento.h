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

  
  /*
    KENTROS block
*/
  Int_t ken;
  Int_t keid[MAXKE];//ID of particle in track 
  Int_t kereg[MAXKE];//ID of the detector segment from 1 ... 
  Int_t keregtype[MAXKE];//type of the detector segment barrel(=1), fiber(=2), small endcap(=3), big endcap(=4) 
  Double_t keinx[MAXKE];
  Double_t keiny[MAXKE];
  Double_t keinz[MAXKE];
  Double_t keoutx[MAXKE];
  Double_t keouty[MAXKE];
  Double_t keoutz[MAXKE];
  Double_t keinpx[MAXKE];
  Double_t keinpy[MAXKE];
  Double_t keinpz[MAXKE];
  Double_t keoutpx[MAXKE];
  Double_t keoutpy[MAXKE];
  Double_t keoutpz[MAXKE];
  Double_t kede[MAXKE];
  Double_t keal[MAXKE];
  Double_t ketim[MAXKE];
  
//signal
  Int_t    keSigN;
  Int_t    keSigReg[MAXKE]; //ID of the detector segment from 1 ... 
  Double_t keSigTim[MAXKE];
  Double_t keSigAmp[MAXKE];
  Double_t keSigED[MAXKE];
  Double_t keSigX[MAXKE];
  Double_t keSigY[MAXKE];
  Double_t keSigZ[MAXKE];
  Double_t keSigpX[MAXKE];
  Double_t keSigpY[MAXKE];
  Double_t keSigpZ[MAXKE];
  
//digitised signal
  Int_t keSigDigN;
  Int_t keSigRegDig[MAXKE];
  Int_t keSigTimDig[MAXKE];
  Int_t keSigAmpDig[MAXKE];


  /*
    TP-MUSIC IV
*/
//  MU_STRUCT mu;

  //MU_PRO_Nb= number of proporional chamber regions (=8)
  Int_t mun[MU_PRO_Nb];

  Int_t muid[MU_PRO_Nb][MAXMU];
  Double_t muinx[MU_PRO_Nb][MAXMU];
  Double_t muiny[MU_PRO_Nb][MAXMU];
  Double_t muinz[MU_PRO_Nb][MAXMU];
  Double_t muoutx[MU_PRO_Nb][MAXMU];
  Double_t muouty[MU_PRO_Nb][MAXMU];
  Double_t muoutz[MU_PRO_Nb][MAXMU];
  Double_t muinpx[MU_PRO_Nb][MAXMU];
  Double_t muinpy[MU_PRO_Nb][MAXMU];
  Double_t muinpz[MU_PRO_Nb][MAXMU];
  Double_t muoutpx[MU_PRO_Nb][MAXMU];
  Double_t muoutpy[MU_PRO_Nb][MAXMU];
  Double_t muoutpz[MU_PRO_Nb][MAXMU];
  Double_t mude[MU_PRO_Nb][MAXMU];
  Double_t mual[MU_PRO_Nb][MAXMU];
  Double_t mutim[MU_PRO_Nb][MAXMU];
//signal
  Int_t muSigN[MU_PRO_Nb];
  Double_t muSigTim[MU_PRO_Nb][MAXMU];
  Double_t muSigAmp[MU_PRO_Nb][MAXMU];
  Double_t muSigHei[MU_PRO_Nb][MAXMU]; //Height (y-coordinate)
  Double_t muSigED[MU_PRO_Nb][MAXMU];
//digitised signal
  Int_t muSigDigN[MU_PRO_Nb];
  Int_t muSigTimDig[MU_PRO_Nb][MAXMU];
  Int_t muSigAmpDig[MU_PRO_Nb][MAXMU];
  Int_t muSigHeiDig[MU_PRO_Nb][MAXMU];//Height (y-coordinate)


 /*
    TOF WALL
*/
  // scoring plane in front of TOF-Wall 
  Int_t twn;
  Int_t twplid[MAXTW];
  Int_t twplflg[MAXTW];
  Double_t twplinx[MAXTW];
  Double_t twpliny[MAXTW];
  Double_t twplinz[MAXTW];
  Double_t twplinxTF[MAXTW]; // x-coord in TOF-Wall 
  Double_t twplinyTF[MAXTW]; 
  Double_t twplinzTF[MAXTW];
  Double_t twploutx[MAXTW];
  Double_t twplouty[MAXTW];
  Double_t twploutz[MAXTW];
  Double_t twplinpx[MAXTW];
  Double_t twplinpy[MAXTW];
  Double_t twplinpz[MAXTW];
  Double_t twploutpx[MAXTW];
  Double_t twploutpy[MAXTW];
  Double_t twploutpz[MAXTW];
  Double_t twplde[MAXTW];
  Double_t twplal[MAXTW];
  Double_t twpltim[MAXTW];
// scoring in TOF-Wall slats 
  Int_t twSlNbS[MAXTW],twSlNbF[MAXTW],twSlNbB[MAXTW];
  Int_t twSlID[4][MAXTW];
  Double_t twInX[4][MAXTW]  ,twInY[4][MAXTW]  ,twInZ[4][MAXTW];
  Double_t twOutX[4][MAXTW] ,twOutY[4][MAXTW] ,twOutZ[4][MAXTW];
  Double_t twInPX[4][MAXTW] ,twInPY[4][MAXTW] ,twInPZ[4][MAXTW];
  Double_t twOutPX[4][MAXTW],twOutPY[4][MAXTW],twOutPZ[4][MAXTW];
  Double_t twER[4][MAXTW],twQER[4][MAXTW],twTime[4][MAXTW];
  
  Int_t twSigNF;
  Int_t twSigNR;
  Int_t twSigIDF[TW_NbSlatFront];
  Int_t twSigIDR[TW_NbSlatBack];
//signal
  Double_t twSigTimFT[TW_NbSlatFront],twSigTimFB[TW_NbSlatFront],twSigTimRT[TW_NbSlatBack],twSigTimRB[TW_NbSlatBack];
  Double_t twSigAmpFT[TW_NbSlatFront],twSigAmpFB[TW_NbSlatFront],twSigAmpRT[TW_NbSlatBack],twSigAmpRB[TW_NbSlatBack];
  Double_t twSigEDF[TW_NbSlatFront],twSigEDR[TW_NbSlatBack];
//digitised signal
  Int_t twSigTimDigFT[TW_NbSlatFront],twSigTimDigFB[TW_NbSlatFront],twSigTimDigRT[TW_NbSlatBack],twSigTimDigRB[TW_NbSlatBack];
  Int_t twSigAmpDigFT[TW_NbSlatFront],twSigAmpDigFB[TW_NbSlatFront],twSigAmpDigRT[TW_NbSlatBack],twSigAmpDigRB[TW_NbSlatBack];
  
 
  /*
    ONION block
*/
  Int_t onn;
  Int_t onid[MAXON];
  Int_t onreg[MAXON];
  Double_t oninx[MAXON];
  Double_t oniny[MAXON];
  Double_t oninz[MAXON];
  Double_t onoutx[MAXON];
  Double_t onouty[MAXON];
  Double_t onoutz[MAXON];
  Double_t oninpx[MAXON];
  Double_t oninpy[MAXON];
  Double_t oninpz[MAXON];
  Double_t onoutpx[MAXON];
  Double_t onoutpy[MAXON];
  Double_t onoutpz[MAXON];
  Double_t onde[MAXON];
  Double_t onal[MAXON];
  Double_t ontim[MAXON];
  //signal
  Int_t    onSigN;
  Int_t    onSigReg[MAXON]; //ID of the detector segment from 1 ... 
  Double_t onSigTim[MAXON];
  Double_t onSigAmp[MAXON];
  Double_t onSigED[MAXON];
  //digitised signal
  Int_t onSigDigN;
  Int_t onSigRegDig[MAXON];
  Int_t onSigTimDig[MAXON];
  Int_t onSigAmpDig[MAXON];


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
		
  Int_t AddKENT(Int_t  keid, Int_t kereg, Int_t keregtype,
		       Double_t  keinx, Double_t  keiny, Double_t  keinz,
		       Double_t  keoutx, Double_t  keouty, Double_t  keoutz,
		       Double_t  keinpx, Double_t  keinpy, Double_t  keinpz,
		       Double_t  kepoutx, Double_t  kepouty, Double_t  kepoutz,
		       Double_t  kede, Double_t  keal, Double_t  ketim);

  Int_t AddTOFW(Int_t idtw, Int_t flgtw,
		Double_t xintw,   Double_t yintw,   Double_t zintw,
		Double_t xinTFtw, Double_t yinTFtw, Double_t zinTFtw,
		Double_t xouttw,  Double_t youttw,  Double_t zouttw,
		Double_t pxintw,  Double_t pyintw,  Double_t pzintw,
		Double_t pxouttw, Double_t pyouttw, Double_t pzouttw,
		Double_t detw, Double_t altw, Double_t timtw,
		Int_t twSlNbS,Int_t twSlNbF,Int_t twSlNbB,
		Int_t twSlID[4],
		Double_t twInX[4]  ,Double_t twInY[4]  ,Double_t twInZ[4],
		Double_t twOutX[4] ,Double_t twOutY[4] ,Double_t twOutZ[4],
		Double_t twInPX[4] ,Double_t twInPY[4] ,Double_t twInPZ[4],
		Double_t twOutPX[4],Double_t twOutPY[4],Double_t twOutPZ[4],
		Double_t twER[4],Double_t twQER[4],Double_t twTime[4]
		);

  Int_t AddONIO(Int_t  onid, Int_t onreg,
		       Double_t  oninx, Double_t  oniny, Double_t  oninz,
		       Double_t  onoutx, Double_t  onouty, Double_t  onoutz,
		       Double_t  oninpx, Double_t  oninpy, Double_t  oninpz,
		       Double_t  onpoutx, Double_t  onpouty, Double_t  onpoutz,
		       Double_t  onde, Double_t  onal, Double_t  ontim);


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

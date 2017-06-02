#ifndef RecoTools_h
#define RecoTools_h

#include <TString.h>
#include <TFile.h>
#include <TSpline.h>
#include <TTree.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

#include "Evento.h"
// #include "Segnale.h"
#include "Geometry.h"
// #include "Trigger.h"

#include "TAGpadGroup.hxx"
#include "TAGgeoTrafo.hxx"

#include "TAGactTreeWriter.hxx"
#include "TAGdataDsc.hxx"
#include "TAGroot.hxx"

#include "KFitter.hxx"
#include "GlobalPar.hxx"



class RecoTools {
  
 public :

  RecoTools(int d, TString istr, bool list, TString ostr, TString wd, int nev,
	    TFile *hf);
  virtual ~RecoTools();

  //Reconstructions
  void RecoLoop(TAGroot *tagr, int fr);

  //Actions for Tupling MC info for the various subdetectors
  void FillMCEvent(EVENT_STRUCT *myStr);
  void FillMCBeamMonitor(EVENT_STRUCT *myStr);
  void FillMCDriftChamber(EVENT_STRUCT *myStr);
  void FillMCVertex(EVENT_STRUCT *myStr);
  void FillMCMSD(EVENT_STRUCT *myStr);
  void FillMCInnerTracker(EVENT_STRUCT *myStr);
  void FillMCInteractionRegion(EVENT_STRUCT *myStr);
  void FillMCCalorimeter(EVENT_STRUCT *myStr);
  void FillMCTofWall(EVENT_STRUCT *myStr);

  //Monitors/Displays
  void DisplayIRMonitor(TAGpadGroup* pg, EVENT_STRUCT *myStr);
  void DisplayBeamMonitor(TAGpadGroup* pg);

  void initBMGeo(TAGparaDsc* p_bmgeo); //Beam Monitor Geometry
  void initBMCon(TAGparaDsc* beamcon);

  void initDCGeo(TAGparaDsc* p_dcgeo); //Drift Chamber Geometry
  void initDCCon(TAGparaDsc* driftcon);

  void bookHisto(TFile *f);
  void CalibBMVT();
  void MonitorBMVTMat();
  void MonitorBMNew(Long64_t jentry);
  void MonitorBM();
  void AssociateHitsToParticle();

  TAGgeoTrafo* fGeoTrafo;

  KFitter* m_kFitter;

  int m_nev;
  int m_debug;
  bool m_flaghisto, m_fullmoni;
  TFile* m_of;
  TFile* m_hf;
  TString m_oustr;
  TString m_instr;
  TString m_wd;

  //MC block
  TAGdataDsc* myn_mceve;
  TAGdataDsc* myn_mcmimo;

  vector<string> my_files;

  //Parameters
  TAGparaDsc* myp_bmgeo;
  TAGparaDsc* myp_dcgeo;
  shared_ptr<TAVTparGeo> m_vtgeo;
  shared_ptr<TAITparGeo> m_itgeo;
  shared_ptr<TADCparGeo> m_dcgeo;

  TAGparaDsc* myp_vtmap;
  TAGparaDsc* myp_vtcal;
  TAGparaDsc* myp_vtgeo;
  TAGparaDsc* myp_vtconf;

  TAGparaDsc* myp_msdmap;
  TAGparaDsc* myp_msdcal;
  TAGparaDsc* myp_msdgeo;
  TAGparaDsc* myp_msdconf;

  TAGparaDsc* myp_itmap;
  TAGparaDsc* myp_itcal;
  TAGparaDsc* myp_itgeo;
  TAGparaDsc* myp_itconf;

  //Data descriptions
 
  TAGdataDsc* myn_irraw;

  TAGparaDsc* myp_bmcon;
  TAGdataDsc* myn_bmraw;
  TAGparaDsc* myp_dccon;
  TAGdataDsc* myn_dcraw;
  TAGdataDsc* myn_vtraw;
  TAGdataDsc* myn_vtclus;
  TAGdataDsc* myn_vtrk;
  TAGdataDsc* myn_vtvtx;
  TAGdataDsc* myn_bmtrk;
  TAGdataDsc* myn_dctrk;
  TAGdataDsc* myn_msdraw;


  TAGdataDsc* myn_itraw;
  TAGdataDsc* myn_itclus;
  TAGdataDsc* myn_itrk;

  TAGdataDsc* myn_trtrk;
  TAGdataDsc* myn_trtin;

  TAGdataDsc* myn_twraw;
  TAGdataDsc* myn_caraw;

  //Actions

  TAGactTreeWriter* my_out;
  TAGaction* mya_msdraw;
  TAGaction* mya_vtraw;
  TAGaction* mya_vtclus;
  TAGaction* mya_vttrack;
  TAGaction* mya_vtvtx;

  TAGaction* mya_itraw;
  TAGaction* mya_itclus;
  TAGaction* mya_ittrack;



};

#endif

#ifdef RecoTools_cxx

RecoTools::~RecoTools() {}

#endif

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
#include "Segnale.h"
#include "Geometry.h"
#include "Trigger.h"

#include "TAGpadGroup.hxx"
#include "TAGgeoTrafo.hxx"

#include "TAGactTreeWriter.hxx"
#include "TAGdataDsc.hxx"
#include "TAGroot.hxx"


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
  void FillMCVertex(EVENT_STRUCT *myStr);
  void FillMCInteractionRegion(EVENT_STRUCT *myStr);

  //Monitors/Displays
  void DisplayIRMonitor(TAGpadGroup* pg, EVENT_STRUCT *myStr);
  void DisplayBeamMonitor(TAGpadGroup* pg);

  void initBMGeo(TAGparaDsc* p_bmgeo); //Beam Monitor Geometry
  void initBMCon(TAGparaDsc* beamcon);

  void bookHisto(TFile *f);
  void CalibBMVT();
  void MonitorBMVTMat();
  void MonitorBM();

  TAGgeoTrafo* fGeoTrafo;

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

  TAGparaDsc* myp_vtmap;
  TAGparaDsc* myp_vtcal;
  TAGparaDsc* myp_vtgeo;
  TAGparaDsc* myp_vtconf;

  //Data descriptions
 
  TAGdataDsc* myn_irraw;

  TAGparaDsc* myp_bmcon;
  TAGdataDsc* myn_bmraw;
  TAGdataDsc* myn_vtraw;
  TAGdataDsc* myn_vtclus;
  TAGdataDsc* myn_vtrk;
  TAGdataDsc* myn_vtvtx;
  TAGdataDsc* myn_bmtrk;

  TAGdataDsc* myn_trtrk;
  TAGdataDsc* myn_trtin;

  TAGactTreeWriter* my_out;
  TAGaction* mya_vtraw;
  TAGaction* mya_vtclus;
  TAGaction* mya_vttrack;
  TAGaction* mya_vtvtx;

};

#endif

#ifdef RecoTools_cxx

RecoTools::~RecoTools() {}

#endif

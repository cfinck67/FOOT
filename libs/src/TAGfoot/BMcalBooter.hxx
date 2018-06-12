#ifndef BMcalBooter_hxx
#define BMcalBooter_hxx


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

#include "Evento.h"
// #include "Segnale.h"
#include "Geometry.h"
// #include "Trigger.h"

// #include "TAGpadGroup.hxx"
#include "TAGgeoTrafo.hxx"

// #include "TAGactTreeWriter.hxx"
// #include "TAGdataDsc.hxx"
#include "TAGroot.hxx"

#include "KFitter.hxx"
#include "GlobalPar.hxx"
#include "UpdatePDG.hxx"
#include "BM_struct.h"
#include "TABMdatRaw.hxx"

#include <ctime>
#include <time.h>


#define TDC_BOARDNUM 513

 

class BMcalBooter {
  
 public :

  BMcalBooter();
  ~BMcalBooter() {};

  void Initialize( TString wd_in,TString instr_in, bool isroma_in);
  void Process();
  void Finalize();

  void GeoPrint();
  //~ Bool_t CalculateT0();
  Bool_t read_event(bool &status);//read event from datafile and charge bmstruct, if status=true it means that this event contain some error, it return true if read the event, false if the file is end
  void evaluateT0();//evaluate the T0 from datafile
  Bool_t drop_event();//read event from datafile and discharge it, it return false if the file is end
  Long64_t GetData_numev(){return data_num_ev;};
  
  //Actions for Tupling DATA info for the various subdetectors
  void FillDataBeamMonitor();
  //~ void FillMCMSD(EVENT_STRUCT *myStr);

  //Monitors/Displays
  // void DisplayIRMonitor(TAGpadGroup* pg, EVENT_STRUCT *myStr);
  // void DisplayBeamMonitor(TAGpadGroup* pg);

  void initBMGeo(); //Beam Monitor Geometry
  void initBMCon();
  void initBMMap();
  
  
  //~ void bookHisto(TFile *f);
  //~ void CalibBMVT();
  //~ void MonitorBMVTMat();
  //~ void MonitorBMNew(Long64_t jentry);
  //~ void MonitorBM();

  TAGgeoTrafo* fGeoTrafo;
  TGeoVolume *top;
  //~ KFitter* m_kFitter;
  //~ fstream eventListFile;
  // time variables
  clock_t start_tot, end_tot;
  clock_t start_kal, end_kal;
  //~ double m_tempo_kal;
  //~ int m_nev;
  //~ int m_debug;
  //~ TFile* m_of;
  //~ TFile* m_hf;
  //~ TString m_oustr;
  //~ TString m_instr;
  TString m_wd;//working dir (from level0)
  TString m_instr;//input file name

  // vector<string> my_files;

  //Parameters
  TAGparaDsc* myp_bmgeo;
  TAGparaDsc* myp_bmcon;
  TAGparaDsc* myp_bmmap;
  shared_ptr<TAMSDparGeo> m_msdgeo;
  TAGparaDsc* myp_msdmap;
  TAGparaDsc* myp_msdcal;
  TAGparaDsc* myp_msdgeo;
  TAGparaDsc* myp_msdconf;
  
  TABMparCon* bmcon;
  //~ TABMparGeo* bmgeo;
  TABMparMap* bmmap;
  //~ TABMdatRaw* bmdatraw;
  
  //Data descriptions 
  TAGdataDsc* myn_bmraw;
  TAGdataDsc* myn_bmdatraw;
  TAGdataDsc* myn_bmtrk;
  TAGdataDsc* myn_msdraw;

  // TAGactTreeWriter* my_out;
  TAGaction* mya_msdraw;
  
  bool isroma;
  ifstream datafile;
  BM_struct bmstruct;
  Long64_t data_num_ev;
};

#endif

#ifndef BmBooter_hxx
#define BmBooter_hxx


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

// #include "TAGactTreeWriter.hxx"
// #include "TAGdataDsc.hxx"
#include "TCanvas.h"
#include "TH1F.h"
#include "TVector3.h"
#include "TObjString.h"
#include "TNamed.h"

#include "TAGroot.hxx"
#include "TAGview.hxx"
#include "GlobalPar.hxx"
#include "ControlPlotsRepository.hxx"
#include "TABMparGeo.hxx"
#include "TABMparCon.hxx"
#include "TABMactDatRaw.hxx"
#include "TABMactNtuRaw.hxx"
#include "TABMactNtuTrack.hxx"
#include "TABMdatRaw.hxx"
#include "TABMrawHit.hxx"
#include "TABMntuRaw.hxx"
#include "TABMntuTrack.hxx"
#include "TABMntuTrackTr.hxx"
#include "TABMvieTrackFOOT.hxx"
#include "BM_struct.h"

//Start Counter
#include "TAIRdatRaw.hxx"
#include "TAIRactDatRaw.hxx"

#define TDC_BOARDNUM 513

class BmBooter {
  
public :

BmBooter();
~BmBooter() {};

public:
  void Initialize( TString instr_in, Bool_t isdata_in);
  void Process();
  void Finalize();
  
  void FillDataBeamMonitor();
  void PrintSTrel();
  void Allign_estimate();
  void evaluateT0();//evaluate the T0 from datafile
  Bool_t drop_event();//read event from datafile and discharge it, it return false if the file is end
  void clear_bmstruct(Bool_t forced);  
  Bool_t read_event(Bool_t &status);//read event from datafile and charge bmstruct, if status=true it means that this event contain some error, it return true if read the event, false if the file is end
  Long64_t GetTotnumev(){return tot_num_ev;};
  Long64_t GetData_numev(){return data_num_ev;};
  void evaluate_cell_occupy(); //fill the cell_occupy matrix

  
private:

  //~ TH1F* histo1;
  //data descriptors
  TAGdataDsc* myn_bmdatraw;
  TAGdataDsc* myn_bmraw;
  TAGdataDsc* myn_stdatraw;
  TAGdataDsc* myn_bmtrk;

  //parameters
  TAGparaDsc* myp_bmcon;
  TAGparaDsc* myp_bmgeo;
  TAGparaDsc* myp_bmmap;
  
  //par objects
  TABMparCon* bmcon;
  TABMparGeo* bmgeo;
  TABMparMap* bmmap;
  
  //real data objects
  TABMdatRaw* bmdatraw;
  const TABMrawHit* bmdathit;//hit of tabmdatraw  
  TAIRdatRaw* stdatraw;
  
  //general data objects
  TABMntuRaw* bmnturaw;
  TABMntuHit* bmntuhit;//hit of tabmnturaw
  TABMntuTrack* bmntutrack;
  TABMntuTrackTr* bmntutracktr;//track of tabmntutrack

  //~ TABMvieTrackFOOT* pbmh_view;
  TAGpadGroup* pg;
  ControlPlotsRepository* m_controlPlotter;
  string bm_outputdir;   //FOOTRES/BeamMonitor
  string plot_name;

  vector<vector<Int_t>> cell_occupy;// occupancy of the cell for a given event, first index is the cellid, the value stored is the position of the hit in the bmnturaw vector
  TString m_instr;   //input file name
  Bool_t isdata;     //to separate MC and data
  Bool_t isroma;     //true if the input data is the BM refurbishment data taken in 2017 in Rome
  ifstream datafile;
  BM_struct bmstruct;
  Long64_t tot_num_ev;//current number of events
  Long64_t data_num_ev;//total number of events

};

#endif

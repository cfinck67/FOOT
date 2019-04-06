#ifndef BmBooter_hxx
#define BmBooter_hxx


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <ctime>    

#include "Evento.h"
// #include "Segnale.h"
// #include "Geometry.h"
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
#include "TAGactTreeWriter.hxx"

class BmBooter {
  
public :

BmBooter();
~BmBooter() {};

public:
  
  //initialize
  void Initialize( TString instr_in, Bool_t isdata_in, EVENT_STRUCT* evStr_in);
  void evaluateT0();//evaluate the T0 from datafile
  Int_t GetNentries(Int_t m_nev);
    
  //process
  void Process();
  void FillDataBeamMonitor();
  void Projecttracktr(); //to save the tracktr2dprojects matrix
  void ResidualDistance();//to save the residual_distance matrix
  Bool_t MCxEvent();// to save the mcxevent matrix
  Double_t EvalUnif(Double_t min, Double_t max){return 100.*(1.-((max-min)/(max+min)));};//evaluate the uniformity
  
  //read data event
  Bool_t read_event(Bool_t);//read an event from the datafile and charge bmstruct, it returns true if it read the event, false if the file is end
  Bool_t drop_event();//read an event from datafile and discharge it, it return false if the file is end
  void monitorQDC(vector<Int_t>& adc792_words);
  void clear_bmstruct(Bool_t forced);  
  void PrintBMstruct();// to print the content of bmstruct  
  
  //finalize
  void Finalize();
  void PrintSTrel(); //print the st relations in RecoHistos
  void PrintFromControlPlots();//print graphs derived from the controlplots
  void PrintEFFpp(); //print the efficiency evaluation with the pivot-probe method
  void PrintProjections();// print the projected fitted tracks saved in tracktr2dprojects
  void PrintResDist();//print the residual_distance matrix
  void PrintMCxEvent();//print the mcxevent matrix
  void LegendrePoly();//to plot the legendre poly (r,theta) th2d
  void Allign_estimate();  //estimate the bm allignment with the residual methods and print the results  
  void evaluate_cell_occupy(); //fill the cell_occupy matrix
  void efficiency_pivot_probe();//evaluation of the efficiency with the eff_pp matrix (pivot-probe method), made with the cell_occupy matrix
  void efficiency_paoloni();//evaluation of the efficiency with the "Paoloni" method 
  void efficiency_fittedplane();//evaluation of the efficiency with the "Paoloni" method on fitted tracks
  void efficiency_fittedtracks();//evaluate the efficiency on the fitted tracks
  void fit_histos(); //to fit the Recohistos output
  

  //getters
  Long64_t GetTotnumev(){return tot_num_ev;};
  Long64_t GetData_numev(){return data_num_ev;};
  Int_t GetAcqStartEv(){return acq_start_ev;};
  
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
  //~ TAGactTreeWriter* outTree;
  EVENT_STRUCT* evStr;  //MC event struct
  string bm_outputdir;   //FOOTRES/BeamMonitor
  string plot_name;
  vector<vector<Int_t>> cell_occupy;// occupancy of the cell for a given event, first index is the cellid, the value stored is the position of the hit in the bmnturaw vector
  TString m_instr;   //input file name
  TString m_nopath_instr;   //input file name without path
  Bool_t isdata;     //to separate MC and data
  Bool_t isallign;   //true if the allign parameters can be shown
  ifstream datafile;
  BM_struct bmstruct;
  TVector3 mylar1realpos; //only for MC 
  TVector3 mylar2realpos; //only for MC
  Int_t MC_track; //0=no MC track; 1=ok 1 MC track 
  Int_t track_ok;  //-2=maxnhit_cut; -1=minhit_cut; otherwise=track_status 
  
  Long64_t tot_num_ev;  //total number of events
  Long64_t data_num_ev; //current number of events
  Long64_t data_sync_num_ev; //current number of events + number of sync
  Int_t    acq_start_ev;//acquisition start event
  
  vector<vector<Int_t>> eff_pp; //efficiency calculation with the pivot-probe method, eff_pp[0]=pivot counter row, eff_pp[1]=probe counter row, each row is made by 16 elements, ordered as the cellindex, wihtout the non probe cells 
  vector<vector<Int_t>> eff_plane;//efficieny with the "Paoloni" method 
  vector<vector<Int_t>> eff_fittedplane;//efficieny with the "Paoloni" method only on fitted tracks

  //provv!!!!!! this is a very dummy method, to be used provv!!!!
  vector<vector<Double_t>> tracktr2dprojects;//projections of tracks:0=data_num_ev, 1=pvers.X, 2=R0.X, 3=pvers.Y, 4=R0.Y
  vector<vector<Double_t>> residual_distance;//if(isSelected): 0=data_num_ev, 1=cell_index, 2=drift_time, 3=distance, 4=residual;  otherwise 0=data_num_ev 1=cellindex, 2=drift_time, 3=dist,
  vector<vector<Double_t>> mcxevent;//data from mc:0=data_num_ev, 1=mylar1.X, 2=mylar1.Y, 3=mylar2.X, 4=mylar2.Y
  TString txt_outputname;//txt output
  double mylar1onlinex;
  double mylar1onliney;
  double mylar1onlinez;
  int counter_fast;
  int counter_track;
};

#endif

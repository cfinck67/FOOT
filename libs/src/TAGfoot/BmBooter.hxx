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

class BmBooter {
  
public :

BmBooter();
~BmBooter() {};

public:
  void Initialize( TString instr_in, Bool_t isdata_in);
  void Process();
  void Finalize();
  
  void FillDataBeamMonitor();
  void PrintSTrel(); //print st relations in RecoHistos
  void PrintEFFpp(); //print the efficiency evaluation with the pivot-probe method
  void PrintProjections();// print the projected fitted tracks on mylar1 and mylar2 th2d (mylarprojects)
  void PrintResDist();//print the residual vs distance (residual_distance)
  void Allign_estimate();  //estimate the bm allignment with the residual methods and print the results  
  void evaluateT0();//evaluate the T0 from datafile
  Bool_t drop_event();//read event from datafile and discharge it, it return false if the file is end
  void clear_bmstruct(Bool_t forced);  
  void PrintBMstruct();// to print the content of bmstruct  
  Bool_t read_event(Bool_t);//read event from datafile and charge bmstruct, if status=true it means that this event contain some error, it return true if read the event, false if the file is end
  void evaluate_cell_occupy(); //fill the cell_occupy matrix
  void monitorQDC(vector<Int_t>& adc792_words);
  void efficiency_pivot_probe();//evaluation of the efficiency with the eff_pp matrix (pivot-probe method), made with the cell_occupy matrix
  void efficiency_paoloni();//evaluation of the efficiency with the "Paoloni" method 
  void efficiency_fittedplane();//evaluation of the efficiency with the "Paoloni" method on fitted tracks
  void Projectmylars(); //to save the mylar1 and mylar2 xy positions in mylarprojects
  void ResidualDistance();//to save the residual_distance matrix

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
  string bm_outputdir;   //FOOTRES/BeamMonitor
  string plot_name;
  vector<vector<Int_t>> cell_occupy;// occupancy of the cell for a given event, first index is the cellid, the value stored is the position of the hit in the bmnturaw vector
  TString m_instr;   //input file name
  Bool_t isdata;     //to separate MC and data
  Bool_t isroma;     //true if the input data is the BM refurbishment data taken in 2017 in Rome
  Bool_t isallign;   //true if the allign parameters can be shown
  ifstream datafile;
  BM_struct bmstruct;
  Long64_t tot_num_ev;  //total number of events
  Long64_t data_num_ev; //current number of events
  Int_t    acq_start_ev;//acquisition start event
  vector<vector<Int_t>> eff_pp; //efficiency calculation with the pivot-probe method, eff_pp[0]=pivot counter row, eff_pp[1]=probe counter row, each row is made by 16 elements, ordered as the cellindex, wihtout the non probe cells 
  vector<vector<Int_t>> eff_plane;//efficieny with the "Paoloni" method 
  vector<vector<Int_t>> eff_fittedplane;//efficieny with the "Paoloni" method only on fitted tracks

  //provv!!!!!! this is a very dummy method, to be used provv!!!!
  vector<vector<Double_t>> mylarprojects;//projections of tracks: mylar1.X=mylarprojects[i][0]; mylar1.Y=mylarprojects[i][1]; mylar2.X=mylarprojects[i][3]; mylar2.X=mylarprojects[i][4]; i=tracks
  vector<vector<Double_t>> residual_distance;//0=residual, 1=distance, 2=drift_time

};

#endif

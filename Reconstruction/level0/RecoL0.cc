#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TMath.h>
#include <TStyle.h>
#include <TF1.h>
#include <TLatex.h>
#include <TError.h>
#include <TVectorD.h>
#include <TApplication.h>
#include <TGraphErrors.h>

#include "TAGroot.hxx"

#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

#include "RecoTools.h"
#include "GlobalPar.hxx"
#include "ControlPlotsRepository.hxx"


//#include "MCTree.h"

using namespace std;

// extern TAGparGlobal  *gPar; 

int main (int argc, char *argv[]) {

  clock_t start_tot, end_tot;

  //  gROOT->SetBatch(kTRUE);
  gROOT->SetStyle("Plain"); 
  
  char name[200];
  TString out("DecodeMC.root");
  TString reco("recohistos.root");
  TString treeoutname("bmtree.root");
  TString bmconfile("/config/beammonitor.cfg");
  TString in("../data/mc/MC_ID040_Evt1k.root");
  TString wdir(".");
  int nTotEv = 0;
  int debug = 0; int pl_freq(100);
  int ke_display = 0;
  int evstart=0;
  bool isdata(kFALSE); //0=MC, 1=real data analysis 
  bool alist(kFALSE);
  bool printtree=false;
  for (int i = 0; i < argc; i++){
    if(strcmp(argv[i],"-bmconfile") == 0)  { bmconfile =TString(argv[++i]);  }              // file name for the beammonitor config file
    if(strcmp(argv[i],"-reco") == 0)  { reco =TString(argv[++i]);  }                       // Root file name for recohistos output
    if(strcmp(argv[i],"-tree") == 0)  { treeoutname =TString(argv[++i]); printtree=true;}                // Root file name for ttree ntupleout output
    if(strcmp(argv[i],"-out") == 0)   { out =TString(argv[++i]);  }                        // Root file name for output
    if(strcmp(argv[i],"-in") == 0)    { in = TString(argv[++i]);  }                        // Root file in input
    if(strcmp(argv[i],"-wd") == 0)    { wdir = TString(argv[++i]);  }                      // Working dir
    if(strcmp(argv[i],"-deb") == 0)   { debug = atoi(argv[++i]);      }                    // Enables debugging
    if(strcmp(argv[i],"-evstart") == 0){ evstart = atoi(argv[++i]);      }                 // Start event number
    if(strcmp(argv[i],"-nev") == 0)   { nTotEv = atoi(argv[++i]);      }                   // Number of events to be analized
    if(strcmp(argv[i],"-pl_fr") == 0) { pl_freq = atoi(argv[++i]);    }                  // Plot Frequency Number 
    if(strcmp(argv[i],"-list") == 0)  { alist = kTRUE;                 }                  // Input File is a list
    if(strcmp(argv[i],"-data") == 0)  { isdata = kTRUE;               }          // iteration on data/MC

    if(strcmp(argv[i],"-help") == 0)  { 
      cout<<" Decoder help:"<<endl;
      cout<<" Ex: Decoder [opts] "<<endl;
      cout<<" possible opts are:"<<endl;
      cout<<"      -out path/file : [def=DecodedMC.root] Root output file"<<endl;
      cout<<"      -in path/file  : [def=../data/mc/MC_ID040_Evt1k.root] ROOT input file"<<endl;
      cout<<"      -deb value     : [def=0] Enables debugging. Values 1,2 are allowed"<<endl;
      cout<<"      -nev value     : [def=10^7] Numbers of events to process"<<endl;
      cout<<"      -pl_fr value   : [def=100] Produce an event display each _value_ events"<<endl;
      cout<<"      -reco  filename  : name of the output histograms recohistos.root file"<<endl;
      cout<<"      -tree  filename  : name of the output ttree recotree.root file"<<endl;
      cout<<"      -bmconfile  path/filename  : name of the bm config filename, default= /config/beammonitor.cfg"<<endl;
      return 1;
    }
  }
  
  // start time
  start_tot = clock();

  TApplication::CreateApplication();

  TAGroot tagroot;
  GlobalPar::Instance();
  string outputfilename=reco.Data();
  GlobalPar::GetPar()->SetOutputFile(outputfilename);
  string outputttreename=treeoutname.Data();
  GlobalPar::GetPar()->SetOutputntuple(outputttreename);
  GlobalPar::GetPar()->SetPrintOutputNtuple(printtree);
  //~ GlobalPar::GetPar()->Print();

  ControlPlotsRepository::Instance();

  // clean results dir, and recreate it
  if ( (string)getenv("FOOTRES") != "" ) {
    system(("rm -r "+(string)getenv("FOOTRES")).c_str());
    system(("mkdir "+(string)getenv("FOOTRES")).c_str());
  }
  else {
    cout << "ERROR >> main  ::  $FOOTRES environment variable not set. Try to lunch setupFoot.sh.\n";
    exit(0); 
  }

  cout<<endl<<endl<<"Input file="<<in.Data()<<"   output file="<<reco.Data()<<endl;
  
  /*
    ------------------- init the Decoder class -------------------
  */

  RecoTools d(debug,in,alist,out,wdir,nTotEv, evstart, isdata, bmconfile);

  d.RecoLoop(&tagroot,pl_freq);


  // stop time
  end_tot = clock();
  double tempo=((double)(end_tot-start_tot ))/CLOCKS_PER_SEC;
  int t_h = trunc( tempo/3600 );
  int t_m = trunc( fmod(tempo, 3600)/60 );
  int t_s = trunc( fmod(fmod(tempo, 3600), 60) );
  cout<< "Execution Time: "<< tempo << " seconds" << endl;
  cout<< "Execution Time in human units: "<< t_h <<" : "<< t_m <<" : "<< t_s << endl;
  
  double tempoKal = d.m_tempo_kal / CLOCKS_PER_SEC;
  int t_h2 = trunc( tempoKal/3600 );
  int t_m2 = trunc( fmod(tempoKal, 3600)/60 );
  int t_s2 = trunc( fmod(fmod(tempoKal, 3600), 60) );
  cout<< "Execution Time: "<< tempoKal << " seconds" << endl;
  cout<< "Execution Time in human units: "<< t_h2 <<" : "<< t_m2 <<" : "<< t_s2 << endl<<endl;
  cout<<" your reconstruction output file is:  "<<reco.Data()<<endl;
  if(printtree)
    cout<<" your ttree output file is:  "<<treeoutname.Data()<<endl;
  cout<<endl<<" Job done! "<<endl;
  return 0;
} 
















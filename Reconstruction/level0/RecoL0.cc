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


//#include "MCTree.h"

using namespace std;

// extern TAGparGlobal  *gPar; 

int main (int argc, char *argv[]) {

  //  gROOT->SetBatch(kTRUE);
  gROOT->SetStyle("Plain"); 
  
  char name[200];
  TString out("DecodedMC.root");
  TString in("../data/mc/MC_ID040_Evt1k.root");
  TString wdir("./");
  int nTotEv = 0;
  int debug = 0; int pl_freq(100);
  int ke_display = 0;
  TString outH("DecodeMC_histo.root");
  bool alist(kFALSE);
  for (int i = 0; i < argc; i++){
    if(strcmp(argv[i],"-out") == 0)   { out =TString(argv[++i]);  }                        // Root file name for output
    if(strcmp(argv[i],"-outh") == 0)  { outH =TString(argv[++i]);  }                      // Root file name for histo output
    if(strcmp(argv[i],"-in") == 0)    { in = TString(argv[++i]);  }                        // Root file in input
    if(strcmp(argv[i],"-wd") == 0)    { wdir = TString(argv[++i]);  }                      // Working dir
    if(strcmp(argv[i],"-deb") == 0)   { debug = atoi(argv[++i]);      }                    // Enables debugging
    if(strcmp(argv[i],"-nev") == 0)   { nTotEv = atoi(argv[++i]);      }                   // Number of events to be analized
    if(strcmp(argv[i],"-pl_fr") == 0) { pl_freq = atoi(argv[++i]);    }                  // Plot Frequency Number 
    if(strcmp(argv[i],"-list") == 0)  { alist = kTRUE;                 }                  // Input File is a list

    if(strcmp(argv[i],"-help") == 0)  { 
      cout<<" Decoder help:"<<endl;
      cout<<" Ex: Decoder [opts] "<<endl;
      cout<<" possible opts are:"<<endl;
      cout<<"      -out path/file : [def=DecodedMC.root] Root output file"<<endl;
      cout<<"      -in path/file  : [def=../data/mc/MC_ID040_Evt1k.root] ROOT input file"<<endl;
      cout<<"      -deb value     : [def=0] Enables debugging. Values 1,2 are allowed"<<endl;
      cout<<"      -nev value     : [def=10^7] Numbers of events to process"<<endl;
      cout<<"      -pl_fr value   : [def=100] Produce an event display each _value_ events"<<endl;
      return 1;
    }
  }

  TApplication::CreateApplication();

  TAGroot tagroot;

  GlobalPar::Instance();
  GlobalPar::GetPar()->Print();

  cout<<"Opening:  outp file:: "<<out.Data()<<" input file:: "<<in.Data()<<endl;
  
  /*
    ------------------- init the Decoder class -------------------
  */

  TFile *hF = new TFile(outH.Data(),"RECREATE"); hF->cd();

  RecoTools d(debug,in,alist,out,wdir,nTotEv, hF);

  d.bookHisto(hF);

  d.RecoLoop(&tagroot,pl_freq);

  hF->cd();

  hF->Write();
  hF->Close();

  cout<<" Job done! "<<endl;
  return 0;
} 


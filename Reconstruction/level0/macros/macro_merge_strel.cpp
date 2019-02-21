#include "macro_provv_strel.h"
#include "macro_strel_functions.cpp"

using namespace std;
using namespace TMath;

void macro_merge_strel(){
  gROOT->SetBatch(kTRUE);//turn off graphical output on screen
  
  char tmp_char[200];
  TString out_filename("Outstrel_merged.root");
  TFile *f_out = new TFile(out_filename.Data(),"RECREATE");
  
  vector<TString> input_filename;
  TString filename;
  filename="./Outstrel_228.root";
  input_filename.push_back(filename);
  filename="./Outstrel_80.root";
  input_filename.push_back(filename);
    
  BookingBMMSD(f_out, false, true);

  //merge the graphs
  for(int fileindex=0;fileindex<input_filename.size();fileindex++){
    TFile *infile = new TFile(input_filename.at(fileindex).Data(),"READ");
    if(!infile->IsOpen()){
      cout<<"I cannot open "<<filename.Data()<<endl; return;
    }    
    cout<<"Merging input file="<<input_filename.at(fileindex).Data()<<endl;
    cout<<"output file="<<out_filename.Data()<<endl;
    
    merge_graphics(infile, f_out);

    infile->Close();
    delete infile;
  }
  f_out->cd();
  
  
  if(debug>0)
    cout<<"Now I'll fit the strel_rdrift and strel_tdrift"<<endl;
    
  TF1  *gaus = new TF1("gaus","gaus",-0.4,0.2);  
  vector<double> rdrift_fitmean(STBIN,0.), tdrift_fitmean(STBIN,0.), rdrift_fitstdev(STBIN,0.), tdrift_fitstdev(STBIN,0.);
  for(int i=0; i<STBIN;i++){
    gaus->SetParameter(2,-0.1);
    gaus->SetParameter(3,0.15);
    sprintf(tmp_char,"Res_vs_rdrift/strel_rdrift_%d", i);
    ((TH1D*)gDirectory->Get(tmp_char))->Fit("gaus","QMB","",-0.4,0.2);
    rdrift_fitmean.at(i)=gaus->GetParameter(1);
    rdrift_fitstdev.at(i)=gaus->GetParameter(2);
    gaus->SetParameter(2,-0.1);
    gaus->SetParameter(3,0.15);
    sprintf(tmp_char,"Res_vs_tdrift/strel_tdrift_%d", i);
    ((TH1D*)gDirectory->Get(tmp_char))->Fit("gaus","QMB","",-0.4,0.2);
    tdrift_fitmean.at(i)=gaus->GetParameter(1);
    tdrift_fitstdev.at(i)=gaus->GetParameter(2);
  }  
  
  if(debug>0)
    cout<<"Now I'll draw the strel th2d"<<endl;
  
  //draw the newstrel merged from space and time residuals
  fitStrel(f_out, 0);
  TF1* oldstrelpol= (TF1*)(((TH2D*)gDirectory->Get("old_strel_pfx"))->GetFunction("poly"));
  for(int i=0;i<STBIN;i++){
    sprintf(tmp_char,"Res_vs_rdrift/strel_rdrift_%d", i);
    ((TH2D*)gDirectory->Get("new_strel_rdrift"))->Fill(i*3.,oldstrelpol->Eval(i*3.)+((TH1D*)gDirectory->Get(tmp_char))->GetMean());
    sprintf(tmp_char,"Res_vs_tdrift/strel_tdrift_%d", i);
    ((TH2D*)gDirectory->Get("new_strel_time"))->Fill(oldstrelpol->Eval(i*3.)+((TH1D*)gDirectory->Get(tmp_char))->GetMean(), i*3.);

    //~ ((TH2D*)gDirectory->Get("new_strel_rdrift"))->Fill(i*3.,oldstrelpol->Eval(i*3.)+rdrift_fitmean.at(i));
    //~ ((TH2D*)gDirectory->Get("new_strel_time"))->Fill(oldstrelpol->Eval(i*3.)+tdrift_fitmean.at(i), i*3.);  
  
  } 
  fitStrel(f_out,1);
  fitStrel(f_out,2);
  
  f_out->Write();
  //~ f_out->Close();
  gROOT->SetBatch(kFALSE);//enable graphical output  
  new TBrowser();
  
  return 0;
}


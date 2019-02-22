#include "macro_mcvertex_strel.h"
#include "macro_mcvertex_stfunc.cpp"

using namespace std;
using namespace TMath;

void macro_mcvertex_strel(){
  gROOT->SetBatch(kTRUE);//turn off graphical output on screen
  
  TString out_filename("Outstrel_mcvertex.root");
  //~ TString bmin_filename("RecoTree_nosmearrdrift_nosmearhits_w0_strel_primZ-40_BM_X0.2_Y0.1_Z_-14_Xang1.5_VTX_Y0.2_Z1.5.root");
  //~ TString vtxin_filename("bmvertex_strel_primZ-40_BM_X0.2_Y0.1_Z_-14_Xang1.5_VTX_Y0.2_Z1.5_Outprovayun.root");
  //~ TString bmin_filename("RecoTree_dritto.root");
  //~ TString vtxin_filename("dritto_txt_Outyun.root");
  //~ TString bmin_filename("RecoTree_largo_dritto.root");
  TString bmin_filename("RecoTree_prova.root");
  TString vtxin_filename("largo_dritto_Outyun.root");
  
  TFile *f_out = new TFile(out_filename.Data(),"RECREATE");  
  TFile *bminfile = new TFile(bmin_filename.Data(),"READ");  
  TFile *vtxinfile = new TFile(vtxin_filename.Data(),"READ");
  
  if(!bminfile->IsOpen()){
    cout<<"I cannot open "<<bmin_filename.Data()<<endl; return;
  }
  if(!vtxinfile->IsOpen()){
    cout<<"I cannot open "<<vtxin_filename.Data()<<endl; return;
  }
  
  
  cout<<"Beam Monitor input file="<<bmin_filename.Data()<<endl;
  cout<<"vtx input file="<<vtxin_filename.Data()<<endl;
  cout<<"output file="<<out_filename.Data()<<endl;

  BookingBMVTX(f_out, false, false);
  
  //******************************************BM stuff****************************************
  //bm geo
  vector<TVector3> wire_pos, wire_dir;//wire position and direction in isocenter system of ref.
  setbmgeo(wire_pos, wire_dir);
  //~ TVector3 BMshift(BMSHIFTX, BMSHIFTY, BMSHIFTZ);
  //~ TVector3 BMrot(BMYZANGLE, BMXZANGLE,0.);
  
  //read event bm eventtree stuff
  bminfile->cd();
  TTreeReader bmReader("EventTree", bminfile);
  TTreeReaderValue<int> evnumreader(bmReader, "evnum");
  TTreeReaderValue<int> timeacqreader(bmReader, "timeacq");
  TTreeReaderValue<double> trackchi2reader(bmReader, "BM_track_chi2");
  TTreeReaderValue<double> pversxreader(bmReader, "BM_track_PversX");
  TTreeReaderValue<double> pversyreader(bmReader, "BM_track_PversY");
  TTreeReaderValue<double> pverszreader(bmReader, "BM_track_PversZ");
  TTreeReaderValue<double> r0xreader(bmReader, "BM_track_R0X");
  TTreeReaderValue<double> r0yreader(bmReader, "BM_track_R0Y");
  TTreeReaderValue<double> rdriftreader(bmReader, "BM_hitrdrift");
  TTreeReaderValue<double> residualreader(bmReader, "BM_hitresidual");
  TTreeReaderValue<double> hittimereader(bmReader, "BM_hittime");
  TTreeReaderValue<int> planereader(bmReader, "BM_hitplane");
  TTreeReaderValue<int> viewreader(bmReader, "BM_hitview");
  TTreeReaderValue<int> cellreader(bmReader, "BM_hitcell");
  
  BM_evstruct bmevent;
  vector<BM_evstruct> allbmeventin;
  clean_bmevstruct(bmevent, true);
  
  
  //read BM loop
  while(bmreadevent(bmReader, bmevent, evnumreader, timeacqreader, trackchi2reader, pversxreader, pversyreader, pverszreader, r0xreader, r0yreader, rdriftreader, residualreader, hittimereader, planereader, viewreader, cellreader)){
    allbmeventin.push_back(bmevent);
  };
  
  cout<<"BM read events="<<allbmeventin.size()<<"   total ttree events="<<bmReader.GetEntries(true)<<endl;
  if(bmmcstudy==0)
    bminfile->Close();

  //***************************************************** vtx stuff ***********************************
  vtx_evstruct vtxevent;
  vector<vtx_evstruct> allvtxeventin;
  clean_vtxevstruct(vtxevent);

//comment the following line to use the BM MC info and use "fake vtx data"!!!!!!!!!!!!!!!!
  if(bmmcstudy==0){
    vtxinfile->cd();
    TTreeReader vtxReader("EventTree", vtxinfile);
    TTreeReaderValue<int> evnum(vtxReader, "evnum");
    TTreeReaderValue<int> tracknum(vtxReader, "tracknum");
    TTreeReaderValue<double> chi2tot(vtxReader, "chi2tot");
    TTreeReaderValue<double> chi2uview(vtxReader, "chi2uview");
    TTreeReaderValue<double> chi2vview(vtxReader, "chi2vview");
    TTreeReaderValue<double> r0x(vtxReader, "r0x");
    TTreeReaderValue<double> r0y(vtxReader, "r0y");
    TTreeReaderValue<double> pversx(vtxReader, "pversx");
    TTreeReaderValue<double> pversy(vtxReader, "pversy");  
    //read vtx loop
    while(vtxreadevent(vtxevent, vtxReader, evnum, tracknum, chi2tot, chi2uview, chi2vview, r0x, r0y, pversx, pversy)){
      allvtxeventin.push_back(vtxevent);
    };
    cout<<"VTX read events="<<allvtxeventin.size()<<"  total ttree events="<<vtxReader.GetEntries(true)<<endl;  
    vtxinfile->Close();
  }else{//**************************************************** real BM MC ************************************
    //here I read the BM MC data and fill the allvtxeventin vector to simulate the vtx
    bminfile->cd();
    TTreeReader mcvtxReader("EventTree", bminfile);
    TTreeReaderValue<int> mcevnumreader(bmReader, "evnum");
    TTreeReaderValue<double> mcpversxreader(bmReader, "BM_MC_PversX");
    TTreeReaderValue<double> mcpversyreader(bmReader, "BM_MC_PversY");
    TTreeReaderValue<double> mcr0xreader(bmReader, "BM_MC_R0X");
    TTreeReaderValue<double> mcr0yreader(bmReader, "BM_MC_R0Y");
    while(bmMCreadevent(vtxevent, mcvtxReader, mcevnumreader, mcpversxreader, mcpversyreader, mcr0xreader, mcr0yreader)){
      allvtxeventin.push_back(vtxevent);
    };
    cout<<"BM MC read events="<<allvtxeventin.size()<<"   total ttree events="<<mcvtxReader.GetEntries(true)<<endl;
    bminfile->Close();
  }
  
  //****************************************************** evaluate strel *************************************
  vector<vector<double>> space_residual(STBIN+1);
  vector<vector<double>> time_residual(STBIN+1);
  vector<vector<int>> selected_index;//matrix with the two index in allbmeventin and allvtxeventin that are correlated
  vector<int> event_index(3,-1);//0=allbmeventin index, 1=allvtxeventin index, 2=combine code(0=bm && msd, 1=only msd1, 2=only msd2)
  for(int i=0;i<allbmeventin.size();i++){
    int k=allbmeventin.at(i).evnum;
    k+=allbmeventin.at(i).evnum<CHANGESHIFT ? NUMEVTSHIFT : NUMEVT2SHIFT;
    if(allbmeventin.at(i).bm_track_chi2<BMNCUT && allvtxeventin.size()>k && k>=0){
      if(allvtxeventin.at(k).vtx_track_chi2tot<1. && allvtxeventin.at(k).vtx_track_chi2uview<1. && allvtxeventin.at(k).vtx_track_chi2vview<1.){//there's a good track fitted both by msd and bm
        event_index.at(0)=i;
        event_index.at(1)=k;
        event_index.at(2)=0;
        selected_index.push_back(event_index);
        EvaluateSpaceResidual(space_residual, time_residual, allbmeventin.at(i), allvtxeventin.at(k), wire_pos, wire_dir);        
        //~ ((TH1D*)gDirectory->Get("time_diff"))->Fill(allbmeventin.at(i).timeacq-allvtxeventin.at(k).timeacq);
      }else if(allvtxeventin.at(k).vtx_track_chi2uview>1.){
        event_index.at(0)=i;
        event_index.at(1)=k;
        event_index.at(2)=1;
        selected_index.push_back(event_index);
      }else if (allvtxeventin.at(k).vtx_track_chi2vview>1.){
        event_index.at(0)=i;
        event_index.at(1)=k;
        event_index.at(2)=2;
        selected_index.push_back(event_index);
      }
    }
  }
  
  cout<<"number of combined events="<<selected_index.size()<<endl;
  
  //****************************************************** end of program, print! *********************************
  Printoutput(f_out, allbmeventin, allvtxeventin, space_residual, time_residual, selected_index, false);
  //~ fitPositionResidual();
  Allign_estimate();
  
  f_out->Write();
  //~ f_out->Close();
  gROOT->SetBatch(kFALSE);//enable graphical output  
  new TBrowser();
  
  return 0;
}




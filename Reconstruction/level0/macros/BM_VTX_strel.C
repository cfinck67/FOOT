#include "BM_VTX_strel_main.h"
#include "BM_VTX_strel_func.cpp"

using namespace std;
using namespace TMath;

void FillBmVME(TString name) {
   
   cout<<"start FillBmVME"<<endl;
   
   TAGparaDsc* bmGeo    = new TAGparaDsc(TABMparGeo::GetDefParaName(), new TABMparGeo());
   TABMparGeo* geomap   = (TABMparGeo*) bmGeo->Object();
   geomap->FromFile();
            
   TAGparaDsc* tgGeo = new TAGparaDsc(TAGparGeo::GetDefParaName(), new TAGparGeo());
   TAGparGeo* parGeo = (TAGparGeo*)tgGeo->Object();
   parGeo->FromFile();

   TAGparaDsc*  bmConf  = new TAGparaDsc("bmConf", new TABMparCon());
   TABMparCon* parConf = (TABMparCon*)bmConf->Object();
   TString parFileName = "./config/beammonitor.cfg";
   parConf->FromFile(parFileName.Data());
   parFileName = "./config/bmreso_vs_r.root";
   parConf->LoadReso(parFileName);
   parConf->loadT0s(10000000000);
   parConf->CoutT0();

   TAGparaDsc*  bmMap  = new TAGparaDsc("bmMap", new TABMparMap());
   TABMparMap*  parMap = (TABMparMap*)bmMap->Object();
   parFileName = "./geomaps/";
   parFileName += parConf->GetParmapfile();
   parMap->FromFile(parFileName.Data(), geomap);
     
   TABMntuTrack*  bmTrack = new TABMntuTrack();
   tree->SetBranchAddress(TABMntuTrack::GetBranchName(), &bmTrack);
   
   cout<<"end of FillBmVME"<<endl;
}

void BM_VTX_strel_main(){
  gROOT->SetBatch(kTRUE);//turn off graphical output on screen
  
  TString out_filename("Outstrel_mcvertex.root");

  TString in_filename("RecoTree_test_align_smearrdrift_garfield_firstreal1real.root");
    
  TFile *f_out = new TFile(out_filename.Data(),"RECREATE");  
  TFile *infile = new TFile(in_filename.Data(),"READ");  
  
  if(!infile->IsOpen()){
    cout<<"I cannot open "<<in_filename.Data()<<endl; 
    return;
  }
  
  cout<<"Input file="<<in_filename.Data()<<endl;
  cout<<"output file="<<out_filename.Data()<<endl;

  BookingBMVTX(f_out, false, false);

  //******************************************BM stuff****************************************
  
  //read event bm eventtree stuff
  infile->cd();
  TTreeReader bmReader("EventTree", infile);
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
  TTreeReaderValue<double> realrdriftreader(bmReader, "BM_MC_hit_realrdrift");
  
  BM_evstruct bmevent;
  vector<BM_evstruct> allbmeventin;
  clean_bmevstruct(bmevent, true);
  
  //read BM loop
  while(bmreadevent(bmReader, bmevent, evnumreader, timeacqreader, trackchi2reader, pversxreader, pversyreader, pverszreader, r0xreader, r0yreader, rdriftreader, residualreader, hittimereader, planereader, viewreader, cellreader, realrdriftreader)){
    allbmeventin.push_back(bmevent);
  };
  
  cout<<"BM read events="<<allbmeventin.size()<<"   total ttree events="<<bmReader.GetEntries(true)<<endl;
  infile->Close();

  //***************************************************** vtx stuff ***********************************
  vtx_evstruct vtxevent;
  vector<vtx_evstruct> allvtxeventin;
  clean_vtxevstruct(vtxevent);

//comment the following line to use the BM MC info and use "fake vtx data"!!!!!!!!!!!!!!!!
  if(bmmcstudy<=0){
    infile->cd();
    TTreeReader vtxReader("EventTree", infile);
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
    infile->Close();
  }
  
  //****************************************************** evaluate strel *************************************
  vector<vector<double>> space_residual(STBIN+1);
  vector<vector<double>> time_residual(STBIN+1);
  vector<vector<int>> selected_index;//matrix with the two index in allbmeventin and allvtxeventin that are correlated
  vector<int> event_index(3,-1);//0=allbmeventin index, 1=allvtxeventin index, 2=combine code(0=bm && msd, 1=only msd1, 2=only msd2)
  int last_vtxindex=0;
  for(int i=0;i<allbmeventin.size();i++){
    int k=allbmeventin.at(i).evnum;
    k+=allbmeventin.at(i).evnum<CHANGESHIFT ? NUMEVTSHIFT : NUMEVT2SHIFT;
    //~ cout<<"i="<<i<<"  allbmeventin.at(i).evnum="<<allbmeventin.at(i).evnum<<"  k="<<k<<endl;
    if(allbmeventin.at(i).bm_track_chi2<BMNCUT && k>=0){
      for(int l=last_vtxindex;l<allvtxeventin.size();l++){
        if(allvtxeventin.at(l).evnum==k){
          //~ cout<<"trovato traccia con stesso evnum: l="<<l<<"  allvtxeventin.at(l).evnum="<<allvtxeventin.at(l).evnum<<endl;
          if(allvtxeventin.at(l).vtx_track_chi2tot<1. && allvtxeventin.at(l).vtx_track_chi2uview<1. && allvtxeventin.at(l).vtx_track_chi2vview<1.){//there's a good track fitted both by msd and bm
            event_index.at(0)=i;
            event_index.at(1)=l;
            event_index.at(2)=0;
            selected_index.push_back(event_index);
            EvaluateSpaceResidual(space_residual, time_residual, allbmeventin.at(i), allvtxeventin.at(l), wire_pos, wire_dir);        
            //~ ((TH1D*)gDirectory->Get("time_diff"))->Fill(allbmeventin.at(i).timeacq-allvtxeventin.at(k).timeacq);
          }else if(allvtxeventin.at(l).vtx_track_chi2uview>1.){
            event_index.at(0)=i;
            event_index.at(1)=l;
            event_index.at(2)=1;
            selected_index.push_back(event_index);
          }else if (allvtxeventin.at(l).vtx_track_chi2vview>1.){
            event_index.at(0)=i;
            event_index.at(1)=l;
            event_index.at(2)=2;
            selected_index.push_back(event_index);
          }
          last_vtxindex=l;
          break;
        }
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
  cout<<"End of program"<<endl;
  
  return 0;
}




#include "macro_provv_strel.h"
#include "macro_strel_functions.cpp"

using namespace std;
using namespace TMath;

void macro_provv_strel(){
  gROOT->SetBatch(kTRUE);//turn off graphical output on screen
  
  if(false){//to read all the msd files
    readallMSDfile();
    return;
  }
  
  TString out_filename("Outstrel_228_chi2ext.root");
  TString bmin_filename("../strel_msddata/RecoTree_228MeV_HV2200_100kEv_400delayLong_1000delayShort_Xbello_chi2_ext.root");
  TString msdin_filename("../strel_msddata/1544818479_hits.root");
  //~ TString out_filename("Outstrel_80_chi2ext.root");
  //~ TString bmin_filename("../strel_msddata/RecoTree_80MeV_HV2200_100kEv_400delayLong_1000delayShort_Xbello_chi2_ext.root");
  //~ TString msdin_filename("../strel_msddata/1544818921_hits.root");

  //~ TString bmin_filename("../strel_msddata/RecoTree_80MeV_HV2200_100kEv_400delayLong_1000delayShort_Xbello_tiltx5deg_0shift.root");
  //~ TString msdin_filename("../strel_msddata/1544819626_hits.root");
  
  TFile *f_out = new TFile(out_filename.Data(),"RECREATE");  
  TFile *bminfile = new TFile(bmin_filename.Data(),"READ");  
  TFile *msdinfile = new TFile(msdin_filename.Data(),"READ");
  
  if(!bminfile->IsOpen()){
    cout<<"I cannot open "<<bmin_filename.Data()<<endl; return;
  }
  if(!msdinfile->IsOpen()){
    cout<<"I cannot open "<<msdin_filename.Data()<<endl; return;
  }
  
  
  cout<<"Beam Monitor input file="<<bmin_filename.Data()<<endl;
  cout<<"MSD input file="<<msdin_filename.Data()<<endl;
  cout<<"output file="<<out_filename.Data()<<endl;

  BookingBMMSD(f_out, false, false);
  
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
  
  
  //read msd loop
  bmReader.Next();
  while(bmreadevent(bmReader, bmevent, evnumreader, timeacqreader, trackchi2reader, pversxreader, pversyreader, pverszreader, r0xreader, r0yreader, rdriftreader, residualreader, hittimereader, planereader, viewreader, cellreader)){
    allbmeventin.push_back(bmevent);
  };
  
  cout<<"BM read events="<<allbmeventin.size()<<endl;
  bminfile->Close();

  //***************************************************** MSD stuff ***********************************

  //msd geo
  
  msdinfile->cd();
  TTreeReader msd1Reader("treeMSD1", msdinfile);
  TTreeReader msd2Reader("treeMSD2", msdinfile);
  TTreeReaderValue<double> x1msd(msd1Reader, "x12");
  TTreeReaderValue<double> y1msd(msd1Reader, "y12");
  TTreeReaderValue<double> x2msd(msd2Reader, "x16");
  
  //read msd
  MSD_evstruct msdevent;
  vector<MSD_evstruct> allmsdeventin;
  clean_msdevstruct(msdevent);
  
  if(msd1Reader.GetEntries(true)!=msd2Reader.GetEntries(true)){
    cout<<"ERROR!!!! The two msd file have different number of entries: msd1Reader.GetEntries()="<<msd1Reader.GetEntries(true)<<"   msd2Reader.GetEntries()="<<msd2Reader.GetEntries(true)<<endl<<"The program will be stoped"<<endl;
    return;
  }
  
  //read msd loop
  while(msdreadevent(msdevent, msd1Reader, msd2Reader, x1msd, y1msd, x2msd)){
    allmsdeventin.push_back(msdevent);
  };
  
  cout<<"MSD read events="<<allmsdeventin.size()<<endl;  
  msdinfile->Close();
  
  
  
  
  
  
  
  //****************************************************** evaluate strel *************************************
  vector<vector<double>> space_residual(STBIN+1);
  vector<vector<double>> time_residual(STBIN+1);
  vector<vector<int>> selected_index;//matrix with the two index in allbmeventin and allmsdeventin that are correlated
  vector<int> event_index(3,-1);//0=allbmeventin index, 1=allmsdeventin index, 2=combine code(0=msd1 && msd2, 1=only msd1, 2=only msd2)
  for(int i=0;i<allbmeventin.size();i++){
    int k=allbmeventin.at(i).evnum;
    k+=allbmeventin.at(i).evnum<CHANGESHIFT ? NUMEVTSHIFT : NUMEVT2SHIFT;
    if(allbmeventin.at(i).bm_track_chi2<BMNCUT){
      if(allmsdeventin.at(k).trackset==true){//there's a good track fitted both by msd and bm
        event_index.at(0)=i;
        event_index.at(1)=k;
        event_index.at(2)=0;
        selected_index.push_back(event_index);
        EvaluateSpaceResidual(space_residual, time_residual, allbmeventin.at(i), allmsdeventin.at(k), wire_pos, wire_dir);        
        //~ ((TH1D*)gDirectory->Get("time_diff"))->Fill(allbmeventin.at(i).timeacq-allmsdeventin.at(k).timeacq);
      }else if(allmsdeventin.at(k).x1raw!=-999 && allmsdeventin.at(k).x2raw==-999){
        event_index.at(0)=i;
        event_index.at(1)=k;
        event_index.at(2)=1;
        selected_index.push_back(event_index);
      }else if (allmsdeventin.at(k).x1raw==-999 && allmsdeventin.at(k).x2raw!=-999){
        event_index.at(0)=i;
        event_index.at(1)=k;
        event_index.at(2)=2;
        selected_index.push_back(event_index);
      }
    }
  }
  
  
  //****************************************************** end of program, print! *********************************
  Printoutput(f_out, allbmeventin, allmsdeventin, space_residual, time_residual, selected_index, false);
  fitPositionResidual();
  Allign_estimate();
  
  f_out->Write();
  //~ f_out->Close();
  gROOT->SetBatch(kFALSE);//enable graphical output  
  new TBrowser();
  
  return 0;
}



void readallMSDfile(){
  
  TString out_filename("allmsdfile.root");
  TFile *f_out = new TFile(out_filename.Data(),"RECREATE");  
  //dummy variables
  vector<BM_evstruct> dummybm_evstruct_vec;
  vector<vector<double>> dummy_vecvecdouble;
  vector<vector<double>> dummy2_vecvecdouble;
  vector<vector<int>> dummy_vecvecint;  

  vector<TString> msd_input_filename;
  TString msdin_filename("../strel_msddata/1544818921_hits.root");
  msd_input_filename.push_back(msdin_filename);
  msdin_filename=("../strel_msddata/1544818479_hits.root");
  msd_input_filename.push_back(msdin_filename);
  msdin_filename=("../strel_msddata/1544819626_hits.root");
  msd_input_filename.push_back(msdin_filename);
  msdin_filename=("../strel_msddata/1544820767_hits.root");
  msd_input_filename.push_back(msdin_filename);
  msdin_filename=("../strel_msddata/1544821979_hits.root");
  msd_input_filename.push_back(msdin_filename);
  msdin_filename=("../strel_msddata/1544823209_hits.root");
  msd_input_filename.push_back(msdin_filename);
  
  
  BookingBMMSD(f_out, true, false);
  cout<<"output file="<<out_filename.Data()<<endl;

  //read msd
  MSD_evstruct msdevent;
  vector<MSD_evstruct> allmsdeventin;
  int totalmsdev=0;//number of total msd events   
  for(int i=0;i<msd_input_filename.size();i++){
    TFile *msdinfile = new TFile(msd_input_filename.at(i).Data(),"READ");
    if(!msdinfile->IsOpen()){
      cout<<"I cannot open "<<msd_input_filename.at(i).Data()<<endl; return;
    }
    cout<<"MSD input file="<<msd_input_filename.at(i).Data()<<endl;
      
    msdinfile->cd();
    TTreeReader msd1Reader("treeMSD1", msdinfile);
    TTreeReader msd2Reader("treeMSD2", msdinfile);
    TTreeReaderValue<double> x1msd(msd1Reader, "x12");
    TTreeReaderValue<double> y1msd(msd1Reader, "y12");
    TTreeReaderValue<double> x2msd(msd2Reader, "x16");
    
    clean_msdevstruct(msdevent);
    
    if(msd1Reader.GetEntries(true)!=msd2Reader.GetEntries(true)){
      cout<<"ERROR!!!! The two msd file have different number of entries: msd1Reader.GetEntries()="<<msd1Reader.GetEntries(true)<<"   msd2Reader.GetEntries()="<<msd2Reader.GetEntries(true)<<endl<<"The program will be stoped"<<endl;
      return;
    }
    
    //read msd loop
    while(msdreadevent(msdevent, msd1Reader, msd2Reader, x1msd, y1msd, x2msd)){
      allmsdeventin.push_back(msdevent);
    };
    
    cout<<"From msd file "<<msd_input_filename.at(i).Data()<<"  number of events read="<<allmsdeventin.size()-totalmsdev<<endl;
    totalmsdev=allmsdeventin.size();  
    Printoutput(f_out, dummybm_evstruct_vec, allmsdeventin, dummy_vecvecdouble,dummy2_vecvecdouble, dummy_vecvecint, true);
    msdinfile->Close();
    delete msdinfile;
  }
  
  fitMSDgraph();
  
  f_out->Write();
  //~ f_out->Close();
  gROOT->SetBatch(kFALSE);//enable graphical output  
  new TBrowser();
  
  return;
}



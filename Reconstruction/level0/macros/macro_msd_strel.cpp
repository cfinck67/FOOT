#include "macro_msd_strel.h"
#include "macro_msd_stfunc.cpp"

using namespace std;
using namespace TMath;

void macro_msd_strel(){
  gROOT->SetBatch(kTRUE);//turn off graphical output on screen
  
  TString out_filename("combined/combo_3points_1551900359.root");
  //~ TString out_filename("combined/combo_2points_1551900359.root");

  TString bmin_filename("recotrees/RecoTree_1551900359.root");
  TString vtxin_filename("msddata/80MeV/out_3points_1551900359.root");
  //~ TString vtxin_filename("msddata/80MeV/out_2points_1551900359.root");
    
  TFile *f_out = new TFile(out_filename.Data(),"RECREATE");  
  TFile *bminfile = new TFile(bmin_filename.Data(),"READ");  
  TFile *vtxinfile=nullptr; 
  if(bmmcstudy<=0)
    vtxinfile= new TFile(vtxin_filename.Data(),"READ");
  
  if(!bminfile->IsOpen()){
    cout<<"I cannot open "<<bmin_filename.Data()<<endl; 
    return;
  }
  if(bmmcstudy<=0)
    if(!vtxinfile->IsOpen()){
      cout<<"I cannot open "<<vtxin_filename.Data()<<endl; 
      return;
    }
  
  
  cout<<"Beam Monitor input file="<<bmin_filename.Data()<<endl;
  if(bmmcstudy<=0)
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
  TString tmp_tstring;
  
  BM_evstruct bmevent;
  vector<BM_evstruct> allbmeventin;
  clean_bmevstruct(bmevent, true);
  
  
  //read BM loop
  while(bmreadevent(bmReader, bmevent, evnumreader, timeacqreader, trackchi2reader, pversxreader, pversyreader, pverszreader, r0xreader, r0yreader, rdriftreader, residualreader, hittimereader, planereader, viewreader, cellreader)){
    allbmeventin.push_back(bmevent);
  };
  
  cout<<"BM read events="<<allbmeventin.size()<<"   total ttree events="<<bmReader.GetEntries(true)<<endl;
  bminfile->Close();

  //***************************************************** vtx stuff ***********************************
  vtx_evstruct vtxevent;
  vector<vtx_evstruct> allvtxeventin;
  clean_vtxevstruct(vtxevent);

//comment the following line to use the BM MC info and use "fake vtx data"!!!!!!!!!!!!!!!!
  if(bmmcstudy<=0){
    vtxinfile->cd();
    TTreeReader msdReader("treeMSD", vtxinfile);
    //~ TTreeReaderValue<int> timeacqMSDreader(msdReader, "timeacq");
    TTreeReaderValue<double> trackchi2MSDreader(msdReader, "logchi2");
    TTreeReaderValue<double> thetaMSDreader(msdReader, "theta");
    TTreeReaderValue<double> phiMSDreader(msdReader, "phi");
    TTreeReaderValue<double> r0xMSDreader(msdReader, "x0");
    TTreeReaderValue<double> r0yMSDreader(msdReader, "y0");
      //read vtx loop
      //~ while(vtxreadevent(vtxevent, vtxReader, evnum, tracknum, chi2tot, chi2uview, chi2vview, r0x, r0y, pversx, pversy)){
        //~ allvtxeventin.push_back(vtxevent);
      //~ };
      
    while(msdreadevent(vtxevent, msdReader,trackchi2MSDreader,thetaMSDreader,phiMSDreader,r0xMSDreader,r0yMSDreader)){
        allvtxeventin.push_back(vtxevent);
      };
    
    
    cout<<"VTX read events="<<allvtxeventin.size()<<"  total ttree events="<<msdReader.GetEntries(true)<<endl;  
    vtxinfile->Close();
  }else{//**************************************************** real BM MC ************************************
  
    //here I read the BM MC data and fill the allvtxeventin vector to simulate the vtx
    TFile *mcbminfile = new TFile(bmin_filename.Data(),"READ");  
    mcbminfile->cd();
    TTreeReader mcvtxReader("EventTree", mcbminfile);
    TTreeReaderValue<int> mcevnumreader(mcvtxReader, "evnum");
    TTreeReaderValue<double> mcpversxreader(mcvtxReader, "BM_MC_PversX");
    TTreeReaderValue<double> mcpversyreader(mcvtxReader, "BM_MC_PversY");
    TTreeReaderValue<double> mcr0xreader(mcvtxReader, "BM_MC_R0X");
    TTreeReaderValue<double> mcr0yreader(mcvtxReader, "BM_MC_R0Y");
    while(bmMCreadevent(vtxevent, mcvtxReader, mcevnumreader, mcpversxreader, mcpversyreader, mcr0xreader, mcr0yreader)){
      allvtxeventin.push_back(vtxevent);
    };
    cout<<"BM MC read events="<<allvtxeventin.size()<<"   total ttree events="<<mcvtxReader.GetEntries(true)<<endl;
    mcbminfile->Close();
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
          if(allvtxeventin.at(l).vtx_track_chi2tot<VTXCUT && allvtxeventin.at(l).status==1){//there's a good track fitted both by msd and bm
            event_index.at(0)=i;
            event_index.at(1)=l;
            event_index.at(2)=0;
            selected_index.push_back(event_index);
            EvaluateSpaceResidual(space_residual, time_residual, allbmeventin.at(i), allvtxeventin.at(l), wire_pos, wire_dir);        
            //~ ((TH1D*)gDirectory->Get("time_diff"))->Fill(allbmeventin.at(i).timeacq-allvtxeventin.at(k).timeacq);
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
  Allign_estimate(bmin_filename, vtxin_filename, out_filename);
  
  f_out->Write();
  //~ f_out->Close();
  gROOT->SetBatch(kFALSE);//enable graphical output  
  new TBrowser();
  cout<<"End of program"<<endl;
  
  return 0;
}




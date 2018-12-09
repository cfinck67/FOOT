#include "macro_strel.h"

using namespace std;
using namespace TMath;


void macro_strel(){
  
  TFile *f_out = new TFile("Outstrel.root","RECREATE");  
  TFile *bminfile = new TFile("../RecoTree_pos0.root","READ");  
  TFile *msdinfile = new TFile("provoyun.root","READ");  
  Booking(f_out);
  
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
  
 //~ double pversx, pversy, pversz, r0x, r0y;
  //set bm data
  //~ TTree *bmdata = (TTree*)bminfile->Get("EventTree");
  //~ TBranch *bra_evnum = bmdata->GetBranch("evnum");
  //~ bra_evnum->SetAddress(&evnum);
  //~ totentries=bra_evnum->GetEntries();
  //~ TBranch *bra_pversx = bmdata->GetBranch("BM_track_PverX");
  //~ bra_pversx->SetAddress(&pversx);
  //~ TBranch *bra_pversy = bmdata->GetBranch("BM_track_PverY");
  //~ bra_pversy->SetAddress(&pversy);
  //~ TBranch *bra_pversz = bmdata->GetBranch("BM_track_PverZ");
  //~ bra_pversz->SetAddress(&pversz);
  //~ TBranch *bra_r0x = bmdata->GetBranch("BM_track_R0X");
  //~ bra_r0x->SetAddress(&r0x);
  //~ TBranch *bra_r0y = bmdata->GetBranch("BM_track_R0Y");
  //~ bra_r0y->SetAddress(&r0y);  
  
  //read msd loop
  bmReader.Next();
  while(bmreadevent(bmReader, bmevent, evnumreader, timeacqreader, trackchi2reader, pversxreader, pversyreader, pverszreader, r0xreader, r0yreader, rdriftreader, residualreader, hittimereader, planereader, viewreader, cellreader)){
    allbmeventin.push_back(bmevent);
  };
  
  cout<<"BM read events="<<allbmeventin.size()<<endl;

  //***************************************************** MSD stuff ***********************************

  //msd geo
  TVector3 msd_targetpos(0.,0.,0.);
  TVector3 msd_firstplanepos(0.,0.,0.);
  TVector3 msd_secondplanepos(0.,0.,0.);
  TVector3 msd_thirdplanepos(0.,0.,0.);
  
  msdinfile->cd();
  TTreeReader msdReader("MSDEventTree", msdinfile);
  TTreeReaderValue<int> evnumMSDreader(msdReader, "evnum");
  TTreeReaderValue<int> timeacqMSDreader(msdReader, "timeacq");
  TTreeReaderValue<double> trackchi2MSDreader(msdReader, "MSD_track_chi2");
  TTreeReaderValue<double> thetaMSDreader(msdReader, "MSD_track_Theta");
  TTreeReaderValue<double> thetaerrMSDreader(msdReader, "MSD_track_ThetaErr");
  TTreeReaderValue<double> phiMSDreader(msdReader, "MSD_track_Phi");
  TTreeReaderValue<double> phierrMSDreader(msdReader, "MSD_track_PhiErr");
  TTreeReaderValue<double> r0xMSDreader(msdReader, "MSD_track_R0X");
  TTreeReaderValue<double> r0xerrMSDreader(msdReader, "MSD_track_R0Xerr");
  TTreeReaderValue<double> r0yMSDreader(msdReader, "MSD_track_R0Y");
  TTreeReaderValue<double> r0yerrMSDreader(msdReader, "MSD_track_R0Yerr");
  TTreeReaderValue<double> r0zMSDreader(msdReader, "MSD_track_R0Z");
  TTreeReaderValue<double> r0zerrMSDreader(msdReader, "MSD_track_R0Zerr");
  
  //read msd
  MSD_evstruct msdevent;
  vector<MSD_evstruct> allmsdeventin;//charge only the selected tracks!
  clean_msdevstruct(msdevent);
  
  //read msd loop
  while(msdreadevent(msdReader, msdevent,evnumMSDreader,timeacqMSDreader,trackchi2MSDreader,thetaMSDreader,thetaerrMSDreader,phiMSDreader,phierrMSDreader,r0xMSDreader,r0xerrMSDreader,r0yMSDreader,r0yerrMSDreader,r0zMSDreader,r0zerrMSDreader)){
    allmsdeventin.push_back(msdevent);
  };
  
  cout<<"MSD read events="<<allmsdeventin.size()<<endl;  
  
  //****************************************************** evaluate strel *************************************
  vector<vector<vector<double>>> space_residual(STBIN+1);
  int kindex=0;
  for(int i=0;i<allbmeventin.size();i++){
    for(int k=kindex;k<allmsdeventin.size();k++){
      if(allbmeventin[i].evnum==allmsdeventin[k].evnum){//there's a track fitted both by msd and bm
        kindex=k;
        EvaluateSpaceResidual(space_residual, allbmeventin[i], allmsdeventin[k], wire_pos, wire_dir);        
      }
    }
  }
  
  
  //****************************************************** end of program, print stuff! *********************************
  Printoutput(f_out, allbmeventin, allmsdeventin, space_residual);
  
  //~ //read msd data
  //~ msdinfile->GetObject("EventTree",ptree);
  
  

  
  return 0;
}



void Booking(TFile* f_out){

  f_out->cd();
  TH1D *h; 
  TH2D *h2;
  char tmp_char[200];
  
  //bm stuff
  h = new TH1D("bmnhitsxevent","number of hits x event ;N of hits;Events",36,0.,36.);
  h = new TH1D("bm_polar_angle","BM polar angle distribution ;AngZ(deg);Events",200,0.,1.);
  h2 = new TH2D("bmisopro","isocenter plane projections ;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  h2 = new TH2D("bm_msd1pro","bm track projection on msd 1 plane ;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  h2 = new TH2D("bm_msd2pro","bm track projection on msd 2 plane;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  h2 = new TH2D("bm_msd3pro","bm track projection on msd 3 plane;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  h2 = new TH2D("bmmylar1BMsys","mylar1 projections ;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  h2 = new TH2D("bmmylar2BMsys","mylar2 projections ;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  h2 = new TH2D("bmmylar1ISOsys","mylar2 projections ;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  h2 = new TH2D("bmmylar2ISOsys","mylar2 projections ;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  
  //msd stuff
  h = new TH1D("msd_polar_angle","MSD polar angle distribution ;AngZ(deg);Events",200,0.,1.);
  
  //combined stuff
  h2 = new TH2D("rdrift_vs_residual","residual calculation ;rdrift[cm];residual",STBIN,0.,1.,400, -0.5,0.5 );
  h = new TH1D("space_residual_error","error events ;rdrift;Events",STBIN,0.,1.);
  h2 = new TH2D("old_strel","old FIRST strel ;time;rdrift",400,0.,400.,1000, 0., 1.);
  h2 = new TH2D("new_strel","new strel ;time;rdrift",400,0.,400., 1000, 0., 1.);
  
  f_out->mkdir("Res_vs_rdrift");
  f_out->cd("Res_vs_rdrift");
  for(int i=0;i<STBIN;i++){
    sprintf(tmp_char,"strel_rdrift_%d", i);
    h = new TH1D(tmp_char,"strel residual ;Residual[cm];Events",600,-1.,1.);
  }
  f_out->cd("..");
  
  
  
  return;
}

void Printoutput(TFile* f_out, vector<BM_evstruct> &allbmeventin, vector<MSD_evstruct> &allmsdeventin, vector<vector<vector<double>>> &space_residual){
  f_out->cd();
  if(debug)
    cout<<"I'm in Printoutput:: allbmeventin.size():"<<allbmeventin.size()<<endl;
  TVector3 tmp_tvector3;
  char tmp_char[200];
  
  //BM events
  for(int i=0; i<allbmeventin.size();i++){
    ((TH1D*)gDirectory->Get("bmnhitsxevent"))->Fill(allbmeventin[i].hitnum);
    ((TH1D*)gDirectory->Get("bm_polar_angle"))->Fill(allbmeventin[i].bm_track_pvers.Theta()*RAD2DEG);
    tmp_tvector3=BMProjections(allbmeventin[i].bm_track_pvers, allbmeventin[i].bm_track_r0pos, BMISOZ, true);
    ((TH2D*)gDirectory->Get("bmisopro"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
     tmp_tvector3=BMProjections(allbmeventin[i].bm_track_pvers, allbmeventin[i].bm_track_r0pos, MSD1Z, true);
    ((TH2D*)gDirectory->Get("bm_msd1pro"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    tmp_tvector3=BMProjections(allbmeventin[i].bm_track_pvers, allbmeventin[i].bm_track_r0pos, MSD2Z, true);
    ((TH2D*)gDirectory->Get("bm_msd2pro"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    tmp_tvector3=BMProjections(allbmeventin[i].bm_track_pvers, allbmeventin[i].bm_track_r0pos, MSD3Z, true);
    ((TH2D*)gDirectory->Get("bm_msd3pro"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    tmp_tvector3=BMProjections(allbmeventin[i].bm_track_pvers, allbmeventin[i].bm_track_r0pos, -(BMN_LENGTH+BMN_MYL_THICK)/2., false);
    ((TH2D*)gDirectory->Get("bmmylar1BMsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    tmp_tvector3=BMProjections(allbmeventin[i].bm_track_pvers, allbmeventin[i].bm_track_r0pos, (BMN_LENGTH+BMN_MYL_THICK)/2., false);
    ((TH2D*)gDirectory->Get("bmmylar2BMsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    tmp_tvector3=BMProjections(allbmeventin[i].bm_track_pvers, allbmeventin[i].bm_track_r0pos, -(BMN_LENGTH+BMN_MYL_THICK)/2., true);
    ((TH2D*)gDirectory->Get("bmmylar1ISOsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    tmp_tvector3=BMProjections(allbmeventin[i].bm_track_pvers, allbmeventin[i].bm_track_r0pos, (BMN_LENGTH+BMN_MYL_THICK)/2., true);
    ((TH2D*)gDirectory->Get("bmmylar2ISOsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    
    for(int k=0;k<allbmeventin[i].hitnum;k++)
      ((TH2D*)gDirectory->Get("old_strel"))->Fill(allbmeventin[i].bm_hit_time[k],allbmeventin[i].bm_hit_rdrift[k]);
  }
  
  //MSD events
  for(int i=0; i<allmsdeventin.size();i++){
    ((TH1D*)gDirectory->Get("msd_polar_angle"))->Fill(allmsdeventin[i].msd_track_pvers.Theta());
  }
  
  //combined stuff
  for(int i=0; i<STBIN;i++){
    for(int k=0; k<space_residual.at(i).size();k++){
      ((TH2D*)gDirectory->Get("rdrift_vs_residual"))->Fill(i/STBIN,space_residual.at(i).at(k).at(0));
      sprintf(tmp_char,"Res_vs_rdrift/strel_rdrift_%d", i);
      ((TH1D*)gDirectory->Get(tmp_char))->Fill(space_residual.at(i).at(k).at(0));
    }
  }
  for(int k=0; k<space_residual.at(STBIN).size();k++)
      ((TH1D*)gDirectory->Get("space_residual_error"))->Fill(space_residual.at(STBIN).at(k).at(0));
  
  //try to estimate the new strel
  //no gaussian fit on the slice of rdrift for the moment
  int binpos;
  double tmp_double;
  for(int i=0; i<allbmeventin.size();i++){
    for(int k=0;k<allbmeventin[i].hitnum;k++){
      binpos=(int)(allbmeventin[i].bm_hit_rdrift[k]*STBIN);
      if(binpos<STBIN){
        sprintf(tmp_char,"Res_vs_rdrift/strel_rdrift_%d", binpos);
        tmp_double=((TH1D*)gDirectory->Get(tmp_char))->GetMean();
        ((TH2D*)gDirectory->Get("new_strel"))->Fill(allbmeventin[i].bm_hit_time[k],allbmeventin[i].bm_hit_rdrift[k]-tmp_double);
      }    
    }
  }
  
  //fit the new_strel
  TProfile *prof = ((TH2D*)gDirectory->Get("new_strel"))->ProfileX();
  TF1 *poly = new TF1("poly","pol5", 0, 400);
  prof->Fit("poly","Q");
  cout<<"new strel parameters:"<<endl;
  cout<<poly->GetParameter(0)<<" + ("<<poly->GetParameter(1)<<"*tdrift) + ("<<poly->GetParameter(2)<<"*tdrift*tdrift) + ("<<poly->GetParameter(3)<<"*tdrift*tdrift*tdrift) + ("<<poly->GetParameter(4)<<"*tdrift*tdrift*tdrift*tdrift) + ("<<poly->GetParameter(5)<<"*tdrift*tdrift*tdrift*tdrift*tdrift)"<<endl;
  
  return;  
}

//to be made with labo sys of ref. (with respect to isocenter) 
void EvaluateSpaceResidual(vector<vector<vector<double>>> &space_residual, BM_evstruct &bmevent, MSD_evstruct &msdevent, vector<TVector3> &wire_pos, vector<TVector3> &wire_dir){
  int binpos;
  double residual;
  vector<double> eventtoadd(2,-999.);
  for(int i=0;i<bmevent.hitnum;i++){
    binpos=(int)(bmevent.bm_hit_rdrift[i]*STBIN);
    if(binpos>STBIN-1) 
      binpos=STBIN;
    residual=bmevent.bm_hit_rdrift[i]-FindRdrift(msdevent.msd_track_pvers, msdevent.msd_track_r0pos, wire_pos.at(bmevent.bm_hit_cellid[i]), wire_dir.at(bmevent.bm_hit_cellid[i]));
    //~ if(fabs(residual)<0.8 && binpos!=STBIN){
    if(binpos!=STBIN){
      eventtoadd.at(0)=residual;
      eventtoadd.at(1)=bmevent.bm_hit_time[i];
      space_residual.at(binpos).push_back(eventtoadd);
    }else{
      if(debug>1)
        cout<<"ERROR in EvaluateSpaceResidual: residual="<<residual<<"  hit_id="<<bmevent.bm_hit_cellid[i]<<"  hit_rdrift="<<bmevent.bm_hit_rdrift[i]<<endl;
      eventtoadd.at(0)=bmevent.bm_hit_rdrift[i];
      eventtoadd.at(1)=bmevent.bm_hit_time[i];
      space_residual.at(STBIN).push_back(eventtoadd);
    }  
  }
  return;
}


//read the next bm event, return false if the file is finished, otherwise true
//at the end bmReader is on the new event
bool bmreadevent(TTreeReader &bmReader, BM_evstruct &bmevent, TTreeReaderValue<int> &evnumreader,   TTreeReaderValue<int> &timeacqreader, TTreeReaderValue<double> &trackchi2reader, TTreeReaderValue<double> &pversxreader,   TTreeReaderValue<double> &pversyreader, TTreeReaderValue<double> &pverszreader,   TTreeReaderValue<double> &r0xreader,   TTreeReaderValue<double> &r0yreader,   TTreeReaderValue<double> &rdriftreader,   TTreeReaderValue<double> &residualreader,   TTreeReaderValue<double> &hittimereader,   TTreeReaderValue<int> &planereader,  TTreeReaderValue<int> &viewreader,   TTreeReaderValue<int> &cellreader){
  
  if(debug)
    cout<<"I'm in bmreadevent"<<endl;
  
  clean_bmevstruct(bmevent, false);
  if(bmReader.GetCurrentEntry()==0)
    bmReader.Next(); 
   
  if(debug)
    cout<<"bmreadevent:: old evnum="<<bmevent.evnum<<",  new evnum="<<*evnumreader<<endl;  
   
  int evhitnum=0;
  do{ 
    if(bmevent.evnum!=*evnumreader && evhitnum!=0)
      cout<<"bmreadevent:: Something is wrong!!! the old evnum is "<<bmevent.evnum<<", the new evnum is "<<*evnumreader<<endl; 
    bmevent.evnum=*evnumreader;
    bmevent.timeacq=*timeacqreader;
    bmevent.bm_track_chi2=*trackchi2reader;
    bmevent.bm_track_pvers.SetXYZ(*pversxreader,*pversyreader,*pverszreader);
    bmevent.bm_track_r0pos.SetXYZ(*r0xreader,*r0yreader,0.);
    bmevent.bm_hit_rdrift[evhitnum]=*rdriftreader;
    bmevent.bm_hit_residual[evhitnum]=*residualreader;
    bmevent.bm_hit_time[evhitnum]=*hittimereader;
    bmevent.bm_hit_cellid[evhitnum]=GetBMNcell(*planereader, *viewreader,*cellreader);
    evhitnum++;
  }while(bmReader.Next() && bmevent.evnum==*evnumreader);
  bmevent.hitnum=evhitnum;
  
  if(debug)
    cout<<"bmreadevent finished"<<endl;
    
  if(debug>2)
    print_bmevstruct(bmevent);  

  if((bmReader.GetCurrentEntry()==bmReader.GetEntries(true)) || (BMNEV && bmevent.evnum>=BMNEV) )
    return false;  
    
  return true;
}

    
bool msdreadevent(TTreeReader &msdReader, MSD_evstruct &msdevent, TTreeReaderValue<int> &evnumMSDreader,   TTreeReaderValue<int> &timeacqMSDreader, TTreeReaderValue<double> &trackchi2MSDreader, TTreeReaderValue<double> &thetaMSDreader,   TTreeReaderValue<double> &thetaerrMSDreader, TTreeReaderValue<double> &phiMSDreader,   TTreeReaderValue<double> &phierrMSDreader,   TTreeReaderValue<double> &r0xMSDreader,   TTreeReaderValue<double> &r0xerrMSDreader,   TTreeReaderValue<double> &r0yMSDreader,   TTreeReaderValue<double> &r0yerrMSDreader,   TTreeReaderValue<double> &r0zMSDreader,  TTreeReaderValue<double> &r0zerrMSDreader){
  
  if(debug)
    cout<<"I'm in MSDreadevent"<<endl;
  
  //~ clean_msdevstruct(msdevent);
  msdReader.Next(); 
   
  if(debug)
    cout<<"msdreadevent::  old evnum="<<msdevent.evnum<<",  new evnum="<<*evnumMSDreader<<endl;  
   
  msdevent.evnum=*evnumMSDreader;
  msdevent.timeacq=*timeacqMSDreader;
  msdevent.msd_track_chi2=*trackchi2MSDreader;
  msdevent.msd_track_pvers.SetXYZ(0.,0.,1.);
  msdevent.msd_track_pvers.SetTheta(*thetaMSDreader);
  msdevent.msd_track_pvers.SetPhi(*phiMSDreader);
  msdevent.msd_track_r0pos.SetXYZ(*r0xMSDreader,*r0yMSDreader,*r0zMSDreader);
  msdevent.msd_track_r0err.SetXYZ(*r0xerrMSDreader,*r0yerrMSDreader,*r0zerrMSDreader);
  msdevent.thetaerr=*thetaerrMSDreader;
  msdevent.phierr=*phierrMSDreader;
  
  if(debug)
    cout<<"bmreadevent finished"<<endl;
  
  if(debug>2)
    print_msdevstruct(msdevent);
  
  if((msdReader.GetCurrentEntry()==msdReader.GetEntries(true)) || (MSDNEV && msdevent.evnum>=MSDNEV) )
    return false;
    
  return true;
}
  
void clean_bmevstruct(BM_evstruct &bmevstruct, bool forced){

if(forced){
  bmevstruct.evnum=-1;
  bmevstruct.timeacq=-1;
  bmevstruct.hitnum=0;
  bmevstruct.bm_track_chi2=-1;
  bmevstruct.bm_track_pvers.SetXYZ(0,0,0);
  bmevstruct.bm_track_r0pos.SetXYZ(0,0,0);
  for(int index=0;index<MAXBMHITNUM;index++){
    bmevstruct.bm_hit_rdrift[index]=-1;
    bmevstruct.bm_hit_residual[index]=-1;
    bmevstruct.bm_hit_time[index]=-1;
    bmevstruct.bm_hit_cellid[index]=-1;
  }
  return;
}
int index=0;
while(bmevstruct.bm_hit_rdrift[index]!=-1 && index<MAXBMHITNUM){
  bmevstruct.bm_hit_rdrift[index]=-1;
  bmevstruct.bm_hit_residual[index]=-1;
  bmevstruct.bm_hit_time[index]=-1;
  bmevstruct.bm_hit_cellid[index]=-1;
  index++;
}

return;
}

void clean_msdevstruct(MSD_evstruct &msdevstruct){

  msdevstruct.evnum=-1;
  msdevstruct.timeacq=-1;
  msdevstruct.msd_track_chi2=-1;
  msdevstruct.msd_track_pvers.SetXYZ(0,0,0);
  msdevstruct.msd_track_r0pos.SetXYZ(0,0,0);
  msdevstruct.msd_track_r0err.SetXYZ(999.,999.,999.);
  msdevstruct.thetaerr=999.;
  msdevstruct.phierr=999.;

return;
}

void print_bmevstruct(BM_evstruct &bmevstruct){
  cout<<"print bmevstruct"<<endl;
  cout<<"bm evnum="<<bmevstruct.evnum<<"  timeacq="<<bmevstruct.timeacq<<endl;
  cout<<"hitnum="<<bmevstruct.hitnum<<"  trackchi2="<<bmevstruct.bm_track_chi2<<"  pvers=("<<bmevstruct.bm_track_pvers.X()<<", "<<bmevstruct.bm_track_pvers.Y()<<", "<<bmevstruct.bm_track_pvers.Z()<<")  r0=("<<bmevstruct.bm_track_r0pos.X()<<", "<<bmevstruct.bm_track_r0pos.Y()<<", "<<bmevstruct.bm_track_r0pos.Z()<<endl;
  cout<<"loop on hits:"<<endl;
  for(int i=0;i<bmevstruct.hitnum;i++){
    cout<<"i="<<i<<"  cellid="<<bmevstruct.bm_hit_cellid[i]<<"  rdrift="<<bmevstruct.bm_hit_rdrift[i]<<"  residual="<<bmevstruct.bm_hit_residual[i]<<"  time="<<bmevstruct.bm_hit_time[i]<<endl;
  }  
  
return;  
}

void print_msdevstruct(MSD_evstruct &msdevstruct){

  cout<<"print_msdevstruct:"<<endl;
  cout<<"msd evnum="<<msdevstruct.evnum<<"  timeacq="<<msdevstruct.timeacq<<endl;
  cout<<"track chi2="<<msdevstruct.msd_track_chi2<<"  pvers=("<<msdevstruct.msd_track_pvers.X()<<", "<<msdevstruct.msd_track_pvers.Y()<<", "<<msdevstruct.msd_track_pvers.Z()<<")  r0=("<<msdevstruct.msd_track_r0pos.X()<<", "<<msdevstruct.msd_track_r0pos.Y()<<", "<<msdevstruct.msd_track_r0pos.Z()<<")  r0_err=("<<msdevstruct.msd_track_r0err.X()<<", "<<msdevstruct.msd_track_r0err.Y()<<", "<<msdevstruct.msd_track_r0err.Z()<<")   thetaerr="<<msdevstruct.thetaerr<<"  phierr="<<msdevstruct.phierr<<endl;

return;
}

//evaluate the projections
//pvers=particle direction, r0pos=where pvers is calculated, propos=position where I want to 
TVector3 BMProjections(TVector3 pvers, TVector3 r0pos, double proposz, bool global){
  TVector3 projection;
  if(global){
    TVector3 BMshift(BMSHIFTX, BMSHIFTY, BMSHIFTZ);
    r0pos+=BMshift;
    pvers.RotateX(BMYZANGLE*DEG2RAD);
    pvers.RotateY(BMXZANGLE*DEG2RAD);
  }
  projection.SetXYZ(pvers.X()/pvers.Z()*proposz+r0pos.X(),  pvers.Y()/pvers.Z()*proposz+r0pos.Y(), proposz);
  return projection;
}

//set geostuff
//long inefficient method in shoe, to set wire_pos and wire_dir
void setbmgeo(vector<TVector3> &wire_pos, vector<TVector3> &wire_dir){
  
  if(debug)
    cout<<"I'm starting setbmgeo"<<endl;
      
  double aa[BMN_NWIRELAY], bb[BMN_NWIRELAY];
  TVector3 bm_DeltaDch(BMN_DELTAX,BMN_DELTAY,BMN_DELTAZ);
  TVector3 bm_SideDch(BMN_WIDTH,BMN_HEIGHT,BMN_LENGTH);

  for(int nn=0;nn<7;nn++) {
    bb[nn] = nn*BMN_PASSO;
    bb[nn+7] = nn*BMN_PASSO;
    bb[nn+14] = nn*BMN_PASSO;

    aa[nn] = 0. ;
    aa[nn+7] = BMN_STEP;
    aa[nn+14] = 2*BMN_STEP;
  }
  
  double x_pos[BMN_NWIRELAY][BMN_NLAY][2], y_pos[BMN_NWIRELAY][BMN_NLAY][2], z_pos[BMN_NWIRELAY][BMN_NLAY][2];
  double cx_pos[BMN_NWIRELAY][BMN_NLAY][2], cy_pos[BMN_NWIRELAY][BMN_NLAY][2], cz_pos[BMN_NWIRELAY][BMN_NLAY][2];
  
  for(int il=0; il<BMN_NLAY;il++){
    for (int kk =0; kk<BMN_NWIRELAY;kk++){       
      /*  U wires -> along x, SIDE view */
      x_pos[kk][il][0] = - bm_SideDch[0]/2;
      z_pos[kk][il][0] = - bm_SideDch[2]/2 + bm_DeltaDch[2] +	aa[kk] + (2*il)*(2*BMN_STEP + BMN_LAYDIST);
      if( (il==0) || (il==2) || (il==4) ){
        y_pos[kk][il][0] = - bm_SideDch[1]/2 + bm_DeltaDch[1] + bb[kk] + BMN_PASSO;
      }
      else{
        y_pos[kk][il][0] = - bm_SideDch[1]/2 + bm_DeltaDch[1] + bb[kk];
      }
      cx_pos[kk][il][0] = bm_SideDch[0];
      cy_pos[kk][il][0] = 0.;
      cz_pos[kk][il][0] = 0.;

      /* V wires -> along y, TOP view*/
      y_pos[kk][il][1] = - bm_SideDch[1]/2;
      z_pos[kk][il][1] = - bm_SideDch[2]/2 + bm_DeltaDch[2] + aa[kk] + (2*il+1)*(2*BMN_STEP + BMN_LAYDIST);
      if( (il==0) || (il==2) || (il==4) ){
        x_pos[kk][il][1] = - bm_SideDch[0]/2 + bm_DeltaDch[0] + bb[kk];
      }
      else{
        x_pos[kk][il][1] = - bm_SideDch[0]/2 + bm_DeltaDch[0] + bb[kk] + BMN_PASSO;
      }
      cx_pos[kk][il][1] = 0.;
      cy_pos[kk][il][1] = bm_SideDch[1];
      cz_pos[kk][il][1] = 0.;
    }
  }
  
  wire_pos.clear();
  wire_dir.clear();
  TVector3 tmp_tvector3, bmshift(BMISOX+BMSHIFTX, BMISOY+BMSHIFTY, BMISOZ+BMSHIFTZ);
  int ilay, iview, icell;
  int bm_idsense[3]={8,10,12};
  for(int i=0;i<36;i++){
    Getlvc(i,ilay, iview, icell);
    //set wirepos in the bm system of ref.
    tmp_tvector3.SetXYZ(x_pos[bm_idsense[icell]][ilay][iview], y_pos[bm_idsense[icell]][ilay][iview],z_pos[bm_idsense[icell]][ilay][iview]);
    //rotate the bm wires (before the traslation)
    tmp_tvector3.RotateX((BMISOYZANGLE+BMYZANGLE)*DEG2RAD);
    tmp_tvector3.RotateY((BMISOXZANGLE+BMXZANGLE)*DEG2RAD);
    //translate the bm wires
    tmp_tvector3+=bmshift;
    wire_pos.push_back(tmp_tvector3);
    tmp_tvector3.SetXYZ(cx_pos[bm_idsense[icell]][ilay][iview], cy_pos[bm_idsense[icell]][ilay][iview],cz_pos[bm_idsense[icell]][ilay][iview]);
    //rotate the bm wires (before the traslation)
    tmp_tvector3.RotateX((BMISOYZANGLE+BMYZANGLE)*DEG2RAD);
    tmp_tvector3.RotateY((BMISOXZANGLE+BMXZANGLE)*DEG2RAD);
    tmp_tvector3.SetMag(1.);
    wire_dir.push_back(tmp_tvector3);
    if(debug>1){
      cout<<"bm geo wire position check:  id="<<i<<endl;
      wire_pos.at(i).Print();
      wire_dir.at(i).Print();
    }
  }
  
  if(debug)
    cout<<"bm geo created!"<<endl;
  
  return;
}

TVector3 BMlocaltoiso(TVector3 local){
  local.RotateX((BMISOYZANGLE+BMYZANGLE)*DEG2RAD);
  local.RotateY((BMISOXZANGLE+BMXZANGLE)*DEG2RAD);
  TVector3 bmshift(BMISOX+BMSHIFTX, BMISOY+BMSHIFTY, BMISOZ+BMSHIFTZ);
  return local+bmshift;
}


//similar, but NOT identical to GetBMNlvc in tabmpargeo
bool Getlvc(const Int_t cellid, Int_t& ilay, Int_t& iview, Int_t& icell){
  if(cellid>35 || cellid<0){
    cout<<"ERROR in TABMparGeo::GetBMNcell, cellid is wrong: cellid="<<cellid<<endl;
    return kFALSE;
  }
  icell=cellid%3;
  iview=(((Int_t)(cellid/3))%2==0) ? 0:1; 
  ilay=(Int_t)(cellid/6);
  return kTRUE;
}


Double_t FindRdrift(TVector3 pos, TVector3 dir, TVector3 A0, TVector3 Wvers) {

  Double_t tp = 0., tf= 0., rdrift; 
  TVector3 D0, R0, Pvers;

  if(dir.Mag()==0.){
    cout<<"WARNING: FindRdrift: momentum is 0 and the hit shouldn't be charged because this hit is from a fragmentated particle with zero momentum"<<endl;
    return 99;//fake value
    }
    
  R0.SetXYZ(pos.X(),pos.Y(),pos.Z());//set position
  Pvers=dir;
  Pvers.SetMag(1.);
  
  D0 = R0 - A0;//distance between position of reference point of current wire and current particle position

  Double_t prosca = Pvers*Wvers ;//scalar product of directions
  Double_t D0W = D0*Wvers;//distance projected on wire
  Double_t D0P = D0*Pvers;//distance projected on particle direction

  if(prosca!= 1.) {//if the don't fly parallel
    tp = (D0W*prosca - D0P)/(1.-prosca*prosca);
    tf = (-D0P*prosca + D0W)/(1.-prosca*prosca);
    rdrift = sqrt( abs(D0.Mag2() + tp*tp + tf*tf + 2.*tp*D0P -2.*tf*D0W -2.*prosca*tf*tp ));
    } 
  else  //if they go parallel
    rdrift = sqrt(abs( D0.Mag2() - D0W*D0W)); 

  //~ if(rdrift>0.945) //for the fitted tracks it is possible to have a rdrift bigger than the cell size
    //~ cout<<"WARNING!!!!! SOMETHING IS WRONG, YOU HAVE A TOO BIG RDRIFT!!!!!!!!! look at TABMntuTrack::FindRdrift   rdrift="<<rdrift<<endl;
  if(rdrift<0){
    cout<<"WARNING!!!!! SOMETHING IS WRONG, YOU HAVE A NEGATIVE RDRIFT!!!!!!!!! look at TABMntuTrack::FindRdrift    rdrift="<<rdrift<<endl;    
    cout<<"pos=("<<pos.X()<<","<<pos.Y()<<","<<pos.Z()<<")  dir=("<<dir.X()<<","<<dir.Y()<<","<<dir.Z()<<")"<<endl;
    cout<<"A0=("<<A0.X()<<","<<A0.Y()<<","<<A0.Z()<<")  Wvers=("<<Wvers.X()<<","<<Wvers.Y()<<","<<Wvers.Z()<<")"<<endl;
    }
    
  return rdrift;
}

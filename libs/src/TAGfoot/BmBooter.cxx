#include "BmBooter.hxx"

#include <FieldManager.h>


using namespace std;



//----------------------------------------------------------------------------------------------------
BmBooter::BmBooter() {
  //~ myn_bmraw=nullptr;
  //~ myn_bmtrk=nullptr;
  //~ bmraw=nullptr;
  //~ bmtrack=nullptr;
  //~ m_controlPlotter=nullptr;
  //~ bm_outputdir="null";
  //~ p_tracktr=nullptr;
  //~ p_hit=nullptr; 
}



//----------------------------------------------------------------------------------------------------
void BmBooter::Initialize( TString instr_in, Bool_t isdata_in ) {  
  isdata=isdata_in;
  m_instr=instr_in;
  data_num_ev=0;
  isallign=kFALSE;
  clear_bmstruct(kTRUE);
  myp_bmgeo = gTAGroot->FindParaDsc("myp_bmgeo", "TABMparGeo");  
  myp_bmcon = gTAGroot->FindParaDsc("myp_bmcon", "TABMparCon");  
  myp_bmmap = gTAGroot->FindParaDsc("myp_bmmap", "TABMparMap");  
  bmcon = (TABMparCon*) (gTAGroot->FindParaDsc("myp_bmcon", "TABMparCon")->Object()); 
  bmgeo = (TABMparGeo*) (gTAGroot->FindParaDsc("myp_bmgeo", "TABMparGeo")->Object());
  bmmap = (TABMparMap*) (gTAGroot->FindParaDsc("myp_bmmap", "TABMparMap")->Object());
  if(bmcon->GetParmapfile().compare("beammonitor_geoch_roma.map")==0)
    isroma=kTRUE;
  else
    isroma=kFALSE;
  cell_occupy.resize(36);
    
  if (bmcon->GetBMdebug()>10) 
    cout<<"initialize BmBooter"<<endl;
  struct stat info;
  
  //cartella results/BeamMonitor: da capire come accederci e cosa metterci!
  //~ bm_outputdir=(string)getenv("FOOTRES")+"/BeamMonitor";
  //~ if(stat(bm_outputdir.c_str(), &info ) != 0)		//cannot access
    //~ system(("mkdir "+bm_outputdir).c_str());  
  m_controlPlotter = ControlPlotsRepository::GetControlObject("BM_output"); //??? it work also if instead of "BM_output" I write "dlasjflasj"  
  
  //if plot track is enable
  if(bmcon->GetBMvietrack()){
    pg = new TAGpadGroup();
    bm_outputdir=(string)getenv("FOOTRES")+"/BMtracks";    
    //~ bm_outputdir+="/Tracks";
    if(stat(bm_outputdir.c_str(), &info ) != 0)		//cannot access
      system(("mkdir "+bm_outputdir).c_str());   
    //~ pbmh_view = new TABMvieTrackFOOT(bmgeo);         
  }
  
  if(isdata){
    datafile.open(m_instr.Data(), ios::in | ios::binary);
    if(!datafile.is_open())
      cout<<"ERROR in BmBooter::CalculateT0: cannot open the datafile="<<m_instr.Data()<<endl;
    evaluateT0();
    datafile.close();
    if(bmcon->GetmanageT0BM()==0)
      bmcon->PrintT0s(m_instr);
    else
      bmcon->loadT0s();
    if(bmcon->GetBMdebug()>1)
      bmcon->CoutT0();
    datafile.open(m_instr.Data(), ios::in | ios::binary);
    if(!datafile.is_open())
      cout<<"ERROR in BmBooter::CalculateT0: cannot open the datafile="<<m_instr.Data()<<endl;
    FillDataBeamMonitor();
  }
    
  tot_num_ev=data_num_ev;  
  data_num_ev=0;
  
return;
}





//----------------------------------------------------------------------------------------------------
void BmBooter::Process() {
  if (bmcon->GetBMdebug()>0)
    cout<<"I'm in BmBooter::Process, event number="<<data_num_ev<<endl;
  
  Int_t track_ok;
   
   if(isroma && (data_num_ev==0 || data_num_ev==1) && isdata ){
    drop_event();
    return;
  }
  
  if(isdata){
    Bool_t error;
    read_event(error);
    if(!error){
      bmdatraw = (TABMdatRaw*) (gTAGroot->FindDataDsc("myn_bmdatraw", "TABMdatRaw")->GenerateObject());  
      stdatraw = (TAIRdatRaw*) (gTAGroot->FindDataDsc("myn_stdatraw", "TAIRdatRaw")->GenerateObject());
     
      //loop on dat hit, only for data
      for(Int_t i=0;i<bmdatraw->NHit();i++){
        bmdathit=&bmdatraw->Hit(i);
        //~ cout<<"evento numero="<<data_num_ev<<"  numero hit"<<i<<"raw hit time="<<bmdathit->Time()<<endl;
      }
    }else
      return;
  }
  
  bmnturaw = (TABMntuRaw*) (gTAGroot->FindDataDsc("myn_bmraw", "TABMntuRaw")->GenerateObject());
  evaluate_cell_occupy();
  
  
  if(bmnturaw->nhit > bmcon->GetMaxnhit_cut())
    track_ok=-2;
  else if(bmnturaw->nhit < bmcon->GetMinnhit_cut())
    track_ok=-1;  
  else{  
    bmntutrack = (TABMntuTrack*) (gTAGroot->FindDataDsc("myn_bmtrk", "TABMntuTrack")->GenerateObject());
    track_ok=bmntutrack->trk_status;
  }
 
  if (bmcon->GetBMdebug()>10)
    cout<<"in BmBooter::Process, I finished to create the BM hits and tracks"<<endl<<"Now I'll printout BM hits if enable"<<endl;

  if (GlobalPar::GetPar()->IsPrintOutputFile())
    m_controlPlotter->BM_setnturaw_info("BM_output",bmnturaw, bmgeo, bmcon, cell_occupy);  
  
  if (bmcon->GetBMdebug()>10)
    cout<<"in BmBooter::Process, I finished to printout BM hits, it's BM tracks printout (if enable)"<<endl;
  
  if (GlobalPar::GetPar()->IsPrintOutputFile() && track_ok>=0)
    if(m_controlPlotter->BM_setntutrack_info("BM_output", bmntutrack, bmnturaw, bmcon)==0)
      isallign=kTRUE;      

  //draw and save tracks
  if(bmcon->GetBMvietrack()>0 && data_num_ev%bmcon->GetBMvietrack()==0){
      TCanvas *c_bmhview = new TCanvas("bmhview", "BM_tracks",20,20,800,900);
      pg->AddPad(c_bmhview);
      //~ TAGview* pbmh_view = new TABMvieTrackFOOT(bmntutrack, bmnturaw, bmgeo, track_ok);
      TABMvieTrackFOOT* pbmh_view = new TABMvieTrackFOOT(bmntutrack, bmnturaw, bmgeo, track_ok);
      pbmh_view->SetCelloccupy(cell_occupy);
      pbmh_view->Draw();
      pg->Modified();//marca i pad come modificati
      //~ pg->Update();//fa update del canvas
      //~ if(track_ok==0)
      plot_name=bm_outputdir+"/BM_track_"+to_string(track_ok)+"_"+to_string(data_num_ev);
      //~ else
        //~ plot_name=bm_outputdir+"/BM_track_"+to_string(data_num_ev);
      pg->Print(&plot_name[0]);  
  }
  
  
  data_num_ev++;

  if (bmcon->GetBMdebug()>10)
    cout<<"I finished BmBooter::Process"<<endl;

return;
}





//----------------------------------------------------------------------------------------------------
void BmBooter::Finalize() {
  
  if (bmcon->GetBMdebug()>10)
    cout<<"I'm in BmBooter::Finalize"<<endl;

  PrintSTrel();  
  if(isallign)
    Allign_estimate();
  
  if (bmcon->GetBMdebug()>10)
    cout<<"I finished BmBooter::Finalize"<<endl;
    
return;
}

void BmBooter::Allign_estimate(){
  
  Double_t xrot=-atan((((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_y")))->GetMean()-((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_y")))->GetMean())/(bmgeo->GetMylar2().Z()-bmgeo->GetMylar1().Z()))*RAD2DEG;  
  
  Double_t yrot=atan((((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_x")))->GetMean()-((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_x")))->GetMean())/(bmgeo->GetMylar2().Z()-bmgeo->GetMylar1().Z()))*RAD2DEG;
   
  Double_t xtra=-(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_x")))->GetMean()+((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_x")))->GetMean())/2.;  
      
  Double_t xtr_err=sqrt(pow(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_x")))->GetMean()/sqrt(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_x")))->GetEntries()),2.)  + pow(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_x")))->GetMean()/sqrt(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_x")))->GetEntries()),2.));  
    
  Double_t ytra=-(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_y")))->GetMean()+((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_y")))->GetMean())/2.;
    
  Double_t ytr_err=sqrt(pow(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_y")))->GetMean()/sqrt(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_y")))->GetEntries()),2.)  +  pow(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_y")))->GetMean()/sqrt(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_y")))->GetEntries()),2.));  
    
  if(bmcon->GetBMdebug()>3){  
    cout<<"Beam Monitor allignment parameters"<<endl;
    cout<<"estimated rotation around X axis= "<<xrot<<endl;
    cout<<"estimated rotation around Y axis= "<<yrot<<endl;
    cout<<"estimated translation in X="<<xtra<<"   +-   "<<xtr_err<<endl;
    cout<<"estimated translation in Y="<<ytra<<"   +-   "<<ytr_err<<endl;
    cout<<"AngPhi mean value="<<((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_AngPhi")))->GetMean()<<endl;
    cout<<"AngPhi devstrd ="<<((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_AngPhi")))->GetStdDev()<<endl;
  }
  
  if(((TDirectory*)(m_controlPlotter->GetTFile()))->IsFolder()){
    TString tmp_str("BM allign par: xrot=");
    tmp_str+= xrot; 
    tmp_str+="  yrot=";
    tmp_str+=yrot;
    tmp_str+="  x_tra=";
    tmp_str+=xtra;
    tmp_str+=" +- ";
    tmp_str+=xtr_err;
    tmp_str+="  y_tra=";
    tmp_str+=ytra;
    tmp_str+=" +- ";
    tmp_str+=ytr_err;
    TNamed n("BM_allign_par",tmp_str.Data());
    n.Write();
  }
  
  return;
}


void BmBooter::PrintSTrel(){
  TH1D* histo=new TH1D( "strel", "strel", 4000, 0., 400.);
  histo->GetXaxis()->SetTitle("time [ns]");
  histo->GetYaxis()->SetTitle("distance [cm]");
  double time=0.;
  for(int i=0;i<4000;i++){
    histo->SetBinContent(i,time*bmcon->GetVDrift()+bmcon->FirstSTrel(time));
    time+=0.1;
  }
  histo->Draw();
  if(((TDirectory*)(m_controlPlotter->GetTFile()))->IsFolder())
    ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Add(histo);    
return;
}


void BmBooter::evaluateT0() {
  //~ TABMparCon* bmcon = (TABMparCon*) myp_bmcon->Object();  
  TFile *f_out = new TFile("bm_t0.root","RECREATE");
  f_out->cd();
  TH1D* h=nullptr;
  Int_t tmp_int, trash;
  char tmp_char[200];
  bool error;
  for(Int_t i=0;i<bmmap->GetTdcMaxcha();i++){
    if(i!=bmmap->GetTrefCh())
      sprintf(tmp_char,"tdc_cha_%d",i);
    else
      sprintf(tmp_char,"tdc_synccha_%d",i);  
    h=new TH1D(tmp_char,"Registered time;Time [ns]; counts",3000,-1000.,2000.);
  }
  h=new TH1D("all_tdc_chan","Number of tdc signals; TDC channel; counts",bmmap->GetTdcMaxcha(),0.,bmmap->GetTdcMaxcha());
  h=new TH1D("tdc_error","Number of tdc signals with errors; Event number; counts",4,0.,4);//provv, distinguish the type of error!
  
  //jump the first event
  drop_event();
  if(isroma)//jump the second event if isroma    
    drop_event();
      
  //charge the tdc_cha_* TH1D graph of the tdc signals    
  while(read_event(error)){
    if(bmcon->GetBMdebug()>11)
      cout<<"data_num_ev="<<data_num_ev<<endl;
    if(!error && bmstruct.synctime[0]!=-10000 && bmstruct.synctime[1]==-10000){
      ((TH1D*)gDirectory->Get("tdc_error"))->Fill(0);//no error
      sprintf(tmp_char,"tdc_synccha_%d",bmmap->GetTrefCh());  
        ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.synctime[0])/10.);    
      for(Int_t i=0;i<bmstruct.hitnum;i++){
        sprintf(tmp_char,"tdc_cha_%d",bmmap->cell2tdc(bmstruct.hit_id[i]));
        ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.hit_meas[i]-bmstruct.synctime[0])/10.);    
        ((TH1D*)gDirectory->Get("all_tdc_chan"))->Fill(bmmap->cell2tdc(bmstruct.hit_id[i]));    
      } 
    }else{
      if(bmcon->GetBMdebug()>1)
        cout<<"BmBooter::evaluateT0::ERROR in the data detected at data_num_ev="<<data_num_ev<<endl;
      if(bmstruct.synctime[0]==-10000)
        ((TH1D*)gDirectory->Get("tdc_error"))->Fill(1);//no synctime
      else if(bmstruct.synctime[1]!=-10000)      
        ((TH1D*)gDirectory->Get("tdc_error"))->Fill(2);//multi synctime
      else if(error)
        ((TH1D*)gDirectory->Get("tdc_error"))->Fill(3);//generic reading error  
    }
    data_num_ev++;
  }
  
  //fit the tdc signals with a function to evaluate the T0, for the moment I take the shortest signal close to the peak
  //~ TF1 *f1 = new TF1("f1","[0]*pow(([1]/[2]),(x/[2]))(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1)", 0, 2000);
  //~ TF1 *f1 = new TF1("f1","[0]*pow([1]/[2],x/[2]-[3])/(TMath::Gamma(x/[2]-[3]+1))*(TMath::Exp(-[1]/[2]))", 0, 2000);
  //~ TF1 *f1 = new TF1("f1","gaus(0)", ((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()-100, ((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()+100);
  for(Int_t i=0;i<bmmap->GetTdcMaxcha();i++){
    sprintf(tmp_char,"tdc_cha_%d",i);
    if(bmmap->tdc2cell(i)>=0 && i!=bmmap->GetTrefCh()){
      if(((TH1D*)gDirectory->Get(tmp_char))->GetEntries()>0){
        //~ cout<<"fit channel number="<<i<<endl;
        //~ f1->SetParameters(1, ((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin(), 10);
        //~ f1->SetParLimits(1,0,100);
        //~ ((TH1D*)gDirectory->Get(tmp_char))->Fit(f1,"QR+","",((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()-100,((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()+100);
      
        //take the first signal, not too distant from other signals
        tmp_int=((TH1D*)gDirectory->Get(tmp_char))->GetMinimumBin();
        for(Int_t j=((TH1D*)gDirectory->Get(tmp_char))->GetMinimumBin()+1;j<((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin();j++)
          if(((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(j)>0)
            if(j-tmp_int<50){
              j=2000;
            }else
              tmp_int=j;
        //~ if(bmcon->GetBMdebug()>9)
          //~ cout<<"tdc channel="<<i<<"   T0="<<(Double_t)((TH1D*)gDirectory->Get(tmp_char))->GetBinCenter(tmp_int)/10.<<endl;
        bmcon->SetT0(bmmap->tdc2cell(i),(Double_t)((TH1D*)gDirectory->Get(tmp_char))->GetBinCenter(tmp_int)); 
      }
    }  
  }  
  
  f_out->Write();
  f_out->Close();    
    
  return;
}


Bool_t BmBooter::read_event(Bool_t &error) {
  //~ TABMparCon* bmcon = (TABMparCon*) myp_bmcon->Object();
  //~ TABMparMap* o_bmmap = (TABMparMap*) myp_bmmap->Object();
  
  Int_t tmp_int; //dummy int variable to read and convert the input file
  //~ vector<int> ev_words;//words of the event
  Bool_t new_event, read_meas;
  Int_t tdc_evnum, sync_evnum=0;
  double tdc_meas;
  error=false;
  //~ Int_t board_num=513;//number of board used in the acquisition //provv, da modificare: mettere in config file questo numero
  
  clear_bmstruct(kFALSE);
  
  if(bmcon->GetBMdebug()>11)
    cout<<"I'm in BmBooter:read_event"<<endl;
  if(datafile.read((char *) &tmp_int,sizeof(int))){//read number of words of this event
    bmstruct.words=tmp_int;
    }
  else{
    if(bmcon->GetBMdebug()>11)
      cout<<"file ended"<<endl;
    return kFALSE;
  }
  Int_t ev_words[bmstruct.words];
  for(Int_t i=0;i<bmstruct.words;i++){
    datafile.read((char *) &tmp_int,sizeof(int));
    //~ cout<<"i="<<i<<"  word="<<tmp_int<<endl;
    ev_words[i]=tmp_int;
  }
  //some check on the event words:
  if(ev_words[1]!=TDC_BOARDNUM){
    cout<<"ERROR in BmBooter:read_event: board num != "<<TDC_BOARDNUM<<"  ev_words[1]="<<ev_words[1]<<endl;
    error=true;
  }
  if(ev_words[3]!=3){//provv, da modificare: da togliere nell'acquisizione e qua...
    cout<<"ERROR in BmBooter:read_event: ev_words[3]="<<ev_words[3]<<" ,!=3"<<endl;
    error=true;          
  }
  bmstruct.evnum=ev_words[0];
  if((error && bmcon->GetBMdebug()>0) || bmcon->GetBMdebug()>11)
    for(Int_t i=0;i<bmstruct.words;i++)
      cout<<"event_num="<<data_num_ev<<"   ev_words["<<i<<"]="<<ev_words[i]<<endl;
  if(!error){//read the tdc words
    new_event=true;
    read_meas=false;
    for(Int_t i=4;i<ev_words[2]+4 && !error;i++){
      if(new_event && !error){//global header found
        tdc_evnum=ev_words[i++];
        read_meas=true;
        if(bmcon->GetBMdebug()>11)
          cout<<"global header found, i="<<i<<"  tdc_evnum="<<tdc_evnum<<endl;
        }
      if(ev_words[i]<0 && isroma==kFALSE && !error){//global trailer found //se uso acquisizione mio (yun)
        read_meas=false;
        new_event=true;
        bmstruct.status=ev_words[i];
        if(ev_words[i]!=-1000){
          cout<<"Warning in BmBooter: global trailer found with error in tdc_evnum="<<tdc_evnum<<"  trailer="<<ev_words[i]<<endl;
          error=true;
          new_event=false;
        }
        if(bmcon->GetBMdebug()>11)
          cout<<"global trailer found, i="<<i<<"  ev_words="<<ev_words[i]<<endl;
      }        
      if(ev_words[i]==0 && isroma==kTRUE && !error){//global trailer found //se uso dati letti a Roma per BM refurbishment
        read_meas=false;
        new_event=true;
        bmstruct.status=-1000;
        if(bmcon->GetBMdebug()>11)
          cout<<"global trailer found, i="<<i<<"  ev_words="<<ev_words[i]<<endl;
      }        
      if(read_meas && !error){  
        if(ev_words[i++]!=tdc_evnum){
          cout<<"ERROR in BmBooter:read_event: tdc_evnum="<<tdc_evnum<<"  measured event number="<<ev_words[i-1]<<"  i="<<i<<endl;
          error=true;
        }
        if(ev_words[i]>-1 && ev_words[i]<bmmap->GetTdcMaxcha()){//measure found
          if(ev_words[i]==bmmap->GetTrefCh()){
            bmstruct.synctime[sync_evnum]=ev_words[++i];
            sync_evnum++;
          }else{      
            bmstruct.hit_id[bmstruct.hitnum]=bmmap->tdc2cell(ev_words[i++]);
            bmstruct.hit_meas[bmstruct.hitnum]=ev_words[i];
            bmstruct.hitnum++;
          }
        }
        else{
          cout<<"ERROR in BmBooter:read_event: tdc_channel out of range!!! tdc_channel="<<ev_words[i]<<endl;
          error=true;
        }
        new_event=false;
        if(bmcon->GetBMdebug()>11 && ev_words[i-1]!=bmmap->GetTrefCh())
          cout<<"BMbooter::measure found: tdc_evnum="<<tdc_evnum<<" hit_id="<<bmstruct.hit_id[bmstruct.hitnum-1]<<" hit_meas="<<bmstruct.hit_meas[bmstruct.hitnum-1]<<endl;
        else if(bmcon->GetBMdebug()>11 && ev_words[i-1]==bmmap->GetTrefCh())
          cout<<"BMbooter::trigger found: sync registered="<<sync_evnum<<"  time="<<bmstruct.synctime[sync_evnum-1]<<endl;
      }
    }//end of reading tdc words for loop
    if(error && bmcon->GetBMdebug()>0){
      cout<<"BMbooter::read_event::error detected previously, the whole event read is:    data_num_ev="<<data_num_ev<<endl;
      for(Int_t i=0;i<bmstruct.words;i++)
        cout<<"ev_words["<<i<<"]="<<ev_words[i]<<endl;
    }
  }
  //~ else{//read tdc words if    
    //~ cout<<"data_num_ev="<<data_num_ev<<endl;
    //~ for(Int_t i=0;i<bmstruct.words;i++)
        //~ cout<<"ev_words["<<i<<"]="<<ev_words[i]<<endl;
  //~ }
  return kTRUE;
}



Bool_t BmBooter::drop_event(){
  Int_t tmp_int, trash;
  if(!isdata){
    data_num_ev++;
    return kTRUE;
  }
  
  if(datafile.read((char *) &tmp_int,sizeof(int))){
    for(Int_t i=0;i<tmp_int;i++)
      datafile.read((char *) &trash,sizeof(int));
    data_num_ev++;
  }else
    return kFALSE;  

  return kTRUE;
}


void BmBooter::evaluate_cell_occupy(){
  Int_t cell_index, tmp_int;
  TABMntuHit* oldhit;
  for(Int_t i=0;i<36;i++)
    cell_occupy[i].clear();
  
  //charge cell_occupy
  for(Int_t i=0;i<bmnturaw->nhit;i++){
    bmntuhit=bmnturaw->Hit(i);
    cell_index=bmgeo->GetBMNcell(bmntuhit->Plane(),bmntuhit->View(), bmntuhit->Cell());
    if(cell_occupy[cell_index].size()==0)
      cell_occupy[cell_index].push_back(i);
    else{
      tmp_int=cell_occupy[cell_index].size();
      while(tmp_int){
        tmp_int--;
        oldhit=bmnturaw->Hit(cell_occupy[cell_index][tmp_int]);
        if(oldhit->Dist()<bmntuhit->Dist()){
          cell_occupy[cell_index].insert(cell_occupy[cell_index].begin()+tmp_int+1, i);
          tmp_int=0;//exit the while        
        }
      }  
    }
  }
  
  if(bmcon->GetBMdebug()>12){//provv
    cout<<"BmBooter::evaluate_cell_occupy: print cell_occupy"<<endl;
    for(Int_t i=0;i<36;i++){
    cout<<endl;
      for(Int_t j=0;j<cell_occupy[i].size();j++)
        cout<<cell_occupy[i][j]<<" ";
    }
  }

  return;
}


void BmBooter::clear_bmstruct(Bool_t forced){
  bmstruct.evnum=0;
  bmstruct.words=0;
  bmstruct.hitnum=0;
  bmstruct.status=0;
  if(forced){
    for(Int_t i=0;i<MAXHITTDC;i++){
     bmstruct.hit_id[i]=-10000;
     bmstruct.hit_meas[i]=-10000;
     bmstruct.synctime[i]=-10000;
    }
  return;
  }
  Int_t tmp_int=0;
  while(bmstruct.synctime[tmp_int]!=-10000){
    bmstruct.synctime[tmp_int]=-10000;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.hit_id[tmp_int]!=-10000){
    bmstruct.hit_id[tmp_int]=-10000;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.hit_meas[tmp_int]!=-10000){
    bmstruct.hit_meas[tmp_int]=-10000;
    tmp_int++;
  }
return;
}


void BmBooter::FillDataBeamMonitor() {
  if (GlobalPar::GetPar()->Debug()>10)
    cout<<"I'm in BmBooter::FillDataBeamMonitor"<<endl;  
  
  myn_bmdatraw    = new TAGdataDsc("myn_bmdatraw", new TABMdatRaw());
  new TABMactDatRaw("an_bmdatraw",myn_bmdatraw, myp_bmgeo, &bmstruct); 

  myn_stdatraw    = new TAGdataDsc("myn_stdatraw", new TAIRdatRaw());
  new TAIRactDatRaw("an_stdatraw", myn_stdatraw, myp_bmmap, &bmstruct);
   
  myn_bmraw    = new TAGdataDsc("myn_bmraw", new TABMntuRaw());
  new TABMactNtuRaw("an_bmraw", myn_bmraw, myn_bmdatraw, myn_stdatraw, myp_bmgeo, myp_bmcon); 
  
  myn_bmtrk    = new TAGdataDsc("myn_bmtrk", new TABMntuTrack());  
  new TABMactNtuTrack("an_bmtrk", myn_bmtrk, myn_bmraw, myp_bmgeo, myp_bmcon);
    
  if (GlobalPar::GetPar()->Debug()>10)
    cout<<"I finish BmBooter::FillDataBeamMonitor"<<endl;  
  return;
}


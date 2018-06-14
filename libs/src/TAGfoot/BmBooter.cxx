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
void BmBooter::Initialize( TString instr_in, Bool_t isdata_in, Bool_t isroma_in ) {  
  isdata=isdata_in;
  isroma=isroma_in;
  m_instr=instr_in;
  data_num_ev=0;
  clear_bmstruct(kTRUE);
  myp_bmgeo = gTAGroot->FindParaDsc("myp_bmgeo", "TABMparGeo");  
  myp_bmcon = gTAGroot->FindParaDsc("myp_bmcon", "TABMparCon");  
  myp_bmmap = gTAGroot->FindParaDsc("myp_bmmap", "TABMparMap");  
  bmcon = (TABMparCon*) (gTAGroot->FindParaDsc("myp_bmcon", "TABMparCon")->Object()); 
  bmgeo = (TABMparGeo*) (gTAGroot->FindParaDsc("myp_bmgeo", "TABMparGeo")->Object());
  bmmap = (TABMparMap*) (gTAGroot->FindParaDsc("myp_bmmap", "TABMparMap")->Object());
  
  if (bmcon->GetBMdebug()>10) 
    cout<<"initialize BmBooter"<<endl;
  struct stat info;
  
  //cartella results/BeamMonitor: da capire come accederci e cosa metterci!
  bm_outputdir=(string)getenv("FOOTRES")+"/BeamMonitor";
  if(stat(bm_outputdir.c_str(), &info ) != 0)		//cannot access
    system(("mkdir "+bm_outputdir).c_str());  
  m_controlPlotter = ControlPlotsRepository::GetControlObject(bm_outputdir);
  
  //if plot track is enable
  if(bmcon->GetBMvietrack()){
    pg = new TAGpadGroup();
    bm_outputdir+="/Tracks";
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
  if (bmcon->GetBMdebug()>10)
    cout<<"I'm in BmBooter::Process, evento numero="<<data_num_ev<<endl;
  
  //~ myn_bmraw  = gTAGroot->FindDataDsc("myn_bmraw", "TABMntuRaw");
  //~ myn_bmtrk  = gTAGroot->FindDataDsc("myn_bmtrk", "TABMntuTrack");
  //~ if (myn_bmtrk == 0 || myn_bmraw==0) {
      //~ cout << "ERROR in BmBooter:: myn_bmraw or myn_bmtrk not found" << endl;
      //~ exit(0);
    //~ }
    
  //~ bmraw = (TABMntuRaw*) myn_bmraw->GenerateObject();
  //~ bmtrack = (TABMntuTrack*) myn_bmtrk->GenerateObject();
 
  //~ bmdatraw = (TABMdatRaw*) (gTAGroot->FindDataDsc("myn_bmdatraw", "TABMdatRaw")->GenerateObject());
  
   if((data_num_ev==0 || (isroma && data_num_ev==1) && isdata )){
    drop_event();
    data_num_ev++;
    return;
  }
  
  if(isdata){
    Bool_t error;
    read_event(error);
    if(!error){
      TABMdatRaw* bmdatraw = (TABMdatRaw*) (gTAGroot->FindDataDsc("myn_bmdatraw", "TABMdatRaw")->GenerateObject());  
      TAIRdatRaw* stdatraw = (TAIRdatRaw*) (gTAGroot->FindDataDsc("myn_stdatraw", "TAIRdatRaw")->GenerateObject());
      //~ TABMntuTrack* bmtack = (TABMntuTrack*) (gTAGroot->FindDataDsc("myn_bmtrk", "TABMntuTrack")->GenerateObject());
     
     
     /*
      TABMntuHit* bmntuhit;
      const TABMrawHit* bmrawhit;
      for(Int_t i=0;i<bmraw->nhit;i++){
        bmntuhit = bmraw->Hit(i);
        //~ cout<<"evento numero="<<data_num_ev<<"   numero hit="<<i<<"   tempo="<<bmntuhit->Tdrift()<<endl;
      }
      for(Int_t i=0;i<bmdatraw->NHit();i++){
        bmrawhit=&bmdatraw->Hit(i);
        //~ cout<<"evento numero="<<data_num_ev<<"  numero hit"<<i<<"raw hit time="<<bmrawhit->Time()<<endl;
      }
      */
      
      
    }
  }
  
  
  bmraw = (TABMntuRaw*) (gTAGroot->FindDataDsc("myn_bmraw", "TABMntuRaw")->GenerateObject());
  //~ bmtrack = (TABMntuTrack*) (gTAGroot->FindDataDsc("myn_bmtrk", "TABMntuTrack")->GenerateObject());
 
  if (bmcon->GetBMdebug()>10)
    cout<<"in BmBooter::Process, I finished to create the BM hits and tracks"<<endl<<"Now I'll printout BM hits if enable"<<endl;

  //loop on hits
  for (int i = 0; i < bmraw->nhit; i++) {
    p_hit = bmraw->Hit(i);
    //~ cout<<"hit number="<<i<<"  event number="<<data_num_ev<<"   distanza="<<p_hit->Dist()<<endl;
    if (GlobalPar::GetPar()->IsPrintOutputFile())
      m_controlPlotter->BM_setnturaw_info("BM_output",p_hit);  
    if (GlobalPar::GetPar()->IsPrintOutputNtuple())
      m_controlPlotter->BM_setntuple_hit(p_hit->Dist());
  }
  
  if (bmcon->GetBMdebug()>10)
    cout<<"in BmBooter::Process, I finished to printout BM hits, it's BM tracks printout (if enable)"<<endl;
  
  
  /*
  //loop on tracks
  for (int i = 0; i < bmtrack->ntrk; i++) {
    p_tracktr = bmtrack->Track(i);
    if (GlobalPar::GetPar()->IsPrintOutputFile())
      m_controlPlotter->BM_setntutrack_info("BM_output", p_tracktr);       
    if (GlobalPar::GetPar()->IsPrintOutputNtuple())
      m_controlPlotter->BM_setntuple_track(p_tracktr->GetChi2New());
  }

  //draw and save tracks
  if(bmcon->GetBMvietrack()>0 && data_num_ev%bmcon->GetBMvietrack()==0){
      TCanvas *c_bmhview = new TCanvas("bmhview", "BM_tracks",20,20,800,900);
      pg->AddPad(c_bmhview);
      TAGview* pbmh_view = new TABMvieTrackFOOT(bmtrack, bmraw, bmgeo);
      //~ pbmh_view->SetTrackRaw(bmtrack,bmraw);
      pbmh_view->Draw();
      pg->Modified();//marca i pad come modificati
      //~ cout<<"faccio update"<<endl;
      //~ pg->Update();//fa update del canvas
      plot_name=bm_outputdir+"/BM_track_"+to_string(data_num_ev);
      pg->Print(&plot_name[0]);  
  }
  */
  
  data_num_ev++;

  if (bmcon->GetBMdebug()>10)
    cout<<"I finished BmBooter::Process"<<endl;

return;
}





//----------------------------------------------------------------------------------------------------
void BmBooter::Finalize() {
  
  if (bmcon->GetBMdebug()>10)
    cout<<"I'm in BmBooter::Finalize"<<endl;
  
  //~ cout<<"filename="<<m_controlPlotter->GetTFile()->GetName()<<"  è aperto="<<m_controlPlotter->GetTFile()->IsOpen()<<endl;
  //~ m_controlPlotter->GetTFile()->ls();
  //~ TH1F *graph=nullptr;
  //~ m_controlPlotter->GetTFile()->GetObject("BM_output/BM_output__track_chi2red",graph);
  //~ cout<<"media="<<((TH1F*)(m_controlPlotter->GetTFile()->Get("BM_output/BM_output__track_chi2red")))->GetMean()<<endl;
  //~ cout<<"vediamo media="<<graph->GetMean()<<endl;
  
	TH1F* histo = new TH1F( "prova", "prova", 100, 0., 100.);
  for(int i=0;i<100;i++)
    histo->SetBinContent(i,pow(i,2));
  histo->Draw();
	((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Add(histo);  
  
  //vediamo le strel:
  histo=new TH1F( "strel", "strel", 4000, 0., 400.);
  histo->GetXaxis()->SetTitle("time [ns]");
  histo->GetYaxis()->SetTitle("distance [cm]");
  double time=0.;
  for(int i=0;i<4000;i++){
    histo->SetBinContent(i,time*bmcon->GetVDrift()+bmcon->STrelCorr(time,0,0,0));
    time+=0.1;
  }
  histo->Draw();
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Add(histo);    
  
  if (bmcon->GetBMdebug()>10)
    cout<<"I finished BmBooter::Finalize"<<endl;
    
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
    sprintf(tmp_char,"tdc_cha_%d",i);
    h=new TH1D(tmp_char,"Registered time;Time [ns]; counts",3000,-1000.,2000.);
  }
  h=new TH1D("all_tdc_chan","Number of tdc signals; TDC channel; counts",bmmap->GetTdcMaxcha(),0.,bmmap->GetTdcMaxcha());
  h=new TH1D("tdc_error","Number of tdc signals with errors; Event number; counts",3,0.,3);//provv, distinguish the type of error!
  
  //jump the first event
  drop_event();
  if(isroma)//jump the second event if isroma    
    drop_event();
      
  //charge the tdc_cha_* TH1D graph of the tdc signals    
  while(read_event(error)){
    if(bmcon->GetBMdebug()>9)
      cout<<"data_num_ev="<<data_num_ev<<endl;
    if(!error && bmstruct.synctime[0]!=-10000 && bmstruct.synctime[1]==-10000){
      for(Int_t i=0;i<bmstruct.hitnum;i++){
        sprintf(tmp_char,"tdc_cha_%d",bmmap->cell2tdc(bmstruct.hit_id[i]));
        ((TH1D*)gDirectory->Get(tmp_char))->Fill(bmstruct.hit_meas[i]-bmstruct.synctime[0]);    
        ((TH1D*)gDirectory->Get("all_tdc_chan"))->Fill(bmmap->cell2tdc(bmstruct.hit_id[i]));    
      } 
    }else{
      if(bmcon->GetBMdebug()>1)
        cout<<"BmBooter::evaluateT0::ERROR in the data detected at data_num_ev="<<data_num_ev<<endl;
      if(bmstruct.synctime[0]==-10000)
        ((TH1D*)gDirectory->Get("tdc_error"))->Fill(1);//no synctime
      if(bmstruct.synctime[1]!=-10000)      
        ((TH1D*)gDirectory->Get("tdc_error"))->Fill(2);//multi synctime
    }
    data_num_ev++;
  }
  
  //fit the tdc signals with a function to evaluate the T0, for the moment I take the shortest signal close to the peak
  //~ TF1 *f1 = new TF1("f1","[0]*pow(([1]/[2]),(x/[2]))(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1)", 0, 2000);
  //~ TF1 *f1 = new TF1("f1","[0]*pow([1]/[2],x/[2]-[3])/(TMath::Gamma(x/[2]-[3]+1))*(TMath::Exp(-[1]/[2]))", 0, 2000);
  //~ TF1 *f1 = new TF1("f1","gaus(0)", ((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()-100, ((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()+100);
  for(Int_t i=0;i<bmmap->GetTdcMaxcha();i++){
    sprintf(tmp_char,"tdc_cha_%d",i);
    if(bmmap->tdc2cell(i)>0){
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
      if(bmcon->GetBMdebug()>9)
        cout<<"tdc channel="<<i<<"   T0="<<(Double_t)((TH1D*)gDirectory->Get(tmp_char))->GetBinCenter(tmp_int)/10.<<endl;
      bmcon->SetT0(bmmap->tdc2cell(i),(Double_t)((TH1D*)gDirectory->Get(tmp_char))->GetBinCenter(tmp_int)/10.); 
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
  Int_t ev_num=0;//number of events given by the acquisition
  
  clear_bmstruct(kFALSE);
  
  if(bmcon->GetBMdebug()>9)
    cout<<"I'm in BMcalBooter:read_event"<<endl;
  if(datafile.read((char *) &tmp_int,sizeof(int))){//read number of words of this event
    bmstruct.words=tmp_int;
    }
  else{
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
    cout<<"ERROR in BMcalBooter:read_event: board num != "<<TDC_BOARDNUM<<"  ev_words[1]="<<ev_words[1]<<endl;
    error=true;
  }
  if((ev_words[0]-ev_num)!=1 && ev_num!=0){
    cout<<"ERROR in BMcalBooter:read_event: previous ev_num="<<ev_num<<"  new ev_num="<<ev_words[0]<<endl;
    error=true;    
  }
  if(ev_words[3]!=3){//provv, da modificare: da togliere nell'acquisizione e qua...
    cout<<"ERROR in BMcalBooter:read_event: ev_words[3]="<<ev_words[3]<<" ,!=3"<<endl;
    error=true;          
  }
  bmstruct.evnum=ev_words[0];
  if(error || bmcon->GetBMdebug()>9)
    for(Int_t i=0;i<bmstruct.words;i++)
      cout<<"event_num="<<data_num_ev<<"   ev_words["<<i<<"]="<<ev_words[i]<<endl;
  if(!error){//read the tdc words
    new_event=true;
    read_meas=false;
    for(Int_t i=4;i<ev_words[2]+4;i++){
      if(new_event){//global header found
        tdc_evnum=ev_words[i++];
        read_meas=true;
        if(bmcon->GetBMdebug()>9)
          cout<<"global header found, i="<<i<<"  tdc_evnum="<<tdc_evnum<<endl;
        }
      if(ev_words[i]<0 && isroma==kFALSE){//global trailer found //se uso acquisizione mio (yun)
        read_meas=false;
        new_event=true;
        bmstruct.status=ev_words[i];
        if(ev_words[i]!=-1000){
          cout<<"Warning in BMcalBooter: global trailer found with error in tdc_evnum="<<tdc_evnum<<"  trailer="<<ev_words[i]<<endl;
          error=true;
        }
        if(bmcon->GetBMdebug()>9)
          cout<<"global trailer found, i="<<i<<"  ev_words="<<ev_words[i]<<endl;
      }        
      if(ev_words[i]==0 && isroma==kTRUE){//global trailer found //se uso dati letti a Roma per BM refurbishment
        read_meas=false;
        new_event=true;
        bmstruct.status=-1000;
        if(bmcon->GetBMdebug()>9)
          cout<<"global trailer found, i="<<i<<"  ev_words="<<ev_words[i]<<endl;
      }        
      if(read_meas){  
        if(ev_words[i++]!=tdc_evnum){
          cout<<"ERROR in BMcalBooter:read_event: tdc_evnum="<<tdc_evnum<<"  measured event number="<<ev_words[i-1]<<"  i="<<i<<endl;
          error=true;
        }
        if(ev_words[i]>-1 && ev_words[i]<bmmap->GetTdcMaxcha()){//measure found
          bmstruct.hit_id[bmstruct.hitnum]=bmmap->tdc2cell(ev_words[i++]);
          bmstruct.hit_meas[bmstruct.hitnum]=ev_words[i]/10.;//conversion to ns
          if(ev_words[i-1]==bmmap->GetTrefCh()){
            bmstruct.synctime[sync_evnum]=ev_words[i]/10.;
            sync_evnum++;
          }
          bmstruct.hitnum++;
        }
        else
          cout<<"ERROR in BMcalBooter:read_event: tdc_channel out of range!!! tdc_channel="<<ev_words[i]<<endl;
        new_event=false;
        if(bmcon->GetBMdebug()>9)
          cout<<"measure found: tdc_evnum="<<tdc_evnum<<" hit_id="<<bmstruct.hit_id<<" hit_meas="<<bmstruct.hit_meas<<endl;
      }
    }//end of reading tdc words for loop
  }else{//read tdc words if    
    cout<<"data_num_ev="<<data_num_ev<<endl;
    for(Int_t i=0;i<bmstruct.words;i++)
        cout<<"ev_words["<<i<<"]="<<ev_words[i]<<endl;
  }
  return kTRUE;
}



Bool_t BmBooter::drop_event(){
  Int_t tmp_int, trash;
  if(datafile.read((char *) &tmp_int,sizeof(int))){
    for(Int_t i=0;i<tmp_int;i++)
      datafile.read((char *) &trash,sizeof(int));
  }else
    return kFALSE;  

  return kTRUE;
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


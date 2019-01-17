#include "BMcalBooter.hxx"

using namespace std;



//----------------------------------------------------------------------------------------------------
BMcalBooter::BMcalBooter() {
}




void BMcalBooter::Initialize( TString wd_in, TString instr_in, bool isroma_in ) {
    isroma=isroma_in;
    data_num_ev=0;
    m_wd=wd_in;  
    m_instr=instr_in;
    clear_bmstruct(kTRUE);
    //detector positioning and global to local transformations
    fGeoTrafo = new TAGgeoTrafo();   
    TString filename = m_wd + "/FOOT_geo.map";   // obsolete, to be removed carefully
    fGeoTrafo->InitGeo(filename.Data());
    // Setting up the detectors that we want to decode.    ;
    //~ if( GlobalPar::GetPar()->IncludeMSD() )             FillMCMSD(evStr);
    TGeoManager *masterGeo = new TGeoManager("genfitGeom", "GENFIT geometry");
  	Materials* listMaterials = new Materials() ;
    listMaterials->PrintCompMap();
	  listMaterials->PrintMatMap();
    top = gGeoManager->MakeBox("TOPPER", gGeoManager->GetMedium("AIR"), 25., 25., 120.);
    gGeoManager->SetTopVolume(top); // mandatory !
    //~ string magFieldMapName = GlobalPar::GetPar()->MagFieldInputMapName();
    // genfit::FieldManager::getInstance()->init(new genfit::ConstField(0. ,10., 0.)); // 1 T
    // genfit::FieldManager::getInstance()->init(new genfit::ConstField(0. ,0., 0.)); // no mag
    //~ genfit::FieldManager::getInstance()->init( new FootField(magFieldMapName.c_str()) ); // variable field
    // genfit::FieldManager::getInstance()->init( new FootField( 7 ) ); // const field
    myp_bmgeo  = new TAGparaDsc("myp_bmgeo", new TABMparGeo());
    myp_bmcon  = new TAGparaDsc("myp_bmcon", new TABMparCon());
    myp_bmmap = new TAGparaDsc("myp_bmmap", new TABMparMap());
    initBMGeo();
    initBMCon();
    initBMMap();
    bmcon = (TABMparCon*) myp_bmcon->Object();
    bmmap = (TABMparMap*) myp_bmmap->Object();
    //~ bmgeo = (TABMparGeo*) myp_bmgeo->Object();

    //~ CalculateT0();
    
    datafile.open(m_instr.Data(), ios::in | ios::binary);
    if(!datafile.is_open())
      cout<<"ERROR in BMcalBooter::CalculateT0: cannot open the datafile="<<m_instr.Data()<<endl;
    evaluateT0();
    datafile.close();
    datafile.open(m_instr.Data(), ios::in | ios::binary);
    if(!datafile.is_open())
      cout<<"ERROR in BMcalBooter::CalculateT0: cannot open the datafile="<<m_instr.Data()<<endl;

    //~ if( GlobalPar::GetPar()->IncludeMSD() ) {
      //~ m_msdgeo = shared_ptr<TAMSDparGeo> ( (TAMSDparGeo*) myp_msdgeo->Object() );
        //~ //Initialization of MSD parameters
        //~ m_msdgeo->InitGeo();
        //~ top->AddNode( m_msdgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  0, new TGeoRotation("Strip",0,0,0)) );
    //~ }
    // set material and geometry into genfit
    MaterialEffects* materialEffects = MaterialEffects::getInstance();
    materialEffects->init(new TGeoMaterialInterface());
    //--- close the geometry
    // gGeoManager->CloseGeometry();
    //--- draw the ROOT box
    gGeoManager->SetVisLevel(10);
    GeoPrint();
    FillDataBeamMonitor();
    
    tot_num_ev=data_num_ev;  
    data_num_ev=0;
      
    if (GlobalPar::GetPar()->Debug()>10)
      cout<<"I finish BMcalBooter::Initialize"<<endl;
    
}




void BMcalBooter::Process() {
    
  if (GlobalPar::GetPar()->Debug()>0)
    cout<<"I'm in BMcalBooter::Process"<<endl;
  
  
  if(data_num_ev==0 || (isroma && data_num_ev==1)){
    drop_event();
    data_num_ev++;
    return;
  }
  
  // start time
  start_kal = clock();
  
  Bool_t error;
  read_event(error);
  if(!error){
    TABMdatRaw* bmdatraw = (TABMdatRaw*) (gTAGroot->FindDataDsc("myn_bmdatraw", "TABMdatRaw")->GenerateObject());  
    TAIRdatRaw* stdatraw = (TAIRdatRaw*) (gTAGroot->FindDataDsc("myn_stdatraw", "TAIRdatRaw")->GenerateObject());
    TABMntuRaw* bmraw = (TABMntuRaw*) (gTAGroot->FindDataDsc("myn_bmraw", "TABMntuRaw")->GenerateObject());
    //~ TABMntuTrack* bmtack = (TABMntuTrack*) (gTAGroot->FindDataDsc("myn_bmtrk", "TABMntuTrack")->GenerateObject());

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
    
  }
  
  // stop time
  end_kal = clock();
  //~ m_tempo_kal+=(double)(end_kal-start_kal);
  

  data_num_ev++;

  if (GlobalPar::GetPar()->Debug()>0)
    cout<<"I finish BMcalBooter::Process"<<endl;  
        
}


void BMcalBooter::Finalize() {

    //~ if ( GlobalPar::GetPar()->IsPrintOutputFile() ) {
        //~ ControlPlotsRepository::GetControlObject( "BooterFinalize" )->PrintOutputFile();//oggeto lascio BooterFinalize?????
      //~ }
    //~ else                        
        //~ ControlPlotsRepository::GetControlObject( "BooterFinalize" )->PrintMap();
    
    //~ if( GlobalPar::GetPar()->IsPrintOutputNtuple() )        
        //~ ControlPlotsRepository::GetControlObject( "BooterFinalize" )->PrintOutputNtuple();
    
    datafile.close();

    //~ if (GlobalPar::GetPar()->Debug() > 1)   eventListFile.close();

}



//----------------------------------------------------------------------------------------------------


void BMcalBooter::FillDataBeamMonitor() {
  if (GlobalPar::GetPar()->Debug()>10)
    cout<<"I'm in BMcalBooter::FillDataBeamMonitor"<<endl;  
  
  myn_bmdatraw    = new TAGdataDsc("myn_bmdatraw", new TABMdatRaw());
  new TABMactDatRaw("an_bmdatraw",myn_bmdatraw, myp_bmgeo, &bmstruct); 

  myn_stdatraw    = new TAGdataDsc("myn_stdatraw", new TAIRdatRaw());
  new TAIRactDatRaw("an_stdatraw", myn_stdatraw, myp_bmmap, &bmstruct);
   
  myn_bmraw    = new TAGdataDsc("myn_bmraw", new TABMntuRaw());
  new TABMactNtuRaw("an_bmraw", myn_bmraw, myn_bmdatraw, myn_stdatraw, myp_bmgeo, myp_bmcon); 
  
  myn_bmtrk    = new TAGdataDsc("myn_bmtrk", new TABMntuTrack());  
  new TABMactNtuTrack("an_bmtrk", myn_bmtrk, myn_bmraw, myp_bmgeo, myp_bmcon);
    
  if (GlobalPar::GetPar()->Debug()>10)
    cout<<"I finish BMcalBooter::FillDataBeamMonitor"<<endl;  
  return;
}



//----------------------------------------------------------------------------------------------------
void BMcalBooter::initBMMap(){
  myp_bmmap = gTAGroot->FindParaDsc("myp_bmmap", "TABMparMap");
  if (myp_bmmap == 0) {
    cout << "p_bmmap not found or holding wrong parameter object type" << endl;
    return;
  }
  TABMparMap* o_bmmap = (TABMparMap*) myp_bmmap->Object();
  TABMparGeo* o_bmgeo = (TABMparGeo*) myp_bmgeo->Object();
  o_bmmap->Clear();
  Bool_t b_bad = kTRUE;
  TString filename = m_wd + "/geomaps/beammonitor_geoch.map";
  //~ cout << "   from file " << filename << endl;
  b_bad = o_bmmap->FromFile(filename, o_bmgeo);  
  if (!b_bad) myp_bmmap->SetBit(TAGparaDsc::kValid);  
  return;
}

void BMcalBooter::initBMCon()  {
  //~ Int_t i_run = gTAGroot->CurrentRunNumber();
  //~ Int_t i_cam = gTAGroot->CurrentCampaignNumber();
  //~ cout << "Loading beamcon for cam/run = " << i_cam << "/" << i_run << endl;
  myp_bmcon = gTAGroot->FindParaDsc("myp_bmcon", "TABMparCon");
  if (myp_bmcon == 0) {
    cout << "p_bmcon not found or holding wrong parameter object type" << endl;
    return;
  }
  TABMparCon* pro_bmcon = (TABMparCon*) myp_bmcon->Object();
  //~ TABMparMap* o_bmmap = (TABMparMap*) myp_bmmap->Object();
  pro_bmcon->Clear();
  Bool_t b_bad = kTRUE;
  TString filename = m_wd + "/config/beammonitor.cfg";
  //~ cout << "   from file " << filename << endl;
  b_bad = pro_bmcon->FromFile(filename);
  //~ filename = m_wd + "/config/beammonitor_t0s.cfg";
  //~ pro_bmcon->loadT0s(filename);//da modificare!!!
  //~ filename = m_wd + "/config/file_stlist_FIRST.txt";
  //  filename = "config/file_stlist_8020_Cst1_1750.txt";
  //~ pro_bmcon->LoadSTrel(filename);
  //~ pro_bmcon->SetIsMC(true);//provv dovrebbe farlo nel momento della lettura del file
  //~ pro_bmcon->ConfigureTrkCalib();
  filename = m_wd + "/config/bmreso_vs_r.root";
  pro_bmcon->LoadReso(filename);
  if (!b_bad) 
    myp_bmcon->SetBit(TAGparaDsc::kValid);
  
  
  return;
}


void BMcalBooter::initBMGeo()  {

  myp_bmgeo = gTAGroot->FindParaDsc("myp_bmgeo", "TABMparGeo");
  if (myp_bmgeo == 0) {
    cout << "p_bmgeo not found or holding wrong parameter object type" << endl;
    return;
  }

  Int_t i_run = gTAGroot->CurrentRunNumber();
  Int_t i_cam = gTAGroot->CurrentCampaignNumber();

  Info("BMcalBooter::initBMGeo","Loading p_bmgeo for cam/run = %d / %d",i_cam,i_run);

  //Initialization of BM parameters
  TABMparGeo* o_bmgeo = (TABMparGeo*) myp_bmgeo->Object();
  o_bmgeo->InitGeo();
  top->AddNode( o_bmgeo->GetVolume(), 0, new TGeoCombiTrans( o_bmgeo->GetCenter().X(),o_bmgeo->GetCenter().Y(),o_bmgeo->GetCenter().Z(),new TGeoRotation("BeamMonitor",0,0,0)) );
   
  myp_bmgeo->SetBit(TAGparaDsc::kValid);

  return;

}




void BMcalBooter::evaluateT0() {
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
        cout<<"BMcalBooter::evaluateT0::ERROR in the data detected at data_num_ev="<<data_num_ev<<endl;
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
  
  bmcon->PrintT0();  
    
  f_out->Write();
  f_out->Close();    
  
    
  return;
}


Bool_t BMcalBooter::read_event(bool &error) {
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




void BMcalBooter::GeoPrint() {
    
    // save an image of the foot geometry
    //top->Draw("ogl");
    TCanvas* mirror = new TCanvas("footGeometry", "footGeometry",  700, 700);
    top->Draw("ap");
    mirror->SaveAs("footGeometry.png");
    mirror->SaveAs("footGeometry.root");

    
    // save the geometry info in .root
    TFile *outfile = TFile::Open("genfitGeomFOOT.root","RECREATE");
    gGeoManager->Write();
        
    outfile->Close();

}

void BMcalBooter::clear_bmstruct(Bool_t forced){
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
  
  
Bool_t BMcalBooter::drop_event(){
  Int_t tmp_int, trash;
  if(datafile.read((char *) &tmp_int,sizeof(int))){
    for(Int_t i=0;i<tmp_int;i++)
      datafile.read((char *) &trash,sizeof(int));
  }else
    return kFALSE;  

  return kTRUE;
}


//OLD METHOD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*
Bool_t BMcalBooter::CalculateT0() {
  vector<vector<int>> tdc_chtime(bmmap->GetTdcMaxcha());
  
  ifstream inF;
  int words_num; //number of words of the event
  int tmp_int; //dummy int variable to read and convert the input file
  //~ vector<int> ev_words;//words of the event
  Bool_t error=false, new_event, read_meas;
  int board_num=513;//number of board used in the acquisition //provv, da modificare: mettere in config file questo numero
  int ev_num=0;//number of events given by the acquisition
  int tdc_evnum, tdc_cha;
  double tdc_meas;
  int rate=0;//provv, da aggiungere grafico rate...
  if(bmcon->GetBMdebug()>9)
    cout<<"I'm in BMcalBooter:CalculateT0"<<endl;
  inF.open(m_instr.Data(), ios::in | ios::binary);
  if(!inF.is_open())
    cout<<"ERROR in BMcalBooter::CalculateT0: cannot open the datafile="<<m_instr.Data()<<endl;
  cout<<"ho aperto file"<<endl;
  //jump the first event (problem of the acquisition software...)
  inF.read((char *) &tmp_int,sizeof(int));
  words_num=tmp_int;
  if(bmcon->GetBMdebug()>9)
    cout<<"first words_num="<<words_num<<endl;
  for(int i=0;i<words_num;i++)
    inF.read((char *) &tmp_int,sizeof(int));
  while(inF.read((char *) &tmp_int,sizeof(int))) {//loop on .dat file    
    words_num = tmp_int;//number of words of the event convertion [ppc and intel]
    error=false;
    int ev_words[words_num], tmp_evwords[words_num];
    //~ for(int i=0;i<words_num;i++){
      //~ inF.read((char *) &tmp_int,sizeof(int));
      //~ tmp_evwords[i]=tmp_int;
    //~ }//now ev_words contain all the words of the event
    inF.read((char *) tmp_evwords,sizeof(int)*words_num);
    for(int i=0;i<words_num;i++)
      ev_words[i]=tmp_evwords[i];
    for(int i=0;i<words_num;i++)
      cout<<"ev_words["<<i<<"]="<<ev_words[i]<<endl;
    //some check on the event words:
    if(ev_words[1]!=513){
      cout<<"ERROR in BMcalBooter:CalculateT0: board num != "<<board_num<<"  ev_words[1]="<<ev_words[1]<<endl;
      error=true;
    }
    if((ev_words[0]-ev_num)!=1 && ev_num!=0){
      cout<<"ERROR in BMcalBooter:CalculateT0: previous ev_num="<<ev_num<<"  new ev_num="<<ev_words[0]<<endl;
      error=true;    
    }
    if(ev_words[3]!=3){//provv, da modificare: da togliere nell'acquisizione e qua...
      cout<<"ERROR in BMcalBooter:CalculateT0: ev_words[3]="<<ev_words[3]<<" ,!=3"<<endl;
      error=true;          
    }
    ev_num=ev_words[0];
    if(error || bmcon->GetBMdebug()>9)
      for(int i=0;i<words_num;i++)
        cout<<"ev_words["<<i<<"]="<<ev_words[i]<<endl;
    if(!error){//read the tdc words
      new_event=true;
      read_meas=false;
      for(int i=4;i<ev_words[2]+4;i++){
        if(new_event){//global header found
          tdc_evnum=ev_words[i++];
          read_meas=true;
          if(bmcon->GetBMdebug()>9)
            cout<<"global header found, i="<<i<<"  tdc_evnum="<<tdc_evnum<<endl;
          }
        if(read_meas){  
          if(ev_words[i++]!=tdc_evnum){
            cout<<"ERROR in BMcalBooter:CalculateT0: tdc_evnum="<<tdc_evnum<<"  measured event number="<<ev_words[i-1]<<endl;
            error=true;
          }
          tdc_cha=ev_words[i++];
          tdc_meas=ev_words[i++]/10.;//conversion to ns
          if(tdc_cha>-1 && tdc_cha<bmmap->GetTdcMaxcha())
            tdc_chtime[tdc_cha].push_back(tdc_meas);
          else
            cout<<"ERROR in BMcalBooter:CalculateT0: tdc_cha out of range!!! tdc_cha="<<tdc_cha<<endl;
          new_event=false;
          if(bmcon->GetBMdebug()>9)
            cout<<"measure found: tdc_evnum="<<tdc_evnum<<" tdc_cha="<<tdc_cha<<" tdc_meas="<<tdc_meas<<endl;
        }
        if(ev_words[i]<0){//global trailer found
          read_meas=false;
          new_event=true;
          if(ev_words[i]!=-1000){
            cout<<"Warning in BMcalBooter: global trailer found with error in tdc_evnum="<<tdc_evnum<<"  trailer="<<ev_words[i]<<endl;
            error=true;
          }
          if(bmcon->GetBMdebug()>9)
            cout<<"global trailer found, i="<<i<<"  ev_words="<<ev_words[i]<<endl;
        }
      }//end of reading tdc words for loop
    }//read tdc words if    
  }//end of loop on .dat file  
  inF.close();
  
  //write the T0 in the output root file
  TFile *f_out = new TFile("bm_t0.root","RECREATE");
  f_out->cd();
  TH1D* h=nullptr;
  char tmp_char[200];
  for(int i=0;i<bmmap->GetTdcMaxcha();i++){
    sprintf(tmp_char,"tdc_cha_%d",i);
    h=new TH1D(tmp_char,"Registered time;Time [ns]; counts",2000,0.,2000.);
    for(int j=0;j<tdc_chtime[i].size();j++)
      h->Fill(tdc_chtime[i][j]);
  }
  
  f_out->Write();
  f_out->Close();  
    
  return kTRUE;
}
*/

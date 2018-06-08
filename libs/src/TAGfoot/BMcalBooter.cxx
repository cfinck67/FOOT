
#include <TCanvas.h>
#include <TError.h>
#include <TMath.h>

#include <TH2.h>
#include <TH1.h>

#include "TAGaction.hxx"
#include "TAGview.hxx"

//Beam Monitor
#include "TABMparGeo.hxx"
#include "TABMparCon.hxx"
#include "TABMparMap.hxx"
#include "TABMntuRaw.hxx"
#include "TABMdatRaw.hxx"
#include "TABMntuTrack.hxx"
#include "TABMactNtuMC.hxx"
#include "TABMactNtuTrack.hxx"
#include "TABMactNtuRaw.hxx"
#include "TABMactDatRaw.hxx"
//~ #include "TABMvieTrackFOOT.hxx"

//MicroStrip Detector
#include "TAMSDparGeo.hxx"
#include "TAMSDparConf.hxx"
#include "TAMSDparMap.hxx"
#include "TAMSDntuRaw.hxx"
#include "TAMSDactNtuMC.hxx"

#include "foot_geo.h"

#include "Materials.hxx"
#include "FootField.hxx"
#include "MeasurementCreator.h"

#include <iostream>
#include <vector>



#include "BMcalBooter.hxx"

using namespace std;



//----------------------------------------------------------------------------------------------------
BMcalBooter::BMcalBooter() {
}




void BMcalBooter::Initialize( TString wd_in, TString instr_in ) {
    cout<<"sono in initialize"<<endl;
    m_wd=wd_in;  
    m_instr=instr_in;
    //detector positioning and global to local transformations
    cout<<"prima di geotrafo"<<endl;
    fGeoTrafo = new TAGgeoTrafo();   
    cout<<"fatto geotrafo"<<endl;
    TString filename = m_wd + "/FOOT_geo.map";   // obsolete, to be removed carefully
    fGeoTrafo->InitGeo(filename.Data());
    // Setting up the detectors that we want to decode.    ;
    //~ if( GlobalPar::GetPar()->IncludeMSD() )             FillMCMSD(evStr);
  	cout << "Make BM calibration Geo" << endl;
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
    initBMMap();
    initBMCon();
    CalculateT0();
    
    datafile.open(m_instr.Data(), ios::in | ios::binary);
    if(!datafile.is_open())
      cout<<"ERROR in BMcalBooter::CalculateT0: cannot open the datafile="<<m_instr.Data()<<endl;
    //~ evaluateT0();

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
      
    if (GlobalPar::GetPar()->Debug()>10)
      cout<<"I finish BMcalBooter::Initialize"<<endl;
    
}




void BMcalBooter::Process() {

  if (GlobalPar::GetPar()->Debug()>0)
    cout<<"I'm in BMcalBooter::Process"<<endl;

  // start time
  start_kal = clock();




  // stop time
  end_kal = clock();
  //~ m_tempo_kal+=(double)(end_kal-start_kal);

  if (GlobalPar::GetPar()->Debug()>0)
    cout<<"I finish BMcalBooter::Process"<<endl;  
        
}


void BMcalBooter::Finalize() {

    if ( GlobalPar::GetPar()->IsPrintOutputFile() ) {
        ControlPlotsRepository::GetControlObject( "BooterFinalize" )->PrintOutputFile();//oggeto lascio BooterFinalize?????
      }
    else                        
        ControlPlotsRepository::GetControlObject( "BooterFinalize" )->PrintMap();
    
    if( GlobalPar::GetPar()->IsPrintOutputNtuple() )        
        ControlPlotsRepository::GetControlObject( "BooterFinalize" )->PrintOutputNtuple();
    
    datafile.close();

    //~ if (GlobalPar::GetPar()->Debug() > 1)   eventListFile.close();

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







//--------------------------------------------------------------------------------------------------

// void BMcalBooter::DisplayIRMonitor(TAGpadGroup* pg, EVENT_STRUCT *myStr) {}






// //----------------------------------------------------------------------------------------------------
// void BMcalBooter::DisplayBeamMonitor(TAGpadGroup* pg) {

//   TCanvas *c_bmhview;
//   c_bmhview = new TCanvas("bmhview", "Beam Monitor - horizontal view",20,20,700,900);
//   pg->AddPad(c_bmhview);

//   TAGview* pbmh_view = new TABMvieTrackFOOT(myn_bmtrk,
// 					     myn_bmraw,
// 					     myp_bmgeo);
//   pbmh_view->Draw();
// }






//----------------------------------------------------------------------------------------------------


void BMcalBooter::FillDataBeamMonitor() {
  if (GlobalPar::GetPar()->Debug()>10)
    cout<<"I'm in BMcalBooter::FillDataBeamMonitor"<<endl;  
  
  myn_bmdatraw    = new TAGdataDsc("myn_bmdatraw", new TABMdatRaw());
  new TABMactDatRaw("an_bmdatraw",myn_bmdatraw, myp_bmmap, myp_bmcon); 
   
  myn_bmraw    = new TAGdataDsc("myn_bmraw", new TABMntuRaw());
  new TABMactNtuRaw("an_bmraw", myn_bmraw, myn_bmdatraw, myp_bmgeo, myp_bmcon); 
  
  myn_bmtrk    = new TAGdataDsc("myn_bmtrk", new TABMntuTrack());  
  new TABMactNtuTrack("an_bmtrk", myn_bmtrk, myn_bmraw, myp_bmgeo, myp_bmcon);
    
  if (GlobalPar::GetPar()->Debug()>10)
    cout<<"I finish BMcalBooter::FillDataBeamMonitor"<<endl;  
  return;
}



//----------------------------------------------------------------------------------------------------







//----------------------------------------------------------------------------------------------------
//~ void BMcalBooter::FillMCMSD(EVENT_STRUCT *myStr) {
  
  
   //~ /*Ntupling the MC Vertex information*/
   //~ myn_msdraw    = new TAGdataDsc("msdRaw", new TAMSDntuRaw());
   //~ // myn_msdclus   = new TAGdataDsc("msdClus", new TAMSDntuCluster());

   //~ myp_msdmap    = new TAGparaDsc("msdMap", new TAMSDparMap());

   //~ myp_msdconf  = new TAGparaDsc("msdConf", new TAMSDparConf());
   //~ TAMSDparConf* parconf = (TAMSDparConf*) myp_msdconf->Object();
   //~ TString filename = m_wd + "/config/TAMSDdetector.cfg";
   //~ parconf->FromFile(filename.Data());

   //~ myp_msdgeo    = new TAGparaDsc("msdGeo", new TAMSDparGeo());

   //~ mya_msdraw   = new TAMSDactNtuMC("msdActRaw", myn_msdraw, myp_msdgeo, myp_msdmap, myStr);
      
//~ }


//~ void BMcalBooter::MonitorBM() {}
//Yun new graphs:
//~ void BMcalBooter::MonitorBMNew(Long64_t jentry) {
  
  
  
  
  //~ return;
  //~ }
// void BMcalBooter::MonitorBMVTMat() {}
// void BMcalBooter::CalibBMVT() {}


//----------------------------------------------------------------------------------------------------
void BMcalBooter::initBMMap(){
  TABMparMap* o_bmmap = (TABMparMap*) myp_bmmap->Object();
  o_bmmap->Clear();
  Bool_t b_bad = kTRUE;
  TString filename = m_wd + "/geomaps/beammonitor_geoch.map";
  //~ cout << "   from file " << filename << endl;
  b_bad = o_bmmap->FromFile(filename);  
  if (!b_bad) myp_bmmap->SetBit(TAGparaDsc::kValid);  
  return;
}

void BMcalBooter::initBMCon()  {
  //~ Int_t i_run = gTAGroot->CurrentRunNumber();
  //~ Int_t i_cam = gTAGroot->CurrentCampaignNumber();
  //~ cout << "Loading beamcon for cam/run = " << i_cam << "/" << i_run << endl;
  TABMparCon* o_bmcon = (TABMparCon*) myp_bmcon->Object();
  TABMparMap* o_bmmap = (TABMparMap*) myp_bmmap->Object();
  o_bmcon->Clear();
  Bool_t b_bad = kTRUE;
  TString filename = m_wd + "/config/beammonitor.cfg";
  //~ cout << "   from file " << filename << endl;
  b_bad = o_bmcon->FromFile(filename);
  //~ filename = m_wd + "/config/beammonitor_t0s.cfg";
  //~ o_bmcon->loadT0s(filename);//da modificare!!!
  //~ filename = m_wd + "/config/file_stlist_FIRST.txt";
  //  filename = "config/file_stlist_8020_Cst1_1750.txt";
  //~ o_bmcon->LoadSTrel(filename);
  //~ o_bmcon->SetIsMC(true);//provv dovrebbe farlo nel momento della lettura del file
  //~ o_bmcon->ConfigureTrkCalib();
  filename = m_wd + "/config/bmreso_vs_r.root";
  o_bmcon->LoadReso(filename);
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

Bool_t BMcalBooter::CalculateT0(){
  TABMparCon* o_bmcon = (TABMparCon*) myp_bmcon->Object();
  TABMparMap* o_bmmap = (TABMparMap*) myp_bmmap->Object();

  if(o_bmcon->IsMC()){
    cout<<"WARNING in TABMparCon::CalculateT0: is MC data, no need to calculate T0!!!!!!"<<endl;
    return kFALSE;
  }
  vector<vector<int>> tdc_chtime(o_bmcon->GetTdcMaxcha());
  
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
  //~ char tmp_char[200];
  if(o_bmcon->GetBMdebug()>9)
    cout<<"I'm in BMcalBooter:CalculateT0"<<endl;
  inF.open(m_instr.Data(), ios::in | ios::binary);
  if(!inF.is_open())
    cout<<"ERROR in BMcalBooter::CalculateT0: cannot open the datafile="<<m_instr.Data()<<endl;
  cout<<"ho aperto file"<<endl;
  //jump the first event (problem of the acquisition software...)
  inF.read((char *) &tmp_int,sizeof(int));
  words_num=tmp_int;
  if(o_bmcon->GetBMdebug()>9)
    cout<<"first words_num="<<words_num<<endl;
  //~ cout<<"tmp_char[0]="<<tmp_char[0]<<"   tmp_char[0]-'0'="<<tmp_char[0]-'0'<<endl;
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
    inF.read((char *) &tmp_evwords,sizeof(int)*words_num);
    for(int i=0;i<words_num;i++)
      ev_words[i]=tmp_evwords[i];
    for(int i=0;i<words_num;i++)
      cout<<"ev_words["<<i<<"]="<<ev_words[i]<<endl;
    //some check on the event words:
    if(ev_words[1]!=513){
      cout<<"ERROR in TABMparCon:CalculateT0: board num != "<<board_num<<"  ev_words[1]="<<ev_words[1]<<endl;
      error=true;
    }
    if((ev_words[0]-ev_num)!=1 && ev_num!=0){
      cout<<"ERROR in TABMparCon:CalculateT0: previous ev_num="<<ev_num<<"  new ev_num="<<ev_words[0]<<endl;
      error=true;    
    }
    if(ev_words[3]!=3){//provv, da modificare: da togliere nell'acquisizione e qua...
      cout<<"ERROR in TABMparCon:CalculateT0: ev_words[3]="<<ev_words[3]<<" ,!=3"<<endl;
      error=true;          
    }
    ev_num=ev_words[0];
    if(error || o_bmcon->GetBMdebug()>9)
      for(int i=0;i<words_num;i++)
        cout<<"ev_words["<<i<<"]="<<ev_words[i]<<endl;
    if(!error){//read the tdc words
      new_event=true;
      read_meas=false;
      for(int i=4;i<ev_words[2]+4;i++){
        if(new_event){//global header found
          tdc_evnum=ev_words[i++];
          read_meas=true;
          if(o_bmcon->GetBMdebug()>9)
            cout<<"global header found, i="<<i<<"  tdc_evnum="<<tdc_evnum<<endl;
          }
        if(read_meas){  
          if(ev_words[i++]!=tdc_evnum){
            cout<<"ERROR in TABMparCon:CalculateT0: tdc_evnum="<<tdc_evnum<<"  measured event number="<<ev_words[i-1]<<endl;
            error=true;
          }
          tdc_cha=ev_words[i++];
          tdc_meas=ev_words[i++]/10.;//conversion to ns
          if(tdc_cha>-1 && tdc_cha<o_bmcon->GetTdcMaxcha())
            tdc_chtime[tdc_cha].push_back(tdc_meas);
          else
            cout<<"ERROR in TABMparCon:CalculateT0: tdc_cha out of range!!! tdc_cha="<<tdc_cha<<endl;
          new_event=false;
          if(o_bmcon->GetBMdebug()>9)
            cout<<"measure found: tdc_evnum="<<tdc_evnum<<" tdc_cha="<<tdc_cha<<" tdc_meas="<<tdc_meas<<endl;
        }
        if(ev_words[i]<0){//global trailer found
          read_meas=false;
          new_event=true;
          if(ev_words[i]!=-1000){
            cout<<"Warning in TABMparCon: global trailer found with error in tdc_evnum="<<tdc_evnum<<"  trailer="<<ev_words[i]<<endl;
            error=true;
          }
          if(o_bmcon->GetBMdebug()>9)
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
  for(int i=0;i<o_bmcon->GetTdcMaxcha();i++){
    sprintf(tmp_char,"tdc_cha_%d",i);
    h=new TH1D(tmp_char,"Registered time;Time [ns]; counts",2000,0.,2000.);
    for(int j=0;j<tdc_chtime[i].size();j++)
      h->Fill(tdc_chtime[i][j]);
  }
  f_out->Write();
  f_out->Close();  
    
  return kTRUE;
}



void BMcalBooter::evaluateT0(){
  TABMparCon* o_bmcon = (TABMparCon*) myp_bmcon->Object();  
  TFile *f_out = new TFile("bm_t0.root","RECREATE");
  f_out->cd();
  TH1D* h=nullptr;
  char tmp_char[200];
  for(Int_t i=0;i<o_bmcon->GetTdcMaxcha();i++){
    sprintf(tmp_char,"tdc_cha_%d",i);
    h=new TH1D(tmp_char,"Registered time;Time [ns]; counts",2000,0.,2000.);
  }
  
  while(read_event()){
    for(Int_t i=0;i<bmstruct.hitnum;i++){
      sprintf(tmp_char,"tdc_cha_%d",bmstruct.tdc_chan[i]);
      ((TH1D*)gDirectory->Get("tmp_char"))->Fill(bmstruct.tdc_meas[i]);    
    }
  }
  
  f_out->Write();
  f_out->Close();    
    
  return;
}


Bool_t BMcalBooter::read_event(){
  TABMparCon* o_bmcon = (TABMparCon*) myp_bmcon->Object();
  TABMparMap* o_bmmap = (TABMparMap*) myp_bmmap->Object();

  vector<vector<int>> tdc_chtime(o_bmcon->GetTdcMaxcha());
  
  Int_t tmp_int; //dummy int variable to read and convert the input file
  //~ vector<int> ev_words;//words of the event
  Bool_t error=false, new_event, read_meas;
  Int_t board_num=513;//number of board used in the acquisition //provv, da modificare: mettere in config file questo numero
  Int_t ev_num=0;//number of events given by the acquisition
  Int_t tdc_evnum, tdc_cha;
  double tdc_meas;
  if(o_bmcon->GetBMdebug()>9)
    cout<<"I'm in BMcalBooter:read_event"<<endl;
  //jump the first event (problem of the acquisition software...)
  if(datafile.read((char *) &tmp_int,sizeof(int)))
    bmstruct.words=tmp_int;
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
  if(ev_words[1]!=513){
    cout<<"ERROR in TABMparCon:CalculateT0: board num != "<<board_num<<"  ev_words[1]="<<ev_words[1]<<endl;
    error=true;
  }
  if((ev_words[0]-ev_num)!=1 && ev_num!=0){
    cout<<"ERROR in TABMparCon:CalculateT0: previous ev_num="<<ev_num<<"  new ev_num="<<ev_words[0]<<endl;
    error=true;    
  }
  if(ev_words[3]!=3){//provv, da modificare: da togliere nell'acquisizione e qua...
    cout<<"ERROR in TABMparCon:CalculateT0: ev_words[3]="<<ev_words[3]<<" ,!=3"<<endl;
    error=true;          
  }
  bmstruct.evnum=ev_words[0];
  bmstruct.hitnum=0;
  bmstruct.status=0;
  bmstruct.synctime=-1000;
  if(error || o_bmcon->GetBMdebug()>9)
    for(Int_t i=0;i<bmstruct.words;i++)
      cout<<"ev_words["<<i<<"]="<<ev_words[i]<<endl;
  if(!error){//read the tdc words
    new_event=true;
    read_meas=false;
    for(Int_t i=4;i<ev_words[2]+4;i++){
      if(new_event){//global header found
        tdc_evnum=ev_words[i++];
        read_meas=true;
        if(o_bmcon->GetBMdebug()>9)
          cout<<"global header found, i="<<i<<"  tdc_evnum="<<tdc_evnum<<endl;
        }
      if(read_meas){  
        if(ev_words[i++]!=tdc_evnum){
          cout<<"ERROR in TABMparCon:CalculateT0: tdc_evnum="<<tdc_evnum<<"  measured event number="<<ev_words[i-1]<<endl;
          error=true;
        }
        tmp_int=ev_words[i++];
        if(tmp_int>-1 && tmp_int<o_bmcon->GetTdcMaxcha()){
          bmstruct.tdc_chan[bmstruct.hitnum]=tmp_int;
          bmstruct.tdc_meas[bmstruct.hitnum]=ev_words[i++]/10.;//conversion to ns
          bmstruct.hitnum++;
        }
        else
          cout<<"ERROR in TABMparCon:CalculateT0: tdc_cha out of range!!! tdc_cha="<<tdc_cha<<endl;
        new_event=false;
        if(o_bmcon->GetBMdebug()>9)
          cout<<"measure found: tdc_evnum="<<tdc_evnum<<" tdc_cha="<<tdc_cha<<" tdc_meas="<<tdc_meas<<endl;
      }
      if(ev_words[i]<0){//global trailer found
        read_meas=false;
        new_event=true;
        bmstruct.status=ev_words[i];
        if(ev_words[i]!=-1000){
          cout<<"Warning in TABMparCon: global trailer found with error in tdc_evnum="<<tdc_evnum<<"  trailer="<<ev_words[i]<<endl;
          error=true;
        }
        if(o_bmcon->GetBMdebug()>9)
          cout<<"global trailer found, i="<<i<<"  ev_words="<<ev_words[i]<<endl;
      }
    }//end of reading tdc words for loop
  }//read tdc words if    
  
  return kTRUE;
}

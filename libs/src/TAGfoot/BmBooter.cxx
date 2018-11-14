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
  data_num_ev= (isdata==true) ? -1000:0;
  acq_start_ev=1;
  isallign=kFALSE;
  clear_bmstruct(kTRUE);
  myp_bmgeo = gTAGroot->FindParaDsc("myp_bmgeo", "TABMparGeo");  
  myp_bmcon = gTAGroot->FindParaDsc("myp_bmcon", "TABMparCon");  
  myp_bmmap = gTAGroot->FindParaDsc("myp_bmmap", "TABMparMap");  
  bmcon = (TABMparCon*) (gTAGroot->FindParaDsc("myp_bmcon", "TABMparCon")->Object()); 
  bmgeo = (TABMparGeo*) (gTAGroot->FindParaDsc("myp_bmgeo", "TABMparGeo")->Object());
  bmmap = (TABMparMap*) (gTAGroot->FindParaDsc("myp_bmmap", "TABMparMap")->Object());
  if(bmcon->GetParmapfile().compare("beammonitor_geoch_roma.map")==0 || bmcon->GetParmapfile().compare("beammonitor_geoch_trento.map")==0)
    isroma=kTRUE;
  else
    isroma=kFALSE;
  cell_occupy.resize(36);
  vector<Int_t> row(16,0);
  eff_pp.push_back(row);
  eff_pp.push_back(row);
  //provv
  vector<Int_t> row2(2,0);
  for(Int_t i=0;i<8;i++){
    eff_plane.push_back(row2);
    eff_fittedplane.push_back(row2);
  }    
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
    //T0
    if(bmcon->GetmanageT0BM()==0)
      bmcon->PrintT0s(m_instr, data_num_ev);
    else
      bmcon->loadT0s(data_num_ev);
    if(bmcon->GetBMdebug()>1 || bmcon->GetmanageT0BM()>1)
      bmcon->CoutT0();
    //ADC pedestals  
    if(bmmap->GetAdc792Ch()>0){
      if(bmcon->GetmanageADCped()==0)
        bmcon->PrintADCped(m_instr, data_num_ev);
      if(bmcon->GetBMdebug()>1 || bmcon->GetmanageADCped()>1)
        bmcon->CoutADCped();
    }

    datafile.open(m_instr.Data(), ios::in | ios::binary);
    if(!datafile.is_open())
      cout<<"ERROR in BmBooter::CalculateT0: cannot open the datafile="<<m_instr.Data()<<endl;
    FillDataBeamMonitor();
  }
    
  tot_num_ev=data_num_ev;  
  data_num_ev= (isdata==true) ? -1000:0;
  
return;
}





//----------------------------------------------------------------------------------------------------
void BmBooter::Process() {
  if (bmcon->GetBMdebug()>0)
    cout<<"I'm in BmBooter::Process, event number="<<data_num_ev<<endl;
  
  Int_t track_ok;
   
   //~ if(isroma && (data_num_ev==-1000 || data_num_ev==-999) && isdata ){
    //~ drop_event();
    //~ return;
  //~ }
  
  if(isdata){
    read_event(kFALSE);
    if(bmstruct.tot_status==0 && bmstruct.tdc_status==-1000){
      bmdatraw = (TABMdatRaw*) (gTAGroot->FindDataDsc("myn_bmdatraw", "TABMdatRaw")->GenerateObject());  
      stdatraw = (TAIRdatRaw*) (gTAGroot->FindDataDsc("myn_stdatraw", "TAIRdatRaw")->GenerateObject());
     
      //loop on dat hit, only for data
      for(Int_t i=0;i<bmdatraw->NHit();i++){
        bmdathit=&bmdatraw->Hit(i);
        //~ cout<<"evento numero="<<data_num_ev<<"  numero hit"<<i<<"raw hit time="<<bmdathit->Time()<<endl;
      }
    }else{
      data_num_ev++;
      return;
    }
  }
  
  bmnturaw = (TABMntuRaw*) (gTAGroot->FindDataDsc("myn_bmraw", "TABMntuRaw")->GenerateObject());
  evaluate_cell_occupy();
  
  if(bmnturaw->nhit > bmcon->GetMaxnhit_cut())
    track_ok=-2;
  else if(bmnturaw->nhit < bmcon->GetMinnhit_cut())
    track_ok=-1;  
  else if(bmcon->GetFitterIndex()>0){  
    bmntutrack = (TABMntuTrack*) (gTAGroot->FindDataDsc("myn_bmtrk", "TABMntuTrack")->GenerateObject());
    track_ok=bmntutrack->trk_status;
  }
 
  if (bmcon->GetBMdebug()>10)
    cout<<"in BmBooter::Process, I finished to create the BM hits and tracks"<<endl<<"Now I'll printout BM hits if enable"<<endl;

  if (GlobalPar::GetPar()->IsPrintOutputFile())
    m_controlPlotter->BM_setnturaw_info("BM_output",bmnturaw, bmgeo, bmcon, bmmap, cell_occupy);  
  
  if (bmcon->GetBMdebug()>10)
    cout<<"in BmBooter::Process, I finished to printout BM hits"<<endl;
  
  if (GlobalPar::GetPar()->IsPrintOutputFile() && track_ok>=0)
    if(m_controlPlotter->BM_setntutrack_info("BM_output", bmntutrack, bmnturaw, bmcon)==0)
      isallign=kTRUE;      

  //draw and save tracks
  if(bmcon->GetBMvietrack()>0 && data_num_ev%bmcon->GetBMvietrack()==0){
      TCanvas *c_bmhview = new TCanvas("bmhview", "BM_tracks",20,20,800,900);
      pg->AddPad(c_bmhview);
      //~ TAGview* pbmh_view = new TABMvieTrackFOOT(bmntutrack, bmnturaw, bmgeo, track_ok);
      TABMvieTrackFOOT* pbmh_view = new TABMvieTrackFOOT(bmntutrack, bmnturaw, bmgeo, track_ok,data_num_ev);
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

  efficiency_pivot_probe();
  efficiency_fittedplane();
  efficiency_paoloni();
  if(bmcon->GetFitterIndex()>0)
    if(track_ok==0){  
      Projecttracktr();
      ResidualDistance();
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
  PrintEFFpp();
  PrintProjections();  
  PrintResDist();  
    
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
    TString tmp_str("BM input file=");
    tmp_str+=m_instr;
    tmp_str+="BM allign par: xrot=";
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
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))==nullptr)
    return;
  
  Int_t numpoints=4000, tmp_int=1;
  Double_t pass=0.1;
  TH1D* histo=new TH1D( "strel", "Space time relation;time [ns];distance [cm]", numpoints, 0., numpoints*pass);
  TH1D* histo2=new TH1D( "time_vs_velodrift", "Drift Velocity;Time [s]; velocity[cm/s]", numpoints, 0., numpoints*pass);
  TH1D* histo3=new TH1D( "dist_vs_velodrift", "Drift Velocity;Distance [cm]; velocity[cm/s]", 800, 0.04, 0.8);
  Double_t time=0.;
  for(int i=1;i<numpoints;i++){
    time+=pass;
    histo->SetBinContent(i,bmcon->FirstSTrel(time));
    histo2->SetBinContent(i,bmcon->FirstSTrel(time)/time);
    //~ cout<<"time="<<time<<"  bmcon->FirstSTrel(time)="<<bmcon->FirstSTrel(time)<<"  tmp_int"<<tmp_int<<"  histo3->GetBinCenter(tmp_int)="<<histo3->GetBinCenter(tmp_int)<<endl;
    if(bmcon->FirstSTrel(time)>histo3->GetBinCenter(tmp_int) && bmcon->FirstSTrel(time-pass)<histo3->GetBinCenter(tmp_int)){
      histo3->SetBinContent(tmp_int, bmcon->FirstSTrel(time)/time);
      tmp_int++;
    }
  }
  


  
  
return;
}

//provv:: modificare i vari histo e usare solo un puntatore TH1D
void BmBooter::PrintEFFpp(){
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))==nullptr)
    return;
    
  //"Tommasino" method
  TH1D* histo=new TH1D( "eff_pp_pivot", "pivot counter for the pivot-probe efficiency method; Pivot-cell index; Counter", eff_pp[0].size(), 0., eff_pp[0].size());
  TH1D* histo1=new TH1D( "eff_pp_probe", "probe counter for the pivot-probe efficiency method; Pivot-cell index; Counter", eff_pp[1].size(), 0., eff_pp[1].size());
  TH1D* histo2=new TH1D( "eff_pp_eval", "efficiency for the pivot-probe method; Pivot-cell index; Counter", 16, 0., 16.);
  Double_t hist2_errors[eff_pp[0].size()];
  Double_t eff;
  for(int i=0;i<eff_pp[0].size();i++){
    histo->SetBinContent(i+1, eff_pp[0][i]);
    histo1->SetBinContent(i+1, eff_pp[1][i]);
    if(eff_pp[0][i]!=0){
      eff=(Double_t) (eff_pp[1][i])/eff_pp[0][i];
      histo2->SetBinContent(i+1, eff);
      hist2_errors[i]=TMath::Factorial(eff_pp[0][i]+2)/TMath::Factorial(eff_pp[1][i]+1)/TMath::Factorial(eff_pp[0][i]-eff_pp[1][i]+1)*pow(eff,eff_pp[1][i])*pow(1.-eff,eff_pp[0][i]-eff_pp[1][i]);
    }else{
      histo2->SetBinContent(i+1, 0.);
      hist2_errors[i]=0.;
    }
  }
  histo2->SetError(hist2_errors); 
  
  Double_t mean_planeff=0.;
  //"Paoloni" plane method
  TH1D* histo3=new TH1D( "eff_plane_pivot", "pivot counter for the plane efficiency method; index; Counter", 8, 0., 8.);
  TH1D* histo4=new TH1D( "eff_plane_probe", "probe counter for the plane efficiency method; index; Counter", 8, 0., 8.);
  TH1D* histo5=new TH1D( "eff_plane_eval", "efficiency for the paoloni plane method; index; Counter", 8, 0., 8.);
  TH1D* histo6=new TH1D( "eff_plane_mean", "efficiency for the paoloni plane method; ; mean efficiency", 1, 0., 1.);
  for(Int_t i=0;i<8;i++){
    histo3->SetBinContent(i+1, eff_plane[i][0]);
    histo4->SetBinContent(i+1, eff_plane[i][1]);
    if(eff_plane[i][0]!=0){
      histo5->SetBinContent(i+1, (Double_t) eff_plane[i][1]/eff_plane[i][0]);
      mean_planeff+=(Double_t) eff_plane[i][1]/eff_plane[i][0];
    }else
      histo5->SetBinContent(i+1, 0.);
  }
  histo6->SetBinContent(1,mean_planeff/8.);
  //~ //provv
  //~ cout<<"eff_plane_mean="<<mean_planeff/4.<<"  pivot:"<<endl;
  //~ for(Int_t i=0;i<4;i++)
    //~ cout<<"i="<<i<<"  pivot_number="<<eff_plane[i][0]<<"  probe_plane="<<eff_plane[i][1]<<"  plane_eval="<<(Double_t) eff_plane[i][1]/eff_plane[i][0]<<endl;
  

  //Plane method for fittedtracks
  TH1D* histo7=new TH1D( "eff_fittedplane_pivot", "pivot counter for the fitted plane efficiency method; index; Counter", 4, 0., 4.);
  TH1D* histo8=new TH1D( "eff_fittedplane_probe", "probe counter for the fitted plane efficiency method; index; Counter", 4, 0., 4.);
  TH1D* histo9=new TH1D( "eff_fittedplane_eval", "efficiency for the fitted paoloni plane method; index; Counter", 4, 0., 4.);
  TH1D* histo10=new TH1D( "eff_fittedplane_mean", "efficiency for the fitted paoloni plane method; ; mean efficiency", 1, 0., 1.);
  mean_planeff=0.;
  for(Int_t i=0;i<4;i++){
    histo7->SetBinContent(i+1, eff_fittedplane[i][0]);
    histo8->SetBinContent(i+1, eff_fittedplane[i][1]);
    if(eff_fittedplane[i][0]!=0){
      histo9->SetBinContent(i+1, (Double_t) eff_fittedplane[i][1]/eff_fittedplane[i][0]);
      mean_planeff+=(Double_t) eff_fittedplane[i][1]/eff_fittedplane[i][0];
    }else
      histo9->SetBinContent(i+1, 0.);
  }
  histo10->SetBinContent(1,mean_planeff/4.);  
  
  
return;
}


void BmBooter::PrintProjections(){
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))==nullptr)
    return;

  TH2D* histoa=new TH2D( "mylar1_xy", "mylar1 projected tracks; x[cm]; y[cm]", 500, -5., 5.,500, -5.,5.);
  TH2D* histob=new TH2D( "mylar2_xy", "mylar2 projected tracks; x[cm]; y[cm]", 500, -5., 5.,500, -5.,5.);
  TH2D* histoc=new TH2D( "R0_xy", "R0 projected tracks; x[cm]; y[cm]", 500, -5., 5.,500, -5.,5.);
  
  for(Int_t i=0;i<tracktr2dprojects.size();i++){
    histoa->Fill(tracktr2dprojects[i][0], tracktr2dprojects[i][1]);
    histob->Fill(tracktr2dprojects[i][2], tracktr2dprojects[i][3]);
    histoc->Fill(tracktr2dprojects[i][4], tracktr2dprojects[i][5]);
  }

return;
}


void BmBooter::PrintResDist(){
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))==nullptr)
    return;
  TH2D* histo2d;
  TH1D* histo1d;
  
  histo2d=new TH2D( "hitres_dis", "Residual vs rdrift; Residual[cm]; Measured rdrift[cm]", 250, -0.3, 0.3,250,0.,1.);
  if(isdata)
    histo2d=new TH2D( "hitres_time", "Residual vs drift time; Time[ns]; Residual[cm]", 350, 0., 350.,600,-0.3,0.3);

  //create ResVsDist_perCell graphs
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->mkdir("ResVsDist_perCell");
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd("ResVsDist_perCell");
  char tmp_char[200];
  for(Int_t i=0;i<36;i++){
    sprintf(tmp_char,"hitres_dis_perCell_%d",i);  
    histo2d=new TH2D( tmp_char, "Residual vs rdrift; Residual[cm]; Measured rdrift[cm]", 300, -0.3, 0.3,250,0.,1.);
  }
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output/ResVsDist_perCell")))->cd("..");
  
  //create TDC_dist
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->mkdir("TDC_time");
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd("TDC_time");
  for(Int_t i=0;i<36;i++){
    sprintf(tmp_char,"tdc_cha_%d",i);  
    histo1d=new TH1D( tmp_char, "Drift time charged; Time[ns]; counts", 3000, -1000, 2000);
  }
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output/TDC_time")))->cd("..");  
  
  cout<<"fatto i grafici inizio a fillare"<<endl;
  
  //fill the histos
  for(Int_t i=0;i<residual_distance.size();i++){
    if(residual_distance[i].size()!=2){
      ((TH2D*)(m_controlPlotter->GetTFile()->Get("BM_output/hitres_dis")))->Fill(residual_distance[i][3], residual_distance[i][1]);
      if(isdata)
        ((TH2D*)(m_controlPlotter->GetTFile()->Get("BM_output/hitres_time")))->Fill(residual_distance[i][2], residual_distance[i][3]);
      sprintf(tmp_char,"BM_output/ResVsDist_perCell/hitres_dis_perCell_%d",(Int_t) (residual_distance[i][0]+0.5));  
      ((TH2D*)(m_controlPlotter->GetTFile()->Get(tmp_char)))->Fill(residual_distance[i][3], residual_distance[i][1]);    
    }else if(isdata){
      sprintf(tmp_char,"BM_output/TDC_time/tdc_cha_%d",(Int_t) (residual_distance[i][0]+0.5));      
      ((TH1D*)(m_controlPlotter->GetTFile()->Get(tmp_char)))->Fill(residual_distance[i][1]);
    }
  }
  
return;
}




void BmBooter::evaluateT0() {
  //~ TABMparCon* bmcon = (TABMparCon*) myp_bmcon->Object();  
  TString tmp_tstring("bmraw.root");
  if(m_instr.EndsWith(".dat")){
    tmp_tstring=m_instr;
    if(tmp_tstring.Last('/'))  
      tmp_tstring.Remove(0,tmp_tstring.Last('/')+1);
    tmp_tstring.Replace(tmp_tstring.Last('.')+1,3,"root",4);
    tmp_tstring="bmraw_"+tmp_tstring;
  }
  TFile *f_out = new TFile(tmp_tstring.Data(),"RECREATE");
  f_out->cd();
  TH1D* h=nullptr;
  Int_t tmp_int, trash;
  char tmp_char[200];
  Int_t adc_maxbin=4200;

  //book histos
  //general histos
  h=new TH1D("rate_evtoev","Rate of the events, from bmstruct.time_evtoev;Hz; Number of events",1000,0.,1000.);
  //~ h=new TH1D("rate_timeacq","Rate of the events, from bmstruct.time_acq;Hz; Number of events",1000,0.,10000.);
  h=new TH1D("rate_readev","time occurred to read the data for each event, from bmsturct.time_read;[micro seconds]; Number of events",1000,1000.,100000.);
        
  f_out->mkdir("TDC");
  f_out->cd("TDC");
  gDirectory->mkdir("TDC_raw");
    gDirectory->cd("TDC_raw");
    h=new TH1D("all_tdc_rawcha","Number of tdc signals; TDC channel; counts",bmmap->GetTdcMaxcha(),0.,bmmap->GetTdcMaxcha());
    for(Int_t i=0;i<bmmap->GetTdcMaxcha();i++){
      if(i!=bmmap->GetTrefCh())
        sprintf(tmp_char,"tdc_rawcha_%d",i);
      else
        sprintf(tmp_char,"tdc_rawsynccha_%d",i);  
      h=new TH1D(tmp_char,"Registered time;Time [ns]; counts",3000,-1000.,2000.);
    }
    gDirectory->cd("..");
    
  gDirectory->mkdir("TDC_meas");
    gDirectory->cd("TDC_meas");
    for(Int_t i=0;i<bmmap->GetTdcMaxcha();i++){
      if(i!=bmmap->GetTrefCh())
        sprintf(tmp_char,"tdc_cha_%d",i);
      else
        sprintf(tmp_char,"tdc_synccha_%d",i);  
      h=new TH1D(tmp_char,"Registered time;Time [ns]; counts",3000,-1000.,2000.);
    }
    gDirectory->cd("..");
    
  gDirectory->mkdir("TDC_meas_less_sync");
    gDirectory->cd("TDC_meas_less_sync");
    for(Int_t i=0;i<bmmap->GetTdcMaxcha();i++)
      if(i!=bmmap->GetTrefCh()){
        sprintf(tmp_char,"tdc_cha-sync_%d",i);
        h=new TH1D(tmp_char,"TDC time - synctime;Time [ns]; counts",3000,-1000.,2000.);
      }
    gDirectory->cd("..");
  
  h=new TH1D("all_tdc_cha","Number of tdc signals; TDC channel; counts",bmmap->GetTdcMaxcha(),0.,bmmap->GetTdcMaxcha());
  h=new TH1D("tdc_error","Number of tdc signals with errors; Event number; counts",10,0.,10);//distinguish the type of error!
  f_out->cd("..");
  if(bmmap->GetSca830Ch()>0){
    f_out->mkdir("SCA");
    f_out->cd("SCA");
    for(Int_t i=0;i<bmmap->GetSca830Ch();i++){
      sprintf(tmp_char,"sca_cha_%d",i);  
      h=new TH1D(tmp_char,"Scaler counts;Scaler counts; Number of events",1000,0.,1000.);
    }    
    h=new TH1D("sca_allch","Scaler channel with at least 1 count x event;Scaler channel; Number of events",bmmap->GetSca830Ch(),0.,bmmap->GetSca830Ch());
    h=new TH1D("sca_error","Scaler Error;0=no_error    1=error; Number of events",2,0.,2);
    f_out->cd("..");
  }
  if(bmmap->GetAdc792Ch()>0){
    f_out->mkdir("ADC");
    f_out->cd("ADC");
    h=new TH1D("adc_error","Adc Error;0=no_error  1=read data error  2=overflow channels  3=channel overflow not set; Number of events",3,0.,3);
    h=new TH1D("adc_pedmeas","Adc pedestals level ;channel; Number of counts",bmmap->GetAdc792Ch(),0.,bmmap->GetAdc792Ch());
    h=new TH1D("adc_pedrms","Adc pedestals rms ;channel; Counts rms",bmmap->GetAdc792Ch(),0.,bmmap->GetAdc792Ch());   
    gDirectory->mkdir("ADC_meas");
    gDirectory->cd("ADC_meas");
      for(Int_t i=0;i<bmmap->GetAdc792Ch();i++){
        sprintf(tmp_char,"adc_meas_cha_%d",i);  
        h=new TH1D(tmp_char,"adc measurements;Adc measurements; Number of events",adc_maxbin,0.,adc_maxbin);
      }
    gDirectory->cd("..");
    gDirectory->mkdir("ADC_over");
    gDirectory->cd("ADC_over");
      for(Int_t i=0;i<bmmap->GetAdc792Ch();i++){
        sprintf(tmp_char,"adc_over_cha_%d",i);  
        h=new TH1D(tmp_char,"adc overflow;Adc overflow events: 0=no overr, 1=over, 2=not set; Number of events",3,0.,3.);
      }    
    gDirectory->cd("..");
    if(bmcon->GetmanageADCped()!=0){
      bmcon->loadADCped(bmmap->GetAdc792Ch());
      gDirectory->mkdir("ADC_noped");
      gDirectory->cd("ADC_noped");
        for(Int_t i=0;i<bmmap->GetAdc792Ch();i++){
          sprintf(tmp_char,"adc_noped_cha_%d",i);  
          h=new TH1D(tmp_char,"ADC measurements - pedestals;ADC measurements - pedestals; Number of events",adc_maxbin,0.,adc_maxbin);
        }    
      gDirectory->cd("..");      
    }
    f_out->cd("..");
  }
  gDirectory->cd("..");
  
  //~ if(bmmap->GetAdc792Ch()>0){
    //~ f_out->mkdir("ADC");
    //~ f_out->cd("ADC");
    //~ for(Int_t i=0;i<bmmap->GetAdc792Ch();i++){
      //~ sprintf(tmp_char,"adc_cha_%d",i);  
      //~ h=new TH1D(tmp_char,"Adc counts;adc counts; Number of events",3000,-1000.,2000.);
    //~ }    
    //~ f_out->cd("..");
  //~ }
  
  
  //~ f_out->ls();
  
  //jump the first event IS NECESSARY????????????
  //~ drop_event();
  if(isroma){//jump two event if isroma    
    drop_event();
    drop_event();
  }
      
  //charge the tdc_cha_* TH1D graph of the tdc signals    
  while(read_event(kTRUE)) {
    if(bmcon->GetBMdebug()>11 && bmcon->GetBMdebug()!=99)
      cout<<"data_num_ev="<<data_num_ev<<endl<<"Fill the general graph and tdc"<<endl;
    //General Graphs
    ((TH1D*)gDirectory->Get("rate_evtoev"))->Fill(1000000./bmstruct.time_evtoev);
    ((TH1D*)gDirectory->Get("rate_readev"))->Fill(bmstruct.time_read);
    
    //TDC  
    tmp_int=0;
    sprintf(tmp_char,"TDC/TDC_raw/tdc_rawsynccha_%d",bmmap->GetTrefCh());
    while(bmstruct.tdc_sync[tmp_int]!=-10000){
      ((TH1D*)gDirectory->Get("TDC/TDC_raw/all_tdc_rawcha"))->Fill(bmmap->GetTrefCh());       
      ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.tdc_sync[0])/10.);    
      tmp_int++;
    }
    tmp_int=0;
    while(bmstruct.tdc_meas[tmp_int]!=-10000){
      sprintf(tmp_char,"TDC/TDC_raw/tdc_rawcha_%d",bmstruct.tdc_id[tmp_int]);
      ((TH1D*)gDirectory->Get("TDC/TDC_raw/all_tdc_rawcha"))->Fill(bmstruct.tdc_id[tmp_int]);            
      ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.tdc_meas[tmp_int])/10.);    
      tmp_int++;
    }    
    if(bmstruct.tot_status==0 && bmstruct.tdc_status==-1000){ 
      if(bmstruct.tdcev==1 && bmstruct.tdc_sync[0]!=-10000 && bmstruct.tdc_sync[1]==-10000){
        ((TH1D*)gDirectory->Get("TDC/tdc_error"))->Fill(0);//no error
        ((TH1D*)gDirectory->Get("TDC/all_tdc_cha"))->Fill(bmmap->GetTrefCh());   
        sprintf(tmp_char,"TDC/TDC_meas/tdc_synccha_%d",bmmap->GetTrefCh());  
        ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.tdc_sync[0])/10.);    
        for(Int_t i=0;i<bmstruct.tdc_hitnum[0];i++){
          //~ cout<<"data_num_ev="<<data_num_ev<<"  i="<<i<<"  bmstruct.tdc_hitnum[0]="<<bmstruct.tdc_hitnum[0]<<"  bmstruct.tdc_id[i]="<<bmstruct.tdc_id[i]<<"  bmmap->tdc2cell(bmstruct.tdc_id[i])="<<bmmap->tdc2cell(bmstruct.tdc_id[i])<<"  (Double_t) (bmstruct.tdc_meas[i])/10.="<<(Double_t) (bmstruct.tdc_meas[i])/10.<<endl; //provv
          if(bmmap->tdc2cell(bmstruct.tdc_id[i])!=-1){
            sprintf(tmp_char,"TDC/TDC_meas/tdc_cha_%d",bmstruct.tdc_id[i]);
            ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.tdc_meas[i])/10.);    
            sprintf(tmp_char,"TDC/TDC_meas_less_sync/tdc_cha-sync_%d",bmstruct.tdc_id[i]);
            ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.tdc_meas[i]-bmstruct.tdc_sync[0])/10.);    
            ((TH1D*)gDirectory->Get("TDC/all_tdc_cha"))->Fill(bmstruct.tdc_id[i]);    
          }   
        } 
      }else{
        if(bmstruct.tdc_sync[0]==-10000){
          cout<<"BmBooter::evaluateT0::ERROR no sync in the TDC data at data_num_ev="<<data_num_ev<<endl;
          ((TH1D*)gDirectory->Get("TDC/tdc_error"))->Fill(4);//no synctime
        }
        else if(bmstruct.tdc_sync[1]!=-10000){      
          if(bmcon->GetBMdebug()>0)
            cout<<"BmBooter::evaluateT0::ERROR multisync in the TDC data at data_num_ev="<<data_num_ev<<endl;
          ((TH1D*)gDirectory->Get("TDC/tdc_error"))->Fill(5);//multi synctime   
        }else if(bmstruct.tdcev!=1){
          if(bmcon->GetBMdebug()>0)
            cout<<"BmBooter::evaluateT0::ERROR multievent in the TDC data at data_num_ev="<<data_num_ev<<endl;
          ((TH1D*)gDirectory->Get("TDC/tdc_error"))->Fill(6);//multi event             
        }   
      }
    }else if(bmstruct.tdc_status!=-1000){
      ((TH1D*)gDirectory->Get("TDC/tdc_error"))->Fill(bmstruct.tdc_status);//other tdc error  
    }else if(bmstruct.tot_status!=0){
      ((TH1D*)gDirectory->Get("TDC/tdc_error"))->Fill(9);//other tdc error  
    }
    
    if(bmcon->GetBMdebug()>11 && bmcon->GetBMdebug()!=99)
      cout<<"Fill the scaler:"<<endl;    
    
    //SCALER
    if(bmmap->GetSca830Ch()>0){    
      ((TH1D*)gDirectory->Get("SCA/sca_error"))->Fill(bmstruct.sca_status);
      if(bmstruct.sca_status==0)
        for(Int_t i=0;i<bmmap->GetSca830Ch();i++){
          sprintf(tmp_char,"SCA/sca_cha_%d",i);  
          ((TH1D*)gDirectory->Get(tmp_char))->Fill(bmstruct.sca830_meas[i]);
          if(bmstruct.sca830_meas[i]>0)
            ((TH1D*)gDirectory->Get("SCA/sca_allch"))->Fill(i);
        }    
    }
    //ADC
    if(bmmap->GetAdc792Ch()>0){
      Double_t mulf_qdc = 400./4095;// perchè c'è fattore di conversione?
      ((TH1D*)gDirectory->Get("ADC/adc_error"))->Fill(bmstruct.adc_status);
      if(bmstruct.sca_status!=1){
        for(Int_t i=0;i<bmmap->GetAdc792Ch();i++){
          sprintf(tmp_char,"ADC/ADC_meas/adc_meas_cha_%d",i);
          ((TH1D*)gDirectory->Get(tmp_char))->Fill(bmstruct.adc792_meas[i]);
          sprintf(tmp_char,"ADC/ADC_over/adc_over_cha_%d",i);  
          ((TH1D*)gDirectory->Get(tmp_char))->Fill((bmstruct.adc792_over[i]==-10000) ? 3:bmstruct.adc792_over[i]);
        }
        if(bmcon->GetmanageADCped()!=0)
          for(Int_t i=0;i<bmmap->GetAdc792Ch();i++){
            sprintf(tmp_char,"ADC/ADC_noped/adc_noped_cha_%d",i);
            ((TH1D*)gDirectory->Get(tmp_char))->Fill(bmstruct.adc792_meas[i]-bmcon->GetADCped(i));
          }
      }
    }
    data_num_ev++;
  }
  data_num_ev--;
  //I created the TDC signal histograms
  
  //fit the tdc signals with a function to evaluate the T0, for the moment I take the shortest signal close to the peak
  //~ TF1 *f1 = new TF1("f1","[0]*pow(([1]/[2]),(x/[2]))(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1)", 0, 2000);
  //~ TF1 *f1 = new TF1("f1","[0]*pow([1]/[2],x/[2]-[3])/(TMath::Gamma(x/[2]-[3]+1))*(TMath::Exp(-[1]/[2]))", 0, 2000);
  //~ TF1 *f1 = new TF1("f1","gaus(0)", ((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()-100, ((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()+100);


  //EVALUATE T0
  Int_t tdc_peak;
  if(bmcon->GetBMdebug()>3)
    cout<<"BMbooter::evaluatet0 step EVALUATE T0"<<endl;
  if(bmcon->GetmanageT0BM()==0){
    for(Int_t i=0;i<bmmap->GetTdcMaxcha();i++){
      if(bmmap->tdc2cell(i)>=0){
        sprintf(tmp_char,"TDC/TDC_meas_less_sync/tdc_cha-sync_%d",i);
        //~ cout<<"i="<<i<<" bmmap->tdc2cell(i)="<<bmmap->tdc2cell(i)<<"  ((TH1D*)gDirectory->Get(tmp_char))->GetEntries()="<<((TH1D*)gDirectory->Get(tmp_char))->GetEntries()<<"  ((TH1D*)gDirectory->Get(tmp_char))->FindFirstBinAbove()="<<((TH1D*)gDirectory->Get(tmp_char))->FindFirstBinAbove()<<"  ((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()="<<((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()<<endl;//provv
        if(((TH1D*)gDirectory->Get(tmp_char))->GetEntries()>20){//to evaluate the T0 I need at least 10 data... number to be optimized
          //~ cout<<"fit channel number="<<i<<endl;
          //~ f1->SetParameters(1, ((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin(), 10);
          //~ f1->SetParLimits(1,0,100);
          //~ ((TH1D*)gDirectory->Get(tmp_char))->Fit(f1,"QR+","",((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()-100,((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()+100);
        
          //old WRONG method:take the first signal, not too distant from other signals
          //~ tdc_peak=((TH1D*)gDirectory->Get(tmp_char))->FindFirstBinAbove();
          //~ for(Int_t j=((TH1D*)gDirectory->Get(tmp_char))->FindFirstBinAbove()+1;j<((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin();j++)
            //~ if(((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(j)>0)
              //~ if(j-tdc_peak<50){
                //~ j=((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()+10;
              //~ }else
                //~ tdc_peak=j;
                

          //old WRONG method: here I'm considering the begining of the tdc rise (plus: it works only if I have enough data)
          tdc_peak=((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin();
          if(bmcon->GetT0switch()==0){
            for(Int_t j=((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin();j>0;j--)
              if(((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(j)>((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin())/10.)
                tdc_peak=j;
          }else if(bmcon->GetT0switch()==1){ //I take the first peak as the T0
            for(Int_t j=((TH1D*)gDirectory->Get(tmp_char))->FindFirstBinAbove();j<=((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin();j++)
              if((((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(j) < ((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(j-1)) && (((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(j-1) > ((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin())/2.)){
                tdc_peak=j-1;      
                break;      
              }              
          }   
           
          //~ cout<<"tdc_peak="<<tdc_peak<<"  i="<<i<<"  bmcon->GetT0switch="<<bmcon->GetT0switch()<<endl;            
          bmcon->SetT0(bmmap->tdc2cell(i),(Double_t)((TH1D*)gDirectory->Get(tmp_char))->GetBinCenter(tdc_peak)); 
        }
        else{
          cout<<"WARNING IN BmBooter::EvaluateT0! too few events to evaluate T0 in tdc_cha=i="<<i<<"  cellid="<<bmmap->tdc2cell(i)<<"  Number of events="<<((TH1D*)gDirectory->Get(tmp_char))->GetEntries()<<"  T0 for this channel will wrongly set to -20000"<<endl;
          bmcon->SetT0(bmmap->tdc2cell(i),-20000.);
        }
      }  
    }
    for(Int_t i=0;i<36;i++)
      if(bmcon->GetT0(i)==-10000)
        cout<<"WARNING IN BmBooter::EvaluateT0! channel not considered in tdc map tdc_cha=i="<<i<<"  cellid="<<bmmap->tdc2cell(i)<<" T0 for this channel will set to -10000"<<endl;
  }

  
  if(bmcon->GetBMdebug()>3)
    cout<<"BMbooter::evaluatet0:: evaluate adc pedestals"<<endl;
  
  //Evaluate ADC pedestals  
  TF1 *fb = new TF1("fb","gaus", -1000,5000);
  if(bmcon->GetmanageADCped()==0 &&  bmmap->GetAdc792Ch()>0){
    bmcon->SetADCchanum(bmmap->GetAdc792Ch());
    //~ fb->SetParLimits(1,10,150);//parametri trovati in Analizer
    //~ fb->SetParLimits(2,1,10);
    for(Int_t i=0;i<bmmap->GetAdc792Ch();i++){
      sprintf(tmp_char,"ADC/ADC_meas/adc_meas_cha_%d",i);  
      ((TH1D*)gDirectory->Get(tmp_char))->Fit("fb","Q+","", 0., adc_maxbin);
      //~ if(bmcon->GetBMdebug()>11)
        //~ cout<<"ADC pedestals: cha="<<i<<"  mean="<<fb->GetParameter(1)<<"  rms="<<fb->GetParameter(2)<<endl;
      bmcon->SetADCped(i, fb->GetParameter(1), fb->GetParameter(2));
      ((TH1D*)gDirectory->Get("ADC/adc_pedmeas"))->SetBinContent(i+1,fb->GetParameter(1));
      ((TH1D*)gDirectory->Get("ADC/adc_pedrms"))->SetBinContent(i+1,fb->GetParameter(2));
    }
  }else if (bmcon->GetmanageADCped()!=0 &&  bmmap->GetAdc792Ch()>0)
    for(Int_t i=0;i<bmmap->GetAdc792Ch();i++){
      ((TH1D*)gDirectory->Get("ADC/adc_pedmeas"))->SetBinContent(i+1,bmcon->GetADCped(i));
      ((TH1D*)gDirectory->Get("ADC/adc_pedrms"))->SetBinContent(i+1,bmcon->GetADCrms(i));  
    }
   
  delete fb;
  
  f_out->Write();
  f_out->Close();    

  return;
}

Bool_t BmBooter::read_event(Bool_t evt0) {
  //~ TABMparCon* bmcon = (TABMparCon*) myp_bmcon->Object();
  //~ TABMparMap* o_bmmap = (TABMparMap*) myp_bmmap->Object();
  
  Int_t tmp_int; //dummy int variable to read and convert the input file
  //~ vector<int> ev_words;//words of the event
  Bool_t new_event, read_meas;
  Int_t sync_evnum=0, windex, endindex;
  Int_t tdc_wnum=0, adc_wnum=0, sca_wnum=0;
  
  clear_bmstruct(kFALSE);
  
  if(bmcon->GetBMdebug()>11 && (!(evt0 && bmcon->GetBMdebug()==99)))
    cout<<"I'm in BmBooter:read_event"<<endl;
  if(datafile.read((char *) &tmp_int,sizeof(int))){//read number of words of this event
    bmstruct.words=tmp_int;
    }
  else{
    if(bmcon->GetBMdebug()>11 && (!(evt0 && bmcon->GetBMdebug()==99)))
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
  bmstruct.evnum=ev_words[0];
  if(data_num_ev<0){
    acq_start_ev=bmstruct.evnum;
    data_num_ev=bmstruct.evnum;
  } else if(data_num_ev!=bmstruct.evnum){
    cout<<"ERROR in BmBooter:read_event: data_num_ev="<<data_num_ev<<"  bmstruct.evnum="<<bmstruct.evnum<<endl; 
    bmstruct.tot_status=-1;
  } 
  if(ev_words[1]!=bmmap->GetBoardNum()){
    cout<<"ERROR in BmBooter:read_event: board num != "<<bmmap->GetBoardNum()<<"  ev_words[1]="<<ev_words[1]<<endl;
    bmstruct.tot_status=1;
  }

  tdc_wnum=ev_words[2];
  if(tdc_wnum<=0)
    bmstruct.tdc_status=3;
  windex=2;
  if(bmmap->GetAdc792Ch()>=0){
    windex++;
    adc_wnum=ev_words[windex];
    if(adc_wnum!=bmmap->GetAdc792Ch()+2 && adc_wnum!=0){
      cout<<"ERROR in BmBooter:read_event: adc_wnum="<<adc_wnum<<"  bmmap->GetAdc792Ch()+2="<<bmmap->GetAdc792Ch()+2<<endl;
      bmstruct.adc_status=1;
    }
  }
  if(bmmap->GetSca830Ch()>=0){
    windex++;
    sca_wnum=ev_words[windex];
    if(sca_wnum!=bmmap->GetSca830Ch() && sca_wnum!=0){
      cout<<"ERROR in BmBooter:read_event: sca_wnum="<<sca_wnum<<"  bmmap->GetSca830Ch()="<<bmmap->GetSca830Ch()<<endl;
      bmstruct.sca_status=1;
    }
  }
  
  if(ev_words[++windex]!=3){//provv, da modificare: da togliere nell'acquisizione e qua...
    cout<<"ERROR in BmBooter:read_event: ev_words["<<windex<<"]="<<ev_words[windex]<<" ,!=3"<<endl;
    bmstruct.tot_status=3;          
  }
  
  if((windex+tdc_wnum+adc_wnum+sca_wnum+4) != bmstruct.words){
    cout<<"ERROR in BmBooter:read_event: counter for the event is wrong:  windex="<<windex<<"  tdc_wnum="<<tdc_wnum<<"  adc_wnum="<<adc_wnum<<"  sca_wnum="<<sca_wnum<<"  total number of words="<<bmstruct.words<<"  windex+tdc_wnum+adc_wnum+sca_wnum+4="<<windex+tdc_wnum+adc_wnum+sca_wnum+4<<endl;
    bmstruct.tot_status=4;
  }
  
  //~ if((bmstruct.tot_status!=0 && bmcon->GetBMdebug()>0) || bmcon->GetBMdebug()>11)
    //~ for(Int_t i=0;i<bmstruct.words;i++)
      //~ cout<<"data_num_ev="<<data_num_ev<<"   ev_words["<<i<<"]="<<ev_words[i]<<endl;
  
  //board reading
  windex++;

  if(bmstruct.tot_status==0 && tdc_wnum>0 && tdc_wnum>0) {//read the tdc words
    endindex=windex+tdc_wnum;//endindex-1 is the position of the last tdc word
    new_event=true;
    read_meas=false;
    for(;windex<endindex;windex++){
      if(new_event && (bmstruct.tdc_status==0 || bmstruct.tdc_status==-1000)){//global header found
        bmstruct.tdcev++;
        bmstruct.tdc_evnum[bmstruct.tdcev-1]=ev_words[windex++];
        read_meas=true;
        if(bmcon->GetBMdebug()>11 && (!(evt0 && bmcon->GetBMdebug()==99)))
          cout<<"global header found, windex="<<windex<<"  tdcev="<<bmstruct.tdcev<<endl;
        }
      //~ if(read_meas && ev_words[windex]<0 && isroma==kFALSE && (bmstruct.tdc_status==0 || bmstruct.tdc_status==-1000)){//global trailer found //se uso acquisizione mio (yun)
        //~ read_meas=false;
        //~ new_event=true;
        //~ bmstruct.tdc_status=ev_words[windex];
        //~ if(ev_words[windex]!=-1000){
          //~ cout<<"Warning in BmBooter: global trailer found with error in tdc_evnum="<<bmstruct.tdc_evnum[bmstruct.tdcev-1]<<"  trailer="<<ev_words[windex]<<endl;
          //~ new_event=false;
        //~ }
        //~ if(bmcon->GetBMdebug()>11 && (!(evt0 && bmcon->GetBMdebug()==99)))
          //~ cout<<"global trailer found, windex="<<windex<<"  ev_words="<<ev_words[windex]<<endl;
      //~ }
      //~ //old trento software...i wanna get rid of this!!!
      if(read_meas && ev_words[windex]==0 && isroma==kFALSE && (bmstruct.tdc_status==0 || bmstruct.tdc_status==-1000)){//global trailer found //se uso acquisizione trento con 0 invece che -1000
        read_meas=false;
        new_event=true;
        bmstruct.tdc_status=-1000;
        if(ev_words[windex]!=0){
          cout<<"Warning in BmBooter: global trailer found with error in tdc_evnum="<<bmstruct.tdc_evnum[bmstruct.tdcev-1]<<"  trailer="<<ev_words[windex]<<endl;
          new_event=false;
        }
        if(bmcon->GetBMdebug()>11 && (!(evt0 && bmcon->GetBMdebug()==99)))
          cout<<"global trailer found, windex="<<windex<<"  ev_words="<<ev_words[windex]<<endl;
      }      
              
      if(read_meas && ev_words[windex]==0 && isroma==kTRUE && (bmstruct.tdc_status==0 || bmstruct.tdc_status==-1000)){//global trailer found //se uso dati letti a Roma per BM refurbishment
        read_meas=false;
        new_event=true;
        bmstruct.tdc_status=-1000;
        if(bmcon->GetBMdebug()>11 && (!(evt0 && bmcon->GetBMdebug()==99)))
          cout<<"global trailer found, i="<<windex<<"  ev_words="<<ev_words[windex]<<endl;
      }        
      if(read_meas && (bmstruct.tdc_status==0 || bmstruct.tdc_status==-1000)){//read measure  
        if(ev_words[windex++]!=bmstruct.tdc_evnum[bmstruct.tdcev-1]){
          cout<<"ERROR in BmBooter:read_event: tdc_evnum="<<bmstruct.tdc_evnum[bmstruct.tdcev-1]<<"  measured event number="<<ev_words[windex-1]<<"  windex="<<windex<<"  data_num_ev="<<data_num_ev<<endl;
          bmstruct.tdc_status=1;
        }
        if(ev_words[windex]>-1 && ev_words[windex]<bmmap->GetTdcMaxcha()){//measure found
          if(ev_words[windex]==bmmap->GetTrefCh()){
            bmstruct.tdc_sync[sync_evnum]=ev_words[++windex];
            sync_evnum++;
          }else{      
            bmstruct.tdc_id[bmstruct.tdc_hitnum[bmstruct.tdcev-1]]=ev_words[windex++];
            bmstruct.tdc_meas[bmstruct.tdc_hitnum[bmstruct.tdcev-1]]=ev_words[windex];
            bmstruct.tdc_hitnum[bmstruct.tdcev-1]++;
          }
        }else{
          cout<<"ERROR in BmBooter:read_event: tdc_channel out of range!!! tdc_channel="<<ev_words[windex]<<endl;
          bmstruct.tdc_status=2;
        }
        new_event=false;
        if(bmcon->GetBMdebug()>11 && ev_words[windex-1]!=bmmap->GetTrefCh() && (!(evt0 && bmcon->GetBMdebug()==99)))
          cout<<"BMbooter::measure found: tdc_evnum="<<bmstruct.tdc_evnum[bmstruct.tdcev-1]<<" tdc_id="<<bmstruct.tdc_id[bmstruct.tdc_hitnum[bmstruct.tdcev-1]-1]<<"  corresponding bm channel="<<bmmap->tdc2cell(bmstruct.tdc_id[bmstruct.tdc_hitnum[bmstruct.tdcev-1]-1])<<" hit_meas="<<bmstruct.tdc_meas[bmstruct.tdc_hitnum[bmstruct.tdcev-1]-1]<<endl;
        else if(bmcon->GetBMdebug()>11 && ev_words[windex-1]==bmmap->GetTrefCh() && (!(evt0 && bmcon->GetBMdebug()==99)))
          cout<<"BMbooter::trigger found: sync registered="<<sync_evnum<<"  time="<<bmstruct.tdc_sync[sync_evnum-1]<<endl;
      }
    }//end of reading tdc words for loop
  }//end of tdc reading
  
  vector<Int_t> adc792_words;
  if(adc_wnum>0 && bmstruct.tot_status==0 && bmstruct.adc_status==0){//adc reading
    endindex=windex+adc_wnum;
    for(;windex<endindex;windex++)
      adc792_words.push_back(ev_words[windex]);
    monitorQDC(adc792_words);
  }

  if(sca_wnum>0 && bmstruct.tot_status==0 && bmstruct.sca_status==0){//scaler reading
    endindex=windex+sca_wnum;
    tmp_int=0;
    for(;windex<endindex;windex++){
      bmstruct.sca830_meas[tmp_int]=ev_words[windex]-bmstruct.sca830_counts[tmp_int];
      bmstruct.sca830_counts[tmp_int++]=ev_words[windex];
    }
  }
  
  bmstruct.time_evtoev=ev_words[windex++];
  bmstruct.time_read=ev_words[windex++];
  bmstruct.time_acq=ev_words[windex++];
  
  if(windex!=bmstruct.words){
    cout<<"ERROR in BmBooter:read_event: there are missing words: read word="<<windex<<"  bmstruct.words="<<bmstruct.words<<""<<endl;
    cout<<"windex="<<windex<<"  tdc_wnum="<<tdc_wnum<<"  adc_wnum="<<adc_wnum<<"  sca_wnum="<<sca_wnum<<"  total number of words="<<bmstruct.words<<endl;
    bmstruct.tot_status=4;
  }
  
  if((((bmstruct.tdc_status!=-1000 || bmstruct.tot_status!=0 || bmstruct.adc_status!=0 || bmstruct.sca_status!=0) && bmcon->GetBMdebug()>2) || bmcon->GetBMdebug()>11) && (!(evt0 && bmcon->GetBMdebug()==99))){
    cout<<"BMbooter::read_event::bmstruct.tdc_status="<<bmstruct.tdc_status<<" bmstruct.tot_status="<<bmstruct.tot_status<<" bmstruct.adc_status="<<bmstruct.adc_status<<" bmstruct.sca_status="<<bmstruct.sca_status<<endl;
    if(bmstruct.tdc_status!=-1000 || bmstruct.tot_status!=0 || bmstruct.adc_status!=0 || bmstruct.sca_status!=0)
      cout<<"Error detected previously; ";
    cout<<"The whole event read is:    data_num_ev="<<data_num_ev<<endl;
    for(Int_t i=0;i<bmstruct.words;i++)
      cout<<"ev_words["<<i<<"]="<<ev_words[i]<<endl;
  }
  
  if(bmcon->GetBMdebug()>12 && (!(evt0 && bmcon->GetBMdebug()==99)))
    PrintBMstruct();
  
  //provv:
  //~ if(bmstruct.tdc_hitnum[0]>10){
    //~ cout<<"TROVATO EVENTO CON "<<bmstruct.tdc_hitnum[0]<<" eventi!!"<<endl;
    //~ for(Int_t i=0;i<bmstruct.words;i++)
      //~ cout<<"ev_words["<<i<<"]="<<ev_words[i]<<endl;   
    //~ PrintBMstruct(); 
  //~ }
  
  //~ else{//read tdc words if    
    //~ cout<<"data_num_ev="<<data_num_ev<<endl;
    //~ for(Int_t i=0;i<bmstruct.words;i++)
        //~ cout<<"ev_words["<<i<<"]="<<ev_words[i]<<endl;
  //~ }
  return kTRUE;
}

//provv per qdc/adc
void BmBooter::monitorQDC(vector<Int_t>& adc792_words) {
  
  Int_t qdc_cnt(0);
  Int_t data, dt_type, evtnum, chan;

  Bool_t bltread = kTRUE;
  //~ Double_t mulf = 400./4095;  //not used...
  
  //12 bit scale --> 400 pc

  for(Int_t iac=0; iac<bmmap->GetAdc792Ch()+2; iac++) {
    
    qdc_cnt = 0;
    data = adc792_words[iac];
    if(bltread) {
      dt_type = data>>24 & 0x7;
      if(bmcon->GetBMdebug()>3)      
        cout<<"BMbooter::monitorQDC:: dt_type="<<dt_type<<endl;
      if(!(dt_type & 0x7)) {
        qdc_cnt = data & 0xFFF;
        chan = data>>17 & 0xF;
        if(bmcon->GetBMdebug()>11) 
          cout<<"BMbooter::monitorQDC:: qdc_cnt="<<qdc_cnt<<"   chan="<<chan<<" "<<"  adc792_words["<<iac<<"]="<<adc792_words[iac]<<endl;
        if(data>>12 & 0x1) {
          if(bmcon->GetBMdebug()>3) 
            cout<<"BMbooter::monitorQDC:: Overflow, my dear !!  chan="<<chan<<" qdc_cnt="<<qdc_cnt<<endl;
          bmstruct.adc792_over[chan]=1;
          bmstruct.adc_status=2;
        }
        else
          bmstruct.adc792_over[chan]=0;//non ne sono sicuro...
      } else if(dt_type & 0x4) {
        //EOB
        evtnum = data & 0xFFFFFF;
        if(bmcon->GetBMdebug()>1) 
          cout<<"EvtNum "<<evtnum<<endl;
      } else if(dt_type & 0x2) {
        //Header
        if(bmcon->GetBMdebug()>11) 
          cout<<"BMbooter::monitorQDC:: Header found. "<<endl;
      }
    } else {//end of bltread==true
      dt_type = 0;
      qdc_cnt = data;
      chan = iac;
    }
    if(!dt_type) {
      if(bmcon->GetBMdebug()>5) 
        cout<<"BMbooter::monitorQDC::chan="<<chan<<" meas="<<qdc_cnt<<endl;
      if(chan>=0 && chan<bmmap->GetAdc792Ch()){
        bmstruct.adc792_meas[chan] = qdc_cnt;
      }
    } 
   
  }
  
  for(Int_t i=0;i<bmmap->GetAdc792Ch();i++)
    if(bmstruct.adc792_over[i]==-10000)
      bmstruct.adc_status=3;
      
  return;
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
    //~ data_num_ev++;
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
  
  if(bmcon->GetBMdebug()>12){
    cout<<"BmBooter::evaluate_cell_occupy: print cell_occupy"<<endl;
    for(Int_t i=0;i<36;i++){
      if(cell_occupy[i].size()>0){
        cout<<"cell index="<<i<<"  hits=";
        for(Int_t j=0;j<cell_occupy[i].size();j++)
          cout<<cell_occupy[i][j]<<" ";
        cout<<endl;
      }
    }
  }

  return;
}


void BmBooter::efficiency_pivot_probe() {  
  Int_t pivota=0;
  for(Int_t i=0;i<23;i++){
    if(i==2 || i==14)
      i+=2;
    else if(i==6 || i==11 || i==18)
      i++;  
    if(cell_occupy[i].size()>0 && cell_occupy[i+12].size()>0){
      eff_pp[0][pivota]++;
      if(cell_occupy[i+6].size()>0 || cell_occupy[i+7].size()>0)
        eff_pp[1][pivota]++;
    }
    pivota++;
  }

  if(bmcon->GetBMdebug()>12){
    cout<<"BmBooter::efficiency_pivot_probe: print eff_pp"<<endl<<"eff_pp[0]=";
    for(Int_t i=0;i<eff_pp[0].size();i++)
      cout<<" "<<eff_pp[0][i];
    cout<<endl<<"eff_pp[1]=";
    for(Int_t i=0;i<eff_pp[1].size();i++)
      cout<<" "<<eff_pp[1][i];
    cout<<endl;
  }
return;
}

void BmBooter::efficiency_fittedplane(){

  //~ vector<Int_t> hit_plane(12,0);
  vector<Int_t> hit_fittedplane(12,0);
  
  for (Int_t i = 0; i < bmnturaw->nhit; i++) { 
    bmntuhit = bmnturaw->Hit(i); 
    if(bmntuhit->GetIsSelected() && bmntutrack->trk_status==0){
      if(bmntuhit->View()==1)
        hit_fittedplane[bmntuhit->Plane()]++;
      else
        hit_fittedplane[bmntuhit->Plane()+6]++;
    }  
  }  
  
  //view==1
  if(hit_fittedplane[0]>0 && hit_fittedplane[2]>0 && hit_fittedplane[4]>0){
    eff_fittedplane[0][0]++;
    if(hit_fittedplane[1]>0 && hit_fittedplane[3]>0)
      eff_fittedplane[0][1]++;
  }
  if(hit_fittedplane[1]>0 && hit_fittedplane[3]>0 && hit_fittedplane[5]>0){
    eff_fittedplane[1][0]++;
    if(hit_fittedplane[2]>0 && hit_fittedplane[4]>0)
      eff_fittedplane[1][1]++;
  }
  //view==-1
  if(hit_fittedplane[6]>0 && hit_fittedplane[8]>0 && hit_fittedplane[10]>0){
    eff_fittedplane[2][0]++;
    if(hit_fittedplane[7]>0 && hit_fittedplane[9]>0)
      eff_fittedplane[2][1]++;
  }
  if(hit_fittedplane[7]>0 && hit_fittedplane[9]>0 && hit_fittedplane[11]>0){
    eff_fittedplane[3][0]++;
    if(hit_fittedplane[8]>0 && hit_fittedplane[10]>0)
      eff_fittedplane[3][1]++;
  }  

return;
}

void BmBooter::efficiency_paoloni(){
  //~ for (Int_t i = 0; i < bmnturaw->nhit; i++) { 
    //~ bmntuhit = bmnturaw->Hit(i); 
    //~ if(bmntuhit->View()==1)
      //~ hit_plane[bmntuhit->Plane()]++;
    //~ else
      //~ hit_plane[bmntuhit->Plane()+6]++;
  //~ }
  
  //~ //eff_plane calculation
  //~ //view==1
  //~ if(hit_plane[0]>0 && hit_plane[2]>0 && hit_plane[4]>0){
    //~ eff_plane[0][0]++;
    //~ if(hit_plane[1]>0 && hit_plane[3]>0)
      //~ eff_plane[0][1]++;
  //~ }
  //~ if(hit_plane[1]>0 && hit_plane[3]>0 && hit_plane[5]>0){
    //~ eff_plane[1][0]++;
    //~ if(hit_plane[2]>0 && hit_plane[4]>0)
      //~ eff_plane[1][1]++;
  //~ }
  //~ //view==-1
  //~ if(hit_plane[6]>0 && hit_plane[8]>0 && hit_plane[10]>0){
    //~ eff_plane[2][0]++;
    //~ if(hit_plane[7]>0 && hit_plane[9]>0)
      //~ eff_plane[2][1]++;
  //~ }
  //~ if(hit_plane[7]>0 && hit_plane[9]>0 && hit_plane[11]>0){
    //~ eff_plane[3][0]++;
    //~ if(hit_plane[8]>0 && hit_plane[10]>0)
      //~ eff_plane[3][1]++;
  //~ }

  //xview
  if(cell_occupy[0].size()>0 && cell_occupy[12].size()>0 && cell_occupy[24].size()>0){
    eff_plane[0][0]++;
    if((cell_occupy[7].size()>0 || cell_occupy[6].size()>0) && (cell_occupy[19].size()>0 || cell_occupy[18].size()>0))
      eff_plane[0][1]++;
  }
  if(cell_occupy[1].size()>0 && cell_occupy[13].size()>0 && cell_occupy[25].size()>0){
    eff_plane[1][0]++;
    if((cell_occupy[8].size()>0 || cell_occupy[7].size()>0) && (cell_occupy[19].size()>0 || cell_occupy[20].size()>0))
      eff_plane[1][1]++;
  }
  if(cell_occupy[7].size()>0 && cell_occupy[19].size()>0 && cell_occupy[31].size()>0){
    eff_plane[2][0]++;
    if((cell_occupy[12].size()>0 || cell_occupy[13].size()>0) && (cell_occupy[24].size()>0 || cell_occupy[25].size()>0))
      eff_plane[2][1]++;
  }
  if(cell_occupy[8].size()>0 && cell_occupy[20].size()>0 && cell_occupy[32].size()>0){
    eff_plane[3][0]++;
    if((cell_occupy[13].size()>0 || cell_occupy[14].size()>0) && (cell_occupy[25].size()>0 || cell_occupy[26].size()>0))
      eff_plane[3][1]++;
  }
  
  //yview
  if(cell_occupy[4].size()>0 && cell_occupy[16].size()>0 && cell_occupy[28].size()>0){
    eff_plane[4][0]++;
    if((cell_occupy[10].size()>0 || cell_occupy[19].size()>0) && (cell_occupy[22].size()>0 || cell_occupy[21].size()>0))
      eff_plane[4][1]++;
  }
  if(cell_occupy[5].size()>0 && cell_occupy[17].size()>0 && cell_occupy[29].size()>0){
    eff_plane[5][0]++;
    if((cell_occupy[10].size()>0 || cell_occupy[11].size()>0) && (cell_occupy[22].size()>0 || cell_occupy[23].size()>0))
      eff_plane[5][1]++;
  }
  if(cell_occupy[9].size()>0 && cell_occupy[21].size()>0 && cell_occupy[33].size()>0){
    eff_plane[6][0]++;
    if((cell_occupy[15].size()>0 || cell_occupy[16].size()>0) && (cell_occupy[27].size()>0 || cell_occupy[28].size()>0))
      eff_plane[6][1]++;
  }
  if(cell_occupy[10].size()>0 && cell_occupy[22].size()>0 && cell_occupy[34].size()>0){
    eff_plane[7][0]++;
    if((cell_occupy[16].size()>0 || cell_occupy[17].size()>0) && (cell_occupy[28].size()>0 || cell_occupy[29].size()>0))
      eff_plane[7][1]++;
  }
  
return;
}


//to be used in process to charge mylarproject
void BmBooter::Projecttracktr(){
  
  vector<Double_t> tracktr_pro(6);
  for (Int_t i = 0; i < bmntutrack->ntrk; i++) {
    bmntutracktr = bmntutrack->Track(i);  
    tracktr_pro[0]=bmntutracktr->GetMylar1Pos().X();
    tracktr_pro[1]=bmntutracktr->GetMylar1Pos().Y();
    tracktr_pro[2]=bmntutracktr->GetMylar2Pos().X();
    tracktr_pro[3]=bmntutracktr->GetMylar2Pos().Y();
    tracktr_pro[4]=bmntutracktr->GetR0().X();
    tracktr_pro[5]=bmntutracktr->GetR0().Y();
  }
  tracktr2dprojects.push_back(tracktr_pro);
  
return;
}
  
//used in process to charge residual_distance
void BmBooter::ResidualDistance(){
  vector<Double_t> selecthit(4);
  vector<Double_t> rejhit(2);
  
  for(Int_t i=0;i<bmnturaw->nhit;i++){
    bmntuhit = bmnturaw->Hit(i);
    if(bmntuhit->GetIsSelected()){
      selecthit[0]=bmgeo->GetBMNcell(bmntuhit->Plane(), bmntuhit->View(),bmntuhit->Cell());
      selecthit[1]=bmntuhit->Dist();
      if(isdata)
        selecthit[2]=bmntuhit->Tdrift();
      selecthit[3]=bmntuhit->GetResidual();
      residual_distance.push_back(selecthit);
    }else if(isdata){
      rejhit[0]=bmgeo->GetBMNcell(bmntuhit->Plane(), bmntuhit->View(),bmntuhit->Cell());
      rejhit[1]=bmntuhit->Tdrift();
      residual_distance.push_back(rejhit);
    }          
  }
  
return;  
}

void BmBooter::PrintBMstruct(){
  cout<<"PrintBMstruct:"<<endl;
  cout<<"bmstruct.evnum="<<bmstruct.evnum<<endl;
  cout<<"bmstruct.words="<<bmstruct.words<<endl;
  cout<<"bmstruct.tdcev="<<bmstruct.tdcev<<endl;
  cout<<"bmstruct.tot_status="<<bmstruct.tot_status<<endl;
  cout<<"bmstruct.adc_status="<<bmstruct.adc_status<<endl;
  cout<<"bmstruct.sca_status="<<bmstruct.sca_status<<endl;
  cout<<"bmstruct.tdc_status="<<bmstruct.tdc_status<<endl;
  Int_t tmp_int=0;
  while(bmstruct.tdc_sync[tmp_int]!=-10000){
    cout<<"i="<<tmp_int<<"  bmstruct.tdc_sync[i]="<<bmstruct.tdc_sync[tmp_int]<<endl;;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.tdc_id[tmp_int]!=-10000){
    cout<<"i="<<tmp_int<<"  bmstruct.tdc_id[i]="<<bmstruct.tdc_id[tmp_int]<<endl;;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.tdc_meas[tmp_int]!=-10000){
    cout<<"i="<<tmp_int<<"  bmstruct.tdc_meas[i]="<<bmstruct.tdc_meas[tmp_int]<<endl;;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.tdc_evnum[tmp_int]!=-10000){
    cout<<"i="<<tmp_int<<"  bmstruct.tdc_evnum[i]="<<bmstruct.tdc_evnum[tmp_int]<<endl;;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.tdc_hitnum[tmp_int]!=0){
    cout<<"i="<<tmp_int<<"  bmstruct.tdc_hitnum[i]="<<bmstruct.tdc_hitnum[tmp_int]<<endl;;
    tmp_int++;
  }
  tmp_int=0;
  tmp_int=0;
  while(bmstruct.adc792_meas[tmp_int]!=-10000){
    cout<<"i="<<tmp_int<<"  bmstruct.adc792_meas[i]="<<bmstruct.adc792_meas[tmp_int]<<endl;;
    tmp_int++;
  }
  while(bmstruct.adc792_over[tmp_int]!=-10000){
    cout<<"i="<<tmp_int<<"  bmstruct.adc792_over[i]="<<bmstruct.adc792_over[tmp_int]<<endl;;
    tmp_int++;
  }
  while(bmstruct.sca830_meas[tmp_int]!=-10000){
    cout<<"i="<<tmp_int<<"  bmstruct.sca830_meas[i]="<<bmstruct.sca830_meas[tmp_int]<<endl;;
    tmp_int++;
  }
  cout<<"bmstruct.time_evtoev="<<bmstruct.time_evtoev<<endl;
  cout<<"bmstruct.time_read="<<bmstruct.time_read<<endl;
  cout<<"bmstruct.time_acq="<<bmstruct.time_acq<<endl;

  cout<<endl;
  
}

void BmBooter::clear_bmstruct(Bool_t forced){
  bmstruct.evnum=0;
  bmstruct.words=0;
  bmstruct.tdcev=0;
  bmstruct.tot_status=0;
  bmstruct.tdc_status=0;
  bmstruct.adc_status=0;
  bmstruct.sca_status=0;
  bmstruct.time_evtoev=0;
  bmstruct.time_read=0;
  bmstruct.time_acq=0;
  if(forced){
    for(Int_t i=0;i<MAXHITTDC;i++){
     bmstruct.tdc_id[i]=-10000;
     bmstruct.tdc_meas[i]=-10000;
     bmstruct.tdc_sync[i]=-10000;
    }
    for(Int_t i=0;i<MAXEVTDC;i++){
     bmstruct.tdc_hitnum[i]=0;
     bmstruct.tdc_evnum[i]=-10000;
    }
    for(Int_t i=0;i<SCA830MAX;i++){
      bmstruct.sca830_meas[i]=-10000;
      bmstruct.sca830_counts[i]=0;
    }
    for(Int_t i=0;i<ADC792MAX;i++){
      bmstruct.adc792_meas[i]=-10000;
      bmstruct.adc792_over[i]=-10000;
    }
    return;
  }
  Int_t tmp_int=0;
  while(bmstruct.tdc_sync[tmp_int]!=-10000){
    bmstruct.tdc_sync[tmp_int]=-10000;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.tdc_id[tmp_int]!=-10000){
    bmstruct.tdc_id[tmp_int]=-10000;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.tdc_meas[tmp_int]!=-10000){
    bmstruct.tdc_meas[tmp_int]=-10000;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.tdc_evnum[tmp_int]!=-10000){
    bmstruct.tdc_evnum[tmp_int]=-10000;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.tdc_hitnum[tmp_int]!=0){
    bmstruct.tdc_hitnum[tmp_int]=0;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.sca830_meas[tmp_int]!=-10000){
    bmstruct.sca830_meas[tmp_int]=-10000;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.adc792_meas[tmp_int]!=-10000){
    bmstruct.adc792_meas[tmp_int]=-10000;
    tmp_int++;
  }
  tmp_int=0;
  while(bmstruct.adc792_over[tmp_int]!=-10000){
    bmstruct.adc792_over[tmp_int]=-10000;
    tmp_int++;
  }
return;
}


void BmBooter::FillDataBeamMonitor() {
  if (GlobalPar::GetPar()->Debug()>10)
    cout<<"I'm in BmBooter::FillDataBeamMonitor"<<endl;  
  
  myn_stdatraw    = new TAGdataDsc("myn_stdatraw", new TAIRdatRaw());
  new TAIRactDatRaw("an_stdatraw", myn_stdatraw, myp_bmmap, &bmstruct);

  myn_bmdatraw    = new TAGdataDsc("myn_bmdatraw", new TABMdatRaw());
  new TABMactDatRaw("an_bmdatraw",myn_bmdatraw, myp_bmmap, myp_bmcon, myp_bmgeo, myn_stdatraw, &bmstruct); 
   
  myn_bmraw    = new TAGdataDsc("myn_bmraw", new TABMntuRaw());
  new TABMactNtuRaw("an_bmraw", myn_bmraw, myn_bmdatraw, myn_stdatraw, myp_bmgeo, myp_bmcon); 
  
  myn_bmtrk    = new TAGdataDsc("myn_bmtrk", new TABMntuTrack());  
  new TABMactNtuTrack("an_bmtrk", myn_bmtrk, myn_bmraw, myp_bmgeo, myp_bmcon);
    
  if (GlobalPar::GetPar()->Debug()>10)
    cout<<"I finish BmBooter::FillDataBeamMonitor"<<endl;  
  return;
}


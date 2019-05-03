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
Bool_t BmBooter::Initialize( TString instr_in, Bool_t isdata_in, EVENT_STRUCT* evStr_in ) {  
  txt_outputname="gsi_out.txt";
  isdata=isdata_in;
  m_instr=instr_in;
  evStr=evStr_in;
  data_num_ev= (isdata==true) ? -1000:0;
  acq_start_ev=1;
  isallign=kFALSE;
  clear_bmstruct(kTRUE);
  myp_bmgeo = gTAGroot->FindParaDsc("myp_bmgeo", "TABMparGeo");  
  myp_bmcon = gTAGroot->FindParaDsc("myp_bmcon", "TABMparCon");  
  if(isdata)
    myp_bmmap = gTAGroot->FindParaDsc("myp_bmmap", "TABMparMap");  
  bmcon = (TABMparCon*) (gTAGroot->FindParaDsc("myp_bmcon", "TABMparCon")->Object()); 
  bmgeo = (TABMparGeo*) (gTAGroot->FindParaDsc("myp_bmgeo", "TABMparGeo")->Object());
  if(isdata)
    bmmap = (TABMparMap*) (gTAGroot->FindParaDsc("myp_bmmap", "TABMparMap")->Object());

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
    else if(bmcon->loadT0s(data_num_ev))
      return kTRUE;
    
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
    if(bmcon->GetAutstrel()>0)
      if(autocalibstrel_readfile())
        return kTRUE;    
  }
  
  //provo
  //~ outTree = new TAGactTreeWriter("outTree");
  //~ tagr->AddRequiredItem("myn_bmraw");
  //~ tagr->AddRequiredItem("myn_bmtrk");
  //~ tagr->AddRequiredItem("outTree");
  //~ tagr->Print();   
    
//~ if (outTree->Open("BMrecotree.root", "RECREATE")) return;
    
  tot_num_ev=data_num_ev;  
  data_num_ev= (isdata==true) ? -1000:0;
  data_sync_num_ev=0;
   
return kFALSE;
}





//----------------------------------------------------------------------------------------------------
void BmBooter::Process() {
     
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
      data_sync_num_ev+=bmstruct.tdc_numsync;
      return;
    }
    if (bmcon->GetBMdebug()>3){
      cout<<"I'm in BmBooter::Process, data_num_ev="<<data_num_ev<<"   bmstruct.evnum="<<bmstruct.evnum<<"   tdcev="<<bmstruct.tdcev<<"   tdc_numsync="<<bmstruct.tdc_numsync<<"  data_sync_num_ev="<<data_sync_num_ev<<endl;
      //~ if(bmmap->GetSca830Ch()>0)
        //~ cout<<"  sca_counts="<<bmstruct.sca830_counts[7]<<endl;
      //~ else
        //~ cout<<endl;  
    } 
  }
  
  if(bmcon->GetBMdebug()>0)
    cout<<"I'm in BmBooter::Process, event number: data_num_ev="<<data_num_ev<<endl;  
 
  bmnturaw = (TABMntuRaw*) (gTAGroot->FindDataDsc("myn_bmraw", "TABMntuRaw")->GenerateObject());
  //~ bmnturaw=(TABMntuRaw*) myn_bmraw->GenerateObject();
  evaluate_cell_occupy();
  track_ok=-5;
  
  if(bmstruct.tdc_sync[0]==-10000 && isdata)
    track_ok=-4;
  else if(bmstruct.tdc_sync[1]!=-10000 && isdata)
    track_ok=-3;
  else if(bmnturaw->nhit >= bmcon->GetMaxnhit_cut())
    track_ok=-2;
  else if(bmnturaw->nhit <= bmcon->GetMinnhit_cut())
    track_ok=-1;  
  else if(bmcon->GetFitterIndex()>0){    
    bmntutrack = (TABMntuTrack*) (gTAGroot->FindDataDsc("myn_bmtrk", "TABMntuTrack")->GenerateObject());
    track_ok=bmntutrack->trk_status;
  }
 
  if (bmcon->GetBMdebug()>10)
    cout<<"in BmBooter::Process, I finished to create the BM hits and tracks"<<endl<<"Now I'll printout BM hits if enable"<<endl;

  if (GlobalPar::GetPar()->IsPrintOutputFile())
    m_controlPlotter->BM_setnturaw_info("BM_output",bmnturaw, bmgeo, bmcon, bmmap, cell_occupy, track_ok); 
  
  //~ if (bmcon->GetBMdebug()>10)
    //~ cout<<"in BmBooter::Process, I finished to printout BM hits"<<endl;
  
  if (GlobalPar::GetPar()->IsPrintOutputFile() && track_ok>=0)
    if(m_controlPlotter->BM_setntutrack_info("BM_output", bmgeo, bmntutrack, bmnturaw, bmcon)==0)
      isallign=kTRUE;      
      
  if(!isdata){
    MC_track=MCxEvent();  
    if(GlobalPar::GetPar()->IsPrintOutputFile()){    
      m_controlPlotter->BM_setMCnturaw_info("BM_output",evStr, bmnturaw, bmgeo, bmcon);
      if(track_ok==0 && MC_track)
        m_controlPlotter->BM_setMCntutrack_info("BM_output",evStr, bmntutrack, bmgeo, mylar1realpos, mylar2realpos);
    }
  }
  
  //temporary ttree output
  if( GlobalPar::GetPar()->IsPrintOutputNtuple() && track_ok==0 ){        
      ControlPlotsRepository::GetControlObject( "BooterFinalize" )->BM_setTTree_output(bmnturaw, bmntutrack, data_num_ev,bmstruct.time_acq, isdata);
    if(!isdata)
      ControlPlotsRepository::GetControlObject( "BooterFinalize" )->BM_setMCTTree_output(mylar1realpos, mylar2realpos);
  }
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
      efficiency_fittedtracks();
    }
    
  data_num_ev++;
  data_sync_num_ev+=bmstruct.tdc_numsync;

  if (bmcon->GetBMdebug()>10)
    cout<<"I finished BmBooter::Process"<<endl;

return;
}





//----------------------------------------------------------------------------------------------------
void BmBooter::Finalize() {
  
  if (bmcon->GetBMdebug()>10)
    cout<<"I'm in BmBooter::Finalize"<<endl;

  PrintEFFpp();      //efficiency
  PrintProjections();//tracktr2dprojects  matrix
  PrintResDist();    //residual_distance matrix
  PrintSTrel();      //strel must be used after printresdist
  PrintFromControlPlots(); //other plots from contrloplots 
  //~ LegendrePoly(); //create the legendre poly plots
  //~ fit_histos();   //to be fixed!!!! 
  if(bmcon->GetAutstrel()>0)
    autocalibstrel_writefile();  
    
  if(!isdata)
    PrintMCxEvent();  
    
  if(isallign)
    Allign_estimate();
  
  if (bmcon->GetBMdebug()>10)
    cout<<"I finished BmBooter::Finalize"<<endl;
    
return;
}

void BmBooter::Allign_estimate(){
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))==nullptr)
    return;
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd();  
  
  Double_t xrot=atan((((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_y")))->GetMean()-((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_y")))->GetMean())/(bmgeo->GetMylar2().Z()-bmgeo->GetMylar1().Z()))*RAD2DEG;  
  
  Double_t yrot=-atan((((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_x")))->GetMean()-((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_x")))->GetMean())/(bmgeo->GetMylar2().Z()-bmgeo->GetMylar1().Z()))*RAD2DEG;
   
  Double_t xtra=-(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_x")))->GetMean()+((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_x")))->GetMean())/2.;  
      
  Double_t xtr_err=sqrt(pow(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_x")))->GetMean()/sqrt(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_x")))->GetEntries()),2.)  + pow(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_x")))->GetMean()/sqrt(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_x")))->GetEntries()),2.));  
    
  Double_t ytra=-(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_y")))->GetMean()+((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_y")))->GetMean())/2.;
    
  Double_t ytr_err=sqrt(pow(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_y")))->GetMean()/sqrt(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar2_y")))->GetEntries()),2.)  +  pow(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_y")))->GetMean()/sqrt(((TH1D*)(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->Get("BM_output__tracksel_mylar1_y")))->GetEntries()),2.));  
    
  if(bmcon->GetBMdebug()>0){  
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
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd();
    
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
  
  if(bmcon->GetAutstrel()>0){
    TH2D* oldstrel2d=new TH2D( "oldstrel2d", "Space time relation;time [ns];distance [cm]", numpoints, 0., bmcon->GetHitTimecut()+10.,numpoints,0.,1.);
    TH1D* newvsoldstrel=new TH1D( "newvsoldstrel", "difference between last and new strel iteration;Time [s]; distance[cm]", strelresiduals.size(), 0., bmcon->GetHitTimecut());
    TH2D* newstrel2d=new TH2D( "newstrel2d", "Space time relation;time [ns];distance [cm]", numpoints, 0., bmcon->GetHitTimecut()+10.,numpoints,0.,1.);
    TH2D* histo2newres_dis=new TH2D( "newhitres_dis", "New  Residual vs rdrift; Residual[cm]; Measured rdrift[cm]", 250, -0.3, 0.3,250,0.,1.);

    for(Int_t i=0;i<residual_distance.size();i++){
      oldstrel2d->Fill(residual_distance.at(i).at(2), residual_distance.at(i).at(3));
      tmp_int=(Int_t) (residual_distance.at(i).at(2)/bmcon->GetHitTimecut()*bmcon->GetResnbin());      
      newstrel2d->Fill(residual_distance.at(i).at(2), residual_distance.at(i).at(3)-strelresiduals.at(tmp_int).at(0));
      if(residual_distance.at(i).size()>4)
        histo2newres_dis->Fill(residual_distance.at(i).at(4), residual_distance.at(i).at(3)-strelresiduals.at(tmp_int).at(0));
    }
    for(Int_t i=0;i<strelresiduals.size();i++){
      newvsoldstrel->SetBinContent(i+1,strelresiduals.at(i).at(0));
      newvsoldstrel->SetBinError(i+1,strelresiduals.at(i).at(1));
    }
    //fit the new strel
    TProfile *prof_newstrel=newstrel2d->ProfileX();
    TF1 poly ("poly","pol5", 0, bmcon->GetHitTimecut());
    prof_newstrel->Fit("poly","Q+");
    prof_newstrel->SetLineColor(2);
    prof_newstrel->Draw();
    vector<Double_t> parin;
    for(Int_t i=0;i<6;i++)
      parin.push_back(poly.GetParameter(i));
    bmcon->AddStrelparameters(parin);
  
    //create and fill the old strel history
    char tmp_char[200];
    TF1* oldtf1strel;
    for(Int_t i=0;i<bmcon->GetStrelparSize();i++){
      sprintf(tmp_char,"old_strel_ite_%d",i);
      oldtf1strel=new TF1(tmp_char,"pol5",0.,bmcon->GetHitTimecut());  
      for(Int_t k=0;k<6;k++)
        oldtf1strel->SetParameter(k,bmcon->GetStrelPar(i,k));
      oldtf1strel->Write();
    }
  }
  

  
  
  
return;
}


void BmBooter::PrintFromControlPlots(){
  
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))==nullptr)
  return;
  
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->GetList()->Contains("BM_output__raw_occupancy")==kFALSE  )
    return;
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd();

  TH2D* histo2da=new TH2D( "cell_raw_occupancy_2d_x", "Cell occupancy for raw hits; z; x", 11, -5.5, 5.5,7, -3.5,3.5);
  TH2D* histo2db=new TH2D( "cell_raw_occupancy_2d_y", "Cell occupancy for raw hits; z; y", 11, -5.5, 5.5,7, -3.5,3.5);
  
  Int_t cell, view, plane, up;
  for(Int_t i=0;i<36;i++){
    bmgeo->GetBMNlvc(i, plane, view, cell);
    if(view==0){
      up=(plane%2==0) ? 1:0;
      histo2db->SetBinContent(plane*2+1,cell*2+up+1,((TH2D*)(m_controlPlotter->GetTFile()->Get("BM_output/BM_output__raw_occupancy")))->GetBinContent(((TH1D*)(m_controlPlotter->GetTFile()->Get("BM_output/BM_output__raw_occupancy")))->FindBin((Double_t)i)));
      histo2db->SetBinContent(plane*2+1,cell*2+up+2,((TH2D*)(m_controlPlotter->GetTFile()->Get("BM_output/BM_output__raw_occupancy")))->GetBinContent(((TH1D*)(m_controlPlotter->GetTFile()->Get("BM_output/BM_output__raw_occupancy")))->FindBin((Double_t)i)));
    }else{
      up=(plane%2==0) ? 0:1;
      histo2da->SetBinContent(plane*2+1,cell*2+up+1,((TH2D*)(m_controlPlotter->GetTFile()->Get("BM_output/BM_output__raw_occupancy")))->GetBinContent(((TH1D*)(m_controlPlotter->GetTFile()->Get("BM_output/BM_output__raw_occupancy")))->FindBin((Double_t)i)));
      histo2da->SetBinContent(plane*2+1,cell*2+up+2,((TH2D*)(m_controlPlotter->GetTFile()->Get("BM_output/BM_output__raw_occupancy")))->GetBinContent(((TH1D*)(m_controlPlotter->GetTFile()->Get("BM_output/BM_output__raw_occupancy")))->FindBin((Double_t)i)));
    }  
  }
  
return;
}


void BmBooter::PrintEFFpp(){
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))==nullptr)
    return;
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd();  
    
  //"Tommasino" method
  TH1D* histo=new TH1D( "eff_pp_pivot", "pivot counter for the pivot-probe efficiency method; Pivot-cell index; Counter", eff_pp.at(0).size(), 0., eff_pp.at(0).size());
  TH1D* histo1=new TH1D( "eff_pp_probe", "probe counter for the pivot-probe efficiency method; Pivot-cell index; Counter", eff_pp.at(1).size(), 0., eff_pp.at(1).size());
  TH1D* histo2=new TH1D( "eff_pp_eval", "efficiency for the pivot-probe method; Pivot-cell index; Counter", 16, 0., 16.);
  Double_t hist2_errors[eff_pp.at(0).size()];
  Double_t eff;
  for(int i=0;i<eff_pp.at(0).size();i++){
    histo->SetBinContent(i+1, eff_pp.at(0).at(i));
    histo1->SetBinContent(i+1, eff_pp.at(1).at(i));
    if(eff_pp.at(0).at(i)!=0){
      eff=(Double_t) (eff_pp.at(1).at(i))/eff_pp.at(0).at(i);
      histo2->SetBinContent(i+1, eff);
      hist2_errors[i]=TMath::Factorial(eff_pp.at(0).at(i)+2)/TMath::Factorial(eff_pp.at(1).at(i)+1)/TMath::Factorial(eff_pp.at(0).at(i)-eff_pp.at(1).at(i)+1)*pow(eff,eff_pp.at(1).at(i))*pow(1.-eff,eff_pp.at(0).at(i)-eff_pp.at(1).at(i));
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
    histo3->SetBinContent(i+1, eff_plane.at(i).at(0));
    histo4->SetBinContent(i+1, eff_plane.at(i).at(1));
    if(eff_plane.at(i).at(0)!=0){
      histo5->SetBinContent(i+1, (Double_t) eff_plane.at(i).at(1)/eff_plane.at(i).at(0));
      mean_planeff+=(Double_t) eff_plane.at(i).at(1)/eff_plane.at(i).at(0);
    }else
      histo5->SetBinContent(i+1, 0.);
  }
  histo6->SetBinContent(1,mean_planeff/8.);
  //~ //provv
  //~ cout<<"eff_plane_mean="<<mean_planeff/4.<<"  pivot:"<<endl;
  //~ for(Int_t i=0;i<4;i++)
    //~ cout<<"i="<<i<<"  pivot_number="<<eff_plane.at(i).at(0)<<"  probe_plane="<<eff_plane.at(i).at(1)<<"  plane_eval="<<(Double_t) eff_plane.at(i).at(1)/eff_plane.at(i).at(0)<<endl;
  

  //Plane method for fittedtracks
  TH1D* histo7=new TH1D( "eff_fittedplane_pivot", "pivot counter for the fitted plane efficiency method; index; Counter", 4, 0., 4.);
  TH1D* histo8=new TH1D( "eff_fittedplane_probe", "probe counter for the fitted plane efficiency method; index; Counter", 4, 0., 4.);
  TH1D* histo9=new TH1D( "eff_fittedplane_eval", "efficiency for the fitted paoloni plane method; index; Counter", 4, 0., 4.);
  TH1D* histo10=new TH1D( "eff_fittedplane_mean", "efficiency for the fitted paoloni plane method; ; mean efficiency", 1, 0., 1.);
  mean_planeff=0.;
  for(Int_t i=0;i<4;i++){
    histo7->SetBinContent(i+1, eff_fittedplane.at(i).at(0));
    histo8->SetBinContent(i+1, eff_fittedplane.at(i).at(1));
    if(eff_fittedplane.at(i).at(0)!=0){
      histo9->SetBinContent(i+1, (Double_t) eff_fittedplane.at(i).at(1)/eff_fittedplane.at(i).at(0));
      mean_planeff+=(Double_t) eff_fittedplane.at(i).at(1)/eff_fittedplane.at(i).at(0);
    }else
      histo9->SetBinContent(i+1, 0.);
  }
  histo10->SetBinContent(1,mean_planeff/4.);  
  
return;
}


void BmBooter::PrintProjections(){
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))==nullptr)
    return;
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd();
  TString tmp_tstring;
  TH2D* histoa=new TH2D( "mylar1_xy", "mylar1 projected tracks; x[cm]; y[cm]", 600, -3., 3.,600, -3.,3.);
  TH2D* histob=new TH2D( "mylar2_xy", "mylar2 projected tracks; x[cm]; y[cm]", 600, -3., 3.,600, -3.,3.);
  TH2D* histoc=new TH2D( "R0_xy", "R0 projected tracks; x[cm]; y[cm]", 600, -3., 3.,600, -3.,3.);
  TH2D* histod=new TH2D( "target_xy", "target projected tracks; x[cm]; y[cm]", 600, -3., 3.,600, -3.,3.);
  tmp_tstring="project_2d_a"+m_nopath_instr+";x[cm];y[cm]";
  TH2D* histoe=new TH2D( "project_2d_a", tmp_tstring, 200, -10., 10.,200, -10.,10.);
  tmp_tstring="project_2d_b"+m_nopath_instr+";x[cm];y[cm]";
  TH2D* histof=new TH2D( "project_2d_b", tmp_tstring, 200, -10., 10.,200, -10.,10.);
  tmp_tstring="project_2d_c"+m_nopath_instr+";x[cm];y[cm]";
  TH2D* histog=new TH2D( "project_2d_c", tmp_tstring, 200, -10., 10.,200, -10.,10.);
  tmp_tstring="project_x_a"+m_nopath_instr+"; x[cm]; track num";
  TH1D* histoex=new TH1D( "project_x_a", tmp_tstring, 200, -10., 10.);
  tmp_tstring="project_x_b"+m_nopath_instr+"; x[cm]; track num";
  TH1D* histofx=new TH1D( "project_x_b", tmp_tstring, 200, -10., 10.);
  tmp_tstring="project_x_c"+m_nopath_instr+"; x[cm]; track num";
  TH1D* histogx=new TH1D( "project_x_c", tmp_tstring, 200, -10., 10.);
  tmp_tstring="project_y_a"+m_nopath_instr+"; track num; y[cm]";
  TH1D* histoey=new TH1D( "project_y_a", tmp_tstring, 200, -10., 10.);
  tmp_tstring="project_y_b"+m_nopath_instr+"; track num; y[cm]";
  TH1D* histofy=new TH1D( "project_y_b", tmp_tstring, 200, -10., 10.);
  tmp_tstring="project_y_c"+m_nopath_instr+"; track num; y[cm]";
  TH1D* histogy=new TH1D( "project_y_c", tmp_tstring, 200, -10., 10.);
  //~ TH2D* histoe=new TH2D( "pvers_mx_MCvsFitted", "mx for the MC and fitted tracks; mx tracks; mx MC", 500,-0.05,0.05,500,-0.05,0.05);
  
  TVector3 projection;
  //~ Int_t mcindex=0;
  for(Int_t i=0;i<tracktr2dprojects.size();i++){
    projection=bmgeo->ProjectFromPversR0(tracktr2dprojects.at(i).at(1),tracktr2dprojects.at(i).at(3), tracktr2dprojects.at(i).at(2), tracktr2dprojects.at(i).at(4), bmgeo->GetMylar1().Z());
    histoa->Fill(projection.X(), projection.Y());
    projection=bmgeo->ProjectFromPversR0(tracktr2dprojects.at(i).at(1),tracktr2dprojects.at(i).at(3), tracktr2dprojects.at(i).at(2), tracktr2dprojects.at(i).at(4), bmgeo->GetMylar2().Z());
    histob->Fill(projection.X(), projection.Y());
    histoc->Fill(tracktr2dprojects.at(i).at(2), tracktr2dprojects.at(i).at(4));
    projection=bmgeo->ProjectFromPversR0(tracktr2dprojects.at(i).at(1),tracktr2dprojects.at(i).at(3), tracktr2dprojects.at(i).at(2), tracktr2dprojects.at(i).at(4), bmgeo->GetTarget().Z());
    histod->Fill(projection.X(), projection.Y());
    if(bmcon->GetMeas_shift().X()!=0 && bmcon->GetCalibro()<0){
      projection=bmgeo->ProjectFromPversR0(tracktr2dprojects.at(i).at(1),tracktr2dprojects.at(i).at(3), tracktr2dprojects.at(i).at(2), tracktr2dprojects.at(i).at(4), bmcon->GetMeas_shift().X());
      histoe->Fill(projection.X(), projection.Y());
      histoex->Fill(projection.X());
      histoey->Fill(projection.Y());
    }
    if(bmcon->GetMeas_shift().Y()!=0 && bmcon->GetCalibro()<0){
      projection=bmgeo->ProjectFromPversR0(tracktr2dprojects.at(i).at(1),tracktr2dprojects.at(i).at(3), tracktr2dprojects.at(i).at(2), tracktr2dprojects.at(i).at(4), bmcon->GetMeas_shift().Y());
      histof->Fill(projection.X(), projection.Y());
      histofx->Fill(projection.X());
      histofy->Fill(projection.Y());
    }
    if(bmcon->GetMeas_shift().Z()!=0 && bmcon->GetCalibro()<0){
      projection=bmgeo->ProjectFromPversR0(tracktr2dprojects.at(i).at(1),tracktr2dprojects.at(i).at(3), tracktr2dprojects.at(i).at(2), tracktr2dprojects.at(i).at(4), bmcon->GetMeas_shift().Z());
      histog->Fill(projection.X(), projection.Y());
      histogx->Fill(projection.X());
      histogy->Fill(projection.Y());
    }
  }

  //evaluate uniformity:
  //~ Double_t min_x_unif=-1., max_x_unif=1.,min_y_unif=-1., max_y_unif=1.;

  //~ ofstream outtxt;
  //~ outtxt.open(txt_outputname.Data(),ios::app);
  if(bmcon->GetBMvieproj()>0){
    TString tmp_tstring;
    if(bmcon->GetMeas_shift().X()!=0 && bmcon->GetCalibro()<0){  
      TCanvas *canvas_a = new TCanvas("canvas_a", "BM projects",20,20,800,900);
      canvas_a->cd();
      histoe->Draw();
      tmp_tstring="project_a2d_"+m_nopath_instr+"pdf";
      canvas_a->SaveAs(tmp_tstring.Data(), "pdf");
      TCanvas *canvas_ax = new TCanvas("canvas_ax", "BM projects",20,20,800,900);
      canvas_ax->cd();
      histoex->Draw();
      tmp_tstring="project_ax_"+m_nopath_instr+"pdf";
      canvas_ax->SaveAs(tmp_tstring.Data(), "pdf");
      TCanvas *canvas_ay = new TCanvas("canvas_ay", "BM projects",20,20,800,900);
      canvas_ay->cd();
      histoey->Draw();
      tmp_tstring="project_ay_"+m_nopath_instr+"pdf";
      canvas_ay->SaveAs(tmp_tstring.Data(), "pdf");
    }      
    if(bmcon->GetMeas_shift().Y()!=0 && bmcon->GetCalibro()<0){  
      TCanvas *canvas_b = new TCanvas("canvas_b", "BM projecta",20,20,800,900);
      canvas_b->cd();
      histof->Draw();
      tmp_tstring="project_b2d_"+m_nopath_instr+"pdf";
      canvas_b->SaveAs(tmp_tstring.Data(), "pdf");
      TCanvas *canvas_bx = new TCanvas("canvas_b", "BM projecta",20,20,800,900);
      canvas_bx->cd();
      histofx->Draw();
      tmp_tstring="project_bx_"+m_nopath_instr+"pdf";
      canvas_bx->SaveAs(tmp_tstring.Data(), "pdf");
      TCanvas *canvas_by = new TCanvas("canvas_b", "BM projecta",20,20,800,900);
      canvas_by->cd();
      histofy->Draw();
      tmp_tstring="project_by_"+m_nopath_instr+"pdf";
      canvas_by->SaveAs(tmp_tstring.Data(), "pdf");
    }      
    if(bmcon->GetMeas_shift().Z()!=0 && bmcon->GetCalibro()<0){  
      TCanvas *canvas_c = new TCanvas("canvas_a", "BM projects",20,20,800,900);
      canvas_c->cd();
      histog->Draw();
      tmp_tstring="project_c2d_"+m_nopath_instr+"pdf";
      canvas_c->SaveAs(tmp_tstring.Data(), "pdf");
      TCanvas *canvas_cx = new TCanvas("canvas_a", "BM projects",20,20,800,900);
      canvas_cx->cd();
      histogx->Draw();
      tmp_tstring="project_cx_"+m_nopath_instr+"pdf";
      canvas_cx->SaveAs(tmp_tstring.Data(), "pdf");
      TCanvas *canvas_cy = new TCanvas("canvas_a", "BM projects",20,20,800,900);
      canvas_cy->cd();
      histogy->Draw();
      tmp_tstring="project_cy_"+m_nopath_instr+"pdf";
      canvas_cy->SaveAs(tmp_tstring.Data(), "pdf");
    }      
  }
  
  //~ outtxt.close();  

return;
}


void BmBooter::PrintResDist(){
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))==nullptr)
    return;
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd();
  
  TH1D* histo1d;
  TH2D* histo2d;
  
  //create ResVsDist_perCell graphs
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->mkdir("ResVsDist_perCell");
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd("ResVsDist_perCell");
  char tmp_char[200];
  for(Int_t i=0;i<36;i++){
    sprintf(tmp_char,"hitres_dis_perCell_%d",i);  
    histo2d=new TH2D( tmp_char, "Residual vs rdrift; Residual[cm]; Measured rdrift[cm]", 6000, -0.3, 0.3,250,0.,1.);
  }
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output/ResVsDist_perCell")))->cd("..");
  
  //create ResxDist graphs
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->mkdir("ResxDist");
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd("ResxDist");
  for(Int_t i=0;i<bmcon->GetResnbin();i++){
    sprintf(tmp_char,"hitres_x_dist_%d",i);  
    histo1d=new TH1D( tmp_char, "Residual x dist bin; Residual [cm];", 600, -0.3, 0.3);
  }
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output/ResxDist")))->cd("..");
  
  //create ResxTime graphs
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->mkdir("ResxTime");
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd("ResxTime");
  for(Int_t i=0;i<bmcon->GetResnbin();i++){
    sprintf(tmp_char,"hitres_x_time_%d",i);  
    histo1d=new TH1D( tmp_char, "Residual x time bin; Residual [cm];", 600, -0.3, 0.3);
  }
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output/ResxTime")))->cd("..");
  
  
  //create TDC_dist
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->mkdir("TDC_time");
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd("TDC_time");
  for(Int_t i=0;i<36;i++){
    sprintf(tmp_char,"tdc_cha_%d",i);  
    TH1D *histo1d=new TH1D( tmp_char, "Drift time charged; Time[ns]; counts", 3000, -1000, 2000);
  }
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output/TDC_time")))->cd("..");  

  histo2d=new TH2D( "hitres_dis", "Residual vs rdrift; Residual[cm]; Measured rdrift[cm]", 600, -0.3, 0.3,250,0.,1.);
  histo2d=new TH2D( "hitres_time", "Residual vs drift time; Time[ns]; Residual[cm]",600, -0.3, 0.3,bmcon->GetHitTimecut(),0.,bmcon->GetHitTimecut()+10.);
       
  if(bmcon->GetBMdebug()>10)
    cout<<"BmBooter::PrintResDist: fill the histos"<<endl;      
        
  //fill the histos
  for(Int_t i=0;i<residual_distance.size();i++){
    if(residual_distance.at(i).size()>4){
      ((TH2D*)(m_controlPlotter->GetTFile()->Get("BM_output/hitres_dis")))->Fill(residual_distance.at(i).at(4), residual_distance.at(i).at(3));
      ((TH2D*)(m_controlPlotter->GetTFile()->Get("BM_output/hitres_time")))->Fill(residual_distance.at(i).at(4),residual_distance.at(i).at(2));
      sprintf(tmp_char,"BM_output/ResVsDist_perCell/hitres_dis_perCell_%d",(Int_t) (residual_distance.at(i).at(1)+0.5));  
      ((TH2D*)(m_controlPlotter->GetTFile()->Get(tmp_char)))->Fill(residual_distance.at(i).at(4), residual_distance.at(i).at(3));    
      sprintf(tmp_char,"BM_output/ResxDist/hitres_x_dist_%d",(Int_t) (residual_distance.at(i).at(3)/0.945*bmcon->GetResnbin()));
      ((TH1D*)(m_controlPlotter->GetTFile()->Get(tmp_char)))->Fill(residual_distance.at(i).at(4));
      sprintf(tmp_char,"BM_output/ResxTime/hitres_x_time_%d",(Int_t) (residual_distance.at(i).at(2)/bmcon->GetHitTimecut()*bmcon->GetResnbin()));      
      ((TH1D*)(m_controlPlotter->GetTFile()->Get(tmp_char)))->Fill(residual_distance.at(i).at(4));
    }
    sprintf(tmp_char,"BM_output/TDC_time/tdc_cha_%d",(Int_t) (residual_distance.at(i).at(1)+0.5));      
    ((TH1D*)(m_controlPlotter->GetTFile()->Get(tmp_char)))->Fill(residual_distance.at(i).at(2));
  }
  
  if(bmcon->GetBMdebug()>10)
    cout<<"BmBooter::PrintResDist: fit hitres_x_dist"<<endl;
  
  //fit hitres_x_dist
  TF1 *fb = new TF1("fb","gaus", -0.3,0.3);
  histo1d=new TH1D( "resolution_dist", "Resolution evaluation; Distance from cell center [cm];Spatial Resolution[#mum]", bmcon->GetResnbin(), 0., 0.945);
  histo1d=new TH1D( "resolution_time_old", "Resolution evaluation; Time [ns];Spatial Resolution[#mum]", bmcon->GetResnbin(), 0., bmcon->GetHitTimecut());
  histo1d=new TH1D( "resolution_time_new", "Resolution evaluation; Time [ns];Spatial Resolution[#mum]", bmcon->GetResnbin(), 0., bmcon->GetHitTimecut());
  vector<Double_t> streleachbin(3,0);
  Double_t meantimeresolution=0.;
  for(Int_t i=0;i<bmcon->GetResnbin();i++){
    ((TH1D*)(m_controlPlotter->GetTFile()->Get("BM_output/resolution_time_old")))->SetBinContent(i+1,bmcon->ResoEvalTime(((TH1D*)(m_controlPlotter->GetTFile()->Get("BM_output/resolution_time_old")))->GetBinCenter(i+1))*10000.);    
    sprintf(tmp_char,"BM_output/ResxDist/hitres_x_dist_%d",i);      
    //~ ((TH1D*)(m_controlPlotter->GetTFile()->Get("BM_output/resolution")))->SetBinContent(i+1,((TH1D*)(m_controlPlotter->GetTFile()->Get(tmp_char)))->GetStdDev()*10000);    
    ((TH1D*)(m_controlPlotter->GetTFile()->Get(tmp_char)))->Fit("fb", "Q+");
    ((TH1D*)(m_controlPlotter->GetTFile()->Get("BM_output/resolution_dist")))->SetBinContent(i+1,fb->GetParameter(2)*10000.);    
    sprintf(tmp_char,"BM_output/ResxTime/hitres_x_time_%d",i);      
    ((TH1D*)(m_controlPlotter->GetTFile()->Get(tmp_char)))->Fit("fb", "Q+");
    ((TH1D*)(m_controlPlotter->GetTFile()->Get("BM_output/resolution_time_new")))->SetBinContent(i+1,fb->GetParameter(2)*10000.); 
    meantimeresolution+=fb->GetParameter(2)*10000;   
    streleachbin.at(0)=fb->GetParameter(1);
    streleachbin.at(1)=fb->GetParameter(2);
    streleachbin.at(2)=((TH1D*)(m_controlPlotter->GetTFile()->Get(tmp_char)))->GetEntries();
    strelresiduals.push_back(streleachbin);
  }
  TF1 tfpoly("tfpoly","pol10", 0.,bmcon->GetHitTimecut());
  ((TH1D*)(m_controlPlotter->GetTFile()->Get("BM_output/resolution_time_new")))->Fit("tfpoly", "Q+");
  bmcon->SetTimeReso(tfpoly);
  bmcon->SetMeanTimeReso(meantimeresolution/bmcon->GetResnbin());
    
return;
}


//try Legendre poly:
void BmBooter::LegendrePoly(){
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))==nullptr || bmcon->GetBMvietrack()==0)
    return;
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd();
  
  Double_t theta;
  TVector3 A0, Wvers;
  Int_t nxbin=360; //nxbin=360 if r and theta convention, 20 if p, f(p)
  Int_t res_index=0; 
  char tmp_char[200];
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->mkdir("Legendre_poly");
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd("Legendre_poly");
  TF1 *horiz = new TF1("horiz","[0]", 0,180);
  horiz->SetLineColor(2);
  horiz->SetLineWidth(1);
  TF1 *vert = new TF1("vert","(x-[0])*[1]", 0,180);
  vert->SetLineColor(4);
  vert->SetLineWidth(1);
  vert->FixParameter(1,10.);
  for(Int_t n=0;n<tracktr2dprojects.size();n++){
    if((Int_t)(tracktr2dprojects.at(n).at(0))%bmcon->GetBMvietrack()==0){  
      sprintf(tmp_char,"leg_poly_xz_%d",(Int_t)tracktr2dprojects.at(n).at(0));  
      TH2D *histolegxz=new TH2D( tmp_char, "Legendre polynomial space on xz plane; Theta[deg]; r[cm]", nxbin , 0., 180.,1000,-10., 10.);
      //~ TH2D *histolegxz=new TH2D( tmp_char, "Legendre polynomial space on xz plane; P; F(P)", nxbin , -1., 1.,400,-2.,2.);
      sprintf(tmp_char,"leg_poly_yz_%d",(Int_t)tracktr2dprojects.at(n).at(0));  
      TH2D *histolegyz=new TH2D( tmp_char, "Legendre polynomial space on yz plane; Theta[deg]; r[cm]", nxbin , 0., 180.,1000,-10., 10.);
      //~ TH2D *histolegyz=new TH2D( tmp_char, "Legendre polynomial space on yz plane; P; F(P)", nxbin , -1., 1.,400,-2.,2.);
      for(Int_t i=res_index;i<residual_distance.size();i++){
        //~ if(residual_distance.at(i).at(0)==(Int_t)tracktr2dprojects.at(n).at(0) && residual_distance.at(i).size()>4){
        if(residual_distance.at(i).at(0)==(Int_t)tracktr2dprojects.at(n).at(0)){
          res_index=i;
          bmgeo->SetA0Wvers(residual_distance.at(i).at(1), A0, Wvers);
          if(Wvers.Y()>0 && Wvers.X()==0){
            for(Int_t k=1;k<=nxbin;k++){
              theta=histolegxz->GetXaxis()->GetBinCenter(k);
              //~ histolegxz->Fill(theta, A0.X()-A0.Z()*theta+residual_distance.at(i).at(3)*sqrt(theta*theta+1.));
              //~ histolegxz->Fill(theta, -A0.X()+A0.Z()*theta+residual_distance.at(i).at(3)*sqrt(theta*theta+1.));
              histolegxz->Fill(theta, A0.Z()*cos(theta*DEG2RAD)+A0.X()*sin(theta*DEG2RAD)+residual_distance.at(i).at(3));  
              histolegxz->Fill(theta, A0.Z()*cos(theta*DEG2RAD)+A0.X()*sin(theta*DEG2RAD)-residual_distance.at(i).at(3));  
            }
          }else if(Wvers.X()>0 && Wvers.Y()==0){
            for(Int_t k=1;k<=nxbin;k++){
              theta=histolegyz->GetXaxis()->GetBinCenter(k);
              //~ histolegyz->Fill(theta, A0.Y()-A0.Z()*theta+residual_distance.at(i).at(3)*sqrt(theta*theta+1.));
              //~ histolegyz->Fill(theta, -A0.Y()+A0.Z()*theta+residual_distance.at(i).at(3)*sqrt(theta*theta+1.));             
              histolegyz->Fill(theta, A0.Z()*cos(theta*DEG2RAD)+A0.Y()*sin(theta*DEG2RAD)+residual_distance.at(i).at(3));  
              histolegyz->Fill(theta, A0.Z()*cos(theta*DEG2RAD)+A0.Y()*sin(theta*DEG2RAD)-residual_distance.at(i).at(3));  
            }
          }
        }
        if(residual_distance.at(i).at(0)>(Int_t)tracktr2dprojects.at(n).at(0))
          break;
      }//end of i loop on residual_distance  
      //draw two lines on the legendre th2d to represent the fitted theta and r
      theta=atan(-1./tracktr2dprojects.at(n).at(1));//here theta is in radiant;//maybe are inverted, to be checked!!!
      horiz->FixParameter(0,-tracktr2dprojects.at(n).at(2)*sin(theta));
      histolegxz->Fit("horiz","QC","", 0., 180.);
      theta=(theta<0) ? theta*RAD2DEG+180. : theta*RAD2DEG;
      vert->FixParameter(0,theta);
      histolegxz->Fit("vert","QC+","",theta-1.,theta+1.);
      theta=atan(-1./tracktr2dprojects.at(n).at(3));//here theta is in radiant;
      horiz->FixParameter(0,-tracktr2dprojects.at(n).at(4)*sin(theta));
      histolegyz->Fit("horiz","QC","", 0., 180.);
      theta=(theta<0) ? theta*RAD2DEG+180. : theta*RAD2DEG;
      vert->FixParameter(0,theta);
      histolegyz->Fit("vert","QC+","",theta-1.,theta+1.);

      //~ horiz->FixParameter(0,-tracktr2dprojects.at(n).at(2));
      //~ histolegxz->Fit("horiz","QC","", -1., 1.);
        
        
    }
  }
  
  delete horiz;
  delete vert;
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output/Legendre_poly")))->cd("..");  
  return;
}
  
  
void BmBooter::PrintMCxEvent(){
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))==nullptr)
    return;
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd();
  
  TH2D* histoa=new TH2D( "MC_mylar1_xy", "mylar1 projected tracks; x[cm]; y[cm]", 600, -3., 3.,600, -3.,3.);
  TH2D* histob=new TH2D( "MC_mylar2_xy", "mylar2 projected tracks; x[cm]; y[cm]", 600, -3., 3.,600, -3.,3.);
  TH1D* hphi=new TH1D("MC_angle_phi","track phi angle;Phi[DEG]; Number of events",360.,-180.,180.);
  TH1D* htheta=new TH1D("MC_angle_theta","track theta angle;Theta[DEG]; Number of events",180.,0.,180.);
  TVector3 mcpvers;
  for(Int_t i=0;i<mcxevent.size();i++){
    histoa->Fill(mcxevent.at(i).at(1), mcxevent.at(i).at(2));
    histob->Fill(mcxevent.at(i).at(3), mcxevent.at(i).at(4));
    mcpvers.SetXYZ(mcxevent.at(i).at(3)-mcxevent.at(i).at(1),mcxevent.at(i).at(4)-mcxevent.at(i).at(2),bmgeo->GetMylar2().Z()-bmgeo->GetMylar1().Z());
    hphi->Fill(mcpvers.Phi()*RAD2DEG);
    htheta->Fill(mcpvers.Theta()*RAD2DEG);
  }  
  
  return;
}


void BmBooter::evaluateT0() {
  
  Int_t petal1ch=46, petal2ch=45, petal3ch=44, petal4ch=43, sync_petal_gate=50, majodelay=-60;
  //~ TABMparCon* bmcon = (TABMparCon*) myp_bmcon->Object();  
  TString tmp_tstring="bmraw.root";
  m_nopath_instr="bmraw.root";
  if(m_instr.EndsWith(".dat")){
    m_nopath_instr=m_instr;
    if(m_instr.Last('/'))  
      m_nopath_instr.Remove(0,m_nopath_instr.Last('/')+1);
    tmp_tstring="bmraw_"+m_nopath_instr;
    m_nopath_instr.Remove(m_nopath_instr.Last('.')+1,m_nopath_instr.Last('.')+3);
    tmp_tstring.Replace(tmp_tstring.Last('.')+1,3,"root",4);
  }
  TFile *f_out = new TFile(tmp_tstring.Data(),"RECREATE");
  f_out->cd();
  TH1D* h=nullptr;
  TH2D* h2=nullptr;
  Int_t tmp_int, trash;
  char tmp_char[200];
  Int_t adc_maxbin=4200, rate_xevent_max=200000, sca_cha_maxbin=1000;

  //book histos
  //general histos
  h=new TH1D("rate_evtoev","Trigger rate, from bmstruct.time_evtoev;Hz; Number of events",1000,0.,20000.);
  h=new TH1D("rate_xevent_evtoev","trigger rate as function of the event number (from bmsturct.time_read);Event num scaled; Hz",rate_xevent_max,0.,rate_xevent_max);
  h=new TH1D("rate_beam_khz","Beam Rate with the trigger rate and the coincidence scaler meas;kHz; Number of events",1000,0.,20000.);
  h=new TH1D("rate_beam_hz","Beam Rate with the trigger rate and the coincidence scaler meas;Hz; Number of events",1000,0.,20000.);
  h=new TH1D("rate_xevent_beam_hz","Beam Rate with the trigger rate and the coincidence scaler meas as function of the event number ;Event num scaled; Hz",rate_xevent_max,0.,rate_xevent_max);
  //~ h=new TH1D("rate_timeacq","Rate of the events, from bmstruct.time_acq;Hz; Number of events",1000,0.,10000.);
  h=new TH1D("rate_readev","time occurred to read the data for each event, from bmsturct.time_read;[micro seconds]; Number of events",500,0.,1000.);
  
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
    
  gDirectory->mkdir("TDC_raw-sync");
    gDirectory->cd("TDC_raw-sync");
    for(Int_t i=0;i<bmmap->GetTdcMaxcha();i++){
      if(i!=bmmap->GetTrefCh())
        sprintf(tmp_char,"tdc_raw-sync_cha_%d",i);
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
    h=new TH1D("all_tdc_cha_meas","Number of tdc signals from meas; TDC channel; counts",bmmap->GetTdcMaxcha(),0.,bmmap->GetTdcMaxcha());
    gDirectory->cd("..");
    
  gDirectory->mkdir("TDC_meas_less_sync");
    gDirectory->cd("TDC_meas_less_sync");
    for(Int_t i=0;i<bmmap->GetTdcMaxcha();i++)
      if(i!=bmmap->GetTrefCh()){
        sprintf(tmp_char,"tdc_cha-sync_%d",i);
        h=new TH1D(tmp_char,"TDC time - synctime;Time [ns]; counts",3000,-1000.,2000.);
      }
    gDirectory->cd("..");
  
  h=new TH1D("tdc_majo","Number of events with the 4 petals majority; majority; events",5,0.,5);
  h=new TH1D("tdc_error","0=ok 1=evnum 2=ch_out 3=wordnum 4=nosync 5=multisync 6=multievent; Number of tdc signals with errors; counts",10,0.,10);//distinguish the type of error!
  h2=new TH2D("bmhitnum_vs_tdcsyncnum","number of bmhits vs tdc sync;number of bmhits; tdc number of sync",30,0.,30,5,0,5);
  f_out->cd("..");
  if(bmmap->GetSca830Ch()>0){
    f_out->mkdir("SCA");
    f_out->cd("SCA");
    for(Int_t i=0;i<bmmap->GetSca830Ch();i++){
      sprintf(tmp_char,"sca_cha_%d",i);  
      h=new TH1D(tmp_char,"Scaler counts;Scaler counts; Number of events",sca_cha_maxbin,0.,sca_cha_maxbin);
    }    
    h=new TH1D("sca_allch","Scaler channel with at least 1 count x event;Scaler channel; Number of events",bmmap->GetSca830Ch(),0.,bmmap->GetSca830Ch());
    h=new TH1D("sca_allcounts_allch","Scaler channel final counts;Scaler channel; Counts",bmmap->GetSca830Ch(),0.,bmmap->GetSca830Ch());
    h=new TH1D("sca_error","Scaler Error;0=no_error    1=error; Number of events",2,0.,2);
    f_out->cd("..");
  }
  if(bmmap->GetAdc792Ch()>0){
    f_out->mkdir("ADC");
    f_out->cd("ADC");
    h=new TH1D("adc_error","Adc Error;0=no_error  1=read data error  2=overflow channels  3=channel overflow not set; Number of events",3,0.,3);
    h=new TH1D("adc_pedmeas","Adc pedestals level ;channel; Number of counts",bmmap->GetAdc792Ch(),0.,bmmap->GetAdc792Ch());
    h=new TH1D("adc_pedrms","Adc pedestals rms ;channel; Counts rms",bmmap->GetAdc792Ch(),0.,bmmap->GetAdc792Ch());   
    if(bmmap->GetAdcPetNum()>0){
      h=new TH1D("adc_petals_sum_meas","Adc sum of the petals measurement - pedestal;Adc measurement; Number of events",adc_maxbin*4,0.,adc_maxbin*4);   
      h2=new TH2D("adc_vs_numhit","Sum of the margherita Adc counts vs number of BM hits;Sum of margherita Adc counts; Number of BM hits",adc_maxbin,0.,adc_maxbin,30,0,30.);
      h2=new TH2D("adc_vs_tdcsyncnum","Sum of the margherita Adc counts vs number of BM sync;Sum of margherita Adc counts; Number of BM sync",adc_maxbin,0.,adc_maxbin,5,0,5.);
      h=new TH1D("rate_adc_up","Rate of the event with a qdc value > adc_double;Hz; Number of events",1000,0.,20000.);
    }
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
  
  //charge the tdc_cha_* TH1D graph of the tdc signals
  vector<vector<Double_t>> rate_evtoev_vecvec; //vector of evtoevstuff_vec for each event
  vector<Double_t> evtoevstuff_vec(4,0.);  //0=time_evtoev; 1=scaler counter of the coincidance; 2=qdc sum of the 4 petals; 3= time of the acquisition  
  Int_t only_bm_signal, tdc_syncnum;
  while(read_event(kTRUE)) {
    if(bmcon->GetBMdebug()>11 && bmcon->GetBMdebug()!=99)
      cout<<"data_num_ev="<<data_num_ev<<endl<<"Fill the general graph and tdc"<<endl;
    //General Graphs
    ((TH1D*)gDirectory->Get("rate_evtoev"))->Fill(1000000./bmstruct.time_evtoev);
    ((TH1D*)gDirectory->Get("rate_readev"))->Fill(bmstruct.time_read);
    ((TH1D*)gDirectory->Get("rate_beam_hz"))->Fill(1000000./bmstruct.time_evtoev*bmstruct.sca830_meas[bmmap->GetScaCoinc()]);
    ((TH1D*)gDirectory->Get("rate_beam_khz"))->Fill(1000./bmstruct.time_evtoev*bmstruct.sca830_meas[bmmap->GetScaCoinc()]);
    
    //TDC  
    tmp_int=0;
    only_bm_signal=0;
    tdc_syncnum=0;
    sprintf(tmp_char,"TDC/TDC_raw/tdc_rawsynccha_%d",bmmap->GetTrefCh());
    while(bmstruct.tdc_sync[tmp_int]!=-10000){
      ((TH1D*)gDirectory->Get("TDC/TDC_raw/all_tdc_rawcha"))->Fill(bmmap->GetTrefCh());       
      ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.tdc_sync[0])/10.);    
      tmp_int++;
    }
    tmp_int=0;
    vector<Int_t> hitpetals(4,0);
    while(bmstruct.tdc_meas[tmp_int]!=-10000){
      sprintf(tmp_char,"TDC/TDC_raw/tdc_rawcha_%d",bmstruct.tdc_id[tmp_int]);
      ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.tdc_meas[tmp_int])/10.);    
      ((TH1D*)gDirectory->Get("TDC/TDC_raw/all_tdc_rawcha"))->Fill(bmstruct.tdc_id[tmp_int]);            
      if(bmstruct.tdc_id[tmp_int]!=bmmap->GetTrefCh() && bmstruct.tdc_sync[0]!=-10000){    
        sprintf(tmp_char,"TDC/TDC_raw-sync/tdc_raw-sync_cha_%d",bmstruct.tdc_id[tmp_int]);
        ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.tdc_meas[tmp_int]-bmstruct.tdc_sync[0])/10.);    
      }
      if(bmstruct.tdc_id[tmp_int]==petal1ch && bmstruct.tdc_sync[0]!=-10000)
        if(fabs(bmstruct.tdc_meas[tmp_int]/10.-bmstruct.tdc_sync[0]/10.-majodelay)<sync_petal_gate)
          hitpetals.at(0)=1;
      if(bmstruct.tdc_id[tmp_int]==petal2ch && bmstruct.tdc_sync[0]!=-10000)
        if(fabs(bmstruct.tdc_meas[tmp_int]/10.-bmstruct.tdc_sync[0]/10.-majodelay)<sync_petal_gate)
          hitpetals.at(1)=1;
      if(bmstruct.tdc_id[tmp_int]==petal3ch && bmstruct.tdc_sync[0]!=-10000)
        if(fabs(bmstruct.tdc_meas[tmp_int]/10.-bmstruct.tdc_sync[0]/10.-majodelay)<sync_petal_gate)
          hitpetals.at(2)=1;
      if(bmstruct.tdc_id[tmp_int]==petal4ch && bmstruct.tdc_sync[0]!=-10000)
        if(fabs(bmstruct.tdc_meas[tmp_int]/10.-bmstruct.tdc_sync[0]/10.-majodelay)<sync_petal_gate)
          hitpetals.at(3)=1;
      tmp_int++;
    }
    Int_t majo_soft=0;
    for(Int_t i=0;i<4;i++)
      majo_soft+=hitpetals.at(i);
    for(Int_t i=1;i<=majo_soft;i++)
      ((TH1D*)gDirectory->Get("TDC/tdc_majo"))->Fill(i);            
          
    if(bmstruct.tot_status==0 && bmstruct.tdc_status==-1000){ 
      tmp_int=0;
      while(bmstruct.tdc_id[tmp_int]!=-10000 && tmp_int<MAXHITTDC){
        if(bmmap->tdc2cell(bmstruct.tdc_id[tmp_int])!=-1)
          only_bm_signal++;
        tmp_int++;
      }
      tmp_int=0;
      while(bmstruct.tdc_sync[tmp_int]!=-10000 && tmp_int<MAXHITTDC){
        if(bmmap->tdc2cell(bmstruct.tdc_id[tmp_int])!=-1)
          tdc_syncnum++;
        tmp_int++;
      }
      if(bmmap->GetAdc792Ch()>0 && bmmap->GetAdcPetNum()>0){
        ((TH2D*)gDirectory->Get("ADC/adc_vs_numhit"))->Fill(bmstruct.adc792_sum, only_bm_signal);
        ((TH2D*)gDirectory->Get("ADC/adc_vs_tdcsyncnum"))->Fill(bmstruct.adc792_sum, tdc_syncnum);
      }
      ((TH2D*)gDirectory->Get("TDC/bmhitnum_vs_tdcsyncnum"))->Fill(only_bm_signal, tdc_syncnum);
      if(bmstruct.tdcev==1 && bmstruct.tdc_sync[0]!=-10000 && bmstruct.tdc_sync[1]==-10000){
        ((TH1D*)gDirectory->Get("TDC/tdc_error"))->Fill(0);//no error
        ((TH1D*)gDirectory->Get("TDC/TDC_meas/all_tdc_cha_meas"))->Fill(bmmap->GetTrefCh());   
        sprintf(tmp_char,"TDC/TDC_meas/tdc_synccha_%d",bmmap->GetTrefCh());  
        ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.tdc_sync[0])/10.);    
        for(Int_t i=0;i<bmstruct.tdc_hitnum[0];i++){
          //~ cout<<"data_num_ev="<<data_num_ev<<"  i="<<i<<"  bmstruct.tdc_hitnum[0]="<<bmstruct.tdc_hitnum[0]<<"  bmstruct.tdc_id[i]="<<bmstruct.tdc_id[i]<<"  bmmap->tdc2cell(bmstruct.tdc_id[i])="<<bmmap->tdc2cell(bmstruct.tdc_id[i])<<"  (Double_t) (bmstruct.tdc_meas[i])/10.="<<(Double_t) (bmstruct.tdc_meas[i])/10.<<endl; //provv
          if(bmmap->tdc2cell(bmstruct.tdc_id[i])!=-1){
            sprintf(tmp_char,"TDC/TDC_meas/tdc_cha_%d",bmstruct.tdc_id[i]);
            ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.tdc_meas[i])/10.);    
            sprintf(tmp_char,"TDC/TDC_meas_less_sync/tdc_cha-sync_%d",bmstruct.tdc_id[i]);
            ((TH1D*)gDirectory->Get(tmp_char))->Fill((Double_t) (bmstruct.tdc_meas[i]-bmstruct.tdc_sync[0])/10.);    
            ((TH1D*)gDirectory->Get("TDC/TDC_meas/all_tdc_cha_meas"))->Fill(bmstruct.tdc_id[i]);    
          }   
        } 
      }else{
        if(bmstruct.tdc_sync[0]==-10000){
          cout<<"BmBooter::evaluateT0::ERROR no sync in the TDC data at data_num_ev="<<data_num_ev<<endl;
          ((TH1D*)gDirectory->Get("TDC/tdc_error"))->Fill(4);//no synctime
        }
        else if(bmstruct.tdc_sync[1]!=-10000){      
          if(bmcon->GetBMdebug()>3)
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
      ((TH1D*)gDirectory->Get("ADC/adc_error"))->Fill(bmstruct.adc_status);
      ((TH1D*)gDirectory->Get("ADC/adc_petals_sum_meas"))->Fill(bmstruct.adc792_sum);
      if(bmstruct.adc_status!=1){
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
      
    //fill evtoevstuff_vec and rate_evtoev_vecvec
    if(bmstruct.tot_status==0){
      for(Int_t i=0;i<evtoevstuff_vec.size();i++)
        evtoevstuff_vec.at(i)=0;
      evtoevstuff_vec.at(0)=bmstruct.time_evtoev;  //to transform in hz: 1000000./bmstruct.time_evtoev
      if(bmmap->GetSca830Ch()>0 && bmmap->GetScaCoinc()>=0)
        evtoevstuff_vec.at(1)=bmstruct.sca830_meas[bmmap->GetScaCoinc()];
      if(bmmap->GetAdc792Ch()>0 && bmmap->GetAdcPetNum()>0)
        evtoevstuff_vec.at(2)=bmstruct.adc792_sum;
      evtoevstuff_vec.at(3)=bmstruct.time_acq;
      
      rate_evtoev_vecvec.push_back(evtoevstuff_vec);
    }
    data_num_ev++;
  }//end of the while readevent loop
  data_num_ev--;

  if(bmcon->GetBMdebug()>3)
    cout<<"BMbooter::evaluatet0 step: end of the while loop on readevent"<<endl;

  //fill rate and other graphs
  for(Int_t i=0;i<bmmap->GetSca830Ch();i++)
    ((TH1D*)gDirectory->Get("SCA/sca_allcounts_allch"))->SetBinContent(i+1,bmstruct.sca830_counts[i]);

  if(rate_evtoev_vecvec.size()<rate_xevent_max){
    for(Int_t i=1;i<rate_evtoev_vecvec.size();i++){
      ((TH1D*)gDirectory->Get("rate_xevent_evtoev"))->SetBinContent(i+1,1000000./rate_evtoev_vecvec.at(i).at(0));
      if(bmmap->GetSca830Ch()>0 && bmmap->GetScaCoinc()>=0){
        ((TH1D*)gDirectory->Get("rate_xevent_beam_hz"))->SetBinContent(i+1,1000000./rate_evtoev_vecvec.at(i).at(0)*rate_evtoev_vecvec.at(i).at(1));
      }
    }
    for(Int_t i=rate_evtoev_vecvec.size();i<rate_xevent_max;i++){
      ((TH1D*)gDirectory->Get("rate_xevent_beam_hz"))->SetBinContent(i+1,0);
    }
  }else{
    Int_t lastvecpos=1, step=rate_evtoev_vecvec.size()/rate_xevent_max;//step has to be an Int_t
    Double_t mean_trigger, mean_beam;
    for(Int_t i=1; i<rate_xevent_max;i++){
      mean_trigger=0.;
      mean_beam=0.;
      for(Int_t k=0;k<step;k++){
        mean_trigger+=1000000./rate_evtoev_vecvec.at(lastvecpos).at(0);
        if(bmmap->GetSca830Ch()>0 && bmmap->GetScaCoinc()>=0)
          mean_beam+=1000000./rate_evtoev_vecvec.at(lastvecpos).at(0)*rate_evtoev_vecvec.at(lastvecpos).at(1);
        lastvecpos++;
      }
      ((TH1D*)gDirectory->Get("rate_xevent_evtoev"))->SetBinContent(i+1,mean_trigger/step);
      if(bmmap->GetSca830Ch()>0 && bmmap->GetScaCoinc()>=0){
        ((TH1D*)gDirectory->Get("rate_xevent_beam_hz"))->SetBinContent(i+1,mean_beam/step);
      }
    }
  }
  
  if(bmmap->GetAdc792Ch()>0 && bmmap->GetAdcPetNum()>0){
    Double_t time_last=-1000.;
    for(Int_t i=0;i<rate_evtoev_vecvec.size();i++){
      if(rate_evtoev_vecvec.at(i).at(2)>bmmap->GetAdcDouble()){
        if(time_last==-1000.)
          time_last=rate_evtoev_vecvec.at(i).at(3);
        else{
          ((TH1D*)gDirectory->Get("ADC/rate_adc_up"))->Fill(1000000./(rate_evtoev_vecvec.at(i).at(3)-time_last));
          time_last=rate_evtoev_vecvec.at(i).at(3);
        }
      }
    }
  }

//write to output_txt
  if(bmmap->GetAdc792Ch()>0 && bmmap->GetAdcPetNum()>0){
    Double_t adcratio=((TH1D*)gDirectory->Get("ADC/adc_petals_sum_meas"))->Integral(bmmap->GetAdcDouble(),adc_maxbin)/((TH1D*)gDirectory->Get("ADC/adc_petals_sum_meas"))->Integral(0,bmmap->GetAdcDouble());
    ofstream outtxt;
    outtxt.open(txt_outputname.Data(),ios::app);
    outtxt<<"file name: "<<m_nopath_instr<<"  analized at:"<<std::time(0)<<endl;
    outtxt<<"busy trigger rate="<<((TH1D*)gDirectory->Get("rate_evtoev"))->GetMean()<<"  nobusy rate="<<((TH1D*)gDirectory->Get("rate_beam_hz"))->GetMean()<<endl;
    outtxt<<"adc threshold="<<bmmap->GetAdcDouble()<<"  adc under="<<((TH1D*)gDirectory->Get("ADC/adc_petals_sum_meas"))->Integral(0,bmmap->GetAdcDouble())<<"  adc over="<<((TH1D*)gDirectory->Get("ADC/adc_petals_sum_meas"))->Integral(bmmap->GetAdcDouble(),adc_maxbin)<<"  adc: double/single="<<adcratio<<endl;
    cout<<"rescaled the rate of the beam="<<((TH1D*)gDirectory->Get("rate_beam_hz"))->GetMean()*(adcratio+1.)<<endl;
    outtxt.close();
  }
  
  
  //I created the TDC signal histograms
  
  //fit the tdc signals with a function to evaluate the T0, for the moment I take the shoresidual_distancertest signal close to the peak
  //~ TF1 *f1 = new TF1("f1","[0]*pow(([1]/[2]),(x/[2]))(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1)", 0, 2000);
  //~ TF1 *f1 = new TF1("f1","[0]*pow([1]/[2],x/[2]-[3])/(TMath::Gamma(x/[2]-[3]+1))*(TMath::Exp(-[1]/[2]))", 0, 2000);
  //~ TF1 *f1 = new TF1("f1","gaus(0)", ((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()-100, ((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()+100);


  //EVALUATE T0
  Int_t tdc_peak, start_bin, peak_bin;
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
          start_bin=0;
          peak_bin=((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin();
          tdc_peak=peak_bin;
          for(Int_t j=((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin();j>0;j--)
            if(((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(j)>((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin())/10.)
              start_bin=j;
          if(bmcon->GetT0switch()==0)
            tdc_peak=start_bin;
          for(Int_t j=start_bin;j<=((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()+1;j++)
            if((((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(j) < ((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(j-1)) && (((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(j-1) > ((TH1D*)gDirectory->Get(tmp_char))->GetBinContent(((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin())*3./5.)){
              peak_bin=j-1;      
              break;      
            }              
          if(bmcon->GetT0switch()==1 || bmcon->GetT0switch()==2)
            tdc_peak=peak_bin;
          if(bmcon->GetT0switch()==3)//T0=middle point between first signal and peak   
            tdc_peak=(peak_bin+start_bin+0.5)/2;
          if(start_bin==0 || peak_bin==0)
            cout<<"WARNING in BmBooter::EvaluateT0:: check if your tdc_peak is ok:   cell: "<<bmmap->tdc2cell(i)<<"  i="<<i<<"   peak_bin="<<peak_bin<<"   start_bin="<<start_bin<<"  tdc_peak="<<tdc_peak<<"   MaximumBin="<<((TH1D*)gDirectory->Get(tmp_char))->GetMaximumBin()<<endl;
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
      //~ if(read_meas && ev_words[windex]<0 && (bmstruct.tdc_status==0 || bmstruct.tdc_status==-1000)){//global trailer found //se uso acquisizione mio (yun)
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
      if(read_meas && ev_words[windex]==0 && (bmstruct.tdc_status==0 || bmstruct.tdc_status==-1000)){//global trailer found //se uso acquisizione trento con 0 invece che -1000
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
              
      //only for data from cosmic rays taken in 2017 in Rome        
      //~ if(read_meas && ev_words[windex]==0 && false && (bmstruct.tdc_status==0 || bmstruct.tdc_status==-1000)){//global trailer found //se uso dati letti a Roma per BM refurbishment
        //~ read_meas=false;
        //~ new_event=true;
        //~ bmstruct.tdc_status=-1000;
        //~ if(bmcon->GetBMdebug()>11 && (!(evt0 && bmcon->GetBMdebug()==99)))
          //~ cout<<"global trailer found, i="<<windex<<"  ev_words="<<ev_words[windex]<<endl;
      //~ }        
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
  bmstruct.tdc_numsync=sync_evnum;
  
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
    data = adc792_words.at(iac);
    if(bltread) {
      dt_type = data>>24 & 0x7;
      if(bmcon->GetBMdebug()>3)      
        cout<<"BMbooter::monitorQDC:: dt_type="<<dt_type<<endl;
      if(!(dt_type & 0x7)) {
        qdc_cnt = data & 0xFFF;
        chan = data>>17 & 0xF;
        if(bmcon->GetBMdebug()>11) 
          cout<<"BMbooter::monitorQDC:: qdc_cnt="<<qdc_cnt<<"   chan="<<chan<<" "<<"  adc792_words["<<iac<<"]="<<adc792_words.at(iac)<<endl;
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
        if(bmmap->GetAdcPetNum()==4){
          if(chan==bmmap->GetAdcPetBegin() || chan==(bmmap->GetAdcPetBegin()+1) || chan==(bmmap->GetAdcPetBegin()+2) || chan==(bmmap->GetAdcPetBegin()+3)){
            bmstruct.adc792_sum+=qdc_cnt-bmcon->GetADCped(chan);
          }
        }else if(bmmap->GetAdcPetNum()==3){
          if(chan==bmmap->GetAdcPetBegin() || chan==(bmmap->GetAdcPetBegin()+1) || chan==(bmmap->GetAdcPetBegin()+2)){
            bmstruct.adc792_sum+=qdc_cnt-bmcon->GetADCped(chan);
          }
        }else if(bmmap->GetAdcPetNum()==2){
          if(chan==bmmap->GetAdcPetBegin() || chan==(bmmap->GetAdcPetBegin()+1)){
            bmstruct.adc792_sum+=qdc_cnt-bmcon->GetADCped(chan);
          }
        }else if(bmmap->GetAdcPetNum()==1){
          if(chan==bmmap->GetAdcPetBegin()){
            bmstruct.adc792_sum+=qdc_cnt-bmcon->GetADCped(chan);
          }
        }
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
    cell_occupy.at(i).clear();
  
  //charge cell_occupy
  for(Int_t i=0;i<bmnturaw->nhit;i++){
    bmntuhit=bmnturaw->Hit(i);
    cell_index=bmgeo->GetBMNcell(bmntuhit->Plane(),bmntuhit->View(), bmntuhit->Cell());
    if(cell_occupy.at(cell_index).size()==0)
      cell_occupy.at(cell_index).push_back(i);
    else{
      tmp_int=cell_occupy.at(cell_index).size();
      while(tmp_int){
        tmp_int--;
        oldhit=bmnturaw->Hit(cell_occupy.at(cell_index).at(tmp_int));
        if(oldhit->Dist()<bmntuhit->Dist()){
          cell_occupy.at(cell_index).insert(cell_occupy.at(cell_index).begin()+tmp_int+1, i);
          tmp_int=0;//exit the while        
        }
      }  
    }
  }
  
  if(bmcon->GetBMdebug()>12){
    cout<<"BmBooter::evaluate_cell_occupy: print cell_occupy"<<endl;
    for(Int_t i=0;i<36;i++){
      if(cell_occupy.at(i).size()>0){
        cout<<"cell index="<<i<<"  hits=";
        for(Int_t j=0;j<cell_occupy.at(i).size();j++)
          cout<<cell_occupy.at(i).at(j)<<" ";
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
    if(cell_occupy.at(i).size()>0 && cell_occupy.at(i+12).size()>0){
      eff_pp.at(0).at(pivota)++;
      if(cell_occupy.at(i+6).size()>0 || cell_occupy.at(i+7).size()>0)
        eff_pp.at(1).at(pivota)++;
    }
    pivota++;
  }

  if(bmcon->GetBMdebug()>12){
    cout<<"BmBooter::efficiency_pivot_probe: print eff_pp"<<endl<<"eff_pp.at(0)=";
    for(Int_t i=0;i<eff_pp.at(0).size();i++)
      cout<<" "<<eff_pp.at(0).at(i);
    cout<<endl<<"eff_pp.at(1)=";
    for(Int_t i=0;i<eff_pp.at(1).size();i++)
      cout<<" "<<eff_pp.at(1).at(i);
    cout<<endl;
  }
return;
}

void BmBooter::efficiency_fittedplane(){

  vector<Int_t> hit_fittedplane(12,0);
  
  for (Int_t i = 0; i < bmnturaw->nhit; i++) { 
    bmntuhit = bmnturaw->Hit(i); 
    if(bmntuhit->GetIsSelected() && bmntutrack->trk_status==0){
      if(bmntuhit->View()==0)
        hit_fittedplane.at(bmntuhit->Plane())++;
      else
        hit_fittedplane.at(bmntuhit->Plane()+6)++;
    }  
  }  

  //view==0
  if(hit_fittedplane.at(0)>0 && hit_fittedplane.at(2)>0 && hit_fittedplane.at(4)>0){
    eff_fittedplane.at(0).at(0)++;
    if(hit_fittedplane.at(1)>0 && hit_fittedplane.at(3)>0)
      eff_fittedplane.at(0).at(1)++;
  }
  if(hit_fittedplane.at(1)>0 && hit_fittedplane.at(3)>0 && hit_fittedplane.at(5)>0){
    eff_fittedplane.at(1).at(0)++;
    if(hit_fittedplane.at(2)>0 && hit_fittedplane.at(4)>0)
      eff_fittedplane.at(1).at(1)++;
  }
  //view==1
  if(hit_fittedplane.at(6)>0 && hit_fittedplane.at(8)>0 && hit_fittedplane.at(10)>0){
    eff_fittedplane.at(2).at(0)++;
    if(hit_fittedplane.at(7)>0 && hit_fittedplane.at(9)>0)
      eff_fittedplane.at(2).at(1)++;
  }
  if(hit_fittedplane.at(7)>0 && hit_fittedplane.at(9)>0 && hit_fittedplane.at(11)>0){
    eff_fittedplane.at(3).at(0)++;
    if(hit_fittedplane.at(8)>0 && hit_fittedplane.at(10)>0)
      eff_fittedplane.at(3).at(1)++;
  }
  
  //~ for(Int_t i=0;i<4;i++){
    //~ sum_pivot+=eff_fittedplane.at(i).at(0);
    //~ sum_probe+=eff_fittedplane.at(i).at(1);    
  //~ }
  //~ cout<<"sum_pivot="<<sum_pivot<<"  sum_probe="<<sum_probe<<"  data_num_ev="<<data_num_ev<<endl;
    
return;
}

//it's a wrong method!!!! need to be fixed
void BmBooter::efficiency_fittedtracks(){
  
  TVector3 A0, Wvers;
  Double_t fitrdrift;
  Bool_t misshit;
  Int_t cell, tothit=0, totmiss=0;

  for (Int_t i = 0; i < bmntutrack->ntrk; i++) {
    bmntutracktr = bmntutrack->Track(i);
    //~ if(bmntutracktr->GetNhit()==12)//if a track has 12 associated hits, it can not have misshits
      //~ break;  
    for(Int_t k=0;k<cell_occupy.size();k++){
      bmgeo->SetA0Wvers(k, A0,Wvers);      
      fitrdrift=bmntutracktr->FindRdrift(bmntutracktr->GetR0(), bmntutracktr->GetPvers(), A0, Wvers);
      if(fitrdrift<0.7 && cell_occupy.at(k).size()==0){//there's a misshit
        m_controlPlotter->FillMap("BM_output__misshitrdrift",fitrdrift); 
        totmiss++;
      }  
      else if(fitrdrift<0.8 && fitrdrift>=0.7 && cell_occupy.at(k).size()==0){//I have to check the contiguous cells
        misshit=kTRUE;
        cell=bmgeo->CellId2cell(k);
        if(cell==0 || cell==1)
          if(cell_occupy.at(k+1).size()>0)
            for(Int_t j=0;j<cell_occupy.at(k+1).size();j++){
              bmntuhit=bmnturaw->Hit(cell_occupy.at(k+1).at(j));
              if(bmntuhit->Dist()>=0.7)
                misshit=kFALSE;
            }
        if((cell==1 || cell==2) && misshit)                          
          if(cell_occupy.at(k-1).size()>0)
            for(Int_t j=0;j<cell_occupy.at(k-1).size();j++){
              bmntuhit=bmnturaw->Hit(cell_occupy.at(k-1).at(j));
              if(bmntuhit->Dist()>=0.7)
                misshit=kFALSE;
            }          
        if(misshit){
          m_controlPlotter->FillMap("BM_output__misshitrdrift",fitrdrift);
          totmiss++;  
        }
      }else if((cell_occupy.at(k).size()>0 && fitrdrift<0.8) ||  (fitrdrift<0.8 && fitrdrift>=0.7 && cell_occupy.at(k).size()==0 && misshit==kFALSE) )
        tothit++;     
    }
  }
  
  m_controlPlotter->FillMap("BM_output__eff_fittedtrack_eval",((Double_t)(tothit-totmiss))/tothit);    
  m_controlPlotter->FillMap("BM_output__eff_fittedtrack_tothit",tothit);    
  m_controlPlotter->FillMap("BM_output__eff_fittedtrack_totmiss",totmiss);    
  
  return;
}

void BmBooter::efficiency_paoloni(){
  //~ for (Int_t i = 0; i < bmnturaw->nhit; i++) { 
    //~ bmntuhit = bmnturaw->Hit(i); 
    //~ if(bmntuhit->View()==0)
      //~ hit_plane[bmntuhit->Plane()]++;
    //~ else
      //~ hit_plane[bmntuhit->Plane()+6]++;
  //~ }
  
  //~ //eff_plane calculation
  //~ //view==0
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
  //~ //view==0
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
  if(cell_occupy.at(0).size()>0 && cell_occupy.at(12).size()>0 && cell_occupy.at(24).size()>0){
    eff_plane.at(0).at(0)++;
    if((cell_occupy.at(7).size()>0 || cell_occupy.at(6).size()>0) && (cell_occupy.at(19).size()>0 || cell_occupy.at(18).size()>0))
      eff_plane.at(0).at(1)++;
  }
  if(cell_occupy.at(1).size()>0 && cell_occupy.at(13).size()>0 && cell_occupy.at(25).size()>0){
    eff_plane.at(1).at(0)++;
    if((cell_occupy.at(8).size()>0 || cell_occupy.at(7).size()>0) && (cell_occupy.at(19).size()>0 || cell_occupy.at(20).size()>0))
      eff_plane.at(1).at(1)++;
  }
  if(cell_occupy.at(7).size()>0 && cell_occupy.at(19).size()>0 && cell_occupy.at(31).size()>0){
    eff_plane.at(2).at(0)++;
    if((cell_occupy.at(12).size()>0 || cell_occupy.at(13).size()>0) && (cell_occupy.at(24).size()>0 || cell_occupy.at(25).size()>0))
      eff_plane.at(2).at(1)++;
  }
  if(cell_occupy.at(8).size()>0 && cell_occupy.at(20).size()>0 && cell_occupy.at(32).size()>0){
    eff_plane.at(3).at(0)++;
    if((cell_occupy.at(13).size()>0 || cell_occupy.at(14).size()>0) && (cell_occupy.at(25).size()>0 || cell_occupy.at(26).size()>0))
      eff_plane.at(3).at(1)++;
  }
  
  //yview
  if(cell_occupy.at(4).size()>0 && cell_occupy.at(16).size()>0 && cell_occupy.at(28).size()>0){
    eff_plane.at(4).at(0)++;
    if((cell_occupy.at(10).size()>0 || cell_occupy.at(19).size()>0) && (cell_occupy.at(22).size()>0 || cell_occupy.at(21).size()>0))
      eff_plane.at(4).at(1)++;
  }
  if(cell_occupy.at(5).size()>0 && cell_occupy.at(17).size()>0 && cell_occupy.at(29).size()>0){
    eff_plane.at(5).at(0)++;
    if((cell_occupy.at(10).size()>0 || cell_occupy.at(11).size()>0) && (cell_occupy.at(22).size()>0 || cell_occupy.at(23).size()>0))
      eff_plane.at(5).at(1)++;
  }
  if(cell_occupy.at(9).size()>0 && cell_occupy.at(21).size()>0 && cell_occupy.at(33).size()>0){
    eff_plane.at(6).at(0)++;
    if((cell_occupy.at(15).size()>0 || cell_occupy.at(16).size()>0) && (cell_occupy.at(27).size()>0 || cell_occupy.at(28).size()>0))
      eff_plane.at(6).at(1)++;
  }
  if(cell_occupy.at(10).size()>0 && cell_occupy.at(22).size()>0 && cell_occupy.at(34).size()>0){
    eff_plane.at(7).at(0)++;
    if((cell_occupy.at(16).size()>0 || cell_occupy.at(17).size()>0) && (cell_occupy.at(28).size()>0 || cell_occupy.at(29).size()>0))
      eff_plane.at(7).at(1)++;
  }
  
return;
}


//to be used in process to charge mylarproject
void BmBooter::Projecttracktr(){
  
  vector<Double_t> tracktr_pro(5);
  for (Int_t i = 0; i < bmntutrack->ntrk; i++) {
    bmntutracktr = bmntutrack->Track(i);  
    tracktr_pro.at(0)=data_num_ev;
    tracktr_pro.at(1)=bmntutracktr->GetPvers().X();
    tracktr_pro.at(2)=bmntutracktr->GetR0().X();
    tracktr_pro.at(3)=bmntutracktr->GetPvers().Y();
    tracktr_pro.at(4)=bmntutracktr->GetR0().Y();
  }
  tracktr2dprojects.push_back(tracktr_pro);
  
return;
}
  
Bool_t BmBooter::MCxEvent(){
  
  vector<Double_t> vec_pro(5);
  Int_t nuhit=0, nvhit=0;
  for(Int_t i=0;i<bmnturaw->nhit;i++){
    bmntuhit=bmnturaw->Hit(i);
    if(bmntuhit->View()==0 && !bmntuhit->GetIsFake())
      nuhit++;
    else if(!bmntuhit->GetIsFake())
      nvhit++;
  }
  if(nvhit<3 || nuhit<3)
    return kFALSE;
  
  vec_pro.at(0)=data_num_ev;
  TVector3 inpos, outpos;
  inpos.SetZ(99999);
  outpos.SetZ(-99999);
  for(Int_t i=0;i<evStr->BMNn;i++){
    if(evStr->BMNzin[i]<inpos.Z() && evStr->TRpaid[evStr->BMNid[i]-1]==0)
      inpos.SetXYZ(evStr->BMNxin[i], evStr->BMNyin[i], evStr->BMNzin[i]);
    if(evStr->BMNzout[i]>outpos.Z() && evStr->TRpaid[evStr->CROSSid[i]-1]==0)
      outpos.SetXYZ(evStr->BMNxout[i], evStr->BMNyout[i], evStr->BMNzout[i]);
  }
  
  if(inpos.Z()==99999 || outpos.Z()==-99999)
    return kFALSE;
    
  bmgeo->Global2Local(inpos);
  bmgeo->Global2Local(outpos);
  mylar1realpos=bmgeo->ProjectFromTwoPoints(inpos, outpos, bmgeo->GetMylar1().Z());
  mylar2realpos=bmgeo->ProjectFromTwoPoints(inpos, outpos, bmgeo->GetMylar2().Z());    
  vec_pro.at(1)=mylar1realpos.X();
  vec_pro.at(2)=mylar1realpos.Y();
  vec_pro.at(3)=mylar2realpos.X();
  vec_pro.at(4)=mylar2realpos.Y();
  
  mcxevent.push_back(vec_pro);
  
  return kTRUE;
}  
  
//used in process to charge residual_distance
void BmBooter::ResidualDistance(){
  vector<Double_t> selecthit(5);
  vector<Double_t> rejhit(4);
  
  for(Int_t i=0;i<bmnturaw->nhit;i++){
    bmntuhit = bmnturaw->Hit(i);
    if(bmntuhit->GetIsSelected()){
      selecthit.at(0)=data_num_ev;
      selecthit.at(1)=bmgeo->GetBMNcell(bmntuhit->Plane(), bmntuhit->View(),bmntuhit->Cell());
      selecthit.at(2)=bmntuhit->Tdrift();
      selecthit.at(3)=bmntuhit->Dist();
      selecthit.at(4)=bmntuhit->GetResidual();
      residual_distance.push_back(selecthit);
    }else if(isdata){      
      rejhit.at(0)=data_num_ev;
      rejhit.at(1)=bmgeo->GetBMNcell(bmntuhit->Plane(), bmntuhit->View(),bmntuhit->Cell());
      rejhit.at(2)=bmntuhit->Tdrift();
      rejhit.at(3)=bmntuhit->Dist();
      residual_distance.push_back(rejhit);
    }          
  }
  
return;  
}

void BmBooter::fit_histos(){
  
  if(((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))==nullptr)
    return;
  ((TDirectory*)(m_controlPlotter->GetTFile()->Get("BM_output")))->cd();    

//~ ((TH2D*)(m_controlPlotter->GetTFile()->Get("BM_output/BM_output__raw_occupancy")))
  
  TF1 f1("f1","TMath::Prob(x,[0])/[0]",0.,10.);
  f1.SetParameter(0,4);
  ((TH1D*)(m_controlPlotter->GetTFile()->Get("BM_output/BM_output__tracksel_chi2red")))->Fit("f1", "Q");
  
  
  return;
}


Bool_t BmBooter::autocalibstrel_readfile(){
  //read the shiftsfile
  Int_t tmp_int;
  Double_t tmp_double;
  char tmp_char[200];
  ifstream infile;
  TString name="./config/"+bmcon->GetShiftsfile();
  infile.open(name.Data(),ios::in);
  vector<Double_t>parin(6,0);
  vector<Double_t>resoin(11,0);
  stnite=1;
  if(infile.is_open()){
    while(!infile.eof()){
      if(!(infile>>tmp_char))
        break;
      infile>>stnite>>tmp_char>>tmp_char;
      stnite++;
      //~ if(tmp_char!=m_nopath_instr){
        //~ cout<<"BmBooter::autocalibstrel: wrong file!! file analyzed now="<<m_nopath_instr<<"  file from "<<bmcon->GetShiftsfile()<<"="<<tmp_char<<endl<<"The program will be finished"<<endl;
        //~ infile.close();
        //~ return;
      //~ }
      bmcon->SetAutostrel(stnite);
      infile>>tmp_char>>tmp_int; 
      if(tmp_int!=bmcon->GetT0switch()){
        cout<<"BmBooter::autocalibstrel_readfile: wrong T0switch!! now t0switch="<<bmcon->GetT0switch()<<"  t0switch form the file="<<tmp_int<<endl<<"The program will be finished"<<endl;
        infile.close();
        return kTRUE;
      }
      infile>>tmp_char>>tmp_double; 
      if(fabs(tmp_double-bmcon->GetT0sigma())>1){
        cout<<"BmBooter::autocalibstrel_readfile: wrong T0sigma!! now t0sigma="<<bmcon->GetT0sigma()<<"  t0sigma form the file="<<tmp_double<<endl<<"The program will be finished"<<endl;
        infile.close();
        return kTRUE;
      }
      infile>>tmp_char>>tmp_double; 
      if(fabs(tmp_double-bmcon->GetHitTimecut())>1){
        cout<<"BmBooter::autocalibstrel_readfile: wrong T0timecut!! now hit_timecut="<<bmcon->GetHitTimecut()<<"  hit_timecut form the file="<<tmp_double<<endl<<"The program will be finished"<<endl;
        infile.close();
        return kTRUE;
      }
      infile>>tmp_char>>tmp_int; 
      if(tmp_int!=bmcon->GetResnbin()){
        cout<<"BmBooter::autocalibstrel_readfile: wrong resnbin!! now resnbin="<<bmcon->GetResnbin()<<"  resnbin form the file="<<tmp_int<<endl<<"The program will be finished"<<endl;
        infile.close();
        return kTRUE;
      }
      for(Int_t i=0; i<bmcon->GetResnbin();i++)
        infile>>tmp_char>>tmp_char>>tmp_char>>tmp_char>>tmp_char>>tmp_char;
      infile>>tmp_char;
      for(Int_t i=0;i<6;i++){
        infile>>tmp_double;
        parin.at(i)=tmp_double;
        if(tmp_double==-999){
          cout<<"BmBooter::autocalibstrel_readfile: wrong strel parameter!! parin.at("<<i<<")=-999, something wrong happend"<<endl;
          return kTRUE;
        }          
      }
      bmcon->AddStrelparameters(parin);
      infile>>tmp_char;
      for(Int_t i=0;i<11;i++){
        infile>>tmp_double;
        resoin.at(i)=tmp_double;
      }
      infile>>tmp_char>>tmp_double;
      bmcon->SetMeanTimeReso(tmp_double);
    }
    infile.close();
    TF1 readrespoly("readrespoly","pol10", 0.,bmcon->GetHitTimecut());
    for(Int_t i=0;i<resoin.size();i++)
      readrespoly.SetParameter(i,resoin.at(i));
    bmcon->SetTimeReso(readrespoly);  
  }else
    cout<<"BmBooter::autocalibstrel_readfile::FIRST ITERATION, file does not exist yet"<<endl;
  
 
 return kFALSE; 
}


void BmBooter::autocalibstrel_writefile(){
  ofstream outfile;
  TString name="./config/"+bmcon->GetShiftsfile();
  outfile.open(name.Data(),ios::app);
  outfile<<"Number_of_iteration= "<<stnite<<"  input_file= "<<m_nopath_instr<<"  t0_switch= "<<bmcon->GetT0switch()<<" t0sigma= "<<bmcon->GetT0sigma()<<" hit_timecut= "<<bmcon->GetHitTimecut()<<"  resnbin= "<<bmcon->GetResnbin()<<endl;
  for(Int_t i=0;i<bmcon->GetResnbin();i++){
    outfile<<"mean= "<<strelresiduals.at(i).at(0)<<"   sigma= "<<strelresiduals.at(i).at(1)<<" number_of_hits= "<<strelresiduals.at(i).at(2)<<endl;
  }
  outfile<<"New_strel_parameters:"<<endl;
  for(Int_t i=0;i<6;i++){
    if(bmcon->GetLastStrelpar(i)!=-999)
      outfile<<bmcon->GetLastStrelpar(i)<<"  ";
    else
      cout<<"ERROR in BmBooter::autocalibstrel_writefile:: bmcon->GetLastStrelpar not setted!! i="<<i<<"  bmcon->GetLastStrelpar(i)="<<bmcon->GetLastStrelpar(i)<<endl;
  }
  outfile<<endl;
  outfile<<"New_timereso_parameters:"<<endl;
  if(bmcon->GetTimeResoNumPar()!=11)
    cout<<"ERROR in BmBooter::autocalibstrel_writefile:: bmcon->GetTimeResoNumPar()!=11!!!  bmcon->GetTimeResoNumPar()="<<bmcon->GetTimeResoNumPar()<<endl;
  for(Int_t i=0;i<11;i++){
    if(i<bmcon->GetTimeResoNumPar())
      outfile<<bmcon->GetTimeResoPar(i)<<"  ";
    else
      outfile<<"  0  ";
  }
  outfile<<endl;
  outfile<<"Mean_time_reso_value= "<<bmcon->GetMeanTimeReso()<<endl;
  outfile.close(); 
return;
}



void BmBooter::PrintBMstruct(){
  cout<<"PrintBMstruct:"<<endl;
  cout<<"bmstruct.evnum="<<bmstruct.evnum<<endl;
  cout<<"bmstruct.words="<<bmstruct.words<<endl;
  cout<<"bmstruct.tdcev="<<bmstruct.tdcev<<endl;
  cout<<"bmstruct.tdc_numsync="<<bmstruct.tdc_numsync<<endl;
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
  bmstruct.tdc_numsync=0;
  bmstruct.adc792_sum=0;
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
  //~ tmp_int=0;
  //~ while(bmstruct.sca830_meas[tmp_int]!=-10000){
    //~ bmstruct.sca830_meas[tmp_int]=-10000;
    //~ tmp_int++;
  //~ }
  //~ tmp_int=0;
  //~ while(bmstruct.adc792_meas[tmp_int]!=-10000){
    //~ bmstruct.adc792_meas[tmp_int]=-10000;
    //~ tmp_int++;
  //~ }
  //~ tmp_int=0;
  //~ while(bmstruct.adc792_over[tmp_int]!=-10000){
    //~ bmstruct.adc792_over[tmp_int]=-10000;
    //~ tmp_int++;
  //~ }
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


Int_t BmBooter::GetNentries(Int_t m_nev){
  cout<<"Total number of Beam Monitor events="<<tot_num_ev<<endl;
  return (m_nev==0) ? tot_num_ev-acq_start_ev+1 : min((Long64_t)m_nev, tot_num_ev);
}



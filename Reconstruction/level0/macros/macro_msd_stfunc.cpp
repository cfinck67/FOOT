
//*********************************************  combined functions  *****************************************
void BookingBMVTX(TFile* f_out, bool onlyVTX, bool merging){
  
  if(debug>0)
    cout<<"I'm in Booking BMVTX"<<endl;  

  f_out->cd();
  TH1D *h; 
  TH2D *h2;
  char tmp_char[200];
  
  //vtx stuff
  if(!merging){
    h = new TH1D("vtx_xslope_coeff","VTX xz m for corrected positions;m;Events",1000,-0.5,0.5);
    h = new TH1D("vtx_yslope_coeff","VTX yz m coeff for corrected positions;m;Events",1000,-0.5,0.5);
    h = new TH1D("vtx_xR0_coeff","VTX xz R0 for corrected positions;R0[cm];Events",1000, -5., 5.);
    h = new TH1D("vtx_yR0_coeff","VTX yz R0 for corrected positions;R0[cm];Events",1000, -5., 5.);
    h = new TH1D("vtx_polar_angle","vtx polar angle distribution ;AngZ(deg);Events",400,0.,10.);
    h = new TH1D("vtx_azimuth_angle","vtx azimuth angle distribution ;Phi(deg);Events",360,-180.,180.);
    h2 = new TH2D("vtx_isoplane_vtxsys","VTX tracks projection on ISO plane in VTX sys;X[cm];Y[cm]",1000, -5., 5., 1000, -5., 5.);
    h2 = new TH2D("vtx_isoplane_isosys","VTX tracks projection on ISO plane in ISO sys;X[cm];Y[cm]",1000, -5., 5., 1000, -5., 5.);
    h2 = new TH2D("vtx_isoplane_bmsys","VTX tracks projection on ISO plane in BM sys;X[cm];Y[cm]",1000, -5., 5., 1000, -5., 5.);
    
    if(onlyVTX){
      f_out->cd("..");
      return;
    }
  
    //bm stuff
    h = new TH1D("bmnhitsxevent","number of hits x event ;N of hits;Events",36,0.,36.);
    h = new TH1D("bm_hits_chi2","BM polar angle distribution ;AngZ(deg);Events",100,0.,10.);
    h = new TH1D("bm_polar_angle","BM polar angle distribution ;AngZ(deg);Events",400,0.,10.);
    h = new TH1D("bm_azimuth_angle","BM azimuth angle distribution ;Phi(deg);Events",360,-180.,180.);
    h = new TH1D("bm_xslope_coeff","BM m_x coeff ;mx;Events",1000,-0.5,0.5);
    h = new TH1D("bm_yslope_coeff","BM m_y coeff ;my;Events",1000,-0.5,0.5);
    h = new TH1D("bm_xR0_coeff","BM R0_X for corrected positions;R0_X[cm];Events",1000, -5., 5.);
    h = new TH1D("bm_yR0_coeff","BM R0_Y for corrected positions;R0_Y[cm];Events",1000, -5., 5.);    
    h2 = new TH2D("bmisoproISOsys","BM tracks on isocenter  projections in ISO sys ;X[cm];Y[cm]",1000, -5., 5., 1000, -5., 5.);
    h2 = new TH2D("bmisoproBMsys","BM tracks on isocenter projections in BM sys;X[cm];Y[cm]",1000, -5., 5., 1000, -5., 5.);
    h2 = new TH2D("bmmylar1BMsys","bm tracks on mylar1 projections in BM sys;X[cm];Y[cm]",1000, -5., 5., 1000, -5., 5.);
    h2 = new TH2D("bmmylar2BMsys","bm tracks on mylar2 projections in BM sys;X[cm];Y[cm]",1000, -5., 5., 1000, -5., 5.);
    if(bmmcstudy==0)
      h = new TH1D("bm_realrdrift","real bm rdrift;rdrift[cm];Events",1000,0.,1.);    
  
    //combined stuff not for merging
    h = new TH1D("track_iso_diff_total","Difference for bm tracks and vtx tracks on isocenter plane;diff[cm];Events",500,0.,5.);
    h = new TH1D("track_iso_diff_x","Difference for bm tracks and vtx tracks on isocenter plane;x diff[cm];Events",500,-5.,5.);
    h = new TH1D("track_iso_diff_y","Difference for bm tracks and vtx tracks on isocenter plane;y diff[cm];Events",500,-5.,5.);
    h = new TH1D("chi2_msd_combined","chi2 of the msd combined tracks;chi2;events",100,0.,20.);
    h = new TH1D("chi2_bm_combined","chi2 of the BM combined tracks;chi2;events",100,0.,20.);
    h = new TH1D("chi2_hit_bm_combined","chi2 of the BM combined hits;chi2;hit",100,0.,20.);
    h2 = new TH2D("comb_bmisoproISO","Combined BM tracks on isocenter projections in ISO sys;X[cm];Y[cm]",1000, -5., 5.,1000, -5., 5.);
    h2 = new TH2D("comb_vtxisoproISO","Combined MSD tracks on isocenter projections in ISO sys;X[cm];Y[cm]",1000, -5., 5.,1000, -5., 5.);
  }    
  
  //combined stuff also for merging
  h = new TH1D("residual_error","Events with msd fitted rdrift > 0.945 ;Measured rdrift;Events",400.,0.,1.);
  //~ h = new TH1D("time_diff","bm event timeacq - msd event timeacq ;time;Events",500,0.,100.);
  h = new TH1D("pvers_diff_vs_xevent_mx","mx diff bm_track-vtx_track;Event;bm_mx-vtx_mx",100000,0.,100000.);
  h = new TH1D("pvers_diff_vs_xevent_my","my diff bm_track-vtx_track;Event;bm_my-vtx_my",100000,0.,100000.);
  h = new TH1D("r0x_diff_vs_xevent","r0x diff bm_track-vtx_track;Event;bm_mx-vtx_mx",100000,0.,100000.);
  h = new TH1D("r0y_diff_vs_xevent","r0y diff bm_track-vtx_track;Event;bm_my-vtx_my",100000,0.,100000.);
  h2 = new TH2D("pvers_mx_bmvtx","pvers mx angle for bm and vertex for selected tracks;BM Pvers mx;vtx Pverse mx ",500,-0.05,0.05,500,-0.05,0.05);
  h2 = new TH2D("pvers_my_bmvtx","pvers my angle for bm and vertex for selected tracks;BM Pvers mx;vtx Pverse my ",500,-0.05,0.05,500,-0.05,0.05);
  h2 = new TH2D("r0x_bmvtx","R0x for bm and vtx tracks correlation;BM r0x;vtx r0x",600,-3.,3.,600,-3.,3.);
  h2 = new TH2D("r0y_bmvtx","R0y for bm and vtx tracks correlation;BM r0y;vtx r0y",600,-3.,3.,600,-3.,3.);
  h = new TH1D("pvers_angle_diff","angle difference bm_track - vtx_track ;Angle(bm_track-vtx_track);Events",2000,0.,0.2);
  h = new TH1D("residual_space_numev","number of events for each bin in strel residual graph;bin;events",STBIN,0.,STBIN);
  h = new TH1D("residual_space_total_xhit","total space residual;x[cm];events",200,-1.,1.);
  h = new TH1D("residual_time_total_xhit","total time residual;x[cm];events",200,-1.,1.);
  h = new TH1D("residual_total_xtrack","Residual contribution x combined track;res[cm];events",400,-2.,2.);
  h2 = new TH2D("residual_vs_rdrift","residual vs measured rdrift;Residual [cm];BM hit rdrift [cm]",400,-2.,2.,200,0.,1.);
  h2 = new TH2D("msdresidual_vs_bmfitresidual","residual vs measured rdrift;Msd residual [cm];BM fitted residual [cm]",200,-0.5,0.5,200,-0.5,0.5);
  
  h2 = new TH2D("old_strel_data","old FIRST strel from data ;time;rdrift",400,0.,400.,1000, 0., 1.);
  h2 = new TH2D("new_strel_rdrift","new strel ;time;rdrift",300,0.,400., 1000, 0., 1.);
  h2 = new TH2D("new_strel_time","new strel ;rdrift;time", 1000, 0., 1., 400,0.,400.);
  h2 = new TH2D("residual_strel_time","strel residuals of fitted_timestrel-MC teorical strel ;rdrift;time", 400,0.,400., 1000, -0.5, 0.5);
  h2 = new TH2D("residual_strel_rdrift","strel residuals of fitted_rdriftstrel-MC teorical strel ;rdrift;time",400,0.,400., 1000, -0.5, 0.5);
  if(bmmcstudy==0)
    h2 = new TH2D("strel_realrdrift","old FIRST strel from realrdrift ;time;rdrift",400,0.,400.,1000, 0., 1.);
  
  f_out->mkdir("Res_vs_rdrift");
  f_out->cd("Res_vs_rdrift");
  for(int i=0;i<STBIN;i++){
    sprintf(tmp_char,"strel_rdrift_%d", i);
    h = new TH1D(tmp_char,"strel residual ;Residual[cm];Events",600,-1.,1.);
  }
  f_out->cd("..");
  
  f_out->mkdir("Res_vs_tdrift");
  f_out->cd("Res_vs_tdrift");
  for(int i=0;i<STBIN;i++){
    sprintf(tmp_char,"strel_tdrift_%d", i);
    h = new TH1D(tmp_char,"strel residual ;Residual[cm];Events",600,-1.,1.);
  }
  
  f_out->cd("..");
  
  if(debug>0)
    cout<<"Booking finished"<<endl;  
  
  return;
}


void Printoutput(TFile* f_out, vector<BM_evstruct> &allbmeventin, vector<vtx_evstruct> &allvtxeventin, vector<vector<double>> &space_residual, vector<vector<double>> &time_residual, vector<vector<int>> &selected_index, bool onlyVTX){
  
  if(debug)
    cout<<"I'm in Printoutput:: allbmeventin.size():"<<allbmeventin.size()<<"  allvtxeventin.size()="<<allvtxeventin.size()<<endl;
    
  TVector3 tmp_tvector3, tmp2_tvector3;
  char tmp_char[200];
  double tmp_double;

  //oldstrel_stuff
  //~ TCanvas *mcstrel=new TCanvas("mcstrel","mcstrel",800,800);
  //~ mcstrel->cd();
  TF1* first_strel_tf1_1=new TF1("first1_strel_green","1./0.78*(0.0075746330*x-(5.1692440e-05)*x*x+(1.8928600e-07)*x*x*x-(2.4652420e-10)*x*x*x*x)", 0., 300.);  
  TF1* first_strel_tf1_08=new TF1("first08_strel_blue","0.8/0.78*(0.032891770+0.0075746330*x-(5.1692440e-05)*x*x+(1.8928600e-07)*x*x*x-(2.4652420e-10)*x*x*x*x)", 0., 300.);  
  TF1* garfield_strel_tf1=new TF1("garfield_strel_yellow","0.00915267+0.00634507*x+2.02527e-05*x*x-7.60133e-07*x*x*x+5.55868e-09*x*x*x*x-1.68944e-11*x*x*x*x*x+1.87124e-14*x*x*x*x*x*x", 0., 300.);  
  first_strel_tf1_1->SetLineColor(3);
  first_strel_tf1_1->Write();
  first_strel_tf1_08->SetLineColor(4);
  first_strel_tf1_08->Write();
  garfield_strel_tf1->SetLineColor(41);
  garfield_strel_tf1->Write();

  //VTX events
  for(int i=0; i<allvtxeventin.size();i++){
    if(allvtxeventin.at(i).status==1){
      ((TH1D*)gDirectory->Get("vtx_xslope_coeff"))->Fill(allvtxeventin.at(i).vtx_track_pvers.X()/allvtxeventin.at(i).vtx_track_pvers.Z());
      ((TH1D*)gDirectory->Get("vtx_yslope_coeff"))->Fill(allvtxeventin.at(i).vtx_track_pvers.Y()/allvtxeventin.at(i).vtx_track_pvers.Z());
      ((TH1D*)gDirectory->Get("vtx_xR0_coeff"))->Fill(allvtxeventin.at(i).vtx_track_r0pos.X());
      ((TH1D*)gDirectory->Get("vtx_yR0_coeff"))->Fill(allvtxeventin.at(i).vtx_track_r0pos.Y());
      ((TH1D*)gDirectory->Get("vtx_polar_angle"))->Fill(allvtxeventin.at(i).vtx_track_pvers.Theta()*RAD2DEG);
      ((TH1D*)gDirectory->Get("vtx_azimuth_angle"))->Fill(allvtxeventin.at(i).vtx_track_pvers.Phi()*RAD2DEG);
      ((TH2D*)gDirectory->Get("vtx_isoplane_vtxsys"))->Fill(allvtxeventin.at(i).vtx_track_r0pos.X(), allvtxeventin.at(i).vtx_track_r0pos.Y());
      tmp_tvector3=ExtrapolateZ(allvtxeventin.at(i).vtx_track_pvers, allvtxeventin.at(i).vtx_track_r0pos, -VTXISOZ, true, false);
      ((TH2D*)gDirectory->Get("vtx_isoplane_isosys"))->Fill(tmp_tvector3.X(),tmp_tvector3.Y());
      vtxpversr02global(allvtxeventin.at(i).vtx_track_pvers,allvtxeventin.at(i).vtx_track_r0pos, tmp_tvector3, tmp2_tvector3);
      ((TH2D*)gDirectory->Get("vtx_isoplane_bmsys"))->Fill(tmp2_tvector3.X(),tmp2_tvector3.Y());
    }
  }
  
  if(debug)
    cout<<"I'm in Printoutput:: allvtxeventin finished  allvtxeventin.size()="<<allvtxeventin.size()<<endl;
  
  if(onlyVTX)
    return;
  
  //BM events
  for(int i=0; i<allbmeventin.size();i++){
    ((TH1D*)gDirectory->Get("bmnhitsxevent"))->Fill(allbmeventin.at(i).hitnum);
    ((TH1D*)gDirectory->Get("bm_polar_angle"))->Fill(allbmeventin.at(i).bm_track_pvers.Theta()*RAD2DEG);
    ((TH1D*)gDirectory->Get("bm_azimuth_angle"))->Fill(allbmeventin.at(i).bm_track_pvers.Phi()*RAD2DEG);
    ((TH1D*)gDirectory->Get("bm_xslope_coeff"))->Fill(allbmeventin.at(i).bm_track_pvers.X()/allbmeventin.at(i).bm_track_pvers.Z());
    ((TH1D*)gDirectory->Get("bm_yslope_coeff"))->Fill(allbmeventin.at(i).bm_track_pvers.Y()/allbmeventin.at(i).bm_track_pvers.Z());
    ((TH1D*)gDirectory->Get("bm_xR0_coeff"))->Fill(allbmeventin.at(i).bm_track_r0pos.X());
    ((TH1D*)gDirectory->Get("bm_yR0_coeff"))->Fill(allbmeventin.at(i).bm_track_r0pos.Y());
    tmp_tvector3=ExtrapolateZ(allbmeventin.at(i).bm_track_pvers, allbmeventin.at(i).bm_track_r0pos, -BMISOZ, true, true);
    ((TH2D*)gDirectory->Get("bmisoproISOsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    tmp_tvector3=ExtrapolateZ(allbmeventin.at(i).bm_track_pvers, allbmeventin.at(i).bm_track_r0pos, -BMISOZ, false, true);
    ((TH2D*)gDirectory->Get("bmisoproBMsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());//dovrebbe essere = a bmisoproISOsys
     tmp_tvector3=ExtrapolateZ(allbmeventin.at(i).bm_track_pvers, allbmeventin.at(i).bm_track_r0pos, BMN_MYLAR1Z, false, true);
    ((TH2D*)gDirectory->Get("bmmylar1BMsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    tmp_tvector3=ExtrapolateZ(allbmeventin.at(i).bm_track_pvers, allbmeventin.at(i).bm_track_r0pos, BMN_MYLAR2Z, false, true);
    ((TH2D*)gDirectory->Get("bmmylar2BMsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    
    for(int k=0;k<allbmeventin.at(i).hitnum;k++){
      ((TH2D*)gDirectory->Get("old_strel_data"))->Fill(allbmeventin.at(i).bm_hit_time[k],allbmeventin.at(i).bm_hit_rdrift[k]);
      ((TH1D*)gDirectory->Get("bm_hits_chi2"))->Fill(allbmeventin.at(i).bm_hit_chi2[k]);
      if(bmmcstudy==0){
        ((TH1D*)gDirectory->Get("bm_realrdrift"))->Fill(allbmeventin.at(i).bm_hit_realrdrift[k]);
        ((TH2D*)gDirectory->Get("strel_realrdrift"))->Fill(allbmeventin.at(i).bm_hit_time[k],allbmeventin.at(i).bm_hit_realrdrift[k]);
      }
    }
  }
  
  if(debug)
    cout<<"I'm in Printoutput:: allbmeventin finished allbmeventin.size()="<<allbmeventin.size()<<endl;    
  
  //~ gDirectory->ls();
  //~ cout<<"ora faccio crashasre root cercando grafico che non esiste"<<endl;
  //~ ((TH1D*)gDirectory->Get("fdsjaklflkasjdlk"))->Fill(3.);
  
  //combined
  TVector3 bmproject, vtxproject;
  for(int i=0;i<selected_index.size();i++){
    //~ cout<<"i="<<i<<"  selected_index.size()="<<selected_index.size()<<"  selected_index.at(i).size()="<<selected_index.at(i).size()<<"  selected_index.at(i).at(2)="<<selected_index.at(i).at(2)<<endl;
    if(selected_index.at(i).at(2)==0){//both bm and vtx track
      ((TH2D*)gDirectory->Get("pvers_mx_bmvtx"))->Fill(allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.X()/allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.Z(), allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_pvers.X()/allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_pvers.Z());        
      ((TH2D*)gDirectory->Get("pvers_my_bmvtx"))->Fill(allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.Y()/allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.Z(), allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_pvers.Y()/allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_pvers.Z());   
      ((TH2D*)gDirectory->Get("r0x_bmvtx"))->Fill(allbmeventin.at(selected_index.at(i).at(0)).bm_track_r0pos.X(), allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_r0pos.X());        
      ((TH2D*)gDirectory->Get("r0y_bmvtx"))->Fill(allbmeventin.at(selected_index.at(i).at(0)).bm_track_r0pos.Y(), allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_r0pos.Y());        
      ((TH1D*)gDirectory->Get("pvers_diff_vs_xevent_mx"))->SetBinContent(allbmeventin.at(selected_index.at(i).at(0)).evnum , (allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.X()/allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.Z()) - (allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_pvers.X()/allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_pvers.Z()));        
      ((TH1D*)gDirectory->Get("pvers_diff_vs_xevent_my"))->SetBinContent(allbmeventin.at(selected_index.at(i).at(0)).evnum , (allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.Y()/allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.Z()) - (allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_pvers.Y()/allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_pvers.Z()));
      ((TH1D*)gDirectory->Get("r0x_diff_vs_xevent"))->SetBinContent(allbmeventin.at(selected_index.at(i).at(0)).evnum , allbmeventin.at(selected_index.at(i).at(0)).bm_track_r0pos.X() - allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_r0pos.X());        
      ((TH1D*)gDirectory->Get("r0y_diff_vs_xevent"))->SetBinContent(allbmeventin.at(selected_index.at(i).at(0)).evnum , allbmeventin.at(selected_index.at(i).at(0)).bm_track_r0pos.Y() - allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_r0pos.Y());        
      ((TH1D*)gDirectory->Get("pvers_angle_diff"))->Fill(allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.Angle(allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_pvers));        
      ((TH1D*)gDirectory->Get("chi2_msd_combined"))->Fill(allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_chi2tot);        
      ((TH1D*)gDirectory->Get("chi2_bm_combined"))->Fill(allbmeventin.at(selected_index.at(i).at(0)).bm_track_chi2);        
      for(int k=0;k<allbmeventin.at(i).hitnum;k++)
        if(allbmeventin.at(i).bm_hit_chi2[i]<=BMNHITCUT)
          ((TH1D*)gDirectory->Get("chi2_hit_bm_combined"))->Fill(allbmeventin.at(i).bm_hit_chi2[k]);        
      
      bmproject=ExtrapolateZ(allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers, allbmeventin.at(selected_index.at(i).at(0)).bm_track_r0pos, -BMISOZ,true, true);
      vtxproject=ExtrapolateZ(allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_pvers, allvtxeventin.at(selected_index.at(i).at(1)).vtx_track_r0pos, -VTXISOZ,true, false);
      if(bmproject.Z()!=vtxproject.Z())
        cout<<"ERROR on bm/msd iso projection::bmproject.Z()="<<bmproject.Z()<<"   vtxproject.Z()="<<vtxproject.Z()<<endl;
      else{
        ((TH1D*)gDirectory->Get("track_iso_diff_y"))->Fill((bmproject-vtxproject).Y());    
        ((TH1D*)gDirectory->Get("track_iso_diff_x"))->Fill((bmproject-vtxproject).X());    
        ((TH1D*)gDirectory->Get("track_iso_diff_total"))->Fill((bmproject-vtxproject).Mag());
        ((TH2D*)gDirectory->Get("comb_bmisoproISO"))->Fill(bmproject.X(), bmproject.Y());
        ((TH2D*)gDirectory->Get("comb_vtxisoproISO"))->Fill(vtxproject.X(), vtxproject.Y());
      }    
    }
  }
  
  cout<<"calcolo correlation factor"<<endl;
  cout<<"r0x_bmvtx correlation factor from root: "<<((TH2D*)gDirectory->Get("r0x_bmvtx"))->GetCorrelationFactor()<<endl;
  cout<<"r0y_bmvtx correlation factor from root: "<<((TH2D*)gDirectory->Get("r0y_bmvtx"))->GetCorrelationFactor()<<endl;

  if(debug)
    cout<<"I'm in Printoutput:: combined graph finished"<<endl;

  //old strel
  fitStrel(f_out, 0,first_strel_tf1_1, first_strel_tf1_08, garfield_strel_tf1);

  if(((TH2D*)gDirectory->Get("r0x_bmvtx"))->GetCorrelationFactor()>CORRMINIMUM || ((TH2D*)gDirectory->Get("r0y_bmvtx"))->GetCorrelationFactor()>CORRMINIMUM){
    
    // gaussian fit on the slice of rdrift
    TF1  *strelgaus = new TF1("strelgaus","gaus",-1.,1.);  
    strelgaus->SetParameter(1,0.);
    vector<double> newstrel_spaceresidual;
    vector<double> newstrel_timeresidual;
    
    //try to estimate the new strel with space_residual
    for(int i=0; i<STBIN;i++){
      ((TH1D*)gDirectory->Get("residual_space_numev"))->AddBinContent(i+1,space_residual.at(i).size());
      sprintf(tmp_char,"Res_vs_rdrift/strel_rdrift_%d", i);
      for(int k=0; k<space_residual.at(i).size();k++){
        ((TH1D*)gDirectory->Get(tmp_char))->Fill(space_residual.at(i).at(k));
        ((TH1D*)gDirectory->Get("residual_space_total_xhit"))->Fill(space_residual.at(i).at(k));
      }
      if(((TH1D*)gDirectory->Get(tmp_char))->GetEntries()>100){
        if(GAUSMEAN==0){
          ((TH1D*)gDirectory->Get(tmp_char))->Fit("strelgaus","Q+","",-MAXSTRELRES,MAXSTRELRES);
          newstrel_spaceresidual.push_back(strelgaus->GetParameter(1));
        }else
          newstrel_spaceresidual.push_back(((TH1D*)gDirectory->Get(tmp_char))->GetMean());
      }else
        newstrel_spaceresidual.push_back(0.);
    }
    
    
    int binpos;
    for(int i=0; i<allbmeventin.size();i++){
      for(int k=0;k<allbmeventin.at(i).hitnum;k++){
        binpos=(int)(allbmeventin.at(i).bm_hit_rdrift[k]*STBIN);
        if(binpos<STBIN && binpos>=0)
          ((TH2D*)gDirectory->Get("new_strel_rdrift"))->Fill(allbmeventin.at(i).bm_hit_time[k],allbmeventin.at(i).bm_hit_rdrift[k]+newstrel_spaceresidual.at(binpos));
      }
    }
    //fit the new_strel
    fitStrel(f_out,1,first_strel_tf1_1, first_strel_tf1_08, garfield_strel_tf1);
    
    if(debug)
      cout<<"I'm in Printoutput:: estimate of the strel with the position residual finished"<<endl;  
    
  //try to estimate the new strel with TIME_residual
    for(int i=0; i<STBIN;i++){
        sprintf(tmp_char,"Res_vs_tdrift/strel_tdrift_%d", i);
      for(int k=0; k<time_residual.at(i).size();k++){
        //~ cout<<"ora riempio in "<<tmp_char<<"    il valore: "<<time_residual.at(i).at(k)<<"   time_residual.size()="<<time_residual.size()<<endl;
        ((TH1D*)gDirectory->Get(tmp_char))->Fill(time_residual.at(i).at(k));
        ((TH1D*)gDirectory->Get("residual_time_total_xhit"))->Fill(time_residual.at(i).at(k));
      }
      if(((TH1D*)gDirectory->Get(tmp_char))->GetEntries()>100){
        if(GAUSMEAN==0){
          ((TH1D*)gDirectory->Get(tmp_char))->Fit("strelgaus","Q+","",-MAXSTRELRES,MAXSTRELRES);
          newstrel_timeresidual.push_back(strelgaus->GetParameter(1));
        }else
          newstrel_timeresidual.push_back(((TH1D*)gDirectory->Get(tmp_char))->GetMean());
      }else
        newstrel_timeresidual.push_back(0.);
    }
    
    for(int i=0; i<allbmeventin.size();i++){
      for(int k=0;k<allbmeventin.at(i).hitnum;k++){
        binpos=(int)(allbmeventin.at(i).bm_hit_time[k]*STBIN/300.);
        if(binpos<STBIN && binpos>=0){
          sprintf(tmp_char,"Res_vs_tdrift/strel_tdrift_%d", binpos);
          ((TH2D*)gDirectory->Get("new_strel_time"))->Fill(allbmeventin.at(i).bm_hit_rdrift[k]+ ((newstrel_timeresidual.at(binpos)<MAXSTRELRES && newstrel_timeresidual.at(binpos)>-MAXSTRELRES) ? newstrel_timeresidual.at(binpos) : 0.), allbmeventin.at(i).bm_hit_time[k]);
        }    
      }
    }
    //fit the new_strel
    fitStrel(f_out, 2,first_strel_tf1_1, first_strel_tf1_08, garfield_strel_tf1);
    
    if(debug)
      cout<<"I'm in Printoutput:: estimate of the strel with the time residual finished, Printout finished"<<endl;  
  
  }else
    cout<<endl<<"Printoutput: THE DATA ARE NOT CORRELATED ENOUGH, the program will be finished"<<endl<<endl;
         
    return;  
}


void fitStrel(TFile *f_out, const int index, TF1* first_strel_tf1_1, TF1* first_strel_tf1_08 ,TF1* garfield_strel_tf1){

    //try to change the parameters
  TF1 poly ("poly","pol10", 0, 350);
  //try to fix: time=0-->rdrift=0  and time=300-->rdrift=0.8
  //~ TF1 poly ("poly","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+ (0.8-300.*[1]-90000.*[2]-27000000.*[3]-8100000000.*[4])/2430000000000.*x*x*x*x*x", 0., 325.);
  poly.FixParameter(0,0.);
  
  TProfile *prof_newstrel;
  if(index==0) {
    prof_newstrel=((TH2D*)gDirectory->Get("old_strel_data"))->ProfileX(); 
  }else if(index==1){ 
    prof_newstrel=((TH2D*)gDirectory->Get("new_strel_rdrift"))->ProfileX(); 
  }else if(index==2){
    prof_newstrel=((TH2D*)gDirectory->Get("new_strel_time"))->ProfileY(); 
  }
  prof_newstrel->SetLineColor(3);
  prof_newstrel->Draw();
  
  prof_newstrel->Fit("poly","QRB+");
  
  if(index==0){
    cout<<"fit the old strel parameters"<<endl;
  }else if(index==1){
    cout<<endl<<"new strel parameters from space residuals"<<endl;
    //oldstrel_stuff
    //~ first_strel_tf1.Write();
    //~ garfield_strel_tf1.Write();
    poly.SetName("fitted_rdrift_strel");
    poly.Write();
    if(MCSTREL==8){
      for(int i=1;i<((TH2D*)gDirectory->Get("residual_strel_rdrift"))->GetNbinsX();i++)
        ((TH2D*)gDirectory->Get("residual_strel_rdrift"))->Fill( ((TH1D*)gDirectory->Get("residual_strel_rdrift"))->GetXaxis()->GetBinCenter(i) , poly.Eval((double)i)- first_strel_tf1_08->Eval((double)i));
    }else if(MCSTREL==1){
      for(int i=1;i<((TH2D*)gDirectory->Get("residual_strel_rdrift"))->GetNbinsX();i++)
        ((TH2D*)gDirectory->Get("residual_strel_rdrift"))->Fill( ((TH1D*)gDirectory->Get("residual_strel_rdrift"))->GetXaxis()->GetBinCenter(i) , poly.Eval((double)i)- first_strel_tf1_1->Eval((double)i));      
    }else{
      for(int i=1;i<((TH2D*)gDirectory->Get("residual_strel_rdrift"))->GetNbinsX();i++)
        ((TH2D*)gDirectory->Get("residual_strel_rdrift"))->Fill( ((TH1D*)gDirectory->Get("residual_strel_rdrift"))->GetXaxis()->GetBinCenter(i) , poly.Eval((double)i)- garfield_strel_tf1->Eval((double)i));     
      }   
  }else if(index==2){
    cout<<endl<<"new strel parameters from time residuals"<<endl;
    poly.SetName("fitted_time_strel");
    poly.Write();
    if(MCSTREL==8){
      for(int i=1;i<((TH2D*)gDirectory->Get("residual_strel_time"))->GetNbinsX();i++)
        ((TH2D*)gDirectory->Get("residual_strel_time"))->Fill( ((TH1D*)gDirectory->Get("residual_strel_time"))->GetXaxis()->GetBinCenter(i) , poly.Eval((double)i)- first_strel_tf1_08->Eval((double)i));
    }else if(MCSTREL==1){
      for(int i=1;i<((TH2D*)gDirectory->Get("residual_strel_time"))->GetNbinsX();i++)
        ((TH2D*)gDirectory->Get("residual_strel_time"))->Fill( ((TH1D*)gDirectory->Get("residual_strel_time"))->GetXaxis()->GetBinCenter(i) , poly.Eval((double)i)- first_strel_tf1_1->Eval((double)i));      
    }else{
      for(int i=1;i<((TH2D*)gDirectory->Get("residual_strel_time"))->GetNbinsX();i++)
        ((TH2D*)gDirectory->Get("residual_strel_time"))->Fill( ((TH1D*)gDirectory->Get("residual_strel_time"))->GetXaxis()->GetBinCenter(i) , poly.Eval((double)i)- garfield_strel_tf1->Eval((double)i));     
      }     
  }
  //~ cout<<poly.GetParameter(0)<<" + ("<<poly.GetParameter(1)<<"*tdrift) + ("<<poly.GetParameter(2)<<"*tdrift*tdrift) + ("<<poly.GetParameter(3)<<"*tdrift*tdrift*tdrift) + ("<<poly.GetParameter(4)<<"*tdrift*tdrift*tdrift*tdrift) + ("<<poly.GetParameter(5)<<"*tdrift*tdrift*tdrift*tdrift*tdrift)"<<endl<<endl;  
  //~ cout<<poly.GetParameter(0)<<" + ("<<poly.GetParameter(1)<<"*tdrift) + ("<<poly.GetParameter(2)<<"*tdrift*tdrift) + ("<<poly.GetParameter(3)<<"*tdrift*tdrift*tdrift) + ("<<poly.GetParameter(4)<<"*tdrift*tdrift*tdrift*tdrift) + ("<<(0.8-300.*poly.GetParameter(1)-90000.*poly.GetParameter(2)-27000000.*poly.GetParameter(3)-8100000000.*poly.GetParameter(4))/2430000000000.<<"*tdrift*tdrift*tdrift*tdrift*tdrift)"<<endl<<endl;  
  for(int i=0;i<poly.GetNpar();i++)
    cout<<poly.GetParameter(i)<<" , ";
  cout<<endl;
  
  return;
}


//to be made with labo sys of ref. (with respect to isocenter) 
void EvaluateSpaceResidual(vector<vector<double>> &space_residual,vector<vector<double>> &time_residual, BM_evstruct &bmevent, vtx_evstruct &vtxevent, vector<TVector3> &wire_pos, vector<TVector3> &wire_dir){
  
  if(debug>2)
    cout<<"I'm in EvaluateSpaceResidual"<<endl;
  
  //evaluate the space and time residuals
  int binpos, timepos;
  double residual, maxres, msd_rdrift;
  TVector3 pversout, r0posout;
  for(int i=0;i<bmevent.hitnum;i++){
    if((ONLY1VIEW==-1 || (ONLY1VIEW==CellId2view(bmevent.bm_hit_cellid[i]))) && bmevent.bm_hit_chi2[i]<=BMNHITCUT){ 
      binpos=(int)(bmevent.bm_hit_rdrift[i]*STBIN);
      timepos=(int)(bmevent.bm_hit_time[i]*STBIN/300.);
      if(binpos>STBIN-1) 
        binpos=STBIN;
      if(timepos>STBIN-1) 
        timepos=STBIN;
      vtxpversr02global(vtxevent.vtx_track_pvers, vtxevent.vtx_track_r0pos,  pversout, r0posout);    
      msd_rdrift=FindRdrift(pversout, r0posout, wire_pos.at(bmevent.bm_hit_cellid[i]), wire_dir.at(bmevent.bm_hit_cellid[i]));
      residual=msd_rdrift-bmevent.bm_hit_rdrift[i];
      if(debug>2)
        cout<<"residual="<<residual<<"  original_rdrift="<<bmevent.bm_hit_rdrift[i]<<"  fitted_rdrift="<<msd_rdrift<<endl;
      //~ if(fabs(residual)<0.8 && binpos!=STBIN){
      if(binpos!=STBIN && msd_rdrift<0.945){
        bmevent.bm_msd_space_binpos[i]=binpos;
        bmevent.bm_msd_space_res[i]=residual;
        //~ space_residual.at(binpos).push_back(residual);
      }else if(binpos==STBIN){
        cout<<"possible ERROR in EvaluateSpaceResidual space_residual??? residual="<<residual<<"  hit_id="<<bmevent.bm_hit_cellid[i]<<"  hit_rdrift="<<bmevent.bm_hit_rdrift[i]<<"  hit time="<<bmevent.bm_hit_time[i]<<endl;
        bmevent.bm_msd_space_binpos[i]=-2;
      }
      if(timepos!=STBIN && msd_rdrift<0.945){
        bmevent.bm_msd_time_binpos[i]=timepos;
        bmevent.bm_msd_time_res[i]=residual;
        //~ time_residual.at(timepos).push_back(residual);
      }else if(binpos==STBIN){
        cout<<"possible ERROR in EvaluateSpaceResidual time_residual??? residual="<<residual<<"  hit_id="<<bmevent.bm_hit_cellid[i]<<"  hit_rdrift="<<bmevent.bm_hit_rdrift[i]<<"  hit time="<<bmevent.bm_hit_time[i]<<endl;
        bmevent.bm_msd_time_binpos[i]=-2;
      }
      if(msd_rdrift>0.945){
        ((TH1D*)gDirectory->Get("residual_error"))->Fill(bmevent.bm_hit_rdrift[i]);
        if(debug>10)
          cout<<"msd rdrift too big :: residual="<<residual<<"  hit_id="<<bmevent.bm_hit_cellid[i]<<"  hit_rdrift="<<bmevent.bm_hit_rdrift[i]<<"  hit time="<<bmevent.bm_hit_time[i]<<endl;
      }else{
        bmevent.bm_msd_totres+=residual;
      }
    }
  }
  
  bmevent.bm_msd_totres+=999;//due to the default value
  
  if(bmevent.bm_msd_totres!=0)  
    ((TH1D*)gDirectory->Get("residual_total_xtrack"))->Fill(bmevent.bm_msd_totres);
  
  //charge the space and time residuals applying some cuts
  if(debug>10)
    cout<<"EvaluateSpaceResidual: Now I'll charge space_residual and time_residual vectors"<<endl;  
  if(bmevent.bm_msd_totres!=-999 && bmevent.bm_msd_totres<0.1 && bmevent.bm_msd_totres>-0.05){
    for(int i=0;i<bmevent.hitnum;i++){
      if(bmevent.bm_msd_time_binpos[i]!=-1 && bmevent.bm_msd_time_binpos[i]!=-2)
        time_residual.at(bmevent.bm_msd_time_binpos[i]).push_back(bmevent.bm_msd_time_res[i]);  
      if(bmevent.bm_msd_space_binpos[i]!=-1 && bmevent.bm_msd_space_binpos[i]!=-2){
        space_residual.at(bmevent.bm_msd_space_binpos[i]).push_back(bmevent.bm_msd_space_res[i]);  
        ((TH2D*)gDirectory->Get("residual_vs_rdrift"))->Fill(bmevent.bm_msd_time_res[i],bmevent.bm_hit_rdrift[i]);
        ((TH2D*)gDirectory->Get("msdresidual_vs_bmfitresidual"))->Fill(bmevent.bm_msd_time_res[i],bmevent.bm_hit_residual[i]);
      }
    }
  }
  
  if(debug>3)
    cout<<"EvaluateSpaceResidual finished"<<endl;  
  
  return;
}


//evaluate the projections
//pvers=particle direction, r0pos=where pvers is calculated, proposz=position where I want to project, in the local coordinate! 
TVector3 ExtrapolateZ(TVector3 pvers, TVector3 r0pos, double proposz, bool global, bool beammonitor){
  TVector3 projection, shifttoglobal;
  if(global && beammonitor){
    //~ TVector3 BMshift(BMSHIFTX, BMSHIFTY, BMSHIFTZ);
    //~ r0pos+=BMshift;
    pvers.RotateX(BMISOXANGLE*DEG2RAD);
    pvers.RotateY(BMISOYANGLE*DEG2RAD);
    shifttoglobal.SetXYZ(BMISOX,BMISOY,BMISOZ);
  }else if(!beammonitor){
    //~ TVector3 VTXshift(VTXSHIFTX, VTXSHIFTY, VTXSHIFTZ);
    //~ r0pos+=VTXshift;
    pvers.RotateX(VTXYZANGLE*DEG2RAD);
    pvers.RotateY(VTXXZANGLE*DEG2RAD);    
    shifttoglobal.SetXYZ(VTXISOX,VTXISOY,VTXISOZ);
  }
  projection.SetXYZ(pvers.X()/pvers.Z()*proposz+r0pos.X(),  pvers.Y()/pvers.Z()*proposz+r0pos.Y(), proposz);
  if(global)
    projection+=shifttoglobal;
  return projection;
}

//to set the pvers and r0 from vtx to bm coordinate
void vtxpversr02global(const TVector3 vtxpvers, const TVector3 vtxr0pos, TVector3 &pversout, TVector3 &r0posout ){
  TVector3 VTXshift(vtxr0pos.X()+VTXISOX, vtxr0pos.Y()+VTXISOY, 0.);
  double proposz=BMISOZ-VTXISOZ;
  pversout=vtxpvers;
  pversout.RotateX((BMISOXANGLE-VTXYZANGLE)*DEG2RAD);
  pversout.RotateY((BMISOYANGLE-VTXXZANGLE)*DEG2RAD);    
  r0posout.SetXYZ(pversout.X()/pversout.Z()*proposz+VTXshift.X(),  pversout.Y()/pversout.Z()*proposz+VTXshift.Y(), proposz);
  return;
}

//estimate allign with projections on mylars 
//da sistemare!!!!
void Allign_estimate(TString bmin_filename, TString vtxin_filename,TString out_filename){

  if(debug>0)
    cout<<"I'm in Allign_estimate"<<endl;  
  
  //BM par
  Double_t BMxrot=atan((((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetMean(2) - ((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetMean(2))/(BMN_MYLAR2Z-BMN_MYLAR1Z))*RAD2DEG;    
  Double_t BMyrot=-atan((((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetMean(1)-((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetMean(1))/(BMN_MYLAR2Z-BMN_MYLAR1Z))*RAD2DEG;
  //~ Double_t BMxtra=-(((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetMean(1)+((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetMean(1))/2.;  
  //~ Double_t BMxtr_err=sqrt(pow(((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetMean(1)/sqrt(((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetEntries()),2.)  + pow(((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetMean(1)/sqrt(((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetEntries()),2.));  
  Double_t BMxtra=-((TH1D*)gDirectory->Get("bm_xR0_coeff"))->GetMean();  
  Double_t BMxtr_err=((TH1D*)gDirectory->Get("bm_xR0_coeff"))->GetMean()/sqrt(((TH1D*)gDirectory->Get("bm_xR0_coeff"))->GetEntries()) ;  
  //~ Double_t BMytra=-(((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetMean(2)+((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetMean(2))/2.;
  //~ Double_t BMytr_err=sqrt(pow(((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetMean(2)/sqrt(((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetEntries()),2.)  +  pow(((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetMean(2)/sqrt(((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetEntries()),2.));  
  Double_t BMytra=-((TH1D*)gDirectory->Get("bm_yR0_coeff"))->GetMean();
  Double_t BMytr_err=((TH1D*)gDirectory->Get("bm_yR0_coeff"))->GetMean()/sqrt(((TH1D*)gDirectory->Get("bm_xR0_coeff"))->GetEntries()) ;  
    
  //MSD par  
  Double_t MSDxtra=-((TH2D*)gDirectory->Get("vtx_isoplane_vtxsys"))->GetMean(1);  
  Double_t MSDytra=-((TH2D*)gDirectory->Get("vtx_isoplane_vtxsys"))->GetMean(2);  
    
  cout<<"Beam Monitor allignment parameters:"<<endl;
  cout<<"estimated rotation around X axis= "<<BMxrot<<endl;
  cout<<"estimated rotation around Y axis= "<<BMyrot<<endl;
  cout<<"estimated translation in X="<<BMxtra<<"   +-   "<<BMxtr_err<<endl;
  cout<<"estimated translation in Y="<<BMytra<<"   +-   "<<BMytr_err<<endl<<endl;
  
  cout<<"MSD allignment parameters:"<<endl;
  cout<<"estimated translation in X="<<MSDxtra<<"    estimated translation in Y="<<MSDytra<<endl;
      
  TString tmp_str("BM_infile="); tmp_str+=bmin_filename;
  tmp_str+="    MSD_infile=";tmp_str+=vtxin_filename;    
  tmp_str+="    OUTfile=";tmp_str+=out_filename;
  tmp_str+="    ONLYVIEW=";tmp_str+=ONLY1VIEW;    
  tmp_str+="    BM allign par: xrot="; tmp_str+= BMxrot; tmp_str+="  yrot="; tmp_str+=BMyrot;
  tmp_str+="  x_tra="; tmp_str+=BMxtra; tmp_str+=" +- ";  tmp_str+=BMxtr_err;
  tmp_str+="  y_tra="; tmp_str+=BMytra; tmp_str+=" +- ";  tmp_str+=BMytr_err;
  tmp_str+="    MSD_xtra=";tmp_str+=MSDxtra; tmp_str+="   MSD_ytra="; tmp_str+=MSDytra;
  TNamed n("BM_MSD_allign_par",tmp_str.Data());
  n.Write();
  
  if(debug>0)
    cout<<"Allign_estimate finished"<<endl;
  
  return;
}


void fitPositionResidual(){
TString graphname("bm_onmsd1diff_x");
vector<TString> title_vec;
title_vec.push_back(graphname);
graphname="bm_onmsd2diff_x";
title_vec.push_back(graphname);
graphname="bm_onmsd1diff_channel_x";
title_vec.push_back(graphname);
graphname="bm_onmsd2diff_channel_x";
title_vec.push_back(graphname);
graphname="pvers_total_diff";
title_vec.push_back(graphname);

for(int i=0;i<title_vec.size();i++){
  TF1  *gaus = new TF1("gaus","gaus",-2000.,2000.);  
  gaus->SetParameter(2,((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->GetMean());
  gaus->SetParameter(3,((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->GetStdDev());
  ((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->Fit("gaus","QMB","",((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->GetXaxis()->GetBinCenter(((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->FindFirstBinAbove(10)),((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->GetXaxis()->GetBinCenter(((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->FindLastBinAbove(10)));
  cout<<endl<<title_vec.at(i).Data()<<" fit results:"<<endl;
  cout<<"mean="<<gaus->GetParameter(1)<<" +- "<<gaus->GetParError(1)<<endl;
  cout<<"devstd="<<gaus->GetParameter(2)<<" +- "<<gaus->GetParError(2)<<endl;
  delete gaus;
}
cout<<endl;
return;  
}

void merge_graphics(TFile* infile, TFile* f_out){
  vector<TString> h2tstring, h1tstring;
  TString tmp_tstring;
  TH2D* histo2d;
  TH1D* histo1d;
  char tmp_char[200];
  
  //th2d
  tmp_tstring="pvers_mx_bmmsd";
  h2tstring.push_back(tmp_tstring);
  tmp_tstring="old_strel";
  h2tstring.push_back(tmp_tstring);
  //th1d
  tmp_tstring="space_residual_error";
  h1tstring.push_back(tmp_tstring);
  tmp_tstring="time_residual_error";
  h1tstring.push_back(tmp_tstring);
  tmp_tstring="pvers_total_diff";
  h1tstring.push_back(tmp_tstring);
  tmp_tstring="residual_space_numev";
  h1tstring.push_back(tmp_tstring);
  tmp_tstring="residual_space_total_xhit";
  h1tstring.push_back(tmp_tstring);
  tmp_tstring="residual_time_total_xhit";
  h1tstring.push_back(tmp_tstring);
  
  //merge h1tstring and h2tstring histograms
  for(int i=0;i<h2tstring.size();i++){
    infile->cd();
    histo2d=(TH2D*)((TH2D*)gDirectory->Get(h2tstring.at(i).Data()))->Clone();
    f_out->cd();
    ((TH2D*)gDirectory->Get(h2tstring.at(i).Data()))->Add(histo2d);
  }
  for(int i=0;i<h1tstring.size();i++){
    infile->cd();
    histo1d=(TH1D*)((TH1D*)gDirectory->Get(h1tstring.at(i).Data()))->Clone();
    f_out->cd();
    ((TH1D*)gDirectory->Get(h1tstring.at(i).Data()))->Add(histo1d);
  }
  
  //merge residual histograms
  for(int i=0;i<STBIN;i++){
    sprintf(tmp_char,"Res_vs_rdrift/strel_rdrift_%d", i);
    infile->cd();
    histo1d=(TH1D*)((TH1D*)gDirectory->Get(tmp_char))->Clone();
    f_out->cd();
    ((TH1D*)gDirectory->Get(tmp_char))->Add(histo1d);
    sprintf(tmp_char,"Res_vs_tdrift/strel_tdrift_%d", i);
    infile->cd();
    histo1d=(TH1D*)((TH1D*)gDirectory->Get(tmp_char))->Clone();
    f_out->cd();
    ((TH1D*)gDirectory->Get(tmp_char))->Add(histo1d);
  }
  
  return;
}


//*************************************************** MSD STUFF  ***************************************** 
bool msdreadevent(vtx_evstruct &vtxevent, TTreeReader &msdReader, TTreeReaderValue<double> &trackchi2MSDreader, TTreeReaderValue<double> &thetaMSDreader, TTreeReaderValue<double> &phiMSDreader,   TTreeReaderValue<double> &r0xMSDreader,   TTreeReaderValue<double> &r0yMSDreader, vector<int> &vtxin_shifts, int filenum, int lastvtxeventnum){
  
  if(debug)
    cout<<"I'm in MSDreadevent"<<endl;
  
  //~ clean_msdevstruct(vtxevent);
  msdReader.Next(); 
   
  if(debug)
    cout<<"msdreadevent::  old evnum="<<vtxevent.evnum<<",  msdReader.GetCurrentEntry()="<<msdReader.GetCurrentEntry()<<"  filenum="<<filenum<<"   lastvtxeventnum="<<lastvtxeventnum<<endl;  
  
  vtxevent.evnum=msdReader.GetCurrentEntry()-vtxin_shifts.at(filenum)+lastvtxeventnum;
  //~ vtxevent.timeacq=*timeacqMSDreader;
  vtxevent.tracknum=1;
  vtxevent.vtx_track_chi2tot=*trackchi2MSDreader;
  vtxevent.vtx_track_chi2uview=0.;
  vtxevent.vtx_track_chi2vview=0.;

  vtxevent.vtx_track_pvers.SetXYZ(0.,0.,1.);
  vtxevent.vtx_track_pvers.SetTheta(*thetaMSDreader);
  vtxevent.vtx_track_pvers.SetPhi(*phiMSDreader);
  vtxevent.vtx_track_pvers.SetX(-1.*vtxevent.vtx_track_pvers.X());
  vtxevent.vtx_track_pvers.SetY(-1.*vtxevent.vtx_track_pvers.Y());

  vtxevent.vtx_track_r0pos.SetXYZ(-*r0xMSDreader/10.,-*r0yMSDreader/10.,VTXR0Z);
  if(vtxevent.tracknum!=-999 && vtxevent.vtx_track_chi2tot!=-999 && *thetaMSDreader!=-999 && *phiMSDreader!=-999 && *r0xMSDreader!=-999 && *r0yMSDreader!=-999)
    vtxevent.status=1;
  else
    vtxevent.status=-1;
  
  if(debug)
    cout<<"msdreadevent finished, current event="<<msdReader.GetCurrentEntry()<<"/"<<msdReader.GetEntries(true)<<endl;
  
  if(debug>2)
    print_vtxevstruct(vtxevent);
  
  if((msdReader.GetCurrentEntry()==(msdReader.GetEntries(true)-3)) || (VTXNEV && vtxevent.evnum>=VTXNEV) )
    return false;
    
  return true;
}


bool vtxreadevent(vtx_evstruct &vtxevent,TTreeReader &vtxReader, TTreeReaderValue<int> &evnum, TTreeReaderValue<int> &tracknum, TTreeReaderValue<double> &chi2tot, TTreeReaderValue<double> &chi2uview, TTreeReaderValue<double> &chi2vview, TTreeReaderValue<double> &r0x, TTreeReaderValue<double> &r0y, TTreeReaderValue<double> &pversx, TTreeReaderValue<double> &pversy){
  
  if(debug>2)
    cout<<"I'm in VtXreadevent"<<endl;
  
  //~ clean_vtxevstruct(vtxevent);
  vtxReader.Next(); 
  
  if(debug>2)
    cout<<"vtxreadevent::  old evnum="<<vtxevent.evnum<<",  vtxReader.GetCurrentEntry()="<<vtxReader.GetCurrentEntry()<<endl;  
  clean_vtxevstruct(vtxevent);
    
  vtxevent.evnum=*evnum;
  vtxevent.tracknum=*tracknum;
  vtxevent.vtx_track_chi2tot=*chi2tot;
  vtxevent.vtx_track_chi2uview=*chi2uview;
  vtxevent.vtx_track_chi2vview=*chi2vview;
  vtxevent.vtx_track_r0pos.SetXYZ(*r0x, *r0y, 0.);
  vtxevent.vtx_track_pvers.SetXYZ(*pversx, *pversy, 1.);
  
  if(debug>2)
    cout<<"msdreadevent finished, current event="<<vtxReader.GetCurrentEntry()<<"/"<<vtxReader.GetEntries(true)<<endl;
  
  if(debug>2)
    print_vtxevstruct(vtxevent);
  
  if((vtxReader.GetCurrentEntry()>=(vtxReader.GetEntries(true)-1)) || (VTXNEV && vtxevent.evnum>=VTXNEV) )
    return false;
    
  return true;
}
  
void clean_vtxevstruct(vtx_evstruct &vtxevstruct){

  vtxevstruct.evnum=-1;
  vtxevstruct.tracknum=-1;
  vtxevstruct.vtx_track_chi2tot=-999;
  vtxevstruct.vtx_track_chi2uview=-999;
  vtxevstruct.vtx_track_chi2vview=-999;
  vtxevstruct.vtx_track_pvers.SetXYZ(-999,-999,-999);
  vtxevstruct.vtx_track_r0pos.SetXYZ(-999,-999,-999);
  vtxevstruct.status=0;

return;
}
  
  
void print_vtxevstruct(vtx_evstruct &vtxevstruct){

  cout<<"print_vtxevstruct:"<<endl;
  cout<<"vtx evnum="<<vtxevstruct.evnum<<"  tracknum="<<vtxevstruct.tracknum<<endl;
  cout<<"vtx_track_chi2tot="<<vtxevstruct.vtx_track_chi2tot<<"  vtx_track_chi2uview="<<vtxevstruct.vtx_track_chi2uview<<"  vtx_track_chi2vview="<<vtxevstruct.vtx_track_chi2vview<<endl;
  cout<<"pvers=("<<vtxevstruct.vtx_track_pvers.X()<<", "<<vtxevstruct.vtx_track_pvers.Y()<<", "<<vtxevstruct.vtx_track_pvers.Z()<<")  r0=("<<vtxevstruct.vtx_track_r0pos.X()<<", "<<vtxevstruct.vtx_track_r0pos.Y()<<", "<<vtxevstruct.vtx_track_r0pos.Z()<<")"<<endl;
  cout<<"status="<<vtxevstruct.status<<endl;

return;
}  

void fitVTXgraph(){
TString graphname("MSD_1xraw");
vector<TString> title_vec;
title_vec.push_back(graphname);
graphname="MSD_1xval";
title_vec.push_back(graphname);
graphname="MSD_1xtra";
title_vec.push_back(graphname);
graphname="MSD_1yraw";
title_vec.push_back(graphname);
graphname="MSD_1yval";
title_vec.push_back(graphname);
graphname="MSD_1ytra";
title_vec.push_back(graphname);
graphname="MSD_xzraw_coeff";
title_vec.push_back(graphname);
graphname="MSD_xzval_coeff";
title_vec.push_back(graphname);

for(int i=0;i<title_vec.size();i++){
  TF1  *gaus = new TF1("gaus","gaus",-2000.,2000.);  
  gaus->SetParameter(2,((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->GetMean());
  gaus->SetParameter(3,((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->GetStdDev());
  ((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->Fit("gaus","QMB","",((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->GetXaxis()->GetBinCenter(((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->FindFirstBinAbove()),((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->GetXaxis()->GetBinCenter(((TH1D*)gDirectory->Get(title_vec.at(i).Data()))->FindLastBinAbove()));
  cout<<endl<<title_vec.at(i).Data()<<" fit results:"<<endl;
  cout<<"mean="<<gaus->GetParameter(1)<<" +- "<<gaus->GetParError(1)<<endl;
  cout<<"devstd="<<gaus->GetParameter(2)<<" +- "<<gaus->GetParError(2)<<endl;
  delete gaus;
}

return;  
}

//************************************************* BM stuff *************************************************

//read the next bm event, return false if the file is finished, otherwise true
//at the end bmReader is on the new event
bool bmreadevent(TTreeReader &bmReader, BM_evstruct &bmevent, TTreeReaderValue<int> &evnumreader,   TTreeReaderValue<int> &timeacqreader, TTreeReaderValue<double> &trackchi2reader, TTreeReaderValue<double> &pversxreader,   TTreeReaderValue<double> &pversyreader, TTreeReaderValue<double> &pverszreader,   TTreeReaderValue<double> &r0xreader,   TTreeReaderValue<double> &r0yreader,   TTreeReaderValue<double> &rdriftreader,   TTreeReaderValue<double> &residualreader,   TTreeReaderValue<double> &hittimereader,   TTreeReaderValue<int> &planereader,  TTreeReaderValue<int> &viewreader,   TTreeReaderValue<int> &cellreader, TTreeReaderValue<double> &hitchi2reader, int lastvtxeventnum){
  
  if(debug>2)
    cout<<"I'm in bmreadevent, current entry="<<bmReader.GetCurrentEntry()<<endl;
  
  clean_bmevstruct(bmevent, false);
  if(bmReader.GetCurrentEntry()<0)
    bmReader.Next(); 
   
  if(debug>3)
    cout<<"bmreadevent:: old evnum="<<bmevent.evnum<<",  new evnum="<<*evnumreader<<"   lastvtxeventnum="<<lastvtxeventnum<<endl;  
   
  int evhitnum=0;
  TVector3 tmp_tvector3;
  do{ 
    if(bmevent.evnum!=(*evnumreader+lastvtxeventnum) && evhitnum!=0)
      cout<<"bmreadevent:: Something is wrong!!! the old evnum is "<<bmevent.evnum<<", the new evnum is "<<*evnumreader<<"  lastvtxeventnum="<<lastvtxeventnum<<endl; 
    bmevent.evnum=*evnumreader+lastvtxeventnum;
    bmevent.timeacq=*timeacqreader;
    bmevent.bm_track_chi2=*trackchi2reader;
    tmp_tvector3.SetXYZ(*pversxreader,*pversyreader,*pverszreader);
    //~ tmp_tvector3.RotateX(BMYZANGLE*DEG2RAD);
    //~ tmp_tvector3.RotateY(BMXZANGLE*DEG2RAD);
    bmevent.bm_track_pvers=tmp_tvector3;
    //~ bmevent.bm_track_r0pos.SetXYZ(*r0xreader+BMSHIFTX,*r0yreader+BMSHIFTY,0.);
    bmevent.bm_track_r0pos.SetXYZ(*r0xreader,*r0yreader,0.);
    bmevent.bm_hit_rdrift[evhitnum]=*rdriftreader;
    bmevent.bm_hit_residual[evhitnum]=*residualreader;
    bmevent.bm_hit_time[evhitnum]=*hittimereader;
    bmevent.bm_hit_cellid[evhitnum]=GetBMNcell(*planereader, *viewreader,*cellreader);
    bmevent.bm_hit_chi2[evhitnum]=*hitchi2reader;
    evhitnum++;
  }while(bmReader.Next() && (bmevent.evnum)==(*evnumreader+lastvtxeventnum));
  bmevent.hitnum=evhitnum;
  
  if(debug>2)
    cout<<"bmreadevent finished"<<endl;
    
  if(debug>2)
    print_bmevstruct(bmevent);  

  if((bmReader.GetCurrentEntry()>=bmReader.GetEntries(true)-1) || (BMNEV && bmevent.evnum>=BMNEV) )
    return false;  
    
  return true;
}


bool bmMCreadevent(vtx_evstruct &vtxevent,TTreeReader &mcvtxReader, TTreeReaderValue<int> &mcevnumreader, TTreeReaderValue<double> &mcpversxreader, TTreeReaderValue<double> &mcpversyreader, TTreeReaderValue<double> &mcr0xreader, TTreeReaderValue<double> &mcr0yreader){

  if(debug>2)
    cout<<"I'm in bmMCreadevent, current entry="<<mcvtxReader.GetCurrentEntry()<<endl;
  
  if(mcvtxReader.GetCurrentEntry()<0)
    mcvtxReader.Next(); 
        
  mcvtxReader.Next(); 
  if(debug>2)
    cout<<"bmMCreadevent::  old evnum="<<vtxevent.evnum<<",  new evnum="<<mcvtxReader.GetCurrentEntry()<<endl;  
  clean_vtxevstruct(vtxevent);   
  int evhitnum=0;
  
  do{ 
    if(vtxevent.evnum!=*mcevnumreader && evhitnum!=0)
      cout<<"bmMCreadevent:: Something is wrong!!! the old evnum is "<<vtxevent.evnum<<", the new evnum is "<<*mcevnumreader<<endl;   
    vtxevent.evnum=*mcevnumreader;
    vtxevent.tracknum=1;
    vtxevent.vtx_track_chi2tot=0.;
    vtxevent.vtx_track_chi2uview=0.;
    vtxevent.vtx_track_chi2vview=0.;
    vtxevent.vtx_track_r0pos.SetXYZ(*mcr0xreader, *mcr0yreader, 0.);
    vtxevent.vtx_track_pvers.SetXYZ(*mcpversxreader, *mcpversyreader, 1.);
  }while(mcvtxReader.Next() && vtxevent.evnum==*mcevnumreader);    
  
  if(debug>2)
    cout<<"bmMCreadevent finished, current event="<<mcvtxReader.GetCurrentEntry()<<"/"<<mcvtxReader.GetEntries(true)<<endl;
  
  if(debug>2)
    print_vtxevstruct(vtxevent);
  
  if((mcvtxReader.GetCurrentEntry()>=(mcvtxReader.GetEntries(true)-1)) || (VTXNEV && vtxevent.evnum>=VTXNEV) )
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
  bmevstruct.bm_msd_totres=-999;
  for(int index=0;index<MAXBMHITNUM;index++){
    bmevstruct.bm_hit_rdrift[index]=-1;
    bmevstruct.bm_hit_residual[index]=-1;
    bmevstruct.bm_hit_time[index]=-1;
    bmevstruct.bm_hit_cellid[index]=-1;
    bmevstruct.bm_hit_chi2[index]=-1;
    bmevstruct.bm_msd_time_binpos[index]=-1;
    bmevstruct.bm_msd_time_res[index]=-999;
    bmevstruct.bm_msd_space_binpos[index]=-1;
    bmevstruct.bm_msd_space_res[index]=-999;
  }
  return;
}
int index=0;
while(bmevstruct.bm_hit_rdrift[index]!=-1 && index<MAXBMHITNUM){
  bmevstruct.bm_hit_rdrift[index]=-1;
  bmevstruct.bm_hit_residual[index]=-1;
  bmevstruct.bm_hit_time[index]=-1;
  bmevstruct.bm_hit_cellid[index]=-1;
  bmevstruct.bm_hit_chi2[index]=-1;
  bmevstruct.bm_msd_time_binpos[index]=-1;
  bmevstruct.bm_msd_time_res[index]=-999;
  bmevstruct.bm_msd_space_binpos[index]=-1;
  bmevstruct.bm_msd_space_res[index]=-999;
  index++;
}

return;
}


void print_bmevstruct(BM_evstruct &bmevstruct){
  cout<<"print bmevstruct"<<endl;
  cout<<"bm evnum="<<bmevstruct.evnum<<"  timeacq="<<bmevstruct.timeacq<<endl;
  cout<<"hitnum="<<bmevstruct.hitnum<<"  trackchi2="<<bmevstruct.bm_track_chi2<<"  pvers=("<<bmevstruct.bm_track_pvers.X()<<", "<<bmevstruct.bm_track_pvers.Y()<<", "<<bmevstruct.bm_track_pvers.Z()<<")  r0=("<<bmevstruct.bm_track_r0pos.X()<<", "<<bmevstruct.bm_track_r0pos.Y()<<", "<<bmevstruct.bm_track_r0pos.Z()<<endl;
  cout<<"loop on hits:"<<endl;
  for(int i=0;i<bmevstruct.hitnum;i++){
    cout<<"i="<<i<<"  cellid="<<bmevstruct.bm_hit_cellid[i]<<"  rdrift="<<bmevstruct.bm_hit_rdrift[i]<<"  residual="<<bmevstruct.bm_hit_residual[i]<<"  time="<<bmevstruct.bm_hit_time[i]<<"   hitchi2="<<bmevstruct.bm_hit_chi2[i]<<endl;
  }  
  
return;  
}




//************************************************  BM geostuff  *******************************************
//long inefficient method in shoe, to set wire_pos and wire_dir
void setbmgeo(vector<TVector3> &wire_pos, vector<TVector3> &wire_dir){
  
  if(debug)
    cout<<"I'm starting setbmgeo"<<endl;
      
  double aa[21], bb[21];
  TVector3 bm_DeltaDch(2.8,2.8,2.85);
  TVector3 bm_SideDch(11.2,11.2,21.);

  for(int nn=0;nn<7;nn++) {
    bb[nn] = nn*0.8;
    bb[nn+7] = nn*0.8;
    bb[nn+14] = nn*0.8;

    aa[nn] = 0. ;
    aa[nn+7] = 0.5;
    aa[nn+14] = 2*0.5;
  }
  
  double x_pos[21][6][2], y_pos[21][6][2], z_pos[21][6][2];
  double cx_pos[21][6][2], cy_pos[21][6][2], cz_pos[21][6][2];
  
  for(int il=0; il<6;il++){
    for (int kk =0; kk<21;kk++){       
      /*  U wires -> along x, SIDE view */
      x_pos[kk][il][0] = - bm_SideDch[0]/2;
      z_pos[kk][il][0] = - bm_SideDch[2]/2 + bm_DeltaDch[2] +	aa[kk] + (2*il)*(2*0.5 + 0.3);
      if( (il==0) || (il==2) || (il==4) ){
        y_pos[kk][il][0] = - bm_SideDch[1]/2 + bm_DeltaDch[1] + bb[kk] + 0.8;
      }
      else{
        y_pos[kk][il][0] = - bm_SideDch[1]/2 + bm_DeltaDch[1] + bb[kk];
      }
      cx_pos[kk][il][0] = bm_SideDch[0];
      cy_pos[kk][il][0] = 0.;
      cz_pos[kk][il][0] = 0.;

      /* V wires -> along y, TOP view*/
      y_pos[kk][il][1] = - bm_SideDch[1]/2;
      z_pos[kk][il][1] = - bm_SideDch[2]/2 + bm_DeltaDch[2] + aa[kk] + (2*il+1)*(2*0.5 + 0.3);
      if( (il==0) || (il==2) || (il==4) ){
        x_pos[kk][il][1] = - bm_SideDch[0]/2 + bm_DeltaDch[0] + bb[kk];
      }
      else{
        x_pos[kk][il][1] = - bm_SideDch[0]/2 + bm_DeltaDch[0] + bb[kk] + 0.8;
      }
      cx_pos[kk][il][1] = 0.;
      cy_pos[kk][il][1] = bm_SideDch[1];
      cz_pos[kk][il][1] = 0.;
    }
  }
  
  wire_pos.clear();
  wire_dir.clear();
  TVector3 tmp_tvector3, bmshift(BMISOX, BMISOY, BMISOZ);
  int ilay, iview, icell;
  int bm_idsense[3]={8,10,12};
  for(int i=0;i<36;i++){
    Getlvc(i,ilay, iview, icell);
    //rotate and translate the bm wires (before the traslation)
    tmp_tvector3.SetXYZ(x_pos[bm_idsense[icell]][ilay][iview], y_pos[bm_idsense[icell]][ilay][iview],z_pos[bm_idsense[icell]][ilay][iview]);
    tmp_tvector3.RotateX((BMISOXANGLE)*DEG2RAD);
    tmp_tvector3.RotateY((BMISOYANGLE)*DEG2RAD);
    tmp_tvector3+=bmshift;
    wire_pos.push_back(tmp_tvector3);
    //rotate the bm direction wires (before the traslation)
    tmp_tvector3.SetXYZ(cx_pos[bm_idsense[icell]][ilay][iview], cy_pos[bm_idsense[icell]][ilay][iview],cz_pos[bm_idsense[icell]][ilay][iview]);
    tmp_tvector3.RotateX((BMISOXANGLE)*DEG2RAD);
    tmp_tvector3.RotateY((BMISOYANGLE)*DEG2RAD);
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
  local.RotateX((BMISOXANGLE)*DEG2RAD);
  local.RotateY((BMISOYANGLE)*DEG2RAD);
  TVector3 bmshift(BMISOX, BMISOY, BMISOZ);
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

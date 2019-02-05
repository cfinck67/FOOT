
//*********************************************  combined functions  *****************************************
void BookingBMMSD(TFile* f_out, bool onlyMSD){

  f_out->cd();
  TH1D *h; 
  TH2D *h2;
  char tmp_char[200];
  
  //msd stuff
  h = new TH1D("MSD_xzval_coeff","MSD xz m coeff for corrected positions;m;Events",1000,-0.5,0.5);
  h = new TH1D("MSD_xzraw_coeff","MSD xz m coeff from raw data;m;Events",2000,-100.,100.);
  h = new TH1D("MSD_xztra_coeff","MSD xz m coeff from translated data;m;Events",1000,-0.5,0.5);
  h = new TH1D("MSD_1xraw","MSD 1x raw channel value;x[cm];Events",2000,0.,2000.);
  h = new TH1D("MSD_2xraw","MSD 2x raw channel value;x[cm];Events",2000,0.,2000.);
  h = new TH1D("MSD_1yraw","MSD 1y raw channel value;y[cm];Events",2000,0.,2000.);
  h = new TH1D("MSD_1xtra","MSD 1x channel value translated;x[cm];Events",2000,-1000.,1000.);
  h = new TH1D("MSD_2xtra","MSD 2x channel value translated;x[cm];Events",2000,-1000.,1000.);
  h = new TH1D("MSD_1ytra","MSD 1y channel value translated;y[cm];Events",2000,-1000.,1000.);
  h = new TH1D("MSD_1xval","MSD 1x selected value;x[cm];Events",1000,-5.,5.);
  h = new TH1D("MSD_2xval","MSD 2x selected value;x[cm];Events",1000,-5.,5.);
  h = new TH1D("MSD_1yval","MSD 1y selected value;y[cm];Events",1000,-5.,5.);
  h = new TH1D("MSD_trackset","MSD tracksset;x[cm];Events",2,0.,2.);
  h2 = new TH2D("msd_msd1proISOsys","MSD tracks projection on msd 1 plane in ISO sys;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  
  if(onlyMSD){
    f_out->cd("..");
    return;
  }
  
  //bm stuff
  h = new TH1D("bmnhitsxevent","number of hits x event ;N of hits;Events",36,0.,36.);
  h = new TH1D("bm_polar_angle","BM polar angle distribution ;AngZ(deg);Events",400,0.,10.);
  h = new TH1D("bm_azimuth_angle","BM azimuth angle distribution ;Phi(deg);Events",180,0.,180.);
  h = new TH1D("bm_xz_coeff","BM xz m coeff ;m;Events",1000,-0.5,0.5);
  h2 = new TH2D("bmisoproISOsys","BM tracks on isocenter  projections in ISO sys ;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  h2 = new TH2D("bmisoproBMsys","BM tracks on isocenter projections in BM sys;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  h2 = new TH2D("bm_msd1proISOsys","bm tracks projection on msd 1 plane in ISO sys;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  h2 = new TH2D("bm_msd2proISOsys","bm tracks projection on msd 2 plane in ISO sys;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  h2 = new TH2D("bmmylar1BMsys","bm tracks on mylar1 projections in BM sys;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);
  h2 = new TH2D("bmmylar2BMsys","bm tracks on mylar2 projections in BM sys;X[cm];Y[cm]",600,-3.,3., 600, -3., 3);

  //combined stuff
  h = new TH1D("track_iso_diff_x","x difference for bm tracks and msd tracks on isocenter plane;x[cm];Events",500,-5.,5.);
  h = new TH1D("bm_onmsd1diff_x","x difference for bm tracks on mylar1 plane and msd1 hits;x[cm];Events",500,-5.,5.);
  h = new TH1D("bm_onmsd2diff_x","x difference for bm tracks on mylar2 plane and msd2 hits;x[cm];Events",500,-5.,5.);
  h = new TH1D("bm_onmsd1diff_channel_x","x difference for bm tracks on mylar1 plane and msd1 hits;x[msd channel];Events",2000,-1000.,1000.);
  h = new TH1D("bm_onmsd2diff_channel_x","x difference for bm tracks on mylar2 plane and msd2 hits;x[msd channel];Events",2000,-1000.,1000.);
    
  h2 = new TH2D("rdrift_vs_residual","residual calculation ;rdrift[cm];residual",STBIN,0.,1.,400, -0.5,0.5 );
  h = new TH1D("space_residual_error","error events ;rdrift;Events",STBIN,0.,1.);
  //~ h = new TH1D("time_diff","bm event timeacq - msd event timeacq ;time;Events",500,0.,100.);
  h2 = new TH2D("pvers_mx_bmmsd","pvers mx angle for bm and msd for selected tracks;BM Pvers mx;MSD Pverse mx ",500,-0.05,0.05,500,-0.05,0.05);
  h = new TH1D("pvers_ratio_vs_numev","mx ratio bm_track/msd_track;Event;bm_mx/msd_mx",110000,0.,110000.);
  h = new TH1D("pvers_diff_vs_numev","mx difference bm_track - msd_track x numev;Event;bm_mx/msd_mx",110000,0.,110000.);
  h = new TH1D("pvers_total_diff","mx difference bm_track - msd_track;bm_track - msd_track;Events",2000,-0.1,0.1);
  h = new TH1D("space_residual_numev","number of events for each bin in strel residual graph;bin;events",STBIN,0.,STBIN);
  h = new TH1D("space_residual_total","total space residual;x[cm];events",STBIN,0.,STBIN);
  
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


void Printoutput(TFile* f_out, vector<BM_evstruct> &allbmeventin, vector<MSD_evstruct> &allmsdeventin, vector<vector<vector<double>>> &space_residual, vector<vector<int>> &selected_index, bool onlyMSD){
  f_out->cd();
  if(debug)
    cout<<"I'm in Printoutput:: allbmeventin.size():"<<allbmeventin.size()<<endl;
  TVector3 tmp_tvector3;
  char tmp_char[200];
  
  //MSD events
  for(int i=0; i<allmsdeventin.size();i++){
    ((TH1D*)gDirectory->Get("MSD_trackset"))->Fill(allmsdeventin.at(i).trackset);
    if(allmsdeventin.at(i).x2raw!=-999 && allmsdeventin.at(i).x1raw!=-999)
      ((TH1D*)gDirectory->Get("MSD_xzraw_coeff"))->Fill((allmsdeventin.at(i).x2raw-allmsdeventin.at(i).x1raw)/(MSD2Z-MSD1Z));
    if(allmsdeventin.at(i).y1raw!=-999){
      ((TH1D*)gDirectory->Get("MSD_1yraw"))->Fill(allmsdeventin.at(i).y1raw);
      ((TH1D*)gDirectory->Get("MSD_1ytra"))->Fill(allmsdeventin.at(i).y1raw-MSD1Y);
      ((TH1D*)gDirectory->Get("MSD_1yval"))->Fill(allmsdeventin.at(i).y1val);
    }
    if(allmsdeventin.at(i).x1raw!=-999){
      ((TH1D*)gDirectory->Get("MSD_1xraw"))->Fill(allmsdeventin.at(i).x1raw);
      ((TH1D*)gDirectory->Get("MSD_1xtra"))->Fill(allmsdeventin.at(i).x1raw-MSD1X);
      ((TH1D*)gDirectory->Get("MSD_1xval"))->Fill(allmsdeventin.at(i).x1val);
    }
    if(allmsdeventin.at(i).x2raw!=-999){  
      ((TH1D*)gDirectory->Get("MSD_2xraw"))->Fill(allmsdeventin.at(i).x2raw);
      ((TH1D*)gDirectory->Get("MSD_2xtra"))->Fill(allmsdeventin.at(i).x2raw-MSD2X);
      ((TH1D*)gDirectory->Get("MSD_2xval"))->Fill(allmsdeventin.at(i).x2val);
    } 
    if(allmsdeventin.at(i).trackset==true){
      ((TH1D*)gDirectory->Get("MSD_xzval_coeff"))->Fill(allmsdeventin.at(i).msd_track_pvers.X()/allmsdeventin.at(i).msd_track_pvers.Z());
      tmp_tvector3=ExtrapolateZ(allmsdeventin.at(i).msd_track_pvers, allmsdeventin.at(i).msd_track_r0pos, 0., true, false);
      ((TH2D*)gDirectory->Get("msd_msd1proISOsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y()); 
    } 
  }
  
  if(onlyMSD)
    return;
  
  //BM events
  for(int i=0; i<allbmeventin.size();i++){
    ((TH1D*)gDirectory->Get("bmnhitsxevent"))->Fill(allbmeventin.at(i).hitnum);
    ((TH1D*)gDirectory->Get("bm_polar_angle"))->Fill(allbmeventin.at(i).bm_track_pvers.Theta()*RAD2DEG);
    ((TH1D*)gDirectory->Get("bm_azimuth_angle"))->Fill(allbmeventin.at(i).bm_track_pvers.Phi()*RAD2DEG);
    ((TH1D*)gDirectory->Get("bm_xz_coeff"))->Fill(allbmeventin.at(i).bm_track_pvers.X()/allbmeventin.at(i).bm_track_pvers.Z());
    tmp_tvector3=ExtrapolateZ(allbmeventin.at(i).bm_track_pvers, allbmeventin.at(i).bm_track_r0pos, -BMISOZ, true, true);
    ((TH2D*)gDirectory->Get("bmisoproISOsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    tmp_tvector3=ExtrapolateZ(allbmeventin.at(i).bm_track_pvers, allbmeventin.at(i).bm_track_r0pos, -BMISOZ, false, true);
    ((TH2D*)gDirectory->Get("bmisoproBMsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());//dovrebbe essere = a bmisoproISOsys
     tmp_tvector3=ExtrapolateZ(allbmeventin.at(i).bm_track_pvers, allbmeventin.at(i).bm_track_r0pos, -BMISOZ+MSD1Z, true, true);
    ((TH2D*)gDirectory->Get("bm_msd1proISOsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    tmp_tvector3=ExtrapolateZ(allbmeventin.at(i).bm_track_pvers, allbmeventin.at(i).bm_track_r0pos, -BMISOZ+MSD2Z, true, true);
    ((TH2D*)gDirectory->Get("bm_msd2proISOsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    tmp_tvector3=ExtrapolateZ(allbmeventin.at(i).bm_track_pvers, allbmeventin.at(i).bm_track_r0pos, BMN_MYLAR1Z, false, true);
    ((TH2D*)gDirectory->Get("bmmylar1BMsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    tmp_tvector3=ExtrapolateZ(allbmeventin.at(i).bm_track_pvers, allbmeventin.at(i).bm_track_r0pos, BMN_MYLAR2Z, false, true);
    ((TH2D*)gDirectory->Get("bmmylar2BMsys"))->Fill(tmp_tvector3.X(), tmp_tvector3.Y());
    
    for(int k=0;k<allbmeventin.at(i).hitnum;k++)
      ((TH2D*)gDirectory->Get("old_strel"))->Fill(allbmeventin.at(i).bm_hit_time[k],allbmeventin.at(i).bm_hit_rdrift[k]);
  }  
    
  
  //combined
  TVector3 bmproject, msdproject;
  for(int i=0;i<selected_index.size();i++){
    if(selected_index.at(i).at(2)==0){//both bm and msd track
      ((TH2D*)gDirectory->Get("pvers_mx_bmmsd"))->Fill(allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.X()/allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.Z(), allmsdeventin.at(selected_index.at(i).at(1)).msd_track_pvers.X()/allmsdeventin.at(selected_index.at(i).at(1)).msd_track_pvers.Z());        
      ((TH1D*)gDirectory->Get("pvers_ratio_vs_numev"))->SetBinContent(allbmeventin.at(selected_index.at(i).at(0)).evnum , (allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.X()/allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.Z()) / (allmsdeventin.at(selected_index.at(i).at(1)).msd_track_pvers.X()/allmsdeventin.at(selected_index.at(i).at(1)).msd_track_pvers.Z()));        
      ((TH1D*)gDirectory->Get("pvers_diff_vs_numev"))->SetBinContent(allbmeventin.at(selected_index.at(i).at(0)).evnum , (allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.X()/allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.Z()) - (allmsdeventin.at(selected_index.at(i).at(1)).msd_track_pvers.X()/allmsdeventin.at(selected_index.at(i).at(1)).msd_track_pvers.Z()));        
      ((TH1D*)gDirectory->Get("pvers_total_diff"))->Fill((allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.X()/allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers.Z()) - (allmsdeventin.at(selected_index.at(i).at(1)).msd_track_pvers.X()/allmsdeventin.at(selected_index.at(i).at(1)).msd_track_pvers.Z()));        
      
      
      bmproject=ExtrapolateZ(allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers, allbmeventin.at(selected_index.at(i).at(0)).bm_track_r0pos, BMISOZ,true, true);
      msdproject=ExtrapolateZ(allmsdeventin.at(selected_index.at(i).at(1)).msd_track_pvers, allmsdeventin.at(selected_index.at(i).at(1)).msd_track_r0pos, 0.,true, false);
      if(bmproject.Z()!=msdproject.Z())
        cout<<"ERROR on bm/msd iso projection::bmproject.Z()="<<bmproject.Z()<<"   msdproject.Z()="<<msdproject.Z()<<endl;
      else
        ((TH1D*)gDirectory->Get("track_iso_diff_x"))->Fill((bmproject-msdproject).X());    
    }
    if(selected_index.at(i).at(2)==1 || selected_index.at(i).at(2)==0){//bm track and only msd1xval
      bmproject=ExtrapolateZ(allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers, allbmeventin.at(selected_index.at(i).at(0)).bm_track_r0pos, BMISOZ+MSD1Z,true, true);
      ((TH1D*)gDirectory->Get("bm_onmsd1diff_x"))->Fill(bmproject.X()-allmsdeventin.at(selected_index.at(i).at(1)).x1val);
      ((TH1D*)gDirectory->Get("bm_onmsd1diff_channel_x"))->Fill((bmproject.X()-allmsdeventin.at(selected_index.at(i).at(1)).x1val)/MSD1PASSX);
    }
    if(selected_index.at(i).at(2)==2 || selected_index.at(i).at(2)==0){//bm track and only msd2xval
      bmproject=ExtrapolateZ(allbmeventin.at(selected_index.at(i).at(0)).bm_track_pvers, allbmeventin.at(selected_index.at(i).at(0)).bm_track_r0pos, BMISOZ+MSD2Z,true, true);
      ((TH1D*)gDirectory->Get("bm_onmsd2diff_x"))->Fill(bmproject.X()-allmsdeventin.at(selected_index.at(i).at(1)).x2val);
      ((TH1D*)gDirectory->Get("bm_onmsd2diff_channel_x"))->Fill((bmproject.X()-allmsdeventin.at(selected_index.at(i).at(1)).x2val)/MSD2PASSX);
    }
  }

  //try to estimate the new strel
  for(int i=0; i<space_residual.size();i++){
    ((TH1D*)gDirectory->Get("space_residual_numev"))->SetBinContent(i+1,space_residual.at(i).size());
    for(int k=0; k<space_residual.at(i).size();k++){
      ((TH2D*)gDirectory->Get("rdrift_vs_residual"))->Fill((double)i/STBIN,space_residual.at(i).at(k).at(0));
      sprintf(tmp_char,"Res_vs_rdrift/strel_rdrift_%d", i);
      ((TH1D*)gDirectory->Get(tmp_char))->Fill(space_residual.at(i).at(k).at(0));
    }
  }
  for(int k=0; k<space_residual.at(STBIN).size();k++)
      ((TH1D*)gDirectory->Get("space_residual_error"))->Fill(space_residual.at(STBIN).at(k).at(0));  
  //no gaussian fit on the slice of rdrift for the moment
  int binpos;
  double tmp_double;
  for(int i=0; i<allbmeventin.size();i++){
    for(int k=0;k<allbmeventin.at(i).hitnum;k++){
      binpos=(int)(allbmeventin.at(i).bm_hit_rdrift[k]*STBIN);
      if(binpos<STBIN){
        sprintf(tmp_char,"Res_vs_rdrift/strel_rdrift_%d", binpos);
        tmp_double=((TH1D*)gDirectory->Get(tmp_char))->GetMean();
        ((TH2D*)gDirectory->Get("new_strel"))->Fill(allbmeventin.at(i).bm_hit_time[k],allbmeventin.at(i).bm_hit_rdrift[k]-tmp_double);
      }    
    }
  }

  //fit the new_strel
  //draw the two strel in the same canvas
  TCanvas *strels=new TCanvas("strels","strels",800,800);
  strels->cd();
  TProfile *prof_oldstrel = ((TH2D*)gDirectory->Get("old_strel"))->ProfileX();
  prof_oldstrel->SetLineColor(1);
  prof_oldstrel->Draw();
  TProfile *prof_newstrel = ((TH2D*)gDirectory->Get("new_strel"))->ProfileX();
  prof_newstrel->SetLineColor(3);
  prof_newstrel->Draw("SAME");
  f_out->Append(strels);
  
  TF1 *poly = new TF1("poly","pol5", 0, 400);
  prof_newstrel->Fit("poly","Q");
  cout<<endl<<"new strel parameters:"<<endl;
  cout<<poly->GetParameter(0)<<" + ("<<poly->GetParameter(1)<<"*tdrift) + ("<<poly->GetParameter(2)<<"*tdrift*tdrift) + ("<<poly->GetParameter(3)<<"*tdrift*tdrift*tdrift) + ("<<poly->GetParameter(4)<<"*tdrift*tdrift*tdrift*tdrift) + ("<<poly->GetParameter(5)<<"*tdrift*tdrift*tdrift*tdrift*tdrift)"<<endl<<endl;
  
  return;  
}


//to be made with labo sys of ref. (with respect to isocenter) 
void EvaluateSpaceResidual(vector<vector<vector<double>>> &space_residual, BM_evstruct &bmevent, MSD_evstruct &msdevent, vector<TVector3> &wire_pos, vector<TVector3> &wire_dir){
  int binpos;
  double residual;
  vector<double> eventtoadd(2,-999.);
  for(int i=0;i<bmevent.hitnum;i++){
    if(CellId2view(bmevent.bm_hit_cellid[i])==1)  {
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
        //~ if(debug>0)
        cout<<"possible ERROR in EvaluateSpaceResidual??? residual="<<residual<<"  hit_id="<<bmevent.bm_hit_cellid[i]<<"  hit_rdrift="<<bmevent.bm_hit_rdrift[i]<<endl;
        eventtoadd.at(0)=bmevent.bm_hit_rdrift[i];
        eventtoadd.at(1)=bmevent.bm_hit_time[i];
        space_residual.at(STBIN).push_back(eventtoadd);
      }  
    }
  }
  return;
}


//evaluate the projections
//pvers=particle direction, r0pos=where pvers is calculated, proposz=position where I want to 
TVector3 ExtrapolateZ(TVector3 pvers, TVector3 r0pos, double proposz, bool global, bool beammonitor){
  TVector3 projection, shifttoglobal;
  if(global && beammonitor){
    TVector3 BMshift(BMSHIFTX, BMSHIFTY, BMSHIFTZ);
    r0pos+=BMshift;
    pvers.RotateX(BMYZANGLE*DEG2RAD);
    pvers.RotateY(BMXZANGLE*DEG2RAD);
    shifttoglobal.SetXYZ(-BMISOX,-BMISOY,-BMISOZ);
  }else if(!beammonitor){
    TVector3 MSDshift(MSDSHIFTX, MSDSHIFTY, MSDSHIFTZ);
    r0pos+=MSDshift;
    pvers.RotateX(MSDYZANGLE*DEG2RAD);
    pvers.RotateY(MSDXZANGLE*DEG2RAD);    
    //~ shifttoglobal.SetXYZ(MSDISOX,MSDISOY,MSDISOZ);
  }
  projection.SetXYZ(pvers.X()/pvers.Z()*proposz+r0pos.X(),  pvers.Y()/pvers.Z()*proposz+r0pos.Y(), proposz);
  if(global && beammonitor)
    projection+=shifttoglobal;
  return projection;
}

//estimate allign with projections on mylars 
//da sistemare!!!!
void Allign_estimate(){
  
  //BM par
  Double_t BMxrot=atan((((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetMean(2) - ((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetMean(2))/(BMN_MYLAR2Z-BMN_MYLAR1Z))*RAD2DEG;    
  Double_t BMyrot=-atan((((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetMean(1)-((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetMean(1))/(BMN_MYLAR2Z-BMN_MYLAR1Z))*RAD2DEG;
  Double_t BMxtra=-(((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetMean(1)+((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetMean(1))/2.;  
  Double_t BMxtr_err=sqrt(pow(((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetMean(1)/sqrt(((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetEntries()),2.)  + pow(((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetMean(1)/sqrt(((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetEntries()),2.));  
  Double_t BMytra=-(((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetMean(2)+((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetMean(2))/2.;
  Double_t BMytr_err=sqrt(pow(((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetMean(2)/sqrt(((TH1D*)gDirectory->Get("bmmylar2BMsys"))->GetEntries()),2.)  +  pow(((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetMean(2)/sqrt(((TH1D*)gDirectory->Get("bmmylar1BMsys"))->GetEntries()),2.));  
    
  cout<<"Beam Monitor allignment parameters:"<<endl;
  cout<<"estimated rotation around X axis= "<<BMxrot<<endl;
  cout<<"estimated rotation around Y axis= "<<BMyrot<<endl;
  cout<<"estimated translation in X="<<BMxtra<<"   +-   "<<BMxtr_err<<endl;
  cout<<"estimated translation in Y="<<BMytra<<"   +-   "<<BMytr_err<<endl<<endl;
      
  //~ //MSD par
  //~ Double_t MSDxrot=atan((((TH1D*)gDirectory->Get("msdmylar2MSDsys"))->GetMean(2) - ((TH1D*)gDirectory->Get("msdmylar1MSDsys"))->GetMean(2))/(BMN_MYLAR2Z-BMN_MYLAR1Z))*RAD2DEG;    
  //~ Double_t MSDyrot=-atan((((TH1D*)gDirectory->Get("msdmylar2MSDsys"))->GetMean(1)-((TH1D*)gDirectory->Get("msdmylar1MSDsys"))->GetMean(1))/(BMN_MYLAR2Z-BMN_MYLAR1Z))*RAD2DEG;
  //~ Double_t MSDxtra=-(((TH1D*)gDirectory->Get("msdmylar2MSDsys"))->GetMean(1)+((TH1D*)gDirectory->Get("msdmylar1MSDsys"))->GetMean(1))/2.;  
  //~ Double_t MSDxtr_err=sqrt(pow(((TH1D*)gDirectory->Get("msdmylar2MSDsys"))->GetMean(1)/sqrt(((TH1D*)gDirectory->Get("msdmylar2MSDsys"))->GetEntries()),2.)  + pow(((TH1D*)gDirectory->Get("msdmylar1MSDsys"))->GetMean(1)/sqrt(((TH1D*)gDirectory->Get("msdmylar1MSDsys"))->GetEntries()),2.));  
  //~ Double_t MSDytra=-(((TH1D*)gDirectory->Get("msdmylar2MSDsys"))->GetMean(2)+((TH1D*)gDirectory->Get("msdmylar1MSDsys"))->GetMean(2))/2.;
  //~ Double_t MSDytr_err=sqrt(pow(((TH1D*)gDirectory->Get("msdmylar2MSDsys"))->GetMean(2)/sqrt(((TH1D*)gDirectory->Get("msdmylar2MSDsys"))->GetEntries()),2.)  +  pow(((TH1D*)gDirectory->Get("msdmylar1MSDsys"))->GetMean(2)/sqrt(((TH1D*)gDirectory->Get("msdmylar1MSDsys"))->GetEntries()),2.));  
  
  double MSD_2xpos=-((TH1D*)gDirectory->Get("MSD_xzval_coeff"))->GetMean()*MSD2Z/MSD2PASSX+MSD2X;

  cout<<"MSD2 allignment parameters:"<<endl;
  cout<<"estimated change of msd2xpos= "<<MSD_2xpos<<endl;
  //~ cout<<"estimated rotation around Y axis= "<<MSDyrot<<endl;
  //~ cout<<"estimated translation in X="<<MSDxtra<<"   +-   "<<MSDxtr_err<<endl;
  //~ cout<<"estimated translation in Y="<<MSDytra<<"   +-   "<<MSDytr_err<<endl;
  
  TString tmp_str("BM allign par: xrot="); tmp_str+= BMxrot; tmp_str+="  yrot="; tmp_str+=BMyrot;
  tmp_str+="  x_tra="; tmp_str+=BMxtra; tmp_str+=" +- ";  tmp_str+=BMxtr_err;
  tmp_str+="  y_tra="; tmp_str+=BMytra; tmp_str+=" +- ";  tmp_str+=BMytr_err;
  tmp_str+="       MSD allign par: xrot="; tmp_str+= MSD_2xpos;
  //~ ; tmp_str+="  yrot="; tmp_str+=MSDyrot;
  //~ tmp_str+="  x_tra="; tmp_str+=MSDxtra; tmp_str+=" +- "; tmp_str+=MSDxtr_err;
  //~ tmp_str+="  y_tra="; tmp_str+=MSDytra; tmp_str+=" +- "; tmp_str+=MSDytr_err;    
  TNamed n("BM_MSD_allign_par",tmp_str.Data());
  n.Write();
  
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
graphname="MSD_xzval_coeff";
title_vec.push_back(graphname);
graphname="MSD_xzraw_coeff";
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


//*************************************************** MSD STUFF  ***************************************** 
    
bool msdreadevent(MSD_evstruct &msdevent,TTreeReader &msd1Reader,TTreeReader &msd2Reader, TTreeReaderValue<double> &x1msd, TTreeReaderValue<double> &y1msd, TTreeReaderValue<double> &x2msd){
  
  if(debug)
    cout<<"I'm in MSDreadevent"<<endl;
  
  //~ clean_msdevstruct(msdevent);
  msd1Reader.Next(); 
  msd2Reader.Next(); 
  
  if(msd1Reader.GetCurrentEntry()!=msd2Reader.GetCurrentEntry()){
    cout<<"Error in the msdreadevent!!!!! msd1Reader.GetCurrentEntry()="<<msd1Reader.GetCurrentEntry()<<"  msd2Reader.GetCurrentEntry()"<<msd2Reader.GetCurrentEntry()<<endl<<"The msd reader will be stopped"<<endl;
    return false;
  }else
    clean_msdevstruct(msdevent);
   
  if(debug)
    cout<<"msdreadevent::  old evnum="<<msdevent.evnum<<",  new evnum="<<msd1Reader.GetCurrentEntry()<<endl;  
    
  msdevent.evnum=msd1Reader.GetCurrentEntry();
  msdevent.x1raw=*x1msd;
  msdevent.y1raw=*y1msd;
  msdevent.x2raw=*x2msd;
  if(*y1msd!=0)
    msdevent.y1val=(*y1msd-MSD1Y)*MSD1PASSY;  
  if(*x1msd!=-999)
    msdevent.x1val=(*x1msd-MSD1X)*MSD1PASSX;
  if(*x2msd!=-999)
    msdevent.x2val=(*x2msd-MSD2X)*MSD2PASSX;
  if(*x1msd!=-999 && *x2msd!=-999){
    //~ msdevent.msd_track_pvers.SetXYZ((msdevent.x1val-msdevent.x2val)/(MSD1Z-MSD2Z),0.,1.);  
    msdevent.msd_track_pvers.SetXYZ((msdevent.x1val-msdevent.x2val)/(-MSD1Z+MSD2Z),0.,1.);  //provv
    msdevent.msd_track_pvers.SetMag(1.);  
    //~ msdevent.msd_track_r0pos.SetXYZ(-(*x1msd-*x2msd)/(MSD1Z-MSD2Z)*MSD2Z+*x2msd,*y1msd,0.);  
    msdevent.msd_track_r0pos.SetXYZ(msdevent.x1val, msdevent.y1val, MSD1Z);  
    msdevent.trackset=true;  
  }
  
  if(debug)
    cout<<"msdreadevent finished, current event="<<msd1Reader.GetCurrentEntry()<<"/"<<msd1Reader.GetEntries(true)<<"   trackset="<<msdevent.trackset<<endl;
  
  if(debug>2 && msdevent.trackset==true)
    print_msdevstruct(msdevent);
  
  if((msd1Reader.GetCurrentEntry()==(msd1Reader.GetEntries(true)-1)) || (MSDNEV && msdevent.evnum>=MSDNEV) )
    return false;
    
  return true;
}
  
void clean_msdevstruct(MSD_evstruct &msdevstruct){

  msdevstruct.evnum=-1;
  msdevstruct.x1val=-999;
  msdevstruct.y1val=-999;
  msdevstruct.x2val=-999;
  
  msdevstruct.x1raw=-999;
  msdevstruct.y1raw=-999;
  msdevstruct.x2raw=-999;
  
  //~ msdevstruct.timeacq=-1;
  msdevstruct.msd_track_pvers.SetXYZ(0,0,0);
  msdevstruct.msd_track_r0pos.SetXYZ(0,0,0);
  msdevstruct.trackset=false;

return;
}
  
  
void print_msdevstruct(MSD_evstruct &msdevstruct){

  cout<<"print_msdevstruct:"<<endl;
  cout<<"msd evnum="<<msdevstruct.evnum<<endl;
  cout<<"x1val="<<msdevstruct.x1val<<"  x2val="<<msdevstruct.x2val<<"  y1val="<<msdevstruct.y1val<<endl;
  cout<<"pvers=("<<msdevstruct.msd_track_pvers.X()<<", "<<msdevstruct.msd_track_pvers.Y()<<", "<<msdevstruct.msd_track_pvers.Z()<<")  r0=("<<msdevstruct.msd_track_r0pos.X()<<", "<<msdevstruct.msd_track_r0pos.Y()<<", "<<msdevstruct.msd_track_r0pos.Z()<<")"<<endl;

return;
}  

void fitMSDgraph(){
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
bool bmreadevent(TTreeReader &bmReader, BM_evstruct &bmevent, TTreeReaderValue<int> &evnumreader,   TTreeReaderValue<int> &timeacqreader, TTreeReaderValue<double> &trackchi2reader, TTreeReaderValue<double> &pversxreader,   TTreeReaderValue<double> &pversyreader, TTreeReaderValue<double> &pverszreader,   TTreeReaderValue<double> &r0xreader,   TTreeReaderValue<double> &r0yreader,   TTreeReaderValue<double> &rdriftreader,   TTreeReaderValue<double> &residualreader,   TTreeReaderValue<double> &hittimereader,   TTreeReaderValue<int> &planereader,  TTreeReaderValue<int> &viewreader,   TTreeReaderValue<int> &cellreader){
  
  if(debug)
    cout<<"I'm in bmreadevent"<<endl;
  
  clean_bmevstruct(bmevent, false);
  if(bmReader.GetCurrentEntry()==0)
    bmReader.Next(); 
   
  if(debug)
    cout<<"bmreadevent:: old evnum="<<bmevent.evnum<<",  new evnum="<<*evnumreader<<endl;  
   
  int evhitnum=0;
  TVector3 tmp_tvector3;
  do{ 
    if(bmevent.evnum!=*evnumreader && evhitnum!=0)
      cout<<"bmreadevent:: Something is wrong!!! the old evnum is "<<bmevent.evnum<<", the new evnum is "<<*evnumreader<<endl; 
    bmevent.evnum=*evnumreader;
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

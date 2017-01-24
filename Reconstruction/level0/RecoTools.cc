#define RecoTools_cxx

#include "RecoTools.h"

#include <TCanvas.h>
#include <TError.h>
#include <TMath.h>
#include <TChain.h>
#include <TH2.h>
#include <TH1.h>

#include "TAGaction.hxx"
#include "TAGview.hxx"

//Viewers
#include "TAGvieHorzFIRST.hxx"
#include "TAGvieHorzMCIR.hxx"

//MC block
#include "TAGntuMCeve.hxx"
#include "TAGactNtuMCeve.hxx"
#include "TAGntuMCmimo.hxx"
#include "TAGactNtuMCmimo.hxx"


//Interaction region
#include "TAIRdatRaw.hxx"
#include "TAIRactNtuMC.hxx"

//Beam Monitor
#include "TABMparGeo.hxx"
#include "TABMparCon.hxx"
#include "TABMntuRaw.hxx"
#include "TABMntuTrack.hxx"
#include "TABMactNtuMC.hxx"
#include "TABMactNtuTrack.hxx"
#include "TABMvieTrackFOOT.hxx"

//Vertex
#include "TAVTparMap.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"
#include "TAVTparCal.hxx"
#include "TAVTdatRaw.hxx"
#include "TAVTntuRaw.hxx"
#include "TAVTntuCluster.hxx"
#include "TAVTntuTrack.hxx"
#include "TAVTntuVertex.hxx"
#include "TAVTactNtuVertex.hxx"
#include "TAVTactNtuVertexPD.hxx"
#include "TAVTactNtuMC.hxx"
#include "TAVTactNtuClusterF.hxx"
#include "TAVTactNtuTrack.hxx"
#include "TAVTactNtuTrackH.hxx"
#include "TAVTactNtuTrackF.hxx"

//Inner tracker
#include "TAITparMap.hxx"
#include "TAITparGeo.hxx"
#include "TAITparConf.hxx"
#include "TAITparCal.hxx"
#include "TAITdatRaw.hxx"
#include "TAITntuRaw.hxx"
#include "TAITntuCluster.hxx"
#include "TAITntuTrack.hxx"
#include "TAITactNtuMC.hxx"
#include "TAITactNtuClusterF.hxx"
#include "TAITactNtuTrack.hxx"
#include "TAITactNtuTrackH.hxx"
#include "TAITactNtuTrackF.hxx"
 
//Drift Chamber
#include "TADCparGeo.hxx"
#include "TADCparCon.hxx"
#include "TADCntuRaw.hxx"
#include "TADCntuTrack.hxx"
#include "TADCactNtuMC.hxx"
#include "TADCactNtuTrack.hxx"
#include "TADCvieTrackFIRST.hxx"

//Tof Wall (scintillator)
#include "TATWparMap.hxx"
#include "TATWdatRaw.hxx"
#include "TATWactNtuMC.hxx"

//Calorimeter
#include "TACAparMap.hxx"
#include "TACAdatRaw.hxx"
#include "TACAactNtuMC.hxx"


#include "foot_geo.h"

#include <iostream>
#include <vector>

using namespace std;

RecoTools::RecoTools(int d, TString istr, bool list, TString ostr, TString wd, int nev,
		     TFile *hf) {

  my_files.clear();
  m_debug = d;
  m_oustr = ostr;
  m_wd = wd;

  ifstream inS; 
  char bufConf[200]; char fname[400];
  if(!list) {
    m_instr = istr;
    my_files.push_back(m_instr.Data());
  } else {
    m_instr = istr;
    inS.open(m_instr.Data());
    while (inS.getline(bufConf, 200, '\n')) {
      sscanf(bufConf,"%s",fname);
      my_files.push_back(fname);
    }
  }
  m_nev = nev;
  m_flaghisto = false;
  m_fullmoni = kFALSE;
  //  gErrorIgnoreLevel = kWarning;
  //  gErrorIgnoreLevel = kInfo;
  gErrorIgnoreLevel = kError;
  //  gErrorIgnoreLevel = kFatal;

  m_hf = hf;

}


void RecoTools::RecoLoop(TAGroot *tagr, int fr) {

  fGeoTrafo = new TAGgeoTrafo();
  TString filename = m_wd + "/FOOT_geo.map";
  fGeoTrafo->InitGeo(filename.Data());


  //  TTree *tree = 0;
  TChain *tree = new TChain("EventTree");
  for(int ifi=0; ifi<my_files.size(); ifi++) {
    tree->Add(my_files.at(ifi).data());
    cout<<"Adding :: "<<my_files.at(ifi).data() << " file"<<endl;
  }

  //  TTree* tree = (TTree*)gDirectory->Get("EventTree");

  if(m_debug) cout<<" Creating Geo "<<endl;
  Geometry *my_G = new Geometry(); 
  if(m_debug) cout<<" Creating Sig "<<endl;
  Segnale *my_S = new Segnale(my_G); 
  if(m_debug) cout<<" Creating Eve "<<endl;
  Evento *ev =  new Evento();
  Trigger *tr = new Trigger();

  //  vector < Int_t > RegNumber;
  //  vector < TString > RegName;

  //get FLUKA region number-name correspondance
  if(m_debug) cout<<" Reading Geo info "<<endl;

  /*
  filename = m_wd + "FOOT.reg";
  my_G->InitRegions(filename.Data());
  RegName = my_G->GetRegionNames();
  RegNumber = my_G->GetRegionNumbers();
  */
  EVENT_STRUCT evStr;
  TAGpadGroup* pg = new TAGpadGroup();

  ev->FindBranches(tree,&evStr);

  if(m_debug) cout<<" Found branches "<<endl;

  tagr->SetCampaignNumber(100);
  tagr->SetRunNumber(1);

  my_out = new TAGactTreeWriter("my_out");
 
  /*
    Setting up the detectors that we want to decode.
  */
  bool m_doEvent = kTRUE;
  bool m_doBM = kTRUE;
  bool m_doDC = kTRUE;
  bool m_doIR = kTRUE;
  bool m_doTW = kTRUE;
  bool m_doCA = kTRUE;
  bool m_doInnerTracker = kTRUE;
  bool m_doVertex = kFALSE;

  if(m_doEvent) 
    FillMCEvent(&evStr);

  if(m_doBM)
    FillMCBeamMonitor(&evStr);

  if(m_doIR)
    FillMCInteractionRegion(&evStr);

  if(m_doInnerTracker)
    FillMCInnerTracker(&evStr);

  if(m_doVertex)
    FillMCVertex(&evStr);

  if(m_doDC)
    FillMCDriftChamber(&evStr);

  if(m_doTW)
    FillMCTofWall(&evStr);

  if(m_doCA)
    FillMCCalorimeter(&evStr);

  tagr->AddRequiredItem("my_out");
  tagr->Print();
  if (my_out->Open(m_oustr, "RECREATE")) return;

  if (m_flaghisto && m_doVertex) {
    mya_vtraw->SetHistogramDir(my_out->File());
    mya_vtclus->SetHistogramDir(my_out->File());
    mya_vttrack->SetHistogramDir(my_out->File());
  }
  //Initialize the Event Display for the Music

  if(m_doBM) {
    //    DisplayBeamMonitor(pg);
    
    //    DisplayIRMonitor(pg,&evStr);
  }


  /*
    The event Loop
  */
  tagr->BeginEventLoop();

  Long64_t nentries = tree->GetEntries();

  Long64_t nbytes = 0, nb = 0;
  char flag[200]; bool tobedrawn = kFALSE;
  if(m_nev != 0) nentries = m_nev;

  if(m_debug) cout<<" Starting Event Loop "<<endl;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if(m_debug) cout<<" New Eve "<<endl;
    nb = tree->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;

    tagr->NextEvent();

    if(m_doBM) {
      //      MonitorBM();
      if(m_doVertex) {
	MonitorBMVTMat();
      
	CalibBMVT();
      }
    }

    if(!(jentry%fr))
      cout<<"Processed:: "<<jentry<<" evts!"<<endl;
    
    
    TAGntuMCeve*  p_ntumceve = 
      (TAGntuMCeve*)   myn_mceve->GenerateObject();
    
    int nhitmc = p_ntumceve->nhit;

    //do some MC check
    //to be moved to framework
    if(m_doVertex)
      AssociateHitsToParticle();

    if(m_debug) {

      //Pixels stuff
      TAVTntuRaw*  p_nturaw = 
	(TAVTntuRaw*)   myn_vtraw->GenerateObject();
      
      for(int is = 0; is<8; is++) {
	//      cout<<"Npixel:: "<<p_nturaw->GetPixelsN(is)<<endl;
	for(int i=0; i<p_nturaw->GetPixelsN(is); i++){
	  TAVTntuHit* hit = p_nturaw->GetPixel(is,i);
	  TVector3 apos = hit->GetPosition();
	}
      }
      
      TAVTntuCluster*  p_ntuclus = 
	(TAVTntuCluster*)   myn_vtclus->GenerateObject();
      int i_ncl;
      //Displays the Clusters in the VTX detector
      for(int is=0; is<8; is++) {
	i_ncl = p_ntuclus->GetClustersN(is);
	
	for (Int_t i_cl = 0; i_cl < i_ncl; i_cl++) {
	  
	  TAVTcluster *acl = p_ntuclus->GetCluster(is,i_cl);
	  TVector3 myG = acl->GetPositionG();
	  TVector3 myLG(myG.X()*1./10000.,myG.Y()*1./10000.,myG.Z()*1./10000.);
	  TVector3 myR = fGeoTrafo->FromVTLocalToGlobal(myLG);
	  
	}
      }
    }
    /*
    //Trigger Stuff
    tr->Reset();
    tr->ComputeTrigger(&evStr, &RegName, &RegNumber);
    
    if (!(jentry % 10) && m_debug)
      cout << "Trigger: Top Level:" << tr->IsTopLevel() << " Wanted:"
	   << tr->IsWanted()<< " ST:"
	   << tr->IsStartCounter() << " SE:" << tr->IsSmallEndCap()
	   << " BE:" << tr->IsBigEndCap() << " BR:" << tr->IsBarrel()
	   << " TWS:" << tr->IsTOFWallFSingle() << " TWM:"
	   << tr->IsTOFWallFMulti() << " ON:" << tr->IsONION() << endl;

    for(int iTr = 0; iTr<evStr.trn; iTr++) { 
      if(m_debug) cout<<" Trk Chg:: "<<evStr.trcha[iTr]<<endl; 
    }


    if(tr->IsWanted()) tobedrawn = kTRUE;
    */

    tobedrawn = kTRUE;

    //Debugging of Vertex
    if(m_doVertex) {

      double recAngle_pl(-4), carbMCAngle_pl(-4), pixelAngle_pl(-4);
      
      //VTX tracks
      TAVTntuTrack* p_vttrk = 
	(TAVTntuTrack*) myn_vtrk->GenerateObject();
      for(int i= 0; i<p_vttrk->GetTracksN(); i++) {
	TAVTtrack* t_track = p_vttrk->GetTrack(i);
	TAVTline line = t_track->GetTrackLine();
	TVector3 angVtx = line.GetSlopeZ();
	//      cout<<"Angolo ricostruito "<<angVtx.Theta()<<endl;
	recAngle_pl = angVtx.Theta();
      }
      
      //Momentum in the track block
      for(int iTr = 0; iTr<evStr.TRn; iTr++) { 
	if(evStr.TRcha[iTr] == 6) {
	  TVector3 true_P(evStr.TRipx[iTr],evStr.TRipy[iTr],evStr.TRipz[iTr]);
	  carbMCAngle_pl = true_P.Theta();
	  //	cout<<"size:: "<<carbMCAngle_pl-recAngle_pl<<endl;
	  ((TH1D*)gDirectory->Get("ResoMCCarbons"))->Fill(carbMCAngle_pl-recAngle_pl);
	} 
      }
      
      //Momentum in the pixel region
      /*
      double selecDiff(-4); int idxSen(0); char name[200];
      for(int iTr = 0; iTr<evStr.miSigN; iTr++) { 
	TVector3 truepix_P(evStr.miSigpX[iTr],evStr.miSigpY[iTr],evStr.miSigpZ[iTr]);
	if(selecDiff != truepix_P.Theta()) {
	  selecDiff = truepix_P.Theta();
	  pixelAngle_pl = truepix_P.Theta();
	  //	cout<<" "<<pixelAngle_pl<<endl;
	  if(idxSen<4) {
	    sprintf(name,"ResoMCPixel_%d",idxSen);
	    ((TH1D*)gDirectory->Get(name))->Fill(pixelAngle_pl-recAngle_pl);
	  }
	  idxSen++;
	}
      }
      
      tobedrawn = kTRUE;
      //    if(m_debug) {
      double init = -100; double refx(-100);
      for (Int_t i = 0; i < evStr.miSigN; i++) {
        if(fabs(evStr.miSigX[i] - refx)>0.00001) {      
	  //	  printf("MIMO::: Fluka Id %d X %f Y %f Z %f\n", evStr.miSigChip[i]-1, evStr.miSigX[i], evStr.miSigY[i], evStr.miSigZ[i]);
	  refx = evStr.miSigX[i];
	  
	  TVector3 ip = TVector3(evStr.miSigpX[i],evStr.miSigpY[i],evStr.miSigpZ[i]);
	  
	  //	  cout<<"Track Angle:: "<<ip.Theta()<<" X:: "<<ip.Unit().X()<<" Y:: "<<ip.Unit().Y()<<endl;
	}
	
      }
      */
      //    }
    }

    if (!pg->IsEmpty() && tobedrawn && !(jentry%fr)) {
      pg->Modified();
      pg->Update();
      
      sprintf(flag,"plots/%s_%d","Test_MC",(int)jentry);
      pg->Print(flag);
    }


    if(m_debug) cout<<" Loaded Event:: "<<jentry<<endl;

  }
  
  tagr->EndEventLoop();
  if (m_flaghisto) {
    mya_vtraw->WriteHistogram();
    mya_vtclus->WriteHistogram();
    mya_vttrack->WriteHistogram();
  }
  my_out->Print();
  my_out->Close();

  return;
}


void RecoTools::AssociateHitsToParticle() {

  
  TAGntuMCeve*  p_ntumceve = 
    (TAGntuMCeve*)   myn_mceve->GenerateObject();
  
  vector<int> FragIdxs;
  int nhitmc = p_ntumceve->nhit;
  for(int i=0; i<nhitmc; i++){
    TAGntuMCeveHit *myPart = p_ntumceve->Hit(i);

    int part_reg = myPart->Reg();

    //Require that particle is produced inside the TGT
    if(part_reg == 3) {
      FragIdxs.push_back(i);
    }
  }

  //Pixels stuff
  TAVTntuRaw*  p_nturaw = 
    (TAVTntuRaw*)   myn_vtraw->GenerateObject();
  
  int tmp_vtxid(0);
  TAVTntuHit* hit;

  //inner tracker stuff
  TAITntuRaw*  p_itnturaw = 
    (TAITntuRaw*)   myn_itraw->GenerateObject();
  
  int tmp_itid(0);
  TAITntuHit* hitIT;

  for(int t_frg = 0; t_frg<FragIdxs.size(); t_frg++) {

    //Check VTX pixels
    for(int i=0; i<p_nturaw->GetPixelsN(0); i++){
      hit = p_nturaw->GetPixel(0,i);
      tmp_vtxid = hit->GetMCid()-1;
      if(tmp_vtxid == FragIdxs.at(t_frg)){
	if(m_debug) cout<<" Vtx hit associated to part "<<t_frg<<" That is a:: "<<p_ntumceve->Hit(t_frg)->FlukaID()<<"and has charge, mass:: "<<p_ntumceve->Hit(t_frg)->Chg()<<" "<<p_ntumceve->Hit(t_frg)->Mass()<<" "<<endl;
      }
    }

    //Check IT pixels
    for(int i=0; i<p_itnturaw->GetPixelsN(0); i++){
      hitIT = p_itnturaw->GetPixel(0,i);
      tmp_itid = hitIT->GetMCid()-1;
      if(tmp_itid == FragIdxs.at(t_frg)){
	if(m_debug) cout<<" IT hit associated to part "<<t_frg<<" That is a:: "<<p_ntumceve->Hit(t_frg)->FlukaID()<<"and has charge, mass:: "<<p_ntumceve->Hit(t_frg)->Chg()<<" "<<p_ntumceve->Hit(t_frg)->Mass()<<" "<<endl;
      }
    }

  }//Loop on fragments

  return;

}

void RecoTools::DisplayIRMonitor(TAGpadGroup* pg, EVENT_STRUCT *myStr) {

  TCanvas *c_irhview, *c_irhview_z;

  c_irhview = new TCanvas("irhview", "IR - horizontal view",20,20,1200,1200);
  pg->AddPad(c_irhview);

  TAGview* pirh_view = new TAGvieHorzMCIR(myn_bmtrk, myn_bmraw,
					  myp_bmgeo, myn_vtclus,
					  myn_vtrk, myp_vtgeo, myStr);
  
  pirh_view->Draw();

  c_irhview_z = new TCanvas("irhview_z", "IR - Zoom horizontal view",20,20,1200,1200);
  pg->AddPad(c_irhview_z);

  TAGview* pirh_view_z = new TAGvieHorzMCIR(myn_bmtrk, myn_bmraw,
					    myp_bmgeo, myn_vtclus,
					    myn_vtrk, myp_vtgeo, myStr);
  
  pirh_view_z->Draw("zoom");

  return;
}

void RecoTools::DisplayBeamMonitor(TAGpadGroup* pg) {

  TCanvas *c_bmhview;

  c_bmhview = new TCanvas("bmhview", "Beam Monitor - horizontal view",20,20,700,900);
  pg->AddPad(c_bmhview);

  TAGview* pbmh_view = new TABMvieTrackFOOT(myn_bmtrk,
					     myn_bmraw,
					     myp_bmgeo);
  pbmh_view->Draw();


  return;
}


void RecoTools::FillMCEvent(EVENT_STRUCT *myStr) {

  /*Ntupling the general MC event information*/
  myn_mceve    = new TAGdataDsc("myn_mceve", new TAGntuMCeve());

  new TAGactNtuMCeve("an_mceve", myn_mceve, myStr);

  my_out->SetupElementBranch(myn_mceve,     "mceve.");


  /*Ntupling the general MC mimosa information*/
  myn_mcmimo    = new TAGdataDsc("myn_mcmimo", new TAGntuMCmimo());

  new TAGactNtuMCmimo("an_mcmimo", myn_mcmimo, myStr);

  my_out->SetupElementBranch(myn_mcmimo,     "mcmimo.");

  return;

}

void RecoTools::FillMCBeamMonitor(EVENT_STRUCT *myStr) {
  
  /*Ntupling the MC Beam Monitor information*/
  myn_bmraw    = new TAGdataDsc("myn_bmraw", new TABMntuRaw());
  myp_bmcon  = new TAGparaDsc("myp_bmcon", new TABMparCon());

  initBMCon(myp_bmcon);

  myp_bmgeo  = new TAGparaDsc("p_bmgeo", new TABMparGeo());

  initBMGeo(myp_bmgeo);

  new TABMactNtuMC("an_bmraw", myn_bmraw, myp_bmcon, myp_bmgeo, myStr);

  my_out->SetupElementBranch(myn_bmraw,     "bmrh.");

  //  myn_bmtrk    = new TAGdataDsc("myn_bmtrk", new TABMntuTrack());

  //  new TABMactNtuTrack("an_bmtrk", myn_bmtrk, myn_bmraw, myp_bmgeo, myp_bmcon);

  //  my_out->SetupElementBranch(myn_bmtrk,     "bmtrk.");

  return;
}

void RecoTools::FillMCDriftChamber(EVENT_STRUCT *myStr) {
  
  /*Ntupling the MC Drift Chamber information*/
  myn_dcraw    = new TAGdataDsc("myn_dcraw", new TADCntuRaw());
  myp_dccon  = new TAGparaDsc("myp_dccon", new TADCparCon());

  initDCCon(myp_dccon);

  new TADCactNtuMC("an_dcraw", myn_dcraw, myp_dccon, myStr);

  my_out->SetupElementBranch(myn_dcraw,     "dcrh.");

  myp_dcgeo  = new TAGparaDsc("p_dcgeo", new TADCparGeo());

  initDCGeo(myp_dcgeo); 
  /*
  myn_dctrk    = new TAGdataDsc("myn_dctrk", new TADCntuTrack());

  new TADCactNtuTrack("an_dctrk", myn_dctrk, myn_dcraw, myp_dcgeo, myp_dccon);

  my_out->SetupElementBranch(myn_dctrk,     "dctrk.");
  */
  return;
}

void RecoTools::FillMCTofWall(EVENT_STRUCT *myStr) {
  
  /*Ntupling the MC Tof Wall information*/
  myn_twraw    = new TAGdataDsc("myn_twraw", new TATWdatRaw());

  new TATWactNtuMC("an_twraw", myn_twraw, myStr);

  my_out->SetupElementBranch(myn_twraw,     "twrh.");

  return;
}

void RecoTools::FillMCCalorimeter(EVENT_STRUCT *myStr) {
  
  /*Ntupling the MC Calorimeter information*/
  myn_caraw    = new TAGdataDsc("myn_caraw", new TACAdatRaw());

  new TACAactNtuMC("an_caraw", myn_caraw, myStr);

  my_out->SetupElementBranch(myn_caraw,     "carh.");

  return;
}

void RecoTools::FillMCInteractionRegion(EVENT_STRUCT *myStr) {
  
  /*Ntupling the MC Beam Monitor information*/
  myn_irraw    = new TAGdataDsc("myn_irraw", new TAIRdatRaw());

  new TAIRactNtuMC("an_irraw", myn_irraw, myStr);

  my_out->SetupElementBranch(myn_irraw,     "irrh.");


  return;
}

void RecoTools::FillMCVertex(EVENT_STRUCT *myStr) {
   
   /*Ntupling the MC Vertex information*/
   myn_vtraw    = new TAGdataDsc("vtRaw", new TAVTntuRaw());
   myn_vtclus   = new TAGdataDsc("vtClus", new TAVTntuCluster());
   myn_vtrk     = new TAGdataDsc("vtTrack", new TAVTntuTrack());
   myn_vtvtx    = new TAGdataDsc("vtVtx", new TAVTntuVertex());

   myp_vtmap    = new TAGparaDsc("vtMap", new TAVTparMap());

   myp_vtconf  = new TAGparaDsc("vtConf", new TAVTparConf());
   TAVTparConf* parconf = (TAVTparConf*) myp_vtconf->Object();
   TString filename = m_wd + "/config/TAVTdetector.cfg";
   parconf->FromFile(filename.Data());

   myp_vtgeo    = new TAGparaDsc("vtGeo", new TAVTparGeo());
   TAVTparGeo* geomap   = (TAVTparGeo*) myp_vtgeo->Object();
   filename = m_wd + "/geomaps/TAVTdetector.map";
   geomap->FromFile(filename.Data());

   myp_vtcal = new TAGparaDsc("vtCal", new TAVTparCal());
   TAVTparCal* cal   = (TAVTparCal*) myp_vtcal->Object();
   filename = m_wd + "/config/TAVTdetector.cal";
   cal->FromFile(filename.Data());
   
   mya_vtraw   = new TAVTactNtuMC("vtActRaw", myn_vtraw, myp_vtgeo, myp_vtmap, myStr);
   mya_vtclus  = new TAVTactNtuClusterF("vtActClus", myn_vtraw, myn_vtclus, myp_vtconf, myp_vtgeo);
   mya_vttrack = new TAVTactNtuTrack("vtActTrack", myn_vtclus, myn_vtrk, myp_vtconf, myp_vtgeo, myp_vtcal);
   // L algo mya_vttrack = new TAVTactNtuTrackF("vtActTrack", myn_vtclus, myn_vtrk, myp_vtconf, myp_vtgeo, myp_vtcal);
   mya_vtvtx   = new TAVTactNtuVertexPD("vtActVtx", myn_vtrk, myn_vtvtx, myp_vtconf, myp_vtgeo, myn_bmtrk);
   //IPA   mya_vtvtx   = new TAVTactNtuVertex("vtActVtx", myn_vtrk, myn_vtvtx, myp_vtconf, myp_vtgeo, myn_bmtrk);

   if (m_flaghisto) {
     mya_vtraw->CreateHistogram();
     mya_vtclus->CreateHistogram();
     mya_vttrack->CreateHistogram();
     mya_vtvtx->CreateHistogram();
   }

   my_out->SetupElementBranch(myn_vtraw, "vtrh.");
   my_out->SetupElementBranch(myn_vtclus, "vtclus.");
   my_out->SetupElementBranch(myn_vtrk, "vtTrack.");
   my_out->SetupElementBranch(myn_vtvtx, "vtVtx.");
}


void RecoTools::FillMCInnerTracker(EVENT_STRUCT *myStr) {
   
   /*Ntupling the MC Vertex information*/
   myn_itraw    = new TAGdataDsc("itRaw", new TAITntuRaw());
   myn_itclus   = new TAGdataDsc("itClus", new TAITntuCluster());
   myn_itrk     = new TAGdataDsc("itTrack", new TAITntuTrack());

   myp_itmap    = new TAGparaDsc("itMap", new TAITparMap());

   myp_itconf  = new TAGparaDsc("itConf", new TAITparConf());
   TAITparConf* parconf = (TAITparConf*) myp_itconf->Object();
   TString filename = m_wd + "/config/TAITdetector.cfg";
   parconf->FromFile(filename.Data());

   myp_itgeo    = new TAGparaDsc("itGeo", new TAITparGeo());
   TAITparGeo* geomap   = (TAITparGeo*) myp_itgeo->Object();
   filename = m_wd + "/geomaps/TAITdetector.map";
   geomap->FromFile(filename.Data());

   myp_itcal = new TAGparaDsc("itCal", new TAITparCal());
   TAITparCal* cal   = (TAITparCal*) myp_itcal->Object();
   filename = m_wd + "/config/TAITdetector.cal";
   cal->FromFile(filename.Data());
   
   mya_itraw   = new TAITactNtuMC("itActRaw", myn_itraw, myp_itgeo, myp_itmap, myStr);
   mya_itclus  = new TAITactNtuClusterF("itActClus", myn_itraw, myn_itclus, myp_itconf, myp_itgeo);
   mya_ittrack = new TAITactNtuTrack("itActTrack", myn_itclus, myn_itrk, myp_itconf, myp_itgeo, myp_itcal);

   if (m_flaghisto) {
     mya_itraw->CreateHistogram();
     mya_itclus->CreateHistogram();
     mya_ittrack->CreateHistogram();
   }

   my_out->SetupElementBranch(myn_itraw, "itrh.");
   my_out->SetupElementBranch(myn_itclus, "itclus.");
   my_out->SetupElementBranch(myn_itrk, "itTrack.");
}


void RecoTools::MonitorBM() {

  m_hf->cd();
  
  char name[200];
  double dist, rho, resi_aft;
  int ce,pl,vi;
  TVector3 bs;
  double my_ux, my_uy, my_uz, my_uzs, ang_x, ang_z;
  TVector3 myVec, vX, vZ;
  vZ.SetXYZ(0,0,1);
  vX.SetXYZ(1,0,0);
  double phi_an, psi_an, phi_an_dg, psi_an_dg;
  double trigtime(-10000);

  TVector3 bmPos = fGeoTrafo->GetBMCenter();
  TVector3 tgPos = fGeoTrafo->GetVTCenter();
  double zDiff  = tgPos.Z() - bmPos.Z();	  

  //Raw Data

  //Total number of events
  ((TH1D*)gDirectory->Get("beammonitor/eff"))->SetBinContent(1,((TH1D*)gDirectory->Get("beammonitor/eff"))->GetBinContent(1)+1);

  double trk_chi2;
  //Look at the Beam Monitor ONLY if there's a trigger!
  Int_t nwirehit[36]; Double_t firsttime[36];
  Int_t nwiretot=0;

  ((TH1D*)gDirectory->Get("beammonitor/eff"))->SetBinContent(2,((TH1D*)gDirectory->Get("beammonitor/eff"))->GetBinContent(2)+1);
  
  // initialization
  
  //Tracks
  TABMntuTrack* p_ntutrk  = 
    (TABMntuTrack*) myn_bmtrk->GenerateObject();
  
  //Hits
  TABMntuRaw* p_ntuhit  = 
    (TABMntuRaw*) myn_bmraw->GenerateObject();
  
  Int_t i_nnhit = p_ntuhit->nhit;
  for (Int_t i = 0; i < i_nnhit; i++) {
    const TABMntuHit* aHi = p_ntuhit->Hit(i);
    
    int tmpv = aHi->View();
    if(tmpv<0)tmpv = 0;
    int chidx = aHi->Cell()+aHi->Plane()*3+tmpv*18;
    
    nwirehit[chidx]++;
    sprintf(name,"beammonitor/t0_tdc_%d",chidx);
    ((TH1D*)gDirectory->Get(name))->Fill(aHi->Timmon());
  }
  
  
  Int_t i_ntrk = p_ntutrk->ntrk;
  
  for (Int_t i_h = 0; i_h < i_ntrk; i_h++) {
    TABMntuTrackTr* trk = p_ntutrk->Track(i_h);
    
    trk_chi2 = trk->GetChi2();
    //    if(!i_h && trk_chi2>20) tobepl = kTRUE;
    
    sprintf(name,"beammonitor/chi2_%d",i_h);
    ((TH1D*)gDirectory->Get(name))->Fill(trk_chi2);
    
    
    my_ux = trk->GetUx();
    my_uy = trk->GetUy();
    my_uz = 0;
    my_uzs = 1. - my_ux*my_ux - my_uy*my_uy;
    if(my_uzs>=0) my_uz = sqrt(my_uzs);
    
    phi_an = atan(my_ux/my_uz);
    psi_an = atan(my_uy/my_uz);
    
    phi_an_dg = phi_an*TMath::RadToDeg();
    psi_an_dg = psi_an*TMath::RadToDeg();
    
    myVec.SetXYZ(my_ux, my_uy, my_uz);
    ang_x = myVec.Angle(vX);
    ang_z = myVec.Angle(vZ);
    bs = trk->PointAtLocalZ(zDiff);
    
    ((TH1D*)gDirectory->Get("beammonitor/z_ang"))->Fill(ang_z);
    
  }
  if(trk_chi2<10 && i_ntrk) {
    ((TH1D*)gDirectory->Get("beammonitor/eff"))->SetBinContent(7,((TH1D*)gDirectory->Get("beammonitor/eff"))->GetBinContent(7)+1);
  }
  if(trk_chi2<5 && i_ntrk) {
    ((TH1D*)gDirectory->Get("beammonitor/eff"))->SetBinContent(6,((TH1D*)gDirectory->Get("beammonitor/eff"))->GetBinContent(6)+1);
  }
  if(trk_chi2<2 && i_ntrk) {
    ((TH1D*)gDirectory->Get("beammonitor/eff"))->SetBinContent(5,((TH1D*)gDirectory->Get("beammonitor/eff"))->GetBinContent(5)+1);
  }
  if(trk_chi2<20 && i_ntrk) {
    ((TH1D*)gDirectory->Get("beammonitor/eff"))->SetBinContent(4,((TH1D*)gDirectory->Get("beammonitor/eff"))->GetBinContent(4)+1);
  }
  
  if(trk_chi2<100 && i_ntrk) {
    ((TH1D*)gDirectory->Get("beammonitor/eff"))->SetBinContent(3,((TH1D*)gDirectory->Get("beammonitor/eff"))->GetBinContent(3)+1);
    //      ((TH1D*)gDirectory->Get("beammonitor/z_ang"))->Fill(ang_z);
    if(m_fullmoni) {
      ((TH2D*)gDirectory->Get("beammonitor/xy_pr_ang"))->Fill(phi_an,psi_an);
      ((TH2D*)gDirectory->Get("beammonitor/xy_pr_ang_dg"))->Fill(phi_an_dg,psi_an_dg);
    }
    ((TH1D*)gDirectory->Get("beammonitor/x_ang_dg"))->Fill(phi_an_dg);
    ((TH1D*)gDirectory->Get("beammonitor/y_ang_dg"))->Fill(psi_an_dg);
    
    ((TH1D*)gDirectory->Get("beammonitor/xpr_ang"))->Fill(phi_an);
    ((TH1D*)gDirectory->Get("beammonitor/ypr_ang"))->Fill(psi_an);
    ((TH1D*)gDirectory->Get("beammonitor/x_pro"))->Fill(bs.X());
    ((TH1D*)gDirectory->Get("beammonitor/y_pro"))->Fill(bs.Y());
    ((TH1D*)gDirectory->Get("beammonitor/xy_dist"))->Fill(bs.X(),bs.Y());
  }
  
  //    if(i_ntrk && p_ntutrk->GetChi2()<100) {
  if(i_ntrk) {
    Int_t i_nhit = p_ntuhit->nhit;
    for (Int_t i_h = 0; i_h < i_nhit; i_h++) {
      TABMntuHit* hit = p_ntuhit->Hit(i_h);
      if(hit->TrkAss()) {
	rho = 	hit->GetRho(); 
	dist = 	hit->Dist();
	ce = 	hit->Cell();
	pl = 	hit->Plane();
	vi = 	hit->View();
	if(vi<0) vi = 0;
	//	  cout<<" "<<rho<<" "<<dist<<" "<<" "<<ce<<" "<<pl<<" "<<vi<<endl;
	resi_aft = rho-dist;
	if(!TMath::IsNaN(rho)) {
	  ((TH1D*)gDirectory->Get("beammonitor/resi_aft"))->Fill(resi_aft);
	  ((TH2D*)gDirectory->Get("beammonitor/resi_vs_t"))->Fill(resi_aft,hit->Tdrift());
	  if(m_fullmoni) 
	    ((TH2D*)gDirectory->Get("beammonitor/resi_vs_zmt"))->Fill(resi_aft,hit->Tdrift());
	  ((TH2D*)gDirectory->Get("beammonitor/resi_vs_r"))->Fill(resi_aft,hit->Dist());
	  if(m_fullmoni) {
	    if(ang_z>0.02) {
	      ((TH1D*)gDirectory->Get("beammonitor/resian_aft"))->Fill(resi_aft);
	      ((TH2D*)gDirectory->Get("beammonitor/resian_vs_t"))->Fill(resi_aft,hit->Tdrift());
	      ((TH2D*)gDirectory->Get("beammonitor/resian_vs_zmt"))->Fill(resi_aft,hit->Tdrift());
	      ((TH2D*)gDirectory->Get("beammonitor/resian_vs_r"))->Fill(resi_aft,hit->Dist());
	    }
	  }
	  if(m_fullmoni) {
	    sprintf(name,"beammonitor/resi_aft_%d_%d_%d",ce,pl,vi);
	    ((TH1D*)gDirectory->Get(name))->Fill(resi_aft);
	    sprintf(name,"beammonitor/resi_vs_t_%d_%d_%d",ce,pl,vi);
	    ((TH2D*)gDirectory->Get(name))->Fill(resi_aft,hit->Tdrift());
	    sprintf(name,"beammonitor/resi_vs_zmt_%d_%d_%d",ce,pl,vi);
	    ((TH2D*)gDirectory->Get(name))->Fill(resi_aft,hit->Tdrift());
	    sprintf(name,"beammonitor/resi_vs_r_%d_%d_%d",ce,pl,vi);
	    ((TH2D*)gDirectory->Get(name))->Fill(resi_aft,hit->Dist());
	  }
	}
      }
    }
  }

  return;

}

void RecoTools::MonitorBMVTMat() {

  m_hf->cd();

  TVector3 bmPos = fGeoTrafo->GetBMCenter();
  TVector3 tgPos = fGeoTrafo->GetVTCenter();
  double zDiff  = tgPos.Z() - bmPos.Z() - 0.7;	  
  double my_ux, my_uy, my_uz, my_uzs, phi_an, psi_an;
  //Tracks
  TABMntuTrack* p_ntutrk  = 
    (TABMntuTrack*) myn_bmtrk->GenerateObject();

  TAVTntuTrack* p_vttrk = 
    (TAVTntuTrack*) myn_vtrk->GenerateObject();
  

  vector<TVector3> myOrigs;
  for(int i= 0; i<p_vttrk->GetTracksN(); i++) {
    TAVTtrack* t_track = p_vttrk->GetTrack(i);
    TVector3 t_origin = t_track->GetTrackLine().GetOrigin();
    //Origin in cm
    t_origin *= 1./10000;
    myOrigs.push_back(t_origin);
  }
  double distance;
  for(int ia =0; ia<(int)myOrigs.size(); ia++) {
    for(int ib = ia+1; ib<(int)myOrigs.size(); ib++) {

      distance = sqrt(pow(myOrigs.at(ia).X()-myOrigs.at(ib).X(),2)+
		      pow(myOrigs.at(ia).Y()-myOrigs.at(ib).y(),2));
      
      ((TH1D*)gDirectory->Get("beammonitor/vtdist"))->Fill(distance);
      ((TH2D*)gDirectory->Get("beammonitor/vtdist_nt"))->Fill(distance,myOrigs.size());
    }
  }

  double phi_bm, phi_vt_1, psi_bm, psi_vt_1;
  double phi_vt_2, psi_vt_2;

  TAGntuMCeve* p_mceve  = 
    (TAGntuMCeve*) myn_mceve->GenerateObject();

  if(p_ntutrk->ntrk) {
    TABMntuTrackTr* bmTrack  = p_ntutrk->Track(0);
    double chi2 = bmTrack->GetChi2();
    
    TVector3 trackPos = bmTrack->PointAtLocalZ(zDiff);
    
    TVector3 trackCen = bmTrack->PointAtLocalZ(0);
    
    TVector3 GlbBM = fGeoTrafo->FromBMLocalToGlobal(trackPos);
    
    TVector3 GlbBMCent = fGeoTrafo->FromBMLocalToGlobal(trackCen);
    my_ux = bmTrack->GetUx();
    my_uy = bmTrack->GetUy();
    my_uz = 1;
    my_uzs = 1. - my_ux*my_ux - my_uy*my_uy;
    if(my_uzs>=0) my_uz = sqrt(my_uzs);
    
    phi_an = atan(my_ux/my_uz);
    psi_an = atan(my_uy/my_uz);
    
    //from local to global in the BM
    TVector3 versBM (my_ux/my_uz,my_uy/my_uz,1);
    //	TVector3 angGlbBM = fGeoTrafo->VecFromGlobalToBMLocal(versBM);
    TVector3 angGlbBM = fGeoTrafo->VecFromBMLocalToGlobal(versBM);
    
    double phi_BM_GL = angGlbBM.X()/angGlbBM.Z();
    double psi_BM_GL = angGlbBM.Y()/angGlbBM.Z();
    
    //requires only 1 track in the vtx
    vector <double> dis_vec;
    dis_vec.clear(); 
    double dis_1, dis_2;

    for(int i= 0; i<p_vttrk->GetTracksN(); i++) {
      TAVTtrack* track = p_vttrk->GetTrack(i);
      
      TAVTline line = track->GetTrackLine();
      TVector3 angVtx = line.GetSlopeZ();
      double phi_Vtx = angVtx.X()/angVtx.Z();
      double psi_Vtx = angVtx.Y()/angVtx.Z();
      //      cout<<"VTXref:: " << phi_Vtx<<" "<< psi_Vtx<<endl;
      //      cout<<"VTXref:: " << angVtx.Theta()<< " "<<angVtx.Phi()<<endl;
      //from local to global in the vtx
      //	TVector3 angGlbVtx = fGeoTrafo->VecFromGlobalToVTLocal(angVtx);
      TVector3 angGlbVtx = fGeoTrafo->VecFromVTLocalToGlobal(angVtx);
      double phi_Vtx_GL = angGlbVtx.X()/angGlbVtx.Z();
      double psi_Vtx_GL = angGlbVtx.Y()/angGlbVtx.Z();

      //      //      cout<<"GLBref:: " << phi_Vtx_GL<<" "<< psi_Vtx_GL<<endl;
      //      cout<<"GLBref:: " << angGlbVtx.Theta()<< " "<<angGlbVtx.Phi()<<endl;

      /*  
	  TVector3 origin_chk = track->Intersection(0);      
	  origin_chk *= 1./10000;
	  TVector3 GlbOrigin_chk = fGeoTrafo->FromVTLocalToGlobal(origin_chk);
	  cout<<"orichk:: "<<origin_chk.X()<<" "<<GlbOrigin_chk.X()<<endl;
      */
      
      TVector3 origin = track->GetTrackLine().GetOrigin();
      origin *= 1./10000;
      TVector3 GlbOrigin = fGeoTrafo->FromVTLocalToGlobal(origin);

      double rec_Angle = (track->GetTrackLine()).GetTheta()*TMath::DegToRad();

      TVector3 res = GlbOrigin - GlbBM;
      TVector3 resCen = GlbOrigin - GlbBMCent;

      for(int ih=0; ih<p_mceve->nhit; ih++) {
	TAGntuMCeveHit* hit = p_mceve->Hit(ih);
	
	double chgMC = hit->Chg();
	//	maMC = hit->Mass();
	double angMC = (hit->InitP()).Theta();

	if(chgMC == 6) {
	  //	  cout<<res.X()<<" "<<res.Y()<<" "<<rec_Angle<<" "<<angMC<<endl;
	  ((TH2D*)gDirectory->Get("beammonitor/resx_vs_angRes"))->Fill(res.X(),-rec_Angle+angMC);
	  ((TH2D*)gDirectory->Get("beammonitor/resy_vs_angRes"))->Fill(res.Y(),-rec_Angle+angMC);
	}
      }

      
      if(chi2<10 && p_vttrk->GetTracksN()==1 && m_fullmoni) {
	((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_ang_chi10_2D_x"))->Fill(phi_BM_GL,phi_Vtx_GL);
	((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_ang_chi10_2D_y"))->Fill(psi_BM_GL,psi_Vtx_GL);
      }


      phi_bm = -0.011+0.266*phi_BM_GL;
      psi_bm = -0.0067+0.3938*psi_BM_GL;
      
      if(p_vttrk->GetTracksN()==2) 	{
	if(i == 0) {
	  phi_vt_1 = phi_Vtx_GL;
	  psi_vt_1 = psi_Vtx_GL;
	}
	if(i == 1) {
	  phi_vt_2 = phi_Vtx_GL;
	  psi_vt_2 = psi_Vtx_GL;
	}
      }
      if(m_fullmoni) {
	if(chi2<2 && p_vttrk->GetTracksN()==1) {
	  ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_ang_chi2_2D_x"))->Fill(phi_BM_GL,phi_Vtx_GL);
	  ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_ang_chi2_2D_y"))->Fill(psi_BM_GL,psi_Vtx_GL);
	}
	
	if(chi2<2 && p_vttrk->GetTracksN()==1 && phi_BM_GL>=-0.5*pow(10.,-2) && phi_BM_GL<=0*pow(10.,-3)) {
	  ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_ang_chi2_2D_x_cut0.5"))->Fill(phi_BM_GL,phi_Vtx_GL);
	  ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_ang_chi2_2D_x_cut1"))->Fill(phi_BM_GL,phi_Vtx_GL);
	}
      }
      //Residuals on the target
      ((TH1D*)gDirectory->Get("beammonitor/bmvt_resx"))->Fill(res.X());
      if(p_vttrk->GetTracksN()==1) 	{
	((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resx"))->Fill(res.X());
      } else {
	dis_vec.push_back(sqrt(pow(res.X(),2)+pow(res.Y(),2)));
      }

      if(p_vttrk->GetTracksN()==2) 	{
	if(i == 0) 
	  dis_1 = sqrt(pow(res.X(),2)+pow(res.Y(),2));
	if(i == 1)
	  dis_2 = sqrt(pow(res.X(),2)+pow(res.Y(),2));
      }

      if(m_fullmoni) {
	if(chi2<2) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_res2x"))->Fill(res.X());
	  if(p_vttrk->GetTracksN()==1) 	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res2x"))->Fill(res.X());
	}
	
	if(chi2<20) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_res20x"))->Fill(res.X());
	  if(p_vttrk->GetTracksN()==1) 	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res20x"))->Fill(res.X());
	}
	
	if(chi2<10) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_res10x"))->Fill(res.X());
	  if(p_vttrk->GetTracksN()==1) 	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res10x"))->Fill(res.X());
	}
      }
      if(chi2<5) {
	((TH1D*)gDirectory->Get("beammonitor/bmvt_res5x"))->Fill(res.X());
	if(p_vttrk->GetTracksN()==1) {	
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res5x"))->Fill(res.X());
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_vtx5x"))->Fill(GlbOrigin.X());
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_bmx5x"))->Fill(GlbBM.X());
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_bmcex5x"))->Fill(GlbBMCent.X());
	}

      }
      
      ((TH1D*)gDirectory->Get("beammonitor/bmvt_resy"))->Fill(res.Y());
      if(p_vttrk->GetTracksN()==1) 	
	((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resy"))->Fill(res.Y());
      
      if(m_fullmoni) {
	if(chi2<20) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_res20y"))->Fill(res.Y());
	  if(p_vttrk->GetTracksN()==1) 	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res20y"))->Fill(res.Y());
	}
	if(chi2<15) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_res15y"))->Fill(res.Y());
	  if(p_vttrk->GetTracksN()==1) {	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res15y"))->Fill(res.Y());
	  }
	}
	
	if(chi2<2) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_res2y"))->Fill(res.Y());
	  if(p_vttrk->GetTracksN()==1) 	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res2y"))->Fill(res.Y());
	}
	
	
	if(chi2<10) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_res10y"))->Fill(res.Y());
	  if(p_vttrk->GetTracksN()==1) 	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res10y"))->Fill(res.Y());
	}
      }
      
      if(chi2<5) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_res5y"))->Fill(res.Y());
	  if(p_vttrk->GetTracksN()==1) {	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res5y"))->Fill(res.Y());
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_vtx5y"))->Fill(GlbOrigin.Y());
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_bmx5y"))->Fill(GlbBM.Y());
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_bmcex5y"))->Fill(GlbBMCent.Y());
	  }
      }
      
      //Rediduals on X and Y with ANGLE CONDITION
      
      if(chi2<10 && phi_an>=-0.5*pow(10.,-3) && phi_an<=0.5*pow(10.,-3)) {
	if(p_vttrk->GetTracksN()==1) 	
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res2x_ang"))->Fill(res.X());
	/*	cout << "bmvt_1t_res2x_ang "<<res.X()<<" chi2 "<< chi2 << endl;*/
      }
      
      if(chi2<10 && psi_an>=-0.5*pow(10.,-3) && psi_an<=0.5*pow(10.,-3)) {
	if(p_vttrk->GetTracksN()==1) 	
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res2y_ang"))->Fill(res.Y());
	
	/*	cout << "bmvt_1t_res2y_ang "<<res.Y()<<" chi2 "<< chi2 << endl;*/
      }
      //Rediduals on X and Y with ANGLE CONDITION - CHI2 vs X
      
      if(chi2<10 && phi_an>=-0.5*pow(10.,-3) && phi_an<=0.5*pow(10.,-3)) {
	if(p_vttrk->GetTracksN()==1) 	
	  ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_res2x_ang_2D"))->Fill(res.X(),chi2);
	
	// 	    ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_x_ang_2D_vertex"))->Fill(theta_vt,phi_vt);
	
	/*	cout << "bmvt_1t_res2x_ang "<<res.X()<<" chi2 "<< chi2 << endl;*/
      }
      
      if(chi2<10 && psi_an>=-0.5*pow(10.,-3) && psi_an<=0.5*pow(10.,-3)) {
	if(p_vttrk->GetTracksN()==1) 	
	  ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_res2y_ang_2D"))->Fill(res.Y(),chi2);
	
	// 	    ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_y_ang_2D_vertex"))->Fill(theta_vt,phi_vt);
	
	/*	cout << "bmvt_1t_res2y_ang "<<res.Y()<<" chi2 "<< chi2 << endl;*/
      }
      
      
      if(m_fullmoni) {
	//Residuals on the BM center 
	((TH1D*)gDirectory->Get("beammonitor/bmvt_resCenx"))->Fill(resCen.X());
	if(p_vttrk->GetTracksN()==1) 	
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCenx"))->Fill(resCen.X());
	
	if(chi2<20) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCen20x"))->Fill(resCen.X());
	  if(p_vttrk->GetTracksN()==1) 	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen20x"))->Fill(resCen.X());
	}
	
	if(chi2<10) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCen10x"))->Fill(resCen.X());
	  if(p_vttrk->GetTracksN()==1) 	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen10x"))->Fill(resCen.X());
	}
	
	if(chi2<5) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCen5x"))->Fill(resCen.X());
	  if(p_vttrk->GetTracksN()==1) 	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen5x"))->Fill(resCen.X());
	}
	
	if(chi2<2) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCen2x"))->Fill(resCen.X());
	  if(p_vttrk->GetTracksN()==1) 	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen2x"))->Fill(resCen.X());
	}
	
	//Residuals on the BM center on Y
	
	((TH1D*)gDirectory->Get("beammonitor/bmvt_resCeny"))->Fill(resCen.Y());
	if(p_vttrk->GetTracksN()==1) 	
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCeny"))->Fill(resCen.Y());
	
	if(chi2<20) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCen20y"))->Fill(resCen.Y());
	  if(p_vttrk->GetTracksN()==1) 	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen20y"))->Fill(resCen.Y());
	}
	
	if(chi2<2) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCen2y"))->Fill(resCen.Y());
	  if(p_vttrk->GetTracksN()==1) 	
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen2y"))->Fill(resCen.Y());
	}
	
	//Residuals on the BM center with Angle Condition on X and Y
	
	if(chi2<10 && phi_an>=-0.5*pow(10.,-3) && phi_an<=0.5*pow(10.,-3)) {
	  if(p_vttrk->GetTracksN()==1)
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen2x_ang"))->Fill(resCen.X());
	  /*cout << "bmvt_1t_resCen2x_ang "<<resCen.X()<<endl;*/
	}
	if(chi2<10 && psi_an>=-0.5*pow(10.,-3) && psi_an<=0.5*pow(10.,-3)) {
	  if(p_vttrk->GetTracksN()==1)
	    ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen2y_ang"))->Fill(resCen.Y());
	  /*cout << "bmvt_1t_resCen2y_ang "<<resCen.Y()<<endl;*/
	}
      }
      
      if(chi2<10 && phi_an>=-0.5*pow(10.,-3) && phi_an<=0.5*pow(10.,-3)) {
	if(p_vttrk->GetTracksN()==1) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_origin2x_ang"))->Fill(origin.X());
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_orvtx2x_ang"))->Fill(GlbOrigin.X());
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_orbmx2x_ang"))->Fill(GlbBM.X());
	}
	/*cout << "bmvt_1t_origin2x_ang "<<origin.X()<<endl;*/
      }
      
      if(chi2<10 && psi_an>=-0.5*pow(10.,-3) && psi_an<=0.5*pow(10.,-3)) {
	if(p_vttrk->GetTracksN()==1) {
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_origin2y_ang"))->Fill(origin.Y());
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_orvtx2y_ang"))->Fill(GlbOrigin.Y());
	  ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_orbmx2y_ang"))->Fill(GlbBM.Y());
	}
	/*cout << "bmvt_1t_origin2y_ang "<<origin.Y()<<endl;*/
      }
      
    }//Loop on VTX tracks

    if(p_vttrk->GetTracksN()==2) {
      //      cout<<" "<<dis_1<<" "<<dis_2<<" "<<phi_bm<<" "<<psi_bm<<" "<<phi_vt_1<<" "<<psi_vt_1<<" "<<phi_vt_2<<" "<<psi_vt_2<<" "<<endl;
      if(dis_1<dis_2) {
	((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_phigoo"))->Fill(phi_bm-phi_vt_1); 
	((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_psigoo"))->Fill(psi_bm-psi_vt_1); 
	((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_phibad"))->Fill(phi_bm-phi_vt_2); 
	((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_psibad"))->Fill(psi_bm-psi_vt_2); 
      } else {
	((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_phigoo"))->Fill(phi_bm-phi_vt_2); 
	((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_psigoo"))->Fill(psi_bm-psi_vt_2); 
	((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_phibad"))->Fill(phi_bm-phi_vt_1); 
	((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_psibad"))->Fill(psi_bm-psi_vt_1); 
      }
    }

    if(dis_vec.size()) {
      //Sorting distances
      sort (dis_vec.begin(), dis_vec.end());
      ((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_resmin"))->Fill(dis_vec.at(0));
      ((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_resnex"))->Fill(dis_vec.at(1));
    }

  }//There's a BM track
  
}


void RecoTools::CalibBMVT() {

  char name[200];
  
  //Tracks
  TABMntuTrack* p_ntutrk  = 
    (TABMntuTrack*) myn_bmtrk->GenerateObject();

  //VTX tracks
  TAVTntuTrack* p_vttrk = 
    (TAVTntuTrack*) myn_vtrk->GenerateObject();

  //BM local geo
  TABMparGeo* p_bmgeo = 
    (TABMparGeo*)(gTAGroot->FindParaDsc("p_bmgeo", "TABMparGeo")->Object());
  
  //Hits
  TABMntuRaw* p_ntuhit  = 
    (TABMntuRaw*) myn_bmraw->GenerateObject();

  //requires only 1 track in the vtx
  double sDistanceFromTrack, DistanceFromTrack, Residuals;
  if(p_vttrk->GetTracksN() == 1) {

    TAVTtrack* track = p_vttrk->GetTrack(0);
    
    TAVTline line = track->GetTrackLine();

    //First of all loops on the hits
    Int_t i_nnhit = p_ntuhit->nhit;
    for (Int_t i = 0; i < i_nnhit; i++) {
      const TABMntuHit* aHi = p_ntuhit->Hit(i);
      
      int tmpv = aHi->View();
      if(tmpv<0)tmpv = 0;
      //      int chidx = aHi->Cell()+aHi->Plane()*3+tmpv*18;

      int idwire = p_bmgeo->GetID(aHi->Cell());

      //U view, along x, SIDE
      TVector3 bm_ChkPoi(p_bmgeo->GetX(idwire,aHi->Plane(),tmpv),
			 p_bmgeo->GetY(idwire,aHi->Plane(),tmpv),
			 p_bmgeo->GetZ(idwire,aHi->Plane(),tmpv));


      TVector3 fir_CenPoi = fGeoTrafo->GetBMCenter();

      TVector3 fir_ChkPoi = fGeoTrafo->FromBMLocalToGlobal(bm_ChkPoi);
      //      TVector3 fir_CenPoi = fGeoTrafo->FromBMLocalToGlobal(bm_CenPoi);

      TVector3 vt_ChkPoi = fGeoTrafo->FromGlobalToVTLocal(fir_ChkPoi);
      TVector3 vt_CenPoi = fGeoTrafo->FromGlobalToVTLocal(fir_CenPoi);

      //cm --> mum
      TVector3 vt_ExtPoi = track->Intersection(vt_ChkPoi.Z()*10000);      
      //mum --> cm
      vt_ExtPoi.SetX(vt_ExtPoi.X()/10000);
      vt_ExtPoi.SetY(vt_ExtPoi.Y()/10000);
      vt_ExtPoi.SetZ(vt_ExtPoi.Z()/10000);

      TVector3 fir_ExtPoi = fGeoTrafo->FromVTLocalToGlobal(vt_ExtPoi);
      
      //cm --> mum
      TVector3 vt_ExtCen = track->Intersection(vt_CenPoi.Z()*10000);      
      //mum --> cm
      vt_ExtCen.SetX(vt_ExtCen.X()/10000);
      vt_ExtCen.SetY(vt_ExtCen.Y()/10000);
      vt_ExtCen.SetZ(vt_ExtCen.Z()/10000);
      
      if(m_debug)
	cout<<"Z BM:: "<< p_bmgeo->GetZ(idwire,aHi->Plane(),tmpv)<<
	  " Z VT::"<<vt_ExtPoi.Z()<<endl;

      if(m_debug)
	cout<<"X,Y extr from Vtx:: "<<vt_ExtPoi.X()<<" "<<vt_ExtPoi.Y()<<" "<<vt_ExtPoi.Z()<<endl;
      if(i == 0 && m_debug)
	cout<<"X,Y extr from Vtx LOC:: "<<vt_ExtCen.X()<<" "<<vt_ExtCen.Y()<<" "<<vt_ExtCen.Z()<<endl;

      TVector3 fir_ExtCen = fGeoTrafo->FromVTLocalToGlobal(vt_ExtCen);

      if(i==0 && m_debug)
	cout<<"X,Y extr from Vtx GLB:: "<<fir_ExtCen.X()<<" "<<fir_ExtCen.Y()<<" "<<fir_ExtCen.Z()<<endl;
      //      cout<<"X,Y xchk from Vtx:: "<<fir_ExtXck.X()<<" "<<fir_ExtXck.Y()<<" "<<fir_ExtXck.Z()<<endl;
      
      TVector3 fir_ExtXck = fGeoTrafo->FromVTLocalToGlobal(TVector3(-10,0,0));

      TVector3 bm_ExtPoi = fGeoTrafo->FromGlobalToBMLocal(fir_ExtPoi);

      double x_tr = bm_ExtPoi.X();
      double y_tr = bm_ExtPoi.Y();
      double x_wi = p_bmgeo->GetX(idwire,aHi->Plane(),tmpv);
      double y_wi = p_bmgeo->GetY(idwire,aHi->Plane(),tmpv);

      if(i == 0) {
	sprintf(name,"beammonitor/VTproj_BeamSN_BMcen_X");
	((TH1D*)gDirectory->Get(name))->Fill(fir_ExtCen.X());
	sprintf(name,"beammonitor/VTproj_BeamSN_BMcen_Y");
	((TH1D*)gDirectory->Get(name))->Fill(fir_ExtCen.Y());
      }


      if(!tmpv) {
	//	wires along x, only Y matters.
	DistanceFromTrack = fabs(y_wi - y_tr);
	sDistanceFromTrack = y_wi - y_tr;
	if(m_debug) cout<<"Cell: "<<idwire<<" Plane: "<<aHi->Plane()<<" View: "<<aHi->View()<<" "<<y_tr<<" "<<y_wi<<" Dis:: "<<DistanceFromTrack<<" sDis:: "<<sDistanceFromTrack<<" "<<aHi->Dist()<<endl;
      } else {
	//	wires along y, only X matters.
	DistanceFromTrack = fabs(x_wi - x_tr);
	sDistanceFromTrack = x_wi - x_tr;
	if(m_debug) cout<<"Cell: "<<idwire<<" Plane: "<<aHi->Plane()<<" View: "<<aHi->View()<<" "<<x_tr<<" "<<x_wi<<" Dis:: "<<DistanceFromTrack<<" sDis:: "<<sDistanceFromTrack<<" "<<aHi->Dist()<<endl;
      }      

      if(m_fullmoni) {
	Residuals = DistanceFromTrack - aHi->Dist();
	sprintf(name,"beammonitor/VTresi_aft_%d_%d_%d",aHi->Cell(),aHi->Plane(),tmpv);
	((TH1D*)gDirectory->Get(name))->Fill(Residuals);
	sprintf(name,"beammonitor/VTresi_vs_t_%d_%d_%d",aHi->Cell(),aHi->Plane(),tmpv);
	((TH2D*)gDirectory->Get(name))->Fill(Residuals,aHi->Tdrift());
	sprintf(name,"beammonitor/VTdist_vs_t_%d_%d_%d",aHi->Cell(),aHi->Plane(),tmpv);
	((TH2D*)gDirectory->Get(name))->Fill(DistanceFromTrack,aHi->Tdrift());
	sprintf(name,"beammonitor/VTsdist_vs_t_%d_%d_%d",aHi->Cell(),aHi->Plane(),tmpv);
	((TH2D*)gDirectory->Get(name))->Fill(sDistanceFromTrack,aHi->Tdrift());
	sprintf(name,"beammonitor/VTresi_vs_zmt_%d_%d_%d",aHi->Cell(),aHi->Plane(),tmpv);
	((TH2D*)gDirectory->Get(name))->Fill(Residuals,aHi->Tdrift());
	sprintf(name,"beammonitor/VTresi_vs_r_%d_%d_%d",aHi->Cell(),aHi->Plane(),tmpv);
	((TH2D*)gDirectory->Get(name))->Fill(Residuals,aHi->Dist());
      }
    }


  }

  return;
}


void RecoTools::initBMCon(TAGparaDsc* beamcon)  {

  Int_t i_run = gTAGroot->CurrentRunNumber();
  Int_t i_cam = gTAGroot->CurrentCampaignNumber();

  cout << "Loading beamcon for cam/run = " << i_cam << "/" << i_run << endl;

  TABMparCon* o_beamcon = (TABMparCon*) beamcon->Object();

  o_beamcon->Clear();

  Bool_t b_bad = kTRUE;

  TString filename = m_wd + "/config/beammonitor.cfg";

  cout << "   from file " << filename << endl;

  b_bad = o_beamcon->FromFile(filename);

  filename = m_wd + "/config/beammonitor_t0s.cfg";

  o_beamcon->loadT0s(filename);

  filename = m_wd + "/config/file_stlist_FIRST.txt";
  //  filename = "config/file_stlist_8020_Cst1_1750.txt";

  o_beamcon->LoadSTrel(filename);

  o_beamcon->SetIsMC(true);

  o_beamcon->ConfigureTrkCalib();

  filename = m_wd + "/config/bmreso_vs_r.root";
  o_beamcon->LoadReso(filename);

  if (!b_bad) beamcon->SetBit(TAGparaDsc::kValid);

  return;
}


void RecoTools::initBMGeo(TAGparaDsc* p_bmgeo)  {

  p_bmgeo = gTAGroot->FindParaDsc("p_bmgeo", "TABMparGeo");
  if (p_bmgeo == 0) {
    cout << "p_bmgeo not found or holding wrong parameter object type" << endl;
    return;
  }

  Int_t i_run = gTAGroot->CurrentRunNumber();
  Int_t i_cam = gTAGroot->CurrentCampaignNumber();

  Info("RecoTools::initBMGeo","Loading p_bmgeo for cam/run = %d / %d",i_cam,i_run);

  TABMparGeo* o_bmgeo = (TABMparGeo*) p_bmgeo->Object();

  //Initialization of BM parameters
  o_bmgeo->InitGeo();

  p_bmgeo->SetBit(TAGparaDsc::kValid);

  return;

}


void RecoTools::initDCCon(TAGparaDsc* driftcon)  {

  Int_t i_run = gTAGroot->CurrentRunNumber();
  Int_t i_cam = gTAGroot->CurrentCampaignNumber();

  cout << "Loading driftcon for cam/run = " << i_cam << "/" << i_run << endl;

  TADCparCon* o_driftcon = (TADCparCon*) driftcon->Object();

  o_driftcon->Clear();

  Bool_t b_bad = kTRUE;

  TString filename = m_wd + "/config/driftchamber.cfg";

  cout << "   from file " << filename << endl;

  b_bad = o_driftcon->FromFile(filename);

  filename = m_wd + "/config/driftchamber_t0s.cfg";

  o_driftcon->loadT0s(filename);

  filename = m_wd + "/config/file_stlist_FIRST.txt";
  //  filename = "config/file_stlist_8020_Cst1_1750.txt";

  o_driftcon->LoadSTrel(filename);

  o_driftcon->SetIsMC(true);

  o_driftcon->ConfigureTrkCalib();

  filename = m_wd + "/config/dcreso_vs_r.root";
  o_driftcon->LoadReso(filename);

  if (!b_bad) driftcon->SetBit(TAGparaDsc::kValid);

  return;
}


void RecoTools::initDCGeo(TAGparaDsc* p_dcgeo)  {

  p_dcgeo = gTAGroot->FindParaDsc("p_dcgeo", "TADCparGeo");
  if (p_dcgeo == 0) {
    cout << "p_dcgeo not found or holding wrong parameter object type" << endl;
    return;
  }

  Int_t i_run = gTAGroot->CurrentRunNumber();
  Int_t i_cam = gTAGroot->CurrentCampaignNumber();

  Info("RecoTools::initDCGeo","Loading p_dcgeo for cam/run = %d / %d",i_cam,i_run);

  TADCparGeo* o_dcgeo = (TADCparGeo*) p_dcgeo->Object();

  //Initialization of DC parameters
  o_dcgeo->InitGeo();

  p_dcgeo->SetBit(TAGparaDsc::kValid);

  return;

}


void RecoTools::bookHisto(TFile *f) {

  cout<< " Histo booking " <<endl;

  f->cd();

  TH1D *h ; char name[200];
  TH2D *h2;

  h = new TH1D("ResoMCCarbons","reso",100,-0.01,0.003);
  h = new TH1D("ResoMCPixel_0","reso",100,-0.01,0.003);
  h = new TH1D("ResoMCPixel_1","reso",100,-0.01,0.003);
  h = new TH1D("ResoMCPixel_2","reso",100,-0.01,0.003);
  h = new TH1D("ResoMCPixel_3","reso",100,-0.01,0.003);

  /* BEAM MONITOR */  
  gDirectory->mkdir("beammonitor");
  gDirectory->cd("beammonitor");

  h = new TH1D("vtdist","dist btw trks",100,0,2);
  h2 = new TH2D("vtdist_nt","dist btw trks",100,0,2,10,0,10);


  h2 = new TH2D("resx_vs_angRes","dist btw trks",100,-0.1,0.1,100,-0.004,0.001);
  h2 = new TH2D("resy_vs_angRes","dist btw trks",100,-0.1,0.1,100,-0.004,0.001);

  h = new TH1D("hit_effi","hit efficiency",12,-0.5,11.5);
  h = new TH1D("bmvt_resx","Chi2 Dist",400,-2,2);
  h = new TH1D("bmvt_res5x","Chi2 Dist",400,-2,2);
  if(m_fullmoni) {
    h = new TH1D("bmvt_res2x","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_res10x","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_res20x","Chi2 Dist",400,-2,2);
  }

  h = new TH1D("VTproj_BeamSN_BMcen_Y","Chi2 Dist",400,-4,4);
  h = new TH1D("VTproj_BeamSN_BMcen_X","Chi2 Dist",400,-4,4);

  h = new TH1D("bmvt_1t_resx","Chi2 Dist",400,-2,2);
  h = new TH1D("bmvt_1t_res5x","Chi2 Dist",400,-2,2);
  h = new TH1D("bmvt_1t_vtx5x","Chi2 Dist",400,-4,4);
  h = new TH1D("bmvt_1t_bmx5x","Chi2 Dist",400,-4,4);
  h = new TH1D("bmvt_1t_bmcex5x","Chi2 Dist",400,-4,4);

  if(m_fullmoni) {
    h = new TH1D("bmvt_1t_res2x","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_res10x","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_res20x","Chi2 Dist",400,-2,2);
  }

  h = new TH1D("bmvt_resy","Chi2 Dist",400,-2,2);
  h = new TH1D("bmvt_res5y","Chi2 Dist",400,-2,2);
  if(m_fullmoni) {
    h = new TH1D("bmvt_res2y","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_res20y","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_res15y","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_res10y","Chi2 Dist",400,-2,2);
  }
  h = new TH1D("bmvt_1t_resy","Chi2 Dist",400,-2,2);
  h = new TH1D("bmvt_1t_res5y","Chi2 Dist",400,-2,2);
  h = new TH1D("bmvt_1t_vtx5y","Chi2 Dist",400,-4,4);
  h = new TH1D("bmvt_1t_bmx5y","Chi2 Dist",400,-4,4);
  h = new TH1D("bmvt_1t_bmcex5y","Chi2 Dist",400,-4,4);
  if(m_fullmoni) {
    h = new TH1D("bmvt_1t_res2y","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_res20y","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_res10y","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_res15y","Chi2 Dist",400,-2,2);
  }

  h = new TH1D("bmvt_res2x_ang","Chi2 Dist",400,-2,2);
  h = new TH1D("bmvt_res2y_ang","Chi2 Dist",400,-2,2);
  h = new TH1D("bmvt_1t_res2x_ang","Chi2 Dist",400,-2,2);
  h = new TH1D("bmvt_1t_res2y_ang","Chi2 Dist",400,-2,2);
  h = new TH1D("bmvt_2t_resmin","Chi2 Dist",100,0,0.5);
  h = new TH1D("bmvt_2t_resnex","Chi2 Dist",100,0,0.5);

  h = new TH1D("bmvt_2t_phigoo","Chi2 Dist",400,-0.5,0.5); 
  h = new TH1D("bmvt_2t_psigoo","Chi2 Dist",400,-0.5,0.5); 
  h = new TH1D("bmvt_2t_phibad","Chi2 Dist",400,-0.5,0.5); 
  h = new TH1D("bmvt_2t_psibad","Chi2 Dist",400,-0.5,0.5); 

  if(m_fullmoni) {
    h = new TH1D("bmvt_resCenx","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_resCen2x","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_resCen5x","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_resCen10x","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_resCen20x","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_resCenx","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_resCen2x","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_resCen5x","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_resCen10x","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_resCen20x","Chi2 Dist",400,-2,2);
    
    h = new TH1D("bmvt_resCeny","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_resCen2y","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_resCen20y","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_resCeny","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_resCen2y","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_resCen20y","Chi2 Dist",400,-2,2);
  }
  h = new TH1D("bmvt_1t_origin2x_ang","Chi2 Dist",400,-2,2);
  h = new TH1D("bmvt_1t_orvtx2x_ang","Chi2 Dist",400,-4,4);
  h = new TH1D("bmvt_1t_orbmx2x_ang","Chi2 Dist",400,-4,4);
  h = new TH1D("bmvt_1t_origin2y_ang","Chi2 Dist",400,-2,2);
  h = new TH1D("bmvt_1t_orvtx2y_ang","Chi2 Dist",400,-4,4);
  h = new TH1D("bmvt_1t_orbmx2y_ang","Chi2 Dist",400,-4,4);
  if(m_fullmoni) {
    h = new TH1D("bmvt_1t_resCen2x_ang","Chi2 Dist",400,-2,2);
    h = new TH1D("bmvt_1t_resCen2y_ang","Chi2 Dist",400,-2,2);
  }

  h2 = new TH2D("NPixels_Vtx_vs_chi2_plane4","chi2 versus NPixels_Vtx plane4",20,0.,30.,400,0.,10.);
  h2 = new TH2D("NPixels_Vtx_vs_chi2_plane0","chi2 versus NPixels_Vtx plane0",20,0.,30.,400,0.,10.);

  h2 = new TH2D("bmvt_1t_res2x_ang_2D","bmvt_1t_res2x_ang_2D",400,-2,2,400,0.,10.);
  h2 = new TH2D("bmvt_1t_res2y_ang_2D","bmvt_1t_res2y_ang_2D",400,-2,2,400,0.,10.);


  h = new TH1D("eff","track effi,Efficiency",7,0,7);

  h = new TH1D("chi2","Chi2 Dist",200,0,20);
  h = new TH1D("chi2_0","Chi2 Dist",200,0,20);
  h = new TH1D("chi2_1","Chi2 Dist",200,0,20);
  h = new TH1D("z_ang","Angle wrt to Beam Axis",100,-0.01,0.14);
  h = new TH1D("xpr_ang","X,Z Angle",100,-0.14,0.14);
  h = new TH1D("ypr_ang","Y,Z Angle",100,-0.14,0.14);

  h = new TH1D("x_ang_dg","X,Y Angle",200,-9.0,+9.0);
  h = new TH1D("y_ang_dg","X,Y Angle",200,-9.0,+9.0);

  if(m_fullmoni) {
    h2 = new TH2D("xy_pr_ang","X,Y Angle",200,-0.14,0.14,200,-0.14,0.14);
    h2 = new TH2D("xy_pr_ang_dg","X,Y Angle",200,-9.0,9.0,200,-9.0,+9.0);
    h2 = new TH2D("bmvt_1t_x_ang_2D_vertex","bmvt_1t_x_ang_2D_vertex",400,-10,10,400,-10.,10.);
    h2 = new TH2D("bmvt_1t_y_ang_2D_vertex","bmvt_1t_y_ang_2D_vertex",400,-10,10,400,-10.,10.);
    h2 = new TH2D("bmvt_1t_ang_chi10_2D_x","bmvt_1t_ang_chi10_2D_x",400,-0.028,0.028,400,-0.028,0.028);
    h2 = new TH2D("bmvt_1t_ang_chi10_2D_y","bmvt_1t_ang_chi10_2D_y",400,-0.028,0.028,400,-0.028,0.028);
    h2 = new TH2D("bmvt_1t_ang_chi2_2D_x","bmvt_1t_ang_chi2_2D_x",400,-0.028,0.028,400,-0.028,0.028);
    h2 = new TH2D("bmvt_1t_ang_chi2_2D_y","bmvt_1t_ang_chi2_2D_y",400,-0.028,0.028,400,-0.028,0.028);
    h2 = new TH2D("bmvt_1t_ang_chi2_2D_x_cut0.5","bmvt_1t_ang_chi2_2D_x_cut0.5",400,-0.28,0.28,400,-0.28,0.28);
    h2 = new TH2D("bmvt_1t_ang_chi2_2D_x_cut1","bmvt_1t_ang_chi2_2D_x_cut1",400,-0.28,0.28,400,-0.28,0.28);
  }

  h = new TH1D("x_pro","X BeamSpot Proj",100,-3,3);
  h = new TH1D("y_pro","Y BeamSpot Proj",100,-3,3);
  h2 = new TH2D("xy_dist","XY BeamSpot",100,-3,3,100,-3,3);

  h = new TH1D("resi_aft","Residuals distribution",100,-0.8,0.8);
  h2 = new TH2D("resi_vs_t","Residuals versus meas dist",500,-0.8,0.8,200,0.,500.);
  if(m_fullmoni)
    h2 = new TH2D("resi_vs_zmt","Residuals versus meas dist",500,-0.8,0.8,100,0.,25.);
  h2 = new TH2D("resi_vs_r","Residuals versus meas dist",500,-0.8,0.8,500,0.,1.);
  if(m_fullmoni) {
    h = new TH1D("resian_aft","Residuals distribution",100,-0.8,0.8);
    h2 = new TH2D("resian_vs_t","Residuals versus meas dist",500,-0.8,0.8,200,0.,500.);
    h2 = new TH2D("resian_vs_zmt","Residuals versus meas dist",500,-0.8,0.8,100,0.,25.);
    h2 = new TH2D("resian_vs_r","Residuals versus meas dist",500,-0.8,0.8,500,0.,1.);

    for(int ic=0; ic<3; ic++) {
      for(int ip=0; ip<6; ip++) {
	for(int iv=0; iv<2; iv++) {
	  sprintf(name,"resi_aft_%d_%d_%d",ic,ip,iv);
	  h = new TH1D(name,"Residuals distribution",100,-0.8,0.8);
	  sprintf(name,"resi_vs_t_%d_%d_%d",ic,ip,iv);
	  h2 = new TH2D(name,"Residuals versus meas dist",500,-0.8,0.8,200,0.,500.);
	  if(m_fullmoni) {
	    sprintf(name,"resi_vs_zmt_%d_%d_%d",ic,ip,iv);
	    h2 = new TH2D(name,"Residuals versus meas dist",500,-0.8,0.8,100,0.,25.);
	  }
	  sprintf(name,"resi_vs_r_%d_%d_%d",ic,ip,iv);
	  h2 = new TH2D(name,"Residuals versus meas dist",500,-0.8,0.8,500,0.,1.);
	  
	  sprintf(name,"VTresi_aft_%d_%d_%d",ic,ip,iv);
	  h = new TH1D(name,"Residuals distribution",100,-0.8,0.8);
	  sprintf(name,"VTresi_vs_t_%d_%d_%d",ic,ip,iv);
	  h2 = new TH2D(name,"Residuals versus meas dist",500,-0.8,0.8,200,0.,500.);
	  sprintf(name,"VTdist_vs_t_%d_%d_%d",ic,ip,iv);
	  h2 = new TH2D(name,"Residuals versus meas dist",400,0.,0.8,200,0.,500.);
	  sprintf(name,"VTsdist_vs_t_%d_%d_%d",ic,ip,iv);
	  h2 = new TH2D(name,"Residuals versus meas dist",800,-0.8,0.8,200,0.,500.);
	  sprintf(name,"VTresi_vs_zmt_%d_%d_%d",ic,ip,iv);
	  h2 = new TH2D(name,"Residuals versus meas dist",500,-0.8,0.8,100,0.,25.);
	  sprintf(name,"VTresi_vs_r_%d_%d_%d",ic,ip,iv);
	  h2 = new TH2D(name,"Residuals versus meas dist",500,-0.8,0.8,500,0.,1.);
	}
      }
    }
  }

  if(m_fullmoni) {
    h = new TH1D("ts","overall Time spectra",100,0,2000);
  }
  h = new TH1D("hits_tot","overall number of hits per wire",10,0,10);
  for(int it=0; it<36; it++) {
    if(m_fullmoni) {
      sprintf(name,"tdc_%d",it);
      h = new TH1D(name,"Channel Time spectra",2000,0,2000);
    }
    sprintf(name,"t0_tdc_%d",it);
    h = new TH1D(name,"Channel Time spectra",3000,-1500,1500);
    sprintf(name,"first_time_tdc_%d",it);
    h = new TH1D(name,"Channel First Time spectra",100,0,2000);
    sprintf(name,"hits_tdc_%d",it);
    h = new TH1D(name,"Channel number of hits per wire",10,0,10);
  }
  h = new TH1D("nhit","Number of reconstructed hits",36,0,36);
  h = new TH1D("nwire","Number of fired wires",36,0,36);
  h = new TH1D("topwire","Top wires occupancy",18,-0.5,17.5);
  h = new TH1D("sidewire","Side wires occupancy",18,-0.5,17.5);
  gDirectory->cd("..");

  /* Start Counter */  
  gDirectory->mkdir("startcounter");
  gDirectory->cd("startcounter");
  h = new TH1D("nhit","Number of reconstructed hits",36,0,36);
  h = new TH1D("firsthit","First SC Time",500,0,500);
  h = new TH1D("firstSCch","First SC Channel",5,-1.5,3.5);
  h = new TH1D("TimeDiff02","Time Difference (ns)",200,-10,10);
  h = new TH1D("TimeDiff13","Time Difference (ns)",200,-10,10);
  h = new TH1D("NPetals","Number of fired petals",5,-0.5,4.5);
  h = new TH1D("pattern","Fired petals pattern",5,-0.5,4.5);
  h = new TH1D("TimeDiff01","Time Difference (ns)",200,-10,10);
  h = new TH1D("TimeDiff03","Time Difference (ns)",200,-10,10);
  for(int it = 0; it<4; it++) {
    sprintf(name,"adc_%d",it);
    h = new TH1D(name,"Channel Charge spectra",410,0,4096);
    h->SetXTitle("Charge (adc counts)");
    sprintf(name,"tdc_%d",it);
    h = new TH1D(name,"Channel Time spectra",500,0,500);
    h->SetXTitle("Time (ns)");
    sprintf(name,"nhit_%d",it);
    h = new TH1D(name,"Number of hits",10,0,10);
    h->SetXTitle("Number of hits");
  }
  gDirectory->cd("..");

  /* Trigger */  
  gDirectory->mkdir("trigger");
  gDirectory->cd("trigger");

  h = new TH1D("nhit","Number of reconstructed hits",36,0,36);

  h = new TH1D("tdc47_time","Trigger 0 time",1000,0,1000);
  h = new TH1D("tdc47_mult","Trigger 0 mult",10,0,10);
  h = new TH1D("mast_time","Trigger 1 time",1000,0,1000);
  h = new TH1D("mast_mult","Trigger 1 mult",10,0,10);
  h = new TH1D("doud_time","Trigger 2 time",1000,0,1000);
  h = new TH1D("doud_mult","Trigger 2 mult",10,0,10);
  h = new TH1D("tof_time","Trigger 3 time",1000,0,1000);
  h = new TH1D("tof_mult","Trigger 3 mult",10,0,10);
  h = new TH1D("ke_mast_time","Trigger 5 time",1000,0,1000);
  h = new TH1D("ke_mast_mult","Trigger 5 mult",10,0,10);
  h = new TH1D("ke_doud_time","Trigger 6 time",1000,0,1000);
  h = new TH1D("ke_doud_mult","Trigger 6 mult",10,0,10);

  h = new TH1D("mast_doud","Trigger time Difference 1-2 ",1000,0,1000);
  h = new TH1D("mast_tof","Trigger time Difference 1-3 ",1000,-1000,0);
  h = new TH1D("mast_kemast","Trigger time Difference 1-2 ",1000,-500,500);
  h = new TH1D("mast_kedoud","Trigger time Difference 1-3 ",1000,0,1000);

  h = new TH1D("z_mast_doud","Zoom Trigger time Difference 1-2 ",100,360,400);
  h = new TH1D("z_mast_tof","Zoom Trigger time Difference 1-3 ",100,-420,-400);
  h = new TH1D("z_mast_kemast","Zoom Trigger time Difference 1-2 ",100,-100,100);
  h = new TH1D("z_mast_kedoud","Zoom Trigger time Difference 1-3 ",100,300,500);
  gDirectory->cd("..");


  return;
}


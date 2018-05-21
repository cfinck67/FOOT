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
void BmBooter::Initialize( EVENT_STRUCT* evStr ) {  
  bmcon = (TABMparCon*) (gTAGroot->FindParaDsc("myp_bmcon", "TABMparCon")->Object()); 
  bmgeo = (TABMparGeo*) (gTAGroot->FindParaDsc("myp_bmgeo", "TABMparGeo")->Object());
  
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

return;
}





//----------------------------------------------------------------------------------------------------
void BmBooter::Process( Long64_t jentry ) {
  if (bmcon->GetBMdebug()>10)
    cout<<"I'm in BmBooter::Process, evento numero="<<jentry<<endl;
    
  //~ myn_bmraw  = gTAGroot->FindDataDsc("myn_bmraw", "TABMntuRaw");
  //~ myn_bmtrk  = gTAGroot->FindDataDsc("myn_bmtrk", "TABMntuTrack");
  //~ if (myn_bmtrk == 0 || myn_bmraw==0) {
      //~ cout << "ERROR in BmBooter:: myn_bmraw or myn_bmtrk not found" << endl;
      //~ exit(0);
    //~ }
    
  //~ bmraw = (TABMntuRaw*) myn_bmraw->GenerateObject();
  //~ bmtrack = (TABMntuTrack*) myn_bmtrk->GenerateObject();
 
 bmraw = (TABMntuRaw*) (gTAGroot->FindDataDsc("myn_bmraw", "TABMntuRaw")->GenerateObject());
 bmtrack = (TABMntuTrack*) (gTAGroot->FindDataDsc("myn_bmtrk", "TABMntuTrack")->GenerateObject());
 
 
  if (bmcon->GetBMdebug()>10)
    cout<<"in BmBooter::Process, I finished to create the BM hits and tracks"<<endl<<"Now I'll printout BM hits if enable"<<endl;

  //loop on hits
  for (int i = 0; i < bmraw->nhit; i++) {
    p_hit = bmraw->Hit(i);
    //~ cout<<"hit number="<<i<<"  event number="<<jentry<<"   distanza="<<p_hit->Dist()<<endl;
    if (GlobalPar::GetPar()->IsPrintOutputFile())
      m_controlPlotter->BM_setnturaw_info("BM_output",p_hit);  
    if (GlobalPar::GetPar()->IsPrintOutputNtuple())
      m_controlPlotter->BM_setntuple_hit(p_hit->Dist());
  }
  
  if (bmcon->GetBMdebug()>10)
    cout<<"in BmBooter::Process, I finished to printout BM hits, it's BM tracks printout (if enable)"<<endl;
  
  //loop on tracks
  for (int i = 0; i < bmtrack->ntrk; i++) {
    p_tracktr = bmtrack->Track(i);
    if (GlobalPar::GetPar()->IsPrintOutputFile())
      m_controlPlotter->BM_setntutrack_info("BM_output", p_tracktr);       
    if (GlobalPar::GetPar()->IsPrintOutputNtuple())
      m_controlPlotter->BM_setntuple_track(p_tracktr->GetChi2New());
  }
  

  //draw and save tracks
  if(bmcon->GetBMvietrack()>0 && jentry%bmcon->GetBMvietrack()==0){
      TCanvas *c_bmhview = new TCanvas("bmhview", "BM_tracks",20,20,800,900);
      pg->AddPad(c_bmhview);
      TAGview* pbmh_view = new TABMvieTrackFOOT(bmtrack, bmraw, bmgeo);
      //~ pbmh_view->SetTrackRaw(bmtrack,bmraw);
      pbmh_view->Draw();
      pg->Modified();//marca i pad come modificati
      //~ cout<<"faccio update"<<endl;
      //~ pg->Update();//fa update del canvas
      plot_name=bm_outputdir+"/BM_track_"+to_string(jentry);
      pg->Print(&plot_name[0]);  
  }
  
  if (bmcon->GetBMdebug()>10)
    cout<<"I finished BmBooter::Process"<<endl;

return;
}





//----------------------------------------------------------------------------------------------------
void BmBooter::Finalize() {
  
  if (bmcon->GetBMdebug()>10)
    cout<<"I'm in BmBooter::Finalize"<<endl;
  
  //~ TCanvas* mirror = new TCanvas("bmstuff", "bmstuff",  700, 700);
  //~ histo1->Draw();
  //~ mirror->SaveAs("bmstuff.png");
  
  if (bmcon->GetBMdebug()>10)
    cout<<"I finished BmBooter::Finalize"<<endl;
    
return;
}




































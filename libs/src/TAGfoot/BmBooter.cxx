#include "BmBooter.hxx"

#include <FieldManager.h>


using namespace std;



//----------------------------------------------------------------------------------------------------
BmBooter::BmBooter() {



}



//----------------------------------------------------------------------------------------------------
void BmBooter::Initialize( EVENT_STRUCT* evStr ) {
  if (GlobalPar::GetPar()->Debug()>10) 
    cout<<"initialize BmBooter"<<endl;
  histo1 = new TH1F("histo1", "histo1",200, 0., 2. );

	// Create dir for kalman output
  //~ struct stat info;
	//~ string bm_OutputDir = (string)getenv("FOOTRES")+"/BeamMonitor";
	//~ if( stat( bm_OutputDir.c_str(), &info ) != 0 )		//cannot access
	    //~ system(("mkdir "+bm_OutputDir).c_str());

	//~ // class for control plot dumping
	//~ m_controlPlotter = ControlPlotsRepository::GetControlObject( bm_OutputDir );
//~ //ARRIVATO QUA



return;
}





//----------------------------------------------------------------------------------------------------
void BmBooter::Process( Long64_t jentry ) {
  if (GlobalPar::GetPar()->Debug()>10)
    cout<<"I'm in BmBooter::Process"<<endl;
    
  TAGdataDsc* myn_bmraw  = gTAGroot->FindDataDsc("myn_bmraw", "TABMntuRaw");
  TAGdataDsc* myn_bmtrk  = gTAGroot->FindDataDsc("myn_bmtrk", "TABMntuTrack");
  //~ if (myn_bmtrk == 0 || myn_bmraw==0) {
      //~ cout << "ERROR in BmBooter:: myn_bmraw or myn_bmtrk not found" << endl;
      //~ exit(0);
    //~ }
  bmraw = (TABMntuRaw*) myn_bmraw->GenerateObject();//così facendo lui fa l'action che lo genera
  bmtrack = (TABMntuTrack*) myn_bmtrk->GenerateObject();

  for (int i = 0; i < bmraw->nhit; i++)  {
    TABMntuHit* p_hit = bmraw->Hit(i);
    histo1->Fill(p_hit->Dist());
  }
  
  if (GlobalPar::GetPar()->Debug()>10)
    cout<<"I finished BmBooter::Process"<<endl;

return;
}





//----------------------------------------------------------------------------------------------------
void BmBooter::Finalize() {
  
  if (GlobalPar::GetPar()->Debug()>10)
    cout<<"I'm in BmBooter::Finalize"<<endl;
  
  TCanvas* mirror = new TCanvas("bmstuff", "bmstuff",  700, 700);
  histo1->Draw();
  mirror->SaveAs("bmstuff.png");
  
  if (GlobalPar::GetPar()->Debug()>10)
    cout<<"I finished BmBooter::Finalize"<<endl;
    
return;
}




































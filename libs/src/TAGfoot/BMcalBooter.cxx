
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




void BMcalBooter::Initialize( TString wd_in ) {
    m_wd=wd_in;  
  
    //detector positioning and global to local transformations
    fGeoTrafo = new TAGgeoTrafo();   
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
    initBMCon();

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
  m_tempo_kal+=(double)(end_kal-start_kal);

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


    if (GlobalPar::GetPar()->Debug() > 1)   eventListFile.close();

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
void BMcalBooter::MonitorBMNew(Long64_t jentry) {
  
  
  
  
  return;
  }
// void BMcalBooter::MonitorBMVTMat() {}
// void BMcalBooter::CalibBMVT() {}


//----------------------------------------------------------------------------------------------------
void BMcalBooter::initBMCon()  {

  Int_t i_run = gTAGroot->CurrentRunNumber();
  Int_t i_cam = gTAGroot->CurrentCampaignNumber();

  cout << "Loading beamcon for cam/run = " << i_cam << "/" << i_run << endl;
  TABMparCon* o_beamcon = (TABMparCon*) myp_bmcon->Object();
  o_beamcon->Clear();

  Bool_t b_bad = kTRUE;

  TString filename = m_wd + "/config/beammonitor.cfg";

  cout << "   from file " << filename << endl;

  b_bad = o_beamcon->FromFile(filename);

  filename = m_wd + "/config/beammonitor_t0s.cfg";

  o_beamcon->loadT0s(filename);

  //~ filename = m_wd + "/config/file_stlist_FIRST.txt";
  //  filename = "config/file_stlist_8020_Cst1_1750.txt";

  //~ o_beamcon->LoadSTrel(filename);

  //~ o_beamcon->SetIsMC(true);//provv dovrebbe farlo nel momento della lettura del file

  //~ o_beamcon->ConfigureTrkCalib();

  filename = m_wd + "/config/bmreso_vs_r.root";
  o_beamcon->LoadReso(filename);

  if (!b_bad) myp_bmcon->SetBit(TAGparaDsc::kValid);

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




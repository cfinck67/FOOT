
#include <TCanvas.h>
#include <TError.h>
#include <TMath.h>

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
// #include "TAVTparCal.hxx"
#include "TAVTdatRaw.hxx"
#include "TAVTntuRaw.hxx"
// #include "TAVTntuCluster.hxx"
//#include "TAVTntuTrack.hxx"
// #include "TAVTntuVertex.hxx"
// #include "TAVTactNtuVertex.hxx"
// #include "TAVTactNtuVertexPD.hxx"
#include "TAVTactNtuMC.hxx"
// #include "TAVTactNtuClusterF.hxx"
// #include "TAVTactNtuTrack.hxx"
// #include "TAVTactNtuTrackH.hxx"
// #include "TAVTactNtuTrackF.hxx"

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

//MicroStrip Detector
#include "TAMSDparGeo.hxx"
#include "TAMSDparConf.hxx"
#include "TAMSDparMap.hxx"
#include "TAMSDntuRaw.hxx"
#include "TAMSDactNtuMC.hxx"


//Tof Wall (scintillator)
#include "TATWparGeo.hxx"
#include "TATWparMap.hxx"
#include "TATWdatRaw.hxx"
#include "TATWactNtuMC.hxx"

//Calorimeter
#include "TACAparGeo.hxx"
#include "TACAparMap.hxx"
#include "TACAdatRaw.hxx"
#include "TACAactNtuMC.hxx"

#include "foot_geo.h"

#include "Materials.hxx"
#include "FootField.hxx"
#include "MeasurementCreator.h"

#include <iostream>
#include <vector>



#include "Booter.hxx"

using namespace std;



//----------------------------------------------------------------------------------------------------
Booter::Booter() {



}




void Booter::Initialize( EVENT_STRUCT* evStr ) {


    // debug fie
    if ( GlobalPar::GetPar()->Debug() > 1 ) {
        eventListFile.open( ((string)getenv("FOOTLEVEL0")+"/"+"eventListFile.dat").c_str(), fstream::trunc | fstream::out );
        if ( !eventListFile.is_open() )        cout<< "ERROR  -->  eventListFile.dat cannot open file."<< endl, exit(0);
    }


	//Initializing the Geometry class that handles the
    //detector positioning and global to local transformations
    fGeoTrafo = new TAGgeoTrafo();   
    TString filename = m_wd + "/FOOT_geo.map";   // obsolete, to be removed carefully
    fGeoTrafo->InitGeo(filename.Data());


    
    // Setting up the detectors that we want to decode.    
    if( GlobalPar::GetPar()->IncludeEvent() )           FillMCEvent(evStr);
    if( GlobalPar::GetPar()->IncludeBM() )              FillMCBeamMonitor(evStr);
    if( GlobalPar::GetPar()->IncludeIR() )              FillMCInteractionRegion(evStr);
    if( GlobalPar::GetPar()->IncludeInnerTracker() )    FillMCInnerTracker(evStr);
    if( GlobalPar::GetPar()->IncludeVertex() )          FillMCVertex(evStr);
    if( GlobalPar::GetPar()->IncludeMSD() )             FillMCMSD(evStr);
    if( GlobalPar::GetPar()->IncludeTW() )              FillMCTofWall(evStr);
    if( GlobalPar::GetPar()->IncludeCA() )              FillMCCalorimeter(evStr);


	cout << "Make Geo" << endl;

    TGeoManager *masterGeo = new TGeoManager("genfitGeom", "GENFIT geometry");
    
	Materials* listMaterials = new Materials() ;
    // listMaterials->PrintCompMap();
    // 	listMaterials->PrintMatMap();

    top = gGeoManager->MakeBox("TOPPER", gGeoManager->GetMedium("AIR"), 25., 25., 120.);
    gGeoManager->SetTopVolume(top); // mandatory !


    string magFieldMapName = GlobalPar::GetPar()->MagFieldInputMapName();
    // genfit::FieldManager::getInstance()->init(new genfit::ConstField(0. ,10., 0.)); // 1 T
    // genfit::FieldManager::getInstance()->init(new genfit::ConstField(0. ,0., 0.)); // no mag
    genfit::FieldManager::getInstance()->init( new FootField(magFieldMapName.c_str()) ); // variable field
    // genfit::FieldManager::getInstance()->init( new FootField( 7 ) ); // const field

    // MagFieldTest();


    if( GlobalPar::GetPar()->IncludeBM() ) {
    //     // DisplayBeamMonitor(pg);
      shared_ptr<TABMparGeo> m_bmgeo = shared_ptr<TABMparGeo> ( (TABMparGeo*) myp_bmgeo->Object() );
      m_bmgeo->InitGeo();
      m_bmgeo->ShiftBmon();
    //     //    DisplayIRMonitor(pg,&evStr);
    }


    if ( GlobalPar::GetPar()->IncludeVertex() ) {
        m_vtgeo = shared_ptr<TAVTparGeo> ( (TAVTparGeo*) myp_vtgeo->Object() );
        //Initialization of VTX parameters
        m_vtgeo->InitGeo();
        top->AddNode( m_vtgeo->GetVolume(), 0, new TGeoCombiTrans( 0,0,0,new TGeoRotation("Vertex",0,0,0)) );

    }


    if( GlobalPar::GetPar()->IncludeInnerTracker() ) {
      m_itgeo = shared_ptr<TAITparGeo> ( (TAITparGeo*) myp_itgeo->Object() );
        //Initialization of IT parameters
        m_itgeo->InitGeo();
        // m_itgeo->PrintBodies("geppo");
        // m_itgeo->PrintRegions("geppo");
        // m_itgeo->PrintAssignMaterial("geppo");
        top->AddNode( m_itgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  0, new TGeoRotation("InnerTracker",0,0,0)) );
    }


    if( GlobalPar::GetPar()->IncludeMSD() ) {
      m_msdgeo = shared_ptr<TAMSDparGeo> ( (TAMSDparGeo*) myp_msdgeo->Object() );
        //Initialization of MSD parameters
        m_msdgeo->InitGeo();
        top->AddNode( m_msdgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  0, new TGeoRotation("Strip",0,0,0)) );
    }
    
    if( GlobalPar::GetPar()->IncludeTW() ) {
      shared_ptr<TATWparGeo> m_twgeo = shared_ptr<TATWparGeo> ( (TATWparGeo*) myp_twgeo->Object() );
      //Initialization of SCINT parameters
      m_twgeo->InitGeo();
      top->AddNode( m_twgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0, 0, new TGeoRotation("Scint",0,0,0)) );
      // top->AddNode( m_twgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  m_twgeo->GetCenter().z(), new TGeoRotation("Scint",0,0,0)) );
    }    

    if( GlobalPar::GetPar()->IncludeCA() ) {
        // shared_ptr<TACAparGeo> m_cageo = shared_ptr<TACAparGeo> ( (TACAparGeo*) myp_cageo->Object() );
        // m_cageo->InitGeo();
        // top->AddNode( m_cageo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  m_cageo->GetCenter().z(), new TGeoRotation("Strip",0,0,0)) );
    
    }
 

    if( GlobalPar::GetPar()->IncludeIR() ) {
        // shared_ptr<TAIRparGeo>  m_irgeo = shared_ptr<TAIRparGeo> ( (TAIRparGeo*) myp_irgeo->Object() );
        // m_irgeo->InitGeo();
        // top->AddNode( m_irgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  m_irgeo->GetCenter().z(), new TGeoRotation("Strip",0,0,0)) );
    }


    // set material and geometry into genfit
    MaterialEffects* materialEffects = MaterialEffects::getInstance();
    materialEffects->init(new TGeoMaterialInterface());

    //--- close the geometry
    // gGeoManager->CloseGeometry();

    //--- draw the ROOT box
    gGeoManager->SetVisLevel(10);

    //    GeoPrint();


    // Initialisation of KFfitter
    if ( GlobalPar::GetPar()->Debug() > 1 )       cout << "KFitter init!" << endl;
    m_kFitter = new KFitter();
    if ( GlobalPar::GetPar()->Debug() > 1 )       cout << "KFitter init done!" << endl;
    
}








void Booter::Process( Long64_t jentry ) {


        if ( GlobalPar::GetPar()->IncludeBM() ) {
                MonitorBMNew(jentry); // Yun
        }

        // //to be moved to framework
	// if( GlobalPar::GetPar()->IncludeVertex() && GlobalPar::GetPar()->IncludeInnerTracker() )
	// AssociateHitsToParticle();


        // Kalman
        if( GlobalPar::GetPar()->IncludeVertex() && GlobalPar::GetPar()->IncludeKalman() ) {
            m_kFitter->UploadHitsVT( myn_vtraw, m_vtgeo );
        }

        if( GlobalPar::GetPar()->IncludeInnerTracker() && GlobalPar::GetPar()->IncludeKalman() ) {
            m_kFitter->UploadHitsIT( myn_itraw, m_itgeo );
        }

        if( GlobalPar::GetPar()->IncludeMSD() && GlobalPar::GetPar()->IncludeKalman() ) {
            m_kFitter->UploadHitsMSD( myn_msdraw, m_msdgeo );
        }


        // start time
        start_kal = clock();

        // Kalman Filter
        int isKalmanConverged = 0;
        if ( GlobalPar::GetPar()->IncludeKalman() ) {
             // check other tracking systems are enabled
            if ( GlobalPar::GetPar()->Debug() > 0 )         cout << "MakeFit" << endl;
            isKalmanConverged = m_kFitter->MakeFit( jentry );
            if ( GlobalPar::GetPar()->Debug() > 0 )         cout << "MakeFit done. Converged = " << isKalmanConverged << endl;

            if ( isKalmanConverged == 1 && GlobalPar::GetPar()->Debug() > 1 )    eventListFile << jentry<< endl;

        }
        // stop time
        end_kal = clock();
        m_tempo_kal+=(double)(end_kal-start_kal);



}


void Booter::Finalize() {

	if ( GlobalPar::GetPar()->IncludeKalman() )      m_kFitter->Finalize();

    if (GlobalPar::GetPar()->Debug() > 1)   eventListFile.close();

}










void Booter::MagFieldTest() {

    FootField * ff = new FootField( GlobalPar::GetPar()->MagFieldInputMapName().c_str() );
    if ( GlobalPar::GetPar()->Debug() > 1 )       cout << endl << "Magnetic Field test  ", genfit::FieldManager::getInstance()->getFieldVal( TVector3( 1,1,14.7 ) ).Print();
    if ( GlobalPar::GetPar()->Debug() > 1 )       cout << endl << "Magnetic no Field test  ", genfit::FieldManager::getInstance()->getFieldVal( TVector3( 0,0,2 ) ).Print();
    cout << "Total mag field on the FOOT axis (from 0 to 40 cm) = " << ff->IntegralField( 4000, 0, 40 ) << endl;


    // print out of the magnetic field
    if ( GlobalPar::GetPar()->Debug() > 1 ) {
        ofstream bFieldTest;
        bFieldTest.open("bField.test", std::ofstream::out | std::ofstream::trunc );

        bFieldTest << setiosflags(ios::fixed) << setprecision(8);

        float zCoord = -29.75 + 14;     // global coordinates
        for (int k=0; k<120; k++) {
            float yCoord = -4.75;
            for (int j=0; j<20; j++) {
                float xCoord = -4.75;
                for (int i=0; i<20; i++) {

                    TVector3 vecB = genfit::FieldManager::getInstance()->getFieldVal( TVector3( xCoord, yCoord, zCoord ) );
                    // bFieldTest << vecB.x() << " " << vecB.y() << " " << vecB.z() << endl;
                    bFieldTest << " " << xCoord << " " << yCoord << " " << zCoord-14;
                    bFieldTest << "  " << genfit::FieldManager::getInstance()->getFieldVal( TVector3( xCoord, yCoord, zCoord ) ).x() / 10
                                << "  " << genfit::FieldManager::getInstance()->getFieldVal( TVector3( xCoord, yCoord, zCoord ) ).y() / 10
                                << "  " << genfit::FieldManager::getInstance()->getFieldVal( TVector3( xCoord, yCoord, zCoord ) ).z() / 10 << endl;
                    xCoord += 0.5;
                }
                yCoord += 0.5;
            }
            zCoord += 0.5;
        }
        bFieldTest.close();
    }


}





void Booter::GeoPrint() {

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







//----------------------------------------------------------------------------------------------------
void Booter::AssociateHitsToParticle() {

    // to be done for all particles ---  full revision
  TAGntuMCeve*  p_ntumceve = (TAGntuMCeve*)   myn_mceve->GenerateObject();
  vector<int> FragIdxs;
  int nhitmc = p_ntumceve->nhit;
  cout << " nhitmc   "<< nhitmc << endl;
  for(int i=0; i<nhitmc; i++){
    TAGntuMCeveHit *myPart = p_ntumceve->Hit(i);
    
    int part_reg = myPart->Reg();

    //Require that particle is produced inside the TG
    if(part_reg == 3) {
      FragIdxs.push_back(i);
    }
  }

  // //Pixels stuff
  // TAVTntuRaw*  p_nturaw = (TAVTntuRaw*)   myn_vtraw->GenerateObject();

  // int tmp_vtxid(0);
  // TAVTntuHit* hit;

  // //inner tracker stuff
  // TAITntuRaw*  p_itnturaw = (TAITntuRaw*)   myn_itraw->GenerateObject();

  // int tmp_itid(0);
  // TAITntuHit* hitIT;

  // accociate maybe a pointer instead?
  // for(int t_frg = 0; t_frg<FragIdxs.size(); t_frg++) {

  //   //Check VTX pixels
  //   for(int i=0; i<p_nturaw->GetPixelsN(0); i++){
  //     hit = p_nturaw->GetPixel(0,i);
  //     tmp_vtxid = hit->GetMCid()-1;
  //     if(tmp_vtxid == FragIdxs.at(t_frg)){
  //   if(m_debug) cout<<" Vtx hit associated to part "<<t_frg<<" That is a:: "<<p_ntumceve->Hit(t_frg)->FlukaID()<<"and has charge, mass:: "<<p_ntumceve->Hit(t_frg)->Chg()<<" "<<p_ntumceve->Hit(t_frg)->Mass()<<" "<<endl;
  //     }
  //   }

  //   //Check IT pixels
  //   for(int i=0; i<p_itnturaw->GetPixelsN(0); i++){
  //     hitIT = p_itnturaw->GetPixel(0,i);
  //     tmp_itid = hitIT->GetMCid()-1;
  //     if(tmp_itid == FragIdxs.at(t_frg)){
  //   if(m_debug) cout<<" IT hit associated to part "<<t_frg<<" That is a:: "<<p_ntumceve->Hit(t_frg)->FlukaID()<<"and has charge, mass:: "<<p_ntumceve->Hit(t_frg)->Chg()<<" "<<p_ntumceve->Hit(t_frg)->Mass()<<" "<<endl;
  //     }
  //   }

  // }//Loop on fragments

}

// void Booter::DisplayIRMonitor(TAGpadGroup* pg, EVENT_STRUCT *myStr) {}






// //----------------------------------------------------------------------------------------------------
// void Booter::DisplayBeamMonitor(TAGpadGroup* pg) {

//   TCanvas *c_bmhview;
//   c_bmhview = new TCanvas("bmhview", "Beam Monitor - horizontal view",20,20,700,900);
//   pg->AddPad(c_bmhview);

//   TAGview* pbmh_view = new TABMvieTrackFOOT(myn_bmtrk,
// 					     myn_bmraw,
// 					     myp_bmgeo);
//   pbmh_view->Draw();
// }






//----------------------------------------------------------------------------------------------------
void Booter::FillMCEvent(EVENT_STRUCT *myStr) {

  /*Ntupling the general MC event information*/


  myn_mceve    = new TAGdataDsc("myn_mceve", new TAGntuMCeve());
  mya_mceve    = new TAGactNtuMCeve("mya_mceve", myn_mceve, myStr);

  //  my_out->SetupElementBranch(myn_mceve,     "mceve.");
    /*Ntupling the general MC mimosa information*/
  
  // myn_mcmimo    = new TAGdataDsc("myn_mcmimo", new TAGntuMCmimo());
  // new TAGactNtuMCmimo("an_mcmimo", myn_mcmimo, myStr);
  // my_out->SetupElementBranch(myn_mcmimo,     "mcmimo.");

}








//----------------------------------------------------------------------------------------------------
void Booter::FillMCBeamMonitor(EVENT_STRUCT *myStr) {

  /*Ntupling the MC Beam Monitor information*/
  myn_bmraw    = new TAGdataDsc("myn_bmraw", new TABMntuRaw());
  myp_bmcon  = new TAGparaDsc("myp_bmcon", new TABMparCon());

  initBMCon(myp_bmcon);

  myp_bmgeo  = new TAGparaDsc("p_bmgeo", new TABMparGeo());

  initBMGeo(myp_bmgeo);

  new TABMactNtuMC("an_bmraw", myn_bmraw, myp_bmcon, myp_bmgeo, myStr);

  // my_out->SetupElementBranch(myn_bmraw,     "bmrh.");

  myn_bmtrk    = new TAGdataDsc("myn_bmtrk", new TABMntuTrack());

  new TABMactNtuTrack("an_bmtrk", myn_bmtrk, myn_bmraw, myp_bmgeo, myp_bmcon);

  // my_out->SetupElementBranch(myn_bmtrk,     "bmtrk.");

}









//----------------------------------------------------------------------------------------------------
void Booter::FillMCTofWall(EVENT_STRUCT *myStr) {

  /*Ntupling the MC Tof Wall information*/
  myn_twraw    = new TAGdataDsc("myn_twraw", new TATWdatRaw());
  myp_twgeo    = new TAGparaDsc("twGeo", new TATWparGeo());
  new TATWactNtuMC("an_twraw", myn_twraw, myStr);
  // my_out->SetupElementBranch(myn_twraw,     "twrh.");
}






//----------------------------------------------------------------------------------------------------
void Booter::FillMCCalorimeter(EVENT_STRUCT *myStr) {

  /*Ntupling the MC Calorimeter information*/
  myn_caraw    = new TAGdataDsc("myn_caraw", new TACAdatRaw());
  new TACAactNtuMC("an_caraw", myn_caraw, myStr);
  // my_out->SetupElementBranch(myn_caraw,     "carh.");
}









//----------------------------------------------------------------------------------------------------
void Booter::FillMCInteractionRegion(EVENT_STRUCT *myStr) {

  /*Ntupling the MC Beam Monitor information*/
  myn_irraw    = new TAGdataDsc("myn_irraw", new TAIRdatRaw());
  new TAIRactNtuMC("an_irraw", myn_irraw, myStr);
  // my_out->SetupElementBranch(myn_irraw,     "irrh.");

}







//----------------------------------------------------------------------------------------------------
void Booter::FillMCVertex(EVENT_STRUCT *myStr) {
  
   /*Ntupling the MC Vertex information*/
   myn_vtraw    = new TAGdataDsc("vtRaw", new TAVTntuRaw());
   // myn_vtclus   = new TAGdataDsc("vtClus", new TAVTntuCluster());
   // myn_vtrk     = new TAGdataDsc("vtTrack", new TAVTntuTrack());

   myp_vtmap    = new TAGparaDsc("vtMap", new TAVTparMap());

   myp_vtconf  = new TAGparaDsc("vtConf", new TAVTparConf());
   TAVTparConf* parconf = (TAVTparConf*) myp_vtconf->Object();
   TString filename = m_wd + "/config/TAVTdetector.cfg";
   parconf->FromFile(filename.Data());

   myp_vtgeo    = new TAGparaDsc("vtGeo", new TAVTparGeo());
   
   mya_vtraw   = new TAVTactNtuMC("vtActRaw", myn_vtraw, myp_vtgeo, myp_vtmap, myStr);
  
}






//----------------------------------------------------------------------------------------------------
void Booter::FillMCMSD(EVENT_STRUCT *myStr) {

   /*Ntupling the MC Vertex information*/
   myn_msdraw    = new TAGdataDsc("msdRaw", new TAMSDntuRaw());
   // myn_msdclus   = new TAGdataDsc("msdClus", new TAMSDntuCluster());

   myp_msdmap    = new TAGparaDsc("msdMap", new TAMSDparMap());

   myp_msdconf  = new TAGparaDsc("msdConf", new TAMSDparConf());
   TAMSDparConf* parconf = (TAMSDparConf*) myp_msdconf->Object();
   TString filename = m_wd + "/config/TAMSDdetector.cfg";
   parconf->FromFile(filename.Data());

   myp_msdgeo    = new TAGparaDsc("msdGeo", new TAMSDparGeo());

   mya_msdraw   = new TAMSDactNtuMC("msdActRaw", myn_msdraw, myp_msdgeo, myp_msdmap, myStr);
}






//----------------------------------------------------------------------------------------------------
void Booter::FillMCInnerTracker(EVENT_STRUCT *myStr) {

   /*Ntupling the MC Vertex information*/
   myn_itraw    = new TAGdataDsc("itRaw", new TAITntuRaw());
   // myn_itclus   = new TAGdataDsc("itClus", new TAITntuCluster());

   myp_itmap    = new TAGparaDsc("itMap", new TAITparMap());

   // remove?
   myp_itconf  = new TAGparaDsc("itConf", new TAITparConf());
   TAITparConf* parconf = (TAITparConf*) myp_itconf->Object();
   TString filename = m_wd + "/config/TAITdetector.cfg";
   parconf->FromFile(filename.Data());

   myp_itgeo    = new TAGparaDsc("itGeo", new TAITparGeo());
   mya_itraw   = new TAITactNtuMC("itActRaw", myn_itraw, myp_itgeo, myp_itmap, myStr);

}




void Booter::MonitorBM() {}
//Yun new graphs:
void Booter::MonitorBMNew(Long64_t jentry) {}
// void Booter::MonitorBMVTMat() {}
// void Booter::CalibBMVT() {}


//----------------------------------------------------------------------------------------------------
void Booter::initBMCon(TAGparaDsc* beamcon)  {

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


void Booter::initBMGeo(TAGparaDsc* p_bmgeo)  {

  p_bmgeo = gTAGroot->FindParaDsc("p_bmgeo", "TABMparGeo");
  if (p_bmgeo == 0) {
    cout << "p_bmgeo not found or holding wrong parameter object type" << endl;
    return;
  }

  Int_t i_run = gTAGroot->CurrentRunNumber();
  Int_t i_cam = gTAGroot->CurrentCampaignNumber();

  Info("Booter::initBMGeo","Loading p_bmgeo for cam/run = %d / %d",i_cam,i_run);

  TABMparGeo* o_bmgeo = (TABMparGeo*) p_bmgeo->Object();

  //Initialization of BM parameters
  o_bmgeo->InitGeo();

  p_bmgeo->SetBit(TAGparaDsc::kValid);

  return;

}


void Booter::bookHisto(TFile *f) {}






































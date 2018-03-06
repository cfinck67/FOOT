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
// #include "TAVTparCal.hxx"
#include "TAVTdatRaw.hxx"
#include "TAVTntuRaw.hxx"
// #include "TAVTntuCluster.hxx"
// #include "TAVTntuTrack.hxx"
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
#include "TATWparMap.hxx"
#include "TATWdatRaw.hxx"
#include "TATWactNtuMC.hxx"

//Calorimeter
#include "TACAparMap.hxx"
#include "TACAdatRaw.hxx"
#include "TACAactNtuMC.hxx"

#include "foot_geo.h"

#include "Materials.hxx"
#include "FootField.hxx"
#include "MeasurementCreator.h"

#include <iostream>
#include <vector>

using namespace std;




//----------------------------------------------------------------------------------------------------
RecoTools::RecoTools(int d, TString istr, bool list, TString ostr, TString wd, int nev,
		     TFile *hf) {

    cout << "\tstart Constructor RecoTools\n";

    my_files.clear();
    m_debug = GlobalPar::GetPar()->Debug();

    m_oustr = ostr;
    m_wd = wd;

    m_tempo_kal=0;

    // take input file list
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
    gErrorIgnoreLevel = kError;

    m_hf = hf;
    cout << "\tend Constructor RecoTools\n";
}






//----------------------------------------------------------------------------------------------------
void RecoTools::RecoLoop(TAGroot *tagr, int fr) {


    // debug fie
    fstream eventListFile;
    if ( GlobalPar::GetPar()->Debug() > 1 ) {
        eventListFile.open( ((string)getenv("FOOTLEVEL0")+"/"+"eventListFile.dat").c_str(), fstream::trunc | fstream::out );
        if ( !eventListFile.is_open() )        cout<< "ERROR  -->  eventListFile.dat cannot open file."<< endl, exit(0);
    }

    //Initializing the Geometry class that handles the
    //detector positioning and global to local transformations
    fGeoTrafo = new TAGgeoTrafo();   
    TString filename = m_wd + "/FOOT_geo.map";   // obsolete, to be removed carefully
    fGeoTrafo->InitGeo(filename.Data());

    // input ntuple tree
    TChain *tree = new TChain("EventTree");
    for(unsigned int ifi=0; ifi<my_files.size(); ifi++) {
        tree->Add(my_files.at(ifi).data());
        cout<<"Adding :: "<<my_files.at(ifi).data() << " file"<<endl;
    }

    // create the event object
    if(m_debug) cout<<" Creating Eve "<<endl;
    Evento *ev =  new Evento();
    EVENT_STRUCT evStr;
    TAGpadGroup* pg = new TAGpadGroup();

    ev->FindBranches(tree,&evStr);

    if(m_debug) cout<<" Found branches "<<endl;

    //Configure the output flagging
    tagr->SetCampaignNumber(100);
    tagr->SetRunNumber(1);

    //Define the output file content.
    my_out = new TAGactTreeWriter("my_out");

    /*
    Setting up the detectors that we want to decode.
    */
    bool m_doEvent = kTRUE;
    bool m_doKalman = kTRUE;
    // bool m_doKalman = kFALSE;
    // bool m_doBM = kFALSE;
    bool m_doBM = kFALSE;
    bool m_doIR = kFALSE;
    bool m_doTW = kFALSE;
    bool m_doMSD = kTRUE;
    // bool m_doMSD = kFALSE;
    bool m_doCA = kFALSE;
    // bool m_doInnerTracker = kFALSE;
    bool m_doInnerTracker = kTRUE;
    // bool m_doVertex = kFALSE;
    bool m_doVertex = kTRUE;


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

    if(m_doMSD)
    FillMCMSD(&evStr);

    if(m_doTW)
    FillMCTofWall(&evStr);

    if(m_doCA)
    FillMCCalorimeter(&evStr);


    tagr->AddRequiredItem("my_out");
    tagr->Print();
    if (my_out->Open(m_oustr, "RECREATE")) return;


    cout << "Make Geo" << endl;

    TGeoManager *masterGeo = new TGeoManager("genfitGeom", "GENFIT geometry");
    
	Materials* listMaterials = new Materials() ;
    listMaterials->PrintCompMap();
	listMaterials->PrintMatMap();

    TGeoVolume *top = gGeoManager->MakeBox("TOPPER", gGeoManager->GetMedium("AIR"), 25., 25., 80.);
    gGeoManager->SetTopVolume(top); // mandatory !


    string magFieldMapName = "DoubleDipole.table";
    // string magFieldMapName = "DoubleGaussMag.table";
    // genfit::FieldManager::getInstance()->init(new genfit::ConstField(0. ,10., 0.)); // 1 T
    // genfit::FieldManager::getInstance()->init(new genfit::ConstField(0. ,0., 0.)); // no mag
    genfit::FieldManager::getInstance()->init( new FootField(magFieldMapName.c_str()) ); // variable field
    // genfit::FieldManager::getInstance()->init( new FootField( 7 ) ); // const field

    FootField * ff = new FootField( magFieldMapName.c_str() );
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



    // if(m_doBM) {
    //     // DisplayBeamMonitor(pg);
    //   TABMparGeo* m_bmgeo = shared_ptr<TABMparGeo> ( (TABMparGeo*) myp_bmgeo->Object() );
    //   m_bmgeo->InitGeo();
    //   m_bmgeo->ShiftBmon();
    //   // m_bmgeo->PrintBodies("geppo");
    //   // m_bmgeo->PrintRegions("geppo");
    //     //    DisplayIRMonitor(pg,&evStr);
    // }


    if (m_doVertex) {

        m_vtgeo = shared_ptr<TAVTparGeo> ( (TAVTparGeo*) myp_vtgeo->Object() );
        //Initialization of VTX parameters
        m_vtgeo->InitGeo();
        top->AddNode( m_vtgeo->GetVolume(), 0, new TGeoCombiTrans( 0,0,0,new TGeoRotation("Vertex",0,0,0)) );

    }

    if(m_doInnerTracker) {
      m_itgeo = shared_ptr<TAITparGeo> ( (TAITparGeo*) myp_itgeo->Object() );

        //Initialization of IT parameters
        m_itgeo->InitGeo();
        // m_itgeo->PrintBodies("geppo");
        // m_itgeo->PrintRegions("geppo");
        // m_itgeo->PrintAssignMaterial("geppo");
        top->AddNode( m_itgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  0, new TGeoRotation("InnerTracker",0,0,0)) );
    }

    if(m_doMSD) {

      m_msdgeo = shared_ptr<TAMSDparGeo> ( (TAMSDparGeo*) myp_msdgeo->Object() );

        //Initialization of MSD parameters
        m_msdgeo->InitGeo();
        top->AddNode( m_msdgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  0, new TGeoRotation("Strip",0,0,0)) );
    }


    // if(m_doTW) {
      
    //   TATWparGeo* m_twgeo = shared_ptr<TATWparGeo> ( (TATWparGeo*) myp_twgeo->Object() );

    //   //Initialization of TW parameters
    //   // m_twgeo->InitGeo();
    //   // m_twgeo->PrintBodies("geppo");
    //   // m_twgeo->PrintRegions("geppo");
    //   // top->AddNode( m_twgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  m_twgeo->GetCenter().z(), new TGeoRotation("Strip",0,0,0)) );
      
    // }
    

  //   if(m_doCA) {
      
  //   TACAparGeo* m_cageo = shared_ptr<TACAparGeo> ( (TACAparGeo*) myp_cageo->Object() );
 
  //   //Initialization of CA parameters
  //   // m_cageo->InitGeo();
  //   // m_cageo->PrintBodies("geppo");
  //   // m_cageo->PrintRegions("geppo");
  //   // top->AddNode( m_cageo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  m_cageo->GetCenter().z(), new TGeoRotation("Strip",0,0,0)) );
    
  // }
 
  // if(m_doIR) {
      
  //   m_irgeo = shared_ptr<TAIRparGeo> ( (TAIRparGeo*) myp_irgeo->Object() );
 
  //   //Initialization of IR parameters
  //   // m_irgeo->InitGeo();
  //   // m_irgeo->PrintBodies("geppo");
  //   // m_irgeo->PrintRegions("geppo");
  //   // top->AddNode( m_irgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  m_irgeo->GetCenter().z(), new TGeoRotation("Strip",0,0,0)) );
  //   }


    // set material and geometry into genfit
    MaterialEffects* materialEffects = MaterialEffects::getInstance();
    materialEffects->init(new TGeoMaterialInterface());

    //--- close the geometry
    // gGeoManager->CloseGeometry();

    //--- draw the ROOT box
    gGeoManager->SetVisLevel(10);

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


    const int nIter = 20; // max number of iterations
    const double dPVal = 1.E-3; // convergence criterion


    // vector<genfit::eMeasurementType> measurementTypes;
    // for (unsigned int i = 0; i<nMeasurements; ++i) {
    //     measurementTypes.push_back(genfit::eMeasurementType(8));
    //     // measurementTypes.push_back(genfit::eMeasurementType(i%8));
    // }

    // Initialisation of KFfitter
    if ( GlobalPar::GetPar()->Debug() > 1 )       cout << "KFitter init!" << endl;
    m_kFitter = new KFitter ( nIter, dPVal );
    if ( GlobalPar::GetPar()->Debug() > 1 )       cout << "KFitter init done!" << endl;

    /***********  The event Loop   ****************************************   */
    tagr->BeginEventLoop();
    Long64_t nentries = tree->GetEntries();
    Long64_t nbytes = 0, nb = 0;
    char flag[200]; bool tobedrawn = kFALSE;
    if(m_nev != 0)      nentries = m_nev;

    if(m_debug)         cout<<" Starting Event Loop "<<endl;

    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        if(m_debug) cout<<" New Eve "<<endl;
        nb = tree->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        // if (jentry>1)  break;
        // if (jentry<33061)  continue;

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        tagr->NextEvent();
        if(!(jentry%fr))        cout<<"Processed:: "<<jentry<<" evts!"<<endl;

        if(m_doBM) {
                MonitorBMNew(jentry); // Yun
                // MonitorBM();
            if(m_doVertex) {
            	// MonitorBMVTMat();
            	// CalibBMVT();
            }
        }

        //to be moved to framework
        if(m_doVertex && m_doInnerTracker)
            AssociateHitsToParticle();

        tobedrawn = kTRUE;

        // Kalman
        if( m_doVertex && m_doKalman ) {
            m_kFitter->UploadHitsVT( myn_vtraw, m_vtgeo );
        }


        if( m_doInnerTracker && m_doKalman ) {
            m_kFitter->UploadHitsIT( myn_itraw, m_itgeo );
        }

        if( m_doMSD && m_doKalman ) {
            m_kFitter->UploadHitsMSD( myn_msdraw, m_msdgeo );
        }


        // start time
        start_kal = clock();

        // Kalman Filter
        int isKalmanConverged = 0;
        if ( m_doKalman ) {
             // check other tracking systems are enabled
            if ( GlobalPar::GetPar()->Debug() > 0 )         cout << "MakeFit" << endl;
            isKalmanConverged = m_kFitter->MakeFit( jentry );
            if ( GlobalPar::GetPar()->Debug() > 0 )         cout << "MakeFit done. Converged = " << isKalmanConverged << endl;

            if ( isKalmanConverged == 1 && GlobalPar::GetPar()->Debug() > 1 )    eventListFile << jentry<< endl;

        }
        // stop time
        end_kal = clock();
        m_tempo_kal+=(double)(end_kal-start_kal);





        if (!pg->IsEmpty() && tobedrawn && !(jentry%fr)) {
            pg->Modified();
            pg->Update();

            sprintf(flag,"plots/%s_%d","Test_MC",(int)jentry);
            pg->Print(flag);
        }


        if(m_debug) cout<<" Loaded Event:: "<<jentry<<endl;

        if ( GlobalPar::GetPar()->Debug() > 0 )         cout << "End event n: " << jentry << endl;

    }
    cout << "End of the event loop " << endl;


    if ( m_doKalman )      m_kFitter->Finalize();

    if (GlobalPar::GetPar()->Debug() > 1)   eventListFile.close();

    tagr->EndEventLoop();
    
    my_out->Print();
    my_out->Close();

    // materialEffects->drawdEdx( 11 );  // to look at it in genfit

    return;
}










//----------------------------------------------------------------------------------------------------
void RecoTools::AssociateHitsToParticle() {

    // to be done for all particles ---  full revision

  TAGntuMCeve*  p_ntumceve = (TAGntuMCeve*)   myn_mceve->GenerateObject();

  vector<int> FragIdxs;
  int nhitmc = p_ntumceve->nhit;
  for(int i=0; i<nhitmc; i++){
    TAGntuMCeveHit *myPart = p_ntumceve->Hit(i);

    int part_reg = myPart->Reg();

    //Require that particle is produced inside the TG
    if(part_reg == 3) {
      FragIdxs.push_back(i);
    }
  }

  //Pixels stuff
  TAVTntuRaw*  p_nturaw = (TAVTntuRaw*)   myn_vtraw->GenerateObject();

  int tmp_vtxid(0);
  TAVTntuHit* hit;

  //inner tracker stuff
  TAITntuRaw*  p_itnturaw = (TAITntuRaw*)   myn_itraw->GenerateObject();

  int tmp_itid(0);
  TAITntuHit* hitIT;

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

void RecoTools::DisplayIRMonitor(TAGpadGroup* pg, EVENT_STRUCT *myStr) {}






//----------------------------------------------------------------------------------------------------
void RecoTools::DisplayBeamMonitor(TAGpadGroup* pg) {

  TCanvas *c_bmhview;
  c_bmhview = new TCanvas("bmhview", "Beam Monitor - horizontal view",20,20,700,900);
  pg->AddPad(c_bmhview);

  TAGview* pbmh_view = new TABMvieTrackFOOT(myn_bmtrk,
					     myn_bmraw,
					     myp_bmgeo);
  pbmh_view->Draw();
}






//----------------------------------------------------------------------------------------------------
void RecoTools::FillMCEvent(EVENT_STRUCT *myStr) {

  /*Ntupling the general MC event information*/
  myn_mceve    = new TAGdataDsc("myn_mceve", new TAGntuMCeve());
  new TAGactNtuMCeve("an_mceve", myn_mceve, myStr);
  my_out->SetupElementBranch(myn_mceve,     "mceve.");

  /*Ntupling the general MC mimosa information*/
  myn_mcmimo    = new TAGdataDsc("myn_mcmimo", new TAGntuMCmimo());
  new TAGactNtuMCmimo("an_mcmimo", myn_mcmimo, myStr);
  my_out->SetupElementBranch(myn_mcmimo,     "mcmimo.");

}








//----------------------------------------------------------------------------------------------------
void RecoTools::FillMCBeamMonitor(EVENT_STRUCT *myStr) {

  /*Ntupling the MC Beam Monitor information*/
  myn_bmraw    = new TAGdataDsc("myn_bmraw", new TABMntuRaw());
  myp_bmcon  = new TAGparaDsc("myp_bmcon", new TABMparCon());

  initBMCon(myp_bmcon);

  myp_bmgeo  = new TAGparaDsc("p_bmgeo", new TABMparGeo());

  initBMGeo(myp_bmgeo);

  new TABMactNtuMC("an_bmraw", myn_bmraw, myp_bmcon, myp_bmgeo, myStr);

  my_out->SetupElementBranch(myn_bmraw,     "bmrh.");

  myn_bmtrk    = new TAGdataDsc("myn_bmtrk", new TABMntuTrack());

  new TABMactNtuTrack("an_bmtrk", myn_bmtrk, myn_bmraw, myp_bmgeo, myp_bmcon);

  my_out->SetupElementBranch(myn_bmtrk,     "bmtrk.");

}









//----------------------------------------------------------------------------------------------------
void RecoTools::FillMCTofWall(EVENT_STRUCT *myStr) {

  /*Ntupling the MC Tof Wall information*/
  myn_twraw    = new TAGdataDsc("myn_twraw", new TATWdatRaw());
  new TATWactNtuMC("an_twraw", myn_twraw, myStr);
  my_out->SetupElementBranch(myn_twraw,     "twrh.");
}






//----------------------------------------------------------------------------------------------------
void RecoTools::FillMCCalorimeter(EVENT_STRUCT *myStr) {

  /*Ntupling the MC Calorimeter information*/
  myn_caraw    = new TAGdataDsc("myn_caraw", new TACAdatRaw());
  new TACAactNtuMC("an_caraw", myn_caraw, myStr);
  my_out->SetupElementBranch(myn_caraw,     "carh.");
}









//----------------------------------------------------------------------------------------------------
void RecoTools::FillMCInteractionRegion(EVENT_STRUCT *myStr) {

  /*Ntupling the MC Beam Monitor information*/
  myn_irraw    = new TAGdataDsc("myn_irraw", new TAIRdatRaw());
  new TAIRactNtuMC("an_irraw", myn_irraw, myStr);
  my_out->SetupElementBranch(myn_irraw,     "irrh.");

}







//----------------------------------------------------------------------------------------------------
void RecoTools::FillMCVertex(EVENT_STRUCT *myStr) {

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
void RecoTools::FillMCMSD(EVENT_STRUCT *myStr) {

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
void RecoTools::FillMCInnerTracker(EVENT_STRUCT *myStr) {

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




void RecoTools::MonitorBM() {}
//Yun new graphs:
void RecoTools::MonitorBMNew(Long64_t jentry) {}
// void RecoTools::MonitorBMVTMat() {}
// void RecoTools::CalibBMVT() {}


//----------------------------------------------------------------------------------------------------
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


void RecoTools::bookHisto(TFile *f) {}









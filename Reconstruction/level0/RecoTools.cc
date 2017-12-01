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

//Drift Chamber
#include "TADCparGeo.hxx"
#include "TADCparCon.hxx"
#include "TADCntuRaw.hxx"
#include "TADCntuTrack.hxx"
#include "TADCactNtuMC.hxx"
#include "TADCactNtuTrack.hxx"
#include "TADCvieTrackFIRST.hxx"

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

#include <iostream>
#include <vector>

using namespace std;

RecoTools::RecoTools(int d, TString istr, bool list, TString ostr, TString wd, int nev,
		     TFile *hf) {

  cout << "\tstart Constructor RecoTools\n";

  my_files.clear();
  m_debug = d;
  m_oustr = ostr;
  m_wd = wd;

  tempo_kal=0;

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
  cout << "\tend Constructor RecoTools\n";
}


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
  TString filename = m_wd + "/FOOT_geo.map";
  fGeoTrafo->InitGeo(filename.Data());

  // Materials* listMaterials=new Materials() ;
  //listMaterials->PrintMap();

  //  TTree *tree = 0;
  TChain *tree = new TChain("EventTree");
  for(unsigned int ifi=0; ifi<my_files.size(); ifi++) {
    tree->Add(my_files.at(ifi).data());
    cout<<"Adding :: "<<my_files.at(ifi).data() << " file"<<endl;
  }

  //  TTree* tree = (TTree*)gDirectory->Get("EventTree");

  // if(m_debug) cout<<" Creating Geo "<<endl;
  // Geometry *my_G = new Geometry();
  // if(m_debug) cout<<" Creating Sig "<<endl;
  // Segnale *my_S = new Segnale(my_G);
  if(m_debug) cout<<" Creating Eve "<<endl;
  Evento *ev =  new Evento();
  // Trigger *tr = new Trigger();
  
  // vector < Int_t > RegNumber;
  // vector < TString > RegName;

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
  // bool m_doBM = kFALSE;
  bool m_doBM = kFALSE;
  bool m_doDC = kFALSE;
  bool m_doIR = kFALSE;
  bool m_doTW = kFALSE;
  bool m_doMSD = kTRUE;
  bool m_doCA = kFALSE;
  bool m_doInnerTracker = kTRUE;
  bool m_doVertex = kTRUE;

  bool breakAfterThisEvent = false;

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

  if(m_doMSD)
    FillMCMSD(&evStr);

  if(m_doTW)
    FillMCTofWall(&evStr);

  if(m_doCA)
    FillMCCalorimeter(&evStr);

  tagr->AddRequiredItem("my_out");
  tagr->Print();
  if (my_out->Open(m_oustr, "RECREATE")) return;

  // if (m_flaghisto && m_doVertex) {
  //   mya_vtraw->SetHistogramDir(my_out->File());
  //   mya_vtclus->SetHistogramDir(my_out->File());
  //   mya_vttrack->SetHistogramDir(my_out->File());
  // }



    cout << "Make Geo" << endl;

    TGeoManager *prova=new TGeoManager("genfitGeom", "GENFIT geometry");

    TGeoMixture *airMat = new TGeoMixture("Air",3);
    airMat->AddElement(14.01,7.,.78);   // N
    airMat->AddElement(16.00,8.,.21);   // O
    airMat->AddElement(39.95,18.,.01);  // Ar
    airMat->SetDensity(1.2e-3);
    // airMat->SetPressure();      // std 6.32420e+8 = 1atm
    // 1,26484e+8 MeV/mm3 = 0,2 atm
    // 1 MeV/mm3 = 1,58122538 × 10-9 atm
    cout << "airMat->GetPressure()   " << airMat->GetPressure() << endl;

		Materials* listMaterials = new Materials() ;
		listMaterials->PrintCompMap();

  	TGeoMaterial *matAr = new TGeoMaterial("Argon", 39.948, 18., 0.001662);//densità viene da flair,
    TGeoMaterial *matC = new TGeoMaterial("Carbon", 12.0107, 6., 2.26);
 	  TGeoMaterial *matO = new TGeoMaterial("Oxygen", 16., 8., 0.0013315);
  	TGeoMaterial *matAl = new TGeoMaterial("Aluminium", 26.981539, 13., 2.6989);
   	TGeoMaterial *matSi = new TGeoMaterial("Silicon", 28.085, 14., 2.329);
   	TGeoMaterial *matW = new TGeoMaterial("Tungsten", 183.84, 74., 19.3);// poi magari mettere la copertura in oro
   	TGeoMaterial *vacuum = new TGeoMaterial("Vacuum",0,0,0);//a,z,rho


   	// TGeoMixture *matMylar = new TGeoMixture("Mylar",3,   1.39000    );
    // // matMylar->SetUniqueID(  18);
    // matMylar->DefineElement(0,12.01,6,0.624935);
    // matMylar->DefineElement(1,1.01,1,0.4204392E-01);
    // matMylar->DefineElement(2,16,8,0.3330211);


    // TGeoMixture *matEpo = new TGeoMixture("Epoxy",3,   1.18    );
    // matEpo->AddElement(12,6, 18./40.);  // C
    // matEpo->AddElement(1,1, 19./40.);   // H
    // matEpo->AddElement(16,8, 3./40.);  // O


    TGeoMixture *matSiC = new TGeoMixture("SiliconCarbon",2, 3.22); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    matSiC->AddElement(matC ,0.5);
    matSiC->AddElement(28.085, 14 ,0.5);


    // // CHECK
    // TGeoMixture *matSiCFoam = new TGeoMixture("SiCFoam",2,   0.1288    );
    // matSiCFoam->AddElement(matSiC, 0.04);
    // matSiCFoam->AddElement(airMat, 0.96);


   TGeoMixture *ArCO2 = new TGeoMixture("ArCO2",3);
   ArCO2->AddElement(matAr ,1./4.);
   ArCO2->AddElement(matC ,1./4.);
   ArCO2->AddElement(matO ,2./4.);
//   ArCO2->AddElement(matCO2 ,20.);
   ArCO2->SetDensity(0.001677136); //da flair
   ArCO2->SetPressure(1.26484e+8);    // 0.2 atm
   cout << "ArCO2->GetPressure()   " << ArCO2->GetPressure() << endl;


   // TGeoMixture *matKapton = new TGeoMixture("Kapton",4, 1.42);
   // matKapton->AddElement(1,1, 0.02636 );            // H
   // matKapton->AddElement(matC ,0.691133);
   // matKapton->AddElement(14.01, 7, 0.07327);        // N
   // matKapton->AddElement(16,8, 0.209235);               // O


      int medID = 0;
   TGeoMedium *vacuum_med = new TGeoMedium("Vacuum_med",medID++, gGeoManager->GetMaterial("Vacuum"));
   TGeoMedium *air = new TGeoMedium("Air_med",medID++, gGeoManager->GetMaterial("Air"));
   TGeoMedium *gas_med = new TGeoMedium("ArCO2_med",medID++, gGeoManager->GetMaterial("ArCO2"));
   // TGeoMedium *c_wire_med = new TGeoMedium("catod wire",medID++, gGeoManager->GetMaterial("Aluminium"));
   TGeoMedium *a_wire_med = new TGeoMedium("Tungsten_med",medID++, gGeoManager->GetMaterial("Tungsten"));
   TGeoMedium *a_maylar_med = new TGeoMedium("Mylar_med",medID++, gGeoManager->GetMaterial("Mylar"));
    TGeoMedium* silicon = new TGeoMedium( "Silicon_med", medID++, gGeoManager->GetMaterial("Silicon") );
    TGeoMedium* kapton = new TGeoMedium( "Kapton_med", medID++, gGeoManager->GetMaterial("Kapton") );
    TGeoMedium* epoxy = new TGeoMedium( "Epoxy_med", medID++, gGeoManager->GetMaterial("Epoxy") );
    TGeoMedium* aluminium = new TGeoMedium( "Aluminium_med", medID++, gGeoManager->GetMaterial("Aluminium") );
    TGeoMedium* siCFoam = new TGeoMedium( "SiCFoam_med", medID++, gGeoManager->GetMaterial("SiCFoam") );



//     cout << endl << "List of Materil\n ";
//     TIter next( gGeoManager->GetListOfMaterials() );
//     while ( TGeoMaterial *obj = (TGeoMaterial*) next() ) {
//       cout << obj->GetName () << endl;
//     }
// cout << endl << "List of Media\n ";
//     TIter nnext( gGeoManager->GetListOfMedia() );
//     while ( TGeoMedium *obj = (TGeoMedium *) nnext()  ) {
//       cout << obj->GetName () << endl;
//     }



    TGeoVolume *top = gGeoManager->MakeBox("TOPPER", gGeoManager->GetMedium("AIR"), 25., 25., 80.);
    // TGeoVolume *top = gGeoManager->MakeBox("TOPPER", vacuum_med, 100., 100., 200.);
    gGeoManager->SetTopVolume(top); // mandatory !


    // genfit::FieldManager::getInstance()->init(new genfit::ConstField(0. ,10., 0.)); // 1 T
    // genfit::FieldManager::getInstance()->init(new genfit::ConstField(0. ,0., 0.)); // no mag
    // genfit::FieldManager::getInstance()->init(new FootField("DoubleDipole.table")); // variable field
    genfit::FieldManager::getInstance()->init(new FootField("DoubleGaussMag.table")); // variable field
    // genfit::FieldManager::getInstance()->init( new FootField( 7 ) ); // const field
    // FieldManager::getInstance()->useCache(true, 8);


    if ( GlobalPar::GetPar()->Debug() > 1 )       cout << endl << "Magnetic Field test  ", genfit::FieldManager::getInstance()->getFieldVal( TVector3( 1,1,14.7 ) ).Print();
    if ( GlobalPar::GetPar()->Debug() > 1 )       cout << endl << "Magnetic no Field test  ", genfit::FieldManager::getInstance()->getFieldVal( TVector3( 0,0,2 ) ).Print();


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

    if(m_doBM) {
        // DisplayBeamMonitor(pg);

        //    DisplayIRMonitor(pg,&evStr);
    }

    if (m_doVertex) {

      m_vtgeo = shared_ptr<TAVTparGeo> ( (TAVTparGeo*) myp_vtgeo->Object() );


        //Initialization of VTX parameters
        m_vtgeo->InitGeo();
        top->AddNode( m_vtgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  m_vtgeo->GetCenter().z(), new TGeoRotation("Vertex",0,0,0)) );

    }

    if(m_doInnerTracker) {
      m_itgeo = shared_ptr<TAITparGeo> ( (TAITparGeo*) myp_itgeo->Object() );
      // m_dcgeo = (TADCparGeo*) myp_dcgeo->Object();

        //Initialization of IT parameters
        m_itgeo->InitGeo();
        // top->AddNode( m_itgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  m_itgeo->GetCenter().z(), new TGeoRotation("InnerTracker",0,0,0)) );
    }

    if(m_doMSD) {

      m_msdgeo = shared_ptr<TAMSDparGeo> ( (TAMSDparGeo*) myp_msdgeo->Object() );

        //Initialization of MSD parameters
        m_msdgeo->InitGeo();
        top->AddNode( m_msdgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  m_msdgeo->GetCenter().z(), new TGeoRotation("Strip",0,0,0)) );

    }

    if(m_doDC) {

      m_dcgeo = shared_ptr<TADCparGeo> ( (TADCparGeo*) myp_dcgeo->Object() );
      // m_dcgeo = (TADCparGeo*) myp_dcgeo->Object();

        //Initialization of DC parameters
        m_dcgeo->InitGeo();
        top->AddNode( m_dcgeo->GetVolume(), 0, new TGeoCombiTrans( 0, 0,  m_dcgeo->GetCenter().z(), new TGeoRotation("DriftChamber",0,0,0)) );

    }


    // set material into genfit
    MaterialEffects::getInstance()->init(new TGeoMaterialInterface());

    //--- close the geometry
    gGeoManager->CloseGeometry();

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


    vector<genfit::eMeasurementType> measurementTypes;
    // for (unsigned int i = 0; i<nMeasurements; ++i) {
    //     measurementTypes.push_back(genfit::eMeasurementType(8));
    //     // measurementTypes.push_back(genfit::eMeasurementType(i%8));
    // }

    // Initialisation of KFfitter
    if ( GlobalPar::GetPar()->Debug() > 1 )       cout << "KFitter init!" << endl;
    m_kFitter = new KFitter ( nIter, dPVal );
    if ( GlobalPar::GetPar()->Debug() > 1 )       cout << "KFitter init done!" << endl;



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


        ////////////////////////////////////////////////////////////////////////////////////////////////////

        // if (jentry>1)  break;
        // if (jentry<33061)  continue;

        ////////////////////////////////////////////////////////////////////////////////////////////////////



        tagr->NextEvent();


        if(m_doBM) {
                MonitorBMNew(jentry); // Yun
                // MonitorBM();
            if(m_doVertex) {
            	// MonitorBMVTMat();

            	// CalibBMVT();
            }
        }

        if(!(jentry%fr))
            cout<<"Processed:: "<<jentry<<" evts!"<<endl;



        //do some MC check
        //to be moved to framework
        if(m_doVertex && m_doInnerTracker)
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

             //      TAVTntuCluster*  p_ntuclus =
            	// (TAVTntuCluster*)   myn_vtclus->GenerateObject();
             //      int i_ncl;
             //      //Displays the Clusters in the VTX detector
             //      for(int is=0; is<8; is++) {
            	// i_ncl = p_ntuclus->GetClustersN(is);

            	// for (Int_t i_cl = 0; i_cl < i_ncl; i_cl++) {

            	//   TAVTcluster *acl = p_ntuclus->GetCluster(is,i_cl);
            	//   TVector3 myG = acl->GetPositionG();
            	//   TVector3 myLG(myG.X()*1./10000.,myG.Y()*1./10000.,myG.Z()*1./10000.);
            	//   TVector3 myR = fGeoTrafo->FromVTLocalToGlobal(myLG);

            	// }
             //      }
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
        if( m_doVertex && m_doKalman ) {
            m_kFitter->UploadHitsVT( myn_vtraw, m_vtgeo );
        }


        if( m_doInnerTracker && m_doKalman ) {
            m_kFitter->UploadHitsIT( myn_itraw, m_itgeo );
        }

        if( m_doMSD && m_doKalman ) {
            m_kFitter->UploadHitsMSD( myn_msdraw, m_msdgeo );
        }


        if (m_doDC && m_doKalman ) {

            m_kFitter->UploadHitsDC( myn_dcraw, m_dcgeo );
            if (jentry == 0 && GlobalPar::GetPar()->Debug() > 1  )            m_dcgeo->Print();

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

            // if ( isKalmanConverged == 1  )
              // breakAfterThisEvent = true;

            if ( isKalmanConverged == 1 && GlobalPar::GetPar()->Debug() > 1 )    eventListFile << jentry<< endl;

        }
// stop time
end_kal = clock();
tempo_kal+=(double)(end_kal-start_kal);





        if (!pg->IsEmpty() && tobedrawn && !(jentry%fr)) {
            pg->Modified();
            pg->Update();

            sprintf(flag,"plots/%s_%d","Test_MC",(int)jentry);
            pg->Print(flag);
        }


        if(m_debug) cout<<" Loaded Event:: "<<jentry<<endl;

        if ( GlobalPar::GetPar()->Debug() > 0 )         cout << "End event n: " << jentry << endl;
        if (breakAfterThisEvent)          break;

    }
    cout << "End of the event loop " << endl;


    if ( m_doKalman ) {
        m_kFitter->EvaluateMomentumResolution();
        m_kFitter->PrintEfficiency();
        m_kFitter->Save();
    }

    if (GlobalPar::GetPar()->Debug() > 1)   eventListFile.close();

    tagr->EndEventLoop();
    // if (m_flaghisto) {
    //   mya_vtraw->WriteHistogram();
    //   mya_vtclus->WriteHistogram();
    //   mya_vttrack->WriteHistogram();
    // }
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

 //  for(int t_frg = 0; t_frg<FragIdxs.size(); t_frg++) {

 //    //Check VTX pixels
 //    for(int i=0; i<p_nturaw->GetPixelsN(0); i++){
 //      hit = p_nturaw->GetPixel(0,i);
 //      tmp_vtxid = hit->GetMCid()-1;
 //      if(tmp_vtxid == FragIdxs.at(t_frg)){
	// if(m_debug) cout<<" Vtx hit associated to part "<<t_frg<<" That is a:: "<<p_ntumceve->Hit(t_frg)->FlukaID()<<"and has charge, mass:: "<<p_ntumceve->Hit(t_frg)->Chg()<<" "<<p_ntumceve->Hit(t_frg)->Mass()<<" "<<endl;
 //      }
 //    }

 //    //Check IT pixels
 //    for(int i=0; i<p_itnturaw->GetPixelsN(0); i++){
 //      hitIT = p_itnturaw->GetPixel(0,i);
 //      tmp_itid = hitIT->GetMCid()-1;
 //      if(tmp_itid == FragIdxs.at(t_frg)){
	// if(m_debug) cout<<" IT hit associated to part "<<t_frg<<" That is a:: "<<p_ntumceve->Hit(t_frg)->FlukaID()<<"and has charge, mass:: "<<p_ntumceve->Hit(t_frg)->Chg()<<" "<<p_ntumceve->Hit(t_frg)->Mass()<<" "<<endl;
 //      }
 //    }

 //  }//Loop on fragments

  return;

}

void RecoTools::DisplayIRMonitor(TAGpadGroup* pg, EVENT_STRUCT *myStr) {

  // TCanvas *c_irhview, *c_irhview_z;

  // c_irhview = new TCanvas("irhview", "IR - horizontal view",20,20,1200,1200);
  // pg->AddPad(c_irhview);

  // TAGview* pirh_view = new TAGvieHorzMCIR(myn_bmtrk, myn_bmraw,
		// 			  myp_bmgeo, myn_vtclus,
		// 			  myn_vtrk, myp_vtgeo, myStr);

  // pirh_view->Draw();

  // c_irhview_z = new TCanvas("irhview_z", "IR - Zoom horizontal view",20,20,1200,1200);
  // pg->AddPad(c_irhview_z);

  // TAGview* pirh_view_z = new TAGvieHorzMCIR(myn_bmtrk, myn_bmraw,
		// 			    myp_bmgeo, myn_vtclus,
		// 			    myn_vtrk, myp_vtgeo, myStr);

  // pirh_view_z->Draw("zoom");

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

  myn_bmtrk    = new TAGdataDsc("myn_bmtrk", new TABMntuTrack());

  new TABMactNtuTrack("an_bmtrk", myn_bmtrk, myn_bmraw, myp_bmgeo, myp_bmcon);

  my_out->SetupElementBranch(myn_bmtrk,     "bmtrk.");

  return;
}




void RecoTools::FillMCDriftChamber(EVENT_STRUCT *myStr) {
  cout << "RecoTools::FillMCDriftChamber     ->    start" << endl;
  /*Ntupling the MC Drift Chamber information*/
  myn_dcraw    = new TAGdataDsc("myn_dcraw", new TADCntuRaw());
  myp_dccon  = new TAGparaDsc("myp_dccon", new TADCparCon());

  initDCCon(myp_dccon);

  myp_dcgeo  = new TAGparaDsc("p_dcgeo", new TADCparGeo());
  initDCGeo(myp_dcgeo);

  new TADCactNtuMC("an_dcraw", myn_dcraw, myp_dccon, myp_dcgeo, myStr);
  my_out->SetupElementBranch(myn_dcraw,     "dcrh.");

  myn_dctrk    = new TAGdataDsc("myn_dctrk", new TADCntuTrack());
  new TADCactNtuTrack("an_dctrk", myn_dctrk, myn_dcraw, myp_dcgeo, myp_dccon);
  my_out->SetupElementBranch(myn_dctrk,     "dctrk.");
  cout << "RecoTools::FillMCDriftChamber     ->    end" << endl;
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
   // myn_vtclus   = new TAGdataDsc("vtClus", new TAVTntuCluster());
   // myn_vtrk     = new TAGdataDsc("vtTrack", new TAVTntuTrack());
   // myn_vtvtx    = new TAGdataDsc("vtVtx", new TAVTntuVertex());

   myp_vtmap    = new TAGparaDsc("vtMap", new TAVTparMap());

   myp_vtconf  = new TAGparaDsc("vtConf", new TAVTparConf());
   TAVTparConf* parconf = (TAVTparConf*) myp_vtconf->Object();
   TString filename = m_wd + "/config/TAVTdetector.cfg";
   parconf->FromFile(filename.Data());

   myp_vtgeo    = new TAGparaDsc("vtGeo", new TAVTparGeo());
   // TAVTparGeo* geomap   = (TAVTparGeo*) myp_vtgeo->Object();
   // filename = m_wd + "/geomaps/TAVTdetector.map";
   // geomap->FromFile(filename.Data());

   // myp_vtcal = new TAGparaDsc("vtCal", new TAVTparCal());
   // TAVTparCal* cal   = (TAVTparCal*) myp_vtcal->Object();
   // filename = m_wd + "/config/TAVTdetector.cal";
   // cal->FromFile(filename.Data());

   mya_vtraw   = new TAVTactNtuMC("vtActRaw", myn_vtraw, myp_vtgeo, myp_vtmap, myStr);
   // mya_vtclus  = new TAVTactNtuClusterF("vtActClus", myn_vtraw, myn_vtclus, myp_vtconf, myp_vtgeo);
   // mya_vttrack = new TAVTactNtuTrack("vtActTrack", myn_vtclus, myn_vtrk, myp_vtconf, myp_vtgeo, myp_vtcal);
   // // L algo mya_vttrack = new TAVTactNtuTrackF("vtActTrack", myn_vtclus, myn_vtrk, myp_vtconf, myp_vtgeo, myp_vtcal);
   // mya_vtvtx   = new TAVTactNtuVertexPD("vtActVtx", myn_vtrk, myn_vtvtx, myp_vtconf, myp_vtgeo, myn_bmtrk);
   //IPA   mya_vtvtx   = new TAVTactNtuVertex("vtActVtx", myn_vtrk, myn_vtvtx, myp_vtconf, myp_vtgeo, myn_bmtrk);

   // if (m_flaghisto) {
   //   mya_vtraw->CreateHistogram();
   //   mya_vtclus->CreateHistogram();
   //   mya_vttrack->CreateHistogram();
   //   mya_vtvtx->CreateHistogram();
   // }

   // my_out->SetupElementBranch(myn_vtraw, "vtrh.");
   // my_out->SetupElementBranch(myn_vtclus, "vtclus.");
   // my_out->SetupElementBranch(myn_vtrk, "vtTrack.");
   // my_out->SetupElementBranch(myn_vtvtx, "vtVtx.");
}


void RecoTools::FillMCMSD(EVENT_STRUCT *myStr) {

   /*Ntupling the MC Vertex information*/
   myn_msdraw    = new TAGdataDsc("msdRaw", new TAMSDntuRaw());
   // myn_msdclus   = new TAGdataDsc("msdClus", new TAMSDntuCluster());
   // myn_msdrk     = new TAGdataDsc("msdTrack", new TAMSDntuTrack());
   // myn_msdmsdx    = new TAGdataDsc("msdMsdx", new TAMSDntuVertex());

   myp_msdmap    = new TAGparaDsc("msdMap", new TAMSDparMap());

   myp_msdconf  = new TAGparaDsc("msdConf", new TAMSDparConf());
   TAMSDparConf* parconf = (TAMSDparConf*) myp_msdconf->Object();
   TString filename = m_wd + "/config/TAMSDdetector.cfg";
   parconf->FromFile(filename.Data());

   myp_msdgeo    = new TAGparaDsc("msdGeo", new TAMSDparGeo());
   // TAMSDparGeo* geomap   = (TAMSDparGeo*) myp_msdgeo->Object();
   // filename = m_wd + "/geomaps/TAMSDdetector.map";
   // geomap->FromFile(filename.Data());

   // myp_msdcal = new TAGparaDsc("msdCal", new TAMSDparCal());
   // TAMSDparCal* cal   = (TAMSDparCal*) myp_msdcal->Object();
   // filename = m_wd + "/config/TAMSDdetector.cal";
   // cal->FromFile(filename.Data());

   mya_msdraw   = new TAMSDactNtuMC("msdActRaw", myn_msdraw, myp_msdgeo, myp_msdmap, myStr);
   // mya_msdclus  = new TAMSDactNtuClusterF("msdActClus", myn_msdraw, myn_msdclus, myp_msdconf, myp_msdgeo);
   // mya_msdtrack = new TAMSDactNtuTrack("msdActTrack", myn_msdclus, myn_msdrk, myp_msdconf, myp_msdgeo, myp_msdcal);
   // // L algo mya_msdtrack = new TAMSDactNtuTrackF("msdActTrack", myn_msdclus, myn_msdrk, myp_msdconf, myp_msdgeo, myp_msdcal);
   // mya_msdmsdx   = new TAMSDactNtuVertexPD("msdActMsdx", myn_msdrk, myn_msdmsdx, myp_msdconf, myp_msdgeo, myn_bmtrk);
   //IPA   mya_msdmsdx   = new TAMSDactNtuVertex("msdActMsdx", myn_msdrk, myn_msdmsdx, myp_msdconf, myp_msdgeo, myn_bmtrk);

   // if (m_flaghisto) {
   //   mya_msdraw->CreateHistogram();
   //   mya_msdclus->CreateHistogram();
   //   mya_msdtrack->CreateHistogram();
   //   mya_msdvtx->CreateHistogram();
   // }

   // my_out->SetupElementBranch(myn_msdraw, "msdrh.");
   // my_out->SetupElementBranch(myn_msdclus, "msdclus.");
   // my_out->SetupElementBranch(myn_msdrk, "msdTrack.");
   // my_out->SetupElementBranch(myn_msdmsdx, "msdVtx.");
}


void RecoTools::FillMCInnerTracker(EVENT_STRUCT *myStr) {

   /*Ntupling the MC Vertex information*/
   myn_itraw    = new TAGdataDsc("itRaw", new TAITntuRaw());
   // myn_itclus   = new TAGdataDsc("itClus", new TAITntuCluster());
   // myn_itrk     = new TAGdataDsc("itTrack", new TAITntuTrack());

   myp_itmap    = new TAGparaDsc("itMap", new TAITparMap());

   myp_itconf  = new TAGparaDsc("itConf", new TAITparConf());
   TAITparConf* parconf = (TAITparConf*) myp_itconf->Object();
   TString filename = m_wd + "/config/TAITdetector.cfg";
   parconf->FromFile(filename.Data());

   myp_itgeo    = new TAGparaDsc("itGeo", new TAITparGeo());
   // TAITparGeo* geomap   = (TAITparGeo*) myp_itgeo->Object();
   // filename = m_wd + "/geomaps/TAITdetector.map";
   // geomap->FromFile(filename.Data());

   // myp_itcal = new TAGparaDsc("itCal", new TAITparCal());
   // TAITparCal* cal   = (TAITparCal*) myp_itcal->Object();
   // filename = m_wd + "/config/TAITdetector.cal";
   // cal->FromFile(filename.Data());

   mya_itraw   = new TAITactNtuMC("itActRaw", myn_itraw, myp_itgeo, myp_itmap, myStr);
   // mya_itclus  = new TAITactNtuClusterF("itActClus", myn_itraw, myn_itclus, myp_itconf, myp_itgeo);
   // mya_ittrack = new TAITactNtuTrack("itActTrack", myn_itclus, myn_itrk, myp_itconf, myp_itgeo, myp_itcal);

   // if (m_flaghisto) {
   //   mya_itraw->CreateHistogram();
   //   mya_itclus->CreateHistogram();
   //   mya_ittrack->CreateHistogram();
   // }

   // my_out->SetupElementBranch(myn_itraw, "itrh.");
   // my_out->SetupElementBranch(myn_itclus, "itclus.");
   // my_out->SetupElementBranch(myn_itrk, "itTrack.");
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





//Yun new graphs:
void RecoTools::MonitorBMNew(Long64_t jentry) {
  m_hf->cd();

  TABMntuTrack* p_ntutrk = (TABMntuTrack*) myn_bmtrk->GenerateObject();
  TABMntuRaw* p_ntuhit  =  (TABMntuRaw*) myn_bmraw->GenerateObject();  //da capire come prendere info dagli singoli hit della traccia!
  TABMparGeo* p_bmgeo = (TABMparGeo*)(gTAGroot->FindParaDsc("p_bmgeo", "TABMparGeo")->Object());
  TABMparCon* p_bmcon = (TABMparCon*) (gTAGroot->FindParaDsc("myp_bmcon", "TABMparCon")->Object());
  TABMntuHit* rawhit;
  TABMntuTrackTr* trackTr;

  Int_t i_ntrk = p_ntutrk->ntrk;
  Int_t i_nhit = p_ntuhit->nhit;
  vector<Double_t> realangz_vec, realangphi_vec;
  vector<TVector3> realpos_vec;
  Double_t tmp_double, maxRdriftSmear=10., avReso=0.;
  Int_t tmp_int, nhit_pri_sel_x=0, nhit_pri_sel_y=0, nhit_pri_sel, nhit_sel=0, nhit_pri_all, nhit_pri_all_x=0, nhit_pri_all_y=0;
  bool ReadOnlyPrimary=true, goodEvent=false;
  vector<Int_t> hitxplane(12,0);

  TVector3 target_o(0.,0.,13.9);  //messo a mano, è origine del target nel sistema di riferimento del BM

  //loop on hits
  for (Int_t i_h = 0; i_h < i_nhit; i_h++) {
    rawhit = p_ntuhit->Hit(i_h);
    if(rawhit->GetIdmon()==1){
      realangz_vec.push_back(rawhit->Momentum().Theta()*RAD2DEG);
      realangphi_vec.push_back(rawhit->Momentum().Phi()*RAD2DEG);
      realpos_vec.push_back(rawhit->Position());
      if(rawhit->View()==1)
        nhit_pri_all_x++;
      else if(rawhit->View()==-1)
        nhit_pri_all_y++;
      else
        cout<<"WARNING: something in hitview is wrong!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! check in RecoTools::MonitorBMNew()"<<endl;
      }
    if(rawhit->GetIsSelected()==true){
      nhit_sel++;
      if(rawhit->GetIdmon()==1 && rawhit->View()==1)
        nhit_pri_sel_x++;
      else if(rawhit->GetIdmon()==1 && rawhit->View()==-1)
        nhit_pri_sel_y++;
      else
        ReadOnlyPrimary=false;
      //~ if(rawhit->GetRdriftSmear()<maxRdriftSmear)
      if(p_bmcon->ResoEval(rawhit->GetRealRdrift())<maxRdriftSmear)//provvisorio
        maxRdriftSmear=rawhit->GetRdriftSmear();
      ((TH1D*)gDirectory->Get("NewBM/hitIndex"))->Fill(p_bmgeo->GetBMNcell(rawhit->Plane(),rawhit->View(),rawhit->Cell()));
      hitxplane[rawhit->Plane()]++;
      if(rawhit->GetSigma()>0.02)
        avReso++;;
        //~ avReso+=rawhit->GetSigma();
      if(i_ntrk==1){ //loop on track
        trackTr = p_ntutrk->Track(0);
        if(trackTr->GetIsConverged()==1){//commentami se vuoi mettere condizioni in più come quelli sotto
        //~ if(trackTr->GetIsConverged()==1 && trackTr->GetAngZRes()<0.45 && trackTr->GetAngZRes()>0.3){//provvisorio!
        //~ if(trackTr->GetIsConverged()==1 && trackTr->GetChi2NewRed()<1.){//provvisorio!
          ((TH1D*)gDirectory->Get("NewBM/converged/hitIndex_conv"))->Fill(p_bmgeo->GetBMNcell(rawhit->Plane(),rawhit->View(),rawhit->Cell()));
          ((TH1D*)gDirectory->Get("NewBM/converged/Rdrift_real_conv"))->Fill(rawhit->Dist());
          ((TH1D*)gDirectory->Get("NewBM/converged/Rdrift_smeared_conv"))->Fill(rawhit->GetRealRdrift());
          ((TH1D*)gDirectory->Get("NewBM/converged/smearingRdrift_conv"))->Fill(rawhit->GetRdriftSmear());
          ((TH1D*)gDirectory->Get("NewBM/converged/sigma_conv"))->Fill(rawhit->GetSigma());
          }
        }
      }
    }
    //~ avReso=avReso/nhit_sel;


  //calculate realAngZ and realAngPhi
  if(realangz_vec.size()!=realangphi_vec.size())
    cout<<"WARNING: something is wrong in realangz and realangphi!!!!!check in RecoTools::MonitorBMNew() realangz_vec.size()="<<realangz_vec.size()<<" realangphi_vec.size()="<<realangphi_vec.size()<<endl;
  Double_t maxAngZ=-1000., minAngZ=1000., maxAngPhi=-1000., minAngPhi=1000., avAngZ=0., avAngPhi=0.;
  Double_t realAngZ, realAngZRes, realAngPhi, realAngPhiRes, realAngZResAv=0.;
  for (Int_t i_h = 0; i_h < realangz_vec.size(); i_h++){
    if(realangphi_vec[i_h]>maxAngPhi)
      maxAngPhi=realangphi_vec[i_h];
    if(realangphi_vec[i_h]<minAngPhi)
      minAngPhi=realangphi_vec[i_h];
    if(realangz_vec[i_h]>maxAngZ)
      maxAngZ=realangz_vec[i_h];
    if(realangz_vec[i_h]<minAngZ)
      minAngZ=realangz_vec[i_h];
    avAngZ+=realangz_vec[i_h];
    avAngPhi+=realangphi_vec[i_h];
    }
  if(realangz_vec.size()!=0){
    realAngZ=avAngZ/realangz_vec.size();
    realAngPhi=avAngPhi/realangz_vec.size();
    ((TH1D*)gDirectory->Get("NewBM/MCeve/Real_angZ"))->Fill(realAngZ);
    ((TH1D*)gDirectory->Get("NewBM/MCeve/Real_angPhi"))->Fill(realAngPhi);
    realAngZRes=maxAngZ-minAngZ;
    realAngPhiRes=maxAngPhi-minAngPhi;
    ((TH1D*)gDirectory->Get("NewBM/MCeve/Real_angZRes"))->Fill(realAngZRes);
    ((TH1D*)gDirectory->Get("NewBM/MCeve/Real_angPhiRes"))->Fill(realAngPhiRes);
    for (Int_t i_hh = 0; i_hh < realangz_vec.size(); i_hh++)
      realAngZResAv+=fabs(realangz_vec[i_hh]-realAngZ);
    realAngZResAv=realAngZResAv/realangz_vec.size();
    ((TH1D*)gDirectory->Get("NewBM/MCeve/Real_angZResAv"))->Fill(realAngZResAv);
    ((TH2D*)gDirectory->Get("NewBM/MCeve/Real_AngzvsAngzRes"))->Fill(realAngZ, realAngZRes);
    }


  if(realangz_vec.size()!=realpos_vec.size())
    cout<<"WARNING: something is very wrong in realangz and realpos!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!check in RecoTools::MonitorBMNew()"<<endl;

  //calculate realRTarget // INUTILE, DA TOGLIERE!!! con tutti i suoi grafici
  Double_t minRTarget=10, maxRTarget=-10, RealRTarget=0., RealRTargetNew=0.;
  for (Int_t i_h = 0; i_h < realpos_vec.size(); i_h++){
    //~ tmp_double=sqrt(realpos_vec[i_h].X()*realpos_vec[i_h].X()+realpos_vec[i_h].Y()*realpos_vec[i_h].Y());
    //~ tmp_double+=tan(realangz_vec[i_h]*DEG2RAD)*(target_o.Z()-realpos_vec[i_h].Z());
    //~ RealRTargetNew+=tmp_double+tan(realangz_vec[i_h]*DEG2RAD)*(target_o.Z()-realpos_vec[i_h].Z());
    RealRTargetNew+=tan(realangz_vec[i_h]*DEG2RAD)*(target_o.Z()-realpos_vec[i_h].Z());
    tmp_double=tan(realAngZ*DEG2RAD)*(target_o.Z()-realpos_vec[i_h].Z());
    if(tmp_double<minRTarget)
      minRTarget=tmp_double;
    if(tmp_double>maxRTarget)
      maxRTarget=tmp_double;
    RealRTarget+=tmp_double;
    }
  if(realpos_vec.size()>0){
    RealRTarget=RealRTarget/realpos_vec.size();
    RealRTargetNew=RealRTargetNew/realpos_vec.size();
    ((TH1D*)gDirectory->Get("NewBM/MCeve/Real_RTarget"))->Fill(RealRTarget);
    ((TH2D*)gDirectory->Get("NewBM/MCeve/Real_AngzvsRTarget"))->Fill(realAngZ, RealRTarget);
    ((TH1D*)gDirectory->Get("NewBM/MCeve/Real_RTargetNew"))->Fill(RealRTargetNew);
    ((TH2D*)gDirectory->Get("NewBM/MCeve/Real_AngzvsRTargetNew"))->Fill(realAngZ, RealRTargetNew);
    if(realpos_vec.size()>1)
      ((TH1D*)gDirectory->Get("NewBM/MCeve/Real_RTargetRes"))->Fill(maxRTarget-minRTarget);
    }



  //calculate RealRTargetFirst
  if(realpos_vec.size()>0){
    //~ Double_t RealRTargetFirst=sqrt(realpos_vec[0].X()*realpos_vec[0].X()+realpos_vec[0].Y()*realpos_vec[0].Y())+
    Double_t RealRTargetFirst=tan(realAngZ*DEG2RAD)*(target_o.Z()-realpos_vec[0].Z());
    ((TH1D*)gDirectory->Get("NewBM/MCeve/Real_RTargetFirst"))->Fill(RealRTargetFirst);
    ((TH1D*)gDirectory->Get("NewBM/MCeve/Real_AngzvsRTarget_pendenza"))->Fill(RealRTargetFirst/realAngZ);
    ((TH2D*)gDirectory->Get("NewBM/MCeve/Real_AngzvsRTargetFirst"))->Fill(realAngZ, RealRTargetFirst);
    }


  //_sel sono gli hit selezionati, quelli che derivano dalla traccia, senza sel sono tutti gli hit letti dal BM
  nhit_pri_all=nhit_pri_all_x+nhit_pri_all_y;
  nhit_pri_sel=nhit_pri_sel_x+nhit_pri_sel_y;
  if(nhit_pri_all_x>2 && nhit_pri_all_y>2 && realAngZ<p_bmcon->GetAngZCut())//goodEvent is true if there is at least 3 hit from the primary particle for any view and angz<angzcut.. no buono secondo me!!!
    goodEvent=true;

  //NO LOOP ON TRACK: dovrebbe esserci solo una traccia o nessuna traccia
  if(i_ntrk!=1 && i_ntrk!=0)
    cout<<"WARNING: you have a wrong number of tracks in BM!!!!!!!!!!!!!!!! message from RecoTools::MonitorBMNew()"<<endl;

  if(i_ntrk==1) {// dovrebbe esserci una sola traccia salvata
    trackTr = p_ntutrk->Track(0);
    if(nhit_sel!=trackTr->GetNhit())//se non vedi mai questa scritta togli nhit_sel
      cout<<"WARNING: Nhit from track of TABMntuTrackTr is different from nhit_sel of Recotools!!!check in RecoTools::MonitorBMNew() nhit_sel="<<nhit_sel<<"  trackTr->GetNhit()="<<trackTr->GetNhit()<<endl;

    ((TH1D*)gDirectory->Get("NewBM/isConverged"))->Fill(trackTr->GetIsConverged());



    if(trackTr->GetIsConverged()==1){
    //provv cutpass_conv era 0.01 e anche gli altri end erano diversi!
      Double_t cutpass_conv=0.05, chi2cut_end_conv=50., angzcut_end_conv=20., rescut_end_conv=5.;
      for(Double_t angzcut_conv=0.00001 ; angzcut_conv<angzcut_end_conv ; angzcut_conv+=cutpass_conv)
          if(trackTr->GetAngZ()<angzcut_conv)
            ((TH1D*)gDirectory->Get("NewBM/converged/AngZcut_conv"))->Fill(angzcut_conv);
      for(Double_t rescut_conv=0.000001 ; rescut_conv<rescut_end_conv ; rescut_conv+=cutpass_conv)
          if(trackTr->GetAngZRes()<rescut_conv){
            ((TH1D*)gDirectory->Get("NewBM/converged/AngZRescut_conv"))->Fill(rescut_conv);
            ((TH1D*)gDirectory->Get("NewBM/converged/AngZRescut_conv_large"))->Fill(rescut_conv);
            }
      for(Double_t chi2cut_conv=0.00001 ; chi2cut_conv<chi2cut_end_conv ; chi2cut_conv+=cutpass_conv)
          if(trackTr->GetChi2NewRed()<chi2cut_conv)
            ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut_conv"))->Fill(chi2cut_conv);

  //~ if(trackTr->GetIsConverged()==1 && trackTr->GetAngZRes()<0.45 && trackTr->GetAngZRes()>0.3){//provvisorio!
  //~ if(trackTr->GetIsConverged()==1 && trackTr->GetChi2NewRed()<1.){//provvisorio!
    if(trackTr->GetIsConverged()==1){// è inutile, ma lo lascio perchè poi è più semplice mettere le condizioni sopra senza dover togliere e mettere la }
      ((TH1D*)gDirectory->Get("NewBM/converged/nhitSelected_conv"))->Fill(trackTr->GetNhit());
      ((TH1D*)gDirectory->Get("NewBM/converged/nhitRead_conv"))->Fill(i_nhit);
      ((TH1D*)gDirectory->Get("NewBM/converged/ndf_conv"))->Fill(trackTr->GetNdf());
      ((TH1D*)gDirectory->Get("NewBM/converged/failedPoint_conv"))->Fill(trackTr->GetFailedPoint());
      ((TH1D*)gDirectory->Get("NewBM/converged/chi2red"))->Fill(trackTr->GetChi2NewRed());
      ((TH1D*)gDirectory->Get("NewBM/converged/chi2red_large"))->Fill(trackTr->GetChi2NewRed());
      ((TH1D*)gDirectory->Get("NewBM/converged/mychi2red"))->Fill(trackTr->GetMyChi2Red());
      ((TH1D*)gDirectory->Get("NewBM/converged/rtarget_conv"))->Fill(trackTr->GetRTarget());
      ((TH1D*)gDirectory->Get("NewBM/converged/AngZ_conv"))->Fill(trackTr->GetAngZ());
      ((TH1D*)gDirectory->Get("NewBM/converged/AngZ_conv_large"))->Fill(trackTr->GetAngZ());
      ((TH1D*)gDirectory->Get("NewBM/converged/AngZRes_conv"))->Fill(trackTr->GetAngZRes());
      ((TH1D*)gDirectory->Get("NewBM/converged/AngZRes_conv_large"))->Fill(trackTr->GetAngZRes());
      ((TH1D*)gDirectory->Get("NewBM/converged/AngZResAv_conv"))->Fill(trackTr->GetAngZResAv());
      ((TH1D*)gDirectory->Get("NewBM/converged/AngPhi_conv"))->Fill(trackTr->GetAngPhi());
      ((TH1D*)gDirectory->Get("NewBM/converged/AngPhiRes_conv"))->Fill(trackTr->GetAngPhiRes());
      ((TH1D*)gDirectory->Get("NewBM/converged/RealAngZ_vs_FitAngZ_conv"))->Fill(fabs(trackTr->GetAngZ()-realAngZ));
      ((TH1D*)gDirectory->Get("NewBM/converged/AngzvsRTarget_pendenza_conv"))->Fill(trackTr->GetRTarget()/trackTr->GetAngZ());
      ((TH1D*)gDirectory->Get("NewBM/converged/RdriftSmear_conv"))->Fill(maxRdriftSmear);
      ((TH2D*)gDirectory->Get("NewBM/converged/AngzvsRTarget_conv"))->Fill(trackTr->GetAngZ(), trackTr->GetRTarget());
      ((TH2D*)gDirectory->Get("NewBM/converged/AngzvsAngzres_conv"))->Fill(trackTr->GetAngZ(), trackTr->GetAngZRes());
      ((TH2D*)gDirectory->Get("NewBM/converged/AngzvsAngzres_conv_large"))->Fill(trackTr->GetAngZ(), trackTr->GetAngZRes());
      ((TH2D*)gDirectory->Get("NewBM/converged/chi2redVsAngz_conv"))->Fill(trackTr->GetChi2NewRed(),trackTr->GetAngZ());
      ((TH2D*)gDirectory->Get("NewBM/converged/chi2redVsAngz_conv_large"))->Fill(trackTr->GetChi2NewRed(),trackTr->GetAngZ());
      ((TH2D*)gDirectory->Get("NewBM/converged/chi2redVsAngzRes_conv"))->Fill(trackTr->GetChi2NewRed(), trackTr->GetAngZRes());
      ((TH2D*)gDirectory->Get("NewBM/converged/chi2redVsAngzRes_conv_large"))->Fill(trackTr->GetChi2NewRed(), trackTr->GetAngZRes());
      ((TH2D*)gDirectory->Get("NewBM/converged/avReso_vs_chi2red_conv"))->Fill(avReso, trackTr->GetChi2NewRed());
      ((TH2D*)gDirectory->Get("NewBM/converged/avReso_vs_angzres_conv"))->Fill(avReso, trackTr->GetAngZRes());
      ((TH1D*)gDirectory->Get("NewBM/converged/n_prihit_sel"))->Fill(nhit_pri_sel);
      if(goodEvent)
        ((TH1D*)gDirectory->Get("NewBM/converged/n_hit_conv_good"))->Fill(nhit_pri_sel);
      else
        ((TH1D*)gDirectory->Get("NewBM/converged/n_hit_conv_bad"))->Fill(nhit_pri_sel);
      for(Int_t i=0;i<hitxplane.size();i++)
        for(Int_t j=0;j<hitxplane[i];j++)
          ((TH1D*)gDirectory->Get("NewBM/converged/planeSelected_conv"))->Fill(i);


      Double_t rescut=0.5, angzcut=0.3;//provvisorio!!! poi prendi da parcon magari
      if(trackTr->GetAngZRes() < rescut && i_nhit<=12) {//enter in rescut// da modificare, poi //provvisorio! WORK NOW
        Double_t rescut_chi2pass=0.01, rescut_angzpass=0.01, rescut_initchi2=1., rescut_endchi2=5., rescut_initangz=0.1, rescut_endangz=0.3;
        Double_t angzcut_end=0.5, chi2redcut_end=10., angzcut_pass=0.01, chi2redcut_pass=0.01;
        Double_t chi2redcut_tight=1.57, chi2redcut_loose=3;
        ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/AngZRes_conv_resloose"))->Fill(trackTr->GetAngZRes());
        ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/AngZ_conv_resloose"))->Fill(trackTr->GetAngZ());
        ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/chi2red_conv_resloose"))->Fill(trackTr->GetChi2NewRed());
        ((TH2D*)gDirectory->Get("NewBM/converged/rescut_l/chi2redVsAngz_conv_resloose"))->Fill(trackTr->GetChi2NewRed(),trackTr->GetAngZ());
        for(Double_t maxangzcut=0.00001 ; maxangzcut<angzcut_end ; maxangzcut+=angzcut_pass)
          if(trackTr->GetAngZ()<maxangzcut)
            ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/AngZcut_conv_resloose"))->Fill(maxangzcut);

        for(Double_t maxchi2cut=0.00001 ; maxchi2cut<chi2redcut_end ; maxchi2cut+=chi2redcut_pass)
          if(trackTr->GetChi2NewRed()<maxchi2cut)
            ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/chi2redcut_conv_resloose"))->Fill(maxchi2cut);

        for(Double_t maxchi2=rescut_initchi2+0.001;maxchi2<rescut_endchi2;maxchi2+=rescut_chi2pass)
          if(trackTr->GetChi2NewRed()<maxchi2)
            for(Double_t maxangz=rescut_initangz+0.001;maxangz<rescut_endangz;maxangz+=rescut_angzpass)
              if(trackTr->GetAngZ()<maxangz)
                ((TH2D*)gDirectory->Get("NewBM/converged/rescut_l/chi2redAndAngz_conv_resloose"))->Fill(maxangz, maxchi2);//da riempire

        //chi2tight
        if(trackTr->GetChi2NewRed()<chi2redcut_tight){
          ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/chi2cut_tight/AngZ_conv_resloose_chi2tight"))->Fill(trackTr->GetAngZ());
          for(Double_t maxangzcut=0.00001 ; maxangzcut<angzcut_end ; maxangzcut+=angzcut_pass)
            if(trackTr->GetAngZ()<maxangzcut)
              ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/chi2cut_tight/AngZcut_conv_resloose_chi2tight"))->Fill(maxangzcut);
          }//end of rescut && chi2tight

        //chi2loose
        if(trackTr->GetChi2NewRed()<chi2redcut_loose){
          ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/chi2cut_l/ndf_conv_resloose_chi2loose"))->Fill(trackTr->GetNdf());
          ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/chi2cut_l/n_hit_conv_resloose_chi2loose_good"))->Fill(nhit_pri_sel);
          ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/chi2cut_l/AngZ_conv_resloose_chi2loose"))->Fill(trackTr->GetAngZ());
          for(Double_t maxangzcut=0.00001 ; maxangzcut<angzcut_end ; maxangzcut+=angzcut_pass)
            if(trackTr->GetAngZ()<maxangzcut)
              ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/chi2cut_l/AngZcut_conv_resloose_chi2loose"))->Fill(maxangzcut);
          }//end of rescut && chi2tight

        //angZcut
        if(trackTr->GetAngZ()<angzcut){
          ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/angzcut/chi2red_conv_resloose_angzcut"))->Fill(trackTr->GetChi2NewRed());
          for(Double_t maxchi2cut=0.00001 ; maxchi2cut<chi2redcut_end ; maxchi2cut+=chi2redcut_pass)
            if(trackTr->GetChi2NewRed()<maxchi2cut)
              ((TH1D*)gDirectory->Get("NewBM/converged/rescut_l/angzcut/chi2redcut_conv_resloose_angzcut"))->Fill(maxchi2cut);
          }//end of rescut_l/angZcut

        }//end of rescut_l




      if(trackTr->GetChi2NewRed() < p_bmcon->GetChi2Redcut()) {
        ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/AngZ_conv_chi2redcut"))->Fill(trackTr->GetAngZ());
        ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/RealAngZ_vs_FitAngZ_conv_chi2redcut"))->Fill(fabs(trackTr->GetAngZ()-realAngZ));
        ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/AngZRes_conv_chi2redcut"))->Fill(trackTr->GetAngZRes());
        ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/rtarget_conv_chi2redcut"))->Fill(trackTr->GetRTarget());
        ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/chi2red_chi2redcut"))->Fill(trackTr->GetChi2NewRed());
        ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/mychi2red_chi2redcut"))->Fill(trackTr->GetMyChi2Red());
        ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/AngzvsRTarget_pendenza_conv_chi2redcut"))->Fill(trackTr->GetRTarget()/trackTr->GetAngZ());
        ((TH2D*)gDirectory->Get("NewBM/converged/chi2redcut/AngzvsRTarget_conv_chi2redcut"))->Fill(trackTr->GetAngZ(), trackTr->GetRTarget());
        if(goodEvent)
          ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/n_hit_conv_chi2redcut_good"))->Fill(nhit_pri_sel);
        else
          ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/n_hit_conv_chi2redcut_bad"))->Fill(nhit_pri_sel);

        if(ReadOnlyPrimary==true){
          ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/AngZ_conv_chi2redcut_onlyPrimary"))->Fill(trackTr->GetAngZ());
          ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/AngZRes_conv_chi2redcut_onlyPrimary"))->Fill(trackTr->GetAngZRes());
          }

        if(trackTr->GetAngZ()<p_bmcon->GetAngZCut()){
          Double_t maxangzres=0.5, angzres_pass=0.01, angzrescut=0.5;
          if(trackTr->GetAngZRes()<angzrescut)
            for(Double_t ar=0.000001 ; ar<maxangzres ; ar+=angzres_pass)
              if(trackTr->GetAngZRes()<ar)
                ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/chi2_angz/AngZRescut_conv_chi2redcut_angzcut"))->Fill(ar);

          ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/chi2_angz/AngZRes_conv_chi2redcut_angzcut"))->Fill(trackTr->GetAngZRes());
          ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/chi2_angz/RealAngZ_vs_FitAngZ_conv_chi2redcut_angzcut"))->Fill(fabs(trackTr->GetAngZ()-realAngZ));
          if(goodEvent)
            ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/chi2_angz/n_hit_conv_chi2redcut_angzcut_good"))->Fill(nhit_pri_sel);
          else
            ((TH1D*)gDirectory->Get("NewBM/converged/chi2redcut/chi2_angz/n_hit_conv_chi2redcut_angzcut_bad"))->Fill(nhit_pri_sel);
          //~ if(trackTr->GetAngZ()<p_bmcon->GetAngZRescut()){

            //~ }//end of chi2_angz_res as level 3

          }//end of chi2_angz as level 2
        }//end of chi2cut as level 1

    Double_t chi2end_angzcut=10., angzcut_passo=0.01, resend_angzcut=0.5;
    Double_t chi2tight_angzcut=1.6, chi2loose_angzcut=5., resloose_angzcut=0.5, restight_angzcut=0.1;
      if(trackTr->GetAngZ() < 0.3 && i_nhit<=12){//primo taglio è su angz

        if(trackTr->GetChi2NewRed() < 5) //chi2loose
          for(Double_t ar=0.000001 ; ar<resend_angzcut ; ar+=angzcut_passo)
            if(trackTr->GetAngZRes()<ar)
              ((TH1D*)gDirectory->Get("NewBM/converged/angzcut/AngZRescut_conv_angzcut_chi2loose"))->Fill(ar);

        if(trackTr->GetChi2NewRed() < 1.6) //chi2tight
          for(Double_t ar=0.000001 ; ar<resend_angzcut ; ar+=angzcut_passo)
            if(trackTr->GetAngZRes()<ar)
              ((TH1D*)gDirectory->Get("NewBM/converged/angzcut/AngZRescut_conv_angzcut_chi2tight"))->Fill(ar);

        if(trackTr->GetAngZRes()<0.5)//resloose
          for(Double_t ar=0.00001 ; ar<chi2end_angzcut ; ar+=angzcut_passo)
            if(trackTr->GetChi2NewRed()<ar)
              ((TH1D*)gDirectory->Get("NewBM/converged/angzcut/chi2redcut_conv_angzcut_resloose"))->Fill(ar);

        if(trackTr->GetAngZRes()<0.1)//restight
          for(Double_t ar=0.00001 ; ar<chi2end_angzcut ; ar+=angzcut_passo)
            if(trackTr->GetChi2NewRed()<ar)
              ((TH1D*)gDirectory->Get("NewBM/converged/angzcut/chi2redcut_conv_angzcut_restight"))->Fill(ar);


        }//fine primo taglio su angz


      }//provvisorio  su converged

    }else if(trackTr->GetIsConverged()==2){
      ((TH1D*)gDirectory->Get("NewBM/not_converged/n_hit_notconv_tot"))->Fill(nhit_pri_sel);
      ((TH1D*)gDirectory->Get("NewBM/not_converged/RdriftSmear_notconv"))->Fill(maxRdriftSmear);
      if(goodEvent){
        if(ReadOnlyPrimary)
          ((TH1D*)gDirectory->Get("NewBM/not_converged/n_hit_notconv_good_onlyPrimary"))->Fill(nhit_pri_sel);
        else
          ((TH1D*)gDirectory->Get("NewBM/not_converged/n_hit_notconv_good"))->Fill(nhit_pri_sel, nhit_sel);
        }
      else
        ((TH1D*)gDirectory->Get("NewBM/not_converged/n_hit_notconv_bad"))->Fill(nhit_sel);
      }
  }else if(i_ntrk==0){
    ((TH1D*)gDirectory->Get("NewBM/isConverged"))->Fill(0);
    ((TH1D*)gDirectory->Get("NewBM/nopossible/n_hit_nopossible_tot"))->Fill(nhit_pri_all);
    if(goodEvent)
      ((TH1D*)gDirectory->Get("NewBM/nopossible/n_hit_nopossible_good"))->Fill(nhit_pri_all);
    else
      ((TH1D*)gDirectory->Get("NewBM/nopossible/n_hit_nopossible_bad"))->Fill(nhit_pri_all);
    }


  return;
}





void RecoTools::MonitorBMVTMat() {

  // m_hf->cd();

  // TVector3 bmPos = fGeoTrafo->GetBMCenter();
  // TVector3 tgPos = fGeoTrafo->GetVTCenter();
  // double zDiff  = tgPos.Z() - bmPos.Z() - 0.7;
  // double my_ux, my_uy, my_uz, my_uzs, phi_an, psi_an;
  // //Tracks
  // TABMntuTrack* p_ntutrk  =
  //   (TABMntuTrack*) myn_bmtrk->GenerateObject();

  // TAVTntuTrack* p_vttrk =
  //   (TAVTntuTrack*) myn_vtrk->GenerateObject();


  // vector<TVector3> myOrigs;
  // for(int i= 0; i<p_vttrk->GetTracksN(); i++) {
  //   TAVTtrack* t_track = p_vttrk->GetTrack(i);
  //   TVector3 t_origin = t_track->GetTrackLine().GetOrigin();
  //   //Origin in cm
  //   t_origin *= 1./10000;
  //   myOrigs.push_back(t_origin);
  // }
  // double distance;
  // for(int ia =0; ia<(int)myOrigs.size(); ia++) {
  //   for(int ib = ia+1; ib<(int)myOrigs.size(); ib++) {

  //     distance = sqrt(pow(myOrigs.at(ia).X()-myOrigs.at(ib).X(),2)+
		//       pow(myOrigs.at(ia).Y()-myOrigs.at(ib).y(),2));

  //     ((TH1D*)gDirectory->Get("beammonitor/vtdist"))->Fill(distance);
  //     ((TH2D*)gDirectory->Get("beammonitor/vtdist_nt"))->Fill(distance,myOrigs.size());
  //   }
  // }

  // double phi_bm, phi_vt_1, psi_bm, psi_vt_1;
  // double phi_vt_2, psi_vt_2;

  // TAGntuMCeve* p_mceve  =
  //   (TAGntuMCeve*) myn_mceve->GenerateObject();

  // if(p_ntutrk->ntrk) {
  //   TABMntuTrackTr* bmTrack  = p_ntutrk->Track(0);
  //   double chi2 = bmTrack->GetChi2();

  //   TVector3 trackPos = bmTrack->PointAtLocalZ(zDiff);

  //   TVector3 trackCen = bmTrack->PointAtLocalZ(0);

  //   TVector3 GlbBM = fGeoTrafo->FromBMLocalToGlobal(trackPos);

  //   TVector3 GlbBMCent = fGeoTrafo->FromBMLocalToGlobal(trackCen);
  //   my_ux = bmTrack->GetUx();
  //   my_uy = bmTrack->GetUy();
  //   my_uz = 1;
  //   my_uzs = 1. - my_ux*my_ux - my_uy*my_uy;
  //   if(my_uzs>=0) my_uz = sqrt(my_uzs);

  //   phi_an = atan(my_ux/my_uz);
  //   psi_an = atan(my_uy/my_uz);

  //   //from local to global in the BM
  //   TVector3 versBM (my_ux/my_uz,my_uy/my_uz,1);
  //   //	TVector3 angGlbBM = fGeoTrafo->VecFromGlobalToBMLocal(versBM);
  //   TVector3 angGlbBM = fGeoTrafo->VecFromBMLocalToGlobal(versBM);

  //   double phi_BM_GL = angGlbBM.X()/angGlbBM.Z();
  //   double psi_BM_GL = angGlbBM.Y()/angGlbBM.Z();

  //   //requires only 1 track in the vtx
  //   vector <double> dis_vec;
  //   dis_vec.clear();
  //   double dis_1, dis_2;

 //    for(int i= 0; i<p_vttrk->GetTracksN(); i++) {
 //      TAVTtrack* track = p_vttrk->GetTrack(i);

 //      TAVTline line = track->GetTrackLine();
 //      TVector3 angVtx = line.GetSlopeZ();
 //      double phi_Vtx = angVtx.X()/angVtx.Z();
 //      double psi_Vtx = angVtx.Y()/angVtx.Z();
 //      //      cout<<"VTXref:: " << phi_Vtx<<" "<< psi_Vtx<<endl;
 //      //      cout<<"VTXref:: " << angVtx.Theta()<< " "<<angVtx.Phi()<<endl;
 //      //from local to global in the vtx
 //      //	TVector3 angGlbVtx = fGeoTrafo->VecFromGlobalToVTLocal(angVtx);
 //      TVector3 angGlbVtx = fGeoTrafo->VecFromVTLocalToGlobal(angVtx);
 //      double phi_Vtx_GL = angGlbVtx.X()/angGlbVtx.Z();
 //      double psi_Vtx_GL = angGlbVtx.Y()/angGlbVtx.Z();

 //      //      //      cout<<"GLBref:: " << phi_Vtx_GL<<" "<< psi_Vtx_GL<<endl;
 //      //      cout<<"GLBref:: " << angGlbVtx.Theta()<< " "<<angGlbVtx.Phi()<<endl;

 //      /*
	//   TVector3 origin_chk = track->Intersection(0);
	//   origin_chk *= 1./10000;
	//   TVector3 GlbOrigin_chk = fGeoTrafo->FromVTLocalToGlobal(origin_chk);
	//   cout<<"orichk:: "<<origin_chk.X()<<" "<<GlbOrigin_chk.X()<<endl;
 //      */

 //      TVector3 origin = track->GetTrackLine().GetOrigin();
 //      origin *= 1./10000;
 //      TVector3 GlbOrigin = fGeoTrafo->FromVTLocalToGlobal(origin);

 //      double rec_Angle = (track->GetTrackLine()).GetTheta()*TMath::DegToRad();

 //      TVector3 res = GlbOrigin - GlbBM;
 //      TVector3 resCen = GlbOrigin - GlbBMCent;

 //      for(int ih=0; ih<p_mceve->nhit; ih++) {
	// TAGntuMCeveHit* hit = p_mceve->Hit(ih);

	// double chgMC = hit->Chg();
	// //	maMC = hit->Mass();
	// double angMC = (hit->InitP()).Theta();

	// if(chgMC == 6) {
	//   //	  cout<<res.X()<<" "<<res.Y()<<" "<<rec_Angle<<" "<<angMC<<endl;
	//   ((TH2D*)gDirectory->Get("beammonitor/resx_vs_angRes"))->Fill(res.X(),-rec_Angle+angMC);
	//   ((TH2D*)gDirectory->Get("beammonitor/resy_vs_angRes"))->Fill(res.Y(),-rec_Angle+angMC);
	// }
 //      }


 //      if(chi2<10 && p_vttrk->GetTracksN()==1 && m_fullmoni) {
	// ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_ang_chi10_2D_x"))->Fill(phi_BM_GL,phi_Vtx_GL);
	// ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_ang_chi10_2D_y"))->Fill(psi_BM_GL,psi_Vtx_GL);
 //      }


 //      phi_bm = -0.011+0.266*phi_BM_GL;
 //      psi_bm = -0.0067+0.3938*psi_BM_GL;

 //      if(p_vttrk->GetTracksN()==2) 	{
	// if(i == 0) {
	//   phi_vt_1 = phi_Vtx_GL;
	//   psi_vt_1 = psi_Vtx_GL;
	// }
	// if(i == 1) {
	//   phi_vt_2 = phi_Vtx_GL;
	//   psi_vt_2 = psi_Vtx_GL;
	// }
 //      }
 //      if(m_fullmoni) {
	// if(chi2<2 && p_vttrk->GetTracksN()==1) {
	//   ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_ang_chi2_2D_x"))->Fill(phi_BM_GL,phi_Vtx_GL);
	//   ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_ang_chi2_2D_y"))->Fill(psi_BM_GL,psi_Vtx_GL);
	// }

	// if(chi2<2 && p_vttrk->GetTracksN()==1 && phi_BM_GL>=-0.5*pow(10.,-2) && phi_BM_GL<=0*pow(10.,-3)) {
	//   ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_ang_chi2_2D_x_cut0.5"))->Fill(phi_BM_GL,phi_Vtx_GL);
	//   ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_ang_chi2_2D_x_cut1"))->Fill(phi_BM_GL,phi_Vtx_GL);
	// }
 //      }
 //      //Residuals on the target
 //      ((TH1D*)gDirectory->Get("beammonitor/bmvt_resx"))->Fill(res.X());
 //      if(p_vttrk->GetTracksN()==1) 	{
	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resx"))->Fill(res.X());
 //      } else {
	// dis_vec.push_back(sqrt(pow(res.X(),2)+pow(res.Y(),2)));
 //      }

 //      if(p_vttrk->GetTracksN()==2) 	{
	// if(i == 0)
	//   dis_1 = sqrt(pow(res.X(),2)+pow(res.Y(),2));
	// if(i == 1)
	//   dis_2 = sqrt(pow(res.X(),2)+pow(res.Y(),2));
 //      }

 //      if(m_fullmoni) {
	// if(chi2<2) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_res2x"))->Fill(res.X());
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res2x"))->Fill(res.X());
	// }

	// if(chi2<20) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_res20x"))->Fill(res.X());
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res20x"))->Fill(res.X());
	// }

	// if(chi2<10) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_res10x"))->Fill(res.X());
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res10x"))->Fill(res.X());
	// }
 //      }
 //      if(chi2<5) {
	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_res5x"))->Fill(res.X());
	// if(p_vttrk->GetTracksN()==1) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res5x"))->Fill(res.X());
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_vtx5x"))->Fill(GlbOrigin.X());
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_bmx5x"))->Fill(GlbBM.X());
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_bmcex5x"))->Fill(GlbBMCent.X());
	// }

 //      }

 //      ((TH1D*)gDirectory->Get("beammonitor/bmvt_resy"))->Fill(res.Y());
 //      if(p_vttrk->GetTracksN()==1)
	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resy"))->Fill(res.Y());

      // if(m_fullmoni) {
	// if(chi2<20) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_res20y"))->Fill(res.Y());
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res20y"))->Fill(res.Y());
	// }
	// if(chi2<15) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_res15y"))->Fill(res.Y());
	//   if(p_vttrk->GetTracksN()==1) {
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res15y"))->Fill(res.Y());
	//   }
	// }

	// if(chi2<2) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_res2y"))->Fill(res.Y());
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res2y"))->Fill(res.Y());
	// }


	// if(chi2<10) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_res10y"))->Fill(res.Y());
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res10y"))->Fill(res.Y());
	// }
 //      }

   //    if(chi2<5) {
	  // ((TH1D*)gDirectory->Get("beammonitor/bmvt_res5y"))->Fill(res.Y());
	  // if(p_vttrk->GetTracksN()==1) {
	  //   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res5y"))->Fill(res.Y());
	  //   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_vtx5y"))->Fill(GlbOrigin.Y());
	  //   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_bmx5y"))->Fill(GlbBM.Y());
	  //   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_bmcex5y"))->Fill(GlbBMCent.Y());
	  // }
   //    }

 //      //Rediduals on X and Y with ANGLE CONDITION

 //      if(chi2<10 && phi_an>=-0.5*pow(10.,-3) && phi_an<=0.5*pow(10.,-3)) {
	// if(p_vttrk->GetTracksN()==1)
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res2x_ang"))->Fill(res.X());
	// /*	cout << "bmvt_1t_res2x_ang "<<res.X()<<" chi2 "<< chi2 << endl;*/
 //      }

 //      if(chi2<10 && psi_an>=-0.5*pow(10.,-3) && psi_an<=0.5*pow(10.,-3)) {
	// if(p_vttrk->GetTracksN()==1)
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_res2y_ang"))->Fill(res.Y());

	// /*	cout << "bmvt_1t_res2y_ang "<<res.Y()<<" chi2 "<< chi2 << endl;*/
 //      }
 //      //Rediduals on X and Y with ANGLE CONDITION - CHI2 vs X

 //      if(chi2<10 && phi_an>=-0.5*pow(10.,-3) && phi_an<=0.5*pow(10.,-3)) {
	// if(p_vttrk->GetTracksN()==1)
	//   ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_res2x_ang_2D"))->Fill(res.X(),chi2);

	// // 	    ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_x_ang_2D_vertex"))->Fill(theta_vt,phi_vt);

	// /*	cout << "bmvt_1t_res2x_ang "<<res.X()<<" chi2 "<< chi2 << endl;*/
 //      }

 //      if(chi2<10 && psi_an>=-0.5*pow(10.,-3) && psi_an<=0.5*pow(10.,-3)) {
	// if(p_vttrk->GetTracksN()==1)
	//   ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_res2y_ang_2D"))->Fill(res.Y(),chi2);

	// // 	    ((TH2D*)gDirectory->Get("beammonitor/bmvt_1t_y_ang_2D_vertex"))->Fill(theta_vt,phi_vt);

	// /*	cout << "bmvt_1t_res2y_ang "<<res.Y()<<" chi2 "<< chi2 << endl;*/
 //      }


 //      if(m_fullmoni) {
	// //Residuals on the BM center
	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCenx"))->Fill(resCen.X());
	// if(p_vttrk->GetTracksN()==1)
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCenx"))->Fill(resCen.X());

	// if(chi2<20) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCen20x"))->Fill(resCen.X());
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen20x"))->Fill(resCen.X());
	// }

	// if(chi2<10) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCen10x"))->Fill(resCen.X());
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen10x"))->Fill(resCen.X());
	// }

	// if(chi2<5) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCen5x"))->Fill(resCen.X());
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen5x"))->Fill(resCen.X());
	// }

	// if(chi2<2) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCen2x"))->Fill(resCen.X());
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen2x"))->Fill(resCen.X());
	// }

	// //Residuals on the BM center on Y

	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCeny"))->Fill(resCen.Y());
	// if(p_vttrk->GetTracksN()==1)
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCeny"))->Fill(resCen.Y());

	// if(chi2<20) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCen20y"))->Fill(resCen.Y());
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen20y"))->Fill(resCen.Y());
	// }

	// if(chi2<2) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_resCen2y"))->Fill(resCen.Y());
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen2y"))->Fill(resCen.Y());
	// }

	// //Residuals on the BM center with Angle Condition on X and Y

	// if(chi2<10 && phi_an>=-0.5*pow(10.,-3) && phi_an<=0.5*pow(10.,-3)) {
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen2x_ang"))->Fill(resCen.X());
	//   /*cout << "bmvt_1t_resCen2x_ang "<<resCen.X()<<endl;*/
	// }
	// if(chi2<10 && psi_an>=-0.5*pow(10.,-3) && psi_an<=0.5*pow(10.,-3)) {
	//   if(p_vttrk->GetTracksN()==1)
	//     ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_resCen2y_ang"))->Fill(resCen.Y());
	//   /*cout << "bmvt_1t_resCen2y_ang "<<resCen.Y()<<endl;*/
	// }
 //      }

 //      if(chi2<10 && phi_an>=-0.5*pow(10.,-3) && phi_an<=0.5*pow(10.,-3)) {
	// if(p_vttrk->GetTracksN()==1) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_origin2x_ang"))->Fill(origin.X());
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_orvtx2x_ang"))->Fill(GlbOrigin.X());
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_orbmx2x_ang"))->Fill(GlbBM.X());
	// }
	// /*cout << "bmvt_1t_origin2x_ang "<<origin.X()<<endl;*/
 //      }

 //      if(chi2<10 && psi_an>=-0.5*pow(10.,-3) && psi_an<=0.5*pow(10.,-3)) {
	// if(p_vttrk->GetTracksN()==1) {
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_origin2y_ang"))->Fill(origin.Y());
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_orvtx2y_ang"))->Fill(GlbOrigin.Y());
	//   ((TH1D*)gDirectory->Get("beammonitor/bmvt_1t_orbmx2y_ang"))->Fill(GlbBM.Y());
	// }
	// /*cout << "bmvt_1t_origin2y_ang "<<origin.Y()<<endl;*/
 //      }

    // }//Loop on VTX tracks

 //    if(p_vttrk->GetTracksN()==2) {
 //      //      cout<<" "<<dis_1<<" "<<dis_2<<" "<<phi_bm<<" "<<psi_bm<<" "<<phi_vt_1<<" "<<psi_vt_1<<" "<<phi_vt_2<<" "<<psi_vt_2<<" "<<endl;
 //      if(dis_1<dis_2) {
	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_phigoo"))->Fill(phi_bm-phi_vt_1);
	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_psigoo"))->Fill(psi_bm-psi_vt_1);
	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_phibad"))->Fill(phi_bm-phi_vt_2);
	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_psibad"))->Fill(psi_bm-psi_vt_2);
 //      } else {
	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_phigoo"))->Fill(phi_bm-phi_vt_2);
	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_psigoo"))->Fill(psi_bm-psi_vt_2);
	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_phibad"))->Fill(phi_bm-phi_vt_1);
	// ((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_psibad"))->Fill(psi_bm-psi_vt_1);
 //      }
 //    }

    // if(dis_vec.size()) {
    //   //Sorting distances
    //   sort (dis_vec.begin(), dis_vec.end());
    //   ((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_resmin"))->Fill(dis_vec.at(0));
    //   ((TH1D*)gDirectory->Get("beammonitor/bmvt_2t_resnex"))->Fill(dis_vec.at(1));
    // }

  // }//There's a BM track

}


void RecoTools::CalibBMVT() {

 //  char name[200];

 //  //Tracks
 //  TABMntuTrack* p_ntutrk  =
 //    (TABMntuTrack*) myn_bmtrk->GenerateObject();

 //  //VTX tracks
 //  // TAVTntuTrack* p_vttrk =
 //  //   (TAVTntuTrack*) myn_vtrk->GenerateObject();

 //  //BM local geo
 //  TABMparGeo* p_bmgeo =
 //    (TABMparGeo*)(gTAGroot->FindParaDsc("p_bmgeo", "TABMparGeo")->Object());

 //  //Hits
 //  TABMntuRaw* p_ntuhit  =
 //    (TABMntuRaw*) myn_bmraw->GenerateObject();

 //  //requires only 1 track in the vtx
 //  double sDistanceFromTrack, DistanceFromTrack, Residuals;
 //  if(p_vttrk->GetTracksN() == 1) {

 //    TAVTtrack* track = p_vttrk->GetTrack(0);

 //    TAVTline line = track->GetTrackLine();

 //    //First of all loops on the hits
 //    Int_t i_nnhit = p_ntuhit->nhit;
 //    for (Int_t i = 0; i < i_nnhit; i++) {
 //      const TABMntuHit* aHi = p_ntuhit->Hit(i);

 //      int tmpv = aHi->View();
 //      if(tmpv<0)tmpv = 0;
 //      //      int chidx = aHi->Cell()+aHi->Plane()*3+tmpv*18;

 //      int idwire = p_bmgeo->GetID(aHi->Cell());

 //      //U view, along x, SIDE
 //      TVector3 bm_ChkPoi(p_bmgeo->GetX(idwire,aHi->Plane(),tmpv),
	// 		 p_bmgeo->GetY(idwire,aHi->Plane(),tmpv),
	// 		 p_bmgeo->GetZ(idwire,aHi->Plane(),tmpv));


 //      TVector3 fir_CenPoi = fGeoTrafo->GetBMCenter();

 //      TVector3 fir_ChkPoi = fGeoTrafo->FromBMLocalToGlobal(bm_ChkPoi);
 //      //      TVector3 fir_CenPoi = fGeoTrafo->FromBMLocalToGlobal(bm_CenPoi);

 //      TVector3 vt_ChkPoi = fGeoTrafo->FromGlobalToVTLocal(fir_ChkPoi);
 //      TVector3 vt_CenPoi = fGeoTrafo->FromGlobalToVTLocal(fir_CenPoi);

 //      //cm --> mum
 //      TVector3 vt_ExtPoi = track->Intersection(vt_ChkPoi.Z()*10000);
 //      //mum --> cm
 //      vt_ExtPoi.SetX(vt_ExtPoi.X()/10000);
 //      vt_ExtPoi.SetY(vt_ExtPoi.Y()/10000);
 //      vt_ExtPoi.SetZ(vt_ExtPoi.Z()/10000);

 //      TVector3 fir_ExtPoi = fGeoTrafo->FromVTLocalToGlobal(vt_ExtPoi);

 //      //cm --> mum
 //      TVector3 vt_ExtCen = track->Intersection(vt_CenPoi.Z()*10000);
 //      //mum --> cm
 //      vt_ExtCen.SetX(vt_ExtCen.X()/10000);
 //      vt_ExtCen.SetY(vt_ExtCen.Y()/10000);
 //      vt_ExtCen.SetZ(vt_ExtCen.Z()/10000);

 //      if(m_debug)
	// cout<<"Z BM:: "<< p_bmgeo->GetZ(idwire,aHi->Plane(),tmpv)<<
	//   " Z VT::"<<vt_ExtPoi.Z()<<endl;

 //      if(m_debug)
	// cout<<"X,Y extr from Vtx:: "<<vt_ExtPoi.X()<<" "<<vt_ExtPoi.Y()<<" "<<vt_ExtPoi.Z()<<endl;
 //      if(i == 0 && m_debug)
	// cout<<"X,Y extr from Vtx LOC:: "<<vt_ExtCen.X()<<" "<<vt_ExtCen.Y()<<" "<<vt_ExtCen.Z()<<endl;

 //      TVector3 fir_ExtCen = fGeoTrafo->FromVTLocalToGlobal(vt_ExtCen);

 //      if(i==0 && m_debug)
	// cout<<"X,Y extr from Vtx GLB:: "<<fir_ExtCen.X()<<" "<<fir_ExtCen.Y()<<" "<<fir_ExtCen.Z()<<endl;
 //      //      cout<<"X,Y xchk from Vtx:: "<<fir_ExtXck.X()<<" "<<fir_ExtXck.Y()<<" "<<fir_ExtXck.Z()<<endl;

 //      TVector3 fir_ExtXck = fGeoTrafo->FromVTLocalToGlobal(TVector3(-10,0,0));

 //      TVector3 bm_ExtPoi = fGeoTrafo->FromGlobalToBMLocal(fir_ExtPoi);

 //      double x_tr = bm_ExtPoi.X();
 //      double y_tr = bm_ExtPoi.Y();
 //      double x_wi = p_bmgeo->GetX(idwire,aHi->Plane(),tmpv);
 //      double y_wi = p_bmgeo->GetY(idwire,aHi->Plane(),tmpv);

 //      if(i == 0) {
	// sprintf(name,"beammonitor/VTproj_BeamSN_BMcen_X");
	// ((TH1D*)gDirectory->Get(name))->Fill(fir_ExtCen.X());
	// sprintf(name,"beammonitor/VTproj_BeamSN_BMcen_Y");
	// ((TH1D*)gDirectory->Get(name))->Fill(fir_ExtCen.Y());
 //      }


 //      if(!tmpv) {
	// //	wires along x, only Y matters.
	// DistanceFromTrack = fabs(y_wi - y_tr);
	// sDistanceFromTrack = y_wi - y_tr;
	// if(m_debug) cout<<"Cell: "<<idwire<<" Plane: "<<aHi->Plane()<<" View: "<<aHi->View()<<" "<<y_tr<<" "<<y_wi<<" Dis:: "<<DistanceFromTrack<<" sDis:: "<<sDistanceFromTrack<<" "<<aHi->Dist()<<endl;
 //      } else {
	// //	wires along y, only X matters.
	// DistanceFromTrack = fabs(x_wi - x_tr);
	// sDistanceFromTrack = x_wi - x_tr;
	// if(m_debug) cout<<"Cell: "<<idwire<<" Plane: "<<aHi->Plane()<<" View: "<<aHi->View()<<" "<<x_tr<<" "<<x_wi<<" Dis:: "<<DistanceFromTrack<<" sDis:: "<<sDistanceFromTrack<<" "<<aHi->Dist()<<endl;
 //      }

 //      if(m_fullmoni) {
	// Residuals = DistanceFromTrack - aHi->Dist();
	// sprintf(name,"beammonitor/VTresi_aft_%d_%d_%d",aHi->Cell(),aHi->Plane(),tmpv);
	// ((TH1D*)gDirectory->Get(name))->Fill(Residuals);
	// sprintf(name,"beammonitor/VTresi_vs_t_%d_%d_%d",aHi->Cell(),aHi->Plane(),tmpv);
	// ((TH2D*)gDirectory->Get(name))->Fill(Residuals,aHi->Tdrift());
	// sprintf(name,"beammonitor/VTdist_vs_t_%d_%d_%d",aHi->Cell(),aHi->Plane(),tmpv);
	// ((TH2D*)gDirectory->Get(name))->Fill(DistanceFromTrack,aHi->Tdrift());
	// sprintf(name,"beammonitor/VTsdist_vs_t_%d_%d_%d",aHi->Cell(),aHi->Plane(),tmpv);
	// ((TH2D*)gDirectory->Get(name))->Fill(sDistanceFromTrack,aHi->Tdrift());
	// sprintf(name,"beammonitor/VTresi_vs_zmt_%d_%d_%d",aHi->Cell(),aHi->Plane(),tmpv);
	// ((TH2D*)gDirectory->Get(name))->Fill(Residuals,aHi->Tdrift());
	// sprintf(name,"beammonitor/VTresi_vs_r_%d_%d_%d",aHi->Cell(),aHi->Plane(),tmpv);
	// ((TH2D*)gDirectory->Get(name))->Fill(Residuals,aHi->Dist());
 //      }
 //    }


 //  }

 //  return;
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



  //NEW BEAM MONITOR
  gDirectory->mkdir("NewBM");
  gDirectory->cd("NewBM");


  h = new TH1D("isConverged","0=no possible track, 1=converged, 2=not converged",3,0.,3.);
  h = new TH1D("hitIndex","cell index fired;cell index",37,0.,37.);

  gDirectory->mkdir("converged");
  gDirectory->cd("converged");
  h = new TH1D("nhitSelected_conv","n_hit x event selected for tracking;nhit selected",37,0.,37.);
  h = new TH1D("nhitRead_conv","n_hit x event read by BM;nhit tot",37,0.,37.);
  h = new TH1D("planeSelected_conv","number of hit x plane x event;plane;nhit x plane",7,0.,7.);
  h = new TH1D("ndf_conv","ndf x converged track",120,0.,12.);
  h = new TH1D("failedPoint_conv","number of failed point x converged track",12,0.,12.);
  h = new TH1D("rtarget_conv","rtarget x converged tracks",300,0.,1.5);
  h = new TH1D("mychi2red","mychi2red",200,0.,10.);
  h = new TH1D("chi2red","chi2red",200,0.,10.);
  h = new TH1D("chi2red_large","chi2red",1000,0.,100.);
  h = new TH1D("AngZ_conv","AngZ_conv;AngZ[deg]",100,0.,1.);
  h = new TH1D("AngZ_conv_large","AngZ_conv;AngZ[deg]",450,0.,45.);
  h = new TH1D("AngZRes_conv","AngZRes_conv;AngZRes[deg]",100,0.,1.);
  h = new TH1D("AngZRes_conv_large","AngZRes_conv;AngZRes[deg]",100,0.,5.);
  h = new TH1D("AngZResAv_conv","AngZResAv_conv;AngZResAv[deg]",100,0.,1.);
  h = new TH1D("AngPhi_conv","AngPhi_conv; AngPhi[deg]",720,0.,360.);
  h = new TH1D("AngPhiRes_conv","AngPhiRes_conv;AngPhiRes[deg]",720,0.,360.);
  h = new TH1D("RealAngZ_vs_FitAngZ_conv","real angZ vs fitted angZ differences for converged tracks",100,0.,2.);
  h = new TH1D("AngzvsRTarget_pendenza_conv","RTarget/AngZ",100,0.,2.);
  h = new TH1D("RdriftSmear_conv","Max rdrift smearing",400,0.,2.);
  h = new TH1D("Rdrift_real_conv","not smeared rdrift",400,0.,2.);
  h = new TH1D("Rdrift_smeared_conv","smeared rdrift (input for genfit)",400,0.,2.);
  h = new TH1D("smearingRdrift_conv","smearing;cm",400,0.,0.3);
  h = new TH1D("sigma_conv","sigma on rdrift;cm",300,0.,0.15);
  //~ h = new TH1D("Rdrift_fitted_conv","fitted rdrift (from genfit)",400,0.,2.);// dovrei implementare in tabmntutrack metodo lungo per salvarlo etc.
  //~ h2 = new TH2D("avReso_vs_chi2red_conv","avReso_vs_chi2red converged;avReso;chi2red",120,0.,0.12,200,0.,10.);
  h = new TH1D("hitIndex_conv","cell index fired converged;cell index",37,0.,37.);
  h2 = new TH2D("avReso_vs_chi2red_conv","avReso_vs_chi2red converged;avReso;chi2red",13,0.,13,200,0.,10.);
  h2 = new TH2D("avReso_vs_angzres_conv","avReso_vs_angzres converged;avReso;angzres",13,0.,13,200,0.,1.);
  h2 = new TH2D("AngzvsRTarget_conv","angZ vs RTarget converged;AngZ [deg];RTarget[cm]",100,0.,2.,300.,0.,1.5);
  h2 = new TH2D("AngzvsAngzres_conv","angZ vs angZres converged;AngZ [deg];AngZres[deg]",100,0.,1.,70.,0.,7.);
  h2 = new TH2D("AngzvsAngzres_conv_large","angZ vs angZres converged;AngZ [deg];AngZres[deg]",100,0.,10.,100.,0.,1.);
  h2 = new TH2D("chi2redVsAngz_conv","chi2red vs angZ converged;chi2red;AngZ [deg]",70,0.,7.,100.,0.,1.);
  h2 = new TH2D("chi2redVsAngz_conv_large","chi2red vs angZ converged;chi2red;AngZ [deg]",100,0.,10.,100.,0.,10.);
  h2 = new TH2D("chi2redVsAngzRes_conv","chi2red vs angZRes converged;chi2red;AngZRes [deg]",70,0.,7.,70.,0.,0.7);
  h2 = new TH2D("chi2redVsAngzRes_conv_large","chi2red vs angZRes converged;chi2red;AngZRes [deg]",100,0.,10.,100.,0.,1.);
  h = new TH1D("n_hit_conv_good","n_primary hit selected for good event converged ;primary hit selected",13,0.,13.);
  h = new TH1D("n_hit_conv_bad","n_primary hit selected for bad event converged;primary hit selected",13,0.,13.);
  h = new TH1D("n_prihit_sel","n_primary hit selected converged event;primary hit selected",13,0.,13.);
  h = new TH1D("chi2redcut_conv","converged event passing chi2redcut;chi2redcut;N events",5000,0.,50.);
  h = new TH1D("AngZcut_conv","converged events passing angzcut;AngZcut[deg];N events",2000,0.,20.);
  h = new TH1D("AngZRescut_conv","converged event passing rescut;AngZRescut [deg]; events",300,0.,1.);
  h = new TH1D("AngZRescut_conv_large","converged event passing rescut;AngZRescut [deg]; events",100,0.,5.);


  gDirectory->mkdir("rescut_l");
  gDirectory->cd("rescut_l");
  h = new TH1D("AngZRes_conv_resloose","AngZRes converged && resloose;AngZRes[deg]",100,0.,1.);
  h = new TH1D("AngZ_conv_resloose","AngZ converged && resloose;AngZ[deg]",100,0.,1.);
  h = new TH1D("AngZcut_conv_resloose","event passing angzcut converged && resloose;AngZcut[deg]; N events",50,0.,0.5);
  h = new TH1D("chi2red_conv_resloose","chi2red converged && resloose",200,0.,10.);
  h = new TH1D("chi2redcut_conv_resloose","event passing chi2redcut converged && resloose;chi2redcut;N events",1000,0.,10.);
  h2 = new TH2D("chi2redVsAngz_conv_resloose","chi2red vs angZ converged && pass resloose;chi2red ;AngZ[deg]",200,0.,5.,100.,0.,1.);
  h2 = new TH2D("chi2redAndAngz_conv_resloose","Event passing chi2redcut and angZcut converged && resloose;AngZcut[deg] ;chi2redcut",20,0.1,0.3,400.,1.,5.);

  gDirectory->mkdir("chi2cut_tight");
  gDirectory->cd("chi2cut_tight");
  h = new TH1D("AngZcut_conv_resloose_chi2tight","events passing res_loose && chi2_tight && angzcut;AngZcut[deg];N events",30,0.,0.3);
  h = new TH1D("AngZ_conv_resloose_chi2tight","angz for event passing res_loose && chi2_tight;AngZ[deg]",100,0.,1.);
  h2 = new TH2D("AngZeff_conv_resloose_chi2tight","efficiency vs angZcut converged && rescut && chi2tightcut;AngZcut[deg] ;efficiency;",30,0.,0.3,100.,0.,1.);
  gDirectory->cd("..");//exit rescut/chi2cut_tight
  gDirectory->mkdir("chi2cut_l");
  gDirectory->cd("chi2cut_l");
  h = new TH1D("ndf_conv_resloose_chi2loose","ndf x converged track",120,0.,12.);
  h = new TH1D("n_hit_conv_resloose_chi2loose_good","n_primary hit selected for good event converged ;primary hit selected",13,0.,13.);
  h = new TH1D("AngZcut_conv_resloose_chi2loose","events passing res_loose && chi2_tight && angzcut;AngZcut[deg];N events",30,0.,0.3);
  h = new TH1D("AngZ_conv_resloose_chi2loose","angz for event passing res_loose && chi2_tight;AngZ[deg]",100,0.,1.);
  gDirectory->cd("..");//exit rescut_l/chi2cut_tight

  gDirectory->mkdir("angzcut");
  gDirectory->cd("angzcut");
  h = new TH1D("chi2red_conv_resloose_angzcut","chi2red converged && resloose && angzcut; chi2red",200,0.,10.);
  h = new TH1D("chi2redcut_conv_resloose_angzcut","event passing chi2redcut converged && resloose && angzcut;chi2redcut;N events",1000,0.,10.);
  gDirectory->cd("..");//exit rescut_l/angzcut

  gDirectory->cd("..");//exit rescut_l

  gDirectory->mkdir("chi2redcut");
  gDirectory->cd("chi2redcut");
  h = new TH1D("mychi2red_chi2redcut","mychi2red < chi2red_cut",100,0.,10.);
  h = new TH1D("chi2red_chi2redcut","chi2red < chi2red_cut",100,0.,10.);
  h = new TH1D("rtarget_conv_chi2redcut","rtarget converged and chi2red < chi2red_cut",300,0.,1.5);
  h = new TH1D("AngZ_conv_chi2redcut","AngZ converged and mychi2red<chi2redcut;AngZ [deg]",100,0.,2.);
  h = new TH1D("AngZRes_conv_chi2redcut","AngZRes converged and mychi2red<chi2redcut;AngZRes [deg]",100,0.,2.);
  h = new TH1D("AngZ_conv_chi2redcut_onlyPrimary","AngZ converged and mychi2red<chi2redcut and track is onlyPrimary;AngZ [deg]",100,0.,2.);
  h = new TH1D("AngZRes_conv_chi2redcut_onlyPrimary","AngZRes converged and mychi2red<chi2redcut and track is onlyPrimary;AngZRes [deg]",100,0.,2.);
  h = new TH1D("RealAngZ_vs_FitAngZ_conv_chi2redcut","real angZ vs fitted angZ differences for converged and mychi2<chi2redcut",100,0.,2.);
  h = new TH1D("AngzvsRTarget_pendenza_conv_chi2redcut","RTarget/AngZ",100,0.,2.);
  h2 = new TH2D("AngzvsRTarget_conv_chi2redcut","angZ vs RTarget converged && chi2redcut;AngZ [deg];RTarget[cm]",400,0.,2.,300,0.,1.5);
  h = new TH1D("n_hit_conv_chi2redcut_good","n_primary hit for good event passing chi2cut (tracking is right);primary hit selected",13,0.,13.);
  h = new TH1D("n_hit_conv_chi2redcut_bad","n_primary hit for bad event passing chi2cut (tracking fails);primary hit selected",13,0.,13.);

  gDirectory->mkdir("chi2_angz");
  gDirectory->cd("chi2_angz");
  h = new TH1D("AngZRescut_conv_chi2redcut_angzcut","event passing chi2redcut && angzcut && rescut;AngZRescut [deg]; events",50,0.,0.5);
  h = new TH1D("AngZRes_conv_chi2redcut_angzcut","AngZ converged with chi2redcut and angzcut;AngZRes [deg]",100,0.,2.);
  h = new TH1D("RealAngZ_vs_FitAngZ_conv_chi2redcut_angzcut","real angZ vs fitted angZ differences for converged && chi2redcut && angzcut",100,0.,2.);
  h = new TH1D("n_hit_conv_chi2redcut_angzcut_good","n_primary hit for good event passing chi2cut && angzcut(tracking is right);primary hit selected",13,0.,13.);
  h = new TH1D("n_hit_conv_chi2redcut_angzcut_bad","n_primary hit for bad event passing chi2cut && angzcut (tracking fails);primary hit selected",13,0.,13.);
  //~ gDirectory->mkdir("chi2_angz_res");
  //~ gDirectory->cd("chi2_angz_res");


  //~ gDirectory->cd("..");//exit chi2_angz_res

  gDirectory->cd("..");//exit chi2_angz
  gDirectory->cd("..");//exit chi2cut

  gDirectory->mkdir("angzcut");//primo loop su angz
  gDirectory->cd("angzcut");
  h = new TH1D("chi2redcut_conv_angzcut_resloose","event passing chi2redcut converged && resloose && angzcut;chi2redcut;N events",1000,0.,10.);
  h = new TH1D("chi2redcut_conv_angzcut_restight","event passing chi2redcut converged && resloose && angzcut;chi2redcut;N events",1000,0.,10.);

  h = new TH1D("AngZRescut_conv_angzcut_chi2loose","event passing chi2redcut && angzcut && rescut;AngZRescut [deg]; events",50,0.,0.5);
  h = new TH1D("AngZRescut_conv_angzcut_chi2tight","event passing chi2redcut && angzcut && rescut;AngZRescut [deg]; events",50,0.,0.5);



  gDirectory->cd("..");//exit angzcut

  gDirectory->cd("..");//exit converged

  gDirectory->mkdir("not_converged");
  gDirectory->cd("not_converged");
  h = new TH1D("n_hit_notconv_tot","n_primary hit for not converged events in total;primary hit selected",13,0.,13.);
  h = new TH1D("n_hit_notconv_good_onlyPrimary","n_primary hit for goodEvent && onlyPrimary (tracking fail->very bad); primary hit selected",13,0.,13.);
  h2 = new TH2D("n_hit_notconv_good","n_primary hit read vs n_hit read, goodEvent(tracking fail);nprimary hit selected;nhit selected",13,0.,13.,13,0.,13.);
  h = new TH1D("n_hit_notconv_bad","n_hit read for not converged events without any primary (tracking is right); all hit selected",13,0.,13.);
  h = new TH1D("RdriftSmear_notconv","Max rdrift smearing",400,0.,2.);
  gDirectory->cd("..");

  gDirectory->mkdir("nopossible");
  gDirectory->cd("nopossible");
  h = new TH1D("n_hit_nopossible_tot","n_primary hit for event rejected by hit preselection; primary hit",13,0.,13.);
  h = new TH1D("n_hit_nopossible_good","n_primary hit, goodEvent (rejection fail);primary hit",13,0.,13.);
  h = new TH1D("n_hit_nopossible_bad","total n_hit for event without any primary rejected by hit preselection (rejection is right);primary hit",13,0.,13.);
  gDirectory->cd("..");

  //~ //WORK IN PROGRESS
  //~ gDirectory->mkdir("efficiency");
  //~ gDirectory->cd("efficiency");
  //~ h2 = new TH2D("chi2_vs_angz","number of event pass chi2cut && angzcut;chi2cut;angzcut",13,0.,13.,13,0.,13.);

  //~ gDirectory->cd("..");

  gDirectory->mkdir("MCeve");
  gDirectory->cd("MCeve");
  h = new TH1D("Real_angZ","real angZ for primary particle; AngZ[deg]",100,0.,1.);
  h = new TH1D("Real_angZRes","real angZRes for primary particle; AngZRes[deg]",100,0.,1.);
  h = new TH1D("Real_angZResAv","real angZResAv for primary particle; AngZResAv[deg]",100,0.,1.);
  h = new TH1D("Real_angPhi","real angPhi for primary particle; AngPhi[deg]",720,0.,360.);
  h = new TH1D("Real_angPhiRes","real angPhiRes for primary particle; AngPhiRes[deg]",720,0.,360.);
  h = new TH1D("Real_RTarget","real RTarget for primary particle",100,0.,5.);
  h = new TH1D("Real_RTargetNew","real RTargetNew for primary particle",100,0.,5.);
  h = new TH1D("Real_RTargetFirst","real RTargetNew for primary particle",100,0.,5.);
  h = new TH1D("Real_RTargetRes","real RTarget for primary particle",100,0.,0.5);
  h = new TH1D("Real_AngzvsRTarget_pendenza","RTarget/AngZ",100,0.,2.);
  h2 = new TH2D("Real_AngzvsRTarget","angZ vs RTarget for primary particle (angz medio);AngZ [deg];RTarget[cm]",180,0.,0.6,400.,0.,1.);
  h2 = new TH2D("Real_AngzvsRTargetNew","angZ vs RTargetNew for primary particle (new=da angZ di ogni hit);AngZ [deg];RTarget[cm]",180,0.,0.6,400.,0.,1.);
  h2 = new TH2D("Real_AngzvsRTargetFirst","angZ vs RTarget for primary particle (da primo hit, angzmedio);AngZ [deg];RTarget[cm]",180,0.,0.6,400.,0.,1.);
  h2 = new TH2D("Real_AngzvsAngzRes","AngZ vs AngzRes for primary particle;AngZ [deg];AngZRes[deg]",100,0.,0.5,100.,0.,0.5);
  gDirectory->cd("..");
  gDirectory->cd("..");//exit NewBM




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

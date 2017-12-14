#include <ConstField.h>
#include <Exception.h>
#include <FieldManager.h>
#include <KalmanFitterRefTrack.h>
#include <StateOnPlane.h>
#include <Track.h>
#include <TrackPoint.h>

#include <MaterialEffects.h>
#include <RKTrackRep.h>
#include <TGeoMaterialInterface.h>

#include <EventDisplay.h>

#include <HelixTrackModel.h>
#include <MeasurementCreator.h>

#include <TDatabasePDG.h>
#include <TEveManager.h>
#include <TGeoManager.h>
#include <TRandom.h>
#include <TVector3.h>
#include <vector>

#include "TDatabasePDG.h"
#include <TMath.h>
#include <TCanvas.h>
#include <TH1F.h>


using namespace std;



//----------------------------------------------------------------------------------------------------
//  measure the Kalman efficiency over the total sets of points satisfaing the pre-fitting criteria
double EvalError( TVector3 mom, TVector3 err ) {

  double dx2 = 2 * sqrt(err.x()) * mom.x();
  double dy2 = 2 * sqrt(err.y()) * mom.y();
  double dz2 = 2 * sqrt(err.z()) * mom.z();

  double p = mom.Mag();
  double dp2 = dx2 + dy2 + dz2;
  double dp = dp2 / ( 2 * p );

  return dp;
}


void MakePdgDatabase() {

  // clean the particle datatbase. Important!
  TDatabasePDG::Instance()->~TDatabasePDG();

  int nNewParticles = 2;
  int pdgCode = 66666600;
  vector<string> nameV     = {  "C12", 
                    "Li6"
                    // , "Li7",
                    // "B7", "B8", "B9",
                    // "Be9", "Be10", "Be11",
                    // "N12", "N13", "N14",
                    // "Alpha", "H",
                    // "O15" 
                  };

  double massV [] = {  12.1095, 
                    6.53383
                    // , 7,
                    // 7, 8, 9.3255,
                    // 9.32444, 10.2525, 11,
                    // 12.1112, 13, 14,
                    // 4, 1,
                    // 15 
                  };

  double chargeV [] = {  18, //18, 18, 
                    9
                    // , 9,
                    // 12, 12, 12,
                    // 15, 15, 15,
                    // 21, 21, 21,
                    // 6, 3,
                    // 24  
                  };

 
  // add the new particles to the standard TDatabasePDG
  // for ( int i=0; i<nNewParticles; i++) {
    TDatabasePDG::Instance()->AddParticle("C12", "C12", 12.1095, true, 0., 18, "ion", 66666600 );  
    // m_pdgCodeMap[ nameV[i] ] = pdgCode;
  // }
}




int main() {

  gRandom->SetSeed(14);

  // init MeasurementCreator
  genfit::MeasurementCreator measurementCreator;


  // init geometry and mag. field
  new TGeoManager("Geometry", "Geane geometry");
  TGeoManager::Import("genfitGeom.root");
  genfit::FieldManager::getInstance()->init(new genfit::ConstField(0.,0., 8.)); //  kGauss
  genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());


  // init event display
  genfit::EventDisplay* display = genfit::EventDisplay::getInstance();

  MakePdgDatabase();

  // init fitter
  genfit::AbsKalmanFitter* fitter = new genfit::KalmanFitterRefTrack();


  // init
  TH1F* h1 = new TH1F( "KalmanPosX", "KalmanPosX", 100, -0.1, 0.1 );
  TH1F* h2 = new TH1F( "KalmanPosX_diff", "KalmanPosX_diff", 100, -1, 1 );
  TH1F* h3 = new TH1F( "KalmanMom", "KalmanMom", 100, -1, 1 );
  TH1F* h4 = new TH1F( "KalmanMomDiff", "KalmanMomDiff", 100, -0.2, 0.2 );
  TH1F* h5 = new TH1F( "KalmanChi2", "KalmanChi2", 200, 0, 50 );

  // main loop
  for (unsigned int iEvent=0; iEvent<10; ++iEvent){

    // true start values
    TVector3 pos(0, 0, 0);
    TVector3 mom(1.,0,0);
    mom.SetPhi(gRandom->Uniform(0.,2*TMath::Pi()));
    mom.SetTheta(gRandom->Uniform(0.4*TMath::Pi(),0.6*TMath::Pi()));
    mom.SetMag(gRandom->Uniform(0.2, 1.));   // GeV


    // helix track model
    const int pdg = 13;               // particle pdg code     mu
    // const int pdg = 66666600;               // particle pdg code     C12
    const double charge = TDatabasePDG::Instance()->GetParticle(pdg)->Charge()/(3.);
    genfit::HelixTrackModel* helix = new genfit::HelixTrackModel(pos, mom, charge);
    measurementCreator.setTrackModel(helix);


    unsigned int nMeasurements = 9;
    // unsigned int nMeasurements = gRandom->Uniform(5, 15);


    // smeared start values
    const bool smearPosMom = true;     // init the Reps with smeared pos and mom
    const double posSmear = 0.1;     // cm
    const double momSmear = 3. /180.*TMath::Pi();     // rad
    const double momMagSmear = 0.1;   // relative

    TVector3 posM(pos);
    TVector3 momM(mom);
    if (smearPosMom) {
      posM.SetX(gRandom->Gaus(posM.X(),posSmear));
      posM.SetY(gRandom->Gaus(posM.Y(),posSmear));
      posM.SetZ(gRandom->Gaus(posM.Z(),posSmear));

      momM.SetPhi(gRandom->Gaus(mom.Phi(),momSmear));
      momM.SetTheta(gRandom->Gaus(mom.Theta(),momSmear));
      momM.SetMag(gRandom->Gaus(mom.Mag(), momMagSmear*mom.Mag()));
    }
    // approximate covariance
    TMatrixDSym covM(6);
    double resolution = 0.01;
    for (int i = 0; i < 3; ++i)
      covM(i,i) = resolution*resolution;
    for (int i = 3; i < 6; ++i) {
      // covM(i,i) = pow(resolution / nMeasurements / sqrt(3), 2);
      covM(i,i) = pow(resolution / nMeasurements / sqrt(3), 2);
    }

    // trackrep
    genfit::AbsTrackRep* rep = new genfit::RKTrackRep(pdg);

    // smeared start state
    genfit::MeasuredStateOnPlane stateSmeared(rep);
    stateSmeared.setPosMomCov(posM, momM, covM);


    // create track
    TVectorD seedState(6);
    TMatrixDSym seedCov(6);
    stateSmeared.get6DStateCov(seedState, seedCov);
    genfit::Track fitTrack(rep, seedState, seedCov);


    // create random measurement types
    std::vector<genfit::eMeasurementType> measurementTypes;
    for (unsigned int i = 0; i < nMeasurements; ++i)
      measurementTypes.push_back(genfit::eMeasurementType( 0 ) );
      // measurementTypes.push_back(genfit::eMeasurementType(gRandom->Uniform(8)));


    // create smeared measurements and add to track
    try{
      for (unsigned int i=0; i<measurementTypes.size(); ++i){
        std::vector<genfit::AbsMeasurement*> measurements = measurementCreator.create(measurementTypes[i], i*5.);
        fitTrack.insertPoint(new genfit::TrackPoint(measurements, &fitTrack));
      }
    }
    catch(genfit::Exception& e){
      std::cerr<<"Exception, next track"<<std::endl;
      std::cerr << e.what();
      continue;
    }

    //check
    fitTrack.checkConsistency();

    // do the fit
    fitter->processTrack(&fitTrack);

    //check
    fitTrack.checkConsistency();


    TVector3 KalmanPos = TVector3( (fitTrack.getFittedState(0).get6DState())[0],  (fitTrack.getFittedState(0).get6DState())[1], (fitTrack.getFittedState(0).get6DState())[2] );
    TVector3 KalmanMom = TVector3( (fitTrack.getFittedState(0).get6DState())[3], (fitTrack.getFittedState(0).get6DState())[4],  (fitTrack.getFittedState(0).get6DState())[5] );
    TVector3 KalmanPos_err = TVector3( (fitTrack.getFittedState(0).get6DCov())[0][0], (fitTrack.getFittedState(0).get6DCov())[1][1], (fitTrack.getFittedState(0).get6DCov())[2][2] );
    TVector3 KalmanMom_err = TVector3( (fitTrack.getFittedState(0).get6DCov())[3][3], (fitTrack.getFittedState(0).get6DCov())[4][4], (fitTrack.getFittedState(0).get6DCov())[5][5] );

    cout << "\t Kalman Pos = "; KalmanPos.Print();
    cout << "\t Gen    Pos = "; posM.Print();
    cout << "\t Kalman Pos Error da State6D = "; KalmanPos_err.Print();
    cout << "\t Kalman Mom = "<< KalmanMom.Mag() <<"     "; KalmanMom.Print();
    cout << "\t Gen    Mom = "<< momM.Mag() <<"    \n "; //momM.Print();
    cout << "\t Kalman Mom Error da State6D = "<< EvalError( KalmanMom, KalmanMom_err ) <<endl; //KalmanMom_err.Print();
    cout << "Chi2 = "<< fitTrack.getFitStatus(fitTrack.getCardinalRep())->getChi2()<< endl;

    
    h1->Fill( KalmanPos.x() );
    h2->Fill( posM.x() - KalmanPos.x() );
    h3->Fill( KalmanMom.Mag() );
    h4->Fill( momM.Mag() - KalmanMom.Mag() );
    h5->Fill( fitTrack.getFitStatus(fitTrack.getCardinalRep())->getChi2() );

    if (iEvent < 1000) {
      // add track to event display
      display->addEvent(&fitTrack);
    }


  }// end loop over events

  //save
  TCanvas* mirror = new TCanvas( "mirror", "mirror", 700, 700 );
  h1->Draw();
  mirror->SaveAs( "KalmanPosX.png" );
  h2->Draw();
  mirror->SaveAs( "KalmanPosX_diff.png" );
  h3->Draw();
  mirror->SaveAs( "KalmanMom.png" );
  h4->Draw();
  mirror->SaveAs( "KalmanMomDiff.png" );
  h5->Draw();
  mirror->SaveAs( "KalmanChi2.png" );

  delete fitter;

  // open event display
  display->open();

}



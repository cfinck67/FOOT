/*!
  \file
  \version $Id: adapted from TABMactNtuTrack.cxx,v 1.12 2003/07/08 18:58:35 mueller Exp $
  \brief   Implementation of TADCactNtuTrack.
*/

#include <math.h>

#include "TF1.h"
#include "TADCparCon.hxx"
#include "TADCntuRaw.hxx"
#include "TADCntuTrack.hxx"

#include "TADCactNtuTrack.hxx"

#include "FieldManager.h"
#include "MaterialEffects.h"
#include "TGeoMaterialInterface.h"
#include "ConstField.h"
#include "Track.h"
#include "WireMeasurementNew.h"


/*!
  \class TADCactNtuTrack TADCactNtuTrack.hxx "TADCactNtuTrack.hxx"
  \brief Track builder for Drift Chamber. **
*/

ClassImp(TADCactNtuTrack);

//------------------------------------------+-----------------------------------
//! Default constructor.

TADCactNtuTrack::TADCactNtuTrack(const char* name,
                        				 TAGdataDsc* p_ntutrk,
                        				 TAGdataDsc* p_ntuhit,
                        				 TAGparaDsc* p_dcgeo,
                        				 TAGparaDsc* p_dccon)
    : TAGaction(name, "TADCactNtuTrack - Track finder for DC"),
    fpNtuTrk(p_ntutrk),
    fpNtuHit(p_ntuhit),
    fpDCGeo(p_dcgeo),
    fpDCCon(p_dccon)
{

    AddDataOut(p_ntutrk, "TADCntuTrack");
    AddDataIn(p_ntuhit,  "TADCntuRaw");
    AddPara(p_dcgeo,  "TADCparGeo");
    AddPara(p_dccon,  "TADCparCon");

    
    const double BField = 1.;       // kGauss [ --> Eq. to 0.1 T]

    //Bfield is along Y in our case.
    FieldManager::getInstance()->init(new ConstField(0.,BField,0.));
    FieldManager::getInstance()->useCache(true, 8);
    MaterialEffects::getInstance()->init(new TGeoMaterialInterface());

}




//------------------------------------------+-----------------------------------
//! Destructor.

TADCactNtuTrack::~TADCactNtuTrack() {}

//------------------------------------------+-----------------------------------
//! Action.
Bool_t TADCactNtuTrack::Action()
{
    TADCntuTrack* p_ntutrk = (TADCntuTrack*) fpNtuTrk->Object();
    TADCntuRaw*   p_ntuhit = (TADCntuRaw*)   fpNtuHit->Object();
    TADCparGeo*   p_dcgeo = (TADCparGeo*)    fpDCGeo->Object();
    TADCparCon*   p_dccon = (TADCparCon*)    fpDCCon->Object();

    TF1 *m_mypol, *m_mypol2;
    m_mypol = p_dccon->GetCalibX();
    m_mypol2 = p_dccon->GetCalibY();

    p_ntutrk->Clear();

    Double_t chisquare_cut = 5.;

    if (!p_ntutrk->t) p_ntutrk->SetupClones();

    Int_t i_nhit = p_ntuhit->nhit;

    // Kalman
    // Track* fitTrack(nullptr);
    // /*  
    // // true start values
    // TVector3 pos(0, 0, 0);
    // TVector3 mom(1.,0,0);
    // // calc helix parameters
    // const double charge = 6;
    // genfit::HelixTrackModel* helix = new genfit::HelixTrackModel(pos, mom, charge);
    // */

    // double sign = 1.; //Charged particles 
    // const int pdg = 2212; //proton
    // const int det_type = 1; //beam monitor
    // AbsTrackRep* rep = new RKTrackRep(sign*pdg);

    // //  genfit::MeasuredStateOnPlane stateRef(rep);
    // //  rep->setPosMomCov(stateRef, pos, mom, covM);

    // genfit::MeasuredStateOnPlane stateSmeared(rep);

    // //Resolution on measurement along wire and on direction
    // const double resolution = 0.02;   // cm; resolution of generated measurements
    // TMatrixDSym covM(6);
    // for (int i = 0; i < 3; ++i)
    // covM(i,i) = resolution*resolution;
    // for (int i = 3; i < 6; ++i)
    // covM(i,i) = pow(resolution / 10 / sqrt(3), 2);

    // TVector3 posM(0, 0, 0);
    // TVector3 momM(1.,0,0);
    // rep->setPosMomCov(stateSmeared, posM, momM, covM);

    // // fill measurements vector
    // vector<AbsMeasurement*> measurements;

    // double hit_reso(0.);
    // double x,y,z,cx,cy,cz;
    // //Looping on hits
    // for(Int_t i_h = 0; i_h < i_nhit; i_h++) {

    //     Info("Action()","create WireHit");

    //     TADCntuHit* p_hit = p_ntuhit->Hit(i_h);
    //     hit_reso = p_hit->GetSigma();
    //     x = p_bmgeo->GetX(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),p_hit->View());
    //     y = p_bmgeo->GetY(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),p_hit->View());
    //     z = p_bmgeo->GetZ(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),p_hit->View());

    //     cx = p_bmgeo->GetCX(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),p_hit->View());
    //     cy = p_bmgeo->GetCY(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),p_hit->View());
    //     cz = p_bmgeo->GetCZ(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),p_hit->View());

    //     TVector3 wire_e1 = TVector3(x+cx/2,y+cy/2,z+cz/2);
    //     TVector3 wire_e2 = TVector3(x-cx/2,y-cy/2,z-cz/2);

    //     AbsMeasurement* measurement = new WireMeasurementNew (p_hit->Dist(), hit_reso, wire_e1, wire_e2, det_type, i_h, nullptr);

    //     measurements.push_back(measurement);
    // }

    // // create track
    // TVectorD seedState(6);
    // TMatrixDSym seedCov(6);
    // rep->get6DStateCov(stateSmeared, seedState, seedCov);
    // fitTrack = new Track(rep, seedState, seedCov); //initialized with smeared rep

    // // add measurements
    // for(unsigned int i=0; i<measurements.size(); ++i){
    //     //    cout<<"Adding measurements :: "<<i<<endl;
    //     fitTrack->insertPoint(new TrackPoint(measurements[i], fitTrack));
    //     // assert(fitTrack->checkConsistency());      // matteo modify
    // }

    //  cout<<"Starting the fitter"<<endl;
    // //  gGeoManager = f_BMgeoMan;
    // f_fitter->processTrack(fitTrack, false);
    // double pval = f_fitter->getPVal(fitTrack, rep);
    

    // // costruire la traccia di FOOT


    // delete fitTrack;

    return kTRUE;
}

































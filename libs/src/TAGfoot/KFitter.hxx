#ifndef KFITTER_H
#define KFITTER_H


#include <TGeoManager.h>

#include <TGeoMaterialInterface.h>
#include <ConstField.h>

#include <KalmanFitterRefTrack.h>
#include <KalmanFitter.h>
#include <DAF.h>
#include <RKTrackRep.h>
#include <Track.h>


#include <Exception.h>
#include <FieldManager.h>
#include <StateOnPlane.h>

#include <TrackPoint.h>
#include <MaterialEffects.h>
#include <TMatrixDSym.h>


// #include <EventDisplay.h>

#include "WireMeasurement.h"
#include "PlanarMeasurement.h"
#include "SpacepointMeasurement.h"
#include "SharedPlanePtr.h"

#include <TCanvas.h>
#include <TH1F.h>

#include <TVector3.h>
#include <vector>

#include "TDatabasePDG.h"
#include <TMath.h>

#include "TAGdataDsc.hxx"

#include "TADCparGeo.hxx"
#include "TADCntuRaw.hxx"

#include "TAVTparGeo.hxx"
#include "TAVTntuRaw.hxx"

#include "TAITparGeo.hxx"
#include "TAITntuRaw.hxx"

#include "GlobalPar.hxx"



// #include "GlobalTrackFoot.hxx"

using namespace std;
using namespace genfit;



typedef vector<genfit::AbsMeasurement*> MeasurementVector;
// typedef vector<GlobalTrackFoot*> TrackVector;

class KFitter {

public:

	
	KFitter( int nIter, double dPVal  );
	~KFitter() {

		delete m_fitter;
		// delete m_refFitter;
		// delete m_dafRefFitter;
		// delete m_dafSimpleFitter;

		// delete m_fitTrackCollection;

		delete h_chi2;
		// delete h_chi2_ndof;
		delete h_momentumRes;
		delete h_momentumMC;
		delete h_momentumKal;
		delete h_posRes;
		delete h_deltaP;
		// delete h_sigmaP;
		delete h_polarAngol;
		delete h_mass;
	};

	// int PrepareData4Fit( string option );
	int PrepareData4Fit( Track* fitTrack );
	void Prepare4Test( Track* fitTrack );
	void Prepare4Vertex( Track* fitTrack );
	void Prepare4InnerTracker( Track* fitTrack );
	void Prepare4DriftChamber( Track* fitTrack );

	int MakeFit(long evNum, string option);

	void RecordTrackInfo( Track* track );

	int UploadHitsVT( TAGdataDsc* footDataObj, shared_ptr<TAVTparGeo> vt_geo );
	int UploadHitsIT( TAGdataDsc* footDataObj, shared_ptr<TAITparGeo> it_geo );
	int UploadHitsDC( TAGdataDsc* footDataObj, shared_ptr<TADCparGeo> dc_geo );

	void PrintEfficiency(  );
	void PrintPositionResidual( TVector3 pos, TVector3 expectedPos );
	void PrintMomentumResidual( TVector3 pos, TVector3 expectedPos, TVector3 cov );
	void Save();

	double EvalError( TVector3 mom, TVector3 err );

	// GlobalTrackFoot* GetTrack( int id ) {
	// 	// fai controllo sul vettore
	// 	if ( id >= (int)m_fitTrackCollection->size() ) {
	// 		cout << "KFitter::GetTrack >> required track id exeeds the TrackCollection size." << endl;
	// 		exit(0);
	// 	}
	// 	return ( m_fitTrackCollection->at(id) );
	// 	// return ( new GlobalTrackFoot( m_fitTrackCollection->at(id) ) );
	// };	


private:

	KalmanFitter* m_fitter;
	AbsKalmanFitter*  m_refFitter;    		 //KalmanFitterRefTrack()
	AbsKalmanFitter*  m_dafRefFitter;    	 //DAF with kalman ref
	AbsKalmanFitter*  m_dafSimpleFitter;    	 //DAF with simple kalman


	// non va fatto il delete perche APPARENTEMENTE gia fatto
	vector<TADCntuHit*> m_DC_hitCollection;
	vector<TAVTntuHit*> m_VT_hitCollection;
	vector<TAITntuHit*> m_IT_hitCollection;
	// vector< shared_ptr<TAVTntuHit> > m_VT_hitCollection;
	// vector< shared_ptr<TAITntuHit> > m_IT_hitCollection;
	// vector< shared_ptr<TADCntuHit> > m_DC_hitCollection;
	

	// kept as std pointer just to remember how to correctely delete and free resources iwth them
	// correctely freed
	vector<AbsMeasurement*> m_hitCollectionToFit;

	shared_ptr<TAVTparGeo> m_VT_geo;
	shared_ptr<TAITparGeo> m_IT_geo;
	shared_ptr<TADCparGeo> m_DC_geo;

	// TrackVector* m_fitTrackCollection;
	vector<int> m_evNum_vect;

	TH1F* h_chi2;
	TH1F* h_momentumRes;
	TH1F* h_momentumKal;
	TH1F* h_momentumMC;
	TH1F* h_posRes;
	TH1F* h_mass;
	// TH1F* h_sigmaP;
	TH1F* h_deltaP;
	TH1F* h_polarAngol;

	map<string, int> m_detectorID_map;
	map<int, int> m_totTracksXParticles;
	map<int, int> m_fittedTracksXParticles;

	int m_nTotTracks;
	int m_nConvergedTracks;

	bool m_reverse;
	int m_debug;

};





#endif
























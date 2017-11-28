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
#include <TMatrixD.h>


// #include <EventDisplay.h>

#include "WireMeasurement.h"
#include "PlanarMeasurement.h"
#include "SpacepointMeasurement.h"
#include "SharedPlanePtr.h"

#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>

#include <TRandom3.h>

#include <TVector3.h>
#include <array>
#include <vector>

#include "TDatabasePDG.h"
#include <TMath.h>

#include "TAGdataDsc.hxx"

#include "TAMSDparGeo.hxx"
#include "TAMSDntuRaw.hxx"

#include "TAVTparGeo.hxx"
#include "TAVTntuRaw.hxx"

#include "TAITparGeo.hxx"
#include "TAITntuRaw.hxx"

#include "GlobalPar.hxx"
#include <sys/types.h>
#include <sys/stat.h>
#include <limits>



// #include "GlobalTrackFoot.hxx"

using namespace std;
using namespace genfit;

#define build_string(expr) \
    (static_cast<ostringstream*>(&(ostringstream().flush() << expr))->str())


typedef vector<genfit::AbsMeasurement*> MeasurementVector;
// typedef vector<GlobalTrackFoot*> TrackVector;

class KFitter {

public:

	
	KFitter( int nIter, double dPVal  );
	~KFitter() {
		delete m_fitter;
	};

	void MakePdgDatabase();

	// int PrepareData4Fit( string option );
	int PrepareData4Fit( Track* fitTrack );
	void Prepare4Vertex( Track* fitTrack );
	void Prepare4InnerTracker( Track* fitTrack );
	void Prepare4Strip( Track* fitTrack );

	bool PrefitRequirements( map< string, vector<AbsMeasurement*> >::iterator element );

	int MakeFit(long evNum);

	string CategoriseHitsToFit_withTrueInfo( int flukaID, int charge, int mass );
	
	void RecordTrackInfo( Track* track, string hitSampleName );

	int UploadHitsVT( TAGdataDsc* footDataObj, shared_ptr<TAVTparGeo> vt_geo );
	int UploadHitsIT( TAGdataDsc* footDataObj, shared_ptr<TAITparGeo> it_geo );
	int UploadHitsMSD( TAGdataDsc* footDataObj, shared_ptr<TAMSDparGeo> msd_geo );

	void EvaluateMomentumResolution();

	void PrintEfficiency(  );
	void PrintPositionResidual( TVector3 pos, TVector3 expectedPos, string hitSampleName );
	void PrintMomentumResidual( TVector3 pos, TVector3 expectedPos, TVector3 cov, string hitSampleName );
	void PrintMomentumResidual( TVector3 pos, TVector3 expectedPos, TMatrixD cov, string hitSampleName );
	
	void InitAllHistos( string hitSampleName );
	void InitSingleHisto( map< string, TH1F* >* histoMap, string collectionName, string histoName, int nBin, float minBin, float maxBin );

	void Save();
	void SaveHisto( TCanvas* mirror, map< string, TH1F* > histoMap, string title, string saveName );
	void SaveHisto( TCanvas* mirror, TH1F* histoMap, string title, string saveName );


	double EvalError( TVector3 mom, TVector3 err );
	double EvalError( TVector3 mom, TMatrixD cov );
	void MatrixToZero( TMatrixD *matrix );

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

	Track*  fitTrack;

	TRandom3* m_diceRoll;

	// TDatabasePDG* m_pdgDatabase;
	map<string, int> m_pdgCodeMap;

	//  delete non va fatto il delete perche APPARENTEMENTE gia fatto
	vector<TAVTntuHit*> m_VT_hitCollection;
	vector<TAITntuHit*> m_IT_hitCollection;
	vector<TAMSDntuHit*> m_MSD_hitCollection;
	
	vector<TVector3> m_MSD_posVectorSmearedHit;
	vector<TVector3> m_MSD_momVectorSmearedHit;
	vector<double> m_MSD_mass;

	// kept as std pointer just to remember how to correctely delete and free resources iwth them
	// correctely freed
	// vector<AbsMeasurement*> m_hitCollectionToFit;
	map <string, vector<AbsMeasurement*> > m_hitCollectionToFit;

	shared_ptr<TAVTparGeo> m_VT_geo;
	shared_ptr<TAITparGeo> m_IT_geo;
	shared_ptr<TAMSDparGeo> m_MSD_geo;

	// TrackVector* m_fitTrackCollection;
	vector<int> m_evNum_vect;

	vector<Color_t> m_vecHistoColor;

	map<string, int> m_nTotTracks;
	map<string, int> m_nConvergedTracks;

	map< string, TH1F* > h_chi2;
	map< string, TH1F* > h_momentumRes;
	map< string, TH1F* > h_momentumKal;
	map< string, TH1F* > h_momentumMC;
	map< string, TH1F* > h_posRes;
	map< string, TH1F* > h_mass;
	map< string, TH1F* > h_sigmaR;
	map< string, TH1F* > h_sigmaP;
	map< string, TH1F* > h_deltaP;
	map< string, TH1F* > h_polarAngol;

	map< string, TH1F* >  h_zPosGen;
	map< string, TH1F* >  h_mass_genFit;
	map< string, TH1F* >  h_charge;
	map< string, TH1F* >  h_isFitConvergedFully;
	map< string, TH1F* >  h_isFitConvergedPartially;
	map< string, TH1F* >  h_NFailedPoints;
	map< string, TH1F* >  h_isTrackPruned;
	map< string, TH1F* >  h_Ndf;
	
	map< string, TH1F* >  h_startX;
	map< string, TH1F* >  h_endX;
	map< string, TH1F* >  h_startY;
	map< string, TH1F* >  h_endY;
	
	map< string, TH1F* >  h_TrackLenght;
	map< string, TH1F* >  h_Radius;

	map< string, TH2F* >  h_covariance;

	map< string, TH1F* > h_dP_over_Ptrue;
	map< string, TH1F* > h_dP_over_Pkf;
	map< string, TH1F* > h_sigmaP_over_Pkf;
	map< string, TH1F* > h_sigmaP_over_Ptrue;

	map< string, TH1F* > h_resoP_over_Pkf;
	map< string, TH1F* > h_biasP_over_Pkf;
	map<string, map<float, TH1F*> > h_dP_x_bin;
	map<string, map<float, TH1F*> > h_dPOverP_x_bin;
	map<string, map<float, TH1F*> > h_dPOverSigmaP_x_bin;

	map<string, int> m_detectorID_map;
	// map<string, int> m_totTracksXParticles;
	// map<string, int> m_fittedTracksXParticles;

	string m_systemsON;

	float m_resoP_step;

	bool m_reverse;
	int m_debug;

};





#endif
























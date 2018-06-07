#ifndef KFITTER_H
#define KFITTER_H


#include <TGeoManager.h>

#include <TGeoMaterialInterface.h>
#include <ConstField.h>

#include <KalmanFitterRefTrack.h>
#include <KalmanFitter.h>
#include <KalmanFitterInfo.h>
#include <KalmanFitStatus.h>
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
// #include <array>
#include <vector>

#include <TMath.h>

#include "TAGdataDsc.hxx"

#include "TAMSDparGeo.hxx"
#include "TAMSDntuRaw.hxx"

#include "TAVTparGeo.hxx"
#include "TAVTntuRaw.hxx"

#include "TAITparGeo.hxx"
#include "TAITntuRaw.hxx"

#include "TAGntuMCeve.hxx"

#include <KalmanFitterInfo.h>
#include <KalmanFitStatus.h>

#include "TAGroot.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"

#include "GlobalPar.hxx"
#include "ControlPlotsRepository.hxx"
#include "GlobalTrackRepostory.hxx"
#include "MagicSkills.hxx"
#include "UpdatePDG.hxx"

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

	
	KFitter();
	~KFitter() {
		delete m_fitter;
	};


	// int PrepareData4Fit( string option );
	int PrepareData4Fit( Track* fitTrack );
	void Prepare4Vertex( Track* fitTrack );
	void Prepare4InnerTracker( Track* fitTrack );
	void Prepare4Strip( Track* fitTrack );

	bool PrefitRequirements( map< string, vector<AbsMeasurement*> >::iterator element );

	int MakeFit(long evNum);

	void GetTrueMCInfo( string hitSampleName, int x, 
						TVector3* tmpPos, TVector3* tmpMom, double* tmp_mass,
						TVector3* tmp_genPos,  TVector3* tmp_genMom, TVector3* hitPos );
	void GetKalmanTrackInfo ( string hitSampleName, int i, Track* track,
								TVector3* KalmanPos, TVector3* KalmanMom, TVector3* KalmanPos_err, TVector3* KalmanMom_err,
								TMatrixD* KalmanPos_cov, TMatrixD* KalmanMom_cov,
								double* KalmanMass );
	void GetTrueParticleType( AbsMeasurement* , int* flukaID, int* partID, int* charge, double* mass );

	void SetTrueSeed( TVector3* pos, TVector3* mom );
	void MakePrefit();

	void CategoriseHitsToFit_withTrueInfo();
	
	void RecordTrackInfo( Track* track, string hitSampleName );

	void IncludeDetectors();

	int UploadHitsVT();
	int UploadHitsIT();
	int UploadHitsMSD();

	void Finalize();	// save control plot and calculate resolutions

	void PrintEfficiency();
	// void PrintPositionResidual( TVector3 pos, TVector3 expectedPos, string hitSampleName );
	// void PrintMomentumResidual( TVector3 pos, TVector3 expectedPos, TVector3 cov, string hitSampleName );
	// void PrintMomentumResidual( TVector3 pos, TVector3 expectedPos, TMatrixD cov, string hitSampleName );
	// void PrintMomentumResidual( TVector3 pos, TVector3 expectedPos, double cov, string hitSampleName );
	
	// void InitAllHistos( string hitSampleName );
	// void InitSingleHisto( map< string, TH1F* >* histoMap, string collectionName, string histoName, int nBin, float minBin, float maxBin );
	// void InitMultiBinHistoMap( map< string, vector<TH1F*> >* histoMap, string collectionName, string histoName, int nBin, float minBin, float maxBin );
	
	void InitEventDisplay();

	// void Save();
	// void SaveHisto( TCanvas* mirror, map< string, TH1F* > histoMap, string title, string saveName );
	// void SaveHisto( TCanvas* mirror, TH1F* histoMap, string title, string saveName );


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


	bool frankFind( string what, string where )	{
	    
	    int wildcard_pos = what.find("*");
	    
	    if ( wildcard_pos == 0 )    {
	    	if( where.find( what.substr( wildcard_pos+1 ) ) != string::npos )
		        return true;
	    }
	    else if( wildcard_pos == what.size()-1 )    {
	    	if( where.find( what.substr( 0, wildcard_pos ) ) != string::npos )
		        return true;
	    }
	    else if ( wildcard_pos != string::npos )    {
	        int pre = where.find( what.substr( 0, wildcard_pos ) );
	        int post = where.find( what.substr( wildcard_pos+1 ) );
	        if( pre!=string::npos && post!=string::npos )
		        return true;
	    }

	    return false;
	}



private:

	KalmanFitter* m_fitter;
	AbsKalmanFitter*  m_refFitter;    		 //KalmanFitterRefTrack()
	AbsKalmanFitter*  m_dafRefFitter;    	 //DAF with kalman ref
	AbsKalmanFitter*  m_dafSimpleFitter;    	 //DAF with simple kalman

	// Track*  m_fitTrack;
	ControlPlotsRepository* m_controlPlotter;
	GlobalTrackRepostory* m_fitTrackCollection;

	TRandom3* m_diceRoll;


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
	vector<AbsMeasurement*> m_allHitsInMeasurementFormat;

	shared_ptr<TAVTparGeo> m_VT_geo;
	shared_ptr<TAITparGeo> m_IT_geo;
	shared_ptr<TAMSDparGeo> m_MSD_geo;

	// TrackVector* m_fitTrackCollection;
	vector<int> m_evNum_vect;

	vector<Color_t> m_vecHistoColor;

	map<string, int> m_nTotTracks;
	map<string, int> m_nConvergedTracks;
	vector<string> m_categoryFitted;
	map<string, int> m_detectorID_map;

	string m_systemsON;
	string m_kalmanOutputDir;

	int m_debug;

	long m_evNum;

	bool m_reverse;
	

};





#endif
























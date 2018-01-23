#ifndef GLOBALTRACKKALMAN_H
#define GLOBALTRACKKALMAN_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <array>
#include <sys/stat.h>

#include <TROOT.h>
#include <TStyle.h>
#include "TCanvas.h"
#include "TH1F.h"
#include <TF1.h>

#include "TVector3.h"
#include "GlobalPar.hxx"

#include <Track.h>


#define build_string(expr) \
    (static_cast<ostringstream*>(&(ostringstream().flush() << expr))->str())


using namespace std;
using namespace genfit;

class GlobalTrackKalman {
	
public:

	GlobalTrackKalman( string name, Track* track, long evNum, int stateID, 
						TVector3* mom, TVector3* pos,
						TVector3* mom_MC, TVector3* pos_MC, 
						TMatrixD* mom_cov 
						 ) {

		m_name = name;

		// bin width of the momentum resolution plot -- param file???
		m_debug = GlobalPar::GetPar()->Debug();
		m_resoP_step = 0.2;

		m_evNum = evNum;
		m_stateID = stateID;
		m_mom = *mom;
		m_pos = *pos;

		m_lenght;
		m_pdgID;
		m_chi2;
		m_pVal;

		PrintPositionResidual( *pos, *pos_MC, name );
		PrintMomentumResidual( *mom, *mom_MC, *mom_cov, name );

		m_sigmaMom;
		m_sigmaPos;

	};
	~GlobalTrackKalman() {};

	double EvalError( TVector3 mom, TVector3 err );
	double EvalError( TVector3 mom, TMatrixD cov );

	void PrintPositionResidual( TVector3 pos, TVector3 expectedPos, string hitSampleName );
	
	void PrintMomentumResidual( TVector3 meas, TVector3 expected, TVector3 cov, string hitSampleName );
	void PrintMomentumResidual( TVector3 meas, TVector3 expected, TMatrixD cov, string hitSampleName );
	void PrintMomentumResidual( TVector3 meas, TVector3 expected, double err, string hitSampleName );

	void EvaluateMomentumResolution();

	void Save();

/***	variables	***/

	int m_debug;

	string m_name;
	
	long m_evNum;
	int m_stateID;
	TVector3 m_mom;
	TVector3 m_pos;

	double m_lenght;
	int m_pdgID;
	double m_chi2;
	double m_pVal;

	double m_resoP_step;

	TVector3 m_sigmaMom;
	TVector3 m_sigmaPos;

	map<string, map<float, TH1F*> > h_dP_x_bin;
	map<string, map<float, TH1F*> > h_dPOverP_x_bin;
	map<string, map<float, TH1F*> > h_dPOverSigmaP_x_bin;
	
	// map< string, TH1F* > h_posRes;

	// map< string, TH1F* > h_dP_over_Ptrue;
	// map< string, TH1F* > h_dP_over_Pkf;
	// map< string, TH1F* > h_sigmaP_over_Pkf;

	map< string, TH1F* > h_resoP_over_Pkf;
	map< string, TH1F* > h_biasP_over_Pkf;

private:


};

#endif
















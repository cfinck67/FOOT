#ifndef ControlPlotsRepository_H
#define ControlPlotsRepository_H


#include <Track.h>
#include <TMath.h>
#include "ControlPlotInfo.hxx"

#define build_string(expr) \
    (static_cast<ostringstream*>(&(ostringstream().flush() << expr))->str())


using namespace genfit;


class ControlPlotsRepository {
	
public:

	static ControlPlotsRepository* Instance();
	static ControlPlotsRepository* GetControlObject( string outputDir );
	~ControlPlotsRepository() {};

	void SetOutputDir( string outputDir ) { m_outputDir = outputDir; };
	void FillMap( string mapName, double x );
        void PrintMap();
        void PrintMap(TFile* file);
        void PrintOutputFile();

	//change dir
	void SetControlPos_4eachState( string hitSampleName, int i, TVector3 *kal, TVector3 *trueMC, TVector3 *detector ) {

		FillMap( hitSampleName + "__State" +build_string(i)+ "__position__kalVStrue__mag", fabs( (*kal - *trueMC).Mag() ) );
		FillMap( hitSampleName + "__State" +build_string(i)+ "__position__kalVSdet__mag",  fabs( (*kal - *detector).Mag() ) );
		FillMap( hitSampleName + "__State" +build_string(i)+ "__position__trueVSdet__mag", fabs( (*trueMC - *detector).Mag() ) );

		FillMap( hitSampleName + "__State" +build_string(i)+ "__position__kalVStrue__x", fabs( (*kal - *trueMC).x() ) );
		FillMap( hitSampleName + "__State" +build_string(i)+ "__position__kalVStrue__y", fabs( (*kal - *trueMC).y() ) );
		FillMap( hitSampleName + "__State" +build_string(i)+ "__position__kalVStrue__z", fabs( (*kal - *trueMC).z() ) );

		FillMap( hitSampleName + "__State" +build_string(i)+ "__position__kalVSdet__x", fabs( (*kal - *detector).x() ) );
		FillMap( hitSampleName + "__State" +build_string(i)+ "__position__kalVSdet__y", fabs( (*kal - *detector).y() ) );
		FillMap( hitSampleName + "__State" +build_string(i)+ "__position__kalVSdet__z", fabs( (*kal - *detector).z() ) );

	}

	//change dir
	void SetControlMom_4eachState( string hitSampleName, int i, TVector3 *kal, TVector3 *trueMC, TVector3 *gen ) {

		FillMap( hitSampleName + "__State" +build_string(i)+ "__momentum__kalVStrue__mag", (kal->Mag() - trueMC->Mag()) / trueMC->Mag() );
		FillMap( hitSampleName + "__State" +build_string(i)+ "__momentum__kalVSgen__mag",  (kal->Mag() - gen->Mag()) / gen->Mag() );

		FillMap( hitSampleName + "__State" +build_string(i)+ "__momentum__kalVStrue__angle",  (kal->Angle(*trueMC) * 180 / TMath::Pi() ) );

		FillMap( hitSampleName + "__State" +build_string(i)+ "__momentum__kalVStrue__x", ( kal->x() - trueMC->x() ) / trueMC->x() );
		FillMap( hitSampleName + "__State" +build_string(i)+ "__momentum__kalVStrue__y", ( kal->x() - trueMC->x() ) / trueMC->y() );
		FillMap( hitSampleName + "__State" +build_string(i)+ "__momentum__kalVStrue__z", ( kal->x() - trueMC->x() ) / trueMC->z() );

		FillMap( hitSampleName + "__State" +build_string(i)+ "__momentum__kalVSgen__x", ( kal->x() - gen->x() ) / gen->x() );
		FillMap( hitSampleName + "__State" +build_string(i)+ "__momentum__kalVSgen__y", ( kal->x() - gen->x() ) / gen->y() );
		FillMap( hitSampleName + "__State" +build_string(i)+ "__momentum__kalVSgen__z", ( kal->x() - gen->x() ) / gen->z() );

	}


	void SetMom_Gen( string hitSampleName, TVector3 *p ) {

		FillMap( hitSampleName + "__momentum__gen__mag", p->Mag() );
		FillMap( hitSampleName + "__momentum__gen__x", p->x() );
		FillMap( hitSampleName + "__momentum__gen__y", p->y() );
		FillMap( hitSampleName + "__momentum__gen__z", p->z() );

	};


	void SetMom_TrueMC( string hitSampleName, TVector3 *p, double mass ) {

		FillMap( hitSampleName + "__momentum__trueMC__mag", p->Mag() );
		FillMap( hitSampleName + "__momentum__trueMC__x", p->x() );
		FillMap( hitSampleName + "__momentum__trueMC__y", p->y() );
		FillMap( hitSampleName + "__momentum__trueMC__z", p->z() );
		
		FillMap( hitSampleName + "__mass__trueMC", mass );

	};

	void SetMom_Kal( string hitSampleName, TVector3 *p, TVector3 *err ) {

		FillMap( hitSampleName + "__momentum__kalman__mag", p->Mag() );
		FillMap( hitSampleName + "__momentum__kalman__polarAngle", p->Theta() *180 / M_PI );
		// Nomalised
		FillMap( hitSampleName + "__momentum__kalman__x", p->x() / p->Mag() );
		FillMap( hitSampleName + "__momentum__kalman__y", p->y() / p->Mag() );
		FillMap( hitSampleName + "__momentum__kalman__z", p->z() / p->Mag() );

		// err/mom 
		FillMap( hitSampleName + "__momentum__kalman__sigmax", sqrt(err->x()) / p->x() );
		FillMap( hitSampleName + "__momentum__kalman__sigmay", sqrt(err->y()) / p->y() );
		FillMap( hitSampleName + "__momentum__kalman__sigmaz", sqrt(err->z()) / p->z() );

	};


	// void SetPos_Gen( string hitSampleName, TVector3* p, TVector3* err ) { };
	// void SetPos_TrueMC( string hitSampleName, TVector3* p, TVector3* err ) { };
	void SetPos_Kal( string hitSampleName, TVector3* p, TVector3* err ) {

		double mag_err = (TVector3( sqrt(err->x()), sqrt(err->y()), sqrt(err->z()) )).Mag();
		FillMap( hitSampleName + "__position__kalman__mag", sqrt( mag_err )/ p->Mag() );
		FillMap( hitSampleName + "__position__kalman__x", sqrt( err->x() )/p->x() );
		FillMap( hitSampleName + "__position__kalman__y", sqrt( err->y() )/p->y() );
		FillMap( hitSampleName + "__position__kalman__z", sqrt( err->z() )/p->z() );
	};


	void SetTrackInfo( string hitSampleName, Track* track ) {

		FillMap( hitSampleName + "__chi2__kalman", track->getFitStatus(track->getCardinalRep())->getChi2() );
		// if ( m_debug > 0 )		cout << "Chi2 = "<< track->getFitStatus(track->getCardinalRep())->getChi2() << endl;

		// 		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!                ±  
		FillMap( hitSampleName + "__mass__kalman", track->getFittedState(0).getMass() );
			
		FillMap( hitSampleName + "__charge__kalman", track->getFitStatus(track->getCardinalRep())->getCharge() );
		// FillMap( hitSampleName + "__position__kalman__mag", track->getFittedState(i).getMass() );
		FillMap( hitSampleName + "__isFitConvergedFully__kalman", track->getFitStatus(track->getCardinalRep())->isFitConvergedFully() );
		FillMap( hitSampleName + "__isFitConvergedPartially__kalman", track->getFitStatus(track->getCardinalRep())->isFitConvergedPartially() );
		FillMap( hitSampleName + "__NFailedPoints__kalman", track->getFitStatus(track->getCardinalRep())->getNFailedPoints() );
		FillMap( hitSampleName + "__isTrackPruned__kalman", track->getFitStatus(track->getCardinalRep())->isTrackPruned() );
		FillMap( hitSampleName + "__number__ofDegreeOfFreedom__kalman", track->getFitStatus(track->getCardinalRep())->getNdf() );

		FillMap( hitSampleName + "__trackLenght__kalman", track->getTrackLen( track->getCardinalRep() ) );
		// FillMap( hitSampleName + "__position__kalman__mag", track->getTOF( track->getCardinalRep() ) );		// doesn't work!!!!
		// FillMap( hitSampleName + "__position__kalman__mag", (KalmanPos - tmpPos).Mag() );	

	};


	void SetCovarianceMomentum_2D( string hitSampleName, TMatrixD* KalmanPos_cov ) {

		// init
		// if ( h_covariance.find( hitSampleName ) == h_covariance.end() )					
		// 	h_covariance[hitSampleName] = new TH2F( "covariance", "covariance", 3, 0, 3, 3, 0, 3 );


		// fill
		// prduce the covariance matrix on the measured state
		// for ( int j=0; j<3; j++ ) {
		// 	for ( int k=0; k<3; k++ ) {
		// 		// USA KalmanMom_cov !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// 		// h_covariance[ hitSampleName ]->Fill( j, k, (track->getFittedState(i).get6DCov())[j+3][k+3] );
		// 	}
		// }

		
		//	print
		// 2D
		// for ( map< string, TH2F* >::iterator it=h_covariance.begin(); it != h_covariance.end(); it++ ) {

		// 	// (*it).second->GetXaxis()->SetTitle("p [GeV]");
		// 	(*it).second->Scale( 1./( (float)((*it).second->GetEntries())/9. ) );
		// 	gStyle->SetPalette(1);
		// 	mirror->SetLogz(1);
		// 	(*it).second->Draw("colz text");
		// 	mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+"covariance" + ".png").c_str() );
		// 	mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+"covariance" + ".root").c_str() );
		// 	mirror->SetLogz(0);
		// }

	}

	bool m_alreadyPrintedOut;
	string m_lastPrintedDir;
	string m_outputDir;
	map< string, ControlPlotInfo > m_chizu;
        string m_outputfilename;

private:

	ControlPlotsRepository();
	static ControlPlotsRepository* m_pInstance;
  
  
};

#endif

































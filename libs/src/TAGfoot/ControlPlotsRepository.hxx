#ifndef ControlPlotsRepository_H
#define ControlPlotsRepository_H


#include <Track.h>
#include <TMath.h>

#include "ControlPlotInfo.hxx"
#include "TABMntuTrack.hxx"
#include "TABMntuTrackTr.hxx"
#include "TABMntuRaw.hxx"

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
  void PrintOutputFile();
  void PrintOutputNtuple();
  void SaveOutputFile();
  TFile* GetTFile(){return f_out;};
  
  TFile* f_out;
  

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

  //Beam Monitor OutputFile
  void BM_setnturaw_info(string hitSampleName, TABMntuRaw* bmnturaw, TABMparGeo* bmgeo, TABMparCon* bmcon, vector< vector<Int_t> > &cell_occupy){
    FillMap( hitSampleName + "__raw_nhitsxevent", bmnturaw->nhit);
    if(bmnturaw->nhit < bmcon->GetMinnhit_cut())
      FillMap( hitSampleName + "__track_error", -1);
    if(bmnturaw->nhit > bmcon->GetMaxnhit_cut())
      FillMap( hitSampleName + "__track_error", -2);
    //track_error code meaning: 0=ok, -1=nhit<minnhit_cut, -2=nhit>nmaxhit, 1=firedUview<planehit_cut, 2=firedVview<planehit_cut, 3=fit not converged, 4=track_chi2red>bmcon_chi2redcut

    //loop on hits
    for (Int_t i = 0; i < bmnturaw->nhit; i++) { 
      bmntuhit = bmnturaw->Hit(i);    
      if(bmntuhit->Dist()>0 && bmntuhit->Dist()<1.)
        FillMap( hitSampleName + "__raw_rdrift_right", bmntuhit->Dist());
      else
        FillMap( hitSampleName + "__raw_rdrift_error", bmntuhit->Dist());
      FillMap( hitSampleName + "__raw_cell", bmntuhit->Cell());
      FillMap( hitSampleName + "__raw_view", bmntuhit->View());
      FillMap( hitSampleName + "__raw_plane", bmntuhit->Plane());
      FillMap( hitSampleName + "__raw_time", bmntuhit->Tdrift());
      FillMap( hitSampleName + "__raw_occupancy", bmgeo->GetBMNcell(bmntuhit->Plane(), bmntuhit->View(), bmntuhit->Cell()));
      FillMap( hitSampleName + "__raw_selected_rejected", (bmntuhit->GetIsSelected()) ? 1:-1);
      if(bmntuhit->GetIsSelected()){
        FillMap( hitSampleName + "__rawsel_chi2", bmntuhit->GetChi2());
        FillMap( hitSampleName + "__rawsel_rdrift", bmntuhit->Dist());
        FillMap( hitSampleName + "__rawsel_cell", bmntuhit->Cell());
        FillMap( hitSampleName + "__rawsel_view", bmntuhit->View());
        FillMap( hitSampleName + "__rawsel_plane", bmntuhit->Plane());
        FillMap( hitSampleName + "__rawsel_occupancy", bmgeo->GetBMNcell(bmntuhit->Plane(), bmntuhit->View(), bmntuhit->Cell()));
      }
    }
    
    //loop on cell_occupy
    for(Int_t i=0;i<36;i++)
      FillMap( hitSampleName + "__raw_nhitsxcell", cell_occupy[i].size());
            
    return;  
  }
  
  
  Bool_t BM_setntutrack_info(string hitSampleName, TABMntuTrack* bmntutrack,TABMntuRaw* bmnturaw, TABMparCon* bmcon){
    
    FillMap( hitSampleName + "__track_error", bmntutrack->trk_status);
    FillMap( hitSampleName + "__track_tracknumxevent", bmntutrack->ntrk);
    
    //__track_error code meaning: 0=ok, -1=nhit<minnhit_cut, -2=nhit>nmaxhit, 1=firedUview<planehit_cut, 2=firedVview<planehit_cut, 3=number of hit rejected > rejmax_cut ,4=fit not converged, 5=track_chi2red>bmcon_chi2redcut
    
    //loop on ntrk (it should be only one for the moment)  
    for (Int_t i = 0; i < bmntutrack->ntrk; i++) {
      bmntutracktr = bmntutrack->Track(i);
      FillMap( hitSampleName + "__track_chi2red", bmntutracktr->GetMyChi2Red());
      if(bmntutrack->trk_status==0){//selected tracks
	FillMap( hitSampleName + "__track_mylar1_x", bmntutracktr->GetMylar1Pos().X());
	FillMap( hitSampleName + "__track_mylar1_y", bmntutracktr->GetMylar1Pos().Y());
	FillMap( hitSampleName + "__track_mylar2_x", bmntutracktr->GetMylar2Pos().X());
	FillMap( hitSampleName + "__track_mylar2_y", bmntutracktr->GetMylar2Pos().Y());
	if(fabs(bmntutracktr->GetMylar1Pos().X())<5.5)
	  FillMap( hitSampleName + "__track_mylar1_x_tight", bmntutracktr->GetMylar1Pos().X());
	if(fabs(bmntutracktr->GetMylar1Pos().Y())<5.5)
	  FillMap( hitSampleName + "__track_mylar1_y_tight", bmntutracktr->GetMylar1Pos().Y());
	if(fabs(bmntutracktr->GetMylar2Pos().X())<5.5)
	  FillMap( hitSampleName + "__track_mylar2_x_tight", bmntutracktr->GetMylar2Pos().X());
	if(fabs(bmntutracktr->GetMylar2Pos().Y())<5.5)
	  FillMap( hitSampleName + "__track_mylar2_y_tight", bmntutracktr->GetMylar2Pos().Y());
	
	
        FillMap( hitSampleName + "__track_target_x", bmntutracktr->GetTargetPos().X());
        FillMap( hitSampleName + "__track_target_y", bmntutracktr->GetTargetPos().Y());
        FillMap( hitSampleName + "__tracksel_chi2red", bmntutracktr->GetMyChi2Red());
        FillMap( hitSampleName + "__tracksel_hitselected", bmntutracktr->GetNhit());
        FillMap( hitSampleName + "__tracksel_hitrejected", bmnturaw->nhit-bmntutracktr->GetNhit());
        FillMap( hitSampleName + "__tracksel_AngZ", bmntutracktr->GetAngZ());
        FillMap( hitSampleName + "__tracksel_AngZRes", bmntutracktr->GetAngZRes());
        FillMap( hitSampleName + "__tracksel_AngPhi", bmntutracktr->GetAngPhi());
        FillMap( hitSampleName + "__tracksel_AngPhiRes", bmntutracktr->GetAngPhiRes());
        FillMap( hitSampleName + "__tracksel_AngXZ", atan((bmntutracktr->GetMylar2Pos().X()-bmntutracktr->GetMylar1Pos().X())/(bmntutracktr->GetMylar2Pos().Z()-bmntutracktr->GetMylar1Pos().Z())));
        FillMap( hitSampleName + "__tracksel_AngYZ", atan((bmntutracktr->GetMylar2Pos().Y()-bmntutracktr->GetMylar1Pos().Y())/(bmntutracktr->GetMylar2Pos().Z()-bmntutracktr->GetMylar1Pos().Z())));
        FillMap( hitSampleName + "__tracksel_mylar1_x", bmntutracktr->GetMylar1Pos().X());
        FillMap( hitSampleName + "__tracksel_mylar1_y", bmntutracktr->GetMylar1Pos().Y());
        FillMap( hitSampleName + "__tracksel_mylar2_x", bmntutracktr->GetMylar2Pos().X());
        FillMap( hitSampleName + "__tracksel_mylar2_y", bmntutracktr->GetMylar2Pos().Y());
        FillMap( hitSampleName + "__tracksel_target_x", bmntutracktr->GetTargetPos().X());
        FillMap( hitSampleName + "__tracksel_target_y", bmntutracktr->GetTargetPos().Y());
      }
    }
  
    return bmntutrack->trk_status;  
  }
  
  //Beam Monitor OutputNtuple
  void BM_setntuple_hit(Double_t rdrift){
    ntuple_out.BM_hit_rdrift.push_back(rdrift);
  return;
  }
  void BM_setntuple_track(Double_t chi2){
    ntuple_out.BM_track_chi2.push_back(chi2);
  return;
  }


	struct Ntuple_out {
		vector< Double_t >  Reco_track_px;
		vector< Double_t >  Reco_track_py;
		vector< Double_t >  Reco_track_pz;
		vector< Double_t >  Reco_track_x;
		vector< Double_t >  Reco_track_y;
		vector< Double_t >  Reco_track_z;
		vector< Double_t >  Truth_track_px;
		vector< Double_t >  Truth_track_py;
		vector< Double_t >  Truth_track_pz;
		
    //Beam Monitor stuff
    vector< Double_t >  BM_hit_rdrift;
		vector< Double_t >  BM_track_chi2;
	};
  
  Ntuple_out  ntuple_out;
  
  
	void Set_Outputntuple( TVector3 *Reco_mom, TVector3 *Reco_pos, TVector3 *Truth_mom) {

		ntuple_out.Reco_track_px.push_back(Reco_mom->x());
		ntuple_out.Reco_track_py.push_back(Reco_mom->y());
		ntuple_out.Reco_track_pz.push_back(Reco_mom->z());

		ntuple_out.Reco_track_x.push_back(Reco_pos->x());
		ntuple_out.Reco_track_y.push_back(Reco_pos->y());
		ntuple_out.Reco_track_z.push_back(Reco_pos->z());

		ntuple_out.Truth_track_px.push_back(Truth_mom->x());
		ntuple_out.Truth_track_py.push_back(Truth_mom->y());
		ntuple_out.Truth_track_pz.push_back(Truth_mom->z());

	}



	bool m_alreadyPrintedOut;

	string m_lastPrintedDir;
	string m_outputDir;
	string m_outputfilename;
	string m_outputntuplename;
	
	map< string, ControlPlotInfo > m_chizu;


private:

	ControlPlotsRepository();
	static ControlPlotsRepository* m_pInstance;
  TABMntuHit* bmntuhit;           //hit of tabmnturaw
  TABMntuTrackTr* bmntutracktr;   //track of tabmntutrack

};

#endif

































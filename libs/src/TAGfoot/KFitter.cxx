

#include "KFitter.hxx"


KFitter::KFitter ( int nIter, double dPVal ) {
	

	m_debug = GlobalPar::GetPar()->Debug();

	// test variable of control
	m_reverse = GlobalPar::GetPar()->IsKalReverse();

	// init
	m_nTotTracks = 0;
	m_nConvergedTracks = 0;

	m_VT_hitCollection.clear();
	m_IT_hitCollection.clear();
	m_DC_hitCollection.clear();

	vector<string> tmp_detName = { "STC", "BM", "TG", "VTX", "ITR", "DC" };
	for (unsigned int i=0; i<tmp_detName.size(); i++)
		m_detectorID_map[ tmp_detName[i] ] = i;

	if ( !(GlobalPar::GetPar()->KalSystems().size() == 1 && GlobalPar::GetPar()->KalSystems().at(0) == "all") )	 {
		for (unsigned int i=0; i<GlobalPar::GetPar()->KalSystems().size(); i++ ) {
			if ( m_detectorID_map.find( GlobalPar::GetPar()->KalSystems().at(i) ) == m_detectorID_map.end() ) 
				cout<< "ERROR::KFitter::KFitter  --> KalSystems parameter not set properly, check befor continue."<< endl, exit(0);
		}
	}
	if ( m_debug > 2 )		cout << GlobalPar::GetPar()->KalSystems().at(0) << endl;

	// needed for the event display
	// //const genfit::eFitterType fitterId = genfit::SimpleKalman;
 //    const genfit::eFitterType fitterId = genfit::RefKalman;
 //    //const genfit::eFitterType fitterId = genfit::DafRef;
 //    //const genfit::eFitterType fitterId = genfit::DafSimple;

    // const genfit::eMultipleMeasurementHandling mmHandling = genfit::weightedAverage;	//	suggested
    // const genfit::eMultipleMeasurementHandling mmHandling = genfit::unweightedClosestToReference;
    //const genfit::eMultipleMeasurementHandling mmHandling = genfit::unweightedClosestToPrediction;
    //const genfit::eMultipleMeasurementHandling mmHandling = genfit::weightedClosestToReference;
    //const genfit::eMultipleMeasurementHandling mmHandling = genfit::weightedClosestToPrediction;
    // const genfit::eMultipleMeasurementHandling mmHandling = genfit::unweightedClosestToReferenceWire;
    // const genfit::eMultipleMeasurementHandling mmHandling = genfit::unweightedClosestToPredictionWire; //
    //const genfit::eMultipleMeasurementHandling mmHandling = genfit::weightedClosestToReferenceWire;
    //const genfit::eMultipleMeasurementHandling mmHandling = genfit::weightedClosestToPredictionWire;

	// temorary safty
	if ( GlobalPar::GetPar()->KalMode() != 1 )
		cout<< "ERROR::KFitter::KFitter  --> KalMode parameter not set properly, check befor continue."<< endl, exit(0);

	if ( GlobalPar::GetPar()->KalMode() == 1 )
		m_fitter = new KalmanFitter(nIter, dPVal);
	else if ( GlobalPar::GetPar()->KalMode() == 2 )
		m_refFitter = new KalmanFitterRefTrack(nIter, dPVal);  
	else if ( GlobalPar::GetPar()->KalMode() == 3 ) 
		m_dafRefFitter = new DAF(true, nIter, dPVal);
	else if ( GlobalPar::GetPar()->KalMode() == 4 )
		m_dafSimpleFitter = new DAF(false, nIter, dPVal);

	// m_fitter->setMultipleMeasurementHandling(mmHandling);
	// f_fitter->setMultipleMeasurementHandling(unweightedClosestToPredictionWire);

	// save track info .... really usefull?
	// m_fitTrackCollection->clear();


	// output histos
	h_chi2 = new TH1F( "TrackChi2", "TrackChi2", 100, 0, 10 );
	h_posRes = new TH1F( "h_posRes", "h_posRes", 40, 0, 4 );
	// h_sigmaP = new TH1F( "h_sigmaP", "h_sigmaP", 100, 0, 0.1 );
	h_deltaP = new TH1F( "h_deltaP", "h_deltaP", 100, -1, 1 );
	h_momentumRes = new TH1F( "h_momentumRes", "h_momentumRes", 80, -4, 4 );
	h_momentumMC = new TH1F( "h_momentumMC", "h_momentumMC", 50, 0, 10 );
	h_momentumKal = new TH1F( "h_momentumKal", "h_momentumKal", 50, 0, 10 );
	h_polarAngol = new TH1F( "h_polarAngol", "h_polarAngol", 100, 0, 6 );
	h_mass = new TH1F( "h_mass", "h_mass", 90, 0, 15 );

}




// upload measurement points from vertex pixel
int KFitter::UploadHitsVT( TAGdataDsc* footDataObj, shared_ptr<TAVTparGeo> vt_geo ) {
	
	m_VT_geo = vt_geo;
	TAVTntuRaw* ntup = (TAVTntuRaw*) footDataObj->GenerateObject();

	if ( m_debug > 0 )		cout << "N vertex pixel read: " << ntup->GetPixelsN(0) << endl;
	for (int i = 0; i < ntup->GetPixelsN(0); i++) 
        m_VT_hitCollection.push_back( ntup->GetPixel(0,i) );

	return ntup->GetPixelsN(0);
}




// upload measurement points from Inner Tracker pixel
int KFitter::UploadHitsIT( TAGdataDsc* footDataObj, shared_ptr<TAITparGeo> it_geo ) {
	
	m_IT_geo = it_geo;
	TAITntuRaw* ntup = (TAITntuRaw*) footDataObj->GenerateObject();

	if ( m_debug > 0 )		cout << "N inner pixel read: " << ntup->GetPixelsN(0) << endl;
	for (int i = 0; i < ntup->GetPixelsN(0); i++) 
        m_IT_hitCollection.push_back( ntup->GetPixel(0,i) );

	return ntup->GetPixelsN(0);
}




// upload measurement points from drift chamber
int KFitter::UploadHitsDC( TAGdataDsc* footDataObj, shared_ptr<TADCparGeo> dc_geo ) {

	m_DC_geo = dc_geo;
	TADCntuRaw* ntup = (TADCntuRaw*) footDataObj->GenerateObject();

	if ( m_debug > 0 )		cout << "N wire read: " << ntup->NHit() << endl;
	for (int i = 0; i < ntup->NHit(); i++) 
        m_DC_hitCollection.push_back( ntup->Hit(i) );

	return ntup->NHit();
}







// pack together the hits to be fitted, from all the detectors, selct different preselecion options
int KFitter::PrepareData4Fit( Track* fitTrack ) {

	string option = "";
	for (unsigned int i=0; i<GlobalPar::GetPar()->KalSystems().size(); i++ ) {
		if (i != 0)		option += " ";
		option += GlobalPar::GetPar()->KalSystems().at(i);
	}
	

	if (option == "test") {

	    cout<<  endl << "KFitter::PrepareData4Fit test" << endl<< endl;
		Prepare4Test(fitTrack);
	    return m_hitCollectionToFit.size();

	}
	// Vertex
	if ( option == "all" ||  option.find( "VT" ) != string::npos ) {

		if ( m_debug > 0 )		cout << endl<<endl << "Filling vertex hit collection  = " << m_VT_hitCollection.size() << endl;
		Prepare4Vertex(fitTrack);

	}
	// Inner Tracker
	if ( option == "all" || option.find( "IT" ) != string::npos) {
		
		if ( m_debug > 0 )		cout <<endl<<endl << "Filling inner detector hit collection = " << m_IT_hitCollection.size() << endl;
		Prepare4InnerTracker(fitTrack);
	        
	}
	// DC
	if ( option == "all" || option.find( "DC" ) != string::npos ) {
		
		Prepare4DriftChamber(fitTrack);
	    if ( m_debug > 0 )		cout << endl<<endl << "Filling Drift Chamber hit collection = " << m_DC_hitCollection.size() << endl;

	}



	int testHitNumberLimit = 0;
	if ( option == "all" || 
		( option.find( "VT" ) != string::npos && option.find( "IT" ) != string::npos && option.find( "DC" ) != string::npos ) ) {
		testHitNumberLimit = 17;
	}
	else if ( option.find( "VT" ) == string::npos && option.find( "IT" ) != string::npos && option.find( "DC" ) != string::npos ) {
		testHitNumberLimit = 14;
	}
	else if ( option.find( "VT" ) != string::npos && option.find( "IT" ) == string::npos && option.find( "DC" ) != string::npos ) {
		testHitNumberLimit = 15;
	}
	else if ( option.find( "VT" ) != string::npos && option.find( "IT" ) != string::npos && option.find( "DC" ) == string::npos ) {
		testHitNumberLimit = 5;
	}
	else if ( option.find( "VT" ) == string::npos && option.find( "IT" ) == string::npos && option.find( "DC" ) != string::npos ) {
		testHitNumberLimit = 12;
	}
	else {
		cout << "ERROR >> KFitter::PrepareData4Fit :: option mode is wrong!!!" << endl;
		exit(0);
	}


	//	clean all collections
	if ( (int)m_hitCollectionToFit.size() != testHitNumberLimit ) {
		for ( vector<AbsMeasurement*>::iterator it=m_hitCollectionToFit.begin(); it != m_hitCollectionToFit.end(); it++ )
			delete (*it);
		m_hitCollectionToFit.clear();
		m_VT_hitCollection.clear();
		m_IT_hitCollection.clear();
		m_DC_hitCollection.clear();
		return 0;
	}	


	return m_hitCollectionToFit.size();
}






void KFitter::Prepare4Vertex( Track* fitTrack ) {
	TMatrixDSym hitCov(3); 
	TVectorD hitCoords(3);
    for (unsigned int i = 0; i < m_VT_hitCollection.size(); i++) {
        
        TAVTntuHit* p_hit = m_VT_hitCollection.at(i);
        TVector3 hitPos = m_VT_geo->GetPosition( p_hit->GetLayer(), p_hit->GetPixelColumn(), p_hit->GetPixelLine() );
        
        if ( m_debug > 0 )		cout << "VTX test = Layer:" << p_hit->GetLayer() <<" col:"<< p_hit->GetPixelColumn() <<" row:"<< p_hit->GetPixelLine() << endl;
        if ( m_debug > 0 )		cout << "Hit " << i;
        if ( m_debug > 0 )		hitPos.Print();

        hitCoords(0)=hitPos.x();
		hitCoords(1)=hitPos.y();
		hitCoords(2)=hitPos.z();
		
		double pixReso = 0.002*2;
		hitCov.UnitMatrix();         
		hitCov *= pixReso*pixReso; 
		double zErr = 0.0025;
		hitCov[2][2] = zErr*zErr; 

        AbsMeasurement* hit = new SpacepointMeasurement(hitCoords, hitCov, m_detectorID_map["IT"], i, new TrackPoint(fitTrack));
        m_hitCollectionToFit.push_back(hit);
    }
}



void KFitter::Prepare4InnerTracker( Track* fitTrack ) {
	TMatrixDSym hitCov(3); 
	TVectorD hitCoords(3);

    for (unsigned int i = 0; i < m_IT_hitCollection.size(); i++) {
        
        TAITntuHit* p_hit = m_IT_hitCollection.at(i);

        TVector3 hitPos = m_IT_geo->GetPosition( p_hit->GetLayer(), p_hit->GetPixelColumn(), p_hit->GetPixelLine() );

        if ( m_debug > 0 )		cout << "IT test = Layer:" << p_hit->GetLayer() <<" col:"<< p_hit->GetPixelColumn() <<" row:"<< p_hit->GetPixelLine() << endl;
        if ( m_debug > 0 )		cout << "Hit " << i;
        if ( m_debug > 0 )		hitPos.Print();

        hitCoords(0)=hitPos.x();
		hitCoords(1)=hitPos.y();
		hitCoords(2)=hitPos.z();

		double pixReso = 0.002*2;
		hitCov.UnitMatrix();         
		hitCov *= pixReso*pixReso; 
		double zErr = 0.0025;
		hitCov[2][2] = zErr*zErr; 

        AbsMeasurement* hit = new SpacepointMeasurement(hitCoords, hitCov, m_detectorID_map["IT"], i, new TrackPoint(fitTrack));
        m_hitCollectionToFit.push_back(hit);
    }
}




void KFitter::Prepare4DriftChamber( Track* fitTrack ) {

	vector<string> viewTranslator = { "SIDE", "TOP" };
    TMatrixDSym hitCov(7); 
	TVectorD hitCoords(7);

    for (unsigned int i = 0; i < m_DC_hitCollection.size(); i++) {
        
        TADCntuHit* p_hit = m_DC_hitCollection.at(i);
        int view = ( p_hit->View() == 1 ? 0 : 1 );

        TVector3 wire_end1, wire_end2;        
        m_DC_geo->GetWireEndPoints( p_hit->Cell(), p_hit->Plane(), viewTranslator[view], wire_end1, wire_end2 );

        if ( m_debug > 0 )		cout << "DC test = cell:" << p_hit->Cell() <<" layer:"<< p_hit->Plane() <<" view:"<< viewTranslator[view] << endl;
        if ( m_debug > 0 )		cout << "Hit " << i << " : cell: " << p_hit->Cell() 
              << " plane: " << p_hit->Plane() << " view: " << viewTranslator[view] << endl;
        if ( m_debug > 0 )		cout << "end1 ";
        if ( m_debug > 0 )		wire_end1.Print();
              
        hitCoords(0)=wire_end1.x();
		hitCoords(1)=wire_end1.y();
		hitCoords(2)=wire_end1.z();
		hitCoords(3)=wire_end2.x();
		hitCoords(4)=wire_end2.y();
		hitCoords(5)=wire_end2.z();

		hitCoords(6)= p_hit->Dist();  //  -> drift radius, misleading name => change!!!!!

		double wireReso = 0.003;
		hitCov.UnitMatrix();         
		hitCov *= wireReso*wireReso; 
		hitCov[6][6]=p_hit->GetSigma()*p_hit->GetSigma(); 

        if ( m_debug > 0 )		cout << "DIST; " << p_hit->Dist() << "  " << p_hit->GetSigma() << endl;
        
        // det_type -> never called in genfit, usefull for the user 
        AbsMeasurement* hit = new WireMeasurement(hitCoords, hitCov, m_detectorID_map["DC"], i, new TrackPoint(fitTrack));

        m_hitCollectionToFit.push_back(hit);
    }
}




void KFitter::Prepare4Test( Track* fitTrack ) {
	vector<string> viewTranslator = { "SIDE", "TOP" };
    int NHits = 12;
    vector<int> v_cell = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
    vector<int> v_plane = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
    // vector<float> v_Rdrift = { 0.4, 1.2, 0.4, 1.2,0.4, 1.2,0.4, 1.2,0.4, 1.2,0.4, 1.2 };
    float reso = 0.04;
    float wireReso = 0.02;

    TMatrixDSym hitCov(7); 
	TVectorD hitCoords(7);

    for (int i = 0; i < NHits; i++) {
        
        int view = ( i%2 == 0 ? 0 : 1 );

        TVector3 wire_end1, wire_end2;        
        m_DC_geo->GetWireEndPoints( v_cell[i], v_plane[i], viewTranslator[view], wire_end1, wire_end2 );
        
        hitCoords(0)=wire_end1.x();		hitCoords(1)=wire_end1.y();		hitCoords(2)=wire_end1.z();
		hitCoords(3)=wire_end2.x();		hitCoords(4)=wire_end2.y();		hitCoords(5)=wire_end2.z();
		// hitCoords(6)= v_Rdrift[i];
		hitCoords(6)= 0.4;

		
		hitCov.UnitMatrix();         
		hitCov *= wireReso*wireReso; //ed errore su rdrift (da grafico da parcon), manca studio sulla correlazione tra le componenti!!
		hitCov[6][6]=reso * reso; 

        AbsMeasurement* hit = new WireMeasurement(hitCoords, hitCov, m_detectorID_map["DC"], i, new TrackPoint(fitTrack));

        m_hitCollectionToFit.push_back(hit);
    }
}






int KFitter::MakeFit( long evNum, string option ) {

	if ( m_debug > 0 )		cout << "Starting MakeFit " << endl;
	// particle pdg code
	// const int pdg = 2212;	//proton
	// const int pdg = 211;	//pion
	// double sign = 1; //Charged particles 
	int isConverged = 0;

	// start values for the fit, e.g. from pattern recognition (here random)
	TVector3 pos(0, 0, 0);	// global   [cm]
	TVector3 mom(0, 0, 6);	// GeV
	
	TDatabasePDG* pdg_base = new TDatabasePDG();
	pdg_base->AddParticle("Lithium", "Lithium", 6, true, 0., 9., "ion", 66666601);

	// pdg_base->AddParticle("Carbonio", "Carbonio", 11.1749, true, 0., 18., "ion", 6661212);
	pdg_base->AddParticle("Carbonio", "Carbonio", 10.254, true, 0., 18., "ion", 66666602);
	// pdg_base->AddParticle("Carbonio", "Carbonio", 12, true, 0., 18., "ion", 6661212);
	
	// trackrep for creating measurements and the state on a plane
	AbsTrackRep* rep = new RKTrackRep(66666602);
	// AbsTrackRep* rep = new RKTrackRep(sign*pdg);

	Track*  fitTrack = new Track(rep, pos, mom);

    PrepareData4Fit( fitTrack );

    // check the hit vector not empty
	if ( m_hitCollectionToFit.size() <= 0 )	{	
		delete fitTrack;
		delete pdg_base;	// !!! occupa uno strafottio di memoria!!!!! Molto di piu di Track!!!
		return 2;
	}

	if ( m_debug > 0 )		cout << "MakeFit::m_hitCollectionToFit.size  =  " << m_hitCollectionToFit.size() << endl;
	
	// insert points to be fitted
	for (unsigned int i=0; i<m_hitCollectionToFit.size(); i++ ) {
		
		// fitTrack->insertPoint(new genfit::TrackPoint(m_hitCollectionToFit[i], fitTrack));
		fitTrack->insertMeasurement(m_hitCollectionToFit[i]);
		fitTrack->checkConsistency();
		if ( m_debug > 3 )		fitTrack->Print("C");
	}

	if ( m_reverse )		fitTrack->reverseTrackPoints();

	//check
	fitTrack->checkConsistency();
	if ( m_debug > 3 )		fitTrack->Print();

	// Check detector!!!!!!!!
	if ( !m_reverse ) {
		pos = m_VT_hitCollection.at( m_hitCollectionToFit.at(0)->getHitId() )->GetMCPosition();
		m_VT_geo->Local2Global( &pos );
		mom = m_VT_hitCollection.at( m_hitCollectionToFit.at(0)->getHitId() )->GetMCMomentum();
		m_VT_geo->Local2Global_RotationOnly( &mom );
	}
	else {
		pos = m_DC_hitCollection.at( m_hitCollectionToFit.at(m_hitCollectionToFit.size()-1)->getHitId() )->Position();
		m_DC_geo->Local2Global( &pos );
		mom = m_DC_hitCollection.at( m_hitCollectionToFit.at(m_hitCollectionToFit.size()-1)->getHitId() )->Momentum();
		m_DC_geo->Local2Global_RotationOnly( &mom );
	}
	

	if ( m_debug > 1 )		cout << "pos mom "<< endl, pos.Print(), mom.Print();
	fitTrack->setStateSeed(pos, mom);

	// THE REAL FIT
	if ( GlobalPar::GetPar()->KalMode() == 1 )
		m_fitter->processTrack(fitTrack);
	else if ( GlobalPar::GetPar()->KalMode() == 2 )
		m_refFitter->processTrack(fitTrack);
	else if ( GlobalPar::GetPar()->KalMode() == 3 )
		m_dafRefFitter->processTrack(fitTrack);
	else if ( GlobalPar::GetPar()->KalMode() == 4 )
		m_dafSimpleFitter->processTrack(fitTrack);


	if ( m_debug > 0 )		cout << "Fitted " << endl;
	if ( fitTrack->getFitStatus(rep)->isFitConverged() )	isConverged = 1;	// convergence check
	if ( m_debug > 3 )		fitTrack->Print("C");

	m_nTotTracks++;
	// m_fitTrackCollection->push_back( new GlobalTrackFoot( *fitTrack, m_nTotTracks, evNum ) );

	if (isConverged) {	
		m_nConvergedTracks++;
		RecordTrackInfo( fitTrack );
	}

	// delete make by track class
	m_VT_hitCollection.clear();
	m_IT_hitCollection.clear();
	m_DC_hitCollection.clear();
	delete fitTrack;	// include un delete rep pare
	delete pdg_base;	// very very big object!!!!!!!!!!
	m_hitCollectionToFit.clear();
	if ( m_debug > 1 )		cout << "Ready for the next track fit!" << endl;

	return isConverged;
}





void KFitter::RecordTrackInfo( Track* track ) {

	TVector3 expectedPos;
	TVector3 expectedMom;
	TVector3 kalmanMom, kalmanPos;
	TVector3 kalmanMom_err;

	// loop over tracks
	for ( unsigned int i =0; i<m_hitCollectionToFit.size(); i++ ) {

		int x = i;	// track index, same in case of forward and reverse
		if ( m_reverse ) 	x = m_hitCollectionToFit.size() - 1 - i;	

		// check if making tests with only some subdetectors on
		//////////			CAMBIA L'USO DELLE COORDINATE LOCALI QUI!!!!!!!!!!!!!!!!!!!!!   /////////////////////////////
		// Check detector!!!!!!!!
		TVector3 tmpPos, tmpMom;
		if ( x < (int)m_VT_hitCollection.size() ) {
			tmpPos = m_VT_hitCollection.at(	m_hitCollectionToFit.at(x)->getHitId() )->GetMCPosition();
			m_VT_geo->Local2Global( &tmpPos );
			tmpMom = m_VT_hitCollection.at(	m_hitCollectionToFit.at(x)->getHitId() )->GetMCMomentum();
			m_VT_geo->Local2Global_RotationOnly( &tmpMom );
		}
		else if ( x < (int)(m_IT_hitCollection.size()+m_VT_hitCollection.size()) ) {
			tmpPos = m_IT_hitCollection.at(	m_hitCollectionToFit.at(x)->getHitId() )->GetMCPosition();
			m_IT_geo->Local2Global( &tmpPos );
			tmpMom = m_IT_hitCollection.at(	m_hitCollectionToFit.at(x)->getHitId() )->GetMCMomentum();
			m_IT_geo->Local2Global_RotationOnly( &tmpMom );
		}
		else {	// if ( m_DC_hitCollection.size() ) {
			tmpPos = m_DC_hitCollection.at( m_hitCollectionToFit.at(x)->getHitId() )->Position();
			m_DC_geo->Local2Global( &tmpPos );
			tmpMom =  m_DC_hitCollection.at( m_hitCollectionToFit.at(x)->getHitId() )->Momentum();
			m_DC_geo->Local2Global_RotationOnly( &tmpMom );
		}


		// Get track kinematics and errors
		TVector3 KalmanPos = TVector3( (track->getFittedState(i).get6DState())[0],	(track->getFittedState(i).get6DState())[1],	(track->getFittedState(i).get6DState())[2] );
		TVector3 KalmanMom = TVector3( (track->getFittedState(i).get6DState())[3], (track->getFittedState(i).get6DState())[4],	(track->getFittedState(i).get6DState())[5] );
		TVector3 KalmanPos_err = TVector3( (track->getFittedState(i).get6DCov())[0][0], (track->getFittedState(i).get6DCov())[1][1], (track->getFittedState(i).get6DCov())[2][2] );
		TVector3 KalmanMom_err = TVector3( (track->getFittedState(i).get6DCov())[3][3],	(track->getFittedState(i).get6DCov())[4][4], (track->getFittedState(i).get6DCov())[5][5] );

		if ( m_debug > 0 )	{
			cout <<endl<< "Single Event Debug" << endl;
			cout << "Hit num = " << i << endl;
			cout << "\t MCPos =      "; tmpPos.Print();
			cout << "\t Kalman Pos da State6D = "; KalmanPos.Print();
			cout << "\t Kalman Pos Error da State6D = "; KalmanPos_err.Print();
			cout <<endl<< "\t MCMom =      "; tmpMom.Print();
			cout << "\t Kalman Mom da State6D = "; KalmanMom.Print();
			cout << "\t Kalman Mom Error da State6D = "; KalmanMom_err.Print();
		}

		if ( i==0 ) {
			expectedPos = tmpPos;
			expectedMom = tmpMom;
			kalmanMom = KalmanMom;
			kalmanMom_err = KalmanMom_err;
			kalmanPos = KalmanPos;
		}

	}
	if ( m_debug > 0 )		cout << "\t\tChi2 = "<< track->getFitStatus(track->getCardinalRep())->getChi2()<< endl;

	// fill histo with tracking info 
	h_chi2->Fill( track->getFitStatus(track->getCardinalRep())->getChi2() );
	h_momentumMC->Fill( expectedMom.Mag() );
	h_momentumKal->Fill( kalmanMom.Mag() );
	h_mass->Fill( m_DC_hitCollection.at(0)->m_genPartMass );	// cambia, possibile problema!!!!!!!!!!!!!!
	h_polarAngol->Fill( kalmanMom.Theta() *180 / M_PI );		// convert in radiant

	PrintPositionResidual( kalmanPos, expectedPos );
	PrintMomentumResidual( kalmanMom, expectedMom, kalmanMom_err );

}









void KFitter::PrintEfficiency() {

	TCanvas* mirror = new TCanvas("TrackEfficiencyPlot", "TrackEfficiencyPlot", 700, 700);
	TH1F* h_trackEfficiency = new TH1F( "TrackEfficiency", "TrackEfficiency", 1, 0, 1 );
	
	float eff = (float)m_nConvergedTracks / m_nTotTracks;
	if ( m_debug > 1 )		cout << "Efficiency = " << eff << "  " << m_nConvergedTracks<< " " << m_nTotTracks<< endl;
	h_trackEfficiency->SetBinContent(1, eff);
	h_trackEfficiency->GetXaxis()->SetBinLabel(1,"Litium");
	h_trackEfficiency->GetYaxis()->SetTitle("Reco Efficiency");

	h_trackEfficiency->Draw();
	mirror->SaveAs("TrackEfficiencyPlot.png");
}



void KFitter::PrintPositionResidual( TVector3 pos, TVector3 expectedPos ) {
	double dR = pos.DeltaR( expectedPos );
	h_posRes->Fill(dR);
}


void KFitter::PrintMomentumResidual( TVector3 pos, TVector3 expectedPos, TVector3 cov ) {

	double dP = pos.Mag() - expectedPos.Mag();
	double err = EvalError( pos, cov );

	if ( m_debug > 1 )		cout << "dp= " <<pos.Mag() << "-"<<expectedPos.Mag() << "   err= " << err<< endl;
	if ( m_debug > 1 )		cout << " residuo= "<< dP / err <<endl;

	h_deltaP->Fill( dP );
	// h_sigmaP->Fill(err);
	h_momentumRes->Fill( dP /err);
	
}



double KFitter::EvalError( TVector3 mom, TVector3 err ) {

	double dx2 = 2 * sqrt(err.x()) * mom.x();
	double dy2 = 2 * sqrt(err.y()) * mom.y();
	double dz2 = 2 * sqrt(err.z()) * mom.z();

	double p = mom.Mag();
	double dp2 = dx2 + dy2 + dz2;
	double dp = dp2 / ( 2 * p );

	return dp;
}






void KFitter::Save( ) {

	TCanvas* mirror = new TCanvas("TrackChi2Plot", "TrackChi2Plot", 700, 700);
	h_chi2->GetXaxis()->SetTitle("Chi2");

	h_chi2->Draw();
	mirror->SaveAs("TrackChi2Plot.png");

	// position residual
	h_posRes->GetXaxis()->SetTitle("dR");
	h_posRes->Draw();
	mirror->SaveAs("PositionRes.png");

	// momentum residual
	h_momentumRes->GetXaxis()->SetTitle("dp/sigma(p)");
	h_momentumRes->Draw();
	mirror->SaveAs("MomentumRes.png");

	// momentum residual
	h_deltaP->GetXaxis()->SetTitle("dp");
	h_deltaP->Draw();
	mirror->SaveAs("deltaP.png");

	// momentum residual
	h_mass->GetXaxis()->SetTitle("mass(u)");
	h_mass->Draw();
	mirror->SaveAs("mass.png");

	// momentum residual
	h_polarAngol->GetXaxis()->SetTitle("polarAngol(deg)");
	h_polarAngol->Draw();
	mirror->SaveAs("polarAngol.png");

	// momentum distribution comparison
	h_momentumMC->GetXaxis()->SetTitle("p [GeV]");
	h_momentumMC->SetLineColor(kGreen+1);
	h_momentumMC->SetLineWidth(2);
	h_momentumKal->SetLineColor(kOrange+8);
	h_momentumKal->SetLineWidth(2);
	h_momentumMC->Draw();
	h_momentumKal->Draw("same");
	mirror->SaveAs("Momentum.png");

}



























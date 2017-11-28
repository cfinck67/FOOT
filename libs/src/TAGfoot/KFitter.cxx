

#include "KFitter.hxx"







//----------------------------------------------------------------------------------------------------
KFitter::KFitter ( int nIter, double dPVal ) {
	
	gROOT->SetStyle("Plain");
	gStyle->SetFrameBorderMode(0);
	m_debug = GlobalPar::GetPar()->Debug();

	// test variable of control
	m_reverse = GlobalPar::GetPar()->IsKalReverse();

	m_diceRoll = new TRandom3();
    m_diceRoll->SetSeed(0);

	m_VT_hitCollection.clear();
	m_IT_hitCollection.clear();
	m_MSD_hitCollection.clear();

	vector<string> tmp_detName = { "STC", "BM", "TG", "VT", "IT", "MSD" };
	for (unsigned int i=0; i<tmp_detName.size(); i++)
		m_detectorID_map[ tmp_detName[i] ] = i;

	if ( !(GlobalPar::GetPar()->KalSystems().size() == 1 && GlobalPar::GetPar()->KalSystems().at(0) == "all") )	 {
		for (unsigned int i=0; i<GlobalPar::GetPar()->KalSystems().size(); i++ ) {
			if ( m_detectorID_map.find( GlobalPar::GetPar()->KalSystems().at(i) ) == m_detectorID_map.end() ) 
				cout<< "ERROR::KFitter::KFitter  --> KalSystems parameter not set properly, check befor continue."<< endl, exit(0);
		}
	}

	m_systemsON = "";
	for (unsigned int i=0; i<GlobalPar::GetPar()->KalSystems().size(); i++ ) {
		if (i != 0)		m_systemsON += " ";
		m_systemsON += GlobalPar::GetPar()->KalSystems().at(i);
	}
	if (m_debug > 0)	cout << "Detector systems for Kalman:  " << m_systemsON << endl;

	MakePdgDatabase();

	if ( (string)getenv("FOOTRES") != "" ) {
		system(("rm -r "+(string)getenv("FOOTRES")).c_str());
		system(("mkdir "+(string)getenv("FOOTRES")).c_str());
	}

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

	m_resoP_step = 0.2;
	
	m_vecHistoColor = { kBlack, kRed-9, kRed+1, kRed-2, kOrange+7, kOrange, kOrange+3, kGreen+1, 
						kGreen+3, kBlue+1, kBlue+3, kAzure+8, kAzure+1, kMagenta+2, 
						kMagenta+3, kViolet+1, kViolet+6, kViolet-4 };

    cout << "GlobalPar::GetPar()->MCParticles()";
	for (unsigned int i=0; i<GlobalPar::GetPar()->MCParticles().size(); i++ ) {
		cout << "   " << GlobalPar::GetPar()->MCParticles().at(i);
	}
	cout << endl;

}





//----------------------------------------------------------------------------------------------------
// create the fragment database
void KFitter::MakePdgDatabase() {

	// clean the particle datatbase. Important!
	TDatabasePDG::Instance()->~TDatabasePDG();

	int nNewParticles = 17;
	int pdgCode = 66666600;
	vector<string> nameV 		 = { 	"C10", "C11", "C12", 
										"Li6", "Li7",
										"B7", "B8", "B9",
										"Be9", "Be10", "Be11",
										"N12", "N13", "N14",
										"Alpha", "H",
										"O15" };
	if ( (int)nameV.size() != nNewParticles ) 	cout << "ERROR::KFitter::MakePdgDatabase  -->  particle collection name size not match "<< nameV.size() <<endl;

	vector<double> massV = { 	10.254, 11.1749, 12.1095, 
										6.53383, 7,
										7, 8, 9.3255,
										9.32444, 10.2525, 11,
										12.1112, 13, 14,
										4, 1,
										15 };
	if ( (int)massV.size() != nNewParticles ) 	cout << "ERROR::KFitter::MakePdgDatabase  -->  particle collection mass size not match "<< massV.size() <<endl;

	vector<double> chargeV = { 	18, 18, 18, 
										9, 9,
										12, 12, 12,
										15, 15, 15,
										21, 21, 21,
										6, 3,
										24  };
	if ( (int)chargeV.size() != nNewParticles ) 	cout << "ERROR::KFitter::MakePdgDatabase  -->  particle collection charge size not match "<< chargeV.size() <<endl;


	// check that every particle defined in the parameter file is defined in nameV
	for ( unsigned int i=0; i<GlobalPar::GetPar()->MCParticles().size(); i++) {
		if ( find( nameV.begin(), nameV.end(), GlobalPar::GetPar()->MCParticles()[i] ) == nameV.end() ) {
			cout << "ERROR::KFitter::MakePdgDatabase()  -->  required " << GlobalPar::GetPar()->MCParticles()[i] << " particle from input parameter not defined" << endl;
			exit(0);
		}
	}

	// add the new particles to the standard TDatabasePDG
	for ( int i=0; i<nNewParticles; i++) {
		TDatabasePDG::Instance()->AddParticle(nameV[i].c_str(), nameV[i].c_str(), massV[i], true, 0., chargeV[i], "ion", ++pdgCode);	
		m_pdgCodeMap[ nameV[i] ] = pdgCode;
	}
}




//----------------------------------------------------------------------------------------------------
// upload measurement points from vertex pixel
int KFitter::UploadHitsVT( TAGdataDsc* footDataObj, shared_ptr<TAVTparGeo> vt_geo ) {
	
	m_VT_geo = vt_geo;
	TAVTntuRaw* ntup = (TAVTntuRaw*) footDataObj->GenerateObject();
	if ( m_debug > 0 )		cout << "N vertex pixel read: " << ntup->GetPixelsN(0) << endl;
	
	// save pixels in the collection
	for (int i = 0; i < ntup->GetPixelsN(0); i++) 
        m_VT_hitCollection.push_back( ntup->GetPixel(0,i) );

	return ntup->GetPixelsN(0);
}




//----------------------------------------------------------------------------------------------------
// upload measurement points from Inner Tracker pixel
int KFitter::UploadHitsIT( TAGdataDsc* footDataObj, shared_ptr<TAITparGeo> it_geo ) {
	
	m_IT_geo = it_geo;
	TAITntuRaw* ntup = (TAITntuRaw*) footDataObj->GenerateObject();
	if ( m_debug > 0 )		cout << "N inner pixel read: " << ntup->GetPixelsN(0) << endl;

	// save pixels in the collection
	for (int i = 0; i < ntup->GetPixelsN(0); i++) 
        m_IT_hitCollection.push_back( ntup->GetPixel(0,i) );

	return ntup->GetPixelsN(0);
}



//----------------------------------------------------------------------------------------------------
// upload measurement points from Strip
int KFitter::UploadHitsMSD( TAGdataDsc* footDataObj, shared_ptr<TAMSDparGeo> msd_geo ) {
	
	m_MSD_geo = msd_geo;
	TAMSDntuRaw* ntup = (TAMSDntuRaw*) footDataObj->GenerateObject();

	if ( m_debug > 0 )		cout << "Strip detector hits read: " << ntup->GetPixelsN(0) << endl;

	// save strips hits in the collection
	for (int i = 0; i < ntup->GetPixelsN(0); i++) 
        m_MSD_hitCollection.push_back( ntup->GetPixel(0,i) );

	return ntup->GetPixelsN(0);
}





//----------------------------------------------------------------------------------------------------
// pack together the hits to be fitted, from all the detectors, selct different preselecion m_systemsONs
int KFitter::PrepareData4Fit( Track* fitTrack ) {

	
	// Vertex -  fill fitter collections
	if ( m_systemsON == "all" ||  m_systemsON.find( "VT" ) != string::npos ) {

		if ( m_debug > 0 )		cout << endl<<endl << "Filling vertex hit collection  = " << m_VT_hitCollection.size() << endl;
		Prepare4Vertex(fitTrack);
	}
	// Inner Tracker -  fill fitter collections
	if ( m_systemsON == "all" || m_systemsON.find( "IT" ) != string::npos) {
		
		if ( m_debug > 0 )		cout <<endl<<endl << "Filling inner detector hit collection = " << m_IT_hitCollection.size() << endl;
		Prepare4InnerTracker(fitTrack);    
	}
	// MSD -  fill fitter collections
	if ( m_systemsON == "all" || m_systemsON.find( "MSD" ) != string::npos ) {

		if ( m_debug > 0 )		cout << endl<<endl << "Filling Strip hit collection = " << m_MSD_hitCollection.size() << endl;		
		Prepare4Strip(fitTrack);
	}

	// loop over all the hit-collections to be fit
	for ( map< string, vector<AbsMeasurement*> >::iterator it=m_hitCollectionToFit.begin(); it != m_hitCollectionToFit.end(); it++ ) {
		// check the pre-fit requirements
		if ( !PrefitRequirements( it ) ) {
			// if requirements are FALSE -> delete each AbsMeasurement objects and clear the map element
			for ( vector<AbsMeasurement*>::iterator it2=(*it).second.begin(); it2 != (*it).second.end(); it2++ ) {
				delete (*it2);
				// delete (*it).second.at(i);	// wrong!
			}
			m_hitCollectionToFit.erase(it);
		}
	}
	//	if no map element survive -> clear the single-detector hit-collections
	if ( m_hitCollectionToFit.size() == 0 ) {
		m_VT_hitCollection.clear();
		m_IT_hitCollection.clear();
		m_MSD_hitCollection.clear();
		return 0;
	}	

	return m_hitCollectionToFit.size();
}






//----------------------------------------------------------------------------------------------------
// pre-fit requirements to be applied to EACH of the hitCollections
bool KFitter::PrefitRequirements( map< string, vector<AbsMeasurement*> >::iterator element ) {

	int testHitNumberLimit = 0;
	int testHit_VT = 0;
	int testHit_IT = 0;
	int testHit_MSD = 0;
	// define the number of hits per each detector to consider to satisfy the pre-fit requirements
	if ( m_systemsON == "all" ) {
		testHit_VT = m_VT_geo->GetNLayers(), testHit_IT = m_IT_geo->GetNLayers(), testHit_MSD = m_MSD_geo->GetNLayers();
	}
	else {
		if ( m_systemsON.find( "VT" ) != string::npos )			testHit_VT = m_VT_geo->GetNLayers();
		if ( m_systemsON.find( "IT" ) != string::npos )			testHit_IT = m_IT_geo->GetNLayers();
		if ( m_systemsON.find( "MSD" ) != string::npos )		testHit_MSD = m_MSD_geo->GetNLayers();
	}
	// num of total hits
	testHitNumberLimit = testHit_VT + testHit_IT + testHit_MSD;
	if ( testHitNumberLimit == 0 ) 			cout << "ERROR >> KFitter::PrefitRequirements :: m_systemsON mode is wrong!!!" << endl, exit(0);

	// test the total number of hits ->  speed up the test
	if ( (int)((*element).second.size()) != testHitNumberLimit ) {
		if ( m_debug > 0 )		cout << "WARNING :: KFitter::PrefitRequirements  -->  number of elements different wrt the expected ones : Nel=" << (int)((*element).second.size()) << "   Nexp= " << testHitNumberLimit << endl;
		return false;
	}
 
 	int nHitVT = 0;
	int nHitIT = 0;
	int nHitMSD = 0;
	// count the hits per each detector
	for ( vector<AbsMeasurement*>::iterator it=(*element).second.begin(); it != (*element).second.end(); it++ ) {
		if ( (*it)->getDetId() == m_detectorID_map["VT"] )	nHitVT++;
		if ( (*it)->getDetId() == m_detectorID_map["IT"] )	nHitIT++;
		if ( (*it)->getDetId() == m_detectorID_map["MSD"] )	nHitMSD++;
		if ( m_debug > 2 )		cout << "nHitVT  " << nHitVT << " nHitIT" << nHitIT << " nHitMSD "<< nHitMSD<<endl;
	}
	// test the num of hits per each detector
	if ( nHitVT != testHit_VT || nHitIT != testHit_IT || nHitMSD != testHit_MSD )	return false;

	return true;
}







//----------------------------------------------------------------------------------------------------
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
		
		double pixReso = 0.002*1.5;
		hitCov.UnitMatrix();         
		hitCov *= pixReso*pixReso; 
		double zErr = 0.0025;
		hitCov[2][2] = zErr*zErr; 

        // AbsMeasurement* hit = new SpacepointMeasurement(hitCoords, hitCov, m_detectorID_map["VT"], i, new TrackPoint(fitTrack));
        AbsMeasurement* hit = new SpacepointMeasurement(hitCoords, hitCov, m_detectorID_map["VT"], i, nullptr );
        
         // try to categorise the particle that generated the hit. If it fails --> clean the hit object
        string category = CategoriseHitsToFit_withTrueInfo( p_hit->m_genPartFLUKAid, p_hit->m_genPartCharge, p_hit->m_genPartMass );
        if (category == "fail")	{
        	delete hit;
        	continue;
        }
        // cout << category << "   " << p_hit->m_genPartMass << endl;
        m_hitCollectionToFit[ category ].push_back(hit);
    }
}



//----------------------------------------------------------------------------------------------------
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

		double pixReso = 0.002*1.5;
		hitCov.UnitMatrix();         
		hitCov *= pixReso*pixReso; 
		double zErr = 0.0025;
		hitCov[2][2] = zErr*zErr; 

        AbsMeasurement* hit = new SpacepointMeasurement(hitCoords, hitCov, m_detectorID_map["IT"], i, nullptr );

         // try to categorise the particle that generated the hit. If it fails --> clean the hit object
        string category = CategoriseHitsToFit_withTrueInfo( p_hit->m_genPartFLUKAid, p_hit->m_genPartCharge, p_hit->m_genPartMass );
        if (category == "fail")	{
        	delete hit;
        	continue;
        }
        m_hitCollectionToFit[ category ].push_back(hit);
    }
}



//----------------------------------------------------------------------------------------------------
void KFitter::Prepare4Strip( Track* fitTrack ) {
	
	TMatrixDSym hitCov(3); 
	TVectorD hitCoords(3);
	 
	vector<TAMSDntuHit*> allStripSignals_x; 
	vector<TAMSDntuHit*> allStripSignals_y; 
    
    for (unsigned int i = 0; i < m_MSD_hitCollection.size(); i++) {
        
        TAMSDntuHit* p_hit = m_MSD_hitCollection.at(i);

        if ( m_debug > 0 )		cout << "Hit " << i << endl;
        if ( m_debug > 0 )		cout << "\tMSD test = Layer:" << p_hit->GetLayer() <<" view:"<< p_hit->GetPixelView() <<
        								" strip:"<< p_hit->GetPixelStrip() << "  partType: " << p_hit->m_genPartFLUKAid << 
        								"  genID= " << p_hit->m_genPartID << endl;
        // if ( m_debug > 0 )		hitPos.Print();
        
        if ( p_hit->GetPixelView() == 0 )        allStripSignals_x.push_back(p_hit);
        if ( p_hit->GetPixelView() == 1 )        allStripSignals_y.push_back(p_hit);

    }

	if ( m_debug > 0 )		cout << "x=  " << allStripSignals_x.size() << "  y= " << allStripSignals_y.size() << endl;
    
    int countStripHits = 0;
    m_MSD_posVectorSmearedHit.clear();
    m_MSD_momVectorSmearedHit.clear();
    m_MSD_mass.clear();
    for ( vector<TAMSDntuHit*>::iterator xIt=allStripSignals_x.begin(); xIt != allStripSignals_x.end(); xIt++ ) {
	    for ( vector<TAMSDntuHit*>::iterator yIt=allStripSignals_y.begin(); yIt != allStripSignals_y.end(); yIt++ ) {
	
	    	if ( (*xIt)->GetLayer() != (*yIt)->GetLayer() )		continue;

	    	// combine strip signals ONLY if coming from th same fragment
	    	if ( (*xIt)->m_genPartID != (*yIt)->m_genPartID )		continue;


	        TVector3 x_hitPos = m_MSD_geo->GetPosition( (*xIt)->GetLayer(), (*xIt)->GetPixelView(), (*xIt)->GetPixelStrip() );
	        TVector3 y_hitPos = m_MSD_geo->GetPosition( (*yIt)->GetLayer(), (*yIt)->GetPixelView(), (*yIt)->GetPixelStrip() );
	        
	        TVector3 geo_hitPos = TVector3 ( x_hitPos.y(), y_hitPos.y(), 0.5*(x_hitPos.z() + y_hitPos.z()) );
	        
	        if ( m_debug > 0 )		cout << "Hit strip " << countStripHits << endl;
	        if ( m_debug > 0 )		cout << "\tMSD hit = Layer:" << (*xIt)->GetLayer() << " xStrip:" << (*xIt)->GetPixelStrip() <<" yStrip:"<<  (*yIt)->GetPixelStrip() << endl;
	        
	        if ( m_debug > 0 )		cout << "\t", geo_hitPos.Print();
			
			double stripReso = 0.0035;
			hitCov.UnitMatrix();         
			hitCov *= stripReso*stripReso; 
			double zErr = 0.0025;
			hitCov[2][2] = zErr*zErr; 

			double simulatedStripHit_X = (*xIt)->GetMCPosition_Global().X() + m_diceRoll->Gaus(0.,1.) * stripReso;
			double simulatedStripHit_Y = (*yIt)->GetMCPosition_Global().Y() + m_diceRoll->Gaus(0.,1.) * stripReso;
			double simulatedStripHit_Z = m_MSD_geo->GetLayerCenter( (*xIt)->GetLayer() ).Z() + m_diceRoll->Gaus(0.,1.) * zErr;

			TVector3 hitPos = TVector3 ( simulatedStripHit_X, simulatedStripHit_Y, simulatedStripHit_Z );
			if ( m_debug > 0 )		cout << "\tSimulated hits coosrdinate using smearing: \t\t ";
			if ( m_debug > 0 )		hitPos.Print();

	        hitCoords(0)=hitPos.x();
			hitCoords(1)=hitPos.y();
			hitCoords(2)=hitPos.z();

			// MC info
			TVector3 hitPosMC( (*xIt)->GetMCPosition_Global().X(), (*yIt)->GetMCPosition_Global().Y(), m_MSD_geo->GetLayerCenter( (*xIt)->GetLayer() ).Z() );
			m_MSD_posVectorSmearedHit.push_back( hitPosMC );
			TVector3 hitMomMC( (*xIt)->GetMCMomentum_Global().X(), (*yIt)->GetMCMomentum_Global().Y(), (*xIt)->GetMCMomentum_Global().Z() );
			m_MSD_momVectorSmearedHit.push_back( hitMomMC );
			m_MSD_mass.push_back( (*xIt)->m_genPartMass );

	        // AbsMeasurement* hit = new SpacepointMeasurement(hitCoords, hitCov, m_detectorID_map["MSD"], i, new TrackPoint(fitTrack));
	        AbsMeasurement* hit = new SpacepointMeasurement(hitCoords, hitCov, m_detectorID_map["MSD"], countStripHits, nullptr );

	        // count the combined hits
			countStripHits++;
	        
	         // try to categorise the particle that generated the hit. If it fails --> clean the hit object
	        string category = CategoriseHitsToFit_withTrueInfo( (*xIt)->m_genPartFLUKAid, (*xIt)->m_genPartCharge, (*xIt)->m_genPartMass );
	        if (category == "fail")	{
	        	delete hit;
	        	continue;
	        }
	        // cout << category << "   " << p_hit->m_genPartMass << endl;
	        m_hitCollectionToFit[ category ].push_back(hit);

	    }
    }

}


//----------------------------------------------------------------------------------------------------
int KFitter::MakeFit( long evNum ) {

	if ( m_debug > 0 )		cout << "Starting MakeFit " << endl;
	int isConverged = 0;
	
	// start values for the fit, change below
	TVector3 pos(0, 0, 0);	// global   [cm]
	TVector3 mom(0, 0, 6);	// GeV
	

	// loop here for different particle hipothesis fit

	Track*  fitTrack = new Track();

    PrepareData4Fit( fitTrack );

    // check the hit vector not empty otherwise clear
	if ( m_hitCollectionToFit.size() <= 0 )	{	
		m_VT_hitCollection.clear();
		m_IT_hitCollection.clear();
		m_MSD_hitCollection.clear();
		delete fitTrack;
		return 2;
	}

	if ( m_debug > 0 )		cout << "MakeFit::m_hitCollectionToFit.size  =  " << m_hitCollectionToFit.size() << endl;
	
	
	// loop over all hit category
	for ( map< string, vector<AbsMeasurement*> >::iterator hitSample=m_hitCollectionToFit.begin(); hitSample != m_hitCollectionToFit.end(); hitSample++ ) {

		// check if the cathegory is defined in m_pdgCodeMap
		if ( m_pdgCodeMap.find( (*hitSample).first ) == m_pdgCodeMap.end() ) 
			cout << "ERROR :: KFitter::MakeFit  -->	 in m_pdgCodeMap not found the cathegory " << (*hitSample).first << endl;

		// SET PARTICLE HYPOTHESIS  --> set repository
		AbsTrackRep* rep = new RKTrackRep( m_pdgCodeMap[ (*hitSample).first ] );
		fitTrack->addTrackRep( rep );

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//	si puo aggiungere piu di una repository e lui le prova tutte
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


		// SET SEED
		int firstHitToProcess = -666;
		if ( !m_reverse ) 		firstHitToProcess = 0;
		else 					firstHitToProcess = (*hitSample).second.size()-1;

		int detID = (*hitSample).second.at( firstHitToProcess )->getDetId();
		int hitID = (*hitSample).second.at( firstHitToProcess )->getHitId();
		if ( detID == m_detectorID_map["VT"] ) {
			pos = m_VT_hitCollection.at( hitID )->GetMCPosition_Global();
			mom = m_VT_hitCollection.at( hitID )->GetMCMomentum_Global();
		}
		else if ( detID == m_detectorID_map["IT"] ) {
			pos = m_IT_hitCollection.at( hitID )->GetMCPosition_Global();
			mom = m_IT_hitCollection.at( hitID )->GetMCMomentum_Global();
		}
		else if ( detID == m_detectorID_map["MSD"] ) {
			pos = m_MSD_posVectorSmearedHit.at( hitID );
			mom = m_MSD_momVectorSmearedHit.at( hitID );
		}
		if ( m_debug > 2 )		cout << "pos mom "<< endl, pos.Print(), mom.Print();
		
		// set seed
		fitTrack->setStateSeed(pos, mom);		

		// insert points to be fitted  -   loop over each measurement in the current collection
		for ( unsigned int i=0; i < (*hitSample).second.size(); i++ )	{
			// fitTrack->insertPoint(new genfit::TrackPoint( (*hitSample).second.at(i), fitTrack ) );	// differenza con insertMeasurement???
			fitTrack->insertMeasurement( (*hitSample).second.at(i) );
			fitTrack->checkConsistency();
			if ( m_debug > 3 )		fitTrack->Print("C");
		}

		if ( m_reverse )		fitTrack->reverseTrackPoints();

		//check
		fitTrack->checkConsistency();
		if ( m_debug > 3 )		fitTrack->Print();


//pre-fit
		// try{
	 //        if ( m_debug > 0 ) 		cout<<"Starting the fitter"<<endl;

	 //        // if (prefit) {
	 //          genfit::KalmanFitter prefitter(1, dPVal);
	 //          prefitter.setMultipleMeasurementHandling(genfit::weightedClosestToPrediction);
	 //          prefitter.processTrackWithRep(fitTrack, fitTrack->getCardinalRep());
	 //        // }

	 //        fitter->processTrack(fitTrack, false);

	 //        if ( m_debug > 0 ) cout<<"fitter is finished!"<<endl;
	 //      }
	 //      catch(genfit::Exception& e){
	 //        cout << e.what();
	 //        cout << "Exception, next track" << endl;
	 //        continue;
	 //    }


		// THE REAL FIT 	-->		different kalman modes
		try{
			if ( GlobalPar::GetPar()->KalMode() == 1 )
				m_fitter->processTrack(fitTrack);
			else if ( GlobalPar::GetPar()->KalMode() == 2 )
				m_refFitter->processTrack(fitTrack);
			else if ( GlobalPar::GetPar()->KalMode() == 3 )
				m_dafRefFitter->processTrack(fitTrack);
			else if ( GlobalPar::GetPar()->KalMode() == 4 )
				m_dafSimpleFitter->processTrack(fitTrack);
		
			if ( m_debug > 3 )		fitTrack->Print();
			if ( m_debug > 0 )		cout << "Fitted " << fitTrack->getFitStatus(rep)->isFitted() << endl;
			if ( fitTrack->getFitStatus(rep)->isFitConverged() &&  fitTrack->getFitStatus(rep)->isFitted() )	isConverged = 1;	// convergence check
			if ( m_debug > 3 )		fitTrack->Print("C");

			if ( m_nTotTracks.find( (*hitSample).first ) == m_nTotTracks.end() )	m_nTotTracks[ (*hitSample).first ] = 0;
			m_nTotTracks[ (*hitSample).first ]++;
			// m_fitTrackCollection->push_back( new GlobalTrackFoot( *fitTrack, m_nTotTracks, evNum ) );

			if (isConverged) {	
				// if ( fitTrack->getFitStatus(fitTrack->getCardinalRep())->getChi2() <= 1 ) {
					if ( m_nConvergedTracks.find( (*hitSample).first ) == m_nConvergedTracks.end() )	m_nConvergedTracks[ (*hitSample).first ] = 0;
					m_nConvergedTracks[ (*hitSample).first ]++;
					RecordTrackInfo( fitTrack, (*hitSample).first );
				// }
			}
		}
		catch (genfit::Exception& e){
          std::cerr << e.what();
          std::cerr << "Exception, next track" << std::endl;
          continue;
		}

	}

	// clear & delete objects  -  measueremnt delete made by Track class
	
	// no perche' gestito come puntatore esterno 
	// for ( vector<TAVTntuHit*>::iterator it=m_VT_hitCollection.begin(); it != m_VT_hitCollection.end(); it++ ) {
	// 	delete (*it);
	// }
	// for ( vector<TAITntuHit*>::iterator it=m_IT_hitCollection.begin(); it != m_IT_hitCollection.end(); it++ ) {
	// 	delete (*it);
	// }

	m_VT_hitCollection.clear();
	m_IT_hitCollection.clear();
	m_MSD_hitCollection.clear();
	delete fitTrack;	// include un delete rep pare
	for ( map< string, vector<AbsMeasurement*> >::iterator it=m_hitCollectionToFit.begin(); it != m_hitCollectionToFit.end(); it++ ) {
		// for ( vector<AbsMeasurement*>::iterator it2=(*it).second.begin(); it2 != (*it).second.end(); it2++ )
		// 	delete (*it2);	// no perche fatto da altri
		m_hitCollectionToFit.erase(it);
	}
	m_hitCollectionToFit.clear();	
	if ( m_debug > 1 )		cout << "Ready for the next track fit!" << endl;

	return isConverged;
}




//----------------------------------------------------------------------------------------------------
string KFitter::CategoriseHitsToFit_withTrueInfo( int flukaID, int charge, int mass ) {

	string outName = "fail";

	if ( flukaID == -2 && charge == 6 &&  round(mass) == 10 )  outName =  "C10";
	if ( flukaID == -2 && charge == 6 &&  round(mass) == 11 )  outName =  "C11";
	if ( flukaID == -2 && charge == 6 &&  round(mass) == 12 )  outName =  "C12";
	
	if ( flukaID == -2 && charge == 3 &&  round(mass) == 6 )  outName =  "Li6";
	if ( flukaID == -2 && charge == 3 &&  round(mass) == 7 && mass < 7 )  outName =  "Li7";
	
	if ( flukaID == -2 && charge == 4 &&  round(mass) == 7 )  outName =  "B7";
	if ( flukaID == -2 && charge == 4 &&  round(mass) == 8 )  outName =  "B8";
	if ( flukaID == -2 && charge == 4 &&  round(mass) == 9 )  outName =  "B9";

	if ( flukaID == -2 && charge == 5 &&  round(mass) == 9 )  outName =  "Be9";
	if ( flukaID == -2 && charge == 5 &&  round(mass) == 10 )  outName =  "Be10";

	if ( flukaID == -2 && charge == 7 &&  round(mass) == 12 )  outName =  "N12";
	// if ( flukaID == -2 && charge == 7 &&  round(mass) == 13 )  outName =  "N13";
	if ( flukaID == -2 && charge == 7 &&  round(mass) == 14 )  outName =  "N14";

	if ( flukaID == -2 && charge == 8 &&  round(mass) == 15 )  outName =  "O15";

	if ( flukaID == -6 && charge == 2 )  outName =  "Alpha";
	if ( flukaID == 1 && charge == 1 )  outName =  "H";

	if ( !GlobalPar::GetPar()->Find_MCParticle( outName ) )
		return "fail";


	return outName;
}



void KFitter::MatrixToZero( TMatrixD *matrix ) {

	for ( int j=0; j<matrix->GetNrows(); j++ ) {
		for ( int k=0; k<matrix->GetNcols(); k++ ) {
			(*matrix)(j,k) = 0;
		}
	}

}




//----------------------------------------------------------------------------------------------------
void KFitter::RecordTrackInfo( Track* track, string hitSampleName ) {

	TVector3 expectedPos;
	TVector3 expectedMom;
	TVector3 kalmanMom, kalmanPos;
	TVector3 kalmanMom_err;
	double massMC = -666;

	InitAllHistos( hitSampleName );

	TMatrixD covarianceR(3,3); 
	TMatrixD covarianceP(3,3); 
	// loop over tracks
	for ( unsigned int i =0; i<m_hitCollectionToFit[ hitSampleName ].size(); i++ ) {

		int x = i;	// track index, same in case of forward and reverse
		if ( m_reverse ) 	x = m_hitCollectionToFit[ hitSampleName ].size() - 1 - i;	


		TVector3 tmpPos, tmpMom;
		double tmp_mass = -666;
		TVector3 tmp_genPos;

		int detID = m_hitCollectionToFit[ hitSampleName ].at(x)->getDetId();
		int hitID = m_hitCollectionToFit[ hitSampleName ].at(x)->getHitId();
		if ( detID == m_detectorID_map["VT"] ) {
			tmpPos = m_VT_hitCollection.at(	hitID )->GetMCPosition_Global();
			tmpMom = m_VT_hitCollection.at(	hitID )->GetMCMomentum_Global();
			tmp_mass = m_VT_hitCollection.at( hitID )->m_genPartMass;
			tmp_genPos = m_VT_hitCollection.at( hitID )->m_genPartPosition;
		}
		else if ( detID == m_detectorID_map["IT"] ) {
			tmpPos = m_IT_hitCollection.at(	hitID )->GetMCPosition_Global();
			tmpMom = m_IT_hitCollection.at(	hitID )->GetMCMomentum_Global();
			tmp_mass = m_IT_hitCollection.at( hitID )->m_genPartMass;
			tmp_genPos = m_IT_hitCollection.at( hitID )->m_genPartPosition;
		}
		else if ( detID == m_detectorID_map["MSD"] ) {
			tmpPos = m_MSD_posVectorSmearedHit.at( hitID );
			tmpMom =  m_MSD_momVectorSmearedHit.at( hitID );
			tmp_mass = m_MSD_mass.at( hitID );
			tmp_genPos = TVector3(-1, -1, -1);
		}
		// Get track kinematics and errors
		TVector3 KalmanPos = TVector3( (track->getFittedState(i).get6DState())[0],	(track->getFittedState(i).get6DState())[1],	(track->getFittedState(i).get6DState())[2] );
		TVector3 KalmanMom = TVector3( (track->getFittedState(i).get6DState())[3], (track->getFittedState(i).get6DState())[4],	(track->getFittedState(i).get6DState())[5] );
		TVector3 KalmanPos_err = TVector3( (track->getFittedState(i).get6DCov())[0][0], (track->getFittedState(i).get6DCov())[1][1], (track->getFittedState(i).get6DCov())[2][2] );
		TVector3 KalmanMom_err = TVector3( (track->getFittedState(i).get6DCov())[3][3],	(track->getFittedState(i).get6DCov())[4][4], (track->getFittedState(i).get6DCov())[5][5] );


		if ( m_debug > 0 )	{
			cout <<endl<< "Single Event Debug\t--\t" << hitSampleName << endl;
			cout << "Hit num = " << i << "  --  MC mass = " << tmp_mass << endl;
			cout << "\t MCPos =       "; tmpPos.Print();
			cout << "\t Kalman Pos da State6D = "; KalmanPos.Print();
			cout << "\t Kalman Pos Error da State6D = "; KalmanPos_err.Print();
			cout <<endl<< "\t MCMom = "<< tmpMom.Mag() <<"     "; tmpMom.Print();
			cout << "\t Kalman Mom da State6D = "<< KalmanMom.Mag() <<"    "; KalmanMom.Print();
			cout << "\t Kalman Mom Error da State6D = "; KalmanMom_err.Print();
		}
		

		// keep quantities to be plotted of the state CLOSER to the interaction point
		unsigned int measuredState = ( m_reverse ? m_hitCollectionToFit[ hitSampleName ].size()-1 : 0 );
		if ( i == measuredState ) {
			expectedPos = tmpPos;
			expectedMom = tmpMom;
			kalmanMom = KalmanMom;
			kalmanMom_err = KalmanMom_err;
			kalmanPos = KalmanPos;
			massMC = tmp_mass;
		}


		// double dmom = kalmanMom.Mag() - expectedMom.Mag();
		// if ( fabs( dmom ) > 0.2  )	return;
		
		
		if ( i == measuredState  ) {
			MatrixToZero(&covarianceR);
			MatrixToZero(&covarianceP);
			for ( int j=0; j<3; j++ ) {
				for ( int k=0; k<3; k++ ) {
					h_covariance[ hitSampleName ]->Fill( j, k, (track->getFittedState(i).get6DCov())[j+3][k+3] );
					covarianceP(j,k) = (track->getFittedState(i).get6DCov())[j+3][k+3];
					covarianceR(j,k) = (track->getFittedState(i).get6DCov())[j][k];
				}
			}
			

			h_sigmaR[ hitSampleName ]->Fill( EvalError( expectedPos, covarianceR ) );
// h_sigmaR[ hitSampleName ]->Fill( EvalError( expectedPos, KalmanMom_err ) );

			h_mass_genFit[ hitSampleName ]->Fill( track->getFittedState(i).getMass() );
			
			h_zPosGen[ hitSampleName ]->Fill( tmp_genPos.Z() );

			h_charge[ hitSampleName ]->Fill( track->getFitStatus(track->getCardinalRep())->getCharge() );
			// h_chargeSign->Fill( track->getFittedState(i).getMass() );
			h_isFitConvergedFully[ hitSampleName ]->Fill( track->getFitStatus(track->getCardinalRep())->isFitConvergedFully() );
			h_isFitConvergedPartially[ hitSampleName ]->Fill( track->getFitStatus(track->getCardinalRep())->isFitConvergedPartially() );
			h_NFailedPoints[ hitSampleName ]->Fill( track->getFitStatus(track->getCardinalRep())->getNFailedPoints() );
			h_isTrackPruned[ hitSampleName ]->Fill( track->getFitStatus(track->getCardinalRep())->isTrackPruned() );
			h_Ndf[ hitSampleName ]->Fill( track->getFitStatus(track->getCardinalRep())->getNdf() );

			h_TrackLenght[ hitSampleName ]->Fill( track->getTrackLen( track->getCardinalRep() ) );
			// h_TrackTOF[ hitSampleName ]->Fill( track->getTOF( track->getCardinalRep() ) );		// doesn't work!!!!

		}

		if ( i == 0 ) {
			h_startX[ hitSampleName ]->Fill( tmpPos.X() );
			h_startY[ hitSampleName ]->Fill( tmpPos.Y() );
		}
		if ( i == m_hitCollectionToFit[ hitSampleName ].size() -1 ) {
			h_endX[ hitSampleName ]->Fill( tmpPos.X() );
			h_endY[ hitSampleName ]->Fill( tmpPos.Y() );
		}
	}
	if ( m_debug > 0 )		cout << "Chi2 = "<< track->getFitStatus(track->getCardinalRep())->getChi2()<< endl;

	

	// fill histo with tracking info 
	h_chi2[ hitSampleName ]->Fill( track->getFitStatus(track->getCardinalRep())->getChi2() );
	h_momentumMC[ hitSampleName ]->Fill( expectedMom.Mag() );
	h_momentumKal[ hitSampleName ]->Fill( kalmanMom.Mag() );
	h_mass[ hitSampleName ]->Fill( massMC );
	h_polarAngol[ hitSampleName ]->Fill( kalmanMom.Theta() *180 / M_PI );		// convert in radiant

	PrintPositionResidual( kalmanPos, expectedPos, hitSampleName );
	PrintMomentumResidual( kalmanMom, expectedMom, covarianceP, hitSampleName );

}





//----------------------------------------------------------------------------------------------------
void KFitter::InitAllHistos( string hitSampleName ) {

	if ( m_debug > 2 )		cout << "KFitter::InitAllHistos -- Start!!!!  " << endl;
	// initialize output histos
	InitSingleHisto(&h_chi2, hitSampleName, "TrackChi2", 100, 0, 10);
	InitSingleHisto(&h_posRes, hitSampleName, "h_posRes", 40, 0, 0.06);
	InitSingleHisto(&h_sigmaR, hitSampleName, "h_sigmaR", 1000, -1, 1);
	InitSingleHisto(&h_sigmaP, hitSampleName, "h_sigmaP", 1000, -1, 1);
	InitSingleHisto(&h_deltaP, hitSampleName, "h_deltaP", 100, -1, 1);
	InitSingleHisto(&h_momentumRes, hitSampleName, "h_momentumRes", 80, -4, 4);

	InitSingleHisto(&h_momentumMC, hitSampleName, "h_momentumMC", 64, 0, 16);
	InitSingleHisto(&h_momentumKal, hitSampleName, "h_momentumKal", 64, 0, 16);

	InitSingleHisto(&h_polarAngol, hitSampleName, "h_polarAngol", 50, 0, 10);
	InitSingleHisto(&h_mass, hitSampleName, "h_mass", 90, 0, 15);
	InitSingleHisto(&h_zPosGen, hitSampleName, "h_zPosGen", 100, -0.5, 0.5);
	
	InitSingleHisto(&h_mass_genFit, hitSampleName, "h_mass_genFit", 90, 0, 15);
	InitSingleHisto(&h_charge, hitSampleName, "h_charge", 21, -10.5, 10.5);
	// InitSingleHisto(&h_chargeSign, hitSampleName, "h_chargeSign", 5, -2.5, 2.5);
	// InitSingleHisto(&h_startDir, hitSampleName, "h_startDir", 100, -2.5, 2.5);
	// InitSingleHisto(&h_endDir, hitSampleName, "h_endDir", 100, -2.5, 2.5);
	InitSingleHisto(&h_startX, hitSampleName, "h_startX", 40, -2, 2);
	InitSingleHisto(&h_endX, hitSampleName, "h_endY", 100, -10, 10);
	InitSingleHisto(&h_startY, hitSampleName, "h_startX", 40, -2, 2);
	InitSingleHisto(&h_endY, hitSampleName, "h_endY", 100, -10, 10);
	
	// InitSingleHisto(&h_TrackLenght, hitSampleName, "h_TrackLenght", 100, 28, 31);
	InitSingleHisto(&h_TrackLenght, hitSampleName, "h_TrackLenght", 100, 12.5, 15.5);		// VT and IT only
	// InitSingleHisto(&h_TrackTOF, hitSampleName, "h_TrackTOF", 100, 0, 50);
	
	InitSingleHisto(&h_isFitConvergedFully, hitSampleName, "h_isFitConvergedFully", 5, -2.5, 2.5);
	InitSingleHisto(&h_isFitConvergedPartially, hitSampleName, "h_isFitConvergedPartially", 5, -2.5, 2.5);
	InitSingleHisto(&h_NFailedPoints, hitSampleName, "h_NFailedPoints", 10, 0, 10);
	InitSingleHisto(&h_isTrackPruned, hitSampleName, "h_isTrackPruned", 5, -2.5, 2.5);
	InitSingleHisto(&h_Ndf, hitSampleName, "h_Ndf", 15, 0, 15);
	
	InitSingleHisto(&h_dP_over_Ptrue, hitSampleName, "h_dP_over_Ptrue", 40, -0.2, 0.2);
	InitSingleHisto(&h_dP_over_Pkf, hitSampleName, "h_dP_over_Pkf", 40, -0.2, 0.2);
	InitSingleHisto(&h_sigmaP_over_Pkf, hitSampleName, "h_sigmaP_over_Pkf", 40, 0, 0.1);
	InitSingleHisto(&h_sigmaP_over_Ptrue, hitSampleName, "h_sigmaP_over_Ptrue", 40, 0, 0.5);


	// 2D
	if ( h_covariance.find( hitSampleName ) == h_covariance.end() )					
		h_covariance[hitSampleName] = new TH2F( "covariance", "covariance", 3, 0, 3, 3, 0, 3 );


 	if ( m_debug > 2 )		cout << "KFitter::InitAllHistos -- End!!!!  " << endl;
}



//----------------------------------------------------------------------------------------------------
void KFitter::InitSingleHisto( map< string, TH1F* >* histoMap, string collectionName, string histoName, int nBin, float minBin, float maxBin ) {
	if ( histoMap->find( collectionName ) == histoMap->end() )					
		(*histoMap)[collectionName] = new TH1F( histoName.c_str(), histoName.c_str(), nBin, minBin, maxBin );
}

//----------------------------------------------------------------------------------------------------
// void KFitter::InitMultiBinHistoMap( map< map< string, TH1F* > >* histoMap, string collectionName, string histoName, int nBin, int minBin, int maxBin ) {
// 	if ( histoMap.find( collectionName ) == histoMap.end() )					
// 		histoMap[collectionName] = new TH1F( histoName.c_str(), histoName.c_str(), 100, minBin, maxBin );
// }



//----------------------------------------------------------------------------------------------------
void KFitter::PrintEfficiency() {

	TCanvas* mirror = new TCanvas("TrackEfficiencyPlot", "TrackEfficiencyPlot", 700, 700);
	mirror->SetRightMargin(0.05);
    mirror->SetLeftMargin(0.13);

	int nCollection = m_nTotTracks.size();
	TH1F* h_trackEfficiency = new TH1F( "TrackEfficiency", "TrackEfficiency", nCollection, 0, nCollection );
	
	int k = 0;
	for ( map<string, int>::iterator itTotNum = m_nTotTracks.begin(); itTotNum != m_nTotTracks.end(); itTotNum++ ) {
		k++;
		float eff = (float)m_nConvergedTracks[ (*itTotNum).first ] / (*itTotNum).second;
		if ( m_debug > -1 )		cout << "Efficiency " << (*itTotNum).first << " = " << eff << "  " << m_nConvergedTracks[ (*itTotNum).first ]<< " " << (*itTotNum).second << endl;
		h_trackEfficiency->SetBinContent(k, eff);
		h_trackEfficiency->GetXaxis()->SetBinLabel(k, (*itTotNum).first.c_str() );
	}
	h_trackEfficiency->SetTitle(0);
	h_trackEfficiency->SetStats(0);
	h_trackEfficiency->GetYaxis()->SetTitle("KF Efficiency");
	h_trackEfficiency->GetYaxis()->SetTitleOffset(1.1);
	h_trackEfficiency->SetLineWidth(1.8);
	h_trackEfficiency->Draw();
	mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+"TrackEfficiencyPlot.png").c_str() );
	mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+"TrackEfficiencyPlot.root").c_str() );
	
}


//----------------------------------------------------------------------------------------------------
void KFitter::PrintPositionResidual( TVector3 pos, TVector3 expectedPos, string hitSampleName ) {
	double dR = pos.DeltaR( expectedPos );
	h_posRes[ hitSampleName ]->Fill(dR);
}



//----------------------------------------------------------------------------------------------------
void KFitter::PrintMomentumResidual( TVector3 meas, TVector3 expected, TVector3 cov, string hitSampleName ) {

	if ( m_debug > 2 )		cout << "KFitter::PrintMomentumResidual -- Start!!!!  " << endl;
	double dP = meas.Mag() - expected.Mag();
	double err = EvalError( meas, cov );

	if ( m_debug > 1 )		cout << "dp= " <<meas.Mag() << "-"<<expected.Mag() << "   err= " << err<< endl;
	if ( m_debug > 1 )		cout << " residuo= "<< dP / err <<endl;

	h_deltaP[ hitSampleName ]->Fill( dP );
	h_sigmaP[ hitSampleName ]->Fill(err);
	h_momentumRes[ hitSampleName ]->Fill( dP /err);

	h_dP_over_Ptrue[ hitSampleName ]->Fill( dP / expected.Mag() );
	h_dP_over_Pkf[ hitSampleName ]->Fill( dP / meas.Mag() );
	h_sigmaP_over_Ptrue[ hitSampleName ]->Fill( err / expected.Mag() );
	h_sigmaP_over_Pkf[ hitSampleName ]->Fill( err / meas.Mag() );

	
	// histos for momentum reso
	if ( meas.Mag() == 0 || expected.Mag() == 0 ) 
		cout<< "ERROR::KFitter::PrintMomentumResidual  -->  track momentum - 0. "<< endl, exit(0);
	// find the center of the momentum bin
	int roundUp = ceil( (double)expected.Mag() );
	int roundDown = floor( (double)expected.Mag() );
	float binCenter = -666;
	int nstep = ((float)(roundUp - roundDown)) / m_resoP_step;
	for ( int i=0; i<nstep; i++ ) {
		if ( expected.Mag() > roundDown+(i*m_resoP_step) &&  expected.Mag() <= roundDown+((i+1)*m_resoP_step) ) {
			binCenter = roundDown + m_resoP_step*i + 0.5*m_resoP_step;
			break;
		}
	}

	// fill the h_dP_x_bin
	if ( h_dP_x_bin.find( hitSampleName ) == h_dP_x_bin.end() ) {
		map<float, TH1F*> tmp_dP_x_bin;
		
		string name = "dP_dist_"+hitSampleName+"_"+build_string(binCenter);
		TH1F* h = new TH1F( name.c_str(), name.c_str(), 80 , -2, 2 );
		tmp_dP_x_bin[ binCenter ] = h;
				
		h_dP_x_bin[ hitSampleName ] = tmp_dP_x_bin;
	}
	else if ( h_dP_x_bin[ hitSampleName ].find( binCenter ) == h_dP_x_bin[ hitSampleName ].end() ) {
		string name = "dP_dist_"+hitSampleName+"_"+build_string(binCenter);
		TH1F* h = new TH1F( name.c_str(), name.c_str(), 80 , -2, 2 );
		h_dP_x_bin[ hitSampleName ][ binCenter ] = h;
	}
	h_dP_x_bin[ hitSampleName ][ binCenter ]->Fill( dP );

	// fill the h_dPOverP_x_bin
	if ( h_dPOverP_x_bin.find( hitSampleName ) == h_dPOverP_x_bin.end() ) {
		map<float, TH1F*> tmp_dPOverP_x_bin;
		
		string name = "dPOverP_dist_"+hitSampleName+"_"+build_string(binCenter);
		TH1F* h = new TH1F( name.c_str(), name.c_str(), 80 , -0.2, 0.2 );
		tmp_dPOverP_x_bin[ binCenter ] = h;
				
		h_dPOverP_x_bin[ hitSampleName ] = tmp_dPOverP_x_bin;
	}
	else if ( h_dPOverP_x_bin[ hitSampleName ].find( binCenter ) == h_dPOverP_x_bin[ hitSampleName ].end() ) {
		string name = "dPOverP_dist_"+hitSampleName+"_"+build_string(binCenter);
		TH1F* h = new TH1F( name.c_str(), name.c_str(), 80 , -0.2, 0.2 );
		h_dPOverP_x_bin[ hitSampleName ][ binCenter ] = h;
	}
	h_dPOverP_x_bin[ hitSampleName ][ binCenter ]->Fill( dP/expected.Mag() );

	// fill the h_dPOverSigmaP_x_bin
	if ( h_dPOverSigmaP_x_bin.find( hitSampleName ) == h_dPOverSigmaP_x_bin.end() ) {
		map<float, TH1F*> tmp_dPOverSigmaP_x_bin;
		
		string name = "dPOverSigmaP_dist_"+hitSampleName+"_"+build_string(binCenter);
		TH1F* h = new TH1F( name.c_str(), name.c_str(), 80 , -2, 2 );
		tmp_dPOverSigmaP_x_bin[ binCenter ] = h;
				
		h_dPOverSigmaP_x_bin[ hitSampleName ] = tmp_dPOverSigmaP_x_bin;
	}
	else if ( h_dPOverSigmaP_x_bin[ hitSampleName ].find( binCenter ) == h_dPOverSigmaP_x_bin[ hitSampleName ].end() ) {
		string name = "dPOverSigmaP_dist_"+hitSampleName+"_"+build_string(binCenter);
		TH1F* h = new TH1F( name.c_str(), name.c_str(), 80 , -4, 4 );
		h_dPOverSigmaP_x_bin[ hitSampleName ][ binCenter ] = h;
	}
	h_dPOverSigmaP_x_bin[ hitSampleName ][ binCenter ]->Fill( dP/err );



	if ( m_debug > 1 )		cout << "KFitter::PrintMomentumResidual -- End!!!!  " << endl;
}




//----------------------------------------------------------------------------------------------------
void KFitter::PrintMomentumResidual( TVector3 meas, TVector3 expected, TMatrixD cov, string hitSampleName ) {

	if ( m_debug > 2 )		cout << "KFitter::PrintMomentumResidual -- Start!!!!  " << endl;
	double dP = meas.Mag() - expected.Mag();
	double err = EvalError( meas, cov );

	if ( m_debug > 1 )		cout << "dp= " <<meas.Mag() << "-"<<expected.Mag() << "   err= " << err<< endl;
	if ( m_debug > 1 )		cout << " residuo= "<< dP / err <<endl;

	h_deltaP[ hitSampleName ]->Fill( dP );
	h_sigmaP[ hitSampleName ]->Fill(err);
	h_momentumRes[ hitSampleName ]->Fill( dP /err);

	h_dP_over_Ptrue[ hitSampleName ]->Fill( dP / expected.Mag() );
	h_dP_over_Pkf[ hitSampleName ]->Fill( dP / meas.Mag() );
	h_sigmaP_over_Ptrue[ hitSampleName ]->Fill( err / expected.Mag() );
	h_sigmaP_over_Pkf[ hitSampleName ]->Fill( err / meas.Mag() );

	
	// histos for momentum reso
	if ( meas.Mag() == 0 || expected.Mag() == 0 ) 
		cout<< "ERROR::KFitter::PrintMomentumResidual  -->  track momentum - 0. "<< endl, exit(0);
	// find the center of the momentum bin
	int roundUp = ceil( (double)expected.Mag() );
	int roundDown = floor( (double)expected.Mag() );
	float binCenter = -666;
	int nstep = ((float)(roundUp - roundDown)) / m_resoP_step;
	for ( int i=0; i<nstep; i++ ) {
		if ( expected.Mag() > roundDown+(i*m_resoP_step) &&  expected.Mag() <= roundDown+((i+1)*m_resoP_step) ) {
			binCenter = roundDown + m_resoP_step*i + 0.5*m_resoP_step;
			break;
		}
	}

	// fill the h_dP_x_bin
	if ( h_dP_x_bin.find( hitSampleName ) == h_dP_x_bin.end() ) {
		map<float, TH1F*> tmp_dP_x_bin;
		
		string name = "dP_dist_"+hitSampleName+"_"+build_string(binCenter);
		TH1F* h = new TH1F( name.c_str(), name.c_str(), 80 , -2, 2 );
		tmp_dP_x_bin[ binCenter ] = h;
				
		h_dP_x_bin[ hitSampleName ] = tmp_dP_x_bin;
	}
	else if ( h_dP_x_bin[ hitSampleName ].find( binCenter ) == h_dP_x_bin[ hitSampleName ].end() ) {
		string name = "dP_dist_"+hitSampleName+"_"+build_string(binCenter);
		TH1F* h = new TH1F( name.c_str(), name.c_str(), 80 , -2, 2 );
		h_dP_x_bin[ hitSampleName ][ binCenter ] = h;
	}
	h_dP_x_bin[ hitSampleName ][ binCenter ]->Fill( dP );

	// fill the h_dPOverP_x_bin
	if ( h_dPOverP_x_bin.find( hitSampleName ) == h_dPOverP_x_bin.end() ) {
		map<float, TH1F*> tmp_dPOverP_x_bin;
		
		string name = "dPOverP_dist_"+hitSampleName+"_"+build_string(binCenter);
		TH1F* h = new TH1F( name.c_str(), name.c_str(), 80 , -0.2, 0.2 );
		tmp_dPOverP_x_bin[ binCenter ] = h;
				
		h_dPOverP_x_bin[ hitSampleName ] = tmp_dPOverP_x_bin;
	}
	else if ( h_dPOverP_x_bin[ hitSampleName ].find( binCenter ) == h_dPOverP_x_bin[ hitSampleName ].end() ) {
		string name = "dPOverP_dist_"+hitSampleName+"_"+build_string(binCenter);
		TH1F* h = new TH1F( name.c_str(), name.c_str(), 80 , -0.2, 0.2 );
		h_dPOverP_x_bin[ hitSampleName ][ binCenter ] = h;
	}
	h_dPOverP_x_bin[ hitSampleName ][ binCenter ]->Fill( dP/expected.Mag() );

	// fill the h_dPOverSigmaP_x_bin
	if ( h_dPOverSigmaP_x_bin.find( hitSampleName ) == h_dPOverSigmaP_x_bin.end() ) {
		map<float, TH1F*> tmp_dPOverSigmaP_x_bin;
		
		string name = "dPOverSigmaP_dist_"+hitSampleName+"_"+build_string(binCenter);
		TH1F* h = new TH1F( name.c_str(), name.c_str(), 80 , -2, 2 );
		tmp_dPOverSigmaP_x_bin[ binCenter ] = h;
				
		h_dPOverSigmaP_x_bin[ hitSampleName ] = tmp_dPOverSigmaP_x_bin;
	}
	else if ( h_dPOverSigmaP_x_bin[ hitSampleName ].find( binCenter ) == h_dPOverSigmaP_x_bin[ hitSampleName ].end() ) {
		string name = "dPOverSigmaP_dist_"+hitSampleName+"_"+build_string(binCenter);
		TH1F* h = new TH1F( name.c_str(), name.c_str(), 80 , -4, 4 );
		h_dPOverSigmaP_x_bin[ hitSampleName ][ binCenter ] = h;
	}
	h_dPOverSigmaP_x_bin[ hitSampleName ][ binCenter ]->Fill( dP/err );



	if ( m_debug > 1 )		cout << "KFitter::PrintMomentumResidual -- End!!!!  " << endl;
}



//----------------------------------------------------------------------------------------------------
//  measure the Kalman efficiency over the total sets of points satisfaing the pre-fitting criteria
double KFitter::EvalError( TVector3 mom, TVector3 err ) {

	double dx2 = 2 * sqrt(err.x()) * mom.x();
	double dy2 = 2 * sqrt(err.y()) * mom.y();
	double dz2 = 2 * sqrt(err.z()) * mom.z();

	double p = mom.Mag();
	double dp2 = dx2 + dy2 + dz2;
	double dp = dp2 / ( 2 * p );

	return dp;
}



//----------------------------------------------------------------------------------------------------
//  measure the Kalman uncertainty INCLUDING the cross terms in the covariance matrix
double KFitter::EvalError( TVector3 mom, TMatrixD cov ) {

	if ( cov.GetNcols() != 3 || cov.GetNrows() != 3 ) 
		cout << "ERROR :: KFitter::EvalError  >>  covariance dimension (should be 6) is wrong " << cov.GetNcols() << " x " << cov.GetNrows() << endl, exit(0);

	array<double,3> partialDer = { mom.x()/sqrt(mom.Mag()), mom.y()/sqrt(mom.Mag()), mom.z()/sqrt(mom.Mag()) };


	double err = 0;
	for ( int j=0; j<cov.GetNrows(); j++ ) {
		for ( int k=0; k<cov.GetNcols(); k++ ) {
			
			err += partialDer[j] * partialDer[k] * cov(j,k); // * diagFactor;
			
		}
	}

	double dp = sqrt(err);

	return dp;
}



//----------------------------------------------------------------------------------------------------
// Called from outside!
void KFitter::EvaluateMomentumResolution() {


	for ( map<string, map<float, TH1F*> >::iterator collIt=h_dPOverP_x_bin.begin(); collIt != h_dPOverP_x_bin.end(); collIt++ ) {
		
		//  initialize output resolution histos
		float resoP_min = (*(*collIt).second.begin()).first - m_resoP_step*0.5;
		float resoP_max = (*(*collIt).second.rbegin()).first + m_resoP_step*0.5;
		float nfbin = (resoP_max-resoP_min)/m_resoP_step;
		// if ( modf( (resoP_max-resoP_min)/m_resoP_step, &nfbin ) == 0.0 ) 
		if ( fabs(nfbin-round(nfbin)) > 0.001 )
			cout<<"ERROR :: KFitter::EvaluateMomentumResolution  --> "<<(*collIt).first<<" resolution binning not round! min=" <<resoP_min<<" max="<<resoP_max<<" step="<<m_resoP_step<<" = "<<nfbin<< endl, exit(0);		// check correct binning
		int nbin = round(nfbin);

		string histoName = (string)"h_resoP_over_Pkf"+"__"+(*collIt).first;
		h_resoP_over_Pkf[ (*collIt).first ] = new TH1F( histoName.c_str(), histoName.c_str(), nbin, resoP_min, resoP_max );
		
		histoName = (string)"h_biasP_over_Pkf"+"__"+(*collIt).first;
		h_biasP_over_Pkf[ (*collIt).first ] = new TH1F( histoName.c_str(), histoName.c_str(), nbin, resoP_min, resoP_max );
	


		int k=0;
		for ( map<float, TH1F*>::iterator it=(*collIt).second.begin(); it != (*collIt).second.end(); it++ ) {
			
			k++;
			if ( k > h_resoP_over_Pkf[ (*collIt).first ]->GetNbinsX() )			
				cout<<"ERROR :: KFitter::EvaluateMomentumResolution  --> "<<(*collIt).first<< "  binning problem! do not fill all the reso plot. " << endl, exit(0);	

			if ( m_debug <= 0 && (*it).second->GetEntries() < 100 )		continue;

			// check if the binning produce even bounds for the bins, not irrational numbers for examples
			float a = (h_resoP_over_Pkf[ (*collIt).first ]->GetXaxis()->GetBinLowEdge(k) + h_resoP_over_Pkf[ (*collIt).first ]->GetXaxis()->GetBinUpEdge(k)) /2;
			float b = (*it).first;
			if ( fabs(a - b) > 0.00001 ) {
				cout << "WARNING::KFitter::EvaluateMomentumResolution \t >> \t chosen binning do not make bin's round bounds "<<(*collIt).first<<": " << h_resoP_over_Pkf[ (*collIt).first ]->GetXaxis()->GetBinLowEdge(k) << " + "
						<< h_resoP_over_Pkf[ (*collIt).first ]->GetXaxis()->GetBinUpEdge(k) <<" = " << a << " instead of 		" << b << endl; 
				it--;
				continue;
			}

			// TF1 *f1 = new TF1("f1","gaus",-3,3);
			TF1 * f1 = new TF1("gauss", "[0] / sqrt(2.0 * TMath::Pi()) / [2] * exp(-(x-[1])*(x-[1])/2./[2]/[2])", 0, 100);
			f1->SetParNames("Constant","Mean","Sigma");
		    f1->SetParameters( 0, (*it).second->GetMean(), (*it).second->GetRMS() );
		    // f1->SetParameters( 0, h_resoP_over_Pkf[ (*collIt).first ]->GetMean(), h_resoP_over_Pkf[ (*collIt).first ]->GetStdDev() );
		    f1->SetParLimits(0,  0.1, (*it).second->GetMaximum());
		    f1->SetParLimits(1, -0.18,  0.18);
		    f1->SetParLimits(2,  0.001,  0.16);
		    f1->SetLineWidth(2);
		    f1->SetLineColor(2);
			// h_resoP_over_Pkf[ (*collIt).first ]->Fit("f1","R");
			(*it).second->Fit("gauss", "LQ");	// log likelihood fit, quiet mode

			if ( m_debug <= 0 && (f1->GetParError(f1->GetParNumber("Sigma")) / f1->GetParameter(f1->GetParNumber("Sigma")) > 0.1) )	
				continue;

			// GetNumberFreeParameters()
			// h_resoP_over_Pkf[ (*collIt).first ]->SetBinContent( k, f1->GetParameter(1) );
			h_resoP_over_Pkf[ (*collIt).first ]->SetBinContent( k, f1->GetParameter(f1->GetParNumber("Sigma")) );
			h_resoP_over_Pkf[ (*collIt).first ]->SetBinError( k, f1->GetParError (f1->GetParNumber("Sigma")) );
			// h_resoP_over_Pkf[ (*collIt).first ]->SetBinContent( k, (*it).second->GetStdDev(1) );
			// h_resoP_over_Pkf[ (*collIt).first ]->SetBinError( k, (*it).second->GetStdDev(1) );
			

			h_biasP_over_Pkf[ (*collIt).first ]->SetBinContent( k, f1->GetParameter(f1->GetParNumber("Mean")) );
			h_biasP_over_Pkf[ (*collIt).first ]->SetBinError( k, f1->GetParameter(f1->GetParNumber("Mean")) );
		}
	}
}




//----------------------------------------------------------------------------------------------------
void KFitter::SaveHisto( TCanvas* mirror, map< string, TH1F* > histoMap, string title, string saveName ) {

	for ( map< string, TH1F* >::iterator it=histoMap.begin(); it != histoMap.end(); it++ ) {

		(*it).second->GetXaxis()->SetTitle( title.c_str() );
		(*it).second->Draw();
		
		// (*it).second->SetLineColor( m_vecHistoColor[ k ] );
		(*it).second->SetLineWidth(2);

		mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+saveName + ".png").c_str() );
		mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+saveName + ".root").c_str() );
	}
}



//----------------------------------------------------------------------------------------------------
// to be cancelled
void KFitter::SaveHisto( TCanvas* mirror, TH1F* histoMap, string title, string saveName ) {


	histoMap->GetXaxis()->SetTitle( title.c_str() );
	histoMap->Draw();
	
	// (*it).second->SetLineColor( m_vecHistoColor[ k ] );
	histoMap->SetLineWidth(2);

	mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+"C"+"/"+saveName + ".png").c_str() );

}




//----------------------------------------------------------------------------------------------------
void KFitter::Save( ) {

	// cambiare!!!!!
	struct stat info;
	for ( map< string, TH1F* >::iterator it=h_deltaP.begin(); it != h_deltaP.end(); it++ ) {

		// struct stat info;
		string pathName = (string)getenv("FOOTRES")+"/"+(*it).first;
		if( stat( pathName.c_str(), &info ) != 0 )		//cannot access
		    system(("mkdir "+pathName).c_str());
		// else if( info.st_mode & S_IFDIR )  // is a directory    
		// else    cout << "WARNING::KFitter::Save( )	>>		recognise directory strange behaviour..."<< endl;
	}

	TCanvas* mirror = new TCanvas("TrackChi2Plot", "TrackChi2Plot", 700, 700);

	SaveHisto( mirror, h_chi2, "#chi^{2}", "TrackChi2Plot");
	SaveHisto( mirror, h_momentumRes, "dp/sigma(p)", "MomentumRes" );
	// SaveHisto( mirror, h_momentumKal );
	SaveHisto( mirror, h_posRes, "dR", "PositionRes" );
	SaveHisto( mirror, h_mass, "mass(GeV)", "mass" );
	SaveHisto( mirror, h_zPosGen, "gen z(cm)", "GenZ" );

	SaveHisto( mirror, h_mass_genFit, "mass(GeV)", "mass" );
	SaveHisto( mirror, h_charge, "q", "charge" );
	SaveHisto( mirror, h_isFitConvergedFully, "isFitConvergedFully", "a" );
	SaveHisto( mirror, h_isFitConvergedPartially, "isConvergedPartially", "b" );
	SaveHisto( mirror, h_NFailedPoints, "nFailedPoints", "c" );
	SaveHisto( mirror, h_isTrackPruned, "isPruned", "d" );
	SaveHisto( mirror, h_Ndf, "nDoF", "e" );
	
	SaveHisto( mirror, h_startX, "startX", "f" );
	SaveHisto( mirror, h_endX, "endY", "g" );
	SaveHisto( mirror, h_startY, "startY", "h" );
	SaveHisto( mirror, h_endY, "endY", "i" );
	
	SaveHisto( mirror, h_TrackLenght, "TrackLenght", "Track lenght (cm)" );
	// SaveHisto( mirror, h_TrackTOF, "TrackTOF", "TOF (ns)" );

	SaveHisto( mirror, h_sigmaP, "sigma(p)", "sigmaP" );
	SaveHisto( mirror, h_sigmaR, "sigma(r)", "sigmaR" );
	SaveHisto( mirror, h_deltaP, "dp", "deltaP" );
	SaveHisto( mirror, h_polarAngol, "polarAngol(deg)", "polarAngol" );
	SaveHisto( mirror, h_dP_over_Ptrue, "dp/p_{true}", "dP_over_Ptrue" );
	SaveHisto( mirror, h_dP_over_Pkf, "dp/p_{KF}", "dP_over_Pkf" );
	SaveHisto( mirror, h_sigmaP_over_Pkf, "sigma(p)/p_{true}", "sigmaP_over_Pkf" );
	SaveHisto( mirror, h_sigmaP_over_Ptrue, "sigma(p)/p_{KF}", "sigmaP_over_Ptrue" );

	
	for ( map<string, map<float, TH1F*> >::iterator collIt=h_dP_x_bin.begin(); collIt != h_dP_x_bin.end(); collIt++ ) {
		string pathName = (string)getenv("FOOTRES")+"/"+(*collIt).first+"/dP_x_bin";
		if( stat( pathName.c_str(), &info ) != 0 )		//cannot access
		    system(("mkdir "+pathName).c_str());
		for ( map<float, TH1F*>::iterator it=(*collIt).second.begin(); it != (*collIt).second.end(); it++ ) {
			(*it).second->GetXaxis()->SetTitle("dP(GeV)");
			(*it).second->Draw();
			mirror->SaveAs(( pathName+"/"+"dP_x_bin__"+(*collIt).first+"__"+build_string( (*it).first )+".png").c_str());
		}
	}
	gStyle->SetOptFit(111111);
	for ( map<string, map<float, TH1F*> >::iterator collIt=h_dPOverP_x_bin.begin(); collIt != h_dPOverP_x_bin.end(); collIt++ ) {
		string pathName = (string)getenv("FOOTRES")+"/"+(*collIt).first+"/dPOverP_x_bin";
		if( stat( pathName.c_str(), &info ) != 0 )		//cannot access
		    system(("mkdir "+pathName).c_str());
		for ( map<float, TH1F*>::iterator it=(*collIt).second.begin(); it != (*collIt).second.end(); it++ ) {
			(*it).second->GetXaxis()->SetTitle("dP/P");
			(*it).second->Draw();
			mirror->SaveAs(( pathName+"/"+"dPOverP_x_bin__"+(*collIt).first+"__"+build_string( (*it).first )+".png").c_str());
		}
	}
	gStyle->SetOptFit(111);
	for ( map<string, map<float, TH1F*> >::iterator collIt=h_dPOverSigmaP_x_bin.begin(); collIt != h_dPOverSigmaP_x_bin.end(); collIt++ ) {
		string pathName = (string)getenv("FOOTRES")+"/"+(*collIt).first+"/dPOverSigmaP_x_bin";
		if( stat( pathName.c_str(), &info ) != 0 )		//cannot access
		    system(("mkdir "+pathName).c_str());
		for ( map<float, TH1F*>::iterator it=(*collIt).second.begin(); it != (*collIt).second.end(); it++ ) {
			(*it).second->GetXaxis()->SetTitle("dP/sigmaP");
			(*it).second->Draw();
			mirror->SaveAs(( pathName+"/"+"dPOverSigmaP_x_bin__"+(*collIt).first+"__"+build_string( (*it).first )+".png").c_str());
		}
	}

	for ( map< string, TH1F* >::iterator it=h_resoP_over_Pkf.begin(); it != h_resoP_over_Pkf.end(); it++ ) {
		(*it).second->GetXaxis()->SetTitle("p(GeV)");
		(*it).second->GetYaxis()->SetTitle("dp/p");
		(*it).second->Draw();
		
		mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+"differentialMomentumReso" + ".png").c_str() );
		mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+"differentialMomentumReso" + ".root").c_str() );
		// string aaa = mirror->GetName();
		// mirror->SetName( (*it).second->GetName() );
		// mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+"differentialMomentumReso" + ".root").c_str() );
		// mirror->SetName( aaa.c_str() );
	}

	for ( map< string, TH1F* >::iterator it=h_biasP_over_Pkf.begin(); it != h_biasP_over_Pkf.end(); it++ ) {
		(*it).second->GetXaxis()->SetTitle("p(GeV)");
		(*it).second->GetYaxis()->SetTitle("Bias(dp/p)");
		(*it).second->Draw();
		mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+"differentialResoBias" + ".png").c_str() );
		mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+"differentialResoBias" + ".root").c_str() );
	}

	// momentum distribution comparison
	for ( map< string, TH1F* >::iterator it=h_momentumMC.begin(); it != h_momentumMC.end(); it++ ) {
		
		(*it).second->GetXaxis()->SetTitle("p [GeV]");
		(*it).second->SetLineColor(kGreen+1);
		(*it).second->SetLineWidth(2);
		h_momentumKal[ (*it).first ]->SetLineColor(kOrange+8);
		h_momentumKal[ (*it).first ]->SetLineWidth(2);
		(*it).second->Draw();
		h_momentumKal[ (*it).first ]->Draw("same");
		mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+"Momentum" + ".png").c_str() );
		mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+"Momentum" + ".root").c_str() );
		
	}

	// 2D
	for ( map< string, TH2F* >::iterator it=h_covariance.begin(); it != h_covariance.end(); it++ ) {

		// (*it).second->GetXaxis()->SetTitle("p [GeV]");
		(*it).second->Scale( 1./( (float)((*it).second->GetEntries())/9. ) );
		gStyle->SetPalette(1);
		mirror->SetLogz(1);
		(*it).second->Draw("colz text");
		mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+"covariance" + ".png").c_str() );
		mirror->SaveAs( ((string)getenv("FOOTRES")+"/"+(*it).first+"/"+"covariance" + ".root").c_str() );
		mirror->SetLogz(0);
	}

}















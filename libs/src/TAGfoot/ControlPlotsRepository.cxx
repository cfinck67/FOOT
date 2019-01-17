

#include "ControlPlotsRepository.hxx"


// Global static pointer used to ensure a single instance of the class.
ControlPlotsRepository* ControlPlotsRepository::m_pInstance = NULL;




ControlPlotsRepository::ControlPlotsRepository() {
	m_alreadyPrintedOut = false;
	m_lastPrintedDir = "none";
};





ControlPlotsRepository* ControlPlotsRepository::Instance() {
	if (!m_pInstance) {  // Only allow one instance of class to be generated.
        m_pInstance = new ControlPlotsRepository();
    }
    else {
        cout << "ERROR::ControlPlotsRepository::Instance  -->  already instanced, cannot instance twice!!!" << endl, exit(0);
    }

    return m_pInstance;
};





ControlPlotsRepository* ControlPlotsRepository::GetControlObject( string outputDir ) {
	if (!m_pInstance) 
       cout << "ERROR::ControlPlotsRepository::GetControlObject()  -->  called a get before GlobalPar object istance." << endl, exit(0);

   	m_pInstance->SetOutputDir( outputDir );

    return m_pInstance;
};






void ControlPlotsRepository::FillMap( string mapName, double x ) {

	if ( m_chizu.find( mapName ) == m_chizu.end() ) {
		m_chizu[ mapName ] = ControlPlotInfo( mapName, x );	// create a new map element
	}
	else {
		m_chizu[ mapName ].addInfo( x );
	}
	m_chizu[ mapName ].SetOutputDir( m_outputDir );

};



void ControlPlotsRepository::PrintMap() {

	if( m_alreadyPrintedOut ) {
		cout << "\n\n\nWARNING >> ControlPlotsRepository::PrintMap()  ::  Control plot already printed out. Plots will be re-wrote. Please, leave ONLY ONE PrintMap call.\n";
		cout << "Last call from the file that write out in the dir:  " << m_lastPrintedDir << endl<<endl<<endl;
	}

	m_alreadyPrintedOut = true;
	m_lastPrintedDir = m_outputDir;

	TCanvas* mirror = new TCanvas( "mirror", "mirror", 700, 700);
	for ( map< string, ControlPlotInfo >::iterator it = m_chizu.begin(); it != m_chizu.end(); it++ ) {

		(*it).second.PrintHisto( mirror );

	}

};





void ControlPlotsRepository::PrintOutputFile() {
 
	m_outputfilename = GlobalPar::GetPar()->OutputFile();
	f_out = new TFile(m_outputfilename.c_str(),"RECREATE");  

	TCanvas* mirror = new TCanvas( "mirror", "mirror", 700, 700);

	for ( map< string, ControlPlotInfo >::iterator it = m_chizu.begin(); it != m_chizu.end(); it++ ) {

		vector<string> token = Tokenize((*it).first, "__" );	
		string first_token =  token[0];	
		TDirectory *dir = f_out->GetDirectory(first_token.c_str());

		if(!dir)	  	{
			f_out->mkdir(first_token.c_str());
			f_out->cd(first_token.c_str()); 
		}

		(*it).second.PrintHisto( mirror);	
	}

	//~ f_out->Write();
	//~ f_out->Close();
};

void ControlPlotsRepository::SaveOutputFile() {
  f_out->Write();
  f_out->Close();
return;
}

 
void ControlPlotsRepository::PrintOutputNtuple() {
	m_outputntuplename = GlobalPar::GetPar()->OutputNtuple();
	TFile* n_out = new TFile(m_outputntuplename.c_str(),"RECREATE");  

	//  cout <<" outputntuplename  " << m_outputntuplename.c_str() << endl;

	TTree* tree_out = new TTree("EventTree","Reco Event Tree");
	//~ for (int i = 0; i< ntuple_out.Reco_track_px.size(); i++){
		//~ //cout <<" Reco_px "  << ntuple_out.Reco_px.at(i) << endl;
		//~ tree_out->Branch("Reco_track_px",            &ntuple_out.Reco_track_px.at(i)   );
		//~ tree_out->Branch("Reco_track_py",            &ntuple_out.Reco_track_py.at(i)   );
		//~ tree_out->Branch("Reco_track_pz",            &ntuple_out.Reco_track_pz.at(i)   );
		//~ tree_out->Branch("Reco_track_x",             &ntuple_out.Reco_track_x.at(i)    );
		//~ tree_out->Branch("Reco_track_y",             &ntuple_out.Reco_track_y.at(i)    );
		//~ tree_out->Branch("Reco_track_z",             &ntuple_out.Reco_track_z.at(i)    );
		//~ tree_out->Branch("Truth_track_px",           &ntuple_out.Truth_track_px.at(i)  );
		//~ tree_out->Branch("Truth_track_py",           &ntuple_out.Truth_track_py.at(i)  );
		//~ tree_out->Branch("Truth_track_pz",           &ntuple_out.Truth_track_pz.at(i)  );

		//~ tree_out->Fill();

	//~ }

  //Beam Monitor stuff
  //provv, it will be replaced by the real ntupleoutput with the bm objects  
  if(GlobalPar::GetPar()->IncludeBM()){
    Double_t hitrdrift,hittime, hitresidual, trackchi2, trackpversx, trackpversy,trackpversz,trackr0x,trackr0y;
    Int_t evnum,hitplane,hitview,hitcell, time_acq;
    tree_out->Branch("evnum", &evnum);
    tree_out->Branch("timeacq", &time_acq);
    tree_out->Branch("BM_hitrdrift", &hitrdrift);
    tree_out->Branch("BM_hitresidual", &hitresidual);
    tree_out->Branch("BM_hittime",&hittime);
    tree_out->Branch("BM_hitplane", &hitplane);
    tree_out->Branch("BM_hitview",&hitview );
    tree_out->Branch("BM_hitcell",&hitcell);
    tree_out->Branch("BM_track_chi2", &trackchi2);
    tree_out->Branch("BM_track_PversX", &trackpversx);
    tree_out->Branch("BM_track_PversY", &trackpversy);
    tree_out->Branch("BM_track_PversZ", &trackpversz);
    tree_out->Branch("BM_track_R0X", &trackr0x );
    tree_out->Branch("BM_track_R0Y", &trackr0y );
    for(Int_t k=0;k<ntuple_out.evnum.size();k++){
      for (Int_t i = 0; i< ntuple_out.BM_hit_rdrift.at(k).size(); i++){//loop on BM hits
        evnum=ntuple_out.evnum.at(k);
        hitrdrift=ntuple_out.BM_hit_rdrift.at(k).at(i);
        hitresidual=ntuple_out.BM_hit_residual.at(k).at(i);
        hittime=ntuple_out.BM_hit_time.at(k).at(i);
        hitplane=ntuple_out.BM_hit_plane.at(k).at(i);
        hitview=ntuple_out.BM_hit_view.at(k).at(i);
        hitcell=ntuple_out.BM_hit_cell.at(k).at(i);
        trackchi2=ntuple_out.BM_track_chi2.at(k);
        trackpversx=ntuple_out.BM_track_PversX.at(k);
        trackpversy=ntuple_out.BM_track_PversY.at(k);
        trackpversz=ntuple_out.BM_track_PversZ.at(k);
        trackr0x=ntuple_out.BM_track_R0X.at(k);
        trackr0y=ntuple_out.BM_track_R0Y.at(k);
        tree_out->Fill();
      }
    }

  }
    
	n_out->Write();
	n_out->Close();
}
























#include "ControlPlotInfo.hxx"

void  ControlPlotInfo::PrintHisto( TCanvas* mirror) {
	// check if parameter from file is enabled		-> 		otherwise guess them

	// nBin
	if ( GlobalPar::GetPar()->GetLowBinHisto( m_name ) != -666 )
		m_nBin = GlobalPar::GetPar()->GetLowBinHisto( m_name ); 
	else 
		m_nBin = 100;

	// range
	if ( GlobalPar::GetPar()->GetNBinHisto( m_name ) != -666 ) {
		m_minBin = GlobalPar::GetPar()->GetNBinHisto( m_name ); 
		m_maxBin = GlobalPar::GetPar()->GetUpBinHisto( m_name ); 
	}
	else {
		// automatic calculation of the range if not defined in the parameters
		if ( isDoub ) {
			double min =  *min_element( m_variablesDoub.begin(), m_variablesDoub.end() );
			double max =  *max_element( m_variablesDoub.begin(), m_variablesDoub.end() );

			double length = max - min;
			m_minBin = min - length*0.1;
			m_maxBin = max + length*0.1;
		}
		if ( isInt ) {
			m_minBin = *min_element( m_variablesInt.begin(), m_variablesInt.end() ) - 1;
			m_maxBin = *max_element( m_variablesInt.begin(), m_variablesInt.end() ) + 1;
		}
	}

	// saving directory
	if ( GlobalPar::GetPar()->GetSaveDirHisto( m_name ) != "default" ) 
		m_dir = GlobalPar::GetPar()->GetSaveDirHisto( m_name ); 
	// x axis title
	if ( GlobalPar::GetPar()->GetXTitlesHisto( m_name ) != "default" ) 
		m_titleX = GlobalPar::GetPar()->GetXTitlesHisto( m_name ); 
	// y axis title
	if ( GlobalPar::GetPar()->GetYTitlesHisto( m_name ) != "default" ) 
		m_titleY = GlobalPar::GetPar()->GetYTitlesHisto( m_name ); 


	// init histo
	TH1F* histo = new TH1F( m_name.c_str(), m_title.c_str(), m_nBin, m_minBin, m_maxBin );
	
	// fill histo 
	if (isDoub) {
		for (unsigned int i=0; i<m_variablesDoub.size(); i++) 
			histo->Fill( m_variablesDoub.at(i) );
	}
	if (isInt) {
		for (unsigned int i=0; i<m_variablesInt.size(); i++) 
			histo->Fill( m_variablesInt.at(i) );
	}

	if ( m_titleX != "" )		histo->GetXaxis()->SetTitle( m_titleX.c_str() );
	if ( m_titleY != "" )		histo->GetYaxis()->SetTitle( m_titleY.c_str() );

	m_printoutfile = GlobalPar::GetPar()->IsPrintOutputFile();

	if (!m_printoutfile) { 

		struct stat info;
		if( stat( m_dir.c_str(), &info ) != 0 )		//cannot access
		    system(("mkdir "+m_dir).c_str());

		// save histo
		histo->Draw();
		mirror->SaveAs(  (m_dir + "/" + m_name + ".png").c_str() ); // very slow operation !!!!!!!!!!!!!!!!
	}

	// clear histo ???

};


































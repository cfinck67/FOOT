

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
       cout << "ERROR::GlobalPar::GetPar()  -->  called a get before GlobalPar object istance." << endl, exit(0);

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





























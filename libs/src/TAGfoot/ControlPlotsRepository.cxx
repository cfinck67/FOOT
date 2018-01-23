

#include "ControlPlotsRepository.hxx"


void ControlPlotsRepository::FillMap( string mapName, double x ) {

	if ( m_chizu.find( mapName ) == m_chizu.end() ) {
		m_chizu[ mapName ] = ControlPlotInfo( mapName, x );
	}
	else {
		m_chizu[ mapName ].addInfo( x );
	}

};



void ControlPlotsRepository::PrintMap() {

	TCanvas* mirror = new TCanvas( "mirror", "mirror", 700, 700);
	for ( map< string, ControlPlotInfo >::iterator it = m_chizu.begin(); it != m_chizu.end(); it++ ) {

		(*it).second.PrintHisto( mirror );

	}

};
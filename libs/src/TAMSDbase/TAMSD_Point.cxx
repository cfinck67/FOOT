



#include "TString.h"
#include "TClonesArray.h"

#include "TAMSD_Point.hxx"





ClassImp(TAMSD_Point) // Description of Single Detector TAMSD_Point 




//______________________________________________________________________________
//  build a point
TAMSD_Point::TAMSD_Point( int iCol, TAMSD_Hit* colHit, int iRow, TAMSD_Hit* rowHit )   {
   
	m_columnHitID = iCol;
	m_rowHitID = iRow;

	m_columnHit = colHit;
	m_rowHit = rowHit;

	Initialise();

}





//______________________________________________________________________________
//  
void TAMSD_Point::Initialise() {

	m_posMC = TVector3( 0, 0, 0 );
	m_momMC = TVector3( 0, 0, 0 );

	// take the detector geometry
	m_geometry = (TAMSDparGeo*) gTAGroot->FindParaDsc("TAMSDGeo", "TAMSDparGeo")->Object();

	// set center position
	m_position = TVector3( m_columnHit->GetHitCoordinate_detectorFrame(), m_rowHit->GetHitCoordinate_detectorFrame(), 
                        	( m_columnHit->GetHitZ_detectorFrame() + m_rowHit->GetHitZ_detectorFrame() )/2 );

	m_column = m_columnHit->GetStrip();
	m_row = m_rowHit->GetStrip();

	if ( m_columnHit->GetLayer() == m_rowHit->GetLayer() ) 		cout << "ERROR :: TAMSD_Point::Initialise  -->  colum and row layer doesn't match during point creation!" << endl, exit(0);
	m_layer = m_columnHit->GetLayer();

	m_isMC = ( m_columnHit->GetMCid() == -1 ? false : true );
	m_isTrueGhost = ( m_columnHit->GetGenPartID() == m_rowHit->GetGenPartID() ? false : true );

	m_de = m_columnHit->GetEnergyLoss() + m_rowHit->GetEnergyLoss();
	m_time =  m_columnHit->GetTime();

	if ( !m_isTrueGhost ) {
		m_posMC = ( m_columnHit->GetMCPosition_detectorFrame() + m_rowHit->GetMCPosition_detectorFrame() )*0.5;
		m_momMC = ( m_columnHit->GetMCMomentum_detectorFrame() + m_rowHit->GetMCMomentum_detectorFrame() )*0.5;
	}

}





//______________________________________________________________________________
void TAMSD_Point::TrueGhostWarning() {
	if ( !m_isTrueGhost )	return;
	cout << "WARNING :: TAMSD_Point::TrueGhostWarning()  -->  you are asking for a single MC value of a ghost point."<< endl;
	exit(0);
}



//______________________________________________________________________________
int TAMSD_Point::GetGenPartID() {
	TrueGhostWarning();
	return m_columnHit->GetGenPartID();
}

//______________________________________________________________________________
TAGntuMCeveHit* TAMSD_Point::GetGenParticle() {
	TrueGhostWarning();
	return m_columnHit->GetGenParticle();
}



//______________________________________________________________________________
TVector3 TAMSD_Point::GetPosition_detectorFrame() { 
	// TrueGhostWarning();
	return m_position; 
}


//______________________________________________________________________________
TVector3 TAMSD_Point::GetPosition_footFrame() { 
	// TrueGhostWarning();
	TVector3 glob = m_position;
	m_geometry->Local2Global( &glob ); 
	return glob; 
};


//______________________________________________________________________________
TVector3 TAMSD_Point::GetMCPosition_detectorFrame() { 
	TrueGhostWarning();
	return m_posMC; 
};


//______________________________________________________________________________
TVector3 TAMSD_Point::GetMCPosition_footFrame() { 
	TrueGhostWarning();
	TVector3 glob = m_posMC;
	m_geometry->Local2Global( &glob ); 
	return glob; 
};


//______________________________________________________________________________
TVector3 TAMSD_Point::GetMCMomentum_detectorFrame() { 
	TrueGhostWarning();
	return m_momMC; 
};


//______________________________________________________________________________
TVector3 TAMSD_Point::GetMCMomentum_footFrame() { 
	TrueGhostWarning();
	TVector3 globP = m_momMC;
	m_geometry->Local2Global_RotationOnly( &globP ); 
	return globP; 
};
































#include "TString.h"
#include "TClonesArray.h"

#include "TAGgeoTrafo.hxx"

#include "TATW_Point.hxx"





ClassImp(TATW_Point) // Description of Single Detector TATW_Point 




//______________________________________________________________________________
//  build a point
TATW_Point::TATW_Point( int iCol, TATW_Hit* colHit, int iRow, TATW_Hit* rowHit )   {
   
	m_columnHitID = iCol;
	m_rowHitID = iRow;

	m_columnHit = colHit;
	m_rowHit = rowHit;

	Initialise();

}





//______________________________________________________________________________
//  
void TATW_Point::Initialise() {

	m_posMC = TVector3( 0, 0, 0 );
	m_momMC = TVector3( 0, 0, 0 );

	// take the detector geometry
	m_geometry = (TATWparGeo*) gTAGroot->FindParaDsc("twGeo", "TATWparGeo")->Object();

	// set center position
	m_position = TVector3( m_columnHit->GetHitCoordinate_detectorFrame(), m_rowHit->GetHitCoordinate_detectorFrame(), 
                        	( m_columnHit->GetHitZ_detectorFrame() + m_rowHit->GetHitZ_detectorFrame() )/2 );

	m_column = m_columnHit->GetBar();
	m_row = m_rowHit->GetBar();

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
void TATW_Point::TrueGhostWarning() {
	if ( !m_isTrueGhost )	return;
	cout << "WARNING :: TATW_Point::TrueGhostWarning()  -->  you are asking for a single MC value of a ghost point."<< endl;
	exit(0);
}



//______________________________________________________________________________
int TATW_Point::GetGenPartID() {
	TrueGhostWarning();
	return m_columnHit->GetGenPartID();
}

//______________________________________________________________________________
TAGntuMCeveHit* TATW_Point::GetGenParticle() {
	TrueGhostWarning();
	return m_columnHit->GetGenParticle();
}



//______________________________________________________________________________
TVector3 TATW_Point::GetPosition_detectorFrame() { 
	// TrueGhostWarning();
	return m_position; 
}


//______________________________________________________________________________
TVector3 TATW_Point::GetPosition_footFrame() { 
	// TrueGhostWarning();
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

	TVector3 glob = geoTrafo->FromTWLocalToGlobal( m_position );
	return glob; 
};


//______________________________________________________________________________
TVector3 TATW_Point::GetMCPosition_detectorFrame() { 
	TrueGhostWarning();
	return m_posMC; 
};


//______________________________________________________________________________
TVector3 TATW_Point::GetMCPosition_footFrame() { 
	TrueGhostWarning();
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

   TVector3 glob = geoTrafo->FromTWLocalToGlobal( m_posMC );
	return glob;
};


//______________________________________________________________________________
TVector3 TATW_Point::GetMCMomentum_detectorFrame() { 
	TrueGhostWarning();
	return m_momMC; 
};


//______________________________________________________________________________
TVector3 TATW_Point::GetMCMomentum_footFrame() { 
	TrueGhostWarning();

   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   TVector3 globP = geoTrafo->VecFromTWLocalToGlobal(m_momMC) ;
   
	return globP;
};




























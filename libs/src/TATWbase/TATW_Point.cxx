



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
void TATW_Point::Initialise()
{
	// set center position
	m_position = TVector3( m_columnHit->GetHitCoordinate_detectorFrame(), m_rowHit->GetHitCoordinate_detectorFrame(), 
                        	( m_columnHit->GetHitZ_detectorFrame() + m_rowHit->GetHitZ_detectorFrame() )/2 );

	m_column = m_columnHit->GetBar();
	m_row = m_rowHit->GetBar();

	m_isMC = ( m_columnHit->GetMcTrackId(0) == -1 ? false : true );
   // Not possible in real life !!!!
	//m_isTrueGhost = ( m_columnHit->GetGenPartID() == m_rowHit->GetGenPartID() ? false : true );

	m_de = m_columnHit->GetEnergyLoss() + m_rowHit->GetEnergyLoss();
	m_time =  m_columnHit->GetTime();
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
	return m_columnHit->GetMcTrackId(0);
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

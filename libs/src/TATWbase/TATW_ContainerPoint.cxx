/*!
  \file
  \version $Id: TATW_ContainerPoint.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TATW_ContainerPoint.
*/


/*!
  \class TATW_ContainerPoint TATW_ContainerPoint.hxx "TATW_ContainerPoint.hxx"
  \brief Container class for VTX ntu hit **
*/

////////////////////////////////////////////////////////////
// Class Description of TATW_Point                        //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////


#include "TString.h"
#include "TClonesArray.h"

#include "TATW_ContainerPoint.hxx"




ClassImp(TATW_ContainerPoint);
// TString TATW_ContainerPoint::fgkBranchName   = "vtrh.";




//------------------------------------------+-----------------------------------
//! 
TATW_ContainerPoint::TATW_ContainerPoint() 
: TAGdata(),
  m_listOfPoints(0x0)
{
	m_pointVector.clear();
	m_twGeo = (TATWparGeo*) gTAGroot->FindParaDsc("twGeo", "TATWparGeo")->Object();
	SetupClones();
}





//------------------------------------------+-----------------------------------
//! Destructor.
TATW_ContainerPoint::~TATW_ContainerPoint() {
   delete m_listOfPoints;
}







//______________________________________________________________________________
//  standard 
TATW_Point* TATW_ContainerPoint::NewPoint( int iCol, TATW_Hit* colHit, int iRow, TATW_Hit* rowHit ) {

	
	// check on aorigin
	TClonesArray &pixelArray = *m_listOfPoints;
	TATW_Point* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TATW_Point( iCol, colHit, iRow, rowHit );

	int mask = AlgoColRow( iCol, iRow );

	// fill DegeneratePoint map
	if ( m_degeneratePointMap.find( mask ) == m_degeneratePointMap.end() ) {	// if element doesn't exist
		
		// m_degeneratePointMap[mask].SetPrimaryPoint( pixel );
		m_degeneratePointMap[mask].primaryPoint = pixel;
		m_pointVector.push_back( mask );

	}
	else if ( !pixel->IsTrueGhost() ) {
		
		// m_degeneratePointMap[mask].SetPrimaryPoint( pixel );
		m_degeneratePointMap[mask].primaryPoint = pixel;

	}

	// m_degeneratePointMap[mask].AddPoint( pixel );
	m_degeneratePointMap[mask].allPoints.push_back( pixel );

	return pixel;


}



//------------------------------------------+-----------------------------------
int TATW_ContainerPoint::AlgoColRow( int col, int row ) {
	return (col * m_twGeo->GetNBars()) + row;
}



//------------------------------------------+-----------------------------------
int TATW_ContainerPoint::GetPointN() { 
	// DegeneratePoint dimension
	return m_degeneratePointMap.size();
}



//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
TATW_Point* TATW_ContainerPoint::GetPoint( int iPoint ) {

	if ( m_degeneratePointMap.size() != m_pointVector.size() ) 	{	// sanity check
		cout << "ERROR >> TATW_ContainerPoint::GetPoint  -->  different size of m_degeneratePointMap "<<m_degeneratePointMap.size()<<" and m_pointVector.size() " << m_pointVector.size() << endl;
        exit(0);
	}

	if ( iPoint < 0  || iPoint >= GetPointN() ) {
		cout << "ERROR >> TATW_ContainerPoint::GetPoint  -->  number of point "<<iPoint<<" required is wrong. Max num  " << m_degeneratePointMap.size() << endl;
        exit(0);
	}

	// from DegeneratePoint
	// return m_degeneratePointMap[ m_pointVector.at(iPoint) ].GetPrimaryPoint();
	return m_degeneratePointMap[ m_pointVector.at(iPoint) ].primaryPoint;
}




//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
vector<TATW_Point*> TATW_ContainerPoint::GetPoint_AllTheDegenerate( int iPoint ) {

	if ( m_degeneratePointMap.size() != m_pointVector.size() ) 	{	// sanity check
		cout << "ERROR >> TATW_ContainerPoint::GetPoint  -->  different size of m_degeneratePointMap "<<m_degeneratePointMap.size()<<" and m_pointVector.size() " << m_pointVector.size() << endl;
        exit(0);
	}

	if ( iPoint < 0  || iPoint >= GetPointN() ) {
		cout << "ERROR >> TATW_ContainerPoint::GetPoint  -->  number of point "<<iPoint<<" required is wrong. Max num  " << m_degeneratePointMap.size() << endl;
        exit(0);
	}

	// from DegeneratePoint
	// return m_degeneratePointMap[ m_pointVector.at(iPoint) ].GetAllPoints();
	return m_degeneratePointMap[ m_pointVector.at(iPoint) ].allPoints;
}




//------------------------------------------+-----------------------------------
int TATW_ContainerPoint::GetPointN_includingDuplicates() {
	return m_listOfPoints->GetEntries();   
}



//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
TATW_Point* TATW_ContainerPoint::GetPoint_includingDuplicates( int iPoint ) {

	if ( iPoint < 0  || iPoint >= GetPointN_includingDuplicates() ) {
		cout << "ERROR >> TATW_ContainerPoint::GetPoint_includingDuplicates  -->  number of point "<<iPoint<<" required is wrong. Max num  " << GetPointN_includingDuplicates() << endl;
        exit(0);
	}

	return (TATW_Point*)m_listOfPoints->At( iPoint );
}



//------------------------------------------+-----------------------------------
//! Setup clones. Crate and initialise the list of pixels
void TATW_ContainerPoint::SetupClones()	{
   if (m_listOfPoints) return;
   m_listOfPoints = new TClonesArray("TATW_Point", 500);
}





//------------------------------------------+-----------------------------------
//! Clear event.
void TATW_ContainerPoint::Clear(Option_t*) {
	m_listOfPoints->Delete();   
	m_listOfPoints->Clear();
	// m_listOfPoints->Clear("C");
}





/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TATW_ContainerPoint::ToStream(ostream& os, Option_t* option) const
{
   // for (Int_t i = 0; i < m_vtxGeo->GetNSensors(); ++i) {
	  
	  // os << "TATW_ContainerPoint " << GetName()
	  // << Form("  nPixels=%3d", GetPixelsN(i))
	  // << endl;
	  
	  // //TODO properly
	  // //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
	  // for (Int_t j = 0; j < GetPixelsN(i); j++) {  // all by default
		 // const TATW_Point*  pixel = GetPixel(i,j, "all");
		 // if (pixel)
			// os << Form("%4d", pixel->GetPixelIndex());
		 // os << endl;
	  // }
   // }
}






























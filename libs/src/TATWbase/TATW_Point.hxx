
#ifndef _TATW_Point_HXX
#define _TATW_Point_HXX

// ROOT classes
#include "TObject.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TVector3.h"

#include "TATWdatRaw.hxx"
#include "TATW_Hit.hxx"

// all 3 needed to take from gTagROOT
#include "TAGroot.hxx"
#include "TATWparGeo.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

#include "TAGntuMCeve.hxx"
// #include "TAGntuMCeveHit.hxx"


// class TAVTrawHit;

/** TATW_Point class is the scintillator hit reconstructed by the intersection of 2 hits on a 
	column and a row bar respectively
 
	Created in 2018 by Matteo Franchini franchinim@bo.infn.it

	All the coordinates are in cm and in the detector reference frame, i.e. the center
	is the center of the detector.

*/
/*------------------------------------------+---------------------------------*/



class TATW_Point : public TObject {
   
private:

	TATWparGeo* m_geometry;

	TVector3 m_position;
	int m_column;
	int m_row;

	int m_columnHitID;
	int m_rowHitID;

	TATW_Hit* m_columnHit;
	TATW_Hit* m_rowHit;

	bool m_isMC;
	bool m_isTrueGhost;
	// float m_isGhost;

	double m_de;                          // energy loss in the scintillator bar
	double m_time;                        // for the moment I take the column time

	// currently the exists for the non-TrueGhost
	// for the non-TrueGhost, average of the col and row values
	// maybe I should do instead? :
	// for the trueGhost, pos = col_MCx, row_MCy, average of z
	// for the trueGhost, mom = MC momentum average
	TVector3 m_posMC;                     // MC hit position = track hitting the scint.
	TVector3 m_momMC;                     // MC momentum of the hit

	void TrueGhostWarning();



public:

	TATW_Point() {};
	TATW_Point( int iCol, TATW_Hit* colHit, int iRow, TATW_Hit* rowHit );
	~TATW_Point() {};

	void Initialise();


	// all the Set methods 
	// void SetIsGhost();


	//    All the Get methods
	TVector3 GetPosition_detectorFrame();
	TVector3 GetPosition_footFrame();

	int GetColumn()         { return m_column; };
	int GetRow()            { return m_row; };

	bool IsMC()             { return m_isMC; };
	bool IsTrueGhost()      { return m_isTrueGhost; };

	int GetColumnHitID()     { return m_columnHitID; };
	int GetRowHitID()        { return m_rowHitID; };

	TATW_Hit* GetColumnHit()		{ return m_columnHit; };
	TATW_Hit* GetRowHit()			{ return m_rowHit; };

	double GetEnergyLoss()          { return m_de; };
	double GetTime()                { return m_time; };

	// float IsGhost()         { return m_isGhost; };

	int                 GetGenPartID();
    TAGntuMCeveHit*     GetGenParticle();

	TVector3          GetMCPosition_detectorFrame();
	TVector3          GetMCPosition_footFrame();

	TVector3           GetMCMomentum_detectorFrame();
	TVector3           GetMCMomentum_footFrame();

	
	ClassDef(TATW_Point,3)                            
};

//##############################################################################



#endif
























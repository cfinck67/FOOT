#ifndef TACA_Hit_HXX
#define TACA_Hit_HXX

// ROOT classes
#include "TObject.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TVector3.h"

#include "TACAdatRaw.hxx"

// all 3 needed to take from gTagROOT
#include "TAGroot.hxx"
#include "TACAparGeo.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

//#include "TAGntuMCeve.hxx"



/** TACA_Hit class contains information respect to the BGO calorimeter:
 index, position, noise, pulse height, size, etc...
 
    Created by 	Lorenzo Scavarda in 2018-2019
    Contact:    Matteo Franchini franchinim@bo.infn.it

    All the coordinates are in cm and in the detector reference frame, i.e. the center
    is the center of the detector.
*/
/*------------------------------------------+---------------------------------*/



class TACA_Hit : public TObject {
   
private:

	TACAparGeo* m_geometry;

    int m_BGOx;                         // coord x of BGO
    int m_BGOy;                         // coord y of BGO
    int m_BGOz; 						// coord z of BGO
    int m_BGO;                          // BGO ID
	int m_indexMC;                      // MC index of the hit in the ntuple
	
    double m_enLoss;                    // energy loss in the BGO crystals
    double m_time;                        

   	TVector3 m_coordinate;              // x and y coordinates in the local detector frame    
    TVector3 m_posMC;                   // MC hit position = track hitting the scint.
	TVector3 m_momMC;                   // MC momentum of the hit

    // int m_genPartIndex;
    // TAGntuMCeveHit* m_genPartPointer;

	

public:

	TACA_Hit() {};
	TACA_Hit( TACArawHit* hit );
	TACA_Hit( int id_BGO , double enLoss , double time );     
	~TACA_Hit() {};

	void Initialise();

    // all the Set methods 
    void               SetMCPosition(TVector3 a_pos)   { m_posMC = a_pos;          }        //! Set MC truth position
    void               SetMCMomentum(TVector3 a_mom)   { m_momMC = a_mom;            }      //! Set MC truth momentum

    // void               SetGenPartID( int agenPartID );  // also the m_genPartPointer

    //    All the Get methods
    int                 GetX_BGO()            { return  m_BGOx; }       // 	return the BGO in x position
    int                 GetY_BGO()            { return  m_BGOy; } 		// 	return the BGO in y position
    int                 Get_idBGO()           { return  m_BGO; }       	//	return the BGO id
    
    int                 GetMCid()           { return  m_indexMC; };     //MC index of the hit in the ntuple
    // int                 GetGenPartID()      { return m_genPartIndex; };
    // TAGntuMCeveHit*     GetGenParticle()    { return m_genPartPointer; };

    
    // TVector3            GetHitCoordinate_sensorFrame()        { return m_geometry->GetCoordiante_sensorFrame( m_BGO ); };
    // TVector3            GetHitCoordinate_detectorFrame()      { return m_geometry->GetCoordiante_detectorFrame( m_BGO ); };
    // float            GetHitCoordinate_footFrame()          { return m_geometry->GetCoordiante_footFrame( m_BGOx, m_BGOy , m_BGOz ); };
       
    TVector3            GetMCPosition_sensorFrame();
    TVector3            GetMCPosition_detectorFrame()      { return  m_posMC; }
    TVector3            GetMCPosition_footFrame();

    // TVector3         GetMCMomentum_sensorFrame()        { return  m_momMC;            }
    TVector3            GetMCMomentum_detectorFrame()      { return  m_momMC;            }
    TVector3            GetMCMomentum_footFrame();

    
    double              GetEnergyLoss()                { return m_enLoss; };
    double              GetTime()                      { return m_time; };

    ClassDef(TACA_Hit,1)                        
};

//##############################################################################



#endif
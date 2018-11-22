
#ifndef _TAMSD_Hit_HXX
#define _TAMSD_Hit_HXX

// ROOT classes
#include "TObject.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TVector3.h"

#include "TAMSDdatRaw.hxx"

// all 3 needed to take from gTagROOT
#include "TAGroot.hxx"
#include "TAMSDparGeo.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

#include "TAGntuMCeve.hxx"




/** TAMSD_Hit class contains information respect to a scintillator in cmos detectors
 index, position, noise, pulse height, size, etc...
 
    Created by Serena Valle in 2018
    Contact:    Matteo Franchini franchinim@bo.infn.it

    All the coordinates are in cm and in the detector reference frame, i.e. the center
    is the center of the detector.

    layer 0 -->  column, asse X
    layer 1 -->  row, asse Y

*/
/*------------------------------------------+---------------------------------*/



class TAMSD_Hit : public TObject {
   
private:

	TAMSDparGeo* m_geometry;

    int m_sensor;
    int m_layer;
    int m_view;
    int m_strip;
	int m_indexMC;                        // MC index of the hit in the ntuple
	
    double m_de;                          // energy loss in the scintillator bar
    double m_time;                        

    float m_coordinate;                   // x or y coordinate in the local detector frame, depending on the layer
    float m_z;                            // z coordinate in the local detector frame

    TVector3 m_posMC;                     // MC hit position = track hitting the scint.
	TVector3 m_momMC;                     // MC momentum of the hit

    int m_genPartIndex;
    TAGntuMCeveHit* m_genPartPointer;

	

public:

	TAMSD_Hit() {};
	TAMSD_Hit( TAMSDrawHit* hit );
	TAMSD_Hit ( int aSensor, int aStrip, double aDe, double aTime, int aID, int parentID );
	~TAMSD_Hit() {};

	void Initialise();

    bool IsColumn() { return ( m_layer == 0 ? true : false ); };
    bool IsRow() { return ( m_layer == 1 ? true : false ); };

    // all the Set methods 
    void               SetMCPosition(TVector3 a_pos)   { m_posMC = a_pos;          }    //! Set MC truth position
    void               SetMCMomentum(TVector3 a_mom)   { m_momMC = a_mom;            }    //! Set MC truth momentum

    void               SetGenPartID( int agenPartID );  // also the m_genPartPointer


    //    All the Get methods
    int              GetStrip()                { return  m_strip; }
    int              GetLayer()                { return  m_layer; }
    int              GetSensor()               { return  m_sensor; }
    
    int                 GetMCid()           { return  m_indexMC; };  //MC index of the hit in the ntuple
    int                 GetGenPartID()      { return m_genPartIndex; };
    TAGntuMCeveHit*     GetGenParticle()    { return m_genPartPointer; };

    
    float          GetHitCoordinate_sensorFrame()        { return m_geometry->GetStripCoordiante_sensorFrame( m_layer, m_view, m_strip ); };
    float          GetHitCoordinate_detectorFrame()      { return m_coordinate; };
    float          GetHitCoordinate_footFrame()          { return m_geometry->GetStripCoordiante_footFrame( m_layer, m_view, m_strip ); };
    
    float          GetHitZ_sensorFrame()        { return m_geometry->GetStripZ_sensorFrame( m_layer, m_view, m_strip ); };
    float          GetHitZ_detectorFrame()      { return m_z; };
    float          GetHitZ_footFrame()          { return m_geometry->GetStripZ_footFrame( m_layer, m_view, m_strip ); };
    
    TVector3          GetMCPosition_sensorFrame();
    TVector3          GetMCPosition_detectorFrame()      { return  m_posMC;            }
    TVector3          GetMCPosition_footFrame();

    // TVector3          GetMCMomentum_sensorFrame()        { return  m_momMC;            }
    TVector3           GetMCMomentum_detectorFrame()      { return  m_momMC;            }
    TVector3           GetMCMomentum_footFrame();

    
    double           GetEnergyLoss()                { return m_de; };
    double           GetTime()                      { return m_time; };

    ClassDef(TAMSD_Hit,3)                            // Pixel or Pixel of a Detector Plane
};

//##############################################################################



#endif
























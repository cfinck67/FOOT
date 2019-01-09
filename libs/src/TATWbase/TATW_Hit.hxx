
#ifndef _TATW_Hit_HXX
#define _TATW_Hit_HXX

// ROOT classes
#include "TObject.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TVector3.h"

#include "TATWdatRaw.hxx"

// all 3 needed to take from gTagROOT
#include "TAGroot.hxx"
#include "TATWparGeo.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

#include "TAGntuMCeve.hxx"




/** TATW_Hit class contains information respect to a scintillator in cmos detectors
 index, position, noise, pulse height, size, etc...
 
    Created by Serena Valle in 2018
    Contact:    Matteo Franchini franchinim@bo.infn.it

    All the coordinates are in cm and in the detector reference frame, i.e. the center
    is the center of the detector.

    layer 0 -->  column, asse X
    layer 1 -->  row, asse Y

*/
/*------------------------------------------+---------------------------------*/



class TATW_Hit : public TObject {
   
private:
   static const Int_t fgkMAXTRACK = 10;

    Int_t m_layer;
    Int_t m_bar;
	
    Double_t m_de;                          // energy loss in the scintillator bar
    Double_t m_time;                        

    Float_t m_coordinate;                   // x or y coordinate in the local detector frame, depending on the layer
    Float_t m_z;                            // z coordinate in the local detector frame

    Int_t   m_McTrackCount;                 // Variable that count the number of times a crystal is touched
    Int_t   m_McTrackId[fgkMAXTRACK];         // Id of the track created in the simulation

   TVector3 m_posMC;                     // MC hit position = track hitting the scint.

public:
	TATW_Hit() {};
	TATW_Hit( TATWrawHit* hit );
	TATW_Hit ( Int_t aView, Int_t aBar, Double_t aDe, Double_t aTime);
	~TATW_Hit() {};

	void Initialise();

   bool IsColumn() { return ( m_layer == 0 ? true : false ); };
   bool IsRow()    { return ( m_layer == 1 ? true : false ); };

   //    All the Get methods
   Int_t     GetBar()                  const   { return  m_bar;              }
   Int_t     GetLayer()                const   { return  m_layer;            }
   
   Double_t  GetEnergyLoss()           const   { return m_de;                }
   Double_t  GetTime()                 const   { return m_time;              }
   
   Float_t    GetHitCoordinate_detectorFrame() const   { return m_coordinate; }
   Float_t    GetHitZ_detectorFrame()          const   { return m_z;          }

   // MC track id
   Int_t      GetTrackIdMc(Int_t index) const   { return m_McTrackId[index];  }
   Int_t      GetMcTrackCount()         const   { return m_McTrackCount;      }
   
   // Add MC track Id
   void       AddMcTrackId(Int_t trackId);

   void       SetMCPosition(TVector3 a_pos)       { m_posMC = a_pos;          }    //! Set MC truth position
   TVector3   GetMCPosition_detectorFrame() const { return  m_posMC;          }

    ClassDef(TATW_Hit,3)                            // Pixel or Pixel of a Detector Plane
};

//##############################################################################



#endif
























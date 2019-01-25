
#ifndef _TATW_Hit_HXX
#define _TATW_Hit_HXX

// ROOT classes
#include "TArrayC.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TVector3.h"

#include "TAGobject.hxx"
#include "TATWdatRaw.hxx"

// all 3 needed to take from gTagROOT
#include "TAGroot.hxx"
#include "TATWparGeo.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"


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



class TATW_Hit : public TAGobject {
   
private:
    Int_t m_layer;
    Int_t m_bar;
	
    Double32_t m_de;                     // energy loss in the scintillator bar
    Double32_t m_time;                   // time of flight

    Double32_t m_coordinate;                // x or y coordinate in the local detector frame, depending on the layer
    Float_t m_z;                         // z coordinate in the local detector frame

   TArrayC   m_MCindex;                  // Id of the hit created in the simulation
   TArrayC   m_McTrackId;                // Id of the track created in the simulation
   
public:
	TATW_Hit() {};
	TATW_Hit( TATWrawHit* hit );
	TATW_Hit ( Int_t aView, Int_t aBar, Double_t aDe, Double_t aTime, Double_t pos);
   TATW_Hit(const TATW_Hit& aHit);

	~TATW_Hit() {};
   
   void   Clear(Option_t* option = "C");

   bool IsColumn() { return ( m_layer == 0 ? true : false ); };
   bool IsRow()    { return ( m_layer == 1 ? true : false ); };

   //    All the Get methods
   Int_t     GetBar()                  const   { return  m_bar;              }
   Int_t     GetLayer()                const   { return  m_layer;            }
   
   Double_t  GetEnergyLoss()           const   { return m_de;                }
   Double_t  GetTime()                 const   { return m_time;              }
   Double_t  GetPosition()             const   { return m_coordinate;        }
   
   Float_t    GetHitCoordinate_detectorFrame() const   { return m_coordinate; }
   Float_t    GetHitZ_detectorFrame()          const   { return m_z;          }

   // MC track id
   Int_t      GetMcIndex(Int_t index)   const   { return m_MCindex[index];      }
   Int_t      GetMcTrackId(Int_t index) const   { return m_McTrackId[index];    }
   Int_t      GetMcTrackCount()         const   { return m_McTrackId.GetSize(); }
   
   // Add MC track Id
   void       AddMcTrackId(Int_t trackId, Int_t mcId = -1);

    ClassDef(TATW_Hit,3)                            // Pixel or Pixel of a Detector Plane
};

//##############################################################################



#endif
























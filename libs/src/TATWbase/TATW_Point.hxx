
#ifndef _TATW_Point_HXX
#define _TATW_Point_HXX

// ROOT classes
#include "TVector3.h"

#include "TAGobject.hxx"
#include "TATW_Hit.hxx"


/** TATW_Point class is the scintillator hit reconstructed by the intersection of 2 hits on a 
	column and a row bar respectively
 
	Created in 2018 by Matteo Franchini franchinim@bo.infn.it
 
   Complete revised to be compliant with foot framework and raw data.
   Copy hits in point not just assigning pointers
   by Ch. Finck cfinck@iphc.cnrs.fr

	All the coordinates are in cm and in the detector reference frame, i.e. the center
	is the center of the detector.

*/
/*------------------------------------------+---------------------------------*/

class TATW_Point : public TAGobject {
   
private:
   TVector3    m_position;      // position in detector framework
	int         m_column;        // column number
	int         m_row;           // row number

	TATW_Hit*   m_columnHit;     // hit col
	TATW_Hit*   m_rowHit;        // hit row 

	Double32_t  m_de;            // energy loss in the scintillator bars
	Double32_t  m_time;          // for the moment I take the column time

public:

   TATW_Point();
	TATW_Point( double x, TATW_Hit* colHit, double y, TATW_Hit* rowHit );
	~TATW_Point() {};
   
	//    All the Get methods
   TVector3  GetPosition()    const  { return m_position;            }

	int       GetColumnID()    const  { return m_column;              }
	int       GetRowID()       const  { return m_row;                 }

	int       GetColumn()      const  { return m_columnHit->GetBar(); }
	int       GetRow()         const  { return m_rowHit->GetBar();    }

	TATW_Hit* GetColumnHit()	const	 { return m_columnHit;           }
   TATW_Hit* GetRowHit()	   const	 { return m_rowHit;              }

	double    GetEnergyLoss()  const  { return m_de;                  }
	double    GetTime()        const  { return m_time;                }
   
   void      Clear(Option_t* opt);

	ClassDef(TATW_Point,3)                            
};

//##############################################################################



#endif
























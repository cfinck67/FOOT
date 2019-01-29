#ifndef _TATW_ContainerPoint_HXX
#define _TATW_ContainerPoint_HXX
/*!
 \file
 \version $Id: TATW_ContainerPoint.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
 \brief   Declaration of TATW_ContainerPoint.
 */
/*------------------------------------------+---------------------------------*/


#include "TATW_Point.hxx"

#include "TAGdata.hxx"
#include <map>


class TATW_ContainerPoint : public TAGdata {
   
private:
   
   TClonesArray*        m_listOfPoints;

	TATWparGeo* m_twGeo; //! do not save in file
   
public:

	TATW_ContainerPoint();
	virtual ~TATW_ContainerPoint();
	
	TATW_Point*         NewPoint( double x, TATW_Hit* colHit, double y, TATW_Hit* rowHit );

	int                 GetPointN();
	TATW_Point*         GetPoint( int iPoint );


	virtual void        Clear(Option_t* opt="");

	// delete?
	virtual void        ToStream(ostream& os=cout, Option_t* option="") const;
 
   virtual void        SetupClones();

public:
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }
   
private:
   static TString fgkBranchName;    // Branch name in TTree
   
   ClassDef(TATW_ContainerPoint,1)
};

#endif




















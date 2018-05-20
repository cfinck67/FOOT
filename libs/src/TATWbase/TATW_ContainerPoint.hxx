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
   

	// class DegeneratePoint {
	// public: 
	// 	DegeneratePoint() {};
	// 	~DegeneratePoint() {};

	// 	void SetPrimaryPoint( TATW_Point* point )		{primaryPoint = point;};
	// 	void AddPoint( TATW_Point* point )				{allPoints.push_back(point);};
	// 	TATW_Point* GetPrimaryPoint()					{return primaryPoint;};
	// 	vector<TATW_Point*> GetAllPoints()				{return allPoints;};

	// 	TATW_Point* primaryPoint;
	// 	vector<TATW_Point*> allPoints;
	// };

	// struct DegeneratePoint {
	// 	TATW_Point* primaryPoint;
	// 	vector<TATW_Point*> allPoints;
	// };



	TClonesArray*        m_listOfPoints; 
	// static TString    fgkBranchName;    // Branch name in TTree

	TATWparGeo* m_twGeo;

	// vector<int> m_pointVector;
	// map<int, DegeneratePoint> m_degeneratePointMap;


	virtual void      SetupClones();



public:

	struct DegeneratePoint {
		TATW_Point* primaryPoint;
		vector<TATW_Point*> allPoints;
	};

	vector<int> m_pointVector;
	map<int, DegeneratePoint> m_degeneratePointMap;


	TATW_ContainerPoint();
	virtual ~TATW_ContainerPoint();
	
	TATW_Point*         NewPoint( int iCol, TATW_Hit* colHit, int iRow, TATW_Hit* rowHit );

	int                 GetPointN(); 
	TATW_Point*         GetPoint( int iPixel );
	vector<TATW_Point*> GetPoint_AllTheDegenerate( int iPoint );

	int GetPointN_includingDuplicates();
	TATW_Point* GetPoint_includingDuplicates( int iPoint );

	int AlgoColRow( int col, int row );

	virtual void        Clear(Option_t* opt="");

	// delete?
	virtual void        ToStream(ostream& os=cout, Option_t* option="") const;
 
   // static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }
   
   ClassDef(TATW_ContainerPoint,1)
};

#endif




















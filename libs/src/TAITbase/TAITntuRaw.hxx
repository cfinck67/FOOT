#ifndef _TAITntuRaw_HXX
#define _TAITntuRaw_HXX
/*!
 \file
 \version $Id: TAITntuRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
 \brief   Declaration of TAITntuRaw.
 */
/*------------------------------------------+---------------------------------*/


#include "TAITntuHit.hxx"

#include "TAGdata.hxx"
#include <map>

/*

mc_hit      = only pixels coming from FLUKA ntuples (now just one hit per particle track)
mc_cluster  = only px created in digitization from the mc_hit (original px coordinate already in, but it is a different object)
noise       = only noise px signal
pileup      = only pileup pixel signal
forPhys     = mc_cluster + noise + pileup 
all 

*/



class TAITntuRaw : public TAGdata {
   
private:
	//using TObjArray here
	TObjArray*        m_listOfPixels; 
	// static TString    fgkBranchName;    // Branch name in TTree
	

	TAITparGeo* m_itxGeo;
   
	// vector of map of < Nsensor, listID >
    map<int, vector<int> >      m_mcHitList;
    map<int, vector<int> >      m_mcClusterList;
    map<int, vector<int> >      m_noiseList;
    map<int, vector<int> >      m_pileUpList;

    // map< map<int, vector<int> > >       m_pixelIndexList;

    int               GetPixelsN(int iSensor) const; 
    // TAITntuHit*       NewPixel(Int_t sensor, Double_t value, Int_t aLine, Int_t aColumn); //  Deprecated, to be private



public:
	TAITntuRaw();
	virtual          ~TAITntuRaw();

	TAITntuHit*         NewPixel(Int_t sensor, TAITrawHit* pixel);

	TAITntuHit*         NewPixel(int iSensor, double value, int aLine, int aColumn, string aorigin);
    TAITntuHit*         NewPixel(int iSensor, double value, int aLine, int aColumn, string aorigin, int amcID, int agenPartID);
    TAITntuHit*         NewPixel(int iSensor, double value, int aLine, int aColumn, string aorigin, TAITntuHit* originatingHit );


	void                FillPixelList(int iSensor, string command, int id );

    TAITntuHit*       GetPixel(int iSensor, int iPixel, string command );
    const TAITntuHit* GetPixel(int iSensor, int iPixel, string command ) const;

    TClonesArray*     GetListOfPixels(int iSensor);
    TClonesArray*     GetListOfPixels(int iSensor) const;

    int               GetPixelsN( int iSensor, string command ); 
    int               GetNSensors() { return m_itxGeo->GetNSensors(); };

	virtual void      SetupClones();

	virtual void      Clear(Option_t* opt="");

	virtual void      ToStream(ostream& os=cout, Option_t* option="") const;


   // static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }
   
   ClassDef(TAITntuRaw,1)
};

#endif

































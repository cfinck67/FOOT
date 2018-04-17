#ifndef _TAVTntuRaw_HXX
#define _TAVTntuRaw_HXX
/*!
 \file
 \version $Id: TAVTntuRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
 \brief   Declaration of TAVTntuRaw.
 */
/*------------------------------------------+---------------------------------*/


#include "TAVTntuHit.hxx"

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


class TAVTntuRaw : public TAGdata {
   
private:
   //using TObjArray here
   TObjArray*        fListOfPixels; 
   static TString    fgkBranchName;    // Branch name in TTree
   
   TAVTparGeo* m_vtxGeo;

    // vector of map of < Nsensor, listID >
    map<int, vector<int> >      m_mcHitList;
    map<int, vector<int> >      m_mcClusterList;
    map<int, vector<int> >      m_noiseList;
    map<int, vector<int> >      m_pileUpList;

    // map< map<int, vector<int> > >       m_pixelIndexList;


   // TAVTntuHit*       AddPixel(int iSensor, double value, int aLine, int aColumn);
    int               GetPixelsN(int iSensor) const; 

public:
    TAVTntuRaw();
    virtual          ~TAVTntuRaw();

    // to be put private!
    TAVTntuHit*         NewPixel(Int_t sensor, Double_t value, Int_t aLine, Int_t aColumn);
    TAVTntuHit*         NewPixel(Int_t sensor, TAVTrawHit* pixel);

    TAVTntuHit*         NewPixel(int iSensor, double value, int aLine, int aColumn, string aorigin);
    TAVTntuHit*         NewPixel(int iSensor, double value, int aLine, int aColumn, string aorigin, int amcID, int agenPartID);
    TAVTntuHit*         NewPixel(int iSensor, double value, int aLine, int aColumn, string aorigin, TAVTntuHit* originatingHit );

    void                FillPixelList(int iSensor, string command, int id );

    TAVTntuHit*       GetPixel(Int_t iSensor, Int_t iPixel, string command );
    const TAVTntuHit* GetPixel(Int_t iSensor, Int_t iPixel, string command ) const;

    TClonesArray*     GetListOfPixels(Int_t iSensor);
    TClonesArray*     GetListOfPixels(Int_t iSensor) const;

    int               GetPixelsN( int iSensor, string command ); 
    int               GetNSensors() { return m_vtxGeo->GetNSensors(); };


    virtual void      SetupClones();

    virtual void      Clear(Option_t* opt="");

    virtual void      ToStream(ostream& os=cout, Option_t* option="") const;
 
public:   
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }
   
   ClassDef(TAVTntuRaw,1)
};

#endif

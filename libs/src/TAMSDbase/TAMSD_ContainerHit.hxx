#ifndef _TAMSD_ContainerHit_HXX
#define _TAMSD_ContainerHit_HXX
/*!
 \file
 \version $Id: TAMSD_ContainerHit.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
 \brief   Declaration of TAMSD_ContainerHit.
 */
/*------------------------------------------+---------------------------------*/


#include "TAMSD_Hit.hxx"

#include "TAGdata.hxx"
#include <map>







class TAMSD_ContainerHit : public TAGdata {
   
private:
    //using TObjArray here
    TObjArray*        m_listOfHits; 
    // static TString    fgkBranchName;    // Branch name in TTree

    TAMSDparGeo* m_msdGeo;

    // virtual void      SetupClones();
    // TClonesArray*     GetListOfHits( int layer );

public:

    TAMSD_ContainerHit();
    virtual          ~TAMSD_ContainerHit();

    TAMSD_Hit*         NewHit( int sensor, int strip, double energyLoss, double time, int ntupID, int parentID );
    TAMSD_Hit*         NewHit( TAMSDrawHit* hit );
    
    int               GetNSensors() { return m_msdGeo->GetNSensors(); };
    int               GetHitN(  int sensor  ); 
    TAMSD_Hit*        GetHit(  int sensor, int hitID );
    
    
    virtual void      SetupClones();
    TClonesArray*     GetListOfHits( int sensor );

    virtual void      Clear(Option_t* opt="");

    // delete?
    virtual void      ToStream(ostream& os=cout, Option_t* option="") const;
   
   // static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }
   
   ClassDef(TAMSD_ContainerHit,1)
};

#endif













#ifndef _TATW_ContainerHit_HXX
#define _TATW_ContainerHit_HXX
/*!
 \file
 \version $Id: TATW_ContainerHit.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
 \brief   Declaration of TATW_ContainerHit.
 */
/*------------------------------------------+---------------------------------*/


#include "TATW_Hit.hxx"

#include "TAGdata.hxx"

class TATW_ContainerHit : public TAGdata {
   
private:
    TObjArray*        m_listOfHits;
    TATWparGeo*       m_twGeo;

public:
    TATW_ContainerHit();
    virtual          ~TATW_ContainerHit();

    TATW_Hit*         NewHit( int layer, int bar, double energyLoss, double time, int ntupID, int parentID );
    TATW_Hit*         NewHit( TATWrawHit* hit );
    
    int               GetHitN(  int layer  ); 
    TATW_Hit*         GetHit(  int layer, int hitID );
    
    
    virtual void      SetupClones();
    TClonesArray*     GetListOfHits( int layer );

    virtual void      Clear(Option_t* opt="");

    // delete?
    virtual void      ToStream(ostream& os=cout, Option_t* option="") const;
   
private:
   static TString     fgkBranchName;    // Branch name in TTree
   
public:
   static const Char_t* GetBranchName()   { return fgkBranchName.Data(); }
   
   ClassDef(TATW_ContainerHit,1)
};

#endif













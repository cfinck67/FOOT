#ifndef TACA_ContainerHit_HXX
#define TACA_ContainerHit_HXX
/*!
 \file
 \version $Id: TATW_ContainerHit.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
 \brief   Declaration of TATW_ContainerHit.
 */
/*------------------------------------------+---------------------------------*/


#include "TACA_Hit.hxx"

#include "TAGdata.hxx"
#include <map>

   
      


class TACA_ContainerHit : public TAGdata {
   
private:
    //using TObjArray here
    TObjArray*        m_listOfHits; 
    // static TString    fgkBranchName;    // Branch name in TTree

    TACAparGeo* m_caGeo;

    // virtual void      SetupClones();
    // TClonesArray*     GetListOfHits( int layer );

public:

    TACA_ContainerHit();
    virtual             ~TACA_ContainerHit();

    TACA_Hit*           NewHit( int id_BGO , double enLoss , double time );
    TACA_Hit*           NewHit( TACArawHit* hit );
    
    int                 GetHitN();
    TACA_Hit*           GetHitBGO( int hitID );
    TClonesArray*       GetListOfHits();
    virtual void        SetupClones();
    

    virtual void        Clear(Option_t* opt=""){};

    // delete?
    virtual void        ToStream(ostream& os=cout, Option_t* option="") const {};
   
   // static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }
   
   ClassDef(TACA_ContainerHit,1)
};

#endif






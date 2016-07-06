#ifndef _TAGntuMCeve_HXX
#define _TAGntuMCeve_HXX
/*!
  \file
  \version $Id: TAGntuMCeve.hxx,v 1.7 2003/06/22 11:48:38 mueller Exp $
  \brief   Declaration of TAGntuMCeve.
*/
/*------------------------------------------+---------------------------------*/

#include "TObject.h"
#include "TVector3.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"


class TAGntuMCeveHit : public TObject {
  public:
                    TAGntuMCeveHit();

  // VM changed 17/11/13 to add information for simulate pile-up events
                    TAGntuMCeveHit(Int_t i_id, Int_t i_chg, Int_t i_type, 
                                   Int_t i_reg, Int_t i_bar, Int_t i_dead,
				   Double_t i_mass, TVector3 i_ipos, 
				   TVector3 i_fpos, TVector3 i_imom,
				   Int_t i_mid, TVector3 i_mothmom,
				   Int_t i_pileup);

    virtual         ~TAGntuMCeveHit();

    TVector3         InitPos();
    TVector3         InitP();
    TVector3         FinalPos();
    Double_t         Mass();
    Int_t            Type();
    Int_t            Reg();
    Int_t            Bar();
    Int_t            ID();
    Int_t            Chg();
    Int_t            Dead();
    TVector3         MotherP();
    Int_t            MotherID();
    Int_t            PileUp();    // VM 17/11/13

    void  SetInitPos(TVector3 pos);
    void  SetInitP(TVector3 mom);
    void  SetMotherP(TVector3 mom);
    void  SetFinalPos(TVector3 pos);
    void  SetMass(double amass);
    void  SetID(int aid);
    void  SetDead(int adead);
    void  SetReg(int areg);
    void  SetBar(int abar);
    void  SetType(int atyp);
    void  SetChg(int achg);
    void  SetMotherID(int aid);
  void  SetPileUp(int pup);   // VM 17/11/13


    Int_t         id;                      // identity
    Int_t         chg;                     // charge
    Int_t         reg;                     // region
    Int_t         bar;                     // barionic number
    Int_t         type;                    // Type
    Int_t         dead;                    // region in whic die
    Double_t      mass;                    // mass
    TVector3      inpos;		   // initial position
    TVector3      fipos;		   // final position
    TVector3      ip;	          	   // initial momentum
    Int_t         mothid;                  // mother identity
    TVector3      mothp;		   // mother momentum
    Int_t         pileup;                  // pile-up index =0 current event
                                         // >0 index of overlapped event
                                         // VM added 17/11/13

    ClassDef(TAGntuMCeveHit,1)
};

//##############################################################################

class TAGntuMCeve : public TAGdata {
  public:
                    TAGntuMCeve();
    virtual         ~TAGntuMCeve();

    TAGntuMCeveHit*       Hit(Int_t i);
    const TAGntuMCeveHit* Hit(Int_t i) const;

    virtual void    SetupClones();
    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TAGntuMCeve,1)

  public:
    Short_t         nhit;		    // nhit
    TClonesArray*   h;			    // hits
};

#include "TAGntuMCeve.icc"

#endif

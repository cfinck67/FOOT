#ifndef _TAIRdatRaw_HXX
#define _TAIRdatRaw_HXX
/*!
  \file
  \version $Id: TAIRdatRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TAIRdatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TObject.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"

class TAIRrawHit : public TObject {
  public:
    TAIRrawHit();
    TAIRrawHit(int typ, int cha, double charge, double time);
    virtual         ~TAIRrawHit();

    void            SetData(Int_t type, Int_t id, Double_t time, Double_t charge);
    Double_t        Time() const;
    Double_t        Charge() const;
    Int_t           ChID() const;
    Int_t           Type() const;

    void            SetTime(double time);
    void            SetCharge(double charge);
    void            SetChID(int id);  //SC channel ID
    void            SetType(int typ); //meaningless for now.

    ClassDef(TAIRrawHit,1)

  private:
    Double_t ir_time;    
    Double_t ir_chg;    
    Int_t ir_typ;
    Int_t ir_chid;
};

//##############################################################################

class TAIRdatRaw : public TAGdata {
  public:

                    TAIRdatRaw();
    virtual         ~TAIRdatRaw();

    void            SetCounter(Int_t i_ntdc, Int_t i_nadc, Int_t i_ndrop);

    TAIRrawHit*       Hit(Int_t i_ind);
    const TAIRrawHit* Hit(Int_t i_ind) const;

    void            SetTrigTime(double time);
    Double_t        TrigTime() const;

    Int_t           NTdc() const;
    Int_t           NAdc() const;
    Int_t           NDrop() const;

    virtual void    Clear(Option_t* opt="");

    void SetupClones();

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TAIRdatRaw,1)

  public:
    Int_t           nirhit;		    // 
    TClonesArray*   hir;			    // hits

  private:

    double          trg_time;               //SC trigger time
    Int_t           fiNAdc;		    // 
    Int_t           fiNTdc;		    // 
    Int_t           fiNDrop;		    // 

};

#include "TAIRdatRaw.icc"

#endif

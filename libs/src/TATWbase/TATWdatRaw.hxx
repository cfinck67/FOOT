#ifndef _TATWdatRaw_HXX
#define _TATWdatRaw_HXX
/*!
  \file
  \version $Id: TATWdatRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TATWdatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TObject.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"

class TATWrawHit : public TObject {
  public:
    TATWrawHit();
    TATWrawHit(int typ, int cha, double charge, double time);
    virtual         ~TATWrawHit();

    void            SetData(Int_t type, Int_t id, Double_t time, Double_t charge);
    Double_t        Time() const;
    Double_t        Charge() const;
    Int_t           ChID() const;
    Int_t           IDMC() const;
    Int_t           Type() const;

    void            SetTime(double time);
    void            SetCharge(double charge);
    void            SetChID(int id);  //SC channel ID
    void            SetMCID(int id);  //SC mc ID
    void            SetType(int typ); //meaningless for now.

    ClassDef(TATWrawHit,1)

  private:
    Double_t ir_time;    
    Double_t ir_chg;    
    Int_t ir_typ;
    Int_t ir_chid;
    Int_t ir_mcid;
};

//##############################################################################

class TATWdatRaw : public TAGdata {
  public:

                    TATWdatRaw();
    virtual         ~TATWdatRaw();

    void            SetCounter(Int_t i_ntdc, Int_t i_nadc, Int_t i_ndrop);

    TATWrawHit*       Hit(Int_t i_ind);
    const TATWrawHit* Hit(Int_t i_ind) const;

    Int_t           NTdc() const;
    Int_t           NAdc() const;
    Int_t           NDrop() const;

    virtual void    Clear(Option_t* opt="");

    void SetupClones();

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TATWdatRaw,1)

  public:
    Int_t           nirhit;		    // 
    TClonesArray*   hir;			    // hits

  private:

    Int_t           fiNAdc;		    // 
    Int_t           fiNTdc;		    // 
    Int_t           fiNDrop;		    // 

};

#include "TATWdatRaw.icc"

#endif

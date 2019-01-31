#ifndef _TATRdatRaw_HXX
#define _TATRdatRaw_HXX
/*!
  \file
  \version $Id: TATRdatRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TATRdatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TObject.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"

class TATRrawHit : public TObject {
  public:
    TATRrawHit();
    TATRrawHit(int typ, int cha, double charge, double time);
    virtual         ~TATRrawHit();

    void            SetData(Int_t type, Int_t id, Double_t time, Double_t charge);
    Double_t        Time() const;
    Double_t        Charge() const;
    Int_t           ChID() const;
    Int_t           Type() const;

    void            SetTime(double time);
    void            SetCharge(double charge);
    void            SetChID(int id);  //SC channel ID
    void            SetType(int typ); //meaningless for now.

    void            Clear(Option_t* option = "C");

    ClassDef(TATRrawHit,1)

  private:
    Double_t ir_time;    
    Double_t ir_chg;    
    Int_t ir_typ;
    Int_t ir_chid;
};

//##############################################################################

class TATRdatRaw : public TAGdata {
  public:

                     TATRdatRaw();
    virtual         ~TATRdatRaw();

    void              SetCounter(Int_t i_ntdc, Int_t i_nadc, Int_t i_ndrop);

    Int_t             GetHitsN() const;

    TATRrawHit*       Hit(Int_t i_ind);
    const TATRrawHit* Hit(Int_t i_ind) const;
   
    TATRrawHit*       NewHit(int tyoe, int channel, double charge, double time);


    void              SetTrigTime(double time);
    Double_t          TrigTime() const;

    Int_t             NTdc() const;
    Int_t             NAdc() const;
    Int_t             NDrop() const;

    virtual void      Clear(Option_t* opt="");

    void              SetupClones();

    virtual void      ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TATRdatRaw,1)

  private:
    TClonesArray*   fListOfHits; // hits
    double          fdTrgTime;   // SC trigger time
    Int_t           fiNAdc;		//
    Int_t           fiNTdc;		//
    Int_t           fiNDrop;		// 

};

#include "TATRdatRaw.icc"

#endif

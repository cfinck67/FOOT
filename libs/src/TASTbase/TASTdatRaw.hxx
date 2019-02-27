#ifndef _TASTdatRaw_HXX
#define _TASTdatRaw_HXX
/*!
  \file
  \version $Id: TASTdatRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TASTdatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TObject.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"

class TASTrawHit : public TObject {
  public:
    TASTrawHit();
    TASTrawHit(int typ, int cha, double charge, double time);
    virtual         ~TASTrawHit();

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

    ClassDef(TASTrawHit,1)

  private:
    Double_t ir_time;    
    Double_t ir_chg;    
    Int_t ir_typ;
    Int_t ir_chid;
};

//##############################################################################

class TASTdatRaw : public TAGdata {
  public:

                     TASTdatRaw();
    virtual         ~TASTdatRaw();

    void              SetCounter(Int_t i_ntdc, Int_t i_nadc, Int_t i_ndrop);

    Int_t             GetHitsN() const;

    TASTrawHit*       Hit(Int_t i_ind);
    const TASTrawHit* Hit(Int_t i_ind) const;
   
    TASTrawHit*       NewHit(int tyoe, int channel, double charge, double time);


    void              SetTrigTime(double time);
    Double_t          TrigTime() const;

    Int_t             NTdc() const;
    Int_t             NAdc() const;
    Int_t             NDrop() const;

    virtual void      Clear(Option_t* opt="");

    void              SetupClones();

    virtual void      ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TASTdatRaw,1)

  private:
    TClonesArray*   fListOfHits; // hits
    double          fdTrgTime;   // SC trigger time
    Int_t           fiNAdc;		//
    Int_t           fiNTdc;		//
    Int_t           fiNDrop;		// 

};

#include "TASTdatRaw.icc"

#endif

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
    TASTrawHit(int cha, double charge, double time);
    virtual         ~TASTrawHit();

    void            SetData(Int_t id, Double_t time, Double_t charge);
    Double_t        Time() const;
    Double_t        Charge() const;
    Int_t           ChID() const;

    void            SetTime(double);
    void            SetCharge(double);
    void            SetChID(int);  //SC channel ID

    ClassDef(TASTrawHit,1)

  private:
    Double_t st_time;    
    Double_t st_chg;    
    Int_t st_chid;
};




//##############################################################################

class TASTdatRaw : public TAGdata {
  public:

    TASTdatRaw();
    virtual         ~TASTdatRaw();

 
    TASTrawHit*       Hit(Int_t i_ind);
    const TASTrawHit* Hit(Int_t i_ind) const;

    void            SetTime(double time);
    Double_t        Time() const;

    void            SetCharge(double time);
    Double_t        Charge() const;
  
    virtual void    Clear(Option_t* opt="");

  void SetupClones();
  
  virtual void   ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TASTdatRaw,1)

  public:
    Int_t           nsthit;	//fare getter	    // 
    TClonesArray*   hst;			    // hits

  private:
 
    Double_t         time;                //SC trigger time
    Double_t         charge;		    // 
   
};

#include "TASTdatRaw.icc"

#endif

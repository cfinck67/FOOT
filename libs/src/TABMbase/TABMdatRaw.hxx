#ifndef _TABMdatRaw_HXX
#define _TABMdatRaw_HXX
/*!
  \file
  \version $Id: TABMdatRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TABMdatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TAGdata.hxx"

class TABMrawHit {
  public:
    TABMrawHit();
    virtual         ~TABMrawHit();

    void            SetData(Int_t view, Int_t lay, Int_t cell, double time);
    Int_t           Cell() const;
    Int_t           Plane() const;
    Int_t           View() const;
    Double_t        Time() const;

    ClassDef(TABMrawHit,1)

  private:
    Int_t iview;    
    Int_t ilayer;    
    Int_t icell;
    Double_t tdctime;
};

//##############################################################################

class TABMdatRaw : public TAGdata {
  public:

                    TABMdatRaw();
    virtual         ~TABMdatRaw();

    void            SetHitData(Int_t lay, Int_t view, Int_t cell, double time);
    void            SetCounter(Int_t i_ntdc, Int_t i_ndrop);

    Int_t           NHit() const;
    const TABMrawHit& Hit(Int_t i_ind) const;

    Int_t           NTdc() const;
    Int_t           NDrop() const;

    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TABMdatRaw,1)

  private:
    Int_t           fiNTdc;		      //number of accepted tdc values
    Int_t           fiNDrop;		    //number of discharged tdc values
    vector<TABMrawHit> fHitList;		    //list of TABMrawHit ATTENZIONE!! NON È UN TCLONESARRAY COME GLI ALTRI!!!
};

#include "TABMdatRaw.icc"

#endif

#ifndef _TADCdatRaw_HXX
#define _TADCdatRaw_HXX
/*!
  \file
  \version $Id: adapted from TABMdatRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TADCdatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TAGdata.hxx"

class TADCrawHit {
  public:
    TADCrawHit();
    virtual         ~TADCrawHit();

    void            SetData(Int_t view, Int_t lay, Int_t cell, double time);
    Int_t           Cell() const;
    Int_t           Plane() const;
    Int_t           View() const;
    Double_t        Time() const;

    ClassDef(TADCrawHit,1)

  private:
    Int_t iview;    
    Int_t ilayer;    
    Int_t icell;
    Double_t tdctime;
};

//##############################################################################

class TADCdatRaw : public TAGdata {
  public:

                    TADCdatRaw();
    virtual         ~TADCdatRaw();

    void            SetHitData(Int_t view, Int_t lay, Int_t cell, double time);
    void            SetCounter(Int_t i_ntdc, Int_t i_ndrop);

    Int_t           NHit() const;
    const TADCrawHit& Hit(Int_t i_ind) const;

    Int_t           NTdc() const;
    Int_t           NDrop() const;

    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TADCdatRaw,1)

  private:
    Int_t           fiNTdc;		    // 
    Int_t           fiNDrop;		    // 
    vector<TADCrawHit> fHitList;		    // 
};

#include "TADCdatRaw.icc"

#endif

#ifndef _TAGlineFit_HXX
#define _TAGlineFit_HXX
/*!
  \file
  \version $Id: TAGlineFit.hxx,v 1.1 2003/06/15 18:22:01 mueller Exp $
  \brief   Declaration of TAGlineFit.
*/
/*------------------------------------------+---------------------------------*/

#include "TAGobject.hxx"

class TAGlineFit : public TAGobject {
  public:
                    TAGlineFit();
    virtual         ~TAGlineFit();

    void            AddPoint(Float_t f_x, Float_t f_y, Float_t f_w=1.);
    void            SubPoint(Float_t f_x, Float_t f_y, Float_t f_w=1.);

    Int_t           NPoint() const;
    Bool_t          Fit(Float_t& f_p0, Float_t& f_p1) const;
    Bool_t          Fit(Float_t& f_p0, Float_t& f_p1,
			Float_t& f_errp0, Float_t& f_errp1) const;

    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;

    ClassDef(TAGlineFit,0)

  private:
    Double_t fdSn;			    // sum: n
    Double_t fdSx;			    // sum: x
    Double_t fdSxx;			    // sum: x*x
    Double_t fdSy;			    // sum: y
    Double_t fdSxy;			    // sum: x*y
  
};

#include "TAGlineFit.icc" 

#endif

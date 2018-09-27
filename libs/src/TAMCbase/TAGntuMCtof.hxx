#ifndef _TAGntuMCtof_HXX
#define _TAGntuMCtof_HXX
/*!
  \file
  \version $Id: TAGntuMCtof.hxx,v 1.7 2003/06/22 11:48:38 mueller Exp $
  \brief   Declaration of TAGntuMCtof.
*/
/*------------------------------------------+---------------------------------*/

#include "TObject.h"
#include "TVector3.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"

class TAGntuMCtofHit : public TObject {
  public:
                    TAGntuMCtofHit();
                    TAGntuMCtofHit(Int_t i_id, TVector3 i_ipos, 
				   TVector3 i_imom, Double_t i_tof);

    virtual         ~TAGntuMCtofHit();

    TVector3         InitPos();
    TVector3         InitP();
    Int_t            ID();
    Double_t         GetTof();

    void  SetInitPos(TVector3 pos);
    void  SetInitP(TVector3 mom);
    void  SetID(int aid);
    void  SetTof(double atof);

    Int_t         id;                      // identity
    TVector3      inpos;		   // initial position
    TVector3      ip;	          	   // initial momentum    
    Double_t      tof;                     // tof

    ClassDef(TAGntuMCtofHit,1)
};

//##############################################################################

class TAGntuMCtof : public TAGdata {
  public:
                    TAGntuMCtof();
    virtual         ~TAGntuMCtof();

    TAGntuMCtofHit*       Hit(Int_t i);
    const TAGntuMCtofHit* Hit(Int_t i) const;

    virtual void    SetupClones();
    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TAGntuMCtof,1)

  public:
    Short_t         nhit;		    // nhit
    TClonesArray*   h;			    // hits
};

#include "TAGntuMCtof.icc"

#endif

#ifndef _TAGntuMCmimo_HXX
#define _TAGntuMCmimo_HXX
/*!
  \file
  \version $Id: TAGntuMCmimo.hxx,v 1.7 2003/06/22 11:48:38 mueller Exp $
  \brief   Declaration of TAGntuMCmimo.
*/
/*------------------------------------------+---------------------------------*/

#include "TObject.h"
#include "TVector3.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"

class TAGntuMCmimoHit : public TObject {
  public:
                    TAGntuMCmimoHit();
                    //TAGntuMCmimoHit(Int_t i_id, TVector3 i_ipos, 
                    //				    TVector3 i_imom);
                    // new constructor VM 3/11/13
                    TAGntuMCmimoHit(Int_t i_id, TVector3 i_ipos, 
		                    TVector3 i_imom,Double_t de);

    virtual         ~TAGntuMCmimoHit();

    TVector3         InitPos();
    TVector3         InitP();
    Int_t            ID();

    void  SetInitPos(TVector3 pos);
    void  SetInitP(TVector3 mom);
    void  SetID(int aid);
    Double_t         DE();  // VM 3/11/13
    void SetDE(Double_t de); // VM 3/11/13

    Int_t         id;                      // identity
    TVector3      inpos;		   // initial position
    TVector3      ip;	          	   // initial momentum    

    Double_t     de;     // deposited energy VM 3/11/13

    ClassDef(TAGntuMCmimoHit,1)
};

//##############################################################################

class TAGntuMCmimo : public TAGdata {
  public:
                    TAGntuMCmimo();
    virtual         ~TAGntuMCmimo();

    TAGntuMCmimoHit*       Hit(Int_t i);
    const TAGntuMCmimoHit* Hit(Int_t i) const;

    virtual void    SetupClones();
    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TAGntuMCmimo,1)

  public:
    Short_t         nhit;		    // nhit
    TClonesArray*   h;			    // hits
};

#include "TAGntuMCmimo.icc"

#endif

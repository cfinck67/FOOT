#ifndef _TAMntuHit_HXX
#define _TAMntuHit_HXX
/*!
 \file
 \version $Id: TAMntuHit.hxx,v 1.7 2003/06/22 11:48:38 mueller Exp $
 \brief   Declaration of TAMntuHit.
 */
/*------------------------------------------+---------------------------------*/

#include "TObject.h"
#include "TVector3.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"

class TAMChit : public TObject {
public:
   TAMChit();
   TAMChit(Int_t id, TVector3 ipos, TVector3 imom, Double_t de, Double_t tof = 0);
   virtual      ~TAMChit();
   
   Int_t         GetSensorId()      const  { return fSensorId; }
   TVector3      GetPosition()      const  { return fPosition; }
   TVector3      GetMomentum()      const  { return fMomentum; }
   Double_t      GetDeltaE()        const  { return fDelatE;   }
   Double_t      GetTof()           const  { return fTof;      }
   
   
   void          SetSensorId(int aid)      { fSensorId = aid;  }
   void          SetPosition(TVector3 pos) { fPosition = pos;  }
   void          SetMomentum(TVector3 mom) { fMomentum = mom;  }
   void          SetDeltaE(Double_t e)     { fDelatE   = e;    }
   void          SetTof(Double_t tof)      { fTof      = tof;  }

   Int_t         fSensorId;   // identity
   TVector3      fPosition;   // initial position
   TVector3      fMomentum;   // initial momentum
   Double_t      fDelatE;     // deposited energy
   Double_t      fTof;        // time fo flight
   
   ClassDef(TAMChit,1)
};

//##############################################################################

class TAMCntuHit : public TAGdata {
public:
   TAMCntuHit();
   virtual          ~TAMCntuHit();
   
   Int_t             GetHitsN() const;

   TAMChit*          GetHit(Int_t i);
   const TAMChit*    GetHit(Int_t i) const;
   
   TAMChit*          NewHit(Int_t id, TVector3 ipos, TVector3 imom, Double_t de, Double_t tof = 0);

   virtual void      SetupClones();
   virtual void      Clear(Option_t* opt="");
   
   virtual void      ToStream(ostream& os=cout, Option_t* option="") const;

private:
   TClonesArray*   fListOfHits; // hits
   
   ClassDef(TAMCntuHit,1)
   
};

#endif

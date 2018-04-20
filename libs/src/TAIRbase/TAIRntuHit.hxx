#ifndef _TAIRntuHit_HXX
#define _TAIRntuHit_HXX


#include "TObject.h"
#include "TClonesArray.h"
#include "TString.h"

#include "TAGdata.hxx"

class TAIRhit : public TObject {
public:
   TAIRhit();
   TAIRhit(int cha, double charge, double time, Int_t type = 0);
   virtual         ~TAIRhit();
   
   Double_t        GetTime()          const { return fTime;      }
   Double_t        GetCharge()        const { return fCharge;    }
   Int_t           GetChannelId()     const { return fChannelId; }
   Int_t           GetType()          const { return fType;      }
   
   void            SetTime(double time)     { fTime = time;      }
   void            SetCharge(double charge) { fCharge = charge;  }
   void            SetChannelId(int id)     { fChannelId = id;   }
   void            SetType(int type)        { fType = type;      }
   
   Int_t           GetTrackIdSim(Int_t index) const   { return fMcTrackId[index];  }
   Int_t           GetMcTrackCount()          const   { return fMcTrackCount;      }

   void            AddMcTrackId(Int_t trackId);

private:
   static const    Int_t fMAXTRACK = 10;

   Double_t        fTime;
   Double_t        fCharge;
   Int_t           fChannelId;
   Int_t           fType;
   
   Int_t           fMcTrackCount;                 // Variable that count the number of times a crystal is touched
   Int_t           fMcTrackId[fMAXTRACK];         // Id of the track created in the simulation
   
   ClassDef(TAIRhit,1)
};

//##############################################################################

class TAIRntuHit : public TAGdata {
public:
   TAIRntuHit();
   virtual         ~TAIRntuHit();
   
   Int_t           GetHitsN() const;
   TAIRhit*        GetHit(Int_t i_ind);
   const TAIRhit*  GetHit(Int_t i_ind) const;
   
   TAIRhit*        NewHit(int channel, double charge, double time, int type = 0);
   
   
   virtual void    Clear(Option_t* opt="");
   
   void            SetupClones();
   
   virtual void    ToStream(ostream& os=cout, Option_t* option="") const;
   
public:
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }

private:
   TClonesArray*   fListOfHits; // hits

 
private:
   static TString fgkBranchName;    // Branch name in TTree
   
   ClassDef(TAIRntuHit,1)
};

#endif

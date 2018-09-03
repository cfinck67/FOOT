#ifndef _TACAntuRaw_HXX
#define _TACAntuRaw_HXX


#include "TObject.h"
#include "TClonesArray.h"
#include "TString.h"

#include "TAGdata.hxx"

class TACAntuHit : public TObject {
public:
   TACAntuHit();
   TACAntuHit(int cha, double charge, double time, Int_t type = 0);
   virtual         ~TACAntuHit();
   
   Double_t        GetTime()          const { return fTime;      }
   Double_t        GetCharge()        const { return fCharge;    }
   Int_t           GetChannelId()     const { return fChannelId; }
   Int_t           GetType()          const { return fType;      }
   
   void            SetTime(double time)     { fTime = time;      }
   void            SetCharge(double charge) { fCharge = charge;  }
   void            SetChannelId(int id)     { fChannelId = id;   }
   void            SetType(int type)        { fType = type;      }
   
   Int_t           GetTrackIdMc(Int_t index) const   { return fMcTrackId[index];  }
   Int_t           GetMcTrackCount()          const   { return fMcTrackCount;      }

   void            AddMcTrackId(Int_t trackId);

private:
   static const    Int_t fgkMAXTRACK = 10;

   Double_t        fTime;
   Double_t        fCharge;
   Int_t           fChannelId;
   Int_t           fType;
   
   Int_t           fMcTrackCount;                 // Variable that count the number of times a crystal is touched
   Int_t           fMcTrackId[fgkMAXTRACK];         // Id of the track created in the simulation
   
   ClassDef(TACAntuHit,1)
};

//##############################################################################

class TACAntuRaw : public TAGdata {
public:
   TACAntuRaw();
   virtual         ~TACAntuRaw();
   
   Int_t           GetHitsN() const;
   TACAntuHit*        GetHit(Int_t i_ind);
   const TACAntuHit*  GetHit(Int_t i_ind) const;
   
   TACAntuHit*        NewHit(int channel, double charge, double time, int type = 0);
   
   void            SetTrigTime(Double_t time) { fTrigTime = time; }
   Double_t        GetTrigTime()        const { return fTrigTime; }
   
   Int_t           GetNTdc() const  { return fiNTdc;  }
   Int_t           GetNAdc() const  { return fiNTdc;  }
   Int_t           GetNDrop() const { return fiNDrop; }
   
   virtual void    Clear(Option_t* opt="");
   
   void            SetupClones();
   
   virtual void    ToStream(ostream& os=cout, Option_t* option="") const;
   
public:
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }

private:
   TClonesArray*   fListOfHits; // hits
   Double_t        fTrigTime;     // SC trigger time
   Int_t           fiNAdc;		    //
   Int_t           fiNTdc;		    //
   Int_t           fiNDrop;		 //
 
private:
   static TString fgkBranchName;    // Branch name in TTree
   
   ClassDef(TACAntuRaw,1)
};

#endif

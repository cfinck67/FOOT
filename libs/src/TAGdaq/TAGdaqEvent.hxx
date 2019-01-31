#ifndef _TAGdaqEvent_HXX
#define _TAGdaqEvent_HXX

/*------------------------------------------+---------------------------------*/
/*
     Interface with DAQ fragment
 */

#include <vector>
#include <string>
using namespace std;

#include "TString.h"

#include "DAQFileHeader.hh"
#include "InfoEvent.hh"
#include "TrgEvent.hh"
#include "BaseFragment.hh"

#include "TAGdata.hxx"

class TAGdaqEvent : public TAGdata {
public:
   TAGdaqEvent();
   virtual              ~TAGdaqEvent();

   InfoEvent*             GetInfoEvent()          const  { return fInfoEvent;                    }
   TrgEvent*              GetTrgEvent()           const  { return fTrgEvent;                     }
   
   void                   SetInfoEvent(InfoEvent* info)  {  fInfoEvent = info;                   }
   void                   SetTrgEvent(TrgEvent* trg)     {  fTrgEvent = trg;                     }
   
   Int_t                  GetFragmentsN()         const  { return (int)fListOfFragments.size();  }
   const BaseFragment*    GetFragment(Int_t idx)         { return fListOfFragments[idx];         }
   
   const Char_t*          GetClassType(Int_t idx) const  { return fListOfClassTypes[idx].data(); }
   
   void                   AddFragment(const BaseFragment* frag);

   virtual void           Clear(Option_t* opt="");
   
   virtual void           ToStream(ostream& os = cout, Option_t* option = "") const;
   
private:
   InfoEvent*                   fInfoEvent;
   TrgEvent*                    fTrgEvent;
   vector<const BaseFragment*>  fListOfFragments;
   vector<string>               fListOfClassTypes;
   
   ClassDef(TAGdaqEvent,0)
};

#endif

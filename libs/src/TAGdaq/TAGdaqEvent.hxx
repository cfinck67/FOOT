#ifndef _TAGdaqEvent_HXX
#define _TAGdaqEvent_HXX

/*------------------------------------------+---------------------------------*/
/* TEMPLATE CLASS
   Need real implementation
 */

#include <vector>
using namespace std;

#include "TAGdata.hxx"

class TAGdaqEvent : public TAGdata {
public:
   TAGdaqEvent();
   virtual         ~TAGdaqEvent();

   Int_t           NSubEvent()                   const {return (Int_t)fOffset.size(); }
   
   Int_t           SubEventType(Int_t i_ind)     const { return (fData[fOffset[i_ind]+1]>>16) & 0xffff;; }
   Int_t           SubEventSubType(Int_t i_ind)  const { return fData[fOffset[i_ind]+1] & 0xffff;;       }
   Int_t           SubEventProcId(Int_t i_ind)   const { return (fData[fOffset[i_ind]+2]>>16) & 0xffff;; }
   Int_t           SubEventProcType(Int_t i_ind) const { return fData[fOffset[i_ind]+2] & 0xff;;         }
   Int_t           SubEventCrate(Int_t i_ind)    const { return fData[fOffset[i_ind]+2] & 0xff;          }
   
   Int_t           SubEventSize(Int_t i_ind) const { return fData[fOffset[i_ind]];        }
   const UInt_t*   SubEventData(Int_t i_ind) const { return &fData[0] + fOffset[i_ind]+3; }
   
   virtual Bool_t  NeedAutoDelete() const;
   
   virtual void    Clear(Option_t* opt="");
   
   virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;
   
   ClassDef(TAGdaqEvent,2)
   
private:
   void            SetupOffset();
   
private:				    // NOTE: CUSTOM STREAMER
   vector<UInt_t>  fData;		    // data vector (common for all SE's)
   vector<Int_t>   fOffset;		    // offset table for SE's
};

#endif

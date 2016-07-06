#ifndef _TAVTrawEvent_HXX
#define _TAVTrawEvent_HXX
/*!
  \file
  \version $Id: TAVTrawEvent.hxx,v 1.6 2003/06/09 18:33:17 mueller Exp $
  \brief   Declaration of TAVTrawEvent.
*/
/*------------------------------------------+---------------------------------*/
#include "TAGdata.hxx"

class TAVTrawEvent : public TAGdata {
public:
   TAVTrawEvent();
   virtual         ~TAVTrawEvent();
   
   void            AddEvent(Int_t size, const UInt_t data[]);
   
   Int_t           EventTrigger() const { return fEventTrigger; }
   Int_t           EventSize()    const { return fEventSize;    }
   const UInt_t*   EventData()    const { return fData;         }
   
   virtual Bool_t  NeedAutoDelete() const;
   
   virtual void    Clear(Option_t* opt="");
   
   virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;
   
private:				    
   UInt_t* fData;          //! container of data
   Int_t   fEventTrigger;  // trigger event if ever 
   Int_t   fEventSize;     // event sise 
   
   ClassDef(TAVTrawEvent,2)

};

#endif

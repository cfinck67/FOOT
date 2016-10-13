#ifndef _TAITrawEvent_HXX
#define _TAITrawEvent_HXX
/*!
  \file
  \version $Id: TAITrawEvent.hxx,v 1.6 2003/06/09 18:33:17 mueller Exp $
  \brief   Declaration of TAITrawEvent.
*/
/*------------------------------------------+---------------------------------*/
#include "TAGdata.hxx"

class TAITrawEvent : public TAGdata {
public:
   TAITrawEvent();
   virtual         ~TAITrawEvent();
   
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
   
   ClassDef(TAITrawEvent,2)

};

#endif

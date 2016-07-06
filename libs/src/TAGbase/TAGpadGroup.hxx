#ifndef _TAGpadGroup_HXX
#define _TAGpadGroup_HXX
/*!
  \file
  \version $Id: TAGpadGroup.hxx,v 1.4 2003/04/27 21:01:41 mueller Exp $
  \brief   Declaration of TAGpadGroup.
*/
/*------------------------------------------+---------------------------------*/

#include "TVirtualPad.h"
#include "TList.h"

#include "TAGobject.hxx"

class TAGpadGroup : public TAGobject {
  public:
                    TAGpadGroup();
    virtual         ~TAGpadGroup();

    void            AddPad(TVirtualPad* p_pad);
    void            AddSubPads(TVirtualPad* p_pad);

    Bool_t          IsEmpty() const;

    void            Modified() const;
    void            Update() const;
    void            Print(char* fl, const char* suff=".eps") const;

    void            Clear(Option_t*option ="");

    ClassDef(TAGpadGroup,0)

  private:
    TList*          fpListPad;
    TList*          fpListCanvas;
};

#endif

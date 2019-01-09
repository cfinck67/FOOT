
#ifndef _TAVTntuHit_HXX
#define _TAVTntuHit_HXX

#include "TAVTbaseNtuHit.hxx"

/** TAVTntuHit class interface to base class

*/
/*------------------------------------------+---------------------------------*/

class TAVTntuHit : public TAVTbaseNtuHit {
   
public:
    TAVTntuHit();
    TAVTntuHit( Int_t iSensor, const Int_t aIndex, Double_t aValue);
    TAVTntuHit( Int_t iSensor, Double_t aValue, Int_t aLine, Int_t aColumn);
    virtual ~TAVTntuHit() {};
   
    ClassDef(TAVTntuHit,3) 
};

//##############################################################################



#endif

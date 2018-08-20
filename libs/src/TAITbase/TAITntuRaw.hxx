#ifndef _TAITntuRaw_HXX
#define _TAITntuRaw_HXX
/*!
 \file
 \version $Id: TAITntuRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
 \brief   Declaration of TAITntuRaw.
 */
/*------------------------------------------+---------------------------------*/

#include <map>
#include "TAITntuHit.hxx"
#include "TAGdata.hxx"

class TAITparGeo;
class TAITntuRaw : public TAGdata {
   
protected:
   //using TObjArray here
   TObjArray*        fListOfPixels;
   TAITparGeo*       fpGeoMap;         //! do not store
   
    std::map<pair<int, int>, int > fMap; //!
    
private:
   static TString    fgkBranchName;    // Branch name in TTree
   
public:
   TAITntuRaw();
   virtual          ~TAITntuRaw();
   
   TAITntuHit*       GetPixel(Int_t iSensor, Int_t iPixel);
   const TAITntuHit* GetPixel(Int_t iSensor, Int_t iPixel) const;
   
   TClonesArray*     GetListOfPixels(Int_t iSensor);
   TClonesArray*     GetListOfPixels(Int_t iSensor) const;
   
   Int_t             GetPixelsN(Int_t iSensor) const;
   
   TAITntuHit*       NewPixel(Int_t sensor, Double_t value, Int_t aLine, Int_t aColumn);
   
   virtual void      SetupClones();
   
   virtual void      Clear(Option_t* opt="");
   
   virtual void      ToStream(ostream& os=cout, Option_t* option="") const;
   
public:
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }
   
   ClassDef(TAITntuRaw,1)
};

#endif


























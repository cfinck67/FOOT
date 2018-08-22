#ifndef _TAMSDntuRaw_HXX
#define _TAMSDntuRaw_HXX
/*!
 \file
 \version $Id: TAMSDntuRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
 \brief   Declaration of TAMSDntuRaw.
 */
/*------------------------------------------+---------------------------------*/

#include <map>
#include "TAMSDntuHit.hxx"
#include "TAGdata.hxx"

class TAMSDparGeo;
class TAMSDntuRaw : public TAGdata {
   
protected:
   //using TObjArray here
   TObjArray*        fListOfPixels;
   TAMSDparGeo*      fpGeoMap;         //! do not store
   
    std::map<pair<int, int>, int > fMap; //!
    
private:
   static TString    fgkBranchName;    // Branch name in TTree
   
public:
   TAMSDntuRaw();
   virtual           ~TAMSDntuRaw();
   
   TAMSDntuHit*       GetPixel(Int_t iSensor, Int_t iPixel);
   const TAMSDntuHit* GetPixel(Int_t iSensor, Int_t iPixel) const;
   
   TClonesArray*      GetListOfPixels(Int_t iSensor);
   TClonesArray*      GetListOfPixels(Int_t iSensor) const;
   
   Int_t              GetPixelsN(Int_t iSensor) const;
   
   TAMSDntuHit*       NewPixel(Int_t sensor, Double_t value, Int_t aView, Int_t aStrip);
   
   virtual void       SetupClones();
   
   virtual void       Clear(Option_t* opt="");
   
   virtual void       ToStream(ostream& os=cout, Option_t* option="") const;
   
public:
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }
   
   ClassDef(TAMSDntuRaw,1)
};

#endif


























#ifndef _TAVTntuRaw_HXX
#define _TAVTntuRaw_HXX
/*!
 \file
 \version $Id: TAVTntuRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
 \brief   Declaration of TAVTntuRaw.
 */
/*------------------------------------------+---------------------------------*/


#include "TAVTntuHit.hxx"

#include "TAGdata.hxx"




class TAVTntuRaw : public TAGdata {
   
private:
   //using TObjArray here
   TObjArray*        fListOfPixels; 
   static TString    fgkBranchName;    // Branch name in TTree
   
public:
   TAVTntuRaw();
   virtual          ~TAVTntuRaw();
   
   TAVTntuHit*       GetPixel(Int_t iSensor, Int_t iPixel);
   const TAVTntuHit* GetPixel(Int_t iSensor, Int_t iPixel) const;
   
   TClonesArray*     GetListOfPixels(Int_t iSensor);
   TClonesArray*     GetListOfPixels(Int_t iSensor) const;
   
   Int_t             GetPixelsN(Int_t iSensor) const; 
   
   TAVTntuHit*       NewPixel(Int_t sensor, Double_t value, Int_t aLine, Int_t aColumn);
   TAVTntuHit*       NewPixel(Int_t sensor, TAVTrawHit* pixel);
   TAVTntuHit*       AddPixel(int iSensor, double value, int aLine, int aColumn);

   
   virtual void      SetupClones();
   
   virtual void      Clear(Option_t* opt="");
   
   virtual void      ToStream(ostream& os=cout, Option_t* option="") const;
 
public:   
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }
   
   ClassDef(TAVTntuRaw,1)
};

#endif

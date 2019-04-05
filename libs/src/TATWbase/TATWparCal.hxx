#ifndef _TATWparCal_HXX
#define _TATWparCal_HXX
/*!
  \file
  \version $Id: TATWparCal.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TATWparCal.
*/
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TObject.h"
#include "TArrayF.h"
#include "TF1.h"
#include "TAGparTools.hxx"
#include "CCalibrationMap.hxx"

//##############################################################################

class TATWparCal : public TAGparTools {
      
private:
   struct ChargeParameter_t : public  TObject {
	  Int_t   Charge;   // charge
	  Float_t CutLow;   // Lower cut
	  Float_t CutUp;    // Upper cut
   };
   
   ChargeParameter_t  fChargeParameter[8];
private: 

   static Int_t   fgkChargesN;       // number of charge states
   static TString fgkDefaultCalName; // default detector charge calibration file
   CCalibrationMap *cMapCal;
   
public:

   TATWparCal();
    virtual ~TATWparCal();

   //! Read from file
   Bool_t             FromFile(const TString& name = "");
   inline CCalibrationMap* getCalibrationMap() {return cMapCal;}
   //
   //   //! Get charge
   Int_t              GetChargeZ(Float_t edep) const;
   //
   //! Clear
   virtual void       Clear(Option_t* opt="");

   //! Stream output
   virtual void       ToStream(ostream& os = cout, Option_t* option = "") const;

   //   //! Get charge parameter
   //   ChargeParameter_t& GetChargePar(Int_t idx) {return fChargeParameter[idx];}
   //
   //   Int_t              GetCharge(Int_t idx) const { return fChargeParameter[idx].Charge; }
   //   Float_t            GetCutLow(Int_t idx) const { return fChargeParameter[idx].CutLow; }
   //   Float_t            GetCutUp(Int_t idx)  const { return fChargeParameter[idx].CutUp; }

public:
   static const Char_t* GetDefaultCalName()      { return fgkDefaultCalName.Data(); }

   ClassDef(TATWparCal,1)
};

#endif

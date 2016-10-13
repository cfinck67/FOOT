#ifndef _TAITactNtuRaw_HXX
#define _TAITactNtuRaw_HXX
/*!
 \file
 \version $Id: TAITactNtuRaw.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAITactNtuRaw.
 */
/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"
class TH2F;

class TAITactNtuRaw : public TAGaction {
public:
   explicit  TAITactNtuRaw(const char* name=0,
						   TAGdataDsc* p_nturaw=0, 
						   TAGdataDsc* p_datraw=0, 
						   TAGparaDsc* p_parmap=0,
						   TAGparaDsc* p_geomap=0);
   virtual ~TAITactNtuRaw();
   
   //! Base action 
   virtual Bool_t  Action();
   
   //! Base creation of histogram
   virtual  void   CreateHistogram();
   
   //! Delete
   virtual void DeleteDoublet(Int_t iSensor);
   
   ClassDef(TAITactNtuRaw,0)
   
private:
   TAGdataDsc*     fpNtuRaw;		    // output data dsc
   TAGdataDsc*     fpDatRaw;		    // input data dsc
   TAGparaDsc*     fpParMap;		    // map para dsc
   TAGparaDsc*     fpGeoMap;		    // geometry para dsc
   
   Int_t           fDebugLevel;         // debug level
   
   TH2F*           fpHisPosMap[8];    // pixel map per sensor   
};

#endif

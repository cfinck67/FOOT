#ifndef _TAVTactNtuRaw_HXX
#define _TAVTactNtuRaw_HXX
/*!
 \file
 \version $Id: TAVTactNtuRaw.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAVTactNtuRaw.
 */
/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"
class TH2F;

class TAVTactNtuRaw : public TAGaction {
public:
   explicit  TAVTactNtuRaw(const char* name=0,
						   TAGdataDsc* p_nturaw=0, 
						   TAGdataDsc* p_datraw=0, 
						   TAGparaDsc* p_parmap=0,
						   TAGparaDsc* p_geomap=0);
   virtual ~TAVTactNtuRaw();
   
   //! Base action 
   virtual Bool_t  Action();
   
   //! Base creation of histogram
   virtual  void   CreateHistogram();
   
   //! Delete
   virtual void DeleteDoublet(Int_t iSensor);
   
   ClassDef(TAVTactNtuRaw,0)
   
private:
   TAGdataDsc*     fpNtuRaw;		    // output data dsc
   TAGdataDsc*     fpDatRaw;		    // input data dsc
   TAGparaDsc*     fpParMap;		    // map para dsc
   TAGparaDsc*     fpGeoMap;		    // geometry para dsc
   
   Int_t           fDebugLevel;         // debug level
   
   TH2F*           fpHisPosMap[8];    // pixel map per sensor   
};

#endif

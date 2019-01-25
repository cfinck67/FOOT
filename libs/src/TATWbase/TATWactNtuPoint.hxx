#ifndef _TATWactNtuPoint_HXX
#define _TATWactNtuPoint_HXX
/*!
 \file
 \version $Id: TATWactNtuPoint.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TATWactNtuPoint.
 */
/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"

class TAVTntuCluster;
class TAVTntuHit;
class TAVTparGeo;

class TATWactNtuPoint : public TAGaction {
   
public:
   explicit  TATWactNtuPoint(const char* name     = 0,
								       TAGdataDsc* p_nturaw  = 0,
                               TAGdataDsc* p_ntupoint = 0,
							          TAGparaDsc* p_geomap  = 0);
   
   virtual ~TATWactNtuPoint();
   
   //! Action
   virtual  Bool_t Action();
   

   //! Find point
   virtual Bool_t  FindPoints();
   

private:
   TAGdataDsc*     fpNtuRaw;		  // input data dsc
   TAGdataDsc*     fpNtuPoint;		  // output data dsc
   TAGparaDsc*     fpGeoMap;		  // geometry para dsc

   ClassDef(TATWactNtuPoint,0)
};

#endif

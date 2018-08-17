#ifndef _TAITactNtuClusterF_HXX
#define _TAITactNtuClusterF_HXX
/*!
 \file
 \version $Id: TAITactNtuClusterF.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAITactNtuClusterF.
 */
/*------------------------------------------+---------------------------------*/
#include <map>
#include "TArrayI.h"

#include "TAVTactNtuClusterF.hxx"

class TAVTcluster;
class TAVTntuHit;

class TAITactNtuClusterF : public TAVTactNtuClusterF {
   
public:
   explicit  TAITactNtuClusterF(const char* name     =  0,
								   TAGdataDsc* p_nturaw =  0, 
								   TAGdataDsc* p_ntuclus = 0, 
								   TAGparaDsc* p_config =  0,
							      TAGparaDsc* p_geomap =  0);
   
   virtual ~TAITactNtuClusterF();
   
   //! Find cluster charge, noise & position
   Bool_t  FindClusters(Int_t iSensor);
   
   
   ClassDef(TAITactNtuClusterF,0)
};

#endif

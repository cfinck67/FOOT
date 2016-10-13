#ifndef _TAITactNtuCluster_HXX
#define _TAITactNtuCluster_HXX
/*!
 \file
 \version $Id: TAITactNtuCluster.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAITactNtuCluster.
 */
/*------------------------------------------+---------------------------------*/
#include "TArrayI.h"

#include "TAITactBaseNtuCluster.hxx"

class TAITcluster;
class TAITntuHit;
class TAITactNtuCluster : public TAITactBaseNtuCluster {
   
public:
   explicit  TAITactNtuCluster(const char* name     =  0,
								   TAGdataDsc* p_nturaw =  0, 
								   TAGdataDsc* p_ntuclus = 0, 
								   TAGparaDsc* p_config =  0,
							       TAGparaDsc* p_geomap =  0);
   
   virtual ~TAITactNtuCluster();
   
   //! Find cluster charge, noise & position
   Bool_t  FindClusters(Int_t iSensor);  

   //! Compute position
   void    ComputePosition();
   //! Array that contains the index of pixels belonging to current cluster
   TArrayI  GetIndexArray()  const { return fIndexArray;      }
   
private: 
   Bool_t  AddPixels(Int_t seed);
   void    ReorderPixels();

private:
   TArrayI        fIndexArray;        // array of pixel index

   ClassDef(TAITactNtuCluster,0)
};

#endif

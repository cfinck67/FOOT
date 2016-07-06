#ifndef _TAVTactNtuCluster_HXX
#define _TAVTactNtuCluster_HXX
/*!
 \file
 \version $Id: TAVTactNtuCluster.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAVTactNtuCluster.
 */
/*------------------------------------------+---------------------------------*/
#include "TArrayI.h"

#include "TAVTactBaseNtuCluster.hxx"

class TAVTcluster;
class TAVTntuHit;
class TAVTactNtuCluster : public TAVTactBaseNtuCluster {
   
public:
   explicit  TAVTactNtuCluster(const char* name     =  0,
								   TAGdataDsc* p_nturaw =  0, 
								   TAGdataDsc* p_ntuclus = 0, 
								   TAGparaDsc* p_config =  0,
							       TAGparaDsc* p_geomap =  0);
   
   virtual ~TAVTactNtuCluster();
   
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

   ClassDef(TAVTactNtuCluster,0)
};

#endif

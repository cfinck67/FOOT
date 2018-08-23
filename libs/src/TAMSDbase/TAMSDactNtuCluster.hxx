#ifndef _TAMSDactNtuCluster_HXX
#define _TAMSDactNtuCluster_HXX
/*!
 \file
 \version $Id: TAMSDactNtuCluster.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAMSDactNtuCluster.
 */
/*------------------------------------------+---------------------------------*/

#include "TAVTactBaseNtuCluster.hxx"

class TAMSDntuCluster;
class TAVTntuHit;
class TAVTbaseParGeo;

class TAMSDactNtuCluster : public TAVTactBaseNtuCluster {
   
public:
   explicit  TAMSDactNtuCluster(const char* name     = 0,
								       TAGdataDsc* p_nturaw  = 0,
                               TAGdataDsc* p_ntuclus = 0,
								       TAGparaDsc* p_config  = 0,
							          TAGparaDsc* p_geomap  = 0);
   
   virtual ~TAMSDactNtuCluster();
   
   //! Action
   virtual  Bool_t Action();

   //! Find cluster charge, noise & position
   virtual Bool_t  FindClusters(Int_t iSensor);
   
   //! Apply basic cuts
   virtual Bool_t  ApplyCuts(TAVTbaseCluster* /*cluster*/) { return true; }// no config for the moment
   
   //! Compute position
   void    ComputePosition();

private:
   TAGdataDsc*     fpNtuRaw;		  // input data dsc
   TAGdataDsc*     fpNtuClus;		  // output data dsc
   
private:
   Bool_t  ShapeCluster(Int_t noClus, Int_t IndX, Int_t IndY, TAVTbaseParGeo* pGeoMap);
   Bool_t  FindClusters(Int_t iSensor, TAMSDntuCluster* pNtuClus, TAVTbaseParGeo* pGeoMap);
   Bool_t  CreateClusters(Int_t iSensor, TAMSDntuCluster* pNtuClus, TAVTbaseParGeo* pGeoMap);


   ClassDef(TAMSDactNtuCluster,0)
};

#endif

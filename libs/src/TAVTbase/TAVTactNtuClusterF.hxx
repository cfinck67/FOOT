#ifndef _TAVTactNtuClusterF_HXX
#define _TAVTactNtuClusterF_HXX
/*!
 \file
 \version $Id: TAVTactNtuClusterF.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAVTactNtuClusterF.
 */
/*------------------------------------------+---------------------------------*/
#include <map>
#include "TArrayI.h"

#include "TAVTactBaseNtuCluster.hxx"

class TAVTcluster;
class TAVTntuHit;

class TAVTactNtuClusterF : public TAVTactBaseNtuCluster {
   
public:
   explicit  TAVTactNtuClusterF(const char* name     =  0,
                                       TAGdataDsc* p_nturaw =  0, 
                                       TAGdataDsc* p_ntuclus = 0, 
                                       TAGparaDsc* p_config =  0,
                                       TAGparaDsc* p_geomap =  0, 
                                       string command = ""
                                    );
   
   virtual ~TAVTactNtuClusterF() {};
   
   //! Find cluster charge, noise & position
   Bool_t  FindClusters(int iSensor);   // called by action() of the parent class TAVTactBaseNtuCluster
   
   //! Compute position
   void    ComputePosition();
   
private: 
   Bool_t  ShapeCluster( int sensorID, int clusterID, int line, int col );
   
private:
   
   map<int, int> m_pixelHitMap;        // map of pixels that are present in the ntuRaw (value is always 1), indexed by line*nLine+col algorithm: x[ line*nLine+col ] = 1
   map<int, int> m_indexMap;           // map of the pixel index in ntuRaw, indexed by line*nLine+col algorithm: x[ line*nLine+col ] = ID
   vector<int> m_pixelIDMap;           // map of the line*nLine+col algorithm, indexed by index in ntuRaw: x[ ID ] = line*nLine+col
   map<int, int> m_usedPixelMap;         // map used pixels, store the belongning cluster else -1, indexed by line*nLine+col algorithm: x[ line*nLine+col ] = clusterID

   int                m_nLine;
   int                m_nCol;
   
   ClassDef(TAVTactNtuClusterF,0)
};

#endif

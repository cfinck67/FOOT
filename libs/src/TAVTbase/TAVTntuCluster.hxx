#ifndef _TAVTcluster_HXX
#define _TAVTcluster_HXX

// ROOT classes
#include "TObject.h"
#include "TList.h"
#include "TVector3.h"
#include "TClonesArray.h"
#include "TObjArray.h"

#include "TAGdata.hxx"
#include "TAVTntuRaw.hxx"


class TAVTtrack;

/** TAVTcluster class, simple container class for tracks with the associated clusters                    
 
 \author Ch. Finck
 */

class TAVTcluster : public TObject {
   
private:
   
   TAVTparGeo* m_geometry;

   TVector3          fPosition;                 // position of the cluster in plane frame 
   TVector3          fPosError;                 // position's errors of the cluster in plane frame 
   // TVector3*          fPositionG;                // position of the clus in tracker frame
   TClonesArray*      fListOfPixels;             // list of pixel attached to this cluster
   
   // string             m_command;                 // type of pixel to be used for clustering

   Int_t              fDebugLevel;             // debug level
   int              m_clusterID;                   // number  -->  maybe its position
   int              m_sensorID;              // plane number
   // Bool_t             fFound;                    // kTRUE is associated to a track
   // Bool_t             fFoundXZ;					 // kTRUE is associated to a track in XZ Projection
   // Bool_t             fFoundYZ;					 // kTRUE is associated to a track in YZ Projection
	
   Float_t            fClusterPulseSum;          // sum of pulseheight on strips in hit cluster, involves noise cuts
   Float_t            fClusterAreaPulseSum;      // sum of pulseheight on strips in hit cluster area, no noise cuts
   Float_t            fClusterNoiseAverage;      // hit cluster signal noise average
   Float_t            fSNneighbour;              // signal / Noise of neighbours.;
   Int_t              fStripsInClusterArea;      // # strips in cluster area
   
   Float_t            fPhSeed;                   // pulseheight on seed strip
   Int_t              fIndexSeed;                // index of seed strip
   
public:
   TAVTcluster(); 
   TAVTcluster(const TAVTcluster& cluster);
   ~TAVTcluster();
   
   //! Add pixel to the list
   void               AddPixel(TAVTntuHit* pixel);
   //! reset pixels
   void               ResetPixels();
   

   //! Set position in local frame
   void               SetPosition(TVector3 pos); 
   //! Set position error in local frame
   void               SetPosError(TVector3 pos); 
   // void               SetPosition(Float_t u, Float_t v, Float_t z) { fPosition->SetXYZ(u,v,z); }
   //! Set position in global tracker frame
   // void               SetPositionG(TVector3 pos); 
   //! Set pixel index for a given pixel  -->  remove
   void               SetIndexSeed(Int_t index)              { fIndexSeed = index;     } 
   //! Found flag for this cluster
   // void               SetFound(Bool_t flag = true)           { fFound = flag;          }
   //! Found flag for this cluster (Hough Transform XZ)
   // void               SetFoundXZ(Bool_t flag = true)         { fFoundXZ = flag;        }
   // //! Found flag for this cluster ((Hough Transform YZ)
   // void               SetFoundYZ(Bool_t flag = true)         { fFoundYZ = flag;        }
   //! Set cluster number   -->  remove
   void               SetClusterID(int nb)                 { m_clusterID = nb;           } 
   //! Set plane number
   void               SetSensorID(Int_t nb)               { m_sensorID = nb;      }
   //! Set debug level  -->  remove
   void               SetDebug( Int_t aDebug)                { fDebugLevel = aDebug; }   
   
   //! Get position in local frame
   TVector3           GetPosition()                    { return fPosition;      }
   //! Get position error in local frame
   TVector3&           GetPosError()                   { return fPosError;      }   
   //! Get position in global tracker frame
   TVector3           GetPosition_Global()            { 
      TVector3 globPos = fPosition;
      m_geometry->Local2Global( &globPos ); 
      return globPos; 
   }
   //! Get pixel, takes the index of the list interal to the cluster
   TAVTntuHit*        GetPixel(Int_t idx);

   //! Get U position in local frame
   // Float_t            GetPositionU()                   const { return (*fPosition)(0); }
   // //! Get V position in local frame
   // Float_t            GetPositionV()                   const { return (*fPosition)(1); }
   //! Get Pixel list  -->  not safe
   TClonesArray*      GetListOfPixels()                const { return fListOfPixels;   }
   //! Get cluster number
   int                GetClusterID()                   const { return m_clusterID;         }
   //! Get sensor id 
   int              GetSensorID()                const { return m_sensorID;    }
   //! Get found flag this cluster
   // Bool_t             GetFound()                       const { return fFound;          }
   // //! Get found flag this cluster
   // Bool_t             GetFoundXZ()                     const { return fFoundXZ;        }
   // //! Get found flag this cluster
   // Bool_t             GetFoundYZ()                     const { return fFoundYZ;        }
   //! Get debug level  -->  remove
   Int_t              GetDebug()                       const { return fDebugLevel;   }
   //! Get index of seed pixel  -->  remove
   Int_t              GetIndexSeed()                   const { return fIndexSeed;      }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //! Get index for a given pixel
   Int_t              GetIndex(Int_t tSk)              const { return ((TAVTntuHit*)fListOfPixels->At(tSk))->GetPixelIndex();  } 
   //! Get pulse height for a given pixel
   Float_t            GetPulseHeight(Int_t tSk)        const { return ((TAVTntuHit*)fListOfPixels->At(tSk))->GetPulseHeight(); } 
   //! Get number of pixels in this clusters
   Int_t              GetPixelsN()                     const { return  fListOfPixels->GetEntries(); }
   // /! Get position of seed pixel  -->  change
   Float_t            GetSeedU()                       const;
   Float_t            GetSeedV()                       const;
   
   //! Get distance from cluster position to a given pixel
   Float_t            GetPixelDistanceU(Int_t index)   const;
   Float_t            GetPixelDistanceV(Int_t index)   const;
   
   //! Compute distance from another cluster
   Float_t            Distance(TAVTcluster *aClus);
   //! Compute distance from a track 
   Float_t            Distance(TAVTtrack *aTrack); 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   //! Get Sum of neighbour (not used)
   Float_t            GetSNneighbour()                 const { return  fSNneighbour;          } 
   //! Get sum of pulse of cluster for a given area
   Float_t            GetClusterAreaPulseSum()         const { return fClusterAreaPulseSum;  }
   //! Get sum of pulse of cluster 
   Float_t            GetClusterPulseSum();
   
   
   
   
   ClassDef(TAVTcluster,2)                          // Describes TAVTcluster
};





//##############################################################################

class TAVTntuCluster : public TAGdata {
   
private:
   TObjArray*         fListOfClusters; 

private:   
   static TString    fgkBranchName;    // Branch name in TTree

public:
   TAVTntuCluster();
   virtual          ~TAVTntuCluster();
   
   TAVTcluster*       GetCluster(Int_t iSensor, Int_t i);
   const TAVTcluster* GetCluster(Int_t iSensor, Int_t i) const;
   
   Int_t              GetClustersN(Int_t iSensor)   const; 
   
   TClonesArray*      GetListOfClusters(Int_t iSensor);
   TClonesArray*      GetListOfClusters(Int_t iSensor) const;
   
   TAVTcluster*       NewCluster(Int_t iSensor);
   TAVTcluster*       NewCluster(TAVTcluster* clus, Int_t iSensor);

   virtual void       SetupClones();
   virtual void       Clear(Option_t* opt="");
   
   virtual void       ToStream(ostream& os=cout, Option_t* option="") const;
   
public:
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }

   ClassDef(TAVTntuCluster,1)
};

#endif


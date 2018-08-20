#ifndef _TAVTbaseCluster_HXX
#define _TAVTbaseCluster_HXX

// ROOT classes
#include "TObject.h"
#include "TList.h"
#include "TVector3.h"
#include "TClonesArray.h"
#include "TObjArray.h"

#include "TAGdata.hxx"
#include "TAVTntuRaw.hxx"


class TAVTparGeo;

/** TAVTbaseCluster class, simple container class for tracks with the associated clusters                    
 
 \author Ch. Finck
 */

class TAVTbaseCluster : public TObject {
   
protected:
   TVector3*          fPosition;                 // position of the cluster in plane frame
   TVector3*          fPosError;                 // position's errors of the cluster in plane frame 
   TVector3*          fPositionG;                // position of the clus in tracker frame
   TClonesArray*      fListOfPixels;             // list of pixel attached to this cluster
   
   Int_t              fDebugLevel;             // debug level
   Int_t              fNumber;                   // number
   Int_t              fPlaneNumber;              // plane number
   Bool_t             fFound;                    // kTRUE is associated to a track
   Bool_t             fFoundXZ;					 // kTRUE is associated to a track in XZ Projection
   Bool_t             fFoundYZ;					 // kTRUE is associated to a track in YZ Projection
	
   Float_t            fClusterPulseSum;          // sum of pulseheight on strips in hit cluster, involves noise cuts
   Float_t            fClusterAreaPulseSum;      // sum of pulseheight on strips in hit cluster area, no noise cuts
   Float_t            fClusterNoiseAverage;      // hit cluster signal noise average
   Float_t            fSNneighbour;              // signal / Noise of neighbours.;
   Int_t              fStripsInClusterArea;      // # strips in cluster area
   
   Float_t            fPhSeed;                   // pulseheight on seed strip
   Int_t              fIndexSeed;                // index of seed strip
   
public:
   TAVTbaseCluster(); 
   TAVTbaseCluster(const TAVTbaseCluster& cluster);
   ~TAVTbaseCluster();
   
   //! Set position in local frame
   void               SetPosition(TVector3* pos); 
   //! Set position error in local frame
   void               SetPosError(TVector3* pos); 
   void               SetPosition(Float_t u, Float_t v, Float_t z) { fPosition->SetXYZ(u,v,z); }
   //! Set position in global tracker frame
   void               SetPositionG(TVector3* pos); 
   //! Set pixel index for a given pixel
   void               SetIndexSeed(Int_t index)              { fIndexSeed = index;     } 
   //! Found flag for this cluster
   void               SetFound(Bool_t flag = true)           { fFound = flag;          }
   //! Found flag for this cluster (Hough Transform XZ)
   void               SetFoundXZ(Bool_t flag = true)         { fFoundXZ = flag;        }
   //! Found flag for this cluster ((Hough Transform YZ)
   void               SetFoundYZ(Bool_t flag = true)         { fFoundYZ = flag;        }
   //! Set cluster number
   void               SetNumber(Int_t nb)                    { fNumber = nb;           } 
   //! Set plane number
   void               SetPlaneNumber(Int_t nb)               { fPlaneNumber = nb;      }
   //! Set debug level
   void               SetDebug( Int_t aDebug)                { fDebugLevel = aDebug; }   
   
   //! Get position in local frame
   TVector3&           GetPosition()                    const { return *fPosition;      }
   //! Get position error in local frame
   TVector3&           GetPosError()                    const { return *fPosError;      }   
   //! Get position in global tracker frame
   TVector3&           GetPositionG()                   const { return *fPositionG ;    }
   //! Get U position in local frame
   Float_t            GetPositionU()                   const { return (*fPosition)(0); }
   //! Get V position in local frame
   Float_t            GetPositionV()                   const { return (*fPosition)(1); }
   //! Get Pixel list
   TClonesArray*      GetListOfPixels()                const { return fListOfPixels;   }
   //! Get cluster number
   Int_t              GetNumber()                      const { return fNumber;         }
   //! Get cluster number
   Int_t              GetPlaneNumber()                 const { return fPlaneNumber;    }
   //! Get found flag this cluster
   Bool_t             GetFound()                       const { return fFound;          }
   //! Get found flag this cluster
   Bool_t             GetFoundXZ()                     const { return fFoundXZ;        }
   //! Get found flag this cluster
   Bool_t             GetFoundYZ()                     const { return fFoundYZ;        }
   //! Get debug level
   Int_t              GetDebug()                       const { return fDebugLevel;   }
   //! Get index of seed pixel
   Int_t              GetIndexSeed()                   const { return fIndexSeed;      }
   
   //! Get index for a given pixel
   Int_t              GetIndex(Int_t tSk)              const { return ((TAVTntuHit*)fListOfPixels->At(tSk))->GetPixelIndex();  } 
   //! Get pulse height for a given pixel
   Float_t            GetPulseHeight(Int_t tSk)        const { return ((TAVTntuHit*)fListOfPixels->At(tSk))->GetPulseHeight(); } 
   //! Get number of pixels in this clusters
   Int_t              GetPixelsN()                     const { return  fListOfPixels->GetEntries(); }
   //! Get Sum of neighbour (not used)
   Float_t            GetSNneighbour()                 const { return  fSNneighbour;          } 
   //! Get sum of pulse of cluster for a given area
   Float_t            GetClusterAreaPulseSum()         const { return fClusterAreaPulseSum;  }
   //! Get sum of pulse of cluster 
   Float_t            GetClusterPulseSum();
   //! Get pixel
   TAVTntuHit*        GetPixel(Int_t idx);
   //! Get position of seed pixel
   Float_t            GetSeedU()                       const;
   Float_t            GetSeedV()                       const;
   
   //! Get distance from cluster position to a given pixel
   Float_t            GetPixelDistanceU(Int_t index)   const;
   Float_t            GetPixelDistanceV(Int_t index)   const;
   
   //! Compute distance from another cluster
   Float_t            Distance(TAVTbaseCluster *aClus);
   //! reset pixels
   void               ResetPixels();
   
   ClassDef(TAVTbaseCluster,2)                          // Describes TAVTbaseCluster
};


#endif


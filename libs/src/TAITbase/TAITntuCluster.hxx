#ifndef _TAITcluster_HXX
#define _TAITcluster_HXX

// ROOT classes
#include "TObject.h"
#include "TList.h"
#include "TVector3.h"
#include "TClonesArray.h"
#include "TObjArray.h"

#include "TAGdata.hxx"
#include "TAITntuRaw.hxx"


/** TAITcluster class, simple container class for tracks with the associated clusters                    
 \author Ch. Finck
 */


class TAITcluster : public TObject {
   
private:
   
   TAITparGeo* m_geometry;

   TVector3          fPosition;                 // position of the cluster in plane frame 
   TVector3          fPosError;                 // position's errors of the cluster in plane frame 
   TClonesArray*      fListOfPixels;             // list of pixel attached to this cluster
   
   // string             m_command;                 // type of pixel to be used for clustering

   Int_t              fDebugLevel;             // debug level
   int              m_clusterID;                   // number  -->  maybe its position
   int              m_sensorID;              // plane number
	
   Float_t            fClusterPulseSum;          // sum of pulseheight on strips in hit cluster, involves noise cuts
   // Float_t            fClusterAreaPulseSum;      // sum of pulseheight on strips in hit cluster area, no noise cuts
   // Float_t            fClusterNoiseAverage;      // hit cluster signal noise average
   // Float_t            fSNneighbour;              // signal / Noise of neighbours.;
   // Int_t              fStripsInClusterArea;      // # strips in cluster area
   
   // Float_t            fPhSeed;                   // pulseheight on seed strip
   // Int_t              fIndexSeed;                // index of seed strip
   
public:

    TAITcluster(); 
    TAITcluster(const TAITcluster& cluster);
    ~TAITcluster();
   

    void               AddPixel(TAITntuHit* pixel);    //! Add pixel to the list
    void               ResetPixels();



    void               SetPosition(TVector3 pos)  { fPosition = pos; };     //! Set position in local detector frame
    void               SetPosError(TVector3 pos)  { fPosError = pos; };     //! Set position error in local frame
    
    //! Set pixel index for a given pixel  -->  remove
    // void               SetIndexSeed(Int_t index)              { fIndexSeed = index;     } 


    void               SetClusterID(int nb)                 { m_clusterID = nb; }     //! Set cluster number   -->  remove?
    
    void               SetSensorID(Int_t nb)               { m_sensorID = nb; }    
    
 
    //! Get U position in local frame
    // Float_t            GetPositionU()                   const { return (*fPosition)(0); }
    // //! Get V position in local frame
    // Float_t            GetPositionV()                   const { return (*fPosition)(1); }

    TVector3           GetPosition_sensorFrame();
    TVector3           GetPosition_detectorFrame()     { return fPosition;      }
    TVector3           GetPosition_footFrame();

    TVector3           GetPosError()                   { return fPosError;      }   

    TAITntuHit*        GetPixel(Int_t idx);    //! Get pixel, takes the index of the list interal to the cluster
    
    TClonesArray*      GetListOfPixels()                const { return fListOfPixels;   }    //! Get Pixel list  -->  not safe
    int                GetClusterID()                   { return m_clusterID; }
    int                GetSensorID()                    { return m_sensorID; }

    //! Get index of seed pixel  -->  remove
    // Int_t              GetIndexSeed()                   const { return fIndexSeed;      }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! Get index for a given pixel
    // Int_t              GetIndex(Int_t tSk)              const { return ((TAITntuHit*)fListOfPixels->At(tSk))->GetPixelIndex();  } 
    //! Get pulse height for a given pixel
    Float_t            GetPulseHeight(Int_t tSk)        const { return ((TAITntuHit*)fListOfPixels->At(tSk))->GetPulseHeight(); } 
    //! Get number of pixels in this clusters
    Int_t              GetPixelsN()                     const { return  fListOfPixels->GetEntries(); }
    // /! Get position of seed pixel  -->  change
    // Float_t            GetSeedU()                       const;
    // Float_t            GetSeedV()                       const;

    //! Get distance from cluster position to a given pixel
    Float_t            GetPixelDistanceU(Int_t index)   const;
    Float_t            GetPixelDistanceV(Int_t index)   const;

    //! Compute distance from another cluster
    Float_t            Distance(TAITcluster *aClus);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // //! Get Sum of neighbour (not used)
    // Float_t            GetSNneighbour()                 const { return  fSNneighbour;          } 
    // //! Get sum of pulse of cluster for a given area
    // Float_t            GetClusterAreaPulseSum()         const { return fClusterAreaPulseSum;  }
    //! Get sum of pulse of cluster 
    Float_t            GetClusterPulseSum();

   
   
   
   ClassDef(TAITcluster,2)                          // Describes TAITcluster
};





//##############################################################################

class TAITntuCluster : public TAGdata {
   
private:
   TObjArray*         fListOfClusters; 
   static TString    fgkBranchName;    // Branch name in TTree
   TAITparGeo* m_geometry;

public:
   TAITntuCluster();
   virtual            ~TAITntuCluster();
   
   TAITcluster*       GetCluster(Int_t iSensor, Int_t i);
   const TAITcluster* GetCluster(Int_t iSensor, Int_t i) const;
   
   Int_t              GetClustersN(Int_t iSensor)   const; 
   
   TClonesArray*      GetListOfClusters(Int_t iSensor);
   TClonesArray*      GetListOfClusters(Int_t iSensor) const;
   
   TAITcluster*       NewCluster(Int_t iSensor);
   TAITcluster*       NewCluster(TAITcluster* clus, Int_t iSensor);

   virtual void       SetupClones();
   virtual void       Clear(Option_t* opt="");
   
   virtual void       ToStream(ostream& os=cout, Option_t* option="") const;
   
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }

   ClassDef(TAITntuCluster,1)
};

#endif


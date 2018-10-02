
#ifndef _BaseLocalReco_HXX_
#define _BaseLocalReco_HXX_

#include "TNamed.h"

#include "TAGaction.hxx"
#include "TAGactTreeWriter.hxx"

#include "TAVTactNtuClusterF.hxx"
#include "TAVTactNtuTrackF.hxx"
#include "TAVTactNtuTrack.hxx"
#include "TAVTactNtuVertexPD.hxx"

#include "TAITactNtuClusterF.hxx"
#include "TAMSDactNtuCluster.hxx"


class BaseLocalReco : public TNamed // using TNamed for the in/out files
{
public:
   //! default constructor
   BaseLocalReco(TString fileNameIn, TString fileNameout);
   
   virtual ~BaseLocalReco();
   
   //! Create raw action
   virtual void CreateRecAction();
   
   //! Create rec action
   virtual void CreateRawAction()      { return; }
   
   //! Add required items
   virtual void AddRawRequiredItem()   { return; }
   
   //! Add required items
   virtual void AddRecRequiredItem();
   
   //! Set raw histogram directory
   virtual void SetRawHistogramDir()   { return; }
   
   //! Set rec histogram directory
   virtual void SetRecHistogramDir();
   
   //! Loop events
   virtual void LoopEvent(Int_t /*nEvents*/) { return; }
   
   //! Begin loop
   virtual void BeforeEventLoop();

   //! End loop
   virtual void AfterEventLoop();
   
   //! Open File In
   virtual void OpenFileIn()  { return; }
   
   //! Close File in
   virtual void CloseFileIn() { return; }

   //! Open File Out
   virtual void OpenFileOut();
   
   //! Close File Out
   virtual void CloseFileOut();
   
   //! Create branch in tree
   virtual void SetTreeBranches();
   
   //! Enable detectors
   void EnableTr()  { fFlagTr = true;  }
   void EnableBm()  { fFlagBm = true;  }
   void EnableVtx() { fFlagVtx = true; }
   void EnableIt()  { fFlagIt = true;  }
   void EnableMsd() { fFlagMsd = true; }
   void EnableTw()  { fFlagTw = true;  }
   void EnableCa()  { fFlagCa = true;  }
   
   // Enable global
   void EnableTree()      { fFlagTree = true;      }
   void DisableTree()     { fFlagTree = false;     }
   
   void EnableHisto()     { fFlagHisto = true;     }
   void DisableHisto()    { fFlagHisto = false;    }
   
   void EnableVtxTrack()  { fVtxTrackFlag = true;  }
   void DisableVtxTrack() { fVtxTrackFlag = false; }

   //! Set Tracking algorithm
   void SetTrackingAlgo(char c);
  
private:
   void InitParameters();

protected:
   TAGroot*              fTAGroot;             // pointer to TAGroot
   TAGparaDsc*           fpParGeoTr;
   TAGparaDsc*           fpParGeoBm;
   
   TAGparaDsc*           fpParGeoIt;
   TAGparaDsc*           fpParGeoVtx;
   TAGparaDsc*           fpParGeoMsd;
   TAGparaDsc*           fpParGeoTw;
   TAGparaDsc*           fpParGeoCa;
   
   TAGparaDsc*           fpParConfIt;
   TAGparaDsc*           fpParConfVtx;
   TAGparaDsc*           fpParConfMsd;
   
   TAGdataDsc*           fpNtuRawVtx;	  // input raw data dsc
   TAGdataDsc*           fpNtuClusVtx;	  // input cluster data dsc
   TAGdataDsc*           fpNtuTrackVtx;  // input track data dsc
   TAGdataDsc*           fpNtuVtx;		  // input Vtx data dsc
   
   TAGdataDsc*           fpNtuRawIt;	  // input raw data dsc
   TAGdataDsc*           fpNtuClusIt;	  // input cluster data dsc
   
   TAGdataDsc*           fpNtuRawMsd;	  // input raw data dsc
   TAGdataDsc*           fpNtuClusMsd;	  // input cluster data dsc

   TAGactTreeWriter*     fActEvtWriter;  // write histo and tree
   
   TAVTactNtuClusterF*   fActClusVtx;    // action for clusters
   TAVTactBaseNtuTrack*  fActTrackVtx;   // action for tracks
   TAVTactBaseNtuVertex* fActVtx;        // action for vertex
   
   TAITactNtuClusterF*    fActClusIt;    // action for clusters
   TAMSDactNtuCluster*    fActClusMsd;    // action for clusters
   
   Bool_t                fFlagTr;
   Bool_t                fFlagBm;
   Bool_t                fFlagVtx;
   Bool_t                fFlagIt;
   Bool_t                fFlagMsd;
   Bool_t                fFlagTw;
   Bool_t                fFlagCa;
   
   Bool_t                fFlagTree;
   Bool_t                fFlagHisto;
   Bool_t                fVtxTrackFlag;      // flag for tracking
   TString               fTrackingAlgo;   // tracking algorithm ("std" with BM, "Full" combinatory)

   ClassDef(BaseLocalReco, 1); // Base class for event display
};


#endif

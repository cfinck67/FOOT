
#ifndef _BaseLocalReco_HXX_
#define _BaseLocalReco_HXX_

#include "TNamed.h"
#include "TString.h"

#include "TAGaction.hxx"
#include "TAGactTreeWriter.hxx"
#include "TAGgeoTrafo.hxx"

#include "TASTparMap.hxx"
#include "TABMparMap.hxx"

#include "TASTparGeo.hxx"
#include "TABMparGeo.hxx"
#include "TAGparGeo.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparGeo.hxx"
#include "TAITparGeo.hxx"
#include "TADIparGeo.hxx"
#include "TAMSDparGeo.hxx"
#include "TATWparGeo.hxx"
#include "TACAparGeo.hxx"

#include "TATWparCal.hxx"

#include "TABMparCon.hxx"
#include "TAVTparConf.hxx"
#include "TAITparConf.hxx"
#include "TAMSDparConf.hxx"

#include "TAGclusterDisplay.hxx"
#include "TAGtrackDisplay.hxx"
#include "TAGwireDisplay.hxx"
#include "TAGglbTrackDisplay.hxx"
#include "TAGbaseEventDisplay.hxx"
#include "TAGactionFile.hxx"

#include "TAVTactNtuClusterF.hxx"
#include "TAITactNtuClusterF.hxx"
#include "TAMSDactNtuCluster.hxx"
#include "TATWactNtuPoint.hxx"

#include "TABMactNtuTrack.hxx"
#include "TAVTactBaseNtuTrack.hxx"
#include "TAVTactNtuVertex.hxx"

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
   
   //! Set experiment name
   virtual void SetExpName(const Char_t* name) { fExpName = name; }
   
   // Enable global
   void EnableTree()      { fFlagTree = true;    }
   void DisableTree()     { fFlagTree = false;   }
   
   void EnableSaveHits()  { fFlagHits = true;    }
   void DisableDaveHits() { fFlagHits = false;   }

   
   void EnableHisto()     { fFlagHisto = true;   }
   void DisableHisto()    { fFlagHisto = false;  }
   
   void EnableTracking()  { fFlagTrack = true;  }
   void DisableTracking() { fFlagTrack = false; }

   //! Set Tracking algorithm
   void SetTrackingAlgo(char c);
  
private:
   void InitParameters();

protected:
   TString               fExpName;
   TAGroot*              fTAGroot;             // pointer to TAGroot
   TAGgeoTrafo*          fpFootGeo;           // trafo prointer

   TAGparaDsc*           fpParMapSt;
   TAGparaDsc*           fpParMapBm;

   TAGparaDsc*           fpParGeoSt;
   TAGparaDsc*           fpParGeoG;
   TAGparaDsc*           fpParGeoDi;
   TAGparaDsc*           fpParGeoBm;
   
   TAGparaDsc*           fpParGeoIt;
   TAGparaDsc*           fpParGeoVtx;
   TAGparaDsc*           fpParGeoMsd;
   TAGparaDsc*           fpParGeoTw;
   TAGparaDsc*           fpParGeoCa;
   
   TAGparaDsc*           fpParCalTw;
   
   TAGparaDsc*           fpParConfBm;
   TAGparaDsc*           fpParConfIt;
   TAGparaDsc*           fpParConfVtx;
   TAGparaDsc*           fpParConfMsd;
   
   TAGdataDsc*           fpDatRawSt;    // input data dsc
   TAGdataDsc*           fpNtuRawSt;    // input data dsc
   TAGdataDsc*           fpDatRawBm;    // input data dsc
   TAGdataDsc*           fpNtuRawBm;    // input data dsc
   TAGdataDsc*           fpNtuTrackBm;  // input track data dsc
   
   TAGdataDsc*           fpDatRawVtx;    // input data dsc
   TAGdataDsc*           fpNtuRawVtx;	  // input ntu data dsc
   TAGdataDsc*           fpNtuClusVtx;	  // input cluster data dsc
   TAGdataDsc*           fpNtuTrackVtx;  // input track data dsc
   TAGdataDsc*           fpNtuVtx;		  // input Vtx data dsc
   
   TAGdataDsc*           fpDatRawIt;     // input data dsc
   TAGdataDsc*           fpNtuRawIt;	  // input ntu data dsc
   TAGdataDsc*           fpNtuClusIt;	  // input cluster data dsc
   
   TAGdataDsc*           fpDatRawMsd;    // input data dsc
   TAGdataDsc*           fpNtuRawMsd;	  // input ntu data dsc
   TAGdataDsc*           fpNtuClusMsd;	  // input cluster data dsc
   
   TAGdataDsc*           fpNtuRawTw;     // input data dsc
   TAGdataDsc*           fpNtuRecTw;     // input data dsc
   TAGdataDsc*           fpNtuRawCa;     // input data dsc
   
   TAGactionFile*        fActEvtReader;
   TAGactTreeWriter*     fActEvtWriter;  // write histo and tree

   TABMactNtuTrack*      fActTrackBm;    // action for tracks
   
   TAVTactNtuClusterF*   fActClusVtx;    // action for clusters
   TAVTactBaseNtuTrack*  fActTrackVtx;   // action for tracks
   TAVTactBaseNtuVertex* fActVtx;        // action for vertex
   
   TAITactNtuClusterF*   fActClusIt;     // action for clusters
   
   TAMSDactNtuCluster*   fActClusMsd;    // action for clusters
   
   // TATWactNtuRaw*        fActNtuRawTw;  // action for ntu data
   TATWactNtuPoint*      fActPointTw;    // action for clusters
   
   Bool_t                fFlagTree;      // flag to save in tree
   Bool_t                fFlagHits;      // flag to save hits in tree
   Bool_t                fFlagHisto;     // flag for histo generatiom
   Bool_t                fFlagTrack;     // flag for tracking
   TString               fgTrackingAlgo; // tracking algorithm ("std" with BM, "Full" combinatory)

protected:
   void CreateRecActionBm();
   void CreateRecActionVtx();
   void CreateRecActionIt();
   void CreateRecActionMsd();
   void CreateRecActionTw();
   
   void AddRequiredItemSt();
   void AddRequiredItemBm();
   void AddRequiredItemVtx();
   void AddRequiredItemIt();
   void AddRequiredItemMsd();
   void AddRequiredItemTw();
   void AddRequiredItemCa();
   
   ClassDef(BaseLocalReco, 1); // Base class for event display
};


#endif

#ifndef MultiTrackCheck_hxx
#define MultiTrackCheck_hxx


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

#include "Booter.hxx"

//MC block
#include "TAGntuMCeve.hxx"
#include "TAGactNtuMCeve.hxx"

#include "TAVTparMap.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"
#include "TAVTparCal.hxx"
#include "TAVTdatRaw.hxx"
#include "TAVTntuRaw.hxx"
#include "TAVTntuCluster.hxx"
#include "TAVTntuTrack.hxx"
#include "TAVTntuVertex.hxx"
#include "TAVTactNtuVertex.hxx"
#include "TAVTactNtuVertexPD.hxx"
#include "TAVTactNtuMC.hxx"
#include "TAVTactNtuClusterF.hxx"
#include "TAVTactNtuTrack.hxx"
#include "TAVTactNtuTrackH.hxx"
#include "TAVTactNtuTrackF.hxx"

class MultiTrackCheck{

public :
  
  MultiTrackCheck();
  ~MultiTrackCheck() {};
  
  void Initialize( EVENT_STRUCT* evStr );
  void Process( Long64_t jentry );
  void Finalize();

private:

  int debug_multi;
  
  void BookHisto();

  // //MC block
  TAGdataDsc* myn_mceve;
  TAGaction* mya_mceve;
  
 
  TAITntuRaw* myn_itraw;
  TAVTntuRaw* myn_vtraw;
  TAMSDntuRaw* myn_msdraw;
 
  
  TFile* f;
  TH1F* histo_Track;
  TH1F* histo_VT;
  TH1F* histo_IT;
  TH1F* histo_MSD;
  TH1F* histo_TrackingHit;
  TH2F* h_zx;
  TH2F* h_zy;
  TH2F* h_yx;
  TH2F* h_zr;
  TH2F* h_rphi;
  TH2F* h_etaphi;
  TH2F* h_thetaphi;
  
  vector<TAVTntuHit*> m_VT_hitCollection;
  vector<TAITntuHit*> m_IT_hitCollection;
  vector<TAMSDntuHit*> m_MSD_hitCollection;
  vector<TAGntuMCeveHit*> m_Frag_hitCollection;
  vector<TAGntuMCeveHit*> m_Trcks9hits;
  
  vector<int> m_Frag_indexCollection;
  vector< int > m_VT_partIDColl;

  vector<int> m_FragIdxs;
  vector<int> m_Trcks9index;
  map< string, vector<TVector3>> m_DetTrackpos;
  vector<TVector3> m_VTX_Trackpos;
  vector<TVector3> m_IT_Trackpos;
  vector<TVector3> m_MSD_Trackpos;

  int tmpVT_partID, VT_partID, VT_charge;
  int tmpIT_partID, IT_partID, IT_charge;
  int tmpMSD_partID, MSD_partID, MSD_charge;

  
 
  
  //map< int,  vector<TAITntuHit*>> m_IT_hitColl;
  // map< int,  vector<TAMSDntuHit*>> m_MSD_hitColl;
};
#endif

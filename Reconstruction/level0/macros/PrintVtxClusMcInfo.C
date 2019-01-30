// Macro to read back MC info from reconstructed cluster of vertex
// Ch. Finck, Jan 19.


#if !defined(__CINT__) || defined(__MAKECINT__)

#include <Riostream.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TVector3.h>

#include "TAMCntuEve.hxx"
#include "TAMCntuHit.hxx"

#include "TAVTntuRaw.hxx"
#include "TAVTntuCluster.hxx"
#endif

// main
void PrintVtxClusMcInfo(TString nameFile = "12C_400_vtx_Out.root", Int_t nentries = 2, Int_t plane = 0)
{
   TAGroot gTAGroot;
   
   TAGparaDsc* parGeoVtx = new TAGparaDsc(TAVTparGeo::GetDefParaName(), new TAVTparGeo());
   TAVTparGeo* parGeo = (TAVTparGeo*)parGeoVtx->Object();
   parGeo->FromFile();
   
   TTree *tree = 0;
   TFile *f = new TFile(nameFile.Data());
   tree = (TTree*)f->Get("tree");
   
   TAVTntuCluster *vtClus = new TAVTntuCluster();
   vtClus->SetParGeometry(parGeo);
   tree->SetBranchAddress(TAVTntuCluster::GetBranchName(), &vtClus);
   
   TAMCntuEve *eve = new TAMCntuEve();
   tree->SetBranchAddress(TAMCntuEve::GetBranchName(), &eve);

   TAMCntuHit *vtMc = new TAMCntuHit();
   tree->SetBranchAddress(TAMCntuHit::GetVtxBranchName(), &vtMc);
   
   if (nentries == 0)
      nentries = tree->GetEntries();
   printf("nentries %d\n", nentries);
   
   printf("plane %d\n", plane);
   
   for (Int_t ev = 0; ev < nentries; ++ev) {

      printf("Event: %d\n", ev);
      vtClus->Clear();
      vtMc->Clear();
      eve->Clear();
      tree->GetEntry(ev);

      Int_t nClus = vtClus->GetClustersN(plane);
      printf("nClusters %d\n", nClus);
      
      for(Int_t i = 0; i < nClus; ++i){
         printf("Clusters# %d\n", i);
         TAVTcluster* clus = vtClus->GetCluster(plane, i);
         Int_t nHits = clus->GetPixelsN();
         
         for (Int_t j = 0; j < nHits; ++j) {
            TAVTntuHit* hit = clus->GetPixel(j);
            for (Int_t k = 0; k < hit->GetMcTrackCount(); ++k) {
               Int_t id = hit->GetMcTrackId(k);
               Int_t idx = hit->GetMcIndex(k);
               printf("TrackMcId %d ", id);
               TAMCeveTrack* track = eve->GetHit(id);
               printf("charge %d mass %g ", track->GetCharge(), track->GetMass());
               TAMChit* mcHit = vtMc->GetHit(idx);
               TVector3 pos = mcHit->GetPosition();
               printf("MC pos (%.4f %.4f %.4f)\n", pos[0], pos[1], pos[2]);
            }
         }
      }
   }
}




#include "Riostream.h"

#ifndef _TAVTeventDisplayMC_HXX_
#include "TAVTeventDisplayMC.hxx"
#endif

#ifndef ROOT_TFile
#include "TFile.h"
#endif

#include "Evento.hxx"
//#include "Segnale.h"
//#include "Geometry.h"

#include "TAVTparMap.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"

#include "TAVTntuRaw.hxx"
#include "TAVTactNtuMC.hxx"

ClassImp(TAVTeventDisplayMC)

TAVTeventDisplayMC* TAVTeventDisplayMC::fgInstance = 0x0;

//__________________________________________________________
TAVTeventDisplayMC* TAVTeventDisplayMC::Instance(const TString name)
{
   if (fgInstance == 0x0) 
	  fgInstance = new TAVTeventDisplayMC(name);

   return fgInstance;
}

//__________________________________________________________
TAVTeventDisplayMC::TAVTeventDisplayMC(const TString name)
: TAVTbaseEventDisplay(name),
  fTree(0x0)
{ 
   
}

//__________________________________________________________
TAVTeventDisplayMC::~TAVTeventDisplayMC()
{
   // default destructor
   delete fTree;
}

//__________________________________________________________
Bool_t TAVTeventDisplayMC::SetNextEvent()
{
   fTree->GetEntry(fCurrentEventId);
   return fAGRoot->NextEvent();
}

//__________________________________________________________
void TAVTeventDisplayMC::CreateRawAction()
{   
   // default constructor   
   TFile* f = new TFile(fFileName.Data());
   fTree = (TTree*)f->Get("EventTree");
   
   if (fTree == 0x0) {
	  printf("EventTree not found in file %s\n", fFileName.Data());  
	  return;
   }
   
   //   Geometry* my_G = new Geometry(); 
   //   Segnale*  my_S = new Segnale(my_G); 
   Evento*   ev   = new Evento();
   
   EVENT_STRUCT evStr;
   
   ev->FindBranches(fTree,&evStr);
   
   TAVTparConf::SetHistoMap();
   
   fpParMap    = new TAGparaDsc("vtMap", new TAVTparMap());
   
   fpGeoMap    = new TAGparaDsc("vtGeo", new TAVTparGeo());
   TAVTparGeo* geoMap   = (TAVTparGeo*) fpGeoMap->Object();
   geoMap->FromFile("TAVTdetector_MC.map");
   
   fpConfig  = new TAGparaDsc("vtConf", new TAVTparConf());
   TAVTparConf* parconf = (TAVTparConf*) fpConfig->Object();
   parconf->FromFile("");
   
   fpNtuRaw  = new TAGdataDsc("vtRaw", new TAVTntuRaw());
   fActMCRaw = new TAVTactNtuMC("vtActMCRaw", fpNtuRaw, fpGeoMap, fpParMap, &evStr);
   fActMCRaw->CreateHistogram();   
}

//__________________________________________________________
void TAVTeventDisplayMC::DrawRawdata()
{
   TAVTparGeo* geoMap    = (TAVTparGeo*) fpGeoMap->Object();
   Int_t nPlanes         = geoMap->GetSensorsN();

   TList* histoDat  = fActMCRaw->GetHistogrammList();
   TVirtualPad* pad1 = (TVirtualPad*)fListOfPads->FindObject("VtxPixelPos2D");
   TVirtualPad* pad2 = (TVirtualPad*)fListOfPads->FindObject("VtxPixelRaw2D");

   for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
	  TH2F* pHisPosMap = (TH2F*)histoDat->FindObject(Form("vtPosMap%d", iPlane+1));
	  pad1->cd(iPlane+1);
	  pHisPosMap->Draw("colz");
   
	  TH2F* pHisPixel = (TH2F*)histoDat->FindObject(Form("vtPixelMap%d", iPlane+1));
	  pad2->cd(iPlane+1);
	  pHisPixel->Draw("colz");
   }
}

//__________________________________________________________
void TAVTeventDisplayMC::DrawRate()
{
   TAVTparGeo* geoMap    = (TAVTparGeo*) fpGeoMap->Object();
   Int_t nPlanes         = geoMap->GetSensorsN();
   Int_t entries         = fRefreshEvent->GetIntNumber();
   
   TList* histoDat  = fActMCRaw->GetHistogrammList();
   TVirtualPad* pad6  = (TVirtualPad*)fListOfPads->FindObject("VtxIndRates");
   TVirtualPad* pad10 = (TVirtualPad*)fListOfPads->FindObject("VtxIndQRates");

   for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
	  
	  TH2F* pHisRateMap = (TH2F*)histoDat->FindObject(Form("vtRateMap%d", iPlane+1));
	  pHisRateMap->Scale(1./(entries*3.*576.));
	  pad6->cd(iPlane+1);
	  if (pHisRateMap->GetEntries() != 0)
		 gPad->SetLogy();
	  pHisRateMap->DrawCopy();
	  
	  TH1F* pHisQRate = (TH1F*)histoDat->FindObject(Form("vtRateMapQ%d", iPlane+1));
	  pHisQRate->Scale(1./(entries*3.*576.));
	  pad10->cd(iPlane+1);
	  pHisQRate->DrawCopy();
   }
}

//__________________________________________________________
void TAVTeventDisplayMC::ResetRate()
{
   TAVTparGeo* geoMap    = (TAVTparGeo*) fpGeoMap->Object();
   Int_t nPlanes         = geoMap->GetSensorsN();
   
   TList* histoDat  = fActMCRaw->GetHistogrammList();
   for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
	  if (fRateButton->IsOn()) {
		 TH1F* pHisRateMap = (TH1F*)histoDat->FindObject(Form("vtRateMap%d", iPlane+1));
		 pHisRateMap->Reset(); 
		 
		 TH1F* pHisQRate = (TH1F*)histoDat->FindObject(Form("vtRateMapQ%d", iPlane+1));
		 pHisQRate->Reset();
	  }
   }
}

//__________________________________________________________
void TAVTeventDisplayMC::ResetHistogram()
{   
   TAVTparGeo* geoMap    = (TAVTparGeo*) fpGeoMap->Object();
   Int_t nPlanes         = geoMap->GetSensorsN();

   TList* histoDat  = fActMCRaw->GetHistogrammList();
   for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
	  if (fRawDataButton->IsOn()) {
		 TH2F* pHisPixel = (TH2F*)histoDat->FindObject(Form("vtPixelMap%d", iPlane+1));
		 pHisPixel->Reset();
		 
		 TH2F* pHisPosMap = (TH2F*)histoDat->FindObject(Form("vtPosMap%d", iPlane+1));
		 pHisPosMap->Reset();
	  }
	  
	  if (fRateButton->IsOn()) {
		 TH2F* pHisRateMap = (TH2F*)histoDat->FindObject(Form("vtRateMap%d", iPlane+1));
		 pHisRateMap->Reset();
	  }
   }	  
}
//__________________________________________________________
void TAVTeventDisplayMC::AddRequiredItem()
{
   fAGRoot->AddRequiredItem("vtActMCRaw");
   
   if (fgDisplayFlag) {
	  fAGRoot->AddRequiredItem("vtActClus");
	  fAGRoot->AddRequiredItem("vtActTrack");
   }
   fAGRoot->BeginEventLoop();
   fAGRoot->Print();   
}   

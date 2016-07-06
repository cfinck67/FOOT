
#include <Riostream.h>

#ifndef _TAVTeventDisplay_HXX_
#include "TAVTeventDisplay.hxx"
#endif

// root include 

#ifndef ROOT_TGeoManager
#include "TGeoManager.h"
#endif

#ifndef ROOT_TGeoNode
#include "TGeoNode.h"
#endif

#ifndef ROOT_TGeoVolume
#include "TGeoVolume.h"
#endif

#ifndef ROOT_TGButton
#include "TGButton.h"
#endif

#ifndef ROOT_TGLViewer
#include "TGLViewer.h"
#endif

#ifndef ROOT_TList
#include "TList.h"
#endif

#ifndef ROOT_TMath
#include "TMath.h"
#endif

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

#ifndef ROOT_TPad
#include "TPad.h"
#endif

#ifndef ROOT_TPaveLabel
#include "TPaveLabel.h"
#endif

#ifndef ROOT_TRootEmbeddedCanvas
#include "TRootEmbeddedCanvas.h"
#endif

#ifndef ROOT_TVector3
#include "TVector3.h"
#endif

#ifndef ROOT_TVirtualPad
#include "TVirtualPad.h"
#endif

#include "TAGgeoTrafo.hxx"

#include "TAGactTreeWriter.hxx"
#include "TAGactMbsReader.hxx"
#include "TAVTactAscReader.hxx"

#include "TAGmbsEvent.hxx"
#include "TAVTrawEvent.hxx"
#include "TAVTactAscRaw.hxx"
#include "TAGmbsEventFilterType.hxx"

#include "TAVTparMap.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"

#include "TAVTdatRaw.hxx"
#include "TAVTntuRaw.hxx"

#include "TAVTactDatRaw.hxx"
#include "TAVTactAscRaw.hxx"

//
ClassImp(TAVTeventDisplay)

TAVTeventDisplay* TAVTeventDisplay::fgInstance = 0x0;

//__________________________________________________________
TAVTeventDisplay* TAVTeventDisplay::Instance(const TString name, Int_t type)
{
   if (fgInstance == 0x0) 
	  fgInstance = new TAVTeventDisplay(name, type);
   
   return fgInstance;
}

//__________________________________________________________
TAVTeventDisplay::TAVTeventDisplay(const TString name, Int_t type) 
  : TAVTbaseEventDisplay(name, type)
{ 
  // default constructor
}

//__________________________________________________________
TAVTeventDisplay::~TAVTeventDisplay()
{
  // default destructor
}

//__________________________________________________________
void TAVTeventDisplay::CreateRawAction()
{
   TString confFile;
   TString geoFile;
   if (fType == 0) {
	  confFile = "";
	  geoFile  = "";
	  fpEvent = new TAGdataDsc("vtEvent", new TAGmbsEvent());
   } else if (fType == 1 || fType == 2) {
	  confFile = "TAVTdetector_Cat.cfg";
	  geoFile  = "TAVTdetector_Cat.map";
	  fpEvent = new TAGdataDsc("vtEvent", new TAVTrawEvent());
   } else {
	  printf("Undefined type for event\n");
	  return;
   }

   TAVTparConf::SetHistoMap();

   fpParMap    = new TAGparaDsc("vtMap", new TAVTparMap());
   
   fpGeoMap    = new TAGparaDsc("vtGeo", new TAVTparGeo());
   TAVTparGeo* geoMap   = (TAVTparGeo*) fpGeoMap->Object();
   if (geoMap->FromFile(geoFile.Data())) exit(0);
   
   fpConfig  = new TAGparaDsc("vtConf", new TAVTparConf());
   TAVTparConf* parconf = (TAVTparConf*) fpConfig->Object();
   if (parconf->FromFile(confFile.Data())) exit(0);
   
   fpDatRaw    = new TAGdataDsc("vtDat", new TAVTdatRaw());
   fpNtuRaw    = new TAGdataDsc("vtRaw", new TAVTntuRaw());
   
   if (fType == 0) {
	  fActEvtReader = new TAGactMbsReader("vtActEvtReader");
	  fActEvtReader->SetupChannel(fpEvent,  new TAGmbsEventFilterType(42));
	  fActDatRaw  = new TAVTactDatRaw("vtActRaw", fpDatRaw, fpEvent, fpGeoMap, fpConfig);   
   } else if (fType == 1) {
	  fActEvtReader = new TAVTactAscReader("vtActEvtReader", fpEvent, fpConfig);
	  fActDatRaw  = new TAVTactAscRaw("vtActRaw", fpDatRaw, fpEvent, fpGeoMap);   
   }   
   fActEvtReader->Open(fFileName.Data());
   
   fActDatRaw->CreateHistogram();
   fActNtuRaw = new TAVTactNtuRaw("vtActNtu", fpNtuRaw, fpDatRaw, fpParMap, fpGeoMap);
   fActNtuRaw->CreateHistogram();   
}


//__________________________________________________________
void TAVTeventDisplay::ResetHistogram()
{
   TList* histoList  = 0x0;
   histoList = fActClus->GetHistogrammList();
   for (Int_t i = 0; i < histoList->GetEntries(); ++i) {
	  TH1* h = (TH1*)histoList->At(i);
	  h->Reset();
   }
   
   histoList = fActTrack->GetHistogrammList();
   for (Int_t i = 0; i < histoList->GetEntries(); ++i) {
	  TH1* h = (TH1*)histoList->At(i);
	  h->Reset();
   }
}

//__________________________________________________________
void TAVTeventDisplay::DrawRawdata()
{
   TAVTparGeo* geoMap    = (TAVTparGeo*) fpGeoMap->Object();
   Int_t nPlanes         = geoMap->GetSensorsN();
   
   TList* histoDat  = fActDatRaw->GetHistogrammList();
   TVirtualPad* pad2 = (TVirtualPad*)fListOfPads->FindObject("VtxPixelRaw2D");
   for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
	  TH2F* pHisPixel = (TH2F*)histoDat->FindObject(Form("vtPixelMap%d", iPlane+1));
	  pad2->cd(iPlane+1);
	  pHisPixel->Draw("colz");
   }
}

//__________________________________________________________
void TAVTeventDisplay::DrawRate()
{
   TAVTparGeo* geoMap    = (TAVTparGeo*) fpGeoMap->Object();
   Int_t nPlanes         = geoMap->GetSensorsN();
   Int_t entries         = fRefreshEvent->GetIntNumber();
   
   TList* histoDat  = fActDatRaw->GetHistogrammList();
   TVirtualPad* pad6  = (TVirtualPad*)fListOfPads->FindObject("VtxIndRates");
   TVirtualPad* pad9 = (TVirtualPad*)fListOfPads->FindObject("VtxIndLengthEvt");
   TVirtualPad* pad10 = (TVirtualPad*)fListOfPads->FindObject("VtxIndQRates");
   
   for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
	  
	  TH1F* pHisRateMap = (TH1F*)histoDat->FindObject(Form("vtRateMap%d", iPlane+1));
	  pHisRateMap->Scale(1./(entries*3.*geoMap->GetPixelsNv()));
	  pad6->cd(iPlane+1);
	  if (pHisRateMap->GetEntries() != 0)
		 gPad->SetLogy();
	  pHisRateMap->DrawCopy(); 
	  
	  TH1F* pHisQRate = (TH1F*)histoDat->FindObject(Form("vtRateMapQ%d", iPlane+1));
	  pHisQRate->Scale(1./(entries*3.*geoMap->GetPixelsNv()));
	  pad10->cd(iPlane+1);
	  pHisQRate->DrawCopy();
	  
	  TH1F* pHisEvtLength = (TH1F*)histoDat->FindObject(Form("vtEvtLength%d", iPlane+1));
	  pad9->cd(iPlane+1);
	  pHisEvtLength->Draw(); 
   }
}

//__________________________________________________________
void TAVTeventDisplay::ResetRate()
{
   TAVTparGeo* geoMap    = (TAVTparGeo*) fpGeoMap->Object();
   Int_t nPlanes         = geoMap->GetSensorsN();
   
   TList* histoDat  = fActDatRaw->GetHistogrammList();
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
void TAVTeventDisplay::AddRequiredItem()
{
   fAGRoot->AddRequiredItem("vtActRaw");
   fAGRoot->AddRequiredItem("vtActNtu");
   
   if (fgDisplayFlag) {
	  fAGRoot->AddRequiredItem("vtActClus");
	  fAGRoot->AddRequiredItem("vtActTrack");
	  fAGRoot->AddRequiredItem("vtActVtx");
	  if (fgPostTracking) 
		 fAGRoot->AddRequiredItem("vtActPostTrack");
   }
   
   fAGRoot->BeginEventLoop();
   fAGRoot->Print();      
}


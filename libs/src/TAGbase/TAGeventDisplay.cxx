
#include <Riostream.h>

#ifndef _TAGeventDisplay_HXX_
#include "TAGeventDisplay.hxx"
#endif

// root include 
#ifndef ROOT_TEveGeoNode
#include "TEveGeoNode.h"
#endif

#ifndef ROOT_TEveManager
#include "TEveManager.h"
#endif

#ifndef ROOT_TEveBrowser
#include "TEveBrowser.h"
#endif

#ifndef ROOT_TEveWindow
#include "TEveWindow.h"
#endif

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

#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif

#ifndef ROOT_TList
#include "TList.h"
#endif

#ifndef ROOT_TMath
#include "TMath.h"
#endif

#ifndef ROOT_TStopwatch
#include "TStopwatch.h"
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

#ifndef ROOT_TSystem
#include "TSystem.h"
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

#include "TAGview.hxx" 

//
ClassImp(TAGeventDisplay)

Bool_t TAGeventDisplay::fgIsGeoLoaded   = false;
Bool_t TAGeventDisplay::fgIsDisplayed   = false;
Bool_t TAGeventDisplay::fgGeoDone       = false;
Bool_t TAGeventDisplay::fgGUIFlag       = true;
Bool_t TAGeventDisplay::fgDisplayFlag   = true;
//__________________________________________________________
TAGeventDisplay::TAGeventDisplay(Int_t type) 
  : TEveEventManager(),
    fAGRoot(0x0),
    fTopVolume(0x0),
    fCurrentEventId(0),
    fFirstEventDone(false),
    fMaxEnergy(1024),
    fBoxDefWidth(200.),
    fBoxDefHeight(200.),
    fQuadDefWidth(200),
    fQuadDefHeight(200),
    fInfoView(0x0),
    fEventEntry(0x0),
    fDebugLevel(0),
    fListOfCanvases(new TList()),
    fListOfPads(new TList())
{ 
  // default constructor
   gErrorIgnoreLevel = kError;

   fListOfCanvases->SetOwner(false);
   fListOfPads->SetOwner(false);
   
   fAGRoot = new TAGroot();
   
   if ( gGeoManager == NULL ) { // a new Geo Manager is created if needed
	  
	  new TGeoManager("FIRST","Creating Geometry");
	  
	  TGeoMaterial *mat  = new TGeoMaterial("Vacuum",0,0,0); 
	  TGeoMedium   *med  = new TGeoMedium("Vacuum",1,mat);
	  
	  fTopVolume = gGeoManager->MakeBox("World",med,25000,25000,50000); 
	  gGeoManager->SetTopVolume(fTopVolume);
   }
}

//__________________________________________________________
TAGeventDisplay::~TAGeventDisplay()
{
  // default destructor
   delete fListOfCanvases;
   delete fListOfPads;
   
   if (fInfoView)
	  delete fInfoView;
   if (fEventEntry)
	  delete fEventEntry;
   if (fRefreshButton)
	  fRefreshButton->Delete();
   if (fNumberEvent)
	  fNumberEvent->Delete();
   if (frmMain)
	  frmMain->Delete();
   
   fAGRoot->EndEventLoop();
   delete fAGRoot;
   delete fTopVolume;
}

//__________________________________________________________
void TAGeventDisplay::LoadGeometry(const Char_t* fileName)
{  
  if (fgIsGeoLoaded) 
	  return;
  
  if ( !TGeoManager::Import(fileName) ) {
	cout << "Failed to load geometry" << endl; 
  } 
  fgIsGeoLoaded = true;
}

//__________________________________________________________
void TAGeventDisplay::AddGeometry(TGeoVolume* volume, TGeoMatrix* transf)
{   
   static Int_t nb = 0;
   TObjArray* list =  fTopVolume->GetNodes();
   if (list) {
	  for (Int_t i = 0; i < list->GetEntries(); ++i) {
		 TGeoNode* vol = (TGeoNode*)list->At(i);
		 TString volName(vol->GetName());
		 if (volName.Contains(volume->GetName())) {
			cout << Form("Geometry of %s already loaded", volume->GetName()) << endl;
			return;
		 }
	  }
   }
   
   fTopVolume->AddNode(volume, nb++, transf);
}

//__________________________________________________________
void TAGeventDisplay::SetTransparency(Char_t  transparency)
{
  // refresh must be done by user
  if (!gGeoManager) 
	  return;
  TObjArray* list =  gGeoManager->GetListOfVolumes();
  if (!list) return;
  for (Int_t i = 0; i < list->GetEntries(); ++i) {
	TGeoVolume* vol = (TGeoVolume*)list->At(i);
	vol->SetTransparency(transparency);
  }
}

//__________________________________________________________
void TAGeventDisplay::ShowDisplay()
{
   if (fgIsDisplayed) {
	  gEve->FullRedraw3D(kTRUE);
	  cout << "Display already open, just redraw canvas" << endl;
	  return;
   }
   
   CreateRawAction();
   CreateRecAction();
   AddRequiredItem();
   
   BuildDefaultGeometry();
   
   TEveManager::Create();
   gEve->AddEvent(this);
   
   if (gGeoManager) {
	  TGeoNode* topNode = gGeoManager->GetTopNode();
	  TGeoManager::SetVerboseLevel(0);
	  if (topNode == 0x0) {
		 cout << "No top node in geometry" << endl;;
		 return;
	  } else {
		 TEveGeoTopNode* out = new TEveGeoTopNode(gGeoManager, topNode);
		 gEve->AddGlobalElement(out);
	  }
   } else {
	  cout << "No geometry loaded" << endl;
   }

   if (fgGUIFlag)
	  MakeGUI();
   
   TGLViewer* v = gEve->GetDefaultGLViewer();
   
   v->SetClearColor(kWhite);
   
   gEve->FullRedraw3D(kTRUE);
   
   fgIsDisplayed = true;
   
   AddElements();
}

//__________________________________________________________
void TAGeventDisplay::MakeGUI()
{
   TEveBrowser* browser = gEve->GetBrowser();
   browser->StartEmbedding(TRootBrowser::kLeft);

   frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
   frmMain->SetWindowName("XX GUI");
   frmMain->SetCleanup(kDeepCleanup);
   
   // event frame
   TGHorizontalFrame *eventFrame = new TGHorizontalFrame(frmMain);
   
   TString icondir( Form("%s/icons/", gSystem->Getenv("ROOTSYS")) );
   TGPictureButton* b = 0x0;
   
   b = new TGPictureButton(eventFrame, gClient->GetPicture(icondir + "GoForward.gif"));
   eventFrame->AddFrame(b);
   b->SetToolTipText("Next Event");
   b->Connect("Clicked()", "TAGeventDisplay", this, "NextEvent()");
   
   b = new TGPictureButton(eventFrame, gClient->GetPicture(icondir + "ReloadPage.gif"));
   eventFrame->AddFrame(b);
   b->SetToolTipText("Loop Event");
   b->Connect("Clicked()", "TAGeventDisplay", this, "LoopEvent()");

   fNumberEvent  = new TGNumberEntry(eventFrame, 0, 4, -1, 
									 TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
									 TGNumberFormat::kNELLimitMinMax, 1, 1500);
   fNumberEvent->Resize(60,20);
   fNumberEvent->SetNumber(10);
   eventFrame->AddFrame(fNumberEvent, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 0, 5, 0));
   
   fRefreshButton = new TGCheckButton(eventFrame, "Refresh", 1);
   fRefreshButton->SetState(kButtonDown);
   eventFrame->AddFrame(fRefreshButton, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 0, 5, 0));
   
   frmMain->AddFrame(eventFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 0, 5, 5));
   
   // info frame
   TGVerticalFrame *infoFrameView = new TGVerticalFrame(frmMain);
   
   // event entry
   TGLabel*  eventName = new TGLabel(infoFrameView, "Event:");
   infoFrameView->AddFrame(eventName, new TGLayoutHints(kLHintsLeft | kLHintsTop, 2, 0, 5, 5));
   fEventEntry = new TGTextEntry(infoFrameView);
   infoFrameView->AddFrame(fEventEntry, new TGLayoutHints(kLHintsLeft | kLHintsCenterY  | 
														  kLHintsExpandX, 2, 10, 0, 10));
   // progress bar
   TGLabel*  barName = new TGLabel(infoFrameView, "Events to Read:");
   infoFrameView->AddFrame(barName, new TGLayoutHints(kLHintsLeft | kLHintsTop, 2, 0, 5, 5));
   fEventProgress = new TGHProgressBar(infoFrameView, TGProgressBar::kFancy);
   infoFrameView->AddFrame(fEventProgress, new TGLayoutHints(kLHintsLeft | kLHintsCenterY  | 
															 kLHintsExpandX, 2, 10, 0, 10));
   // selection
   TGLabel*  infoName = new TGLabel(infoFrameView, "Selection:");
   infoFrameView->AddFrame(infoName, new TGLayoutHints(kLHintsLeft | kLHintsTop, 2, 0, 5, 5));
   
   fInfoView = new TGTextView(infoFrameView, 300, 350);
   fInfoView->ShowBottom();
   infoFrameView->AddFrame(fInfoView, new TGLayoutHints(kLHintsLeft | kLHintsCenterY  | 
														kLHintsExpandX, 2, 10, 0, 0));
   
   TGTextButton* clearInfo = new TGTextButton(infoFrameView,"&Clear");
   clearInfo->Connect("Clicked()","TAGeventDisplay",this,"ClearInfoView()");
   clearInfo->SetToolTipText("Clear Info View");
   infoFrameView->AddFrame(clearInfo, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 15, 5, 3, 4));
   
   frmMain->AddFrame(infoFrameView, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 0, 5, 5));
   
   frmMain->MapSubwindows();
   frmMain->Resize();
   frmMain->MapWindow();
   browser->StopEmbedding();
   browser->SetTabTitle("Info", 0);
   
   ConnectElements();  
   CreateCanvases();
}

//__________________________________________________________
void TAGeventDisplay::ClearInfoView()
{
   // clear log message 
   fInfoView->Clear();
   fInfoView->ShowBottom();  
}

//______________________________________________________________________________
void TAGeventDisplay::UpdateEventBar()
{
   // display current event number with variable frequency
   Int_t frequency = 1; 
   Float_t max     = fEventProgress->GetMax();;
   
   if( max > 100000)      frequency = 10000;
   else if( max > 10000)  frequency = 1000;
   else if( max > 1000)   frequency = 100;
   else if( max > 10)     frequency = 10;
   
   if( fCurrentEventId % frequency == 0) {
	  fEventProgress->Increment(frequency); 
   }
}

//______________________________________________________________________________
Bool_t TAGeventDisplay::SetNextEvent()
{
   return fAGRoot->NextEvent();
}

//__________________________________________________________
void TAGeventDisplay::LoopEvent(Int_t nEvts)
{
	if (!fgGUIFlag) {
		printf("TAGeventDisplay: No GUI available\n");
		return;
	}
//	TStopwatch watch;
//	watch.Start();
	
	if (nEvts == 0) 
		nEvts = fNumberEvent->GetIntNumber();
	
	fEventProgress->SetRange(0, nEvts);
	
	if (fRefreshButton->IsOn())
		ResetHistogram();
	
	Int_t freq = fRefreshEvent->GetIntNumber();
	
	for (Int_t i = 0; i < nEvts; ++i) {
	    if (!SetNextEvent()) return;
		fCurrentEventId++;
		UpdateEventBar();
		
		if (!fRefreshButton->IsOn())
			UpdateElements();
		
		if (fCurrentEventId % freq == 0) {
			UpdateFreqCanvases();
		}
	}
	
	UpdateElements();
	
	fEventProgress->Reset();
	fEventProgress->SetPosition(0.);
	
	UpdateDefCanvases();
	
//	printf("Real time %4.1f\n", watch.RealTime());
	
	fFirstEventDone = true;     
}

//__________________________________________________________
void TAGeventDisplay::NextEvent()
{   
   LoopEvent(1);
}


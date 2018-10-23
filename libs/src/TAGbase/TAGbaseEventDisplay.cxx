//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "TMath.h"
#include "TGeoMedium.h"
#include "TGeoMaterial.h"

#include "TEveGeoNode.h"
#include "TEveManager.h"
#include "TEveBrowser.h"
#include "TEveWindow.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TGeoVolume.h"
#include "TGButton.h"
#include "TGLViewer.h"
#include "TGLabel.h"
#include "TList.h"
#include "TMath.h"
#include "TNamed.h"
#include "TPad.h"
#include "TPaveLabel.h"
#include "TSystem.h"
#include "TRootEmbeddedCanvas.h"
#include "TVector3.h"
#include "TVirtualPad.h"
#include "Riostream.h"

#include "TAGbaseEventDisplay.hxx"
#include "TAGgeoTrafo.hxx"
#include "TAGroot.hxx"
#include "TAGparTools.hxx"

Bool_t TAGbaseEventDisplay::fgIsGeoLoaded   = false;
Bool_t TAGbaseEventDisplay::fgIsDisplayed   = false;
Bool_t TAGbaseEventDisplay::fgGeoDone       = false;
Bool_t TAGbaseEventDisplay::fgGUIFlag       = true;
Bool_t TAGbaseEventDisplay::fgDisplayFlag   = true;

ClassImp(TAGbaseEventDisplay)

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TAGbaseEventDisplay::TAGbaseEventDisplay(const TString expName)
: TEveEventManager(),
  fExpName(expName),
  fWorldSizeZ(120),
  fWorldSizeXY(25),
  fWorldMedium(0x0),
  fAGRoot(0x0),
  fTopVolume(0x0),
  fGeoTrafo(0x0),
  fCurrentEventId(0),
  fFirstEventDone(false),
  fMaxEnergy(1024),
  fMaxMomentum(30),
  fBoxDefWidth(200.),
  fBoxDefHeight(200.),
  fQuadDefWidth(200.),
  fQuadDefHeight(200.),
  fInfoView(0x0),
  fEventEntry(0x0),
  fDebugLevel(0),
  fListOfCanvases(new TList()),
  fListOfPads(new TList())
{ 
  // default constructor

   fListOfCanvases->SetOwner(false);
   fListOfPads->SetOwner(false);
   
   // define TAGroot
   fAGRoot = new TAGroot();

   // check geometry manager
   if ( gGeoManager == NULL )  // a new Geo Manager is created if needed
      new TGeoManager(expName.Data(), "Creating Geometry");
   
   // define material
   DefineMaterial();

   // default parameter values
   TGeoMedium* med = gGeoManager->GetMedium("Vacuum");
	if (med) fWorldMedium = med;
   
   // Read Trafo file
   TAGgeoTrafo* geoTrafo = new TAGgeoTrafo();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TAGbaseEventDisplay::~TAGbaseEventDisplay()
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


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TAGbaseEventDisplay::BuildDefaultGeometry()
{
   // World
   TGeoMedium* med = gGeoManager->GetMedium("Vacuum");
   fTopVolume = gGeoManager->MakeBox("World",med,fWorldSizeZ, fWorldSizeXY, fWorldSizeXY);
   gGeoManager->SetTopVolume(fTopVolume);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TAGbaseEventDisplay::DefineMaterial()
{
   //hall: fill with vacuum
   TGeoMaterial *mat  = new TGeoMaterial("Vacuum",0,0,0);
   TGeoMedium   *med  = new TGeoMedium("Vacuum",1,mat);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TAGbaseEventDisplay::SetWordMedium(TString materialChoice)
{
   // search the material by its name
   TGeoMedium* med = gGeoManager->GetMedium(materialChoice.Data());
   if (med) fWorldMedium = med;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TAGbaseEventDisplay::SetWorldSizeZ(Float_t sizeZ)
{
   fWorldSizeZ  = sizeZ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TAGbaseEventDisplay::SetWorldSizeXY(Float_t sizeXY)
{
   fWorldSizeXY = sizeXY;
}

//__________________________________________________________
void TAGbaseEventDisplay::LoadGeometry(const Char_t* fileName)
{
   if (fgIsGeoLoaded)
      return;
   
   if ( !TGeoManager::Import(fileName) ) {
      cout << "Failed to load geometry" << endl;
   }
   fgIsGeoLoaded = true;
}

//__________________________________________________________
void TAGbaseEventDisplay::AddGeometry(TGeoVolume* volume, TGeoMatrix* transf)
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
void TAGbaseEventDisplay::SetTransparency(Char_t  transparency)
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
void TAGbaseEventDisplay::ShowDisplay(const TString fileName)
{
   if (fgIsDisplayed) {
      gEve->FullRedraw3D(kTRUE);
      cout << "Display already open, just redraw canvas" << endl;
      return;
   }
   
   CreateRawAction();
   CreateRecAction();
   AddRequiredItem();
   
   OpenFile(fileName);
   
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
void TAGbaseEventDisplay::MakeGUI()
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
   b->Connect("Clicked()", "TAGbaseEventDisplay", this, "NextEvent()");
   
   b = new TGPictureButton(eventFrame, gClient->GetPicture(icondir + "ReloadPage.gif"));
   eventFrame->AddFrame(b);
   b->SetToolTipText("Loop Event");
   b->Connect("Clicked()", "TAGbaseEventDisplay", this, "LoopEvent()");
   
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
   clearInfo->Connect("Clicked()","TAGbaseEventDisplay",this,"ClearInfoView()");
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
void TAGbaseEventDisplay::ClearInfoView()
{
   // clear log message
   fInfoView->Clear();
   fInfoView->ShowBottom();
}

//______________________________________________________________________________
void TAGbaseEventDisplay::UpdateEventBar()
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

//__________________________________________________________
void TAGbaseEventDisplay::LoopEvent(Int_t nEvts)
{
   if (!fgGUIFlag) {
      printf("TAGbaseEventDisplay: No GUI available\n");
      return;
   }
   
   if (nEvts == 0)
      nEvts = fNumberEvent->GetIntNumber();
   
   fEventProgress->SetRange(0, nEvts);
   
   if (fRefreshButton->IsOn())
      ResetHistogram();
   
   Int_t freq = fRefreshEvent->GetIntNumber();
   
   for (Int_t i = 0; i < nEvts; ++i) {
      if (!fAGRoot->NextEvent()) return;
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
   
   fFirstEventDone = true;     
}

//__________________________________________________________
void TAGbaseEventDisplay::NextEvent()
{   
   LoopEvent(1);
}



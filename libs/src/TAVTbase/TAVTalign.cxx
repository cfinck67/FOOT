/*!
 \file
 \version $Id: TAVTalign.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
 \brief   Implementation of TAVTalign.
 */
#include "TMath.h"
#include "TGeoMatrix.h"
#include "TLegend.h"
#include "TList.h"
#include "TPaveLabel.h"
#include "TStyle.h"

#include "TAGactTreeWriter.hxx"
#include "TAGactMbsReader.hxx"
#include "TAVTactAscReader.hxx"

#include "TAGmbsEvent.hxx"
#include "TAVTrawEvent.hxx"
#include "TAVTactAscRaw.hxx"
#include "TAGmbsEventFilterType.hxx"

#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"
#include "TAVTparMap.hxx"
#include "TAVTdatRaw.hxx"

#include "TAVTntuRaw.hxx"
#include "TAVTntuTrack.hxx"
#include "TAVTntuCluster.hxx"

#include "TAVTactDatRaw.hxx"
#include "TAVTactAscRaw.hxx"
#include "TAVTactNtuRaw.hxx"
#include "TAVTactNtuClusterF.hxx"
#include "TAVTactNtuTrack.hxx"
#include "TAVTalign.hxx"

/*!
 \class TAVTalign 
 \brief Alignment class for vertex tracks. **
 */

ClassImp(TAVTalign);

TAVTalign* TAVTalign::fgInstance = 0x0;

//__________________________________________________________
TAVTalign* TAVTalign::Instance(const TString name, Int_t type)
{
   if (fgInstance == 0x0) 
	  fgInstance = new TAVTalign(name, type);
   
   return fgInstance;
}

//------------------------------------------+-----------------------------------
//! Default constructor.
TAVTalign::TAVTalign(const TString name, Int_t type)
: TObject(),
  fAlignMonitor(0x0),
  fAlignTracks(100),
  fTinyBound(150),
  fIsDrawn(false),
  fIsFullAlignment(true),
  fDebugLevel(0)
{
   
   //------ defeinition of actions --------
   fAGRoot = new TAGroot();
   TString confFile("");
   TString geoFile("");
   if (type == 0)
	  fpEvent = new TAGdataDsc("vtEvent", new TAGmbsEvent());
   else if (type == 1 || type == 2) {
   	  fpEvent = new TAGdataDsc("vtEvent", new TAVTrawEvent());
	  confFile = "TAVTdetector_Cat.cfg";
	  geoFile  = "TAVTdetector_Cat0.map";
   } else {
	  printf("Undefined type for event\n");
	  return;
   }
   
   fpParMap    = new TAGparaDsc("vtMap", new TAVTparMap());
   
   fpGeoMap    = new TAGparaDsc("vtGeo", new TAVTparGeo());
   TAVTparGeo* geoMap   = (TAVTparGeo*) fpGeoMap->Object();
   geoMap->FromFile(geoFile.Data());
   
   fpConfig  = new TAGparaDsc("vtConf", new TAVTparConf());
   TAVTparConf* parconf = (TAVTparConf*) fpConfig->Object();
   parconf->FromFile(confFile.Data());
   
   fpDatRaw    = new TAGdataDsc("vtDat", new TAVTdatRaw());
   fpNtuRaw    = new TAGdataDsc("vtRaw", new TAVTntuRaw());
   fpNtuClus   = new TAGdataDsc("vtClus", new TAVTntuCluster());
   fpNtuTrack  = new TAGdataDsc("vtTrack", new TAVTntuTrack());
   
   if (type == 0) {
	  fActEvtReader = new TAGactMbsReader("vtActEvtReader");
	  fActEvtReader->SetupChannel(fpEvent,  new TAGmbsEventFilterType(42));
	  fActDatRaw  = new TAVTactDatRaw("vtActRaw", fpDatRaw, fpEvent, fpGeoMap, fpConfig);   
   } else if (type == 1) {
	  fActEvtReader = new TAVTactAscReader("vtActEvtReader", fpEvent, fpConfig);
	  fActDatRaw  = new TAVTactAscRaw("vtActRaw", fpDatRaw, fpEvent, fpGeoMap);   
   }
   fActEvtReader->Open(name.Data());
   
   fActNtuRaw  = new TAVTactNtuRaw("vtActNtu", fpNtuRaw, fpDatRaw, fpParMap, fpGeoMap);
   
   fActClus    = new TAVTactNtuClusterF("vtActClus", fpNtuRaw, fpNtuClus, fpConfig, fpGeoMap);
   fActTrack   = new TAVTactNtuTrack("vtActTrack", fpNtuClus, fpNtuTrack, fpConfig, fpGeoMap);
   
   fAGRoot->AddRequiredItem("vtActRaw");
   fAGRoot->AddRequiredItem("vtActNtu");
   
   fAGRoot->AddRequiredItem("vtActClus");
   fAGRoot->AddRequiredItem("vtActTrack");
   
   fAGRoot->BeginEventLoop();
   fAGRoot->Print();   
   
   
   //--- Section for alignment parameters ---------
   for (Int_t i = 0; i < parconf->GetSensorsN(); ++i) {
	  if (parconf->GetSensorPar(i).Status != -1) { // detector to be aligned
		 fSecArray.Set(fSecArray.GetSize()+1);
		 fSecArray.AddAt(i, fSecArray.GetSize()-1);
	  }
   }
   
   for(Int_t i=0; i< geoMap->GetSensorsN(); i++){ 
	  fOffsetU[i] = geoMap->GetSensorPar(i).Position[0];
	  fOffsetV[i] = geoMap->GetSensorPar(i).Position[1];
   }
   
   fStopAlign = new Bool_t[fSecArray.GetSize()]; 
   
   fPposU     = new Float_t[fSecArray.GetSize()];
   fdifU      = new Float_t[fSecArray.GetSize()];
   fPposUold  = new Float_t[fSecArray.GetSize()];
   
   fPosV     = new Float_t[fSecArray.GetSize()];
   fdifV     = new Float_t[fSecArray.GetSize()];
   fPosVold  = new Float_t[fSecArray.GetSize()];
   
   fTiltW     = new Float_t[fSecArray.GetSize()];
   fAlignmentU= new Float_t[fSecArray.GetSize()];
   fAlignmentV= new Float_t[fSecArray.GetSize()];
   fdiftiltW  = new Float_t[fSecArray.GetSize()];
   fTiltWold  = new Float_t[fSecArray.GetSize()];
   
   fAlignTracks = parconf->GetAnalysisPar().TracksForAlignmentFit;
   //fTinyBound = parconf->GetAnalysisPar().SearchHitDistance;
   for (Int_t i = 0; i < fSecArray.GetSize(); ++i) {
	  fAlign[i] = new TAVTntuAlign(fSecArray[i]);
	  fAlign[i]->SetTracks(fAlignTracks);
	  fAlign[i]->SetBounding(fTinyBound);
	  fStopAlign[i] = false;
	  fIteration[i] = 0;
   }
   
   CreateHistograms();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTalign::~TAVTalign()
{
   for(Int_t i=0; i<fSecArray.GetSize(); i++)
	  delete fAlign[i];
   
   delete[] fPposU;
   delete[] fdifU;
   delete[] fPposUold;
   delete[] fPosV;
   delete[] fdifV;
   delete[] fPosVold;
   delete[] fTiltW;
   delete[] fAlignmentU;
   delete[] fAlignmentV;
   delete[] fdiftiltW;
   delete[] fTiltWold;
}

//_____________________________________________________________________________
// 
void TAVTalign::SetBounding(Float_t bound)
{
   fTinyBound = bound;
   for (Int_t i = 0; i < fSecArray.GetSize(); ++i) {
	  fAlign[i]->SetBounding(fTinyBound);
   }
}

//_____________________________________________________________________________
// 
void TAVTalign::CreateHistograms()
{
   gStyle->SetOptStat(111);
   fLegend = new TLegend( .13,.45, .17,.95, "Planes:");
   
   Char_t hname[50];
   Char_t htitle[50];
   
   for( Int_t iPl = 0; iPl < fSecArray.GetSize(); iPl++) {
	  
	  sprintf( hname, "hu%d", iPl);
	  sprintf( htitle, "Position in U");
	  fhPosu[iPl] = new TH2F();
	  fhPosu[iPl]->SetName(hname);
	  fhPosu[iPl]->SetTitle(htitle);
	  fhPosu[iPl]->SetMarkerStyle(20);
	  fhPosu[iPl]->SetMarkerSize(.8);
	  fhPosu[iPl]->SetMarkerColor(iPl+1);
	  fhPosu[iPl]->GetXaxis()->SetTitle("# of event");
	  fhPosu[iPl]->GetYaxis()->SetTitle("Position of secondary planes");
	  
	  sprintf( hname, "hv%d", iPl);
	  sprintf( htitle, "Position in V");
	  fhPosv[iPl] = new TH2F();
	  fhPosv[iPl]->SetName(hname);
	  fhPosv[iPl]->SetTitle(htitle);
	  fhPosv[iPl]->SetMarkerStyle(21);
	  fhPosv[iPl]->SetMarkerSize(.8);
	  fhPosv[iPl]->SetMarkerColor(iPl+1);
	  fhPosv[iPl]->GetXaxis()->SetTitle("# of event");
	  fhPosv[iPl]->GetYaxis()->SetTitle("Position of secondary planes");
	  
	  sprintf( hname, "htilt%d", iPl);
	  sprintf( htitle, "Tilt in Z");
	  fhTiltz[iPl] = new TH2F();
	  fhTiltz[iPl]->SetName(hname);
	  fhTiltz[iPl]->SetTitle(htitle);
	  fhTiltz[iPl]->SetMarkerStyle(21);
	  fhTiltz[iPl]->SetMarkerSize(.8);
	  fhTiltz[iPl]->SetMarkerColor(iPl+1);
	  fhTiltz[iPl]->GetXaxis()->SetTitle("# of event");
	  fhTiltz[iPl]->GetYaxis()->SetTitle("Tilt of secondary planes");
	  
	  sprintf( htitle, "%d", fSecArray[iPl]);
	  fLegend->AddEntry(fhPosu[iPl], htitle ,"p");
   }
}

//______________________________________________________________________________
//
void TAVTalign::ShowCorrection()
{
   for(Int_t j = 0; j < fSecArray.GetSize(); j++) {
	  printf("\nSensor# %d\n", fSecArray[j]);
	  fAlign[j]->ShowCorrection();
   }
}

//_____________________________________________________________________________
// 
void TAVTalign::LoopEvent(Int_t nEvts)
{
   for( Int_t iPl = 0; iPl < fSecArray.GetSize(); iPl++) {
	  fhPosu[iPl]->SetBins(nEvts/2, 0, nEvts, 200,-100,100);
	  fhPosv[iPl]->SetBins(nEvts/2, 0, nEvts, 200,-100,100);
	  fhTiltz[iPl]->SetBins(nEvts/2, 0, nEvts, 100,-0.2,0.2);
   }
   
   for (Int_t i = 0; i < nEvts; ++i ) {
	  if(i % 100 == 0) 
		 cout<<" Loaded Event:: " << i << endl;
	  fAGRoot->NextEvent();
	  if(!Update()) break;
   }
   
   // Now display the filled histos
   TAVTparGeo* geoMap   = (TAVTparGeo*) fpGeoMap->Object();
   printf("------------------------------------------------------------\n");
   printf("--------------------- Alignment RESULT --------------------\n");
   printf("------------------------------------------------------------\n");
   
   for(Int_t i = 0; i < fSecArray.GetSize(); i++) {
	  Int_t iPlane = fSecArray[i];
	  printf("----------------------------------------------\n");
	  printf("Sensor: %d AlignmentU: %5.1f AlignmentV: %5.1f TiltW: %6.3f status %d\n", iPlane,
			 geoMap->GetSensorPar(iPlane).AlignmentU, 
			 geoMap->GetSensorPar(iPlane).AlignmentV,
			 geoMap->GetSensorPar(iPlane).TiltW*TMath::RadToDeg(),
			 fStopAlign[i]);
   }
   
   printf("\n----------------------------------------------\n");
   printf("Restart to produce residual plots with 400 evts\n");
   
   fActTrack->CreateHistogram();
   
   for (Int_t i = 0; i < 400; ++i ) {
	  if(i % 100 == 0) 
		 cout<<" Loaded Event:: " << i << endl;
	  fAGRoot->NextEvent();
   }
   
   TList* list = fActTrack->GetHistogrammList();
   
   TCanvas* canvas = new TCanvas("canvas", "Residual");
   canvas->Divide(2,1);
   gStyle->SetOptStat(1111);
   
   canvas->cd(1);
   TH1F* histoX = (TH1F*)list->FindObject("vtResTotX");
   histoX->Draw();
   canvas->cd(2);
   TH1F* histoY = (TH1F*)list->FindObject("vtResTotY");
   histoY->Draw();
   
   // Create new geomaps file with updated parameters
   UpdateGeoMaps();
}

//_____________________________________________________________________________
//  
Bool_t TAVTalign::Update(){
   
   // Performs an alignement of the telescope in the coordinate system of tracker.
   // Tracks built from working planes are used to accumulate
   // residus with clusters (if clusters are at a distance smaller than tBound) 
   // A given number of tracks is used for this and then the alignment 
   // parameters (translation and rotation coeff.) are fitted
   // Do this until the shift in translation parameters is below a given limit,
   // At each iteration the tBound distance to associate track and clusters is automatically decreased.
   //
   
   TAVTntuTrack* pNtuTrack = (TAVTntuTrack*) fpNtuTrack->Object();
   TAVTparGeo*   pGeoMap   = (TAVTparGeo*)   fpGeoMap->Object();
   
   Float_t limitShift = 5; // microns, if the shift in position falls under this limit, stop the alignment (microns)
   Float_t limitTilt  = 1*TMath::DegToRad(); // convert degrees in radians 
   
   if(fDebugLevel) 
	  printf("\nTAVTalign::AlignTracker ALIGNING tracker (%d planes)\n with %d tracks,    using hits at max distance %.1f of tracks\n    ,stopping when shift changes below %.3f and tilt change below %.3f\n\n", 
			 fSecArray.GetSize(), fAlignTracks, fTinyBound, limitShift, limitTilt);
   
   Bool_t stop  = false;
   Int_t iPlane = -1;	
   Int_t nTrack = pNtuTrack->GetTracksN();
   if (nTrack == 0) return true;
   
   Int_t counts = fAGRoot->CurrentEventNumber();
   for (Int_t iTrack = 0; iTrack < nTrack; ++iTrack) {    
	  
	  TAVTtrack* track = pNtuTrack->GetTrack(iTrack);
	  if (!IsForAlignment(track)) continue;
	  
	  // align with given track
	  for(Int_t i = 0; i < fSecArray.GetSize(); i++) { // loop on secondary ref planes
		 
		 if( !fStopAlign[i] ) Align(i, track); // stop alignment when it is already good, 
		 
		 if ( !fStopAlign[i] && fAlign[i]->Enough2D()) { // if enough event for new position, 
			
			iPlane       = fSecArray[i];
			fPposU[i]    = pGeoMap->GetSensorPar(iPlane).Position[0]-fOffsetU[iPlane];	   
			fPosV[i]     = pGeoMap->GetSensorPar(iPlane).Position[1]-fOffsetV[iPlane];	   
			fTiltW[i]    = pGeoMap->GetSensorPar(iPlane).TiltW;
			fdifU[i]     = TMath::Abs(fPposU[i]-fPposUold[i]);
			fdifV[i]     = TMath::Abs(fPosV[i]-fPosVold[i]);
			fdiftiltW[i] = TMath::Abs(fTiltW[i]-fTiltWold[i]);
			
			if(fDebugLevel >= 3) {
			   cout << " plane " << i << " posU=" << fPposU[i] << " posUold= " << fPposUold[i];
			   cout << ";  posV=" << fPosV[i] << " posVold= " << fPosVold[i];
			   cout << ";  tilt W " << fTiltW[i] << "  tilt W old=" << fTiltWold[i] << endl;
			}
			fPposUold[i] = fPposU[i];
			fPosVold[i]  = fPosV[i];
			fTiltWold[i] = fTiltW[i];

			fhPosu[i]->Fill(counts, fPposU[i]);
			fhPosv[i]->Fill(counts, fPosV[i]);
			fhTiltz[i]->Fill(counts, fTiltW[i]*TMath::RadToDeg());
			
			// Redraw all planes
			if (!fAlignMonitor) {
			   fAlignMonitor = new TCanvas("AlignMonitor","align monitor",0,0,800,600);
			   fAlignMonitor->Divide(1,3);
			}
			
			if (fIsDrawn) {
			   fAlignMonitor->cd(1);
			   fhPosu[i]->Draw("PSAME");
			   fAlignMonitor->cd(2);
			   fhPosv[i]->Draw("PSAME");
			   fAlignMonitor->cd(3);
			   fhTiltz[i]->Draw("PSAME");
			} else {
			   fAlignMonitor->cd(1);
			   fhPosu[i]->Draw("P");
			   fLegend->Draw();
			   fAlignMonitor->cd(2);
			   fhPosv[i]->Draw("P");
			   fAlignMonitor->cd(3);
			   fhTiltz[i]->Draw("P");
			   fIsDrawn = true;
			}
			
			fStopAlign[i] = ((fdifU[i]<limitShift) & (fdifV[i]<limitShift)) & (fdiftiltW[i]<limitTilt); 
			printf("Iteration %d for plane %d, difU=%.2f < %.2f ? %d, difV=%.2f < %.2f ? %d, difTilt=%.3f < %.3f ? %d, stop=%d\n", 
				   fIteration[i]++, fSecArray[i]+1, fdifU[i], limitShift, (fdifU[i]<limitShift), fdifV[i], limitShift, (fdifV[i]<limitShift), 
				   fdiftiltW[i], limitTilt, fdiftiltW[i]<limitTilt, fStopAlign[i]);
			fAlignMonitor->Update();
		 } // end if enough event for new position
		 
	  } // end loop on sec ref planes
	  
	  // compute STOP condition = shift for all planes below limit
	  stop = true;
	  for(Int_t j = 0; j < fSecArray.GetSize(); j++) {
		 stop &= fStopAlign[j];
	  }
	  
	  if( stop ) { 
		 printf("  \nAlignment can stop now\n\n");
		 return false;
	  }
	  
   } // end if a track is found
   return true;    
}

//______________________________________________________________________________
//
void TAVTalign::Align(Int_t idx, TAVTtrack* aTrack)
{
   // Compute the  alignment parameters
   //
   // Loop on the hits in the plane to find the one nearest to the track within a given bound.
   // Submit this hit to the DAlign object which will return alignement parameters
   // and update the bound to associate track and hit,
   // once enough hits have been accumulated (limit hard coded in DAlign).
   // Angle is compute in radians (since sin(a) := a because a << 1)
   
   TAVTparGeo*     pGeoMap   = (TAVTparGeo*)     fpGeoMap->Object();
   TAVTntuCluster* pNtuClus  = (TAVTntuCluster*) fpNtuClus->Object();
   
   Float_t         tDistanceU, tDistanceV=0., tDist2D=0.;
   Float_t         tMinDistanceU, tMinDistanceV, tMinDistance2D;  
   Int_t           iPlane    = fSecArray[idx];
   TVector3        tTrackPos = aTrack->Intersection(pGeoMap->GetSensorPar(iPlane).Position[2]);
   
   // take hit - track position in uvd Plane frame, the order/sign matters!!
   Int_t nclus = pNtuClus->GetClustersN(iPlane);
   if (nclus > 0) {
	  tMinDistanceU  = 1000000.; // init
	  tMinDistanceV  = 1000000.;
	  tMinDistance2D = 1000000.;
	  for (Int_t k = 0; k < nclus; ++k) { // loop on hits
		 TAVTcluster* cluster = pNtuClus->GetCluster(iPlane, k);
		 
		 tDistanceU = cluster->GetPositionG()[0] - tTrackPos[0]; // strips and pixels
		 tDistanceV = cluster->GetPositionG()[1] - tTrackPos[1];
		 tDist2D    = sqrt( tDistanceU*tDistanceU + tDistanceV*tDistanceV );
		 if( TMath::Abs(tDist2D) <= TMath::Abs(tMinDistance2D)) {
			tMinDistance2D = tDist2D;
			tMinDistanceU  = tDistanceU;
			tMinDistanceV  = tDistanceV;
		 }
		 
		 if( fDebugLevel >= 2 ) 
			printf("   TAVTalign::Align plane %d Trying hit %d and distance (%f,%f)=%f\n", iPlane, k, tDistanceU, tDistanceV, tDist2D);
		 
	  } // end loop on hits
	  
	  if( fDebugLevel >= 1 )
		 printf("   TAVTalign::Align plane %d Accumulating with distance (%f,%f)=%f\n",iPlane, tMinDistanceU, tMinDistanceV, tMinDistance2D);
	  
	  // Only store the distances if no fit recquired, JB 2009/05/13
	  fAlign[idx]->Accumulate2D(tTrackPos, tMinDistanceU, tMinDistanceV); 
	  
   } // enf if there is some hits
   
   if (fAlign[idx]->Enough2D())  {
	  printf(" ---------------------------------------------------------- \n");
	  printf(" Plane: %d with bound %.1f um\n", iPlane+1, fAlign[idx]->GetBounding());
	  printf(" ---------------------------------------------------------- \n");
	  // now do the alignement correction
	  fAlign[idx]->ShowCorrection();
	  UpdateTransfo(idx);
	  fAlign[idx]->Modified();
   } // end if enough events
}

//______________________________________________________________________________
//
void TAVTalign::UpdateTransfo(Int_t idx)
{
   TAVTparGeo* pGeoMap  = (TAVTparGeo*)fpGeoMap->Object();
   Int_t       iPlane   = fSecArray[idx];
   TVector3    position = pGeoMap->GetSensorPar(iPlane).Position;
   Float_t     tiltW    = pGeoMap->GetSensorPar(iPlane).TiltW;
   
   TVector3 newPos(position.X()-fAlign[idx]->GetOffsetU(), position.Y()-fAlign[idx]->GetOffsetV(), position[2]);
   Float_t newTiltW  = tiltW + fAlign[idx]->GetTiltW();
   fAlignmentU[idx] += fAlign[idx]->GetOffsetU();
   fAlignmentV[idx] += fAlign[idx]->GetOffsetV();
   
   TGeoHMatrix* transfo = pGeoMap->GetTransfo(iPlane);
   Double_t* mat = transfo->GetRotationMatrix();
   TGeoRotation rot;
   rot.SetMatrix(mat);
   TGeoRotation rotW, rotWinv;
   rotW.RotateZ(newTiltW*TMath::RadToDeg());
   rotWinv = rotW.Inverse();
   
   TGeoTranslation trans(newPos[0], newPos[1], newPos[2]);
   TGeoHMatrix newTransfo  = trans;
   newTransfo  *= rot*rotWinv;
   TGeoHMatrix* pNewTransfo = new TGeoHMatrix(newTransfo);
   pGeoMap->AddTransMatrix(pNewTransfo, iPlane);
   
   pGeoMap->GetSensorPar(iPlane).Position   = newPos;
   pGeoMap->GetSensorPar(iPlane).AlignmentU = fAlignmentU[idx];
   pGeoMap->GetSensorPar(iPlane).AlignmentV = fAlignmentV[idx];
   pGeoMap->GetSensorPar(iPlane).TiltW      = newTiltW;
}

//______________________________________________________________________________
//
void TAVTalign::UpdateGeoMaps()
{
   TAVTparGeo* pGeoMap  = (TAVTparGeo*)fpGeoMap->Object();
   fstream  configFileOld;
   fstream  configFileNew;
   Char_t configFileName[1000];
   sprintf(configFileName,"%s", pGeoMap->GetFileName().Data());
   configFileOld.open(configFileName, ios::in);
   sprintf(configFileName,"%s_new", pGeoMap->GetFileName().Data());
   configFileNew.open(configFileName, ios::out);
   Char_t tmp[255];
   
   printf("\nCreating new file %s with updated alignment parameters\n", configFileName);
   
   TString key;
   while (!configFileOld.eof()) {
	  configFileOld.getline(tmp, 255);
	  key = tmp;
	  configFileNew << tmp;
	  configFileNew << "\n";
	  if (key.Contains("Inputs:")) {
		 Int_t pos = key.First(':');
		 TString sIdx = key(pos+1, key.Length());
		 Int_t idx = (Int_t)sIdx.Atoi()-1;
		 for (Int_t i = 0; i < fSecArray.GetSize(); ++i) {
			if (idx == fSecArray[i]){
			   UpdateGeoMapsUVW(configFileOld, configFileNew, idx);
			   break;
			}
		 }
	  } 
   }
   configFileOld.close();
}

//______________________________________________________________________________  
//
void TAVTalign::UpdateGeoMapsUVW(fstream &fileIn, fstream &fileOut, Int_t idx)
{
   Char_t tmp[255];
   TString key;
   TAVTparGeo* pGeoMap  = (TAVTparGeo*)fpGeoMap->Object();
   
   Float_t alignU = pGeoMap->GetSensorPar(idx).AlignmentU;
   Float_t alignV = pGeoMap->GetSensorPar(idx).AlignmentV;
   Float_t tiltW  = pGeoMap->GetSensorPar(idx).TiltW;
   
   while (!fileIn.eof()) {
	  fileIn.getline(tmp, 255);
	  key = tmp;
	  if (key.Contains("AlignementU:")) {
		 key = "AlignementU:";
		 TString sIdx;
		 sIdx = Form("   %8.2f\n", alignU);
		 TString line = key+sIdx;
		 fileOut << line.Data();
		 fileIn.getline(tmp, 255);
		 key = "AlignementV:";
		 sIdx = Form("   %8.2f\n", alignV);
		 line = key + sIdx;
		 fileOut << line.Data();
		 fileIn.getline(tmp, 255);
		 key = "AlignementTilt:";
		 sIdx = Form("   %6.3f\n", tiltW*TMath::RadToDeg());
		 line = key + sIdx;
		 fileOut << line.Data();
		 break;
	  } else {
		 fileOut << tmp;
		 fileOut << "\n";
	  }
   } 
}

//______________________________________________________________________________  
//
Bool_t TAVTalign::IsForAlignment(TAVTtrack* track)
{
   Bool_t isOk = false;
   if (fIsFullAlignment) {
	  if (track->GetClustersN() < 5) 
		 isOk = false;
	  else 
		 isOk = true;
   } else {
	  isOk = true;
   }
   
   TAVTparConf* parconf = (TAVTparConf*) fpConfig->Object();
   Float_t maxSlope = parconf->GetAnalysisPar().MaxSlopeForAlignment;
   TAVTline line  = track->GetTrackLine();
   Float_t slopeX = line.GetSlopeZ()(0);
   Float_t slopeY = line.GetSlopeZ()(1);
   if (TMath::Abs(slopeX) > maxSlope || TMath::Abs(slopeY) > maxSlope) // put a cut on slope ?
	  isOk = false;
   
   return isOk;
}

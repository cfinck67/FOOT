/*!
  \file
  \version $Id: TAVTbaseParConf.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TAVTbaseParConf.
*/

#include <Riostream.h>

#include "TGeoBBox.h"
#include "TColor.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TList.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TSystem.h"

#include "TAVTparMap.hxx"
#include "TAVTbaseParConf.hxx"

//##############################################################################

/*!
  \class TAVTbaseParConf TAVTbaseParConf.hxx "TAVTbaseParConf.hxx"
  \brief Map and Geometry parameters for Tof wall. **
*/

ClassImp(TAVTbaseParConf);

TString TAVTbaseParConf::fgkDefaultConfName = "./config/TAVTdetector.cfg";
 Bool_t TAVTbaseParConf::fgIsMapHist       = false;

//______________________________________________________________________________
TAVTbaseParConf::TAVTbaseParConf()
: TAGparTools(),
  fSensorsN(0)
{
   // Standard constructor
}

//______________________________________________________________________________
TAVTbaseParConf::~TAVTbaseParConf()
{
   // Destructor
}

//______________________________________________________________________________
Bool_t TAVTbaseParConf::FromFile(const TString& name) 
{   
   // simple file reading, waiting for real config file
   TString nameExp;
   
   if (name.IsNull())
     nameExp = fgkDefaultConfName;
   else 
     nameExp = name;
   
   if (Open(nameExp)) return kTRUE;
   
   // read position algorithme
   ReadItem(fAnalysisParameter.TracksMaximum);  
   if(fDebugLevel)
     cout << "Maximum tracks per event: "<< fAnalysisParameter.TracksMaximum << endl;
   
   ReadItem(fAnalysisParameter.PlanesForTrackMinimum);  
   if(fDebugLevel)
     cout << "Minimum number of plane for tracking: "<< fAnalysisParameter.PlanesForTrackMinimum << endl;
   
   ReadItem(fAnalysisParameter.SearchHitDistance);  
   if(fDebugLevel)
     cout << "Maximum distance between cluster and corresponding track: "<< fAnalysisParameter.SearchHitDistance << endl;
   
   ReadItem(fAnalysisParameter.HitsInPlaneMaximum);  
   if(fDebugLevel)
     cout << "Maximum number of clusters per plane: "<< fAnalysisParameter.HitsInPlaneMaximum << endl;
   
   ReadItem(fAnalysisParameter.TrackChi2Limit);  
   if(fDebugLevel)
     cout << "Limit of Chi2 for tracks: "<< fAnalysisParameter.TrackChi2Limit << endl;
   
   ReadItem(fAnalysisParameter.TracksForAlignmentFit);  
   if(fDebugLevel)
     cout << "Number of tracks for alignment: "<< fAnalysisParameter.TracksForAlignmentFit << endl;
   
   ReadItem(fAnalysisParameter.MaxSlopeForAlignment);  
   if(fDebugLevel)
	  cout << "Maximum slope for alignment: "<< fAnalysisParameter.MaxSlopeForAlignment << endl;
   
   ReadItem(fAnalysisParameter.BmTrackChi2Limit);  
   if(fDebugLevel)
	  cout << "Limit of Chi2 for BM tracks: "<< fAnalysisParameter.BmTrackChi2Limit << endl;
   
   ReadItem(fSensorsN);  
   if(fDebugLevel)
     cout << endl << "Reading Parameters for "<< fSensorsN << " Sensors" << endl;
   
   for (Int_t p = 0; p < fSensorsN; p++) { // Loop on each plane
     
     // read sensor index
	  ReadItem(fSensorParameter[p].SensorIdx);
     if(fDebugLevel)
       cout << endl << " - Reading Parameters of Sensor " <<  fSensorParameter[p].SensorIdx << endl;
     
     // read sensor status
	  ReadItem(fSensorParameter[p].Status);
     if(fDebugLevel)
       cout << "Status: "<< fSensorParameter[p].Status << endl;
     
     // read min of pixel per cluster
	  ReadItem(fSensorParameter[p].MinNofPixelsInCluster);  
     if(fDebugLevel)
       cout << "Minimum number of pixels per cluster: "<< fSensorParameter[p].MinNofPixelsInCluster << endl;
     
     // read max of pixel per cluster
	  ReadItem(fSensorParameter[p].MaxNofPixelsInCluster);  
     if(fDebugLevel)
       cout << "Maximum number of pixels per cluster: "<< fSensorParameter[p].MaxNofPixelsInCluster << endl;
   }	  
   
   return kFALSE;
}

//------------------------------------------+-----------------------------------
//! Clear geometry info.
void TAVTbaseParConf::Clear(Option_t*)
{
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAVTbaseParConf::ToStream(ostream& os, Option_t*) const
{
//  os << "TAVTbaseParConf " << GetName() << endl;
//  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
//     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}


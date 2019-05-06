#ifndef _TAVTbaseParConf_HXX
#define _TAVTbaseParConf_HXX
/*!
  \file
  \version $Id: TAVTbaseParConf.hxx
  \brief   Declaration of TAVTbaseParConf.
*/
/*------------------------------------------+---------------------------------*/

#include <map>
#include "Riostream.h"

#include "TObject.h"
#include "TString.h"
#include "TVector3.h"

#include "TAGparTools.hxx"

using namespace std;

//##############################################################################

class TAVTbaseParConf : public TAGparTools {
      
protected:
   Int_t      fSensorsN;         // Number of sensors	 
   TString    fkDefaultConfName; // default detector configuration file


   struct SensorParameter_t : public  TObject {
	  Int_t     SensorIdx;              // sensor index
	  Int_t     Status;                 // Status flag: Primary Reference = 1., Secondary Reference = 2. DeviceUunderTest = 3. Out = -1.
	  Int_t     MinNofPixelsInCluster;  // minimum pixels in a cluster
	  Int_t     MaxNofPixelsInCluster;  // maximum pixels in a cluster
     map< pair<int, int>, int > DeadPixelMap;    // dead pixel map
   };
   
   struct AnalysisParameter_t : public  TObject {
	  Int_t      TracksMaximum;          // maximum number of tracks to be allowed
	  Int_t      PlanesForTrackMinimum;  // min # planes to build a track in an event
	  Float_t    SearchHitDistance;      // max distance hit-track to add hit to track
	  Float_t    TrackChi2Limit;         // chi2 limit on the track
	  Float_t    BmTrackChi2Limit;       // chi2 limit on the BM track
   };
   
   
private:
   static Bool_t  fgIsMapHist;   // enable  map histo

public:

   TAVTbaseParConf();
    virtual ~TAVTbaseParConf();

   
   Bool_t          FromFile(const TString& name = "");
    
   virtual void    Clear(Option_t* opt="");
   
   virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;
   
   Int_t           GetStatus(Int_t idx) const;
   Int_t           GetStatusMinor(Int_t idx) const ;


      
   SensorParameter_t fSensorParameter[32];
   SensorParameter_t& GetSensorPar(Int_t idx)  { return fSensorParameter[idx];    }
   
   AnalysisParameter_t fAnalysisParameter;
   AnalysisParameter_t& GetAnalysisPar()       { return fAnalysisParameter;       }
   
   Int_t         GetSensorsN()          const  { return fSensorsN;                }
   const Char_t* GetDefaultConfName()   const  { return fkDefaultConfName.Data(); }

public:
   
   static Bool_t IsMapHistOn()                   { return fgIsMapHist; }
   static void   SetHistoMap(Bool_t flag = true) { fgIsMapHist = flag; }
   

   ClassDef(TAVTbaseParConf,2)
};

#endif

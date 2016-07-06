#ifndef _TAGactGlbTracking_HXX
#define _TAGactGlbTracking_HXX
/*!
  \file
  \version $Id: TAGactGlbTracking.hxx,v 1.3 2003/06/15 18:27:04 mueller Exp $
  \brief   Declaration of TAGactGlbTracking.
*/
/*------------------------------------------+---------------------------------*/

#include "TVector3.h"

#include "TAGaction.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

#include "MagneticField.hxx"
#include "GlobalTrack.hxx"

// VM added 8/5/13
#include "ChargeBetheBlock.hxx"


class TH1F;
class TAGactGlbTracking : public TAGaction {
public:
  
  explicit        TAGactGlbTracking(const char* name=0,
				    TAGdataDsc* p_glb=0,
				    TAGdataDsc* p_vtvtx=0,
				    TAGdataDsc* p_traw=0,
				    TAGparaDsc* p_geotof=0,
				    double Current=0, TString dir="./",
				    TAGdataDsc* p_mceve=0);


    virtual         ~TAGactGlbTracking();

    virtual Bool_t  Action();

    //! Base creation of histogram
    virtual  void   CreateHistogram();
  
  void EnableClusterTof()  { fGloTrack->EnableClusterTof();  }
  void DisableClusterTof() { fGloTrack->DisableClusterTof(); }
  void EnableEnTofSingleChCalib()  { fGloTrack->EnableEnTofSingleChCalib();  } 
  void DisableEnTofSingleChCalib() { fGloTrack->DisableEnTofSingleChCalib(); }
  void EnableBmVtxMatch()  { fGloTrack->EnableBmVtxMatch(); }
  void DisableBmVtxMatch() { fGloTrack->DisableBmVtxMatch(); }
   
    ClassDef(TAGactGlbTracking,0)

  private:
    TAGdataDsc*     fpGlb;		    // global tracks
    TAGdataDsc*     fpVtvtx;		    // Vertex vertices
    TAGdataDsc*     fpMCeve;		    // MC class
    TAGdataDsc*     fpTraw;		    // Tof wall hits
    TAGparaDsc*     fpParTofGeo;		    // TOF-Wall geometry


    MagneticField *fField;
    GlobalTrack *fGloTrack;
   TH1F* fpHisMass;
};

#endif

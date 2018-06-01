#ifndef BmBooter_hxx
#define BmBooter_hxx


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

#include "Evento.h"
// #include "Segnale.h"
#include "Geometry.h"
// #include "Trigger.h"

#include "TAGpadGroup.hxx"
#include "TAGgeoTrafo.hxx"

// #include "TAGactTreeWriter.hxx"
// #include "TAGdataDsc.hxx"
#include "TCanvas.h"
#include "TH1F.h"
#include "TVector3.h"

#include "TAGroot.hxx"
#include "TAGview.hxx"
#include "GlobalPar.hxx"
#include "ControlPlotsRepository.hxx"
#include "TABMparGeo.hxx"
#include "TABMparCon.hxx"
#include "TABMdatRaw.hxx"
#include "TABMntuRaw.hxx"
#include "TABMntuTrack.hxx"
#include "TABMvieTrackFOOT.hxx"


class BmBooter {
  
public :

BmBooter();
~BmBooter() {};

public:
  void Initialize( EVENT_STRUCT* evStr );
  void Process( Long64_t jentry );
  void Finalize();

private:

  //~ TH1F* histo1;
  
  //~ TAGdataDsc* myn_bmraw;
  //~ TAGdataDsc* myn_bmtrk;
  TABMdatRaw* bmdatraw;
  TABMntuRaw* bmraw;
  TABMntuTrack* bmtrack;
  TABMparCon* bmcon;
  TABMparGeo* bmgeo;
  TABMntuTrackTr* p_tracktr;
  TABMntuHit* p_hit;
  //~ TABMvieTrackFOOT* pbmh_view;
  TAGpadGroup* pg;
  ControlPlotsRepository* m_controlPlotter;//prova
  string bm_outputdir;
  string plot_name;


};

#endif

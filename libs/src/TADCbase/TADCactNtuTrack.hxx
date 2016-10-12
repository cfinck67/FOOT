#ifndef _TADCactNtuTrack_HXX
#define _TADCactNtuTrack_HXX
/*!
  \file
  \version $Id: adapted from TABMactNtuTrack.hxx,v 1.3 2003/06/29 21:52:35 mueller Exp $
  \brief   Declaration of TADCactNtuTrack.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"
#include "TADCntuTrack.hxx"

class TADCactNtuTrack : public TAGaction {
public:
  explicit        TADCactNtuTrack(const char* name=0,
				  TAGdataDsc* p_ntutrk=0,
				  TAGdataDsc* p_ntuhit=0,
				  TAGparaDsc* p_dcgeo=0,
				  TAGparaDsc* p_dccon=0);
  virtual         ~TADCactNtuTrack();
  virtual Bool_t  Action();
  
  void sortDoubleHits(TADCntuRaw *hp, TADCntuTrack* pntr, TADCparGeo* ppg, TADCntuTrackTr & aRTr, TADCntuTrackTr & RefTr, int add[36], int ndou);
  void refitImprovedTrackSide(TADCntuRaw *hp, TADCntuTrack* pntr, TADCparGeo* ppg, int mulT, int mulS, double trchi2, TADCntuTrackTr & aRTr, int j);
  void refitImprovedTrackTop(TADCntuRaw *hp, TADCntuTrack* pntr, TADCparGeo* ppg, int mulT, int mulS, double trchi2, TADCntuTrackTr & aRTr, int j);
  
  ClassDef(TADCactNtuTrack,0)
    
  private:
  TAGdataDsc*     fpNtuTrk;		    // output data dsc
  TAGdataDsc*     fpNtuHit;		    // input data dsc
  TAGparaDsc*     fpDCGeo;		    // input data dsc
  TAGparaDsc*     fpDCCon;		    // input data dsc
};

#endif

#ifndef _TATRtrigRaw_HXX
#define _TATRtrigRaw_HXX
/*!
  \file
  \version $Id: TATRtrigRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TATRtrigRaw.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TObject.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"

class TATRtrigHit : public TObject {
public:
  
  TATRtrigHit();

  TATRtrigHit(int sta, int pat, int cnt);

  virtual        ~TATRtrigHit();

  void           SetData(int sta, int pat, int cnt);

  double         TrigTime(int ch) const;
  double         TrigMult(int ch) const;
  Int_t          Count() const;
  Int_t          Pattern() const;
  Int_t          Status() const;
  Int_t          VulomTime() const;
  Int_t          ScaCount(int ch) const;
  
  void           SetTrigBefLMU(int id, int val);
  int            GetTrigBefLMU(int id);
  
  void           SetTrigBefDT(int id, int val); //
  int            GetTrigBefDT(int id);    //
  void           SetTrigAftDT(int id, int val);
  int            GetTrigAftDT(int id);    
  void           SetTrigAftRed(int id, int val);
  int            GetTrigAftRed(int id);    

  void           SetTrigTime(int ch, double time);
  void           SetTrigMult(int ch, int mult);
  void           SetCount(int count);
  void           SetPattern(int patt);
  void           SetStatus(int stat);
  void           SetScaCount(int ch, int count);

  void           SetVulomTime(int tim);

  void           Clear(Option_t* option = "C");

  ClassDef(TATRtrigHit,1)

  private:

  Int_t vu_stat;    
  Int_t trg_pat;    
  Int_t trg_count;
  Int_t trg_vul_time;
  vector<double> trg_tim;
  vector<int>    trg_mul;
  Int_t trg_bf_lmu[16];
  Int_t trg_bf_dti[16];
  Int_t trg_af_dti[16];
  Int_t trg_af_red[16];
  Int_t sca_cnt[16];
   
};

//##############################################################################

class TATRtrigRaw : public TAGdata {
  public:

                    TATRtrigRaw();
    virtual         ~TATRtrigRaw();

    Int_t             GetHitsN() const;

    TATRtrigHit*       Hit(Int_t i_ind);
    const TATRtrigHit* Hit(Int_t i_ind) const;

   TATRtrigHit*        NewHit(int channel, double charge, double time);

    void              SetTrigTime(double time) { fTrigTime = time; }
    Double_t          TrigTime()         const { return fTrigTime; }

    virtual void      Clear(Option_t* opt="");

    void              SetupClones();

    ClassDef(TATRtrigRaw,1)

  private:
    Double32_t        fTrigTime;               //SC trigger tim
    TClonesArray*     fListOfHits;			    // hits
};

#include "TATRtrigRaw.icc"

#endif

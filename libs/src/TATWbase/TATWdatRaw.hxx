#ifndef _TATWdatRaw_HXX
#define _TATWdatRaw_HXX
/*!
  \file
  \version $Id: TATWdatRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TATWdatRaw.
*/
/*------------------------------------------+---------------------------------*/
#include <vector>
using namespace std;
#include "TObject.h"
#include "TClonesArray.h"
#include "TAGdata.hxx"
#include "TWaveformContainer.hxx"
//

/**
 * This class stores the params of a single channel waveform
 */
class TATWrawHit : public TObject {
  public:
    TATWrawHit();
    virtual         ~TATWrawHit();
    TATWrawHit( TWaveformContainer &w);
    TATWrawHit(Int_t cha ,Int_t board, Double_t charge, Double_t amplitude, Double_t pedestal, Double_t time);
    void  SetData(Int_t cha ,Int_t board, Double_t charge, Double_t amplitude, Double_t pedestal, Double_t time);
    //
    Double_t        Time() const;
    Double_t        Charge() const;
    Double_t 		Amplitude() const;
    Double_t 		Pedestal() const;
    Int_t           ChID() const;
    Int_t 			BoardId() const;
    Int_t           IDMC() const;
    //
    void            SetTime(double time); // waveform timestamp
    void            SetCharge(double charge); // waveform charge
    void 			SetAmplitdue(double amplitude); //waveform amplitude
    void 			SetPedestal(double pedestal); // waveform pedestal
    void            SetChID(int id);  //WD  channel ID [0,16]
    void 			SetBoardId(int id); // WD board ID
    void            SetMCID(int id);  //WD mc ID
    ClassDef(TATWrawHit,1);
    //
  private:
    Double_t ir_time;    
    Double_t ir_chg;
    Double_t ir_pedestal;
    Double_t ir_amplitude;
    Int_t ir_chid;
    Int_t ir_boardid;
    Int_t ir_mcid;
};

//##############################################################################

class TATWdatRaw : public TAGdata {
  public:
                    TATWdatRaw();
    virtual         ~TATWdatRaw();
    void          	  SetCounter(Int_t i_ntdc, Int_t i_nadc, Int_t i_ndrop);
    TATWrawHit*       Hit(Int_t i_ind);
    const TATWrawHit* Hit(Int_t i_ind) const;
  void       NewHit(TWaveformContainer &W);
  virtual void    Clear(Option_t* opt="");
    void SetupClones();
    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;
    ClassDef(TATWdatRaw,1);
  public:
    Int_t           nirhit;		    // 
    TClonesArray*   hir;			// hits
  private:
    static TString fgkBranchName;    // Branch name in TTree

    Int_t           fiNAdc;		    // 
    Int_t           fiNTdc;		    // 
    Int_t           fiNDrop;		//

};

#include "TATWdatRaw.icc"

#endif

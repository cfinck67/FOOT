#ifndef ROOT_Evento
#define ROOT_Evento

#include <iostream>
#include <fstream>
#include "TObject.h"
#include "TTree.h"

#include "EventStruct.h"

using namespace std;

class Evento : public TObject {

 public:

  Evento();
  virtual ~Evento();

  Int_t Clean();

  Int_t SetEvent(Int_t fnumero_evento);

  Int_t AddPart(Int_t fidpa, Int_t figen,
		Int_t ficha, Int_t fnumreg, Int_t fiba,
		Int_t fidead, Int_t fjpa,
		Double_t fvxi, Double_t fvyi, Double_t fvzi,
		Double_t fvxf, Double_t fvyf, Double_t fvzf,
		Double_t fpx, Double_t fpy, Double_t fpz,
		Double_t fpxf, Double_t fpyf, Double_t fpzf,
		Double_t famass, Double_t ftempo,
		Double_t ftof, Double_t ftrlen);

  Int_t AddSc(Int_t fidsc, 
	      Double_t fxinsc, Double_t fyinsc, Double_t fzinsc,
	      Double_t fxoutsc, Double_t fyoutsc, Double_t fzoutsc,
	      Double_t fpxinsc, Double_t fpyinsc, Double_t fpzinsc,
	      Double_t fpxoutsc, Double_t fpyoutsc, Double_t fpzoutsc,
	      Double_t fdesc, Double_t falsc, Double_t ftimsc);

  Int_t Add1DC(Int_t fid1dc, Int_t fipla1dc, Int_t fiview1dc,
                 Int_t ficell1dc,
		 Double_t fxin1dc, Double_t fyin1dc, Double_t fzin1dc,
		 Double_t fxout1dc, Double_t fyout1dc, Double_t fzout1dc,
		 Double_t fpxin1dc, Double_t fpyin1dc, Double_t fpzin1dc,
		 Double_t fpxout1dc, Double_t fpyout1dc,
		 Double_t fpzout1dc,
		 Double_t fde1dc, Double_t fal1dc, Double_t ftim1dc);

  Int_t AddVtx(Int_t fidvtx, Int_t fiplavtx, Int_t firowvtx, Int_t ficolvtx,
	       Double_t fxinvtx, Double_t fyinvtx, Double_t fzinvtx,
	       Double_t fxoutvtx, Double_t fyoutvtx, Double_t fzoutvtx,
	       Double_t fpxinvtx, Double_t fpyinvtx, Double_t fpzinvtx,
	       Double_t fpxoutvtx, Double_t fpyoutvtx, Double_t fpzoutvtx,
	       Double_t fdevtx, Double_t falvtx, Double_t ftimvtx);

  Int_t Add2DC(Int_t fid2dc, Int_t fipla2dc, Int_t fiview2dc,
                 Int_t ficell2dc,
		 Double_t fxin2dc, Double_t fyin2dc, Double_t fzin2dc,
		 Double_t fxout2dc, Double_t fyout2dc, Double_t fzout2dc,
		 Double_t fpxin2dc, Double_t fpyin2dc, Double_t fpzin2dc,
		 Double_t fpxout2dc, Double_t fpyout2dc,
		 Double_t fpzout2dc,
		 Double_t fde2dc, Double_t fal2dc, Double_t ftim2dc);
 
  Int_t AddScint(Int_t fidscint, Int_t firowscint,
		 Int_t ficolscint,
		 Double_t fxinscint, Double_t fyinscint, Double_t fzinscint,
		 Double_t fxoutscint, Double_t fyoutscint, Double_t fzoutscint,
		 Double_t fpxinscint, Double_t fpyinscint, Double_t fpzinscint,
		 Double_t fpxoutscint, Double_t fpyoutscint,
		 Double_t fpzoutscint,Double_t fdescint, Double_t falscint,
		 Double_t ftimscint );

  Int_t AddCry(Int_t fidcry, Int_t firowcry, Int_t ficolcry, 
	       Double_t fxincry, Double_t fyincry, Double_t fzincry, 
	       Double_t fxoutcry, Double_t fyoutcry, Double_t fzoutcry, 
	       Double_t fpxincry, Double_t fpyincry, Double_t fpzincry, 
	       Double_t fpxoutcry, Double_t fpyoutcry, Double_t fpzoutcry,
	       Double_t fdecry, Double_t falcry, Double_t ftimcry);
  
  Int_t AddCross(Int_t fidcross, Int_t fnregcross, Int_t fnregoldcross,
		 Double_t fxcross, Double_t fycross, Double_t fzcross,
		 Double_t fpxcross, Double_t fpycross, Double_t fpzcross,
		 Double_t fmcross, Double_t fchcross,Double_t ftcross);

  int FindBranches(TTree *RootTree,EVENTO_STRUCT *eve);

  Int_t Dump();

  EVENTO_STRUCT Output();

 private:

  EVENTO_STRUCT eve;

 protected:

  ClassDef(Evento,1);

};

#endif

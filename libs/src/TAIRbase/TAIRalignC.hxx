#ifndef _TAIRalignC_HXX
#define _TAIRalignC_HXX
/*!
 \file
 \brief   Declaration of TAIRalignC.
 
 \author C.A. Reidel & Ch. Finck
 */
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"
#include "TH1F.h"
#include "TObject.h"
#include "TString.h"
#include "TArrayI.h"
#include "TGraphErrors.h"

#include "TAGroot.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"
#include "TAGactTreeReader.hxx"
#include "TAIRparDiff.hxx"
#include "TAIRntuAlignC.hxx"

class TAIRalignC : public TObject {
   
private:
   static TAIRalignC* fgInstance; // static instance of class
   
public:
   //! Instance of class
   static TAIRalignC* Instance(const TString name = "provaout0020.lmd",
                               const TString confFile = "./config/TAVTdetector.cfg",
                               Int_t weight = -1); // The weight is barely influencing the results
   virtual ~TAIRalignC();
   
private:
   //! ctr
   TAIRalignC(const TString name, const TString confFile, Int_t weight);
public:
   void   LoopEvent(Int_t nEvts = 1);
   
private:
   
   void    CreateHistogram();
   Bool_t  AlignRough();
   Bool_t  AlignPrecise();
   Bool_t  FillHistograms();
   Bool_t  DefineWeights();
   void    UpdateAlignmentParams();
   void    UpdateTransfo(Int_t idx);
   void    UpdateGeoMaps();
   void    UpdateGeoMapsUVW(fstream &fileIn, fstream &fileOut, Int_t idx);
   void    Reset();
      
private:
   
   TAGroot*             fAGRoot;            // pointer to TAGroot
   TAGdataDsc*          fpNtuClus;	    // output data dsc
   TAGparaDsc*          fpConfig;	    // configuration dsc
   TAGparaDsc*          fpGeoMap;           // geometry para dsc
   TAGparaDsc*          fpGeoMapG;           // geometry para dsc
   TAIRparDiff*         fpDiff;		    // diffusion parameters
   TAGactTreeReader*    fInfile;            // action for reading cluster
   TAIRntuAlignC*       fAlign;           // pointer to align para
   
   const TString       fFileName;  // input file
   const TString       fconfFile;  // config file
   
   //MaterialParameter_t
   //map<TString, MaterialParameter_t> matParam;
   
   Int_t     fWeighted;   // activation of the weight
   Int_t     fPreciseIt;  // number of iteration for precise alignment
   Int_t     fCut1;
   Int_t     fCut2;
   Int_t     fCutFactor;
   
   TGraphErrors*        fResidualX;
   TGraphErrors*        fResidualY;
   TArrayI              fSecArray;   // contains the sensor number to be aligned
   
   Int_t    fHitPlanes;
   Int_t    fEvents1;
   Int_t    fEvents2;
   Int_t    fEvents3;
   
   Float_t fEbeamInit;
   Float_t fpcInit;
   Float_t fBetaInit;
   Float_t fAbeam;
   Int_t   fZbeam;
   Float_t fEbeam;
   Float_t fpc;
   Float_t fBeta;
   
   Double_t*  fWeight;
   Double_t*  fWeightQ;
   Double_t*  fZposition;
   Double_t*  fSigmaAlfaDist;
   Double_t*  fSigmaMeasQfinal;
   
   Double_t* fPosUClusters;
   Double_t* fPosVClusters;
   Double_t* fErrUClusters;
   Double_t* fErrVClusters;
   
   Float_t*   fTiltW;
   Float_t*   fAlignmentU;
   Float_t*   fAlignmentV;
   
   Bool_t*    fStatus;
   
   Double_t  fSumWeightQ;
   
   Bool_t fFixPlaneRef1;
   Bool_t fFixPlaneRef2;
   Int_t  fPlaneRef1;
   Int_t  fPlaneRef2;
   
   TH1F*   fpResXC[32];        // Residual in X
   TH1F*   fpResYC[32];        // Residual in Y
   TH1F*   fpResTotXC;        // Total residuals in x
   TH1F*   fpResTotYC;        // Total residuals in y
   
   ClassDef(TAIRalignC,0)
};

#endif

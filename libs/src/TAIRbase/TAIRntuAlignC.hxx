#ifndef _TAIRntuAlignC_HXX
#define _TAIRntuAlignC_HXX
/*!
 \file
 \brief   Declaration of TAIRntuAlignC.
 
 \author C.A. Reidel & Ch. Finck
 */
/*------------------------------------------+---------------------------------*/
#include "TAGdata.hxx"
#include "TAGparaDsc.hxx"

#include "Riostream.h"
#include "TString.h"
#include "TArrayI.h"
#include "TMatrixD.h"

class TAIRntuAlignC : public TAGdata {
   
public:
   TAIRntuAlignC(TArrayI* arraySensor, Int_t* pConf = 0);
   ~TAIRntuAlignC();
   
public:
   void   Reset();
   void   DefineConstant(Double_t* weightQ, Double_t* position);
   Bool_t Accumulate(Double_t* Ucluster, Double_t* Vcluster);
   void   Sum();
   void   Constraint(Int_t planeRef1, Int_t planeRef2);
   void   Minimize();
   
   Float_t*     GetOffsetU() const { return fAlignOffsetU; }
   Float_t*     GetOffsetV() const { return fAlignOffsetV; }
   Float_t*     GetTiltW()   const { return fAlignTiltW;   }
   
private:
   Int_t*      fpDevStatus;
   TArrayI*    fSensorArray;           // contains the sensor number to be aligned
   
   Float_t*    fAlignTiltW;
   Float_t*    fAlignOffsetU;
   Float_t*    fAlignOffsetV;
   
   Double_t    fSumZiWiQ;
   Double_t    fSumZiQWiQ;
   
   Int_t       fColumnY;
   Int_t       fColumnPhi;
   
   TMatrixD    fQ;

   
   TMatrixD    fCxInit;   // = Q . Px
   TMatrixD    fCyInit;   // = Q . Py
   TMatrixD    fCx;
   TMatrixD    fCy;
   TMatrixD    fCxtCx;
   TMatrixD    fCytCy;
   TMatrixD    fCxtCxT;
   TMatrixD    fCytCyT;
   
   TMatrixD    fDxInit;  // vector = Q . Xvector
   TMatrixD    fDyInit;  // vector = Q . Yvector
   TMatrixD    fDx;
   TMatrixD    fDy;
   
   TMatrixD    fTermeL;
   TMatrixD    fTermeCst;
   
   TMatrixD    fCm;
   TMatrixD    fDm;
   TMatrixD    fCmf;
   TMatrixD    fDmf;
   
   TMatrixD fAv;
   
   ClassDef(TAIRntuAlignC,0)
};

#endif

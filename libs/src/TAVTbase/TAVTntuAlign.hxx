// @(#)maf/dtools:$Name:  $:$Id: TAVTntuAlign.h,v.1 2005/10/02 18:03:46 sha Exp $
// Author: Dirk Meier   97/01/20 

#ifndef _TAVTntuAlign_HXX
#define _TAVTntuAlign_HXX

#include "TAGdata.hxx"

// ROOT classes
#include "Riostream.h"
#include "TVector3.h"


/** TAVTntuAlign class book keeping for alignment
 
    \author Dirk Meier
 */

class TAVTntuAlign : public TAGdata {
   
private:
   Int_t        fDebugAlign;                // Align  debug flag
   Int_t        fPlaneNumber;               // plane nb being aligned
   Bool_t       fEnough2D;
   Int_t        fNumberOfTracks;            // min # events to accumulate
   Double_t     fBound;                     // max distance track-hit
   Double_t     fAlignResolutionU;
   Double_t     fAlignMeanU;
   Double_t     fAlignResolutionV;
   Double_t     fAlignMeanV;
   Double_t     fAlignSum_du;    
   Double_t     fAlignSum_dudu;
   Double_t     fAlignSum_duvt;  
   Double_t     fAlignSum_vt;   
   Double_t     fAlignSum_vtvt; 
   Int_t        fAlignCountu;    
   Double_t     fAlignSum_dv;    
   Double_t     fAlignSum_dvdv;
   Double_t     fAlignSum_dvut;  
   Double_t     fAlignSum_ut;   
   Double_t     fAlignSum_utut; 
   Int_t        fAlignCountv;    
   Double_t     fAlignSum_uvt2;
   Double_t     fAlignSum_dvuduv;
   Int_t        fAlignCount2D;    
   Double_t     fAlignTiltW;    
   Double_t     fAlignOffsetU;    
   Double_t     fAlignOffsetV;    
   Double_t     fAlignTiltWError; 
   Double_t     fAlignOffsetUError; 
   Double_t     fAlignOffsetVError; 
   Double_t     fAlignCorrelation;
   TVector3     fAlignOffset;
   
public:
   TAVTntuAlign();                                 
   TAVTntuAlign(Int_t aPlaneNumber);                                 
   ~TAVTntuAlign();
   
   //! Set debug level
   void         SetDebug(Int_t aDebug) { fDebugAlign = aDebug; }
   //! Get debug level
   Int_t        GetDebug()    const    { return fDebugAlign; }
   
   //! Accumulated track for alignment
   void         Accumulate2D(TVector3& aTrackPos, Double_t aDistanceU, Double_t aDistanceV);
   
   //! Get U-frame offset
   Double_t     GetOffsetU()                const { return fAlignOffsetU; }
   //! Get V-frame offset
   Double_t     GetOffsetV()                const { return fAlignOffsetV; }
   //! Get Tilt in z
   Double_t     GetTiltW()                  const { return fAlignTiltW; }
   //! Get Resolution in U-frame
   Double_t     GetResolutionU()            const { return fAlignResolutionU; }
   //! Get mean in U-frame
   Double_t     GetMeanU()                  const { return fAlignMeanU; }
   //! Get resolution in V-frame
   Double_t     GetResolutionV()            const { return fAlignResolutionV; }
   //! Get mean in V-frame
   Double_t     GetMeanV()                  const { return fAlignMeanV; }
   //! Get Alignement offset
   TVector3&    GetOffset()                       { return fAlignOffset; }
   //! Get counts
   Int_t        GetCounts()                 const { return TMath::Max(fAlignCountu, fAlignCount2D); }
   //! Get boundings
   Double_t     GetBounding()               const { return fBound; }
   //! Get Enough
    Bool_t       Enough2D()                  const { return fEnough2D; }
     
   //! Show correction histograms
   void         ShowCorrection();
   //! Update alignement histogram
   void         Modified();
   //! Set boundings
   void         SetBounding(Double_t aBound);
   //! Set number of events for alignements
   void         SetTracks( Int_t nTracks)    { fNumberOfTracks = nTracks; } // JB, 2009/05/25
   //! Set U-frame offset
    void        SetOffsetU(Double_t u) {fAlignOffsetU = u; }
   //! Set V-frame offset
    void        SetOffsetV(Double_t v) {fAlignOffsetV = v; }
   //! Set Tilt in z
    void        SetTiltW(Double_t w)   {fAlignTiltW = w;   }
   
   
   ClassDef(TAVTntuAlign,1)           // book keeping for alignment 
};

#endif

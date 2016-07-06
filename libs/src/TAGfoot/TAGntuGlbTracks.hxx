#ifndef _TAGntuGlbTracks_HXX
#define _TAGntuGlbTracks_HXX
/*!
  \file
  \version $Id: TAGntuGlbTracks.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TAGntuGlbTracks.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
#include "TVector3.h"

using namespace std;


#include "TObject.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"
#include "TrackableParticle.hxx"
#include "TATntuRaw.hxx"
#include "TAVTntuVertex.hxx"


class TAGntuGlbTrk : public TObject {

  public:

    TAGntuGlbTrk();
    TAGntuGlbTrk(vector<TVector3> apath);
    virtual         ~TAGntuGlbTrk();

    void                       SetPath(vector<TVector3> apath) { fPath = apath;}
    vector<TVector3>           Path()        const { return fPath; }

    void                       SetMass(Double_t amass)    { fMass = amass; }
    Double_t                   GetMass()     const { return fMass; }

    void                       SetMomentum(Double_t amom) { fMom = amom; }
    Double_t                   GetMomentum() const { return fMom;}

    void                       SetCharge(Double_t acharge) { fCharge = acharge; }
    Double_t                   GetCharge()   const { return fCharge; }

    void                       SetToff(Double_t atoff)     { fToff = atoff; }
    Double_t                   GetToff()     const { return fToff; }

    void                       SetTofr(Double_t atofr)     { fTofr = atofr; }
    Double_t                   GetTofr()     const { return fTofr; }
   
    void                       SetId(Int_t id)             { fId = id; }
    Int_t                      GetId()       const { return fId; }

    void                       SetTgtDirection(TVector3 dir) { fTgtDir = dir; }
    TVector3                   GetTgtDirection() { return fTgtDir; }

    void                       SetTgtPosition(TVector3 pos) { fTgtPos = pos; }
    TVector3                   GetTgtPosition() { return fTgtPos; }

    void                       SetTofPosition(TVector3 pos) { fTofPos = pos; }
    TVector3                   GetTofPosition() { return fTofPos; }

    void                       SetTofDirection(TVector3 dir) { fTofDir = dir; }
    TVector3                   GetTofDirection() { return fTofDir; }

    void                       SetMatPosition(TVector3 pos) { fMatPos = pos; }
    TVector3                   GetMatPosition() { return fMatPos; }

    void                       SetMatchedVertexId(Int_t vtxid) { fVtxTrkID = vtxid; }
    Int_t                      GetMatchedVertexId() { return fVtxTrkID; }

    // VM added 29/9/2013
    void                       SetMatchedId(Int_t tof_id) { fMatchedID = tof_id; }
    Int_t                      GetMatchedId() { return fMatchedID; }

    void                       SetTrackId(Int_t trid) { fTrkID = trid; }
    Int_t                      GetTrackId() { return fTrkID; }

    // VM - test charge 8/5/2013
    void                       SetCharge_test(Int_t chg) { fCharge_test = chg; }
    Int_t                      GetCharge_test() { return fCharge_test; }
   
    // ChF - add energy 30/9/2013
    void                       SetEnergy(Double_t e) { fEnergy = e; }
    Double_t                   GetEnergy() { return fEnergy; }
   
   
    virtual void    Clear(Option_t* opt="");

    void SetupClones();

    void NewVertex(TAVTvertex& vertex);

    TAVTvertex*               GetVertex();
    Int_t                     GetNVertexes() { return vtxs ? vtxs->GetEntries() : 0; }
  
    void AttachHit(TATntuRawHit& hit);

    TATntuRawHit*       GetHit(Int_t iHit);

    Int_t GetNAttachedHits() {
      return atthit->GetEntries();
    }
  

  ClassDef(TAGntuGlbTrk,1)

  private:

    vector<TVector3> fPath;
    Double_t fMass;
    Double_t fMom;
    Double_t fCharge;
    Double_t fToff;
    Double_t fTofr;
    Double_t fEnergy;
    Int_t    fId;
    Int_t    fTrkID;

    Int_t fCharge_test;

  //Particle directions and positions computed on ToF Wall
    TVector3 fTgtDir;
    TVector3 fTgtPos;

  //Particle directions and positions computed on ToF Wall
    TVector3 fTofPos;
    TVector3 fTofDir;

    Int_t fMatchedID; // TOF ID

  //Matched ToF wall Hit position
    TVector3 fMatPos;

  //    vector<TATntuRawHit*> fAttachedHits;

    Int_t    fVtxTrkID;   //Index of the track in the Vertex vertex block

    TClonesArray*   vtxs;                   // Attached Vertexes
    TClonesArray*   atthit;		    // Attached Hits

};

//##############################################################################

class TAGntuGlbTracks : public TAGdata {
  public:

                    TAGntuGlbTracks();
    virtual         ~TAGntuGlbTracks();

    TAGntuGlbTrk*       Track(Int_t i_ind);
    const TAGntuGlbTrk* Track(Int_t i_ind) const;

    virtual void    Clear(Option_t* opt="");

    void SetupClones();

    ClassDef(TAGntuGlbTracks,1)

  public:
    Int_t           nglbtrk;		    // 
    TClonesArray*   glbtrk;		    // Global Tracks

};

#include "TAGntuGlbTracks.icc"

#endif

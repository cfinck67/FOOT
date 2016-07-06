#ifndef _TAVTntuVertex_HXX
#define _TAVTntuVertex_HXX

// ROOT classes
#include "TObject.h"
#include "TClonesArray.h"
#include "TVector3.h"

#include "TAGdata.hxx"
#include "TAVTntuTrack.hxx"

//##############################################################################

/** TAVTvertex class, simple container class for vertices with the associated tracks                    
 
 \author R. Rescigno
 */

class TAVTtrack;
class TClonesArray;
class TAVTvertex : public TObject {
    
private:
    TClonesArray*   fListOfTracks;      // list of track associated to the vertex 
    TVector3        fVertexPosition;    // vertex position
    Int_t           fIsValid;           // is zero if not valid 1 if it has almost two tracks
    Bool_t          fIsBmMatched;       // Matched flag with BM
    Double_t        fDistanceMin;       // distance value between the vertex point and the track. Sum of distance of all tracks
    Int_t           fDebugLevel;       // debug level

public:
    TAVTvertex();                                 
    ~TAVTvertex();
    TAVTvertex(const TAVTvertex& aVertex);
    
    //! Get list of tracks
    TClonesArray*  GetListOfTracks()        const { return   fListOfTracks;                      }
    //! Get number of tracks
    Int_t          GetTracksN()             const { return   fListOfTracks->GetEntries();        }
   //! Get Tracks
    TAVTtrack*     GetTrack(Int_t index)          { return (TAVTtrack*)fListOfTracks->At(index); }
    //! Add track
    void           AddTrack(TAVTtrack* track);
   
    //!Get Vertex Position
    TVector3&      GetVertexPosition()            { return fVertexPosition; }
    //!Set Vertex Position
    void SetVertexPosition(TVector3& xyz)         { fVertexPosition = xyz;}
    
    //! Set flag isValid
    void  SetVertexValidity(Int_t q)              { fIsValid = q;        }
    //Get flag Validity
    Int_t GetVertexValidity()               const { return fIsValid;     }
    
    //! Set Distance Min
    void SetDistanceMin(Double_t q)               { fDistanceMin = q;    }
    //! Getdistance Min
    Double_t GetDistanceMin()              const  { return fDistanceMin; }
    
    //! Set BM matched flag
    void SetBmMatched(Bool_t flag = true)         { fIsBmMatched = flag; }
    //! Get BM matched flag
    Bool_t IsBmMatched()                   const  { return fIsBmMatched; }
   
    //! Reset
    void Reset();
    
    ClassDef(TAVTvertex,1)                      // Describes TAVTvertex
};


//##############################################################################

class TAVTntuVertex : public TAGdata {

private:
   TClonesArray*       fListOfVertex;		// list of vertice, return an object TAVTvertex

private:
   static TString fgkBranchName;    // Branch name in TTree
   
public:
    TAVTntuVertex();
    virtual           ~TAVTntuVertex();
    
    TAVTvertex*        GetVertex(Int_t i);
    const TAVTvertex*  GetVertex(Int_t i) const;
    Int_t              GetVertexN()  const;
   
    TAVTvertex*        NewVertex();
    TAVTvertex*        NewVertex(TAVTvertex& vertex);
    
    virtual void       SetupClones();
    virtual void       Clear(Option_t* opt="");
        
public:
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }

    ClassDef(TAVTntuVertex,1)
};

#endif


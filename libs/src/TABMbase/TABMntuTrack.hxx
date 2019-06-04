#ifndef _TABMntuTrack_HXX
#define _TABMntuTrack_HXX
/*!
  \file
  \version $Id: TABMntuTrack.hxx,v 1.9 2003/07/07 18:42:17 mueller Exp $
  \brief   Declaration of TABMntuTrack.
*/

/*------------------------------------------+---------------------------------*/

#include "TAGroot.hxx"
#include "TAGdata.hxx"
#include "TABMntuTrackTr.hxx"

#include "TObject.h"
#include "TClonesArray.h"

//##############################################################################

class TABMntuTrack : public TAGdata {
  public:
                    TABMntuTrack();
    virtual         ~TABMntuTrack();

    Int_t                 GetTracksN()  const;
    TABMntuTrackTr*       Track(Int_t i);
    const TABMntuTrackTr* Track(Int_t i) const;
    
    //~ TABMntuTrackTr*       NewTrack(TABMntuTrackTr& track);//prova
    virtual void    SetupClones();
    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

   TClonesArray*    GetListOfTracks() { return fListOfTracks; }
   
   Int_t&           GetTrackStatus();
   
   static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }

  private:
    Int_t           fStatus; //-1000=notset, 0=ok, 1=firedUplane<plane_mincut, 2=firedVplane<plane_mincut, 3=hit rejected > rejmax_cut, 4=fit is not converged, 5=chi2red>chi2redcut
    
    TClonesArray*   fListOfTracks;			    // list of tracks
       
   static TString fgkBranchName;    // Branch name in TTree

   ClassDef(TABMntuTrack,2)

};

#include "TABMntuTrack.icc"

#endif

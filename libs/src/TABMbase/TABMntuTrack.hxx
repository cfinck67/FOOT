#ifndef _TABMntuTrack_HXX
#define _TABMntuTrack_HXX
/*!
  \file
  \version $Id: TABMntuTrack.hxx,v 1.9 2003/07/07 18:42:17 mueller Exp $
  \brief   Declaration of TABMntuTrack.
*/
#define RAD2DEG 57.2957795130823208768
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
   
    ClassDef(TABMntuTrack,2)

  private:
    Int_t           fStatus; //-1000=notset, 0=ok, 1=firedUplane<plane_mincut, 2=firedVplane<plane_mincut, 3=hit rejected > rejmax_cut, 4=Genfit fit is not converged, 5=chi2red>chi2redcut
    TClonesArray*   fListOfTracks;			    // list of tracks
};

#include "TABMntuTrack.icc"

#endif

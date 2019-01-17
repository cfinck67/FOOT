/*!
  \file
  \version $Id: TABMntuTrack.cxx,v 1.9 2003/07/07 18:42:33 mueller Exp $
  \brief   Implementation of TABMntuTrack.
*/

#include "TABMntuTrack.hxx"

using namespace genfit;


//########################################   TABMntuTrack   ######################################

/*!
  \class TABMntuTrack TABMntuTrack.hxx "TABMntuTrack.hxx"
  \brief PC track description - container (Qad level). **
*/

ClassImp(TABMntuTrack);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMntuTrack::TABMntuTrack() :
   //~ TAGdata(),//da capire se serve...
     ntrk(0),
    t(0),
    trk_status(-1000)
{}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMntuTrack::~TABMntuTrack()
{
  if(t)
    t->Delete();
    //~ delete t;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TABMntuTrack::SetupClones()
{
  if (!t) 
    t = new TClonesArray("TABMntuTrackTr");
  return;
}

//------------------------------------------+-----------------------------------
//! Clear.

void TABMntuTrack::Clear(Option_t*)
{ 
  TAGdata::Clear();
  ntrk   = 0;
  trk_status=-1000;
  if(t) 
    t->Delete();//presente prima
    //~ t->Clear();    
  return;
}

//*************************************************** OLD TRACKING ********************************************************

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

//da modificare
void TABMntuTrack::ToStream(ostream& os, Option_t* option) const
{
  //~ os << "TABMntuTrack" << Form("  ntrk=%3d", ntrk) << endl;

  //~ if (ntrk == 0) return;
  //~ os << "ind nh "
     //~ << "    x0     ux   y0     uy"<< endl;  
                  
  //~ for (Int_t i = 0; i < ntrk; i++) {
    //~ const TABMntuTrackTr* p_trk = Track(i);
    //~ os << Form("%3d", i)
       //~ << Form(" %2d", p_trk->nwire);
    //~ os << Form(" %5.0f %6.3f", p_trk->x0, p_trk->ux)
       //~ << Form(" %4.0f %6.3f", p_trk->y0, p_trk->uy);
    
    //~ os << endl;
  //~ }

  return;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

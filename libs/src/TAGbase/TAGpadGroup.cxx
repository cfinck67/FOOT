/*!
  \file
  \version $Id: TAGpadGroup.cxx,v 1.3 2003/04/27 21:02:04 mueller Exp $
  \brief   Implementation of TAGpadGroup.
*/

#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"

#include "TAGpadGroup.hxx"

/*!
  \class TAGpadGroup TAGpadGroup.hxx "TAGpadGroup.hxx"
  \brief List of pads. **
*/

ClassImp(TAGpadGroup);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGpadGroup::TAGpadGroup()
{
  fpListPad    = new TList();
  fpListCanvas = new TList();
  fpListPad->SetBit(kMustCleanup);
  fpListCanvas->SetBit(kMustCleanup);
  gROOT->GetListOfCleanups()->Add(fpListPad);
  gROOT->GetListOfCleanups()->Add(fpListCanvas);
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGpadGroup::~TAGpadGroup()
{
  delete fpListPad;
  delete fpListCanvas;
}

//------------------------------------------+-----------------------------------
//! Add a pad to the group.

void TAGpadGroup::AddPad(TVirtualPad* p_pad)
{
  fpListPad->Add(p_pad);
  TCanvas* p_canvas = p_pad->GetCanvas();
  if (p_canvas) {
    if (!fpListCanvas->FindObject(p_canvas)) fpListCanvas->Add(p_canvas);
  }
  return;
}

//------------------------------------------+-----------------------------------
//! Add all sub-pads of a pad to the group.

void TAGpadGroup::AddSubPads(TVirtualPad* p_pad)
{
  TObject* p_obj;
  TIter next(p_pad->GetListOfPrimitives());
  while ((p_obj = next())) {
    if (p_obj->InheritsFrom(TVirtualPad::Class())) AddPad((TVirtualPad*)p_obj);
  }
  return;
}

//------------------------------------------+-----------------------------------
//! Returns true if no pad has been added.

Bool_t TAGpadGroup::IsEmpty() const
{
  return fpListPad->IsEmpty();
}

//------------------------------------------+-----------------------------------
//! Mark all pad of the group as modified.

void TAGpadGroup::Modified() const
{
  TIter iter(fpListPad);
  TVirtualPad* p_pad;
  while ((p_pad = (TVirtualPad*) iter())) p_pad->Modified();
  return;
}

//------------------------------------------+-----------------------------------
//! Mark all canvas associated with group as to be updated.

void TAGpadGroup::Update() const
{
  TIter iter(fpListCanvas);
  TCanvas* p_canvas;
  while ((p_canvas = (TCanvas*) iter())) p_canvas->Update();
  return;
}

void TAGpadGroup::Print(char* fl, const char* suff) const
{
  TIter iter(fpListCanvas);
  TCanvas* p_canvas;
  int ip = 0; char name[200];
  while ((p_canvas = (TCanvas*) iter())) {
    ip++;
    sprintf(name,"%s_%d_pl%s",fl,ip,suff);
    p_canvas->Print(name);
  }
  return;
}

void TAGpadGroup::Clear(Option_t * /*option*/)
{
  TIter iter(fpListCanvas);
  TCanvas* p_canvas;
  while ((p_canvas = (TCanvas*) iter())) {
    p_canvas->Clear();
  }
  return;
}

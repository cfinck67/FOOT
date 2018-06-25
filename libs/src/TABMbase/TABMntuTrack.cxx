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
    trk_status(-1)
{}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMntuTrack::~TABMntuTrack()
{
  if(t)
    //~ t->Delete();
  delete t;
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

void TABMntuTrack::Clear(Option_t*)//capire cos'è l'opzione t...
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


//~ void TABMntuTrack::Chi2Fit(TABMntuRaw *hitp, TABMntuTrackTr* p_trk, int n_ite)
//~ {

  //~ int mynwire(0);

  //~ Int_t nmeas, ntass;
  //~ Double_t chi2;
  //~ TMatrixD AA, VV;
  //~ TVectorD alpha, Dy, Eta;

  //~ alpha.ResizeTo(4);
  //~ Eta.ResizeTo(4);
  
  //~ nmeas = hitp->nhit;
  //~ ntass = p_trk->nass;

  //~ Dy.ResizeTo(ntass);
  //~ alpha(0) = p_trk->GetX0();
  //~ alpha(1) = p_trk->GetY0();
  //~ alpha(2) = p_trk->GetUx();
  //~ alpha(3) = p_trk->GetUy();
  //~ AA.ResizeTo(ntass,4);
  //~ VV.ResizeTo(ntass,ntass);

  //~ Dy = ComputeDy(hitp,p_trk);
  //~ VV = ComputeVV(hitp,p_trk);
  //~ chi2 = ComputeChiQua(Dy,VV);
  //~ p_trk->SetChi2(chi2/(double)ntass);
  //~ //  cout<<" Before iteration "<<endl;

  //~ //  Dy.Print(); AA.Print(); alpha.Print(); VV.Print(); Eta.Print();

  //~ for (int hh=0; hh<n_ite;hh++){
    //~ Info("Action()"," Iteration number %d\n",hh);
    //~ Dy = ComputeDy(hitp, p_trk);
    //~ AA = ComputeAA(hitp, p_trk, alpha);
    //~ Eta = Mini(ntass, AA, VV, Dy);
    //~ Update(Dy, alpha, Eta, VV, p_trk, hitp);
    //~ Info("Action()","A:: %lf, %lf, %lf %lf\n",alpha(0),alpha(1),
	 //~ alpha(2),alpha(3));

  //~ }
  
  //~ return;
//~ }

//~ /*-----------------------------------------------------------------*/

//~ TVectorD TABMntuTrack::ComputeDy(TABMntuRaw *hitp, TABMntuTrackTr* p_trk)
//~ {
  //~ TVectorD Dy; 
  //~ int nmeas = hitp->nhit;
  //~ int nass = p_trk->nass;
  //~ int u_fil(0);
  //~ Dy.ResizeTo(nass);
  //~ for (int ifilo=0;ifilo<nmeas;ifilo++){
    //~ TABMntuHit *wr = hitp->Hit(ifilo);
    //~ if(!wr->TrkAss()) continue;
    //~ Dy(u_fil) =  wr->Dist() - wr->GetRho(); 
    //~ if(wr->Dist()<0 || wr->GetRho()<0) {
      //~ Warning("Action()","TABMntuTrack::Negative Distances!!! %lf %lf",wr->Dist(),wr->GetRho());
    //~ }
    //~ u_fil++;
  //~ }
  //~ /*
  //~ cout<<"Printing Dy vector="<<endl;
  //~ Dy.Print();
  //~ */
  //~ return Dy ;
//~ }

//~ TMatrixD TABMntuTrack::ComputeAA(TABMntuRaw *hitp, TABMntuTrackTr *p_trk, TVectorD alpha)
//~ /*  numerical computation of the Df/Dalpha derivatives */
//~ {

  //~ double eps = 0.0001;
  //~ TMatrixD AA;
    
  //~ int nmeas = hitp->nhit;
  //~ int ntass = p_trk->nass;
  //~ AA.ResizeTo(ntass,4);
  
  //~ TVectorD distanze_moved(ntass);
  //~ TVectorD parametri_moved(4);
  //~ TVectorD distanze0(ntass);
  //~ distanze0 = p_trk->GetTrackRho();

  //~ for(int ii = 0; ii<4;ii++){
    //~ parametri_moved = alpha;
    //~ if(parametri_moved(ii)!=0.){
      //~ parametri_moved(ii) *= (1.+eps);
    //~ }
      //~ else{
	//~ parametri_moved(ii) = eps;
      //~ }
    //~ p_trk->Set(parametri_moved);
    
    //~ //    cout<<"print out of alpha with incremented parameter # "<<ii<<endl;
    //~ //    parametri_moved.Print();
    
    //~ p_trk->ComputeDataAll(hitp);
    //~ distanze_moved = p_trk->GetTrackRho();
    //~ for(int kk = 0;kk<ntass;kk++){
      
      //~ AA(kk,ii) = (distanze_moved(kk)-distanze0(kk))/
	//~ (parametri_moved(ii)-alpha(ii));
    //~ }
  //~ }

  //~ p_trk->Set(alpha);
  //~ p_trk->ComputeDataAll(hitp);

  //~ return AA;
//~ }

//~ TMatrixD TABMntuTrack::ComputeVV(TABMntuRaw *hitp, TABMntuTrackTr* p_trk) {
  
  //~ double errore=1000000.;
  //~ TMatrixD VV;
  //~ int nmeas = hitp->nhit;
  //~ int ntass = p_trk->nass;
  //~ int tmp_i(0);
  //~ VV.ResizeTo(ntass,ntass);
  //~ VV = 0.;
  //~ for(int ii = 0; ii<nmeas;ii++){
    //~ TABMntuHit *wr = hitp->Hit(ii);
    //~ if(!wr->TrkAss()) continue;
    //~ errore = wr->GetSigma();
    //~ if(!errore)
      //~ errore = 0.012; //120 mum
    //~ VV(tmp_i,tmp_i) =  1./errore/errore;
    //~ tmp_i++;
  //~ }
  //~ return VV ;

//~ }

//~ TVectorD TABMntuTrack::Mini(int nmeas, TMatrixD AA, TMatrixD VV, TVectorD Dy) {

  //~ TMatrixD B, AAt, VVa;
  //~ TVectorD Eta;
  //~ VVa.ResizeTo(4,4);
  //~ AAt.ResizeTo(4,nmeas);
  //~ B.ResizeTo(4,nmeas);
  //~ Eta.ResizeTo(4);

  //~ AAt.Transpose(AA);
  //~ VVa = AAt*VV*AA;
  //~ VVa.Invert();
  //~ B = VVa*AAt*VV;
  //~ Eta = B*Dy;
  //~ return Eta ;
//~ }

//~ /*-----------------------------------------------------------------*/
//~ void TABMntuTrack::Update(TVectorD Dy, TVectorD& alpha, TVectorD Eta, TMatrixD VV, TABMntuTrackTr * ptraccia, TABMntuRaw *hitp) {

  //~ Int_t status=0;
  //~ TVectorD Aus; double chiqua;
  //~ Int_t rows = Dy.GetNrows();
  //~ Aus.ResizeTo(rows);

  //~ alpha = Eta + alpha;
  //~ if(fabs(alpha(2))>1 || TMath::IsNaN(alpha(2))) alpha(2) = 0;
  //~ if(fabs(alpha(3))>1 || TMath::IsNaN(alpha(3))) alpha(3) = 0;
  //~ status = ptraccia->Set(alpha(0),alpha(1),alpha(2),alpha(3));
  //~ status = ptraccia->ComputeDataAll(hitp);
  //~ ComputeDy(hitp, ptraccia);
  //~ Aus = VV*Dy;
  //~ chiqua = Dy*Aus;

  //~ if(ptraccia->nass) {
    //~ ptraccia->SetChi2(chiqua/(double)ptraccia->nass);
    //~ ptraccia->SetChi2H(Dy, Aus, hitp);
  //~ }
  //~ return;
//~ }

//~ Double_t TABMntuTrack::ComputeChiQua(TVectorD Dy, TMatrixD VV) {

  //~ TVectorD Aus; double chiqua;
  //~ Int_t numero_rows = Dy.GetNrows();
  //~ Aus.ResizeTo(numero_rows);
  //~ Aus = VV*Dy;
  //~ chiqua = Dy*Aus;

  //~ return chiqua;
//~ }

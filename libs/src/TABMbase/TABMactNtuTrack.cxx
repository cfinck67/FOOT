/*!
  \file
  \version $Id: TABMactNtuTrack.cxx,v 1.12 2003/07/08 18:58:35 mueller Exp $
  \brief   Implementation of TABMactNtuTrack.
*/

#include "TABMactNtuTrack.hxx"

using namespace std;
/*!
  \class TABMactNtuTrack TABMactNtuTrack.hxx "TABMactNtuTrack.hxx"
  \brief Track builder for Beam Monitor. **
*/

ClassImp(TABMactNtuTrack);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMactNtuTrack::TABMactNtuTrack(const char* name,
                                 TAGdataDsc* dscntutrk,
                                 TAGdataDsc* dscnturaw,
                                 TAGparaDsc* dscbmgeo,
                                 TAGparaDsc* dscbmcon,
                                 TAGparaDsc* dsctggeo)
  : TAGaction(name, "TABMactNtuTrack - Track finder for BM"),
    fpNtuTrk(dscntutrk),
    fpNtuHit(dscnturaw),
    fpBMGeo(dscbmgeo),
    fpBMCon(dscbmcon),
    fpTGgeo(dsctggeo)
{
  AddDataOut(fpNtuTrk, "TABMntuTrack");
  AddDataIn(fpNtuHit,  "TABMntuRaw");
  AddPara(fpBMGeo,  "TABMparGeo");
  AddPara(fpBMCon,  "TABMparCon");


  const Int_t nIter = 20; // max number of iterations
  const Double_t dPVal = 1.E-3; // convergence criterion used by GenFit

  //~ simpleFitter = new KalmanFitter(nIter, dPVal);
  //~ refFitter = new KalmanFitterRefTrack(nIter, dPVal);   
  //~ dafRefFitter = new DAF(true, nIter, dPVal);
  //~ dafSimpleFitter = new DAF(false, nIter, dPVal);
  
  //~ const double BField = 0.;       // kGauss [ --> Eq. to 0.1 T]

  // init geometry and mag. field
  
  //~ new TGeoManager("Geometry", "Geane geometry");//load geometry
 // TGeoManager::Import("genfitGeomFOOT.root");
  
  //~ TDatabasePDG* pdg_base = new TDatabasePDG();
  //~ char nome_carbonio[]="carbon";
  //~ char title_carbonio[]="Carbon12";
  //~ char class_carbonio[]="ion";
  //~ pdg_base->AddParticle(nome_carbonio, title_carbonio, 11.1749, true, 0., 18., class_carbonio, 1006661212120);//
  
  //Bfield is along Y in our case.
  
  //~ FieldManager::getInstance()->init(new ConstField(0.,0.,0.)); //one day the magnetic field map will be load here, but now there isn't any accurate magnetic field... 
  //~ MaterialEffects::getInstance()->init(new TGeoMaterialInterface());//it is necessary
  
  p_bmcon = (TABMparCon*) fpBMCon->Object();
  p_bmgeo = (TABMparGeo*) fpBMGeo->Object();
  p_tggeo = (TAGparGeo*)  fpTGgeo->Object();
   
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   
   TVector3 tgCenter = geoTrafo->GetTGCenter();
   TVector3 bmCenter = geoTrafo->GetBMCenter();
   
   Float_t tgThick = p_tggeo->GetTargetPar().Size[2];
   
   TVector3 targetPos(tgCenter[0], tgCenter[1], -bmCenter[2]+tgCenter[2]+tgThick/2.);
   p_bmgeo->SetTarget(targetPos);

   
  //~ TVector3 Xvers(1.,0,0), Yvers(0.,1.,0.), central(0.,0.,0.);
  //~ SharedPlanePtr tmp_mylar1_plane(new DetPlane(p_bmgeo->GetMylar1(), Xvers, Yvers));
  //~ SharedPlanePtr tmp_central_plane(new DetPlane(central, Xvers, Yvers));
  //~ SharedPlanePtr tmp_mylar2_plane(new DetPlane(p_bmgeo->GetMylar2(), Xvers, Yvers));
  //~ SharedPlanePtr tmp_target_plane(new DetPlane(p_bmgeo->GetTarget(), Xvers, Yvers));
  //~ mylar1_plane=tmp_mylar1_plane;
  //~ central_plane=tmp_central_plane;
  //~ mylar2_plane=tmp_mylar2_plane;
  //~ target_plane=tmp_target_plane;

  //~ mylar1_detplane=new DetPlane(p_bmgeo->GetMylar1(), Xvers, Yvers);
  //~ mylar2_detplane=new DetPlane(p_bmgeo->GetMylar2(), Xvers, Yvers);
  //~ target_detplane=new DetPlane(p_bmgeo->GetTarget(), Xvers, Yvers);
  //~ mylar1_plane=mylar1_detplane;  
  //~ mylar2_plane=mylar2_detplane;  
  //~ target_plane=target_detplane;    


}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMactNtuTrack::~TABMactNtuTrack()
{ 
  //~ delete simpleFitter;
  //~ delete refFitter;
  //~ delete dafSimpleFitter;
  //~ delete dafRefFitter;
  //~ delete mylar1_detplane;
  //~ delete mylar2_detplane;
  //~ delete target_detplane;
}


//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TABMactNtuTrack::CreateHistogram()
{
   DeleteHistogram();
   
   //~ fpHisR02d = new TH2F("bmR02d","BM - Position of the track on the BM center plane", 500, -3., 3.,500 , -3., 3.);
   //~ AddHistogram(fpHisR02d);   
   fpHisMap = new TH2F("BM_Track_bmMap","BM - Position of the track at target center", 250, -3., 3.,250 , -3, 3);
   AddHistogram(fpHisMap);
   fpHisMylar12d = new TH2F("BM_Track_bmmylar1_2d","BM - Position of the track on the BM center plane", 500, -3., 3.,500 , -3., 3.);
   AddHistogram(fpHisMylar12d);   
   fpHisMylar22d = new TH2F("BM_Track_bmmylar2_2d","BM - Position of the track on the BM center plane", 500, -3., 3.,500 , -3., 3.);
   AddHistogram(fpHisMylar22d);   
   fpResTot = new TH2F("BM_Track_bm_residual_tot","Residual vs Rdrift; Residual [cm]; Measured rdrift [cm]", 6000, -0.3, 0.3,250 , 0., 1.);
   AddHistogram(fpResTot);  
   fpHisTrackStatus = new TH1I("BM_Track_track_status","Track status; -3=prefitstatuscut -2=maxhitcut__-1=minhitcut__0=ok__1/2=firedV/Uplane__3=hitrejected__4=noconv__5=chi2cut; Events", 15, -5, 10);
   AddHistogram(fpHisTrackStatus);  
   fpHisNhitTrack = new TH1I("BM_Trak_nhitsxtrack","number of hits x track; ; Events", 30, 0, 30);
   AddHistogram(fpHisNhitTrack);  
   fpHisPrefitStatus = new TH1I("BM_Track_prefit_status","bm prefit status; ; Events", 3, 0, 3);
   AddHistogram(fpHisPrefitStatus);  
   fpHisNite = new TH1I("BM_Track_nite","Number of iteration of the tracking method; Number of iteration; Events", p_bmcon->GetNumIte(), 0,p_bmcon->GetNumIte() );
   AddHistogram(fpHisNite);  
   fpHisChi2Red = new TH1F("BM_Track_chi2red","chi2red", 100, 0., 100.);
   AddHistogram(fpHisChi2Red);   

   SetValidHistogram(kTRUE);
}

//------------------------------------------+-----------------------------------
//! Action.
Bool_t TABMactNtuTrack::Action()
{  
  TABMntuTrack* p_ntutrk = (TABMntuTrack*) fpNtuTrk->Object();
  p_nturaw = (TABMntuRaw*)   fpNtuHit->Object();
  //~ TABMparGeo*   p_bmgeo = (TABMparGeo*)    fpBMGeo->Object();
  //~ TABMparCon*   p_bmcon = (TABMparCon*)    fpBMCon->Object();
  
  //~ TF1 *m_mypol, *m_mypol2;
  //~ m_mypol = p_bmcon->GetCalibX();
  //~ m_mypol2 = p_bmcon->GetCalibY();
  
  p_ntutrk->Clear();//maybe useless? 
  p_ntutrk->SetEffFittedPlane(-3.); 

  Double_t chisquare_cut = 5.;


  Int_t i_nhit = p_nturaw->GetHitsN();

   //NB.: If the preselection reject the event no track will be saved     
  
  if(p_bmcon->GetBMdebug()>10)
    cout<<"I'm in TABMactNtuTrack::Action"<<endl;  
                
                
                
  //parameters  
  Double_t tmp_double, res;
  bool converged;
  TABMntuTrackTr *tmp_trackTr=new TABMntuTrackTr();
  TABMntuTrackTr best_trackTr;
  //~ vector<AbsMeasurement*> measurements_vec;  
  //~ Double_t x, y, z, cx, cy, cz;
  Int_t hit_view, tmp_int, tmp_cellx=1, tmp_celly=1, tracknum=1;
  TMatrixDSym hitCov(7);
  TVectorD hitCoords(7);
  vector<vector<Int_t>> hitxplane(BMN_NLAY*2); //number of hit for every bm plane (plane should be 12 in BM)
  Int_t firedPlane=BMN_NLAY*2; //number of plane fired
  Int_t firedUview=BMN_NLAY;//number of U view plane fired
  Int_t firedVview=BMN_NLAY;//number of V view plane fired
  Int_t firedSingleUview=0;//number of U view plane fired with only 1 hit
  Int_t firedSingleVview=0;//number of V view plane fired with only 1 hit
  TDecompChol fitTrack_cov;  
  //~ TVector3 wire_a_x, wire_b_x, wire_a_y, wire_b_y;
  bool tmp_bool;  
  vector<Int_t> singlehittrack;  

  Track* fitTrack(nullptr);
  AbsTrackRep* rep(nullptr);  

  //COUNTER FOR NUMBER OF POSSIBLE TRACKS:
  for(Int_t i_h = 0; i_h < i_nhit; i_h++) {
    p_hit = p_nturaw->Hit(i_h);
    //~ if(ToBeConsider(p_hit->Cell(), p_hit->View(), p_hit->Plane())) 
    if(p_bmcon->GetBMdebug()>10)
      cout<<"hit="<<i_h<<" plane="<<p_hit->Plane()<<"  view="<<p_hit->View()<<"  cell="<<p_hit->Cell()<<"  piano="<<p_bmgeo->GetWirePlane(p_hit->Plane(),p_hit->View())<<"  rdrift="<<p_hit->Dist()<<endl;
     //~ if (p_bmgeo->GetWirePlane(p_hit->Plane(),p_hit->View()) >= 0)
      hitxplane.at(p_bmgeo->GetWirePlane(p_hit->Plane(),p_hit->View())).push_back(i_h);
  }
  //calculate number of possible tracks (tracknum), the number of the plane with at least one hit for each view (firedUview/firedVview) and for both the views (firedPlane)
  for(Int_t j = 0; j < hitxplane.size(); j++) {  
    if(hitxplane.at(j).size()!=0){
      tracknum*=hitxplane.at(j).size();
      if(hitxplane.at(j).size()==1){
        singlehittrack.push_back(hitxplane.at(j).at(0));
        if(j%2==0)
          firedSingleUview++;
        else
          firedSingleVview++;
      }
    }else{
      firedPlane--;
      if(j%2==0)
        firedUview--;
      else
        firedVview--;   
    }
  }
  
  Int_t rejhit=i_nhit-firedPlane;
  if(rejhit<0)
    cout<<"ERROR!!!! in TABMactNtuTrack::rejhit<0   i_nhit="<<i_nhit<<"  firedPlane="<<firedPlane<<endl;//provv
  
  if(p_bmcon->GetBMdebug()>4)
    cout<<"TABMactNtuTrack:: tracknum="<<tracknum<<"  firedPlane="<<firedPlane<<"  firedUview="<<firedUview<<"  firedVview="<<firedVview<<endl;    
  
  //*************APPLY SOME CUTS************
  if(firedUview<p_bmcon->GetPlanehitcut() || firedVview<p_bmcon->GetPlanehitcut()){
    if(p_bmcon->GetBMdebug()>3)
      cout<<"TABMactNtuTrack::WARNING!!::no possible track!!: firedUview="<<firedUview<<"  firedVview="<<firedVview<<"   planehitcut="<<p_bmcon->GetPlanehitcut()<<endl;
    if(firedUview<p_bmcon->GetPlanehitcut())
      p_ntutrk->GetTrackStatus()=1;
    else
      p_ntutrk->GetTrackStatus()=2;
    //~ delete fitTrack;
    //~ delete tmp_trackTr;
    fpNtuTrk->SetBit(kValid);
    return kTRUE;
  }else if(i_nhit>=p_bmcon->GetMaxnhit_cut()){
    if(p_bmcon->GetBMdebug()>3)
      cout<<"TABMactNtuTrack::WARNING!!::the number of hits is too high:  number of hit="<<i_nhit<<"  Maxhitcut="<<p_bmcon->GetMaxnhit_cut()<<endl;
      p_ntutrk->GetTrackStatus()=-2;
    fpNtuTrk->SetBit(kValid);
    return kTRUE;
  }else if(i_nhit<=p_bmcon->GetMinnhit_cut()){
    if(p_bmcon->GetBMdebug()>3)
      cout<<"TABMactNtuTrack::WARNING!!::the number of hits is too low:  number of hit="<<i_nhit<<"  Minhitcut="<<p_bmcon->GetMinnhit_cut()<<endl;
      p_ntutrk->GetTrackStatus()=-1;  
    fpNtuTrk->SetBit(kValid);
    return kTRUE;
  }else
    p_ntutrk->GetTrackStatus()=-1000;
  
  //print hitxplane
  if(p_bmcon->GetBMdebug()>4){  
    cout<<"TABMactNtuTrack::print hitxplane"<<endl;  
    Print_matrix(hitxplane);    
  }
  
  //CHECK WHAT IS NEEDED ONLY BY GENFIT AND WHAT IS GLOBAL!!!!!!!!!!!!!!!!
  //~ vector<TABMntuTrackTr*> alltrack; 
  vector<vector<Int_t>> hitxtrack; 
  vector<Double_t> hit_res; //needed in CalculateMyChi2, it stores the hits resolutions
  vector<Double_t> hit_mysqrtchi2; //needed in CalculateMyChi2, it stores the mychi2 contribution for the hits of tmp_trackTr 
  vector<Double_t> best_mysqrtchi2; //stores the mysqrtchi2 contribution of best_trackTr's hits 
  Int_t best_index; //the position in hitxtrack matrix of the best tracktr
  vector<vector<Int_t>> prunedhit;//it is calculated for each hitxtrack row; each prunedhit row represent a possible new track, columns represent the hit position of hitxtrack that have to be pruned 
  Int_t prefit_status=-5;
//*************************************************NEW LEAST CHI2 TRACKING*************************************
 if(p_bmcon->GetFitterIndex()!=0) {
    //~ TABMntuTrackTr *tmp_atrackTr=new TABMntuTrackTr();    
    //~ TABMntuTrackTr *tmp_btrackTr=new TABMntuTrackTr();
        
    prunedhit.resize(1);
    //prefit only on the single hits
    if(firedSingleUview>2 && firedSingleVview>2){
      tmp_trackTr->Clean();
      EstimateFIRSTTrackPar(singlehittrack, firedSingleUview, firedSingleVview, tmp_trackTr);
      Chi2Fit(singlehittrack, prunedhit, tmp_trackTr, converged);
      if((firedSingleUview+firedSingleVview)<(firedUview+firedVview)){  //tracks with more hit on the same view and plane
        SortFirstDoubleHits(tmp_trackTr, hitxplane, hitxtrack);
        prefit_status=1;
        if(p_bmcon->GetOnlyprefit()!=0){
          p_ntutrk->GetTrackStatus()=-3;
          if(ValidHistogram())
            fpHisTrackStatus->Fill(p_ntutrk->GetTrackStatus());  
          fpNtuTrk->SetBit(kValid);
          return kTRUE;
        }
      }else{
        hitxtrack.push_back(singlehittrack);//no double hits
        prefit_status=0;
      }
    }else{
      prefit_status=-1;
      ChargeAllTracks(hitxtrack,hitxplane, tracknum, firedPlane);
      if(p_bmcon->GetOnlyprefit()!=0){
        p_ntutrk->GetTrackStatus()=-3;
        if(ValidHistogram())
          fpHisTrackStatus->Fill(p_ntutrk->GetTrackStatus());  
        fpNtuTrk->SetBit(kValid);
        return kTRUE;
      }
    }
    
    if(p_bmcon->GetBMdebug()>10)
      cout<<"TABMactNtuTrack:: End of prefit_status="<<prefit_status<<endl;
        
    best_index=0;
    for(Int_t i=0;i<hitxtrack.size();i++){
      
     if(p_bmcon->GetBMdebug()>10)
      cout<<"TABMactNtuTrack:: loop on hitxtrack: i="<<i<<endl; 
      
      if(prefit_status!=0 || (prefit_status==0 && i>0)){
        tmp_trackTr->Clean();
        EstimateFIRSTTrackPar(hitxtrack.at(i), firedUview, firedVview, tmp_trackTr);
        Chi2Fit(hitxtrack.at(i),prunedhit,tmp_trackTr, converged);
      }else{
        best_trackTr=*tmp_trackTr;
        best_index=i;        
      }
      
      if((tmp_trackTr->GetMyChi2Red() < best_trackTr.GetMyChi2Red()) || i==0){
        best_trackTr=*tmp_trackTr;
        best_index=i;
      }
      
      if(prunedhit.at(0).size()>0){//charge prune hits
        if((i_nhit-hitxtrack.at(i).size()-prunedhit.at(0).size())>p_bmcon->GetRejmaxcut())
          ChargePrunedTrack(prunedhit.at(0), firedUview, firedVview, hitxtrack, i);
        else if(p_bmcon->GetBMdebug()>3)
          cout<<"TABMactNtuTrack:: prunedhit cannot added to hitxtrack due to Rejmaxcut:  hitxtrack index="<<i<<"  i_nhit="<<i_nhit<<"  hitxtrack.at(i).size()="<<hitxtrack.at(i).size()<<"  prunedhit.at(0).size()="<<prunedhit.at(0).size()<<"  rejhitmax="<<p_bmcon->GetRejmaxcut()<<endl;
        prunedhit.at(0).clear();
      }
      
    }//end of the loop on hitxtrack
    
    //save and update the best_track
    best_trackTr.CalculateFromFirstPar(p_bmcon, p_bmgeo);
    best_trackTr.SetPrefitStatus(prefit_status);
    //~ delete tmp_atrackTr;
    //~ delete tmp_btrackTr;
    //~ UpdateHitsFromTrack(best_mysqrtchi2, best_trackTr, hitxtrack[best_index]);//useless

  }
/* else if(p_bmcon->GetFitterIndex()<5){   //************************* GENFIT TRACKING***********************************
    //provv check
    //~ if(singlehittrack.size()!=firedSingleVview+firedSingleUview){
      //~ cout<<"TABMactNtuTrack::ERROR in PrefitTracking:: singlehittrack.size()="<<singlehittrack.size()<<" firedSingleUview="<<firedSingleUview<<"  firedSingleVview="<<firedSingleVview<<endl;
    //~ }
    
    //PERFORM A PREFIT AND CHARGE HITXTRACK    
    //~ cout<<"p_bmcon->GetPrefitEnable()="<<p_bmcon->GetPrefitEnable()<<"  firedSingleUview="<<firedSingleUview<<"  firedSingleVview="<<firedSingleVview<<"  (firedUview+firedVview)="<<firedUview+firedVview<<endl;
    if(p_bmcon->GetPrefitEnable()>0 && firedSingleUview>2 && firedSingleVview>2 && (firedSingleUview+firedSingleVview)<(firedUview+firedVview))
      PrefitTracking(prefit_status, firedUview, firedVview,singlehittrack, hitxtrack, hitxplane, hit_res, hitCov, hitCoords, best_trackTr, fitTrack, rep); 
  
    if(!(prefit_status==0 || prefit_status==1))//charge ALL possible tracks in hitxtrack
      ChargeAllTracks(hitxtrack,hitxplane, tracknum, firedPlane);
    
    //check for possible tracks: reject tracks in which there are hits on the same view that are too different-->bad idea if the bm is not alligned with the track!!!
    //~ tmp_int=0;
    //~ for(Int_t i=0; i<hitxtrack.size(); i++) {
      //~ RejectSlopedTrack(hitxtrack, possiblePrimary,p_hit, p_nturaw, i);
      //~ if(possiblePrimary[i]==true)
        //~ tmp_int++;  
      //~ }
    //~ if(tmp_int==0){ 
      //~ if(p_bmcon->GetBMdebug()>0)
        //~ cout<<"TABMactNtuTrack::ERROR!!::no possible track!!!"<<endl;
      //~ p_ntutrk->trk_status=5;  
      //~ delete fitTrack;
      //~ delete tmp_trackTr;
      //~ fpNtuTrk->SetBit(kValid);
      //~ return kTRUE;
      //~ }
      
    if((tracknum>1 && p_bmcon->GetBMdebug()>1) || p_bmcon->GetBMdebug()>10)
      cout<<"TABMactNtuTrack::number of total hits="<<i_nhit<<"   number of possible tracks="<<hitxtrack.size()<<"  number of fired plane="<<firedPlane<<endl;  
      
    //print hitxtrack
    if(p_bmcon->GetBMdebug()>4){
      cout<<"TABMactNtuTrack::print hitxtrack.size()="<<hitxtrack.size()<<endl;  
      Print_matrix(hitxtrack);
    }
    
    //check on rejmax_cut
    if(rejhit>p_bmcon->GetRejmaxcut()){
      p_ntutrk->GetTrackStatus()=3;
      //~ delete fitTrack;
      //~ delete tmp_trackTr;      
      if(p_bmcon->GetBMdebug()>0)
        cout<<"TABMactNtuTrack:: no track given the rejmax_cut="<<p_bmcon->GetRejmaxcut()<<"  i_nhit="<<i_nhit<<"  firedPlane="<<firedPlane<<endl;
      if(ValidHistogram())
        fpHisTrackStatus->Fill(p_ntutrk->GetTrackStatus());  
      fpNtuTrk->SetBit(kValid);
      return kTRUE;    
    }
    
    //**********************loop on all possible tracks:**********************
    for(Int_t i=0; i<hitxtrack.size(); i++) {
      rejhit=i_nhit-hitxtrack.at(i).size();
      //~ if(rejhit<0)
        //~ cout<<"ERROR!!!! in TABMactNtuTrack::rejhit<0   i_nhit="<<i_nhit<<"  hitxtrack.at(i).size()="<<hitxtrack.at(i).size()<<"   i="<<i<<endl;//check
        
      if(p_bmcon->GetBMdebug()>4){//provv
        cout<<"TABMactNtuTrack:: new hitxtrack: i="<<i<<" hitxtrack.size()="<<hitxtrack.size()<<endl;
        for(Int_t kk=0;kk<hitxtrack.at(i).size();kk++)
          cout<<hitxtrack.at(i).at(kk)<<" ";
        cout<<endl;  
      }
      
      //~ if(rejhit>p_bmcon->GetRejmaxcut()){//check
        //~ cout<<"ERROR!!! in TABMactNtuTrack:: rejhit>p_bmcon->GetRejmaxcut()    rejhit="<<rejhit<<"  p_bmcon->GetRejmaxcut()="<<p_bmcon->GetRejmaxcut()<<endl;
        //~ possiblePrimary[i]=false;
      //~ }
      
        
      if(p_bmcon->GetBMdebug()>3) 
        cout<<endl<<"********************* TABMactNtuTrack :: charging track number "<<i<<"  *******************"<<endl;
      tmp_trackTr->Clean(); 
      tmp_trackTr->SetNhit(hitxtrack.at(i).size()); 
      tmp_trackTr->SetPrefitStatus(prefit_status);
      //~ delete fitTrack;
      //~ delete rep;
      //~ fitTrack = nullptr;
      //~ rep=nullptr;
      //~ AbsTrackRep* rep = new RKTrackRep(sign*pdg);
      //~ AbsTrackRep* rep = new RKTrackRep(1006661212120);//provv
        
      //~ AbsTrackRep *rep = new RKTrackRep(2212);//protons for the moment  
      //~ Track* fitTrack = new Track(rep, init_pos, init_mom);
      //FITTING      
      if(prefit_status!=0 || (prefit_status==0 && i>0))
        MyGenfitFitting(hitxtrack.at(i), firedUview, firedVview, hit_res, hitCov, hitCoords, fitTrack, rep);    
      
      converged=fitTrack->getFitStatus(rep)->isFitConverged();
      tmp_trackTr->SetIsConverged((converged) ? 1:2);
      
      //~ if(converged && fitTrack->getNumPoints()!=fitTrack->getNumPointsWithMeasurement()){
        //~ cout<<"TABMactNtuTrack::WARNING: number of trackPoints is different from number of trackPointWithMeasurement.. something odd happened"<<endl;
        //~ converged=false;
      //~ } 
      if(p_bmcon->GetBMdebug()>10 && converged)
        cout<<"TABMactNtuTrack::fit converged"<<endl;
      else if(p_bmcon->GetBMdebug()>4 && !converged)
        cout<<"TABMactNtuTrack::fit NOT converged"<<endl;
      
      if(converged) {
          tmp_trackTr->SetChi2New(fitTrack->getFitStatus(rep)->getChi2());
          tmp_trackTr->SetNdf(fitTrack->getFitStatus(rep)->getNdf());  
          tmp_trackTr->SetFailedPoint(fitTrack->getFitStatus(rep)->getNFailedPoints());  
          if(fitTrack->getFitStatus(rep)->getNdf()!=0)        
            tmp_trackTr->SetChi2NewRed(fitTrack->getFitStatus(rep)->getChi2()/fitTrack->getFitStatus(rep)->getNdf());
          hit_mysqrtchi2.clear();
          hit_mysqrtchi2.resize(hitxtrack.at(i).size(),999.);
          tmp_trackTr->CalculateFitPar(fitTrack, hit_res, hit_mysqrtchi2, prunedhit,p_bmcon, p_bmgeo, rejhit, mylar1_plane,central_plane, mylar2_plane, target_plane);
          if(p_bmcon->GetBMdebug()>3 && converged){
            cout<<"TABMactNtuTrack::print fit status:"<<endl;
            fitTrack->getFitStatus(rep)->Print();
          }
          //~ alltrack.push_back(tmp_trackTr);
          if(tmp_trackTr->GetMyChi2Red()<best_trackTr.GetMyChi2Red() || best_trackTr.GetNhit()==0){
            if(p_bmcon->GetBMdebug()>5)
              cout<<"New best_trackTr found!  previous mychi2red="<<best_trackTr.GetMyChi2Red()<<"  New mychi2red="<<tmp_trackTr->GetMyChi2Red()<<endl;
            best_trackTr=*tmp_trackTr;
            best_mysqrtchi2=hit_mysqrtchi2;
            best_index=i;
          }
        }else if((rejhit+1)<=p_bmcon->GetRejmaxcut()) //end of converged
          PruneNotConvTrack(prunedhit,hitxtrack, i);
      //~ delete simpleFitter;//provv
      delete fitTrack; //it should delete rep also 
      if(prunedhit.size()>0){
        for(Int_t k=0;k<prunedhit.size();k++)
          ChargePrunedTrack(prunedhit.at(k), firedUview, firedVview, hitxtrack, i);
        prunedhit.clear();
      }
    }//end of loop on all possible track
    
 
  }//***********************************************END OF GENFIT TRACKING*************************  
*/    
  if(p_bmcon->GetBMdebug()>10)  
    cout<<"TABMactNtuTrack:end of tracking"<<endl;
  
  if(best_trackTr.GetNhit()!=0){
    TABMntuTrackTr* trk = new((*(p_ntutrk->GetListOfTracks()))[p_ntutrk->GetTracksN()]) TABMntuTrackTr(best_trackTr);
    p_ntutrk->GetTrackStatus()++;
    p_ntutrk->GetTrackStatus()= (best_trackTr.GetMyChi2Red()>=p_bmcon->GetChi2Redcut())? 5:0;
    vector<Int_t> hit_fittedplane(12,0);
    //flag the hit of the best track
    for(Int_t i=0;i<hitxtrack.at(best_index).size();i++){
      p_hit = p_nturaw->Hit(hitxtrack.at(best_index).at(i));    
      p_hit->SetIsSelected(true);
      if(p_bmcon->GetFitterIndex()<5){
        p_hit->SetChi2(best_mysqrtchi2.at(i)*best_mysqrtchi2.at(i));
        p_hit->SetResidualSigma(best_mysqrtchi2.at(i));
      }
      if(p_hit->View()==0)
        hit_fittedplane.at(p_hit->Plane())++;
      else
        hit_fittedplane.at(p_hit->Plane()+6)++;      
      if (ValidHistogram())  
        fpResTot->Fill(p_hit->GetResidual(),p_hit->Dist());    
    }//end of loop on hits
    
    //evalueate eff_fittedplane
    Int_t eff_fittedplane_probe=0, eff_fittedplane_pivot=0; 
    //view==0
    if(hit_fittedplane.at(0)>0 && hit_fittedplane.at(2)>0 && hit_fittedplane.at(4)>0){
      eff_fittedplane_pivot++;
      if(hit_fittedplane.at(1)>0 && hit_fittedplane.at(3)>0)
        eff_fittedplane_probe++;
    }
    if(hit_fittedplane.at(1)>0 && hit_fittedplane.at(3)>0 && hit_fittedplane.at(5)>0){
      eff_fittedplane_pivot++;
      if(hit_fittedplane.at(2)>0 && hit_fittedplane.at(4)>0)
        eff_fittedplane_probe++;
    }
    //view==1
    if(hit_fittedplane.at(6)>0 && hit_fittedplane.at(8)>0 && hit_fittedplane.at(10)>0){
      eff_fittedplane_pivot++;
      if(hit_fittedplane.at(7)>0 && hit_fittedplane.at(9)>0)
        eff_fittedplane_probe++;
    }
    if(hit_fittedplane.at(7)>0 && hit_fittedplane.at(9)>0 && hit_fittedplane.at(11)>0){
      eff_fittedplane_pivot++;
      if(hit_fittedplane.at(8)>0 && hit_fittedplane.at(10)>0)
        eff_fittedplane_probe++;
    }
    
    p_ntutrk->SetEffFittedPlane( (eff_fittedplane_pivot==0) ?  -1 : (Double_t) eff_fittedplane_probe/eff_fittedplane_pivot);
    if (ValidHistogram()){
       //~ fpHisR02d->Fill(trk->GetR0()[0],trk->GetR0()[1]);
       TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
       Float_t posZ = geoTrafo->FromGlobalToBMLocal(TVector3(0,0,0)).Z();
       TVector3 pos = trk->PointAtLocalZ(posZ);
       fpHisMap->Fill(pos[0], pos[1]);
       fpHisMylar12d->Fill(trk->GetMylar1Pos().X(),trk->GetMylar1Pos().Y());
       fpHisMylar22d->Fill(trk->GetMylar2Pos().X(),trk->GetMylar2Pos().Y());
       fpHisChi2Red->Fill(trk->GetMyChi2Red());
       fpHisNhitTrack->Fill(trk->GetNhit());
       fpHisNite->Fill(trk->GetNite());
       fpHisPrefitStatus->Fill(prefit_status);
     }
  }else if(converged==false)
    p_ntutrk->GetTrackStatus()=4;
  else
    cout<<"ERROR in TABMactNtuTrack :: track converged lost!"<<endl;
  
  
  //~ delete fitTrack;
  //~ delete rep; //included in fitTrack delete
  delete tmp_trackTr;    
  
  if(ValidHistogram())
    fpHisTrackStatus->Fill(p_ntutrk->GetTrackStatus());  

  if(p_bmcon->GetBMdebug()>10)
    cout<<"end of TABMactNtuTrack"<<endl;

  fpNtuTrk->SetBit(kValid);
  return kTRUE;
  
}


void TABMactNtuTrack::PruneNotConvTrack(vector<vector<Int_t>> &prunedhit, vector< vector<Int_t> > &hitxtrack, Int_t index){
  prunedhit.clear();
  prunedhit.resize(hitxtrack.at(index).size());
  for(Int_t i=0;i<hitxtrack.at(index).size();i++)
    prunedhit.at(i).push_back(i);
return;
}

void TABMactNtuTrack::ChargePrunedTrack(vector<Int_t> &tobepruned, Int_t &firedUview, Int_t &firedVview, vector< vector<Int_t> > &hitxtrack, Int_t index){
  vector<Int_t> tmp_vec_int;
  //check if the pruned track can pass the plane cuts
  std::sort(tobepruned.rbegin(), tobepruned.rend());
  for(Int_t kk=0;kk<tobepruned.size();kk++){
    if(p_bmcon->GetBMdebug()>4)
      cout<<"TABMactNtuTrack::a track has to be pruned:"<<"kk="<<kk<<"  tobepruned(pos)="<<tobepruned.at(kk)<<"  hitxtrack.at(index)[tobepruned.at(kk)]="<<hitxtrack.at(index).at(tobepruned.at(kk))<<endl;
    p_hit=p_nturaw->Hit(hitxtrack.at(index).at(tobepruned.at(kk)));
    if(p_bmcon->GetBMdebug()>4)
      cout<<"hit_view of the pruned hit="<<p_hit->View()<<"firedUview="<<firedUview<<"  firedVview="<<firedVview<<"  planehitcut="<<p_bmcon->GetPlanehitcut()<<"  new number_of_hit="<<hitxtrack.at(index).size()-tobepruned.size()<<"  minhitcut="<<p_bmcon->GetMinnhit_cut()<<endl;          
    //if it can pass, add the pruned track to hitxtrack         
    if(((p_hit->View()==1 && (firedUview-1)>=p_bmcon->GetPlanehitcut()) || (p_hit->View()==-1 && (firedVview-1)>=p_bmcon->GetPlanehitcut())) && (hitxtrack.at(index).size()-tobepruned.size())>=p_bmcon->GetMinnhit_cut()){
      tmp_vec_int=hitxtrack.at(index);
      if(p_bmcon->GetBMdebug()>4){
        cout<<"Before the pruning:"<<endl;
        for(Int_t kk=0;kk<tmp_vec_int.size();kk++)
          cout<<tmp_vec_int.at(kk)<<" ";
        cout<<endl;  
      }
      for(Int_t kk=0;kk<tobepruned.size();kk++)
        tmp_vec_int.erase(tmp_vec_int.begin()+tobepruned.at(kk));
      if(p_bmcon->GetBMdebug()>4){
        cout<<"After the pruning:"<<endl;
        for(Int_t kk=0;kk<tmp_vec_int.size();kk++)
          cout<<tmp_vec_int.at(kk)<<" ";
        cout<<endl;
	}
      hitxtrack.push_back(tmp_vec_int);
    }else if(p_bmcon->GetBMdebug()>4)
      cout<<"this pruning cannot be made for the planehitcut or the minnhitcut"<<endl;
  }
  
  return;
}  
  
  
//to print matrix
void TABMactNtuTrack::Print_matrix(vector<vector<int>>& vec){
  for(int i=0;i<vec.size();i++){
    for(int j=0;j<vec.at(i).size();j++)
      cout<<vec.at(i).at(j)<<" ";
    cout<<endl;
    }  
  cout<<endl;  
return;  
}

//primary particle shouldn't have any hit in some cell (cell very far from x=0, y=0)
//this method return true if the cell can be hit by primary and false if not (it need view==1 || view==-1)
bool TABMactNtuTrack::ToBeConsider(const Int_t cell, const Int_t view, const Int_t lay){
  bool consider=true;
  if((lay%2==0 && cell==2 && view==0) || (lay%2==1 && cell==0 && view==0) || (lay%2==0 && cell==0 && view==1) || (lay%2==1 && cell==2 && view==1))    consider=false;
  return consider;
}


//set initial position (init_pos)
void TABMactNtuTrack::SetInitPos(TVector3 &init_pos, Int_t fit_index, Double_t xwire, Double_t xrdrift, Double_t ywire, Double_t yrdrift, Double_t init_z){
  if(fit_index==0){
    init_pos.SetXYZ(0., 0., init_z);
  }else if(fit_index==1){
    init_pos.SetXYZ(xwire+xrdrift, ywire+yrdrift, init_z);
  }else if(fit_index==2){
    init_pos.SetXYZ(xwire+xrdrift,ywire-yrdrift,init_z);  
  }else if(fit_index==3){
    init_pos.SetXYZ(xwire-xrdrift,ywire-yrdrift,init_z);  
  }else if(fit_index==4){
    init_pos.SetXYZ(xwire-xrdrift,ywire-yrdrift,init_z);  
  }
  return;  
}


void TABMactNtuTrack::ChargeAllTracks(vector< vector<Int_t> > &hitxtrack,vector< vector<Int_t> > &hitxplane, Int_t tracknum, Int_t firedPlane) {
  hitxtrack.clear();
  hitxtrack.resize(tracknum);
  for(Int_t j=0; j<tracknum; j++)
    hitxtrack.at(j).resize(firedPlane);  
  
  Int_t block=1, planeindex=0, shift, tmp_int;
  for(Int_t i=0;i<BMN_NLAY*2;i++){
    if(hitxplane.at(i).size()>0){
      tmp_int=0;
      shift=0;
      block*=hitxplane.at(i).size();
      while(tmp_int<tracknum){
        for(Int_t k=0;k<hitxplane.at(i).size();k++){
          for(Int_t j=tmp_int;j<tracknum/block+shift;j++){
            hitxtrack.at(tmp_int).at(planeindex)=hitxplane.at(i).at(k);
            tmp_int++;
          }
          shift=tmp_int;
        }
      }
      planeindex++;
    }
  }
  
  return;
}     

//charge hits: set fitTrack, firedVview, firedUview, wire_a_x, wire_a_y, rdrift_a_x, rdrift_a_y, hit_res 
void TABMactNtuTrack::ChargeHits4GenfitTrack(vector<Int_t> &singlehittrack,Int_t &firedUview,Int_t &firedVview, vector<Double_t> &hit_res, TMatrixDSym &hitCov, TVectorD &hitCoords, Track *&fitTrack, Double_t &wire_a_x, Double_t &wire_a_y, Double_t &rdrift_a_x, Double_t &rdrift_a_y){
  
  hit_res.clear();
  hit_res.resize(singlehittrack.size(),999.);
  firedUview=0;
  firedVview=0;      
  //charging loop
  for(Int_t i_h = 0; i_h <singlehittrack.size() ; i_h++) {
    
     if (fDebugLevel > 0)
        Info("Action()","create WireHit");
    p_hit = p_nturaw->Hit(singlehittrack.at(i_h));
    
    if(p_hit->View()==0)
      firedUview++;
    else 
      firedVview++;        
     
    hitCoords(0)= p_hit->GetA0().X();
    hitCoords(1)= p_hit->GetA0().Y();
    hitCoords(2)= p_hit->GetA0().Z();

    hitCoords(3)= p_hit->GetA0().X()+p_hit->GetWvers().X();
    hitCoords(4)= p_hit->GetA0().Y()+p_hit->GetWvers().Y();
    hitCoords(5)= p_hit->GetA0().Z()+p_hit->GetWvers().Z();

    hitCoords(6)= p_hit->Dist();
    
    if(p_hit->GetSigma()==0)
      cout<<"TABMactNtuTrack::WARNING:   something is wrong in hit sigma!!!!!!!, p_hit->GetSigma==0..."<<endl;
    hitCov.UnitMatrix();         // matrice di covarianza da settare meglio: per ora metto solo matrice diagonale con errore su posizione fili 
    hitCov *= wire_err*wire_err; //ed errore su rdrift, manca studio sulla correlazione tra le componenti... ma forse non serve
    hitCov[6][6]=p_hit->GetSigma()*p_hit->GetSigma(); 
        
    hit_res.at(i_h)=p_hit->GetSigma();
        
    if(p_bmcon->GetBMdebug()>=12){
      cout<<"show charging BM data:"<<endl;
      cout<<"x:"<<hitCoords[0]<<"  y:"<<hitCoords[1]<<"  z:"<<hitCoords[2]<<endl;
      cout<<"cx:"<<hitCoords[3]-hitCoords[0]<<"  cy:"<<hitCoords[4]-hitCoords[1]<<"  cz:"<<hitCoords[5]-hitCoords[2]<<endl;
      cout<<"view"<<p_hit->View()<<"  rdrift="<<p_hit->Dist()<<endl;
    }
    
    //~ fitTrack->insertMeasurement(measurements_vec.back()); 	
    fitTrack->insertMeasurement(new WireMeasurement(hitCoords, hitCov, det_Id, i_h, new TrackPoint(fitTrack))); 	
    //set variables for setinitpos:
    if(p_hit->View()==0 && wire_a_x==-1000.){// view 0 are wire on x, that give a y measurement
      wire_a_x=hitCoords[1];
      rdrift_a_x=p_hit->Dist();
    }else if(p_hit->View()==1 && wire_a_y==-1000.){
      wire_a_y=hitCoords[0];
      rdrift_a_y=p_hit->Dist();
    }
      
  }//end of charge hits loop  

  
  return;
}


/*
//prefit
void TABMactNtuTrack::PrefitTracking(Int_t &prefit_status,Int_t &firedUview,Int_t &firedVview, vector<Int_t> singlehittrack, vector< vector<Int_t> > &hitxtrack, vector< vector<Int_t> > &hitxplane, vector<Double_t> &hit_res, TMatrixDSym &hitCov, TVectorD &hitCoords, TABMntuTrackTr &best_trackTr,Track *&fitTrack, AbsTrackRep *&rep){
   
   MyGenfitFitting(singlehittrack, firedUview, firedVview, hit_res, hitCov, hitCoords, fitTrack, rep);      
  if(p_bmcon->GetBMdebug()>4)
    cout<<"TABMactNtuTrack::PrefitTrack::prefitting ended, converged="<<fitTrack->getFitStatus(rep)->isFitConverged()<<endl;  
  
  if(fitTrack->getFitStatus(rep)->isFitConverged()){
    singlehittrack.clear();
    MeasuredStateOnPlane state, provv_state;
    TVector3 wire_pos, wire_dir;
    Int_t hit_view, best_hit;
    Double_t best_diff, rdrift_diff;
    for(Int_t i=0; i<hitxplane.size();i++){
      //~ cout<<"i="<<i<<"  hitxplane.at(i).size()="<<hitxplane.at(i).size()<<endl;//provv
      if(hitxplane.at(i).size()==1){
        singlehittrack.push_back(hitxplane.at(i).at(0));
      }else if(hitxplane.at(i).size()>1){
        //~ cout<<"provo a fare getpointwithmeasurement"<<endl;//provv
        if(fitTrack->getPointWithMeasurement(0)->hasFitterInfo(fitTrack->getTrackRep(0))){        
          state=fitTrack->getFittedState(0);
          best_diff=1000.;
          //~ cout<<"try to calculate best_hit"<<endl;//provv
          for(Int_t k=0;k<hitxplane.at(i).size();k++){
            p_hit=p_nturaw->Hit(hitxplane.at(i).at(k));

             //~ cout<<"più hit su un unico piano i="<<i<<" size="<<hitxplane.at(i).size()<<"  k="<<k<<"  hitxplane.at(i).at(k)="<<hitxplane.at(i).at(k)<<" plane="<<p_hit->Plane()<<"  view="<<hit_view<<"  cell="<<p_hit->Cell()<<endl;
            
            //da testare
            //~ wire_pos.SetXYZ(p_bmgeo->GetX(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),hit_view), p_bmgeo->GetY(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),hit_view), p_bmgeo->GetZ(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),hit_view));
            //~ wire_dir.SetXYZ(p_bmgeo->GetCX(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),hit_view), p_bmgeo->GetCY(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),hit_view), p_bmgeo->GetCZ(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),hit_view));
            wire_pos=p_hit->GetA0();
            wire_dir=p_hit->GetWvers();
            wire_dir.SetMag(1.);
            rdrift_diff=fabs(best_trackTr.FindRdrift(state.getPos(), state.getMom(), wire_pos, wire_dir) - p_hit->Dist());
            //~ cout<<"hit="<<k<<"  fittedrdrift="<<best_trackTr.FindRdrift(state.getPos(), state.getMom(), wire_pos, wire_dir)<<"  realrdrfit="<<p_hit->Dist()<<"   rdrift_diff="<<rdrift_diff<<endl;//provv
            if(rdrift_diff<best_diff){
              best_diff=rdrift_diff;
              best_hit=k;
            }
          }
          //~ cout<<"best_hit="<<best_hit<<"  best_diff="<<best_diff<<endl;//provv
          if(best_diff<0.943)
            singlehittrack.push_back(hitxplane.at(i).at(best_hit));
            //~ rejhit+=hitxplane.at(i).size()-1;
        }else{
          cout<<"TABMactNtuTrack::UNEXPECTED ERROR in PrefitTracking!!!! fitted measurement without FitterInfo-->Please modify the code to include this case!!"<<endl;
          prefit_status=-10;
          delete fitTrack;
          return;
        }
        prefit_status=1;   
      }    
    }
    if(prefit_status==-5)  
      prefit_status=0;
  }else
    prefit_status=-1;
  
  if(p_bmcon->GetBMdebug()>4)  
    cout<<"TABMactNtuTrack::end of PrefitTrack   prefit_status="<<prefit_status<<"   converged="<<fitTrack->getFitStatus(rep)->isFitConverged()<<endl;
  
  hitxtrack.push_back(singlehittrack); 
  return;
}
*/

//fitting with genfit
//~ void TABMactNtuTrack::MyGenfitFitting(vector<Int_t> &singlehittrack,Int_t &firedUview,Int_t &firedVview, vector<Double_t> &hit_res, TMatrixDSym &hitCov, TVectorD &hitCoords, Track *&fitTrack, AbsTrackRep *&rep){

   //~ TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   //~ TVector3 bmCenter = geoTrafo->GetBMCenter();

  //~ TVector3 init_mom(0.,0.,p_bmcon->GetBMmom());//initial momentum for tracking, for BM porpouse the primary track should always have these inital values
  //~ TVector3 init_pos(0.,0.,bmCenter.z()-p_bmgeo->GetLength()/2. -3.);
  
  //~ rep = new RKTrackRep(2212);//protons for the moment  
  //~ fitTrack = new Track(rep, init_pos, init_mom);
  //~ //charge hits
  //~ Double_t wire_a_x=-1000., wire_a_y=-1000., rdrift_a_x, rdrift_a_y;
  //~ ChargeHits4GenfitTrack(singlehittrack, firedUview, firedVview, hit_res, hitCov, hitCoords, fitTrack,wire_a_x, wire_a_y, rdrift_a_x, rdrift_a_y);
  //~ fitTrack->checkConsistency();
  //~ Int_t fit_index=0;
  //~ do{
    //~ fitTrack->deleteFitterInfo();
    //~ SetInitPos(init_pos, fit_index, wire_a_x, rdrift_a_x, wire_a_y, rdrift_a_y, bmCenter.z()-p_bmgeo->GetLength()/2. -3.);
    //~ fitTrack->setStateSeed(init_pos, init_mom);
    //~ switch(p_bmcon->GetFitterIndex()){
      //~ case 1:
        //~ simpleFitter->processTrack(fitTrack);
        //~ break;
      //~ case 2:
        //~ refFitter->processTrack(fitTrack);
        //~ break;
      //~ case 3:
        //~ dafSimpleFitter->processTrack(fitTrack);
        //~ break;
      //~ case 4:
        //~ dafRefFitter->processTrack(fitTrack);
        //~ break;
    //~ }
    //~ fit_index++;
  //~ }while(!fitTrack->getFitStatus(rep)->isFitConverged() && fit_index<5);
  //~ fitTrack->checkConsistency();
  //~ if(p_bmcon->GetBMdebug()>10)
    //~ cout<<"TABMactNtuTrack::TABMactNtuTrack::end of fitting"<<endl;

//~ return;
//~ }


//not used:
void TABMactNtuTrack::UpdateHitsFromTrack(vector<Double_t> &best_mysqrtchi2, TABMntuTrackTr &best_trackTr, vector<Int_t> &singlehittrack){
  
  if(p_bmcon->GetBMdebug()>10)
    cout<<"UpdateHitsFromTrack::begin"<<endl;
  
  Double_t chi2sum=0.;
  best_mysqrtchi2.resize(singlehittrack.size());
  for(Int_t i=0;i<singlehittrack.size();i++){
    p_hit=p_nturaw->Hit(singlehittrack.at(i));
    best_mysqrtchi2.at(i)=p_hit->GetResidual()/p_hit->GetSigma();
    chi2sum+=best_mysqrtchi2.at(i)*best_mysqrtchi2.at(i);
  }
  
  if(p_bmcon->GetBMdebug()>10)
    cout<<"TABMactNtuTrack::UpdateHitsFromTrack::ended;  chi2sum="<<chi2sum/(Double_t) singlehittrack.size()<<"  best_trackTr.mychi2red="<<best_trackTr.GetMyChi2Red()<<endl;
  
  return;
}


Int_t TABMactNtuTrack::EstimateFIRSTTrackPar(vector<Int_t> &singlehittrack, Int_t &firedUview, Int_t &firedVview,TABMntuTrackTr *&tmp_trackTr){
  
  if(p_bmcon->GetBMdebug()>10)
    cout<<"TABMactNtuTrack::EstimateFIRSTTrackPar::begin of EstimateFIRSTTrackPar"<<endl;
  
  //charge hits and vectors
  vector<Double_t> Ycentro_U, Xcentro_V, Zcentro_U, Zcentro_V, R_U, R_V;
  firedUview=0;
  firedVview=0;
  for(Int_t i_h = 0; i_h <singlehittrack.size() ; i_h++) {
    p_hit = p_nturaw->Hit(singlehittrack.at(i_h));

     if(p_hit->View()==0){//here the inversion in p_bmgeo->Z and Ycentro is right
        Ycentro_U.push_back(p_bmgeo->GetWireZ(p_bmgeo->GetSenseId(p_hit->Cell()),p_hit->Plane(),p_hit->View()));
        Zcentro_U.push_back(p_bmgeo->GetWireY(p_bmgeo->GetSenseId(p_hit->Cell()),p_hit->Plane(),p_hit->View()));
      R_U.push_back(p_hit->Dist());
      firedUview++;
      //provv
      //~ cout<<"ho caricato i dati: i_h="<<i_h<<"   Ycentro_U.size()="<<Ycentro_U.size()<<"  Yu="<<Ycentro_U.back()<<"  zcU="<<Zcentro_U.back()<<"   ru="<<R_U.back()<<endl;
    }else{
      Xcentro_V.push_back(p_bmgeo->GetWireZ(p_bmgeo->GetSenseId(p_hit->Cell()),p_hit->Plane(),p_hit->View()));
      Zcentro_V.push_back(p_bmgeo->GetWireX(p_bmgeo->GetSenseId(p_hit->Cell()),p_hit->Plane(),p_hit->View()));
      R_V.push_back(p_hit->Dist());
      firedVview++;
    }
  }//end of charging Xcentro_V etc,  
  
  if(p_bmcon->GetBMdebug()>10)
    cout<<"TABMactNtuTrack::EstimateFIRSTTrackPar:: end of charging Xcentro_V etc."<<endl;
  
  // estimate track line in the U view ( namely y-z plane)  start to find the 4 tangent of the 2 farer hits
  Int_t ii_max=-1, jj_max=-1;
  Double_t dist_max=0., dist_curr;
  TG_STRUCT tan_max;
  for (Int_t ii = 1; ii<firedUview;ii++){
    for(Int_t jj=0;jj<ii;jj++ ){
      dist_curr = sqrt( (Ycentro_U.at(ii)-Ycentro_U.at(jj))*(Ycentro_U.at(ii)-Ycentro_U.at(jj)) + (Zcentro_U.at(ii)-Zcentro_U.at(jj))*(Zcentro_U.at(ii)-Zcentro_U.at(jj)));
      if(p_bmcon->GetBMdebug()>10)
    	  cout <<"ii="<<ii<<" jj="<<jj<<" yc[ii]="<<Ycentro_U.at(ii)<<" zc[ii]="<<Zcentro_U.at(ii)<<" yc[jj]="<<Ycentro_U.at(jj)<<" ,zc[jj]="<<Zcentro_U.at(jj)<<endl;
      if(dist_curr>dist_max) {
        dist_max = dist_curr;
        ii_max = ii;
        jj_max = jj;
        tan_max = Circles2Tangents(Ycentro_U.at(ii), Zcentro_U.at(ii), R_U.at(ii), Ycentro_U.at(jj), Zcentro_U.at(jj), R_U.at(jj));
      }
    }
  }   
  if(p_bmcon->GetBMdebug()>10) { 
    cout<<"TABMactNtuTrack::EstimateFIRSTTrackPar::celle piu' distanti= "<<ii_max<<" "<<jj_max<<endl;
    for(Int_t hh=0;hh<4;hh++)
      cout<<" m= "<<tan_max.mm[hh]<<" q= "<<tan_max.qq[hh]<<endl;
  }
  Double_t min_sum_scarto=10000000., sum_scarto, distanza,p_U(0), q_U(0), p_V(0), q_V(0);   
  Int_t tmp_int;
  for(int hh=0;hh<4;hh++){
    sum_scarto = 0.;
    if(p_bmcon->GetBMdebug()>10)  cout<<"tangente # "<<hh<<endl;
    for (int ii = 0; ii<firedUview;ii++){
      if((ii!=ii_max)&&(ii!=jj_max)){
        distanza = fabs(Zcentro_U.at(ii)-tan_max.mm[hh]*Ycentro_U.at(ii) - tan_max.qq[hh])/sqrt(1. + tan_max.mm[hh]*tan_max.mm[hh]);
        sum_scarto +=(distanza - R_U.at(ii))*(distanza - R_U.at(ii));
        if(p_bmcon->GetBMdebug()>10)
          cout<<"piano="<<ii<<" distanza="<<distanza<<"  R_U="<<R_U.at(ii)<<" distanza - R_U.at(ii)="<<distanza - R_U.at(ii)<<"  sum_scarto="<<sum_scarto<<endl;
      }
    }
    if(sum_scarto<min_sum_scarto){
      min_sum_scarto = sum_scarto;
      p_U = tan_max.mm[hh];
      q_U = tan_max.qq[hh];
      tmp_int=hh;
    }
  }
  
  if(p_bmcon->GetBMdebug()>10)
    cout<<"TABMactNtuTrack::EstimateFIRSTTrackPar:: estimated track line in the U view p_U="<<p_U<<"  q_U="<<q_U<<"  tangente migliore="<<tmp_int<<endl;   
  
  //  estimate track line in the V view ( namely x-z plane)  
  dist_max=0;
  ii_max=-1;
  jj_max=-1;
  /* start to find the 4 tangent of the 2 farest hits */
  for (int ii = 1; ii<firedVview;ii++){
    for(int jj=0;jj<ii;jj++ ){
      dist_curr = sqrt((Xcentro_V.at(ii)-Xcentro_V.at(jj))*(Xcentro_V.at(ii)-Xcentro_V.at(jj)) + (Zcentro_V[ii]-Zcentro_V[jj])*(Zcentro_V[ii]-Zcentro_V[jj]));
      if(p_bmcon->GetBMdebug()>10)  
    	  cout <<"ii="<<ii<<"  ,jj="<<jj<<"  xV[ii]="<<Xcentro_V.at(ii)<<" zV[ii]="<<Zcentro_V[ii]<<"  xV[jj]="<<Xcentro_V.at(jj)<<"  zV[jj]="<<Zcentro_V[jj]<<endl;
      if(dist_curr>dist_max) {
        dist_max = dist_curr;
        ii_max = ii;
        jj_max = jj;
        tan_max = Circles2Tangents(Xcentro_V.at(ii),Zcentro_V[ii], R_V.at(ii), Xcentro_V.at(jj), Zcentro_V[jj], R_V.at(jj));
      }
    }
  }
  if(p_bmcon->GetBMdebug()>10) {
    cout<<"TABMactNtuTrack::EstimateFIRSTTrackPar::celle piu' distanti V view= "<<ii_max<<" "<<jj_max<<endl;
    for(int hh=0;hh<4;hh++)
      cout<<"m="<<tan_max.mm[hh]<<"  q="<<tan_max.qq[hh]<<endl;
  }
  min_sum_scarto = 10000000.;
  for(int hh=0;hh<4;hh++){
    sum_scarto = 0.;
    if(p_bmcon->GetBMdebug()>10)   cout<<"tangente # "<<hh<<endl;
    for (int ii = 0; ii<firedVview;ii++){
      if((ii!=ii_max)&&(ii!=jj_max)){
        distanza = fabs(Zcentro_V[ii]-tan_max.mm[hh]*Xcentro_V.at(ii) - tan_max.qq[hh])/sqrt(1. + tan_max.mm[hh]*tan_max.mm[hh]);
        sum_scarto +=(distanza - R_V.at(ii))*(distanza - R_V.at(ii));
        if(p_bmcon->GetBMdebug()>10) {
          cout<<"piano= "<<ii<<" distanza="<<distanza<<"  R_V="<<R_V.at(ii)<<"  (distanza - R_V.at(ii))="<<(distanza - R_V.at(ii))<<"  sum_scarto="<<sum_scarto<<endl;
        }
      }
    }
    if(sum_scarto<min_sum_scarto){
      min_sum_scarto = sum_scarto;
      p_V = tan_max.mm[hh];
      q_V = tan_max.qq[hh];
      tmp_int=hh;
    }
  }  
  
  if(p_bmcon->GetBMdebug()>10)
    cout<<"TABMactNtuTrack::EstimateFIRSTTrackPar:: estimated track line in the V view, p_V="<<p_V<<"  q_V="<<q_V<<"  tangente migliore="<<tmp_int<<endl;  

  tmp_trackTr->SetNhit(singlehittrack.size());
  TVector3 tmp_tvector3(p_V,p_U,1.);
  tmp_tvector3.SetMag(1.);
  tmp_trackTr->SetPvers(tmp_tvector3);
  tmp_trackTr->SetR0(q_V,q_U,0.);
  if(p_bmcon->GetBMdebug()>10)
    cout<<"R0=("<<tmp_trackTr->GetR0().X()<<", "<<tmp_trackTr->GetR0().Y()<<", "<<tmp_trackTr->GetR0().Z()<<")   Pvers=("<<tmp_trackTr->GetPvers().X()<<", "<<tmp_trackTr->GetPvers().Y()<<", "<<tmp_trackTr->GetPvers().Z()<<")"<<endl;

  //calculate the residual of all the points (aka ComputeDataAll)
  ComputeDataAll(tmp_trackTr, singlehittrack);
  
  if(p_bmcon->GetBMdebug()>10)
    cout<<"TABMactNtuTrack::EstimateFIRSTTrackPar::ended"<<endl;
  
  return 0;
}


//from https://en.wikipedia.org/wiki/Tangent_lines_to_circles
//just an alternative method to calculate 1 external tangent line, just to check
void TABMactNtuTrack::NewCircles2Tan(Double_t xc1, Double_t yc1, Double_t r1, Double_t xc2, Double_t yc2, Double_t r2){
  
  if(r2<r1){
    swap(r2,r1);
    swap(xc2,xc1);
    swap(yc2,yc1);
    }
  
  Double_t gamma= -atan((yc2-yc1)/(xc2-xc1));   
  Double_t beta= asin((r2-r1)/sqrt((xc2-xc1)*(xc2-xc1)+(yc2-yc1)*(yc2-yc1)));
  Double_t alfa= gamma-beta;
  Double_t xn=xc1+r1*cos(PI/2.-alfa);
  Double_t yn=yc1+r1*sin(PI/2.-alfa);
  Double_t xz=xc2+r2*cos(PI/2.-alfa);
  Double_t yz=yc2+r2*sin(PI/2.-alfa);
  cout<<"xc1="<<xc1<<"  yc1="<<yc1<<" r1="<<r1<<"  xc2="<<xc2<<"  yc2="<<yc2<<"  r2="<<r2<<endl;
  cout<<"m="<<(yz-yn)/(xz-xn)<<"  q="<<yz-(yz-yn)/(xz-xn)*xz<<endl;
 
 return; 
}


//Given 2 circles in the x,y plane return the 4 tangents
//da ottimizzare e controllare!
TG_STRUCT TABMactNtuTrack::Circles2Tangents(Double_t xc1, Double_t yc1, Double_t r1, Double_t xc2, Double_t yc2, Double_t r2){
  TG_STRUCT tan_par;
  for (int ii=0;ii<4;ii++){ /* clean up */
    tan_par.mm[ii] =0. ;
    tan_par.qq[ii] =0. ;
  }
  /*    cout<<"sono in Circles2Tangets"<<endl;
  cout<<"xc1= "<<xc1<<" yc1= "<<yc1<<" r1= "<<r1<<"  xc2= "<<xc2<<" yc1= "<<
  yc2<<" r2= "<<r2<<endl; */

  Double_t aa,bb,cc, dete, qpiu1, qmeno1, qpiu2, qmeno2;
  cc = (r1*r1 -2*r1*r2 +r2*r2 - yc2*yc2 + 2*yc1*yc2 -yc1*yc1);
  bb = 2*(yc1-yc2)*(xc1-xc2);
  aa = (r1*r1 -2*r1*r2 +r2*r2 -xc1*xc1 +2*xc1*xc2 - xc2*xc2);
  dete = bb*bb -4*aa*cc;
  if(dete>0) {
    tan_par.mm[0] = (-bb + sqrt(dete))/2/aa;
    tan_par.mm[1] = (-bb - sqrt(dete))/2/aa;
  }
  cc = (r1*r1 +2*r1*r2 +r2*r2 - yc2*yc2 + 2*yc1*yc2 -yc1*yc1);
  bb = 2*(yc1-yc2)*(xc1-xc2);
  aa = (r1*r1 +2*r1*r2 +r2*r2 -xc1*xc1 +2*xc1*xc2 - xc2*xc2);
  dete = bb*bb -4*aa*cc;
  
  if(dete>0) {
    tan_par.mm[2] = (-bb + sqrt(dete))/2/aa;
    tan_par.mm[3] = (-bb - sqrt(dete))/2/aa;
  }
  
  for(int hh=0;hh<4;hh++){
    qpiu1 = yc1 -tan_par.mm[hh]*xc1 + r1*sqrt(1+ tan_par.mm[hh]*tan_par.mm[hh]);
    qmeno1 = yc1 -tan_par.mm[hh]*xc1 - r1*sqrt(1+ tan_par.mm[hh]*tan_par.mm[hh]);
    qpiu2 = yc2 -tan_par.mm[hh]*xc2 + r2*sqrt(1+ tan_par.mm[hh]*tan_par.mm[hh]);
    qmeno2 = yc2 -tan_par.mm[hh]*xc2 - r2*sqrt(1+ tan_par.mm[hh]*tan_par.mm[hh]);

    if( (fabs(qpiu1-qpiu2)<.001) || (fabs(qpiu1-qmeno2)<0.001) ){ 
      tan_par.qq[hh] = qpiu1; 
    }
    else if( (fabs(qmeno1-qmeno2)<0.001)||(fabs(qmeno1-qpiu2)<0.001)){ 
      tan_par.qq[hh] = qmeno1; 
    }
  }
  
  return tan_par;
}


//dopo estimatetrackpar, in tmp_trackTr sono salvati r0 e pvers iniziali
void TABMactNtuTrack::NewChi2MyFit(vector<Int_t> &singlehittrack, vector<vector<Int_t>> &prunedhit, TABMntuTrackTr *&tmp_trackTr, Bool_t converged){



  
}


//https://root.cern.ch/root/html600/tutorials/fit/NumericalMinimization.C.html
void TABMactNtuTrack::NumericalMinimization(vector<Int_t> &singlehittrack, TABMntuTrackTr *&tmp_trackTr, vector<Double_t> &newpar){
  
  ROOT::Math::Minimizer* min = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
   min->SetMaxFunctionCalls(1000000);
   min->SetMaxIterations(100000);
   min->SetTolerance(0.001);
   min->SetPrintLevel(0);
   
   Int_t parnum=singlehittrack.size()+4;
   
   //~ ROOT::Math::Functor f(&EvaluateChi2,parnum); 
   Double_t step[4] = {p_bmcon->GetParMove(),p_bmcon->GetParMove(),p_bmcon->GetParMove(),p_bmcon->GetParMove()};
   Double_t variable[4]; 
   variable[0]=tmp_trackTr->GetPvers().X()/tmp_trackTr->GetPvers().Z();
   variable[1]=tmp_trackTr->GetR0().X();
   variable[2]=tmp_trackTr->GetPvers().Y()/tmp_trackTr->GetPvers().Z();
   variable[3]=tmp_trackTr->GetR0().Y();
 
   //~ min->SetFunction(f);
 
   // Set the free variables to be minimized!
   min->SetVariable(0,"mx",variable[0], step[0]);
   min->SetVariable(1,"qx",variable[1], step[1]);
   min->SetVariable(2,"my",variable[2], step[2]);
   min->SetVariable(3,"qy",variable[3], step[3]);
   for(Int_t i=0;i<singlehittrack.size();i++)
     min->SetFixedVariable(i+4,"fixpar",singlehittrack.at(i));
 
   min->Minimize(); 
   
   newpar.clear();
   for(Int_t i=0;i<4;i++)
     newpar.push_back(min->X()[i]);
 
   return ;
}





void TABMactNtuTrack::Chi2Fit(vector<Int_t> &singlehittrack, vector<vector<Int_t>> &prunedhit, TABMntuTrackTr *&tmp_trackTr, Bool_t converged){
  
  TABMntuTrackTr *tmp_atrackTr=new TABMntuTrackTr();    
  TABMntuTrackTr *tmp_btrackTr=new TABMntuTrackTr();  
  TMatrixD AA, VV, VbV, AbA;
  TVectorD alpha, Dy, Eta, alphb, Dby;

  alpha.ResizeTo(4);
  alphb.ResizeTo(4);
  Eta.ResizeTo(4);
  Eta.Zero();
    
  Dy.ResizeTo(tmp_trackTr->GetNhit());
  Dby.ResizeTo(tmp_trackTr->GetNhit());
  alpha(0) = tmp_trackTr->GetR0().X();
  alpha(1) = tmp_trackTr->GetR0().Y();
  alpha(2) = tmp_trackTr->GetPvers().X();
  alpha(3) = tmp_trackTr->GetPvers().Y();
  alphb=alpha;
  AA.ResizeTo(tmp_trackTr->GetNhit(),4);
  AbA.ResizeTo(tmp_trackTr->GetNhit(),4);
  VV.ResizeTo(tmp_trackTr->GetNhit(),tmp_trackTr->GetNhit());
  VbV.ResizeTo(tmp_trackTr->GetNhit(),tmp_trackTr->GetNhit());
  
  if(p_bmcon->GetBMdebug()>10)
    cout<<"TABMactNtuTrack::Chi2Fit::start with computeDy and VV"<<endl;
  
  //compute Dy and VV:
  ComputeDy(singlehittrack, tmp_trackTr, Dy);
  ComputeVV(singlehittrack, tmp_trackTr, VV);
  VbV=VV;
  Dby=Dy;
  if(p_bmcon->GetBMdebug()>10)
    cout<<"TABMactNtuTrack::Chi2Fit::computechiqua"<<endl;
  //~ cout<<"Before iteration matrices: Dy, AA, alpha, VV, Eta: "<<endl;
  //~ Dy.Print(); AA.Print(); alpha.Print(); VV.Print(); Eta.Print();
  Double_t old_chi2;
  *tmp_atrackTr=*tmp_trackTr;
  //~ *tmp_btrackTr=*tmp_trackTr;
  Int_t worst_hit=-1;
  for (Int_t hh=0; hh<p_bmcon->GetNumIte();hh++){
     if(p_bmcon->GetBMdebug()>2)
    Info("Action()"," Iteration number %d\n",hh);
    old_chi2=tmp_atrackTr->GetMyChi2Red();
    ComputeDy(singlehittrack, tmp_atrackTr, Dy);
    ComputeAA(singlehittrack, tmp_atrackTr, alpha, AA, true);
    if(Mini(tmp_atrackTr->GetNhit(), AA, VV, Dy, Eta)){
      Update(tmp_atrackTr, singlehittrack, Dy, alpha, Eta, VV, worst_hit); 
      if(converged==false)
        converged=true;
      if(p_bmcon->GetBMdebug()>2)
        cout<<"TABMactNtuTrack::Chi2Fit::iterazione numero="<<hh<<"  chi2red="<<tmp_atrackTr->GetMyChi2Red()<<"  old_chi2="<<old_chi2<<endl;
      if(tmp_atrackTr->GetMyChi2Red()>old_chi2){
        tmp_atrackTr->SetNite(hh);
        break;
      }
    }else{
      if(p_bmcon->GetBMdebug()>0)
      cout<<"TABMactNtuTrack::Chi2Fit:: Mini not possible"<<hh<<endl;
      break;
    //~ Info("Action()","A:: %lf, %lf, %lf %lf\n",alpha(0),alpha(1),alpha(2),alpha(3));
    }
  }
  if(tmp_atrackTr->GetNite()==0)
    tmp_atrackTr->SetNite(p_bmcon->GetNumIte());
  
  //~ Int_t worst_bhit=-1;
  //~ for (Int_t hh=0; hh<p_bmcon->GetNumIte();hh++){
    //~ Info("Action()"," Iteration number %d\n",hh);
    //~ old_chi2=tmp_btrackTr->GetMyChi2Red();
    //~ ComputeDy(singlehittrack, tmp_btrackTr, Dby);
    //~ ComputeAA(singlehittrack, tmp_btrackTr, alphb, AbA, false);
    //~ Eta.Zero();
    //~ if(Mini(tmp_btrackTr->GetNhit(), AbA, VbV, Dby, Eta)){
      //~ Update(tmp_btrackTr, singlehittrack, Dby, alphb, Eta, VbV, worst_bhit); 
      //~ if(converged==false)
        //~ converged=true;
      //~ if(p_bmcon->GetBMdebug()>2)
        //~ cout<<"TABMactNtuTrack::Chi2Fit::iterazione numero="<<hh<<"  chi2red="<<tmp_btrackTr->GetMyChi2Red()<<"  old_chi2="<<old_chi2<<endl;
      //~ if(tmp_btrackTr->GetMyChi2Red()>old_chi2){
        //~ tmp_btrackTr->SetNite(-hh);
	//~ break;
      //~ }
    //~ }else if(p_bmcon->GetBMdebug()>0)
      //~ cout<<"TABMactNtuTrack::Chi2Fit:: Mini not possible"<<hh<<endl;
   if(p_bmcon->GetBMdebug()>0)
    Info("Action()","A:: %lf, %lf, %lf %lf\n",alpha(0),alpha(1),alpha(2),alpha(3));
  //~ }
  //~ if(tmp_btrackTr->GetNite()==0)
    //~ tmp_btrackTr->SetNite(-p_bmcon->GetNumIte());
  
  //~ if(tmp_atrackTr->GetMyChi2Red()<tmp_btrackTr->GetMyChi2Red()){
    *tmp_trackTr=*tmp_atrackTr;
    ComputeDataAll(tmp_trackTr, singlehittrack);
  //~ }else{
    //~ *tmp_trackTr=*tmp_btrackTr;
    //~ worst_hit=worst_bhit;
    //~ }
    
    
  //charge prunedhit
  prunedhit.at(0).clear();
  if(tmp_trackTr->GetMyChi2Red() > p_bmcon->GetChi2Redcut() && worst_hit>-1)
    prunedhit.at(0).push_back(worst_hit);  

  delete tmp_atrackTr;
  //~ delete tmp_btrackTr;

  if(p_bmcon->GetBMdebug()>10)
    cout<<"Chi2Fit::end"<<endl;  
  
  return;
}


void TABMactNtuTrack::ComputeDy(vector<Int_t> &singlehittrack, TABMntuTrackTr *&tmp_trackTr, TVectorD &Dy)
{
  Dy.ResizeTo(tmp_trackTr->GetNhit());
  for (Int_t ifilo=0;ifilo<singlehittrack.size();ifilo++){
    p_hit = p_nturaw->Hit(singlehittrack.at(ifilo));
    Dy(ifilo) =p_hit->GetResidual(); 
  }
  
  //~ cout<<"Printing Dy vector="<<endl;
  //~ Dy.Print();
  return ;
}


void TABMactNtuTrack::ComputeVV(vector<Int_t> &singlehittrack, TABMntuTrackTr *&tmp_trackTr, TMatrixD &VV) {
  
  VV.ResizeTo(tmp_trackTr->GetNhit(),tmp_trackTr->GetNhit());
  VV = 0.;
  for(Int_t ii = 0; ii<singlehittrack.size();ii++){
    p_hit = p_nturaw->Hit(ii);
    VV(ii,ii) =  1./p_hit->GetSigma()/p_hit->GetSigma();
  }
  return;
}

Double_t TABMactNtuTrack::ComputeChiQua(TVectorD &Dy, TMatrixD &VV) {
  TVectorD Aus; 
  Aus.ResizeTo(Dy.GetNrows());
  Aus = VV*Dy;

  return Dy*Aus;
}


//  numerical computation of the Df/Dalpha derivatives
void TABMactNtuTrack::ComputeAA(vector<Int_t> &singlehittrack, TABMntuTrackTr *&tmp_trackTr, TVectorD &alpha, TMatrixD &AA, Bool_t sign){
    
  AA.ResizeTo(tmp_trackTr->GetNhit(),4);
  TVectorD parametri_moved(4);
  Int_t tmp_int;

  for(Int_t ii = 0; ii<4;ii++){
    parametri_moved = alpha;
    if(parametri_moved(ii)!=0.){
      if(sign)
	parametri_moved(ii) *= (1.+p_bmcon->GetParMove());
      else
	parametri_moved(ii) *= (1.-p_bmcon->GetParMove());
    }else
      parametri_moved(ii) = p_bmcon->GetParMove();
    
    if(p_bmcon->GetBMdebug()>10){
      cout<<"ii="<<ii<<"old parameters:"<<endl;
      tmp_trackTr->PrintR0Pvers();
    }
    
    tmp_trackTr->NewSet(parametri_moved); //tmp_trackTr should have new R0 and Pvers 
    
    if(p_bmcon->GetBMdebug()>10){
      cout<<"new parameters:"<<endl;
      tmp_trackTr->PrintR0Pvers();
    }
    //    cout<<"print out of alpha with incremented parameter # "<<ii<<endl;
    //    parametri_moved.Print();
    
    //ComputeDataAll and charge AA
    for(Int_t j=0;j<singlehittrack.size();j++){
      p_hit=p_nturaw->Hit(singlehittrack.at(j));
      AA(j,ii)=(tmp_trackTr->FindRdrift(tmp_trackTr->GetR0(), tmp_trackTr->GetPvers(), p_hit->GetA0(), p_hit->GetWvers()) - p_hit->Dist())/(parametri_moved(ii)-alpha(ii));
      //~ cout<<"j="<<j<<" ii="<<ii<<"  AA="<<AA(j,ii)<<"  dist0="<<p_hit->Dist()<<"  newDist="<<tmp_trackTr->FindRdrift(tmp_trackTr->GetR0(), tmp_trackTr->GetPvers(), p_hit->GetA0(), p_hit->GetWvers())<<endl;
    }
  }

  tmp_trackTr->NewSet(alpha);
  ComputeDataAll(tmp_trackTr, singlehittrack);

  return;
}

void TABMactNtuTrack::ComputeDataAll(TABMntuTrackTr *&tmp_trackTr, vector<Int_t> &singlehittrack){
  Double_t res, chi2red=0.;
  for(Int_t i=0;i<singlehittrack.size();i++){
    p_hit=p_nturaw->Hit(singlehittrack.at(i));
    res=p_hit->Dist()-tmp_trackTr->FindRdrift(tmp_trackTr->GetR0(), tmp_trackTr->GetPvers(), p_hit->GetA0(), p_hit->GetWvers());
    p_hit->SetResidual(res);
    //~ cout<<"res="<<res<<"  hit->View="<<p_hit->View()<<"  plane="<<p_hit->Plane()<<"  cell="<<p_hit->Cell()<<endl;
    p_hit->SetChi2(res*res/p_hit->GetSigma()/p_hit->GetSigma());
    chi2red+=p_hit->GetChi2();
    //~ cout<<"residui che calcolo ="<<p_hit->GetResidual()<<"  dist="<<p_hit->Dist()<<"  newdist="<<tmp_trackTr->FindRdrift(tmp_trackTr->GetR0(), tmp_trackTr->GetPvers(), p_hit->GetA0(), p_hit->GetWvers())<<endl;//provv
  }
  if(singlehittrack.size()>4)
    tmp_trackTr->SetMyChi2Red(chi2red/((Double_t)(singlehittrack.size()-4)));
  else
    tmp_trackTr->SetMyChi2Red(1000.);
  return;  
}

Int_t TABMactNtuTrack::Mini(Int_t nmeas,TMatrixD &AA, TMatrixD &VV, TVectorD &Dy, TVectorD &Eta) {
  TMatrixD B, AAt, VVa;
  VVa.ResizeTo(4,4);
  AAt.ResizeTo(4,nmeas);
  B.ResizeTo(4,nmeas);
  
  AAt.Transpose(AA);
  VVa = AAt*VV*AA;
  //~ cout<<"VVa.Determinant()="<<VVa.Determinant()<<"  VV.Determinant()="<<VV.Determinant()<<endl;
  if(VVa.Determinant()>0.0001 && VVa.IsValid()){
    Eta.ResizeTo(4);
    VVa.Invert();
    B = VVa*AAt*VV;
    Eta = B*Dy;
    return 1;
  }
  
  return 0;
}


void TABMactNtuTrack::Update(TABMntuTrackTr *&tmp_trackTr, vector<Int_t> &singlehittrack, TVectorD Dy, TVectorD& alpha, TVectorD &Eta, TMatrixD &VV, Int_t &worst_hit) {

  //~ TVectorD Aus;
  //~ Aus.ResizeTo(Dy.GetNrows());

  alpha = Eta + alpha;
  if(fabs(alpha(2))>1 || TMath::IsNaN(alpha(2))) alpha(2) = 0;
  if(fabs(alpha(3))>1 || TMath::IsNaN(alpha(3))) alpha(3) = 0;
  tmp_trackTr->NewSet(alpha);
  ComputeDataAll(tmp_trackTr, singlehittrack);
  ComputeDy(singlehittrack, tmp_trackTr, Dy);
  //~ Aus = VV*Dy;
  //~ tmp_trackTr->SetMyChi2Red((Dy*Aus)/(Double_t)(tmp_trackTr->GetNhit()-4.));

  //set the hits chi2 contribution and the worst_hit
  Double_t worst_cont=-1.;
  //provv
  //~ Double_t tmp_double;
  for(int ii=0;ii<singlehittrack.size();ii++){
    p_hit = p_nturaw->Hit(singlehittrack.at(ii));
    //~ p_hit->SetChi2(Dy(ii)*Aus(ii)/((Double_t)(singlehittrack.size()-4.)));
    //~ tmp_double=p_hit->Dist()-tmp_trackTr->FindRdrift(tmp_trackTr->GetR0(), tmp_trackTr->GetPvers(), p_hit->GetA0(), p_hit->GetWvers());
    //provv
    //~ cout<<"hit="<<ii<<"   chi2="<<Dy(ii)*Aus(ii)<<"   realchi2="<<tmp_double*tmp_double/p_hit->GetSigma()/p_hit->GetSigma()<<"  Dy="<<Dy(ii)<<"  Aus="<<Aus(ii)<<endl;
    if(p_hit->GetChi2()>worst_cont){
      worst_cont=p_hit->GetChi2();
      worst_hit=ii;
    }
  }

  return;
}


//set the hits chi2 contribution
void TABMactNtuTrack::SetChi2H(TVectorD &dy, TVectorD &au, vector<Int_t> &singlehittrack) {
  for(int ii=0;ii<singlehittrack.size();ii++){
    p_hit = p_nturaw->Hit(singlehittrack.at(ii));
    p_hit->SetChi2(dy(ii)*au(ii));
  }
  return;
}


//~ (TABMntuRaw *hp, TABMntuTrack* pntr, TABMparGeo* ppg, TABMntuTrackTr &aSrTr, TABMntuTrackTr &RefTr, int add[36], int ndou)
//aSrTr is a new clean track
//function creates in hitxtrack a track candidate with the double hits 
Int_t TABMactNtuTrack::SortFirstDoubleHits(TABMntuTrackTr *&tmp_trackTr, vector< vector<Int_t> > &hitxplane, vector< vector<Int_t> > &hitxtrack){
  
  if(p_bmcon->GetBMdebug()>10)
      cout<<"TABMactNtuTrack::SortFirstDobuleHits start"<<endl;
  
  vector<Int_t> bigtrackhits;
  Double_t mindist, provv_dist;
  Int_t minhit_index;
  for(Int_t i=0; i<hitxplane.size();i++){
    if(hitxplane.at(i).size()==1)
      bigtrackhits.push_back(hitxplane.at(i).at(0));
    else if(hitxplane.at(i).size()>1){
      mindist=10000000.;
      for(Int_t k=0;k<hitxplane.at(i).size();k++){
        p_hit=p_nturaw->Hit(hitxplane.at(i).at(k));
        provv_dist=fabs(p_hit->Dist() - tmp_trackTr->FindRdrift(tmp_trackTr->GetR0(), tmp_trackTr->GetPvers(), p_hit->GetA0(), p_hit->GetWvers()));
        if(provv_dist<mindist){
          mindist=provv_dist;
          minhit_index=k;
        }
      }
      bigtrackhits.push_back(hitxplane.at(i).at(minhit_index));
    }  
  }
  
  hitxtrack.push_back(bigtrackhits);
  
  if(p_bmcon->GetBMdebug()>10){
    cout<<"TABMactNtuTrack::SortFirstDobuleHits end, bigtrackhits=";
    for(Int_t i=0; i<bigtrackhits.size(); i++)
      cout<<bigtrackhits.at(i)<<"  ";
    cout<<endl;  
  }
  
  return 0;
  
}

//check for a given hitxtrack.at(i) if it pass the plane cut selection criteria
//~ Bool_t TABMactNtuTrack::PlaneCounter(vector<Int_t> &hitxtrack_vec, TABMparCon *p_bmcon){
  
  //~ for(Int_t j = 0; j < hitxtrack.size(); j++) {  
    //~ if(hitxplane.at(j).size()!=0)
      //~ tracknum*=hitxplane.at(j).size();
    //~ else{
      //~ firedPlane--;
      //~ if(j%2==0)
        //~ firedUview--;
      //~ else
        //~ firedVview--;   
    //~ }
  //~ }  
  
  //~ if(firedUview)
  
  //~ return kFALSE;
//~ }








/*!
  \class TABMntuTrackTr TABMntuTrack.hxx "TABMntuTrack.hxx"
  \brief Beam Monitor track description - track object **
*/

#include "TABMntuTrackTr.hxx"

using namespace genfit;

ClassImp(TABMntuTrackTr);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMntuTrackTr::TABMntuTrackTr():
  //~ nwire(0), nass(0),
    //~ x0(0.), y0(0.), ux(0.), uy(0.),
    nhit(0), chi2(999.), chi2Red(999),
    mychi2(999), mychi2Red(999), ndf(0), failedPoint(0), 
    isConverged(0), MaxRdriftErr(100), AngZ(100), AngZRes(100),
    AngZResAv(100), AngPhi(100), AngPhiRes(100),prefit_status(-10)
    
{
  target_pos.SetXYZ(-100.,-100.,-100.);
  mylar1_pos.SetXYZ(-100.,-100.,-100.);
  mylar2_pos.SetXYZ(-100.,-100.,-100.);
  Pvers.SetXYZ(-100.,-100.,-100.);
  R0.SetXYZ(-100.,-100.,-100.);
  nite=0;
 }

//------------------------------------------+-----------------------------------
//! Destructor.

TABMntuTrackTr::~TABMntuTrackTr(){}

/*-----------------------------------------------------------------*/
//**************************OLD AND NEW TRACKING***************************
/*-----------------------------------------------------------------*/

void TABMntuTrackTr::Dump() const
{ 
  cout<<endl<<"------------ Dump Track Class ---------"<<endl;
  //~ cout<<"nwire= "<<nwire<<" x0= "<<x0<<" y0= "<<y0<<
    //~ " z0= "<<z0<<" Pvers= "<<ux<<" "<<uy<<" "<<uz<<endl;
  cout<<"new tracking: nhit="<<nhit<<"  chi2="<<chi2<<endl;
  cout<<"ndf="<<ndf<<"   failedPoint="<<failedPoint<<endl;
}

/*-----------------------------------------------------------------*/

void TABMntuTrackTr::Clean()
{
  /*  
      reset the Track values to default 
  */

  //new tracking
  nhit=0;
  chi2=999.;
  chi2Red=999;
  mychi2=999;
  mychi2Red=999;
  ndf=0;
  failedPoint=0;
  isConverged=0;
  MaxRdriftErr=100;
  AngZ=100;
  AngZRes=100;
  AngZResAv=100;
  AngPhi=100;
  AngPhiRes=100;
  target_pos.SetXYZ(-100.,-100.,-100.);
  mylar1_pos.SetXYZ(-100.,-100.,-100.);
  mylar2_pos.SetXYZ(-100.,-100.,-100.);
  Pvers.SetXYZ(-100.,-100.,-100.);
  R0.SetXYZ(-100.,-100.,-100.);
  prefit_status=-10;
  nite=0;
}

TABMntuTrackTr::TABMntuTrackTr(const TABMntuTrackTr &tr_in){
  nhit=tr_in.nhit;
  ndf=tr_in.ndf;
  failedPoint=tr_in.failedPoint;
  chi2=tr_in.chi2;
  chi2Red=tr_in.chi2Red;
  mychi2=tr_in.mychi2;
  mychi2Red=tr_in.mychi2Red;
  isConverged=tr_in.isConverged;
  MaxRdriftErr=tr_in.MaxRdriftErr;
  AngZ=tr_in.AngZ;
  AngZRes=tr_in.AngZRes;
  AngZResAv=tr_in.AngZResAv;
  AngPhi=tr_in.AngPhi;
  AngPhiRes=tr_in.AngPhiRes;
  target_pos=tr_in.target_pos;
  mylar1_pos=tr_in.mylar1_pos;
  mylar2_pos=tr_in.mylar2_pos;
  prefit_status=tr_in.prefit_status;
  R0=tr_in.R0;
  Pvers=tr_in.Pvers;
  nite=tr_in.nite;
}

TVector3 TABMntuTrackTr::PointAtLocalZ(double zloc)
{
   TVector3 projected(Pvers.X()/Pvers.Z()*zloc+R0.X() ,Pvers.Y()/Pvers.Z()*zloc+R0.Y(), zloc);
   
   return projected;
}

//~ TABMntuTrackTr* TABMntuTrack::NewTrack(TABMntuTrackTr& trk)
//~ {
   //~ TClonesArray &trackArray = *t;
   //~ TABMntuTrackTr* track = new(trackArray[trackArray.GetEntriesFast()]) TABMntuTrackTr(trk);
   //~ return track;
//~ }

//********************************* NEW TRACKING  **********************************

  //~ TABMntuTrackTr& operator=(TABMntuTrackTr const& in){
    //~ if(this!=&in){
      //~ this->nhit=in.nhit;
      //~ this->ndf=in.ndf;
      //~ this->failedPoint=in.failedPoint;
      //~ this->chi2=in.chi2;
      //~ this->chi2Red=in.chi2Red;
      //~ this->mychi2=in.mychi2;
      //~ this->mychi2Red=in.mychi2Red;
      //~ this->isConverged=in.isConverged;
      //~ this->MaxRdriftErr=in.MaxRdriftErr;
      //~ this->AngZ=in.AngZ;
      //~ this->AngZRes=in.AngZRes;
      //~ this->AngZResAv=in.AngZResAv;
      //~ this->AngPhi=in.AngPhi;
      //~ this->AngPhiRes=in.AngPhiRes;
      //~ this->target_pos=in.target_pos;
      //~ this->mylar1_pos=in.mylar1_pos;
      //~ this->mylar2_pos=in.mylar2_pos;
    //~ }
    //~ return *this;
  //~ }


void TABMntuTrackTr::CalculateFitPar(Track* fitTrack, vector<Double_t>& hit_res, vector<Double_t>& hit_mysqrtchi2, vector<vector<Int_t>> &prunedhit, TABMparCon* p_bmcon, TABMparGeo* p_bmgeo, Int_t rejhit, SharedPlanePtr &mylar1_plane, SharedPlanePtr &central_plane, SharedPlanePtr &mylar2_plane, SharedPlanePtr &target_plane){
  Int_t hit_num=fitTrack->getNumPointsWithMeasurement();
  Double_t worst_hit=-1000.;
  if(hit_num!=hit_mysqrtchi2.size())
    cout<<"TABMntuTrack::CalculateFitPar::WARNING:: hit_num!=mychi2red.size()... some hits has been lost!!!!!"<<endl;
  Int_t hit_num_withcov=0;
  Double_t tmp_double, old_rdrift, new_rdrift, rdrift_err_max=0;
  mychi2=0.;
  TVector3 wire_pos, wire_dir;
  TVector3 first_fit_pos(0,0,100), last_fit_pos(0,0,-100);//per calcolo angZ... ma no buono!!!
  vector<Double_t> angZ_vec, angPhi_vec;
  //~ vector<TVector3> state_pos_vec;
  AbsMeasurement* measurement;
  TDecompChol fitTrack_cov;  
  MeasuredStateOnPlane state;
  //~ KalmanFitterInfo* kalmanInfo;
    
  if(p_bmcon->GetBMdebug()>10)
    cout<<"TABMntuTrack::CalculateFitPar, hit_num="<<hit_num<<"  I'm starting loop on hit_num"<<endl;
  if(hit_res.size()!=hit_num){
    cout<<"TABMntuTrack::CalculateFitParWARNING:    hit_res and hit_num is different!!!!!!!!!!!!!!!!!!!!!!!!!   hit_res.size="<<hit_res.size()<<"   hit_num="<<hit_num<<endl;
    return;
    }
  
  //cicle to calculate mychi2 and set state to last state  
  for(Int_t i=0;i<hit_num;i++){
    if(p_bmcon->GetBMdebug()>10)
      cout<<"TABMntuTrack::CalculateFitPar:: hit_num="<<hit_num<<endl;
    if(fitTrack->getPointWithMeasurement(i)->hasFitterInfo(fitTrack->getTrackRep(0))){
      //~ kalmanInfo=fitTrack->getPointWithMeasurement(i)->getKalmanFitterInfo(fitTrack->getTrackRep(0));
      fitTrack_cov=fitTrack->getPointWithMeasurement(i)->getRawMeasurement(0)->getRawHitCov();
      if(fitTrack_cov.Decompose()) {// questo check in più forse è inutile!!!!!!
        state=fitTrack->getFittedState(i);
        measurement=fitTrack->getPointWithMeasurement(i)->getRawMeasurement(0);
        angZ_vec.push_back(state.getMom().Theta()*RAD2DEG);
        angPhi_vec.push_back(state.getMom().Phi()*RAD2DEG);
        //~ state_pos_vec.push_back(state.getPos());  
        old_rdrift=measurement->getRawHitCoords()[6];
        wire_pos.SetXYZ(measurement->getRawHitCoords()[0],measurement->getRawHitCoords()[1],measurement->getRawHitCoords()[2]);
        wire_dir.SetXYZ(measurement->getRawHitCoords()[3],measurement->getRawHitCoords()[4],measurement->getRawHitCoords()[5]);
        wire_dir=wire_dir-wire_pos;
        wire_dir.SetMag(1.);
        new_rdrift=FindRdrift(state.getPos(), state.getMom(), wire_pos, wire_dir);//da check
        hit_mysqrtchi2.at(i)=(old_rdrift-new_rdrift)/hit_res.at(i);
        if(fabs(hit_mysqrtchi2.at(i))>worst_hit)
          worst_hit=fabs(hit_mysqrtchi2.at(i));
        mychi2+=(old_rdrift-new_rdrift)*(old_rdrift-new_rdrift)/hit_res.at(i)/hit_res.at(i);        
        if(fabs(old_rdrift-new_rdrift)>rdrift_err_max)
          rdrift_err_max=fabs(old_rdrift-new_rdrift);
        if(state.getPos().Z()<first_fit_pos.Z())
          first_fit_pos=state.getPos();
        if(state.getPos().Z()>last_fit_pos.Z())
          last_fit_pos=state.getPos();  
        
        if(wire_dir.Mag()>1.1 || wire_dir.Mag()<0.9)
          cout<<"TABMntuTrack::CalculateFitPar::WARNING:    wire_dir have a problem:   wire_dir=("<<wire_dir.X()<<", "<<wire_dir.Y()<<", "<<wire_dir.Z()<<")"<<endl;
        if(p_bmcon->GetBMdebug()>10){
          cout<<"TABMntuTrack::CalculateFitPar:  wire_pos=("<<wire_pos.X()<<", "<<wire_pos.Y()<<", "<<wire_pos.Z()<<")"<<"   wire_dir=("<<wire_dir.X()<<", "<<wire_dir.Y()<<", "<<wire_dir.Z()<<")"<<endl;
          cout<<"old rdrift ="<<old_rdrift<<"  new rdrift="<<new_rdrift<<"  diff="<<fabs(old_rdrift-new_rdrift)<<"  res="<<hit_res.at(i)<<" chi2 contribution="<<(old_rdrift-new_rdrift)*(old_rdrift-new_rdrift)/hit_res.at(i)/hit_res.at(i)<<endl;
          }
                
        hit_num_withcov++;
      }
    }
  }
    
  MaxRdriftErr=rdrift_err_max;
  if(hit_num>4)
    mychi2Red=mychi2/(Double_t)(hit_num-4.);
  else{
    cout<<"TABMntuTrack::CalculateFitPar::WARNING: you have less than 4 hit!! cannot set mychi2Red!!!"<<endl;
    mychi2Red=1000.;
    }
    
  if(p_bmcon->GetBMdebug()>3)
    cout<<"TABMntuTrack::CalculateFitPar:: hit_num="<<hit_num<<"  mychi2="<<mychi2<<"  mychi2Red="<<mychi2Red<<"  ndf="<<fitTrack->getFitStatus()->getNdf()<<endl;
  
  //calculate prunedhit, I refit pruning the worst_hit that have the highest chi2 contribution
  if((rejhit+1)<=p_bmcon->GetRejmaxcut() && mychi2Red>p_bmcon->GetChi2Redcut()){
    prunedhit.clear();
    prunedhit.resize(1);
    for(Int_t i=0;i<hit_mysqrtchi2.size();i++){
      if(fabs(hit_mysqrtchi2.at(i))==worst_hit){
        prunedhit.at(0).push_back(i);
        if(p_bmcon->GetBMdebug()>10)
          cout<<"add prunedhit:  prunedhit.at(0).size()="<<prunedhit.at(0).size()<<"  val="<<prunedhit.at(0).back()<<endl;
      }
    }
    //~ if(prunedhit.at(0).size()==0 || (rejhit+prunedhit.at(0).size())>p_bmcon->GetRejmaxcut()){
      //~ cout<<"TABMntuTrackTr.cxx:: something odd happened... prunedhit.at(0).size()==0"<<endl;
      //~ prunedhit.clear();
    //~ }
  }
  
  //out of cicle state should be the state of last measurement
  //other fitpos parameter
  if(hit_num_withcov>0 && mychi2Red<p_bmcon->GetChi2Redcut()){
    
    //calculate AngZ and AngPhi with momentum:
    if(angZ_vec.size()!=angPhi_vec.size())
      cout<<"TABMntuTrack::CalculateFitPar::ERROR: something is wrong in angZ and angPhi!!!!"<<endl;
    Double_t maxAngZ=-1000., minAngZ=1000., maxAngPhi=-1000., minAngPhi=1000., avAngZ=0., avAngPhi=0., avAngZres=0.;
    for(Int_t i=0;i<angZ_vec.size();i++){
      if(angPhi_vec.at(i)>maxAngPhi)
        maxAngPhi=angPhi_vec.at(i);
      if(angPhi_vec.at(i)<minAngPhi)
        minAngPhi=angPhi_vec.at(i);
      if(angZ_vec.at(i)>maxAngZ)
        maxAngZ=angZ_vec.at(i);
      if(angZ_vec.at(i)<minAngZ)
        minAngZ=angZ_vec.at(i);
      avAngZ+=angZ_vec.at(i);
      avAngPhi+=angPhi_vec.at(i);
      }
    if(angZ_vec.size()!=0){   
      AngZ=avAngZ/angZ_vec.size();
      AngPhi=avAngPhi/angZ_vec.size();      
      AngZRes=maxAngZ-minAngZ;
      AngPhiRes=maxAngPhi-minAngPhi;
      for(Int_t j=0;j<angZ_vec.size();j++)
        avAngZres+=fabs(angZ_vec.at(j)-AngZ);
      AngZResAv=avAngZres/angZ_vec.size();
      }
      
    if(p_bmcon->GetBMdebug()>10)
      cout<<"TABMntuTrack::CalculateFitPar::  angZRes="<<AngZRes<<"  number of measurement="<<angZ_vec.size()<<"  converge?"<<fitTrack->getFitStatus(fitTrack->getTrackRep(0))->isFitConverged()<<"   angZ="<<AngZ<<endl;
      
    //~ if(state_pos_vec.size()!=angZ_vec.size())
      //~ cout<<"TABMntuTrack::CalculateFitPar::ERROR: state_pos_vec.size is different from angZ_vec.size!!!!!!!!!!!!!!!!!!!!!  something is wrong in TABMntuTrack::CalculatefitPar"<<endl;
        
    //extrapolate track on mylar1
    fitTrack->getTrackRep(0)->extrapolateToPlane(state, mylar1_plane);
    mylar1_pos=state.getPos();

    //extrapolate track on central_plane
    fitTrack->getTrackRep(0)->extrapolateToPlane(state, central_plane);
    R0=state.getPos();
    Pvers=state.getMom();
    Pvers.SetMag(1.);
    
    //extrapolate track on mylar2 with state
    fitTrack->getTrackRep(0)->extrapolateToPlane(state, mylar2_plane);
    mylar2_pos=state.getPos();
  
    //extrapolate track on target with state
    fitTrack->getTrackRep(0)->extrapolateToPlane(state, target_plane);
    target_pos=state.getPos();

    }//fine if hit_num_withcov>0 
    
      if(p_bmcon->GetBMdebug()>10)
        cout<<"TABMntuTrack::CalculateFitPar::end of CalculateFitPar"<<endl;  
  return;
}



void TABMntuTrackTr::CalculateFromFirstPar(TABMparCon* p_bmcon, TABMparGeo* p_bmgeo){
  if(p_bmcon->GetBMdebug()>10)
    cout<<"CalculateFromFirstPar::begin"<<endl;

  AngZ=Pvers.Theta()*RAD2DEG;  
  AngPhi=Pvers.Phi()*RAD2DEG;  
  //extrapolate to mylars and target plane:
  
  mylar1_pos=p_bmgeo->ProjectFromPversR0(Pvers, R0, p_bmgeo->GetMylar1().Z());
  mylar2_pos=p_bmgeo->ProjectFromPversR0(Pvers, R0, p_bmgeo->GetMylar2().Z());
  target_pos=p_bmgeo->ProjectFromPversR0(Pvers, R0, p_bmgeo->GetTarget().Z());

  if(p_bmcon->GetBMdebug()>10)
    cout<<"CalculateFromFirstPar::end"<<endl;
  return;
}


Double_t TABMntuTrackTr::FindRdrift(TVector3 pos, TVector3 dir, TVector3 A0, TVector3 Wvers) {

  Double_t tp = 0., tf= 0., rdrift; 
  TVector3 D0, R0, Pvers;

  if(dir.Mag()==0.){
    cout<<"WARNING: FindRdrift: momentum is 0 and the hit shouldn't be charged because this hit is from a fragmentated particle with zero momentum"<<endl;
    return 99;//fake value
    }
    
  R0.SetXYZ(pos.X(),pos.Y(),pos.Z());//set position
  Pvers=dir;
  Pvers.SetMag(1.);
  
  D0 = R0 - A0;//distance between position of reference point of current wire and current particle position

  Double_t prosca = Pvers*Wvers ;//scalar product of directions
  Double_t D0W = D0*Wvers;//distance projected on wire
  Double_t D0P = D0*Pvers;//distance projected on particle direction

  if(prosca!= 1.) {//if the don't fly parallel
    tp = (D0W*prosca - D0P)/(1.-prosca*prosca);
    tf = (-D0P*prosca + D0W)/(1.-prosca*prosca);
    rdrift = sqrt( abs(D0.Mag2() + tp*tp + tf*tf + 2.*tp*D0P -2.*tf*D0W -2.*prosca*tf*tp ));
    } 
  else  //if they go parallel
    rdrift = sqrt(abs( D0.Mag2() - D0W*D0W)); 

  //~ if(rdrift>0.945) //for the fitted tracks it is possible to have a rdrift bigger than the cell size
    //~ cout<<"WARNING!!!!! SOMETHING IS WRONG, YOU HAVE A TOO BIG RDRIFT!!!!!!!!! look at TABMntuTrack::FindRdrift   rdrift="<<rdrift<<endl;
  if(rdrift<0){
    cout<<"WARNING!!!!! SOMETHING IS WRONG, YOU HAVE A NEGATIVE RDRIFT!!!!!!!!! look at TABMntuTrack::FindRdrift    rdrift="<<rdrift<<endl;    
    cout<<"pos=("<<pos.X()<<","<<pos.Y()<<","<<pos.Z()<<")  dir=("<<dir.X()<<","<<dir.Y()<<","<<dir.Z()<<")"<<endl;
    cout<<"A0=("<<A0.X()<<","<<A0.Y()<<","<<A0.Z()<<")  Wvers=("<<Wvers.X()<<","<<Wvers.Y()<<","<<Wvers.Z()<<")"<<endl;
    }
    
  return rdrift;
}

void TABMntuTrackTr::NewSet(TVectorD ftrackpar){
  Double_t transv=ftrackpar[2]*ftrackpar[2]+ftrackpar[3]*ftrackpar[3];
  if(transv<1.1){
    Pvers.SetXYZ(ftrackpar[2], ftrackpar[3], sqrt(1.-transv));
    R0.SetXYZ(ftrackpar[0], ftrackpar[1], 0.);
  }
  else{
    Error("Action()","TABMntuTrackTr::Set(vec)-> ERROR: pvers>1  px=%f py=%f px*px+py*py=%lf ",ftrackpar[2],ftrackpar[3],transv);
    return ;
  }  
  return;
}


void TABMntuTrackTr::PrintR0Pvers(){
  cout<<"R0=("<<R0.X()<<", "<<R0.Y()<<" ,"<<R0.Z()<<")"<<endl;
  cout<<"Pvers=("<<Pvers.X()<<", "<<Pvers.Y()<<" ,"<<Pvers.Z()<<")"<<endl;
  return;
}

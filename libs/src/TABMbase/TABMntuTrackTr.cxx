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
    mychi2(160), mychi2Red(15), ndf(0), failedPoint(0), 
    isConverged(0), MaxRdriftErr(100), AngZ(100), AngZRes(100),
    AngZResAv(100), AngPhi(100), AngPhiRes(100)
    
{

  //~ trackrho.Zero();
  //~ z0 = 0;
  //~ uz = 1.;
  //~ R0.SetXYZ(x0,y0,z0);
  //~ Pvers.SetXYZ(ux,uy,uz);
  //~ trackpar.ResizeTo(NUMPAR);
  //~ trackpar.Zero();
  //~ trackpar(0) = x0;
  //~ trackpar(1) = y0;
  //~ trackpar(2) = ux;
  //~ trackpar(3) = uy;
  
    //~ nhit=0;
  //~ chi2=999.;
  //~ chi2Red=999;
  //~ mychi2=160;
  //~ mychi2Red=15;
  //~ ndf=0;
  //~ failedPoint=0;
  //~ isConverged=0;
  //~ MaxRdriftErr=100;
  //~ AngZ=100;
  //~ AngZRes=100;
  //~ AngZResAv=100;
  //~ AngPhi=100;
  //~ AngPhiRes=100;
  //~ RTarget=100;
  target_pos.SetXYZ(0.,0.,0.);
  mylar1_pos.SetXYZ(0.,0.,0.);
  mylar2_pos.SetXYZ(0.,0.,0.);
  
  //~ MaxRdriftErr=100;
  //~ AngZ=100;
  //~ AngZRes=100;
  //~ AngZResAv=100;
  //~ AngPhi=100;
  //~ AngPhiRes=100;
  //~ RTarget=100;
  //~ chi2Red=999;
  //~ target_pos.SetXYZ(0.,0.,0.);
  //~ mylar1_pos.SetXYZ(0.,0.,0.);
  //~ mylar2_pos.SetXYZ(0.,0.,0.);

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
  //~ trackpar.Zero();
  //~ trackrho.Zero();
  //~ nwire = 0;
  //~ nass = 0;
  //~ x0 = 0.;
  //~ y0 = 0.;
  //~ z0 = 0;
  //~ ux = 0.;
  //~ uy = 0.;
  //~ uz = 1.;
  //~ R0.SetXYZ(x0,y0,z0);
  //~ Pvers.SetXYZ(ux,uy,uz);
  
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
  target_pos.SetXYZ(0.,0.,0.);
  mylar1_pos.SetXYZ(0.,0.,0.);
  mylar2_pos.SetXYZ(0.,0.,0.);
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


void TABMntuTrackTr::CalculateFitPar(Track* fitTrack, vector<Double_t>& hit_res, vector<Double_t>& hit_mychi2, vector<vector<Int_t>> &prunedhit, TABMparCon* p_bmcon, TABMparGeo* p_bmgeo, Int_t rejhit, SharedPlanePtr &mylar1_plane, SharedPlanePtr &mylar2_plane, SharedPlanePtr &target_plane){
  Int_t hit_num=fitTrack->getNumPointsWithMeasurement();
  if(hit_num!=hit_mychi2.size())
    cout<<"TABMntuTrack::CalculateFitPar::WARNING:: hit_num!=mychi2red.size()... some hits has been lost!!!!!"<<endl;
  Int_t hit_num_withcov=0;
  Double_t tmp_double, old_rdrift, new_rdrift, rdrift_err_max=0;
  mychi2=0.;
  TVector3 wire_pos, wire_dir;
  TVector3 first_fit_pos(0,0,100), last_fit_pos(0,0,-100);//per calcolo angZ... ma no buono!!!
  vector<Double_t> angZ_vec, angPhi_vec;
  vector<TVector3> state_pos_vec;
  AbsMeasurement* measurement;
  TDecompChol fitTrack_cov;  
  MeasuredStateOnPlane state, first_state;
  KalmanFitterInfo* kalmanInfo;
    
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
      kalmanInfo=fitTrack->getPointWithMeasurement(i)->getKalmanFitterInfo(fitTrack->getTrackRep(0));
      fitTrack_cov=fitTrack->getPointWithMeasurement(i)->getRawMeasurement(0)->getRawHitCov();
      if(fitTrack_cov.Decompose()) {// questo check in più forse è inutile!!!!!!
        state=fitTrack->getFittedState(i);
        if(hit_num_withcov==0)
          first_state=fitTrack->getFittedState(i);
        measurement=fitTrack->getPointWithMeasurement(i)->getRawMeasurement(0);
        angZ_vec.push_back(state.getMom().Theta()*RAD2DEG);
        angPhi_vec.push_back(state.getMom().Phi()*RAD2DEG);
        state_pos_vec.push_back(state.getPos());  
        old_rdrift=measurement->getRawHitCoords()[6];
        wire_pos.SetXYZ(measurement->getRawHitCoords()[0],measurement->getRawHitCoords()[1],measurement->getRawHitCoords()[2]);
        wire_dir.SetXYZ(measurement->getRawHitCoords()[3],measurement->getRawHitCoords()[4],measurement->getRawHitCoords()[5]);
        wire_dir=wire_dir-wire_pos;
        wire_dir.SetMag(1.);
        new_rdrift=FindRdrift(state.getPos(), state.getMom(), wire_pos, wire_dir);//da check
        hit_mychi2[i]=(old_rdrift-new_rdrift)*(old_rdrift-new_rdrift)/hit_res[i]/hit_res[i];
        mychi2+=(old_rdrift-new_rdrift)*(old_rdrift-new_rdrift)/hit_res[i]/hit_res[i];        
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
          cout<<"old rdrift ="<<old_rdrift<<"  new rdrift="<<new_rdrift<<"  diff="<<fabs(old_rdrift-new_rdrift)<<"  res="<<hit_res[i]<<" chi2 contribution="<<(old_rdrift-new_rdrift)*(old_rdrift-new_rdrift)/hit_res[i]/hit_res[i]<<endl;
          }
                
        hit_num_withcov++;
      }
    }
  }
    
  MaxRdriftErr=rdrift_err_max;
  if(fitTrack->getFitStatus()->getNdf()!=0)
    mychi2Red=mychi2/fitTrack->getFitStatus()->getNdf();
  else
    cout<<"TABMntuTrack::CalculateFitPar::WARNING: you have 0 Ndf!!!!!!! cannot set mychi2Red!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    
  if(p_bmcon->GetBMdebug()>3)
    cout<<"TABMntuTrack::CalculateFitPar:: hit_num="<<hit_num<<"  mychi2="<<mychi2<<"  mychi2Red="<<mychi2Red<<"  ndf="<<fitTrack->getFitStatus()->getNdf()<<endl;
  
  //calculate prunedhit, for the moment I refit when a hit has a chi2red contribution 2 times larger than the average contribution (the number 2 has to be optimized) 
  if((rejhit+1)<=p_bmcon->GetRejmaxcut() && mychi2Red>p_bmcon->GetChi2Redcut()){
    prunedhit.resize(1);
    for(Int_t i=0;i<hit_mychi2.size();i++){
      if(hit_mychi2[i]/(mychi2/hit_num)>2.){
        prunedhit[0].push_back(i);
        if(p_bmcon->GetBMdebug()>10)
          cout<<"add prunedhit:  prunedhit[0].size()="<<prunedhit[0].size()<<"  val="<<prunedhit[0].back()<<endl;
      }
    }
    if(prunedhit[0].size()==0 || (rejhit+prunedhit[0].size())>p_bmcon->GetRejmaxcut())
      prunedhit.clear();
  }
  
  //out of cicle state should be the state of last measurement and first_state should be the state of the first measurement
  //other fitpos parameter
  if(hit_num_withcov>0 && mychi2Red<p_bmcon->GetChi2Redcut()){
    
    //calculate AngZ and AngPhi with momentum:
    if(angZ_vec.size()!=angPhi_vec.size())
      cout<<"TABMntuTrack::CalculateFitPar::ERROR: something is wrong in angZ and angPhi!!!!"<<endl;
    Double_t maxAngZ=-1000., minAngZ=1000., maxAngPhi=-1000., minAngPhi=1000., avAngZ=0., avAngPhi=0., avAngZres=0.;
    for(Int_t i=0;i<angZ_vec.size();i++){
      if(angPhi_vec[i]>maxAngPhi)
        maxAngPhi=angPhi_vec[i];
      if(angPhi_vec[i]<minAngPhi)
        minAngPhi=angPhi_vec[i];
      if(angZ_vec[i]>maxAngZ)
        maxAngZ=angZ_vec[i];
      if(angZ_vec[i]<minAngZ)
        minAngZ=angZ_vec[i];
      avAngZ+=angZ_vec[i];
      avAngPhi+=angPhi_vec[i];
      }
    if(angZ_vec.size()!=0){   
      AngZ=avAngZ/angZ_vec.size();
      AngPhi=avAngPhi/angZ_vec.size();      
      AngZRes=maxAngZ-minAngZ;
      AngPhiRes=maxAngPhi-minAngPhi;
      for(Int_t j=0;j<angZ_vec.size();j++)
        avAngZres+=fabs(angZ_vec[j]-AngZ);
      AngZResAv=avAngZres/angZ_vec.size();
      }
      
    if(p_bmcon->GetBMdebug()>10)
      cout<<"TABMntuTrack::CalculateFitPar::  angZRes="<<AngZRes<<"  number of measurement="<<angZ_vec.size()<<"  converge?"<<fitTrack->getFitStatus(fitTrack->getTrackRep(0))->isFitConverged()<<"   angZ="<<AngZ<<endl;
      
    if(state_pos_vec.size()!=angZ_vec.size())
      cout<<"TABMntuTrack::CalculateFitPar::ERROR: state_pos_vec.size is different from angZ_vec.size!!!!!!!!!!!!!!!!!!!!!  something is wrong in TABMntuTrack::CalculatefitPar"<<endl;
        
    //extrapolate track on mylar1 with first_state
    if(p_bmcon->GetBMdebug()>10)
      cout<<"TABMntuTrack::CalculateFitPar::extrapolate to mylar1_plane"<<endl;
    //~ SharedPlanePtr mylar1_plane(new DetPlane(p_bmgeo->GetMylar1(), Xvers, Yvers));     
    fitTrack->getTrackRep(0)->extrapolateToPlane(first_state, mylar1_plane);
    mylar1_pos=first_state.getPos();
    
    //extrapolate track on mylar2 with state
    if(p_bmcon->GetBMdebug()>10)
      cout<<"TABMntuTrack::CalculateFitPar::extrapolate to mylar2_plane"<<endl;
    //~ SharedPlanePtr mylar2_plane(new DetPlane(p_bmgeo->GetMylar2(), Xvers, Yvers));     
    fitTrack->getTrackRep(0)->extrapolateToPlane(state, mylar2_plane);
    mylar2_pos=state.getPos();
  
    //extrapolate track on target with state
    if(p_bmcon->GetBMdebug()>10)
      cout<<"TABMntuTrack::CalculateFitPar::extrapolate to target"<<endl;
    //~ SharedPlanePtr target_plane(new DetPlane(p_bmgeo->GetTarget(), Xvers, Yvers));     
    fitTrack->getTrackRep(0)->extrapolateToPlane(state, target_plane);
    target_pos=state.getPos();
    
 
    //~ //Calculate RTarget (distance on the surface of target), calculated from the state of the last measurement
    //~ SharedPlanePtr target_plane(new DetPlane(target_o, Xvers, Yvers));
    //~ tmp_double=fitTrack->getTrackRep(0)->extrapolateToPlane(state, target_plane); //now state is the state of track on the surface of target
    //~ if(state.getPos().Z()!=target_o.Z())
      //~ cout<<"WARNING in calculation of RTarget: state on the target has a different Z from target surface!!!!!!!!!"<<endl;
    //~ else 
      //~ RTarget=(state.getPos()-target_o).Mag();
  
    //Calculate RTarget using AngZ (for any state calculate RTarget and then save the mean RTarget)
    //~ Double_t sum=0, RTargetMax=-10, RTargetMin=10, RTarget_hit;
    //~ for(Int_t i=0;i<state_pos_vec.size();i++){
      //~ tmp_double=sqrt(state_pos_vec[i].X()*state_pos_vec[i].X()+state_pos_vec[i].Y()*state_pos_vec[i].Y());
      //~ if(target_o.Z()<state_pos_vec[i].Z())
        //~ cout<<"WARNING: state_pos.Z is bigger than target_o.Z!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!something is wrong in TABMntuTrack::CalculatefitPar"<<endl;
      //~ else{
        //~ RTarget_hit=tmp_double+tan(AngZ*DEG2RAD)*(target_o.Z()-state.getPos().Z());
        //~ sum+=RTarget_hit;
        //~ if(RTargetMax<RTarget_hit)
          //~ RTargetMax=RTarget_hit;
        //~ if(RTargetMin>RTarget_hit)
          //~ RTargetMin=RTarget_hit;  
        //~ }
      //~ }
    //~ if(state_pos_vec.size()>0)  
      //~ RTarget=sum/state_pos_vec.size();
    //~ if(state_pos_vec.size()>1)
      //~ RTargetRes=RTargetMax-RTargetMin;
       
      
    }//fine if hit_num_withcov>0 
    
      if(p_bmcon->GetBMdebug()>10)
      cout<<"TABMntuTrack::CalculateFitPar::end of CalculateFitPar"<<endl;  
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

  if(rdrift<0)
    cout<<"WARNING!!!!! SOMETHING IS WRONG, YOU HAVE A NEGATIVE RDRIFT!!!!!!!!! look at TABMntuTrack::FindRdrift    rdrift="<<rdrift<<endl;
  //~ if(rdrift>0.945) //for the fitted tracks it is possible to have a rdrift bigger than the cell size
    //~ cout<<"WARNING!!!!! SOMETHING IS WRONG, YOU HAVE A TOO BIG RDRIFT!!!!!!!!! look at TABMntuTrack::FindRdrift   rdrift="<<rdrift<<endl;
  if(rdrift<0){
    cout<<"pos=("<<pos.X()<<","<<pos.Y()<<","<<pos.Z()<<")  dir=("<<dir.X()<<","<<dir.Y()<<","<<dir.Z()<<")"<<endl;
    cout<<"A0=("<<A0.X()<<","<<A0.Y()<<","<<A0.Z()<<")  Wvers=("<<Wvers.X()<<","<<Wvers.Y()<<","<<Wvers.Z()<<")"<<endl;
    }
    
  return rdrift;
}


//**********************************OLD TRACKING****************************************
//~ Int_t TABMntuTrackTr::Set(Double_t fx0, Double_t fy0, Double_t fux, 
		 //~ Double_t fuy, Double_t fuz)
//~ {
  //~ Double_t pmodulo = sqrt( fux*fux + fuz*fuz + fuy*fuy);
  //~ ux = fux/pmodulo;
  //~ uy = fuy/pmodulo;
  //~ uz = fuz/pmodulo;
  //~ Pvers.SetXYZ(ux,uy,uz);

  //~ x0 = fx0;
  //~ y0 = fy0;
  //~ z0 = 0;
  //~ R0.SetXYZ(x0,y0,z0);
  //~ trackpar(0) = x0;
  //~ trackpar(1) = y0;
  //~ trackpar(2) = ux;
  //~ trackpar(3) = uy;
  //~ return 1 ;
//~ }

//~ /*-----------------------------------------------------------------*/

//~ Int_t TABMntuTrackTr::Set(Double_t fx0, Double_t fy0, Double_t fux, Double_t fuy)
//~ {
  //~ ux = fux;
  //~ uy = fuy;
  //~ Double_t transv = ux*ux + uy*uy;
  //~ if(transv<1){
    //~ uz = sqrt(1-transv);
  //~ }
  //~ else{
    //~ Error("Action()","TABMntuTrackTr::Set-> ERROR: pvers>1  px*px+py*py= %lf %lf %lf ",ux,uy,transv);
    //~ return -1;
  //~ }
  //~ Pvers.SetXYZ(ux,uy,uz);

  //~ x0 = fx0;
  //~ y0 = fy0;
  //~ z0 = 0;
  //~ R0.SetXYZ(x0,y0,z0);

  //~ trackpar(0) = x0;
  //~ trackpar(1) = y0;
  //~ trackpar(2) = ux;
  //~ trackpar(3) = uy;

  //~ return 1 ;
//~ }


//~ /*-----------------------------------------------------------------*/

//~ Int_t TABMntuTrackTr::Set(TVectorD ftrackpar)
//~ {
  //~ ux = ftrackpar(2);
  //~ uy = ftrackpar(3);
  //~ Double_t transv = ux*ux + uy*uy;
  //~ if(transv<1){
    //~ uz = sqrt(1-transv);
  //~ }
  //~ else{
    //~ Error("Action()","TABMntuTrackTr::Set(vec)-> ERROR: pvers>1  px*px+py*py= %lf %lf %lf ",ux,uy,transv);

    //~ return -1;
  //~ }
  //~ Pvers.SetXYZ(ux,uy,uz);

  //~ x0 = ftrackpar(0);
  //~ y0 = ftrackpar(1);
  //~ z0 = 0;
  //~ R0.SetXYZ(x0,y0,z0);
  //~ trackpar = ftrackpar;
  //~ return 1 ;
//~ }

//~ /*-----------------------------------------------------------------*/

//~ Int_t TABMntuTrackTr::SetR0(Double_t fx0, Double_t fy0)
//~ {
  //~ x0 = fx0;
  //~ y0 = fy0;
  //~ z0 = 0;
  //~ R0.SetXYZ(x0,y0,z0);
  //~ trackpar(0) = x0;
  //~ trackpar(1) = y0;
  //~ return 1 ;
//~ }
//~ /*-----------------------------------------------------------------*/
//~ void  TABMntuTrackTr::GetPvers(Double_t &fux, Double_t &fuy) {
  
  //~ fux = ux;
  //~ fuy = uy;
  //~ return;
//~ }

//~ Int_t TABMntuTrackTr::SetPvers(Double_t fux, Double_t fuy)
//~ {
  //~ ux = fux;
  //~ uy = fuy;
  //~ Double_t transv = fux*fux + fuy*fuy;
  //~ if(transv<1){
    //~ uz = sqrt(1-transv);
  //~ }
  //~ else{
    //~ Error("Action()","TABMntuTrackTr::SetPvers-> ERROR: pvers>1  px*px+py*py= %lf %lf %lf ",fux,fuy,transv);

    //~ return -1;
  //~ }
  //~ Pvers.SetXYZ(ux,uy,uz);
  //~ trackpar(2) = ux;
  //~ trackpar(3) = uy;

  //~ return 1 ;
//~ }

//~ /*-----------------------------------------------------------------*/

//~ Int_t TABMntuTrackTr::SetPvers(Double_t fux, Double_t fuy, Double_t fuz)
//~ {
  //~ Double_t pmodulo = sqrt( fux*fux + fuz*fuz + fuy*fuy);
  //~ ux = fux/pmodulo;
  //~ uy = fuy/pmodulo;
  //~ uz = fuz/pmodulo;
  //~ Pvers.SetXYZ(ux,uy,uz);
  //~ trackpar(2) = ux;
  //~ trackpar(3) = uy;

  //~ return 1 ;
//~ }


//~ void TABMntuTrackTr::Calibrate(TF1* mypol, TF1* mypol2) { //DA FARE CON NUOVE VARIABILI!!!!


  //~ TAGgeoTrafo *fGeoTrafo =  (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
  //~ if (fGeoTrafo == 0x0)
    //~ Error("SetGeoTrafoName", Form("No GeoTrafo action called %s available", TAGgeoTrafo::GetDefaultActName().Data()));

  //~ double tmp_ux, tmp_uy, tmp_uz;
  //~ tmp_ux = ux;
  //~ tmp_uy = uy;
  //~ tmp_uz = uz;

  //~ //Angle from the BM itself
  //~ double angleBM_x =atan(tmp_ux/tmp_uz);
  //~ double angleBM_y =atan(tmp_uy/tmp_uz);

  //~ //Global angles
  //~ TVector3 versBM (ux,uy,uz);
  //~ TVector3 versGlbBM = fGeoTrafo->VecFromBMLocalToGlobal(versBM);

  //~ double angleBM_x_GL = versGlbBM.X()/versGlbBM.Z();
  //~ double angleBM_y_GL = versGlbBM.Y()/versGlbBM.Z();
  

  //~ //Calibration curve (pol5)
  //~ double angleCal_x_GL;
  //~ double angleCal_y_GL;

  //~ string fn = mypol->GetName();

  //~ //  cout<<"Glb Angle used in calib:: "<<angleBM_x_GL<<" "<<angleBM_y_GL<<" "<<endl;

  //~ if(!strcmp(fn.data(),"mypol")) {
    //~ //Data
    //~ if(angleBM_y_GL<-0.003) { 
      //~ angleCal_y_GL = -0.0002;
    //~ } else if(angleBM_y_GL>0.003) {
      //~ angleCal_y_GL = 0.001;
    //~ } else {
      //~ angleCal_y_GL=   mypol2->Eval(angleBM_y_GL);
    //~ }

    //~ if(angleBM_x_GL<-0.003) { 
      //~ angleCal_x_GL = 0.0004;
    //~ } else if(angleBM_x_GL>0.003) {
      //~ angleCal_x_GL = 0.0006;
    //~ } else {
      //~ angleCal_x_GL=   mypol->Eval(angleBM_x_GL); 
    //~ }
  //~ } else {
    //~ //MC
    //~ angleCal_y_GL=   mypol2->Eval(angleBM_y_GL);
    //~ angleCal_x_GL=   mypol->Eval(angleBM_x_GL); 
  //~ }

  //~ //Getting back from Global to Local

  //~ //Calibrated versors
  //~ double new_ux_GL = tan(angleCal_x_GL)*tmp_uz;
  //~ double new_uy_GL = tan(angleCal_y_GL)*tmp_uz;

  //~ //Renormalize
  //~ double modulo = sqrt(pow(new_ux_GL,2)+pow(new_uy_GL,2)+pow(tmp_uz,2));

  //~ tmp_ux = new_ux_GL/modulo;
  //~ tmp_uy = new_uy_GL/modulo;
  //~ tmp_uz = tmp_uz/modulo;   

  //~ //  cout<<"GLB angle:: "<<tmp_ux<<" "<<tmp_uy<<" "<<tmp_uz<<" "<<endl;

  //~ //Global angles
  //~ TVector3 versCalBM (tmp_ux,tmp_uy,tmp_uz);
  //~ TVector3 versCalBM_Local = fGeoTrafo->VecFromGlobalToBMLocal(versCalBM);

  //~ //  cout<<"Local angle:: "<<versCalBM_Local.X()<<" "<<versCalBM_Local.Y()<<" "<<versCalBM_Local.Z()<<" "<<endl;

  //~ ux = versCalBM_Local.X();
  //~ uy = versCalBM_Local.Y();
  //~ uz = versCalBM_Local.Z();

  //~ Pvers.SetXYZ(versCalBM_Local.X(),versCalBM_Local.Y(),versCalBM_Local.Z());
  //~ trackpar(2) = versCalBM_Local.X();
  //~ trackpar(3) = versCalBM_Local.Y();

  //~ return;

//~ }



//~ TVector3 TABMntuTrackTr::PointAtLocalZ(double zloc) {
  
  //~ TVector3 loc_poi;

  //~ double x_cen = GetX0();
  //~ double y_cen = GetY0();
  //~ double pversx = GetUx();
  //~ double pversy = GetUy();

  //~ double ptra2 = pversx*pversx + pversy*pversy;
  //~ if(ptra2>1.){
    //~ ptra2 = 1.;
    //~ cout <<"warning : wrong output from the fit"<<endl;
  //~ }
  //~ double pversz = sqrt(1. - ptra2);
  //~ /* 
     //~ use parametric equation for track : 
     //~ tf = parameter at specified z in BM ref frame
  //~ */
  //~ double t_i = (zloc)/pversz;
  //~ //  cout<<" "<<angleBM<<" "<<angleVtx_x<<" "<<facscalex<<" "<<endl;
  //~ double xch_i = x_cen + pversx*t_i ;
  //~ double ych_i = y_cen + pversy*t_i ;

  //~ loc_poi.SetXYZ(xch_i,ych_i,zloc);
  
  //~ return loc_poi;

//~ }

//~ /*-----------------------------------------------------------------*/

//~ Int_t TABMntuTrackTr::AddHit(TABMntuHit *h, TABMparGeo *f_bmgeo) {
  
  //~ h->SetAW(f_bmgeo);
  //~ h->SetTrkAss(1);
  //~ nwire ++;
  //~ trackrho.ResizeTo(nwire);

  //~ return 1;
//~ }

//~ /*-----------------------------------------------------------------*/

//~ Int_t TABMntuTrackTr::ComputeDataWire(TABMntuHit *wr, Int_t fwire){

  //~ if(fwire>=nwire){return -1;}
  //~ Double_t tp = 0., tf= 0.; 
  //~ Double_t prosca, D0W, D0P;
  //~ TVector3 D0;
  
  //~ D0 = R0 - wr->GetA0();    

  //~ prosca = Pvers*wr->GetWvers() ;

  //~ D0W = D0*wr->GetWvers();
  //~ D0P = D0*Pvers;
  //~ if(prosca!= 1.) {
    //~ tp = (D0W*prosca - D0P)/(1-prosca*prosca);
    //~ tf = (-D0P*prosca + D0W)/(1-prosca*prosca);
    //~ wr->SetRho(sqrt( D0.Mag2() + tp*tp + tf*tf + 2*tp*D0P -2*tf*D0W 
		     //~ -2*prosca*tf*tp));
    //~ wr->SetPca(R0 + tp*Pvers);
    //~ //    wr->SetPcaWire(wr->GetA0() + tf*wr->GetWvers());
    //~ trackrho(fwire) = wr->GetRho();
  //~ }
  //~ else{
    //~ wr->SetRho(sqrt(D0.Mag2() - D0W*D0W));
    //~ trackrho(fwire) = wr->GetRho();
  //~ }

  //~ return 1 ;

//~ }

//~ /*-----------------------------------------------------------------*/

//~ Int_t TABMntuTrackTr::EstimateTrackPar(TABMntuRaw *hitp, TABMparGeo *f_bmgeo)
//~ {
  //~ Int_t nwire_U=0, nwire_V=0;
  //~ Int_t piano, cella, idwire;
  //~ Double_t raggio=0.;
  //~ Double_t Ycentro_U[MAXNWIRE_VIEW],Zcentro_U[MAXNWIRE_VIEW],R_U[MAXNWIRE_VIEW];
  //~ Double_t Xcentro_V[MAXNWIRE_VIEW],Zcentro_V[MAXNWIRE_VIEW],R_V[MAXNWIRE_VIEW];
  //~ double min_sum_scarto, sum_scarto, scarto, distanza, dist_max, dist_curr;
  //~ Int_t ii_max, jj_max;
  //~ double p_U(0), q_U(0), p_V(0), q_V(0);
  //~ bool dbgflg = false;
  
  //~ TG_STRUCT tan_max;
  //~ int mynwire = hitp->nhit;
  //~ for (int ifilo=0;ifilo<mynwire;ifilo++){
    //~ TABMntuHit *wr = hitp->Hit(ifilo);
    //~ if(!wr->TrkAss()) continue;

    //~ piano = wr->Plane();
    //~ cella = wr->Cell();
    //~ raggio = wr->Dist();

    //~ idwire = f_bmgeo->GetID(cella);
    
    //~ if(wr->View()>0){    /*   U view */
      //~ nwire_U++;
      //~ Ycentro_U[nwire_U-1]= f_bmgeo->GetY(idwire,piano,0); //trackgeo.U_y[idwire][piano];
      //~ Zcentro_U[nwire_U-1]= f_bmgeo->GetZ(idwire,piano,0); //trackgeo.U_z[idwire][piano];
      //~ R_U[nwire_U-1]= raggio;
    //~ }
    //~ else{    /*   V view */
      //~ nwire_V++;
      //~ Xcentro_V[nwire_V-1]= f_bmgeo->GetX(idwire,piano,1); //trackgeo.V_x[idwire][piano];
      //~ Zcentro_V[nwire_V-1]= f_bmgeo->GetZ(idwire,piano,1); //trackgeo.V_z[idwire][piano];
      //~ R_V[nwire_V-1]= raggio;
    //~ }
  //~ }
  //~ if( (nwire_U<=2) || (nwire_V<=2) ){
    //~ return -1; /* not enough wires fired to estimate track parameters */
  //~ }
  
  //~ /*  estimate track line in the U view ( namely y-z plane) */

  //~ /* start to find the 4 tangent of the 2 farer hits */
  //~ dist_max=0; 
  //~ ii_max=-1;
  //~ jj_max=-1;
  //~ for (int ii = 1; ii<nwire_U;ii++){
    //~ for(int jj=0;jj<ii;jj++ ){
      //~ dist_curr = sqrt( pow((Ycentro_U[ii]-Ycentro_U[jj]),2.) + 
			//~ pow((Zcentro_U[ii]-Zcentro_U[jj]),2.0) );
      //~ if(dbgflg)  {
	//~ cout <<"ii,jj= "<<ii<<" "<<jj<<" yc[ii],zc[ii]= "<<Ycentro_U[ii]<<" "<<
	  //~ Zcentro_U[ii]<<" yc[jj],zc[jj]= "<<Ycentro_U[jj]<<" "<<Zcentro_U[jj]<<endl;
      //~ }
      //~ if(dist_curr>dist_max) {
	//~ dist_max = dist_curr;
	//~ ii_max = ii;
	//~ jj_max = jj;
	//~ tan_max = Circles2Tangents(Ycentro_U[ii], Zcentro_U[ii], R_U[ii],
				   //~ Ycentro_U[jj], Zcentro_U[jj], R_U[jj]);
      //~ }
    //~ }
  //~ }
  //~ if(dbgflg) { 
    //~ cout<<"celle piu' distanti= "<<ii_max<<" "<<jj_max<<endl;
    //~ for(int hh=0;hh<4;hh++){
      //~ cout<<" m= "<<tan_max.mm[hh]<<" q= "<<tan_max.qq[hh]<<endl;
    //~ }
    //~ cout<<"scelta tangente"<<endl;
  //~ }
  //~ min_sum_scarto = 10000000.;
  //~ for(int hh=0;hh<4;hh++){
    //~ sum_scarto = 0.;
    //~ if(dbgflg)  cout<<"tangente # "<<hh<<endl;
    //~ for (int ii = 0; ii<nwire_U;ii++){
      //~ if((ii!=ii_max)&&(ii!=jj_max)){
	//~ distanza = fabs(Zcentro_U[ii]-tan_max.mm[hh]*Ycentro_U[ii] - tan_max.qq[hh])/
	  //~ sqrt(1. + tan_max.mm[hh]*tan_max.mm[hh]);
	//~ scarto = pow((distanza - R_U[ii]),2.);
	//~ sum_scarto +=scarto;
	//~ if(dbgflg) {
	  //~ cout<<"piano= "<<ii<<" distanza="<<distanza<<" scarto= "<<scarto<<
	    //~ " sum_scarto= "<<sum_scarto<<endl;
	//~ }
      //~ }
    //~ }
    //~ if(sum_scarto<min_sum_scarto){
      //~ min_sum_scarto = sum_scarto;
      //~ p_U = tan_max.mm[hh];
      //~ q_U = tan_max.qq[hh];
    //~ }
  //~ }

  //~ /*  estimate track line in the V view ( namely x-z plane) */
  //~ //  cout<<" VISTA V"<<endl;
  
  //~ dist_max=0;
  //~ ii_max=-1;
  //~ jj_max=-1;
  //~ /* start to find the 4 tangent of the 2 farest hits */
  //~ for (int ii = 1; ii<nwire_V;ii++){
    //~ for(int jj=0;jj<ii;jj++ ){
      //~ dist_curr = sqrt( pow((Xcentro_V[ii]-Xcentro_V[jj]),2.) + 
			//~ pow((Zcentro_V[ii]-Zcentro_V[jj]),2.0) );
      //~ if(dbgflg)  {
	//~ cout <<"ii,jj= "<<ii<<" "<<jj<<" xc[ii],zc[ii]= "<<Xcentro_V[ii]<<" "<<
	  //~ Zcentro_V[ii]<<" xc[jj],zc[jj]= "<<Xcentro_V[jj]<<" "<<Zcentro_V[jj]<<endl;
      //~ }
      //~ if(dist_curr>dist_max) {
	//~ dist_max = dist_curr;
	//~ ii_max = ii;
	//~ jj_max = jj;
	//~ tan_max = Circles2Tangents(Xcentro_V[ii], Zcentro_V[ii], R_V[ii],
				   //~ Xcentro_V[jj], Zcentro_V[jj], R_V[jj]);
      //~ }
    //~ }
  //~ }
  //~ if(dbgflg) {
    //~ cout<<"celle piu' distanti= "<<ii_max<<" "<<jj_max<<endl;
    //~ for(int hh=0;hh<4;hh++){
      //~ cout<<" m= "<<tan_max.mm[hh]<<" q= "<<tan_max.qq[hh]<<endl;
    //~ }
    //~ cout<<"scelta tangente"<<endl;
  //~ }
  //~ min_sum_scarto = 10000000.;
  //~ for(int hh=0;hh<4;hh++){
    //~ sum_scarto = 0.;
    //~ if(dbgflg)   cout<<"tangente # "<<hh<<endl;
    //~ for (int ii = 0; ii<nwire_V;ii++){
      //~ if((ii!=ii_max)&&(ii!=jj_max)){
	//~ distanza = fabs(Zcentro_V[ii]-tan_max.mm[hh]*Xcentro_V[ii] - tan_max.qq[hh])/
	  //~ sqrt(1. + tan_max.mm[hh]*tan_max.mm[hh]);
	//~ scarto = pow((distanza - R_V[ii]),2.);
	//~ sum_scarto +=scarto;
	//~ if(dbgflg) {
	  //~ cout<<"piano= "<<ii<<" distanza="<<distanza<<" scarto= "<<scarto<<
	    //~ " sum_scarto= "<<sum_scarto<<endl;
	//~ }
      //~ }
    //~ }
    //~ if(sum_scarto<min_sum_scarto){
      //~ min_sum_scarto = sum_scarto;
      //~ p_V = tan_max.mm[hh];
      //~ q_V = tan_max.qq[hh];
    //~ }
  //~ }

  //~ /*  calcolo dei parametri iniziali della traccia x0,y0,px0,py0*/
  //~ double pz0(0), px0, py0, x0, y0;
  //~ TVector3 CenterDch;
  //~ CenterDch = f_bmgeo->GetCenter();

  //~ if(p_U && p_V) {  pz0=1./sqrt(1.0 + 1.0/(p_U*p_U)+ 1.0/(p_V*p_V)); }
  //~ else if(!p_U && p_V) {  pz0=1./sqrt(1.0 + 1.0/(p_V*p_V)); }
  //~ else if(p_U && !p_V) {  pz0=1./sqrt(1.0 + 1.0/(p_U*p_U)); }
  //~ if(p_V) {
    //~ px0 = pz0/p_V;
    //~ x0 = ((CenterDch[2])-q_V)/p_V;
  //~ } else {
    //~ px0 = 0;
    //~ x0 = -0.4;
  //~ }
  //~ if(p_U) {
    //~ py0 = pz0/p_U;
    //~ y0 = ((CenterDch[2])-q_U)/p_U;
  //~ } else {
    //~ py0 = 0;
    //~ y0 = -0.4;
  //~ }
  //~ if(fabs(py0)>1 || fabs(px0)>1) { py0 = px0 = 0;}

  //~ int status = Set(x0,y0,px0,py0);
  //~ if(status<0) {
    //~ Error("Action()","EstimateTrackPat:: %lf %lf %lf %lf",px0,py0,p_V,p_U);
  //~ }
  //~ if(dbgflg){
    //~ cout<<"vista U: p_U= "<<p_U<<" q_U= "<<q_U<<"  vista V: p_V= "<<p_V<<
      //~ " q_V= "<<q_V<<endl;
    //~ cout<<"px, py, pz= "<<px0<<" "<<py0<<" "<<pz0<<" x0, y0= "<<x0<<" "<<y0<<endl;
  //~ }

  //~ return status;
//~ }


//~ TG_STRUCT TABMntuTrackTr::Circles2Tangents(double xc1, double yc1, double r1,
			     //~ double xc2, double yc2, double r2)
//~ {
  //~ /*  Given 2 circles in the x,y plane return the 4 tangents*/
  //~ TG_STRUCT tan_par;
  //~ for (int ii=0;ii<4;ii++){ /* clean up */
    //~ tan_par.mm[ii] =0. ;
    //~ tan_par.qq[ii] =0. ;
  //~ }
  //~ /*    cout<<"sono in Circles2Tangets"<<endl;
  //~ cout<<"xc1= "<<xc1<<" yc1= "<<yc1<<" r1= "<<r1<<"  xc2= "<<xc2<<" yc1= "<<
  //~ yc2<<" r2= "<<r2<<endl; */

  //~ double aa,bb,cc, dete, qpiu1, qmeno1, qpiu2, qmeno2;
  //~ cc = (r1*r1 -2*r1*r2 +r2*r2 - yc2*yc2 + 2*yc1*yc2 -yc1*yc1);
  //~ bb = 2*(yc1-yc2)*(xc1-xc2);
  //~ aa = (r1*r1 -2*r1*r2 +r2*r2 -xc1*xc1 +2*xc1*xc2 - xc2*xc2);
  //~ dete = bb*bb -4*aa*cc;
  //~ if(dete>0) {
    //~ tan_par.mm[0] = (-bb + sqrt(dete))/2/aa;
    //~ tan_par.mm[1] = (-bb - sqrt(dete))/2/aa;
  //~ }
  //~ cc = (r1*r1 +2*r1*r2 +r2*r2 - yc2*yc2 + 2*yc1*yc2 -yc1*yc1);
  //~ bb = 2*(yc1-yc2)*(xc1-xc2);
  //~ aa = (r1*r1 +2*r1*r2 +r2*r2 -xc1*xc1 +2*xc1*xc2 - xc2*xc2);
  //~ dete = bb*bb -4*aa*cc;
  //~ if(dete>0) {
    //~ tan_par.mm[2] = (-bb + sqrt(dete))/2/aa;
    //~ tan_par.mm[3] = (-bb - sqrt(dete))/2/aa;
  //~ }
  //~ for(int hh=0;hh<4;hh++){
    //~ qpiu1 = yc1 -tan_par.mm[hh]*xc1 + r1*sqrt(1+ tan_par.mm[hh]*tan_par.mm[hh]);
    //~ qmeno1 = yc1 -tan_par.mm[hh]*xc1 - r1*sqrt(1+ tan_par.mm[hh]*tan_par.mm[hh]);
    //~ qpiu2 = yc2 -tan_par.mm[hh]*xc2 + r2*sqrt(1+ tan_par.mm[hh]*tan_par.mm[hh]);
    //~ qmeno2 = yc2 -tan_par.mm[hh]*xc2 - r2*sqrt(1+ tan_par.mm[hh]*tan_par.mm[hh]);

    //~ if( (fabs(qpiu1-qpiu2)<.001) || (fabs(qpiu1-qmeno2)<0.001) ){ 
      //~ tan_par.qq[hh] = qpiu1; 
    //~ }
    //~ else if( (fabs(qmeno1-qmeno2)<0.001)||(fabs(qmeno1-qpiu2)<0.001)){ 
      //~ tan_par.qq[hh] = qmeno1; 
    //~ }
    //~ /*     cout<<"p= "<<tan_par.mm[hh]<<" qpiu1= "<<qpiu1<<" qmeno1= "<<qmeno1<<
	   //~ " qpiu2= "<<qpiu2<<" qmeno2= "<<qmeno2<<" q= "<<tan_par.qq[hh]<<endl;*/
  //~ }
  
  //~ return tan_par;
//~ }



//~ /*-----------------------------------------------------------------*/

//~ Int_t TABMntuTrackTr::ComputeDataAll(TABMntuRaw *hitp){

  //~ Double_t tp = 0., tf= 0.; 
  //~ Double_t prosca, D0W, D0P;
  //~ TVector3 D0;
  //~ int usedH(0);
  //~ int mynwire = hitp->nhit;
  //~ for(int ii=0;ii<mynwire;ii++){

    //~ TABMntuHit* wr = hitp->Hit(ii);
    //~ if(!wr->TrkAss()) continue;
    //~ D0 = R0 - wr->GetA0();    
    //~ prosca = Pvers*wr->GetWvers() ;
    //~ D0W = D0*wr->GetWvers();
    //~ D0P = D0*Pvers;
    //~ /*
    //~ cout<<ii<<endl;
    //~ D0.Print();
    //~ */
    //~ if(prosca!= 1.) {
      //~ tp = (D0W*prosca - D0P)/(1-prosca*prosca);
      //~ tf = (-D0P*prosca + D0W)/(1-prosca*prosca);
      //~ wr->SetRho(sqrt( D0.Mag2() + tp*tp + tf*tf + 2*tp*D0P -2*tf*D0W 
		       //~ -2*prosca*tf*tp));
      //~ wr->SetPca(R0 + tp*Pvers);
      //~ //      wr->SetPcaWire(wr->GetA0() + tf*wr->GetWvers());
      //~ trackrho(usedH) = wr->GetRho();
    //~ }
    //~ else{
      //~ wr->SetRho(sqrt(D0.Mag2() - D0W*D0W));
      //~ trackrho(usedH) = wr->GetRho();
    //~ }
    //~ usedH++;
  //~ }
  //~ return 1 ;
//~ }

//~ void   TABMntuTrackTr::SetChi2H(TVectorD dy, TVectorD au, TABMntuRaw *hitp) {

  //~ int mynwire = hitp->nhit;
  //~ int idx(0);
  //~ double tchi2(0);
  //~ for(int ii=0;ii<mynwire;ii++){
    
    //~ TABMntuHit* wr = hitp->Hit(ii);
    //~ if(wr->TrkAss()) {
      //~ tchi2 = dy(idx)*au(idx);
      //~ idx++;
      //~ wr->SetChi2(tchi2);
    //~ }
  //~ }
  //~ return;
//~ }

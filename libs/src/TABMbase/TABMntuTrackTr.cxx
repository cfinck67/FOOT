/*!
  \class TABMntuTrackTr TABMntuTrack.hxx "TABMntuTrack.hxx"
  \brief Beam Monitor track description - track object **
*/

#include "TABMntuTrackTr.hxx"

ClassImp(TABMntuTrackTr);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMntuTrackTr::TABMntuTrackTr():
    nhit(0), chi2Red(999), isConverged(0),prefit_status(-10)
    
{
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
  cout<<"new tracking: nhit="<<nhit<<"  chi2Red="<<chi2Red<<"  isConverged="<<isConverged<<"  prefit_status="<<prefit_status<<"  nite="<<nite<<endl;
  cout<<"Pvers=("<<Pvers.X()<<", "<<Pvers.Y()<<", "<<Pvers.Z()<<")"<<endl;
  cout<<"R0=("<<R0.X()<<", "<<R0.Y()<<", "<<R0.Z()<<")"<<endl;
}

/*-----------------------------------------------------------------*/

void TABMntuTrackTr::Clean()
{
  /*  
      reset the Track values to default 
  */

  //new tracking
  nhit=0;
  chi2Red=999;
  isConverged=0;
  Pvers.SetXYZ(-100.,-100.,-100.);
  R0.SetXYZ(-100.,-100.,-100.);
  prefit_status=-10;
  nite=0;
}

TABMntuTrackTr::TABMntuTrackTr(const TABMntuTrackTr &tr_in){
  nhit=tr_in.nhit;
  chi2Red=tr_in.chi2Red;
  isConverged=tr_in.isConverged;
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


Double_t TABMntuTrackTr::FindRdrift(TVector3 pos, TVector3 dir, TVector3 A0, TVector3 Wvers) {

  Double_t tp = 0., tf= 0., rdrift; 
  TVector3 D0, R0, Pvers;
  Wvers.SetMag(1.);

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


/*!
  \file
  \version $Id: TABMactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TABMactNtuMC.
*/

#include "TAGroot.hxx"
#include "TABMntuRaw.hxx"
#include "TABMparCon.hxx"
#include "TABMactNtuMC.hxx"

//First
#include "TAGgeoTrafo.hxx"

/*!
  \class TABMactNtuMC TABMactNtuMC.hxx "TABMactNtuMC.hxx"
  \brief NTuplizer for BM raw hits. **
*/

ClassImp(TABMactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMactNtuMC::TABMactNtuMC(const char* name,
			   TAGdataDsc* p_nturaw, 
			   TAGparaDsc* p_parcon, 
			   TAGparaDsc* p_bmgeo, 
			   EVENT_STRUCT* evStr)
  : TAGaction(name, "TABMactNtuMC - NTuplize ToF raw data"),
    fpNtuMC(p_nturaw),
    fpParCon(p_parcon),
    fpParGeo(p_bmgeo),
    fpEvtStr(evStr)
{
  Info("Action()"," Creating the Beam Monitor MC tuplizer action\n");
  AddPara(p_parcon, "TABMparCon");
  AddPara(p_bmgeo, "TABMparGeo");
  AddDataOut(p_nturaw, "TABMntuRaw"); 

}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMactNtuMC::~TABMactNtuMC()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TABMactNtuMC::Action()
{
  TAGgeoTrafo* fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

  TABMntuRaw* p_nturaw = (TABMntuRaw*) fpNtuMC->Object();
  TABMparCon* p_parcon = (TABMparCon*) fpParCon->Object();
  TABMparGeo* p_bmgeo = (TABMparGeo*) fpParGeo->Object();

  Int_t BMdebug=0;

  Int_t cell, view, lay, ipoint;
  vector<Int_t> hitxcell(fpEvtStr->BMNn, 99); 
  vector<bool> tobecharged(fpEvtStr->BMNn, true);
  bool needPosm2=true;
  vector<Double_t> rdriftxcell(fpEvtStr->BMNn, 99);
  Int_t nhits=0;
  TVector3 gloc, loc, gmom, mom, A0, Wvers, in_posm2(0,0,-99);
  if (!p_nturaw->h) p_nturaw->SetupClones();
  //The number of hits inside the BM is nmon
  Info("Action()","Processing n :: %2d hits \n",fpEvtStr->BMNn);

  //loop for double hits and hits with energy less than enxcell_cut:
  for (Int_t i = 0; i < fpEvtStr->BMNn; i++) {
    if(fpEvtStr->BMNde[i] < p_parcon->GetEnxcellcut())
      tobecharged[i]=false;
    if(tobecharged[i]){
      cell = fpEvtStr->BMNicell[i];
      lay = fpEvtStr->BMNilay[i]; 
      view = fpEvtStr->BMNiview[i];
      hitxcell[i]=p_bmgeo->GetBMNcell(fpEvtStr->BMNilay[i], view, cell);
      gloc.SetXYZ(fpEvtStr->BMNxin[i],fpEvtStr->BMNyin[i],fpEvtStr->BMNzin[i]);
      //loc = fpFirstGeo->FromGlobalToBMLocal(gloc); //non funzia per ora!!! era geometria di first, da sistemare, per ora faccio così
      //~ loc.SetXYZ(gloc.x()+p_bmgeo->GetCenter().x(),
                                 //~ gloc.y()+p_bmgeo->GetCenter().y(),
                                 //~ gloc.z()+p_bmgeo->GetCenter().z());
      loc=gloc-p_bmgeo->GetCenter();//metto shift a mano per ora
      gmom.SetXYZ(fpEvtStr->BMNpxin[i],fpEvtStr->BMNpyin[i],fpEvtStr->BMNpzin[i]);
      //mom = fpFirstGeo->VecFromGlobalToBMLocal(gmom);
      //~ mom.SetXYZ(gmom.x()-p_bmgeo->GetCenter().x(),               //useless? dovrebbe andar bene il momento normale! senza traslazioni
                                 //~ gmom.y()-p_bmgeo->GetCenter().y(),  //DA VERIFICARE!!!!!!!!!!!!
                                 //~ gmom.z()-p_bmgeo->GetCenter().z());
      if(fpEvtStr->TRpaid[fpEvtStr->BMNid[i]-1]==0 && in_posm2.Z()<loc.Z()){// in in_posm2 ho coordinate diultimo hit del primario che esce
        in_posm2=loc;
        if(BMdebug>=3)
          cout<<"z in_posm2 ora vale="<<in_posm2.Z()<<endl;
        }
      view=(view==-1)?1:0;
      A0.SetXYZ(p_bmgeo->GetX(p_bmgeo->GetID(cell),lay,view),    //sarebbe più elegante mettere questa roba in FindRdrift,  
                p_bmgeo->GetY(p_bmgeo->GetID(cell),lay,view),    //ma in FindRdrift dovrei caricare p_bmgeo, e forse non conviene
                p_bmgeo->GetZ(p_bmgeo->GetID(cell),lay,view));  
      Wvers.SetXYZ(p_bmgeo->GetCX(p_bmgeo->GetID(cell),lay,view), 
                   p_bmgeo->GetCY(p_bmgeo->GetID(cell),lay,view), 
                   p_bmgeo->GetCZ(p_bmgeo->GetID(cell),lay,view)); 
      Wvers.SetMag(1.);                      
      rdriftxcell[i]=FindRdrift(loc, gmom, A0, Wvers);
      if(rdriftxcell[i]==99) //FindRdrift return 99 if a particle is born without energy, so it shouldn't release energy for a hit.
        tobecharged[i]=false;
      //if there is a double hit in the same cell, it charges the hits if they have rdrift difference more than p_parcon->GetRdriftCut()
      for(Int_t j=0;j<i;j++){ 
        if((rdriftxcell[i]-rdriftxcell[j])<p_parcon->GetRdriftCut() && rdriftxcell[i]>=rdriftxcell[j] && hitxcell[i]==hitxcell[j])
          tobecharged[i]=false;
        if((rdriftxcell[j]-rdriftxcell[i])<p_parcon->GetRdriftCut() && rdriftxcell[j]>rdriftxcell[i] && hitxcell[i]==hitxcell[j])
          tobecharged[j]=false;
        }
      }
    }
    
  if((fpEvtStr->BMNn > 12 && BMdebug>=2) || BMdebug>=3)
    cout<<"number of hit totale="<<fpEvtStr->BMNn<<" dimensioni tobecharged="<<tobecharged.size()<<endl;   
    
  if(BMdebug>=3)
    cout<<"hit del primario più vicino al target inposm2=("<<in_posm2.X()<<", "<<in_posm2.Y()<<" ,"<<in_posm2.Z()<<" "<<endl;   
      
  //charge the hits:
  for (Int_t i = 0; i < fpEvtStr->BMNn; i++) {
    if(BMdebug>=3)
      cout<<"In the charging hits loop: I'm going to charge hit number:"<<i<<endl;
    if(tobecharged[i]){
      ipoint=fpEvtStr->BMNid[i]-1;
      cell = fpEvtStr->BMNicell[i];
      lay = fpEvtStr->BMNilay[i];
      view = fpEvtStr->BMNiview[i];
      
      //create hit
      TABMntuHit *mytmp = new((*(p_nturaw->h))[nhits]) TABMntuHit(    
                          fpEvtStr->BMNid[i],	view, lay, cell,        
                          0, 0, 0,  //Will become PCA that is needed @ tracking level.
                          fpEvtStr->BMNpxin[i], fpEvtStr->BMNpyin[i], fpEvtStr->BMNpzin[i],  //mom @ entrance in cell
                          rdriftxcell[i], 0., fpEvtStr->BMNtim[i]);     //tdrift has no meaning for MC (now)
        
      //X,Y and Z needs to be placed in Local coordinates.
      mytmp->SetAW(p_bmgeo);
      mytmp->SetSigma(p_parcon->ResoEval(rdriftxcell[i]));
      mytmp->SmearRdrift();
      
      //siccome non ci sono i crossing, mi accontento dell'ultima posizione in uscita del primario dal bmn... almeno per ora
      mytmp->SetPosm2(in_posm2);
      
      //banca dati crossing non è caricato in framework per ora... un giorno potrebbe tornare utile questo...
      //~ if(fpEvtStr->TRpaid[ipoint]==0){// se è un primario
        //~ if(needPosm2){
          //~ for(Int_t j=0;j<fpEvtStr->CROSSn; j++){
            //~ if(fpEvtStr->TRpaid[fpEvtStr->CROSSid[j]-1]==0)
              //~ cout<<"ehi ho trovato un crossing della particella primaria!"<<fpEvtStr->CROSSnregold[j]<<"  z="<<fpEvtStr->CROSSz[j]<<"  y="<<fpEvtStr->CROSSy[j]<<"  x="<<fpEvtStr->CROSSx[j]<<endl;
            //~ if(fpEvtStr->TRpaid[fpEvtStr->CROSSid[j]-1]==0 &&  fpEvtStr->CROSSnregold[j]==nregMyl2BMN){
              //~ in_posm2.SetXYZ(fpEvtStr->CROSSx[j], fpEvtStr->CROSSy[j], fpEvtStr->CROSSz[j]);
              //~ cout<<"ehi ho trovato il punto dove esce il primario: z="<<in_posm2.Z()<<endl;
              //~ }
            //~ }
          //~ needPosm2=false;
          //~ }
        //~ mytmp->SetPosm2(in_posm2); //if primary haven't cross mylar2 (bad event) in_posm2 is (0,0,99)
        //~ }
        
        
        
      //~ if(ipoint==0 && exitM){        //primary particle exit from mylar2   forse meglio farlo in TABMactNtuTrack.cxx
        //~ mytmp->SetTrkAss(0);    
      //~ }else if(ipoint==0 && !exitM){
        //~ mytmp->SetTrkAss(1);        //primary particle not exit from mylar2
      //~ }else{
        //~ mytmp->SetTrkAss(2);
      //~ }
      
        
      nhits++;
      }//end of tobecharded if
    }
  
  if(nhits>12 && BMdebug>=1)
    cout<<" more than 12 hit charged in tabmactntuMC:"<<nhits<<endl;
  
  if(BMdebug>=2)
    cout<<"number of hits charged="<<nhits<<endl;
  
  p_nturaw->nhit  = nhits;

  fpNtuMC->SetBit(kValid);
  return kTRUE;
}




Double_t TABMactNtuMC::FindRdrift(TVector3 pos, TVector3 dir, TVector3 A0, TVector3 Wvers) {

  Double_t tp = 0., tf= 0., rdrift; 
  TVector3 D0, R0, Pvers;

  //~ if (dir.Mag()!=0.)
    //~ dir.SetMag(1.);
  //~ else{
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
    cout<<"WARNING!!!!! SOMETHING IS WRONG, YOU HAVE A NEGATIVE RDRIFT!!!!!!!!!"<<endl;
  if(rdrift>0.96)
    cout<<"WARNING!!!!! SOMETHING IS WRONG, YOU HAVE A TOO BIG RDRIFT!!!!!!!!! rdrift="<<rdrift<<endl;
  if(rdrift>0.96 || rdrift<0){
    cout<<"pos=("<<pos.X()<<","<<pos.Y()<<","<<pos.Z()<<")  dir=("<<dir.X()<<","<<dir.Y()<<","<<dir.Z()<<")"<<endl;
    cout<<"A0=("<<A0.X()<<","<<A0.Y()<<","<<A0.Z()<<")  Wvers=("<<Wvers.X()<<","<<Wvers.Y()<<","<<Wvers.Z()<<")"<<endl;
    }
    
  return rdrift;
}

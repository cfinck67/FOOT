#include "MultiTrackCheck.hxx"


using namespace std;



//----------------------------------------------------------------------------------------------------
MultiTrackCheck::MultiTrackCheck() {



}

void MultiTrackCheck::Initialize( EVENT_STRUCT* evStr ) {

  debug_multi = 0;
  
  //////// Detector Hit //////////////////
  TAGdataDsc* _vtraw    = gTAGroot->FindDataDsc("vtRaw",  "TAVTntuRaw");
  myn_vtraw = (TAVTntuRaw*) _vtraw->Object();
      
  TAGdataDsc* _itraw    = gTAGroot->FindDataDsc("itRaw",  "TAITntuRaw");
  myn_itraw = (TAITntuRaw*) _itraw->Object();
  
  TAGdataDsc* _msdraw   = gTAGroot->FindDataDsc("msdRaw", "TAMSDntuRaw");
  myn_msdraw = (TAMSDntuRaw*) _msdraw->Object();

   //////// MCtrack //////////////////
  myn_mceve    = new TAGdataDsc("myn_mceve", new TAGntuMCeve());
  mya_mceve    = new TAGactNtuMCeve("mya_mceve", myn_mceve, evStr);

  
  BookHisto();
  
}

void MultiTrackCheck::Process( Long64_t jentry ){
  
  //   cout << " EVENT EVENT  EVENT "  << endl; 
    
    m_VT_hitCollection.clear();
    m_IT_hitCollection.clear();
    m_MSD_hitCollection.clear();
    m_VT_partIDColl.clear();
    m_FragIdxs.clear();
    m_Frag_hitCollection.clear();
    m_Frag_indexCollection.clear();
    m_Trcks9hits.clear();
    
   int ntrackonVT = 0;     
   int ntrackonIT = 0;  
   int ntrackonMSD = 0;

   // cout << myn_vtraw->GetPixelsN(0) << endl;
   for (int i = 0; i < myn_vtraw->GetPixelsN(0, "mc_hit"); i++)  {                    
     TAVTntuHit* vt_hit = myn_vtraw->GetPixel(0,i, "mc_hit"); 
     tmpVT_partID =  vt_hit->m_genPartID;
     if(vt_hit->GetMCPosition_Global().Z() < 0.7) ntrackonVT++;

     m_VT_hitCollection.push_back(vt_hit);
     m_VT_partIDColl.push_back(tmpVT_partID);
     
   }
   
   // save pixels in the collection     
   for (int i = 0; i < myn_itraw->GetPixelsN(0); i++)  {                   
     TAITntuHit* it_hit = myn_itraw->GetPixel(0,i);         
 
     if((it_hit->GetMCPosition_Global().Z() > 13.3 && it_hit->GetMCPosition_Global().Z() < 13.5) || (it_hit->GetMCPosition_Global().Z() > 14.3 && it_hit->GetMCPosition_Global().Z() < 14.5))  ntrackonIT++;
     
    m_IT_hitCollection.push_back(it_hit);   
    
   }
   
   for (int i = 0; i < myn_msdraw->GetPixelsN(0); i++)  {                    
     TAMSDntuHit* msd_hit = myn_msdraw->GetPixel(0,i);       
     
     if(msd_hit->GetMCPosition_Global().Z() < 27.0) 	ntrackonMSD++;
     
     m_MSD_hitCollection.push_back(msd_hit);
   
   }
   
   histo_VT->Fill(ntrackonVT);
   histo_IT->Fill(ntrackonIT);
   histo_MSD->Fill(ntrackonMSD);

   
   
   //// Read track ntuple variable//// 
   TAGntuMCeve*  p_ntumceve = (TAGntuMCeve*)   myn_mceve->GenerateObject();
   int ntrackMC =  p_ntumceve->nhit;
   histo_Track->Fill(ntrackMC);
   
   for(int i=0; i<ntrackMC; i++){
     TAGntuMCeveHit *myTrack = p_ntumceve->Hit(i);
     m_Frag_indexCollection.push_back(i);
     m_Frag_hitCollection.push_back(myTrack);  
   }
   
   
   for (int i=0; i<m_Frag_hitCollection.size(); i++){
       
     int nVT = 0;     
     int nIT = 0;  
     int nMSD = 0;  
     

     for(int j=0; j<m_VT_hitCollection.size(); j++){
       int tmp_vtxid = m_VT_hitCollection.at(j)->GetMCid()-1;  
       

       if(tmp_vtxid == m_Frag_indexCollection.at(i)){
	 
	 if (debug_multi > 0) cout<<" Vtx hit associated to part "<< i << " That is a:: "<< m_Frag_hitCollection.at(i)->FlukaID() <<"  and has charge, mass:: "<< m_Frag_hitCollection.at(i)->Chg()<<" "<<m_Frag_hitCollection.at(i)->Mass()<<" "<<endl; 

	 nVT++;
       }
       
     }
     
     for(int k=0; k<m_IT_hitCollection.size(); k++){
       int tmp_itid = m_IT_hitCollection.at(k)->GetMCid()-1;  
       if(tmp_itid == m_Frag_indexCollection.at(i)){
	 
	 if (debug_multi > 0) cout<<" It hit associated to part "<< i << " That is a:: "<< m_Frag_hitCollection.at(i)->FlukaID() <<"  and has charge, mass:: "<< m_Frag_hitCollection.at(i)->Chg()<<" "<<m_Frag_hitCollection.at(i)->Mass()<<" "<<endl; 
	 
	 nIT++;
	 
       }
     }
         
     for(int z=0; z<m_MSD_hitCollection.size(); z++){
       int tmp_msdid = m_MSD_hitCollection.at(z)->GetMCid()-1;  
   	if (tmp_msdid == m_Frag_indexCollection.at(i))
   	  {
	    
	    if (debug_multi > 0) cout<<" MSD hit associated to part "<< i << " That is a:: "<< m_Frag_hitCollection.at(i)->FlukaID() <<"  and has charge, mass:: "<< m_Frag_hitCollection.at(i)->Chg()<<" "<<m_Frag_hitCollection.at(i)->Mass()<<" "<<endl; 
	    
	    nMSD++;
   	  }
     }

     int nTrackingHit = nVT + nIT + nMSD;
     if (debug_multi > 0)   cout << " Nhit per track  " <<  nVT + nIT + nMSD << endl;
     
     if (nTrackingHit > 0 && nTrackingHit < 10)
       {
	 
	 histo_TrackingHit->Fill(nTrackingHit);

	 if (nTrackingHit == 9){
	   
	   
	   m_Trcks9hits.push_back(m_Frag_hitCollection.at(i));
	   m_Trcks9index.push_back(i);

	   
	   
	   
	   }
       }
   }

   // cout << " m_Trcks9hits  size " <<  m_Trcks9hits.size() << endl;
   // cout << " m_Trcks9index size " <<  m_Trcks9index.size() << endl;

  
   

   
   // cout << " m_VT_hitCollection.size()  " << m_VT_hitCollection.size()  << endl;
   for (int i =0; i< m_Trcks9index.size(); i++) 
     {

       //cout << " TRACK TRACK TRACK "  << endl; 
       // cout << " m_Trcks9index " <<  m_Trcks9index.at(i) << endl;
       m_VTX_Trackpos.clear();
       m_IT_Trackpos.clear();
       m_MSD_Trackpos.clear();
       
       for(int j=0; j<m_VT_hitCollection.size(); j++){
	 if ( m_Trcks9index.at(i) == m_VT_hitCollection.at(j)->GetMCid()-1)
	   { 
	     //cout <<  " GetMCPosition_Global().Z() " << m_VT_hitCollection.at(j)->GetMCPosition_Global().Z() << endl;
	     TVector3 vtxpos = TVector3 (m_VT_hitCollection.at(j)->GetMCPosition_Global().X(),m_VT_hitCollection.at(j)->GetMCPosition_Global().Y(),m_VT_hitCollection.at(j)->GetMCPosition_Global().Z());
	     m_VTX_Trackpos.push_back(vtxpos);
	     
	     // control histograms
	     h_zx->Fill(m_VT_hitCollection.at(j)->GetMCPosition_Global().Z(), m_VT_hitCollection.at(j)->GetMCPosition_Global().X());
	     h_zy->Fill(m_VT_hitCollection.at(j)->GetMCPosition_Global().Z(), m_VT_hitCollection.at(j)->GetMCPosition_Global().Y());
	     h_yx->Fill(m_VT_hitCollection.at(j)->GetMCPosition_Global().Y(), m_VT_hitCollection.at(j)->GetMCPosition_Global().X());

	     
	     float r=pow(m_VT_hitCollection.at(j)->GetMCPosition_Global().X()*m_VT_hitCollection.at(j)->GetMCPosition_Global().X()+
			 m_VT_hitCollection.at(j)->GetMCPosition_Global().Y()*m_VT_hitCollection.at(j)->GetMCPosition_Global().Y(),0.5);
	     
	     double th=atan2(r,m_VT_hitCollection.at(j)->GetMCPosition_Global().Z());
	     double eta=-log(tan(0.5*th));
	     double phi = atan2(m_VT_hitCollection.at(j)->GetMCPosition_Global().Y(), m_VT_hitCollection.at(j)->GetMCPosition_Global().X());
	     
	     h_zr->Fill(m_VT_hitCollection.at(j)->GetMCPosition_Global().Z(),r);
	     h_rphi->Fill(r,phi);
	     h_etaphi->Fill(eta,phi);
	     h_thetaphi->Fill(th,phi);
	   }
       }
       
       for(int k=0; k<m_IT_hitCollection.size(); k++){
	 if ( m_Trcks9index.at(i) == m_IT_hitCollection.at(k)->GetMCid()-1)
	   { 
	     // cout <<  " GetMCPosition_Global().Z() " << m_IT_hitCollection.at(k)->GetMCPosition_Global().Z() << endl;
	     
	     h_zx->Fill(m_IT_hitCollection.at(k)->GetMCPosition_Global().Z(), m_IT_hitCollection.at(k)->GetMCPosition_Global().X());
	     h_zy->Fill(m_IT_hitCollection.at(k)->GetMCPosition_Global().Z(), m_IT_hitCollection.at(k)->GetMCPosition_Global().Y());
	     h_yx->Fill(m_IT_hitCollection.at(k)->GetMCPosition_Global().Y(), m_IT_hitCollection.at(k)->GetMCPosition_Global().X());
	     
	     TVector3 itpos = TVector3 (m_IT_hitCollection.at(k)->GetMCPosition_Global().X(),m_IT_hitCollection.at(k)->GetMCPosition_Global().Y(),m_IT_hitCollection.at(k)->GetMCPosition_Global().Z());
	     m_IT_Trackpos.push_back(itpos);
	   }
	  
       }
       
       for(int z=0; z<m_MSD_hitCollection.size(); z++){
	 if (m_Trcks9index.at(i) == m_MSD_hitCollection.at(z)->GetMCid()-1)
	   {
	     
	     //cout <<  " GetMCPosition_Global().Z() " << m_MSD_hitCollection.at(z)->GetMCPosition_Global().Z() << endl;
	     
	     h_zx->Fill(m_MSD_hitCollection.at(z)->GetMCPosition_Global().Z(), m_MSD_hitCollection.at(z)->GetMCPosition_Global().X());
	     h_zy->Fill(m_MSD_hitCollection.at(z)->GetMCPosition_Global().Z(), m_MSD_hitCollection.at(z)->GetMCPosition_Global().Y());
	     h_yx->Fill(m_MSD_hitCollection.at(z)->GetMCPosition_Global().Y(), m_MSD_hitCollection.at(z)->GetMCPosition_Global().X());
	     
	     TVector3 msdpos =TVector3(m_MSD_hitCollection.at(z)->GetMCPosition_Global().X(),m_MSD_hitCollection.at(z)->GetMCPosition_Global().Y(),m_MSD_hitCollection.at(z)->GetMCPosition_Global().Z());
	     m_MSD_Trackpos.push_back(msdpos);
	   }
	 
       }

       if (debug_multi > 0){
	 cout <<  " HITS per track" << endl;
	 cout <<  " m_VTX_Trackpos.size() " << m_VTX_Trackpos.size() << endl;
	 cout <<  " m_IT_Trackpos.size() " << m_IT_Trackpos.size() << endl;
	 cout <<  " m_MSD_Trackpos.size() " << m_MSD_Trackpos.size() << endl;
       }
       
       m_DetTrackpos["VT"] = m_VTX_Trackpos;
       m_DetTrackpos["IT"] = m_IT_Trackpos;
       m_DetTrackpos["MSD"] = m_MSD_Trackpos;
       
       
       
       for ( map< string, vector <TVector3>>::iterator it = m_DetTrackpos.begin(); it != m_DetTrackpos.end(); it++ ) {       
	 
	 if (debug_multi > 1) cout << " (*it).first  " << (*it).first << endl;
	 // for (unsigned int i=0; i<(*it).second.size(); i++){
	 //   //  cout << " (*it).second z  " << (*it).second.at(i).z() << endl;
	 
	 // }
       }
       
     }
   
   
   
   
   
   
}

void MultiTrackCheck::Finalize(){

  f->Write();
  f->Close();
  

}

void MultiTrackCheck::BookHisto() {

  f = new TFile("TrackFile.root","RECREATE");
  
  int  m_nBin = 100;
  int  m_minBin = 0;
  int  m_maxBin = 100;

  histo_Track = new TH1F( "MC_track", "n tracks ", m_nBin, m_minBin, 2*m_maxBin );
  histo_VT = new TH1F( "VT_hit", "nVT hits ", m_nBin, m_minBin, m_maxBin );
  histo_IT = new TH1F( "IT_hit", "nIT hits ", m_nBin, m_minBin, m_maxBin );
  histo_MSD = new TH1F("MSD_hit", "nMSD hits ", m_nBin, m_minBin, m_maxBin );
  histo_TrackingHit = new TH1F( "Hit in tracking system", "n hits ", m_nBin, m_minBin, m_maxBin/10 );
  h_zx = new TH2F("h_zx","h_zx", 800, 0, 40, 200, -10, 10);
  h_zy = new TH2F("h_zy","h_zy", 800, 0, 40, 200, -10, 10);
  h_yx = new TH2F("h_yx","h_yx",200, -10, 10, 200, -10, 10);
  h_zr = new TH2F("h_zr","h_zr",400, 0, 40, 400,    0, 40);
  h_rphi = new TH2F("h_rphi","h_rphi",400, 0, 40, 128, -TMath::Pi(), TMath::Pi());
  h_etaphi = new TH2F("h_etaphi","h_etaphi",100,0,10, 128, -TMath::Pi(), TMath::Pi());
  h_thetaphi = new TH2F("h_theta","h_thetaphi",128, -TMath::Pi(), TMath::Pi(), 128, -TMath::Pi(), TMath::Pi());
}



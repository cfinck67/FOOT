#define RecoTools_cxx

#include "RecoTools.h"



using namespace std;




//----------------------------------------------------------------------------------------------------
RecoTools::RecoTools(int d, TString istr, bool list, TString ostr, TString wd, int nev, TFile *hf, int evstart, int isdata) {

    cout << "\tstart Constructor RecoTools\n";

    my_files.clear();
    m_debug = GlobalPar::GetPar()->Debug();
    m_evstart=evstart;
    m_oustr = ostr;
    m_wd = wd;
    m_tempo_kal=0;
    m_isdata=isdata;

    // take input file list
    ifstream inS;
    char bufConf[200]; char fname[400];
    if(!list) {
        m_instr = istr;
        my_files.push_back(m_instr.Data());
    } else {
        m_instr = istr;
        inS.open(m_instr.Data());
        while (inS.getline(bufConf, 200, '\n')) {
          sscanf(bufConf,"%s",fname);
          my_files.push_back(fname);
        }
    }

    m_nev = nev;
    gErrorIgnoreLevel = kError;
    m_hf = hf;
    cout << "\end Constructor RecoTools\n";
}






//----------------------------------------------------------------------------------------------------
void RecoTools::RecoLoop(TAGroot *tagr, int fr) {

    //Configure the output flagging
    tagr->SetCampaignNumber(100);
    tagr->SetRunNumber(1);
    TAGpadGroup* pg = new TAGpadGroup();
    
    //MC analysis
    //~ if(m_isdata){//if bm data
      //~ bmcalbooter = new BMcalBooter();
      //~ bmcalbooter->Initialize(m_wd,m_instr,m_isroma);
    //~ }else{
      // input ntuple tree
    EVENT_STRUCT evStr;
    
    //MC tree
    if(!m_isdata){
      tree = new TChain("EventTree");
      for(unsigned int ifi=0; ifi<my_files.size(); ifi++) {
          tree->Add(my_files.at(ifi).data());
          cout<<"Adding :: "<<my_files.at(ifi).data() << " file"<<endl;
      }

      // create the event object
      if(m_debug) cout<<" Creating Eve "<<endl;
      Evento *ev =  new Evento();
      ev->FindBranches(tree,&evStr);
      if(m_debug) cout<<" Found branches "<<endl;
      //Define the output file content.
      my_out = new TAGactTreeWriter("my_out");
      tagr->AddRequiredItem("my_out");
      tagr->Print();
      if (my_out->Open(m_oustr, "RECREATE")) return;    
    }

    booter = new Booter();
    bmbooter = new BmBooter();
    booter->Initialize( &evStr, m_wd, m_isdata);
    if (GlobalPar::GetPar()->IncludeBM())
      if(bmbooter->Initialize( m_instr, m_isdata, &evStr))
        return;
  //~ }//end of MC initializers

    
    /***********  The event Loop   ****************************************   */
    tagr->BeginEventLoop();
    Long64_t nentries; 
    if(m_isdata && GlobalPar::GetPar()->IncludeBM()){
      cout<<"Total number of Beam Monitor events="<<bmbooter->GetTotnumev()<<endl;
      nentries=(m_nev==0) ? bmbooter->GetTotnumev()-bmbooter->GetAcqStartEv()+1 : m_nev;
    }else {
      nentries = tree->GetEntries(); 
      if(m_nev != 0)      
        nentries = m_nev;
    }
    Long64_t nbytes = 0, nb = 0;
    char flag[200]; bool tobedrawn = kFALSE;

    if(m_debug)         cout<<" Starting Event Loop "<<endl;
    int skip_event=0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        if(m_debug) cout<<" New Eve "<<endl;
        
        if(!m_isdata){
          nb = tree->GetEntry(jentry);   
          nbytes += nb;
        }
        // if (Cut(ientry) < 0) continue;
        // if (jentry>1)  break;
        // if (jentry<33061)  continue;
        tagr->NextEvent();
        if(!(jentry%fr))        cout<<"Processed:: "<<jentry<<" evts!"<<endl;


        ///////////////  Call here your Process() functions    /////////////////////////////////////////////

        
        if(!m_isdata && skip_event>=m_evstart)
          booter->Process( jentry );
        if(GlobalPar::GetPar()->IncludeBM() && skip_event>=m_evstart)
          bmbooter->Process();
        else if(GlobalPar::GetPar()->IncludeBM() && skip_event<m_evstart)
          bmbooter->drop_event();
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////


        if (!pg->IsEmpty() && tobedrawn && !(jentry%fr) && !m_isdata && skip_event<m_evstart) {
            pg->Modified();
            pg->Update();

            sprintf(flag,"plots/%s_%d","Test_MC",(int)jentry);
            pg->Print(flag);
        }

        if(m_debug) cout<<" Loaded Event:: "<<jentry<<endl;

        if ( GlobalPar::GetPar()->Debug() > 0 )         cout << "End event n: " << jentry << endl;

        skip_event++;

    }
    cout << "End of the event loop " << endl;

  
    booter->Finalize();
    if (GlobalPar::GetPar()->IncludeBM())
      bmbooter->Finalize();
    if(GlobalPar::GetPar()->IsPrintOutputFile())
      ControlPlotsRepository::GetControlObject( "BooterFinalize" )->SaveOutputFile();//close and save OutputFile    
    if(!m_isdata){
      my_out->Print();
      my_out->Close();
    }
    
    tagr->EndEventLoop();

    // materialEffects->drawdEdx( 11 );  // to look at it in genfit

    return;
}


//OLD STUFF if we want to separate the event loop for the data from the MC it could be useful, otherwise it's useless and it can be deleted
//recoloop for the bm calibration
/*
void RecoTools::RecoBMcal(TAGroot *tagr) {
  TAGpadGroup* pg = new TAGpadGroup();
  //~ ifstream inF; //input file
  //~ int eSwSize; //dummy variable to read the input file 

  //Configure the output flagging
  tagr->SetCampaignNumber(100);
  tagr->SetRunNumber(1);

  //Define the output file content.
  //~ my_out = new TAGactTreeWriter("my_out");


  tagr->AddRequiredItem("my_out");
  tagr->Print();
  //~ if (my_out->Open(m_oustr, "RECREATE")) return;


  bmcalbooter = new BMcalBooter();
  bmcalbooter->Initialize(m_wd,m_instr,m_isroma);

  //~ ***********  The event Loop   ****************************************       
  tagr->BeginEventLoop();
  Long64_t nentries=(m_nev==0) ? bmcalbooter->GetData_numev() : m_nev;
  //~ Long64_t nbytes = 0, nb = 0;
  char flag[200]; bool tobedrawn = kFALSE;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
      if(m_debug) cout<<" New Eve "<<endl;
      //~ nb = tree->GetEntry(jentry);   
      //~ nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      // if (jentry>1)  break;
      // if (jentry<33061)  continue;
      tagr->NextEvent();


      ///////////////  Call here your Process() functions    /////////////////////////////////////////////

      bmcalbooter->Process();

      ////////////////////////////////////////////////////////////////////////////////////////////////////


      if(m_debug) cout<<" Loaded Event:: "<<jentry<<endl;

      if ( GlobalPar::GetPar()->Debug() > 0 )         cout << "End event n: " << jentry << endl;

  }
  cout << "End of the event loop " << endl;

  bmcalbooter->Finalize();
  
  //~ if(GlobalPar::GetPar()->IsPrintOutputFile())
    //~ ControlPlotsRepository::GetControlObject( "BooterFinalize" )->SaveOutputFile();//close and save OutputFile
  
  tagr->EndEventLoop();
  
  //~ my_out->Print();
  //~ my_out->Close();
}
*/












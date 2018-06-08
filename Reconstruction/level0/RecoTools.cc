#define RecoTools_cxx

#include "RecoTools.h"



using namespace std;




//----------------------------------------------------------------------------------------------------
RecoTools::RecoTools(int d, TString istr, bool list, TString ostr, TString wd, int nev, TFile *hf) {

    cout << "\tstart Constructor RecoTools\n";

    my_files.clear();
    m_debug = GlobalPar::GetPar()->Debug();

    m_oustr = ostr;
    m_wd = wd;

    m_tempo_kal=0;

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
    cout << "\tend Constructor RecoTools\n";
}






//----------------------------------------------------------------------------------------------------
void RecoTools::RecoLoop(TAGroot *tagr, int fr) {

    // input ntuple tree
    TChain *tree = new TChain("EventTree");
    for(unsigned int ifi=0; ifi<my_files.size(); ifi++) {
        tree->Add(my_files.at(ifi).data());
        cout<<"Adding :: "<<my_files.at(ifi).data() << " file"<<endl;
    }

    // create the event object
    if(m_debug) cout<<" Creating Eve "<<endl;
    Evento *ev =  new Evento();
    EVENT_STRUCT evStr;
    TAGpadGroup* pg = new TAGpadGroup();

    ev->FindBranches(tree,&evStr);

    if(m_debug) cout<<" Found branches "<<endl;

    //Configure the output flagging
    tagr->SetCampaignNumber(100);
    tagr->SetRunNumber(1);

    //Define the output file content.
    my_out = new TAGactTreeWriter("my_out");


    tagr->AddRequiredItem("my_out");
    tagr->Print();
    if (my_out->Open(m_oustr, "RECREATE")) return;


    Booter* booter = new Booter();
    BmBooter* bmbooter = new BmBooter();
    booter->Initialize( &evStr, m_wd );
    if (GlobalPar::GetPar()->IncludeBM())
      bmbooter->Initialize( &evStr);

    
    /***********  The event Loop   ****************************************   */
    tagr->BeginEventLoop();
    Long64_t nentries = tree->GetEntries();
    Long64_t nbytes = 0, nb = 0;
    char flag[200]; bool tobedrawn = kFALSE;
    if(m_nev != 0)      nentries = m_nev;

    if(m_debug)         cout<<" Starting Event Loop "<<endl;

    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        if(m_debug) cout<<" New Eve "<<endl;
        nb = tree->GetEntry(jentry);   
        nbytes += nb;
        // if (Cut(ientry) < 0) continue;
        // if (jentry>1)  break;
        // if (jentry<33061)  continue;
        tagr->NextEvent();
        if(!(jentry%fr))        cout<<"Processed:: "<<jentry<<" evts!"<<endl;


        ///////////////  Call here your Process() functions    /////////////////////////////////////////////


        booter->Process( jentry );
        if (GlobalPar::GetPar()->IncludeBM())
          bmbooter->Process( jentry );


        ////////////////////////////////////////////////////////////////////////////////////////////////////


        if (!pg->IsEmpty() && tobedrawn && !(jentry%fr)) {
            pg->Modified();
            pg->Update();

            sprintf(flag,"plots/%s_%d","Test_MC",(int)jentry);
            pg->Print(flag);
        }


        if(m_debug) cout<<" Loaded Event:: "<<jentry<<endl;

        if ( GlobalPar::GetPar()->Debug() > 0 )         cout << "End event n: " << jentry << endl;

    }
    cout << "End of the event loop " << endl;

    booter->Finalize();
    if (GlobalPar::GetPar()->IncludeBM())
      bmbooter->Finalize();
    
    if(GlobalPar::GetPar()->IsPrintOutputFile())
      ControlPlotsRepository::GetControlObject( "BooterFinalize" )->SaveOutputFile();//close and save OutputFile
    
    tagr->EndEventLoop();
    
    my_out->Print();
    my_out->Close();

    // materialEffects->drawdEdx( 11 );  // to look at it in genfit

    return;
}


//recoloop for the bm calibration
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


  BMcalBooter* bmcalbooter = new BMcalBooter();
  bmcalbooter->Initialize(m_wd,m_instr);

  /***********  The event Loop   ****************************************   */    
  tagr->BeginEventLoop();
  Long64_t nentries = m_nev;
  //~ Long64_t nbytes = 0, nb = 0;
  char flag[200]; bool tobedrawn = kFALSE;

  //first loop to read the .dat file, charge the TABMdatRaw (without the T0!), evaluate T0,nentries etc.
  //~ for(unsigned int ifi=0; ifi<my_files.size(); ifi++) {//BE CARFUL!: actually, more input data files isn't supported/tested
    //~ if(m_debug)         
      //~ cout<<" Starting first Loop: reading "<<my_files.at(ifi).data()<<" file"<<endl;    
    //~ inF.open(myfiles.at(ifi).data(), ios::in | ios::binary);
    //~ while(inF.read((char *) &eSwSize,sizeof(int))) {//loop sul file .dat    

    //~ }
  //~ }//end of loop on files
  
  
 

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
  
  if(GlobalPar::GetPar()->IsPrintOutputFile())
    ControlPlotsRepository::GetControlObject( "BooterFinalize" )->SaveOutputFile();//close and save OutputFile
  
  tagr->EndEventLoop();
  
  //~ my_out->Print();
  //~ my_out->Close();


}













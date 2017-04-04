

#include "GlobalPar.hxx"




// Global static pointer used to ensure a single instance of the class.
GlobalPar* GlobalPar::m_pInstance = NULL;



GlobalPar* GlobalPar::Instance( string aparFileName )  {

    if (!m_pInstance) {  // Only allow one instance of class to be generated.
        // m_parFileName = ;
        m_pInstance = new GlobalPar( aparFileName );
    }
    else {
        cout << "ERROR::GlobalPar::Instance  -->  already instanced, cannot instance twice!!!" << endl, exit(0);
    }

    return m_pInstance;
}




GlobalPar* GlobalPar::GetPar()  {

    if (!m_pInstance) 
       cout << "ERROR::GlobalPar::GetPar()  -->  called a get before GlobalPar object istance." << endl, exit(0);

    return m_pInstance;
}




// private constructor
GlobalPar::GlobalPar( string aparFileName ) {

    m_parFileName = aparFileName;

	m_copyInputFile.clear();
    m_debug = 0;
    m_nLevelOfDebug = 4;    // from 0 to 4

    m_kalmanMode = -1;

    ReadParemFile();
}





void GlobalPar::ReadParemFile () {

	ifstream ifile;

    if ( !getenv("FOOTCONFIG") )     cout<< "ERROR::GlobalPar::ReadParemFile  -->  FOOTCONFIG env variable not set "<< endl, exit(0);

    ifile.open( ((string)getenv("FOOTCONFIG")+"/"+m_parFileName).c_str() );
    if ( !ifile.is_open() )        cout<< "ERROR  -->  wrong input in GlobalPar::ReadParemFile file "<< endl, exit(0);

    string line = "";
    while( getline( ifile, line ) ) {  

        if (line == "")  continue;
        if ( line.find("#") != string::npos || line.find("//") != string::npos )
            continue;

        m_copyInputFile.push_back(line);

        // remove spaces, not mandatory
        // if ( true )
        //     RemoveSpace( &line );

        

        if ( line.find("Debug:") != string::npos ) {
            m_debug = atoi ( StrReplace( line, "Debug:", "" ).c_str() );
        } 
        // inputFileListName
        // if ( line.find("InputFileListName:") != string::npos ) {
        //     inputFileListName = StrReplace( line, "InputFileListName:", "" );
        // } 
        // 
        else if ( line.find("MC Particle Types:") != string::npos ) {
            m_mcParticles.clear();
            string formulasString = StrReplace( line, "MC Particle Types:", "" );
            istringstream formulasStream( formulasString );
            string tmpString = "";
            while ( formulasStream >> tmpString )
                m_mcParticles.push_back(tmpString);
        } 

        if ( line.find("Kalman Mode:") != string::npos ) {
            vector<string> tmp_Modes = { "OFF", "ON", "ref", "daf", "dafsimple" };
            istringstream sss(  StrReplace( line, "Kalman Mode:", "" ) );
            
            string inputMode;
            sss >> inputMode;
            for (unsigned int i=0; i<tmp_Modes.size(); i++) {

                if ( IEquals( inputMode, tmp_Modes[i] ) ) {
                    m_kalmanMode = i;
                    break;
                }
            }
            if (m_kalmanMode == -1)         cout<< "ERROR  -->  Kalman Mode parameter "<< endl, exit(0);
        } 

        else if ( line.find("Tracking Systems Considered:") != string::npos ) {
            m_trackingSystems.clear();
            string formulasString = StrReplace( line, "Tracking Systems Considered:", "" );
            istringstream formulasStream( formulasString );
            string tmpString = "";
            while ( formulasStream >> tmpString )
                m_trackingSystems.push_back(tmpString);
        } 


        else if ( line.find("Reverse Tracking") != string::npos ) {
            if ( StrReplace( line, "Reverse Tracking:", "" ) == "true" )
                m_kalReverse = true;
            else
                m_kalReverse = false;
        } 

        else if ( line.find("Kalman Particle Types:") != string::npos ) {
            m_kalParticles.clear();
            string formulasString = StrReplace( line, "Kalman Particle Types:", "" );
            istringstream formulasStream( formulasString );
            string tmpString = "";
            while ( formulasStream >> tmpString )
                m_kalParticles.push_back(tmpString);
        } 
         
        // // btagging WP - default = 77%
        // else if ( line.find("BtaggingWP:") != string::npos ) {
        //     btaggingWP = atoi ( StrReplace( line, "BtaggingWP:", "" ).c_str() );
        // } 
        // // if we're using the B-filtered samples
        // else if ( line.find("UsingBFilter") != string::npos ) {
        //     m_useBFilter = true;
        // } 
        // // castingFormulas
        // else if ( line.find("Formulas:") != string::npos ) {
        //     castingFormulas.clear();
        //     string formulasString = StrReplace( line, "Formulas:", "" );
        //     istringstream formulasStream( formulasString );

        //     string tmpString = "";
        //     while ( formulasStream >> tmpString ){
        //         castingFormulas.push_back(tmpString);
        //     }

        // } 
    }


    // Check mandatory parameters set
    if ( m_debug < 0 || m_debug > m_nLevelOfDebug )     cout<< "ERROR :: GlobalPar.cxx  -->  wrong parameters config setting: debug level = "<< m_debug <<endl, exit(0);
    if ( m_trackingSystems.size() < 0 )		cout<< "ERROR :: GlobalPar.cxx  -->  wrong parameters config setting: m_trackingSystems ize = 0"<<endl, exit(0);

 
    ifile.close();

}




void GlobalPar::Print() {

    cout << endl << "========================   Input Parameters  =============================" << endl<<endl;
    for ( unsigned int cl=0; cl<m_copyInputFile.size(); cl++ ) {
        cout << m_copyInputFile[cl] << endl;
    }
    cout << endl <<  "===========================================================================" << endl<<endl;

}











 















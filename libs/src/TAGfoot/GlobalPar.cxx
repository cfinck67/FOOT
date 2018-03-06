

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

    m_kalReverse = false;
    m_geoROOT = true;
    m_geoFLUKA = false;


    ReadParamFile();

    m_nBin_map.clear();
    ReadHistoNBin( &m_nBin_map, "HistoNBin.dat" );

    m_map_range.clear();
    ReadHistoRange( &m_map_range, "HistoRange.dat" );

    m_map_saveDir.clear();
    ReadHistoString( &m_map_saveDir, "HistoSaveDir.dat" );

    m_map_xTitles.clear();
    ReadHistoString( &m_map_xTitles, "HistoXTitles.dat" );

    m_map_yTitles.clear();
    ReadHistoString( &m_map_yTitles, "HistoYTitles.dat" );

}





void GlobalPar::ReadParamFile () {

    ifstream ifile;

    if ( !getenv("FOOTCONFIG") )     cout<< "ERROR::GlobalPar::ReadParamFile  -->  FOOTCONFIG env variable not set "<< endl, exit(0);

    ifile.open( ((string)getenv("FOOTCONFIG")+"/"+m_parFileName).c_str() );
    if ( !ifile.is_open() )        cout<< "ERROR  -->  wrong input in GlobalPar::ReadParamFile file "<< endl, exit(0);

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


        else if ( line.find("Reverse Tracking:") != string::npos ) {
            string rev =StrReplace( line, "Reverse Tracking:", "" );
            RemoveSpace( &rev );
            if ( rev == "true" )
                m_kalReverse = true;
            else
                m_kalReverse = false;
        } 



        else if ( line.find("Create Reconstruction Geo:") != string::npos ) {
            string rev =StrReplace( line, "Create Reconstruction Geo:", "" );
            RemoveSpace( &rev );
            if ( rev == "y" )
                m_geoROOT = true;
            else
                m_geoROOT = false;
        } 


        else if ( line.find("Print FLUKA Geo input files:") != string::npos ) {
            string rev =StrReplace( line, "Print FLUKA Geo input files:", "" );
            RemoveSpace( &rev );
            if ( rev == "y" )
                m_geoFLUKA = true;
            else
                m_geoFLUKA = false;
        } 


        else if ( line.find("Kalman Particle Types:") != string::npos ) {
            m_kalParticles.clear();
            string formulasString = StrReplace( line, "Kalman Particle Types:", "" );
            istringstream formulasStream( formulasString );
            string tmpString = "";
            while ( formulasStream >> tmpString )
                m_kalParticles.push_back(tmpString);
        } 
         

        else if ( line.find("VT  Reso") != string::npos ) {
            m_VTreso = atof ( StrReplace( line, "VT  Reso:", "" ).c_str() );
        } 
        else if ( line.find("IT  Reso") != string::npos ) {
            m_ITreso = atof ( StrReplace( line, "IT  Reso:", "" ).c_str() );
        }
        else if ( line.find("MSD Reso") != string::npos ) {
            m_MSDreso = atof ( StrReplace( line, "MSD Reso:", "" ).c_str() );   
        }
        else if ( line.find("Print OutputFile") != string::npos ) {
            string rev =StrReplace( line, "Print OutputFile:", "" );
            RemoveSpace( &rev );
            if ( rev == "true" )
                m_printoutfile = true;
             else
                m_printoutfile = false;
        }
        else if ( line.find("Output Filename:") != string::npos ) {
            m_outputfilename = StrReplace( line, "Output Filename:", "" );
            RemoveSpace( &m_outputfilename );
        }
        else if ( line.find("Print OutputNtuple") != string::npos ) {
            string rev =StrReplace( line, "Print OutputNtuple:", "" );
            RemoveSpace( &rev );
                if ( rev == "true" )
                    m_printoutntuple = true;
                else 
                    m_printoutntuple = false;
        }
        else if ( line.find("Output Ntuplename:") != string::npos ) {
            m_outputntuplename = StrReplace( line, "Output Ntuplename:", "" );
            RemoveSpace( &m_outputntuplename );
        }

        else if ( line.find("IncludeBM:") != string::npos ) {
            string rev =StrReplace( line, "IncludeBM:", "" );   
            RemoveSpace( &rev );
            if ( rev == "y" )        m_includeBM = true;
            else                     m_includeBM = false;
        }
        else if ( line.find("IncludeIR:") != string::npos ) {
            string rev =StrReplace( line, "IncludeIR:", "" );   
            RemoveSpace( &rev );
            if ( rev == "y" )        m_includeIR = true;
            else                     m_includeIR = false;
        }
        else if ( line.find("IncludeTW:") != string::npos ) {
            string rev =StrReplace( line, "IncludeTW:", "" );   
            RemoveSpace( &rev );
            if ( rev == "y" )        m_includeTW = true;
            else                     m_includeTW = false;
        }
        else if ( line.find("IncludeMSD:") != string::npos ) {
            string rev =StrReplace( line, "IncludeMSD:", "" );   
            RemoveSpace( &rev );
            if ( rev == "y" )        m_includeMSD = true;
            else                     m_includeMSD = false;
        }
        else if ( line.find("IncludeCA:") != string::npos ) {
            string rev =StrReplace( line, "IncludeCA:", "" );   
            RemoveSpace( &rev );
            if ( rev == "y" )        m_includeCA = true;
            else                     m_includeCA = false;
        }
        else if ( line.find("IncludeVertex:") != string::npos ) {
            string rev =StrReplace( line, "IncludeVertex:", "" );   
            RemoveSpace( &rev );
            if ( rev == "y" )        m_includeVertex = true;
            else                     m_includeVertex = false;
        }
        else if ( line.find("IncludeInnerTracker:") != string::npos ) {
            string rev =StrReplace( line, "IncludeInnerTracker:", "" );   
            RemoveSpace( &rev );
            if ( rev == "y" )        m_includeInnerTracker = true;
            else                     m_includeInnerTracker = false;
        }
        else if ( line.find("IncludeKalman:") != string::npos ) {
            string rev =StrReplace( line, "IncludeKalman:", "" );   
            RemoveSpace( &rev );
            if ( rev == "y" )        m_includeKalman = true;
            else                     m_includeKalman = false;
        }
        else if ( line.find("IncludeEvent:") != string::npos ) {
            string rev =StrReplace( line, "IncludeEvent:", "" );   
            RemoveSpace( &rev );
            if ( rev == "y" )        m_includeEvent = true;
            else                     m_includeEvent = false;
        }

        else if ( line.find("Magnetic field map name:") != string::npos ) {
            m_magFieldMap = StrReplace( line, "Magnetic field map name:", "" );
            RemoveSpace( &m_magFieldMap );
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
    if ( m_trackingSystems.size() < 1 )     cout<< "ERROR :: GlobalPar.cxx  -->  wrong parameters config setting: m_trackingSystems ize = 0"<<endl, exit(0);

 
    ifile.close();

}



void GlobalPar::ReadHistoNBin( map< string, int >* map_bin, string fileName ) {

    ifstream ifile;
    ifile.open( ((string)getenv("FOOTCONFIG")+"/"+fileName).c_str() );
    // ifile.open( ((string)getenv("MAGIC_CONF")+"/VariableBinAssociation.dat").c_str() );
    // ifile.open( ((string)getenv("MAGIC_CONF")+"/MVAVariableBinAssociation.dat").c_str() );

    if ( !ifile.is_open() )        cout<< "wrong input file for the bin association"<< endl, exit(0);

    string line = "";
    while( getline( ifile, line ) ) {  

        if (line == "")  continue;
        if ( line.find("#") != string::npos || line.find("//") != string::npos )     continue;

        float binNumber = -1;
        string varName = "default";

        istringstream getall(line);
        getall  >> varName;
        getall  >> binNumber;

        (*map_bin)[varName] = binNumber;

    }  
    ifile.close();
}


void GlobalPar::ReadHistoString( map< string, string >* map_s, string fileName ) {

    ifstream ifile;
    ifile.open( ((string)getenv("FOOTCONFIG")+"/"+fileName).c_str() );

    if ( !ifile.is_open() )        cout<< "ERROR >> Reader::ReadHistoXTitles  ::  wrong input file for the x title"<< endl, exit(0);

    string line = "";
    while( getline( ifile, line ) ) {  

        if (line == "")  continue;
        if ( line.find("#") != string::npos || line.find("//") != string::npos )     continue;

        string title = "default";
        string varName = "default";

        istringstream getall(line);
        getall  >> varName;
        getall  >> title;

        (*map_s)[varName] = title;

    }  
    ifile.close();
}



void GlobalPar::ReadHistoRange( map< string, pair< double, double > >* map_range, string fileName ) {

    ifstream ifile;
    ifile.open( ((string)getenv("FOOTCONFIG")+"/"+fileName).c_str() );
    // ifile.open( ((string)getenv("MAGIC_CONF")+"/VariableRangeAssociation.dat").c_str() );
    // ifile.open( ((string)getenv("MAGIC_CONF")+"/MVAVariableRangeAssociation.dat").c_str() );

    if ( !ifile.is_open() )        cout<< "wrong input file for the range association"<< endl, exit(0);

    string line = "";
    while( getline( ifile, line ) ) {  

        if (line == "")  continue;
        if ( line.find("#") != string::npos || line.find("//") != string::npos )     continue;

        string varName = "default";
        pair< double, double > range;
        double minRange = 0;
        double maxRange = 0;

        istringstream getall(line);
        getall  >> varName;
        getall  >> minRange;
        getall  >> maxRange;

        // range.make_pair( minRange, maxRange );
        range.first = minRange;
        range.second = maxRange;
        (*map_range)[varName] = range;

    }  
    ifile.close();
}




void GlobalPar::Print() {

    cout << endl << "========================   Input Parameters  =============================" << endl<<endl;
    for ( unsigned int cl=0; cl<m_copyInputFile.size(); cl++ ) {
        cout << m_copyInputFile[cl] << endl;
    }
    cout << endl <<  "===========================================================================" << endl<<endl;

}











 















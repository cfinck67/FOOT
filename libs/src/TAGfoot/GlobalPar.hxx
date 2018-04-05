#ifndef GLOBALPAR_H
#define GLOBALPAR_H


#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <memory>
#include <map>


using namespace std;

// singleton class of global foot parameters
class GlobalPar {

public:

	static GlobalPar* Instance( string aparFileName = "FootGlobal.par" );
	static GlobalPar* GetPar();
	~GlobalPar() {};

	void ReadParamFile();
	void Print();

	int Debug() { return m_debug; };

	void ReadHistoNBin( map< string, int >* map_bin, string fileName );
	void ReadHistoString( map< string, string >* map, string fileName );
	void ReadHistoRange( map< string, pair< double, double > >* map_range, string fileName );

	int KalMode() { return m_kalmanMode; };
	bool IsKalReverse() { return m_kalReverse; };
	bool geoROOT() { return m_geoROOT; };
	bool geoFLUKA() { return m_geoFLUKA; };
	bool verFLUKA() { return m_verFLUKA; };
	vector<string> KalSystems() { return m_trackingSystems; };
	vector<string> KalParticles() { return m_kalParticles; };
	vector<string> MCParticles() { return m_mcParticles; };

	double VTReso() 	{return m_VTreso; };
    double ITReso() 	{return m_ITreso; };
    double MSDReso()        {return m_MSDreso; };

    bool IsPrintOutputFile() { return m_printoutfile; };
    string OutputFile() { return m_outputfilename; };
 
    bool IsPrintOutputNtuple() { return m_printoutntuple; };
    string OutputNtuple() { return m_outputntuplename; };
	
    bool IncludeBM() { return m_includeBM; };
    bool IncludeIR() { return m_includeIR; };
    bool IncludeTW() { return m_includeTW; };
    bool IncludeMSD() { return m_includeMSD; };
    bool IncludeCA() { return m_includeCA; };
    bool IncludeVertex() { return m_includeVertex; };
    bool IncludeInnerTracker() { return m_includeInnerTracker; };
    
    bool IncludeKalman() { return m_includeKalman; };
    bool IncludeEvent() { return m_includeEvent; };


    string MagFieldInputMapName() { return m_magFieldMap; };





    
	bool Find_MCParticle( string villain ) 
		{ return ( find( m_mcParticles.begin(), m_mcParticles.end(), villain ) == m_mcParticles.end() ? false : true ); };
	
	double GetLowBinHisto( string villain ) 	{ 
		for ( map< string, pair< double, double > >::iterator it = m_map_range.begin(); it != m_map_range.end(); it++ ) {
			if ( frankFind( (*it).first, villain ) )
				return (*it).second.first; 
		}
		return -666;
	};
	double GetUpBinHisto( string villain ) 	{ 
		for ( map< string, pair< double, double > >::iterator it = m_map_range.begin(); it != m_map_range.end(); it++ ) {
			if ( frankFind( (*it).first, villain ) )
				return (*it).second.second; 
		}
		return -666;
	};
	int GetNBinHisto( string villain )  	{ 
		for ( map< string, int >::iterator it = m_nBin_map.begin(); it != m_nBin_map.end(); it++ ) {
			if ( frankFind( (*it).first, villain ) )
				return (*it).second; 
		}
		return -666;
	};
	string GetSaveDirHisto( string villain )  	{ 
		for ( map< string, string >::iterator it = m_map_saveDir.begin(); it != m_map_saveDir.end(); it++ ) {
			if ( frankFind( (*it).first, villain ) )
				return (*it).second; 
		}
		return "default";
	};
	string GetXTitlesHisto( string villain )  	{ 
		for ( map< string, string >::iterator it = m_map_xTitles.begin(); it != m_map_xTitles.end(); it++ ) {
			if ( frankFind( (*it).first, villain ) )
				return (*it).second; 
		}
		return "default";
	};
	string GetYTitlesHisto( string villain )  	{ 
		for ( map< string, string >::iterator it = m_map_yTitles.begin(); it != m_map_yTitles.end(); it++ ) {
			if ( frankFind( (*it).first, villain ) )
				return (*it).second; 
		}
		return "default";
	};








	//____________________________________________________________________________
	void RemoveSpace( string* s ) {
	  s->erase( ::remove_if(s->begin(), s->end(), ::isspace), s->end() );
	}



	//Replace part of "original" if it founds "erase" with "add". Otherwise return input string.
	string StrReplace(string original, string erase, string add) {

		int found = original.find(erase);
		if ( (size_t)found != string::npos ) {
			int cutLength = erase.size();
			original.replace( found, cutLength, add );
			return original;
		}
		else {
			cout<<"not found "<<erase<<" in "<<original<<endl;
			return original;
		}
	}


	bool IEquals(const string& a, const string& b)	{

	    if (b.size() != a.size())
	        return false;
	    
	    for (unsigned int i = 0; i < a.size(); ++i) {
	        if (tolower(a[i]) != tolower(b[i])) {
	            return false;
	        }
	    }
	    return true;
	}




	bool frankFind( string what, string where )	{
	    
	    int wildcard_pos = what.find("*");
	    
	    if ( wildcard_pos == 0 )    {
	    	if( where.find( what.substr( wildcard_pos+1 ) ) != string::npos )
		        return true;
	    }
	    else if( wildcard_pos == what.size()-1 )    {
	    	if( where.find( what.substr( 0, wildcard_pos ) ) != string::npos )
		        return true;
	    }
	    else if ( wildcard_pos != string::npos )    {
	        int pre = where.find( what.substr( 0, wildcard_pos ) );
	        int post = where.find( what.substr( wildcard_pos+1 ) );
	        if( pre!=string::npos && post!=string::npos )
		        return true;
	    }

	    return false;
	}




private: 

	GlobalPar();
	GlobalPar( string aparFileName );
	static GlobalPar* m_pInstance;

	vector<string> m_copyInputFile;

	map< string, pair< double, double > > m_map_range;
	map< string, int > m_nBin_map;
	map< string, string > m_map_saveDir;
	map< string, string > m_map_xTitles;
	map< string, string > m_map_yTitles;

	string m_parFileName;

	int m_debug;
	int m_nLevelOfDebug;

	vector<string> m_mcParticles;
	
	int m_kalmanMode;
	bool m_kalReverse;
	bool m_geoROOT;
	bool m_geoFLUKA;
	bool m_verFLUKA;
	vector<string> m_trackingSystems;
	vector<string> m_kalParticles;

	double m_VTreso;
    double m_ITreso;
    double m_MSDreso; 

    string  m_outputfilename;
    bool m_printoutfile;
       
    string  m_outputntuplename;
    bool m_printoutntuple;

    string m_magFieldMap;

    bool m_includeBM;
    bool m_includeIR;
    bool m_includeTW;
    bool m_includeMSD;
    bool m_includeCA;
    bool m_includeInnerTracker;
    bool m_includeVertex;
    
    bool m_includeEvent;
    bool m_includeKalman;

};



// extern GlobalPar* gPar; 
// R__EXTERN GlobalPar  *gPar; 


#endif


























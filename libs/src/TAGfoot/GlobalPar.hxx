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



using namespace std;

// singleton class of global foot parameters
class GlobalPar {

public:

	static GlobalPar* Instance( string aparFileName = "FootGlobal.par" );
	static GlobalPar* GetPar();
	~GlobalPar() {};

	void ReadParemFile();
	void Print();

	int Debug() { return m_debug; };

	int KalMode() { return m_kalmanMode; };
	bool IsKalReverse() { return m_kalReverse; };
	vector<string> KalSystems() { return m_trackingSystems; };
	vector<string> KalParticles() { return m_kalParticles; };
	
	vector<string> MCParticles() { return m_mcParticles; };




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


private: 

	GlobalPar();
	GlobalPar( string aparFileName );
	static GlobalPar* m_pInstance;

	vector<string> m_copyInputFile;

	string m_parFileName;

	int m_debug;
	int m_nLevelOfDebug;

	vector<string> m_mcParticles;
	
	int m_kalmanMode;
	bool m_kalReverse;
	vector<string> m_trackingSystems;
	vector<string> m_kalParticles;
	

};



// extern GlobalPar* gPar; 
// R__EXTERN GlobalPar  *gPar; 


#endif


























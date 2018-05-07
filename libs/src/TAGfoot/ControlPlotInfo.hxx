#ifndef ControlPlotInfo_H
#define ControlPlotInfo_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <sys/stat.h>

#include "TCanvas.h"
#include "TH1F.h"
#include "TVector3.h"
#include "GlobalPar.hxx"
#include "MagicSkills.hxx"
#include "TTree.h"



using namespace std;

class ControlPlotInfo {
	
public:

	ControlPlotInfo() {};
	ControlPlotInfo( string name, double x ) {
		m_name = name;
		m_variablesDoub.clear();
		m_variablesDoub.push_back( x );
		isDoub = true;
		isInt = false;
		Initialise();
	};
	ControlPlotInfo( string name, int x ) {
		m_name = name;
		m_variablesInt.clear();
		m_variablesInt.push_back( x );
		isInt = true;
		isDoub = false;
		Initialise();
	};
	~ControlPlotInfo() {};


	void Initialise() {
		m_title = m_name;

		m_minBin = m_maxBin = -1;
		m_nBin = -1;

		m_titleX = "";
		m_titleY = "";
		m_dir = getenv("FOOTRES"); 
	};


	void addInfo( double x ) {
		if ( !isDoub )	{
			cout << "ERROR  >>  ControlPlotInfo::addInfo  ::  wrong type of the variable "<< m_name <<endl;
			exit(0);
		}
		m_variablesDoub.push_back( x );
	};

	void addInfo( int x ) {
		if ( !isInt )	{
			cout << "ERROR  >>  ControlPlotInfo::addInfo  ::  wrong type of the variable "<< m_name <<endl;
			exit(0);
		}
		m_variablesInt.push_back( x );
	};

	void SetOutputDir( string outDir ) { 
		vector<string> token = Tokenize( m_name, "__" );
		m_dir = outDir +"/"+ token[0]; 
	};

	void PrintHisto( TCanvas* mirror );



/***	variables	***/

	string m_name;
	string m_title;
	string m_titleX, m_titleY, m_dir;

	vector<double> m_variablesDoub;
	vector<double> m_variablesInt;

	bool isDoub;
	bool isInt;

	int m_nBin;
	double m_minBin;
	double m_maxBin;

	
	

private:


};

#endif
















#ifndef MATERIALS_HXX
#define MATERIALS_HXX


#include <TROOT.h>

#include <array>
#include <vector>
#include <map>
#include <iterator>

#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
// #include <vector>
// #include <math.h>
#include <algorithm>
// #include <memory>

#include <sys/types.h>
#include <sys/stat.h>
#include <limits>

#include <TGeoManager.h>

#include <TGeoMaterialInterface.h>

using namespace std;


#define build_string(expr) \
    (static_cast<ostringstream*>(&(ostringstream().flush() << expr))->str());


class Materials {

public:


	Materials();
	~Materials() {};



	TGeoMaterial* GetMaterial( string nome ) {
		// if (not find nome)	....
		return m_storeMat[nome];
	};

	void Print() {};

	void PrintMatMap();
  void PrintCompMap();


private:

	void FillStore();
	void ReadFile();
	void WriteMaterial(vector<string> tmpVecStr);
	
  string StrReplace(string original,string erase,string add);
  vector<string> StrSplit(const string& str, int splitLength);
  void RemoveSpace( string* s );
  void RemoveEmpty(vector<string>* tmpVecStr);

  bool ChooseIfWriteCompound(string provamarcopolo);
    
  void AppendCompound(vector<string> tmpVecStr);
  void ChooseHowToWriteCompound(vector<string> tmpVecStr);
  
  void WriteByVolume();
  void WriteByWeight();
  void WriteByAtoms();


  float m_compoundDensity;
  string m_tmpAppendCompoundName;
  vector<string> m_tmpCompoundData;
  
  map<string, TGeoMaterial*> m_storeMat;
  map<string, TGeoMixture*> m_storeComp;


};


#endif

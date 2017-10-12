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

	void ReadFile();

  //put Materials found in .inp in a map <string, TGeoMaterial*>; every material
  //could have different information (A,Z,rho)
	void WriteMaterial(vector<string> tmpVecStr);

  string StrReplace(string original,string erase,string add);
  vector<string> StrSplit(const string& str, int splitLength);
  void RemoveSpace( string* s );
  void RemoveEmpty(vector<string>* tmpVecStr);

  //evaluate if we can write compound or if we have to wait next line
  bool ChooseIfWriteCompound(string provamarcopolo);

  //Compound definition continues in next line so we decide to append information
  //of current line in a global vector to be written when compound definition is over
  void AppendCompound(vector<string> tmpVecStr);

  //Compound definition does NOT continue in next line so we write compound remembering
  //also previous information of compound. We have to decide how to write compound too:
  //by weight, by atoms, by volume
  void ChooseHowToWriteCompound(vector<string> tmpVecStr);

  void WriteByVolume();
  void WriteByWeight();
  void WriteByAtoms();


  float m_compoundDensity;//all comps are previously defined as materials with rho
  string m_tmpAppendCompoundName;
  vector<string> m_tmpCompoundData;

  map<string, TGeoMaterial*> m_storeMat; //map of materials
  map<string, TGeoMixture*> m_storeComp; //map of compound


};


#endif

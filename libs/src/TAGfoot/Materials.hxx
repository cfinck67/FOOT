#ifndef MATERIALS_HXX
#define MATERIALS_HXX


#include <TROOT.h>

#include <array>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <limits>

#include <TGeoManager.h>

// #include <TGeoMaterialInterface.h>

using namespace std;


#define build_string(expr) \
    (static_cast<ostringstream*>(&(ostringstream().flush() << expr))->str());


class Materials {

public:


	Materials( );
	~Materials() {};



	TGeoMaterial* GetMaterial( string nome ) {
		// if (not find nome)	....
		return m_store[nome];
	};

	void Print() {};

  void PrintMap();


private:

  void FillStore();
  void ReadFile();
  void WriteMaterial(vector<string> tmpVecStr);
  string StrReplace(string original,string erase,string add);

  map<string, TGeoMaterial*> m_store;

};





#endif


#ifndef _TAGmaterials_HXX
#define _TAGmaterials_HXX

#include <vector>
#include <map>

#include "TString.h"
#include "TGeoMaterial.h"

#include "TAGobject.hxx"

using namespace std;

class TGeoMixture;
class TGeoElementTable;

class TAGmaterials : public TAGobject
{
public:
   TAGmaterials();
   virtual ~TAGmaterials();
   
   TGeoMaterial* CreateMaterial(const TString formula, Float_t density, Float_t temperature = STP_temperature, Float_t pressure = STP_pressure);
   TGeoMixture*  CreateMixture(const TString formula, const TString densities, const TString prop, Float_t density);

   void          SaveFileFluka(const TString filename = "foot.inp");

public:
   static TAGmaterials* Instance();
   
private:
   static TAGmaterials* fgInstance;
   static map<TString, TString> fgkCommonName;
   static map<TString, Int_t>   fgkLowMat;

private:
   vector<TString>     fIsotope;
   vector<Int_t>       fIsotopeWeight;
   TGeoElementTable*   fTable;
   map<TString, Int_t> fPrintedElt;
   Int_t               fDegugLevel;
   
private:
   void    GetIsotopes(const TString formula);
   TString GetSubFormula(const TString formula);
   void    GetIsotopeAndWeight(const TString formula);
   void    CreateDefaultMaterials();
   vector<TString>  GetStrings(TString key, const Char_t delimiter = '/');
   void             GetCoeff(TString key, Float_t* coeff, Int_t size,  const Char_t delimiter = '/');

};

#endif

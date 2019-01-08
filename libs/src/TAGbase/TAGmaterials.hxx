
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
   
   TGeoMaterial* CreateMaterial(TString formula, Float_t density, Float_t temperature = STP_temperature, Float_t pressure = STP_pressure);
   TGeoMixture*  CreateMixture(TString formula, const TString densities, const TString prop, Float_t density);

   void          SaveFileFluka(const TString filename = "foot.inp");
   
public:
   static TAGmaterials* Instance();
   
private:
   static TAGmaterials*         fgInstance;
   static map<TString, TString> fgkCommonName;
   static map<TString, Int_t>   fgkLowMat;
   static TString               fgkWhat;
   static Int_t                 fgkWhatWidth;

private:
   vector<TString>              fIsotope;
   vector<Int_t>                fIsotopeWeight;
   TGeoElementTable*            fTable;
   map<TString, Int_t>          fPrintedElt;
   Int_t                        fDegugLevel;
   
private:
   void             GetIsotopes(const TString formula);
   TString          GetSubFormula(const TString formula);
   void             GetIsotopeAndWeight(const TString formula);
   void             CreateDefaultMaterials();
   vector<TString>  GetStrings(TString key, const Char_t delimiter = '/');
   void             GetCoeff(TString key, Float_t* coeff, Int_t size,  const Char_t delimiter = '/');
   TString          FindByValue(TString value);

   TString          AppendFluka(const Char_t* string, Int_t what = 1);
   TString          PrependFluka(const Char_t* string, Int_t what = 1);
   TString          PrependFlukaName(const Char_t* string, Int_t what = 1, Int_t align = 1);
   
   ClassDef(TAGmaterials,0)

};

#endif

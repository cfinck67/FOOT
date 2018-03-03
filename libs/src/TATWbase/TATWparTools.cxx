

#include "TMath.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TSystem.h"


#include "TATWparTools.hxx"

//##############################################################################

/*!
  \class TATWparTools TATWparTools.hxx "TATWparTools.hxx"
  \brief tools parameters for vertex. **
*/

ClassImp(TATWparTools);

//______________________________________________________________________________
TATWparTools::TATWparTools()
: TAGpara(),
  fFileStream(),
  fFileName(),
  fDebugLevel(0)
{
   // Standard constructor
}

//______________________________________________________________________________
TATWparTools::~TATWparTools()
{
   // Destructor
}

//_____________________________________________________________________________
void TATWparTools::ReadItem(TString& item)
{
   Int_t pos = -1;
   Char_t buf[255];
   TString key;
   while (pos == -1) {
	  fFileStream.getline(buf, 255);
	  key = buf;
	  pos = key.First(":");
   }
   item = key(pos+1, key.Length()-pos);   
   if (fDebugLevel > 1) cout << item.Data() << endl;
}

//_____________________________________________________________________________
void TATWparTools::ReadItem(Int_t &arg)
{
   TString item;
   ReadItem(item);
   arg = item.Atoi();
   
   if (fDebugLevel > 1) 
	  cout << arg << endl;
}

//_____________________________________________________________________________
void TATWparTools::ReadItem(Float_t &arg)
{
   TString item;
   ReadItem(item);
   arg = item.Atof();
   if (fDebugLevel > 1) 
	  cout << arg << endl;
}

//_____________________________________________________________________________
void TATWparTools::ReadItem(Float_t* coeff, Int_t size,  const Char_t delimiter)
{
   TString key;
   ReadItem(key);
   key = Normalize(key.Data());
   
   TObjArray* list = key.Tokenize(delimiter);
   if (list->GetEntries() != size)
	  Error("ReadItem(Float*, Int_t, const Char_t)", "wrong tokenize");

   
   for (Int_t k = 0; k < list->GetEntries(); k++) {
	  TObjString* obj = (TObjString*)list->At(k);
	  TString item = obj->GetString();
	  coeff[k] = item.Atof();
   }
   
   if (fDebugLevel > 1) {
	  for (Int_t i = 0; i < list->GetEntries(); ++i) {
		 cout << coeff[i] << " " ;      
	  }
	  cout << endl;
   }
   delete list;
}

//_____________________________________________________________________________
void TATWparTools::ReadVector3(TVector3 &arg)
{
   Float_t* co = new Float_t[3];
   ReadItem(co, 3, ':');
   arg.SetXYZ(co[0], co[1], co[2]); 
   delete co;
}

//_____________________________________________________________________________
void TATWparTools::ReadStrings(Char_t *aString, Int_t aLength)
{
   
   // reads a string of a given max length from configuration file
   // The strings is expected to be contained by double quotes : "
   
   TString key;
   ReadItem(key);
   
   Int_t pos = key.First('"');
   Int_t end = key.Last('"');
   
   if (end == -1 || pos == -1 || end-pos-1 > aLength) {
	  cout << "TATWparGeo: problem with reading configuration file, missing \" or line too long" << endl; 
	  return;
   }
   
   TString item(key(pos+1, end-pos-1));
   strcpy(aString, item.Data());
   aString[item.Length()] = '\0';
   
   if (fDebugLevel > 1) 
	  cout << aString << endl;
}

//_____________________________________________________________________________
TString TATWparTools::Normalize(const char* line)
{
   // Remove multiple blanks, and blanks in the begining/end.
   
   TString rv(line);
   
   if ( rv.Length() <= 0 ) return TString();
   
   while ( rv[0] == ' ' )
	  rv.Remove(0,1);
   
   while ( rv[rv.Length()-1] == ' ' )
	  rv.Remove(rv.Length()-1,1);
   
   Ssiz_t i(0);
   bool kill = false;
   
   for ( i = 0; i < rv.Length(); ++i ) {
	  if ( rv[i] == ' ' ) {
		 if (kill) {
			rv.Remove(i,1);
			--i;
		 } else
			kill = true;
	  } else 
		 kill = false;
   }
   
   return rv;
}

//______________________________________________________________________________
Bool_t TATWparTools::Open(const TString& name) 
{
   // openning file
   gSystem->ExpandPathName(name);
   
   fFileStream.open(name.Data());
   if (!fFileStream) {
	  Error("FromFile()", "failed to open file '%s'", name.Data());
	  return kTRUE;
   }
   
   fFileName = name;
   
   return kFALSE;
}

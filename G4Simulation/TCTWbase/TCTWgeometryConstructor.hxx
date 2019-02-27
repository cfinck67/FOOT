
#ifndef TCTWgeometryConstructor_h
#define TCTWgeometryConstructor_h 1

#include "TVector3.h"

#include "TCGbaseConstructor.hxx"

/** Building detector geometry
 
 \author Ch, Finck
 */

class G4LogicalVolume; //General class to define a logical volume (properties, material)

class TATWparGeo;

class TCTWgeometryConstructor : public TCGbaseConstructor
{
public:
   TCTWgeometryConstructor(TATWparGeo* pParGeo);
   virtual ~TCTWgeometryConstructor();
   
   virtual G4LogicalVolume* Construct(); //method in which the physical volume is constructed
   
   TVector3 GetBoxSize()     const { return fSizeBoxTof;  }
   TVector3 GetMinPoistion() const { return fMinPosition; }
   TVector3 GetMaxPoistion() const { return fMaxPosition; }
   
public:
   static const Char_t* GetSDname()   { return fgkTofSDname.Data();}
   
private:
   G4LogicalVolume* fBoxLog;
   G4LogicalVolume* fTofLog;
   TATWparGeo*      fpParGeo;
   TVector3         fSizeBoxTof;
   TVector3         fMinPosition;
   TVector3         fMaxPosition;
   
private:
   static TString fgkTofSDname;

private:
   void DefineMaxMinDimension();
   void DefineSensitive();
   void DefineMaterial();

};

#endif

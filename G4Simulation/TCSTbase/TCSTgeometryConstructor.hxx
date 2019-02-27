
#ifndef TCSTgeometryConstructor_h
#define TCSTgeometryConstructor_h 1

#include "TVector3.h"

#include "TCGbaseConstructor.hxx"

/** Building detector geometry
 
 \author Ch, Finck
 */

class G4LogicalVolume; //General class to define a logical volume (properties, material)

class TASTparGeo;

class TCSTgeometryConstructor : public TCGbaseConstructor
{
public:
   TCSTgeometryConstructor(TASTparGeo* pParGeo);
   virtual ~TCSTgeometryConstructor();
   
   virtual G4LogicalVolume* Construct(); //method in which the physical volume is constructed
   
   TVector3 GetBoxSize()     const  { return fSizeBoxIr;         }
   TVector3 GetMinPoistion() const  { return fMinPosition;       }
   TVector3 GetMaxPoistion() const  { return fMaxPosition;       }
   
public:
   static const Char_t* GetSDname() { return fgkIrSDname.Data(); }

   
private:
   G4LogicalVolume* fBoxLog;
   G4LogicalVolume* fIrLog;
   TASTparGeo*      fpParGeo;
   TVector3         fSizeBoxIr;
   TVector3         fMinPosition;
   TVector3         fMaxPosition;
   
private:
   static TString fgkIrSDname;

   
private:
   void DefineMaxMinDimension();
   void DefineSensitive();
   void DefineMaterial();

};

#endif

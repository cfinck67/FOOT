// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TAMSDparGeodOcint

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "TAMSDparGeo.hxx"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TAMSDparGeo(void *p = 0);
   static void *newArray_TAMSDparGeo(Long_t size, void *p);
   static void delete_TAMSDparGeo(void *p);
   static void deleteArray_TAMSDparGeo(void *p);
   static void destruct_TAMSDparGeo(void *p);
   static void streamer_TAMSDparGeo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TAMSDparGeo*)
   {
      ::TAMSDparGeo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TAMSDparGeo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TAMSDparGeo", ::TAMSDparGeo::Class_Version(), "TAMSDparGeo.hxx", 27,
                  typeid(::TAMSDparGeo), DefineBehavior(ptr, ptr),
                  &::TAMSDparGeo::Dictionary, isa_proxy, 16,
                  sizeof(::TAMSDparGeo) );
      instance.SetNew(&new_TAMSDparGeo);
      instance.SetNewArray(&newArray_TAMSDparGeo);
      instance.SetDelete(&delete_TAMSDparGeo);
      instance.SetDeleteArray(&deleteArray_TAMSDparGeo);
      instance.SetDestructor(&destruct_TAMSDparGeo);
      instance.SetStreamerFunc(&streamer_TAMSDparGeo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TAMSDparGeo*)
   {
      return GenerateInitInstanceLocal((::TAMSDparGeo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TAMSDparGeo*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TAMSDparGeo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TAMSDparGeo::Class_Name()
{
   return "TAMSDparGeo";
}

//______________________________________________________________________________
const char *TAMSDparGeo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparGeo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TAMSDparGeo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparGeo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TAMSDparGeo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparGeo*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TAMSDparGeo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparGeo*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TAMSDparGeo::Streamer(TBuffer &R__b)
{
   // Stream an object of class TAMSDparGeo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TAMSDparTools::Streamer(R__b);
      {
         TAMSDparGeo::SensorMatrix &R__stl =  m_sensorMatrix;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class std::vector<class IronPlate *, class std::allocator<class IronPlate *> >, class std::allocator<class std::vector<class IronPlate *, class std::allocator<class IronPlate *> > > >));
         if (R__tcl1==0) {
            Error("m_sensorMatrix streamer","Missing the TClass object for class std::vector<class std::vector<class IronPlate *, class std::allocator<class IronPlate *> >, class std::allocator<class std::vector<class IronPlate *, class std::allocator<class IronPlate *> > > >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<vector<IronPlate*> > R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> m_rotation;
      m_origin.Streamer(R__b);
      m_center.Streamer(R__b);
      m_dimension.Streamer(R__b);
      R__b >> m_nSensors_X;
      R__b >> m_nSensors_Y;
      R__b >> m_nSensors_Z;
      m_NSensors.Streamer(R__b);
      {
         vector<string> &R__stl =  m_materialOrder;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            string R__t;
            {TString R__str;
             R__str.Streamer(R__b);
             R__t = R__str.Data();}
            R__stl.push_back(R__t);
         }
      }
      {
         map<string,double> &R__stl =  m_materialThick;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            string R__t;
            {TString R__str;
             R__str.Streamer(R__b);
             R__t = R__str.Data();}
            double R__t2;
            R__b >> R__t2;
            typedef class std::basic_string<char> Value_t;
            std::pair<Value_t const, double > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         map<string,string> &R__stl =  m_materialType;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            string R__t;
            {TString R__str;
             R__str.Streamer(R__b);
             R__t = R__str.Data();}
            string R__t2;
            {TString R__str;
             R__str.Streamer(R__b);
             R__t2 = R__str.Data();}
            typedef class std::basic_string<char> Value_t;
            std::pair<Value_t const, class std::basic_string<char> > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> m_singleSensorThick_Lz;
      R__b >> m_layerDistance;
      R__b >> m_nPixel_X;
      R__b >> m_nPixel_Y;
      R__b.CheckByteCount(R__s, R__c, TAMSDparGeo::IsA());
   } else {
      R__c = R__b.WriteVersion(TAMSDparGeo::IsA(), kTRUE);
      TAMSDparTools::Streamer(R__b);
      {
         TAMSDparGeo::SensorMatrix &R__stl =  m_sensorMatrix;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class std::vector<class IronPlate *, class std::allocator<class IronPlate *> >, class std::allocator<class std::vector<class IronPlate *, class std::allocator<class IronPlate *> > > >));
         if (R__tcl1==0) {
            Error("m_sensorMatrix streamer","Missing the TClass object for class std::vector<class std::vector<class IronPlate *, class std::allocator<class IronPlate *> >, class std::allocator<class std::vector<class IronPlate *, class std::allocator<class IronPlate *> > > >!");
            return;
         }
            TAMSDparGeo::SensorMatrix::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<vector<IronPlate*> >*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << m_rotation;
      m_origin.Streamer(R__b);
      m_center.Streamer(R__b);
      m_dimension.Streamer(R__b);
      R__b << m_nSensors_X;
      R__b << m_nSensors_Y;
      R__b << m_nSensors_Z;
      m_NSensors.Streamer(R__b);
      {
         vector<string> &R__stl =  m_materialOrder;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<string>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {TString R__str((*R__k).c_str());
             R__str.Streamer(R__b);};
            }
         }
      }
      {
         map<string,double> &R__stl =  m_materialThick;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<string,double>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {TString R__str(((*R__k).first ).c_str());
             R__str.Streamer(R__b);};
            R__b << ((*R__k).second);
            }
         }
      }
      {
         map<string,string> &R__stl =  m_materialType;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<string,string>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {TString R__str(((*R__k).first ).c_str());
             R__str.Streamer(R__b);};
            {TString R__str(((*R__k).second).c_str());
             R__str.Streamer(R__b);};
            }
         }
      }
      R__b << m_singleSensorThick_Lz;
      R__b << m_layerDistance;
      R__b << m_nPixel_X;
      R__b << m_nPixel_Y;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TAMSDparGeo(void *p) {
      return  p ? new(p) ::TAMSDparGeo : new ::TAMSDparGeo;
   }
   static void *newArray_TAMSDparGeo(Long_t nElements, void *p) {
      return p ? new(p) ::TAMSDparGeo[nElements] : new ::TAMSDparGeo[nElements];
   }
   // Wrapper around operator delete
   static void delete_TAMSDparGeo(void *p) {
      delete ((::TAMSDparGeo*)p);
   }
   static void deleteArray_TAMSDparGeo(void *p) {
      delete [] ((::TAMSDparGeo*)p);
   }
   static void destruct_TAMSDparGeo(void *p) {
      typedef ::TAMSDparGeo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TAMSDparGeo(TBuffer &buf, void *obj) {
      ((::TAMSDparGeo*)obj)->::TAMSDparGeo::Streamer(buf);
   }
} // end of namespace ROOT for class ::TAMSDparGeo

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR(void *p = 0);
   static void *newArray_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<IronPlate*> > >*)
   {
      vector<vector<vector<IronPlate*> > > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<IronPlate*> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<IronPlate*> > >", -2, "vector", 214,
                  typeid(vector<vector<vector<IronPlate*> > >), DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<IronPlate*> > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<IronPlate*> > > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<IronPlate*> > >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<IronPlate*> > >*)0x0)->GetClass();
      vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<vector<vector<IronPlate*> > > : new vector<vector<vector<IronPlate*> > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<vector<vector<IronPlate*> > >[nElements] : new vector<vector<vector<IronPlate*> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<IronPlate*> > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<IronPlate*> > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEIronPlatemUgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<IronPlate*> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<IronPlate*> > >

namespace ROOT {
   static TClass *vectorlEvectorlEIronPlatemUgRsPgR_Dictionary();
   static void vectorlEvectorlEIronPlatemUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEIronPlatemUgRsPgR(void *p = 0);
   static void *newArray_vectorlEvectorlEIronPlatemUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEIronPlatemUgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEIronPlatemUgRsPgR(void *p);
   static void destruct_vectorlEvectorlEIronPlatemUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<IronPlate*> >*)
   {
      vector<vector<IronPlate*> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<IronPlate*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<IronPlate*> >", -2, "vector", 214,
                  typeid(vector<vector<IronPlate*> >), DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEIronPlatemUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<IronPlate*> >) );
      instance.SetNew(&new_vectorlEvectorlEIronPlatemUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEIronPlatemUgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEIronPlatemUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEIronPlatemUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEIronPlatemUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<IronPlate*> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<vector<IronPlate*> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEIronPlatemUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<IronPlate*> >*)0x0)->GetClass();
      vectorlEvectorlEIronPlatemUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEIronPlatemUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEIronPlatemUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<vector<IronPlate*> > : new vector<vector<IronPlate*> >;
   }
   static void *newArray_vectorlEvectorlEIronPlatemUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<vector<IronPlate*> >[nElements] : new vector<vector<IronPlate*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEIronPlatemUgRsPgR(void *p) {
      delete ((vector<vector<IronPlate*> >*)p);
   }
   static void deleteArray_vectorlEvectorlEIronPlatemUgRsPgR(void *p) {
      delete [] ((vector<vector<IronPlate*> >*)p);
   }
   static void destruct_vectorlEvectorlEIronPlatemUgRsPgR(void *p) {
      typedef vector<vector<IronPlate*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<IronPlate*> >

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = 0);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 214,
                  typeid(vector<string>), DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<string>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<string>*)0x0)->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete ((vector<string>*)p);
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] ((vector<string>*)p);
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace ROOT {
   static TClass *vectorlEIronPlatemUgR_Dictionary();
   static void vectorlEIronPlatemUgR_TClassManip(TClass*);
   static void *new_vectorlEIronPlatemUgR(void *p = 0);
   static void *newArray_vectorlEIronPlatemUgR(Long_t size, void *p);
   static void delete_vectorlEIronPlatemUgR(void *p);
   static void deleteArray_vectorlEIronPlatemUgR(void *p);
   static void destruct_vectorlEIronPlatemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<IronPlate*>*)
   {
      vector<IronPlate*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<IronPlate*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<IronPlate*>", -2, "vector", 214,
                  typeid(vector<IronPlate*>), DefineBehavior(ptr, ptr),
                  &vectorlEIronPlatemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<IronPlate*>) );
      instance.SetNew(&new_vectorlEIronPlatemUgR);
      instance.SetNewArray(&newArray_vectorlEIronPlatemUgR);
      instance.SetDelete(&delete_vectorlEIronPlatemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEIronPlatemUgR);
      instance.SetDestructor(&destruct_vectorlEIronPlatemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<IronPlate*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<IronPlate*>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEIronPlatemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<IronPlate*>*)0x0)->GetClass();
      vectorlEIronPlatemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEIronPlatemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEIronPlatemUgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<IronPlate*> : new vector<IronPlate*>;
   }
   static void *newArray_vectorlEIronPlatemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<IronPlate*>[nElements] : new vector<IronPlate*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEIronPlatemUgR(void *p) {
      delete ((vector<IronPlate*>*)p);
   }
   static void deleteArray_vectorlEIronPlatemUgR(void *p) {
      delete [] ((vector<IronPlate*>*)p);
   }
   static void destruct_vectorlEIronPlatemUgR(void *p) {
      typedef vector<IronPlate*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<IronPlate*>

namespace ROOT {
   static TClass *maplEstringcOstringgR_Dictionary();
   static void maplEstringcOstringgR_TClassManip(TClass*);
   static void *new_maplEstringcOstringgR(void *p = 0);
   static void *newArray_maplEstringcOstringgR(Long_t size, void *p);
   static void delete_maplEstringcOstringgR(void *p);
   static void deleteArray_maplEstringcOstringgR(void *p);
   static void destruct_maplEstringcOstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,string>*)
   {
      map<string,string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,string>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,string>", -2, "map", 96,
                  typeid(map<string,string>), DefineBehavior(ptr, ptr),
                  &maplEstringcOstringgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,string>) );
      instance.SetNew(&new_maplEstringcOstringgR);
      instance.SetNewArray(&newArray_maplEstringcOstringgR);
      instance.SetDelete(&delete_maplEstringcOstringgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOstringgR);
      instance.SetDestructor(&destruct_maplEstringcOstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,string> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<string,string>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,string>*)0x0)->GetClass();
      maplEstringcOstringgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOstringgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) map<string,string> : new map<string,string>;
   }
   static void *newArray_maplEstringcOstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) map<string,string>[nElements] : new map<string,string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOstringgR(void *p) {
      delete ((map<string,string>*)p);
   }
   static void deleteArray_maplEstringcOstringgR(void *p) {
      delete [] ((map<string,string>*)p);
   }
   static void destruct_maplEstringcOstringgR(void *p) {
      typedef map<string,string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,string>

namespace ROOT {
   static TClass *maplEstringcOdoublegR_Dictionary();
   static void maplEstringcOdoublegR_TClassManip(TClass*);
   static void *new_maplEstringcOdoublegR(void *p = 0);
   static void *newArray_maplEstringcOdoublegR(Long_t size, void *p);
   static void delete_maplEstringcOdoublegR(void *p);
   static void deleteArray_maplEstringcOdoublegR(void *p);
   static void destruct_maplEstringcOdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,double>*)
   {
      map<string,double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,double>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,double>", -2, "map", 96,
                  typeid(map<string,double>), DefineBehavior(ptr, ptr),
                  &maplEstringcOdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,double>) );
      instance.SetNew(&new_maplEstringcOdoublegR);
      instance.SetNewArray(&newArray_maplEstringcOdoublegR);
      instance.SetDelete(&delete_maplEstringcOdoublegR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOdoublegR);
      instance.SetDestructor(&destruct_maplEstringcOdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<string,double>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,double>*)0x0)->GetClass();
      maplEstringcOdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOdoublegR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) map<string,double> : new map<string,double>;
   }
   static void *newArray_maplEstringcOdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) map<string,double>[nElements] : new map<string,double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOdoublegR(void *p) {
      delete ((map<string,double>*)p);
   }
   static void deleteArray_maplEstringcOdoublegR(void *p) {
      delete [] ((map<string,double>*)p);
   }
   static void destruct_maplEstringcOdoublegR(void *p) {
      typedef map<string,double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,double>

namespace {
  void TriggerDictionaryInitialization_TAMSDparGeodOcint_Impl() {
    static const char* headers[] = {
"TAMSDparGeo.hxx",
0
    };
    static const char* includePaths[] = {
"/opt/exp_software/foot/root/current/include",
"../TAGbase",
"../TAGfoot",
"../../../Simulation/TXT2ROOT",
"../../../Simulation",
"../../../libs/GenFit/fitters/include",
"../../../libs/GenFit/core/include",
"/opt/exp_software/foot/root/root-6.04.18/include",
"/home/FOOT-T3/sartifoott3/software/libs/src/TAMSDbase/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$TAMSDparGeo.hxx")))  TAMSDparGeo;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TAMSDparGeo.hxx"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TAMSDparGeo", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TAMSDparGeo.cint",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TAMSDparGeodOcint_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TAMSDparGeodOcint_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TAMSDparGeodOcint() {
  TriggerDictionaryInitialization_TAMSDparGeodOcint_Impl();
}

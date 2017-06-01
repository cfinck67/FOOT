// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TAMSDparMapdOcint

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
#include "TAMSDparMap.hxx"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TAMSDparMap(void *p = 0);
   static void *newArray_TAMSDparMap(Long_t size, void *p);
   static void delete_TAMSDparMap(void *p);
   static void deleteArray_TAMSDparMap(void *p);
   static void destruct_TAMSDparMap(void *p);
   static void streamer_TAMSDparMap(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TAMSDparMap*)
   {
      ::TAMSDparMap *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TAMSDparMap >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TAMSDparMap", ::TAMSDparMap::Class_Version(), "TAMSDparMap.hxx", 15,
                  typeid(::TAMSDparMap), DefineBehavior(ptr, ptr),
                  &::TAMSDparMap::Dictionary, isa_proxy, 16,
                  sizeof(::TAMSDparMap) );
      instance.SetNew(&new_TAMSDparMap);
      instance.SetNewArray(&newArray_TAMSDparMap);
      instance.SetDelete(&delete_TAMSDparMap);
      instance.SetDeleteArray(&deleteArray_TAMSDparMap);
      instance.SetDestructor(&destruct_TAMSDparMap);
      instance.SetStreamerFunc(&streamer_TAMSDparMap);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TAMSDparMap*)
   {
      return GenerateInitInstanceLocal((::TAMSDparMap*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TAMSDparMap*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TAMSDparMap::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TAMSDparMap::Class_Name()
{
   return "TAMSDparMap";
}

//______________________________________________________________________________
const char *TAMSDparMap::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparMap*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TAMSDparMap::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparMap*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TAMSDparMap::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparMap*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TAMSDparMap::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparMap*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TAMSDparMap::Streamer(TBuffer &R__b)
{
   // Stream an object of class TAMSDparMap.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TAGpara::Streamer(R__b);
      R__b >> fPixelsNu;
      R__b >> fPixelsNv;
      R__b >> fPitchU;
      R__b >> fPitchV;
      R__b >> fPixelSize;
      R__b.CheckByteCount(R__s, R__c, TAMSDparMap::IsA());
   } else {
      R__c = R__b.WriteVersion(TAMSDparMap::IsA(), kTRUE);
      TAGpara::Streamer(R__b);
      R__b << fPixelsNu;
      R__b << fPixelsNv;
      R__b << fPitchU;
      R__b << fPitchV;
      R__b << fPixelSize;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TAMSDparMap(void *p) {
      return  p ? new(p) ::TAMSDparMap : new ::TAMSDparMap;
   }
   static void *newArray_TAMSDparMap(Long_t nElements, void *p) {
      return p ? new(p) ::TAMSDparMap[nElements] : new ::TAMSDparMap[nElements];
   }
   // Wrapper around operator delete
   static void delete_TAMSDparMap(void *p) {
      delete ((::TAMSDparMap*)p);
   }
   static void deleteArray_TAMSDparMap(void *p) {
      delete [] ((::TAMSDparMap*)p);
   }
   static void destruct_TAMSDparMap(void *p) {
      typedef ::TAMSDparMap current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TAMSDparMap(TBuffer &buf, void *obj) {
      ((::TAMSDparMap*)obj)->::TAMSDparMap::Streamer(buf);
   }
} // end of namespace ROOT for class ::TAMSDparMap

namespace {
  void TriggerDictionaryInitialization_TAMSDparMapdOcint_Impl() {
    static const char* headers[] = {
"TAMSDparMap.hxx",
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
class __attribute__((annotate("$clingAutoload$TAMSDparMap.hxx")))  TAMSDparMap;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TAMSDparMap.hxx"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TAMSDparMap", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TAMSDparMap.cint",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TAMSDparMapdOcint_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TAMSDparMapdOcint_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TAMSDparMapdOcint() {
  TriggerDictionaryInitialization_TAMSDparMapdOcint_Impl();
}

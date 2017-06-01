// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TAMSDdatRawdOcint

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
#include "TAMSDdatRaw.hxx"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TAMSDrawHit(void *p = 0);
   static void *newArray_TAMSDrawHit(Long_t size, void *p);
   static void delete_TAMSDrawHit(void *p);
   static void deleteArray_TAMSDrawHit(void *p);
   static void destruct_TAMSDrawHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TAMSDrawHit*)
   {
      ::TAMSDrawHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TAMSDrawHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TAMSDrawHit", ::TAMSDrawHit::Class_Version(), "TAMSDdatRaw.hxx", 15,
                  typeid(::TAMSDrawHit), DefineBehavior(ptr, ptr),
                  &::TAMSDrawHit::Dictionary, isa_proxy, 4,
                  sizeof(::TAMSDrawHit) );
      instance.SetNew(&new_TAMSDrawHit);
      instance.SetNewArray(&newArray_TAMSDrawHit);
      instance.SetDelete(&delete_TAMSDrawHit);
      instance.SetDeleteArray(&deleteArray_TAMSDrawHit);
      instance.SetDestructor(&destruct_TAMSDrawHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TAMSDrawHit*)
   {
      return GenerateInitInstanceLocal((::TAMSDrawHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TAMSDrawHit*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TAMSDdatRaw(void *p = 0);
   static void *newArray_TAMSDdatRaw(Long_t size, void *p);
   static void delete_TAMSDdatRaw(void *p);
   static void deleteArray_TAMSDdatRaw(void *p);
   static void destruct_TAMSDdatRaw(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TAMSDdatRaw*)
   {
      ::TAMSDdatRaw *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TAMSDdatRaw >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TAMSDdatRaw", ::TAMSDdatRaw::Class_Version(), "TAMSDdatRaw.hxx", 64,
                  typeid(::TAMSDdatRaw), DefineBehavior(ptr, ptr),
                  &::TAMSDdatRaw::Dictionary, isa_proxy, 4,
                  sizeof(::TAMSDdatRaw) );
      instance.SetNew(&new_TAMSDdatRaw);
      instance.SetNewArray(&newArray_TAMSDdatRaw);
      instance.SetDelete(&delete_TAMSDdatRaw);
      instance.SetDeleteArray(&deleteArray_TAMSDdatRaw);
      instance.SetDestructor(&destruct_TAMSDdatRaw);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TAMSDdatRaw*)
   {
      return GenerateInitInstanceLocal((::TAMSDdatRaw*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TAMSDdatRaw*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TAMSDrawHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TAMSDrawHit::Class_Name()
{
   return "TAMSDrawHit";
}

//______________________________________________________________________________
const char *TAMSDrawHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDrawHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TAMSDrawHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDrawHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TAMSDrawHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDrawHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TAMSDrawHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDrawHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TAMSDdatRaw::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TAMSDdatRaw::Class_Name()
{
   return "TAMSDdatRaw";
}

//______________________________________________________________________________
const char *TAMSDdatRaw::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDdatRaw*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TAMSDdatRaw::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDdatRaw*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TAMSDdatRaw::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDdatRaw*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TAMSDdatRaw::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDdatRaw*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TAMSDrawHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class TAMSDrawHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TAMSDrawHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(TAMSDrawHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TAMSDrawHit(void *p) {
      return  p ? new(p) ::TAMSDrawHit : new ::TAMSDrawHit;
   }
   static void *newArray_TAMSDrawHit(Long_t nElements, void *p) {
      return p ? new(p) ::TAMSDrawHit[nElements] : new ::TAMSDrawHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_TAMSDrawHit(void *p) {
      delete ((::TAMSDrawHit*)p);
   }
   static void deleteArray_TAMSDrawHit(void *p) {
      delete [] ((::TAMSDrawHit*)p);
   }
   static void destruct_TAMSDrawHit(void *p) {
      typedef ::TAMSDrawHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TAMSDrawHit

//______________________________________________________________________________
void TAMSDdatRaw::Streamer(TBuffer &R__b)
{
   // Stream an object of class TAMSDdatRaw.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TAMSDdatRaw::Class(),this);
   } else {
      R__b.WriteClassBuffer(TAMSDdatRaw::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TAMSDdatRaw(void *p) {
      return  p ? new(p) ::TAMSDdatRaw : new ::TAMSDdatRaw;
   }
   static void *newArray_TAMSDdatRaw(Long_t nElements, void *p) {
      return p ? new(p) ::TAMSDdatRaw[nElements] : new ::TAMSDdatRaw[nElements];
   }
   // Wrapper around operator delete
   static void delete_TAMSDdatRaw(void *p) {
      delete ((::TAMSDdatRaw*)p);
   }
   static void deleteArray_TAMSDdatRaw(void *p) {
      delete [] ((::TAMSDdatRaw*)p);
   }
   static void destruct_TAMSDdatRaw(void *p) {
      typedef ::TAMSDdatRaw current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TAMSDdatRaw

namespace {
  void TriggerDictionaryInitialization_TAMSDdatRawdOcint_Impl() {
    static const char* headers[] = {
"TAMSDdatRaw.hxx",
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
class __attribute__((annotate("$clingAutoload$TAMSDdatRaw.hxx")))  TAMSDrawHit;
class __attribute__((annotate("$clingAutoload$TAMSDdatRaw.hxx")))  TAMSDdatRaw;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TAMSDdatRaw.hxx"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TAMSDdatRaw", payloadCode, "@",
"TAMSDrawHit", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TAMSDdatRaw.cint",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TAMSDdatRawdOcint_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TAMSDdatRawdOcint_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TAMSDdatRawdOcint() {
  TriggerDictionaryInitialization_TAMSDdatRawdOcint_Impl();
}

// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TAMSDntuRawdOcint

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
#include "TAMSDntuRaw.hxx"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TAMSDntuHit(void *p = 0);
   static void *newArray_TAMSDntuHit(Long_t size, void *p);
   static void delete_TAMSDntuHit(void *p);
   static void deleteArray_TAMSDntuHit(void *p);
   static void destruct_TAMSDntuHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TAMSDntuHit*)
   {
      ::TAMSDntuHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TAMSDntuHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TAMSDntuHit", ::TAMSDntuHit::Class_Version(), "TAMSDntuRaw.hxx", 25,
                  typeid(::TAMSDntuHit), DefineBehavior(ptr, ptr),
                  &::TAMSDntuHit::Dictionary, isa_proxy, 4,
                  sizeof(::TAMSDntuHit) );
      instance.SetNew(&new_TAMSDntuHit);
      instance.SetNewArray(&newArray_TAMSDntuHit);
      instance.SetDelete(&delete_TAMSDntuHit);
      instance.SetDeleteArray(&deleteArray_TAMSDntuHit);
      instance.SetDestructor(&destruct_TAMSDntuHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TAMSDntuHit*)
   {
      return GenerateInitInstanceLocal((::TAMSDntuHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TAMSDntuHit*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TAMSDntuRaw(void *p = 0);
   static void *newArray_TAMSDntuRaw(Long_t size, void *p);
   static void delete_TAMSDntuRaw(void *p);
   static void deleteArray_TAMSDntuRaw(void *p);
   static void destruct_TAMSDntuRaw(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TAMSDntuRaw*)
   {
      ::TAMSDntuRaw *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TAMSDntuRaw >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TAMSDntuRaw", ::TAMSDntuRaw::Class_Version(), "TAMSDntuRaw.hxx", 161,
                  typeid(::TAMSDntuRaw), DefineBehavior(ptr, ptr),
                  &::TAMSDntuRaw::Dictionary, isa_proxy, 4,
                  sizeof(::TAMSDntuRaw) );
      instance.SetNew(&new_TAMSDntuRaw);
      instance.SetNewArray(&newArray_TAMSDntuRaw);
      instance.SetDelete(&delete_TAMSDntuRaw);
      instance.SetDeleteArray(&deleteArray_TAMSDntuRaw);
      instance.SetDestructor(&destruct_TAMSDntuRaw);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TAMSDntuRaw*)
   {
      return GenerateInitInstanceLocal((::TAMSDntuRaw*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TAMSDntuRaw*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TAMSDntuHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TAMSDntuHit::Class_Name()
{
   return "TAMSDntuHit";
}

//______________________________________________________________________________
const char *TAMSDntuHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDntuHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TAMSDntuHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDntuHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TAMSDntuHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDntuHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TAMSDntuHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDntuHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TAMSDntuRaw::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TAMSDntuRaw::Class_Name()
{
   return "TAMSDntuRaw";
}

//______________________________________________________________________________
const char *TAMSDntuRaw::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDntuRaw*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TAMSDntuRaw::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDntuRaw*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TAMSDntuRaw::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDntuRaw*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TAMSDntuRaw::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDntuRaw*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TAMSDntuHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class TAMSDntuHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TAMSDntuHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(TAMSDntuHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TAMSDntuHit(void *p) {
      return  p ? new(p) ::TAMSDntuHit : new ::TAMSDntuHit;
   }
   static void *newArray_TAMSDntuHit(Long_t nElements, void *p) {
      return p ? new(p) ::TAMSDntuHit[nElements] : new ::TAMSDntuHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_TAMSDntuHit(void *p) {
      delete ((::TAMSDntuHit*)p);
   }
   static void deleteArray_TAMSDntuHit(void *p) {
      delete [] ((::TAMSDntuHit*)p);
   }
   static void destruct_TAMSDntuHit(void *p) {
      typedef ::TAMSDntuHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TAMSDntuHit

//______________________________________________________________________________
void TAMSDntuRaw::Streamer(TBuffer &R__b)
{
   // Stream an object of class TAMSDntuRaw.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TAMSDntuRaw::Class(),this);
   } else {
      R__b.WriteClassBuffer(TAMSDntuRaw::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TAMSDntuRaw(void *p) {
      return  p ? new(p) ::TAMSDntuRaw : new ::TAMSDntuRaw;
   }
   static void *newArray_TAMSDntuRaw(Long_t nElements, void *p) {
      return p ? new(p) ::TAMSDntuRaw[nElements] : new ::TAMSDntuRaw[nElements];
   }
   // Wrapper around operator delete
   static void delete_TAMSDntuRaw(void *p) {
      delete ((::TAMSDntuRaw*)p);
   }
   static void deleteArray_TAMSDntuRaw(void *p) {
      delete [] ((::TAMSDntuRaw*)p);
   }
   static void destruct_TAMSDntuRaw(void *p) {
      typedef ::TAMSDntuRaw current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TAMSDntuRaw

namespace {
  void TriggerDictionaryInitialization_TAMSDntuRawdOcint_Impl() {
    static const char* headers[] = {
"TAMSDntuRaw.hxx",
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
class __attribute__((annotate(R"ATTRDUMP(Pixel or Pixel of a Detector Plane)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TAMSDntuRaw.hxx")))  TAMSDntuHit;
class __attribute__((annotate("$clingAutoload$TAMSDntuRaw.hxx")))  TAMSDntuRaw;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TAMSDntuRaw.hxx"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TAMSDntuHit", payloadCode, "@",
"TAMSDntuRaw", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TAMSDntuRaw.cint",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TAMSDntuRawdOcint_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TAMSDntuRawdOcint_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TAMSDntuRawdOcint() {
  TriggerDictionaryInitialization_TAMSDntuRawdOcint_Impl();
}

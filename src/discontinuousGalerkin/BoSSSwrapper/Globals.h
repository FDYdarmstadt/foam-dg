// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 15.04.2021 17:47:02
// ##########################################################################
#pragma once
namespace BoSSS
{
class Globals
{
public:
static MonoDomain* _domain;
static void Init(char* ManagedAssemblyDirectory);
static MonoAssembly* _assembly__BoSSS_Foundation;
static MonoImage* _image__BoSSS_Foundation;
static MonoAssembly* _assembly__BoSSS_Platform;
static MonoImage* _image__BoSSS_Platform;
static MonoAssembly* _assembly__BoSSS_Foundation_Grid;
static MonoImage* _image__BoSSS_Foundation_Grid;
static MonoAssembly* _assembly__BoSSS_Application_ExternalBinding;
static MonoImage* _image__BoSSS_Application_ExternalBinding;
static MonoClass*  LookupClass(MonoImage* image, const char *_classname, const char* _namespace);
static MonoMethod* LookupMethod(MonoClass* pClass, const char *name, mono_bool include_namespace);
private:
static int _Initialized;
};
}

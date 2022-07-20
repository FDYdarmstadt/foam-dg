// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 7/14/2022 1:09:54 PM
// ##########################################################################
#include <stdlib.h>
#include <assert.h>
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>

#include <mono/metadata/mono-config.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include "Globals.h"
#include "Globals.h"
namespace BoSSS
{
MonoDomain* Globals::_domain = NULL;
int Globals::_Initialized = 0;
void Globals::Init(char* ManagedAssemblyDirectory)
{
if (_Initialized != 0)
   return;
_Initialized = 0x1234;

char path[1028];
mono_config_parse(NULL);
_domain = mono_jit_init_version("BoSSSdomain", "v4.0.30319");
if (_domain == NULL) {
    fprintf(stderr, "Unable to setup mono domain.");
    throw "Unable to setup mono domain.";
}
mono_domain_set_config(_domain, ManagedAssemblyDirectory, "");
if(strlen(ManagedAssemblyDirectory) + 21 >= 1024) {
    fprintf(stderr, "Path to assembly BoSSS.Foundation.dll exceeds 1024 character limit. \n "); 
}
strcpy(path, ManagedAssemblyDirectory);
strcat(path, "BoSSS.Foundation.dll");
_assembly__BoSSS_Foundation = mono_domain_assembly_open(_domain, path);
if (_assembly__BoSSS_Foundation == NULL) {
    fprintf(stderr, "Unable to open assembly: BoSSS.Foundation.dll \n "); 
    throw "Unable to open assembly.";
}
_image__BoSSS_Foundation = mono_assembly_get_image(_assembly__BoSSS_Foundation);
if (_image__BoSSS_Foundation == NULL) {
    fprintf(stderr, "Unable to get assembly image BoSSS.Foundation.dll.\n");
    throw "Unable to get assembly image.";
}
if(strlen(ManagedAssemblyDirectory) + 26 >= 1024) {
    fprintf(stderr, "Path to assembly BoSSS.Foundation.Grid.dll exceeds 1024 character limit. \n "); 
}
strcpy(path, ManagedAssemblyDirectory);
strcat(path, "BoSSS.Foundation.Grid.dll");
_assembly__BoSSS_Foundation_Grid = mono_domain_assembly_open(_domain, path);
if (_assembly__BoSSS_Foundation_Grid == NULL) {
    fprintf(stderr, "Unable to open assembly: BoSSS.Foundation.Grid.dll \n "); 
    throw "Unable to open assembly.";
}
_image__BoSSS_Foundation_Grid = mono_assembly_get_image(_assembly__BoSSS_Foundation_Grid);
if (_image__BoSSS_Foundation_Grid == NULL) {
    fprintf(stderr, "Unable to get assembly image BoSSS.Foundation.Grid.dll.\n");
    throw "Unable to get assembly image.";
}
if(strlen(ManagedAssemblyDirectory) + 38 >= 1024) {
    fprintf(stderr, "Path to assembly BoSSS.Application.ExternalBinding.dll exceeds 1024 character limit. \n "); 
}
strcpy(path, ManagedAssemblyDirectory);
strcat(path, "BoSSS.Application.ExternalBinding.dll");
_assembly__BoSSS_Application_ExternalBinding = mono_domain_assembly_open(_domain, path);
if (_assembly__BoSSS_Application_ExternalBinding == NULL) {
    fprintf(stderr, "Unable to open assembly: BoSSS.Application.ExternalBinding.dll \n "); 
    throw "Unable to open assembly.";
}
_image__BoSSS_Application_ExternalBinding = mono_assembly_get_image(_assembly__BoSSS_Application_ExternalBinding);
if (_image__BoSSS_Application_ExternalBinding == NULL) {
    fprintf(stderr, "Unable to get assembly image BoSSS.Application.ExternalBinding.dll.\n");
    throw "Unable to get assembly image.";
}
}
MonoAssembly* Globals::_assembly__BoSSS_Foundation = NULL;
MonoImage* Globals::_image__BoSSS_Foundation = NULL;
MonoAssembly* Globals::_assembly__BoSSS_Foundation_Grid = NULL;
MonoImage* Globals::_image__BoSSS_Foundation_Grid = NULL;
MonoAssembly* Globals::_assembly__BoSSS_Application_ExternalBinding = NULL;
MonoImage* Globals::_image__BoSSS_Application_ExternalBinding = NULL;
MonoClass*  Globals::LookupClass(MonoImage* image, const char *_classname, const char* _namespace)
{
if(image == NULL) {
    fprintf(stderr, "Image not loaded - must call MonoBoSSSglobals::Init first.\n");
}
MonoClass* mcls = mono_class_from_name(image, _namespace, _classname);
if (mcls == NULL) {
    fprintf(stderr, "unable to find class %s.%s\n", _namespace, _classname);
    throw "unable to find method description for method. ";
}
return mcls;
}
MonoMethod* Globals::LookupMethod(MonoClass* pClass, const char *name, mono_bool include_namespace)
{
MonoMethodDesc* desc = mono_method_desc_new(name, include_namespace);
if (desc == NULL) {
    fprintf(stderr, "unable to find method description for method %s\n", name);
    throw "unable to find method description for method. ";
}
MonoMethod* methodHandle = mono_method_desc_search_in_class(desc, pClass);
if (methodHandle == NULL) {
    fprintf(stderr, "unable to find method handle for method.\n");
    throw "unable to find method handle for method. ";
}
mono_method_desc_free(desc);
return methodHandle;
}
}

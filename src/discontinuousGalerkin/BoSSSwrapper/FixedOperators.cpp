// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 7/11/2022 12:42:12 PM
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
#include "Prototypes.h"
#include "GridData.h"
#include "OpenFOAMGrid.h"
#include "FixedOperators.h"
#include "Initializer.h"
#include "OpenFoamDGField.h"
#include "OpenFoamMatrix.h"
#include "OpenFoamPatchField.h"
namespace BoSSS
{
namespace Application
{
namespace ExternalBinding
{
void FixedOperators::_InitMonoBindings()
{
_ClassHandle = BoSSS::Globals::LookupClass(BoSSS::Globals::_image__BoSSS_Application_ExternalBinding, "FixedOperators", "BoSSS.Application.ExternalBinding");
_ctor_0 = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.FixedOperators:.ctor", true);
__SetForeignPointer = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.FixedOperators:_SetForeignPointer", true);
__GetForeignPointer = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.FixedOperators:_GetForeignPointer", true);
_Laplacian = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.FixedOperators:Laplacian", true);
}
FixedOperators::~FixedOperators()
{
if (_MonoGCHandle != 0)
{
// See also _ReleaseGChandle()  and _FromMonoObject(MonoObject*) methods:
// For a temporary object creation through _FromMonoObject(...), we only need parts of the destruction, therefore parts of the destructor are blocked.
_SetForeignPointer(NULL);
mono_gchandle_free(_MonoGCHandle);
}
}
void FixedOperators::_ReleaseGChandle()
{
mono_gchandle_free(_MonoGCHandle);
_MonoGCHandle = 0; // blocks destructor functionality
}
FixedOperators::FixedOperators(MonoObject* mo)
{
_InitMonoBindings();
_MonoGCHandle = mono_gchandle_new(mo, true);
}
FixedOperators* FixedOperators::_FromMonoObject(MonoObject* mo)
{
FixedOperators* tmp = new FixedOperators(mo);
void* LoggedRef = tmp->_GetForeignPointer();
if (LoggedRef != NULL) {
    tmp->_ReleaseGChandle();
    delete tmp;
    return ((FixedOperators*)LoggedRef);
} else {
    tmp->_SetForeignPointer(tmp);
    return tmp;
}
}
MonoObject* FixedOperators::_GetMonoObject()
{
return mono_gchandle_get_target(_MonoGCHandle);
}
FixedOperators::FixedOperators()
{
_InitMonoBindings();
MonoObject* ThisObj = mono_object_new(BoSSS::Globals::_domain, _ClassHandle);
_MonoGCHandle = mono_gchandle_new(ThisObj, true);
void* args[1];
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_ctor_0, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (FixedOperators.ctor_0) \n");
}
_SetForeignPointer(this);
}
void FixedOperators::_SetForeignPointer(void* ptr)
{
void* args[1];
args[0] = &ptr;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(__SetForeignPointer, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (FixedOperators._SetForeignPointer) \n");
}
return;
}
void* FixedOperators::_GetForeignPointer()
{
void* args[1];
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(__GetForeignPointer, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (FixedOperators._GetForeignPointer) \n");
}
void* retptr = mono_object_unbox(retval);
return *((void**) retptr);
}
void FixedOperators::Laplacian(BoSSS::Application::ExternalBinding::OpenFoamMatrix* mtx, BoSSS::Application::ExternalBinding::OpenFoamPatchField* ptch)
{
void* args[2];
args[0] = mtx->_GetMonoObject();
args[1] = ptch->_GetMonoObject();
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_Laplacian, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (FixedOperators.Laplacian) \n");
}
return;
}
}
}
}

// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 03.05.2021 13:19:28
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
#include "Initializer.h"
#include "FixedOperators.h"
#include "OpenFoamDGField.h"
#include "OpenFoamMatrix.h"
namespace BoSSS
{
namespace Application
{
namespace ExternalBinding
{
void OpenFoamDGField::_InitMonoBindings()
{
_ClassHandle = BoSSS::Globals::LookupClass(BoSSS::Globals::_image__BoSSS_Application_ExternalBinding, "OpenFoamDGField", "BoSSS.Application.ExternalBinding");
_ctor_0 = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamDGField:.ctor", true);
_SetDGcoordinate = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamDGField:SetDGcoordinate", true);
_GetDGcoordinate = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamDGField:GetDGcoordinate", true);
__SetForeignPointer = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamDGField:_SetForeignPointer", true);
__GetForeignPointer = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamDGField:_GetForeignPointer", true);
}
OpenFoamDGField::~OpenFoamDGField()
{
if (_MonoGCHandle != 0)
{
// See also _ReleaseGChandle()  and _FromMonoObject(MonoObject*) methods:
// For a temporary object creation through _FromMonoObject(...), we only need parts of the destruction, therefore parts of the destructor are blocked.
_SetForeignPointer(NULL);
mono_gchandle_free(_MonoGCHandle);
}
}
void OpenFoamDGField::_ReleaseGChandle()
{
mono_gchandle_free(_MonoGCHandle);
_MonoGCHandle = 0; // blocks destructor functionality
}
OpenFoamDGField::OpenFoamDGField(MonoObject* mo)
{
_InitMonoBindings();
_MonoGCHandle = mono_gchandle_new(mo, true);
}
OpenFoamDGField* OpenFoamDGField::_FromMonoObject(MonoObject* mo)
{
OpenFoamDGField* tmp = new OpenFoamDGField(mo);
void* LoggedRef = tmp->_GetForeignPointer();
if (LoggedRef != NULL) {
    tmp->_ReleaseGChandle();
    delete tmp;
    return ((OpenFoamDGField*)LoggedRef);
} else {
    tmp->_SetForeignPointer(tmp);
    return tmp;
}
}
MonoObject* OpenFoamDGField::_GetMonoObject()
{
return mono_gchandle_get_target(_MonoGCHandle);
}
OpenFoamDGField::OpenFoamDGField(BoSSS::Foundation::Grid::OpenFOAMGrid* g, int degree, int NoOfComponents)
{
_InitMonoBindings();
MonoObject* ThisObj = mono_object_new(BoSSS::Globals::_domain, _ClassHandle);
_MonoGCHandle = mono_gchandle_new(ThisObj, true);
void* args[3];
args[0] = g->_GetMonoObject();
args[1] = &degree;
args[2] = &NoOfComponents;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_ctor_0, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamDGField.ctor_0) \n");
}
_SetForeignPointer(this);
}
void OpenFoamDGField::SetDGcoordinate(int f, int j, int n, double val)
{
void* args[4];
args[0] = &f;
args[1] = &j;
args[2] = &n;
args[3] = &val;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_SetDGcoordinate, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamDGField.SetDGcoordinate) \n");
}
return;
}
double OpenFoamDGField::GetDGcoordinate(int f, int j, int n)
{
void* args[3];
args[0] = &f;
args[1] = &j;
args[2] = &n;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_GetDGcoordinate, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamDGField.GetDGcoordinate) \n");
}
void* retptr = mono_object_unbox(retval);
return *((double*) retptr);
}
void OpenFoamDGField::_SetForeignPointer(void* ptr)
{
void* args[1];
args[0] = &ptr;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(__SetForeignPointer, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamDGField._SetForeignPointer) \n");
}
return;
}
void* OpenFoamDGField::_GetForeignPointer()
{
void* args[1];
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(__GetForeignPointer, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamDGField._GetForeignPointer) \n");
}
void* retptr = mono_object_unbox(retval);
return *((void**) retptr);
}
}
}
}

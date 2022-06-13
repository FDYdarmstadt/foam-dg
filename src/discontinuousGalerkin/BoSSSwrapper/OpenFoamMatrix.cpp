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
void OpenFoamMatrix::_InitMonoBindings()
{
_ClassHandle = BoSSS::Globals::LookupClass(BoSSS::Globals::_image__BoSSS_Application_ExternalBinding, "OpenFoamMatrix", "BoSSS.Application.ExternalBinding");
_ctor_0 = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamMatrix:.ctor", true);
_GetRHScoordinate = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamMatrix:GetRHScoordinate", true);
_GetSolCoordinate = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamMatrix:GetSolCoordinate", true);
_Solve = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamMatrix:Solve", true);
_GetNoOfRowsInBlock = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamMatrix:GetNoOfRowsInBlock", true);
_GetNoOfColsInBlock = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamMatrix:GetNoOfColsInBlock", true);
_GetBlock = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamMatrix:GetBlock", true);
_AccBlock = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamMatrix:AccBlock", true);
_ClearBlock = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamMatrix:ClearBlock", true);
__SetForeignPointer = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamMatrix:_SetForeignPointer", true);
__GetForeignPointer = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Application.ExternalBinding.OpenFoamMatrix:_GetForeignPointer", true);
}
OpenFoamMatrix::~OpenFoamMatrix()
{
if (_MonoGCHandle != 0)
{
// See also _ReleaseGChandle()  and _FromMonoObject(MonoObject*) methods:
// For a temporary object creation through _FromMonoObject(...), we only need parts of the destruction, therefore parts of the destructor are blocked.
_SetForeignPointer(NULL);
mono_gchandle_free(_MonoGCHandle);
}
}
void OpenFoamMatrix::_ReleaseGChandle()
{
mono_gchandle_free(_MonoGCHandle);
_MonoGCHandle = 0; // blocks destructor functionality
}
OpenFoamMatrix::OpenFoamMatrix(MonoObject* mo)
{
_InitMonoBindings();
_MonoGCHandle = mono_gchandle_new(mo, true);
}
OpenFoamMatrix* OpenFoamMatrix::_FromMonoObject(MonoObject* mo)
{
OpenFoamMatrix* tmp = new OpenFoamMatrix(mo);
void* LoggedRef = tmp->_GetForeignPointer();
if (LoggedRef != NULL) {
    tmp->_ReleaseGChandle();
    delete tmp;
    return ((OpenFoamMatrix*)LoggedRef);
} else {
    tmp->_SetForeignPointer(tmp);
    return tmp;
}
}
MonoObject* OpenFoamMatrix::_GetMonoObject()
{
return mono_gchandle_get_target(_MonoGCHandle);
}
OpenFoamMatrix::OpenFoamMatrix(BoSSS::Application::ExternalBinding::OpenFoamDGField* f)
{
_InitMonoBindings();
MonoObject* ThisObj = mono_object_new(BoSSS::Globals::_domain, _ClassHandle);
_MonoGCHandle = mono_gchandle_new(ThisObj, true);
void* args[1];
args[0] = f->_GetMonoObject();
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_ctor_0, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamMatrix.ctor_0) \n");
}
_SetForeignPointer(this);
}
double OpenFoamMatrix::GetRHScoordinate(int f, int j, int n)
{
void* args[3];
args[0] = &f;
args[1] = &j;
args[2] = &n;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_GetRHScoordinate, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamMatrix.GetRHScoordinate) \n");
}
void* retptr = mono_object_unbox(retval);
return *((double*) retptr);
}
double OpenFoamMatrix::GetSolCoordinate(int f, int j, int n)
{
void* args[3];
args[0] = &f;
args[1] = &j;
args[2] = &n;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_GetSolCoordinate, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamMatrix.GetSolCoordinate) \n");
}
void* retptr = mono_object_unbox(retval);
return *((double*) retptr);
}
void OpenFoamMatrix::Solve()
{
void* args[1];
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_Solve, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamMatrix.Solve) \n");
}
return;
}
int OpenFoamMatrix::GetNoOfRowsInBlock(int iLocal)
{
void* args[1];
args[0] = &iLocal;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_GetNoOfRowsInBlock, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamMatrix.GetNoOfRowsInBlock) \n");
}
void* retptr = mono_object_unbox(retval);
return *((int*) retptr);
}
int OpenFoamMatrix::GetNoOfColsInBlock(int iLocal)
{
void* args[1];
args[0] = &iLocal;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_GetNoOfColsInBlock, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamMatrix.GetNoOfColsInBlock) \n");
}
void* retptr = mono_object_unbox(retval);
return *((int*) retptr);
}
void OpenFoamMatrix::GetBlock(int iRowLocal, int jColLocal, double* InputReadBuffer)
{
void* args[3];
args[0] = &iRowLocal;
args[1] = &jColLocal;
args[2] = InputReadBuffer;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_GetBlock, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamMatrix.GetBlock) \n");
}
return;
}
void OpenFoamMatrix::AccBlock(int iRowLocal, int jColLocal, double alpha, double* InputReadBuffer)
{
void* args[4];
args[0] = &iRowLocal;
args[1] = &jColLocal;
args[2] = &alpha;
args[3] = InputReadBuffer;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_AccBlock, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamMatrix.AccBlock) \n");
}
return;
}
void OpenFoamMatrix::ClearBlock(int iRowLocal, int jColLocal)
{
void* args[2];
args[0] = &iRowLocal;
args[1] = &jColLocal;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_ClearBlock, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamMatrix.ClearBlock) \n");
}
return;
}
void OpenFoamMatrix::_SetForeignPointer(void* ptr)
{
void* args[1];
args[0] = &ptr;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(__SetForeignPointer, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamMatrix._SetForeignPointer) \n");
}
return;
}
void* OpenFoamMatrix::_GetForeignPointer()
{
void* args[1];
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(__GetForeignPointer, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamMatrix._GetForeignPointer) \n");
}
void* retptr = mono_object_unbox(retval);
return *((void**) retptr);
}
}
}
}

// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 9/2/2022 4:14:20 PM
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
namespace Foundation
{
namespace Grid
{
void OpenFOAMGrid::_InitMonoBindings()
{
_ClassHandle = BoSSS::Globals::LookupClass(BoSSS::Globals::_image__BoSSS_Foundation_Grid, "OpenFOAMGrid", "BoSSS.Foundation.Grid");
_ctor_0 = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Foundation.Grid.OpenFOAMGrid:.ctor", true);
__SetForeignPointer = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Foundation.Grid.OpenFOAMGrid:_SetForeignPointer", true);
__GetForeignPointer = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Foundation.Grid.OpenFOAMGrid:_GetForeignPointer", true);
_TestMethod = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Foundation.Grid.OpenFOAMGrid:TestMethod", true);
_GetGridData = BoSSS::Globals::LookupMethod(_ClassHandle, "BoSSS.Foundation.Grid.OpenFOAMGrid:GetGridData", true);
}
OpenFOAMGrid::~OpenFOAMGrid()
{
if (_MonoGCHandle != 0)
{
// See also _ReleaseGChandle()  and _FromMonoObject(MonoObject*) methods:
// For a temporary object creation through _FromMonoObject(...), we only need parts of the destruction, therefore parts of the destructor are blocked.
_SetForeignPointer(NULL);
mono_gchandle_free(_MonoGCHandle);
}
}
void OpenFOAMGrid::_ReleaseGChandle()
{
mono_gchandle_free(_MonoGCHandle);
_MonoGCHandle = 0; // blocks destructor functionality
}
OpenFOAMGrid::OpenFOAMGrid(MonoObject* mo)
{
_InitMonoBindings();
_MonoGCHandle = mono_gchandle_new(mo, true);
}
OpenFOAMGrid* OpenFOAMGrid::_FromMonoObject(MonoObject* mo)
{
OpenFOAMGrid* tmp = new OpenFOAMGrid(mo);
void* LoggedRef = tmp->_GetForeignPointer();
if (LoggedRef != NULL) {
    tmp->_ReleaseGChandle();
    delete tmp;
    return ((OpenFOAMGrid*)LoggedRef);
} else {
    tmp->_SetForeignPointer(tmp);
    return tmp;
}
}
MonoObject* OpenFOAMGrid::_GetMonoObject()
{
return mono_gchandle_get_target(_MonoGCHandle);
}
OpenFOAMGrid::OpenFOAMGrid(int nPoints, int nCells, int nFaces, int nInternalFaces, int nNames, int* nameLenghts, int emptyTag, int** faces, int* vertices_per_face, int* neighbour, int* owner, double* points, int** names, int* patchIDs)
{
_InitMonoBindings();
MonoObject* ThisObj = mono_object_new(BoSSS::Globals::_domain, _ClassHandle);
_MonoGCHandle = mono_gchandle_new(ThisObj, true);
void* args[14];
args[0] = &nPoints;
args[1] = &nCells;
args[2] = &nFaces;
args[3] = &nInternalFaces;
args[4] = &nNames;
args[5] = nameLenghts;
args[6] = &emptyTag;
args[7] = faces;
args[8] = vertices_per_face;
args[9] = neighbour;
args[10] = owner;
args[11] = points;
args[12] = names;
args[13] = patchIDs;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_ctor_0, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFOAMGrid.ctor_0) \n");
}
_SetForeignPointer(this);
}
void OpenFOAMGrid::_SetForeignPointer(void* ptr)
{
void* args[1];
args[0] = &ptr;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(__SetForeignPointer, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFOAMGrid._SetForeignPointer) \n");
}
return;
}
void* OpenFOAMGrid::_GetForeignPointer()
{
void* args[1];
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(__GetForeignPointer, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFOAMGrid._GetForeignPointer) \n");
}
void* retptr = mono_object_unbox(retval);
return *((void**) retptr);
}
int OpenFOAMGrid::TestMethod(int a)
{
void* args[1];
args[0] = &a;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_TestMethod, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFOAMGrid.TestMethod) \n");
}
void* retptr = mono_object_unbox(retval);
return *((int*) retptr);
}
BoSSS::Foundation::Grid::Classic::GridData* OpenFOAMGrid::GetGridData()
{
void* args[1];
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_GetGridData, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFOAMGrid.GetGridData) \n");
}
return BoSSS::Foundation::Grid::Classic::GridData::_FromMonoObject(retval);
}
}
}
}

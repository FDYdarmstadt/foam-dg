// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 7/27/2022 9:23:21 AM
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
#pragma once
namespace BoSSS
{
namespace Application
{
namespace ExternalBinding
{
class OpenFoamPatchField
{
public:
~OpenFoamPatchField();
void _ReleaseGChandle();
OpenFoamPatchField(MonoObject* mo);
static OpenFoamPatchField* _FromMonoObject(MonoObject* mo);
MonoObject* _GetMonoObject();
        OpenFoamPatchField(BoSSS::Foundation::Grid::OpenFOAMGrid* grdDat, int dim, int nBoundaries, int* edgeTags, int* edgeTypes, double* edgeValues){
_InitMonoBindings();
MonoObject* ThisObj = mono_object_new(BoSSS::Globals::_domain, _ClassHandle);
_MonoGCHandle = mono_gchandle_new(ThisObj, true);
void* args[6];
args[0] = grdDat->_GetMonoObject();
args[1] = &dim;
args[2] = &nBoundaries;
args[3] = edgeTags;
args[4] = edgeTypes;
args[5] = edgeValues;
MonoObject* exception;
MonoObject* retval;
retval = mono_runtime_invoke(_ctor_0, mono_gchandle_get_target(_MonoGCHandle), args, &exception);
if (exception != NULL) {
    fprintf( stderr, "got exception from C# (OpenFoamPatchField.ctor_0) \n");
}
_SetForeignPointer(this);
        };
void _SetForeignPointer(void* ptr);
void* _GetForeignPointer();
private:
void _InitMonoBindings();
MonoClass* _ClassHandle;
uint32_t _MonoGCHandle;
MonoMethod* _ctor_0;
MonoMethod* __SetForeignPointer;
MonoMethod* __GetForeignPointer;
};
}
}
}

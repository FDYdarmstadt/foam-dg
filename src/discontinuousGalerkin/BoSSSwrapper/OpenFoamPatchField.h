// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 7/14/2022 1:09:54 PM
// ##########################################################################
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
OpenFoamPatchField(BoSSS::Foundation::Grid::OpenFOAMGrid* grdDat, int nBoundaries, int* edgeTags, int* edgeTypes, double* edgeValues);
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

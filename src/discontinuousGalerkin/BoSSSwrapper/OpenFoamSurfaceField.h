// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 7/26/2023 10:08:45AM
// ##########################################################################
#pragma once
namespace BoSSS
{
namespace Application
{
namespace ExternalBinding
{
class OpenFoamSurfaceField
{
public:
~OpenFoamSurfaceField();
void _ReleaseGChandle();
OpenFoamSurfaceField(MonoObject* mo);
static OpenFoamSurfaceField* _FromMonoObject(MonoObject* mo);
MonoObject* _GetMonoObject();
OpenFoamSurfaceField(BoSSS::Foundation::Grid::OpenFOAMGrid* g, double* values, int nFaces);
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

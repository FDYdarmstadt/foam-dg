// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 15.04.2021 16:46:10
// ##########################################################################
#pragma once
namespace BoSSS
{
namespace Application
{
namespace ExternalBinding
{
class OpenFoamDGField
{
public:
~OpenFoamDGField();
void _ReleaseGChandle();
OpenFoamDGField(MonoObject* mo);
static OpenFoamDGField* _FromMonoObject(MonoObject* mo);
MonoObject* _GetMonoObject();
OpenFoamDGField(BoSSS::Foundation::Grid::OpenFOAMGrid* g, int degree, int NoOfComponents);
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

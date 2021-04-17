// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 17.04.2021 08:02:20
// ##########################################################################
#pragma once
namespace BoSSS
{
namespace Application
{
namespace ExternalBinding
{
class FixedOperators
{
public:
~FixedOperators();
void _ReleaseGChandle();
FixedOperators(MonoObject* mo);
static FixedOperators* _FromMonoObject(MonoObject* mo);
MonoObject* _GetMonoObject();
void _SetForeignPointer(void* ptr);
void* _GetForeignPointer();
void Laplacian(BoSSS::Foundation::Grid::OpenFOAMGrid* grid, int DgDegree);
private:
void _InitMonoBindings();
MonoClass* _ClassHandle;
uint32_t _MonoGCHandle;
MonoMethod* __SetForeignPointer;
MonoMethod* __GetForeignPointer;
MonoMethod* _Laplacian;
};
}
}
}

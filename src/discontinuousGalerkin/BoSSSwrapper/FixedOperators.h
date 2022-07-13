// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 7/11/2022 12:42:12 PM
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
FixedOperators();
void _SetForeignPointer(void* ptr);
void* _GetForeignPointer();
void Laplacian(BoSSS::Application::ExternalBinding::OpenFoamMatrix* mtx, BoSSS::Application::ExternalBinding::OpenFoamPatchField* ptch);
private:
void _InitMonoBindings();
MonoClass* _ClassHandle;
uint32_t _MonoGCHandle;
MonoMethod* _ctor_0;
MonoMethod* __SetForeignPointer;
MonoMethod* __GetForeignPointer;
MonoMethod* _Laplacian;
};
}
}
}

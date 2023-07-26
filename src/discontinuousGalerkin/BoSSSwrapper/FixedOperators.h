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
BoSSS::Application::ExternalBinding::OpenFoamDGField* GetFlux();
BoSSS::Application::ExternalBinding::OpenFoamDGField* GetMu();
void CahnHilliard(BoSSS::Application::ExternalBinding::OpenFoamMatrix* mtx, BoSSS::Application::ExternalBinding::OpenFoamSurfaceField* Flux, BoSSS::Application::ExternalBinding::OpenFoamDGField* U, BoSSS::Application::ExternalBinding::OpenFoamPatchField* ptch, BoSSS::Application::ExternalBinding::OpenFoamPatchField* ptchU, double time, double deltaT, double Cahn, double Diff);
void Laplacian(BoSSS::Application::ExternalBinding::OpenFoamMatrix* mtx, BoSSS::Application::ExternalBinding::OpenFoamPatchField* ptch);
private:
void _InitMonoBindings();
MonoClass* _ClassHandle;
uint32_t _MonoGCHandle;
MonoMethod* _ctor_0;
MonoMethod* __SetForeignPointer;
MonoMethod* __GetForeignPointer;
MonoMethod* _GetFlux;
MonoMethod* _GetMu;
MonoMethod* _CahnHilliard;
MonoMethod* _Laplacian;
};
}
}
}

// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 8/5/2022 4:14:06 PM
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
void SetDGcoordinate(int f, int j, int n, double val);
double GetDGcoordinate(int f, int j, int n);
void _SetForeignPointer(void* ptr);
void* _GetForeignPointer();
private:
void _InitMonoBindings();
MonoClass* _ClassHandle;
uint32_t _MonoGCHandle;
MonoMethod* _ctor_0;
MonoMethod* _SetDGcoordinate;
MonoMethod* _GetDGcoordinate;
MonoMethod* __SetForeignPointer;
MonoMethod* __GetForeignPointer;
};
}
}
}

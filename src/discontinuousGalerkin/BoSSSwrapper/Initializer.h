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
class Initializer
{
public:
~Initializer();
void _ReleaseGChandle();
Initializer(MonoObject* mo);
static Initializer* _FromMonoObject(MonoObject* mo);
MonoObject* _GetMonoObject();
Initializer();
void BoSSSInitialize();
void BoSSSFinalize();
void _SetForeignPointer(void* ptr);
void* _GetForeignPointer();
private:
void _InitMonoBindings();
MonoClass* _ClassHandle;
uint32_t _MonoGCHandle;
MonoMethod* _ctor_0;
MonoMethod* _BoSSSInitialize;
MonoMethod* _BoSSSFinalize;
MonoMethod* __SetForeignPointer;
MonoMethod* __GetForeignPointer;
};
}
}
}

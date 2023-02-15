// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 2/10/2023 4:19:36PM
// ##########################################################################
#pragma once
namespace BoSSS
{
namespace Foundation
{
namespace Grid
{
namespace Classic
{
class GridData
{
public:
~GridData();
void _ReleaseGChandle();
GridData(MonoObject* mo);
static GridData* _FromMonoObject(MonoObject* mo);
MonoObject* _GetMonoObject();
void _SetForeignPointer(void* ptr);
void* _GetForeignPointer();
private:
void _InitMonoBindings();
MonoClass* _ClassHandle;
uint32_t _MonoGCHandle;
MonoMethod* __SetForeignPointer;
MonoMethod* __GetForeignPointer;
};
}
}
}
}

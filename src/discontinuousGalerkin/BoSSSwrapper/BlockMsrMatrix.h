// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 15.04.2021 17:47:02
// ##########################################################################
#pragma once
namespace ilPSP
{
namespace LinSolvers
{
class BlockMsrMatrix
{
public:
~BlockMsrMatrix();
void _ReleaseGChandle();
BlockMsrMatrix(MonoObject* mo);
static BlockMsrMatrix* _FromMonoObject(MonoObject* mo);
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

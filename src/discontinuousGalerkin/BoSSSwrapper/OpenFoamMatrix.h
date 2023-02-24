// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 2/24/2023 12:53:12PM
// ##########################################################################
#pragma once
namespace BoSSS
{
namespace Application
{
namespace ExternalBinding
{
class OpenFoamMatrix
{
public:
~OpenFoamMatrix();
void _ReleaseGChandle();
OpenFoamMatrix(MonoObject* mo);
static OpenFoamMatrix* _FromMonoObject(MonoObject* mo);
MonoObject* _GetMonoObject();
OpenFoamMatrix(BoSSS::Foundation::Grid::OpenFOAMGrid* grd, BoSSS::Application::ExternalBinding::OpenFoamDGField* f);
double GetRHScoordinate(int f, int j, int n);
double GetSolCoordinate(int f, int j, int n);
void Solve();
int GetNoOfRowsInBlock(int iLocal);
int GetNoOfColsInBlock(int iLocal);
void GetBlock(int iRowLocal, int jColLocal, double* InputReadBuffer);
void AccBlock(int iRowLocal, int jColLocal, double alpha, double* InputReadBuffer);
void ClearBlock(int iRowLocal, int jColLocal);
void _SetForeignPointer(void* ptr);
void* _GetForeignPointer();
private:
void _InitMonoBindings();
MonoClass* _ClassHandle;
uint32_t _MonoGCHandle;
MonoMethod* _ctor_0;
MonoMethod* _GetRHScoordinate;
MonoMethod* _GetSolCoordinate;
MonoMethod* _Solve;
MonoMethod* _GetNoOfRowsInBlock;
MonoMethod* _GetNoOfColsInBlock;
MonoMethod* _GetBlock;
MonoMethod* _AccBlock;
MonoMethod* _ClearBlock;
MonoMethod* __SetForeignPointer;
MonoMethod* __GetForeignPointer;
};
}
}
}

// ##########################################################################
// This is AUTO-GENERATED code created by the 
// BoSSS External Language Binding code generator.
// **Any manual changes are over-written if the code-generator is executed.**
// Creation Date: 9/2/2022 4:14:20 PM
// ##########################################################################
#pragma once
namespace BoSSS
{
namespace Foundation
{
namespace Grid
{
class OpenFOAMGrid
{
public:
~OpenFOAMGrid();
void _ReleaseGChandle();
OpenFOAMGrid(MonoObject* mo);
static OpenFOAMGrid* _FromMonoObject(MonoObject* mo);
MonoObject* _GetMonoObject();
OpenFOAMGrid(int nPoints, int nCells, int nFaces, int nInternalFaces, int nNames, int* nameLenghts, int emptyTag, int** faces, int* vertices_per_face, int* neighbour, int* owner, double* points, int** names, int* patchIDs);
void _SetForeignPointer(void* ptr);
void* _GetForeignPointer();
int TestMethod(int a);
BoSSS::Foundation::Grid::Classic::GridData* GetGridData();
private:
void _InitMonoBindings();
MonoClass* _ClassHandle;
uint32_t _MonoGCHandle;
MonoMethod* _ctor_0;
MonoMethod* __SetForeignPointer;
MonoMethod* __GetForeignPointer;
MonoMethod* _TestMethod;
MonoMethod* _GetGridData;
};
}
}
}

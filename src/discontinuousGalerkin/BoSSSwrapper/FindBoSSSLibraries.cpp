#include "FindBoSSSLibraries.h"
#include <sys/stat.h>

bool DoesPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

std::string GetBoSSSLibraryDirectory(){
  std::string bosssinstall = getenv("BOSSS_INSTALL");
  std::string bossslibdir = bosssinstall + "/public/src/L4-application/ExternalBinding.CodeGen/bin/Release/net5.0/";
  if (! DoesPathExist(bossslibdir)){
    fprintf(stderr, "Expecting to find the BoSSS libraries in \n ");
    fprintf(stderr, bossslibdir.c_str());
    fprintf(stderr, "\n but the folder does not exist \n ");
    fprintf(stderr, "Make sure that BoSSS was built correctly. \n ");
    fprintf(stderr, "If necessary, change the variable bossslibdir in foam-dg/src/discontiuousGalerkin/BoSSSwrapper/FindBoSSSLibraries.cpp \n ");
    throw "Unable to find the location of the BoSSS libraries.";
  }
  return bossslibdir;
}

void Smart_Init() {
    std::string ManagedAssemblyDirectory = GetBoSSSLibraryDirectory();
    char *cstr = new char[ManagedAssemblyDirectory.length() + 1];
    strcpy(cstr, ManagedAssemblyDirectory.c_str());
    BoSSS::Globals::Init(cstr);
    delete [] cstr;
}

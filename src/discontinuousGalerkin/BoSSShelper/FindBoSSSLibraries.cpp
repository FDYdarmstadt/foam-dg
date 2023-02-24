#include "FindBoSSSLibraries.h"
#include <sys/stat.h>
#include <string.h>

bool DoesPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

std::string GetBoSSSLibraryDirectory(){
  // std::string bosssinstall = getenv("BOSSS_INSTALL");
  char* rootdirCh = getenv("FOAM_DG_ROOT");
  if (rootdirCh == NULL){
    throw "Please set the environment variable $FOAM_DG_ROOT to the absolute path of your foam-dg repository location.";
  }
  std::string rootdir;
  rootdir = rootdirCh;
  // std::string bossslibdir = bosssinstall + "/public/src/L4-application/ExternalBinding.CodeGen/bin/Release/net5.0/";
  // std::string bossslibdir = bosssinstall + "/public/src/L4-application/ExternalBinding.CodeGen/bin/Debug/net6.0/";
  // std::string bossslibdir = bosssinstall + "/public/src/L4-application/ExternalBinding.CodeGen/bin/Release/netstandard2.0/";
  // std::string bossslibdir = bosssinstall + "/public/src/L4-application/ExternalBinding.CodeGen/rel/";
  // std::string bossslibdir = bosssinstall + "/public/src/L4-application/ExternalBinding.CodeGen/bin/Release/net6.0/";
  std::string bossslibdir = rootdir + "/src/discontinuousGalerkin/BoSSSwrapper/rel/";
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

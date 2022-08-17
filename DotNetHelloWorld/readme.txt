This directory contains a C# "Hello World"-application in order to 
test that dotnet is correctly installed.

Compile with:
  dotnet build hello.csproj
  
Run:
  dotnet ./bin/Debug/net6.0/hello.dll 
    or 
  dotnet ./bin/Debug/net5.0/hello.dll

Check dotnet with
  dotnet --list-sdks
  dotnet --list-runtimes
  
The output should look like:
   # dotnet --list-runtimes
   Microsoft.AspNetCore.App 5.0.17 [/usr/share/dotnet/shared/Microsoft.AspNetCore.App]
   Microsoft.NETCore.App 5.0.17 [/usr/share/dotnet/shared/Microsoft.NETCore.App]
   
   # dotnet --list-sdks
   5.0.408 [/usr/share/dotnet/sdk]


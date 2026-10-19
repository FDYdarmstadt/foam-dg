# Source this before invoking wmake directly. Allwmake sources it automatically.
# Override DOTNET_HOST_NATIVE for a nonstandard .NET SDK layout.
if [ -z "${DOTNET_HOST_NATIVE:-}" ]; then
    dotnet_root=${DOTNET_ROOT:-/usr/share/dotnet}
    DOTNET_HOST_NATIVE=$(find "$dotnet_root/packs/Microsoft.NETCore.App.Host.linux-x64" -path '*/10.*/runtimes/linux-x64/native' -type d | sort -V | tail -1)
fi
if [ ! -f "${DOTNET_HOST_NATIVE:-}/libnethost.a" ]; then
    echo 'Install the .NET 10 SDK hosting pack or set DOTNET_HOST_NATIVE.' >&2
    return 1
fi
export DOTNET_HOST_NATIVE

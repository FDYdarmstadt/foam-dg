#!/usr/bin/env bash
set -euo pipefail
# Generate matching sides, compile the C# adapters into ExternalBinding, and deploy.
: "${1:?usage: createBindings.sh <BoSSS source checkout> [Debug|Release]}"
bosss_source=$(cd -- "$1" && pwd)
configuration=${2:-Release}
case "$configuration" in Debug|Release) ;; *) echo 'Expected Debug or Release' >&2; exit 2;; esac
wrapper_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
project_dir="$bosss_source/public/src/L4-application"
staging_dir=$(mktemp -d)
trap 'rm -rf -- "$staging_dir"' EXIT
# Exclude old generated adapters while bootstrapping, so removed/renamed exports
# cannot prevent rebuilding the generator. Publishing below includes the new adapters.
dotnet build "$project_dir/ExternalBinding.CodeGen/ExternalBinding.CodeGen.csproj" -c "$configuration" -p:ExcludeNativeExports=true
dotnet "$project_dir/ExternalBinding.CodeGen/bin/$configuration/net10.0/ExternalBinding.CodeGen.dll" "$staging_dir"
cp "$staging_dir/NativeExports.g.cs" "$project_dir/ExternalBinding/NativeExports.g.cs"
dotnet publish "$project_dir/ExternalBinding/ExternalBinding.csproj" -c "$configuration" \
    --self-contained false -o "$wrapper_dir/rel"
cp "$staging_dir/"*.h "$staging_dir/"*.cpp "$staging_dir/compile.sh" "$wrapper_dir/"
chmod +x "$wrapper_dir/compile.sh"

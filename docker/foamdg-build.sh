#!/usr/bin/env bash
set -e
source "$HOME/.foamdg-env.sh"
if [[ ! -f "$FOAM_DG_ROOT/src/discontinuousGalerkin/BoSSSwrapper/createBindings.sh" ]]; then
    echo "Mount the foam-dg checkout at $FOAM_DG_ROOT before running foamdg-build." >&2
    exit 2
fi
bash "$FOAM_DG_ROOT/src/discontinuousGalerkin/BoSSSwrapper/createBindings.sh" "$BOSSS_SOURCE" Release
bash "$FOAM_DG_ROOT/src/Allwmake"
test -s "$FOAM_DG_ROOT/src/discontinuousGalerkin/BoSSSwrapper/rel/BoSSS.Application.ExternalBinding.runtimeconfig.json"
test -f "$FOAM_USER_LIBBIN/libdiscontinuousGalerkin.so"
test -x "$FOAM_USER_APPBIN/CahnHilliardFoam"
{
    cat "$HOME/foamdg-VERSION"
    printf 'foam-dg=%s\n' "$(git -C "$FOAM_DG_ROOT" rev-parse HEAD)"
    git -C "$FOAM_DG_ROOT" status --short --untracked-files=no
} > "$HOME/foamdg-mounted-VERSION"

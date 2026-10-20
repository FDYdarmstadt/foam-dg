# Shared by interactive shells, non-interactive Bash, and the entrypoint.
# foam-extend's startup scripts are not compatible with `set -u`.
if [[ ${FOAMDG_ENV_READY:-} != 1 && ${FOAMDG_ENV_INITIALIZING:-} != 1 ]]; then
    # foam-extend starts Bash helpers (e.g. foamGetSystemInfo), which inherit
    # BASH_ENV. Block their re-entry before sourcing any foam-extend scripts.
    export FOAMDG_ENV_INITIALIZING=1
    foamdg_restore_nounset=false
    case $- in *u*) foamdg_restore_nounset=true; set +u ;; esac
    source "$HOME/foam/foam-extend-4.1/etc/bashrc"
    export LD_LIBRARY_PATH="$BOSSS_INSTALL/bin/native/linux/amd64-openmpi:$BOSSS_INSTALL/bin/native/linux/amd64-openmpi/dependencies:${LD_LIBRARY_PATH:-}"
    FOAMDG_ENV_READY=1
    # Keep READY local to this shell: a later login shell may reset PATH and
    # must initialize again. Only the temporary recursion guard is inherited.
    unset FOAMDG_ENV_INITIALIZING
    if "$foamdg_restore_nounset"; then set -u; fi
    unset foamdg_restore_nounset
fi

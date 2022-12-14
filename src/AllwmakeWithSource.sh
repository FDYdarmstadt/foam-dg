#!/usr/bin/bash

cd "$(dirname "$(readlink -f "$0")")"

source ~/foam/foam-extend-4.1/etc/bashrc
# ./Allwclean
./Allwmake

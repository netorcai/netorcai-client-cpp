#!/usr/bin/env sh
set -eu

cd ${MESON_BUILD_ROOT}
doxygen ${MESON_SOURCE_ROOT}/doxygen/Doxyfile

exit 0

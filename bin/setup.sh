#!/bin/bash

BOLD="\x1B[1m"
RESET="\x1B[0m"
DEFAULT="\x1B[39m"
RED="\x1B[31m"
YELLOW="\x1B[33m"
GREEN="\x1B[32m"

WARNING="${YELLOW}WARNING${DEFAULT}"
FAILURE="${RED}FAILURE${DEFAULT}"
SUCCESS="${GREEN}SUCCESS${DEFAULT}"
INFO="${DEFAULT}INFO${DEFAULT}"

function log()
{
  echo -e "${BOLD}gfx_checks::setup::[$1]::${RESET} $2"
}

function check_paths()
{
  result=0

  [[ "$1" != *llvm-project/ ]] \
    && log "${FAILURE}" "first argument '$1' does not match with '*llvm-project/'" \
    && result=-1

  [[ "$2" != *src/ ]] \
    && log "${FAILURE}" "second argument '$2' does not match with '*src/'" \
    && result=-1

  [ ${result} == 0 ] || exit ${result}
}

function run()
{
  LLVM_PROJECT=$(realpath "$1")
  GFX_SOURCE=$(realpath "$2")

  LLVM_UNIT_TESTS="${LLVM_PROJECT}/clang-tools-extra/unittests/clang-tidy"
  GFX_CHECKS_UNIT_TESTS="${GFX_SOURCE}/test/unittests"

  if [ -d "${LLVM_UNIT_TESTS}/gfx" ]; then
    log "${WARNING}" "Test module for gfx-check already exists, call to reset"
    echo "rm -rf ${LLVM_UNIT_TESTS}/gfx"
    exit 0
  else
    mkdir "${LLVM_UNIT_TESTS}/gfx"
    ln -s "${GFX_CHECKS_UNIT_TESTS}" "${LLVM_UNIT_TESTS}/gfx"
    ln -s "${PWD}/build/lib" "${LLVM_UNIT_TESTS}/gfx"
    ln -s "${PWD}/src/gfx/include" "${LLVM_UNIT_TESTS}/gfx"
    ln -s "${PWD}/cmake/CMakeLists.txt" "${LLVM_UNIT_TESTS}/gfx"
    tree -l "${LLVM_UNIT_TESTS}/gfx"
  fi

  log "${SUCCESS}" "Done setting up symlinks!"
}

log "${INFO}" "running"
check_paths "$1" "$2"
run "$1" "$2"


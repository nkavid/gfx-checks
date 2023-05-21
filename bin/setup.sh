#!/bin/bash

function log()
{
  echo "gfx_checks::setup::"$1
}

function check_paths()
{
  result=0

  [[ $1 != *llvm-project/ ]] \
    && log "first argument '$1' does not match with '*llvm-project/'" \
    && result=-1
  
  clang_tools_extra=$1"clang-tools-extra"
  [ ! -d $clang_tools_extra ] && log "path '$clang_tools_extra' not found" && result=-1

  [[ $2 != *src/ ]] \
    && log "second argument '$2' does not match with '*src/'" \
    && result=-1

  gfx_module=$2"gfx"
  [ ! -d $gfx_module ] && log "path '$gfx_module' not found" && result=-1

  [ $result == 0 ] || exit $result
}

function run()
{
  log "running"
  LLVM_PROJECT=$(realpath $1)
  GFX_SOURCE=$(realpath $2)

  LLVM_UNIT_TESTS=${LLVM_PROJECT}/clang-tools-extra/unittests/clang-tidy
  GFX_MODULE_TEST_SUITE=${GFX_SOURCE}/test/unittests

  if [ -d ${LLVM_UNIT_TESTS}/gfx ]; then
    log "Path ${LLVM_UNIT_TESTS}/gfx already exists"
  else
    mkdir ${LLVM_UNIT_TESTS}/gfx
    ln -s ${GFX_MODULE_TEST_SUITE} ${LLVM_UNIT_TESTS}/gfx
    ln -s ${PWD}/build/lib ${LLVM_UNIT_TESTS}/gfx
    ln -s ${PWD}/src/gfx/include ${LLVM_UNIT_TESTS}/gfx
  fi

  log "done!"
}

check_paths $1 $2
run $1 $2

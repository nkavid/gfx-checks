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
  CLANG_TIDY_MODULES="$LLVM_PROJECT/clang-tools-extra/clang-tidy"

  GFX_SOURCE=$(realpath $2)
  GFX_MODULE="$GFX_SOURCE/gfx"

  if [ ! -L "$CLANG_TIDY_MODULES/gfx" ]; then
    ln -s $GFX_MODULE $CLANG_TIDY_MODULES
  else
    echo "link exists: '$CLANG_TIDY_MODULES/gfx'"
  fi

  LLVM_TEST_CHECKERS=$LLVM_PROJECT/clang-tools-extra/test/clang-tidy/checkers

  if [ ! -L $LLVM_TEST_CHECKERS/gfx ]; then
    ln -s $GFX_SOURCE/test/checkers/gfx $LLVM_TEST_CHECKERS
  else
    echo "link exists: '$LLVM_TEST_CHECKERS/gfx'"
  fi

  LLVM_UNIT_TESTS=$LLVM_PROJECT/clang-tools-extra/unittests/clang-tidy/
  GFX_MODULE_TEST_SUITE=$GFX_SOURCE/test/unittests/GFXModuleTest.cpp

  if [ ! -L $LLVM_UNIT_TESTS/GFXModuleTest.cpp ]; then
    ln -s $GFX_MODULE_TEST_SUITE $LLVM_UNIT_TESTS
  else
    echo "link exists: '$LLVM_UNIT_TESTS/GFXModuleTest.cpp'"
  fi

  log "done!"
}

check_paths $1 $2
run $1 $2

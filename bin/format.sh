#!/bin/bash

function log()
{
  echo "gfx_checks::format::"$1
}

function format()
{
  [ ! -d $1 ] && log "did not find '$1'..." && exit -1
  log "formatting '$1'"

  find $1 \( -name "*.cpp" -o -name "*.hpp" \) \
  -exec clang-format-15 -i {} +

  find $1 \( -name "*.c" -o -name "*.h" \) \
  -exec clang-format-15 -i {} +
}

format src/gfx/
format src/test/unittests/
format src/test/code/

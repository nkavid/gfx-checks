#!/bin/bash

function log()
{
  echo "gfx_checks::print::"$1
}

function print_file_as_string()
{
  echo "const char* $(basename --suffix=".cpp" $1) = "
  cat $1 | while read line; do echo "\"${line}\\n\""; done
  echo ";"
}

DUMMY_FILE=$1
[ ! -f  ${DUMMY_FILE} ] && log "missing '${DUMMY_FILE}', exiting..."
print_file_as_string ${DUMMY_FILE}

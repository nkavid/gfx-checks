#!/bin/bash

function log()
{
  echo "gfx_checks::print::"$1
}

function print_file_as_string()
{
  cat $1 | while read line; do echo "\"${line}\\n\""; done
}

DUMMY_FILE=src/test/code/$1
[ ! -f  ${DUMMY_FILE} ] && log "missing '${DUMMY_FILE}', exiting..."
print_file_as_string ${DUMMY_FILE}

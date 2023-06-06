#!/bin/bash

function log()
{
  echo "gfx_checks::print::$1"
}

function print_file_as_string()
{
  echo ""
  echo "namespace $(basename "$1" | tr "." "_" || true )"
  echo "{"
  echo "const char* source = R\"("

  IFS=""
  while read -r LINE; do
    echo "${LINE}"
  done < "$1"

  echo ")\";"
  echo ""
  echo "const char* path = \"$(basename "$1")\";"
  echo "} // namespace"
}

DUMMY_FILE=$1
[[ ! -f  "${DUMMY_FILE}" ]] && log "missing '${DUMMY_FILE}', exiting..."
print_file_as_string "${DUMMY_FILE}"

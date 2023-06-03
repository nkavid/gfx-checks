#!/bin/bash

CLANG_TIDY="/opt/pkg/llvm/llvm_16.0.5/bin/clang-tidy"
CLANG_TIDY_OPTIONS=(-p build \
--extra-arg=-Wno-error=unknown-warning-option \
--extra-arg=-nostdinc++ \
--quiet)

function run_clang_tidy()
{
  [[ ! -d $1 ]] && echo "did not find directory '$1'..." && exit 1
  echo "run clang-tidy for headers in '$1'"
  
  FILES=$(find "$1" -regextype posix-extended -regex ".*\.(c|h|cpp|hpp)$")
  NUM_FILES=$(echo "${FILES}" | wc -l)
  PROCESSED=0

  for FILE in ${FILES}; do
    CLANG_TIDY_CHECKS_ARG="--checks="
    INCR="${BOLD}[$((++PROCESSED)) of ${NUM_FILES}]${RESET}"

    [[ ${FILE} =~ /shaders/ ]] \
      && echo "skip ${ITALIC}${FILE}${RESET} ${INCR}" \
      && continue

    [[ -z "${FILE##*"_test.cpp"*}" ]] \
      && CLANG_TIDY_CHECKS_ARG="--checks=-readability-function-size"

    echo "checking ${ITALIC}${FILE}${RESET} ${INCR}"
    ${CLANG_TIDY} "${CLANG_TIDY_OPTIONS[@]}" "${CLANG_TIDY_CHECKS_ARG}" "${FILE}"
    echo "${FILE}"
  done
}

[[ ! -f build/compile_commands.json ]] \
  && echo "missing build/compile_commands.json" && exit 1

run_clang_tidy "$1"


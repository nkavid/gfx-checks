cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(GFX_CMAKE_CONFIG_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/cmake)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/gcc_toolchain.cmake")

set(CMAKE_CXX_STANDARD 17)

project(GFXTidy
  DESCRIPTION "Checkers for clang-tidy used in gfx sandbox repository"
  VERSION 0.0.1
  LANGUAGES CXX C
)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

function(gfx_tidy_object)
  set(one_value_args OBJECT)
  set(multi_value_args SOURCES OPTIONS)
  cmake_parse_arguments(GFX_TIDY
    ""
    "${one_value_args}"
    "${multi_value_args}"
    ${ARGN}
  )

  add_library(${GFX_TIDY_OBJECT} OBJECT 
    ${GFX_TIDY_SOURCES}
  )

  target_compile_options(${GFX_TIDY_OBJECT} PRIVATE
    ${GFX_TIDY_OPTIONS}
    "-fPIC"
    "-fno-rtti"
  )

  target_include_directories(${GFX_TIDY_OBJECT} PRIVATE
    "src/gfx/include"
  )

endfunction()

gfx_tidy_object(
  OBJECT
    "gfxTidyModule"
  SOURCES
    "src/gfx/gfx_tidy_module.cpp"
  OPTIONS
    "-Wno-weak-vtables"
    "-Wno-global-constructors"
    "-Wno-missing-variable-declarations"
)

gfx_tidy_object(
  OBJECT
    "gfxTidyChecks"
  SOURCES
    "src/gfx/class_cohesion_check.cpp"
    "src/gfx/implementation_in_namespace_check.cpp"
    "src/gfx/main_implementation_filename_check.cpp"
    "src/gfx/package_namespace_check.cpp"
    "src/gfx/basename_declaration_check.cpp"
    "src/gfx/fundamental_type_check.cpp"
)

add_library(gfx_tidy_checks SHARED)

target_sources(gfx_tidy_checks PRIVATE
  $<TARGET_OBJECTS:gfxTidyModule>
  $<TARGET_OBJECTS:gfxTidyChecks>
)

install(CODE
  "execute_process(COMMAND \
    clang-tidy -load lib/libgfx_tidy_checks.so \
    --checks=-*,gfx-* \
    --list-checks)"
)

include(cmake/gfx_base_install.cmake)

gfx_base_install_target(gfx_tidy_checks)
gfx_base_install_finish()


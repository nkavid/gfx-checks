cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 17)

project(gfx-checks
  DESCRIPTION "Checkers for clang-tidy used in gfx sandbox repository"
  VERSION 0.0.1
  LANGUAGES CXX C
)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

add_library(gfx_checks SHARED)

target_compile_options(gfx_checks PRIVATE -fno-rtti)

target_sources(gfx_checks PRIVATE
  src/gfx/GFXTidyModule.cpp
  src/gfx/ClassCohesionCheck.cpp
  src/gfx/ImplementationInNamespaceCheck.cpp
  src/gfx/MainImplementationFilenameCheck.cpp
  src/gfx/PackageNamespaceCheck.cpp
)

target_include_directories(gfx_checks PRIVATE
  src/gfx/include
)

target_include_directories(gfx_checks SYSTEM PRIVATE
  ${CLANG_INCLUDE_DIRS}
  ${LLVM_INCLUDE_DIRS}
)


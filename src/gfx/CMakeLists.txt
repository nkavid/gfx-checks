set(LLVM_LINK_COMPONENTS
  FrontendOpenMP
  Support
  )

add_clang_library(clangTidyGFXModule
  GFXTidyModule.cpp
  ImplementationInNamespaceCheck.cpp
  MainImplementationFilenameCheck.cpp
  ClassCohesionCheck.cpp
  PackageNamespaceCheck.cpp

  LINK_LIBS
  clangTidy
  clangTidyPortabilityModule
  clangTidyUtils

  DEPENDS
  omp_gen
  )

target_include_directories(clangTidyGFXModule SYSTEM PRIVATE
  ${GFX_CLANG_TIDY_INCLUDE}
  )

clang_target_link_libraries(clangTidyGFXModule
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTooling
  )

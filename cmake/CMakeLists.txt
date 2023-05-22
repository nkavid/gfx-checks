set(GFX_MODULE_UNIT_TESTS
  "${CMAKE_CURRENT_LIST_DIR}/unittests/GFXModuleTest.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/unittests/gfx_package_namespace_test.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/unittests/gfx_basename_declaration_test.cpp"
  PARENT_SCOPE
)

set(GFX_MODULE_INCLUDE_PATH
  ${CMAKE_CURRENT_LIST_DIR}/include
  PARENT_SCOPE
)

find_library(GFX_MODULE_LIBRARY
  libgfx_checks.so
  PATH
    "${CMAKE_CURRENT_LIST_DIR}/lib"
)

macro(gfx_print_info)
message(NOTICE "GFX_MODULE_UNIT_TESTS: ${GFX_MODULE_UNIT_TESTS}")
message(NOTICE "GFX_MODULE_INCLUDE_PATH: ${GFX_MODULE_INCLUDE_PATH}")
message(NOTICE "GFX_MODULE_LIBRARY: ${GFX_MODULE_LIBRARY}")
endmacro()

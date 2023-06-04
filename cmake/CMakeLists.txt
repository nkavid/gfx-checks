set(GFX_MODULE_UNIT_TESTS
  "${CMAKE_CURRENT_LIST_DIR}/unittests/class_cohesion_check_test.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/unittests/main_implementation_filename_check_test.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/unittests/package_namespace_check_test.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/unittests/basename_declaration_check_test.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/unittests/fundamental_type_check_test.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/unittests/braced_initialization_check_test.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/unittests/package_prefix_check_test.cpp"
  PARENT_SCOPE
)

set(GFX_MODULE_INCLUDE_PATH
  ${CMAKE_CURRENT_LIST_DIR}/include
  PARENT_SCOPE
)

find_library(GFX_MODULE_LIBRARY
  libgfx_tidy_checks.so
  PATH
    "${CMAKE_CURRENT_LIST_DIR}/lib"
)

macro(gfx_print_info)
message(NOTICE "GFX_MODULE_UNIT_TESTS: ${GFX_MODULE_UNIT_TESTS}")
message(NOTICE "GFX_MODULE_INCLUDE_PATH: ${GFX_MODULE_INCLUDE_PATH}")
message(NOTICE "GFX_MODULE_LIBRARY: ${GFX_MODULE_LIBRARY}")
endmacro()

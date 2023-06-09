set(GFX_UNIT_TEST_PATH "${CMAKE_CURRENT_LIST_DIR}/unittests")

set(GFX_MODULE_UNIT_TESTS
    "${GFX_UNIT_TEST_PATH}/utils.cpp"
    "${GFX_UNIT_TEST_PATH}/class_cohesion_check_test.cpp"
    "${GFX_UNIT_TEST_PATH}/main_implementation_filename_check_test.cpp"
    "${GFX_UNIT_TEST_PATH}/package_namespace_check_test.cpp"
    "${GFX_UNIT_TEST_PATH}/basename_declaration_check_test.cpp"
    "${GFX_UNIT_TEST_PATH}/fundamental_type_check_test.cpp"
    "${GFX_UNIT_TEST_PATH}/braced_initialization_check_test.cpp"
    "${GFX_UNIT_TEST_PATH}/package_prefix_check_test.cpp"
    PARENT_SCOPE
)

set(GFX_MODULE_INCLUDE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/include
    PARENT_SCOPE
)

find_library(
  GFX_MODULE_LIBRARY
  libgfx_tidy_checks.so
  PATH
  "${CMAKE_CURRENT_LIST_DIR}/lib"
)

# Prints cmake build variables to terminal to check paths exist
macro(GFX_PRINT_INFO)
  message(NOTICE "GFX_MODULE_UNIT_TESTS: ${GFX_MODULE_UNIT_TESTS}")
  message(NOTICE "GFX_MODULE_INCLUDE_PATH: ${GFX_MODULE_INCLUDE_PATH}")
  message(NOTICE "GFX_MODULE_LIBRARY: ${GFX_MODULE_LIBRARY}")
endmacro()

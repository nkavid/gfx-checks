include(CMakePackageConfigHelpers)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}_${CMAKE_PROJECT_VERSION}"
      CACHE PATH "..." FORCE
  )
endif()

set(TARGETS_EXPORT_NAME "${CMAKE_PROJECT_NAME}Targets")

# Configure install for individual target
function(gfx_base_install_target target_name)
  install(TARGETS ${target_name} EXPORT "${TARGETS_EXPORT_NAME}")
endfunction()

# Call after configuring all build targets
function(gfx_base_install_finish)
  set(cmake_output_dir ${GFX_CMAKE_CONFIG_OUTPUT_DIRECTORY})

  export(
    EXPORT "${TARGETS_EXPORT_NAME}"
    FILE "${cmake_output_dir}/${TARGETS_EXPORT_NAME}.cmake"
    NAMESPACE ${CMAKE_PROJECT_NAME}::
  )

  install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    DESTINATION "lib/cmake"
    NAMESPACE ${CMAKE_PROJECT_NAME}::
  )

  write_basic_package_version_file(
    "${cmake_output_dir}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
    COMPATIBILITY ExactVersion
  )

  configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}Config.cmake"
    "${cmake_output_dir}/${CMAKE_PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "lib/cmake"
  )

  install(
    FILES "${cmake_output_dir}/${CMAKE_PROJECT_NAME}Config.cmake"
          "${cmake_output_dir}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "lib/cmake"
  )
endfunction()

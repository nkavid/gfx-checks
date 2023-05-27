include(CMakePackageConfigHelpers)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
    "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}_${CMAKE_PROJECT_VERSION}"
    CACHE PATH "..." FORCE)
endif()

set(TARGETS_EXPORT_NAME "${CMAKE_PROJECT_NAME}Targets")

function(gfx_base_install_target TARGET_NAME)
  install(
    TARGETS ${TARGET_NAME}
    EXPORT "${TARGETS_EXPORT_NAME}"
  )
endfunction()

function(gfx_base_install_finish)
  export(
    EXPORT "${TARGETS_EXPORT_NAME}"
    FILE "${GFX_CMAKE_CONFIG_OUTPUT_DIRECTORY}/${TARGETS_EXPORT_NAME}.cmake"
    NAMESPACE ${CMAKE_PROJECT_NAME}::
  )

  install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    DESTINATION "lib/cmake"
    NAMESPACE ${CMAKE_PROJECT_NAME}::
  )

  write_basic_package_version_file(
    "${GFX_CMAKE_CONFIG_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
    COMPATIBILITY ExactVersion
  )

  configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}Config.cmake"
    "${GFX_CMAKE_CONFIG_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION
      "lib/cmake"
  )

  install(
    FILES
      "${GFX_CMAKE_CONFIG_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}Config.cmake"
      "${GFX_CMAKE_CONFIG_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION
      "lib/cmake"
  )
endfunction()

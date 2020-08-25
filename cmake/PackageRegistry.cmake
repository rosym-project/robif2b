include(CMakeParseArguments)

function(REGISTER_PACKAGE)
  set(multi_value_args LIBRARIES INCLUDE_DIRS)
  cmake_parse_arguments(REGISTER_PACKAGE "" "${one_value_args}" "${multi_value_args}" ${ARGN})

  # The directory to which all CMake files (e.g. the configuration files) are installed
  set(CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME})

  # Add all targets to the build-tree export set
  export(TARGETS ${REGISTER_PACKAGE_LIBRARIES}
    FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-targets.cmake
  )

  # Export this package to CMake's package registry
  export(PACKAGE ${PROJECT_NAME})


  #
  # Create the config.cmake and config-version.cmake files
  #

  # ... for the build tree
  set(CONF_INCLUDE_DIRS "")
  foreach(include ${REGISTER_PACKAGE_INCLUDE_DIRS})
    list(APPEND CONF_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/${include})
  endforeach()
  configure_file(${PROJECT_NAME}-config.cmake.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake @ONLY
  )

  # ... for the install tree
  set(CONF_INCLUDE_DIRS "")
  foreach(include ${REGISTER_PACKAGE_INCLUDE_DIRS})
    if(NOT IS_ABSOLUTE ${include})
      set(${include} ${CMAKE_INSTALL_PREFIX}/${include})
    endif()
    file(RELATIVE_PATH REL_INCLUDE_DIR "${CMAKE_DIR}" "${include}")
    list(APPEND CONF_INCLUDE_DIRS "\${${PROJECT_NAME}_CMAKE_DIR}/${REL_INCLUDE_DIR}")
  endforeach()
  configure_file(${PROJECT_NAME}-config.cmake.in
    ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}-config.cmake @ONLY
  )

  # ... for both
  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    VERSION 0.1.0
    COMPATIBILITY SameMajorVersion
  )

  # Install the config.cmake and config-version.cmake
  install(
    FILES
      ${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}-config.cmake
      ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    DESTINATION ${CMAKE_DIR}
  )

  # Install the export set for use with the install-tree
  foreach(library ${REGISTER_PACKAGE_LIBRARIES})
    install(EXPORT ${library}-targets
      DESTINATION ${CMAKE_DIR}
    )
  endforeach()
endfunction()
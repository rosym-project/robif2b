cmake_minimum_required(VERSION 3.5)
project(robif2b)

option(ENABLE_PACKAGE_REGISTRY "Add this package to CMake's package registry" Off)
option(ENABLE_KORTEX "Build Kinova Kortex interface" Off)
option(ENABLE_UBX "Build UBX blocks" Off)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(PackageRegistry)

include_directories(
  include
)

add_subdirectory(src)


install(
  DIRECTORY include/
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
)

if(ENABLE_PACKAGE_REGISTRY)
  register_package(
    INCLUDE_DIRS
      include
    LIBRARIES
      ${EXPORTED_LIBRARIES}
  )
endif()

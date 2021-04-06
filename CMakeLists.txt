cmake_minimum_required(VERSION 3.5)
project(robif2b)

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

register_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    ${EXPORTED_LIBRARIES}
)

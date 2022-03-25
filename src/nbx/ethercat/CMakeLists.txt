find_package(soem)


add_library(ethercat SHARED ethercat.c)

target_include_directories(ethercat
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(ethercat
  PRIVATE
    soem
)

target_compile_options(ethercat
  PRIVATE
    -Wall -Wextra -pedantic
)

install(
  TARGETS ethercat
  EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
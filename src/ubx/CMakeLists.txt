find_package(PkgConfig REQUIRED)

pkg_check_modules(UBX REQUIRED ubx0)
pkg_get_variable(UBX_MODDIR ${UBX_MODULE_NAME} UBX_MODDIR)
pkg_get_variable(UBX_EXEC_PREFIX ${UBX_MODULE_NAME} exec_prefix)

file(RELATIVE_PATH UBX_MODDIR_REL ${UBX_EXEC_PREFIX} ${UBX_MODDIR})

include_directories(
  include
  ${UBX_INCLUDE_DIRS}
)


add_subdirectory(2dof)

if(ENABLE_KORTEX)
  add_subdirectory(kortex)
endif()


configure_file(ubx_env.sh.in ${CMAKE_BINARY_DIR}/ubx_env.sh)

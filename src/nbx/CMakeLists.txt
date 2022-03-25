add_subdirectory(2dof)

if(ENABLE_ETHERCAT)
  add_subdirectory(ethercat)
endif()

if(ENABLE_KORTEX)
  add_subdirectory(kortex)
endif()

if(ENABLE_KELO)
  add_subdirectory(kelo)
endif()
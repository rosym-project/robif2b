add_executable(2dof_example 2dof_example.c)
target_link_libraries(2dof_example 2dof)

if(ENABLE_KORTEX)
  add_executable(kinova_gen3_example kinova_gen3_example.c)
  target_link_libraries(kinova_gen3_example kinova_gen3)
endif()

if(ENABLE_ETHERCAT)
  if(ENABLE_KELO)
    add_executable(kelo_robile_example kelo_robile_example.c)
    target_link_libraries(kelo_robile_example kelo ethercat)
  endif()
endif()

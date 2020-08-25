add_executable(2dof_example 2dof_example.c)
target_link_libraries(2dof_example robif2b_2dof)

if(ENABLE_KORTEX)
  add_executable(kinova_gen3_example kinova_gen3_example.c)
  target_link_libraries(kinova_gen3_example kinova_gen3)
endif()

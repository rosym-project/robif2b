add_library(ubx_2dof SHARED ubx_2dof.c)
target_link_libraries(ubx_2dof
  2dof
  ${UBX_LIBRARIES}
)

add_custom_command(TARGET ubx_2dof
  POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_CURRENT_BINARY_DIR}/libubx_2dof.so" "${CMAKE_CURRENT_BINARY_DIR}/../${UBX_MODDIR_REL}/ubx_2dof.so"
)

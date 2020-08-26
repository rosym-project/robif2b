add_library(ubx_kinova_gen3 SHARED ubx_kinova_gen3.c)
target_link_libraries(ubx_kinova_gen3
  kinova_gen3
  ${UBX_LIBRARIES}
)

add_custom_command(TARGET ubx_kinova_gen3
  POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_CURRENT_BINARY_DIR}/libubx_kinova_gen3.so" "${CMAKE_CURRENT_BINARY_DIR}/../${UBX_MODDIR_REL}/ubx_kinova_gen3.so"
)

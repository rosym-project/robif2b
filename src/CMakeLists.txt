add_subdirectory(nbx)
add_subdirectory(example)

if(ENABLE_UBX)
  add_subdirectory(ubx)
endif()
add_subdirectory(nbx)
add_subdirectory(example)

if(ENABLE_UBX)
  add_subdirectory(ubx)
endif()

# Forward exported libraries
set(EXPORTED_LIBRARIES ${EXPORTED_LIBRARIES} PARENT_SCOPE)

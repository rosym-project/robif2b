add_subdirectory(2dof)

if(ENABLE_KORTEX)
  add_subdirectory(kortex)
endif()


# Forward exported libraries
set(EXPORTED_LIBRARIES ${EXPORTED_LIBRARIES} PARENT_SCOPE)

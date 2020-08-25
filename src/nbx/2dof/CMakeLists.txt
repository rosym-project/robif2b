add_library(robif2b_2dof SHARED 2dof.c)


install(
  TARGETS robif2b_2dof
  EXPORT robif2b_2dof-targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

set(EXPORTED_LIBRARIES ${EXPORTED_LIBRARIES} robif2b_2dof PARENT_SCOPE)

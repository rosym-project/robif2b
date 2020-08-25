find_package(KortexApi)

set(CMAKE_CXX_FLAGS "-D_OS_UNIX")

include_directories(
  ${KORTEXAPI_INCLUDE_DIRS}
)

add_library(kinova_gen3 SHARED kinova_gen3.cpp)
target_link_libraries(kinova_gen3 ${KORTEXAPI_LIBRARIES})


install(
  TARGETS kinova_gen3
  EXPORT kinova_gen3-targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# Forward exported libraries
set(EXPORTED_LIBRARIES ${EXPORTED_LIBRARIES} kinova_gen3 PARENT_SCOPE)

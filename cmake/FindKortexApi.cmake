include(GNUInstallDirs)

set(KORTEXAPI_INCLUDE_DIRS
  "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api"
  "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/client"
  "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/client_stubs"
  "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/common"
  "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/google"
  "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/messages"
)

set(KORTEXAPI_LIBRARIES
  "${CMAKE_INSTALL_FULL_LIBDIR}/libKortexApiCpp.a"
  "${CMAKE_INSTALL_FULL_LIBDIR}/libKortexApiCpp_without_protobuf.a"
  pthread
)

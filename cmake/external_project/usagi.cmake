include(ExternalProject)

ExternalProject_Add(
  external_usagi
  GIT_REPOSITORY git@github.com:usagi/usagi.git
  PREFIX         ${CMAKE_CURRENT_BINARY_DIR}/external/usagi
  INSTALL_DIR    ${CMAKE_CURRENT_BINARY_DIR}
  CMAKE_ARGS     -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                 -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
                 -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                 -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
                 -DEXAMPLES=off
)

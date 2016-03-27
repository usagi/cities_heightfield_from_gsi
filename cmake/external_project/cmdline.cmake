include(ExternalProject)

ExternalProject_Add(
  external_cmdline
  GIT_REPOSITORY git@github.com:tanakh/cmdline.git
  PREFIX         ${CMAKE_CURRENT_BINARY_DIR}/external/cmdline
  INSTALL_DIR    ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND    ""
  INSTALL_COMMAND
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_BINARY_DIR}/external/cmdline/src/external_cmdline/cmdline.h
      ${CMAKE_CURRENT_BINARY_DIR}/include
)

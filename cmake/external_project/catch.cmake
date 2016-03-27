include(ExternalProject)

ExternalProject_Add(
  external_catch
  EXCLUDE_FROM_ALL
  GIT_REPOSITORY git@github.com:philsquared/Catch.git
  PREFIX         ${CMAKE_CURRENT_BINARY_DIR}/external/catch
  INSTALL_DIR    ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/external/catch/src/external_catch/include ${CMAKE_CURRENT_BINARY_DIR}/include
)

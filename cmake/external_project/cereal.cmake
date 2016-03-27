include(ExternalProject)

ExternalProject_Add(
  external_cereal
  GIT_REPOSITORY git@github.com:USCiLab/cereal.git
  PREFIX         ${CMAKE_CURRENT_BINARY_DIR}/external/cereal
  INSTALL_DIR    ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND    ""
  INSTALL_COMMAND
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_CURRENT_BINARY_DIR}/external/cereal/src/external_cereal/include
      ${CMAKE_CURRENT_BINARY_DIR}/include
)

include(ExternalProject)

ExternalProject_Add(
  external_stb
  GIT_REPOSITORY git@github.com:nothings/stb.git
  PREFIX         ${CMAKE_CURRENT_BINARY_DIR}/external/stb
  INSTALL_DIR    ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND    ""
  INSTALL_COMMAND
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_BINARY_DIR}/external/stb/src/external_stb/stb_image.h
      ${CMAKE_CURRENT_BINARY_DIR}/external/stb/src/external_stb/stb_image_resize.h
      ${CMAKE_CURRENT_BINARY_DIR}/external/stb/src/external_stb/stb_image_write.h
      ${CMAKE_CURRENT_BINARY_DIR}/include
)

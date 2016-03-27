cmake_minimum_required( VERSION 3.5 )

project( cities_heightfield_from_gsi )

option( CITIES_HEIGHTFIELD_FROM_GSI_TEST off )

set( cities_heightfield_from_gsi_version_major    0 )
set( cities_heightfield_from_gsi_version_minor    0 )
set( cities_heightfield_from_gsi_version_revision 0 )

include( cmake/include.cmake )

file(
  GLOB
  cities_heightfield_from_gsi_source_files
  source/*.cxx
)

add_executable(
  cities_heightfield_from_gsi
  ${cities_heightfield_from_gsi_source_files}
)

target_link_libraries(
  cities_heightfield_from_gsi
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

if( WIN32 )
  target_link_libraries(
    cities_heightfield_from_gsi
    ws2_32
    Mswsock
  )
endif()

add_dependencies(
  cities_heightfield_from_gsi
  external_usagi
  external_stb
  external_cereal
)

target_compile_options(
  cities_heightfield_from_gsi
  PUBLIC
  -DCITIES_HEIGHTFIELD_FROM_GSI_VERSION_MAJOR=${cities_heightfield_from_gsi_version_major}
  -DCITIES_HEIGHTFIELD_FROM_GSI_VERSION_MINOR=${cities_heightfield_from_gsi_version_minor}
  -DCITIES_HEIGHTFIELD_FROM_GSI_VERSION_REVISION=${cities_heightfield_from_gsi_version_revision}
)

if( CITIES_HEIGHTFIELD_FROM_GSI_TEST )
  include( CTest )
  file(
    GLOB
    cities_heightfield_from_gsi_test_source_files
    test/*.cxx
  )
  add_executable(
    cities_heightfield_from_gsi-test
    ${cities_heightfield_from_gsi_test_source_files}
  )
  target_link_libraries(
    cities_heightfield_from_gsi-test
    ${Boost_LIBRARIES}
  )
  add_dependencies(
    cities_heightfield_from_gsi-test
    external_usagi
    external_stb
    external_cereal
    external_catch
  )
  enable_testing()
  add_test(
    NAME cities_heightfield_from_gsi-test
    COMMAND bin/cities_heightfield_from_gsi-test
  )
  set_target_properties(
    cities_heightfield_from_gsi-test
    PROPERTIES COMPILE_FLAGS "-Wno-unknown-pragmas"
  )
endif()

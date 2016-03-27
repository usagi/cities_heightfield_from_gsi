file(
  GLOB
  configuration_cmake_files
  cmake/configuration/*.cmake
)

foreach( cmake_file ${configuration_cmake_files} )
  include( ${cmake_file} )
endforeach()

file(
  GLOB
  external_project_cmake_files
  cmake/external_project/*.cmake
)

foreach( cmake_file ${external_project_cmake_files} )
  include( ${cmake_file} )
endforeach()

file(
  GLOB
  lib_cmake_files
  cmake/lib/*.cmake
)

foreach( cmake_file ${lib_cmake_files} )
  include( ${cmake_file} )
endforeach()

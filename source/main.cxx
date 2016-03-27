// CITIES_HEIGHTFIELD_FROM_GSI
#include "chdir.hxx"
#include "parse_options.hxx"
#include "show_version.hxx"
#include "cities_heightfield_from_gsi.hxx"
// EXTERNAL
#include <usagi/environment_special_support.hxx>
#include <cmdline.h>
// STD
#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <cstdlib>

auto main
( const int count_of_arguments
, const char* const* const arguments
) -> int
try
{
  
  using namespace usagi::cities_heightfield_from_gsi;
  
  chdir( arguments[0] );
  
  const auto& parser = parse_options ( count_of_arguments, arguments );
  
  if ( parser.exist( constants::keyword::version ) )
  {
    show_version();
    return EXIT_SUCCESS;
  }
  
  cities_heightfield_from_gsi
  ( to_radians( parser.get< float_type  >( constants::keyword::latitude  ) * degrees )
  , to_radians( parser.get< float_type  >( constants::keyword::longitude ) * degrees )
  , parser.get< string_type >( constants::keyword::out )
  , parser.get< float_type  >( constants::keyword::altitude_offset ) * meters
  , parser.get< bool >( constants::keyword::thumbnail )
  , parser.get< bool >( constants::keyword::waters_from_relief )
  , parser.get< float_type  >( constants::keyword::waters_delta_height ) * meters
  );
  
}
catch ( const std::exception& e )
{
  
  std::cerr
      << "\nexception: \n"
      << e.what() << "\n"
      ;
  
  return EXIT_FAILURE;
  
}
catch ( ... )
{
  
  std::cerr
    << "\nexception: \n"
    << "(unknown)\n"
    ;
  
  return EXIT_FAILURE;
  
}

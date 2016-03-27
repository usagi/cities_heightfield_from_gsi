#pragma once

#include "types.hxx"
#include "constants.hxx"

#include <cmdline.h>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    auto parse_options
    ( const int count_of_arguments
    , const char* const* const arguments
    )
    {
      auto p = cmdline::parser();
      
      p.add< float_type >
      ( constants::keyword::latitude
      , constants::ascii::null
      , "latitude of a target place in degrees. eg.: --latitude " + std::to_string( constants::location::latitude_of_sapporo_hokkaido_japan.value() )
      , true
      , constants::location::latitude_of_sapporo_hokkaido_japan.value()
      );
      
      p.add< float_type >
      ( constants::keyword::longitude
      , constants::ascii::null
      , "longitude of a target place in degrees. eg.: --longitude " + std::to_string( constants::location::longitude_of_sapporo_hokkaido_japan.value() )
      , true
      , constants::location::longitude_of_sapporo_hokkaido_japan.value()
      );
      
      p.add< string_type >
      ( constants::keyword::out
      , 'o'
      , "output filename."
      , false
      , "out.raw"
      );
      
      p.add< bool >
      ( constants::keyword::thumbnail
      , 't'
      , "output .png thumbnail( 8-bit grayscale )."
      , false
      , true
      );
      
      p.add< bool >
      ( constants::keyword::waters_from_relief
      , 'w'
      , "load water locations from GSI relief tile."
      , false
      , true
      );
      
      p.add< float_type >
      ( constants::keyword::waters_delta_height
      , constants::ascii::null
      , "water locations delta height."
      , false
      , static_cast< float_type >( -10.0 )
      );
      
      p.add< float_type >
      ( constants::keyword::altitude_offset
      , constants::ascii::null
      , "altitude offset in meters. default is +40 meters."
      , false
      , static_cast< float_type >( 40 )
      );
      
      p.add
      ( constants::keyword::version
      , 'v'
      , "version information."
      );
      
      p.parse_check ( count_of_arguments, const_cast< char** > ( arguments ) );
      
      return p;
    }
    
  }
}
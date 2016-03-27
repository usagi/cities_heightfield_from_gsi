#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    using float_type = float;
    using string_type = std::string;
    
    using intermediate_type = std::vector< float_type >;
    using out_type = std::vector< std::uint16_t >;
    
    template < typename T >
    struct vec2
    {
      using value_type = T;
      value_type x, y;
    };
    
    using u32vec2_type = vec2< std::uint32_t >;
    using fvec2_type = vec2< float_type >;
    
    auto to_u32vec2_type( const fvec2_type& in )
      -> u32vec2_type
    {
      return
      { static_cast< std::uint32_t >( in.x )
      , static_cast< std::uint32_t >( in.y )
      };
    }
    
  }
}

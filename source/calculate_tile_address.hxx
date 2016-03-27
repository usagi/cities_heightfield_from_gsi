#pragma once

#include "mod.hxx"

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    
    namespace tile_type
    {
      struct regular
      {
        static auto longitude_origin_shift(){ return 0 * radians; }
        static auto latitude_origin_shift() { return 0 * radians; }
        static constexpr auto is_east_positive  = true;
        static constexpr auto is_north_positive = true;
      };
      struct gsi
      {
        static auto longitude_origin_shift(){ return -constants::math::pi * radians; }
        static auto latitude_origin_shift() { return to_radians( constants::web_mercator::critical_latitude ); }
        static constexpr auto is_east_positive  = true;
        static constexpr auto is_north_positive = false;
      };
    }
    
    auto from_latitude_to_mercator_snorm
    ( const plane_angle_type& latitude )
    {
      using namespace constants::math;
      return std::log( std::tan( ( pi + latitude.value() * 2 ) / 4 ) + epsilon ) / pi;
    }
    
    template < typename T = tile_type::gsi >
    auto calculate_tile_address
    ( const plane_angle_type& latitude
    , const plane_angle_type& longitude
    , const unsigned zoom_level
    )
      -> fvec2_type
    {
      using namespace constants::math;
      using namespace constants::web_mercator;
      
      using unsigned_type = u32vec2_type::value_type;
      
      const auto resolution = static_cast< unsigned_type >( 1 ) << zoom_level;
      const auto half_resolution = resolution >> 1;
      
      const auto critical_web_mercator_snorm = from_latitude_to_mercator_snorm ( to_radians( critical_latitude ) );
      const auto web_mercator_denominator = critical_web_mercator_snorm / half_resolution;
      const auto latitude_origin_shift_snorm = from_latitude_to_mercator_snorm ( T::latitude_origin_shift() );
      
      auto&& x
        = ( T::is_east_positive ? 1 : -1 )
          * math::mod( ( longitude - T::longitude_origin_shift() ).value(), 2 * pi ) / ( 2 * pi )
          * resolution
          ;
      
      auto&& y
        = 
          math::mod
          ( ( T::is_north_positive ? 1 : -1 )
            * ( from_latitude_to_mercator_snorm( latitude ) + latitude_origin_shift_snorm )
            / web_mercator_denominator
          , static_cast< float_type >( resolution )
          );
      
      return
      { std::forward< float_type >( x )
      , std::forward< float_type >( y )
      };
    }
    
  }
}

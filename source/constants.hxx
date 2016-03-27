#pragma once

#include "units.hxx"
#include "types.hxx"
#include <boost/math/constants/constants.hpp>
#include <cmath>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    
    namespace constants
    {
      namespace keyword
      {
        constexpr auto version   = "version";
        constexpr auto latitude  = "latitude";
        constexpr auto longitude = "longitude";
        constexpr auto out       = "out";
        constexpr auto altitude_offset = "altitude_offset";
        constexpr auto thumbnail = "thumbnail";
        constexpr auto waters_from_relief  = "waters_from_relief";
        constexpr auto waters_delta_height = "waters_delta_height";
      }
      
      namespace ascii
      {
        constexpr auto null = '\0';
      }
      
      namespace http
      {
        constexpr auto end_of_line = "\r\n";
      }
      
      namespace math
      {
        constexpr auto pi = boost::math::constants::pi< float_type >();
        constexpr auto epsilon = std::numeric_limits< float_type >::epsilon();
      }
      
      namespace web_mercator
      {
        const auto critical_latitude = 85.051'129 * degrees;
      }
      
      namespace earth
      {
        namespace wgs84
        {
          constexpr auto flettening    = static_cast< float_type >( 1.0 / 2.982'572'235'63e+2 );
          const auto equatorial_radius = static_cast< float_type >( 6.378'137e+6 ) * meters;
          const auto equator_length    = equatorial_radius * 2 * math::pi;
        }
      }
      
      namespace location
      {
        const auto latitude_of_sapporo_hokkaido_japan  = static_cast< float_type >(  43.062096 ) * degrees;
        const auto longitude_of_sapporo_hokkaido_japan = static_cast< float_type >( 141.354376 ) * degrees;
      }
      
      namespace gsi
      {
        constexpr auto pixels_in_tile_arris = static_cast< unsigned >( 256 );
        constexpr auto dem_tile_zoom_level = static_cast< unsigned >( 14 );
        const auto pixel_zoom_level = dem_tile_zoom_level + static_cast< unsigned >( std::log2( pixels_in_tile_arris ) );
        const auto subdivision_ratio = static_cast< float_type >( 1 ) / ( 1 << pixel_zoom_level );
        constexpr auto dem_url_pattern = "http://cyberjapandata.gsi.go.jp/xyz/dem/{z}/{x}/{y}.txt";
        constexpr auto url_x_pattern = "{x}";
        constexpr auto url_y_pattern = "{y}";
        constexpr auto url_z_pattern = "{z}";
        constexpr auto relief_url_pattern = "http://cyberjapandata.gsi.go.jp/xyz/relief/{z}/{x}/{y}.png";
        constexpr auto relief_water_color_0_r = static_cast< std::uint8_t >( 145 );
        constexpr auto relief_water_color_0_g = static_cast< std::uint8_t >( 181 );
        constexpr auto relief_water_color_0_b = static_cast< std::uint8_t >( 181 );
        constexpr auto relief_water_color_1_r = static_cast< std::uint8_t >(   0 );
        constexpr auto relief_water_color_1_g = static_cast< std::uint8_t >( 128 );
        constexpr auto relief_water_color_1_b = static_cast< std::uint8_t >( 255 );
      }
      
      namespace cities_skylines
      {
        constexpr auto pixels_in_tile_arris = static_cast< unsigned >( 1081 );
        const auto pixel_scaling = 8 * meters;
        const auto tile_arris_length = pixel_scaling * pixels_in_tile_arris;
        const auto height_max = 1024 * meters;
        // [ m / value(cities) ]
        const auto height_scaling = static_cast< float_type >( height_max.value() ) / std::numeric_limits< std::uint16_t >::max() * meters;
      }
      
    }
    
  }
}
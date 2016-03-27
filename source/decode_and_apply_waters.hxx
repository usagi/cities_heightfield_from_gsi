#pragma once

#define STB_IMAGE_IMPLEMENTATION
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#include <stb_image.h>
#pragma GCC diagnostic pop
#include <memory>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    template
    < typename T = float_type
    , std::size_t DESTINATION_AREA_ARRIS_SIZE = constants::gsi::pixels_in_tile_arris
    >
    auto decode_and_apply_waters
    ( const std::uint8_t* png
    , const std::size_t size
    , T* destination
    , std::size_t destination_line_stride
    , const length_type& waters_delta_height
    )
    {
      int width = 0, height = 0, elements = 0;
      
      const auto deleter = []( void* p ){ stbi_image_free( p ); };
      
      const auto decoded_data = std::unique_ptr< std::uint8_t, decltype( deleter ) >
      ( stbi_load_from_memory
        ( png
        , size
        , &width
        , &height
        , &elements
        , STBI_rgb
        )
      , deleter
      );
      
      for ( auto n = 0u; n < DESTINATION_AREA_ARRIS_SIZE * DESTINATION_AREA_ARRIS_SIZE; )
      {
        constexpr auto r = 0;
        constexpr auto g = 1;
        constexpr auto b = 2;
        
        constexpr auto rgb_elements = 3;
        
        const auto current_decoded_data = decoded_data.get() + n * rgb_elements;
        
        if
        (
          (   current_decoded_data[ r ] == constants::gsi::relief_water_color_0_r
          and current_decoded_data[ g ] == constants::gsi::relief_water_color_0_g
          and current_decoded_data[ b ] == constants::gsi::relief_water_color_0_b
          )
        or
          (   current_decoded_data[ r ] == constants::gsi::relief_water_color_1_r
          and current_decoded_data[ g ] == constants::gsi::relief_water_color_1_g
          and current_decoded_data[ b ] == constants::gsi::relief_water_color_1_b
          )
        )
          *( destination ) += waters_delta_height.value();
        
        ++destination;
        
        if ( ++n % DESTINATION_AREA_ARRIS_SIZE == 0 )
          destination += destination_line_stride - DESTINATION_AREA_ARRIS_SIZE;
      }
      
    }
    
  }
}
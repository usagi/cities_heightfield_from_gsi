#pragma once

#include <cmath>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    
    auto calculate_required_count_of_gsi_tile_in_arris
    ( const length_type& gsi_dem_tile_pixel_scaling )
    {
      return
        ( constants::cities_skylines::tile_arris_length
          / ( gsi_dem_tile_pixel_scaling * constants::gsi::pixels_in_tile_arris )
        ).value()
        ;
    }
    
    auto calculate_required_count_of_gsi_tile_in_arris_with_outer
    ( const float_type required_count_of_gsi_tile_in_arris
    , const unsigned outer_tiles = 1
    )
    {
      return 
        static_cast< unsigned >( std::ceil( required_count_of_gsi_tile_in_arris ) )
        + outer_tiles
        ;
    }
    
  }
}
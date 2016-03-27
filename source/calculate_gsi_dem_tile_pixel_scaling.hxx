#pragma once

#include <cmath>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    
    auto calculate_gsi_dem_tile_pixel_scaling
    ( const plane_angle_type& latitude )
    {
      return
          constants::earth::wgs84::equator_length
        * cos ( latitude.value() )
        * constants::gsi::subdivision_ratio
        ;
    }
    
  }
}
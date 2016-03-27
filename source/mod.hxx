#pragma once

#include <cmath>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    namespace math
    {
      
      template < typename IN_A, typename IN_B >
      auto mod( IN_A in_a, IN_B in_b )
      { return in_a - in_b * std::floor( in_a / in_b ); }
      
    }
  }
}
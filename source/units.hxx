#pragma once

#include "types.hxx"
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/io.hpp>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    
    using namespace boost::units;
    using namespace boost::units::si;
    using namespace boost::units::degree;
    
    using plane_angle_radians_type = quantity< si::plane_angle, float_type >;
    using plane_angle_degrees_type = quantity< degree::plane_angle, float_type >;
    
    using plane_angle_type = plane_angle_radians_type;
    
    using length_type = quantity< length, float_type >;
    
    template < typename IN >
    inline auto to_radians( const IN& in )
    { return static_cast< plane_angle_radians_type >( in ); }
    
    template < typename IN >
    inline auto to_degrees( const IN& in )
    { return static_cast< plane_angle_degrees_type >( in ); }
    
    template
    < typename UNIT
    , typename IN_BASE
    , typename IN_FACTOR
    >
    inline auto operator*
    ( const IN_FACTOR& in_factor
    , const quantity< UNIT, IN_BASE >& in_base
    )
    { return static_cast< IN_BASE >( in_factor ) * in_base; }

    template
    < typename UNIT
    , typename IN_FACTOR
    , typename IN_BASE
    >
    inline auto operator*
    ( const quantity< UNIT, IN_BASE >& in_base
    , const IN_FACTOR& in_factor
    )
    { return in_base * static_cast< IN_BASE >( in_factor ); }
  }
}
#include <string>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    
    namespace detail
    {
      template < typename T = float >
      inline auto stof( const std::string& value ) { return std::stof( value ); }
      
      template < >
      inline auto stof< double >( const std::string& value ) { return std::stod( value ); }
      
      template < >
      inline auto stof< long double >( const std::string& value ) { return std::stold( value ); }
    }
    
    template
    < typename T = float_type
    , std::size_t DESTINATION_AREA_ARRIS_SIZE = constants::gsi::pixels_in_tile_arris
    >
    auto decode_csv
    ( std::string csv
    , T* destination
    , std::size_t destination_line_stride
    )
    {
      constexpr auto count_of_values = DESTINATION_AREA_ARRIS_SIZE * DESTINATION_AREA_ARRIS_SIZE;
      
      std::size_t counter = 0;
      
      auto* in           = &csv[0];
      const auto* in_end = in + csv.size();
      
      while ( in < in_end )
      {
        const auto in_0 = in;
        
        while ( ++in < in_end )
          switch( *in )
          { case ',':
            case '\n':
              *in = '\0';
              goto csv_while_break_0;
          }
csv_while_break_0:
        
        if ( counter >= count_of_values )
          throw std::runtime_error( "too many values in the CSV: " + std::to_string( counter ) + " ( or more values )" );
        
        if ( *in_0 != 'e' )
          try { *destination += T( detail::stof( in_0 ) ); }
          catch ( ... ) { }
        else
          *destination = 0;
        
        ++destination;
        
        if ( ++counter % DESTINATION_AREA_ARRIS_SIZE == 0 )
          destination += destination_line_stride - DESTINATION_AREA_ARRIS_SIZE;
        
        while ( ++in < in_end )
          switch( *in )
          { case ',':
            case '\n':
              continue;
            default:
              goto csv_while_break_1;
          }
csv_while_break_1:
        ;
      }
      
      if ( counter < count_of_values )
        throw std::runtime_error( "too few values in the CSV: " + std::to_string( counter ) );
    }
    
  }
}
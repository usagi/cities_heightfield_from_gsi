#pragma once

#include "types.hxx"
#include "constants.hxx"
#include "units.hxx"

#include "calculate_gsi_dem_tile_pixel_scaling.hxx"
#include "calculate_required_count_of_gsi_tile_in_arris.hxx"
#include "calculate_tile_address.hxx"
#include "http.hxx"
#include "decode_csv.hxx"
#include "decode_and_apply_waters.hxx"

#include <boost/filesystem.hpp>
#include <boost/xpressive/xpressive_static.hpp>
#include <boost/xpressive/regex_actions.hpp>
#include <iostream>
#include <iomanip>
#include <future>
#include <unordered_map>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    
    auto cities_heightfield_from_gsi
    ( const plane_angle_type& latitude
    , const plane_angle_type& longitude
    , const string_type&      out_filename
    , const length_type&      altitude_offset
    , const bool              thumbnail
    , const bool              waters_from_relief
    , const length_type&      waters_delta_height
    )
    {
      // 1. show input values
      std::cout
        << "[ Check Input Values ]\n"
           "  Latitude            : " << to_degrees( latitude  ) << " ( = " << latitude  << " )\n"
           "  Longitude           : " << to_degrees( longitude ) << " ( = " << longitude << " )\n"
           "  Out File            : " << out_filename    << "\n"
           "  Altitude Offset     : " << altitude_offset << "\n"
           "  Thumbnail           : " << std::boolalpha  << thumbnail << "\n"
           "  Waters from Relief  : " << waters_from_relief  << "\n"
           "  Waters Delta Height : " << waters_delta_height << '\n'
        << std::endl;
      
      // 2. caclulate a pixel scaling of GSI DEM tile from a latitude
      const auto gsi_dem_tile_pixel_scaling = calculate_gsi_dem_tile_pixel_scaling( latitude );
      
      std::cout
        << "[ Calculate a GSI DEM Tile Pixel Scaling ( in the Current Altitude ) ]\n"
        << "  Scaling: " << gsi_dem_tile_pixel_scaling << " / GSI-DEM-TILE-PIXEL\n"
        << std::endl;
      
      // 3. calculate a count of GSI tiles in arris required
      const auto required_count_of_gsi_tile_in_arris
        = calculate_required_count_of_gsi_tile_in_arris( gsi_dem_tile_pixel_scaling );
      
      constexpr auto required_outer_tiles = 1;
      const auto required_count_of_gsi_tile_in_arris_with_outer
        = calculate_required_count_of_gsi_tile_in_arris_with_outer
          ( required_count_of_gsi_tile_in_arris
          , required_outer_tiles
          )
          ;
      
      std::cout
        << "[ Calculate Count of GSI TILE in Arris Required ]\n"
           "  in Real              : " << required_count_of_gsi_tile_in_arris            << " # \n"
           "  Required Outer Tiles : " << required_outer_tiles                           << " # \n"
           "  Ceiled with Outer    : " << required_count_of_gsi_tile_in_arris_with_outer << " # \n"
        << std::endl;
      
      // 4. calculate a center tile address
      const auto real_address_of_center_tile
        = calculate_tile_address
          ( latitude
          , longitude
          , constants::gsi::dem_tile_zoom_level
          );
      
      const auto address_of_center_tile = to_u32vec2_type( real_address_of_center_tile );
      
      std::cout
        << "[ Check Address of Center Tile in GSI ]\n"
           "  x : " << address_of_center_tile.x << " ( " << real_address_of_center_tile.x << " )\n"
           "  y : " << address_of_center_tile.y << " ( " << real_address_of_center_tile.y << " )\n"
        << std::endl;
      
      // 5. prepare intermediate buffer
      const auto intermediate_arris_pixels = required_count_of_gsi_tile_in_arris_with_outer * constants::gsi::pixels_in_tile_arris;
      
      using intermediate_type = std::vector< float_type >;
      intermediate_type intermediate( intermediate_arris_pixels * intermediate_arris_pixels, altitude_offset.value() );
      
      std::cout
        << "[ Prepare the Intermediate Buffer ]\n"
           "  arris pixels             : " << intermediate_arris_pixels                  << " pixels\n"
           "  count of buffer elements : " << intermediate.size()                        << " #\n"
           "  size of buffer           : " << intermediate.size() * sizeof( float_type ) << " bytes\n"
        << std::endl;
      
      // 6. load tiles
      const auto tile_address_of_left_top = u32vec2_type
      { address_of_center_tile.x - ( required_count_of_gsi_tile_in_arris_with_outer >> 1 )
      , address_of_center_tile.y - ( required_count_of_gsi_tile_in_arris_with_outer >> 1 )
      };
      
      {
        auto counter = 0;
        const auto count_of_reuiqred_tiles = required_count_of_gsi_tile_in_arris_with_outer * required_count_of_gsi_tile_in_arris_with_outer;
        
        std:: cout << "[ Get GSI DEM tiles ]" << std::endl;
        
        for ( auto delta_y = 0u; delta_y < required_count_of_gsi_tile_in_arris_with_outer; ++delta_y )
        {
          const auto current_tile_address_y = tile_address_of_left_top.y + delta_y;
          for ( auto delta_x = 0u; delta_x < required_count_of_gsi_tile_in_arris_with_outer; ++delta_x )
          {
            const auto current_tile_address_x = tile_address_of_left_top.x + delta_x;
            
            ++counter;
            
            using namespace boost::xpressive;
            using namespace std::literals::string_literals;
            
            const std::unordered_map< std::string, std::string > replacements =
            { { "{x}"s, std::to_string( current_tile_address_x ) }
            , { "{y}"s, std::to_string( current_tile_address_y ) }
            , { "{z}"s, std::to_string( constants::gsi::dem_tile_zoom_level ) }
            };
            
            local< const std::string* > regex_string_pointer;
            const sregex r = ( a1 = replacements )[ regex_string_pointer = &a1 ];
            
            const auto& dem_url = regex_replace
            ( std::string( constants::gsi::dem_url_pattern )
            , r
            , *regex_string_pointer
            );
            
            std::cout << "  ( " << std::setw( 2 ) <<  counter << " / " << count_of_reuiqred_tiles << " ) : " << dem_url << ' ';
            
            auto future_get_http_dem = std::async
            ( std::launch::async
            , [ & ] () -> std::string { return get_http( dem_url ); }
            );
            
            do
              std::cout << '.' << std::flush;
            while ( future_get_http_dem.wait_for( std::chrono::milliseconds( 100 ) ) != std::future_status::ready );
            
            std::string csv;
            
            try
            { csv = future_get_http_dem.get(); }
            catch ( const http_error& e )
            { std::cout << " -> error! ( HTTP STATUS = " << std::to_string( e.status ) << " )" << std::endl; continue; }
            catch ( const std::exception& e )
            { std::cout << " -> error! ( what = " << e.what() << " )" << std::endl; continue; }
            catch ( ... )
            { std::cout << " -> error! ( unexpected exception )" << std::endl; continue; }
            
            std::cout << " -> OK ; decode " << std::flush;
            
            auto future_decode_csv = std::async
            ( std::launch::async
            , [ & ]
              {
                decode_csv
                ( csv
                , &intermediate.at( delta_x * constants::gsi::pixels_in_tile_arris + delta_y * constants::gsi::pixels_in_tile_arris * intermediate_arris_pixels )
                , intermediate_arris_pixels
                );
              }
            );
            
            do
              std::cout << '.' << std::flush;
            while ( future_decode_csv.wait_for( std::chrono::milliseconds( 100 ) ) != std::future_status::ready );
            
            try
            { future_decode_csv.get(); }
            catch ( const std::exception& e )
            { std::cout << " -> error! ( what = " << e.what() << " )" << std::endl; continue; }
            catch ( ... )
            { std::cout << " -> error! ( unexpected exception )" << std::endl; continue; }
            
            std::cout << " -> OK" << std::endl;
            
            if ( waters_from_relief )
            {
              auto&& relief_url = regex_replace
              ( std::string( constants::gsi::relief_url_pattern )
              , r
              , *regex_string_pointer
              );
              std::cout << "              : " << relief_url << std::flush;
              
              auto future_get_http_relief = std::async
              ( std::launch::async
              , [ & ] () -> std::string { return get_http( relief_url ); }
              );
              
              do
                std::cout << '.' << std::flush;
              while ( future_get_http_relief.wait_for( std::chrono::milliseconds( 100 ) ) != std::future_status::ready );
              
              std::string png;
              
              try
              { png = future_get_http_relief.get(); }
              catch ( const http_error& e )
              { std::cout << " -> error! ( HTTP STATUS = " << std::to_string( e.status ) << " )" << std::endl; continue; }
              catch ( const std::exception& e )
              { std::cout << " -> error! ( what = " << e.what() << " )" << std::endl; continue; }
              catch ( ... )
              { std::cout << " -> error! ( unexpected exception )" << std::endl; continue; }
              
              std::cout << " -> OK ; decode & apply water locations " << std::flush;
              auto future_decode_and_apply_waters = std::async
              ( std::launch::async
              , [ & ]
                {
                  decode_and_apply_waters
                  ( reinterpret_cast< const std::uint8_t* >( png.data() )
                  , png.size()
                  , &intermediate.at( delta_x * constants::gsi::pixels_in_tile_arris + delta_y * constants::gsi::pixels_in_tile_arris * intermediate_arris_pixels )
                  , intermediate_arris_pixels
                  , waters_delta_height
                  );
                }
              );
              
              do
                std::cout << '.' << std::flush;
              while ( future_decode_and_apply_waters.wait_for( std::chrono::milliseconds( 100 ) ) != std::future_status::ready );
              
              try
              { future_decode_and_apply_waters.get(); }
              catch ( const std::exception& e )
              { std::cout << " -> error! ( what = " << e.what() << " )" << std::endl; continue; }
              catch ( ... )
              { std::cout << " -> error! ( unexpected exception )" << std::endl; continue; }
              
              std::cout << " -> OK" << std::endl;
            }
          }
        }
      }
      
      std::cout << std::endl;
      
      // 7. prepare final output buffer
      std::vector< std::uint16_t > out( constants::cities_skylines::pixels_in_tile_arris * constants::cities_skylines::pixels_in_tile_arris );
      
      std::cout
        << "[ Prepare the Final Output Buffer ]\n"
           "  arris pixels             : " << constants::cities_skylines::pixels_in_tile_arris << " pixels\n"
           "  count of buffer elements : " << out.size()                           << " #\n"
           "  size of buffer           : " << out.size() * sizeof( std::uint16_t ) << " bytes\n"
        << std::endl;
      
      // 8. sampling from intermediate buffer to final output buffer
      std::cout
        << "[ Sample an Intermediate Buffer ]\n"
           "  sampling : "
        ;
      
      auto future_samling = std::async
      ( std::launch::async
      , [ & ]
        {
          const auto source_pixel_center_x = ( real_address_of_center_tile.x - tile_address_of_left_top.x ) * constants::gsi::pixels_in_tile_arris;
          const auto source_pixel_center_y = ( real_address_of_center_tile.y - tile_address_of_left_top.y ) * constants::gsi::pixels_in_tile_arris;
          
          for ( auto delta_y = 0u; delta_y < constants::cities_skylines::pixels_in_tile_arris; ++delta_y )
          {
            // dimension: [pixels(cities)]
            const auto destination_pixel_delta_y  = delta_y - constants::cities_skylines::pixels_in_tile_arris / static_cast< float_type >( 2 );
            // dimension: [meters] = [pixels(cities)] * [meters/pixels(cities)]
            const auto destination_meters_delta_y = destination_pixel_delta_y * constants::cities_skylines::pixel_scaling;
            // dimension: [pixels(GSI)] = [meters] / [meters/pixels(GSI)]
            const auto source_pixel_delta_y       = ( destination_meters_delta_y / gsi_dem_tile_pixel_scaling ).value();
            
            for ( auto delta_x = 0u; delta_x < constants::cities_skylines::pixels_in_tile_arris; ++delta_x )
            {
              // dimension: [pixels(cities)]
              const auto destination_pixel_delta_x  = delta_x - constants::cities_skylines::pixels_in_tile_arris / static_cast< float_type >( 2 );
              // dimension: [meters] = [pixels(cities)] * [meters/pixels(cities)]
              const auto destination_meters_delta_x = destination_pixel_delta_x * constants::cities_skylines::pixel_scaling;
              // dimension: [pixels(GSI)] = [meters] / [meters/pixels(GSI)]
              const auto source_pixel_delta_x       = ( destination_meters_delta_x / gsi_dem_tile_pixel_scaling ).value();
              
              // bilinear interpolation
              // note: bicubic or the other some algorithms raise a incorrect terrain edge problem. thus, it was implement using bilinear algorithm.
              const auto source_pixel_x  = source_pixel_center_x + source_pixel_delta_x;
              const auto source_pixel_x0 = static_cast< unsigned >( std::floor( source_pixel_x ) );
              const auto source_pixel_x1 = source_pixel_x0 + 1;
              const auto source_pixel_x0_belnding_ratio = source_pixel_x1 - source_pixel_x;
              const auto source_pixel_x1_belnding_ratio = static_cast< float_type >( 1 ) - source_pixel_x0_belnding_ratio;
              
              const auto source_pixel_y  = source_pixel_center_y + source_pixel_delta_y;
              const auto source_pixel_y0 = static_cast< unsigned >( std::floor( source_pixel_y ) );
              const auto source_pixel_y1 = source_pixel_y0 + 1;
              const auto source_pixel_y0_belnding_ratio = source_pixel_y1 - source_pixel_y;
              const auto source_pixel_y1_belnding_ratio = static_cast< float_type >( 1 ) - source_pixel_y0_belnding_ratio;
              
              const auto datum_x0 = intermediate.at( source_pixel_x0 + source_pixel_y0 * intermediate_arris_pixels );
              const auto datum_x1 = intermediate.at( source_pixel_x1 + source_pixel_y0 * intermediate_arris_pixels );
              const auto datum_x = datum_x0 * source_pixel_x0_belnding_ratio + datum_x1 * source_pixel_x1_belnding_ratio;
              
              const auto datum_y0 = intermediate.at( source_pixel_x0 + source_pixel_y0 * intermediate_arris_pixels );
              const auto datum_y1 = intermediate.at( source_pixel_x0 + source_pixel_y1 * intermediate_arris_pixels );
              const auto datum_y = datum_y0 * source_pixel_y0_belnding_ratio + datum_y1 * source_pixel_y1_belnding_ratio;
              
              const auto height =
              std::max< float_type >
              ( 0
              , std::min< float_type >
                ( constants::cities_skylines::height_max.value()
                , std::round( ( datum_x + datum_y ) / 2 )
                )
              ) * meters;
              
              const auto final_value = static_cast< std::uint16_t >( ( height / constants::cities_skylines::height_scaling ).value() );
              
              out.at( delta_x + delta_y * constants::cities_skylines::pixels_in_tile_arris ) = final_value;
            }
          }
        }
      );
      
      do
        std::cout << '.' << std::flush;
      while ( future_samling.wait_for( std::chrono::milliseconds( 10 ) ) != std::future_status::ready );
      
      // check an exception
      future_samling.get();
      
      std::cout << " done\n" << std::endl;
      
      // 9. write out to a file
      {
        std::ofstream s( out_filename, std::ios::binary | std::ios::out );
        s.write
        ( reinterpret_cast< const char* >( out.data() )
        , out.size() * sizeof( decltype( out )::value_type )
        );
        
        std::cout
          << "[ Output to a File ]\n"
            "  filename : " << out_filename << " done\n"
          << std::endl;
      }
      
      // 10. optional write out thumbnail
      std::cout
        << "[ thumbnail ]\n"
        << "  thumbnail : " << std::boolalpha << thumbnail << '\n'
        ;
      
      if ( thumbnail )
      {
        std::vector< std::uint8_t > buffer( out.size() );
        auto i = buffer.begin();
        for ( auto& v : out )
        {
          *i++ = static_cast< std::uint8_t >
            ( std::max< float_type >
              ( 0, std::min< float_type >
                ( 1
                , static_cast< float_type >( v ) / std::numeric_limits< std::uint16_t >::max()
                )
              ) * std::numeric_limits< std::uint8_t >::max()
            );
        }
        std::string thumbnail_filename = boost::filesystem::change_extension( out_filename, "png" ).string();
        constexpr auto components = 1;
        constexpr auto stride = 0;
        stbi_write_png
        ( thumbnail_filename.c_str()
        , constants::cities_skylines::pixels_in_tile_arris
        , constants::cities_skylines::pixels_in_tile_arris
        , components
        , buffer.data()
        , stride
        );
        std::cout << "  filename : " << thumbnail_filename << '\n';
      }
      
      std::cout << std::endl;
      
      std::cout
        << "We're looking forward to that your Japanese real maps is published. Enjoy cities!\n"
           "  Notice: You don't forgot check GSI Licensing before publishing :)\n"
        << std::endl;
    }
    
  }
}
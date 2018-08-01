#![feature(extern_prelude)]
#[macro_use]
extern crate lazy_static;
extern crate reqwest;
extern crate image;
extern crate resize;

pub mod gsi;
pub mod cities_skylines;
pub mod lib_error;
pub mod gsj;
pub mod wgs84;
pub mod web_mercator;

use std::mem;
use std::io::Write;
use lib_error::{ Error };

pub fn empty_test() { }

pub fn invoke_with_save_to_file
( longitude: f64
, latitude: f64
, out_path: &str
, altitude_bias: f64
, altitude_of_no_data: f64
)
  -> Result< (), Error >
{
  let cropped_data = invoke( longitude, latitude, altitude_bias, altitude_of_no_data )?;
  let _ = save( &cropped_data, out_path )?;
  
  println!( "COMPLETED" );
  
  Ok( () )
}

pub fn invoke
( longitude: f64
, latitude: f64
, altitude_bias: f64
, altitude_of_no_data: f64
)
  -> Result< Vec< u16 >, Error >
{
  let level_of_detail = 14;
  
  let ( loaded_data, loaded_width, meters_per_pixel, roi_width, roi_rx, roi_ry ) = 
    load( longitude, latitude, level_of_detail, altitude_bias, altitude_of_no_data )?;
  
  let ( scaled_data, scaled_width ) = 
    scale( &loaded_data, loaded_width, roi_width, meters_per_pixel )?;
  
  Ok( crop( &scaled_data, scaled_width, roi_rx, roi_ry )? )
}

pub fn load
( longitude: f64
, latitude: f64
, level_of_detail: u8
, altitude_bias: f64
, altitude_of_no_data: f64
)
  -> Result< ( Vec< u16 >, usize, f64, usize, f64, f64 ), Error >
{
  let meters_per_pixel = web_mercator::meters_per_pixel( latitude, level_of_detail );
  let ( px, py ) = web_mercator::angle_to_pixel( longitude, latitude, level_of_detail );
  
  let delta_px = ( cities_skylines::consts::METERS_PER_TILE_ARRIS / 2.0 / meters_per_pixel ).floor() as u32;
  
  // [ x-left
  let px0 = px - delta_px;
  // x-right ]
  let px1 = px + delta_px;
  
  // [ y-top
  let py0 = py - delta_px;
  // y-bottom ]
  let py1 = py + delta_px;
  
  // [ (left,top)
  let ( tx0, ty0 ) = web_mercator::pixel_to_tile( px0, py0 );
  // (right,bottom) ]
  let ( tx1, ty1 ) = web_mercator::pixel_to_tile( px1, py1 );
  
  let tiles_buffer_width = ( tx1 + 1 - tx0 ) as usize;
  let tiles_buffer_height = ( ty1 + 1 - ty0 ) as usize;
  let pixels_buffer_width = tiles_buffer_width * web_mercator::consts::PIXELS_IN_TILE_ARRIS as usize;
  let pixels_buffer_height = tiles_buffer_height * web_mercator::consts::PIXELS_IN_TILE_ARRIS as usize;
  let pixels_buffer_area = pixels_buffer_width * pixels_buffer_height;
  
  let tiles_buffer_area = tiles_buffer_width * tiles_buffer_height;
  
  println!("[ LOADING ] " );
  println!("  : Target Longitude {:>7} [deg]", format!( "{:.6}", longitude ) );
  println!("  : Target Latitude  {:>7} [deg]", format!( "{:.6}", latitude ) );
  println!("  : Target Resolution {:3} [m/px] ( in the latitude)", meters_per_pixel );
  println!("  : Target Area Arris Length {:3} [m]", cities_skylines::consts::METERS_PER_TILE_ARRIS );
  println!("  : Target Pixels X-range [ {:>8} .. {:>8} ] ( in LOD=14 )", px0, px1 );
  println!("  : Target Pixels Y-range [ {:>8} .. {:>8} ] ( in LOD=14 )", py0, py1 );
  println!("  : Target Tiles X-range [ {:>8} .. {:>8} ] ( in LOD=14 )", tx0, tx1 );
  println!("  : Target Tiles Y-range [ {:>8} .. {:>8} ] ( in LOD=14 )", ty0, ty1 );
  println!("  : Target Tiles Total Count {} [#] ", tiles_buffer_area );
  println!("  : Altitude bias       {:>8} [m]", format!( "{:.3}", altitude_bias ) );
  println!("  : Altitude of no data {:>8} [m]", format!( "{:.3}", altitude_of_no_data ) );
  
  let tx_end = tx1 + 1;
  let ty_end = ty1 + 1;
  let area_size = ( tx_end - tx0 ) * ( ty_end - ty0 );
  let mut loaded_data = vec![ 0u16; pixels_buffer_area as usize ];
  loaded_data.reserve( area_size as usize * gsi::consts::PIXELS_PER_TILE_ARRIS as usize * gsi::consts::PIXELS_PER_TILE_ARRIS as usize );
  let mut current_tile = 0;
  
  for ty in ty0 .. ty_end
  {
    for tx in tx0 .. tx_end
    {
      current_tile += 1;
      println!( "  : [ {:>2} / {:>2} ] tile( x:{} y:{} z:{} )", current_tile, area_size, tx, ty, 14 );
      
      let t = 
        match gsi::get_tile( gsi::consts::ID_DEM_PNG, tx, ty, 14 )
        { Ok( blob ) => blob
        , Err( e ) => { println!( "    ! {:?}", e ); continue; }
        };
      let i = image::load_from_memory( &t ).map_err( | e | Error::ImageError( e ) )?;
      let a = gsj::decode_altitudes_from_png_nishioka_nagatsu_2015( i, gsi::consts::DEM_PNG_SCALING )?;
      
      let destination_address_base = ( ty - ty0 ) as usize * web_mercator::consts::PIXELS_IN_TILE_ARRIS as usize * pixels_buffer_width + ( tx - tx0 ) as usize * web_mercator::consts::PIXELS_IN_TILE_ARRIS as usize;
      
      for py in 0usize .. web_mercator::consts::PIXELS_IN_TILE_ARRIS as usize
      {
        let source_address_dy = py as usize * web_mercator::consts::PIXELS_IN_TILE_ARRIS as usize;
        let destination_address_dy = destination_address_base + py * pixels_buffer_width;
        for px in 0usize .. web_mercator::consts::PIXELS_IN_TILE_ARRIS as usize
        {
          let source_address = source_address_dy + px;
          let destination_address = destination_address_dy + px;
          
          let v = 
            match a[ source_address ]
            { n if n.is_nan() => altitude_of_no_data
            , n => n + altitude_bias
            };
          
          loaded_data[ destination_address ] = cities_skylines::height_from_meter_to_value( v );
        }
      }
    }
  }
  
  let roi_width = ( px1 + 1 - px0 ) as usize;
  let roi_rx = ( px0 - tx0 * web_mercator::consts::PIXELS_IN_TILE_ARRIS ) as f64 / pixels_buffer_width as f64;
  let roi_ry = ( py0 - ty0 * web_mercator::consts::PIXELS_IN_TILE_ARRIS ) as f64 / pixels_buffer_height as f64;
  
  Ok
  ( ( loaded_data
    , pixels_buffer_width
    , meters_per_pixel
    , roi_width
    , roi_rx
    , roi_ry
    )
  )
}

fn scale
( loaded_data: &[ u16 ]
, buffer_width: usize
, roi_width: usize
, meters_per_pixel: f64
) -> Result< ( Vec< u16 >, usize ), Error >
{
  println!( "[ SCALING ]" );
  let buffer_height = loaded_data.len() / buffer_width;
  let scaling_factor = cities_skylines::consts::PIXELS_PER_TILE_ARRIS as f64 / roi_width as f64;
  let scaled_width = ( buffer_width as f64 * scaling_factor ) as usize;
  let scaled_height = ( buffer_height as f64 * scaling_factor ) as usize;
  println!( "  : GSI ( Region of Export ) {v:>5} [px] * {v:>5} [px] {r:>7} [m/px]", v = roi_width, r = format!( "{:.3}", meters_per_pixel ) );
  println!( "  : Cities Skylines          {v:>5} [px] * {v:>5} [px] {r:>7} [m/px]", v = cities_skylines::consts::PIXELS_PER_TILE_ARRIS, r = format!( "{:.3}", cities_skylines::consts::METERS_PER_PIXEL ) );
  println!( "  : Scaling Factor {:.3} [-]", scaling_factor );
  println!( "  : Input Size  {:>5} [px] * {:>5} [px]", buffer_width, buffer_height );
  println!( "  : Output Size {:>5} [px] * {:>5} [px]", scaled_width, scaled_height );
  println!( "  : with {} interpolation", "Mitchell-Netravali" );
  println!( "  : ... ( PROCESSING; Wait a moment please. ) ..." );
  
  // scaled pixels
  let mut scaled_data = vec! [ 0u16; scaled_width * scaled_height ];
  let mut resizer = 
    resize::new
    ( buffer_width
    , buffer_height
    , scaled_width
    , scaled_height
    , resize::Pixel::Gray16
    , resize::Type::Mitchell
    );
  resizer.resize( &loaded_data, &mut scaled_data );
  
  Ok( ( scaled_data, scaled_width ) )
}

fn crop( scaled_data: &[ u16 ], scaled_width: usize, roi_rx: f64, roi_ry: f64 )
  -> Result< Vec< u16 >, Error >
{
  println!( "[ CROPPING ]" );
  let scaled_height = scaled_data.len() / scaled_width;
  println!( "  : from {:>7} [px] * {:>7} [px]", scaled_width, scaled_height );
  println!( "  : to   {v:>7} [px] * {v:>7} [px]", v = cities_skylines::consts::PIXELS_PER_TILE_ARRIS );
  println!( "  : ... ( PROCESSING; Wait a moment please. ) ..." );
  // final pixels
  let mut cropped_data = vec! [ 0u16; cities_skylines::consts::PIXELS_PER_TILE ];
  cropped_data.reserve( cities_skylines::consts::PIXELS_PER_TILE_ARRIS as usize * cities_skylines::consts::PIXELS_PER_TILE_ARRIS as usize );
  let source_0_dx = ( roi_rx * scaled_width as f64 ) as usize;
  let source_0_dy = ( roi_ry * scaled_height as f64 ) as usize;
  let source_0 = source_0_dy * scaled_width + source_0_dx;
  for y in 0usize .. cities_skylines::consts::PIXELS_PER_TILE_ARRIS as usize
  {
    let destination_address_dy = y * cities_skylines::consts::PIXELS_PER_TILE_ARRIS;
    let source_address_dy = source_0 + y * scaled_width;
    for x in 0usize .. cities_skylines::consts::PIXELS_PER_TILE_ARRIS as usize
    {
      let destination_address = destination_address_dy + x;
      let source_address = source_address_dy + x;
      { // trick; image::save_buffer are confused endian then swap there.
        let v = scaled_data[ source_address ];
        cropped_data[ destination_address ] = ( v << 8 ) + ( v >> 8 );
      }
    }
  }
  
  Ok( cropped_data )
}

fn save( cropped_data: &[ u16 ], out_path: &str )
  -> Result< (), Error >
{
  println!( "[ OUTPUT PNG ]" );
  println!( "  : {}; PNG/GRAY16", out_path );
  let view = &cropped_data[ 0 ] as *const _ as *const u8;
  let slice = unsafe { std::slice::from_raw_parts( view, cropped_data.len() * mem::size_of::< u16 >()) };
  let mut out: Vec< u8 > = vec![];
  out.reserve( slice.len() );
  out.write( slice ).unwrap();
  let _ = image::save_buffer( out_path, &out, cities_skylines::consts::PIXELS_PER_TILE_ARRIS as u32, cities_skylines::consts::PIXELS_PER_TILE_ARRIS as u32, image::Gray( 16 ) ).map_err( |e| Error::IOError( e ) )?;
  
  Ok( () )
}

use lib_error::Error;
use image::{ DynamicImage, GenericImage, Pixel };

pub fn decode_altitudes_from_png_nishioka_nagatsu_2015( source: DynamicImage, scaling: f64 )
  -> Result< Vec< f64 >, Error >
{
  const PIXELS_IN_ARRIS: u32 = 256;
  const PIXELS_IN_TILE: usize = PIXELS_IN_ARRIS as usize * PIXELS_IN_ARRIS as usize;
  const TARGET_COLOR_TYPE: image::ColorType = image::ColorType::RGB( 8 );
  
  let ( w, h ) = source.dimensions();
  
  if w != PIXELS_IN_ARRIS || h != PIXELS_IN_ARRIS
  { return Err( Error::ImageDecodeError ); }
  
  if source.color() != TARGET_COLOR_TYPE
  { return Err( Error::ImageDecodeError ); }
  
  let mut out = vec![ std::f64::NAN; PIXELS_IN_TILE ];
  
  for ( x, y, p ) in source.pixels()
  {
    let ( r, g, b, _ ) = p.channels4();
    
    if r == 128 && g == 0 && b == 0
    { continue; }
    
    let tmp = r as u32 * 256 * 256 + g as u32 * 256 + b as u32;
    let altitude_in_meter = scaling *
      if tmp < ( 1 << 23 )
      { tmp as f64 }
      else
      { ( tmp - ( 1 << 24 ) ) as f64 }
      ;
    let address = y as usize * PIXELS_IN_ARRIS as usize + x as usize;
    out[ address ] = altitude_in_meter;
  }
  
  Ok( out )
}
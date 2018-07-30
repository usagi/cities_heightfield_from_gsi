extern crate num;

pub mod consts
{
  pub const PIXELS_PER_TILE_ARRIS: usize = 1081;
  pub const PIXELS_PER_TILE: usize = PIXELS_PER_TILE_ARRIS * PIXELS_PER_TILE_ARRIS;
  pub const METERS_PER_TILE_ARRIS: f64 = 17_290.0;
  pub const METERS_PER_PIXEL: f64 = METERS_PER_TILE_ARRIS / PIXELS_PER_TILE_ARRIS as f64;
  pub const HEIGHT_MAX_IN_MATER: f64 = 1024.0;
  pub const HEIGHT_MAX_IN_VALUE: u16 = 65535;
}

use self::consts::{*};

pub fn height_from_meter_to_value( height_in_meter: f64 )
  -> u16
{
  const V_MIN: f64 = 0.0f64;
  const V_MAX: f64 = HEIGHT_MAX_IN_VALUE as f64;
  num::clamp( height_in_meter / HEIGHT_MAX_IN_MATER * V_MAX, V_MIN, V_MAX ) as u16
}
use lib_error::Error;

mod context
{
  use reqwest::Client;
  
  lazy_static!
  {
    pub static ref client: Client = { Client::new() };
  }
}

pub mod consts
{
  pub const PIXELS_PER_TILE_ARRIS: u16 = 256;
  pub const URL_PATTERN: &str = "https://cyberjapandata.gsi.go.jp/xyz/{id}/{z}/{x}/{y}.png";
  pub const URL_PATTERN_X: &str = "{x}";
  pub const URL_PATTERN_Y: &str = "{y}";
  pub const URL_PATTERN_Z: &str = "{z}";
  pub const URL_PATTERN_ID: &str = "{id}";
  pub const ID_DEM_PNG: &str = "dem_png";
  pub const ID_DEM5A_PNG: &str = "dem5a_png";
  pub const ID_DEM5B_PNG: &str = "dem5b_png";
  // cm = 1.0e-2 m
  pub const DEM_PNG_SCALING: f64 = 1.0e-2;
}

pub fn make_url( id: &str, x: u32, y: u32, z: u8 ) -> String
{
  consts::URL_PATTERN
    .replace( "{id}", &id )
    .replace( "{z}", &z.to_string() )
    .replace( "{x}", &x.to_string() )
    .replace( "{y}", &y.to_string() )
}

pub fn get_tile( id: &str, x: u32, y: u32, z: u8 )
  -> Result< Vec< u8 >, Error >
{
  let url = make_url( id, x, y, z );
  let mut r = context::client.get( &url ).send().map_err( |e| Error::ReqwestError( e ) )?;
  
  if ! r.status().is_success()
  { return Err( Error::HTTPStatusError( r.status() ) ); }
  
  let mut blob = vec ![];
  r.copy_to( &mut blob ).map_err( |_| Error::ConvertFromReqwestResponseError )?;
  
  Ok( blob )
}

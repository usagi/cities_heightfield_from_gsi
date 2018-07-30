pub mod consts
{
  pub const FLATTENING: f64 = 1.0 / 298.257_223_563;
  pub const MAJOR_RADIUS_IN_METER: f64 = 6_356_752.314_2;
  pub const EQUATOR_LENGTH_IN_METER: f64 = MAJOR_RADIUS_IN_METER * 2.0 * std::f64::consts::PI;
  pub const MINOR_RADIUS_IN_METER: f64 = -( FLATTENING - 1.0 ) * MAJOR_RADIUS_IN_METER;
  pub const LATITUDE_MAX_IN_METER: f64 = 9_968_431.031_9;
}
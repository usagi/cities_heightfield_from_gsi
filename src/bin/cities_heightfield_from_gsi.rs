extern crate cities_heightfield_from_gsi;
extern crate getopts;
use std::{ env, process };
use getopts::Options;

fn main()
{
  println!( "<< Cities Heightfield from GSI / CUI; version {} >>\n", env!("CARGO_PKG_VERSION") );
  
  let mut arguments = parse_args();
  
  if arguments.interactive
  { arguments = interactive_mode(); }
  
  match cities_heightfield_from_gsi::invoke_with_save_to_file
  ( arguments.longitude
  , arguments.latitude
  , &arguments.out
  , arguments.altitude_bias
  , arguments.altitude_of_no_data
  )
  { Ok( _ ) => println!("  : Succeeded" )
  , Err( e ) => println!("  : Failed; {:?}", e )
  }
}

#[derive(Debug)]
struct Arguments
{ longitude: f64
, latitude: f64
, out: String
, altitude_bias: f64
, altitude_of_no_data: f64
, interactive: bool
//, silent: bool
}

fn interactive_mode()
  -> Arguments
{
  println!( "[ Interactive Mode ]" );
  println!( "  : ... ( Press CTRL + C if you want to exit halfway ) ...", );
  println!( "  : Input the Longitude of the target in degrees ( e.g. 141.315714 )" );
  let longitude: f64 = read();
  println!( "  : Input the Latitude of the target in degrees ( e.g. 43.04724 )> " );
  let latitude: f64 = read();
  println!( "  : Input the path of the output PNG file ( e.g. out.png )> " );
  let out_path: String = read();
  println!( "  : Input the altitude bias in meters ( e.g. 40.0 )" );
  let altitude_bias: f64 = read();
  println!( "  : Input the altitude of no data in meters ( e.g. 0.0 )" );
  let altitude_of_no_data: f64 = read();
  
  Arguments
  { longitude: longitude
  , latitude: latitude
  , out: out_path
  , altitude_bias: altitude_bias
  , altitude_of_no_data: altitude_of_no_data
  , interactive: true
  //, silent: false
  }
}

fn read< T: std::str::FromStr >()
  -> T
{
  loop
  {
    let mut s = String::new();
    match std::io::stdin().read_line( &mut s )
    { Ok( _ ) => ()
    , Err( _ ) => continue
    }
    match s.trim().parse::< T >()
    { Ok( r ) => return r
    , Err( _ ) => ()
    };
    println!("( error; please retry input. Or press CTRL + C if you want to exit )", )
  }
}

fn parse_args()
  -> Arguments
{
  println!("[ Parse Arguments ]" );
  
  let arguments: Vec< String > = env::args().collect();
  
  let mut options = Options::new();
  
  options.optopt( "x", "longitude", "set longitude of a target in degrees", "141.315714" );
  options.optopt( "y", "latitude", "set latitude of a target in degrees", "43.04724" );
  options.optopt( "o", "out", "set output file path", "out.png" );
  options.optopt( "b", "altitude_bias", "set an altitude bias in meters", "40.0" );
  options.optopt( "n", "altitude_of_no_data", "set an altitude of no data in meter", "0.0" );
  options.optflag( "h", "help", "show this help" );
  options.optflag( "v", "version", "show the app version" );
  //options.optflag( "s", "silent", "make silent the app" );
  
  let matches = 
    options.parse( &arguments[ 1 .. ] )
      .unwrap_or_else( |f| { println!( "( error; invalid arguments. {}", f.to_string() ); process::exit( 2 ); } )
    ;
  
  if matches.opt_present( "h" )
  { show_usage( &options ); }
  else if matches.opt_present( "v" )
  { show_version(); }
  
  let has_lon = matches.opt_present( "longitude" );
  let has_lat = matches.opt_present( "latitude" );
  
  if ( has_lon && ! has_lat ) || ( ! has_lon && has_lat )
  {
    println!("  ( error; Required both parameters ( --longitude, --latitude ) for the Direct Invocation mode. )" );
    process::exit( 1 );
  }
  
  println!("  : Done." );
  
  let to_number = | key, default | matches.opt_str( key ).unwrap_or( "".to_string() ).parse().unwrap_or( default );
  
  Arguments
  { longitude: to_number( "longitude", std::f64::NAN )
  , latitude: to_number( "latitude", std::f64::NAN )
  , out: matches.opt_str( "out" ).unwrap_or( "out.png".to_string() )
  , altitude_bias: to_number( "altitude_bias", 40.0 )
  , altitude_of_no_data: to_number( "altitude_of_no_data", 0.0 )
  , interactive: ! has_lon && ! has_lat 
  //, silent: matches.opt_present( "s" )
  }
}

fn show_usage( opts: &Options )
{
  let brief = 
    format!
    ( r"
Cities Heightfield from GSI / CUI

Usage (1) Interactive mode      : cities_heightfield_from_gsi
Usage (2) Direct Invocation mode: cities_heightfield_from_gsi [options]"
    );
  
  println!( "{}", opts.usage( &brief ) );
  
  process::exit( 0 );
}

fn show_version()
{
  println!( "Cities Heightfield from GSI / CUI {}", env!( "CARGO_PKG_VERSION" ) );
  process::exit( 0 );
}
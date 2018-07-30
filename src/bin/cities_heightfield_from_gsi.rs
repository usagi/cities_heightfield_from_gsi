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
, interactive: bool
//, silent: bool
}

fn interactive_mode()
  -> Arguments
{
  println!( "[ Interactive Mode ]" );
  println!( "  : ... ( Press CTRL + C if you want to exit halfway ) ...", );
  println!( "  : Input the Longitude of a target in degrees ( e.g. 141.315714 )" );
  let longitude: f64 = read();
  println!( "  : Input the Longitude of a target in degrees ( e.g. 43.04724 )> " );
  let latitude: f64 = read();
  println!( "  : Input the path of the output PNG file ( e.g. out.png )> " );
  let out_path: String = read();
  
  Arguments
  { longitude: longitude
  , latitude: latitude
  , out: out_path
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
  
  options.optopt( "", "longitude", "set longitude of a target", "141.315714" );
  options.optopt( "", "latitude", "set latitude of a target", "43.04724" );
  options.optopt( "o", "out", "set output file path", "out.png" );
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
  
  Arguments
  { longitude: matches.opt_str( "longitude" ).unwrap_or( "".to_string() ).parse().unwrap_or( std::f64::NAN )
  , latitude: matches.opt_str( "latitude" ).unwrap_or( "".to_string() ).parse().unwrap_or( std::f64::NAN )
  , out: matches.opt_str( "out" ).unwrap_or( "out.png".to_string() )
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
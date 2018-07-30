use std::error;
use std::fmt;

mod consts
{
  pub const UNIMPLEMENTED: &str = "Unimplemented.";
  pub const CONVERT_FROM_REQWEST_RESPONSE_ERROR: &str = "Convert from reqwest::Response was failed.";
  pub const IMAGE_DECODE_ERROR: &str = "Image decoding was failed.";
}

#[derive(Debug)]
pub enum Error
{ Unimplemented
, ReqwestError( reqwest::Error )
, HTTPStatusError( reqwest::StatusCode )
, ConvertFromReqwestResponseError
, ImageError( image::ImageError )
, ImageDecodeError
, IOError( std::io::Error )
}

impl fmt::Display for Error
{
  fn fmt( &self, f: &mut fmt::Formatter ) -> fmt::Result
  {
    match *self
    { Error::Unimplemented => write!( f, "Error: {}", consts::UNIMPLEMENTED )
    , Error::ReqwestError( ref e ) => write!( f, "Error/ReqwestError: {}", e )
    , Error::HTTPStatusError( ref e ) => write!( f, "Error/HTTPStatusError: {}", e )
    , Error::ConvertFromReqwestResponseError => write!( f, "Error: {}", consts::CONVERT_FROM_REQWEST_RESPONSE_ERROR )
    , Error::ImageError( ref e ) => write!( f, "Error/ImageError: {}", e )
    , Error::ImageDecodeError => write!( f, "Error: {}", consts::IMAGE_DECODE_ERROR )
    , Error::IOError( ref e ) => write!( f, "Error/IOError: {}", e )
    }
  }
}

impl error::Error for Error
{
  fn description( &self ) -> &str
  {
    match *self
    { Error::Unimplemented => consts::UNIMPLEMENTED
    , Error::ReqwestError( ref e ) => e.description()
    , Error::HTTPStatusError( ref _e ) => "HTTP Status Code Error"
    , Error::ConvertFromReqwestResponseError => consts::CONVERT_FROM_REQWEST_RESPONSE_ERROR
    , Error::ImageError( ref e) => e.description()
    , Error::ImageDecodeError => consts::IMAGE_DECODE_ERROR
    , Error::IOError( ref e) => e.description()
    }
  }
  
  fn cause( &self ) -> Option< &error::Error >
  {
    match *self
    { Error::Unimplemented => None
    , Error::ReqwestError( ref e ) => Some( e )
    , Error::HTTPStatusError( ref _e ) => None
    , Error::ConvertFromReqwestResponseError => None
    , Error::ImageError( ref e ) => Some( e )
    , Error::ImageDecodeError => None
    , Error::IOError( ref e ) => Some( e )
    }
  }
}
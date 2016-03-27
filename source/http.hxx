#include <boost/asio.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <sstream>
#include <stdexcept>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    
    namespace
    {
      static inline auto _anti_warning()
      {
        (void)boost::asio::error::system_category;
        (void)boost::asio::error::netdb_category;
        (void)boost::asio::error::addrinfo_category;
        (void)boost::asio::error::misc_category;
      }
      
      boost::asio::io_service io_service;
    }
    
    struct http_error
      : std::runtime_error
    {
      const std::uint16_t status;
      
      explicit http_error( const std::uint16_t status_ )
        : std::runtime_error( "HTTP error ( " + std::to_string( status_ ) + " )" )
        , status( status_ )
      { }
    };

    auto get_http( const std::string& url )
      -> std::string
    {
      using namespace std;
      using namespace boost::asio;
      using namespace boost::xpressive;
      using namespace std::literals::string_literals;
      
      ip::tcp::socket socket( io_service );
      ip::tcp::resolver resolver( io_service );
      
      {
        mark_tag scheme(1), host(2), path(3);

        sregex r
          =  bos
          >> ( scheme = +~as_xpr( ':' ) )
          >> ':' >> ! as_xpr( "//" )
          >> ( host = +~as_xpr( '/' ) )
          >> ( path = *_ )
          >> eos;
        
        smatch m;
        
        if ( not regex_search( url, m, r ) )
          throw std::runtime_error( "invalid url pattern"s + url );
        
        if ( m[scheme] != "http" )
          throw std::runtime_error( "invalid url scheme: "s + m[scheme] );
          
        ip::tcp::resolver::query query( m[host], m[scheme] );
        
        ip::tcp::endpoint endpoint( *resolver.resolve( query ) );
        
        socket.connect( endpoint );
        
        {
          boost::asio::streambuf buffer;
          ostream s( &buffer );
          s << "GET " << m[path] << " HTTP/1.0" << constants::http::end_of_line
            << "Host: " << m[host] << constants::http::end_of_line
            << "Connection: Close" << constants::http::end_of_line
            << "User-Agent: Cities Heightfield from GSI version "
              << CITIES_HEIGHTFIELD_FROM_GSI_VERSION_MAJOR << "."
              << CITIES_HEIGHTFIELD_FROM_GSI_VERSION_MINOR << "."
              << CITIES_HEIGHTFIELD_FROM_GSI_VERSION_REVISION
              << constants::http::end_of_line
            << constants::http::end_of_line
            ;
          write( socket, buffer );
        }
      }
      
      const auto response = [ & ]
      {
        stringstream s;
      
        boost::asio::streambuf buffer;
        boost::system::error_code e;
        
        while ( read( socket, buffer, transfer_all(), e ) )
          s << string_type( buffer_cast< const char* >( buffer.data() ), buffer.size() );
        
        if ( e and e != error::eof )
          throw std::runtime_error( e.message() );
        
        return s.str();
      }();
      
      {
        mark_tag header(1), status(2);

        sregex r
          =  bos
          >> ( header = ( as_xpr( "HTTP/1.1 " ) >> ( status = repeat<3,3>( range( '0', '9' ) ) ) >> ( as_xpr( " " ) ) >> *_ ) )
          >> as_xpr( "\r\n\r\n" )
          >> *_
          ;
        
        smatch m;
        
        if ( regex_match( response, m, r ) )
        {
          if ( m[status] != "200" )
            throw http_error( static_cast< std::uint16_t >( std::stoul( m[status] ) ) );
          
          constexpr auto header_body_spliter_size = 4;
          const auto size_of_header = static_cast< std::string >( m[header] ).size();
          if ( size_of_header + header_body_spliter_size < response.size() )
          {
            const auto begin = size_of_header + header_body_spliter_size;
            const auto size  = response.size() - size_of_header - header_body_spliter_size;
            return response.substr( begin, size );
          }
        }
      }
      
      return { };
    }
    
  }
}
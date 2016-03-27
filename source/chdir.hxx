#pragma once

#ifndef BOOST_SYSTEM_NO_DEPRECATED
#define BOOST_SYSTEM_NO_DEPRECATED 1
#endif
#include <boost/filesystem.hpp>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    
    auto chdir( const std::string& executable_path )
    {
      // anti-warning
      //(void)boost::system::posix_category;
      //(void)boost::system::errno_ecat;
      //(void)boost::system::native_ecat;
      
      boost::filesystem::current_path
      ( boost::filesystem::system_complete
        ( boost::filesystem::path( executable_path )
        ).remove_filename().parent_path()
      );
    }
    
  }
}
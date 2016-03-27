#pragma once

#include <iostream>

namespace usagi
{
  namespace cities_heightfield_from_gsi
  {
    
    auto show_version()
    {
      std::cout
        << "Cities Heightfield from GSI\n"
        << "  Version : " << CITIES_HEIGHTFIELD_FROM_GSI_VERSION_MAJOR << "." << CITIES_HEIGHTFIELD_FROM_GSI_VERSION_MINOR << "." << CITIES_HEIGHTFIELD_FROM_GSI_VERSION_REVISION << "\n"
        << "  GitHub  : https://github.com/usagi/citis_heightfield_from_gsi\n"
        << "  License : MIT\n"
        << "  Author  : Usagi Ito<usagi@WonderRabbitProject.net>; Made in Sapporo, Hokkaido, Japan."
        << std::flush
        ;
    }
    
  }
}
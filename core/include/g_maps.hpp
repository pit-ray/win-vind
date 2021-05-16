#ifndef _G_MAPS_HPP
#define _G_MAPS_HPP

#include "mode.hpp"

namespace vind
{
    namespace gmaps {
        void initialize() ;
        void reset() ;

        void map(
                const std::string& incmd,
                const std::string& outcmd,
                mode::Mode mode) ;

        void unmap(
                const std::string& incmd,
                mode::Mode mode) ;

        void mapclear(mode::Mode mode) ;
    }
}

#endif

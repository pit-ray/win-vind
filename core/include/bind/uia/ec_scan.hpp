#ifndef _EC_SCAN_HPP
#define _EC_SCAN_HPP

#include <vector>

#include "point_2d.hpp"

namespace vind
{
    namespace easyclick {
        void initialize_global_uia() ;
        void scan_gui_objects(std::vector<Point2D>& obj_points) ;
    }
}

#endif

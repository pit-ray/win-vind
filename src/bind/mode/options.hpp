#ifndef _OPTIONS_HPP
#define _OPTIONS_HPP

#include "text_area_scanner.hpp"
#include "util/point_2d.hpp"


namespace vind
{
    namespace options {
        void focus_nearest_textarea(
                HWND hwnd,
                const Point2D& point,
                TextAreaScanner& instance) ;

    }
}

#endif

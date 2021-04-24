#ifndef _POINT_2D_HPP
#define _POINT_2D_HPP

#include <windows.h>

namespace vind
{
    namespace easyclick {
        class Point2D {
        private:
            long mx ;
            long my ;

        public:
            explicit Point2D(long x, long y)
            : mx(x), my(y)
            {}

            template <typename T>
            explicit Point2D(T x, T y)
            : Point2D(static_cast<LONG>(x), static_cast<LONG>(y))
            {}

            LONG x() const noexcept {
                return mx ;
            }

            LONG y() const noexcept {
                return my ;
            }

            POINT get() const noexcept {
                return POINT{mx, my} ;
            }

            operator POINT() const noexcept {
                return POINT{mx, my} ;
            }

            bool operator==(const Point2D& rhs) noexcept {
                return mx == rhs.mx && my == rhs.my ;
            }
            bool operator!=(const Point2D& rhs) noexcept {
                return mx != rhs.mx || my != rhs.my ;
            }
            bool operator>(const Point2D& rhs) noexcept {
                return my != rhs.my ? my > rhs.my : mx > rhs.mx ;
            }
            bool operator<(const Point2D& rhs) noexcept {
                return my != rhs.my ? my < rhs.my : mx < rhs.mx ;
            }
            bool operator>=(const Point2D& rhs) noexcept {
                return my != rhs.my ? my >= rhs.my : mx >= rhs.mx ;
            }
            bool operator<=(const Point2D& rhs) noexcept {
                return my != rhs.my ? my <= rhs.my : mx <= rhs.mx ;
            }
        } ;
    }
}

#endif

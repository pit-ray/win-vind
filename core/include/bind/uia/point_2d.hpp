#ifndef _POINT_2D_HPP
#define _POINT_2D_HPP

#include <windows.h>

#include <memory>

namespace vind
{
    class Point2D {
    private:
        LONG mx_ ;
        LONG my_ ;

    public:
        using SPtr = std::shared_ptr<Point2D> ;

        explicit Point2D(LONG x, LONG y)
        : mx_(x), my_(y)
        {}

        template <typename T>
        explicit Point2D(T x, T y)
        : Point2D(static_cast<LONG>(x), static_cast<LONG>(y))
        {}

        explicit Point2D(const POINT& pos)
        : Point2D(pos.x, pos.y)
        {}

        explicit Point2D(POINT&& pos)
        : Point2D(pos.x, pos.y)
        {}

        LONG x() const noexcept {
            return mx_ ;
        }

        LONG y() const noexcept {
            return my_ ;
        }

        POINT get() const noexcept {
            return POINT{mx_, my_} ;
        }

        operator POINT() const noexcept {
            return POINT{mx_, my_} ;
        }

        bool operator==(const Point2D& rhs) noexcept {
            return mx_ == rhs.mx_ && my_ == rhs.my_ ;
        }
        bool operator!=(const Point2D& rhs) noexcept {
            return mx_ != rhs.mx_ || my_ != rhs.my_ ;
        }
        bool operator>(const Point2D& rhs) noexcept {
            return my_ != rhs.my_ ? my_ > rhs.my_ : mx_ > rhs.mx_ ;
        }
        bool operator<(const Point2D& rhs) noexcept {
            return my_ != rhs.my_ ? my_ < rhs.my_ : mx_ < rhs.mx_ ;
        }
        bool operator>=(const Point2D& rhs) noexcept {
            return my_ != rhs.my_ ? my_ >= rhs.my_ : mx_ >= rhs.mx_ ;
        }
        bool operator<=(const Point2D& rhs) noexcept {
            return my_ != rhs.my_ ? my_ <= rhs.my_ : mx_ <= rhs.mx_ ;
        }
    } ;
}

#endif

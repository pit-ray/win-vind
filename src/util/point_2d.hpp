#ifndef _POINT_2D_HPP
#define _POINT_2D_HPP

#include <windows.h>

#include <utility>

namespace vind
{
    class Point2D {
    private:
        POINT pos_ ;

    public:
        explicit Point2D(LONG x=0, LONG y=0)
        : pos_{x, y}
        {}

        template <typename T>
        explicit Point2D(T x, T y)
        : Point2D(static_cast<LONG>(x), static_cast<LONG>(y))
        {}

        explicit Point2D(const POINT& pos)
        : pos_(pos)
        {}

        explicit Point2D(POINT&& pos)
        : pos_(std::move(pos))
        {}

        LONG x() const noexcept {
            return pos_.x ;
        }

        LONG y() const noexcept {
            return pos_.y ;
        }

        const POINT& data() const noexcept {
            return pos_ ;
        }
        POINT& data() noexcept {
            return pos_ ;
        }

        operator POINT() const noexcept {
            return pos_ ;
        }

        bool operator==(const Point2D& rhs) const noexcept {
            return pos_.x == rhs.pos_.x && pos_.y == rhs.pos_.y ;
        }
        bool operator!=(const Point2D& rhs) const noexcept {
            return pos_.x != rhs.pos_.x || pos_.y != rhs.pos_.y ;
        }

        bool operator>(const Point2D& rhs) const noexcept {
            return pos_.y != rhs.pos_.y ? pos_.y > rhs.pos_.y : pos_.x > rhs.pos_.x ;
        }
        bool operator<(const Point2D& rhs) const noexcept {
            return pos_.y != rhs.pos_.y ? pos_.y < rhs.pos_.y : pos_.x < rhs.pos_.x ;
        }

        bool operator>=(const Point2D& rhs) const noexcept {
            return pos_.y != rhs.pos_.y ? pos_.y >= rhs.pos_.y : pos_.x >= rhs.pos_.x ;
        }
        bool operator<=(const Point2D& rhs) const noexcept {
            return pos_.y != rhs.pos_.y ? pos_.y <= rhs.pos_.y : pos_.x <= rhs.pos_.x ;
        }
    } ;
}

#endif

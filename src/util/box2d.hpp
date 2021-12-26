#ifndef _box_2D_HPP
#define _box_2D_HPP

#include <windows.h>

#include <memory>

#include "util/point2d.hpp"

namespace vind
{
    namespace util
    {
        class Box2D {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit Box2D(
                    LONG left=0,
                    LONG top=0,
                    LONG right=0,
                    LONG bottom=0) ;

            template <typename T>
            explicit Box2D(T left, T top, T right, T bottom)
            : Box2D(
                static_cast<LONG>(left),
                static_cast<LONG>(top),
                static_cast<LONG>(right),
                static_cast<LONG>(bottom))
            {}

            explicit Box2D(
                    const Point2D& center,
                    LONG width,
                    LONG height) ;

            explicit Box2D(const RECT& rect) ;
            explicit Box2D(RECT&& rect) ;

            virtual ~Box2D() noexcept ;

            Box2D(const Box2D&) ;
            Box2D& operator=(const Box2D&) ;

            Box2D(Box2D&&) ;
            Box2D& operator=(Box2D&&) ;

            Box2D& operator=(const RECT&) ;

            LONG left() const noexcept ;
            LONG right() const noexcept ;
            LONG top() const noexcept ;
            LONG bottom() const noexcept ;

            LONG& left() noexcept ;
            LONG& right() noexcept ;
            LONG& top() noexcept ;
            LONG& bottom() noexcept ;

            Point2D center() const noexcept ;
            LONG center_x() const noexcept ;
            LONG center_y() const noexcept ;

            LONG width() const noexcept ;
            LONG height() const noexcept ;
            LONG area() const noexcept ;

            const RECT& data() const noexcept ;
            RECT& data() noexcept ;

            operator RECT() const noexcept ;

            // Location-based comparison
            bool operator==(const Box2D& rhs) const noexcept ;
            bool operator!=(const Box2D& rhs) const noexcept ;

            bool operator>(const Box2D& rhs) const noexcept ;
            bool operator<(const Box2D& rhs) const noexcept ;

            bool operator>=(const Box2D& rhs) const noexcept ;
            bool operator<=(const Box2D& rhs) const noexcept ;

            // Size-based comparison
            bool is_same(const Box2D& rhs) const noexcept ;
            bool is_not_same(const Box2D& rhs) const noexcept ;

            bool is_bigger_than(const Box2D& rhs) const noexcept ;
            bool is_bigger_equal(const Box2D& rhs) const noexcept ;

            bool is_smaller_than(const Box2D& rhs) const noexcept ;
            bool is_smaller_equal(const Box2D& rhs) const noexcept ;

            // Area-based comparison
            //
            // [True]
            // self: -
            // rhs : *
            //
            // *************    _____________
            // *           *   |            |
            // *           *   |____________|
            // *           *
            // *           *
            // *************
            bool is_out_of(const Box2D& rhs) const noexcept ;

            //
            // [True]
            // self: -
            // rhs : *
            //
            // *************
            // *  ______   *
            // * |     |   *
            // * |_____|   *
            // *           *
            // *************
            bool is_in(const Box2D& rhs) const noexcept ;

            //
            // [True]
            // self: -
            // rhs : *
            //
            // *************
            // *  _________*_______
            // * |         *      |
            // * |_________*______|
            // *           *
            // *************
            bool is_intersect(const Box2D& rhs) const noexcept ;

            //
            // [True]
            // self: -
            // rhs : *
            //
            //  ____________
            // |           |
            // | *******   |
            // | *     *   |
            // | *******   |
            // |___________|
            //
            bool is_over(const Box2D& rhs) const noexcept ;

            static LONG l1_distance(const Box2D& lhs, const Box2D& rhs) noexcept ;
            static double l2_distance(const Box2D& lhs, const Box2D& rhs) noexcept ;
            static LONG l2_distance_nosq(const Box2D& lhs, const Box2D& rhs) noexcept ;
        } ;
    }
}

#endif

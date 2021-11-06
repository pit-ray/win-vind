#include "util/box_2d.hpp"

#include "util/rect.hpp"


namespace vind
{
    struct Box2D::Impl {
        RECT rect_ ;

        Impl(LONG left=0, LONG top=0, LONG right=0, LONG bottom=0)
        : rect_{left, top, right, bottom}
        {}
    } ;

    Box2D::Box2D(LONG left, LONG top, LONG right, LONG bottom)
    : pimpl(std::make_unique<Impl>(left, top, right, bottom))
    {}

    Box2D::Box2D(const Point2D& center, LONG width, LONG height)
    : pimpl(std::make_unique<Impl>(
                center.x() - width / 2,
                center.y() - height / 2,
                center.x() + width / 2,
                center.y() + height / 2))
    {}

    Box2D::Box2D(const RECT& rect)
    : Box2D(rect.left, rect.top, rect.right, rect.bottom)
    {}
    Box2D::Box2D(RECT&& rect)
    : Box2D(rect.left, rect.top, rect.right, rect.bottom)
    {}

    Box2D::~Box2D() noexcept = default ;

    Box2D::Box2D(const Box2D& rhs)
    : pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
    {}
    Box2D& Box2D::operator=(const Box2D& rhs)
    {
        if(rhs.pimpl) {
            *pimpl = *(rhs.pimpl) ;
        }
        return *this ;
    }
    Box2D& Box2D::operator=(const RECT& rhs)
    {
        util::copy(pimpl->rect_, rhs) ;
        return *this ;
    }

    Box2D::Box2D(Box2D&&)            = default ;
    Box2D& Box2D::operator=(Box2D&&) = default ;

    LONG Box2D::left() const noexcept {
        return pimpl->rect_.left ;
    }
    LONG Box2D::top() const noexcept {
        return pimpl->rect_.top ;
    }
    LONG Box2D::right() const noexcept {
        return pimpl->rect_.right ;
    }
    LONG Box2D::bottom() const noexcept {
        return pimpl->rect_.bottom ;
    }

    LONG& Box2D::left() noexcept {
        return pimpl->rect_.left ;
    }
    LONG& Box2D::top() noexcept {
        return pimpl->rect_.top ;
    }
    LONG& Box2D::right() noexcept {
        return pimpl->rect_.right ;
    }
    LONG& Box2D::bottom() noexcept {
        return pimpl->rect_.bottom ;
    }

    Point2D Box2D::center() const noexcept {
        return Point2D(center_x(), center_y()) ;
    }

    LONG Box2D::center_x() const noexcept {
        return util::center_x(pimpl->rect_) ;
    }

    LONG Box2D::center_y() const noexcept {
        return util::center_y(pimpl->rect_) ;
    }

    LONG Box2D::width() const noexcept {
        return util::width(pimpl->rect_) ;
    }

    LONG Box2D::height() const noexcept {
        return util::height(pimpl->rect_) ;
    }

    LONG Box2D::area() const noexcept {
        return util::width(pimpl->rect_) * util::height(pimpl->rect_) ;
    }

    const RECT& Box2D::data() const noexcept {
        return pimpl->rect_ ;
    }
    RECT& Box2D::data() noexcept {
        return pimpl->rect_ ;
    }

    Box2D::operator RECT() const noexcept {
        return pimpl->rect_ ;
    }

    // Location-based comparison
    bool Box2D::operator==(const Box2D& rhs) const noexcept {
        return util::is_equal(pimpl->rect_, rhs.data()) ;
    }
    bool Box2D::operator!=(const Box2D& rhs) const noexcept {
        return !util::is_equal(pimpl->rect_, rhs.data()) ;
    }

    bool Box2D::operator>(const Box2D& rhs) const noexcept {
        if(pimpl->rect_.left == rhs.left()) {
            if(pimpl->rect_.top == rhs.top()) {
                if(pimpl->rect_.right == rhs.right()) {
                    return pimpl->rect_.bottom < rhs.bottom() ;
                }
                return pimpl->rect_.right > rhs.right() ;
            }
            return pimpl->rect_.top > rhs.top() ;
        }
        return pimpl->rect_.left > rhs.left() ;
    }
    bool Box2D::operator<(const Box2D& rhs) const noexcept {
        if(pimpl->rect_.left == rhs.left()) {
            if(pimpl->rect_.top == rhs.top()) {
                if(pimpl->rect_.right == rhs.right()) {
                    return pimpl->rect_.bottom < rhs.bottom() ;
                }
                return pimpl->rect_.right < rhs.right() ;
            }
            return pimpl->rect_.top < rhs.top() ;
        }
        return pimpl->rect_.left < rhs.left() ;
    }

    bool Box2D::operator>=(const Box2D& rhs) const noexcept {
        if(pimpl->rect_.left == rhs.left()) {
            if(pimpl->rect_.top == rhs.top()) {
                if(pimpl->rect_.right == rhs.right()) {
                    return pimpl->rect_.bottom >= rhs.bottom() ;
                }
                return pimpl->rect_.right > rhs.right() ;
            }
            return pimpl->rect_.top > rhs.top() ;
        }
        return pimpl->rect_.left > rhs.left() ;
    }
    bool Box2D::operator<=(const Box2D& rhs) const noexcept {
        if(pimpl->rect_.left == rhs.left()) {
            if(pimpl->rect_.top == rhs.top()) {
                if(pimpl->rect_.right == rhs.right()) {
                    return pimpl->rect_.bottom <= rhs.bottom() ;
                }
                return pimpl->rect_.right < rhs.right() ;
            }
            return pimpl->rect_.top < rhs.top() ;
        }
        return pimpl->rect_.left < rhs.left() ;
    }

    // Size-based comparison
    bool Box2D::is_same(const Box2D& rhs) const noexcept {
        return width() == rhs.width() && height() == rhs.height() ;
    }
    bool Box2D::is_not_same(const Box2D& rhs) const noexcept {
        return width() != rhs.width() || height() != rhs.height() ;
    }

    bool Box2D::is_bigger_than(const Box2D& rhs) const noexcept {
        return width() > rhs.width() || height() > rhs.height() ;
    }
    bool Box2D::is_bigger_equal(const Box2D& rhs) const noexcept {
        return width() >= rhs.width() || height() >= rhs.height() ;
    }

    bool Box2D::is_smaller_than(const Box2D& rhs) const noexcept {
        return width() < rhs.width() && height() < rhs.height() ;
    }
    bool Box2D::is_smaller_equal(const Box2D& rhs) const noexcept {
        return width() <= rhs.width() && height() <= rhs.height() ;
    }

    // Area-based comparison
    bool Box2D::is_out_of(const Box2D& rhs) const noexcept {
        return util::is_out_of_range(pimpl->rect_, rhs.data()) ;
    }
    bool Box2D::is_in(const Box2D& rhs) const noexcept {
        return util::is_fully_in_range(pimpl->rect_, rhs.data()) ;
    }
    bool Box2D::is_intersect(const Box2D& rhs) const noexcept {
        return util::is_intersect(pimpl->rect_, rhs.data()) ;
    }
    bool Box2D::is_over(const Box2D& rhs) const noexcept {
        return util::is_bigger_than(pimpl->rect_, rhs.data()) ;
    }

    // utilities
    LONG Box2D::l1_distance(const Box2D& lhs, const Box2D& rhs) noexcept {
        return util::l1_distance(lhs.data(), rhs.data()) ;
    }
    double Box2D::l2_distance(const Box2D& lhs, const Box2D& rhs) noexcept {
        return util::l2_distance(lhs.data(), rhs.data()) ;
    }
    LONG Box2D::l2_distance_nosq(const Box2D& lhs, const Box2D& rhs) noexcept {
        return util::l2_distance_nosq(lhs.data(), rhs.data()) ;
    }
}

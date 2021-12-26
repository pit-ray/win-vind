#include "textreg.hpp"

namespace
{
    auto g_rgtype = vind::bind::RegType::None ;
}

namespace vind
{
    namespace bind
    {
        RegType get_register_type() noexcept {
            return g_rgtype ;
        }

        void set_register_type(RegType t) noexcept {
            g_rgtype = t ;
        }
    }
}

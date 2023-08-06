#ifndef _SOURCE_HPP
#define _SOURCE_HPP

#include "bind/bindedfunc.hpp"
#include "core/path.hpp"

namespace vind
{
    namespace bind
    {
        class Source : public BindedFuncFlex<Source> {
        private:
            static bool loaded_default_ ;

        public:
            explicit Source() ;

            static SystemCall sprocess(
                std::uint16_t count,
                const std::string& args,
                bool as_default=false) ;
        } ;
    }
}

#endif

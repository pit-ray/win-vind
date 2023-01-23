#ifndef _EDI_LAYOUT_HPP
#define _EDI_LAYOUT_HPP

#include "changebase.hpp"


namespace vind
{
    namespace bind
    {
        struct JoinNextLine : public ChangeBaseCreator<JoinNextLine> {
            explicit JoinNextLine() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif

#ifndef _BIND_MODE_OPTIONS_HPP
#define _BIND_MODE_OPTIONS_HPP

#include "util/ui_walker.hpp"


namespace vind
{
    namespace bind
    {
        struct TextAreaScanner : public util::UIWalker {
            explicit TextAreaScanner() ;
            virtual bool pinpoint_element(const util::SmartElement& elem) override ;
            virtual bool filter_element(const util::SmartElement& elem) override ;
        } ;
    }
}

#endif

#ifndef _BIND_MODE_OPTIONS_HPP
#define _BIND_MODE_OPTIONS_HPP

#include "uia/ui_walker.hpp"


namespace vind
{
    struct TextAreaScanner : public UIWalker {
        explicit TextAreaScanner() ;
        virtual bool pinpoint_element(const SmartElement& elem) override ;
        virtual bool filter_element(const SmartElement& elem) override ;
    } ;
}

#endif

#ifndef _UI_SCANNER_HPP
#define _UI_SCANNER_HPP

#include <memory>
#include <vector>

#include "util/ui_walker.hpp"

namespace vind
{
    struct UIScanner : public UIWalker {
        explicit UIScanner() ;
        virtual bool filter_element(const SmartElement& elem) override ;
    } ;
}

#endif

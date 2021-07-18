#ifndef _UI_SCANNER_HPP
#define _UI_SCANNER_HPP

#include <memory>
#include <vector>

#include "uia/ui_walker.hpp"

namespace vind
{
    class UIScanner : public UIWalker {
    private:
        virtual bool filter_element(uiauto::SmartElement elem) override ;

    public:
        explicit UIScanner() ;
    } ;
}

#endif

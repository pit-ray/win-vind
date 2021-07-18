#ifndef _BIND_MODE_OPTIONS_HPP
#define _BIND_MODE_OPTIONS_HPP

#include "uia/ui_walker.hpp"


namespace vind
{
    class TextAreaScanner : public UIWalker {
    private:
        virtual bool pinpoint_element(uiauto::SmartElement elem) override ;
        virtual bool filter_element(uiauto::SmartElement elem) override ;

    public:
        explicit TextAreaScanner() ;
    } ;
}

#endif

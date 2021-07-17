#ifndef _UI_SCANNER_HPP
#define _UI_SCANNER_HPP

#include <memory>
#include <vector>

#include "uia/ui_walker.hpp"

namespace vind
{
    class UIScanner : public UIWalker {
    private:
        virtual bool is_target(uiauto::SmartElement& elem) override ;

    public:
        explicit UIScanner() ;
        void scan(std::vector<Box2D>& obj_list, HWND hwnd) override ;
    } ;
}

#endif

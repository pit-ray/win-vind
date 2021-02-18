#ifndef _CHECKBOX_WL_HPP
#define _CHECKBOX_WL_HPP

#include "disable_gcc_warning.hpp"
#include <wx/sizer.h>
#include <wx/window.h>
#include "enable_gcc_warning.hpp"

#include <memory>
#include <string>

namespace wxGUI
{
    class CheckBoxWithLabel {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit CheckBoxWithLabel(wxWindow* const parent) ;
        virtual ~CheckBoxWithLabel() noexcept ;

        void set_sizer(wxSizer* sizer, const wxSizerFlags& flags) ;

        void create(const std::string name) ;

        void translate() ;
        void load_config() ;
        void load_config_default() ;
        void save_config() ;

        CheckBoxWithLabel(CheckBoxWithLabel&&)            = delete ;
        CheckBoxWithLabel& operator=(CheckBoxWithLabel&&) = delete ;
        CheckBoxWithLabel(const CheckBoxWithLabel&)       = delete ;
        CheckBoxWithLabel& operator=(CheckBoxWithLabel&)  = delete ;
    } ;
}

#endif

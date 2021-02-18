#ifndef _CLRPICKER_WL_HPP
#define _CLRPICKER_WL_HPP

#include "disable_gcc_warning.hpp"
#include <wx/sizer.h>
#include <wx/window.h>
#include "enable_gcc_warning.hpp"

#include <memory>
#include <string>

namespace wxGUI
{
    class ColourPickerWithLabel {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit ColourPickerWithLabel(wxWindow* const parent) ;
        virtual ~ColourPickerWithLabel() noexcept ;

        void set_sizer(wxGridSizer* sizer, const wxSizerFlags& flags) ;

        void create(const std::string name) ;

        void translate() ;
        void load_config() ;
        void load_config_default() ;
        void save_config() ;

        ColourPickerWithLabel(ColourPickerWithLabel&&)            = delete ;
        ColourPickerWithLabel& operator=(ColourPickerWithLabel&&) = delete ;
        ColourPickerWithLabel(const ColourPickerWithLabel&)       = delete ;
        ColourPickerWithLabel& operator=(ColourPickerWithLabel&)  = delete ;
    } ;
}

#endif

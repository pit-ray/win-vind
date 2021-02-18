#ifndef _SLIDER_WL_HPP
#define _SLIDER_WL_HPP

#include "disable_gcc_warning.hpp"
#include <wx/sizer.h>
#include <wx/window.h>
#include "enable_gcc_warning.hpp"

#include <memory>
#include <string>

namespace wxGUI
{
    class SliderWithLabel {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit SliderWithLabel(wxWindow* const parent) ;
        virtual ~SliderWithLabel() noexcept ;

        void set_sizer(wxGridSizer* sizer, const wxSizerFlags& flags) ;

        void create(
                const std::string name,
                const int min,
                const int max,
                const int init=0) ;

        void translate() ;
        void load_config() ;
        void load_config_default() ;
        void save_config() ;

        SliderWithLabel(SliderWithLabel&&)            = delete ;
        SliderWithLabel& operator=(SliderWithLabel&&) = delete ;
        SliderWithLabel(const SliderWithLabel&)       = delete ;
        SliderWithLabel& operator=(SliderWithLabel&)  = delete ;
    } ;
}

#endif

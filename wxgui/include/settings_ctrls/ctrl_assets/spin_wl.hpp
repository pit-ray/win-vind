#ifndef _SPIN_WL_HPP
#define _SPIN_WL_HPP

#include "disable_gcc_warning.hpp"

#include <wx/sizer.h>
#include <wx/window.h>

#include "enable_gcc_warning.hpp"

#include <memory>
#include <string>

namespace wxGUI
{
    class SpinWithLabel {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit SpinWithLabel(wxWindow* const parent) ;
        virtual ~SpinWithLabel() noexcept ;

        void set_sizer(wxGridSizer* sizer, const wxSizerFlags& flags) ;
        void create(
                const std::string name,
                const int min,
                const int max,
                const int init=0) ;
        void create(
                const std::string name,
                const double min,
                const double max,
                const double init=0.0,
                const double step=0.1) ;
        void translate() ;
        void load_config() ;
        void load_config_default() ;
        void save_config() ;

        SpinWithLabel(SpinWithLabel&&)            = delete ;
        SpinWithLabel& operator=(SpinWithLabel&&) = delete ;
        SpinWithLabel(const SpinWithLabel&)       = delete ;
        SpinWithLabel& operator=(SpinWithLabel&)  = delete ;
    } ;
}

#endif

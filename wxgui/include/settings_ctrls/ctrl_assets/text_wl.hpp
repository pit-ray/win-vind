#ifndef _TEXT_WL_HPP
#define _TEXT_WL_HPP

#include "disable_gcc_warning.hpp"

#include <wx/sizer.h>
#include <wx/window.h>

#include "enable_gcc_warning.hpp"

#include <memory>
#include <string>

namespace wxGUI
{
    class TextWithLabel {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit TextWithLabel(wxWindow* const parent) ;
        virtual ~TextWithLabel() noexcept ;

        void set_sizer(wxGridSizer* sizer, const wxSizerFlags& flags) ;

        void create(const std::string name) ;

        void translate() ;
        void load_config() ;
        void load_config_default() ;
        void save_config() ;

        TextWithLabel(TextWithLabel&&)            = delete ;
        TextWithLabel& operator=(TextWithLabel&&) = delete ;
        TextWithLabel(const TextWithLabel&)       = delete ;
        TextWithLabel& operator=(TextWithLabel&)  = delete ;
    } ;
}

#endif

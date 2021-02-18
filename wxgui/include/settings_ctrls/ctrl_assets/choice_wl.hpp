#ifndef _CHOICE_WL_HPP
#define _CHOICE_WL_HPP

#include "disable_gcc_warning.hpp"
#include <wx/sizer.h>
#include <wx/window.h>
#include "enable_gcc_warning.hpp"

#include <memory>
#include <string>

namespace wxGUI
{
    class ChoiceWithLabel {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit ChoiceWithLabel(wxWindow* const parent) ;
        virtual ~ChoiceWithLabel() noexcept ;

        void set_sizer(wxGridSizer* sizer, const wxSizerFlags& flags) ;

        void create(const std::string name) ;

        void translate() ;
        void load_config() ;
        void load_config_default() ;
        void save_config() ;

        ChoiceWithLabel(ChoiceWithLabel&&)            = delete ;
        ChoiceWithLabel& operator=(ChoiceWithLabel&&) = delete ;
        ChoiceWithLabel(const ChoiceWithLabel&)       = delete ;
        ChoiceWithLabel& operator=(ChoiceWithLabel&)  = delete ;
    } ;
}

#endif

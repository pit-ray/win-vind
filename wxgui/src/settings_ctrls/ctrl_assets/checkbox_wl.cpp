#include "checkbox_wl.hpp"

#include <unordered_map>

#include "disable_gcc_warning.hpp"

#include <wx/checkbox.h>

#include "enable_gcc_warning.hpp"

#include "io_params.hpp"
#include "util/def.hpp"

namespace wxGUI
{
    struct CheckBoxWithLabel::Impl
    {
        std::unordered_map<std::string, wxCheckBox*> data ;

        wxWindow* const parent ;

        wxSizer* sizer ;
        wxSizerFlags flags ;

        Impl(wxWindow* const parent_ptr)
        : data(),
          parent(parent_ptr),
          sizer(nullptr),
          flags()
        {}

        ~Impl() noexcept             = default ;
        Impl(Impl&&)                 = delete ;
        Impl& operator=(Impl&&)      = delete ;
        Impl(const Impl&)            = delete ;
        Impl& operator=(const Impl&) = delete ;
    } ;

    CheckBoxWithLabel::CheckBoxWithLabel(wxWindow* const parent)
    : pimpl(std::make_unique<Impl>(parent))
    {}

    CheckBoxWithLabel::~CheckBoxWithLabel() noexcept = default ;

    void CheckBoxWithLabel::set_sizer(wxSizer* sizer, const wxSizerFlags& flags) {
        pimpl->sizer = sizer ;
        pimpl->flags = flags ;
    }

    void CheckBoxWithLabel::create(const std::string name) {
        if(pimpl->sizer == nullptr) {
            throw LOGIC_EXCEPT("wxSizer is not registered.") ;
        }

        pimpl->data[name] = new wxCheckBox(pimpl->parent, wxID_ANY, name) ;
        pimpl->sizer->Add(pimpl->data[name], pimpl->flags) ;
    }

    void CheckBoxWithLabel::translate() {
        for(auto& c : pimpl->data) {
            c.second->SetLabel(ioParams::get_label(c.first)) ;
        }
    }

    void CheckBoxWithLabel::load_config() {
        for(auto& c : pimpl->data) {
            c.second->SetValue(ioParams::get_vb(c.first)) ;
        }
    }

    void CheckBoxWithLabel::load_config_default() {
        for(auto& c : pimpl->data) {
            c.second->SetValue(ioParams::Default::get_vb(c.first)) ;
        }
    }

    void CheckBoxWithLabel::save_config() {
        for(const auto& c : pimpl->data) {
            ioParams::set(c.first, c.second->GetValue()) ;
        }
    }
}

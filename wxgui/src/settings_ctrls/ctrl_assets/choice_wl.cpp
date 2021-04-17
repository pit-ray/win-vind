#include "choice_wl.hpp"

#include "disable_gcc_warning.hpp"

#include <wx/stattext.h>

#include "enable_gcc_warning.hpp"

#include "choices_manager.hpp"
#include "io_params.hpp"
#include "util/def.hpp"

namespace wxGUI
{
    struct ChoiceWithLabel::Impl
    {
        ChoicesManager data ;
        std::unordered_map<std::string, wxStaticText*> label ;

        wxWindow* const parent ;

        wxSizer* sizer ;
        wxSizerFlags flags ;

        Impl(wxWindow* const parent_ptr)
        : data(parent_ptr),
          label(),
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

    ChoiceWithLabel::ChoiceWithLabel(wxWindow* const parent)
    : pimpl(std::make_unique<Impl>(parent))
    {}

    ChoiceWithLabel::~ChoiceWithLabel() noexcept = default ;

    void ChoiceWithLabel::set_sizer(wxGridSizer* sizer, const wxSizerFlags& flags) {
        pimpl->sizer = sizer ;
        pimpl->flags = flags ;
    }

    void ChoiceWithLabel::create(const std::string name) {
        if(pimpl->sizer == nullptr) {
            throw LOGIC_EXCEPT("wxSizer is not registered.") ;
        }

        pimpl->label[name] = new wxStaticText(pimpl->parent, wxID_ANY, name) ;
        pimpl->sizer->Add(pimpl->label[name], pimpl->flags) ;

        pimpl->sizer->Add(pimpl->data.create(name), pimpl->flags) ;
    }

    void ChoiceWithLabel::translate() {
        for(auto& l : pimpl->label) {
            l.second->SetLabel(ioParams::get_label(l.first)) ;
        }
        pimpl->data.update_config() ;
    }

    void ChoiceWithLabel::load_config() {
        pimpl->data.load_config() ;
    }

    void ChoiceWithLabel::load_config_default() {
        pimpl->data.load_config_default() ;
    }

    void ChoiceWithLabel::save_config() {
        pimpl->data.save_config() ;
    }
}

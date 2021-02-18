#include "filepicker_wl.hpp"

#include "disable_gcc_warning.hpp"
#include <wx/filepicker.h>
#include <wx/stattext.h>
#include "enable_gcc_warning.hpp"

#include <unordered_map>

#include "wx_constant.hpp"
#include "io_params.hpp"
#include "utility.hpp"

namespace wxGUI
{
    struct FilePickerWithLabel::Impl
    {
        std::unordered_map<std::string, wxFilePickerCtrl*> data ;
        std::unordered_map<std::string, wxStaticText*> label ;

        wxWindow* const parent ;

        wxSizer* sizer ;
        wxSizerFlags flags ;

        Impl(wxWindow* const parent_ptr)
        : data(),
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

    FilePickerWithLabel::FilePickerWithLabel(wxWindow* const parent)
    : pimpl(std::make_unique<Impl>(parent))
    {}

    FilePickerWithLabel::~FilePickerWithLabel() noexcept = default ;

    void FilePickerWithLabel::set_sizer(wxGridSizer* sizer, const wxSizerFlags& flags) {
        pimpl->sizer = sizer ;
        pimpl->flags = flags ;
    }

    void FilePickerWithLabel::create(const std::string name) {
        if(pimpl->sizer == nullptr) {
            throw LOGIC_EXCEPT("wxSizer is not registered.") ;
        }

        pimpl->label[name] = new wxStaticText(pimpl->parent, wxID_ANY, wxT("UNDEFINED")) ;
        pimpl->sizer->Add(pimpl->label[name], pimpl->flags) ;

        pimpl->data[name] = new wxFilePickerCtrl(pimpl->parent, wxID_ANY, wxT("UNDEFINED"),
                wxFileSelectorPromptStr, wxFileSelectorDefaultWildcardStr, wxDefaultPosition,
                wxSize(CTRL_WIDTH * 2, -1),
                wxFLP_SMALL | wxFLP_USE_TEXTCTRL | wxFLP_FILE_MUST_EXIST) ;
        pimpl->data[name]->SetBackgroundColour(wxColour(*wxWHITE)) ;
        pimpl->sizer->Add(pimpl->data[name], pimpl->flags) ;
    }

    void FilePickerWithLabel::translate() {
        for(auto& l : pimpl->label) {
            l.second->SetLabel(ioParams::get_label(l.first)) ;
        }
    }

    void FilePickerWithLabel::load_config() {
        for(auto& s : pimpl->data) {
            s.second->SetPath(ioParams::get_vwxs(s.first)) ;
        }
    }

    void FilePickerWithLabel::load_config_default() {
        for(auto& s : pimpl->data) {
            s.second->SetPath(ioParams::Default::get_vwxs(s.first)) ;
        }
    }

    void FilePickerWithLabel::save_config() {
        for(const auto& s : pimpl->data) {
            ioParams::set(s.first, s.second->GetPath()) ;
        }
    }
}

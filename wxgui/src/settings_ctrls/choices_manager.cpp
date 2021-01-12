#include "choices_manager.hpp"

#include <unordered_map>
#include <vector>

#include "ctrl_core.hpp"
#include "io_params.hpp"

namespace wxGUI
{
    struct ChoicesManager::Impl {
        std::unordered_map<std::string, wxChoice*> chs ;
        std::unordered_map<std::string, std::vector<std::string>> chvals ;
        wxWindow* const parent ;

        explicit Impl(wxWindow* const parent_ptr)
        : chs(),
          chvals(),
          parent(parent_ptr)
        {}

        template <typename T>
        void load_config_core(const T& get_vs_func) noexcept {
            for(auto& c : chs) {
                const auto& name = c.first ;
                const auto& ptr = c.second ;
                ptr->SetSelection(0) ;

                try {
                    const auto v = get_vs_func(name) ;
                    const auto& cvt = chvals.at(name) ;
                    for(size_t i = 0 ; i < cvt.size() ; i ++) {
                        if(cvt[i] != v) continue ;
                        ptr->SetSelection(static_cast<int>(i)) ;
                    }
                }
                catch(const std::exception&) {continue ;}
            }
        }

        ~Impl() noexcept = default ;
        Impl(Impl&&)                 = delete ;
        Impl& operator=(Impl&&)      = delete ;
        Impl(const Impl&)            = delete ;
        Impl& operator=(const Impl&) = delete ;
    } ;

    ChoicesManager::ChoicesManager(wxWindow* const parent_ptr)
    : pimpl(std::make_unique<Impl>(parent_ptr))
    {}
    ChoicesManager::~ChoicesManager() noexcept = default ;

    wxChoice* ChoicesManager::create(const std::string name) {
        pimpl->chs[name] = new wxChoice(
            pimpl->parent, wxID_ANY, wxDefaultPosition,
            wxSize(CTRL_WIDTH, wxDefaultCoord),
            wxArrayString{}
        ) ;

        return pimpl->chs[name] ;
    }

    void ChoicesManager::load_config() noexcept {
        pimpl->load_config_core(ioParams::get_vs) ;
    }

    void ChoicesManager::load_config_default() noexcept {
        pimpl->load_config_core(ioParams::Default::get_vs) ;
    }

    void ChoicesManager::save_config() noexcept {
        for(const auto& c : pimpl->chs) {
            const auto& obj_name = c.first ;
            const auto index = c.second->GetSelection() ;
            if(index != wxNOT_FOUND) {
                try {ioParams::set(obj_name, pimpl->chvals.at(obj_name).at(index)) ;}
                catch(const std::exception&) {continue ;}
            }
        }
    }

    void ChoicesManager::update_config() noexcept {
        for(auto& c : pimpl->chs) {
            const auto& ptr = c.second ;
            const auto& name = c.first ;

            const auto items = ioParams::get_choices(name) ;
            const auto lang_key = ioParams::get_vs("ui_lang") ;

            ptr->Clear() ;
            pimpl->chvals[name].clear() ;
            for(const auto& i : items) {
                try {
                    ptr->Append(wxString::FromUTF8(i.at(lang_key).c_str())) ;
                    pimpl->chvals[name].push_back(i.at("value")) ;
                }
                catch(const std::exception&) {continue ;}
            }
        }
    }
}

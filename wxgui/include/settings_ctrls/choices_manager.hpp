#ifndef _CHOICES_MANAGER_HPP
#define _CHOICES_MANAGER_HPP
#include "wxvcdef.hpp"

#include <memory>

#include "disable_gcc_warning.hpp"
#include <wx/choice.h>
#include "enable_gcc_warning.hpp"

namespace wxGUI
{
    class ChoicesManager {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;


    public:
        explicit ChoicesManager(wxWindow* const parent_ptr) ;
        virtual ~ChoicesManager() noexcept ;
        ChoicesManager(ChoicesManager&&) = delete ;
        ChoicesManager& operator=(ChoicesManager&&) = delete ;
        ChoicesManager(const ChoicesManager&) = delete ;
        ChoicesManager& operator=(const ChoicesManager&) = delete ;

        wxChoice* create(const std::string name) ;

        void load_config() noexcept ;
        void load_config_default() noexcept ;
        void save_config() noexcept ;

        void update_config() noexcept ;
    } ;
}


#endif

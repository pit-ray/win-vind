#include "g_maps.hpp"

#include "mapdefs.hpp"

#include <unordered_map>

namespace
{
    std::unordered_map<vind::Command, std::string> g_maps{} ;
}


namespace vind
{
    namespace gmaps {
        void set_hotkey(const std::string& incmd, const std::string& outcmd) {
        }

        void set_hotkey(const vind::Command& parsed_incmd, const std::string& outcmd) {
        }

        void set_hotkey(vind::Command&& parsed_incmd, const std::string& outcmd) {
        }


        void set_binding(const std::string& incmd, const std::string& funcname) {
        }

        void set_binding(const vind::Command& parsed_incmd, const std::string& funcname) {
        }

        void set_binding(vind::Command&& parsed_incmd, const std::string& funcname) {
        }
    }
}

#include "g_maps.hpp"

#include "bind/bindings_lists.hpp"
#include "mapdefs.hpp"

#include <unordered_map>

namespace
{
    using namespace vind ;

    const auto& func_list = bindingslists::get() ;
    std::vector<std::pair<vind::Command, std::string>> g_maps{} ;
}


namespace vind
{
    namespace gmaps {

        void reset() {
            g_maps.clear() ;
        }

        /*
         * とにかく上書きする
         */
        void map_hotkey(const std::string& incmd, const std::string& outcmd) {
        }

        void map_hotkey(const vind::Command& parsed_incmd, const std::string& outcmd) {
        }

        void map_hotkey(vind::Command&& parsed_incmd, const std::string& outcmd) {
        }


        /* 
         * 既に値がある場合には、ファンクション文字列ならば上書き。そうでなければスキップ
         * これにより、全く同じバインディングの重複を回避する
         */
        void map_binding(const std::string& incmd, const std::string& funcname) {
        }

        void map_binding(const vind::Command& parsed_incmd, const std::string& funcname) {
        }

        void map_binding(vind::Command&& parsed_incmd, const std::string& funcname) {
        }
    }
}

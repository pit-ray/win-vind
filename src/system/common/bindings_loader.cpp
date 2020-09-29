#include "bindings_loader.hpp"

#include <algorithm>
#include <iostream>

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "msg_logger.hpp"
#include "path.hpp"
#include "utility.hpp"
#include "virtual_key_fwd.hpp"
#include "vkc_converter.hpp"

#define CONTINUE_EXCEPTION catch(const std::exception& e) {\
    ERROR_PRINT(std::string(e.what()) + ", so continued.") ;\
}

#define TERMINATE_EXCEPTION catch(const std::exception& e) {\
    ERROR_PRINT(std::string(e.what()) + ", so terminated.") ;\
}

namespace BindingsLoader
{
    keymap_t bindings{} ;
    keymap_t commands{} ;
    std::unordered_set<unsigned char> unbinded_syskeys{} ;

    void load_config() noexcept { try {
        bindings.clear() ;
        commands.clear() ;
        unbinded_syskeys = VKCConverter::get_all_sys_vkc() ;

        std::ifstream ifs{Path::BINDINGS()} ;
        nlohmann::json jp{} ;
        ifs >> jp ;

        for(const auto& obj : jp) { try {
            const auto& name = obj.at("name") ;
            const auto& keys = obj.at("key") ;
            const auto& cmds = obj.at("cmd") ;

            if(!keys.empty()) {
                vvvc_t or_seqs{} ;
                for(auto& key : keys) { try {
                    const auto seq = Utility::split(key, "->") ;

                    vvc_t seq_syncs{} ;
                    for(const auto& sync_str : seq) { try {
                        const auto sync = Utility::split(sync_str, "(+)") ;

                        vc_t sync_vkcs{} ;
                        for(const auto& keycode : sync) { try {

                            if(keycode.size() == 1) { //ascii code
                                //ex) a
                                if(auto vkc = VKCConverter::get_vkc(keycode[0])) {
                                    sync_vkcs.push_back(vkc) ;
                                    continue ;
                                }
                                //ex) A (A is divided to a and SHIFT)
                                if(auto vkc = VKCConverter::get_shifted_vkc(keycode[0])) {
                                    sync_vkcs.push_back(vkc) ;
                                    sync_vkcs.push_back(VKC_SHIFT) ;
                                    continue ;
                                }
                            }
                            //ex) Alt, Ctrl, ESC ...
                            const auto vkc = VKCConverter::get_sys_vkc(keycode) ;
                            sync_vkcs.push_back(vkc) ;
                            if(sync.size() == 1) {
                                const auto pos = std::find(unbinded_syskeys.cbegin(), unbinded_syskeys.cend(), vkc) ;
                                if(pos != unbinded_syskeys.cend()) {
                                    unbinded_syskeys.erase(pos) ;
                                }
                            }
                        } CONTINUE_EXCEPTION }
                        Utility::remove_deplication(sync_vkcs) ;
                        seq_syncs.push_back(std::move(sync_vkcs)) ;
                    } CONTINUE_EXCEPTION }
                    or_seqs.push_back(std::move(seq_syncs)) ;
                } CONTINUE_EXCEPTION }
                bindings[name] = std::move(or_seqs) ;
            }

            if(!cmds.empty()) {
                vvvc_t or_seqs{} ;
                for(const std::string& cmdstr : cmds) { try {
                    vvc_t seq_syncs{} ;

                    //replace ascii to virtual keycode
                    for(const auto c : cmdstr) { try {
                        if(c == '*') {
                            seq_syncs.emplace_back(1, VKC_OPTIONAL) ;
                            continue ;
                        }
                        if(auto vkc = VKCConverter::get_vkc(c)) {
                            seq_syncs.emplace_back(1, vkc) ; //directly construct with std::vector{vkc}
                            continue ;
                        }
                        if(auto vkc = VKCConverter::get_shifted_vkc(c)) {
                            auto init_list = {VKC_SHIFT, vkc} ;
                            seq_syncs.emplace_back(std::move(init_list)) ;
                            continue ;
                        }
                    } CONTINUE_EXCEPTION }
                    or_seqs.push_back(std::move(seq_syncs)) ;
                } CONTINUE_EXCEPTION }
                commands[name] = std::move(or_seqs) ;
            }
        } CONTINUE_EXCEPTION }
    } TERMINATE_EXCEPTION }

    const keymap_t& get_bindings() noexcept
    {
        return bindings ;
    }

    const keymap_t& get_commands() noexcept
    {
        return commands ;
    }

    const std::unordered_set<unsigned char>& get_unbinded_syskeys() noexcept
    {
        return unbinded_syskeys ;
    }
}

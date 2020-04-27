#include "disable_gcc_warning.hpp"
#include <boost/property_tree/xml_parser.hpp>
#include "enable_gcc_warning.hpp"

#include "utility.hpp"
#include "xml_parser.hpp"
#include "msg_logger.hpp"
#include "vkc_converter.hpp"
#include "virtual_key_fwd.hpp"
#include <iostream>
#include <type_traits>

using namespace XMLParser ;
using namespace std ;
using namespace boost::property_tree ;

namespace XMLParser {
    template <typename T>
    inline static void catch_except(T& e) {
        Logger::error_stream << "[Error] " << e.what() << " (xml_parser.cpp)\n" ;
    }

    template <typename T>
    inline static const auto reconstruct_combination(const T& elem_2d) {
        //num of pattern at synchorous system-key
        size_t n_combination = 1 ;
        for(const auto& elem : elem_2d) {
            if(elem.empty()) continue ;
            n_combination *= elem.size() ;
        }

        //use tree-architecture
        T full_comb{n_combination} ; //size is n_combination
        auto step = n_combination ;
        for(auto elem : elem_2d) {
            if(elem.empty()) {
                for(auto& pattern : full_comb) {
                    pattern.emplace_back() ;
                }
                continue ;
            }

            step /= elem.size() ;

            size_t elem_index = 0 ;
            for(size_t pt_base_index = 0 ; pt_base_index < full_comb.size() ; pt_base_index += step) {
                if(elem_index == elem.size()) {
                    //cycle iterating
                    elem_index = 0 ;
                }

                for(size_t i = 0 ; i < step ; i ++) {
                    full_comb[pt_base_index + i].push_back(elem.at(elem_index)) ;
                }

                elem_index ++ ;
            }
        }

        return std::move(full_comb) ;
    }

    auto not_alone_syskeys = VKCConverter::get_all_sys_vkc() ;
    const std::vector<unsigned char> get_ignore_alone() noexcept {
        return std::move(not_alone_syskeys) ;
    }

    const bind_map_t load_bind_map(const string& filename) noexcept {
        bind_map_t map{} ;

        try {
            ptree pt ;
            read_xml(filename, pt) ;

            for(const auto& pt_func : pt.get_child("config.binds")) {
                if(const auto& name = pt_func.second.get_optional<string>("<xmlattr>.name")) {
                    vvvc_t vvvc{} ;
                    auto any_type = Utility::split(
                        static_cast<string>(pt_func.second.data()), "|") ;

                    for(const auto& sequential : any_type) {
                        //or
                        const auto& seq_vec = Utility::split(sequential, "->") ;

                        vvc_t vvc{} ;
                        vvvc_t sys_key_all_patterns{} ;
                        vvc_t ascii_keys{} ;

                        for(const auto& same_type : seq_vec) {
                            //forward
                            auto same_type_vec = Utility::split(same_type, "(+)") ;

                            //encode ascii code to virtual key code
                            vc_t vkcs{} ;
                            vvc_t sys_vkcs_vec{} ;

                            for(const auto& scode : same_type_vec) {
                                //synchronous
                                using namespace VKCConverter ;

                                if(scode.size() == 1) {
                                    //normal key code
                                    if(const auto vkc = get_vkc(scode[0])) {
                                        vkcs.push_back(vkc) ;
                                        continue ;
                                    }

                                    //sifted key code
                                    if(const auto vkc = get_shifted_vkc(scode[0])) {
                                        //if exist, append system-key-list
                                        vkcs.push_back(vkc) ;

                                        sys_vkcs_vec.push_back(vc_t{VKC_LSHIFT, VKC_RSHIFT}) ;
                                        continue ;
                                    }
                                }

                                //System key code
                                const auto sys_vkcs = get_sys_vkc(scode) ;
                                sys_vkcs_vec.push_back(sys_vkcs) ;

                                //alone system-key
                                if(same_type_vec.size() == 1) {
                                    for(const auto& vkc : sys_vkcs) {
                                        const auto pos = find(not_alone_syskeys.cbegin(), not_alone_syskeys.cend(), vkc) ;
                                        if(pos == not_alone_syskeys.cend()) {
                                            continue ;
                                        }
                                        not_alone_syskeys.erase(pos) ;
                                    }
                                }
                            }

                            //remove duplication element
                            Utility::remove_deplication(vkcs) ;
                            ascii_keys.push_back(vkcs) ;

                            Utility::remove_deplication(sys_vkcs_vec) ;

                            //compute all pattern
                            const auto sys_all_pattern = reconstruct_combination(sys_vkcs_vec) ;
                            sys_key_all_patterns.push_back(sys_all_pattern) ;
                        }

                        //compute all pattern
                        auto phase_all_pattern = reconstruct_combination(sys_key_all_patterns) ;

                        //append required keys at each phase
                        for(auto& phase_list : phase_all_pattern) {
                            for(size_t i = 0 ; i < phase_list.size() ; i ++) {
                                for(const auto& key : ascii_keys[i]) {
                                    phase_list[i].push_back(key) ;
                                }
                            }
                        }

                        //append all pattern
                        for(const auto& phase_list : phase_all_pattern) {
                            vvvc.push_back(phase_list) ;
                        }
                    }

                    map[name.get()] = vvvc ;
                }
            }
        }
        catch(xml_parser_error& e) {
            catch_except(e) ;
            return map ;
        }
        catch(ptree_bad_path& e) {
            catch_except(e) ;
            return map ;
        }
        catch(ptree_bad_data& e) {
            catch_except(e) ;
            return map ;
        }
        catch(out_of_range& e) {
            catch_except(e) ;
        }

        return std::move(map) ;
    }

    const bind_map_t load_command_map(const string& filename) noexcept {
        using namespace VKCConverter ;
        bind_map_t map{} ;

        try {
            ptree pt ;
            read_xml(filename, pt) ;

            for(const auto& pt_func : pt.get_child("config.commands")) {
                if(const auto& name = pt_func.second.get_optional<string>("<xmlattr>.name")) {
                    auto cmds = Utility::split(
                        static_cast<string>(pt_func.second.data()), "|") ;

                    vvvc_t vvvc{} ;
                    for(auto cmd : cmds) {
                        auto pos = cmd.find("OPTIONAL_COMMAND") ;
                        if(pos != string::npos) {
                            cmd = cmd.substr(0, pos) + static_cast<char>(VKC_OPTIONAL) ;
                        }

                        vvc_t vkcs_vec{} ;
                        vvc_t sys_vkcs_vec{} ;

                        for(const auto& c : cmd) {
                            if(c == static_cast<char>(VKC_OPTIONAL)) {
                                vkcs_vec.push_back(vc_t{VKC_OPTIONAL}) ;
                                sys_vkcs_vec.emplace_back() ;
                                break ;
                            }

                            if(const auto vkc = get_vkc(c)) {
                                vkcs_vec.push_back(vc_t{vkc}) ;
                                sys_vkcs_vec.emplace_back() ;
                                continue ;
                            }

                            if(const auto vkc = get_shifted_vkc(c)) {
                                vkcs_vec.push_back(vc_t{vkc}) ;

                                sys_vkcs_vec.push_back(vc_t{VKC_LSHIFT, VKC_RSHIFT}) ;
                                continue ;
                            }
                        }

                        auto all_sys_pattern = reconstruct_combination(sys_vkcs_vec) ;
                        const auto phase_num =  vkcs_vec.size() ;

                        vvvc_t all_pattern{} ;
                        //append required keys at each phase
                        for(const auto& sys_pattern : all_sys_pattern) {
                            vvc_t phase{} ;
                            for(size_t i_phase = 0 ; i_phase < phase_num ; i_phase ++) {
                                vc_t sync{vkcs_vec.at(i_phase)} ;

                                if(!sys_pattern.empty()) {
                                    const auto vkc = sys_pattern.at(i_phase) ;
                                    if(vkc) {
                                        sync.push_back(vkc) ;
                                    }
                                }

                                phase.push_back(sync) ;
                            }
                            all_pattern.push_back(phase) ;
                        }

                        //append all pattern
                        for(const auto& phase_list : all_pattern) {
                            vvvc.push_back(phase_list) ;
                        }
                    }

                    map[name.get()] = vvvc ;
                }
            }
        }
        catch(xml_parser_error& e) {
            catch_except(e) ;
            return map ;
        }
        catch(ptree_bad_path& e) {
            catch_except(e) ;
            return map ;
        }
        catch(ptree_bad_data& e) {
            catch_except(e) ;
            return map ;
        }
        catch(out_of_range& e) {
            catch_except(e) ;
        }

        return std::move(map) ;
    }
}
#include "source.hpp"

#include "bind/bindedfunc.hpp"
#include "core/cmdparser.hpp"
#include "core/errlogger.hpp"
#include "core/inputhub.hpp"
#include "core/mode.hpp"
#include "core/path.hpp"
#include "core/rcparser.hpp"
#include "core/settable.hpp"
#include "opt/vcmdline.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/string.hpp"
#include "util/type_traits.hpp"
#include "util/winwrap.hpp"

#include "autocmd.hpp"
#include "command.hpp"
#include "map.hpp"
#include "set.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <utility>
#include <vector>

#include <shlwapi.h>


namespace
{
    using namespace vind ;
    using namespace vind::bind ;

    template <typename Str>
    auto load_remote_vindrc(Str&& args) {
        using namespace vind ;

        static const auto repo_store_path = core::ROOT_PATH() / "repo" ;

        if(!std::filesystem::exists(repo_store_path)) {
            std::filesystem::create_directories(repo_store_path) ;
        }

        auto slash = args.find_first_of("/") ;
        if(slash == std::string::npos) {
            throw RUNTIME_EXCEPT("Specify in the form user/repo") ;
        }

        auto target_repo_path = repo_store_path / args ;
        target_repo_path.make_preferred() ;

        if(!std::filesystem::exists(target_repo_path)) {
            const auto remote_url = "https://github.com/" + args + ".git" ;

            try {
                util::create_process(
                    core::HOME_PATH(), "git",
                    util::concat_args("clone", "--depth=1", remote_url, target_repo_path.u8string()),
                    false, true) ;
            }
            catch(const std::runtime_error&) {
                opt::VCmdLine::print(opt::ErrorMessage("E: 'git' is not available")) ;
                return std::filesystem::path() ;
            }
        }
        /**
         * TODO: For security reasons, it should ensure that
         *       the .vindrc is the same as the first time.
         *       Therefore, we temporarily mark the following as comments.
         */

        /*
        else {
            util::create_process(target_repo_path, "git", "pull", false, true) ;
        }
        */

        return target_repo_path / ".vindrc" ;
    }

    void to_compatible(std::string& cmd, std::string& args) {
        std::vector<std::pair<std::string, std::string>> compatible {
            {"system_command_comclear", "comclear"},
            {"system_command_command", "command"},
            {"system_command_delcommand", "delcommand"},
            {"system_command_map", "map"},
            {"system_command_mapclear", "mapclear"},
            {"system_command_noremap", "noremap"},
            {"system_command_set", "set"},
            {"system_command_source", "source"},
            {"system_command_unmap", "unmap"},
            {"syscmd_comclear", "comclear"},
            {"syscmd_command", "command"},
            {"syscmd_delcommand", "delcommand"},
            {"syscmd_map", "map"},
            {"syscmd_mapclear", "mapclear"},
            {"syscmd_noremap", "noremap"},
            {"syscmd_set", "set"},
            {"syscmd_source", "source"},
            {"syscmd_unmap", "unmap"},
            {"window_accel", "window_velocity"},
            {"window_tweight", "window_velocity"},
            {"window_maxv", "window_velocity"},
            {"cursor_tweight", "cursor_resolution"},

            {"<easy_click_left>", "<easyclick><click_left>"},
            {"<easy_click_right>", "<easyclick><click_right>"},
            {"<easy_click_mid>", "<easyclick><click_mid>"},
            {"<easy_click_hover>", "<easyclick>"},
        } ;
        for(auto& [from, to] : compatible) {
            args = util::replace_all(args, from, to) ;
        }

        if(args.find("noautofocus_textarea") != std::string::npos) {
            cmd.clear() ;
            args.clear() ;
        }
        else if(args.find("autofocus_textarea") != std::string::npos) {
            cmd = "autocmd" ;
            args = "EdiNormalEnter * <focus_textarea>" ;
        }
    }

    void do_runcommand(
            const std::string& cmd,
            const std::string& args,
            const std::filesystem::path& parent_path,
            bool as_default=false) {
        using core::RunCommandsIndex ;

        auto rcindex = core::parse_run_command(cmd) ;
        switch(rcindex) {
            case RunCommandsIndex::SET: {
                Set::sprocess(1, cmd + " " + args) ;
                return ;
            }
            case RunCommandsIndex::COMMAND: {
                Command::sprocess(1, cmd + " " + args, as_default) ;
                return ;
            }
            case RunCommandsIndex::DELCOMMAND: {
                Delcommand::sprocess(1, cmd + " " + args, as_default) ;
                return ;
            }
            case RunCommandsIndex::COMCLEAR: {
                if(!args.empty()) {
                    throw std::make_error_code(std::errc::invalid_argument) ;
                }
                Comclear::sprocess(1, "", as_default) ;
                return ;
            }
            case RunCommandsIndex::SOURCE: {
                if(args.empty()) {
                    throw std::make_error_code(std::errc::invalid_argument) ;
                }

                auto path = \
                    parent_path / std::filesystem::u8path(core::replace_path_magic(args)) ;

                if(std::filesystem::exists(path)) {
                    if(std::filesystem::equivalent(core::RC(), path)) {
                        throw std::make_error_code(std::errc::text_file_busy) ;
                    }
                    Source::sprocess(1, cmd + " " + path.u8string(), as_default) ;
                }
                else if(path.filename().u8string() != ".vindrc") {
                    Source::sprocess(1, cmd + " " + load_remote_vindrc(args).u8string()) ;
                }

                return ;
            }
            case RunCommandsIndex::AUTOCMD: {
                AutoCmdAdd::sprocess(1, cmd + " " + args) ;
                return ;
            }
            case RunCommandsIndex::AUTOCMD_REMOVE: {
                AutoCmdRemove::sprocess(1, cmd + " " + args) ;
                return ;
            }
            default: {
                break ;
            }
        }

        using core::Mode ;
        if(util::enum_has_bits(rcindex, RunCommandsIndex::MASK_MAP)) {
            auto [prefix, _] = core::divide_prefix_and_cmd(cmd, "m") ;
            do_map(cmd + " " + args, prefix, as_default) ;
        }
        else if(util::enum_has_bits(rcindex, RunCommandsIndex::MASK_NOREMAP)) {
            auto [prefix, _] = core::divide_prefix_and_cmd(cmd, "n") ;
            do_noremap(cmd + " " + args, prefix, as_default) ;
        }
        else if(util::enum_has_bits(rcindex, RunCommandsIndex::MASK_UNMAP)) {
            auto [prefix, _] = core::divide_prefix_and_cmd(cmd, "u") ;
            do_unmap(cmd + " " + args, prefix, as_default) ;
        }
        else if(util::enum_has_bits(rcindex, RunCommandsIndex::MASK_MAPCLEAR)) {
            if(!args.empty()) {
                throw std::make_error_code(std::errc::invalid_argument) ;
            }
            auto [prefix, _] = core::divide_prefix_and_cmd(cmd, "m") ;
            do_mapclear(prefix, as_default) ;
        }
        else {
            throw std::make_error_code(std::errc::argument_out_of_domain) ;
        }
    }

    void init_default_mapping(const std::string& tier="huge") {
        auto default_rc_path = \
           core::RESOURCE_ROOT_PATH() / "defaults" / (tier + ".vindrc") ;
        core::Logger::get_instance().message("Version: " + tier) ;
        Source::sprocess(1, "source " + default_rc_path.u8string(), true) ;

        // Setup the solver of each mode as default mappings.
        core::InputHub::get_instance().apply_mapping(core::Mode::UNDEFINED, true) ;
    }
}


namespace vind
{
    namespace bind
    {
        bool Source::loaded_default_ = false ;

        Source::Source()
        : BindedFuncFlex("source")
        {}

        SystemCall Source::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& args,
                bool as_default) {
            auto [_, pargs] = core::divide_cmd_and_args(args) ;
            std::filesystem::path path{} ;
            if(pargs.empty()) {
                path = core::RC() ;
            }
            else {
                path = core::replace_path_magic(pargs) ;
            }

            std::ifstream ifs(path, std::ios::in) ;
            if(!ifs.is_open()) {
                std::stringstream ss ;
                ss << "Could not open \"" << path.u8string() << "\".\n" ;
                opt::VCmdLine::print(opt::ErrorMessage(ss.str())) ;
                return SystemCall::SUCCEEDED ;
            }

            std::string aline ;
            std::size_t lnum = 0 ;
            while(getline(ifs, aline)) {
                lnum ++ ;

                core::remove_dbquote_comment(aline) ;
                if(aline.empty()) {
                    continue ;
                }

                auto [cmd, line_args] = core::divide_cmd_and_args(aline) ;
                if(cmd.empty()) {
                    continue ;
                }

                cmd = util::A2a(cmd) ;
                to_compatible(cmd, line_args) ;
                if(cmd.empty()) {
                    continue ;
                }

                if(!loaded_default_) {
                    loaded_default_ = true ;
                    init_default_mapping(
                        cmd != "version" ? "huge" : util::A2a(line_args)) ;
                }

                if(cmd == "version") {
                    continue ;
                }

                auto ltag = "L:" + std::to_string(lnum) ;
                try {
                    do_runcommand(cmd, line_args, path.parent_path(), as_default) ;
                }
                catch(const std::error_code& err) {
                    std::stringstream cmdline_ss ;
                    std::stringstream log_ss ;

                    cmdline_ss << "E: " ;
                    if(err.value() == static_cast<int>(std::errc::invalid_argument)) {
                        cmdline_ss << "Invalid Argument" ;
                        log_ss << cmd << " is recieved invalid arguments." ;
                    }
                    else if(err.value() == static_cast<int>(std::errc::argument_out_of_domain)) {
                        cmdline_ss << "Not command" ;
                        log_ss << cmd << " is not supported." ;
                    }
                    else {
                        cmdline_ss << err ;
                        log_ss << cmd << ": " << err ;
                    }
                    cmdline_ss << " (" << ltag << ")" ;
                    opt::VCmdLine::print(opt::ErrorMessage(cmdline_ss.str())) ;

                    log_ss << " (" << path.u8string() << ", " << ltag << ")" ;
                    core::Logger::get_instance().error(log_ss.str()) ;

                    core::InputHub::get_instance().clear_mapping(
                        core::Mode::UNDEFINED, as_default) ;
                    break ;
                }
                catch(const std::runtime_error& e) {
                    std::stringstream log_ss ;
                    log_ss << cmd << ": " << e.what() ;
                    log_ss << " (" << path.u8string() << ", " << ltag << ")" ;
                    core::Logger::get_instance().error(log_ss.str()) ;
                    continue ;
                }
            }

            // If the .vindrc is empty, apply a default tier.
            if(!loaded_default_) {
                loaded_default_ = true ;
                init_default_mapping() ;
            }

            return SystemCall::RECONSTRUCT ;
        }
    }
}

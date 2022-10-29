#include "source.hpp"

#include "bind/bindedfunc.hpp"
#include "core/charlogger.hpp"
#include "core/cmdparser.hpp"
#include "core/entry.hpp"
#include "core/errlogger.hpp"
#include "core/inputhub.hpp"
#include "core/mode.hpp"
#include "core/path.hpp"
#include "core/rcparser.hpp"
#include "core/settable.hpp"
#include "opt/vcmdline.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/type_traits.hpp"
#include "util/winwrap.hpp"

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

#include <shlwapi.h>


namespace
{
    using namespace vind ;

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

    std::error_code do_runcommand(const std::string& cmd, const std::string& args) {
        using namespace vind ;
        using namespace vind::bind ;
        using core::RunCommandsIndex ;
        auto rcindex = core::parse_run_command(cmd) ;
        switch(rcindex) {
            case RunCommandsIndex::SET: {
                Set::sprocess(1, args) ;
                return std::error_code() ;
            }
            case RunCommandsIndex::COMMAND: {
                Command::sprocess(1, args) ;
                return std::error_code() ;
            }
            case RunCommandsIndex::DELCOMMAND: {
                Delcommand::sprocess(1, args) ;
                return std::error_code() ;
            }
            case RunCommandsIndex::COMCLEAR: {
                if(!args.empty()) {
                    return std::make_error_code(std::errc::invalid_argument) ;
                }
                Comclear::sprocess(1, "") ;
                return std::error_code() ;
            }
            case RunCommandsIndex::SOURCE: {
                if(args.empty()) {
                    return std::make_error_code(std::errc::invalid_argument) ;
                }

                auto args_path = \
                     std::filesystem::u8path(core::replace_path_magic(args)) ;

                if(std::filesystem::exists(args_path)) {
                    if(std::filesystem::equivalent(core::RC(), args_path)) {
                        return std::make_error_code(std::errc::text_file_busy) ;
                    }
                }

                if(args_path.filename().u8string() != ".vindrc") {
                    args_path = load_remote_vindrc(args) ;
                }

                if(!args_path.empty()) {
                    Source::sprocess(1, args_path.u8string()) ; //overload .vindrc
                }
                return std::error_code() ;
            }
            default: {
                break ;
            }
        }

        using core::Mode ;
        if(util::enum_has_bits(rcindex, RunCommandsIndex::MASK_MAP)) {
            auto [prefix, _] = core::divide_prefix_and_cmd(cmd, "m") ;
            do_map(args, prefix) ;
        }
        else if(util::enum_has_bits(rcindex, RunCommandsIndex::MASK_NOREMAP)) {
            auto [prefix, _] = core::divide_prefix_and_cmd(cmd, "n") ;
            do_noremap(args, prefix) ;
        }
        else if(util::enum_has_bits(rcindex, RunCommandsIndex::MASK_UNMAP)) {
            auto [prefix, _] = core::divide_prefix_and_cmd(cmd, "u") ;
            do_unmap(args, prefix) ;
        }
        else if(util::enum_has_bits(rcindex, RunCommandsIndex::MASK_MAPCLEAR)) {
            if(!args.empty()) {
                return std::make_error_code(std::errc::invalid_argument) ;
            }
            auto [prefix, _] = core::divide_prefix_and_cmd(cmd, "m") ;
            do_mapclear(prefix) ;
        }
        else {
            return std::make_error_code(std::errc::argument_out_of_domain) ;
        }
        return std::error_code() ;
    }
}


namespace vind
{
    namespace bind
    {
        Source::Source()
        : BindedFuncFlex("source")
        {
            std::ifstream ifs(core::RC()) ;
            if(!ifs.is_open()) {
                std::ofstream ofs(core::RC(), std::ios::trunc) ;
            }
        }

        SystemCall Source::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& args) {
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

                if(auto err = do_runcommand(cmd, line_args)) {
                    auto ltag = "L:" + std::to_string(lnum) ;
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

                    log_ss << " (" << path.u8string() << ", " << ltag << ") " ;
                    PRINT_ERROR(log_ss.str()) ;

                    core::InputHub::get_instance().clear_mapping() ;
                    break ;
                }
            }
            return SystemCall::RECONSTRUCT ;
        }
    }
}

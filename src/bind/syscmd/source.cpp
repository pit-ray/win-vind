#include "source.hpp"

#include "bind/bindedfunc.hpp"
#include "core/charlogger.hpp"
#include "core/cmdparser.hpp"
#include "core/entry.hpp"
#include "core/errlogger.hpp"
#include "core/maptable.hpp"
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

#include <shlwapi.h>


namespace
{
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


    template <typename Str>
    void do_runcommand(
            vind::core::RunCommandsIndex rcindex,
            Str&& args) {
        using namespace vind ;
        using namespace vind::bind ;
        using core::RunCommandsIndex ;

        switch(rcindex) {
            case RunCommandsIndex::SET: {
                SyscmdSet::sprocess(1, std::forward<Str>(args)) ;
                return ;
            }
            case RunCommandsIndex::COMMAND: {
                SyscmdCommand::sprocess(1, std::forward<Str>(args)) ;
                return ;
            }
            case RunCommandsIndex::DELCOMMAND: {
                SyscmdDelcommand::sprocess(1, std::forward<Str>(args)) ;
                return ;
            }
            case RunCommandsIndex::COMCLEAR: {
                if(!args.empty()) {
                    throw std::invalid_argument("comclear") ;
                }
                SyscmdComclear::sprocess(1, "") ;
                return ;
            }
            case RunCommandsIndex::SOURCE: {
                if(args.empty()) {
                    throw std::invalid_argument("source") ;
                }

                auto args_path = std::filesystem::u8path(
                        core::replace_path_magic(args)) ;

                if(std::filesystem::exists(args_path)) {
                    if(std::filesystem::equivalent(core::RC(), args_path)) {
                        throw std::invalid_argument(
                                "Recursive references to the same .vindrc are not allowed.") ;
                    }
                }

                if(args_path.filename().u8string() != ".vindrc") {
                    args_path = load_remote_vindrc(std::forward<Str>(args)) ;
                }

                if(!args_path.empty()) {
                    SyscmdSource::sprocess(1, args_path.u8string()) ; //overload .vindrc
                }
                return ;
            }

            default: {
                break ;
            }
        }

        using core::Mode ;
        auto mode = static_cast<Mode>(
                util::enum_and(rcindex, RunCommandsIndex::MASK_MODE)) ;

        if(util::enum_has_bits(rcindex, RunCommandsIndex::MASK_MAP)) {
            SyscmdMap::sprocess(1, std::forward<Str>(args)) ;
        }
        else if(util::enum_has_bits(rcindex, RunCommandsIndex::MASK_NOREMAP)) {
            SyscmdNoremap::sprocess(1, std::forward<Str>(args)) ;
        }
        else if(util::enum_has_bits(rcindex, RunCommandsIndex::MASK_UNMAP)) {
            SyscmdUnmap::sprocess(1, std::forward<Str>(args)) ;
        }
        else if(util::enum_has_bits(rcindex, RunCommandsIndex::MASK_MAPCLEAR)) {
            if(!args.empty()) {
                throw std::invalid_argument("mapclear") ;
            }
            SyscmdMapclear::sprocess(1, "") ;
        }
        else {
            throw std::domain_error(std::to_string(static_cast<int>(rcindex))) ;
        }
    }
}


namespace vind
{
    namespace bind
    {
        SyscmdSource::SyscmdSource()
        : BindedFuncFlex("system_command_source")
        {
            std::ifstream ifs(core::RC()) ;
            if(!ifs.is_open()) {
                std::ofstream ofs(core::RC(), std::ios::trunc) ;
            }
        }

        SystemCall SyscmdSource::sprocess(
                std::uint16_t count, const std::string& args) {
            std::filesystem::path path{} ;
            if(args.empty()) {
                path = core::RC() ;
            }
            else {
                path = core::replace_path_magic(args) ;
            }

            auto return_to_default = [] {
                /*
                core::SetTable::get_instance().reset_todef() ;
                core::MapTable::get_instance().reset_todef() ;
                */
            } ;

            /*
            if(start_from_default) {
                return_to_default() ;
            }
            */

            std::ifstream ifs(path, std::ios::in) ;
            if(!ifs.is_open()) {
                opt::VCmdLine::print(opt::ErrorMessage("Could not open \"" + path.u8string() + "\".\n")) ;

                return SystemCall::NOTHING ;
            }

            std::string aline ;
            std::size_t lnum = 0 ;
            while(getline(ifs, aline)) {
                lnum ++ ;

                try {
                    core::remove_dbquote_comment(aline) ;
                    if(aline.empty()) {
                        continue ;
                    }

                    auto [cmd, args] = core::divide_cmd_and_args(aline) ;
                    if(cmd.empty()) {
                        continue ;
                    }

                    auto rcindex = core::parse_run_command(cmd) ;

                    do_runcommand(rcindex, args) ;
                }
                catch(const std::domain_error& e) {
                    auto ltag = "L:" + std::to_string(lnum) ;
                    opt::VCmdLine::print(opt::ErrorMessage("E: Not command (" + ltag + ")")) ;

                    std::stringstream ss ;
                    ss << "RunCommandsIndex: " << e.what() << " is not supported." ;
                    ss << " (" << path.u8string() << ", " << ltag << ") " ;
                    PRINT_ERROR(ss.str()) ;

                    return_to_default() ;
                    break ;
                }
                catch(const std::invalid_argument& e) {
                    auto ltag = "L:" + std::to_string(lnum) ;
                    opt::VCmdLine::print(opt::ErrorMessage("E: Invalid Argument (" + ltag + ")")) ;

                    std::stringstream ss ;
                    ss << e.what() << " is recieved invalid arguments." ;
                    ss << " (" << path.u8string() << ", " << ltag << ") " ;
                    PRINT_ERROR(ss.str()) ;

                    return_to_default() ;
                    break ;
                }
                catch(const std::logic_error& e) {
                    auto ltag = "L:" + std::to_string(lnum) ;
                    opt::VCmdLine::print(opt::ErrorMessage("E: Invalid Syntax (" + ltag + ")")) ;

                    std::stringstream ss ;
                    ss << e.what() ;
                    ss << " (" + path.u8string() + ", " + ltag + ")" ;
                    PRINT_ERROR(ss.str()) ;

                    return_to_default() ;
                    break ;
                }
                catch(const std::runtime_error& e) {
                    auto ltag = "L:" + std::to_string(lnum) ;
                    opt::VCmdLine::print(opt::ErrorMessage("E: Invalid Syntax (" + ltag + ")")) ;

                    std::stringstream ss ;
                    ss << e.what() ;
                    ss << " (" + path.u8string() + ", " + ltag + ")" ;
                    PRINT_ERROR(ss.str()) ;

                    return_to_default() ;
                    break ;
                }
            }

            return SystemCall::NOTHING ;
        }
    }
}

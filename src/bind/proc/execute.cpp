#include "execute.hpp"

#include "open_window.hpp"

#include "core/char_logger.hpp"
#include "core/ntype_logger.hpp"
#include "core/path.hpp"
#include "core/rc_parser.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace bind
    {
        Execute::Execute()
        : BindedFuncVoid("execute")
        {}

        void Execute::sprocess(std::filesystem::path filepath) {
            if(filepath.empty()) {
                return ;
            }

            filepath = core::replace_path_magic(filepath) ;

            switch(util::shell_execute(filepath)) {
                case 0:
                case SE_ERR_OOM:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Not enough memory")) ;
                    break ;

                case ERROR_FILE_NOT_FOUND:
                case ERROR_PATH_NOT_FOUND:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Not found the file")) ;
                    break ;

                case ERROR_BAD_FORMAT:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Bad format file")) ;
                    break ;

                case SE_ERR_ACCESSDENIED:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Access is denied")) ;
                    break ;

                case SE_ERR_ASSOCINCOMPLETE:
                case SE_ERR_NOASSOC:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Association is incomplete")) ;
                    break ;

                case SE_ERR_DDEBUSY:
                    opt::VCmdLine::print(opt::ErrorMessage("E: DDE is busy")) ;
                    break ;

                case SE_ERR_DDEFAIL:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Failed DDE")) ;
                    break ;

                case SE_ERR_DDETIMEOUT:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Timeout DDE")) ;
                    break ;

                case SE_ERR_DLLNOTFOUND:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Not found DLL")) ;
                    break ;

                case SE_ERR_SHARE:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Association is incomplete")) ;
                    break ;

                default:
                    break ;
            }
        }

        void Execute::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(core::RC()) ;
            }
        }

        void Execute::sprocess(const core::CharLogger& parent_lgr) {
            auto str = parent_lgr.to_str() ;

            auto [cmd, arg] = core::divide_cmd_and_args(str) ;
            if(arg.empty()) {
                sprocess(core::RC()) ;
                return ;
            }

            sprocess(std::filesystem::u8path(arg)) ;
        }
    }
}

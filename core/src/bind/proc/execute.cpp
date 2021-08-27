#include "bind/proc/execute.hpp"

#include "path.hpp"

#include "util/def.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"

#include "key/char_logger.hpp"
#include "opt/vcmdline.hpp"
#include "parser/rc_parser.hpp"

#include "bind/proc/open_window.hpp"


namespace vind
{
    Execute::Execute()
    : BindedFuncCreator("execute")
    {}

    void Execute::sprocess(std::string filepath) {
        if(filepath.empty()) {
            return ;
        }

        path::replace_magic(filepath) ;

        switch(util::shell_execute(filepath)) {
            case 0:
            case SE_ERR_OOM:
                VCmdLine::print(ErrorMessage("E: Not enough memory")) ;
                break ;

            case ERROR_FILE_NOT_FOUND:
            case ERROR_PATH_NOT_FOUND:
                VCmdLine::print(ErrorMessage("E: Not found the file")) ;
                break ;

            case ERROR_BAD_FORMAT:
                VCmdLine::print(ErrorMessage("E: Bad format file")) ;
                break ;

            case SE_ERR_ACCESSDENIED:
                VCmdLine::print(ErrorMessage("E: Access is denied")) ;
                break ;

            case SE_ERR_ASSOCINCOMPLETE:
            case SE_ERR_NOASSOC:
                VCmdLine::print(ErrorMessage("E: Association is incomplete")) ;
                break ;

            case SE_ERR_DDEBUSY:
                VCmdLine::print(ErrorMessage("E: DDE is busy")) ;
                break ;

            case SE_ERR_DDEFAIL:
                VCmdLine::print(ErrorMessage("E: Failed DDE")) ;
                break ;

            case SE_ERR_DDETIMEOUT:
                VCmdLine::print(ErrorMessage("E: Timeout DDE")) ;
                break ;

            case SE_ERR_DLLNOTFOUND:
                VCmdLine::print(ErrorMessage("E: Not found DLL")) ;
                break ;

            case SE_ERR_SHARE:
                VCmdLine::print(ErrorMessage("E: Association is incomplete")) ;
                break ;

            default:
                break ;
        }
    }

    void Execute::sprocess(NTypeLogger& UNUSED(parent_lgr)) {
    }

    void Execute::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;

        auto [cmd, arg] = rcparser::divide_cmd_and_args(str) ;
        if(arg.empty()) {
            OpenNewWindow::sprocess() ;
            return ;
        }

        sprocess(arg) ;
    }
}

#include "command_mode.hpp"

#include <algorithm>
#include <windows.h>

#include <vector>

#include "binded_func.hpp"
#include "i_params.hpp"
#include "io/keybrd.hpp"
#include "key/char_logger.hpp"
#include "key/key_absorber.hpp"
#include "key/key_logger_base.hpp"
#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"
#include "key/keycode_logger.hpp"
#include "bind.hpp"
#include "mode.hpp"
#include "err_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/def.hpp"
#include "entry.hpp"

namespace
{
    using namespace vind ;

    struct CmdPoint
    {
        CharLogger logger{} ;
        BindedFunc::shp_t func = nullptr ;
    } ;
    using hist_point_t = std::shared_ptr<CmdPoint> ;
    using hist_t = std::deque<hist_point_t> ;

    class CmdHist
    {
    private:
        hist_t hist ;
        std::size_t idx ;

    public:
        explicit CmdHist()
        : hist{std::make_shared<CmdPoint>()},
          idx(0)
        {}

        const hist_point_t& get_hist_point() {
            return hist.at(idx) ;
        }

        bool forward() noexcept {
            if(idx == hist.size() - 1) {
                return false ; //Could not forward
            }

            idx ++ ;
            return true ;
        }

        bool backward() noexcept {
            if(idx == 0) {
                return false ; //Could not backward
            }
            idx -- ;
            return true ;
        }

        void forward_to_latest() noexcept {
            idx = hist.size() - 1 ;
        }

        void generate_new_hist() {
            if(idx == hist.size() - 1) {
                //recently logger
                while(hist.size() >= iparams::get_z("cmd_max_history_num")) {
                    hist.pop_front() ;
                }

                idx = hist.size() ; //update to index of recently history
                hist.push_back(std::make_shared<CmdPoint>()) ;
            }
            else {
                //the current point is past one, so move to latest one.
                forward_to_latest() ;

                auto p = hist.at(idx) ;
                p->logger.clear() ;
                p->func = nullptr ;
            }
        }

        bool is_pointing_latest() noexcept {
            return idx == hist.size() - 1 ;
        }
    } ;
}

struct CommandMode::Impl {
    CmdHist ch{} ;
} ;

CommandMode::CommandMode()
: pimpl(std::make_unique<Impl>())
{}

CommandMode::~CommandMode() noexcept               = default ;
CommandMode::CommandMode(CommandMode&&)            = default ;
CommandMode& CommandMode::operator=(CommandMode&&) = default ;

const std::string CommandMode::sname() noexcept {
    return "command_mode" ;
}

void CommandMode::sprocess(
        bool first_call,
        unsigned int UNUSED(repeat_num),
        KeycodeLogger* const UNUSED(parent_keycodelgr),
        const CharLogger* const UNUSED(parent_charlgr)) const {
    if(!first_call) return ;

    VirtualCmdLine::reset() ;

    keyabsorber::InstantKeyAbsorber ika ;

    while(vind::update_background()) {
        auto& p_cmdp = pimpl->ch.get_hist_point() ;
        auto& lgr    = p_cmdp->logger ;

        VirtualCmdLine::cout(":" + lgr.to_str()) ;

        lgr.update() ;

        if(!lgr.is_changed()) { //update log as character input
            continue ;
        }

        //canceling operation
        if(lgr.latest().is_containing(KEYCODE_ESC)){
            if(pimpl->ch.is_pointing_latest()) {
                lgr.clear() ;
                p_cmdp->func = nullptr ;
            }
            else {
                lgr.remove_from_back(1) ;
                pimpl->ch.forward_to_latest() ;
            }

            VirtualCmdLine::reset() ;
            break ;
        }

        //decision of input
        if(lgr.latest().is_containing(KEYCODE_ENTER) && p_cmdp->func) {
            keyabsorber::release_virtually(KEYCODE_ENTER) ;

            lgr.remove_from_back(1) ; //remove keycode of enter

            VirtualCmdLine::reset() ;

            p_cmdp->func->process(true, 1, nullptr, &lgr) ;

            pimpl->ch.generate_new_hist() ;
            break ;
        }

        //edit command
        if(lgr.latest().is_containing(KEYCODE_BKSPACE)) {
            if(lgr.size() == 1) {
                lgr.clear() ;
                p_cmdp->func = nullptr ;
                VirtualCmdLine::reset() ;
                break ;
            }

            lgr.remove_from_back(2) ;
            VirtualCmdLine::refresh() ;

            if(auto mf = keybind::find_func(lgr, p_cmdp->func, true, mode::Mode::Command)) {
                if(mf->is_callable()) {
                    p_cmdp->func = mf ;
                    continue ;
                }
            }
            p_cmdp->func = nullptr ;
            continue ;
        }

        //command history operation
        if(lgr.latest().is_containing(KEYCODE_UP)) {
            keyabsorber::release_virtually(KEYCODE_UP) ; //prohibit duplicate logging
            lgr.remove_from_back(1) ;
            if(pimpl->ch.backward()) {
                VirtualCmdLine::refresh() ;
            }
            continue ;
        }

        if(lgr.latest().is_containing(KEYCODE_DOWN)) {
            keyabsorber::release_virtually(KEYCODE_DOWN) ; //prohibit duplicate logging
            lgr.remove_from_back(1) ;
            if(pimpl->ch.forward()) {
                VirtualCmdLine::refresh() ;
            }
            continue ;
        }

        //invalid keys
        if(is_invalid_log(lgr.latest(), keybind::InvalidPolicy::AllSystemKey) ||
                lgr.size() > iparams::get_z("cmd_max_char")) {
            lgr.remove_from_back(1) ;
            continue ;
        }

        if(auto matched_func = keybind::find_func(lgr, p_cmdp->func, false, mode::Mode::Command)) {
            if(matched_func->is_callable()) {
                p_cmdp->func = matched_func ;
                continue ;
            }
        }
        p_cmdp->func = nullptr ;
    }
}

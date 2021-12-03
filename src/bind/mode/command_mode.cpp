#include "command_mode.hpp"

#include <algorithm>
#include <windows.h>

#include <vector>

#include "bind/binded_func.hpp"
#include "core/char_logger.hpp"
#include "core/entry.hpp"
#include "core/err_logger.hpp"
#include "core/func_finder.hpp"
#include "core/g_params.hpp"
#include "core/key_absorber.hpp"
#include "core/key_logger_base.hpp"
#include "core/keycode_def.hpp"
#include "core/keycodecvt.hpp"
#include "core/logpooler.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "opt/vcmdline.hpp"
#include "util/container.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace
{
    using namespace vind ;

    struct CmdPoint
    {
        core::CharLogger logger {
            KEYCODE_ESC,
            KEYCODE_ENTER,
            KEYCODE_BKSPACE,
            KEYCODE_UP,
            KEYCODE_DOWN
        } ;

        std::size_t lastlgr_size = 0 ;

        bind::BindedFunc::SPtr func = nullptr ;

        using SPtr = std::shared_ptr<CmdPoint> ;

        void reset() {
            logger.clear() ;
            lastlgr_size = 0 ;
            func = nullptr ;
        }

        void backward(std::size_t i) {
            logger.remove_from_back(i) ;
            lastlgr_size = logger.size() ;
        }
    } ;

    using CmdHistory = std::vector<CmdPoint::SPtr> ;

    class CmdHist
    {
    private:
        CmdHistory hist_ ;
        std::size_t idx_ ;

    public:
        explicit CmdHist()
        : hist_{std::make_shared<CmdPoint>()},
          idx_(0)
        {}

        const CmdPoint::SPtr& get_hist_point() {
            return hist_.at(idx_) ;
        }

        std::size_t index() const noexcept {
            return idx_ ;
        }

        bool forward() noexcept {
            if(idx_ == hist_.size() - 1) {
                return false ; //Could not forward
            }

            idx_ ++ ;
            return true ;
        }

        bool backward() noexcept {
            if(idx_ == 0) {
                return false ; //Could not backward
            }
            idx_ -- ;
            return true ;
        }

        void forward_to_latest() noexcept {
            idx_ = hist_.size() - 1 ;
        }

        void generate_new_hist() {
            if(idx_ == hist_.size() - 1) {
                //recently logger

                auto over_num =
                    static_cast<long>(hist_.size()) - core::get_l("cmd_maxhist") ;

                if(over_num > 0) {
                    util::remove_from_top(hist_, over_num) ;
                }

                idx_ = hist_.size() ; //update to index of recently history
                hist_.push_back(std::make_shared<CmdPoint>()) ;
            }
            else {
                //the current point is past one, so move to latest one.
                forward_to_latest() ;

                auto p = hist_.at(idx_) ;
                p->reset() ;
            }
        }

        bool is_pointing_latest() noexcept {
            return idx_ == hist_.size() - 1 ;
        }
    } ;
}

namespace vind
{
    namespace bind
    {
        struct ToCommand::Impl {
            CmdHist ch_{} ;
            core::FuncFinder funcfinder_{} ;
        } ;

        ToCommand::ToCommand()
        : BindedFuncCreator("to_command"),
          pimpl(std::make_unique<Impl>())
        {}

        ToCommand::~ToCommand() noexcept               = default ;
        ToCommand::ToCommand(ToCommand&&)            = default ;
        ToCommand& ToCommand::operator=(ToCommand&&) = default ;

        void ToCommand::reconstruct() {
            pimpl->funcfinder_.reconstruct() ;
        }

        void ToCommand::sprocess() const {
            auto return_mode = [] (core::Mode* m) {
                // If the mode is changed, then do nothing.
                if(core::get_global_mode() == core::Mode::COMMAND) {
                    core::set_global_mode(*m) ;
                }
            } ;
            std::unique_ptr<core::Mode, decltype(return_mode)>
                mode_preserver(new core::Mode(core::get_global_mode()), return_mode) ;

            core::set_global_mode(core::Mode::COMMAND) ;

            pimpl->funcfinder_.reset_parser_states() ;

            opt::VCmdLine::reset() ;

            core::InstantKeyAbsorber ika ;

            constexpr auto cmdline_prefix = ":" ;
            opt::VCmdLine::print(opt::StaticMessage(cmdline_prefix)) ;

            while(core::update_background()) {
                auto& p_cmdp = pimpl->ch_.get_hist_point() ;
                auto& lgr    = p_cmdp->logger ;

                auto log = core::LogPooler::get_instance().pop_log() ;
                if(CHAR_EMPTY(lgr.logging_state(log))) {
                    continue ;
                }

                //canceling operation
                if(lgr.latest().is_containing(KEYCODE_ESC)){
                    if(pimpl->ch_.is_pointing_latest()) {
                        p_cmdp->reset() ;
                    }
                    else {
                        p_cmdp->backward(1) ; // remove <ESC>'s log
                        pimpl->ch_.forward_to_latest() ;
                    }

                    opt::VCmdLine::reset() ;
                    break ;
                }

                //decision of input
                if(lgr.latest().is_containing(KEYCODE_ENTER)) {
                    p_cmdp->backward(1) ; //remove log including KEYCODE_ENTER
                    opt::VCmdLine::reset() ;

                    if(p_cmdp->func) {
                        p_cmdp->func->process(lgr) ;
                    }
                    else {
                        opt::VCmdLine::print(opt::ErrorMessage("E: Not a command")) ;
                    }

                    pimpl->ch_.generate_new_hist() ;
                    auto& new_lgr = pimpl->ch_.get_hist_point()->logger ;
                    new_lgr.sync_state_with(lgr) ;

                    break ;
                }

                //edit command
                if(lgr.latest().is_containing(KEYCODE_BKSPACE)) {
                    if(lgr.size() == 1) {
                        p_cmdp->reset() ;
                        opt::VCmdLine::reset() ;
                        break ;
                    }

                    p_cmdp->backward(2) ;
                    opt::VCmdLine::print(opt::StaticMessage(cmdline_prefix + lgr.to_str())) ;
                    opt::VCmdLine::refresh() ;

                    pimpl->funcfinder_.backward_parser_states(1) ;

                    if(auto acced = pimpl->funcfinder_.find_accepted_parser()) {
                        p_cmdp->func = acced->get_func() ;
                    }
                    else {
                        p_cmdp->func = nullptr ;
                    }
                    continue ;
                }

                //command history operation
                if(lgr.latest().is_containing(KEYCODE_UP)) {
                    p_cmdp->backward(1) ; //to remove a log including KEYCODE_UP
                    if(pimpl->ch_.backward()) {
                        auto& b_lgr = pimpl->ch_.get_hist_point()->logger ;
                        b_lgr.sync_state_with(lgr) ;

                        opt::VCmdLine::print(opt::StaticMessage(cmdline_prefix + b_lgr.to_str())) ;
                        opt::VCmdLine::refresh() ;

                        pimpl->funcfinder_.reset_parser_states() ;
                        pimpl->funcfinder_.transition_parser_states_in_batch(b_lgr) ;
                    }
                    continue ;
                }

                if(lgr.latest().is_containing(KEYCODE_DOWN)) {
                    p_cmdp->backward(1) ; //to remove a log including KEYCODE_DOWN
                    if(pimpl->ch_.forward()) {
                        auto& f_lgr = pimpl->ch_.get_hist_point()->logger ;
                        f_lgr.sync_state_with(lgr) ;

                        opt::VCmdLine::print(opt::StaticMessage(cmdline_prefix + f_lgr.to_str())) ;
                        opt::VCmdLine::refresh() ;

                        pimpl->funcfinder_.reset_parser_states() ;
                        pimpl->funcfinder_.transition_parser_states_in_batch(f_lgr) ;
                    }
                    continue ;
                }

                opt::VCmdLine::print(opt::StaticMessage(cmdline_prefix + lgr.to_str())) ;

                // 
                // Since there may be multiple logging in one iteration,
                // transition the state by the increase from the previous iteration.
                //
                core::LoggerParser::SPtr parser ;
                auto appended_num = lgr.size() - p_cmdp->lastlgr_size ;
                for(auto itr = lgr.end() - appended_num ; itr != lgr.end() ; itr ++) {
                    parser = pimpl->funcfinder_.find_parser_with_transition(*itr, id()) ;
                }
                p_cmdp->lastlgr_size = lgr.size() ;

                if(parser) {
                    if(parser->is_accepted()) {
                        p_cmdp->func = parser->get_func() ;
                        continue ;
                    }
                    else if(parser->is_rejected_with_ready()) {
                        pimpl->funcfinder_.backward_parser_states(1) ;
                        p_cmdp->backward(1) ;
                    }
                }
                p_cmdp->func = nullptr ;
            }
        }

        void ToCommand::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void ToCommand::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess() ;
        }
    }
}

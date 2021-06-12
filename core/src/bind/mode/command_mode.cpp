#include "bind/mode/command_mode.hpp"

#include <algorithm>
#include <windows.h>

#include <vector>

#include "bind/binded_func.hpp"
#include "bind/func_finder.hpp"
#include "entry.hpp"
#include "err_logger.hpp"
#include "g_params.hpp"
#include "io/keybrd.hpp"
#include "key/char_logger.hpp"
#include "key/key_absorber.hpp"
#include "key/key_logger_base.hpp"
#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"
#include "key/ntype_logger.hpp"
#include "mode.hpp"
#include "opt/vcmdline.hpp"
#include "util/container.hpp"
#include "util/def.hpp"

namespace
{
    using namespace vind ;

    struct CmdPoint
    {
        CharLogger logger{
            KEYCODE_ESC,
            KEYCODE_ENTER,
            KEYCODE_BKSPACE,
            KEYCODE_UP,
            KEYCODE_DOWN} ;
        BindedFunc::SPtr func = nullptr ;

        using SPtr = std::shared_ptr<CmdPoint> ;
    } ;

    using CmdHistory = std::vector<CmdPoint::SPtr> ;

    class CmdHist
    {
    private:
        CmdHistory hist ;
        std::size_t idx ;

    public:
        explicit CmdHist()
        : hist{std::make_shared<CmdPoint>()},
          idx(0)
        {}

        const CmdPoint::SPtr& get_hist_point() {
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

                auto over_num =
                    static_cast<long>(hist.size()) - gparams::get_l("cmd_maxhist") ;

                if(over_num > 0) {
                    util::remove_from_top(hist, over_num) ;
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

namespace vind
{
    struct ToCommand::Impl {
        CmdHist ch_{} ;
        FuncFinder funcfinder_{} ;
    } ;

    ToCommand::ToCommand()
    : BindedFuncCreator("to_command"),
      pimpl(std::make_unique<Impl>())
    {}

    ToCommand::~ToCommand() noexcept               = default ;
    ToCommand::ToCommand(ToCommand&&)            = default ;
    ToCommand& ToCommand::operator=(ToCommand&&) = default ;

    void ToCommand::reconstruct() {
        pimpl->funcfinder_.reconstruct_funcset() ;
    }

    void ToCommand::sprocess() const {
        auto return_mode = [] (mode::Mode* m) {
            // If the mode is changed, then do nothing.
            if(mode::get_global_mode() == mode::Mode::COMMAND) {
                mode::set_global_mode(*m) ;
            }
        } ;
        std::unique_ptr<mode::Mode, decltype(return_mode)>
            mode_preserver(new mode::Mode(mode::get_global_mode()), return_mode) ;

        mode::set_global_mode(mode::Mode::COMMAND) ;

        pimpl->funcfinder_.reset_parser_states() ;

        VirtualCmdLine::reset() ;

        keyabsorber::InstantKeyAbsorber ika ;

        while(vind::update_background()) {
            auto& p_cmdp = pimpl->ch_.get_hist_point() ;
            auto& lgr    = p_cmdp->logger ;

            VirtualCmdLine::cout(":" + lgr.to_str()) ;

            if(CHAR_EMPTY(lgr.logging_state())) {
                continue ;
            }

            //canceling operation
            if(lgr.latest().is_containing(KEYCODE_ESC)){
                if(pimpl->ch_.is_pointing_latest()) {
                    lgr.clear() ;
                    p_cmdp->func = nullptr ;
                }
                else {
                    lgr.remove_from_back(1) ;
                    pimpl->ch_.forward_to_latest() ;
                }

                VirtualCmdLine::reset() ;
                break ;
            }

            //decision of input
            if(lgr.latest().is_containing(KEYCODE_ENTER)) {
                lgr.remove_from_back(1) ; //remove log including KEYCODE_ENTER
                VirtualCmdLine::reset() ;

                if(p_cmdp->func) {
                    p_cmdp->func->process(lgr) ;
                }
                else {
                    VirtualCmdLine::msgout("E: Not a command") ;
                }

                pimpl->ch_.generate_new_hist() ;
                auto& new_lgr = pimpl->ch_.get_hist_point()->logger ;
                new_lgr.sync_state_with(lgr) ;

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
                lgr.remove_from_back(1) ; //to remove a log including KEYCODE_UP
                if(pimpl->ch_.backward()) {
                    VirtualCmdLine::refresh() ;

                    auto& b_lgr = pimpl->ch_.get_hist_point()->logger ;
                    b_lgr.sync_state_with(lgr) ;

                    pimpl->funcfinder_.reset_parser_states() ;
                    pimpl->funcfinder_.transition_parser_states_in_batch(b_lgr) ;
                }
                continue ;
            }

            if(lgr.latest().is_containing(KEYCODE_DOWN)) {
                lgr.remove_from_back(1) ; //to remove a log including KEYCODE_DOWN
                if(pimpl->ch_.forward()) {
                    VirtualCmdLine::refresh() ;

                    auto& f_lgr = pimpl->ch_.get_hist_point()->logger ;
                    f_lgr.sync_state_with(lgr) ;

                    pimpl->funcfinder_.reset_parser_states() ;
                    pimpl->funcfinder_.transition_parser_states_in_batch(f_lgr) ;
                }
                continue ;
            }

            if(auto parser = pimpl->funcfinder_.find_parser_with_transition(lgr.latest(), id())) {
                if(parser->is_accepted()) {
                    p_cmdp->func = parser->get_func() ;
                    continue ;
                }
                else if(parser->is_rejected_with_ready()) {
                    pimpl->funcfinder_.backward_parser_states(1) ;
                    lgr.remove_from_back(1) ;
                }
            }
            p_cmdp->func = nullptr ;
        }
    }

    void ToCommand::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void ToCommand::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
}

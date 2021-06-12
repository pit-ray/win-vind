#include "bind/emu/edi_motion.hpp"

#include "bind/emu/edi_delete.hpp"
#include "bind/emu/edi_yank.hpp"
#include "bind/func_finder.hpp"
#include "bind/mode/change_mode.hpp"
#include "bind/safe_repeater.hpp"
#include "entry.hpp"
#include "key/key_absorber.hpp"
#include "key/key_logger_base.hpp"
#include "key/ntype_logger.hpp"
#include "mode.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/def.hpp"

#include <iostream>

namespace
{
    using namespace vind ;
    bool select_by_motion(std::size_t caller_self_id, FuncFinder& ff) {
        NTypeLogger lgr ;
        keyabsorber::InstantKeyAbsorber ika ;

        while(vind::update_background()) {
            auto result = lgr.logging_state() ;
            if(NTYPE_EMPTY(result)) {
                continue ;
            }
            if(NTYPE_HEAD_NUM(result)) {
                VirtualCmdLine::cout(std::to_string(lgr.get_head_num())) ;
                continue ;
            }

            using mode::Mode ;
            using mode::ModeFlags ;
            auto parser = ff.find_parser_with_transition(
                    lgr.latest(),
                    caller_self_id,
                    Mode::EDI_VISUAL) ;
            if(parser && parser->get_func()->is_for_moving_caret()) {
                if(parser->is_accepted()) {
                    mode::set_global_mode(Mode::EDI_VISUAL, ModeFlags::VISUAL_LINE) ;
                    parser->get_func()->process(lgr) ;
                    return true ;
                }
                else if(parser->is_rejected_with_ready()) {
                    ff.backward_parser_states(1) ;
                    lgr.remove_from_back(1) ;
                }
            }
            else {
                break ;
            }
        }

        return false ;
    }

    //The parent logger is stronger than the child logger.
    //For example, the child BindedFunc calling this function is binded with 'c{motion}'
    //and 'cc' are bindings ChangeLine.
    //In this case, if a child process has a message loop, we must consider the parent logger by full scanning.
    //
    // True : called with motion
    // False: called with parent_caller
    //
    bool select_by_motion(
            std::size_t caller_self_id,
            FuncFinder& ff,
            FuncFinder&parent_ff,
            NTypeLogger& parent_lgr) {

        using mode::Mode ;
        using mode::ModeFlags ;
        ff.reset_parser_states(Mode::EDI_VISUAL) ;
        parent_ff.reset_parser_states() ;
        parent_ff.transition_parser_states_in_batch(parent_lgr) ;

        NTypeLogger lgr ;

        keyabsorber::InstantKeyAbsorber ika ;

        while(vind::update_background()) {
            auto result = lgr.logging_state() ;
            auto parent_result = parent_lgr.logging_state() ;
            if(NTYPE_EMPTY(result) && NTYPE_EMPTY(parent_result)) {
                continue ;
            }
            if(NTYPE_HEAD_NUM(result)) {
                VirtualCmdLine::cout(std::to_string(lgr.get_head_num())) ;
                parent_lgr.remove_from_back(1) ;
                continue ;
            }

            auto parser_1 = parent_ff.find_parser_with_transition(lgr.latest(), caller_self_id) ;
            if(parser_1) {
                if(parser_1->get_func()->id() == caller_self_id) {
                    parser_1 = nullptr ;
                }
                else if(parser_1->is_accepted()) {
                    VirtualCmdLine::reset() ;
                    parser_1->get_func()->process(parent_lgr) ;
                    return false ;
                }
            }

            auto parser_2 = ff.find_parser_with_transition(
                    lgr.latest(),
                    caller_self_id,
                    Mode::EDI_VISUAL) ;
            if(parser_2) {
                if(parser_2->is_accepted() && parser_2->get_func()->is_for_moving_caret()) {
                    mode::set_global_mode(Mode::EDI_VISUAL, ModeFlags::VISUAL_LINE) ;
                    repeater::safe_for(parent_lgr.get_head_num(), [f = parser_2->get_func(), &lgr] {
                        f->process(lgr) ;
                    }) ;
                    return true ;
                }
            }
            else if(!parser_1) { //reject
                break ;
            }

            if((parser_1 && parser_1->is_rejected_with_ready()) \
                    || (parser_2 && parser_2->is_rejected_with_ready())) {
                ff.backward_parser_states(1, Mode::EDI_VISUAL) ;
                lgr.remove_from_back(1) ;
                parent_ff.backward_parser_states(1) ;
                parent_lgr.remove_from_back(1) ;
            }
        }
        return false ;
    }
}


namespace vind
{
    //YankWithMotion
    struct YankWithMotion::Impl {
        FuncFinder funcfinder_ ;
        FuncFinder parent_funcfinder_ ;

        explicit Impl()
        : funcfinder_(),
          parent_funcfinder_()
        {}

        void copy() const {
            YankHighlightText::sprocess() ;
            ToEdiNormal::sprocess(false) ;
        }
    } ;
    YankWithMotion::YankWithMotion()
    : BindedFuncCreator("yank_with_motion"),
      pimpl(std::make_unique<Impl>())
    {}
    YankWithMotion::~YankWithMotion() noexcept                 = default ;
    YankWithMotion::YankWithMotion(YankWithMotion&&)            = default ;
    YankWithMotion& YankWithMotion::operator=(YankWithMotion&&) = default ;
    void YankWithMotion::sprocess() const {
        if(select_by_motion(id(), pimpl->funcfinder_)) {
            pimpl->copy() ;
        }
    }
    void YankWithMotion::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            if(select_by_motion(id(), pimpl->funcfinder_,
                        pimpl->parent_funcfinder_, parent_lgr)) {
                pimpl->copy() ;
            }
        }
    }
    void YankWithMotion::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void YankWithMotion::reconstruct() {
        pimpl->funcfinder_.reconstruct_funcset() ;
        pimpl->parent_funcfinder_.reconstruct_funcset() ;
    }

    //DeleteWithMotion
    struct DeleteWithMotion::Impl {
        FuncFinder funcfinder_ ;
        FuncFinder parent_funcfinder_ ;

        explicit Impl()
        : funcfinder_(),
          parent_funcfinder_()
        {}

        void remove() const {
            DeleteHighlightText::sprocess() ;
        }
    } ;
    DeleteWithMotion::DeleteWithMotion()
    : BindedFuncCreator("delete_with_motion"),
      pimpl(std::make_unique<Impl>())
    {}
    DeleteWithMotion::~DeleteWithMotion() noexcept                   = default ;
    DeleteWithMotion::DeleteWithMotion(DeleteWithMotion&&)            = default ;
    DeleteWithMotion& DeleteWithMotion::operator=(DeleteWithMotion&&) = default ;

    void DeleteWithMotion::sprocess() const {
        if(select_by_motion(id(), pimpl->funcfinder_)) {
            pimpl->remove() ;
        }
    }
    void DeleteWithMotion::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            if(select_by_motion(id(), pimpl->funcfinder_,
                        pimpl->funcfinder_, parent_lgr)) {
                pimpl->remove() ;
            }
        }
    }
    void DeleteWithMotion::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void DeleteWithMotion::reconstruct() {
        pimpl->funcfinder_.reconstruct_funcset() ;
        pimpl->parent_funcfinder_.reconstruct_funcset() ;
    }


    //ChangeWithMotion
    struct ChangeWithMotion::Impl {
        FuncFinder funcfinder_ ;
        FuncFinder parent_funcfinder_ ;

        explicit Impl()
        : funcfinder_(),
          parent_funcfinder_()
        {}

        void remove_and_insert() {
            DeleteHighlightText::sprocess() ;
            ToInsert::sprocess(false) ;
        }
    } ;
    ChangeWithMotion::ChangeWithMotion()
    : BindedFuncCreator("change_with_motion"),
      pimpl(std::make_unique<Impl>())
    {}
    ChangeWithMotion::~ChangeWithMotion() noexcept                                 = default ;
    ChangeWithMotion::ChangeWithMotion(ChangeWithMotion&&)            = default ;
    ChangeWithMotion& ChangeWithMotion::operator=(ChangeWithMotion&&) = default ;
    void ChangeWithMotion::sprocess() const {
        if(select_by_motion(id(), pimpl->funcfinder_)) {
            pimpl->remove_and_insert() ;
        }
    }
    void ChangeWithMotion::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            if(select_by_motion(id(), pimpl->funcfinder_,
                        pimpl->parent_funcfinder_, parent_lgr)) {
                pimpl->remove_and_insert() ;
            }
        }
    }
    void ChangeWithMotion::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void ChangeWithMotion::reconstruct() {
        pimpl->funcfinder_.reconstruct_funcset() ;
        pimpl->parent_funcfinder_.reconstruct_funcset() ;
    }
}

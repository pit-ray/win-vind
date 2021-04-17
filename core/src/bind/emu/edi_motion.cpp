#include "bind/emu/edi_motion.hpp"

#include "bind/base/key_logger_base.hpp"
#include "bind/base/ntype_logger.hpp"
#include "bind/emu/edi_change_mode.hpp"
#include "bind/emu/edi_edit.hpp"
#include "bind/func_finder.hpp"
#include "entry.hpp"
#include "key/key_absorber.hpp"
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

            auto parser = ff.find_parser(lgr.latest(), caller_self_id, mode::Mode::EdiLineVisual) ;
            if(parser && parser->get_func()->is_for_moving_caret()) {
                if(parser->is_accepted()) {
                    mode::change_mode(mode::Mode::EdiLineVisual) ;
                    parser->get_func()->process(lgr) ;
                    return true ;
                }
                else if(parser->is_rejected_with_ready()) {
                    ff.undo_parsers(1) ;
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
    //and 'cc' are bindings EdiDeleteLinesAndStartInsert.
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

        const auto vmode = mode::Mode::EdiLineVisual ;

        ff.reset_parsers(vmode) ;
        parent_ff.reset_parsers() ;
        parent_ff.transition_parser_states_in_batch(parent_lgr) ;

        NTypeLogger lgr ;

        keyabsorber::InstantKeyAbsorber ika ;

        while(vind::update_background()) {
            auto result = lgr.logging_state() ;
            parent_lgr.logging_state() ;
            if(NTYPE_EMPTY(result)) {
                continue ;
            }
            if(NTYPE_HEAD_NUM(result)) {
                VirtualCmdLine::cout(std::to_string(lgr.get_head_num())) ;
                parent_lgr.remove_from_back(1) ;
                continue ;
            }

            auto parser_1 = parent_ff.find_parser(lgr.latest(), caller_self_id) ;
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

            auto parser_2 = ff.find_parser(lgr.latest(), caller_self_id, vmode) ;
            if(parser_2) {
                if(parser_2->is_accepted() && parser_2->get_func()->is_for_moving_caret()) {
                    mode::change_mode(vmode) ;
                    for(std::size_t i = 0 ; i < parent_lgr.get_head_num() ; i ++) {
                        parser_2->get_func()->process(lgr) ;
                    }
                    return true ;
                }
            }
            else if(!parser_1) { //reject
                break ;
            }

            if((parser_1 && parser_1->is_rejected_with_ready()) \
                    || (parser_2 && parser_2->is_rejected_with_ready())) {
                ff.undo_parsers(1, vmode) ;
                lgr.remove_from_back(1) ;
                parent_ff.undo_parsers(1) ;
                parent_lgr.remove_from_back(1) ;
            }
        }
        return false ;
    }
}


namespace vind
{
    //EdiCopyMotion
    struct EdiCopyMotion::Impl {
        FuncFinder funcfinder_ ;
        FuncFinder parent_funcfinder_ ;

        explicit Impl()
        : funcfinder_(),
          parent_funcfinder_()
        {}

        void copy() const {
            EdiCopyHighlightText::sprocess() ;
            Change2EdiNormal::sprocess(false) ;
        }
    } ;
    EdiCopyMotion::EdiCopyMotion()
    : BindedFuncCreator("edi_copy_motion"),
      pimpl(std::make_unique<Impl>())
    {}
    EdiCopyMotion::~EdiCopyMotion() noexcept                 = default ;
    EdiCopyMotion::EdiCopyMotion(EdiCopyMotion&&)            = default ;
    EdiCopyMotion& EdiCopyMotion::operator=(EdiCopyMotion&&) = default ;
    void EdiCopyMotion::sprocess() const {
        if(select_by_motion(id(), pimpl->funcfinder_)) {
            pimpl->copy() ;
        }
    }
    void EdiCopyMotion::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            if(select_by_motion(id(), pimpl->funcfinder_,
                        pimpl->parent_funcfinder_, parent_lgr)) {
                pimpl->copy() ;
            }
        }
    }
    void EdiCopyMotion::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EdiCopyMotion::load_config() {
        pimpl->funcfinder_.reconstruct_funcset() ;
        pimpl->parent_funcfinder_.reconstruct_funcset() ;
    }

    //EdiDeleteMotion
    struct EdiDeleteMotion::Impl {
        FuncFinder funcfinder_ ;
        FuncFinder parent_funcfinder_ ;

        explicit Impl()
        : funcfinder_(),
          parent_funcfinder_()
        {}

        void remove() const {
            EdiDeleteHighlightText::sprocess() ;
        }
    } ;
    EdiDeleteMotion::EdiDeleteMotion()
    : BindedFuncCreator("edi_delete_motion"),
      pimpl(std::make_unique<Impl>())
    {}
    EdiDeleteMotion::~EdiDeleteMotion() noexcept                   = default ;
    EdiDeleteMotion::EdiDeleteMotion(EdiDeleteMotion&&)            = default ;
    EdiDeleteMotion& EdiDeleteMotion::operator=(EdiDeleteMotion&&) = default ;

    void EdiDeleteMotion::sprocess() const {
        if(select_by_motion(id(), pimpl->funcfinder_)) {
            pimpl->remove() ;
        }
    }
    void EdiDeleteMotion::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            if(select_by_motion(id(), pimpl->funcfinder_,
                        pimpl->funcfinder_, parent_lgr)) {
                pimpl->remove() ;
            }
        }
    }
    void EdiDeleteMotion::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EdiDeleteMotion::load_config() {
        pimpl->funcfinder_.reconstruct_funcset() ;
        pimpl->parent_funcfinder_.reconstruct_funcset() ;
    }


    //EdiDeleteMotionAndStartInsert
    struct EdiDeleteMotionAndStartInsert::Impl {
        FuncFinder funcfinder_ ;
        FuncFinder parent_funcfinder_ ;

        explicit Impl()
        : funcfinder_(),
          parent_funcfinder_()
        {}

        void remove_and_insert() {
            EdiDeleteHighlightText::sprocess() ;
            Change2EdiInsert::sprocess(false) ;
        }
    } ;
    EdiDeleteMotionAndStartInsert::EdiDeleteMotionAndStartInsert()
    : BindedFuncCreator("edi_delete_motion_and_start_insert"),
      pimpl(std::make_unique<Impl>())
    {}
    EdiDeleteMotionAndStartInsert::~EdiDeleteMotionAndStartInsert() noexcept                                 = default ;
    EdiDeleteMotionAndStartInsert::EdiDeleteMotionAndStartInsert(EdiDeleteMotionAndStartInsert&&)            = default ;
    EdiDeleteMotionAndStartInsert& EdiDeleteMotionAndStartInsert::operator=(EdiDeleteMotionAndStartInsert&&) = default ;
    void EdiDeleteMotionAndStartInsert::sprocess() const {
        if(select_by_motion(id(), pimpl->funcfinder_)) {
            pimpl->remove_and_insert() ;
        }
    }
    void EdiDeleteMotionAndStartInsert::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            if(select_by_motion(id(), pimpl->funcfinder_,
                        pimpl->parent_funcfinder_, parent_lgr)) {
                pimpl->remove_and_insert() ;
            }
        }
    }
    void EdiDeleteMotionAndStartInsert::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EdiDeleteMotionAndStartInsert::load_config() {
        pimpl->funcfinder_.reconstruct_funcset() ;
        pimpl->parent_funcfinder_.reconstruct_funcset() ;
    }
}

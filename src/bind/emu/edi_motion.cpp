#include "edi_motion.hpp"

#include "bind/mode/change_mode.hpp"
#include "bind/safe_repeater.hpp"
#include "core/background.hpp"
#include "core/entry.hpp"
#include "core/func_finder.hpp"
#include "core/key_absorber.hpp"
#include "core/logpooler.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "edi_delete.hpp"
#include "edi_yank.hpp"
#include "movebase.hpp"
#include "opt/async_uia_cache_builder.hpp"
#include "opt/blockstyle_caret.hpp"
#include "opt/optionlist.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"

#include <memory>

#if defined(DEBUG)
#include <iostream>
#endif


namespace
{
    using namespace vind ;
    using namespace vind::bind ;
    bool select_by_motion(
            std::size_t caller_self_id,
            core::Background& bg,
            core::FuncFinder& ff) {
        core::NTypeLogger lgr ;
        core::InstantKeyAbsorber ika ;

        while(true) {
            bg.update() ;

            auto log = core::LogPooler::get_instance().pop_log() ;
            auto result = lgr.logging_state(log) ;
            if(NTYPE_EMPTY(result)) {
                continue ;
            }
            if(NTYPE_HEAD_NUM(result)) {
                opt::VCmdLine::print(
                        opt::StaticMessage(std::to_string(lgr.get_head_num()))) ;
                continue ;
            }

            using core::Mode ;
            using core::ModeFlags ;
            auto parser = ff.find_parser_with_transition(
                    lgr.latest(),
                    caller_self_id,
                    Mode::EDI_VISUAL) ;
            if(parser && std::dynamic_pointer_cast<MoveBase>(parser->get_func())) {
                if(parser->is_accepted()) {
                    core::set_global_mode(Mode::EDI_VISUAL, ModeFlags::VISUAL_LINE) ;
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
            core::Background& bg,
            core::FuncFinder& ff,
            core::FuncFinder&parent_ff,
            core::NTypeLogger& parent_lgr) {

        using core::Mode ;
        using core::ModeFlags ;
        ff.reset_parser_states(Mode::EDI_VISUAL) ;
        parent_ff.reset_parser_states() ;
        parent_ff.transition_parser_states_in_batch(parent_lgr) ;

        core::NTypeLogger lgr ;

        core::InstantKeyAbsorber ika ;

        while(true) {
            bg.update() ;

            auto log = core::LogPooler::get_instance().pop_log() ;
            auto result = lgr.logging_state(log) ;
            auto parent_result = parent_lgr.logging_state() ;
            if(NTYPE_EMPTY(result) && NTYPE_EMPTY(parent_result)) {
                continue ;
            }
            if(NTYPE_HEAD_NUM(result)) {
                opt::VCmdLine::print(
                        opt::StaticMessage(std::to_string(lgr.get_head_num()))) ;
                parent_lgr.remove_from_back(1) ;
                continue ;
            }

            auto parser_1 = parent_ff.find_parser_with_transition(lgr.latest(), caller_self_id) ;
            if(parser_1) {
                if(parser_1->get_func()->id() == caller_self_id) {
                    parser_1 = nullptr ;
                }
                else if(parser_1->is_accepted()) {
                    opt::VCmdLine::reset() ;
                    parser_1->get_func()->process(parent_lgr) ;
                    return false ;
                }
            }

            auto parser_2 = ff.find_parser_with_transition(
                    lgr.latest(),
                    caller_self_id,
                    Mode::EDI_VISUAL) ;
            if(parser_2) {
                if(parser_2->is_accepted() && std::dynamic_pointer_cast<MoveBase>(parser_2->get_func())) {
                    core::set_global_mode(Mode::EDI_VISUAL, ModeFlags::VISUAL_LINE) ;
                    safe_for(parent_lgr.get_head_num(), [f = parser_2->get_func(), &lgr] {
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
    namespace bind
    {
        //YankWithMotion
        struct YankWithMotion::Impl {
            core::FuncFinder funcfinder_ ;
            core::FuncFinder parent_funcfinder_ ;
            core::Background bg_ ;

            explicit Impl()
            : funcfinder_(),
              parent_funcfinder_(),
              bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::BlockStyleCaret().name(),
                    opt::VCmdLine().name()
              ))
            {}

            void copy() const {
                YankHighlightText::sprocess() ;
                ToEdiNormal::sprocess(false) ;
            }
        } ;
        YankWithMotion::YankWithMotion()
        : BindedFuncVoid("yank_with_motion"),
          pimpl(std::make_unique<Impl>())
        {}
        YankWithMotion::~YankWithMotion() noexcept = default ;
        YankWithMotion::YankWithMotion(YankWithMotion&&) = default ;
        YankWithMotion& YankWithMotion::operator=(YankWithMotion&&) = default ;
        void YankWithMotion::sprocess() const {
            if(select_by_motion(
                    id(),
                    pimpl->bg_,
                    pimpl->funcfinder_)) {
                pimpl->copy() ;
            }
        }
        void YankWithMotion::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                if(select_by_motion(
                        id(),
                        pimpl->bg_,
                        pimpl->funcfinder_,
                        pimpl->parent_funcfinder_,
                        parent_lgr)) {
                    pimpl->copy() ;
                }
            }
        }
        void YankWithMotion::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess() ;
        }
        void YankWithMotion::reconstruct() {
            pimpl->funcfinder_.reconstruct() ;
            pimpl->parent_funcfinder_.reconstruct() ;
        }

        //DeleteWithMotion
        struct DeleteWithMotion::Impl {
            core::FuncFinder funcfinder_ ;
            core::FuncFinder parent_funcfinder_ ;
            core::Background bg_ ;

            explicit Impl()
            : funcfinder_(),
              parent_funcfinder_(),
              bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::BlockStyleCaret().name(),
                    opt::VCmdLine().name()
              ))
            {}

            void remove() const {
                DeleteHighlightText::sprocess() ;
            }
        } ;
        DeleteWithMotion::DeleteWithMotion()
        : BindedFuncVoid("delete_with_motion"),
          pimpl(std::make_unique<Impl>())
        {}
        DeleteWithMotion::~DeleteWithMotion() noexcept = default ;
        DeleteWithMotion::DeleteWithMotion(DeleteWithMotion&&) = default ;
        DeleteWithMotion& DeleteWithMotion::operator=(DeleteWithMotion&&) = default ;

        void DeleteWithMotion::sprocess() const {
            if(select_by_motion(
                    id(),
                    pimpl->bg_,
                    pimpl->funcfinder_)) {
                pimpl->remove() ;
            }
        }
        void DeleteWithMotion::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                if(select_by_motion(
                        id(),
                        pimpl->bg_,
                        pimpl->funcfinder_,
                        pimpl->funcfinder_,
                        parent_lgr)) {
                    pimpl->remove() ;
                }
            }
        }
        void DeleteWithMotion::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess() ;
        }
        void DeleteWithMotion::reconstruct() {
            pimpl->funcfinder_.reconstruct() ;
            pimpl->parent_funcfinder_.reconstruct() ;
        }


        //ChangeWithMotion
        struct ChangeWithMotion::Impl {
            core::FuncFinder funcfinder_ ;
            core::FuncFinder parent_funcfinder_ ;
            core::Background bg_ ;

            explicit Impl()
            : funcfinder_(),
              parent_funcfinder_(),
              bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::BlockStyleCaret().name(),
                    opt::VCmdLine().name()
              ))
            {}

            void remove_and_insert() {
                DeleteHighlightText::sprocess() ;
                ToInsert::sprocess(false) ;
            }
        } ;
        ChangeWithMotion::ChangeWithMotion()
        : BindedFuncVoid("change_with_motion"),
          pimpl(std::make_unique<Impl>())
        {}
        ChangeWithMotion::~ChangeWithMotion() noexcept = default ;
        ChangeWithMotion::ChangeWithMotion(ChangeWithMotion&&) = default ;
        ChangeWithMotion& ChangeWithMotion::operator=(ChangeWithMotion&&) = default ;
        void ChangeWithMotion::sprocess() const {
            if(select_by_motion(
                    id(),
                    pimpl->bg_,
                    pimpl->funcfinder_)) {
                pimpl->remove_and_insert() ;
            }
        }
        void ChangeWithMotion::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                if(select_by_motion(
                        id(),
                        pimpl->bg_,
                        pimpl->funcfinder_,
                        pimpl->parent_funcfinder_,
                        parent_lgr)) {
                    pimpl->remove_and_insert() ;
                }
            }
        }
        void ChangeWithMotion::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess() ;
        }
        void ChangeWithMotion::reconstruct() {
            pimpl->funcfinder_.reconstruct() ;
            pimpl->parent_funcfinder_.reconstruct() ;
        }
    }
}

#include "motion.hpp"

#include "changetext.hpp"
#include "deltext.hpp"
#include "motionids.hpp"
#include "yanktext.hpp"

#include "bind/mode/change_mode.hpp"
#include "bind/saferepeat.hpp"
#include "core/background.hpp"
#include "core/entry.hpp"
#include "core/funcfinder.hpp"
#include "core/inputgate.hpp"
#include "core/inputhub.hpp"
#include "core/mode.hpp"
#include "opt/blockstylecaret.hpp"
#include "opt/optionlist.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"

#include <memory>


namespace vind
{
    namespace bind
    {
        //YankWithMotion
        struct YankWithMotion::Impl {
            core::Background bg_ ;
            explicit Impl()
            : bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::BlockStyleCaret().name(),
                    opt::VCmdLine().name()
              ))
            {}
        } ;
        YankWithMotion::YankWithMotion()
        : BindedFuncVoid("yank_with_motion"),
          pimpl(std::make_unique<Impl>())
        {}
        YankWithMotion::~YankWithMotion() noexcept = default ;
        YankWithMotion::YankWithMotion(YankWithMotion&&) = default ;
        YankWithMotion& YankWithMotion::operator=(YankWithMotion&&) = default ;

        void YankWithMotion::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            using core::Mode ;
            using core::ModeFlags ;
            auto& ihub = core::InputHub::get_instance() ;
            core::InstantKeyAbsorber ika ;
            while(true) {
                pimpl->bg_.update() ;

                std::vector<core::CmdUnit::SPtr> inputs ;
                std::vector<std::uint16_t> counts ;
                if(!ihub.fetch_inputs(inputs, counts)) {
                    continue ;
                }

                for(int i = 0 ; i < inputs.size() ; i ++) {
                    std::uint16_t correct_count = count * counts[i] ;
                    if(inputs[i]->id() == id()) {
                        bind::YankLine::sprocess(correct_count, "") ;
                        return ;
                    }
                    if(MotionIds::get_instance().is_motion(inputs[i]->id())) {
                        core::set_global_mode(Mode::EDI_VISUAL, ModeFlags::VISUAL_LINE) ;
                        inputs[i]->execute(correct_count, "") ;
                        YankHighlightText::sprocess(1, "") ;
                        ToEdiNormal::sprocess(1, "", false) ;
                        return ;
                    }
                }
                break ;
            }
        }

        //DeleteWithMotion
        struct DeleteWithMotion::Impl {
            core::Background bg_ ;

            explicit Impl()
            : bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::BlockStyleCaret().name(),
                    opt::VCmdLine().name()
              ))
            {}
        } ;
        DeleteWithMotion::DeleteWithMotion()
        : BindedFuncVoid("delete_with_motion"),
          pimpl(std::make_unique<Impl>())
        {}
        DeleteWithMotion::~DeleteWithMotion() noexcept = default ;
        DeleteWithMotion::DeleteWithMotion(DeleteWithMotion&&) = default ;
        DeleteWithMotion& DeleteWithMotion::operator=(DeleteWithMotion&&) = default ;

        void DeleteWithMotion::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            using core::Mode ;
            using core::ModeFlags ;
            auto& ihub = core::InputHub::get_instance() ;
            core::InstantKeyAbsorber ika ;
            while(true) {
                pimpl->bg_.update() ;

                std::vector<core::CmdUnit::SPtr> inputs ;
                std::vector<std::uint16_t> counts ;
                if(!ihub.fetch_inputs(inputs, counts)) {
                    continue ;
                }

                for(int i = 0 ; i < inputs.size() ; i ++) {
                    std::uint16_t correct_count = count * counts[i] ;
                    if(inputs[i]->id() == id()) {
                        bind::DeleteLine::sprocess(correct_count, "") ;
                        return ;
                    }
                    if(MotionIds::get_instance().is_motion(inputs[i]->id())) {
                        core::set_global_mode(Mode::EDI_VISUAL, ModeFlags::VISUAL_LINE) ;
                        inputs[i]->execute(correct_count, "") ;
                        DeleteHighlightText::sprocess(1, "") ;
                        return ;
                    }
                }
                break ;
            }
        }

        //ChangeWithMotion
        struct ChangeWithMotion::Impl {
            core::Background bg_ ;

            explicit Impl()
            : bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::BlockStyleCaret().name(),
                    opt::VCmdLine().name()
              ))
            {}
        } ;
        ChangeWithMotion::ChangeWithMotion()
        : BindedFuncVoid("change_with_motion"),
          pimpl(std::make_unique<Impl>())
        {}
        ChangeWithMotion::~ChangeWithMotion() noexcept = default ;
        ChangeWithMotion::ChangeWithMotion(ChangeWithMotion&&) = default ;
        ChangeWithMotion& ChangeWithMotion::operator=(ChangeWithMotion&&) = default ;
        void ChangeWithMotion::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            using core::Mode ;
            using core::ModeFlags ;
            auto& ihub = core::InputHub::get_instance() ;
            core::InstantKeyAbsorber ika ;
            while(true) {
                pimpl->bg_.update() ;

                std::vector<core::CmdUnit::SPtr> inputs ;
                std::vector<std::uint16_t> counts ;
                if(!ihub.fetch_inputs(inputs, counts)) {
                    continue ;
                }

                for(int i = 0 ; i < inputs.size() ; i ++) {
                    std::uint16_t correct_count = count * counts[i] ;
                    if(inputs[i]->id() == id()) {
                        bind::ChangeLine::sprocess(correct_count, "") ;
                        return ;
                    }
                    if(MotionIds::get_instance().is_motion(inputs[i]->id())) {
                        core::set_global_mode(Mode::EDI_VISUAL, ModeFlags::VISUAL_LINE) ;
                        inputs[i]->execute(correct_count, "") ;
                        DeleteHighlightText::sprocess(1, "") ;
                        ToInsert::sprocess(1, "", false) ;
                        return ;
                    }
                }
                break ;
            }
        }
    }
}

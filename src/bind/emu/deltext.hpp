#ifndef _EDI_DELETE_HPP
#define _EDI_DELETE_HPP

#include "bind/bindedfunc.hpp"
#include "bind/saferepeat.hpp"
#include "changebase.hpp"
#include "core/inputgate.hpp"
#include "core/keycodedef.hpp"
#include "core/ntypelogger.hpp"
#include "textreg.hpp"
#include "textutil.hpp"
#include "util/keystroke_repeater.hpp"


namespace vind
{
    namespace bind
    {
        //Delete
        struct DeleteHighlightText : public BindedFuncVoid<DeleteHighlightText> {
            explicit DeleteHighlightText() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        class DeleteLine : public ChangeBaseCreator<DeleteLine> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit DeleteLine() ;
            virtual ~DeleteLine() noexcept ;

            DeleteLine(DeleteLine&&) ;
            DeleteLine& operator=(DeleteLine&&) ;
            DeleteLine(const DeleteLine&)            = delete ;
            DeleteLine& operator=(const DeleteLine&) = delete ;
        } ;

        template <typename Derived>
        class DeleteLineUntilEOLBase : public ChangeBaseCreator<Derived> {
        private:
            util::KeyStrokeRepeater ksr_ ;

        public:
            template <typename String>
            explicit DeleteLineUntilEOLBase(String&& func_name)
            : ChangeBaseCreator<Derived>(std::forward<String>(func_name)),
              ksr_()
            {}

            void sprocess(unsigned int count=1) {
                auto& igate = core::InputGate::get_instance() ;

                //delete N - 1 lines under the current line
                safe_for(count - 1, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
                }) ;

                if(!select_line_until_EOL()) {
                    clear_clipboard_with_null() ;
                }
                else {
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                    set_register_type(RegType::Chars) ;
                }
            }

            void sprocess(core::NTypeLogger& parent_lgr) {
                if(!parent_lgr.is_long_pressing()) {
                    sprocess(parent_lgr.get_head_num()) ;
                    ksr_.reset() ;
                }
                else if(ksr_.is_passed()) {
                    sprocess(1) ;
                }
            }

            void sprocess(const core::CharLogger&) {
                sprocess(1) ;
            }
        } ;


        struct DeleteLineUntilEOL : public DeleteLineUntilEOLBase<DeleteLineUntilEOL> {
            explicit DeleteLineUntilEOL() ;
        } ;

        class DeleteAfter : public ChangeBaseCreator<DeleteAfter> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit DeleteAfter() ;
            virtual ~DeleteAfter() noexcept ;

            DeleteAfter(DeleteAfter&&) ;
            DeleteAfter& operator=(DeleteAfter&&) ;
            DeleteAfter(const DeleteAfter&)            = delete ;
            DeleteAfter& operator=(const DeleteAfter&) = delete ;
        } ;

        class DeleteBefore : public ChangeBaseCreator<DeleteBefore> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit DeleteBefore() ;
            virtual ~DeleteBefore() noexcept ;

            DeleteBefore(DeleteBefore&&) ;
            DeleteBefore& operator=(DeleteBefore&&) ;
            DeleteBefore(const DeleteBefore&)            = delete ;
            DeleteBefore& operator=(const DeleteBefore&) = delete ;
        } ;
    }
}

#endif

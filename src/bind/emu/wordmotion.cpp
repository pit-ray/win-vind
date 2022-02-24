#include "wordmotion.hpp"

#include "movebase.hpp"

#include "bind/saferepeat.hpp"
#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "core/settable.hpp"
#include "smartclipboard.hpp"
#include "textanalyze.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/string.hpp"
#include "util/unicode.hpp"
#include "util/winwrap.hpp"

#include <string>


namespace
{
    using namespace vind ;
    using namespace vind::bind ;

    inline auto break_unicode(const std::string& utf8str) {
        auto& settable = core::SetTable::get_instance() ;
        auto mode = settable.get("charbreak").get<std::string>() ;

        auto low_mode = util::A2a(mode) ;

        if(low_mode == "grapheme") {
            return util::break_unicode_grapheme(utf8str) ;
        }
        if(low_mode == "codepoint") {
            return util::break_unicode_codepoint(utf8str) ;
        }

        PRINT_ERROR("Not supported value: " + mode) ;
        return util::break_unicode_grapheme(utf8str) ;
    }

    inline auto cls(char32_t cp, bool bigword) {
        auto type = util::classify_codepoint(cp) ;

        if(bigword) {
            using util::CharType ;
            if(type != CharType::WHITE_SPACE && \
               type != CharType::CARRIAGE_RETURN) {
                return CharType::OTHERWISE ;
            }
        }

        return type ;
    }

    /**
     * TODO: Currently, it does not capture more than
     *       two lines due to efficiency issues.
     */
    auto capture_fwd(unsigned int UNUSED(count)) {
        auto& igate = core::InputGate::get_instance() ;
        auto res = get_selected_text([&igate] {
            igate.pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
            Sleep(30) ;
            igate.pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        }) ;

        auto str = break_unicode(res.str) ;
        if(str.size() > 1) {
            igate.pushup(KEYCODE_LEFT) ;
        }
        return str ;
    }

    auto capture_bck(unsigned int UNUSED(count)) {
        auto& igate = core::InputGate::get_instance() ;
        auto res = get_selected_text([&igate] {
            igate.pushup(KEYCODE_LSHIFT, KEYCODE_UP) ;
            Sleep(30) ;
            igate.pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        }) ;

        auto str = break_unicode(res.str) ;
        if(str.size() > 1) {
            igate.pushup(KEYCODE_RIGHT) ;
        }
        return str ;
    }

    bool inc_caret_if_single(const std::u32string& str) {
        auto size = str.size() ;
        if(size == 0) {
            return true ;
        }
        if(size == 1) {
            core::InputGate::get_instance().pushup(KEYCODE_RIGHT) ;
            return true ;
        }
        return false ;
    }

    bool inc_caret(
            std::u32string::iterator& itr,
            const std::u32string::iterator& end) {
        if(itr == end) {
            return false ;
        }
        core::InputGate::get_instance().pushup(KEYCODE_RIGHT) ;
        return ++ itr != end ;
    }


    bool dec_caret_if_single(const std::u32string& str) {
        auto size = str.size() ;
        if(size == 0) {
            return true ;
        }
        if(size == 1) {
            core::InputGate::get_instance().pushup(KEYCODE_LEFT) ;
            return true ;
        }
        return false ;
    }

    bool dec_caret(
            std::u32string::reverse_iterator& itr,
            const std::u32string::reverse_iterator& end) {
        if(itr == end) {
            return false ;
        }
        core::InputGate::get_instance().pushup(KEYCODE_LEFT) ;
        return ++ itr != end ;
    }


    void fwd_word(unsigned int count, bool bigword) {
        auto str = capture_fwd(count) ;
        if(inc_caret_if_single(str))
            return ;

        auto itr = str.begin() ;
        for(decltype(count) i = 0 ; i < count ; i ++) {
            auto pre_type = cls(*itr, bigword) ;
            auto type = pre_type ;
            do {
                if(!inc_caret(itr, str.end()))
                    return ;
                type = cls(*itr, bigword) ;
            } while(pre_type == type) ;

            while(type == util::CharType::WHITE_SPACE || \
                  type == util::CharType::CARRIAGE_RETURN) {
                if(!inc_caret(itr, str.end()))
                    return ;
                type = cls(*itr, bigword) ;
            }
        }
    }

    void bck_word(unsigned int count, bool bigword) {
        auto str = capture_bck(count) ;
        if(dec_caret_if_single(str))
            return ;

        auto itr = str.rbegin() ;
        /**
         * NOTE: Such as Office Word will insert \r after the
         * copied text, so should ignore it in the backword.
         */
        auto type = cls(*itr, bigword) ;
        while(type == util::CharType::CARRIAGE_RETURN) {
            if(++ itr == str.rend())
                return ;
            type = cls(*itr, bigword) ;
        }

        for(decltype(count) i = 0 ; i < count ; i ++) {
            while(type == util::CharType::WHITE_SPACE || \
                  type == util::CharType::CARRIAGE_RETURN) {
                if(!dec_caret(itr, str.rend()))
                    return ;
                type = cls(*itr, bigword) ;
            }

            auto end_type = type ;
            do {
                if(!dec_caret(itr, str.rend()))
                    return ;
                type = cls(*itr, bigword) ;
            } while(end_type == type) ;
        }
    }

    void end_word(unsigned int count, bool bigword) {
        auto str = capture_fwd(count) ;
        if(inc_caret_if_single(str))
            return ;

        auto itr = str.begin() ;
        util::CharType type ;
        for(decltype(count) i = 0 ; i < count ; i ++) {
            do {
                if(!inc_caret(itr, str.end()))
                    return ;
                type = cls(*itr, bigword) ;
            } while(type == util::CharType::WHITE_SPACE || \
                    type == util::CharType::CARRIAGE_RETURN) ;

            auto head_type = type ;
            if(++ itr == str.end())
                return ;
            type = cls(*itr, bigword) ;

            while(type == head_type) {
                if(!inc_caret(itr, str.end()))
                    return ;
                type = cls(*itr, bigword) ;
            }
            itr -- ;
        }
    }

    void bckend_word(unsigned int count, bool bigword) {
        auto str = capture_bck(count) ;
        if(dec_caret_if_single(str))
            return ;

        auto itr = str.rbegin() ;
        /**
         * NOTE: Such as Office Word will insert \r after the
         * copied text, so should ignore it in the backword.
         */
        auto type = cls(*itr, bigword) ;
        while(type == util::CharType::CARRIAGE_RETURN) {
            if(++ itr == str.rend())
                return ;
            type = cls(*itr, bigword) ;
        }

        auto pre_type = type ;
        for(decltype(count) i = 0 ; i < count ; i ++) {
            do {
                if(!dec_caret(itr, str.rend()))
                    return ;
                type = cls(*itr, bigword) ;
            } while(pre_type == type) ;

            while(type == util::CharType::WHITE_SPACE || \
                  type == util::CharType::CARRIAGE_RETURN) {
                if(!dec_caret(itr, str.rend()))
                    return ;
                type = cls(*itr, bigword) ;
            }

            if(!dec_caret(itr, str.rend()))
                return ;

            pre_type = type ;
        }
    }
}


namespace vind
{
    namespace bind
    {
        MoveFwdWord::MoveFwdWord()
        : MoveBaseCreator("move_fwd_word")
        {}
        void MoveFwdWord::sprocess(unsigned int count) const {
            fwd_word(count, false) ;
        }
        void MoveFwdWord::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MoveFwdWord::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }

        MoveFwdBigWord::MoveFwdBigWord()
        : MoveBaseCreator("move_fwd_bigword")
        {}
        void MoveFwdBigWord::sprocess(unsigned int count) const {
            fwd_word(count, true) ;
        }
        void MoveFwdBigWord::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MoveFwdBigWord::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }

        MoveBckWord::MoveBckWord()
        : MoveBaseCreator("move_bck_word")
        {}
        void MoveBckWord::sprocess(unsigned int count) const {
            bck_word(count, false) ;
        }
        void MoveBckWord::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MoveBckWord::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }

        MoveBckBigWord::MoveBckBigWord()
        : MoveBaseCreator("move_bck_bigword")
        {}
        void MoveBckBigWord::sprocess(unsigned int count) const {
            bck_word(count, true) ;
        }
        void MoveBckBigWord::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MoveBckBigWord::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }

        MoveEndWord::MoveEndWord()
        : MoveBaseCreator("move_end_word")
        {}
        void MoveEndWord::sprocess(unsigned int count) const {
            end_word(count, false) ;
        }
        void MoveEndWord::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MoveEndWord::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }

        MoveEndBigWord::MoveEndBigWord()
        : MoveBaseCreator("move_end_bigword")
        {}
        void MoveEndBigWord::sprocess(unsigned int count) const {
            end_word(count, true) ;
        }
        void MoveEndBigWord::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MoveEndBigWord::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }

        MoveBckEndWord::MoveBckEndWord()
        : MoveBaseCreator("move_bckend_word")
        {}
        void MoveBckEndWord::sprocess(unsigned int count) const {
            bckend_word(count, false) ;
        }
        void MoveBckEndWord::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MoveBckEndWord::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }

        MoveBckEndBigWord::MoveBckEndBigWord()
        : MoveBaseCreator("move_bckend_bigword")
        {}
        void MoveBckEndBigWord::sprocess(unsigned int count) const {
            bckend_word(count, true) ;
        }
        void MoveBckEndBigWord::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MoveBckEndBigWord::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        // For visual mode.
        struct MoveFwdWordSimple::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        MoveFwdWordSimple::MoveFwdWordSimple()
        : MoveBaseCreator("move_fwd_word_simple"),
          pimpl(std::make_unique<Impl>())
        {}

        MoveFwdWordSimple::~MoveFwdWordSimple() noexcept = default ;
        MoveFwdWordSimple::MoveFwdWordSimple(MoveFwdWordSimple&&) = default ;
        MoveFwdWordSimple& MoveFwdWordSimple::operator=(MoveFwdWordSimple&&) = default ;

        void MoveFwdWordSimple::sprocess(unsigned int count) const {
            auto& igate = core::InputGate::get_instance() ;

            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_RIGHT) ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_RIGHT) ;
                }) ;
            }
        }
        void MoveFwdWordSimple::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void MoveFwdWordSimple::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        struct MoveBckWordSimple::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        MoveBckWordSimple::MoveBckWordSimple()
        : MoveBaseCreator("move_bck_word_simple"),
          pimpl(std::make_unique<Impl>())
        {}

        MoveBckWordSimple::~MoveBckWordSimple() noexcept = default ;
        MoveBckWordSimple::MoveBckWordSimple(MoveBckWordSimple&&) = default ;
        MoveBckWordSimple& MoveBckWordSimple::operator=(MoveBckWordSimple&&) = default ;

        void MoveBckWordSimple::sprocess(unsigned int count) const {
            auto& igate = core::InputGate::get_instance() ;

            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_LEFT) ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_LEFT) ;
                }) ;
            }
        }
        void MoveBckWordSimple::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void MoveBckWordSimple::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }
    }
}

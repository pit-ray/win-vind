#include "wordmotion.hpp"

#include "motionids.hpp"

#include "bind/saferepeat.hpp"
#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "core/settable.hpp"
#include "smartclipboard.hpp"
#include "textutil.hpp"
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
            igate.pushup(KEYCODE_LSHIFT, KEYCODE_DOWN, KEYCODE_END) ;
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
            igate.pushup(KEYCODE_LSHIFT, KEYCODE_UP, KEYCODE_HOME) ;
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
        : BindedFuncVoid("move_fwd_word")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveFwdWord::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            fwd_word(count, false) ;
        }

        MoveFwdBigWord::MoveFwdBigWord()
        : BindedFuncVoid("move_fwd_bigword")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveFwdBigWord::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            fwd_word(count, true) ;
        }

        MoveBckWord::MoveBckWord()
        : BindedFuncVoid("move_bck_word")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveBckWord::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            bck_word(count, false) ;
        }

        MoveBckBigWord::MoveBckBigWord()
        : BindedFuncVoid("move_bck_bigword")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveBckBigWord::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            bck_word(count, true) ;
        }

        MoveEndWord::MoveEndWord()
        : BindedFuncVoid("move_end_word")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveEndWord::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            end_word(count, false) ;
        }

        MoveEndBigWord::MoveEndBigWord()
        : BindedFuncVoid("move_end_bigword")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveEndBigWord::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            end_word(count, true) ;
        }

        MoveBckEndWord::MoveBckEndWord()
        : BindedFuncVoid("move_bckend_word")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveBckEndWord::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            bckend_word(count, false) ;
        }

        MoveBckEndBigWord::MoveBckEndBigWord()
        : BindedFuncVoid("move_bckend_bigword")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveBckEndBigWord::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            bckend_word(count, true) ;
        }

        // For visual mode.
        MoveFwdWordSimple::MoveFwdWordSimple()
        : BindedFuncVoid("move_fwd_word_simple")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveFwdWordSimple::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
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

        MoveBckWordSimple::MoveBckWordSimple()
        : BindedFuncVoid("move_bck_word_simple")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveBckWordSimple::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
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
    }
}

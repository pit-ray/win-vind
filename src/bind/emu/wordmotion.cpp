#include "wordmotion.hpp"

#include "movebase.hpp"

#include "bind/saferepeat.hpp"
#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "smartclipboard.hpp"
#include "textanalyze.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/unicode.hpp"
#include "util/winwrap.hpp"

#include <string>


namespace
{
    using namespace vind ;
    using namespace vind::bind ;

    inline auto break_unicode(const std::string& utf8str) {
        auto str = util::break_unicode_grapheme(utf8str) ;
        //auto str = util::break_unicode_codepoint(res.str) ;
        return str ;
    }

    /**
     * TODO: Currently, it does not capture more than
     *       two lines due to efficiency issues.
     */
    auto capture_forward(unsigned int UNUSED(repeat_num)) {
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
}


namespace vind
{
    namespace bind
    {
        MoveFwdWord::MoveFwdWord()
        : MoveBaseCreator("move_fwd_word")
        {}
        void MoveFwdWord::sprocess(unsigned int repeat_num) const {
            auto str = capture_forward(repeat_num) ;
            if(inc_caret_if_single(str))
                return ;

            auto itr = str.begin() ;
            for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
                auto pre_type = util::classify_codepoint(*itr) ;
                auto type = pre_type ;
                do {
                    if(!inc_caret(itr, str.end()))
                        return ;
                    type = util::classify_codepoint(*itr) ;
                } while(pre_type == type) ;

                while(type == util::CharType::WHITE_SPACE) {
                    if(!inc_caret(itr, str.end()))
                        return ;
                    type = util::classify_codepoint(*itr) ;
                }

                if(type == util::CharType::CARRIAGE_RETURN) {
                    if(!inc_caret(itr, str.end()))
                        return ;
                }
            }
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
        void MoveFwdBigWord::sprocess(unsigned int repeat_num) const {
            auto str = capture_forward(repeat_num) ;
            if(inc_caret_if_single(str))
                return ;

            auto itr = str.begin() ;
            for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
                auto type = util::classify_codepoint(*itr) ;
                while(type != util::CharType::WHITE_SPACE) {
                    if(!inc_caret(itr, str.end()))
                        return ;
                    type = util::classify_codepoint(*itr) ;
                }

                while(type == util::CharType::WHITE_SPACE) {
                    if(!inc_caret(itr, str.end()))
                        return ;
                    type = util::classify_codepoint(*itr) ;
                }

                if(type == util::CharType::CARRIAGE_RETURN) {
                    if(!inc_caret(itr, str.end()))
                        return ;
                }
            }
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
        void MoveBckWord::sprocess(unsigned int repeat_num) const {
            std::cout << repeat_num << std::endl ;
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
        void MoveBckBigWord::sprocess(unsigned int repeat_num) const {
            std::cout << repeat_num << std::endl ;
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
        void MoveEndWord::sprocess(unsigned int repeat_num) const {
            auto str = capture_forward(repeat_num) ;
            if(inc_caret_if_single(str))
                return ;

            auto itr = str.begin() ;
            util::CharType type ;
            for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
                do {
                    if(!inc_caret(itr, str.end()))
                        return ;
                    type = util::classify_codepoint(*itr) ;
                } while(type == util::CharType::WHITE_SPACE || \
                        type == util::CharType::CARRIAGE_RETURN) ;

                auto head_type = type ;
                if(++ itr == str.end())
                    return ;
                type = util::classify_codepoint(*itr) ;

                while(type == head_type) {
                    if(!inc_caret(itr, str.end()))
                        return ;
                    type = util::classify_codepoint(*itr) ;
                }
                itr -- ;
            }
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
        void MoveEndBigWord::sprocess(unsigned int repeat_num) const {
            std::cout << repeat_num << std::endl ;
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
        void MoveBckEndWord::sprocess(unsigned int repeat_num) const {
            std::cout << repeat_num << std::endl ;
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
        void MoveBckEndBigWord::sprocess(unsigned int repeat_num) const {
            std::cout << repeat_num << std::endl ;
        }
        void MoveBckEndBigWord::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MoveBckEndBigWord::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }
    }
}

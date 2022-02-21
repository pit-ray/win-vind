#include "wordmotion.hpp"

#include "movebase.hpp"

#include "core/ntypelogger.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/unicode.hpp"

#include <string>


namespace vind
{
    namespace bind
    {
        MoveFwdWord::MoveFwdWord()
        : MoveBaseCreator("move_fwd_word")
        {}
        void MoveFwdWord::sprocess(unsigned int repeat_num) const {
            std::cout << repeat_num << std::endl ;
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
            std::cout << repeat_num << std::endl ;
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
            std::cout << repeat_num << std::endl ;
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

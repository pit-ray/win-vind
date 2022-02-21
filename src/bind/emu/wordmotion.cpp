#include "wordmotion.hpp"

#include "movebase.hpp"

#include "util/unicode.hpp"

#include <string>


namespace vind
{
    namespace bind
    {
        struct MoveFwdWord::Impl {

        } ;

        MoveFwdWord::MoveFwdWord()
        : MoveBaseCreator("move_fwd_word"),
          pimpl(std::make_unique<Impl>())
        {}

        MoveFwdWord::~MoveFwdWord() noexcept = default ;
        MoveFwdWord::MoveFwdWord(MoveFwdWord&&) = default ;
        MoveFwdWord& MoveFwdWord::operator=(MoveFwdWord&&) = default ;

        void MoveFwdWord::sprocess(unsigned int repeat_num) const {

        }

        void MoveFwdWord::sprocess(core::NTypeLogger& parent_lgr) const {

        }

        void MoveFwdWord::sprocess(const core::CharLogger& parent_lgr) const {

        }
    }
}

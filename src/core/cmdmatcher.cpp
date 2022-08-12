#include "cmdmatcher.hpp"

#include "keycode.hpp"

#include "util/def.hpp"

#include <stack>
#include <vector>


namespace
{
    enum State : unsigned char {
        ACCEPTED        = 0b0000'0001,
        REJECTED        = 0b0000'0010,
        MATCHING        = 0b0000'0100,
        ANY_ACCEPTED    = 0b0001'0001,
        ANYNUM_MATCHING = 0b0010'0100,
    } ;
}

namespace vind
{
    namespace core
    {
        struct CmdMatcher::Impl {
            std::vector<CmdUnit::SPtr> cmd_ ;
            std::stack<State, std::vector<State>> states_ ;
            std::stack<int, std::vector<int>> heads_ ;

            template <typename T>
            Impl(T&& cmd)
            : cmd_(std::forward<T>(cmd)),
              states_(),
              heads_()
            {}
        } ;

        CmdMatcher::CmdMatcher(CmdUnit::SPtr&& cmdunit)
        : pimpl(std::make_unique<Impl>(
                std::vector<CmdUnit::SPtr>{std::move(cmdunit)}))
        {}

        CmdMatcher::CmdMatcher(const CmdUnit::SPtr& cmdunit)
        : pimpl(std::make_unique<Impl>(
                std::vector<CmdUnit::SPtr>{cmdunit}))
        {}

        CmdMatcher::CmdMatcher(std::vector<CmdUnit::SPtr>&& cmd)
        : pimpl(std::make_unique<Impl>(std::move(cmd)))
        {}

        CmdMatcher::CmdMatcher(const std::vector<CmdUnit::SPtr>& cmd)
        : pimpl(std::make_unique<Impl>(cmd))
        {}

        CmdMatcher::~CmdMatcher() noexcept = default ;
        CmdMatcher::CmdMatcher(CmdMatcher&&) = default ;
        CmdMatcher& CmdMatcher::operator=(CmdMatcher&&) = default ;

        int CmdMatcher::update_accepted(const CmdUnit& in_cmdunit) {
            return update_rejected(in_cmdunit) ;
        }

        int CmdMatcher::update_rejected(const CmdUnit& UNUSED(in_cmdunit)) {
            pimpl->states_.push(State::REJECTED) ;
            pimpl->heads_.push(pimpl->cmd_.size()) ;
            return 0 ;
        }

        int CmdMatcher::update_matching(const CmdUnit& in_cmdunit) {
            int head = pimpl->heads_.empty() ? 0 : pimpl->heads_.top() + 1 ;
            if(head >= pimpl->cmd_.size()) {
                return update_rejected(in_cmdunit) ;
            }

            const auto& unit = pimpl->cmd_[head] ;

            if(unit->is_containing(KEYCODE_OPTIONAL)) {
                pimpl->heads_.push(head) ;
                return update_any(in_cmdunit) ;
            }
            if(unit->is_containing(KEYCODE_OPTNUMBER)) {
                pimpl->heads_.push(head) ;
                return update_anynum(in_cmdunit) ;
            }

            bool reject = false ;
            for(const auto& ikey : in_cmdunit) {
                if(!ikey.is_unreal()) {
                    // If an inputted key is a physical key (e.g. <lshift>),
                    // confirm the target cmd unit contains the physical key
                    // or its representative key (e.g. <shift>).
                    if(!unit->is_containing(ikey) &&
                        !unit->is_containing(ikey.to_representative())) {
                        reject = true ;
                        break ;
                    }
                    continue ;
                }

                // If an inputted key is unreal (e.g. <shift>, <ctrl>),
                // verifies the target cmd unit contains the certain unreal
                // key or the physical key associated with the unreal key
                // (e.g. <lshift>, <rctrl>).
                bool has_key = false ;
                for(const auto& tkey : *unit) {
                    if(ikey == tkey || ikey == tkey.to_representative()) {
                        has_key = true ;
                        break ;
                    }
                }
                if(!has_key) {
                    reject = true ;
                    break ;
                }
            }
            if(reject) {
                return update_rejected(in_cmdunit) ;
            }

            if(pimpl->states_.size() < pimpl->cmd_.size() - 1) {
                pimpl->states_.push(State::MATCHING) ;
            }
            else {
                pimpl->states_.push(State::ACCEPTED) ;
            }
            pimpl->heads_.push(head) ;
            return unit->size() ;
        }

        int CmdMatcher::update_any(const CmdUnit& in_cmdunit) {
            pimpl->states_.push(State::ANY_ACCEPTED) ;
            pimpl->heads_.push(pimpl->heads_.top()) ;
            return in_cmdunit.size() ;
        }

        int CmdMatcher::update_anynum(const CmdUnit& in_cmdunit) {
            for(const auto& key : in_cmdunit) {
                if(key.is_major_system()) {
                    continue ;
                }

                auto uc = keycode_to_unicode(key, in_cmdunit) ;
                if(uc.empty()) {
                    continue ;
                }

                auto ascii = uc.front() ;
                if(ascii < '0' || '9' < ascii) {
                    return update_matching(in_cmdunit) ;
                }
            }

            pimpl->states_.push(State::ANYNUM_MATCHING) ;
            pimpl->heads_.push(pimpl->heads_.top()) ;
            return 1 ;
        }

        int CmdMatcher::update_state(const CmdUnit& in_cmdunit) {
            if(pimpl->states_.empty()) {
                return update_matching(in_cmdunit) ;
            }

            switch(pimpl->states_.top()) {
                case State::ACCEPTED:
                    return update_accepted(in_cmdunit) ;

                case State::REJECTED:
                    return update_rejected(in_cmdunit) ;

                case State::MATCHING:
                    return update_matching(in_cmdunit) ;

                case State::ANY_ACCEPTED:
                    return update_any(in_cmdunit) ;

                case State::ANYNUM_MATCHING:
                    return update_anynum(in_cmdunit) ;
            }

            throw LOGIC_EXCEPT("Unsupported matcher state.") ;
        }

        bool CmdMatcher::is_accepted() const noexcept {
            if(pimpl->states_.empty()) {
                return false ;
            }
            return (pimpl->states_.top() & State::ACCEPTED) == State::ACCEPTED ;
        }

        bool CmdMatcher::is_rejected() const noexcept {
            if(pimpl->states_.empty()) {
                return false ;
            }
            return (pimpl->states_.top() & State::REJECTED) == State::REJECTED ;
        }

        bool CmdMatcher::is_matching() const noexcept {
            if(pimpl->states_.empty()) {
                return true ;
            }
            return (pimpl->states_.top() & State::MATCHING) == State::MATCHING ;
        }

        void CmdMatcher::reset_state() {
            decltype(pimpl->states_)().swap(pimpl->states_) ;
            decltype(pimpl->heads_)().swap(pimpl->heads_) ;
        }

        void CmdMatcher::backward_state(int n) {
            for(int i = 0 ; i < n ; i ++) {
                if(pimpl->states_.empty()) {
                    break ;
                }
                pimpl->states_.pop() ;
                pimpl->heads_.pop() ;
            }
        }
    }
}

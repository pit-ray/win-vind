#include "cmdmatcher.hpp"

#include "keycode.hpp"

#include "util/def.hpp"
#include "util/type_traits.hpp"

#include <iterator>
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
        ANYNUM_ACCEPTED = 0b0010'0001,
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

            explicit Impl()
            : cmd_(),
              states_(),
              heads_()
            {}

            explicit Impl(const std::vector<CmdUnit::SPtr>& cmd)
            : cmd_(cmd),
              states_(),
              heads_()
            {}

            explicit Impl(std::vector<CmdUnit::SPtr>&& cmd)
            : cmd_(std::move(cmd)),
              states_(),
              heads_()
            {}

            ~Impl() noexcept = default ;

            Impl(const Impl&) = default ;
            Impl& operator=(const Impl&) = default ;

            Impl(Impl&&) = default ;
            Impl& operator=(Impl&&) = default ;
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

        CmdMatcher::CmdMatcher(const CmdMatcher& rhs)
        : pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
        {}

        CmdMatcher& CmdMatcher::operator=(const CmdMatcher& rhs) {
            if(rhs.pimpl) *pimpl = *(rhs.pimpl) ;
            return *this ;
        }

        CmdMatcher::CmdMatcher(CmdMatcher&&) = default ;
        CmdMatcher& CmdMatcher::operator=(CmdMatcher&&) = default ;

        std::size_t CmdMatcher::history_size() const noexcept {
            return pimpl->states_.size() ;
        }

        const std::vector<CmdUnit::SPtr>& CmdMatcher::get_command() const noexcept {
            return pimpl->cmd_ ;
        }

        int CmdMatcher::update_accepted(const CmdUnit& in_cmdunit) {
            return update_rejected(in_cmdunit) ;
        }

        int CmdMatcher::update_rejected(const CmdUnit& UNUSED(in_cmdunit)) {
            pimpl->states_.push(State::REJECTED) ;
            pimpl->heads_.push(static_cast<int>(pimpl->cmd_.size())) ;
            return 0 ;
        }

        int CmdMatcher::update_matching(const CmdUnit& in_cmdunit) {
            int head = pimpl->heads_.empty() ? 0 : pimpl->heads_.top() + 1 ;
            if(head >= static_cast<int>(pimpl->cmd_.size())) {
                return update_rejected(in_cmdunit) ;
            }

            const auto& unit = pimpl->cmd_[head] ;

            if(unit->is_containing(KEYCODE_OPTIONAL)) {
                return update_any(in_cmdunit, head) ;
            }
            if(unit->is_containing(KEYCODE_OPTNUMBER)) {
                return update_anynum(in_cmdunit, head) ;
            }

            std::vector<bool> conds(unit->size(), false) ;
            for(const auto& ikey : in_cmdunit) {
                bool is_matched = false ;
                if(!ikey.is_unreal()) {
                    // If an inputted key is a physical key (e.g. <lshift>),
                    // confirm the target command unit contains the physical key
                    // or its representative key (e.g. <shift>).
                    for(auto itr = unit->begin() ; itr != unit->end() ; itr ++) {
                        auto idx = std::distance(unit->begin(), itr) ;
                        // When the cmd unit has duplicate keycodes (e.g. <shift-lshift-s>),
                        // it processes properly without 'break' statement.
                        if(*itr == ikey || *itr == ikey.to_representative()) {
                            conds[idx] = true ;
                            is_matched = true ;
                        }
                    }
                }
                else {
                    // If an inputted key is unreal (e.g. <shift>, <ctrl>),
                    // verifies the target command unit contains the certain unreal
                    // key or the physical key associated with the unreal key
                    // (e.g. <lshift>, <rctrl>).
                    for(auto itr = unit->begin() ; itr != unit->end() ; itr ++) {
                        auto idx = std::distance(unit->begin(), itr) ;
                        // When the cmd unit has duplicate keycodes (e.g. <shift-lshift-s>),
                        // it processes properly without 'break' statement.
                        if(*itr == ikey || itr->to_representative() == ikey) {
                            conds[idx] = true ;
                            is_matched = true ;
                        }
                    }
                }

                if(!is_matched) {
                    return update_rejected(in_cmdunit) ;
                }
            }

            if(!std::all_of(conds.begin(), conds.end(), [](bool v) {return v ;})) {
                return update_rejected(in_cmdunit) ;
            }

            if(head == static_cast<int>(pimpl->cmd_.size() - 1)) {
                pimpl->states_.push(State::ACCEPTED) ;
            }
            else {
                pimpl->states_.push(State::MATCHING) ;
            }
            pimpl->heads_.push(head) ;
            return static_cast<int>(in_cmdunit.size()) ;
        }

        int CmdMatcher::update_any(const CmdUnit& in_cmdunit, int head) {
            pimpl->states_.push(State::ANY_ACCEPTED) ;
            if(head < 0) {
                head = pimpl->heads_.top() ;
            }
            pimpl->heads_.push(head) ;
            return static_cast<int>(in_cmdunit.size()) ;
        }

        int CmdMatcher::update_anynum(const CmdUnit& in_cmdunit, int head) {
            if(head < 0) {
                head = pimpl->heads_.top() ;
            }

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
                    if(head == static_cast<int>(pimpl->cmd_.size() - 1)) {
                        return update_rejected(in_cmdunit) ;
                    }
                    return update_matching(in_cmdunit) ;
                }
            }

            // Check whether the head points to the last position of
            // the command to take account of some cases when the <num>
            // is placed on the last of the command like `abc<num>`.
            if(head == static_cast<int>(pimpl->cmd_.size() - 1)) {
                pimpl->states_.push(State::ANYNUM_ACCEPTED) ;
            }
            else {
                pimpl->states_.push(State::ANYNUM_MATCHING) ;
            }

            pimpl->heads_.push(head) ;

            return static_cast<int>(in_cmdunit.size()) ;
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
                case State::ANYNUM_ACCEPTED:
                    return update_anynum(in_cmdunit) ;
            }

            throw LOGIC_EXCEPT("Unsupported matcher state.") ;
        }

        bool CmdMatcher::is_accepted() const noexcept {
            if(pimpl->states_.empty()) {
                return false ;
            }
            return util::enum_has_bits(pimpl->states_.top(), State::ACCEPTED) ;
        }

        bool CmdMatcher::is_rejected() const noexcept {
            if(pimpl->states_.empty()) {
                return false ;
            }
            return util::enum_has_bits(pimpl->states_.top(), State::REJECTED) ;
        }

        bool CmdMatcher::is_matching() const noexcept {
            if(pimpl->states_.empty()) {
                return true ;
            }
            return util::enum_has_bits(pimpl->states_.top(), State::MATCHING) ;
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

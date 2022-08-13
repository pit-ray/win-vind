#ifndef _CMDMATCHER_HPP
#define _CMDMATCHER_HPP

#include "cmdunit.hpp"

#include <memory>

namespace vind
{
    namespace core
    {
        class CmdMatcher {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            int update_accepted(const CmdUnit& in_cmdunit) ;
            int update_rejected(const CmdUnit& in_cmdunit) ;
            int update_matching(const CmdUnit& in_cmdunit) ;
            int update_any(const CmdUnit& in_cmdunit) ;
            int update_anynum(const CmdUnit& in_cmdunit) ;

        public:
            explicit CmdMatcher(CmdUnit::SPtr&& cmdunit) ;
            explicit CmdMatcher(const CmdUnit::SPtr& cmdunit) ;

            explicit CmdMatcher(std::vector<CmdUnit::SPtr>&& cmd) ;
            explicit CmdMatcher(const std::vector<CmdUnit::SPtr>& cmd) ;

            virtual ~CmdMatcher() noexcept ;

            CmdMatcher(const CmdMatcher&) ;
            CmdMatcher& operator=(const CmdMatcher&) ;

            CmdMatcher(CmdMatcher&&) ;
            CmdMatcher& operator=(CmdMatcher&&) ;

            std::size_t history_size() const noexcept ;

            const std::vector<CmdUnit::SPtr>& get_command() const noexcept ;

            int update_state(const CmdUnit& in_cmdunit) ;

            bool is_accepted() const noexcept ;
            bool is_rejected() const noexcept ;
            bool is_matching() const noexcept ;

            void reset_state() ;
            void backward_state(int n) ;
        } ;
    }
}

#endif

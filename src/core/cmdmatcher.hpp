#ifndef _CMDMATCHER_HPP
#define _CMDMATCHER_HPP

namespace vind
{
    namespace core
    {
        class CmdUnitMatcher {
        private:

        public:
            explicit CmdUnitMatcher() ;
            virtual ~CmdUnitMatcher() noexcept ;


        } ;

        class CmdMatcher {
        private:

        public:
            explicit CmdMatcher() ;
            virtual ~CmdMatcher() noexcept ;

            bool is_accepted() ;
        } ;
    }
}

#endif

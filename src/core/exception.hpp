#ifndef _EXCEPTION_HPP
#define _EXCEPTION_HPP

#include <string>

namespace vind
{
    namespace core
    {
        class SafeForcedTermination {
        private:
            std::string msg_ ;

        public:
            explicit SafeForcedTermination() ;
            explicit SafeForcedTermination(const std::string& what_arg) ;
            explicit SafeForcedTermination(const char* what_arg) ;

            virtual ~SafeForcedTermination() noexcept ;

            virtual const char* what() const noexcept ;
        } ;
    }
}

#endif

#include "exception.hpp"


namespace vind
{
    namespace core
    {
        SafeForcedTermination::SafeForcedTermination()
        : msg_("SAFE FORCED TERMINATION")
        {}

        SafeForcedTermination::SafeForcedTermination(const std::string& what_arg)
        : msg_("SAFE FORCED TERMINATION: " + what_arg)
        {}

        SafeForcedTermination::SafeForcedTermination(const char* what_arg)
        : msg_("SAFE FORCED TERMINATION: " + std::string(what_arg))
        {}

        SafeForcedTermination::~SafeForcedTermination() noexcept = default ;

        const char* SafeForcedTermination::what() const noexcept {
            return msg_.c_str() ;
        }
    }
}

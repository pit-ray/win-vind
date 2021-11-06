#ifndef _EXCEPT_HPP
#define _EXCEPT_HPP

#include <stdexcept>

#ifndef UNUSED
#define UNUSED(identifier) /* identifier */
#endif

//exception class with scope identifier
#if defined(__GNUC__)
#define LOGIC_EXCEPT(msg) \
    std::logic_error(std::string("An logic exception occurred from ") +\
            __PRETTY_FUNCTION__ + ". " + msg)

#define RUNTIME_EXCEPT(msg) \
    std::runtime_error(std::string("An runtime exception occurred from ") +\
            __PRETTY_FUNCTION__ + ". " + msg)

#elif defined(_MSC_VER) && _MSC_VER >= 1500
#define LOGIC_EXCEPT(msg) \
    std::logic_error(std::string("An logic exception occurred from ") +\
            __FUNCSIG__ + ". " + msg)

#define RUNTIME_EXCEPT(msg) \
    std::runtime_error(std::string("An runtime exception occurred from ") +\
            __FUNCSIG__ + ". " + msg)

#endif //defubed(__GNUC__)

#endif

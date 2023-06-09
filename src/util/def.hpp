#ifndef _EXCEPT_HPP
#define _EXCEPT_HPP

#include <stdexcept>

#ifndef UNUSED
#define UNUSED(identifier) /* identifier */
#endif

#define LOGIC_EXCEPT(msg) \
    std::logic_error(std::string("An logic exception occurred: ") + msg)

#define RUNTIME_EXCEPT(msg) \
    std::runtime_error(std::string("An runtime exception occurred: ") + msg)


#endif

#ifndef _UIAFWD_HPP
#define _UIAFWD_HPP

#include <windows.h>

#include "disable_gcc_warning.hpp"

#if defined(_MSC_VER) && _MSC_VER >= 1500
#include <uiautomationclient.h>
#else
#include <um/uiautomationclient.h>
#endif

#include "enable_gcc_warning.hpp"

#endif

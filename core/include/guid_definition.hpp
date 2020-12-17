#ifndef _GUID_DEFINITION_HPP
#define _GUID_DEFINITION_HPP

#include <initguid.h>

// Actually, we should link uuid library, but we define only the necessary GUID here
// to avoid causing the multiple definition with the explicit GUID of wxWidgets.
// You can refer the well-known GUID.
// https://www.magnumdb.com/search?q=IID_IUIAutomation+CLSID_CUIAutomation
DEFINE_GUID(IID_IUIAutomation,   0x30cbe57d, 0xd9d0, 0x452a, 0xab, 0x13, 0x7a, 0xc5, 0xac, 0x48, 0x25, 0xee) ;
DEFINE_GUID(CLSID_CUIAutomation, 0xff48dba4, 0x60ef, 0x4201, 0xaa, 0x87, 0x54, 0x10, 0x3e, 0xef, 0x59, 0x4e) ;

#endif

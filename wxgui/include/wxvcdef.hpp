//This is a countermeasure for the error in declaring wxStroll() in wx/wxcrt.h
#if defined(_MSC_VER) && _MSC_VER >= 1500
#ifndef wxNEEDS_DECL_BEFORE_TEMPLATE
#define wxNEEDS_DECL_BEFORE_TEMPLATE
#endif
#endif


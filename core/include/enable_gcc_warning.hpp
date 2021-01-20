#if defined(_MSC_VER) && _MSC_VER >= 1500
#pragma warning(default : 4996)
#pragma warning(default : 5054)
#pragma warning(default : 4244)

#else
#pragma warning(disable : 4068)
#pragma GCC diagnostic warning "-Wcast-function-type"
#pragma GCC diagnostic warning "-Wcast-qual"
#pragma GCC diagnostic warning "-Wctor-dtor-privacy"
#pragma GCC diagnostic warning "-Wdouble-promotion"
#pragma GCC diagnostic warning "-Weffc++"
#pragma GCC diagnostic warning "-Wignored-qualifiers"
#pragma GCC diagnostic warning "-Wnon-virtual-dtor"
#pragma GCC diagnostic warning "-Wold-style-cast"
#pragma GCC diagnostic warning "-Wshadow"
#pragma GCC diagnostic warning "-Wsuggest-override"
#pragma GCC diagnostic warning "-Wuseless-cast"
#pragma warning(default : 4068)

#endif

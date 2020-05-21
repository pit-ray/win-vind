#include "filer.hpp"
#include "keybrd_eventer.hpp"

using namespace std ;

//SaveOpenedFile
const string SaveOpenedFile::sname() noexcept
{
    return "save_opened_file" ;
}

bool SaveOpenedFile::sprocess(const string cmd)
{
    return KeybrdEventer::is_pushup(VKC_LCTRL, VKC_S) ;
}


//CloseOpenedFile
const string CloseOpenedFile::sname() noexcept
{
    return "close_opened_file" ;
}

bool CloseOpenedFile::sprocess(const string cmd)
{
    return KeybrdEventer::is_pushup(VKC_LCTRL, VKC_F4) ;
}
#ifndef _EDI_CLIPBOARD_HPP
#define _EDI_CLIPBOARD_HPP

#include "binded_function_with_creator.hpp"
/*
ノーマルモードと、エディタモードでは、
基本的にコピー方式が違う
今は、仮にショートカットキーだが、
アナライザーからでは、レジスタも用意可能

したがって、別に作るとよい

また、ペーストは、
ライン選択と単語選択で挙動が異なる
Nシステムもある


*/

struct EdiCopy : public BindedFunctionWithCreator<EdiCopy>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct EdiNLineCopy : public BindedFunctionWithCreator<EdiNLineCopy>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct EdiPaste : public BindedFunctionWithCreator<EdiPaste>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct EdiCut : public BindedFunctionWithCreator<EdiCut>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct EdiDelete : public BindedFunctionWithCreator<EdiDelete>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct EdiBackSpace : public BindedFunctionWithCreator<EdiDelete>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif
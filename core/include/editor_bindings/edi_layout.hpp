#ifndef _EDI_LAYOUT_HPP
#define _EDI_LAYOUT_HPP

#include "binded_func_with_creator.hpp"

class EdiNRemoveEOL : public BindedFuncWithCreator<EdiNRemoveEOL>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit EdiNRemoveEOL() ;
    virtual ~EdiNRemoveEOL() noexcept ;

    EdiNRemoveEOL(EdiNRemoveEOL&&) ;
    EdiNRemoveEOL& operator=(EdiNRemoveEOL&&) ;
    EdiNRemoveEOL(const EdiNRemoveEOL&)             = delete ;
    EdiNRemoveEOL& operator=(const EdiNRemoveEOL&)  = delete ;
} ;

#endif

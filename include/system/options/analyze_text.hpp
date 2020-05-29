/*

    NOT IMPLEMENTED YET

*/

#ifndef _ANALYZE_TEXT_HPP
#define _ANALYZE_TEXT_HPP

#include "dynamic_option.hpp"

class AnalyzeText : public DynamicOption
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
    bool do_enable() const noexcept override ;
    bool do_disable() const noexcept override ;
    bool do_process() const override ;

public:
    const std::string name() const noexcept override ;

    static std::unique_ptr<DynamicOption> create() ;

    explicit AnalyzeText() ;
    virtual ~AnalyzeText() ;
    AnalyzeText(AnalyzeText&&) ;
    AnalyzeText& operator=(AnalyzeText&&) ;

    AnalyzeText(const AnalyzeText&) = delete ;
    AnalyzeText& operator=(const AnalyzeText&) = delete ;
} ;

#endif
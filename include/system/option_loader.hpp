#ifndef _OPTION_LOADER_HPP
#define _OPTION_LOADER_HPP

#include <memory>
#include <string>
#include <vector>

class KeyBinder ;
class OptionLoader
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    explicit OptionLoader(const KeyBinder* const pkb) ;
    ~OptionLoader() noexcept ;

    void load_config(const std::string& filename) noexcept ;
    void update() noexcept ;

    OptionLoader(OptionLoader&&) = delete ;
    OptionLoader& operator=(OptionLoader&&) = delete ;
    OptionLoader(const OptionLoader&) = delete ;
    OptionLoader& operator=(const OptionLoader&) = delete ;
} ;


#endif
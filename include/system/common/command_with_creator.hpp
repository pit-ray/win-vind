#ifndef _COMMAND_WITH_CREATOR_HPP
#define _COMMAND_WITH_CREATOR_HPP

#include "command.hpp"
#include <memory>

//use Curiously Recurring Template Pattern (CRTP)
//derived class must implement sprocess and sname.
//if derived class does not use variable of member, sprocess prefers static function.
//else, sprocess is constant function.
template <typename Derived>
class CommandWithCreator : public Command {
private:
    bool do_process(const std::string cmd) const override {
        return static_cast<const Derived*>(this)->sprocess(cmd) ;
    }

public:
    static std::unique_ptr<Command> create() {
        return std::make_unique<Derived>() ;
    }

    static std::shared_ptr<Command> create_with_cache() {
        static std::weak_ptr<Derived> cache ;

        auto pobj = cache.lock() ;

        if(!pobj) {
            pobj = std::make_shared<Derived>() ;
            cache = pobj ;
        }

        return pobj ;
    }

    const std::string name() const noexcept override {
        return Derived::sname() ;
    }
} ;


#endif
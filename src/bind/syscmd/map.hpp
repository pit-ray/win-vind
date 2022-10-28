#ifndef _MAP_HPP
#define _MAP_HPP

#include "bind/bindedfunc.hpp"

#include "core/rcparser.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        SystemCall do_map(
                const std::string& args,
                const std::string& prefix) ;

        SystemCall do_noremap(
                const std::string& args,
                const std::string& prefix) ;

        SystemCall do_unmap(
                const std::string& args,
                const std::string& prefix) ;

        SystemCall do_mapclear(const std::string& prefix) ;


        template <typename Derived>
        class MapBase : public BindedFuncCreator<Derived> {
        private:
            SystemCall do_process(
                    std::uint16_t UNUSED(count),
                    const std::string& args) override {
                auto [prefix, _] = core::divide_prefix_and_cmd(
                    static_cast<Derived*>(this)->name(), "m") ;
                return do_map(args, prefix) ;
            }

        public:
            template <typename String>
            explicit MapBase(String&& name)
            : BindedFuncCreator<Derived>(std::forward<String>(name))
            {}
        } ;


        template <typename Derived>
        class NoremapBase : public BindedFuncCreator<Derived> {
        private:
            SystemCall do_process(
                std::uint16_t UNUSED(count),
                const std::string& args) override {
                auto [prefix, _] = core::divide_prefix_and_cmd(
                    static_cast<Derived*>(this)->name(), "n") ;
                return do_noremap(args, prefix) ;
            }

        public:
            template <typename String>
            explicit NoremapBase(String&& name)
            : BindedFuncCreator<Derived>(std::forward<String>(name))
            {}
        } ;


        template <typename Derived>
        class UnmapBase : public BindedFuncCreator<Derived> {
        private:
            SystemCall do_process(
                std::uint16_t UNUSED(count),
                const std::string& args) override {
                auto [prefix, _] = core::divide_prefix_and_cmd(
                    static_cast<Derived*>(this)->name(), "u") ;
                return do_unmap(args, prefix) ;
            }

        public:
            template <typename String>
            explicit UnmapBase(String&& name)
            : BindedFuncCreator<Derived>(std::forward<String>(name))
            {}
        } ;


        template <typename Derived>
        class MapclearBase : public BindedFuncCreator<Derived> {
        private:
            SystemCall do_process(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) override {
                auto [prefix, _] = core::divide_prefix_and_cmd(
                    static_cast<Derived*>(this)->name(), "m") ;
                return do_mapclear(prefix) ;
            }

        public:
            template <typename String>
            explicit MapclearBase(String&& name)
            : BindedFuncCreator<Derived>(std::forward<String>(name))
            {}
        } ;
    }
}

#endif

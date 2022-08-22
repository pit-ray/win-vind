#ifndef _BINDED_FUNC_HPP
#define _BINDED_FUNC_HPP

#include <memory>
#include <string>

#include "core/mode.hpp"
#include "core/syscalldef.hpp"


namespace vind
{
    namespace bind
    {
        class BindedFunc {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            virtual void error_process(const std::exception& e) ;

            virtual SystemCall do_process(
                    std::uint16_t count, const std::string& args) ;

        public:
            using SPtr = std::shared_ptr<BindedFunc> ;

            explicit BindedFunc() ;
            explicit BindedFunc(const std::string& name) ;
            explicit BindedFunc(std::string&& name) ;
            virtual ~BindedFunc() noexcept ;

            BindedFunc(BindedFunc&&) ;
            BindedFunc& operator=(BindedFunc&&) ;
            BindedFunc(const BindedFunc&)            = delete ;
            BindedFunc& operator=(const BindedFunc&) = delete ;

            const std::string& name() const noexcept ;
            const std::size_t& id() const noexcept ;

            static std::size_t name_to_id(const std::string& name) noexcept {
                return std::hash<std::string>()(name) ;
            }
            static std::size_t name_to_id(std::string&& name) noexcept {
                return std::hash<std::string>()(std::move(name)) ;
            }

            SystemCall process(
                    std::uint16_t count=1, const std::string& args="") ;

            virtual void reconstruct() ;

            bool operator==(const BindedFunc& rhs) const noexcept ;
            bool operator==(BindedFunc&& rhs) const noexcept ;
            bool operator!=(const BindedFunc& rhs) const noexcept ;
            bool operator!=(BindedFunc&& rhs) const noexcept ;
        } ;


        //use Curiously Recurring Template Pattern (CRTP)
        //derived class must implement sprocess() and sname().
        //If derived class does not have any variables of member, sprocess() prefers static function.
        //If not, sprocess() is constant function.
        template <typename Derived>
        class BindedFuncCreator: public BindedFunc {
        public:
            template <typename String>
            explicit BindedFuncCreator(String&& name)
            : BindedFunc(std::forward<String>(name))
            {}

            static std::unique_ptr<BindedFunc> create() {
                return std::make_unique<Derived>() ;
            }

            static std::shared_ptr<BindedFunc> create_with_cache() {
                static std::weak_ptr<Derived> cache ;

                auto pobj = cache.lock() ;
                if(!pobj) {
                    pobj = std::make_shared<Derived>() ;
                    cache = pobj ;
                }
                return pobj ;
            }
        } ;


        template <typename Derived>
        class BindedFuncVoid : public BindedFuncCreator<Derived> {
        private:
            SystemCall do_process(
                    std::uint16_t count, const std::string& args) override {
                static_cast<Derived*>(this)->sprocess(count, args) ;
                return SystemCall::NOTHING ;
            }

        public:
            template <typename String>
            explicit BindedFuncVoid(String&& name)
            : BindedFuncCreator<Derived>(std::forward<String>(name))
            {}
        } ;


        template <typename Derived>
        class BindedFuncFlex : public BindedFuncCreator<Derived> {
        private:
            SystemCall do_process(
                    std::uint16_t count, const std::string& args) override {
                return static_cast<Derived*>(this)->sprocess(count, args) ;
            }

        public:
            template <typename String>
            explicit BindedFuncFlex(String&& name)
            : BindedFuncCreator<Derived>(std::forward<String>(name))
            {}
        } ;
    }
}
#endif

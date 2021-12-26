#ifndef _FUNC_FINDER_HPP
#define _FUNC_FINDER_HPP

#include <memory>
#include <string>
#include <vector>

#include "bind/binded_func.hpp"
#include "keylgrbase.hpp"
#include "lgrparser.hpp"
#include "lgrparsermgr.hpp"
#include "mode.hpp"


namespace vind
{
    namespace core
    {
        class FuncFinder : public LoggerParserManager {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            void do_reconstruct(Mode mode) ;
            template <typename T>
            void do_reconstruct(T mode) {
                do_reconstruct(static_cast<Mode>(mode)) ;
            }

        public:
            explicit FuncFinder() ;
            virtual ~FuncFinder() noexcept ;

            FuncFinder(FuncFinder&&) ;
            FuncFinder& operator=(FuncFinder&&) ;

            void reconstruct(Mode mode) ;
            template <typename T>
            void reconstruct(T mode) {
                reconstruct(static_cast<Mode>(mode)) ;
            }

            void reconstruct(
                    Mode mode,
                    const std::vector<bind::BindedFunc::SPtr>& funcs) ;
            template <typename T>
            void reconstruct(
                    T mode,
                    const std::vector<bind::BindedFunc::SPtr>& funcs) {
                reconstruct(static_cast<Mode>(mode), funcs) ;
            }

            void reconstruct(
                    Mode mode,
                    std::vector<bind::BindedFunc::SPtr>&& funcs) ;
            template <typename T>
            void reconstruct(
                    T mode,
                    std::vector<bind::BindedFunc::SPtr>&& funcs) {
                reconstruct(static_cast<Mode>(mode), std::move(funcs)) ;
            }

            bind::BindedFunc::SPtr find_func_byname(const std::string& name) const ;

            FuncFinder(const FuncFinder&) = delete ;
            FuncFinder& operator=(const FuncFinder&) = delete ;
        } ;
    }
}

#endif

#ifndef _ENTRY_HPP
#define _ENTRY_HPP

/*
             _                  _           __
   _      __(_)___       _   __(_)___  ____/ /
  | | /| / / / __ \_____| | / / / __ \/ __  /
  | |/ |/ / / / / /_____/ |/ / / / / / /_/ /
  |__/|__/_/_/ /_/      |___/_/_/ /_/\__,_/

       - Vim Key Binder for Windows -

MIT License

Copyright (c) 2020-2021 pit-ray
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <functional>
#include <memory>
#include <string>

#include "exception.hpp"
#include "syscalldef.hpp"


namespace vind
{
    namespace core
    {
        class VindEntry {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit VindEntry(
                    const std::function<void()>& exit_func,
                    const std::string& mapped_memname="qvCI980BTny1ZSFfY76sO71w7MtLTzuPVd6RQs47_p7Kn4SJZ7cnaH8QwPS901VFd2N5WuxECvx7N3hP7caWK44ZSq6",
                    std::size_t mapped_memsize=1024) ;
            virtual ~VindEntry() noexcept ;

            bool is_subprocess() const noexcept ;

            void init(const std::string& func_request) ;
            void init() ;

            void reconstruct() ;

            void update() ;

            void handle_system_call(SystemCall systemcall) ;

            VindEntry(VindEntry&&)                 = delete ;
            VindEntry& operator=(VindEntry&&)      = delete ;
            VindEntry(const VindEntry&)            = delete ;
            VindEntry& operator=(const VindEntry&) = delete ;
        } ;
    }
}

#endif

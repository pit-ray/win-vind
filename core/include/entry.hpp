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

Copyright (c) 2020 pit-ray
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

namespace vind
{
    //
    // This file defines packaged functions.
    //
    // Example:
    // int main() {
    //     vind::initialize() ;
    //
    //     while(vind::update()) ;
    //
    //     return 0 ;
    // }
    //

    bool initialize(const std::string& func_name="") noexcept ;

    // It load all config and reload them.
    bool load_config() noexcept ;

    // It apply settings of all components from kernel configs. 
    bool reconstruct_all_components() noexcept ;

    // It updates a main loop once with update_background().
    bool update() noexcept ;

    // If you make some loop functions, the function is needed to call.
    // It includes Sleep().
    bool update_background() noexcept ;

    void register_show_window_func(std::function<void()> func) noexcept ;
    void register_exit_window_func(std::function<void()> func) noexcept ;
}

#endif

#ifndef _UICACHE_HPP
#define _UICACHE_HPP

#include <memory>

#include "util/uia.hpp"


namespace vind
{
    namespace opt
    {
        void add_common_cache_property(PROPERTYID id) ;

        class UICache {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            UICache() ;
            explicit UICache(HWND hwnd) ;

            virtual ~UICache() noexcept ;

            UICache(UICache&&) ;
            UICache& operator=(UICache&&) ;

            UICache(const UICache&) = delete ;
            UICache& operator=(const UICache&) = delete ;

            bool is_valid() ;

            void create_thread() ;

            bool check_if_finished() ;

            void update() ;

            const util::SmartElement& latest() ;
        } ;
    }
}

#endif

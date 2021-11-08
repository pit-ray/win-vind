#ifndef _SMARTCOM_HPP
#define _SMARTCOM_HPP

#include <windows.h>

#include <utility>


namespace vind
{
    namespace util
    {
        template <typename T>
        class SmartCom {
        private:
            T* p_ ;

            void addref() const noexcept {
                if(p_) {
                    p_->AddRef() ;
                }
            }

            void release() noexcept {
                // The Release method create event chain.
                // The chain release the resource twice in last instance.
                auto temp = p_ ;
                if(temp) {
                    p_ = nullptr ;
                    temp->Release() ;
                }
            }

        public:
            explicit SmartCom(T* ptr=nullptr)
            : p_(ptr)
            {}

            virtual ~SmartCom() noexcept {
                release() ;
            }

            SmartCom(const SmartCom& rhs)
            : p_(rhs.p_)
            {
                addref() ;
            }
            SmartCom& operator=(const SmartCom& rhs) {
                if(p_ != rhs.p_) {
                    release() ;
                    p_ = rhs.p_ ;
                    addref() ;
                }
                return *this ;
            }

            SmartCom(SmartCom&& rhs)
            : p_(rhs.p_)
            {
                rhs.p_ = nullptr ;
            }
            SmartCom& operator=(SmartCom&& rhs) {
                if(p_ != rhs.p_) {
                    release() ;
                    p_ = rhs.p_ ;
                    rhs.p_ = nullptr ;
                }
                return *this ;
            }

            void swap(SmartCom& rhs) noexcept {
                auto temp = p_ ;
                p_ = rhs.p_ ;
                rhs.p_ = temp ;
            }

            T& operator*() const noexcept {
                return *p_ ;
            }
            T* operator->() const noexcept {
                return p_ ;
            }

            T** operator&() noexcept {
                return &p_ ;
            }

            bool operator!() const noexcept {
                return p_ == nullptr ;
            }

            explicit operator bool() const noexcept {
                return p_ != nullptr ;
            }

            void reset() noexcept {
                release() ;
            }

            void reset(T* ptr) noexcept {
                release() ;
                p_ = ptr ;
            }

            T* get() const noexcept {
                return p_ ;
            }
        } ;

        template <typename T>
        void swap(SmartCom<T> lhs, SmartCom<T> rhs) noexcept {
            lhs.swap(rhs) ;
        }
    }
}

#endif

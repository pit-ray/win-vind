#include "key_logger_base.hpp"

#include "util/container.hpp"

#include "core/inputgate.hpp"

#include <sstream>

namespace vind
{
    namespace core
    {
        struct KeyLoggerBase::Impl {
            Data data{} ;
        } ;

        KeyLoggerBase::KeyLoggerBase()
        : pimpl(std::make_unique<Impl>())
        {}

        KeyLoggerBase::~KeyLoggerBase() noexcept = default ;

        KeyLoggerBase::KeyLoggerBase(const KeyLoggerBase& rhs)
        : pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
        {}

        KeyLoggerBase& KeyLoggerBase::operator=(const KeyLoggerBase& rhs) {
            if(rhs.pimpl) *pimpl = *(rhs.pimpl) ;
            return *this ;
        }

        KeyLoggerBase::KeyLoggerBase(KeyLoggerBase&& rhs)            = default ;
        KeyLoggerBase& KeyLoggerBase::operator=(KeyLoggerBase&& rhs) = default ;

        KeyLoggerBase::Data::iterator KeyLoggerBase::begin() noexcept {
            return pimpl->data.begin() ;
        }
        KeyLoggerBase::Data::iterator KeyLoggerBase::end() noexcept {
            return pimpl->data.end() ;
        }

        KeyLoggerBase::Data::const_iterator KeyLoggerBase::begin() const noexcept {
            return pimpl->data.begin() ;
        }
        KeyLoggerBase::Data::const_iterator KeyLoggerBase::end() const noexcept {
            return pimpl->data.end() ;
        }

        KeyLoggerBase::Data::const_iterator KeyLoggerBase::cbegin() const noexcept {
            return pimpl->data.cbegin() ;
        }
        KeyLoggerBase::Data::const_iterator KeyLoggerBase::cend() const noexcept {
            return pimpl->data.cend() ;
        }

        KeyLoggerBase::Data::reference KeyLoggerBase::latest() {
            return pimpl->data.back() ;
        }
        KeyLoggerBase::Data::const_reference KeyLoggerBase::latest() const {
            return pimpl->data.back() ;
        }

        std::size_t KeyLoggerBase::size() const noexcept {
            return pimpl->data.size() ;
        }

        bool KeyLoggerBase::empty() const noexcept {
            return pimpl->data.empty() ;
        }

        void KeyLoggerBase::clear() noexcept {
            return pimpl->data.clear() ;
        }

        void KeyLoggerBase::logging(const KeyLog& log) {
            pimpl->data.push_back(log) ;
        }
        void KeyLoggerBase::logging(KeyLog&& log) {
            pimpl->data.push_back(std::move(log)) ;
        }

        void KeyLoggerBase::logging(const KeyLog::Data& lograw) {
            pimpl->data.emplace_back(lograw) ;
        }
        void KeyLoggerBase::logging(KeyLog::Data&& lograw) {
            pimpl->data.emplace_back(std::move(lograw)) ;
        }

        void KeyLoggerBase::remove_from_back(std::size_t num_from_back) {
            util::remove_from_back(pimpl->data, num_from_back) ;
        }

        void KeyLoggerBase::remove_from_top(std::size_t num_from_top) {
            util::remove_from_top(pimpl->data, num_from_top) ;
        }

        KeyLoggerBase::Data::const_reference KeyLoggerBase::at(std::size_t n) const {
            return pimpl->data.at(n) ;
        }

        bool KeyLoggerBase::operator==(const KeyLoggerBase& rhs) const {
            return pimpl->data == rhs.pimpl->data ;
        }

        bool KeyLoggerBase::operator!=(const KeyLoggerBase& rhs) const {
            return pimpl->data != rhs.pimpl->data ;
        }

        int KeyLoggerBase::logging_state(const KeyLog& log) {
            logging(log) ;
            return static_cast<int>(latest().size()) ;
        }

        std::ostream& operator<<(std::ostream& stream, const KeyLoggerBase& rhs) {
            if(!rhs.empty()) {
                for(auto itr = rhs.begin() ; itr != rhs.end() ; itr ++) {
                    stream << *itr ;
                }
            }
            return stream ;
        }
    }
}

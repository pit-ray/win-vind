#include "settable.hpp"

#include <mutex>
#include <unordered_map>


namespace vind
{
    namespace core
    {
        struct SetTable::Impl {
            std::unordered_map<std::string, Param> table_{} ;
            std::unordered_map<std::string, Param> deftable_{} ;
            std::mutex mtx_{} ;

            template <typename T>
            void set_from_raw(const std::string& name, T&& val) {
                std::lock_guard<std::mutex> scoped_lock{mtx_} ;
                table_[name] = Param(name, std::forward<T>(val)) ;
            }
        } ;

        SetTable::SetTable()
        : pimpl(std::make_unique<Impl>())
        {}

        SetTable::~SetTable() noexcept = default ;

        SetTable& SetTable::get_instance() {
            static SetTable instance{} ;
            return instance ;
        }

        void SetTable::clear() {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_.clear() ;
            pimpl->deftable_.clear() ;
        }

        void SetTable::save_asdef() {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->deftable_ = pimpl->table_ ;
        }

        void SetTable::reset_todef() {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_ = pimpl->deftable_ ;
        }

        void SetTable::set(const std::string& name, bool val) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[name] = Param(name, val) ;
        }
        void SetTable::set(const std::string& name, long val) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[name] = Param(name, val) ;
        }
        void SetTable::set(const std::string& name, float val) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[name] = Param(name, val) ;
        }
        void SetTable::set(const std::string& name, std::string&& val) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[name] = Param(name, std::move(val)) ;
        }
        void SetTable::set(const std::string& name, const std::string& val) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[name] = Param(name, val) ;
        }

        void SetTable::set(const Param& param) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[param.name()] = param ;
        }
        void SetTable::set(Param&& param) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[param.name()] = std::move(param) ;
        }

        const Param& SetTable::get(const std::string& name) const {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            return pimpl->table_.at(name) ;
        }

        void SetTable::remove(const std::string& name) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_.erase(name) ;
        }
    }
}

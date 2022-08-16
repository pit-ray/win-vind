#include "maptable.hpp"

#include "cmdparser_old.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"

#include <array>
#include <mutex>
#include <unordered_map>


namespace vind
{
    namespace core
    {
        struct MapTable::Impl {
            ModeArray<std::unordered_map<std::size_t, Map>> table_{} ;
            ModeArray<std::unordered_map<std::size_t, Map>> deftable_{} ;
            std::mutex mtx_{} ;
        } ;

        MapTable::MapTable()
        : pimpl(std::make_unique<Impl>())
        {}

        MapTable::~MapTable() noexcept = default ;

        MapTable& MapTable::get_instance() {
            static MapTable instance{} ;
            return instance ;
        }

        void MapTable::save_asdef() {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->deftable_ = pimpl->table_ ;
        }

        void MapTable::reset_todef() {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_ = pimpl->deftable_ ;
        }

        void MapTable::add_map(
                const std::string& trigger_cmd,
                const std::string& target_cmd,
                Mode mode) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            auto mi = static_cast<int>(mode) ;
            Map map(trigger_cmd, target_cmd, true) ;
            pimpl->table_[mi][map.in_hash()] = std::move(map) ;
        }

        void MapTable::add_noremap(
                const std::string& trigger_cmd,
                const std::string& target_cmd,
                Mode mode) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            auto mi = static_cast<int>(mode) ;
            Map map(trigger_cmd, target_cmd, false) ;
            pimpl->table_[mi][map.in_hash()] = std::move(map) ;
        }

        void MapTable::add(const Map& map, Mode mode) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            auto mi = static_cast<int>(mode) ;
            pimpl->table_[mi][map.in_hash()] = map ;
        }

        void MapTable::add(Map&& map, Mode mode) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            auto mi = static_cast<int>(mode) ;
            pimpl->table_[mi][map.in_hash()] = std::move(map) ;
        }

        const Map& MapTable::get(
                const std::string& trigger_cmd,
                Mode mode) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            auto mi = static_cast<int>(mode) ;
            auto hash = Map::compute_hash(trigger_cmd) ;
            return pimpl->table_[mi].at(hash) ;
        }

        void MapTable::remove(
                const std::string& trigger_cmd,
                Mode mode) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            auto mi = static_cast<int>(mode) ;
            auto hash = Map::compute_hash(trigger_cmd) ;
            pimpl->table_[mi].erase(hash) ;
        }

        void MapTable::clear(Mode mode) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            auto mi = static_cast<int>(mode) ;
            pimpl->table_[mi].clear() ;
        }

        void MapTable::clear_all() {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            decltype(pimpl->table_)().swap(pimpl->table_) ;
            decltype(pimpl->deftable_)().swap(pimpl->deftable_) ;
        }

        std::vector<Map> MapTable::get_noremaps(Mode mode) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            auto mi = static_cast<int>(mode) ;

            std::vector<Map> out{} ;
            out.reserve(pimpl->table_[mi].size() / 2) ;
            for(const auto& [hash, map] : pimpl->table_[mi]) {
                if(map.is_noremap()) {
                    out.push_back(map) ;
                }
            }
            return out ;
        }

        std::vector<Map> MapTable::get_remaps(Mode mode) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            auto mi = static_cast<int>(mode) ;

            std::vector<Map> out{} ;
            out.reserve(pimpl->table_[mi].size() / 2) ;
            for(const auto& [hash, map] : pimpl->table_[mi]) {
                if(map.is_map()) {
                    out.push_back(map) ;
                }
            }
            return out ;
        }

        std::vector<Map> MapTable::get_allmaps(Mode mode) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            auto mi = static_cast<int>(mode) ;

            std::vector<Map> out{} ;
            out.reserve(pimpl->table_[mi].size()) ;
            for(const auto& [hash, map] : pimpl->table_[mi]) {
                out.push_back(map) ;
            }
            return out ;
        }

        struct Map::Impl {
            std::string istr_ ;
            Command icmd_ ;
            std::size_t ihash_ ;

            std::string ostr_ ;
            Command ocmd_ ;
            std::size_t ohash_ ;

            bool remappable_ ;

            template <typename T1, typename T2>
            explicit Impl(
                    T1&& in,
                    T2&& out,
                    bool remappable)
            : istr_(std::forward<T1>(in)),
              icmd_(parse_command_old(istr_)),
              ihash_(0),
              ostr_(std::forward<T2>(out)),
              ocmd_(parse_command_old(ostr_)),
              ohash_(0),
              remappable_(remappable)
            {}

            explicit Impl()
            : istr_(),
              icmd_(),
              ihash_(0),
              ostr_(),
              ocmd_(),
              ohash_(0),
              remappable_(false)
            {}
        } ;

        Map::Map()
        : pimpl(std::make_shared<Impl>())
        {}

        Map::Map(
                const std::string& in,
                const std::string& out,
                bool allow_remap)
        : pimpl(std::make_shared<Impl>(in, out, allow_remap))
        {
            if(pimpl->istr_.empty()) {
                throw LOGIC_EXCEPT("Invalid trigger command") ;
            }

            if(pimpl->ostr_.empty()) {
                throw LOGIC_EXCEPT("Invalid target command") ;
            }
        }

        bool Map::is_noremap() const noexcept {
            return !pimpl->remappable_ ;
        }

        bool Map::is_map() const noexcept {
            return pimpl->remappable_ ;
        }

        const Command& Map::trigger_command() const noexcept {
            return pimpl->icmd_ ;
        }

        const std::string& Map::trigger_command_string() const noexcept {
            return pimpl->istr_ ;
        }

        const Command& Map::target_command() const {
            return pimpl->ocmd_ ;
        }

        const std::string& Map::target_command_string() const noexcept {
            return pimpl->ostr_ ;
        }

        bool Map::empty() const noexcept {
            return pimpl->icmd_.empty() || pimpl->ostr_.empty() ;
        }

        std::size_t Map::out_hash() const {
            if(pimpl->ohash_ == 0) {
                pimpl->ohash_ = compute_hash(pimpl->ocmd_) ;
            }
            return pimpl->ohash_ ;
        }

        std::size_t Map::in_hash() const {
            if(pimpl->ihash_ == 0) {
                pimpl->ihash_ = compute_hash(pimpl->icmd_) ;
            }
            return pimpl->ihash_ ;
        }

        std::size_t Map::compute_hash(const std::string& strcmd) {
            return compute_hash(parse_command_old(strcmd)) ;
        }

        std::size_t Map::compute_hash(const Command& cmd) {
            /**
             * TODO: The unsigned char keycodes are considered as 
             *       as 8-bit characters, and compute its hash by
             *       std::string. This should be a more efficient
             *       method in the future.
             */
            std::string strcmd{} ;
            for(const auto& set : cmd) {
                for(const auto& key : set) {
                    strcmd.push_back(key.to_code()) ;
                }
            }
            return std::hash<std::string>()(std::move(strcmd)) ;
        }

        bool Map::operator==(Map&& rhs) const {
            return in_hash() == rhs.in_hash() && out_hash() == rhs.out_hash() ;
        }

        bool Map::operator==(const Map& rhs) const {
            return in_hash() == rhs.in_hash() && out_hash() == rhs.out_hash() ;
        }

        bool Map::operator!=(Map&& rhs) const {
            return in_hash() != rhs.in_hash() || out_hash() != rhs.out_hash() ;
        }

        bool Map::operator!=(const Map& rhs) const {
            return in_hash() != rhs.in_hash() || out_hash() != rhs.out_hash() ;
        }
    } // namespace core
} // namespace vind

#include "bind/base/bindings_matcher.hpp"

#include <iostream>
#include <limits>
#include <mutex>

#include "coreio/err_logger.hpp"
#include "key/key_log.hpp"
#include "bind/base/key_logger_base.hpp"
#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"

namespace
{
    using namespace vind ;
    inline bool is_num_only(const KeyLog& log) noexcept {
        for(auto& key : log) {
            if(!keycodecvt::is_number(key)) {
                return false ;
            }
        }
        return true ;
    }
}


namespace vind
{
    struct BindingsMatcher::Impl {
        cmdlist_t cmdlist_ ;
        bool keyset_is_matched_ ;
        bool accepted_ ;
        std::size_t opt_any_idx_ ;
        std::size_t optnum_begin_idx_ ;
        std::size_t optnum_end_idx_ ;
        bool optnum_is_last_ ;
        bool matched_syskey_in_combined_bind_ ;
        std::mutex mtx_ ;

        explicit Impl(cmdlist_t&& cl)
        : cmdlist_(std::move(cl)),
          keyset_is_matched_(true),
          accepted_(false),
          opt_any_idx_(std::numeric_limits<std::size_t>::max()),
          optnum_begin_idx_(std::numeric_limits<std::size_t>::max()),
          optnum_end_idx_(std::numeric_limits<std::size_t>::max()),
          optnum_is_last_(false),
          matched_syskey_in_combined_bind_(false),
          mtx_()
        {}

        explicit Impl(const cmdlist_t& cl)
        : cmdlist_(cl),
          keyset_is_matched_(true),
          accepted_(false),
          opt_any_idx_(std::numeric_limits<std::size_t>::max()),
          optnum_begin_idx_(std::numeric_limits<std::size_t>::max()),
          optnum_end_idx_(std::numeric_limits<std::size_t>::max()),
          optnum_is_last_(false),
          matched_syskey_in_combined_bind_(false),
          mtx_()
        {}

        void reset() {
            accepted_         = false ;
            keyset_is_matched_     = true ;
            opt_any_idx_      = std::numeric_limits<std::size_t>::max() ;
            optnum_is_last_   = false ;
            optnum_begin_idx_ = std::numeric_limits<unsigned int>::max() ;
            optnum_end_idx_   = std::numeric_limits<unsigned int>::max() ;
        }

        virtual ~Impl() noexcept {
            cmdlist_.clear() ;
        }

        Impl(Impl&&)                 = delete ;
        Impl& operator=(Impl&&)      = delete ;
        Impl(const Impl&)            = delete ;
        Impl& operator=(const Impl&) = delete ;
    } ;

    BindingsMatcher::BindingsMatcher(BindingsMatcher::cmdlist_t&& keyset)
    : pimpl(std::make_unique<Impl>(std::move(keyset)))
    {}

    BindingsMatcher::BindingsMatcher(const cmdlist_t& keyset)
    : pimpl(std::make_unique<Impl>(keyset))
    {}

    BindingsMatcher::~BindingsMatcher() noexcept = default ;
    BindingsMatcher::BindingsMatcher(BindingsMatcher&&)            = default ;
    BindingsMatcher& BindingsMatcher::operator=(BindingsMatcher&&) = default ;

    bool BindingsMatcher::is_optional_area(std::size_t seqidx) const noexcept {
        return seqidx >= pimpl->opt_any_idx_ ;
    }

    unsigned int BindingsMatcher::accept_with_high_priority() const noexcept {
        pimpl->accepted_     = true ;
        pimpl->keyset_is_matched_ = true ;
        return std::numeric_limits<unsigned int>::max() ;
    }

    //If the latest log is in a range of <num>?
    bool BindingsMatcher::is_optional_num_area(std::size_t seqidx) const noexcept {
        return pimpl->optnum_begin_idx_ <= seqidx && seqidx <= pimpl->optnum_end_idx_ ;
    }

    void BindingsMatcher::regard_numbers_as_one_keycode(std::size_t& seqidx) const noexcept {
        seqidx -= (pimpl->optnum_end_idx_ - pimpl->optnum_begin_idx_) ;

        //
        // Ex)
        // command: AB<num>C
        //  ______________________________________________________________
        // |              |     |     |              |              |     |
        // |  latest_log  |  A  |  B  |       1      |      2       |  C  |
        // |--------------|-----|-----|--------------|--------------|-----|
        // |    seqidx    |  0  |  1  |       2      |      3       |  4  |
        // |--------------|-----|-----|--------------|--------------|-----|
        // | optnum_begin | max | max |       2      |      2       |  2  |
        // |--------------|-----|-----|--------------|--------------|-----|
        // | optnum_end   | max | max |      max     |     max      |  3  |
        // |--------------|-----|-----|--------------|--------------|-----|
        // | fixed seqidx |  0  |  1  |  2(in range) |  3(in range) |  3  |
        // |______________|_____|_____|______________|______________|_____|
        //
    }

    /*
     * -- Key matching system like Automata --
     * [code_existed == true] state transitioned successfully
     * [accepted     == true] reached accepting state
     *
     */
    unsigned int BindingsMatcher::compare_onelog(const KeyLog& log, std::size_t seqidx) const {
        //By the way, mutex is already applied in a parent scope.

        if(is_optional_area(seqidx)) {
            return accept_with_high_priority() ;
        }

        const auto is_log_num_only = is_num_only(log) ;

        if(is_optional_num_area(seqidx)) {
            //Whether there are some numbers in the latest log?
            if(is_log_num_only) {
                pimpl->keyset_is_matched_ = true ;
                pimpl->accepted_     = pimpl->optnum_is_last_ ;
                return 1 ;
            }
            else {
                pimpl->optnum_end_idx_ = seqidx - 1 ;
            }
        }

        regard_numbers_as_one_keycode(seqidx) ;

        unsigned int most_matched_num = 0 ;

        pimpl->keyset_is_matched_ = false ;

        // Example) cmd : `gT` <- cmdlist : [`gT`, `gt`, `gg`, `G`]
        for(const auto& cmd : pimpl->cmdlist_) { 
            try {
                unsigned int matched_num = 0 ;
                const auto& keyset = cmd.at(seqidx) ; // Example) `T` <- `gT`  (seqidx==1)

                // Example)  keycode : KEYCODE_SHIFT <- {KEYCODE_SHIFT, KEYCODE_T}
                for(auto itr = keyset.cbegin() ; itr != keyset.cend() ; itr ++) {
                    if(*itr == KEYCODE_OPTIONAL) { //<any>
                        pimpl->opt_any_idx_ = seqidx ;
                        return accept_with_high_priority() ;
                    }
                    if(*itr == KEYCODE_OPTNUMBER) { //<num>
                        if(is_log_num_only) {
                            pimpl->optnum_begin_idx_ = seqidx ;
                            pimpl->optnum_is_last_ = (cmd.size() - 1 - seqidx) == 0 ; //Whether `a<num>` or `a<num>b`?
                            matched_num ++ ;
                            continue ;
                        }
                    }
                    if(log.is_containing(*itr)) {
                        matched_num ++ ;

                        // Is keyset combined bindings?
                        if(keyset.size() > 1) {
                            if(itr == keyset.cbegin()) {
                                pimpl->matched_syskey_in_combined_bind_ = true ;
                            }
                            else {
                                pimpl->matched_syskey_in_combined_bind_ = false ;
                            }
                        }
                    }
                }

                // log is the same as keyset(`T` in `gT`, seqidx==1)
                if(matched_num == keyset.size()) {
                    pimpl->keyset_is_matched_ = true ;

                    if(most_matched_num < matched_num) {
                        most_matched_num = matched_num ;
                    }

                    // `gT` is accepted
                    if(seqidx == (cmd.size() - 1)) {
                        pimpl->accepted_ = true ;
                    }
                }
            }
            catch(const std::out_of_range&) {
                continue ;
            }
        }

        return most_matched_num ;
    }

    void BindingsMatcher::calibrate_range_params(std::size_t seqidx) const noexcept {
        //is not in <any> range
        if(seqidx < pimpl->opt_any_idx_) {
            pimpl->opt_any_idx_ = std::numeric_limits<std::size_t>::max() ;
        }

        //is not in <num> range
        if(seqidx < pimpl->optnum_begin_idx_) {
            pimpl->optnum_is_last_   = false ; 
            pimpl->optnum_begin_idx_ = std::numeric_limits<std::size_t>::max() ;
        }

        if(seqidx < pimpl->optnum_end_idx_) {
            pimpl->optnum_end_idx_ = std::numeric_limits<std::size_t>::max() ;
        }
    }

    unsigned int BindingsMatcher::compare_to_latestlog(const KeyLoggerBase& lgr) const {
        std::lock_guard<std::mutex> scoped_lock(pimpl->mtx_) ;

        pimpl->accepted_ = false ;

        if(lgr.empty()) return 0 ;

        const auto seqidx = lgr.size() - 1 ;
        if(lgr.size() == 1) {
            pimpl->keyset_is_matched_ = true ;
        }

        //The search is not needed anymore.
        if(!pimpl->keyset_is_matched_) return 0 ;

        calibrate_range_params(seqidx) ;

        return compare_onelog(lgr.latest(), seqidx) ;
    }

    unsigned int BindingsMatcher::compare_to_alllog(const KeyLoggerBase& lgr) const {
        std::lock_guard<std::mutex> scoped_lock(pimpl->mtx_) ;

        pimpl->accepted_ = false ;

        if(lgr.empty()) return 0 ;

        pimpl->reset() ;

        unsigned int result = 0 ;
        for(std::size_t i = 0 ; i < lgr.size() ; i ++) {
            result = compare_onelog(lgr.at(i), i) ;

            if(pimpl->accepted_ || !pimpl->keyset_is_matched_) break ;
        }
        return result ;
    }

    bool BindingsMatcher::is_accepted() const noexcept {
        return pimpl->accepted_ ;
    }

    bool BindingsMatcher::is_matched_syskey_in_combined_bindings() const noexcept {
        return pimpl->matched_syskey_in_combined_bind_ ;
    }
}

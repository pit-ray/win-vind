#include "hintassign.hpp"

#include "util/math.hpp"
#include "util/string.hpp"

#include <vector>


namespace vind
{
    namespace core
    {
        void assign_identifier_hints(
                std::size_t target_count,
                std::vector<Hint>& hint_codes,
                std::vector<std::string>& hint_labels,
                const std::string& hintkeys) {
            hint_codes.resize(target_count) ;
            hint_labels.resize(target_count) ;

            const auto hint_length = static_cast<int>(
                util::log(hintkeys.size(), target_count - 1)) + 1 ;
            std::vector<std::size_t> indices(hint_length, 0) ;

            // Consider a hint as a number in base n and
            // compute a number sequence without duplicates.
            for(std::size_t i = 0 ; i < target_count ; i ++) {
                std::size_t num = i ;

                for(int j = 0 ; j < hint_length ; j ++) {
                    indices[j] = num % hintkeys.size() ;
                    num /= hintkeys.size() ;
                }

                std::string str ;
                for(int j = hint_length - 1 ; j >= 0; j --) {
                    auto c = hintkeys[indices[j]] ;
                    hint_codes[i].emplace_back(c) ;
                    hint_labels[i] += util::a2A(c) ;
                }
            }
        }
    }
}

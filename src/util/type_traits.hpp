#ifndef _TYPE_TRAITS_HPP
#define _TYPE_TRAITS_HPP

#include <cstddef>


namespace vind
{
    namespace util {
        template <bool... Bs>
        using bool_sequence = std::integer_sequence<bool, Bs...> ;

        template <bool... Bs>
        using bool_and = std::is_same<bool_sequence<Bs...>, bool_sequence<(Bs || true)...>> ;

        //used De Morgan's lows
        template <bool... Bs>
        using bool_or = std::integral_constant<bool, !bool_and<!Bs...>::value> ;

        template <typename T, typename... Ts>
        using are_same = bool_and<std::is_same<T, Ts>::value...> ;

        template <typename T>
        inline T enum_and(T rhs, T lhs) noexcept {
            return static_cast<T>(
                    static_cast<std::size_t>(rhs) & static_cast<std::size_t>(lhs)) ;
        }

        template <typename T>
        inline T enum_and(std::size_t rhs, T lhs) noexcept {
            return static_cast<T>(
                    rhs & static_cast<std::size_t>(lhs)) ;
        }

        template <typename T>
        inline T enum_and(T rhs, std::size_t lhs) noexcept {
            return static_cast<T>(
                    static_cast<std::size_t>(rhs) & lhs) ;
        }


        template <typename T>
        inline T enum_or(T rhs, T lhs) noexcept {
            return static_cast<T>(
                    static_cast<std::size_t>(rhs) | static_cast<std::size_t>(lhs)) ;
        }

        template <typename T>
        inline T enum_or(std::size_t rhs, T lhs) noexcept {
            return static_cast<T>(
                    rhs | static_cast<std::size_t>(lhs)) ;
        }

        template <typename T>
        inline T enum_or(T rhs, std::size_t lhs) noexcept {
            return static_cast<T>(
                    static_cast<std::size_t>(rhs) | lhs) ;
        }

        template <typename T>
        inline bool enum_has_bits(T rhs, T lhs) noexcept {
            return static_cast<bool>(
                    static_cast<std::size_t>(rhs) & static_cast<std::size_t>(lhs)) ;
        }

        template <typename T>
        inline bool enum_has_bits(std::size_t rhs, T lhs) noexcept {
            return static_cast<bool>(
                    rhs & static_cast<std::size_t>(lhs)) ;
        }

        template <typename T>
        inline bool enum_has_bits(T rhs, std::size_t lhs) noexcept {
            return static_cast<bool>(
                    static_cast<std::size_t>(rhs) & lhs) ;
        }
    }
}

#endif

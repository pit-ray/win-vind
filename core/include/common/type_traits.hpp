#ifndef _TYPE_TRAITS_HPP
#define _TYPE_TRAITS_HPP

namespace TypeTraits
{
    template <bool... Bs>
    using bool_sequence = std::integer_sequence<bool, Bs...> ;

    template <bool... Bs>
    using bool_and = std::is_same<bool_sequence<Bs...>, bool_sequence<(Bs || true)...>> ;

    //used De Morgan's lows
    template <bool... Bs>
    using bool_or = std::integral_constant<bool, !bool_and<!Bs...>::value> ;

    template <typename T, typename... Ts>
    using are_same = bool_and<std::is_same<T, Ts>::value...> ;
}

#endif

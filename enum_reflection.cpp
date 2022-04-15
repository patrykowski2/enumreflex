#include <cstddef>
#include <type_traits>
#include <string_view>

// Enumeration type_traits extension

namespace details {

template <typename _T, bool = std::is_enum_v<_T>> struct is_scoped_enum_impl : std::false_type {}; 
template <typename _T> struct is_scoped_enum_impl<_T, true> : 
std::bool_constant<!std::is_convertible_v<_T, std::underlying_type_t<_T>>> {};

}

template <typename _T> struct is_scoped_enum : details::is_scoped_enum_impl<_T> {};
template <typename _T> inline constexpr bool is_scoped_enum_v = is_scoped_enum<_T>::value;

namespace details {

template <typename _T, bool = std::is_enum_v<_T>> struct is_unscoped_enum_impl : std::false_type {};
template <typename _T> struct is_unscoped_enum_impl<_T, true> : std::bool_constant<!is_scoped_enum_v<_T>> {};

}

template <typename _T> struct is_unscoped_enum : details::is_unscoped_enum_impl<_T> {};
template <typename _T> inline constexpr bool is_unscoped_enum_v = is_unscoped_enum<_T>::value;

// Enumeration reflection implementation

namespace details {

template <typename InputIter, typename CharacterType> 
static constexpr InputIter find_character(InputIter begin, InputIter end, CharacterType character) {
    for( ; begin != end; ++begin) {
        if(*begin == character) {
            return begin;
        }
    }
    return end;
}

template <typename InputIter, typename UnaryPredicate> 
static constexpr InputIter find_character_if(InputIter begin, InputIter end, UnaryPredicate pred) {
    for( ; begin != end; ++begin) {
        if(pred(*begin)) {
            return begin;
        }
    }
    return end;
}

static constexpr std::string_view clear_function_name(std::string_view string) {
    
    string.remove_prefix(std::distance(find_character(string.rbegin(), string.rend(), '='), string.rend()) + 1);

    return string;
}

template <typename enum_type> static constexpr auto pretty_name() -> std::basic_string_view<char> { 
    return {__PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__) - 2}; 
}

template <auto enum_constant> static constexpr auto pretty_name() -> std::basic_string_view<char> {
    return {__PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__) - 2}; 
}

template <typename enum_type> static constexpr auto get_type_name() ->
typename std::enable_if_t<std::is_enum_v<enum_type>, std::string_view> {

    return clear_function_name(pretty_name<enum_type>());
}

template <auto enum_constant> static constexpr auto get_constant_name() ->
typename std::enable_if_t<is_unscoped_enum_v<decltype(enum_constant)>, std::string_view> {

    return clear_function_name(pretty_name<enum_constant>());
}

template <auto enum_constant> static constexpr auto get_constant_name() ->
typename std::enable_if_t<is_scoped_enum_v<decltype(enum_constant)>, std::string_view> {

    // TODO : Add optional prefix removal

    return clear_function_name(pretty_name<enum_constant>());
}

template <typename enum_type> static constexpr auto get_underlying_type_name() ->
typename std::enable_if_t<std::is_enum_v<enum_type>, std::string_view> {
    
    return clear_function_name(pretty_name<std::underlying_type_t<enum_type>>());
}

template <auto enum_constant> static constexpr auto get_underlying_value() -> 
typename std::enable_if_t<std::is_enum_v<decltype(enum_constant)>, std::string_view> {

    return clear_function_name(pretty_name<static_cast<std::underlying_type_t<decltype(enum_constant)>>(enum_constant)>());
}

template <auto enum_constant> static constexpr bool is_valid_enum_constant() {

    auto enum_constant_name = clear_function_name(pretty_name<enum_constant>());
    auto underlying_value_name = clear_function_name(pretty_name<static_cast<std::underlying_type_t<decltype(enum_constant)>>(enum_constant)>());

    auto is_valid_value_name = [](char character){ return !((character >= '0' && character <= '9') || character == '-' || character == '+'); };

    enum_constant_name.remove_prefix(std::distance(find_character_if(enum_constant_name.rbegin(), enum_constant_name.rend(), is_valid_value_name), enum_constant_name.rend()));

    return !(enum_constant_name == underlying_value_name);
}}

template <auto enum_constant, typename enum_type = decltype(enum_constant)> struct enumreflex {
    inline static constexpr std::string_view type_name = details::get_type_name<enum_type>();
    inline static constexpr std::string_view constant_name = details::get_constant_name<enum_constant>();
    inline static constexpr std::string_view underlying_type_name = details::get_underlying_type_name<enum_type>();
    inline static constexpr std::string_view underlying_value = details::get_underlying_value<enum_constant>();
};
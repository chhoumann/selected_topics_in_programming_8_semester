#ifndef STATIC_VISITOR_META_HPP
#define STATIC_VISITOR_META_HPP

#include <type_traits>
#include <string>

/**
 * TODO: implement meta-predicates
 * (template classes/variables taking type and returning bool)
 * is_bool_v
 * is_number_v
 * is_character_v
 * is_string_v
 * is_container_v
 * accepts_v
 *
 * Tips:
 * - read documentation about std::is_same, std::is_integral, std::is_floating_point, std::is_arithmetic.
 * - make your meta-predicates robust against const/volatile/reference combinations (see meta_test.cpp):
 *   see std::remove_reference, std::remove_const, std::remove_cvref, std::decay
 */
template <typename T>
constexpr bool is_bool_v = std::is_same_v<bool, std::remove_cvref_t<T>>;

template <typename T>
constexpr bool is_number_v = std::is_arithmetic_v<std::remove_cvref_t<T>> && !is_bool_v<T>;

template <typename T>
constexpr bool is_character_v = std::is_same_v<std::remove_cvref_t<T>, char>;

template <typename T>
constexpr bool is_string_v =
    std::is_same_v<std::remove_cvref_t<T>, std::string> || std::is_same_v<std::remove_cvref_t<T>, char*> ||
    std::is_same_v<std::remove_cvref_t<T>, const char*> ||
    (std::is_array_v<std::remove_cvref_t<T>> && std::is_same_v<std::remove_extent_t<std::remove_cvref_t<T>>, char>);

template <typename T>
concept container = requires(T t) {
    std::begin(t);
    std::end(t);
};
template <typename T>
// FIXME: use concepts, require std::begin, std::end, and not is_string_v
constexpr auto is_container_v = (container<T> || std::is_array_v<std::remove_cvref_t<T>>)&&!is_string_v<T>;

template <typename Data, typename Visitor>
// FIXME: check for Data::accept(Visitor) by using concepts or detection toolkit
constexpr auto accepts_v = requires(Data data, Visitor visitor) { data.accept(visitor); };

#endif  // STATIC_VISITOR_META_HPP

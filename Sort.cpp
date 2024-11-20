#include <tuple>
#include <type_traits>

template <typename T>
struct is_sorted {
    static constexpr bool value = true;
};

template <typename T, typename U, typename... Rest>
struct is_sorted<std::tuple<T, U, Rest...>> {
    static constexpr bool value =
        std::less<T>()(std::get(std::tuple<T, U, Rest...> {}), std::get(std::tuple<T, U, Rest...> {})) &&
        is_sorted<std::tuple<U, Rest...>>::value;
};

template <typename Tuple>
struct sort_impl;

template <typename T>
struct sort_impl<std::tuple<T>> {};

template <typename T, typename U, typename... Rest>
struct sort_impl<std::tuple<T, U, Rest...>> {
    using type = std::conditional_t<
        std::less<T>()(std::get(std::tuple<T, U, Rest...> {}), std::get(std::tuple<T, U, Rest...> {})),
        typename sort_impl<std::tuple<U, T, Rest...>>::type,
        typename sort_impl<std::tuple<T, Rest..., U>>::type>;
};

template <typename Tuple>
using sort = typename sort_impl<Tuple>::type;

int main() {
    using my_tuple = std::make_tuple<3, 1, 4, 1, 5, 9, 2, 6, 5, 3>;
    using sorted_tuple = sort<my_tuple>;

    // sorted_tuple 现在是一个排序后的 tuple 类型
    static_assert(is_sorted<sorted_tuple>::value, "Sorting failed");
}
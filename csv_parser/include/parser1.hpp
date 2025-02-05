#pragma once

#include <iostream>
#include <tuple>
#include <utility>

template<std::size_t Index, typename Tuple>
void print_tuple_element(std::ostream& os, const Tuple& t) {
    if constexpr (Index > 0) {
        os << ", ";
    }
    os << std::get<Index>(t);
}

template<std::size_t Index = 0, typename... Args>
void print_tuple(std::ostream& os, const std::tuple<Args...>& t) {
    if constexpr (Index < sizeof...(Args)) {
        print_tuple_element<Index>(os, t);
        print_tuple<Index + 1>(os, t);
    }
}

template<typename Ch, typename Tr, typename... Args>
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
    print_tuple(os, t);
    return os;
}
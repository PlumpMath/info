#pragma once
#include <variant>
#include <unordered_map>
#include <type_traits>
#include <vector>

namespace json {

class null {};
using boolean = bool;
using number = double;
using string = std::string;

template <typename T>
using array_t = std::vector<T>;

template <typename T>
using object_t = std::unordered_map<string, T>;

template <typename T>
using data = std::variant<null, boolean, number, string, array_t<T>, object_t<T>>;

class value;

using array = array_t<value>;
using object = object_t<value>;

template <typename T>
struct traits;

template <typename T>
struct is_number {
  static constexpr bool value = std::is_integral<T>::value && !std::is_same<T, bool>::value;
};

}  // namespace json
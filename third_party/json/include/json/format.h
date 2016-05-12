#pragma once
#include <json/types.h>
#include <ostream>

namespace json {
namespace detail {

inline char utf16_hex_char(unsigned char c)
{
  switch (c & 0x0F) {
  case 0x0: return '0';
  case 0x1: return '1';
  case 0x2: return '2';
  case 0x3: return '3';
  case 0x4: return '4';
  case 0x5: return '5';
  case 0x6: return '6';
  case 0x7: return '7';
  case 0x8: return '8';
  case 0x9: return '9';
  case 0xA: return 'a';
  case 0xB: return 'b';
  case 0xC: return 'c';
  case 0xD: return 'd';
  case 0xE: return 'e';
  case 0xF: return 'f';
  }
  return '0';
}

}  // namespace detail

template <typename Value>
std::ostream& format(std::ostream& os, const Value& v, std::size_t offset)
{
  switch (v.data_.template which()) {
  case 0: return format(os, v.data_.template get<null>(), offset);
  case 1: return format(os, v.data_.template get<boolean>(), offset);
  case 2: return format(os, v.data_.template get<number>(), offset);
  case 3: return format(os, v.data_.template get<string>(), offset);
  case 4: return format(os, v.data_.template get<array>(), offset);
  case 5: return format(os, v.data_.template get<object>(), offset);
  default: break;
  }
  return os;
}

template <>
inline std::ostream& format<null>(std::ostream& os, const null& v, std::size_t)
{
  return os << "null";
}

template <>
inline std::ostream& format<boolean>(std::ostream& os, const boolean& v, std::size_t)
{
  return os << (v ? "true" : "false");
}

template <>
inline std::ostream& format<number>(std::ostream& os, const number& v, std::size_t)
{
  return os << v;
}

template <>
inline std::ostream& format<string>(std::ostream& os, const string& v, std::size_t)
{
  os << '"';
  for (auto it = v.begin(); it != v.end(); ++it) {
    char c = *it;
    if ((c & 0x80) == 0) {
      switch (c) {
      case '"': os << "\\\""; break;
      case '\\': os << "\\\\"; break;
      case '\b': os << "\\b"; break;
      case '\f': os << "\\f"; break;
      case '\n': os << "\\n"; break;
      case '\r': os << "\\r"; break;
      case '\t': os << "\\t"; break;
      default:
        if (c < 0x20 || c == 0x7F || (static_cast<unsigned char>(c) >= 0x80 && static_cast<unsigned char>(c) <= 0x9F)) {
          os << "\\u00" << detail::utf16_hex_char(static_cast<unsigned char>(c) >> 4)
            << detail::utf16_hex_char(static_cast<unsigned char>(c));
        } else {
          os << c;
        }
        break;
      }
    } else if ((c & 0xE0) == 0xC0) {
      ++it;
      if (it == v.end() || ((*it) & 0xC0) != 0x80) {
        break;
      }
      os << c << *it;
    } else if ((c & 0xF0) == 0xE0) {
      ++it;
      if (it == v.end() || ((*it) & 0xC0) != 0x80) {
        break;
      }
      char c2 = *it;
      ++it;
      if (it == v.end() || ((*it) & 0xC0) != 0x80) {
        break;
      }
      os << c << c2 << *it;
    } else if ((c & 0xF8) == 0xF0) {
      ++it;
      if (it == v.end() || ((*it) & 0xC0) != 0x80) {
        break;
      }
      char c2 = *it;
      ++it;
      if (it == v.end() || ((*it) & 0xC0) != 0x80) {
        break;
      }
      char c3 = *it;
      ++it;
      if (it == v.end() || ((*it) & 0xC0) != 0x80) {
        break;
      }
      os << c << c2 << c3 << *it;
    } else {
      break;
    }
  }
  return os << '"';
}

template <typename Value>
inline std::ostream& format(std::ostream& os, const array_t<Value>& v, std::size_t offset)
{
  if (v.empty()) {
    return os << "[]";
  }
  os << '[';
  for (auto it = v.begin(); it != v.end(); ++it) {
    if (it != v.begin()) {
      os << ',';
    }
    os << '\n' << std::string(offset + 2, ' ');
    format(os, *it, offset + 2);
  }
  os << '\n' << std::string(offset, ' ');
  return os << ']';
}

template <typename Value>
inline std::ostream& format(std::ostream& os, const object_t<Value>& v, std::size_t offset)
{
  if (v.empty()) {
    return os << "{}";
  }
  os << '{';
  for (auto it = v.begin(); it != v.end(); ++it) {
    if (it != v.begin()) {
      os << ',';
    }
    os << '\n' << std::string(offset + 2, ' ');
    os << '"';
    format(os, it->first, 0);
    os << "\": ";
    format(os, it->second, offset + 2);
  }
  os << '\n' << std::string(offset, ' ');
  return os << '}';
}

}  // namespace json

#pragma once
#include <json/format.h>
#include <json/types.h>
#include <optional>
#include <stdexcept>

namespace json {

class value {
public:
  value() = default;
  value(value&& other) = default;
  value(const value& other) = default;

  template <typename T>
  explicit value(const T& v)
  {
    traits<T>::assign(*this, v);
  }

  template <std::size_t N>
  explicit value(const char(&STR)[N])
  {
    data_.set<string>(STR);
  }

  explicit value(char* str)
  {
    data_.set<string>(str);
  }
  
  explicit value(const char* str)
  {
    data_.set<string>(str);
  }

  value& operator=(value&& other) = default;
  value& operator=(const value& other) = default;

  template <typename T>
  value& operator=(const T& v)
  {
    traits<T>::assign(*this, v);
    return *this;
  }

  template <std::size_t N>
  value& operator=(const char(&STR)[N])
  {
    data_.set<string>(STR);
    return *this;
  }

  value& operator=(char* str)
  {
    data_.set<string>(str);
    return *this;
  }
  
  value& operator=(const char* str)
  {
    data_.set<string>(str);
    return *this;
  }

  value& operator[](const string& name)
  {
    if (!data_.is<object>()) {
      data_ = object();
    }
    return data_.get<object>()[name];
  }

  const value& operator[](const string& name) const
  {
    if (!data_.is<object>()) {
      throw std::runtime_error("error 1");
    }
    const auto& obj = data_.get<object>();
    auto it = obj.find(name);
    if (it == obj.end()) {
      throw std::runtime_error("error 2");
    }
    return it->second;
  }

  operator bool() const
  {
    return !data_.is<null>();
  }

  template <typename T>
  bool is() const
  {
    return traits<T>::is(*this);
  }

  template <typename T>
  T as() const
  {
    return traits<T>::as(*this);
  }

  template <typename T>
  T as(const T& fallback) const
  {
    if (traits<T>::is(*this)) {
      return traits<T>::as(*this);
    }
    return fallback;
  }

  template <typename T>
  T& get()
  {
    data_.get<T>();
  }

  template <typename T>
  const T& get() const
  {
    data_.get<T>();
  }

  std::size_t size() const
  {
    if (data_.is<array>()) {
      return data_.get<array>().size();
    }
    if (data_.is<object>()) {
      return data_.get<object>().size();
    }
    return 0;
  }

  bool has(const string& name) const
  {
    if (data_.is<object>()) {
      const auto& obj = data_.get<object>();
      return obj.find(name) != obj.end();
    }
    return false;
  }

  value get(std::size_t index) const
  {
    if (data_.is<array>()) {
      const auto& arr = data_.get<array>();
      if (index < arr.size()) {
        return arr[index];
      }
    }
    return {};
  }

  value get(const string& name) const
  {
    if (data_.is<object>()) {
      const auto& obj = data_.get<object>();
      auto it = obj.find(name);
      if (it != obj.end()) {
        return it->second;
      }
    }
    return {};
  }

private:
  data<value> data_;

  friend struct traits<null>;
  friend struct traits<boolean>;
  friend struct traits<number>;
  friend struct traits<string>;
  friend struct traits<array>;
  friend struct traits<object>;

  friend std::ostream& format<value>(std::ostream&, const value&, std::size_t);
};

template <>
struct traits<null> {
  static bool is(const value& value)
  {
    return !value.data_.is<null>();
  }

  static void assign(value& value, const null& v)
  {
    value.data_ = v;
  }

  static null as(const value& value)
  {
    return {};
  }
};

template <>
struct traits<boolean> {
  static bool is(const value& self)
  {
    return self.data_.is<boolean>();
  }

  static void assign(value& self, const boolean& v)
  {
    self.data_.set<boolean>(v);
  }

  static boolean as(const value& self)
  {
    if (self.data_.is<boolean>()) {
      return self.data_.get<boolean>();
    }
    return false;
  }
};

template <>
struct traits<number> {
  static bool is(const value& self)
  {
    return self.data_.is<number>();
  }

  static void assign(value& self, const number& v)
  {
    self.data_.set<number>(v);
  }

  static number as(const value& self)
  {
    if (self.data_.is<number>()) {
      return self.data_.get<number>();
    }
    return 0.0;
  }
};

template <>
struct traits<string> {
  static bool is(const value& self)
  {
    return self.data_.is<string>();
  }

  static void assign(value& self, const string& v)
  {
    self.data_.set<string>(v);
  }

  static string as(const value& self)
  {
    if (self.data_.is<string>()) {
      return self.data_.get<string>();
    }
    return {};
  }
};

template <>
struct traits<array> {
  static bool is(const value& self)
  {
    return self.data_.is<array>();
  }

  static void assign(value& self, const array& v)
  {
    self.data_.set<array>(v);
  }

  static array as(const value& self)
  {
    if (self.data_.is<array>()) {
      return self.data_.get<array>();
    }
    return {};
  }
};

template <>
struct traits<object> {
  static bool is(const value& self)
  {
    return self.data_.is<object>();
  }

  static void assign(value& self, const object& v)
  {
    self.data_.set<object>(v);
  }

  static object as(const value& self)
  {
    if (self.data_.is<object>()) {
      return self.data_.get<object>();
    }
    return {};
  }
};

template <typename T>
struct traits {
  static std::enable_if_t<is_number<T>::value, bool> is(const value& self)
  {
    return self.is<number>();
  }

  static std::enable_if_t<is_number<T>::value, void> assign(value& self, const T& v)
  {
    self = static_cast<number>(v);
  }

  static std::enable_if_t<is_number<T>::value, T> as(const value& self)
  {
    if (self.is<number>()) {
      return static_cast<T>(self.as<number>());
    }
    return 0;
  }
};

template <typename T>
struct traits<std::optional<T>> {
  static bool is(const value& self)
  {
    return self.is<null>() || self.is<T>();
  }

  static void assign(value& self, const std::optional<T>& v)
  {
    if (v) {
      self = v.value();
    } else {
      self = {};
    }
  }

  static std::optional<T> as(const value& self)
  {
    if (self.is<T>()) {
      return self.as<T>();
    }
    return {};
  }
};

inline std::ostream& operator<<(std::ostream& os, const value& v)
{
  return format(os, v, 0);
}

}  // namespace json
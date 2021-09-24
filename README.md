# null terminated `string_view`

`nt_string_view` is `std::string_view`, which is guaranteed to be null-terminated.

- Header only
- Requires C++20 compiler
    - Currently, only GCC is supported.
    - MSVC has a problem with `consteval` constructor and cannot be used

## Example

```cpp
#include <string>
#include <string_view>

// Header of this library
#include <nt_string_view.hpp>

using namespace std::string_view_literals;
using namespace std::string_literals;

int main() {
  ntsv::nt_string_view str1("arg string");    // ok
  ntsv::nt_string_view str2("arg string"sv);  // ok

  auto dyn_str = "std::string"s;
  ntsv::nt_string_view str3(dyn_str);         // ok
  
  constexpr auto strview = "std::string_view"sv;
  ntsv::nt_string_view str4(strview.substr(0, 3));  // ng
  
  ntsv::nt_string_view str5("arg string"s);   // ng
  
  constexpr char arr[] = {'s', 't', 'r', 'i', 'n', 'g'};
  ntsv::nt_string_view str6(arr);             // ng
}
```

In the case of ng above, a compilation error will occur.

Assumed use cases

```cpp
#include <string>
#include <string_view>

// Header of this library
#include <nt_string_view.hpp>

using namespace std::string_view_literals;
using namespace std::string_literals;

int c_api_wrapper(ntsv::nt_string_view);
  
int main() {
  c_api_wrapper("arg string");    // ok
  c_api_wrapper("arg string"sv);  // ok

  auto dyn_str = "std::string"sv;
  c_api_wrapper(dyn_str);         // ok
}
```

[A quick test (godbolt)](https://godbolt.org/z/efbjo1jcb)

## Document

Basically, it is the same as `std::string_view`. For more information about `std::string_view`, please refer to the following website:

- [std::basic_string_view - cppreference.com](https://en.cppreference.com/w/cpp/string/basic_string_view)
- [std::basic_string_view - cpprefjp](https://cpprefjp.github.io/reference/string_view/basic_string_view.html)

From here, we will describe the differences from `std::string_view`.

### `ntsv::basic_null_terminated_string_view`

The ones in this library are under the `ntsv` namespace.

`ntsv::basic_null_terminated_string_view` is a basic type that uses a character type as a template and corresponds to `std::basic_string_view`.

```cpp
namespace ntsv {

  template<typename CharT>
  class basic_null_terminated_string_view : private std::basic_string_view<CharT> {
    // ...
  };
}
```

As with `std::string_view`, a specialized alias is provided for each character type.

```cpp
namespace ntsv {

  // ...

  using nt_string_view = basic_null_terminated_string_view<char>;
  using nt_wstring_view = basic_null_terminated_string_view<wchar_t>;
  using nt_u8string_view = basic_null_terminated_string_view<char8_t>;
  using nt_u16string_view = basic_null_terminated_string_view<char16_t>;
  using nt_u32string_view = basic_null_terminated_string_view<char32_t>;

}
```

### Constructor

As for the constructor, it is completely different from `std::string_view`. It is forbidden to construct from anything that is not guaranteed to be null-terminated.

```cpp
using base = std::basic_string_view<CharT>;

// (1) Default constructor
basic_null_terminated_string_view() = default;

// (2) Constructor to construct from std::string
template<typename Allocator>
constexpr basic_null_terminated_string_view(const std::basic_string<CharT, std::char_traits<CharT>, Allocator>& str) noexcept
  : base(str)
{}

// (3) Constructors that construct from std::string, this is not allowed
template<typename Allocator>
basic_null_terminated_string_view(std::basic_string<CharT, std::char_traits<CharT>, Allocator>&&) = delete;

// (4) Constructor to construct from string literal
template<std::size_t N>
consteval basic_null_terminated_string_view(const CharT(&str_literal)[N])
  : base(detail::check_null_terminate(str_literal, N))
{}

// (5) Constructor to construct from std::string_view
consteval basic_null_terminated_string_view(std::basic_string_view<CharT> str_view)
  : base(detail::check_null_terminate(str_view.data(), str_view.size() + 1))
{}
```

Of these, only two can be called at runtime, (1) and (2), while the remaining constructor can only be used at compile time.

In the constructors of (4)(5), a compile error will occur if the null terminator cannot be confirmed.

### Added function

In `basic_null_terminated_string_view`, we have added the following three functions

- `operator std::basic_string_view<CharT>()`
    - Implicit conversion to `std::basic_string_view` of the same character type
- `.str_view()`
    - Explicit conversion function to `std::basic_string_view` of the same character type
- `.c_str()`
    - Get a pointer to a null-terminated string

```cpp
namespace ntsv {

  template<typename CharT>
  class basic_null_terminated_string_view : private std::basic_string_view<CharT> {
  
    // ...

    constexpr operator std::basic_string_view<CharT>() const noexcept {
      return *this;
    }

    constexpr auto str_view() const noexcept -> std::basic_string_view<CharT>{
      return *this;
    }

    constexpr auto c_str() const noexcept -> const CharT* {
      return this->data();
    }
    
    // ...
  };
}
```


### Disabled functions

The use of `std::stirng_view::remove_suffix()` is disabled because it cannot guarantee null termination.

### User defined literals

For simple construction from string literals, a UDL called `_ntsv` is provided.

```cpp
namespace ntsv {
  namespace literals {

    consteval auto operator"" _ntsv(const char* str, std::size_t) -> nt_string_view {
      return nt_string_view{str};
    }

    consteval auto operator"" _ntsv(const wchar_t* str, std::size_t) -> nt_wstring_view {
      return nt_wstring_view{str};
    }

    consteval auto operator"" _ntsv(const char8_t* str, std::size_t) -> nt_u8string_view {
      return nt_u8string_view{str};
    }

    consteval auto operator"" _ntsv(const char16_t* str, std::size_t) -> nt_u16string_view {
      return nt_u16string_view{str};
    }

    consteval auto operator"" _ntsv(const char32_t* str, std::size_t) -> nt_u32string_view {
      return nt_u32string_view{str};
    }

  }
}
```

For example, you can use

```cpp
#include <nt_string_view.hpp>

using namespace ntsv::literals;
  
int main() {
  constexpr auto ntstr = "string literal"_ntsv;
}
```

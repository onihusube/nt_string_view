#pragma once

#include <string_view>
#include <ranges>
#include <compare>
#include <version>
#include <ostream>

namespace ntsv {

  namespace detail {

    template<typename CharT>
    consteval auto check_null_terminate(const CharT* str, std::size_t N) -> std::basic_string_view<CharT> {
      if (str[N - 1] != CharT{}) {
        throw "The string is not null-terminated!";
      }
      return {str, N - 1};
    }
  }
  
  template<typename CharT>
  class basic_null_terminated_string_view : private std::basic_string_view<CharT> {
    using base = std::basic_string_view<CharT>;

  public:
    basic_null_terminated_string_view() = default;

    template<typename Allocator>
    explicit constexpr basic_null_terminated_string_view(const std::basic_string<CharT, std::char_traits<CharT>, Allocator>& str) noexcept
      : base(str)
    {}

    template<typename Allocator>
    explicit basic_null_terminated_string_view(std::basic_string<CharT, std::char_traits<CharT>, Allocator>&&) = delete;

    template<std::size_t N>
    consteval basic_null_terminated_string_view(const CharT(&str_literal)[N])
      : base(detail::check_null_terminate(str_literal, N))
    {}

    explicit consteval basic_null_terminated_string_view(const CharT* ptr)
      : base(ptr)
    {}

    explicit consteval basic_null_terminated_string_view(std::basic_string_view<CharT> str_view)
      : base(detail::check_null_terminate(str_view.data(), str_view.size() + 1))
    {}

    basic_null_terminated_string_view(const basic_null_terminated_string_view &) = default;
    basic_null_terminated_string_view(basic_null_terminated_string_view &&) = default;
    basic_null_terminated_string_view &operator=(const basic_null_terminated_string_view &) = default;
    basic_null_terminated_string_view& operator=(basic_null_terminated_string_view&&) = default;

    constexpr operator std::basic_string_view<CharT>() const noexcept {
      return *this;
    }

    constexpr auto str_view() const noexcept -> std::basic_string_view<CharT>{
      return *this;
    }

    constexpr auto c_str() const noexcept -> const CharT* {
      return this->data();
    }

    friend constexpr std::strong_ordering operator<=>(const basic_null_terminated_string_view &, const basic_null_terminated_string_view &) = default;

    friend std::basic_ostream<CharT> &operator<<(std::basic_ostream<CharT> &os, basic_null_terminated_string_view sv) {
      return os << sv.str_view();
    }

  public:
    // string_viewのインターフェースの有効化
    using base::const_iterator;
    using base::const_pointer;
    using base::const_reference;
    using base::const_reverse_iterator;
    using base::difference_type;
    using base::iterator;
    using base::pointer;
    using base::reference;
    using base::reverse_iterator;
    using base::size_type;
    using base::traits_type;
    using base::value_type;

    using base::begin;
    using base::end;
    using base::cbegin;
    using base::cend;
    using base::rbegin;
    using base::rend;
    using base::crbegin;
    using base::crend;

    using base::size;
    using base::length;
    using base::max_size;
    using base::empty;

    using base::operator[];
    using base::at;
    using base::front;
    using base::back;
    using base::data;

    using base::remove_prefix;
    // 危険！
    //using base::remove_suffix;
    using base::swap;

    using base::copy;
    // 戻り値は普通のstring_viewを返すので問題ない
    using base::substr;
    using base::compare;
    using base::starts_with;
    using base::ends_with;

#ifdef __cpp_lib_string_contains
    // since C++23
    using base::contains;
#endif

    using base::find;
    using base::rfind;
    using base::find_first_of;
    using base::find_last_of;
    using base::find_first_not_of;
    using base::find_last_not_of;

    using base::npos;
  };

  using nt_string_view = basic_null_terminated_string_view<char>;
  using nt_wstring_view = basic_null_terminated_string_view<wchar_t>;
  using nt_u8string_view = basic_null_terminated_string_view<char8_t>;
  using nt_u16string_view = basic_null_terminated_string_view<char16_t>;
  using nt_u32string_view = basic_null_terminated_string_view<char32_t>;

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

template <typename CharT>
inline constexpr bool std::ranges::enable_borrowed_range<ntsv::basic_null_terminated_string_view<CharT>> = true;

template <typename CharT>
inline constexpr bool std::ranges::enable_view<ntsv::basic_null_terminated_string_view<CharT>> = true;

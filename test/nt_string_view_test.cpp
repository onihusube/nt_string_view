
#include "nt_string_view.hpp"

#include <ranges>

#define BOOST_UT_DISABLE_MODULE
#include <boost/ut.hpp>

namespace ut = boost::ut;

int main() {

  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  using ntsv::nt_string_view;
  using namespace ntsv::literals;

  "null_terminated_string_view"_test = [] {

    // ほぼstring_viewの性質を受け継ぐ
    static_assert(std::is_trivially_copyable_v<nt_string_view>);
    static_assert(sizeof(nt_string_view) == sizeof(std::string_view));
    // 右辺値から構築できない
    static_assert(not std::constructible_from<nt_string_view, std::string>);
    static_assert(std::constructible_from<nt_string_view, std::string&>);

    static_assert(std::three_way_comparable<nt_string_view>);
    static_assert(std::equality_comparable<nt_string_view>);

    nt_string_view str1{"test string"};
    ut::expect(str1.str_view().size() == 11);
    ut::expect(str1.str_view() == "test string"sv);

    nt_string_view str2{"test string"sv};
    ut::expect(str2.str_view().size() == 11);
    ut::expect(str2.str_view() == "test string"sv);

    const auto dyn_str = "test string"s;
    nt_string_view str3{dyn_str};
    ut::expect(str3.str_view().size() == 11);
    ut::expect(str3.str_view() == "test string"sv);

    ut::expect(str1 == "test string"_ntsv);
  };

  "string_view innner types"_test = [] {

    static_assert(std::same_as<nt_string_view::const_iterator, std::string_view::const_iterator>);
    static_assert(std::same_as<nt_string_view::const_pointer, std::string_view::const_pointer>);
    static_assert(std::same_as<nt_string_view::const_reference, std::string_view::const_reference>);
    static_assert(std::same_as<nt_string_view::const_reverse_iterator, std::string_view::const_reverse_iterator>);
    static_assert(std::same_as<nt_string_view::difference_type, std::string_view::difference_type>);
    static_assert(std::same_as<nt_string_view::iterator, std::string_view::iterator>);
    static_assert(std::same_as<nt_string_view::pointer, std::string_view::pointer>);
    static_assert(std::same_as<nt_string_view::reference, std::string_view::reference>);
    static_assert(std::same_as<nt_string_view::reverse_iterator, std::string_view::reverse_iterator>);
    static_assert(std::same_as<nt_string_view::size_type, std::string_view::size_type>);
    static_assert(std::same_as<nt_string_view::traits_type, std::string_view::traits_type>);
    static_assert(std::same_as<nt_string_view::value_type, std::string_view::value_type>);

  };

  "range-ness"_test = [] {
    static_assert(std::ranges::range<nt_string_view>);
    static_assert(std::ranges::range<const nt_string_view>);
    static_assert(std::ranges::contiguous_range<nt_string_view>);
    static_assert(std::ranges::contiguous_range<const nt_string_view>);
    static_assert(std::ranges::sized_range<nt_string_view>);
    static_assert(std::ranges::sized_range<const nt_string_view>);
    static_assert(std::ranges::borrowed_range<nt_string_view>);
    static_assert(std::ranges::borrowed_range<const nt_string_view>);
    static_assert(std::ranges::view<nt_string_view>);
    static_assert(std::ranges::viewable_range<nt_string_view>);
    static_assert(std::ranges::viewable_range<const nt_string_view>);
  };
}
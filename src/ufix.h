#include <cassert>
#include <generator>
#include <map>
#include <print>
#include <ranges>
#include <string>
#include <string_view>

namespace ufix {

    constexpr inline char SEP   = '=';
    constexpr inline char DELIM = '|';

} // namespace ufix

namespace ufix::view {

    struct FieldViewIterator {
        using difference_type = std::ptrdiff_t;
        using value_type      = std::string_view;

      private:
        std::size_t      cursor{0};
        std::string_view view;

      public:
        FieldViewIterator() = default;
        FieldViewIterator(std::size_t pos, std::string_view msg) : cursor(pos), view(msg) {}

        FieldViewIterator& operator++(int) { return ++*this; }

        FieldViewIterator& operator++() {
            cursor = view.find(DELIM, cursor);
            if (cursor != std::string::npos) {
                cursor += 1;
            }
            return *this;
        }

        std::string_view operator*() const {
            if (cursor == std::string::npos)
                return {};
            auto next = view.find(ufix::DELIM, cursor);
            if (next == std::string::npos) {
                return std::string_view(view.begin() + cursor, view.end());
            }
            return std::string_view(view.begin() + cursor, view.begin() + next);
        }

        bool operator==(const FieldViewIterator& other) const { return other.view.begin() == view.begin() && other.cursor == cursor; }
    };

    struct FieldView {
        std::string_view tag{};
        std::string_view value{};

        FieldView(std::string_view tag, std::string_view value) : tag(tag), value(value) {}
        FieldView(std::string& tag, std::string& value) : tag(tag), value(value) {}
        FieldView(std::string_view& sv) : FieldView(FieldViewIterator(0, sv)) {}

        FieldView(FieldViewIterator it) {
            auto view  = *it;
            auto parts = view | std::views::split(ufix::SEP);
            auto pit   = parts.begin();
            if (pit == parts.end())
                return;
            tag = std::string_view{*pit};
            if (++pit == parts.end())
                return;
            value = std::string_view{*pit};
        }
    };

    struct FieldViewMap {
        using fv_map_t = std::map<std::string_view, std::string_view>;

      private:
        fv_map_t tv_map;

      public:
        FieldViewMap(FieldViewIterator begin, FieldViewIterator end) {
            for (auto it = begin; it != end; ++it) {
                auto fv = FieldView{it};
                tv_map.insert({fv.tag, fv.value});
            }
        }

        const fv_map_t& kvs() const { return tv_map; }
    };

    struct MessageView;
} // namespace ufix::view

template <> struct std::formatter<ufix::view::MessageView>;

namespace ufix::view {
    struct MessageView {
        using sv = std::string_view;

      private:
        sv buffer;

      public:
        MessageView(std::string& str) : buffer(str) {
            if (buffer.ends_with(ufix::DELIM))
                buffer.remove_suffix(1);
        }

        FieldViewIterator begin() const { return FieldViewIterator{0, buffer}; }
        FieldViewIterator end() const { return FieldViewIterator{std::string::npos, buffer}; }
        FieldViewMap      as_map() const { return FieldViewMap{begin(), end()}; }

        std::generator<FieldView> as_generator() {
            auto it = begin();
            while (*it != "") {
                co_yield FieldView{it};
                ++it;
            }
        };

        sv pop() {
            auto it   = buffer.begin();
            auto next = buffer.find(ufix::DELIM);
            auto end  = (next != std::string::npos) ? it + next + 1 : buffer.end();
            buffer    = sv{end, buffer.end()};
            return (next != std::string::npos) ? sv{it, end - 1} : sv{it, end};
        }

        friend struct std::formatter<ufix::view::MessageView>;
    };

} // namespace ufix::view

template <> struct std::formatter<ufix::view::FieldView> {
    constexpr auto parse(auto& ctx) { return ctx.begin(); }
    auto           format(const auto& fv, auto& ctx) const { return std::format_to(ctx.out(), "{}={}", fv.tag, fv.value); }
};

template <> struct std::formatter<ufix::view::MessageView> {
    constexpr auto parse(auto& ctx) { return ctx.begin(); }
    auto           format(const auto& msg, auto& ctx) const { return std::format_to(ctx.out(), "{}", msg.buffer); }
};

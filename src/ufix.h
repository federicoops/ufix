#include <cassert>
#include <map>
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
        FieldView(FieldViewIterator& it) {
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
      private:
        std::map<std::string_view, std::string_view> tv_map;

      public:
        FieldViewMap(FieldViewIterator begin, FieldViewIterator end) {
            for (auto it = begin; it != end; ++it) {
                auto fv = FieldView{it};
                tv_map.insert({fv.tag, fv.value});
            }
        }

        auto kvs() { return tv_map; }
    };

    struct MessageView {
      private:
        std::string_view buffer;

      public:
        MessageView(std::string& str) : buffer(str) {
            if (buffer.ends_with(ufix::DELIM))
                buffer.remove_suffix(1);
        }

        FieldViewIterator begin() const { return FieldViewIterator{0, buffer}; }
        FieldViewIterator end() const { return FieldViewIterator{std::string::npos, buffer}; }
        FieldViewMap      as_map() const { return FieldViewMap{begin(), end()}; }
    };

} // namespace ufix::view
